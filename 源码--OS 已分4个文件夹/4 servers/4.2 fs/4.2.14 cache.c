++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/cache.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
22400 /* The file system maintains a buffer cache to reduce the number of disk
22401 * accesses needed. Whenever a read or write to the disk is done, a check is
22402 * first made to see if the block is in the cache. This file manages the
22403 * cache.
22404 *
22405 * The entry points into this file are:
22406 * get_block: request to fetch a block for reading or writing from cache
22407 * put_block: return a block previously requested with get_block
22408 * alloc_zone: allocate a new zone (to increase the length of a file)
22409 * free_zone: release a zone (when a file is removed)
22410 * rw_block: read or write a block from the disk itself
22411 * invalidate: remove all the cache blocks on some device
22412 */
22413
22414 #include "fs.h"
22415 #include <minix/com.h>
22416 #include "buf.h"
22417 #include "file.h"
22418 #include "fproc.h"
22419 #include "super.h"
934 File: servers/fs/cache.c MINIX SOURCE CODE
22420
22421 FORWARD _PROTOTYPE( void rm_lru, (struct buf *bp) );
22422
22423 /*===========================================================================*
22424 * get_block *
22425 *===========================================================================*/
22426 PUBLIC struct buf *get_block(dev, block, only_search)
22427 register dev_t dev; /* on which device is the block? */
22428 register block_t block; /* which block is wanted? */
22429 int only_search; /* if NO_READ, don’t read, else act normal */
22430 {
22431 /* Check to see if the requested block is in the block cache. If so, return
22432 * a pointer to it. If not, evict some other block and fetch it (unless
22433 * ’only_search’ is 1). All the blocks in the cache that are not in use
22434 * are linked together in a chain, with ’front’ pointing to the least recently
22435 * used block and ’rear’ to the most recently used block. If ’only_search’ is
22436 * 1, the block being requested will be overwritten in its entirety, so it is
22437 * only necessary to see if it is in the cache; if it is not, any free buffer
22438 * will do. It is not necessary to actually read the block in from disk.
22439 * If ’only_search’ is PREFETCH, the block need not be read from the disk,
22440 * and the device is not to be marked on the block, so callers can tell if
22441 * the block returned is valid.
22442 * In addition to the LRU chain, there is also a hash chain to link together
22443 * blocks whose block numbers end with the same bit strings, for fast lookup.
22444 */
22445
22446 int b;
22447 register struct buf *bp, *prev_ptr;
22448
22449 /* Search the hash chain for (dev, block). Do_read() can use
22450 * get_block(NO_DEV ...) to get an unnamed block to fill with zeros when
22451 * someone wants to read from a hole in a file, in which case this search
22452 * is skipped
22453 */
22454 if (dev != NO_DEV) {
22455 b = (int) block & HASH_MASK;
22456 bp = buf_hash[b];
22457 while (bp != NIL_BUF) {
22458 if (bp->b_blocknr == block && bp->b_dev == dev) {
22459 /* Block needed has been found. */
22460 if (bp->b_count == 0) rm_lru(bp);
22461 bp->b_count++; /* record that block is in use */
22462
22463 return(bp);
22464 } else {
22465 /* This block is not the one sought. */
22466 bp = bp->b_hash; /* move to next block on hash chain */
22467 }
22468 }
22469 }
22470
22471 /* Desired block is not on available chain. Take oldest block (’front’). */
22472 if ((bp = front) == NIL_BUF) panic(__FILE__,"all buffers in use", NR_BUFS);
22473 rm_lru(bp);
22474
22475 /* Remove the block that was just taken from its hash chain. */
22476 b = (int) bp->b_blocknr & HASH_MASK;
22477 prev_ptr = buf_hash[b];
22478 if (prev_ptr == bp) {
22479 buf_hash[b] = bp->b_hash;
MINIX SOURCE CODE File: servers/fs/cache.c 935
22480 } else {
22481 /* The block just taken is not on the front of its hash chain. */
22482 while (prev_ptr->b_hash != NIL_BUF)
22483 if (prev_ptr->b_hash == bp) {
22484 prev_ptr->b_hash = bp->b_hash; /* found it */
22485 break;
22486 } else {
22487 prev_ptr = prev_ptr->b_hash; /* keep looking */
22488 }
22489 }
22490
22491 /* If the block taken is dirty, make it clean by writing it to the disk.
22492 * Avoid hysteresis by flushing all other dirty blocks for the same device.
22493 */
22494 if (bp->b_dev != NO_DEV) {
22495 if (bp->b_dirt == DIRTY) flushall(bp->b_dev);
22496 }
22497
22498 /* Fill in block’s parameters and add it to the hash chain where it goes. */
22499 bp->b_dev = dev; /* fill in device number */
22500 bp->b_blocknr = block; /* fill in block number */
22501 bp->b_count++; /* record that block is being used */
22502 b = (int) bp->b_blocknr & HASH_MASK;
22503 bp->b_hash = buf_hash[b];
22504 buf_hash[b] = bp; /* add to hash list */
22505
22506 /* Go get the requested block unless searching or prefetching. */
22507 if (dev != NO_DEV) {
22508 if (only_search == PREFETCH) bp->b_dev = NO_DEV;
22509 else
22510 if (only_search == NORMAL) {
22511 rw_block(bp, READING);
22512 }
22513 }
22514 return(bp); /* return the newly acquired block */
22515 }
22517 /*===========================================================================*
22518 * put_block *
22519 *===========================================================================*/
22520 PUBLIC void put_block(bp, block_type)
22521 register struct buf *bp; /* pointer to the buffer to be released */
22522 int block_type; /* INODE_BLOCK, DIRECTORY_BLOCK, or whatever */
22523 {
22524 /* Return a block to the list of available blocks. Depending on ’block_type’
22525 * it may be put on the front or rear of the LRU chain. Blocks that are
22526 * expected to be needed again shortly (e.g., partially full data blocks)
22527 * go on the rear; blocks that are unlikely to be needed again shortly
22528 * (e.g., full data blocks) go on the front. Blocks whose loss can hurt
22529 * the integrity of the file system (e.g., inode blocks) are written to
22530 * disk immediately if they are dirty.
22531 */
22532 if (bp == NIL_BUF) return; /* it is easier to check here than in caller */
22533
22534 bp->b_count--; /* there is one use fewer now */
22535 if (bp->b_count != 0) return; /* block is still in use */
22536
22537 bufs_in_use--; /* one fewer block buffers in use */
22538
22539 /* Put this block back on the LRU chain. If the ONE_SHOT bit is set in
936 File: servers/fs/cache.c MINIX SOURCE CODE
22540 * ’block_type’, the block is not likely to be needed again shortly, so put
22541 * it on the front of the LRU chain where it will be the first one to be
22542 * taken when a free buffer is needed later.
22543 */
22544 if (bp->b_dev == DEV_RAM || block_type & ONE_SHOT) {
22545 /* Block probably won’t be needed quickly. Put it on front of chain.
22546 * It will be the next block to be evicted from the cache.
22547 */
22548 bp->b_prev = NIL_BUF;
22549 bp->b_next = front;
22550 if (front == NIL_BUF)
22551 rear = bp; /* LRU chain was empty */
22552 else
22553 front->b_prev = bp;
22554 front = bp;
22555 } else {
22556 /* Block probably will be needed quickly. Put it on rear of chain.
22557 * It will not be evicted from the cache for a long time.
22558 */
22559 bp->b_prev = rear;
22560 bp->b_next = NIL_BUF;
22561 if (rear == NIL_BUF)
22562 front = bp;
22563 else
22564 rear->b_next = bp;
22565 rear = bp;
22566 }
22567
22568 /* Some blocks are so important (e.g., inodes, indirect blocks) that they
22569 * should be written to the disk immediately to avoid messing up the file
22570 * system in the event of a crash.
22571 */
22572 if ((block_type & WRITE_IMMED) && bp->b_dirt==DIRTY && bp->b_dev != NO_DEV) {
22573 rw_block(bp, WRITING);
22574 }
22575 }
22577 /*===========================================================================*
22578 * alloc_zone *
22579 *===========================================================================*/
22580 PUBLIC zone_t alloc_zone(dev, z)
22581 dev_t dev; /* device where zone wanted */
22582 zone_t z; /* try to allocate new zone near this one */
22583 {
22584 /* Allocate a new zone on the indicated device and return its number. */
22585
22586 int major, minor;
22587 bit_t b, bit;
22588 struct super_block *sp;
22589
22590 /* Note that the routine alloc_bit() returns 1 for the lowest possible
22591 * zone, which corresponds to sp->s_firstdatazone. To convert a value
22592 * between the bit number, ’b’, used by alloc_bit() and the zone number, ’z’,
22593 * stored in the inode, use the formula:
22594 * z = b + sp->s_firstdatazone - 1
22595 * Alloc_bit() never returns 0, since this is used for NO_BIT (failure).
22596 */
22597 sp = get_super(dev);
22598
22599 /* If z is 0, skip initial part of the map known to be fully in use. */
MINIX SOURCE CODE File: servers/fs/cache.c 937
22600 if (z == sp->s_firstdatazone) {
22601 bit = sp->s_zsearch;
22602 } else {
22603 bit = (bit_t) z - (sp->s_firstdatazone - 1);
22604 }
22605 b = alloc_bit(sp, ZMAP, bit);
22606 if (b == NO_BIT) {
22607 err_code = ENOSPC;
22608 major = (int) (sp->s_dev >> MAJOR) & BYTE;
22609 minor = (int) (sp->s_dev >> MINOR) & BYTE;
22610 printf("No space on %sdevice %d/%d\n",
22611 sp->s_dev == root_dev ? "root " : "", major, minor);
22612 return(NO_ZONE);
22613 }
22614 if (z == sp->s_firstdatazone) sp->s_zsearch = b; /* for next time */
22615 return(sp->s_firstdatazone - 1 + (zone_t) b);
22616 }
22618 /*===========================================================================*
22619 * free_zone *
22620 *===========================================================================*/
22621 PUBLIC void free_zone(dev, numb)
22622 dev_t dev; /* device where zone located */
22623 zone_t numb; /* zone to be returned */
22624 {
22625 /* Return a zone. */
22626
22627 register struct super_block *sp;
22628 bit_t bit;
22629
22630 /* Locate the appropriate super_block and return bit. */
22631 sp = get_super(dev);
22632 if (numb < sp->s_firstdatazone || numb >= sp->s_zones) return;
22633 bit = (bit_t) (numb - (sp->s_firstdatazone - 1));
22634 free_bit(sp, ZMAP, bit);
22635 if (bit < sp->s_zsearch) sp->s_zsearch = bit;
22636 }
22638 /*===========================================================================*
22639 * rw_block *
22640 *===========================================================================*/
22641 PUBLIC void rw_block(bp, rw_flag)
22642 register struct buf *bp; /* buffer pointer */
22643 int rw_flag; /* READING or WRITING */
22644 {
22645 /* Read or write a disk block. This is the only routine in which actual disk
22646 * I/O is invoked. If an error occurs, a message is printed here, but the error
22647 * is not reported to the caller. If the error occurred while purging a block
22648 * from the cache, it is not clear what the caller could do about it anyway.
22649 */
22650
22651 int r, op;
22652 off_t pos;
22653 dev_t dev;
22654 int block_size;
22655
22656 block_size = get_block_size(bp->b_dev);
22657
22658 if ( (dev = bp->b_dev) != NO_DEV) {
22659 pos = (off_t) bp->b_blocknr * block_size;
938 File: servers/fs/cache.c MINIX SOURCE CODE
22660 op = (rw_flag == READING ? DEV_READ : DEV_WRITE);
22661 r = dev_io(op, dev, FS_PROC_NR, bp->b_data, pos, block_size, 0);
22662 if (r != block_size) {
22663 if (r >= 0) r = END_OF_FILE;
22664 if (r != END_OF_FILE)
22665 printf("Unrecoverable disk error on device %d/%d, block %ld\n",
22666 (dev>>MAJOR)&BYTE, (dev>>MINOR)&BYTE, bp->b_blocknr);
22667 bp->b_dev = NO_DEV; /* invalidate block */
22668
22669 /* Report read errors to interested parties. */
22670 if (rw_flag == READING) rdwt_err = r;
22671 }
22672 }
22673
22674 bp->b_dirt = CLEAN;
22675 }
22677 /*===========================================================================*
22678 * invalidate *
22679 *===========================================================================*/
22680 PUBLIC void invalidate(device)
22681 dev_t device; /* device whose blocks are to be purged */
22682 {
22683 /* Remove all the blocks belonging to some device from the cache. */
22684
22685 register struct buf *bp;
22686
22687 for (bp = &buf[0]; bp < &buf[NR_BUFS]; bp++)
22688 if (bp->b_dev == device) bp->b_dev = NO_DEV;
22689 }
22691 /*===========================================================================*
22692 * flushall *
22693 *===========================================================================*/
22694 PUBLIC void flushall(dev)
22695 dev_t dev; /* device to flush */
22696 {
22697 /* Flush all dirty blocks for one device. */
22698
22699 register struct buf *bp;
22700 static struct buf *dirty[NR_BUFS]; /* static so it isn’t on stack */
22701 int ndirty;
22702
22703 for (bp = &buf[0], ndirty = 0; bp < &buf[NR_BUFS]; bp++)
22704 if (bp->b_dirt == DIRTY && bp->b_dev == dev) dirty[ndirty++] = bp;
22705 rw_scattered(dev, dirty, ndirty, WRITING);
22706 }
22708 /*===========================================================================*
22709 * rw_scattered *
22710 *===========================================================================*/
22711 PUBLIC void rw_scattered(dev, bufq, bufqsize, rw_flag)
22712 dev_t dev; /* major-minor device number */
22713 struct buf **bufq; /* pointer to array of buffers */
22714 int bufqsize; /* number of buffers */
22715 int rw_flag; /* READING or WRITING */
22716 {
22717 /* Read or write scattered data from a device. */
22718
22719 register struct buf *bp;
MINIX SOURCE CODE File: servers/fs/cache.c 939
22720 int gap;
22721 register int i;
22722 register iovec_t *iop;
22723 static iovec_t iovec[NR_IOREQS]; /* static so it isn’t on stack */
22724 int j, r;
22725 int block_size;
22726
22727 block_size = get_block_size(dev);
22728
22729 /* (Shell) sort buffers on b_blocknr. */
22730 gap = 1;
22731 do
22732 gap = 3 * gap + 1;
22733 while (gap <= bufqsize);
22734 while (gap != 1) {
22735 gap /= 3;
22736 for (j = gap; j < bufqsize; j++) {
22737 for (i = j - gap;
22738 i >= 0 && bufq[i]->b_blocknr > bufq[i + gap]->b_blocknr;
22739 i -= gap) {
22740 bp = bufq[i];
22741 bufq[i] = bufq[i + gap];
22742 bufq[i + gap] = bp;
22743 }
22744 }
22745 }
22746
22747 /* Set up I/O vector and do I/O. The result of dev_io is OK if everything
22748 * went fine, otherwise the error code for the first failed transfer.
22749 */
22750 while (bufqsize > 0) {
22751 for (j = 0, iop = iovec; j < NR_IOREQS && j < bufqsize; j++, iop++) {
22752 bp = bufq[j];
22753 if (bp->b_blocknr != bufq[0]->b_blocknr + j) break;
22754 iop->iov_addr = (vir_bytes) bp->b_data;
22755 iop->iov_size = block_size;
22756 }
22757 r = dev_io(rw_flag == WRITING ? DEV_SCATTER : DEV_GATHER,
22758 dev, FS_PROC_NR, iovec,
22759 (off_t) bufq[0]->b_blocknr * block_size, j, 0);
22760
22761 /* Harvest the results. Dev_io reports the first error it may have
22762 * encountered, but we only care if it’s the first block that failed.
22763 */
22764 for (i = 0, iop = iovec; i < j; i++, iop++) {
22765 bp = bufq[i];
22766 if (iop->iov_size != 0) {
22767 /* Transfer failed. An error? Do we care? */
22768 if (r != OK && i == 0) {
22769 printf(
22770 "fs: I/O error on device %d/%d, block %lu\n",
22771 (dev>>MAJOR)&BYTE, (dev>>MINOR)&BYTE,
22772 bp->b_blocknr);
22773 bp->b_dev = NO_DEV; /* invalidate block */
22774 }
22775 break;
22776 }
22777 if (rw_flag == READING) {
22778 bp->b_dev = dev; /* validate block */
22779 put_block(bp, PARTIAL_DATA_BLOCK);
940 File: servers/fs/cache.c MINIX SOURCE CODE
22780 } else {
22781 bp->b_dirt = CLEAN;
22782 }
22783 }
22784 bufq += i;
22785 bufqsize -= i;
22786 if (rw_flag == READING) {
22787 /* Don’t bother reading more than the device is willing to
22788 * give at this time. Don’t forget to release those extras.
22789 */
22790 while (bufqsize > 0) {
22791 put_block(*bufq++, PARTIAL_DATA_BLOCK);
22792 bufqsize--;
22793 }
22794 }
22795 if (rw_flag == WRITING && i == 0) {
22796 /* We’re not making progress, this means we might keep
22797 * looping. Buffers remain dirty if un-written. Buffers are
22798 * lost if invalidate()d or LRU-removed while dirty. This
22799 * is better than keeping unwritable blocks around forever..
22800 */
22801 break;
22802 }
22803 }
22804 }
22806 /*===========================================================================*
22807 * rm_lru *
22808 *===========================================================================*/
22809 PRIVATE void rm_lru(bp)
22810 struct buf *bp;
22811 {
22812 /* Remove a block from its LRU chain. */
22813 struct buf *next_ptr, *prev_ptr;
22814
22815 bufs_in_use++;
22816 next_ptr = bp->b_next; /* successor on LRU chain */
22817 prev_ptr = bp->b_prev; /* predecessor on LRU chain */
22818 if (prev_ptr != NIL_BUF)
22819 prev_ptr->b_next = next_ptr;
22820 else
22821 front = next_ptr; /* this block was at front of chain */
22822
22823 if (next_ptr != NIL_BUF)
22824 next_ptr->b_prev = prev_ptr;
22825 else
22826 rear = prev_ptr; /* this block was at rear of chain */
22827 }