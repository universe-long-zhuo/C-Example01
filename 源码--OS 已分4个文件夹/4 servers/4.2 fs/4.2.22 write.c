++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/write.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
25600 /* This file is the counterpart of "read.c". It contains the code for writing
25601 * insofar as this is not contained in read_write().
25602 *
25603 * The entry points into this file are
25604 * do_write: call read_write to perform the WRITE system call
25605 * clear_zone: erase a zone in the middle of a file
25606 * new_block: acquire a new block
25607 */
25608
25609 #include "fs.h"
25610 #include <string.h>
25611 #include "buf.h"
25612 #include "file.h"
25613 #include "fproc.h"
25614 #include "inode.h"
MINIX SOURCE CODE File: servers/fs/write.c 983
25615 #include "super.h"
25616
25617 FORWARD _PROTOTYPE( int write_map, (struct inode *rip, off_t position,
25618 zone_t new_zone) );
25619
25620 FORWARD _PROTOTYPE( void wr_indir, (struct buf *bp, int index, zone_t zone) );
25621
25622 /*===========================================================================*
25623 * do_write *
25624 *===========================================================================*/
25625 PUBLIC int do_write()
25626 {
25627 /* Perform the write(fd, buffer, nbytes) system call. */
25628
25629   return(read_write(WRITING));
25630 }
25632 /*===========================================================================*
25633 * write_map *
25634 *===========================================================================*/
25635 PRIVATE int write_map(rip, position, new_zone)
25636 register struct inode *rip; /* pointer to inode to be changed */
25637 off_t position; /* file address to be mapped */
25638 zone_t new_zone; /* zone # to be inserted */
25639 {
25640 /* Write a new zone into an inode. */
25641 int scale, ind_ex, new_ind, new_dbl, zones, nr_indirects, single, zindex, ex;
25642 zone_t z, z1;
25643 register block_t b;
25644 long excess, zone;
25645 struct buf *bp;
25646
25647 rip->i_dirt = DIRTY; /* inode will be changed */
25648 bp = NIL_BUF;
25649 scale = rip->i_sp->s_log_zone_size; /* for zone-block conversion */
25650 /* relative zone # to insert */
25651 zone = (position/rip->i_sp->s_block_size) >> scale;
25652 zones = rip->i_ndzones; /* # direct zones in the inode */
25653 nr_indirects = rip->i_nindirs;/* # indirect zones per indirect block */
25654
25655 /* Is ’position’ to be found in the inode itself? */
25656 if (zone < zones) {
25657   zindex = (int) zone; /* we need an integer here */
25658   rip->i_zone[zindex] = new_zone;
25659   return(OK);
25660 }
25661
25662 /* It is not in the inode, so it must be single or double indirect. */
25663 excess = zone - zones; /* first Vx_NR_DZONES don’t count */
25664 new_ind = FALSE;
25665 new_dbl = FALSE;
25666
25667 if (excess < nr_indirects) {
25668 /* ’position’ can be located via the single indirect block. */
25669   z1 = rip->i_zone[zones]; /* single indirect zone */
25670   single = TRUE;
25671 } else {
25672   /* ’position’ can be located via the double indirect block. */
25673   if ( (z = rip->i_zone[zones+1]) == NO_ZONE) {
25674   /* Create the double indirect block. */
25675   if ( (z = alloc_zone(rip->i_dev, rip->i_zone[0])) == NO_ZONE)
25676   return(err_code);
25677   rip->i_zone[zones+1] = z;
25678   new_dbl = TRUE; /* set flag for later */
25679   }
25680
25681   /* Either way, ’z’ is zone number for double indirect block. */
25682   excess -= nr_indirects; /* single indirect doesn’t count */
25683   ind_ex = (int) (excess / nr_indirects);
25684   excess = excess % nr_indirects;
25685   if (ind_ex >= nr_indirects) return(EFBIG);
25686   b = (block_t) z << scale;
25687   bp = get_block(rip->i_dev, b, (new_dbl ? NO_READ : NORMAL));
25688   if (new_dbl) zero_block(bp);
25689   z1 = rd_indir(bp, ind_ex);
25690   single = FALSE;
25691 }
25692
25693 /* z1 is now single indirect zone; ’excess’ is index. */
25694 if (z1 == NO_ZONE) {
25695 /* Create indirect block and store zone # in inode or dbl indir blk. */
25696   z1 = alloc_zone(rip->i_dev, rip->i_zone[0]);
25697   if (single)
25698   rip->i_zone[zones] = z1; /* update inode */
25699   else
25700   wr_indir(bp, ind_ex, z1); /* update dbl indir */
25701
25702   new_ind = TRUE;
25703   if (bp != NIL_BUF) bp->b_dirt = DIRTY; /* if double ind, it is dirty*/
25704   if (z1 == NO_ZONE) {
25705   put_block(bp, INDIRECT_BLOCK); /* release dbl indirect blk */
25706   return(err_code); /* couldn’t create single ind */
25707   }
25708 }
25709 put_block(bp, INDIRECT_BLOCK); /* release double indirect blk */
25710
25711 /* z1 is indirect block’s zone number. */
25712 b = (block_t) z1 << scale;
25713 bp = get_block(rip->i_dev, b, (new_ind ? NO_READ : NORMAL) );
25714 if (new_ind) zero_block(bp);
25715 ex = (int) excess; /* we need an int here */
25716 wr_indir(bp, ex, new_zone);
25717 bp->b_dirt = DIRTY;
25718 put_block(bp, INDIRECT_BLOCK);
25719
25720 return(OK);
25721 }
25723 /*===========================================================================*
25724 * wr_indir *
25725 *===========================================================================*/
25726 PRIVATE void wr_indir(bp, index, zone)
25727 struct buf *bp; /* pointer to indirect block */
25728 int index; /* index into *bp */
25729 zone_t zone; /* zone to write */
25730 {
25731 /* Given a pointer to an indirect block, write one entry. */
25732
25733 struct super_block *sp;
25734
MINIX SOURCE CODE File: servers/fs/write.c 985
25735 sp = get_super(bp->b_dev); /* need super block to find file sys type */
25736
25737 /* write a zone into an indirect block */
25738   if (sp->s_version == V1)
25739   bp->b_v1_ind[index] = (zone1_t) conv2(sp->s_native, (int) zone);
25740   else
25741   bp->b_v2_ind[index] = (zone_t) conv4(sp->s_native, (long) zone);
25742 }
25744 /*===========================================================================*
25745 * clear_zone *
25746 *===========================================================================*/
25747 PUBLIC void clear_zone(rip, pos, flag)
25748 register struct inode *rip; /* inode to clear */
25749 off_t pos; /* points to block to clear */
25750 int flag; /* 0 if called by read_write, 1 by new_block */
25751 {
25752 /* Zero a zone, possibly starting in the middle. The parameter ’pos’ gives
25753 * a byte in the first block to be zeroed. Clearzone() is called from
25754 * read_write and new_block().
25755 */
25756
25757 register struct buf *bp;
25758 register block_t b, blo, bhi;
25759 register off_t next;
25760 register int scale;
25761 register zone_t zone_size;
25762
25763 /* If the block size and zone size are the same, clear_zone() not needed. */
25764 scale = rip->i_sp->s_log_zone_size;
25765 if (scale == 0) return;
25766
25767 zone_size = (zone_t) rip->i_sp->s_block_size << scale;
25768 if (flag == 1) pos = (pos/zone_size) * zone_size;
25769 next = pos + rip->i_sp->s_block_size - 1;
25770
25771 /* If ’pos’ is in the last block of a zone, do not clear the zone. */
25772 if (next/zone_size != pos/zone_size) return;
25773 if ( (blo = read_map(rip, next)) == NO_BLOCK) return;
25774 bhi = ( ((blo>>scale)+1) << scale) - 1;
25775
25776 /* Clear all the blocks between ’blo’ and ’bhi’. */
25777 for (b = blo; b <= bhi; b++) {
25778   bp = get_block(rip->i_dev, b, NO_READ);
25779   zero_block(bp);
25780   put_block(bp, FULL_DATA_BLOCK);
25781 }
25782 }
25784 /*===========================================================================*
25785 * new_block *
25786 *===========================================================================*/
25787 PUBLIC struct buf *new_block(rip, position)
25788 register struct inode *rip; /* pointer to inode */
25789 off_t position; /* file pointer */
25790 {
25791 /* Acquire a new block and return a pointer to it. Doing so may require
25792 * allocating a complete zone, and then returning the initial block.
25793 * On the other hand, the current zone may still have some unused blocks.
25794 */
986 File: servers/fs/write.c MINIX SOURCE CODE
25795
25796 register struct buf *bp;
25797 block_t b, base_block;
25798 zone_t z;
25799 zone_t zone_size;
25800 int scale, r;
25801 struct super_block *sp;
25802
25803 /* Is another block available in the current zone? */
25804 if ( (b = read_map(rip, position)) == NO_BLOCK) {
25805 /* Choose first zone if possible. */
25806 /* Lose if the file is nonempty but the first zone number is NO_ZONE
25807 * corresponding to a zone full of zeros. It would be better to
25808 * search near the last real zone.
25809 */
25810   if (rip->i_zone[0] == NO_ZONE) {
25811     sp = rip->i_sp;
25812     z = sp->s_firstdatazone;
25813   } else {
25814     z = rip->i_zone[0]; /* hunt near first zone */
25815   }
25816   if ( (z = alloc_zone(rip->i_dev, z)) == NO_ZONE) return(NIL_BUF);
25817   if ( (r = write_map(rip, position, z)) != OK) {
25818     free_zone(rip->i_dev, z);
25819     err_code = r;
25820     return(NIL_BUF);
25821   }
25822
25823 /* If we are not writing at EOF, clear the zone, just to be safe. */
25824   if ( position != rip->i_size) clear_zone(rip, position, 1);
25825   scale = rip->i_sp->s_log_zone_size;
25826   base_block = (block_t) z << scale;
25827   zone_size = (zone_t) rip->i_sp->s_block_size << scale;
25828   b = base_block + (block_t)((position % zone_size)/rip->i_sp->s_block_size);
25829 }
25830
25831 bp = get_block(rip->i_dev, b, NO_READ);
25832 zero_block(bp);
25833 return(bp);
25834 }
25836 /*===========================================================================*
25837 * zero_block *
25838 *===========================================================================*/
25839 PUBLIC void zero_block(bp)
25840 register struct buf *bp; /* pointer to buffer to zero */
25841 {
25842   /* Zero a block. */
25843   memset(bp->b_data, 0, MAX_BLOCK_SIZE);
25844   bp->b_dirt = DIRTY;
25845 }