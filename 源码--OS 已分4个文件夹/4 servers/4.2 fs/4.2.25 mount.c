1000 File: servers/fs/mount.c MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/mount.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
26700 /* This file performs the MOUNT and UMOUNT system calls.
26701 *
26702 * The entry points into this file are
26703 * do_mount: perform the MOUNT system call
26704 * do_umount: perform the UMOUNT system call
26705 */
26706
26707 #include "fs.h"
26708 #include <fcntl.h>
26709 #include <minix/com.h>
26710 #include <sys/stat.h>
26711 #include "buf.h"
26712 #include "file.h"
26713 #include "fproc.h"
26714 #include "inode.h"
26715 #include "param.h"
26716 #include "super.h"
26717
26718 FORWARD _PROTOTYPE( dev_t name_to_dev, (char *path) );
26719
26720 /*===========================================================================*
26721 * do_mount *
26722 *===========================================================================*/
26723 PUBLIC int do_mount()
26724 {
26725 /* Perform the mount(name, mfile, rd_only) system call. */
26726
26727 register struct inode *rip, *root_ip;
26728 struct super_block *xp, *sp;
26729 dev_t dev;
26730 mode_t bits;
26731 int rdir, mdir; /* TRUE iff {root|mount} file is dir */
26732 int r, found;
26733
26734 /* Only the super-user may do MOUNT. */
26735 if (!super_user) return(EPERM);
26736
26737 /* If ’name’ is not for a block special file, return error. */
26738 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
26739 if ( (dev = name_to_dev(user_path)) == NO_DEV) return(err_code);
26740
26741 /* Scan super block table to see if dev already mounted & find a free slot.*/
26742 sp = NIL_SUPER;
26743 found = FALSE;
26744 for (xp = &super_block[0]; xp < &super_block[NR_SUPERS]; xp++) {
26745 if (xp->s_dev == dev) found = TRUE; /* is it mounted already? */
26746 if (xp->s_dev == NO_DEV) sp = xp; /* record free slot */
26747 }
26748 if (found) return(EBUSY); /* already mounted */
26749 if (sp == NIL_SUPER) return(ENFILE); /* no super block available */
26750
26751 /* Open the device the file system lives on. */
26752 if (dev_open(dev, who, m_in.rd_only ? R_BIT : (R_BIT|W_BIT)) != OK)
26753 return(EINVAL);
26754
MINIX SOURCE CODE File: servers/fs/mount.c 1001
26755 /* Make the cache forget about blocks it has open on the filesystem */
26756 (void) do_sync();
26757 invalidate(dev);
26758
26759 /* Fill in the super block. */
26760 sp->s_dev = dev; /* read_super() needs to know which dev */
26761 r = read_super(sp);
26762
26763 /* Is it recognized as a Minix filesystem? */
26764 if (r != OK) {
26765 dev_close(dev);
26766 sp->s_dev = NO_DEV;
26767 return(r);
26768 }
26769
26770 /* Now get the inode of the file to be mounted on. */
26771 if (fetch_name(m_in.name2, m_in.name2_length, M1) != OK) {
26772 dev_close(dev);
26773 sp->s_dev = NO_DEV;
26774 return(err_code);
26775 }
26776 if ( (rip = eat_path(user_path)) == NIL_INODE) {
26777 dev_close(dev);
26778 sp->s_dev = NO_DEV;
26779 return(err_code);
26780 }
26781
26782 /* It may not be busy. */
26783 r = OK;
26784 if (rip->i_count > 1) r = EBUSY;
26785
26786 /* It may not be special. */
26787 bits = rip->i_mode & I_TYPE;
26788 if (bits == I_BLOCK_SPECIAL || bits == I_CHAR_SPECIAL) r = ENOTDIR;
26789
26790 /* Get the root inode of the mounted file system. */
26791 root_ip = NIL_INODE; /* if ’r’ not OK, make sure this is defined */
26792 if (r == OK) {
26793 if ( (root_ip = get_inode(dev, ROOT_INODE)) == NIL_INODE) r = err_code;
26794 }
26795 if (root_ip != NIL_INODE && root_ip->i_mode == 0) {
26796 r = EINVAL;
26797 }
26798
26799 /* File types of ’rip’ and ’root_ip’ may not conflict. */
26800 if (r == OK) {
26801 mdir = ((rip->i_mode & I_TYPE) == I_DIRECTORY); /* TRUE iff dir */
26802 rdir = ((root_ip->i_mode & I_TYPE) == I_DIRECTORY);
26803 if (!mdir && rdir) r = EISDIR;
26804 }
26805
26806 /* If error, return the super block and both inodes; release the maps. */
26807 if (r != OK) {
26808 put_inode(rip);
26809 put_inode(root_ip);
26810 (void) do_sync();
26811 invalidate(dev);
26812 dev_close(dev);
26813 sp->s_dev = NO_DEV;
26814 return(r);
1002 File: servers/fs/mount.c MINIX SOURCE CODE
26815 }
26816
26817 /* Nothing else can go wrong. Perform the mount. */
26818 rip->i_mount = I_MOUNT; /* this bit says the inode is mounted on */
26819 sp->s_imount = rip;
26820 sp->s_isup = root_ip;
26821 sp->s_rd_only = m_in.rd_only;
26822 return(OK);
26823 }
26825 /*===========================================================================*
26826 * do_umount *
26827 *===========================================================================*/
26828 PUBLIC int do_umount()
26829 {
26830 /* Perform the umount(name) system call. */
26831 dev_t dev;
26832
26833 /* Only the super-user may do UMOUNT. */
26834 if (!super_user) return(EPERM);
26835
26836 /* If ’name’ is not for a block special file, return error. */
26837 if (fetch_name(m_in.name, m_in.name_length, M3) != OK) return(err_code);
26838 if ( (dev = name_to_dev(user_path)) == NO_DEV) return(err_code);
26839
26840 return(unmount(dev));
26841 }
26843 /*===========================================================================*
26844 * unmount *
26845 *===========================================================================*/
26846 PUBLIC int unmount(dev)
26847 Dev_t dev;
26848 {
26849 /* Unmount a file system by device number. */
26850 register struct inode *rip;
26851 struct super_block *sp, *sp1;
26852 int count;
26853
26854 /* See if the mounted device is busy. Only 1 inode using it should be
26855 * open -- the root inode -- and that inode only 1 time.
26856 */
26857 count = 0;
26858 for (rip = &inode[0]; rip< &inode[NR_INODES]; rip++)
26859 if (rip->i_count > 0 && rip->i_dev == dev) count += rip->i_count;
26860 if (count > 1) return(EBUSY); /* can’t umount a busy file system */
26861
26862 /* Find the super block. */
26863 sp = NIL_SUPER;
26864 for (sp1 = &super_block[0]; sp1 < &super_block[NR_SUPERS]; sp1++) {
26865 if (sp1->s_dev == dev) {
26866 sp = sp1;
26867 break;
26868 }
26869 }
26870
26871 /* Sync the disk, and invalidate cache. */
26872 (void) do_sync(); /* force any cached blocks out of memory */
26873 invalidate(dev); /* invalidate cache entries for this dev */
26874 if (sp == NIL_SUPER) {
MINIX SOURCE CODE File: servers/fs/mount.c 1003
26875 return(EINVAL);
26876 }
26877
26878 /* Close the device the file system lives on. */
26879 dev_close(dev);
26880
26881 /* Finish off the unmount. */
26882 sp->s_imount->i_mount = NO_MOUNT; /* inode returns to normal */
26883 put_inode(sp->s_imount); /* release the inode mounted on */
26884 put_inode(sp->s_isup); /* release the root inode of the mounted fs */
26885 sp->s_imount = NIL_INODE;
26886 sp->s_dev = NO_DEV;
26887 return(OK);
26888 }
26890 /*===========================================================================*
26891 * name_to_dev *
26892 *===========================================================================*/
26893 PRIVATE dev_t name_to_dev(path)
26894 char *path; /* pointer to path name */
26895 {
26896 /* Convert the block special file ’path’ to a device number. If ’path’
26897 * is not a block special file, return error code in ’err_code’.
26898 */
26899
26900 register struct inode *rip;
26901 register dev_t dev;
26902
26903 /* If ’path’ can’t be opened, give up immediately. */
26904 if ( (rip = eat_path(path)) == NIL_INODE) return(NO_DEV);
26905
26906 /* If ’path’ is not a block special file, return error. */
26907 if ( (rip->i_mode & I_TYPE) != I_BLOCK_SPECIAL) {
26908 err_code = ENOTBLK;
26909 put_inode(rip);
26910 return(NO_DEV);
26911 }
26912
26913 /* Extract the device number. */
26914 dev = (dev_t) rip->i_zone[0];
26915 put_inode(rip);
26916 return(dev);
26917 }