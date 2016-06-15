++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/super.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
23300 /* This file manages the super block table and the related data structures,
23301 * namely, the bit maps that keep track of which zones and which inodes are
23302 * allocated and which are free. When a new inode or zone is needed, the
23303 * appropriate bit map is searched for a free entry.
23304 *
23305 * The entry points into this file are
23306 * alloc_bit: somebody wants to allocate a zone or inode; find one
23307 * free_bit: indicate that a zone or inode is available for allocation
23308 * get_super: search the ’superblock’ table for a device
23309 * mounted: tells if file inode is on mounted (or ROOT) file system
23310 * read_super: read a superblock
23311 */
23312
23313 #include "fs.h"
23314 #include <string.h>
23315 #include <minix/com.h>
23316 #include "buf.h"
23317 #include "inode.h"
23318 #include "super.h"
23319 #include "const.h"
23320
23321 /*===========================================================================*
23322 * alloc_bit *
23323 *===========================================================================*/
23324 PUBLIC bit_t alloc_bit(sp, map, origin)
23325 struct super_block *sp; /* the filesystem to allocate from */
23326 int map; /* IMAP (inode map) or ZMAP (zone map) */
23327 bit_t origin; /* number of bit to start searching at */
23328 {
23329 /* Allocate a bit from a bit map and return its bit number. */
23330
23331 block_t start_block; /* first bit block */
23332 bit_t map_bits; /* how many bits are there in the bit map? */
23333 unsigned bit_blocks; /* how many blocks are there in the bit map? */
23334 unsigned block, word, bcount;
23335 struct buf *bp;
23336 bitchunk_t *wptr, *wlim, k;
23337 bit_t i, b;
23338
23339 if (sp->s_rd_only)
23340 panic(__FILE__,"can’t allocate bit on read-only filesys.", NO_NUM);
23341
23342 if (map == IMAP) {
23343 start_block = START_BLOCK;
23344 map_bits = sp->s_ninodes + 1;
23345 bit_blocks = sp->s_imap_blocks;
23346 } else {
23347 start_block = START_BLOCK + sp->s_imap_blocks;
23348 map_bits = sp->s_zones - (sp->s_firstdatazone - 1);
23349 bit_blocks = sp->s_zmap_blocks;
948 File: servers/fs/super.c MINIX SOURCE CODE
23350 }
23351
23352 /* Figure out where to start the bit search (depends on ’origin’). */
23353 if (origin >= map_bits) origin = 0; /* for robustness */
23354
23355 /* Locate the starting place. */
23356 block = origin / FS_BITS_PER_BLOCK(sp->s_block_size);
23357 word = (origin % FS_BITS_PER_BLOCK(sp->s_block_size)) / FS_BITCHUNK_BITS;
23358
23359 /* Iterate over all blocks plus one, because we start in the middle. */
23360 bcount = bit_blocks + 1;
23361 do {
23362 bp = get_block(sp->s_dev, start_block + block, NORMAL);
23363 wlim = &bp->b_bitmap[FS_BITMAP_CHUNKS(sp->s_block_size)];
23364
23365 /* Iterate over the words in block. */
23366 for (wptr = &bp->b_bitmap[word]; wptr < wlim; wptr++) {
23367
23368 /* Does this word contain a free bit? */
23369 if (*wptr == (bitchunk_t) ˜0) continue;
23370
23371 /* Find and allocate the free bit. */
23372 k = conv2(sp->s_native, (int) *wptr);
23373 for (i = 0; (k & (1 << i)) != 0; ++i) {}
23374
23375 /* Bit number from the start of the bit map. */
23376 b = ((bit_t) block * FS_BITS_PER_BLOCK(sp->s_block_size))
23377 + (wptr - &bp->b_bitmap[0]) * FS_BITCHUNK_BITS
23378 + i;
23379
23380 /* Don’t allocate bits beyond the end of the map. */
23381 if (b >= map_bits) break;
23382
23383 /* Allocate and return bit number. */
23384 k |= 1 << i;
23385 *wptr = conv2(sp->s_native, (int) k);
23386 bp->b_dirt = DIRTY;
23387 put_block(bp, MAP_BLOCK);
23388 return(b);
23389 }
23390 put_block(bp, MAP_BLOCK);
23391 if (++block >= bit_blocks) block = 0; /* last block, wrap around */
23392 word = 0;
23393 } while (--bcount > 0);
23394 return(NO_BIT); /* no bit could be allocated */
23395 }
23397 /*===========================================================================*
23398 * free_bit *
23399 *===========================================================================*/
23400 PUBLIC void free_bit(sp, map, bit_returned)
23401 struct super_block *sp; /* the filesystem to operate on */
23402 int map; /* IMAP (inode map) or ZMAP (zone map) */
23403 bit_t bit_returned; /* number of bit to insert into the map */
23404 {
23405 /* Return a zone or inode by turning off its bitmap bit. */
23406
23407 unsigned block, word, bit;
23408 struct buf *bp;
23409 bitchunk_t k, mask;
MINIX SOURCE CODE File: servers/fs/super.c 949
23410 block_t start_block;
23411
23412 if (sp->s_rd_only)
23413 panic(__FILE__,"can’t free bit on read-only filesys.", NO_NUM);
23414
23415 if (map == IMAP) {
23416 start_block = START_BLOCK;
23417 } else {
23418 start_block = START_BLOCK + sp->s_imap_blocks;
23419 }
23420 block = bit_returned / FS_BITS_PER_BLOCK(sp->s_block_size);
23421 word = (bit_returned % FS_BITS_PER_BLOCK(sp->s_block_size))
23422 / FS_BITCHUNK_BITS;
23423
23424 bit = bit_returned % FS_BITCHUNK_BITS;
23425 mask = 1 << bit;
23426
23427 bp = get_block(sp->s_dev, start_block + block, NORMAL);
23428
23429 k = conv2(sp->s_native, (int) bp->b_bitmap[word]);
23430 if (!(k & mask)) {
23431 panic(__FILE__,map == IMAP ? "tried to free unused inode" :
23432 "tried to free unused block", NO_NUM);
23433 }
23434
23435 k &= ˜mask;
23436 bp->b_bitmap[word] = conv2(sp->s_native, (int) k);
23437 bp->b_dirt = DIRTY;
23438
23439 put_block(bp, MAP_BLOCK);
23440 }
23442 /*===========================================================================*
23443 * get_super *
23444 *===========================================================================*/
23445 PUBLIC struct super_block *get_super(dev)
23446 dev_t dev; /* device number whose super_block is sought */
23447 {
23448 /* Search the superblock table for this device. It is supposed to be there. */
23449
23450 register struct super_block *sp;
23451
23452 if (dev == NO_DEV)
23453 panic(__FILE__,"request for super_block of NO_DEV", NO_NUM);
23454
23455 for (sp = &super_block[0]; sp < &super_block[NR_SUPERS]; sp++)
23456 if (sp->s_dev == dev) return(sp);
23457
23458 /* Search failed. Something wrong. */
23459 panic(__FILE__,"can’t find superblock for device (in decimal)", (int) dev);
23460
23461 return(NIL_SUPER); /* to keep the compiler and lint quiet */
23462 }
23464 /*===========================================================================*
23465 * get_block_size *
23466 *===========================================================================*/
23467 PUBLIC int get_block_size(dev_t dev)
23468 {
23469 /* Search the superblock table for this device. */
950 File: servers/fs/super.c MINIX SOURCE CODE
23470
23471 register struct super_block *sp;
23472
23473 if (dev == NO_DEV)
23474 panic(__FILE__,"request for block size of NO_DEV", NO_NUM);
23475
23476 for (sp = &super_block[0]; sp < &super_block[NR_SUPERS]; sp++) {
23477 if (sp->s_dev == dev) {
23478 return(sp->s_block_size);
23479 }
23480 }
23481
23482 /* no mounted filesystem? use this block size then. */
23483 return MIN_BLOCK_SIZE;
23484 }
23486 /*===========================================================================*
23487 * mounted *
23488 *===========================================================================*/
23489 PUBLIC int mounted(rip)
23490 register struct inode *rip; /* pointer to inode */
23491 {
23492 /* Report on whether the given inode is on a mounted (or ROOT) file system. */
23493
23494 register struct super_block *sp;
23495 register dev_t dev;
23496
23497 dev = (dev_t) rip->i_zone[0];
23498 if (dev == root_dev) return(TRUE); /* inode is on root file system */
23499
23500 for (sp = &super_block[0]; sp < &super_block[NR_SUPERS]; sp++)
23501 if (sp->s_dev == dev) return(TRUE);
23502
23503 return(FALSE);
23504 }
23506 /*===========================================================================*
23507 * read_super *
23508 *===========================================================================*/
23509 PUBLIC int read_super(sp)
23510 register struct super_block *sp; /* pointer to a superblock */
23511 {
23512 /* Read a superblock. */
23513 dev_t dev;
23514 int magic;
23515 int version, native, r;
23516 static char sbbuf[MIN_BLOCK_SIZE];
23517
23518 dev = sp->s_dev; /* save device (will be overwritten by copy) */
23519 if (dev == NO_DEV)
23520 panic(__FILE__,"request for super_block of NO_DEV", NO_NUM);
23521 r = dev_io(DEV_READ, dev, FS_PROC_NR,
23522 sbbuf, SUPER_BLOCK_BYTES, MIN_BLOCK_SIZE, 0);
23523 if (r != MIN_BLOCK_SIZE) {
23524 return EINVAL;
23525 }
23526 memcpy(sp, sbbuf, sizeof(*sp));
23527 sp->s_dev = NO_DEV; /* restore later */
23528 magic = sp->s_magic; /* determines file system type */
23529
MINIX SOURCE CODE File: servers/fs/super.c 951
23530 /* Get file system version and type. */
23531 if (magic == SUPER_MAGIC || magic == conv2(BYTE_SWAP, SUPER_MAGIC)) {
23532 version = V1;
23533 native = (magic == SUPER_MAGIC);
23534 } else if (magic == SUPER_V2 || magic == conv2(BYTE_SWAP, SUPER_V2)) {
23535 version = V2;
23536 native = (magic == SUPER_V2);
23537 } else if (magic == SUPER_V3) {
23538 version = V3;
23539 native = 1;
23540 } else {
23541 return(EINVAL);
23542 }
23543
23544 /* If the super block has the wrong byte order, swap the fields; the magic
23545 * number doesn’t need conversion. */
23546 sp->s_ninodes = conv4(native, sp->s_ninodes);
23547 sp->s_nzones = conv2(native, (int) sp->s_nzones);
23548 sp->s_imap_blocks = conv2(native, (int) sp->s_imap_blocks);
23549 sp->s_zmap_blocks = conv2(native, (int) sp->s_zmap_blocks);
23550 sp->s_firstdatazone = conv2(native, (int) sp->s_firstdatazone);
23551 sp->s_log_zone_size = conv2(native, (int) sp->s_log_zone_size);
23552 sp->s_max_size = conv4(native, sp->s_max_size);
23553 sp->s_zones = conv4(native, sp->s_zones);
23554
23555 /* In V1, the device size was kept in a short, s_nzones, which limited
23556 * devices to 32K zones. For V2, it was decided to keep the size as a
23557 * long. However, just changing s_nzones to a long would not work, since
23558 * then the position of s_magic in the super block would not be the same
23559 * in V1 and V2 file systems, and there would be no way to tell whether
23560 * a newly mounted file system was V1 or V2. The solution was to introduce
23561 * a new variable, s_zones, and copy the size there.
23562 *
23563 * Calculate some other numbers that depend on the version here too, to
23564 * hide some of the differences.
23565 */
23566 if (version == V1) {
23567 sp->s_block_size = STATIC_BLOCK_SIZE;
23568 sp->s_zones = sp->s_nzones; /* only V1 needs this copy */
23569 sp->s_inodes_per_block = V1_INODES_PER_BLOCK;
23570 sp->s_ndzones = V1_NR_DZONES;
23571 sp->s_nindirs = V1_INDIRECTS;
23572 } else {
23573 if (version == V2)
23574 sp->s_block_size = STATIC_BLOCK_SIZE;
23575 if (sp->s_block_size < MIN_BLOCK_SIZE)
23576 return EINVAL;
23577 sp->s_inodes_per_block = V2_INODES_PER_BLOCK(sp->s_block_size);
23578 sp->s_ndzones = V2_NR_DZONES;
23579 sp->s_nindirs = V2_INDIRECTS(sp->s_block_size);
23580 }
23581
23582 if (sp->s_block_size < MIN_BLOCK_SIZE) {
23583 return EINVAL;
23584 }
23585 if (sp->s_block_size > MAX_BLOCK_SIZE) {
23586 printf("Filesystem block size is %d kB; maximum filesystem\n"
23587 "block size is %d kB. This limit can be increased by recompiling.\n",
23588 sp->s_block_size/1024, MAX_BLOCK_SIZE/1024);
23589 return EINVAL;
952 File: servers/fs/super.c MINIX SOURCE CODE
23590 }
23591 if ((sp->s_block_size % 512) != 0) {
23592 return EINVAL;
23593 }
23594 if (SUPER_SIZE > sp->s_block_size) {
23595 return EINVAL;
23596 }
23597 if ((sp->s_block_size % V2_INODE_SIZE) != 0 ||
23598 (sp->s_block_size % V1_INODE_SIZE) != 0) {
23599 return EINVAL;
23600 }
23601
23602 sp->s_isearch = 0; /* inode searches initially start at 0 */
23603 sp->s_zsearch = 0; /* zone searches initially start at 0 */
23604 sp->s_version = version;
23605 sp->s_native = native;
23606
23607 /* Make a few basic checks to see if super block looks reasonable. */
23608 if (sp->s_imap_blocks < 1 || sp->s_zmap_blocks < 1
23609 || sp->s_ninodes < 1 || sp->s_zones < 1
23610 || (unsigned) sp->s_log_zone_size > 4) {
23611 printf("not enough imap or zone map blocks, \n");
23612 printf("or not enough inodes, or not enough zones, "
23613 "or zone size too large\n");
23614 return(EINVAL);
23615 }
23616 sp->s_dev = dev; /* restore device number */
23617 return(OK);
23618 }