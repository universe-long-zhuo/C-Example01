++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/protect.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
27800 /* This file deals with protection in the file system. It contains the code
27801 * for four system calls that relate to protection.
27802 *
27803 * The entry points into this file are
27804 * do_chmod: perform the CHMOD system call
27805 * do_chown: perform the CHOWN system call
27806 * do_umask: perform the UMASK system call
27807 * do_access: perform the ACCESS system call
27808 * forbidden: check to see if a given access is allowed on a given inode
27809 */
27810
27811 #include "fs.h"
27812 #include <unistd.h>
27813 #include <minix/callnr.h>
27814 #include "buf.h"
27815 #include "file.h"
27816 #include "fproc.h"
27817 #include "inode.h"
27818 #include "param.h"
27819 #include "super.h"
27820
1016 File: servers/fs/protect.c MINIX SOURCE CODE
27821 /*===========================================================================*
27822 * do_chmod *
27823 *===========================================================================*/
27824 PUBLIC int do_chmod()
27825 {
27826 /* Perform the chmod(name, mode) system call. */
27827
27828 register struct inode *rip;
27829 register int r;
27830
27831 /* Temporarily open the file. */
27832 if (fetch_name(m_in.name, m_in.name_length, M3) != OK) return(err_code);
27833 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
27834
27835 /* Only the owner or the super_user may change the mode of a file.
27836 * No one may change the mode of a file on a read-only file system.
27837 */
27838 if (rip->i_uid != fp->fp_effuid && !super_user)
27839 r = EPERM;
27840 else
27841 r = read_only(rip);
27842
27843 /* If error, return inode. */
27844 if (r != OK) {
27845 put_inode(rip);
27846 return(r);
27847 }
27848
27849 /* Now make the change. Clear setgid bit if file is not in caller’s grp */
27850 rip->i_mode = (rip->i_mode & ˜ALL_MODES) | (m_in.mode & ALL_MODES);
27851 if (!super_user && rip->i_gid != fp->fp_effgid)rip->i_mode &= ˜I_SET_GID_BIT;
27852 rip->i_update |= CTIME;
27853 rip->i_dirt = DIRTY;
27854
27855 put_inode(rip);
27856 return(OK);
27857 }
27859 /*===========================================================================*
27860 * do_chown *
27861 *===========================================================================*/
27862 PUBLIC int do_chown()
27863 {
27864 /* Perform the chown(name, owner, group) system call. */
27865
27866 register struct inode *rip;
27867 register int r;
27868
27869 /* Temporarily open the file. */
27870 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
27871 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
27872
27873 /* Not permitted to change the owner of a file on a read-only file sys. */
27874 r = read_only(rip);
27875 if (r == OK) {
27876 /* FS is R/W. Whether call is allowed depends on ownership, etc. */
27877 if (super_user) {
27878 /* The super user can do anything. */
27879 rip->i_uid = m_in.owner; /* others later */
27880 } else {
MINIX SOURCE CODE File: servers/fs/protect.c 1017
27881 /* Regular users can only change groups of their own files. */
27882 if (rip->i_uid != fp->fp_effuid) r = EPERM;
27883 if (rip->i_uid != m_in.owner) r = EPERM; /* no giving away */
27884 if (fp->fp_effgid != m_in.group) r = EPERM;
27885 }
27886 }
27887 if (r == OK) {
27888 rip->i_gid = m_in.group;
27889 rip->i_mode &= ˜(I_SET_UID_BIT | I_SET_GID_BIT);
27890 rip->i_update |= CTIME;
27891 rip->i_dirt = DIRTY;
27892 }
27893
27894 put_inode(rip);
27895 return(r);
27896 }
27898 /*===========================================================================*
27899 * do_umask *
27900 *===========================================================================*/
27901 PUBLIC int do_umask()
27902 {
27903 /* Perform the umask(co_mode) system call. */
27904 register mode_t r;
27905
27906 r = ˜fp->fp_umask; /* set ’r’ to complement of old mask */
27907 fp->fp_umask = ˜(m_in.co_mode & RWX_MODES);
27908 return(r); /* return complement of old mask */
27909 }
27911 /*===========================================================================*
27912 * do_access *
27913 *===========================================================================*/
27914 PUBLIC int do_access()
27915 {
27916 /* Perform the access(name, mode) system call. */
27917
27918 struct inode *rip;
27919 register int r;
27920
27921 /* First check to see if the mode is correct. */
27922 if ( (m_in.mode & ˜(R_OK | W_OK | X_OK)) != 0 && m_in.mode != F_OK)
27923 return(EINVAL);
27924
27925 /* Temporarily open the file whose access is to be checked. */
27926 if (fetch_name(m_in.name, m_in.name_length, M3) != OK) return(err_code);
27927 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
27928
27929 /* Now check the permissions. */
27930 r = forbidden(rip, (mode_t) m_in.mode);
27931 put_inode(rip);
27932 return(r);
27933 }
27935 /*===========================================================================*
27936 * forbidden *
27937 *===========================================================================*/
27938 PUBLIC int forbidden(register struct inode *rip, mode_t access_desired)
27939 {
27940 /* Given a pointer to an inode, ’rip’, and the access desired, determine
1018 File: servers/fs/protect.c MINIX SOURCE CODE
27941 * if the access is allowed, and if not why not. The routine looks up the
27942 * caller’s uid in the ’fproc’ table. If access is allowed, OK is returned
27943 * if it is forbidden, EACCES is returned.
27944 */
27945
27946 register struct inode *old_rip = rip;
27947 register struct super_block *sp;
27948 register mode_t bits, perm_bits;
27949 int r, shift, test_uid, test_gid, type;
27950
27951 if (rip->i_mount == I_MOUNT) /* The inode is mounted on. */
27952 for (sp = &super_block[1]; sp < &super_block[NR_SUPERS]; sp++)
27953 if (sp->s_imount == rip) {
27954 rip = get_inode(sp->s_dev, ROOT_INODE);
27955 break;
27956 } /* if */
27957
27958 /* Isolate the relevant rwx bits from the mode. */
27959 bits = rip->i_mode;
27960 test_uid = (call_nr == ACCESS ? fp->fp_realuid : fp->fp_effuid);
27961 test_gid = (call_nr == ACCESS ? fp->fp_realgid : fp->fp_effgid);
27962 if (test_uid == SU_UID) {
27963 /* Grant read and write permission. Grant search permission for
27964 * directories. Grant execute permission (for non-directories) if
27965 * and only if one of the ’X’ bits is set.
27966 */
27967 if ( (bits & I_TYPE) == I_DIRECTORY ||
27968 bits & ((X_BIT << 6) | (X_BIT << 3) | X_BIT))
27969 perm_bits = R_BIT | W_BIT | X_BIT;
27970 else
27971 perm_bits = R_BIT | W_BIT;
27972 } else {
27973 if (test_uid == rip->i_uid) shift = 6; /* owner */
27974 else if (test_gid == rip->i_gid ) shift = 3; /* group */
27975 else shift = 0; /* other */
27976 perm_bits = (bits >> shift) & (R_BIT | W_BIT | X_BIT);
27977 }
27978
27979 /* If access desired is not a subset of what is allowed, it is refused. */
27980 r = OK;
27981 if ((perm_bits | access_desired) != perm_bits) r = EACCES;
27982
27983 /* Check to see if someone is trying to write on a file system that is
27984 * mounted read-only.
27985 */
27986 type = rip->i_mode & I_TYPE;
27987 if (r == OK)
27988 if (access_desired & W_BIT)
27989 r = read_only(rip);
27990
27991 if (rip != old_rip) put_inode(rip);
27992
27993 return(r);
27994 }
27996 /*===========================================================================*
27997 * read_only *
27998 *===========================================================================*/
27999 PUBLIC int read_only(ip)
28000 struct inode *ip; /* ptr to inode whose file sys is to be cked */
MINIX SOURCE CODE File: servers/fs/protect.c 1019
28001 {
28002 /* Check to see if the file system on which the inode ’ip’ resides is mounted
28003 * read only. If so, return EROFS, else return OK.
28004 */
28005
28006 register struct super_block *sp;
28007
28008 sp = ip->i_sp;
28009 return(sp->s_rd_only ? EROFS : OK);
28010 }