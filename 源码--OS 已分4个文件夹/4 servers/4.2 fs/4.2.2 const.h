++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21000 /* Tables sizes */
21001 #define V1_NR_DZONES 7 /* # direct zone numbers in a V1 inode */
21002 #define V1_NR_TZONES 9 /* total # zone numbers in a V1 inode */
21003 #define V2_NR_DZONES 7 /* # direct zone numbers in a V2 inode */
21004 #define V2_NR_TZONES 10 /* total # zone numbers in a V2 inode */
21005
21006 #define NR_FILPS 128 /* # slots in filp table */
21007 #define NR_INODES 64 /* # slots in "in core" inode table */
21008 #define NR_SUPERS 8 /* # slots in super block table */
21009 #define NR_LOCKS 8 /* # slots in the file locking table */
21010
21011 /* The type of sizeof may be (unsigned) long. Use the following macro for
21012 * taking the sizes of small objects so that there are no surprises like
21013 * (small) long constants being passed to routines expecting an int.
21014 */
21015 #define usizeof(t) ((unsigned) sizeof(t))
21016
21017 /* File system types. */
21018 #define SUPER_MAGIC 0x137F /* magic number contained in super-block */
21019 #define SUPER_REV 0x7F13 /* magic # when 68000 disk read on PC or vv */
920 File: servers/fs/const.h MINIX SOURCE CODE
21020 #define SUPER_V2 0x2468 /* magic # for V2 file systems */
21021 #define SUPER_V2_REV 0x6824 /* V2 magic written on PC, read on 68K or vv */
21022 #define SUPER_V3 0x4d5a /* magic # for V3 file systems */
21023
21024 #define V1 1 /* version number of V1 file systems */
21025 #define V2 2 /* version number of V2 file systems */
21026 #define V3 3 /* version number of V3 file systems */
21027
21028 /* Miscellaneous constants */
21029 #define SU_UID ((uid_t) 0) /* super_user’s uid_t */
21030 #define SYS_UID ((uid_t) 0) /* uid_t for processes MM and INIT */
21031 #define SYS_GID ((gid_t) 0) /* gid_t for processes MM and INIT */
21032 #define NORMAL 0 /* forces get_block to do disk read */
21033 #define NO_READ 1 /* prevents get_block from doing disk read */
21034 #define PREFETCH 2 /* tells get_block not to read or mark dev */
21035
21036 #define XPIPE (-NR_TASKS-1) /* used in fp_task when susp’d on pipe */
21037 #define XLOCK (-NR_TASKS-2) /* used in fp_task when susp’d on lock */
21038 #define XPOPEN (-NR_TASKS-3) /* used in fp_task when susp’d on pipe open */
21039 #define XSELECT (-NR_TASKS-4) /* used in fp_task when susp’d on select */
21040
21041 #define NO_BIT ((bit_t) 0) /* returned by alloc_bit() to signal failure */
21042
21043 #define DUP_MASK 0100 /* mask to distinguish dup2 from dup */
21044
21045 #define LOOK_UP 0 /* tells search_dir to lookup string */
21046 #define ENTER 1 /* tells search_dir to make dir entry */
21047 #define DELETE 2 /* tells search_dir to delete entry */
21048 #define IS_EMPTY 3 /* tells search_dir to ret. OK or ENOTEMPTY */
21049
21050 #define CLEAN 0 /* disk and memory copies identical */
21051 #define DIRTY 1 /* disk and memory copies differ */
21052 #define ATIME 002 /* set if atime field needs updating */
21053 #define CTIME 004 /* set if ctime field needs updating */
21054 #define MTIME 010 /* set if mtime field needs updating */
21055
21056 #define BYTE_SWAP 0 /* tells conv2/conv4 to swap bytes */
21057
21058 #define END_OF_FILE (-104) /* eof detected */
21059
21060 #define ROOT_INODE 1 /* inode number for root directory */
21061 #define BOOT_BLOCK ((block_t) 0) /* block number of boot block */
21062 #define SUPER_BLOCK_BYTES (1024) /* bytes offset */
21063 #define START_BLOCK 2 /* first block of FS (not counting SB) */
21064
21065 #define DIR_ENTRY_SIZE usizeof (struct direct) /* # bytes/dir entry */
21066 #define NR_DIR_ENTRIES(b) ((b)/DIR_ENTRY_SIZE) /* # dir entries/blk */
21067 #define SUPER_SIZE usizeof (struct super_block) /* super_block size */
21068 #define PIPE_SIZE(b) (V1_NR_DZONES*(b)) /* pipe size in bytes */
21069
21070 #define FS_BITMAP_CHUNKS(b) ((b)/usizeof (bitchunk_t))/* # map chunks/blk */
21071 #define FS_BITCHUNK_BITS (usizeof(bitchunk_t) * CHAR_BIT)
21072 #define FS_BITS_PER_BLOCK(b) (FS_BITMAP_CHUNKS(b) * FS_BITCHUNK_BITS)
21073
21074 /* Derived sizes pertaining to the V1 file system. */
21075 #define V1_ZONE_NUM_SIZE usizeof (zone1_t) /* # bytes in V1 zone */
21076 #define V1_INODE_SIZE usizeof (d1_inode) /* bytes in V1 dsk ino */
21077
21078 /* # zones/indir block */
21079 #define V1_INDIRECTS (STATIC_BLOCK_SIZE/V1_ZONE_NUM_SIZE)
MINIX SOURCE CODE File: servers/fs/const.h 921
21080
21081 /* # V1 dsk inodes/blk */
21082 #define V1_INODES_PER_BLOCK (STATIC_BLOCK_SIZE/V1_INODE_SIZE)
21083
21084 /* Derived sizes pertaining to the V2 file system. */
21085 #define V2_ZONE_NUM_SIZE usizeof (zone_t) /* # bytes in V2 zone */
21086 #define V2_INODE_SIZE usizeof (d2_inode) /* bytes in V2 dsk ino */
21087 #define V2_INDIRECTS(b) ((b)/V2_ZONE_NUM_SIZE) /* # zones/indir block */
21088 #define V2_INODES_PER_BLOCK(b) ((b)/V2_INODE_SIZE)/* # V2 dsk inodes/blk */