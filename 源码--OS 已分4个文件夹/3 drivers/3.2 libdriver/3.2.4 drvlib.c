++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             drivers/libdriver/drvlib.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
11400 /* IBM device driver utility functions. Author: Kees J. Bot
11401 * 7 Dec 1995
11402 * Entry point:
11403 * partition: partition a disk to the partition table(s) on it.
11404 */
11405
11406 #include "driver.h"
11407 #include "drvlib.h"
11408 #include <unistd.h>
11409
11410 /* Extended partition? */
11411 #define ext_part(s) ((s) == 0x05 || (s) == 0x0F)
11412
11413 FORWARD _PROTOTYPE( void extpartition, (struct driver *dp, int extdev,
11414 unsigned long extbase) );
11415 FORWARD _PROTOTYPE( int get_part_table, (struct driver *dp, int device,
11416 unsigned long offset, struct part_entry *table));
11417 FORWARD _PROTOTYPE( void sort, (struct part_entry *table) );
11418
11419 #ifndef CD_SECTOR_SIZE
MINIX SOURCE CODE File: drivers/libdriver/drvlib.c 779
11420 #define CD_SECTOR_SIZE 2048
11421 #endif
11422
11423 /*============================================================================*
11424 * partition *
11425 *============================================================================*/
11426 PUBLIC void partition(dp, device, style, atapi)
11427 struct driver *dp; /* device dependent entry points */
11428 int device; /* device to partition */
11429 int style; /* partitioning style: floppy, primary, sub. */
11430 int atapi; /* atapi device */
11431 {
11432 /* This routine is called on first open to initialize the partition tables
11433 * of a device. It makes sure that each partition falls safely within the
11434 * device’s limits. Depending on the partition style we are either making
11435 * floppy partitions, primary partitions or subpartitions. Only primary
11436 * partitions are sorted, because they are shared with other operating
11437 * systems that expect this.
11438 */
11439 struct part_entry table[NR_PARTITIONS], *pe;
11440 int disk, par;
11441 struct device *dv;
11442 unsigned long base, limit, part_limit;
11443
11444 /* Get the geometry of the device to partition */
11445 if ((dv = (*dp->dr_prepare)(device)) == NIL_DEV
11446 || cmp64u(dv->dv_size, 0) == 0) return;
11447 base = div64u(dv->dv_base, SECTOR_SIZE);
11448 limit = base + div64u(dv->dv_size, SECTOR_SIZE);
11449
11450 /* Read the partition table for the device. */
11451 if(!get_part_table(dp, device, 0L, table)) {
11452 return;
11453 }
11454
11455 /* Compute the device number of the first partition. */
11456 switch (style) {
11457 case P_FLOPPY:
11458 device += MINOR_fd0p0;
11459 break;
11460 case P_PRIMARY:
11461 sort(table); /* sort a primary partition table */
11462 device += 1;
11463 break;
11464 case P_SUB:
11465 disk = device / DEV_PER_DRIVE;
11466 par = device % DEV_PER_DRIVE - 1;
11467 device = MINOR_d0p0s0 + (disk * NR_PARTITIONS + par) * NR_PARTITIONS;
11468 }
11469
11470 /* Find an array of devices. */
11471 if ((dv = (*dp->dr_prepare)(device)) == NIL_DEV) return;
11472
11473 /* Set the geometry of the partitions from the partition table. */
11474 for (par = 0; par < NR_PARTITIONS; par++, dv++) {
11475 /* Shrink the partition to fit within the device. */
11476 pe = &table[par];
11477 part_limit = pe->lowsec + pe->size;
11478 if (part_limit < pe->lowsec) part_limit = limit;
11479 if (part_limit > limit) part_limit = limit;
780 File: drivers/libdriver/drvlib.c MINIX SOURCE CODE
11480 if (pe->lowsec < base) pe->lowsec = base;
11481 if (part_limit < pe->lowsec) part_limit = pe->lowsec;
11482
11483 dv->dv_base = mul64u(pe->lowsec, SECTOR_SIZE);
11484 dv->dv_size = mul64u(part_limit - pe->lowsec, SECTOR_SIZE);
11485
11486 if (style == P_PRIMARY) {
11487 /* Each Minix primary partition can be subpartitioned. */
11488 if (pe->sysind == MINIX_PART)
11489 partition(dp, device + par, P_SUB, atapi);
11490
11491 /* An extended partition has logical partitions. */
11492 if (ext_part(pe->sysind))
11493 extpartition(dp, device + par, pe->lowsec);
11494 }
11495 }
11496 }
11498 /*============================================================================*
11499 * extpartition *
11500 *============================================================================*/
11501 PRIVATE void extpartition(dp, extdev, extbase)
11502 struct driver *dp; /* device dependent entry points */
11503 int extdev; /* extended partition to scan */
11504 unsigned long extbase; /* sector offset of the base extended partition */
11505 {
11506 /* Extended partitions cannot be ignored alas, because people like to move
11507 * files to and from DOS partitions. Avoid reading this code, it’s no fun.
11508 */
11509 struct part_entry table[NR_PARTITIONS], *pe;
11510 int subdev, disk, par;
11511 struct device *dv;
11512 unsigned long offset, nextoffset;
11513
11514 disk = extdev / DEV_PER_DRIVE;
11515 par = extdev % DEV_PER_DRIVE - 1;
11516 subdev = MINOR_d0p0s0 + (disk * NR_PARTITIONS + par) * NR_PARTITIONS;
11517
11518 offset = 0;
11519 do {
11520 if (!get_part_table(dp, extdev, offset, table)) return;
11521 sort(table);
11522
11523 /* The table should contain one logical partition and optionally
11524 * another extended partition. (It’s a linked list.)
11525 */
11526 nextoffset = 0;
11527 for (par = 0; par < NR_PARTITIONS; par++) {
11528 pe = &table[par];
11529 if (ext_part(pe->sysind)) {
11530 nextoffset = pe->lowsec;
11531 } else
11532 if (pe->sysind != NO_PART) {
11533 if ((dv = (*dp->dr_prepare)(subdev)) == NIL_DEV) return;
11534
11535 dv->dv_base = mul64u(extbase + offset + pe->lowsec,
11536 SECTOR_SIZE);
11537 dv->dv_size = mul64u(pe->size, SECTOR_SIZE);
11538
11539 /* Out of devices? */
MINIX SOURCE CODE File: drivers/libdriver/drvlib.c 781
11540 if (++subdev % NR_PARTITIONS == 0) return;
11541 }
11542 }
11543 } while ((offset = nextoffset) != 0);
11544 }
11546 /*============================================================================*
11547 * get_part_table *
11548 *============================================================================*/
11549 PRIVATE int get_part_table(dp, device, offset, table)
11550 struct driver *dp;
11551 int device;
11552 unsigned long offset; /* sector offset to the table */
11553 struct part_entry *table; /* four entries */
11554 {
11555 /* Read the partition table for the device, return true iff there were no
11556 * errors.
11557 */
11558 iovec_t iovec1;
11559 off_t position;
11560 static unsigned char partbuf[CD_SECTOR_SIZE];
11561
11562 position = offset << SECTOR_SHIFT;
11563 iovec1.iov_addr = (vir_bytes) partbuf;
11564 iovec1.iov_size = CD_SECTOR_SIZE;
11565 if ((*dp->dr_prepare)(device) != NIL_DEV) {
11566 (void) (*dp->dr_transfer)(SELF, DEV_GATHER, position, &iovec1, 1);
11567 }
11568 if (iovec1.iov_size != 0) {
11569 return 0;
11570 }
11571 if (partbuf[510] != 0x55 || partbuf[511] != 0xAA) {
11572 /* Invalid partition table. */
11573 return 0;
11574 }
11575 memcpy(table, (partbuf + PART_TABLE_OFF), NR_PARTITIONS * sizeof(table[0]));
11576 return 1;
11577 }
11579 /*===========================================================================*
11580 * sort *
11581 *===========================================================================*/
11582 PRIVATE void sort(table)
11583 struct part_entry *table;
11584 {
11585 /* Sort a partition table. */
11586 struct part_entry *pe, tmp;
11587 int n = NR_PARTITIONS;
11588
11589 do {
11590 for (pe = table; pe < table + NR_PARTITIONS-1; pe++) {
11591 if (pe[0].sysind == NO_PART
11592 || (pe[0].lowsec > pe[1].lowsec
11593 && pe[1].sysind != NO_PART)) {
11594 tmp = pe[0]; pe[0] = pe[1]; pe[1] = tmp;
11595 }
11596 }
11597 } while (--n > 0);
11598 }