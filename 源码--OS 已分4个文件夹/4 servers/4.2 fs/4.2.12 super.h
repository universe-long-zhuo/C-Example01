++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/super.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
22100 /* Super block table. The root file system and every mounted file system
22101 * has an entry here. The entry holds information about the sizes of the bit
22102 * maps and inodes. The s_ninodes field gives the number of inodes available
22103 * for files and directories, including the root directory. Inode 0 is
22104 * on the disk, but not used. Thus s_ninodes = 4 means that 5 bits will be
22105 * used in the bit map, bit 0, which is always 1 and not used, and bits 1-4
22106 * for files and directories. The disk layout is:
22107 *
22108 * Item # blocks
22109 * boot block 1
22110 * super block 1 (offset 1kB)
22111 * inode map s_imap_blocks
22112 * zone map s_zmap_blocks
22113 * inodes (s_ninodes + ’inodes per block’ - 1)/’inodes per block’
22114 * unused whatever is needed to fill out the current zone
22115 * data zones (s_zones - s_firstdatazone) << s_log_zone_size
22116 *
22117 * A super_block slot is free if s_dev == NO_DEV.
22118 */
22119
22120 EXTERN struct super_block {
22121 ino_t s_ninodes; /* # usable inodes on the minor device */
22122 zone1_t s_nzones; /* total device size, including bit maps etc */
22123 short s_imap_blocks; /* # of blocks used by inode bit map */
22124 short s_zmap_blocks; /* # of blocks used by zone bit map */
MINIX SOURCE CODE File: servers/fs/super.h 931
22125 zone1_t s_firstdatazone; /* number of first data zone */
22126 short s_log_zone_size; /* log2 of blocks/zone */
22127 short s_pad; /* try to avoid compiler-dependent padding */
22128 off_t s_max_size; /* maximum file size on this device */
22129 zone_t s_zones; /* number of zones (replaces s_nzones in V2) */
22130 short s_magic; /* magic number to recognize super-blocks */
22131
22132 /* The following items are valid on disk only for V3 and above */
22133
22134 /* The block size in bytes. Minimum MIN_BLOCK SIZE. SECTOR_SIZE
22135 * multiple. If V1 or V2 filesystem, this should be
22136 * initialised to STATIC_BLOCK_SIZE. Maximum MAX_BLOCK_SIZE.
22137 */
22138 short s_pad2; /* try to avoid compiler-dependent padding */
22139 unsigned short s_block_size; /* block size in bytes. */
22140 char s_disk_version; /* filesystem format sub-version */
22141
22142 /* The following items are only used when the super_block is in memory. */
22143 struct inode *s_isup; /* inode for root dir of mounted file sys */
22144 struct inode *s_imount; /* inode mounted on */
22145 unsigned s_inodes_per_block; /* precalculated from magic number */
22146 dev_t s_dev; /* whose super block is this? */
22147 int s_rd_only; /* set to 1 iff file sys mounted read only */
22148 int s_native; /* set to 1 iff not byte swapped file system */
22149 int s_version; /* file system version, zero means bad magic */
22150 int s_ndzones; /* # direct zones in an inode */
22151 int s_nindirs; /* # indirect zones per indirect block */
22152 bit_t s_isearch; /* inodes below this bit number are in use */
22153 bit_t s_zsearch; /* all zones below this bit number are in use*/
22154 } super_block[NR_SUPERS];
22155
22156 #define NIL_SUPER (struct super_block *) 0
22157 #define IMAP 0 /* operating on the inode bit map */
22158 #define ZMAP 1 /* operating on the zone bit map */