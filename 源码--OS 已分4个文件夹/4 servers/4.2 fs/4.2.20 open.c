++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/open.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
24500 /* This file contains the procedures for creating, opening, closing, and
24501 * seeking on files.
24502 *
24503 * The entry points into this file are
24504 * do_creat: perform the CREAT system call
24505 * do_open: perform the OPEN system call
24506 * do_mknod: perform the MKNOD system call
24507 * do_mkdir: perform the MKDIR system call
24508 * do_close: perform the CLOSE system call
24509 * do_lseek: perform the LSEEK system call
24510 */
24511
24512 #include "fs.h"
24513 #include <sys/stat.h>
24514 #include <fcntl.h>
24515 #include <minix/callnr.h>
24516 #include <minix/com.h>
24517 #include "buf.h"
24518 #include "file.h"
24519 #include "fproc.h"
24520 #include "inode.h"
24521 #include "lock.h"
24522 #include "param.h"
24523 #include "super.h"
24524
24525 #define offset m2_l1
24526
24527 PRIVATE char mode_map[] = {R_BIT, W_BIT, R_BIT|W_BIT, 0};
24528
24529 FORWARD _PROTOTYPE( int common_open, (int oflags, mode_t omode) );
24530 FORWARD _PROTOTYPE( int pipe_open, (struct inode *rip,mode_t bits,int oflags));
24531 FORWARD _PROTOTYPE( struct inode *new_node, (char *path, mode_t bits,
24532 zone_t z0) );
24533
24534 /*===========================================================================*
24535 * do_creat *
24536 *===========================================================================*/
24537 PUBLIC int do_creat()
24538 {
24539 /* Perform the creat(name, mode) system call. */
24540 int r;
24541
24542 if (fetch_name(m_in.name, m_in.name_length, M3) != OK) return(err_code);
24543 r = common_open(O_WRONLY | O_CREAT | O_TRUNC, (mode_t) m_in.mode);
24544 return(r);
24545 }
966 File: servers/fs/open.c MINIX SOURCE CODE
24547 /*===========================================================================*
24548 * do_open *
24549 *===========================================================================*/
24550 PUBLIC int do_open()
24551 {
24552 /* Perform the open(name, flags,...) system call. */
24553
24554 int create_mode = 0; /* is really mode_t but this gives problems */
24555 int r;
24556
24557 /* If O_CREAT is set, open has three parameters, otherwise two. */
24558 if (m_in.mode & O_CREAT) {
24559 create_mode = m_in.c_mode;
24560 r = fetch_name(m_in.c_name, m_in.name1_length, M1);
24561 } else {
24562 r = fetch_name(m_in.name, m_in.name_length, M3);
24563 }
24564
24565 if (r != OK) return(err_code); /* name was bad */
24566 r = common_open(m_in.mode, create_mode);
24567 return(r);
24568 }
24570 /*===========================================================================*
24571 * common_open *
24572 *===========================================================================*/
24573 PRIVATE int common_open(register int oflags, mode_t omode)
24574 {
24575 /* Common code from do_creat and do_open. */
24576
24577 register struct inode *rip;
24578 int r, b, exist = TRUE;
24579 dev_t dev;
24580 mode_t bits;
24581 off_t pos;
24582 struct filp *fil_ptr, *filp2;
24583
24584 /* Remap the bottom two bits of oflags. */
24585 bits = (mode_t) mode_map[oflags & O_ACCMODE];
24586
24587 /* See if file descriptor and filp slots are available. */
24588 if ( (r = get_fd(0, bits, &m_in.fd, &fil_ptr)) != OK) return(r);
24589
24590 /* If O_CREATE is set, try to make the file. */
24591 if (oflags & O_CREAT) {
24592 /* Create a new inode by calling new_node(). */
24593 omode = I_REGULAR | (omode & ALL_MODES & fp->fp_umask);
24594 rip = new_node(user_path, omode, NO_ZONE);
24595 r = err_code;
24596 if (r == OK) exist = FALSE; /* we just created the file */
24597 else if (r != EEXIST) return(r); /* other error */
24598 else exist = !(oflags & O_EXCL); /* file exists, if the O_EXCL
24599 flag is set this is an error */
24600 } else {
24601 /* Scan path name. */
24602 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
24603 }
24604
24605 /* Claim the file descriptor and filp slot and fill them in. */
24606 fp->fp_filp[m_in.fd] = fil_ptr;
MINIX SOURCE CODE File: servers/fs/open.c 967
24607 fil_ptr->filp_count = 1;
24608 fil_ptr->filp_ino = rip;
24609 fil_ptr->filp_flags = oflags;
24610
24611 /* Only do the normal open code if we didn’t just create the file. */
24612 if (exist) {
24613 /* Check protections. */
24614 if ((r = forbidden(rip, bits)) == OK) {
24615 /* Opening reg. files directories and special files differ. */
24616 switch (rip->i_mode & I_TYPE) {
24617 case I_REGULAR:
24618 /* Truncate regular file if O_TRUNC. */
24619 if (oflags & O_TRUNC) {
24620 if ((r = forbidden(rip, W_BIT)) !=OK) break;
24621 truncate(rip);
24622 wipe_inode(rip);
24623 /* Send the inode from the inode cache to the
24624 * block cache, so it gets written on the next
24625 * cache flush.
24626 */
24627 rw_inode(rip, WRITING);
24628 }
24629 break;
24630
24631 case I_DIRECTORY:
24632 /* Directories may be read but not written. */
24633 r = (bits & W_BIT ? EISDIR : OK);
24634 break;
24635
24636 case I_CHAR_SPECIAL:
24637 case I_BLOCK_SPECIAL:
24638 /* Invoke the driver for special processing. */
24639 dev = (dev_t) rip->i_zone[0];
24640 r = dev_open(dev, who, bits | (oflags & ˜O_ACCMODE));
24641 break;
24642
24643 case I_NAMED_PIPE:
24644 oflags |= O_APPEND; /* force append mode */
24645 fil_ptr->filp_flags = oflags;
24646 r = pipe_open(rip, bits, oflags);
24647 if (r != ENXIO) {
24648 /* See if someone else is doing a rd or wt on
24649 * the FIFO. If so, use its filp entry so the
24650 * file position will be automatically shared.
24651 */
24652 b = (bits & R_BIT ? R_BIT : W_BIT);
24653 fil_ptr->filp_count = 0; /* don’t find self */
24654 if ((filp2 = find_filp(rip, b)) != NIL_FILP) {
24655 /* Co-reader or writer found. Use it.*/
24656 fp->fp_filp[m_in.fd] = filp2;
24657 filp2->filp_count++;
24658 filp2->filp_ino = rip;
24659 filp2->filp_flags = oflags;
24660
24661 /* i_count was incremented incorrectly
24662 * by eatpath above, not knowing that
24663 * we were going to use an existing
24664 * filp entry. Correct this error.
24665 */
24666 rip->i_count--;
968 File: servers/fs/open.c MINIX SOURCE CODE
24667 } else {
24668 /* Nobody else found. Restore filp. */
24669 fil_ptr->filp_count = 1;
24670 if (b == R_BIT)
24671 pos = rip->i_zone[V2_NR_DZONES+0];
24672 else
24673 pos = rip->i_zone[V2_NR_DZONES+1];
24674 fil_ptr->filp_pos = pos;
24675 }
24676 }
24677 break;
24678 }
24679 }
24680 }
24681
24682 /* If error, release inode. */
24683 if (r != OK) {
24684 if (r == SUSPEND) return(r); /* Oops, just suspended */
24685 fp->fp_filp[m_in.fd] = NIL_FILP;
24686 fil_ptr->filp_count= 0;
24687 put_inode(rip);
24688 return(r);
24689 }
24690
24691 return(m_in.fd);
24692 }
24694 /*===========================================================================*
24695 * new_node *
24696 *===========================================================================*/
24697 PRIVATE struct inode *new_node(char *path, mode_t bits, zone_t z0)
24698 {
24699 /* New_node() is called by common_open(), do_mknod(), and do_mkdir().
24700 * In all cases it allocates a new inode, makes a directory entry for it on
24701 * the path ’path’, and initializes it. It returns a pointer to the inode if
24702 * it can do this; otherwise it returns NIL_INODE. It always sets ’err_code’
24703 * to an appropriate value (OK or an error code).
24704 */
24705
24706 register struct inode *rlast_dir_ptr, *rip;
24707 register int r;
24708 char string[NAME_MAX];
24709
24710 /* See if the path can be opened down to the last directory. */
24711 if ((rlast_dir_ptr = last_dir(path, string)) == NIL_INODE) return(NIL_INODE);
24712
24713 /* The final directory is accessible. Get final component of the path. */
24714 rip = advance(rlast_dir_ptr, string);
24715 if ( rip == NIL_INODE && err_code == ENOENT) {
24716 /* Last path component does not exist. Make new directory entry. */
24717 if ( (rip = alloc_inode(rlast_dir_ptr->i_dev, bits)) == NIL_INODE) {
24718 /* Can’t creat new inode: out of inodes. */
24719 put_inode(rlast_dir_ptr);
24720 return(NIL_INODE);
24721 }
24722
24723 /* Force inode to the disk before making directory entry to make
24724 * the system more robust in the face of a crash: an inode with
24725 * no directory entry is much better than the opposite.
24726 */
MINIX SOURCE CODE File: servers/fs/open.c 969
24727 rip->i_nlinks++;
24728 rip->i_zone[0] = z0; /* major/minor device numbers */
24729 rw_inode(rip, WRITING); /* force inode to disk now */
24730
24731 /* New inode acquired. Try to make directory entry. */
24732 if ((r = search_dir(rlast_dir_ptr, string, &rip->i_num,ENTER)) != OK) {
24733 put_inode(rlast_dir_ptr);
24734 rip->i_nlinks--; /* pity, have to free disk inode */
24735 rip->i_dirt = DIRTY; /* dirty inodes are written out */
24736 put_inode(rip); /* this call frees the inode */
24737 err_code = r;
24738 return(NIL_INODE);
24739 }
24740
24741 } else {
24742 /* Either last component exists, or there is some problem. */
24743 if (rip != NIL_INODE)
24744 r = EEXIST;
24745 else
24746 r = err_code;
24747 }
24748
24749 /* Return the directory inode and exit. */
24750 put_inode(rlast_dir_ptr);
24751 err_code = r;
24752 return(rip);
24753 }
24755 /*===========================================================================*
24756 * pipe_open *
24757 *===========================================================================*/
24758 PRIVATE int pipe_open(register struct inode *rip, register mode_t bits,
24759 register int oflags)
24760 {
24761 /* This function is called from common_open. It checks if
24762 * there is at least one reader/writer pair for the pipe, if not
24763 * it suspends the caller, otherwise it revives all other blocked
24764 * processes hanging on the pipe.
24765 */
24766
24767 rip->i_pipe = I_PIPE;
24768 if (find_filp(rip, bits & W_BIT ? R_BIT : W_BIT) == NIL_FILP) {
24769 if (oflags & O_NONBLOCK) {
24770 if (bits & W_BIT) return(ENXIO);
24771 } else {
24772 suspend(XPOPEN); /* suspend caller */
24773 return(SUSPEND);
24774 }
24775 } else if (susp_count > 0) {/* revive blocked processes */
24776 release(rip, OPEN, susp_count);
24777 release(rip, CREAT, susp_count);
24778 }
24779 return(OK);
24780 }
24782 /*===========================================================================*
24783 * do_mknod *
24784 *===========================================================================*/
24785 PUBLIC int do_mknod()
24786 {
970 File: servers/fs/open.c MINIX SOURCE CODE
24787 /* Perform the mknod(name, mode, addr) system call. */
24788
24789 register mode_t bits, mode_bits;
24790 struct inode *ip;
24791
24792 /* Only the super_user may make nodes other than fifos. */
24793 mode_bits = (mode_t) m_in.mk_mode; /* mode of the inode */
24794 if (!super_user && ((mode_bits & I_TYPE) != I_NAMED_PIPE)) return(EPERM);
24795 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
24796 bits = (mode_bits & I_TYPE) | (mode_bits & ALL_MODES & fp->fp_umask);
24797 ip = new_node(user_path, bits, (zone_t) m_in.mk_z0);
24798 put_inode(ip);
24799 return(err_code);
24800 }
24802 /*===========================================================================*
24803 * do_mkdir *
24804 *===========================================================================*/
24805 PUBLIC int do_mkdir()
24806 {
24807 /* Perform the mkdir(name, mode) system call. */
24808
24809 int r1, r2; /* status codes */
24810 ino_t dot, dotdot; /* inode numbers for . and .. */
24811 mode_t bits; /* mode bits for the new inode */
24812 char string[NAME_MAX]; /* last component of the new dir’s path name */
24813 register struct inode *rip, *ldirp;
24814
24815 /* Check to see if it is possible to make another link in the parent dir. */
24816 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
24817 ldirp = last_dir(user_path, string); /* pointer to new dir’s parent */
24818 if (ldirp == NIL_INODE) return(err_code);
24819 if (ldirp->i_nlinks >= (ldirp->i_sp->s_version == V1 ?
24820 CHAR_MAX : SHRT_MAX)) {
24821 put_inode(ldirp); /* return parent */
24822 return(EMLINK);
24823 }
24824
24825 /* Next make the inode. If that fails, return error code. */
24826 bits = I_DIRECTORY | (m_in.mode & RWX_MODES & fp->fp_umask);
24827 rip = new_node(user_path, bits, (zone_t) 0);
24828 if (rip == NIL_INODE || err_code == EEXIST) {
24829 put_inode(rip); /* can’t make dir: it already exists */
24830 put_inode(ldirp); /* return parent too */
24831 return(err_code);
24832 }
24833
24834 /* Get the inode numbers for . and .. to enter in the directory. */
24835 dotdot = ldirp->i_num; /* parent’s inode number */
24836 dot = rip->i_num; /* inode number of the new dir itself */
24837
24838 /* Now make dir entries for . and .. unless the disk is completely full. */
24839 /* Use dot1 and dot2, so the mode of the directory isn’t important. */
24840 rip->i_mode = bits; /* set mode */
24841 r1 = search_dir(rip, dot1, &dot, ENTER); /* enter . in the new dir */
24842 r2 = search_dir(rip, dot2, &dotdot, ENTER); /* enter .. in the new dir */
24843
24844 /* If both . and .. were successfully entered, increment the link counts. */
24845 if (r1 == OK && r2 == OK) {
24846 /* Normal case. It was possible to enter . and .. in the new dir. */
MINIX SOURCE CODE File: servers/fs/open.c 971
24847 rip->i_nlinks++; /* this accounts for . */
24848 ldirp->i_nlinks++; /* this accounts for .. */
24849 ldirp->i_dirt = DIRTY; /* mark parent’s inode as dirty */
24850 } else {
24851 /* It was not possible to enter . or .. probably disk was full. */
24852 (void) search_dir(ldirp, string, (ino_t *) 0, DELETE);
24853 rip->i_nlinks--; /* undo the increment done in new_node() */
24854 }
24855 rip->i_dirt = DIRTY; /* either way, i_nlinks has changed */
24856
24857 put_inode(ldirp); /* return the inode of the parent dir */
24858 put_inode(rip); /* return the inode of the newly made dir */
24859 return(err_code); /* new_node() always sets ’err_code’ */
24860 }
24862 /*===========================================================================*
24863 * do_close *
24864 *===========================================================================*/
24865 PUBLIC int do_close()
24866 {
24867 /* Perform the close(fd) system call. */
24868
24869 register struct filp *rfilp;
24870 register struct inode *rip;
24871 struct file_lock *flp;
24872 int rw, mode_word, lock_count;
24873 dev_t dev;
24874
24875 /* First locate the inode that belongs to the file descriptor. */
24876 if ( (rfilp = get_filp(m_in.fd)) == NIL_FILP) return(err_code);
24877 rip = rfilp->filp_ino; /* ’rip’ points to the inode */
24878
24879 if (rfilp->filp_count - 1 == 0 && rfilp->filp_mode != FILP_CLOSED) {
24880 /* Check to see if the file is special. */
24881 mode_word = rip->i_mode & I_TYPE;
24882 if (mode_word == I_CHAR_SPECIAL || mode_word == I_BLOCK_SPECIAL) {
24883 dev = (dev_t) rip->i_zone[0];
24884 if (mode_word == I_BLOCK_SPECIAL) {
24885 /* Invalidate cache entries unless special is mounted
24886 * or ROOT
24887 */
24888 if (!mounted(rip)) {
24889 (void) do_sync(); /* purge cache */
24890 invalidate(dev);
24891 }
24892 }
24893 /* Do any special processing on device close. */
24894 dev_close(dev);
24895 }
24896 }
24897
24898 /* If the inode being closed is a pipe, release everyone hanging on it. */
24899 if (rip->i_pipe == I_PIPE) {
24900 rw = (rfilp->filp_mode & R_BIT ? WRITE : READ);
24901 release(rip, rw, NR_PROCS);
24902 }
24903
24904 /* If a write has been done, the inode is already marked as DIRTY. */
24905 if (--rfilp->filp_count == 0) {
24906 if (rip->i_pipe == I_PIPE && rip->i_count > 1) {
972 File: servers/fs/open.c MINIX SOURCE CODE
24907 /* Save the file position in the i-node in case needed later.
24908 * The read and write positions are saved separately. The
24909 * last 3 zones in the i-node are not used for (named) pipes.
24910 */
24911 if (rfilp->filp_mode == R_BIT)
24912 rip->i_zone[V2_NR_DZONES+0] = (zone_t) rfilp->filp_pos;
24913 else
24914 rip->i_zone[V2_NR_DZONES+1] = (zone_t) rfilp->filp_pos;
24915 }
24916 put_inode(rip);
24917 }
24918
24919 fp->fp_cloexec &= ˜(1L << m_in.fd); /* turn off close-on-exec bit */
24920 fp->fp_filp[m_in.fd] = NIL_FILP;
24921
24922 /* Check to see if the file is locked. If so, release all locks. */
24923 if (nr_locks == 0) return(OK);
24924 lock_count = nr_locks; /* save count of locks */
24925 for (flp = &file_lock[0]; flp < &file_lock[NR_LOCKS]; flp++) {
24926 if (flp->lock_type == 0) continue; /* slot not in use */
24927 if (flp->lock_inode == rip && flp->lock_pid == fp->fp_pid) {
24928 flp->lock_type = 0;
24929 nr_locks--;
24930 }
24931 }
24932 if (nr_locks < lock_count) lock_revive(); /* lock released */
24933 return(OK);
24934 }
24936 /*===========================================================================*
24937 * do_lseek *
24938 *===========================================================================*/
24939 PUBLIC int do_lseek()
24940 {
24941 /* Perform the lseek(ls_fd, offset, whence) system call. */
24942
24943 register struct filp *rfilp;
24944 register off_t pos;
24945
24946 /* Check to see if the file descriptor is valid. */
24947 if ( (rfilp = get_filp(m_in.ls_fd)) == NIL_FILP) return(err_code);
24948
24949 /* No lseek on pipes. */
24950 if (rfilp->filp_ino->i_pipe == I_PIPE) return(ESPIPE);
24951
24952 /* The value of ’whence’ determines the start position to use. */
24953 switch(m_in.whence) {
24954 case 0: pos = 0; break;
24955 case 1: pos = rfilp->filp_pos; break;
24956 case 2: pos = rfilp->filp_ino->i_size; break;
24957 default: return(EINVAL);
24958 }
24959
24960 /* Check for overflow. */
24961 if (((long)m_in.offset > 0) && ((long)(pos + m_in.offset) < (long)pos))
24962 return(EINVAL);
24963 if (((long)m_in.offset < 0) && ((long)(pos + m_in.offset) > (long)pos))
24964 return(EINVAL);
24965 pos = pos + m_in.offset;
24966
MINIX SOURCE CODE File: servers/fs/open.c 973
24967 if (pos != rfilp->filp_pos)
24968 rfilp->filp_ino->i_seek = ISEEK; /* inhibit read ahead */
24969 rfilp->filp_pos = pos;
24970 m_out.reply_l1 = pos; /* insert the long into the output message */
24971 return(OK);
24972 }