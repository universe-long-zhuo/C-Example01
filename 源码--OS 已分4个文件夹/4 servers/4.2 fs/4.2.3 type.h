++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/type.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21100 /* Declaration of the V1 inode as it is on the disk (not in core). */
21101 typedef struct { /* V1.x disk inode */
21102 mode_t d1_mode; /* file type, protection, etc. */
21103 uid_t d1_uid; /* user id of the file’s owner */
21104 off_t d1_size; /* current file size in bytes */
21105 time_t d1_mtime; /* when was file data last changed */
21106 u8_t d1_gid; /* group number */
21107 u8_t d1_nlinks; /* how many links to this file */
21108 u16_t d1_zone[V1_NR_TZONES]; /* block nums for direct, ind, and dbl ind */
21109 } d1_inode;
21110
21111 /* Declaration of the V2 inode as it is on the disk (not in core). */
21112 typedef struct { /* V2.x disk inode */
21113 mode_t d2_mode; /* file type, protection, etc. */
21114 u16_t d2_nlinks; /* how many links to this file. HACK! */
21115 uid_t d2_uid; /* user id of the file’s owner. */
21116 u16_t d2_gid; /* group number HACK! */
21117 off_t d2_size; /* current file size in bytes */
21118 time_t d2_atime; /* when was file data last accessed */
21119 time_t d2_mtime; /* when was file data last changed */
21120 time_t d2_ctime; /* when was inode data last changed */
21121 zone_t d2_zone[V2_NR_TZONES]; /* block nums for direct, ind, and dbl ind */
21122 } d2_inode;