++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/read.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
25000 /* This file contains the heart of the mechanism used to read (and write)
25001 * files. Read and write requests are split up into chunks that do not cross
25002 * block boundaries. Each chunk is then processed in turn. Reads on special
25003 * files are also detected and handled.
25004 *
25005 * The entry points into this file are
25006 * do_read: perform the READ system call by calling read_write
25007 * read_write: actually do the work of READ and WRITE
25008 * read_map: given an inode and file position, look up its zone number
25009 * rd_indir: read an entry in an indirect block
25010 * read_ahead: manage the block read ahead business
25011 */
25012
25013 #include "fs.h"
25014 #include <fcntl.h>
25015 #include <minix/com.h>
25016 #include "buf.h"
25017 #include "file.h"
25018 #include "fproc.h"
25019 #include "inode.h"
25020 #include "param.h"
25021 #include "super.h"
25022
25023 FORWARD _PROTOTYPE( int rw_chunk, (struct inode *rip, off_t position,
25024 unsigned off, int chunk, unsigned left, int rw_flag,
25025 char *buff, int seg, int usr, int block_size, int *completed));
25026
25027 /*===========================================================================*
25028 * do_read *
25029 *===========================================================================*/
25030 PUBLIC int do_read()
25031 {
25032 return(read_write(READING));
25033 }
25035 /*===========================================================================*
25036 * read_write *
25037 *===========================================================================*/
25038 PUBLIC int read_write(rw_flag)
25039 int rw_flag; /* READING or WRITING */
25040 {
25041 /* Perform read(fd, buffer, nbytes) or write(fd, buffer, nbytes) call. */
25042
25043 register struct inode *rip;
25044 register struct filp *f;
974 File: servers/fs/read.c MINIX SOURCE CODE
25045 off_t bytes_left, f_size, position;
25046 unsigned int off, cum_io;
25047 int op, oflags, r, chunk, usr, seg, block_spec, char_spec;
25048 int regular, partial_pipe = 0, partial_cnt = 0;
25049 mode_t mode_word;
25050 struct filp *wf;
25051 int block_size;
25052 int completed, r2 = OK;
25053 phys_bytes p;
25054
25055 /* left unfinished rw_chunk()s from previous call! this can’t happen.
25056 * it means something has gone wrong we can’t repair now.
25057 */
25058 if (bufs_in_use < 0) {
25059 panic(__FILE__,"start - bufs_in_use negative", bufs_in_use);
25060 }
25061
25062 /* MM loads segments by putting funny things in upper 10 bits of ’fd’. */
25063 if (who == PM_PROC_NR && (m_in.fd & (˜BYTE)) ) {
25064 usr = m_in.fd >> 7;
25065 seg = (m_in.fd >> 5) & 03;
25066 m_in.fd &= 037; /* get rid of user and segment bits */
25067 } else {
25068 usr = who; /* normal case */
25069 seg = D;
25070 }
25071
25072 /* If the file descriptor is valid, get the inode, size and mode. */
25073 if (m_in.nbytes < 0) return(EINVAL);
25074 if ((f = get_filp(m_in.fd)) == NIL_FILP) return(err_code);
25075 if (((f->filp_mode) & (rw_flag == READING ? R_BIT : W_BIT)) == 0) {
25076 return(f->filp_mode == FILP_CLOSED ? EIO : EBADF);
25077 }
25078 if (m_in.nbytes == 0)
25079 return(0); /* so char special files need not check for 0*/
25080
25081 /* check if user process has the memory it needs.
25082 * if not, copying will fail later.
25083 * do this after 0-check above because umap doesn’t want to map 0 bytes.
25084 */
25085 if ((r = sys_umap(usr, seg, (vir_bytes) m_in.buffer, m_in.nbytes, &p)) != OK)
25086 return r;
25087 position = f->filp_pos;
25088 oflags = f->filp_flags;
25089 rip = f->filp_ino;
25090 f_size = rip->i_size;
25091 r = OK;
25092 if (rip->i_pipe == I_PIPE) {
25093 /* fp->fp_cum_io_partial is only nonzero when doing partial writes */
25094 cum_io = fp->fp_cum_io_partial;
25095 } else {
25096 cum_io = 0;
25097 }
25098 op = (rw_flag == READING ? DEV_READ : DEV_WRITE);
25099 mode_word = rip->i_mode & I_TYPE;
25100 regular = mode_word == I_REGULAR || mode_word == I_NAMED_PIPE;
25101
25102 if ((char_spec = (mode_word == I_CHAR_SPECIAL ? 1 : 0))) {
25103 if (rip->i_zone[0] == NO_DEV)
25104 panic(__FILE__,"read_write tries to read from "
MINIX SOURCE CODE File: servers/fs/read.c 975
25105 "character device NO_DEV", NO_NUM);
25106 block_size = get_block_size(rip->i_zone[0]);
25107 }
25108 if ((block_spec = (mode_word == I_BLOCK_SPECIAL ? 1 : 0))) {
25109 f_size = ULONG_MAX;
25110 if (rip->i_zone[0] == NO_DEV)
25111 panic(__FILE__,"read_write tries to read from "
25112 " block device NO_DEV", NO_NUM);
25113 block_size = get_block_size(rip->i_zone[0]);
25114 }
25115
25116 if (!char_spec && !block_spec)
25117 block_size = rip->i_sp->s_block_size;
25118
25119 rdwt_err = OK; /* set to EIO if disk error occurs */
25120
25121 /* Check for character special files. */
25122 if (char_spec) {
25123 dev_t dev;
25124 dev = (dev_t) rip->i_zone[0];
25125 r = dev_io(op, dev, usr, m_in.buffer, position, m_in.nbytes, oflags);
25126 if (r >= 0) {
25127 cum_io = r;
25128 position += r;
25129 r = OK;
25130 }
25131 } else {
25132 if (rw_flag == WRITING && block_spec == 0) {
25133 /* Check in advance to see if file will grow too big. */
25134 if (position > rip->i_sp->s_max_size - m_in.nbytes)
25135 return(EFBIG);
25136
25137 /* Check for O_APPEND flag. */
25138 if (oflags & O_APPEND) position = f_size;
25139
25140 /* Clear the zone containing present EOF if hole about
25141 * to be created. This is necessary because all unwritten
25142 * blocks prior to the EOF must read as zeros.
25143 */
25144 if (position > f_size) clear_zone(rip, f_size, 0);
25145 }
25146
25147 /* Pipes are a little different. Check. */
25148 if (rip->i_pipe == I_PIPE) {
25149 r = pipe_check(rip, rw_flag, oflags,
25150 m_in.nbytes, position, &partial_cnt, 0);
25151 if (r <= 0) return(r);
25152 }
25153
25154 if (partial_cnt > 0) partial_pipe = 1;
25155
25156 /* Split the transfer into chunks that don’t span two blocks. */
25157 while (m_in.nbytes != 0) {
25158
25159 off = (unsigned int) (position % block_size);/* offset in blk*/
25160 if (partial_pipe) { /* pipes only */
25161 chunk = MIN(partial_cnt, block_size - off);
25162 } else
25163 chunk = MIN(m_in.nbytes, block_size - off);
25164 if (chunk < 0) chunk = block_size - off;
976 File: servers/fs/read.c MINIX SOURCE CODE
25165
25166 if (rw_flag == READING) {
25167 bytes_left = f_size - position;
25168 if (position >= f_size) break; /* we are beyond EOF */
25169 if (chunk > bytes_left) chunk = (int) bytes_left;
25170 }
25171
25172 /* Read or write ’chunk’ bytes. */
25173 r = rw_chunk(rip, position, off, chunk, (unsigned) m_in.nbytes,
25174 rw_flag, m_in.buffer, seg, usr, block_size, &completed);
25175
25176 if (r != OK) break; /* EOF reached */
25177 if (rdwt_err < 0) break;
25178
25179 /* Update counters and pointers. */
25180 m_in.buffer += chunk; /* user buffer address */
25181 m_in.nbytes -= chunk; /* bytes yet to be read */
25182 cum_io += chunk; /* bytes read so far */
25183 position += chunk; /* position within the file */
25184
25185 if (partial_pipe) {
25186 partial_cnt -= chunk;
25187 if (partial_cnt <= 0) break;
25188 }
25189 }
25190 }
25191
25192 /* On write, update file size and access time. */
25193 if (rw_flag == WRITING) {
25194 if (regular || mode_word == I_DIRECTORY) {
25195 if (position > f_size) rip->i_size = position;
25196 }
25197 } else {
25198 if (rip->i_pipe == I_PIPE) {
25199 if ( position >= rip->i_size) {
25200 /* Reset pipe pointers. */
25201 rip->i_size = 0; /* no data left */
25202 position = 0; /* reset reader(s) */
25203 wf = find_filp(rip, W_BIT);
25204 if (wf != NIL_FILP) wf->filp_pos = 0;
25205 }
25206 }
25207 }
25208 f->filp_pos = position;
25209
25210 /* Check to see if read-ahead is called for, and if so, set it up. */
25211 if (rw_flag == READING && rip->i_seek == NO_SEEK && position % block_size== 0
25212 && (regular || mode_word == I_DIRECTORY)) {
25213 rdahed_inode = rip;
25214 rdahedpos = position;
25215 }
25216 rip->i_seek = NO_SEEK;
25217
25218 if (rdwt_err != OK) r = rdwt_err; /* check for disk error */
25219 if (rdwt_err == END_OF_FILE) r = OK;
25220
25221 /* if user-space copying failed, read/write failed. */
25222 if (r == OK && r2 != OK) {
25223 r = r2;
25224 }
MINIX SOURCE CODE File: servers/fs/read.c 977
25225 if (r == OK) {
25226 if (rw_flag == READING) rip->i_update |= ATIME;
25227 if (rw_flag == WRITING) rip->i_update |= CTIME | MTIME;
25228 rip->i_dirt = DIRTY; /* inode is thus now dirty */
25229 if (partial_pipe) {
25230 partial_pipe = 0;
25231 /* partial write on pipe with */
25232 /* O_NONBLOCK, return write count */
25233 if (!(oflags & O_NONBLOCK)) {
25234 fp->fp_cum_io_partial = cum_io;
25235 suspend(XPIPE); /* partial write on pipe with */
25236 return(SUSPEND); /* nbyte > PIPE_SIZE - non-atomic */
25237 }
25238 }
25239 fp->fp_cum_io_partial = 0;
25240 return(cum_io);
25241 }
25242 if (bufs_in_use < 0) {
25243 panic(__FILE__,"end - bufs_in_use negative", bufs_in_use);
25244 }
25245 return(r);
25246 }
25248 /*===========================================================================*
25249 * rw_chunk *
25250 *===========================================================================*/
25251 PRIVATE int rw_chunk(rip, position, off, chunk, left, rw_flag, buff,
25252 seg, usr, block_size, completed)
25253 register struct inode *rip; /* pointer to inode for file to be rd/wr */
25254 off_t position; /* position within file to read or write */
25255 unsigned off; /* off within the current block */
25256 int chunk; /* number of bytes to read or write */
25257 unsigned left; /* max number of bytes wanted after position */
25258 int rw_flag; /* READING or WRITING */
25259 char *buff; /* virtual address of the user buffer */
25260 int seg; /* T or D segment in user space */
25261 int usr; /* which user process */
25262 int block_size; /* block size of FS operating on */
25263 int *completed; /* number of bytes copied */
25264 {
25265 /* Read or write (part of) a block. */
25266
25267 register struct buf *bp;
25268 register int r = OK;
25269 int n, block_spec;
25270 block_t b;
25271 dev_t dev;
25272
25273 *completed = 0;
25274
25275 block_spec = (rip->i_mode & I_TYPE) == I_BLOCK_SPECIAL;
25276 if (block_spec) {
25277 b = position/block_size;
25278 dev = (dev_t) rip->i_zone[0];
25279 } else {
25280 b = read_map(rip, position);
25281 dev = rip->i_dev;
25282 }
25283
25284 if (!block_spec && b == NO_BLOCK) {
978 File: servers/fs/read.c MINIX SOURCE CODE
25285 if (rw_flag == READING) {
25286 /* Reading from a nonexistent block. Must read as all zeros.*/
25287 bp = get_block(NO_DEV, NO_BLOCK, NORMAL); /* get a buffer */
25288 zero_block(bp);
25289 } else {
25290 /* Writing to a nonexistent block. Create and enter in inode.*/
25291 if ((bp= new_block(rip, position)) == NIL_BUF)return(err_code);
25292 }
25293 } else if (rw_flag == READING) {
25294 /* Read and read ahead if convenient. */
25295 bp = rahead(rip, b, position, left);
25296 } else {
25297 /* Normally an existing block to be partially overwritten is first read
25298 * in. However, a full block need not be read in. If it is already in
25299 * the cache, acquire it, otherwise just acquire a free buffer.
25300 */
25301 n = (chunk == block_size ? NO_READ : NORMAL);
25302 if (!block_spec && off == 0 && position >= rip->i_size) n = NO_READ;
25303 bp = get_block(dev, b, n);
25304 }
25305
25306 /* In all cases, bp now points to a valid buffer. */
25307 if (bp == NIL_BUF) {
25308 panic(__FILE__,"bp not valid in rw_chunk, this can’t happen", NO_NUM);
25309 }
25310 if (rw_flag == WRITING && chunk != block_size && !block_spec &&
25311 position >= rip->i_size && off == 0) {
25312 zero_block(bp);
25313 }
25314
25315 if (rw_flag == READING) {
25316 /* Copy a chunk from the block buffer to user space. */
25317 r = sys_vircopy(FS_PROC_NR, D, (phys_bytes) (bp->b_data+off),
25318 usr, seg, (phys_bytes) buff,
25319 (phys_bytes) chunk);
25320 } else {
25321 /* Copy a chunk from user space to the block buffer. */
25322 r = sys_vircopy(usr, seg, (phys_bytes) buff,
25323 FS_PROC_NR, D, (phys_bytes) (bp->b_data+off),
25324 (phys_bytes) chunk);
25325 bp->b_dirt = DIRTY;
25326 }
25327 n = (off + chunk == block_size ? FULL_DATA_BLOCK : PARTIAL_DATA_BLOCK);
25328 put_block(bp, n);
25329
25330 return(r);
25331 }
25334 /*===========================================================================*
25335 * read_map *
25336 *===========================================================================*/
25337 PUBLIC block_t read_map(rip, position)
25338 register struct inode *rip; /* ptr to inode to map from */
25339 off_t position; /* position in file whose blk wanted */
25340 {
25341 /* Given an inode and a position within the corresponding file, locate the
25342 * block (not zone) number in which that position is to be found and return it.
25343 */
25344
MINIX SOURCE CODE File: servers/fs/read.c 979
25345 register struct buf *bp;
25346 register zone_t z;
25347 int scale, boff, dzones, nr_indirects, index, zind, ex;
25348 block_t b;
25349 long excess, zone, block_pos;
25350
25351 scale = rip->i_sp->s_log_zone_size; /* for block-zone conversion */
25352 block_pos = position/rip->i_sp->s_block_size; /* relative blk # in file */
25353 zone = block_pos >> scale; /* position’s zone */
25354 boff = (int) (block_pos - (zone << scale) ); /* relative blk # within zone */
25355 dzones = rip->i_ndzones;
25356 nr_indirects = rip->i_nindirs;
25357
25358 /* Is ’position’ to be found in the inode itself? */
25359 if (zone < dzones) {
25360 zind = (int) zone; /* index should be an int */
25361 z = rip->i_zone[zind];
25362 if (z == NO_ZONE) return(NO_BLOCK);
25363 b = ((block_t) z << scale) + boff;
25364 return(b);
25365 }
25366
25367 /* It is not in the inode, so it must be single or double indirect. */
25368 excess = zone - dzones; /* first Vx_NR_DZONES don’t count */
25369
25370 if (excess < nr_indirects) {
25371 /* ’position’ can be located via the single indirect block. */
25372 z = rip->i_zone[dzones];
25373 } else {
25374 /* ’position’ can be located via the double indirect block. */
25375 if ( (z = rip->i_zone[dzones+1]) == NO_ZONE) return(NO_BLOCK);
25376 excess -= nr_indirects; /* single indir doesn’t count*/
25377 b = (block_t) z << scale;
25378 bp = get_block(rip->i_dev, b, NORMAL); /* get double indirect block */
25379 index = (int) (excess/nr_indirects);
25380 z = rd_indir(bp, index); /* z= zone for single*/
25381 put_block(bp, INDIRECT_BLOCK); /* release double ind block */
25382 excess = excess % nr_indirects; /* index into single ind blk */
25383 }
25384
25385 /* ’z’ is zone num for single indirect block; ’excess’ is index into it. */
25386 if (z == NO_ZONE) return(NO_BLOCK);
25387 b = (block_t) z << scale; /* b is blk # for single ind */
25388 bp = get_block(rip->i_dev, b, NORMAL); /* get single indirect block */
25389 ex = (int) excess; /* need an integer */
25390 z = rd_indir(bp, ex); /* get block pointed to */
25391 put_block(bp, INDIRECT_BLOCK); /* release single indir blk */
25392 if (z == NO_ZONE) return(NO_BLOCK);
25393 b = ((block_t) z << scale) + boff;
25394 return(b);
25395 }
25397 /*===========================================================================*
25398 * rd_indir *
25399 *===========================================================================*/
25400 PUBLIC zone_t rd_indir(bp, index)
25401 struct buf *bp; /* pointer to indirect block */
25402 int index; /* index into *bp */
25403 {
25404 /* Given a pointer to an indirect block, read one entry. The reason for
980 File: servers/fs/read.c MINIX SOURCE CODE
25405 * making a separate routine out of this is that there are four cases:
25406 * V1 (IBM and 68000), and V2 (IBM and 68000).
25407 */
25408
25409 struct super_block *sp;
25410 zone_t zone; /* V2 zones are longs (shorts in V1) */
25411
25412 sp = get_super(bp->b_dev); /* need super block to find file sys type */
25413
25414 /* read a zone from an indirect block */
25415 if (sp->s_version == V1)
25416 zone = (zone_t) conv2(sp->s_native, (int) bp->b_v1_ind[index]);
25417 else
25418 zone = (zone_t) conv4(sp->s_native, (long) bp->b_v2_ind[index]);
25419
25420 if (zone != NO_ZONE &&
25421 (zone < (zone_t) sp->s_firstdatazone || zone >= sp->s_zones)) {
25422 printf("Illegal zone number %ld in indirect block, index %d\n",
25423 (long) zone, index);
25424 panic(__FILE__,"check file system", NO_NUM);
25425 }
25426 return(zone);
25427 }
25429 /*===========================================================================*
25430 * read_ahead *
25431 *===========================================================================*/
25432 PUBLIC void read_ahead()
25433 {
25434 /* Read a block into the cache before it is needed. */
25435 int block_size;
25436 register struct inode *rip;
25437 struct buf *bp;
25438 block_t b;
25439
25440 rip = rdahed_inode; /* pointer to inode to read ahead from */
25441 block_size = get_block_size(rip->i_dev);
25442 rdahed_inode = NIL_INODE; /* turn off read ahead */
25443 if ( (b = read_map(rip, rdahedpos)) == NO_BLOCK) return; /* at EOF */
25444 bp = rahead(rip, b, rdahedpos, block_size);
25445 put_block(bp, PARTIAL_DATA_BLOCK);
25446 }
25448 /*===========================================================================*
25449 * rahead *
25450 *===========================================================================*/
25451 PUBLIC struct buf *rahead(rip, baseblock, position, bytes_ahead)
25452 register struct inode *rip; /* pointer to inode for file to be read */
25453 block_t baseblock; /* block at current position */
25454 off_t position; /* position within file */
25455 unsigned bytes_ahead; /* bytes beyond position for immediate use */
25456 {
25457 /* Fetch a block from the cache or the device. If a physical read is
25458 * required, prefetch as many more blocks as convenient into the cache.
25459 * This usually covers bytes_ahead and is at least BLOCKS_MINIMUM.
25460 * The device driver may decide it knows better and stop reading at a
25461 * cylinder boundary (or after an error). Rw_scattered() puts an optional
25462 * flag on all reads to allow this.
25463 */
25464 int block_size;
MINIX SOURCE CODE File: servers/fs/read.c 981
25465 /* Minimum number of blocks to prefetch. */
25466 # define BLOCKS_MINIMUM (NR_BUFS < 50 ? 18 : 32)
25467 int block_spec, scale, read_q_size;
25468 unsigned int blocks_ahead, fragment;
25469 block_t block, blocks_left;
25470 off_t ind1_pos;
25471 dev_t dev;
25472 struct buf *bp;
25473 static struct buf *read_q[NR_BUFS];
25474
25475 block_spec = (rip->i_mode & I_TYPE) == I_BLOCK_SPECIAL;
25476 if (block_spec) {
25477 dev = (dev_t) rip->i_zone[0];
25478 } else {
25479 dev = rip->i_dev;
25480 }
25481 block_size = get_block_size(dev);
25482
25483 block = baseblock;
25484 bp = get_block(dev, block, PREFETCH);
25485 if (bp->b_dev != NO_DEV) return(bp);
25486
25487 /* The best guess for the number of blocks to prefetch: A lot.
25488 * It is impossible to tell what the device looks like, so we don’t even
25489 * try to guess the geometry, but leave it to the driver.
25490 *
25491 * The floppy driver can read a full track with no rotational delay, and it
25492 * avoids reading partial tracks if it can, so handing it enough buffers to
25493 * read two tracks is perfect. (Two, because some diskette types have
25494 * an odd number of sectors per track, so a block may span tracks.)
25495 *
25496 * The disk drivers don’t try to be smart. With todays disks it is
25497 * impossible to tell what the real geometry looks like, so it is best to
25498 * read as much as you can. With luck the caching on the drive allows
25499 * for a little time to start the next read.
25500 *
25501 * The current solution below is a bit of a hack, it just reads blocks from
25502 * the current file position hoping that more of the file can be found. A
25503 * better solution must look at the already available zone pointers and
25504 * indirect blocks (but don’t call read_map!).
25505 */
25506
25507 fragment = position % block_size;
25508 position -= fragment;
25509 bytes_ahead += fragment;
25510
25511 blocks_ahead = (bytes_ahead + block_size - 1) / block_size;
25512
25513 if (block_spec && rip->i_size == 0) {
25514 blocks_left = NR_IOREQS;
25515 } else {
25516 blocks_left = (rip->i_size - position + block_size - 1) / block_size;
25517
25518 /* Go for the first indirect block if we are in its neighborhood. */
25519 if (!block_spec) {
25520 scale = rip->i_sp->s_log_zone_size;
25521 ind1_pos = (off_t) rip->i_ndzones * (block_size << scale);
25522 if (position <= ind1_pos && rip->i_size > ind1_pos) {
25523 blocks_ahead++;
25524 blocks_left++;
982 File: servers/fs/read.c MINIX SOURCE CODE
25525 }
25526 }
25527 }
25528
25529 /* No more than the maximum request. */
25530 if (blocks_ahead > NR_IOREQS) blocks_ahead = NR_IOREQS;
25531
25532 /* Read at least the minimum number of blocks, but not after a seek. */
25533 if (blocks_ahead < BLOCKS_MINIMUM && rip->i_seek == NO_SEEK)
25534 blocks_ahead = BLOCKS_MINIMUM;
25535
25536 /* Can’t go past end of file. */
25537 if (blocks_ahead > blocks_left) blocks_ahead = blocks_left;
25538
25539 read_q_size = 0;
25540
25541 /* Acquire block buffers. */
25542 for (;;) {
25543 read_q[read_q_size++] = bp;
25544
25545 if (--blocks_ahead == 0) break;
25546
25547 /* Don’t trash the cache, leave 4 free. */
25548 if (bufs_in_use >= NR_BUFS - 4) break;
25549
25550 block++;
25551
25552 bp = get_block(dev, block, PREFETCH);
25553 if (bp->b_dev != NO_DEV) {
25554 /* Oops, block already in the cache, get out. */
25555 put_block(bp, FULL_DATA_BLOCK);
25556 break;
25557 }
25558 }
25559 rw_scattered(dev, read_q, read_q_size, READING);
25560 return(get_block(dev, baseblock, NORMAL));
25561 }