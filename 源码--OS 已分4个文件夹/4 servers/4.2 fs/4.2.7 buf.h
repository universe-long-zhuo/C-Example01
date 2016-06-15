++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/buf.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21600 /* Buffer (block) cache. To acquire a block, a routine calls get_block(),
21601 * telling which block it wants. The block is then regarded as "in use"
21602 * and has its ’b_count’ field incremented. All the blocks that are not
21603 * in use are chained together in an LRU list, with ’front’ pointing
21604 * to the least recently used block, and ’rear’ to the most recently used
21605 * block. A reverse chain, using the field b_prev is also maintained.
21606 * Usage for LRU is measured by the time the put_block() is done. The second
21607 * parameter to put_block() can violate the LRU order and put a block on the
21608 * front of the list, if it will probably not be needed soon. If a block
21609 * is modified, the modifying routine must set b_dirt to DIRTY, so the block
21610 * will eventually be rewritten to the disk.
21611 */
21612
21613 #include <sys/dir.h> /* need struct direct */
21614 #include <dirent.h>
21615
21616 EXTERN struct buf {
21617 /* Data portion of the buffer. */
21618 union {
21619 char b__data[MAX_BLOCK_SIZE]; /* ordinary user data */
21620 /* directory block */
21621 struct direct b__dir[NR_DIR_ENTRIES(MAX_BLOCK_SIZE)];
21622 /* V1 indirect block */
21623 zone1_t b__v1_ind[V1_INDIRECTS];
21624 /* V2 indirect block */
21625 zone_t b__v2_ind[V2_INDIRECTS(MAX_BLOCK_SIZE)];
21626 /* V1 inode block */
21627 d1_inode b__v1_ino[V1_INODES_PER_BLOCK];
21628 /* V2 inode block */
21629 d2_inode b__v2_ino[V2_INODES_PER_BLOCK(MAX_BLOCK_SIZE)];
MINIX SOURCE CODE File: servers/fs/buf.h 927
21630 /* bit map block */
21631 bitchunk_t b__bitmap[FS_BITMAP_CHUNKS(MAX_BLOCK_SIZE)];
21632 } b;
21633
21634 /* Header portion of the buffer. */
21635 struct buf *b_next; /* used to link all free bufs in a chain */
21636 struct buf *b_prev; /* used to link all free bufs the other way */
21637 struct buf *b_hash; /* used to link bufs on hash chains */
21638 block_t b_blocknr; /* block number of its (minor) device */
21639 dev_t b_dev; /* major | minor device where block resides */
21640 char b_dirt; /* CLEAN or DIRTY */
21641 char b_count; /* number of users of this buffer */
21642 } buf[NR_BUFS];
21643
21644 /* A block is free if b_dev == NO_DEV. */
21645
21646 #define NIL_BUF ((struct buf *) 0) /* indicates absence of a buffer */
21647
21648 /* These defs make it possible to use to bp->b_data instead of bp->b.b__data */
21649 #define b_data b.b__data
21650 #define b_dir b.b__dir
21651 #define b_v1_ind b.b__v1_ind
21652 #define b_v2_ind b.b__v2_ind
21653 #define b_v1_ino b.b__v1_ino
21654 #define b_v2_ino b.b__v2_ino
21655 #define b_bitmap b.b__bitmap
21656
21657 EXTERN struct buf *buf_hash[NR_BUF_HASH]; /* the buffer hash table */
21658
21659 EXTERN struct buf *front; /* points to least recently used free block */
21660 EXTERN struct buf *rear; /* points to most recently used free block */
21661 EXTERN int bufs_in_use; /* # bufs currently in use (not on free list)*/
21662
21663 /* When a block is released, the type of usage is passed to put_block(). */
21664 #define WRITE_IMMED 0100 /* block should be written to disk now */
21665 #define ONE_SHOT 0200 /* set if block not likely to be needed soon */
21666
21667 #define INODE_BLOCK 0 /* inode block */
21668 #define DIRECTORY_BLOCK 1 /* directory block */
21669 #define INDIRECT_BLOCK 2 /* pointer block */
21670 #define MAP_BLOCK 3 /* bit map */
21671 #define FULL_DATA_BLOCK 5 /* data, fully used */
21672 #define PARTIAL_DATA_BLOCK 6 /* data, partly used*/
21673
21674 #define HASH_MASK (NR_BUF_HASH - 1) /* mask for hashing block numbers */