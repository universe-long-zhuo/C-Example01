++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/stadir.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
27500 /* This file contains the code for performing four system calls relating to
27501 * status and directories.
27502 *
27503 * The entry points into this file are
27504 * do_chdir: perform the CHDIR system call
27505 * do_chroot: perform the CHROOT system call
27506 * do_stat: perform the STAT system call
27507 * do_fstat: perform the FSTAT system call
27508 * do_fstatfs: perform the FSTATFS system call
27509 */
27510
27511 #include "fs.h"
27512 #include <sys/stat.h>
27513 #include <sys/statfs.h>
27514 #include <minix/com.h>
27515 #include "file.h"
27516 #include "fproc.h"
27517 #include "inode.h"
27518 #include "param.h"
27519 #include "super.h"
27520
27521 FORWARD _PROTOTYPE( int change, (struct inode **iip, char *name_ptr, int len));
27522 FORWARD _PROTOTYPE( int change_into, (struct inode **iip, struct inode *ip));
27523 FORWARD _PROTOTYPE( int stat_inode, (struct inode *rip, struct filp *fil_ptr,
27524 char *user_addr) );
27525
27526 /*===========================================================================*
27527 * do_fchdir *
27528 *===========================================================================*/
27529 PUBLIC int do_fchdir()
27530 {
27531 /* Change directory on already-opened fd. */
27532 struct filp *rfilp;
27533
27534 /* Is the file descriptor valid? */
1012 File: servers/fs/stadir.c MINIX SOURCE CODE
27535 if ( (rfilp = get_filp(m_in.fd)) == NIL_FILP) return(err_code);
27536 return change_into(&fp->fp_workdir, rfilp->filp_ino);
27537 }
27539 /*===========================================================================*
27540 * do_chdir *
27541 *===========================================================================*/
27542 PUBLIC int do_chdir()
27543 {
27544 /* Change directory. This function is also called by MM to simulate a chdir
27545 * in order to do EXEC, etc. It also changes the root directory, the uids and
27546 * gids, and the umask.
27547 */
27548
27549 int r;
27550 register struct fproc *rfp;
27551
27552 if (who == PM_PROC_NR) {
27553 rfp = &fproc[m_in.slot1];
27554 put_inode(fp->fp_rootdir);
27555 dup_inode(fp->fp_rootdir = rfp->fp_rootdir);
27556 put_inode(fp->fp_workdir);
27557 dup_inode(fp->fp_workdir = rfp->fp_workdir);
27558
27559 /* MM uses access() to check permissions. To make this work, pretend
27560 * that the user’s real ids are the same as the user’s effective ids.
27561 * FS calls other than access() do not use the real ids, so are not
27562 * affected.
27563 */
27564 fp->fp_realuid =
27565 fp->fp_effuid = rfp->fp_effuid;
27566 fp->fp_realgid =
27567 fp->fp_effgid = rfp->fp_effgid;
27568 fp->fp_umask = rfp->fp_umask;
27569 return(OK);
27570 }
27571
27572 /* Perform the chdir(name) system call. */
27573 r = change(&fp->fp_workdir, m_in.name, m_in.name_length);
27574 return(r);
27575 }
27577 /*===========================================================================*
27578 * do_chroot *
27579 *===========================================================================*/
27580 PUBLIC int do_chroot()
27581 {
27582 /* Perform the chroot(name) system call. */
27583
27584 register int r;
27585
27586 if (!super_user) return(EPERM); /* only su may chroot() */
27587 r = change(&fp->fp_rootdir, m_in.name, m_in.name_length);
27588 return(r);
27589 }
MINIX SOURCE CODE File: servers/fs/stadir.c 1013
27591 /*===========================================================================*
27592 * change *
27593 *===========================================================================*/
27594 PRIVATE int change(iip, name_ptr, len)
27595 struct inode **iip; /* pointer to the inode pointer for the dir */
27596 char *name_ptr; /* pointer to the directory name to change to */
27597 int len; /* length of the directory name string */
27598 {
27599 /* Do the actual work for chdir() and chroot(). */
27600 struct inode *rip;
27601
27602 /* Try to open the new directory. */
27603 if (fetch_name(name_ptr, len, M3) != OK) return(err_code);
27604 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
27605 return change_into(iip, rip);
27606 }
27608 /*===========================================================================*
27609 * change_into *
27610 *===========================================================================*/
27611 PRIVATE int change_into(iip, rip)
27612 struct inode **iip; /* pointer to the inode pointer for the dir */
27613 struct inode *rip; /* this is what the inode has to become */
27614 {
27615 register int r;
27616
27617 /* It must be a directory and also be searchable. */
27618 if ( (rip->i_mode & I_TYPE) != I_DIRECTORY)
27619 r = ENOTDIR;
27620 else
27621 r = forbidden(rip, X_BIT); /* check if dir is searchable */
27622
27623 /* If error, return inode. */
27624 if (r != OK) {
27625 put_inode(rip);
27626 return(r);
27627 }
27628
27629 /* Everything is OK. Make the change. */
27630 put_inode(*iip); /* release the old directory */
27631 *iip = rip; /* acquire the new one */
27632 return(OK);
27633 }
27635 /*===========================================================================*
27636 * do_stat *
27637 *===========================================================================*/
27638 PUBLIC int do_stat()
27639 {
27640 /* Perform the stat(name, buf) system call. */
27641
27642 register struct inode *rip;
27643 register int r;
27644
27645 /* Both stat() and fstat() use the same routine to do the real work. That
27646 * routine expects an inode, so acquire it temporarily.
27647 */
27648 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
27649 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
27650 r = stat_inode(rip, NIL_FILP, m_in.name2); /* actually do the work.*/
1014 File: servers/fs/stadir.c MINIX SOURCE CODE
27651 put_inode(rip); /* release the inode */
27652 return(r);
27653 }
27655 /*===========================================================================*
27656 * do_fstat *
27657 *===========================================================================*/
27658 PUBLIC int do_fstat()
27659 {
27660 /* Perform the fstat(fd, buf) system call. */
27661
27662 register struct filp *rfilp;
27663
27664 /* Is the file descriptor valid? */
27665 if ( (rfilp = get_filp(m_in.fd)) == NIL_FILP) return(err_code);
27666
27667 return(stat_inode(rfilp->filp_ino, rfilp, m_in.buffer));
27668 }
27670 /*===========================================================================*
27671 * stat_inode *
27672 *===========================================================================*/
27673 PRIVATE int stat_inode(rip, fil_ptr, user_addr)
27674 register struct inode *rip; /* pointer to inode to stat */
27675 struct filp *fil_ptr; /* filp pointer, supplied by ’fstat’ */
27676 char *user_addr; /* user space address where stat buf goes */
27677 {
27678 /* Common code for stat and fstat system calls. */
27679
27680 struct stat statbuf;
27681 mode_t mo;
27682 int r, s;
27683
27684 /* Update the atime, ctime, and mtime fields in the inode, if need be. */
27685 if (rip->i_update) update_times(rip);
27686
27687 /* Fill in the statbuf struct. */
27688 mo = rip->i_mode & I_TYPE;
27689
27690 /* true iff special */
27691 s = (mo == I_CHAR_SPECIAL || mo == I_BLOCK_SPECIAL);
27692
27693 statbuf.st_dev = rip->i_dev;
27694 statbuf.st_ino = rip->i_num;
27695 statbuf.st_mode = rip->i_mode;
27696 statbuf.st_nlink = rip->i_nlinks;
27697 statbuf.st_uid = rip->i_uid;
27698 statbuf.st_gid = rip->i_gid;
27699 statbuf.st_rdev = (dev_t) (s ? rip->i_zone[0] : NO_DEV);
27700 statbuf.st_size = rip->i_size;
27701
27702 if (rip->i_pipe == I_PIPE) {
27703 statbuf.st_mode &= ˜I_REGULAR; /* wipe out I_REGULAR bit for pipes */
27704 if (fil_ptr != NIL_FILP && fil_ptr->filp_mode & R_BIT)
27705 statbuf.st_size -= fil_ptr->filp_pos;
27706 }
27707
27708 statbuf.st_atime = rip->i_atime;
27709 statbuf.st_mtime = rip->i_mtime;
27710 statbuf.st_ctime = rip->i_ctime;
MINIX SOURCE CODE File: servers/fs/stadir.c 1015
27711
27712 /* Copy the struct to user space. */
27713 r = sys_datacopy(FS_PROC_NR, (vir_bytes) &statbuf,
27714 who, (vir_bytes) user_addr, (phys_bytes) sizeof(statbuf));
27715 return(r);
27716 }
27718 /*===========================================================================*
27719 * do_fstatfs *
27720 *===========================================================================*/
27721 PUBLIC int do_fstatfs()
27722 {
27723 /* Perform the fstatfs(fd, buf) system call. */
27724 struct statfs st;
27725 register struct filp *rfilp;
27726 int r;
27727
27728 /* Is the file descriptor valid? */
27729 if ( (rfilp = get_filp(m_in.fd)) == NIL_FILP) return(err_code);
27730
27731 st.f_bsize = rfilp->filp_ino->i_sp->s_block_size;
27732
27733 r = sys_datacopy(FS_PROC_NR, (vir_bytes) &st,
27734 who, (vir_bytes) m_in.buffer, (phys_bytes) sizeof(st));
27735
27736 return(r);
27737 }