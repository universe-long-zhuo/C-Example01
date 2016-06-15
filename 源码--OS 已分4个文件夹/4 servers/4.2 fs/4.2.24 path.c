++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/path.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
26300 /* This file contains the procedures that look up path names in the directory
26301 * system and determine the inode number that goes with a given path name.
26302 *
26303 * The entry points into this file are
26304 * eat_path: the ’main’ routine of the path-to-inode conversion mechanism
26305 * last_dir: find the final directory on a given path
26306 * advance: parse one component of a path name
26307 * search_dir: search a directory for a string and return its inode number
26308 */
26309
26310 #include "fs.h"
26311 #include <string.h>
26312 #include <minix/callnr.h>
26313 #include "buf.h"
26314 #include "file.h"
26315 #include "fproc.h"
26316 #include "inode.h"
26317 #include "super.h"
26318
26319 PUBLIC char dot1[2] = "."; /* used for search_dir to bypass the access */
994 File: servers/fs/path.c MINIX SOURCE CODE
26320 PUBLIC char dot2[3] = ".."; /* permissions for . and .. */
26321
26322 FORWARD _PROTOTYPE( char *get_name, (char *old_name, char string [NAME_MAX]) );
26323
26324 /*===========================================================================*
26325 * eat_path *
26326 *===========================================================================*/
26327 PUBLIC struct inode *eat_path(path)
26328 char *path; /* the path name to be parsed */
26329 {
26330 /* Parse the path ’path’ and put its inode in the inode table. If not possible,
26331 * return NIL_INODE as function value and an error code in ’err_code’.
26332 */
26333
26334 register struct inode *ldip, *rip;
26335 char string[NAME_MAX]; /* hold 1 path component name here */
26336
26337 /* First open the path down to the final directory. */
26338 if ( (ldip = last_dir(path, string)) == NIL_INODE) {
26339 return(NIL_INODE); /* we couldn’t open final directory */
26340 }
26341
26342 /* The path consisting only of "/" is a special case, check for it. */
26343 if (string[0] == ’\0’) return(ldip);
26344
26345 /* Get final component of the path. */
26346 rip = advance(ldip, string);
26347 put_inode(ldip);
26348 return(rip);
26349 }
26351 /*===========================================================================*
26352 * last_dir *
26353 *===========================================================================*/
26354 PUBLIC struct inode *last_dir(path, string)
26355 char *path; /* the path name to be parsed */
26356 char string[NAME_MAX]; /* the final component is returned here */
26357 {
26358 /* Given a path, ’path’, located in the fs address space, parse it as
26359 * far as the last directory, fetch the inode for the last directory into
26360 * the inode table, and return a pointer to the inode. In
26361 * addition, return the final component of the path in ’string’.
26362 * If the last directory can’t be opened, return NIL_INODE and
26363 * the reason for failure in ’err_code’.
26364 */
26365
26366 register struct inode *rip;
26367 register char *new_name;
26368 register struct inode *new_ip;
26369
26370 /* Is the path absolute or relative? Initialize ’rip’ accordingly. */
26371 rip = (*path == ’/’ ? fp->fp_rootdir : fp->fp_workdir);
26372
26373 /* If dir has been removed or path is empty, return ENOENT. */
26374 if (rip->i_nlinks == 0 || *path == ’\0’) {
26375 err_code = ENOENT;
26376 return(NIL_INODE);
26377 }
26378
26379 dup_inode(rip); /* inode will be returned with put_inode */
MINIX SOURCE CODE File: servers/fs/path.c 995
26380
26381 /* Scan the path component by component. */
26382 while (TRUE) {
26383 /* Extract one component. */
26384 if ( (new_name = get_name(path, string)) == (char*) 0) {
26385 put_inode(rip); /* bad path in user space */
26386 return(NIL_INODE);
26387 }
26388 if (*new_name == ’\0’) {
26389 if ( (rip->i_mode & I_TYPE) == I_DIRECTORY) {
26390 return(rip); /* normal exit */
26391 } else {
26392 /* last file of path prefix is not a directory */
26393 put_inode(rip);
26394 err_code = ENOTDIR;
26395 return(NIL_INODE);
26396 }
26397 }
26398
26399 /* There is more path. Keep parsing. */
26400 new_ip = advance(rip, string);
26401 put_inode(rip); /* rip either obsolete or irrelevant */
26402 if (new_ip == NIL_INODE) return(NIL_INODE);
26403
26404 /* The call to advance() succeeded. Fetch next component. */
26405 path = new_name;
26406 rip = new_ip;
26407 }
26408 }
26410 /*===========================================================================*
26411 * get_name *
26412 *===========================================================================*/
26413 PRIVATE char *get_name(old_name, string)
26414 char *old_name; /* path name to parse */
26415 char string[NAME_MAX]; /* component extracted from ’old_name’ */
26416 {
26417 /* Given a pointer to a path name in fs space, ’old_name’, copy the next
26418 * component to ’string’ and pad with zeros. A pointer to that part of
26419 * the name as yet unparsed is returned. Roughly speaking,
26420 * ’get_name’ = ’old_name’ - ’string’.
26421 *
26422 * This routine follows the standard convention that /usr/ast, /usr//ast,
26423 * //usr///ast and /usr/ast/ are all equivalent.
26424 */
26425
26426 register int c;
26427 register char *np, *rnp;
26428
26429 np = string; /* ’np’ points to current position */
26430 rnp = old_name; /* ’rnp’ points to unparsed string */
26431 while ( (c = *rnp) == ’/’) rnp++; /* skip leading slashes */
26432
26433 /* Copy the unparsed path, ’old_name’, to the array, ’string’. */
26434 while ( rnp < &old_name[PATH_MAX] && c != ’/’ && c != ’\0’) {
26435 if (np < &string[NAME_MAX]) *np++ = c;
26436 c = *++rnp; /* advance to next character */
26437 }
26438
26439 /* To make /usr/ast/ equivalent to /usr/ast, skip trailing slashes. */
996 File: servers/fs/path.c MINIX SOURCE CODE
26440 while (c == ’/’ && rnp < &old_name[PATH_MAX]) c = *++rnp;
26441
26442 if (np < &string[NAME_MAX]) *np = ’\0’; /* Terminate string */
26443
26444 if (rnp >= &old_name[PATH_MAX]) {
26445 err_code = ENAMETOOLONG;
26446 return((char *) 0);
26447 }
26448 return(rnp);
26449 }
26451 /*===========================================================================*
26452 * advance *
26453 *===========================================================================*/
26454 PUBLIC struct inode *advance(dirp, string)
26455 struct inode *dirp; /* inode for directory to be searched */
26456 char string[NAME_MAX]; /* component name to look for */
26457 {
26458 /* Given a directory and a component of a path, look up the component in
26459 * the directory, find the inode, open it, and return a pointer to its inode
26460 * slot. If it can’t be done, return NIL_INODE.
26461 */
26462
26463 register struct inode *rip;
26464 struct inode *rip2;
26465 register struct super_block *sp;
26466 int r, inumb;
26467 dev_t mnt_dev;
26468 ino_t numb;
26469
26470 /* If ’string’ is empty, yield same inode straight away. */
26471 if (string[0] == ’\0’) { return(get_inode(dirp->i_dev, (int) dirp->i_num)); }
26472
26473 /* Check for NIL_INODE. */
26474 if (dirp == NIL_INODE) { return(NIL_INODE); }
26475
26476 /* If ’string’ is not present in the directory, signal error. */
26477 if ( (r = search_dir(dirp, string, &numb, LOOK_UP)) != OK) {
26478 err_code = r;
26479 return(NIL_INODE);
26480 }
26481
26482 /* Don’t go beyond the current root directory, unless the string is dot2. */
26483 if (dirp == fp->fp_rootdir && strcmp(string, "..") == 0 && string != dot2)
26484 return(get_inode(dirp->i_dev, (int) dirp->i_num));
26485
26486 /* The component has been found in the directory. Get inode. */
26487 if ( (rip = get_inode(dirp->i_dev, (int) numb)) == NIL_INODE) {
26488 return(NIL_INODE);
26489 }
26490
26491 if (rip->i_num == ROOT_INODE)
26492 if (dirp->i_num == ROOT_INODE) {
26493 if (string[1] == ’.’) {
26494 for (sp = &super_block[1]; sp < &super_block[NR_SUPERS]; sp++){
26495 if (sp->s_dev == rip->i_dev) {
26496 /* Release the root inode. Replace by the
26497 * inode mounted on.
26498 */
26499 put_inode(rip);
MINIX SOURCE CODE File: servers/fs/path.c 997
26500 mnt_dev = sp->s_imount->i_dev;
26501 inumb = (int) sp->s_imount->i_num;
26502 rip2 = get_inode(mnt_dev, inumb);
26503 rip = advance(rip2, string);
26504 put_inode(rip2);
26505 break;
26506 }
26507 }
26508 }
26509 }
26510 if (rip == NIL_INODE) return(NIL_INODE);
26511
26512 /* See if the inode is mounted on. If so, switch to root directory of the
26513 * mounted file system. The super_block provides the linkage between the
26514 * inode mounted on and the root directory of the mounted file system.
26515 */
26516 while (rip != NIL_INODE && rip->i_mount == I_MOUNT) {
26517 /* The inode is indeed mounted on. */
26518 for (sp = &super_block[0]; sp < &super_block[NR_SUPERS]; sp++) {
26519 if (sp->s_imount == rip) {
26520 /* Release the inode mounted on. Replace by the
26521 * inode of the root inode of the mounted device.
26522 */
26523 put_inode(rip);
26524 rip = get_inode(sp->s_dev, ROOT_INODE);
26525 break;
26526 }
26527 }
26528 }
26529 return(rip); /* return pointer to inode’s component */
26530 }
26532 /*===========================================================================*
26533 * search_dir *
26534 *===========================================================================*/
26535 PUBLIC int search_dir(ldir_ptr, string, numb, flag)
26536 register struct inode *ldir_ptr; /* ptr to inode for dir to search */
26537 char string[NAME_MAX]; /* component to search for */
26538 ino_t *numb; /* pointer to inode number */
26539 int flag; /* LOOK_UP, ENTER, DELETE or IS_EMPTY */
26540 {
26541 /* This function searches the directory whose inode is pointed to by ’ldip’:
26542 * if (flag == ENTER) enter ’string’ in the directory with inode # ’*numb’;
26543 * if (flag == DELETE) delete ’string’ from the directory;
26544 * if (flag == LOOK_UP) search for ’string’ and return inode # in ’numb’;
26545 * if (flag == IS_EMPTY) return OK if only . and .. in dir else ENOTEMPTY;
26546 *
26547 * if ’string’ is dot1 or dot2, no access permissions are checked.
26548 */
26549
26550 register struct direct *dp = NULL;
26551 register struct buf *bp = NULL;
26552 int i, r, e_hit, t, match;
26553 mode_t bits;
26554 off_t pos;
26555 unsigned new_slots, old_slots;
26556 block_t b;
26557 struct super_block *sp;
26558 int extended = 0;
26559
998 File: servers/fs/path.c MINIX SOURCE CODE
26560 /* If ’ldir_ptr’ is not a pointer to a dir inode, error. */
26561 if ( (ldir_ptr->i_mode & I_TYPE) != I_DIRECTORY) return(ENOTDIR);
26562
26563 r = OK;
26564
26565 if (flag != IS_EMPTY) {
26566 bits = (flag == LOOK_UP ? X_BIT : W_BIT | X_BIT);
26567
26568 if (string == dot1 || string == dot2) {
26569 if (flag != LOOK_UP) r = read_only(ldir_ptr);
26570 /* only a writable device is required. */
26571 }
26572 else r = forbidden(ldir_ptr, bits); /* check access permissions */
26573 }
26574 if (r != OK) return(r);
26575
26576 /* Step through the directory one block at a time. */
26577 old_slots = (unsigned) (ldir_ptr->i_size/DIR_ENTRY_SIZE);
26578 new_slots = 0;
26579 e_hit = FALSE;
26580 match = 0; /* set when a string match occurs */
26581
26582 for (pos = 0; pos < ldir_ptr->i_size; pos += ldir_ptr->i_sp->s_block_size) {
26583 b = read_map(ldir_ptr, pos); /* get block number */
26584
26585 /* Since directories don’t have holes, ’b’ cannot be NO_BLOCK. */
26586 bp = get_block(ldir_ptr->i_dev, b, NORMAL); /* get a dir block */
26587
26588 if (bp == NO_BLOCK)
26589 panic(__FILE__,"get_block returned NO_BLOCK", NO_NUM);
26590
26591 /* Search a directory block. */
26592 for (dp = &bp->b_dir[0];
26593 dp < &bp->b_dir[NR_DIR_ENTRIES(ldir_ptr->i_sp->s_block_size)];
26594 dp++) {
26595 if (++new_slots > old_slots) { /* not found, but room left */
26596 if (flag == ENTER) e_hit = TRUE;
26597 break;
26598 }
26599
26600 /* Match occurs if string found. */
26601 if (flag != ENTER && dp->d_ino != 0) {
26602 if (flag == IS_EMPTY) {
26603 /* If this test succeeds, dir is not empty. */
26604 if (strcmp(dp->d_name, "." ) != 0 &&
26605 strcmp(dp->d_name, "..") != 0) match = 1;
26606 } else {
26607 if (strncmp(dp->d_name, string, NAME_MAX) == 0) {
26608 match = 1;
26609 }
26610 }
26611 }
26612
26613 if (match) {
26614 /* LOOK_UP or DELETE found what it wanted. */
26615 r = OK;
26616 if (flag == IS_EMPTY) r = ENOTEMPTY;
26617 else if (flag == DELETE) {
26618 /* Save d_ino for recovery. */
26619 t = NAME_MAX - sizeof(ino_t);
MINIX SOURCE CODE File: servers/fs/path.c 999
26620 *((ino_t *) &dp->d_name[t]) = dp->d_ino;
26621 dp->d_ino = 0; /* erase entry */
26622 bp->b_dirt = DIRTY;
26623 ldir_ptr->i_update |= CTIME | MTIME;
26624 ldir_ptr->i_dirt = DIRTY;
26625 } else {
26626 sp = ldir_ptr->i_sp; /* ’flag’ is LOOK_UP */
26627 *numb = conv4(sp->s_native, (int) dp->d_ino);
26628 }
26629 put_block(bp, DIRECTORY_BLOCK);
26630 return(r);
26631 }
26632
26633 /* Check for free slot for the benefit of ENTER. */
26634 if (flag == ENTER && dp->d_ino == 0) {
26635 e_hit = TRUE; /* we found a free slot */
26636 break;
26637 }
26638 }
26639
26640 /* The whole block has been searched or ENTER has a free slot. */
26641 if (e_hit) break; /* e_hit set if ENTER can be performed now */
26642 put_block(bp, DIRECTORY_BLOCK); /* otherwise, continue searching dir */
26643 }
26644
26645 /* The whole directory has now been searched. */
26646 if (flag != ENTER) {
26647 return(flag == IS_EMPTY ? OK : ENOENT);
26648 }
26649
26650 /* This call is for ENTER. If no free slot has been found so far, try to
26651 * extend directory.
26652 */
26653 if (e_hit == FALSE) { /* directory is full and no room left in last block */
26654 new_slots++; /* increase directory size by 1 entry */
26655 if (new_slots == 0) return(EFBIG); /* dir size limited by slot count */
26656 if ( (bp = new_block(ldir_ptr, ldir_ptr->i_size)) == NIL_BUF)
26657 return(err_code);
26658 dp = &bp->b_dir[0];
26659 extended = 1;
26660 }
26661
26662 /* ’bp’ now points to a directory block with space. ’dp’ points to slot. */
26663 (void) memset(dp->d_name, 0, (size_t) NAME_MAX); /* clear entry */
26664 for (i = 0; string[i] && i < NAME_MAX; i++) dp->d_name[i] = string[i];
26665 sp = ldir_ptr->i_sp;
26666 dp->d_ino = conv4(sp->s_native, (int) *numb);
26667 bp->b_dirt = DIRTY;
26668 put_block(bp, DIRECTORY_BLOCK);
26669 ldir_ptr->i_update |= CTIME | MTIME; /* mark mtime for update later */
26670 ldir_ptr->i_dirt = DIRTY;
26671 if (new_slots > old_slots) {
26672 ldir_ptr->i_size = (off_t) new_slots * DIR_ENTRY_SIZE;
26673 /* Send the change to disk if the directory is extended. */
26674 if (extended) rw_inode(ldir_ptr, WRITING);
26675 }
26676 return(OK);
26677 }