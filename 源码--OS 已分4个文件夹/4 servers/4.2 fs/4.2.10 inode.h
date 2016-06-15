++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/inode.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21900 /* Inode table. This table holds inodes that are currently in use. In some
21901 * cases they have been opened by an open() or creat() system call, in other
21902 * cases the file system itself needs the inode for one reason or another,
21903 * such as to search a directory for a path name.
21904 * The first part of the struct holds fields that are present on the
21905 * disk; the second part holds fields not present on the disk.
21906 * The disk inode part is also declared in "type.h" as ’d1_inode’ for V1
21907 * file systems and ’d2_inode’ for V2 file systems.
21908 */
21909
21910 EXTERN struct inode {
21911 mode_t i_mode; /* file type, protection, etc. */
21912 nlink_t i_nlinks; /* how many links to this file */
21913 uid_t i_uid; /* user id of the file’s owner */
21914 gid_t i_gid; /* group number */
21915 off_t i_size; /* current file size in bytes */
21916 time_t i_atime; /* time of last access (V2 only) */
21917 time_t i_mtime; /* when was file data last changed */
21918 time_t i_ctime; /* when was inode itself changed (V2 only)*/
21919 zone_t i_zone[V2_NR_TZONES]; /* zone numbers for direct, ind, and dbl ind */
21920
21921 /* The following items are not present on the disk. */
21922 dev_t i_dev; /* which device is the inode on */
21923 ino_t i_num; /* inode number on its (minor) device */
21924 int i_count; /* # times inode used; 0 means slot is free */
MINIX SOURCE CODE File: servers/fs/inode.h 929
21925 int i_ndzones; /* # direct zones (Vx_NR_DZONES) */
21926 int i_nindirs; /* # indirect zones per indirect block */
21927 struct super_block *i_sp; /* pointer to super block for inode’s device */
21928 char i_dirt; /* CLEAN or DIRTY */
21929 char i_pipe; /* set to I_PIPE if pipe */
21930 char i_mount; /* this bit is set if file mounted on */
21931 char i_seek; /* set on LSEEK, cleared on READ/WRITE */
21932 char i_update; /* the ATIME, CTIME, and MTIME bits are here */
21933 } inode[NR_INODES];
21934
21935 #define NIL_INODE (struct inode *) 0 /* indicates absence of inode slot */
21936
21937 /* Field values. Note that CLEAN and DIRTY are defined in "const.h" */
21938 #define NO_PIPE 0 /* i_pipe is NO_PIPE if inode is not a pipe */
21939 #define I_PIPE 1 /* i_pipe is I_PIPE if inode is a pipe */
21940 #define NO_MOUNT 0 /* i_mount is NO_MOUNT if file not mounted on*/
21941 #define I_MOUNT 1 /* i_mount is I_MOUNT if file mounted on */
21942 #define NO_SEEK 0 /* i_seek = NO_SEEK if last op was not SEEK */
21943 #define ISEEK 1 /* i_seek = ISEEK if last op was SEEK */