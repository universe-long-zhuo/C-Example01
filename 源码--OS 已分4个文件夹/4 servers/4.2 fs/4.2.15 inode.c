++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/inode.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
22900 /* This file manages the inode table. There are procedures to allocate and
22901 * deallocate inodes, acquire, erase, and release them, and read and write
22902 * them from the disk.
22903 *
22904 * The entry points into this file are
MINIX SOURCE CODE File: servers/fs/inode.c 941
22905 * get_inode: search inode table for a given inode; if not there,
22906 * read it
22907 * put_inode: indicate that an inode is no longer needed in memory
22908 * alloc_inode: allocate a new, unused inode
22909 * wipe_inode: erase some fields of a newly allocated inode
22910 * free_inode: mark an inode as available for a new file
22911 * update_times: update atime, ctime, and mtime
22912 * rw_inode: read a disk block and extract an inode, or corresp. write
22913 * old_icopy: copy to/from in-core inode struct and disk inode (V1.x)
22914 * new_icopy: copy to/from in-core inode struct and disk inode (V2.x)
22915 * dup_inode: indicate that someone else is using an inode table entry
22916 */
22917
22918 #include "fs.h"
22919 #include "buf.h"
22920 #include "file.h"
22921 #include "fproc.h"
22922 #include "inode.h"
22923 #include "super.h"
22924
22925 FORWARD _PROTOTYPE( void old_icopy, (struct inode *rip, d1_inode *dip,
22926 int direction, int norm));
22927 FORWARD _PROTOTYPE( void new_icopy, (struct inode *rip, d2_inode *dip,
22928 int direction, int norm));
22929
22930 /*===========================================================================*
22931 * get_inode *
22932 *===========================================================================*/
22933 PUBLIC struct inode *get_inode(dev, numb)
22934 dev_t dev; /* device on which inode resides */
22935 int numb; /* inode number (ANSI: may not be unshort) */
22936 {
22937 /* Find a slot in the inode table, load the specified inode into it, and
22938 * return a pointer to the slot. If ’dev’ == NO_DEV, just return a free slot.
22939 */
22940
22941 register struct inode *rip, *xp;
22942
22943 /* Search the inode table both for (dev, numb) and a free slot. */
22944 xp = NIL_INODE;
22945 for (rip = &inode[0]; rip < &inode[NR_INODES]; rip++) {
22946 if (rip->i_count > 0) { /* only check used slots for (dev, numb) */
22947 if (rip->i_dev == dev && rip->i_num == numb) {
22948 /* This is the inode that we are looking for. */
22949 rip->i_count++;
22950 return(rip); /* (dev, numb) found */
22951 }
22952 } else {
22953 xp = rip; /* remember this free slot for later */
22954 }
22955 }
22956
22957 /* Inode we want is not currently in use. Did we find a free slot? */
22958 if (xp == NIL_INODE) { /* inode table completely full */
22959 err_code = ENFILE;
22960 return(NIL_INODE);
22961 }
22962
22963 /* A free inode slot has been located. Load the inode into it. */
22964 xp->i_dev = dev;
942 File: servers/fs/inode.c MINIX SOURCE CODE
22965 xp->i_num = numb;
22966 xp->i_count = 1;
22967 if (dev != NO_DEV) rw_inode(xp, READING); /* get inode from disk */
22968 xp->i_update = 0; /* all the times are initially up-to-date */
22969
22970 return(xp);
22971 }
22973 /*===========================================================================*
22974 * put_inode *
22975 *===========================================================================*/
22976 PUBLIC void put_inode(rip)
22977 register struct inode *rip; /* pointer to inode to be released */
22978 {
22979 /* The caller is no longer using this inode. If no one else is using it either
22980 * write it back to the disk immediately. If it has no links, truncate it and
22981 * return it to the pool of available inodes.
22982 */
22983
22984 if (rip == NIL_INODE) return; /* checking here is easier than in caller */
22985 if (--rip->i_count == 0) { /* i_count == 0 means no one is using it now */
22986 if (rip->i_nlinks == 0) {
22987 /* i_nlinks == 0 means free the inode. */
22988 truncate(rip); /* return all the disk blocks */
22989 rip->i_mode = I_NOT_ALLOC; /* clear I_TYPE field */
22990 rip->i_dirt = DIRTY;
22991 free_inode(rip->i_dev, rip->i_num);
22992 } else {
22993 if (rip->i_pipe == I_PIPE) truncate(rip);
22994 }
22995 rip->i_pipe = NO_PIPE; /* should always be cleared */
22996 if (rip->i_dirt == DIRTY) rw_inode(rip, WRITING);
22997 }
22998 }
23000 /*===========================================================================*
23001 * alloc_inode *
23002 *===========================================================================*/
23003 PUBLIC struct inode *alloc_inode(dev_t dev, mode_t bits)
23004 {
23005 /* Allocate a free inode on ’dev’, and return a pointer to it. */
23006
23007 register struct inode *rip;
23008 register struct super_block *sp;
23009 int major, minor, inumb;
23010 bit_t b;
23011
23012 sp = get_super(dev); /* get pointer to super_block */
23013 if (sp->s_rd_only) { /* can’t allocate an inode on a read only device. */
23014 err_code = EROFS;
23015 return(NIL_INODE);
23016 }
23017
23018 /* Acquire an inode from the bit map. */
23019 b = alloc_bit(sp, IMAP, sp->s_isearch);
23020 if (b == NO_BIT) {
23021 err_code = ENFILE;
23022 major = (int) (sp->s_dev >> MAJOR) & BYTE;
23023 minor = (int) (sp->s_dev >> MINOR) & BYTE;
23024 printf("Out of i-nodes on %sdevice %d/%d\n",
MINIX SOURCE CODE File: servers/fs/inode.c 943
23025 sp->s_dev == root_dev ? "root " : "", major, minor);
23026 return(NIL_INODE);
23027 }
23028 sp->s_isearch = b; /* next time start here */
23029 inumb = (int) b; /* be careful not to pass unshort as param */
23030
23031 /* Try to acquire a slot in the inode table. */
23032 if ((rip = get_inode(NO_DEV, inumb)) == NIL_INODE) {
23033 /* No inode table slots available. Free the inode just allocated. */
23034 free_bit(sp, IMAP, b);
23035 } else {
23036 /* An inode slot is available. Put the inode just allocated into it. */
23037 rip->i_mode = bits; /* set up RWX bits */
23038 rip->i_nlinks = 0; /* initial no links */
23039 rip->i_uid = fp->fp_effuid; /* file’s uid is owner’s */
23040 rip->i_gid = fp->fp_effgid; /* ditto group id */
23041 rip->i_dev = dev; /* mark which device it is on */
23042 rip->i_ndzones = sp->s_ndzones; /* number of direct zones */
23043 rip->i_nindirs = sp->s_nindirs; /* number of indirect zones per blk*/
23044 rip->i_sp = sp; /* pointer to super block */
23045
23046 /* Fields not cleared already are cleared in wipe_inode(). They have
23047 * been put there because truncate() needs to clear the same fields if
23048 * the file happens to be open while being truncated. It saves space
23049 * not to repeat the code twice.
23050 */
23051 wipe_inode(rip);
23052 }
23053
23054 return(rip);
23055 }
23057 /*===========================================================================*
23058 * wipe_inode *
23059 *===========================================================================*/
23060 PUBLIC void wipe_inode(rip)
23061 register struct inode *rip; /* the inode to be erased */
23062 {
23063 /* Erase some fields in the inode. This function is called from alloc_inode()
23064 * when a new inode is to be allocated, and from truncate(), when an existing
23065 * inode is to be truncated.
23066 */
23067
23068 register int i;
23069
23070 rip->i_size = 0;
23071 rip->i_update = ATIME | CTIME | MTIME; /* update all times later */
23072 rip->i_dirt = DIRTY;
23073 for (i = 0; i < V2_NR_TZONES; i++) rip->i_zone[i] = NO_ZONE;
23074 }
23076 /*===========================================================================*
23077 * free_inode *
23078 *===========================================================================*/
23079 PUBLIC void free_inode(dev, inumb)
23080 dev_t dev; /* on which device is the inode */
23081 ino_t inumb; /* number of inode to be freed */
23082 {
23083 /* Return an inode to the pool of unallocated inodes. */
23084
944 File: servers/fs/inode.c MINIX SOURCE CODE
23085 register struct super_block *sp;
23086 bit_t b;
23087
23088 /* Locate the appropriate super_block. */
23089 sp = get_super(dev);
23090 if (inumb <= 0 || inumb > sp->s_ninodes) return;
23091 b = inumb;
23092 free_bit(sp, IMAP, b);
23093 if (b < sp->s_isearch) sp->s_isearch = b;
23094 }
23096 /*===========================================================================*
23097 * update_times *
23098 *===========================================================================*/
23099 PUBLIC void update_times(rip)
23100 register struct inode *rip; /* pointer to inode to be read/written */
23101 {
23102 /* Various system calls are required by the standard to update atime, ctime,
23103 * or mtime. Since updating a time requires sending a message to the clock
23104 * task--an expensive business--the times are marked for update by setting
23105 * bits in i_update. When a stat, fstat, or sync is done, or an inode is
23106 * released, update_times() may be called to actually fill in the times.
23107 */
23108
23109 time_t cur_time;
23110 struct super_block *sp;
23111
23112 sp = rip->i_sp; /* get pointer to super block. */
23113 if (sp->s_rd_only) return; /* no updates for read-only file systems */
23114
23115 cur_time = clock_time();
23116 if (rip->i_update & ATIME) rip->i_atime = cur_time;
23117 if (rip->i_update & CTIME) rip->i_ctime = cur_time;
23118 if (rip->i_update & MTIME) rip->i_mtime = cur_time;
23119 rip->i_update = 0; /* they are all up-to-date now */
23120 }
23122 /*===========================================================================*
23123 * rw_inode *
23124 *===========================================================================*/
23125 PUBLIC void rw_inode(rip, rw_flag)
23126 register struct inode *rip; /* pointer to inode to be read/written */
23127 int rw_flag; /* READING or WRITING */
23128 {
23129 /* An entry in the inode table is to be copied to or from the disk. */
23130
23131 register struct buf *bp;
23132 register struct super_block *sp;
23133 d1_inode *dip;
23134 d2_inode *dip2;
23135 block_t b, offset;
23136
23137 /* Get the block where the inode resides. */
23138 sp = get_super(rip->i_dev); /* get pointer to super block */
23139 rip->i_sp = sp; /* inode must contain super block pointer */
23140 offset = sp->s_imap_blocks + sp->s_zmap_blocks + 2;
23141 b = (block_t) (rip->i_num - 1)/sp->s_inodes_per_block + offset;
23142 bp = get_block(rip->i_dev, b, NORMAL);
23143 dip = bp->b_v1_ino + (rip->i_num - 1) % V1_INODES_PER_BLOCK;
23144 dip2 = bp->b_v2_ino + (rip->i_num - 1) %
MINIX SOURCE CODE File: servers/fs/inode.c 945
23145 V2_INODES_PER_BLOCK(sp->s_block_size);
23146
23147 /* Do the read or write. */
23148 if (rw_flag == WRITING) {
23149 if (rip->i_update) update_times(rip); /* times need updating */
23150 if (sp->s_rd_only == FALSE) bp->b_dirt = DIRTY;
23151 }
23152
23153 /* Copy the inode from the disk block to the in-core table or vice versa.
23154 * If the fourth parameter below is FALSE, the bytes are swapped.
23155 */
23156 if (sp->s_version == V1)
23157 old_icopy(rip, dip, rw_flag, sp->s_native);
23158 else
23159 new_icopy(rip, dip2, rw_flag, sp->s_native);
23160
23161 put_block(bp, INODE_BLOCK);
23162 rip->i_dirt = CLEAN;
23163 }
23165 /*===========================================================================*
23166 * old_icopy *
23167 *===========================================================================*/
23168 PRIVATE void old_icopy(rip, dip, direction, norm)
23169 register struct inode *rip; /* pointer to the in-core inode struct */
23170 register d1_inode *dip; /* pointer to the d1_inode inode struct */
23171 int direction; /* READING (from disk) or WRITING (to disk) */
23172 int norm; /* TRUE = do not swap bytes; FALSE = swap */
23173
23174 {
23175 /* The V1.x IBM disk, the V1.x 68000 disk, and the V2 disk (same for IBM and
23176 * 68000) all have different inode layouts. When an inode is read or written
23177 * this routine handles the conversions so that the information in the inode
23178 * table is independent of the disk structure from which the inode came.
23179 * The old_icopy routine copies to and from V1 disks.
23180 */
23181
23182 int i;
23183
23184 if (direction == READING) {
23185 /* Copy V1.x inode to the in-core table, swapping bytes if need be. */
23186 rip->i_mode = conv2(norm, (int) dip->d1_mode);
23187 rip->i_uid = conv2(norm, (int) dip->d1_uid );
23188 rip->i_size = conv4(norm, dip->d1_size);
23189 rip->i_mtime = conv4(norm, dip->d1_mtime);
23190 rip->i_atime = rip->i_mtime;
23191 rip->i_ctime = rip->i_mtime;
23192 rip->i_nlinks = dip->d1_nlinks; /* 1 char */
23193 rip->i_gid = dip->d1_gid; /* 1 char */
23194 rip->i_ndzones = V1_NR_DZONES;
23195 rip->i_nindirs = V1_INDIRECTS;
23196 for (i = 0; i < V1_NR_TZONES; i++)
23197 rip->i_zone[i] = conv2(norm, (int) dip->d1_zone[i]);
23198 } else {
23199 /* Copying V1.x inode to disk from the in-core table. */
23200 dip->d1_mode = conv2(norm, (int) rip->i_mode);
23201 dip->d1_uid = conv2(norm, (int) rip->i_uid );
23202 dip->d1_size = conv4(norm, rip->i_size);
23203 dip->d1_mtime = conv4(norm, rip->i_mtime);
23204 dip->d1_nlinks = rip->i_nlinks; /* 1 char */
946 File: servers/fs/inode.c MINIX SOURCE CODE
23205 dip->d1_gid = rip->i_gid; /* 1 char */
23206 for (i = 0; i < V1_NR_TZONES; i++)
23207 dip->d1_zone[i] = conv2(norm, (int) rip->i_zone[i]);
23208 }
23209 }
23211 /*===========================================================================*
23212 * new_icopy *
23213 *===========================================================================*/
23214 PRIVATE void new_icopy(rip, dip, direction, norm)
23215 register struct inode *rip; /* pointer to the in-core inode struct */
23216 register d2_inode *dip; /* pointer to the d2_inode struct */
23217 int direction; /* READING (from disk) or WRITING (to disk) */
23218 int norm; /* TRUE = do not swap bytes; FALSE = swap */
23219
23220 {
23221 /* Same as old_icopy, but to/from V2 disk layout. */
23222
23223 int i;
23224
23225 if (direction == READING) {
23226 /* Copy V2.x inode to the in-core table, swapping bytes if need be. */
23227 rip->i_mode = conv2(norm,dip->d2_mode);
23228 rip->i_uid = conv2(norm,dip->d2_uid);
23229 rip->i_nlinks = conv2(norm,dip->d2_nlinks);
23230 rip->i_gid = conv2(norm,dip->d2_gid);
23231 rip->i_size = conv4(norm,dip->d2_size);
23232 rip->i_atime = conv4(norm,dip->d2_atime);
23233 rip->i_ctime = conv4(norm,dip->d2_ctime);
23234 rip->i_mtime = conv4(norm,dip->d2_mtime);
23235 rip->i_ndzones = V2_NR_DZONES;
23236 rip->i_nindirs = V2_INDIRECTS(rip->i_sp->s_block_size);
23237 for (i = 0; i < V2_NR_TZONES; i++)
23238 rip->i_zone[i] = conv4(norm, (long) dip->d2_zone[i]);
23239 } else {
23240 /* Copying V2.x inode to disk from the in-core table. */
23241 dip->d2_mode = conv2(norm,rip->i_mode);
23242 dip->d2_uid = conv2(norm,rip->i_uid);
23243 dip->d2_nlinks = conv2(norm,rip->i_nlinks);
23244 dip->d2_gid = conv2(norm,rip->i_gid);
23245 dip->d2_size = conv4(norm,rip->i_size);
23246 dip->d2_atime = conv4(norm,rip->i_atime);
23247 dip->d2_ctime = conv4(norm,rip->i_ctime);
23248 dip->d2_mtime = conv4(norm,rip->i_mtime);
23249 for (i = 0; i < V2_NR_TZONES; i++)
23250 dip->d2_zone[i] = conv4(norm, (long) rip->i_zone[i]);
23251 }
23252 }
23254 /*===========================================================================*
23255 * dup_inode *
23256 *===========================================================================*/
23257 PUBLIC void dup_inode(ip)
23258 struct inode *ip; /* The inode to be duplicated. */
23259 {
23260 /* This routine is a simplified form of get_inode() for the case where
23261 * the inode pointer is already known.
23262 */
23263
23264 ip->i_count++;
MINIX SOURCE CODE File: servers/fs/inode.c 947
23265 }