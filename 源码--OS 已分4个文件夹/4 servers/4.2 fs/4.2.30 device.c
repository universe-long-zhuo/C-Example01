++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/device.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
28300 /* When a needed block is not in the cache, it must be fetched from the disk.
28301 * Special character files also require I/O. The routines for these are here.
28302 *
28303 * The entry points in this file are:
28304 * dev_open: FS opens a device
28305 * dev_close: FS closes a device
28306 * dev_io: FS does a read or write on a device
28307 * dev_status: FS processes callback request alert
28308 * gen_opcl: generic call to a task to perform an open/close
28309 * gen_io: generic call to a task to perform an I/O operation
28310 * no_dev: open/close processing for devices that don’t exist
28311 * tty_opcl: perform tty-specific processing for open/close
28312 * ctty_opcl: perform controlling-tty-specific processing for open/close
28313 * ctty_io: perform controlling-tty-specific processing for I/O
28314 * do_ioctl: perform the IOCTL system call
28315 * do_setsid: perform the SETSID system call (FS side)
28316 */
28317
28318 #include "fs.h"
28319 #include <fcntl.h>
28320 #include <minix/callnr.h>
28321 #include <minix/com.h>
28322 #include "file.h"
28323 #include "fproc.h"
28324 #include "inode.h"
28325 #include "param.h"
28326
28327 #define ELEMENTS(a) (sizeof(a)/sizeof((a)[0]))
28328
28329 extern int dmap_size;
28330
28331 /*===========================================================================*
28332 * dev_open *
28333 *===========================================================================*/
28334 PUBLIC int dev_open(dev, proc, flags)
28335 dev_t dev; /* device to open */
28336 int proc; /* process to open for */
28337 int flags; /* mode bits and flags */
28338 {
28339 int major, r;
28340 struct dmap *dp;
28341
28342 /* Determine the major device number call the device class specific
28343 * open/close routine. (This is the only routine that must check the
28344 * device number for being in range. All others can trust this check.)
MINIX SOURCE CODE File: servers/fs/device.c 1023
28345 */
28346 major = (dev >> MAJOR) & BYTE;
28347 if (major >= NR_DEVICES) major = 0;
28348 dp = &dmap[major];
28349 r = (*dp->dmap_opcl)(DEV_OPEN, dev, proc, flags);
28350 if (r == SUSPEND) panic(__FILE__,"suspend on open from", dp->dmap_driver);
28351 return(r);
28352 }
28354 /*===========================================================================*
28355 * dev_close *
28356 *===========================================================================*/
28357 PUBLIC void dev_close(dev)
28358 dev_t dev; /* device to close */
28359 {
28360 (void) (*dmap[(dev >> MAJOR) & BYTE].dmap_opcl)(DEV_CLOSE, dev, 0, 0);
28361 }
28363 /*===========================================================================*
28364 * dev_status *
28365 *===========================================================================*/
28366 PUBLIC void dev_status(message *m)
28367 {
28368 message st;
28369 int d, get_more = 1;
28370
28371 for(d = 0; d < NR_DEVICES; d++)
28372 if (dmap[d].dmap_driver == m->m_source)
28373 break;
28374
28375 if (d >= NR_DEVICES)
28376 return;
28377
28378 do {
28379 int r;
28380 st.m_type = DEV_STATUS;
28381 if ((r=sendrec(m->m_source, &st)) != OK)
28382 panic(__FILE__,"couldn’t sendrec for DEV_STATUS", r);
28383
28384 switch(st.m_type) {
28385 case DEV_REVIVE:
28386 revive(st.REP_PROC_NR, st.REP_STATUS);
28387 break;
28388 case DEV_IO_READY:
28389 select_notified(d, st.DEV_MINOR, st.DEV_SEL_OPS);
28390 break;
28391 default:
28392 printf("FS: unrecognized rep %d to DEV_STATUS\n",st.m_type);
28393 /* Fall through. */
28394 case DEV_NO_STATUS:
28395 get_more = 0;
28396 break;
28397 }
28398 } while(get_more);
28399
28400 return;
28401 }
1024 File: servers/fs/device.c MINIX SOURCE CODE
28403 /*===========================================================================*
28404 * dev_io *
28405 *===========================================================================*/
28406 PUBLIC int dev_io(op, dev, proc, buf, pos, bytes, flags)
28407 int op; /* DEV_READ, DEV_WRITE, DEV_IOCTL, etc. */
28408 dev_t dev; /* major-minor device number */
28409 int proc; /* in whose address space is buf? */
28410 void *buf; /* virtual address of the buffer */
28411 off_t pos; /* byte position */
28412 int bytes; /* how many bytes to transfer */
28413 int flags; /* special flags, like O_NONBLOCK */
28414 {
28415 /* Read or write from a device. The parameter ’dev’ tells which one. */
28416 struct dmap *dp;
28417 message dev_mess;
28418
28419 /* Determine task dmap. */
28420 dp = &dmap[(dev >> MAJOR) & BYTE];
28421
28422 /* Set up the message passed to task. */
28423 dev_mess.m_type = op;
28424 dev_mess.DEVICE = (dev >> MINOR) & BYTE;
28425 dev_mess.POSITION = pos;
28426 dev_mess.PROC_NR = proc;
28427 dev_mess.ADDRESS = buf;
28428 dev_mess.COUNT = bytes;
28429 dev_mess.TTY_FLAGS = flags;
28430
28431 /* Call the task. */
28432 (*dp->dmap_io)(dp->dmap_driver, &dev_mess);
28433
28434 /* Task has completed. See if call completed. */
28435 if (dev_mess.REP_STATUS == SUSPEND) {
28436 if (flags & O_NONBLOCK) {
28437 /* Not supposed to block. */
28438 dev_mess.m_type = CANCEL;
28439 dev_mess.PROC_NR = proc;
28440 dev_mess.DEVICE = (dev >> MINOR) & BYTE;
28441 (*dp->dmap_io)(dp->dmap_driver, &dev_mess);
28442 if (dev_mess.REP_STATUS == EINTR) dev_mess.REP_STATUS = EAGAIN;
28443 } else {
28444 /* Suspend user. */
28445 suspend(dp->dmap_driver);
28446 return(SUSPEND);
28447 }
28448 }
28449 return(dev_mess.REP_STATUS);
28450 }
28452 /*===========================================================================*
28453 * gen_opcl *
28454 *===========================================================================*/
28455 PUBLIC int gen_opcl(op, dev, proc, flags)
28456 int op; /* operation, DEV_OPEN or DEV_CLOSE */
28457 dev_t dev; /* device to open or close */
28458 int proc; /* process to open/close for */
28459 int flags; /* mode bits and flags */
28460 {
28461 /* Called from the dmap struct in table.c on opens & closes of special files.*/
28462 struct dmap *dp;
MINIX SOURCE CODE File: servers/fs/device.c 1025
28463 message dev_mess;
28464
28465 /* Determine task dmap. */
28466 dp = &dmap[(dev >> MAJOR) & BYTE];
28467
28468 dev_mess.m_type = op;
28469 dev_mess.DEVICE = (dev >> MINOR) & BYTE;
28470 dev_mess.PROC_NR = proc;
28471 dev_mess.COUNT = flags;
28472
28473 /* Call the task. */
28474 (*dp->dmap_io)(dp->dmap_driver, &dev_mess);
28475
28476 return(dev_mess.REP_STATUS);
28477 }
28479 /*===========================================================================*
28480 * tty_opcl *
28481 *===========================================================================*/
28482 PUBLIC int tty_opcl(op, dev, proc, flags)
28483 int op; /* operation, DEV_OPEN or DEV_CLOSE */
28484 dev_t dev; /* device to open or close */
28485 int proc; /* process to open/close for */
28486 int flags; /* mode bits and flags */
28487 {
28488 /* This procedure is called from the dmap struct on tty open/close. */
28489
28490 int r;
28491 register struct fproc *rfp;
28492
28493 /* Add O_NOCTTY to the flags if this process is not a session leader, or
28494 * if it already has a controlling tty, or if it is someone elses
28495 * controlling tty.
28496 */
28497 if (!fp->fp_sesldr || fp->fp_tty != 0) {
28498 flags |= O_NOCTTY;
28499 } else {
28500 for (rfp = &fproc[0]; rfp < &fproc[NR_PROCS]; rfp++) {
28501 if (rfp->fp_tty == dev) flags |= O_NOCTTY;
28502 }
28503 }
28504
28505 r = gen_opcl(op, dev, proc, flags);
28506
28507 /* Did this call make the tty the controlling tty? */
28508 if (r == 1) {
28509 fp->fp_tty = dev;
28510 r = OK;
28511 }
28512 return(r);
28513 }
28515 /*===========================================================================*
28516 * ctty_opcl *
28517 *===========================================================================*/
28518 PUBLIC int ctty_opcl(op, dev, proc, flags)
28519 int op; /* operation, DEV_OPEN or DEV_CLOSE */
28520 dev_t dev; /* device to open or close */
28521 int proc; /* process to open/close for */
28522 int flags; /* mode bits and flags */
1026 File: servers/fs/device.c MINIX SOURCE CODE
28523 {
28524 /* This procedure is called from the dmap struct in table.c on opening/closing
28525 * /dev/tty, the magic device that translates to the controlling tty.
28526 */
28527
28528 return(fp->fp_tty == 0 ? ENXIO : OK);
28529 }
28531 /*===========================================================================*
28532 * do_setsid *
28533 *===========================================================================*/
28534 PUBLIC int do_setsid()
28535 {
28536 /* Perform the FS side of the SETSID call, i.e. get rid of the controlling
28537 * terminal of a process, and make the process a session leader.
28538 */
28539 register struct fproc *rfp;
28540
28541 /* Only MM may do the SETSID call directly. */
28542 if (who != PM_PROC_NR) return(ENOSYS);
28543
28544 /* Make the process a session leader with no controlling tty. */
28545 rfp = &fproc[m_in.slot1];
28546 rfp->fp_sesldr = TRUE;
28547 rfp->fp_tty = 0;
28548 return(OK);
28549 }
28551 /*===========================================================================*
28552 * do_ioctl *
28553 *===========================================================================*/
28554 PUBLIC int do_ioctl()
28555 {
28556 /* Perform the ioctl(ls_fd, request, argx) system call (uses m2 fmt). */
28557
28558 struct filp *f;
28559 register struct inode *rip;
28560 dev_t dev;
28561
28562 if ( (f = get_filp(m_in.ls_fd)) == NIL_FILP) return(err_code);
28563 rip = f->filp_ino; /* get inode pointer */
28564 if ( (rip->i_mode & I_TYPE) != I_CHAR_SPECIAL
28565 && (rip->i_mode & I_TYPE) != I_BLOCK_SPECIAL) return(ENOTTY);
28566 dev = (dev_t) rip->i_zone[0];
28567
28568 return(dev_io(DEV_IOCTL, dev, who, m_in.ADDRESS, 0L,
28569 m_in.REQUEST, f->filp_flags));
28570 }
28572 /*===========================================================================*
28573 * gen_io *
28574 *===========================================================================*/
28575 PUBLIC void gen_io(task_nr, mess_ptr)
28576 int task_nr; /* which task to call */
28577 message *mess_ptr; /* pointer to message for task */
28578 {
28579 /* All file system I/O ultimately comes down to I/O on major/minor device
28580 * pairs. These lead to calls on the following routines via the dmap table.
28581 */
28582
MINIX SOURCE CODE File: servers/fs/device.c 1027
28583 int r, proc_nr;
28584 message local_m;
28585
28586 proc_nr = mess_ptr->PROC_NR;
28587 if (! isokprocnr(proc_nr)) {
28588 printf("FS: warning, got illegal process number (%d) from %d\n",
28589 mess_ptr->PROC_NR, mess_ptr->m_source);
28590 return;
28591 }
28592
28593 while ((r = sendrec(task_nr, mess_ptr)) == ELOCKED) {
28594 /* sendrec() failed to avoid deadlock. The task ’task_nr’ is
28595 * trying to send a REVIVE message for an earlier request.
28596 * Handle it and go try again.
28597 */
28598 if ((r = receive(task_nr, &local_m)) != OK) {
28599 break;
28600 }
28601
28602 /* If we’re trying to send a cancel message to a task which has just
28603 * sent a completion reply, ignore the reply and abort the cancel
28604 * request. The caller will do the revive for the process.
28605 */
28606 if (mess_ptr->m_type == CANCEL && local_m.REP_PROC_NR == proc_nr) {
28607 return;
28608 }
28609
28610 /* Otherwise it should be a REVIVE. */
28611 if (local_m.m_type != REVIVE) {
28612 printf(
28613 "fs: strange device reply from %d, type = %d, proc = %d (1)\n",
28614 local_m.m_source,
28615 local_m.m_type, local_m.REP_PROC_NR);
28616 continue;
28617 }
28618
28619 revive(local_m.REP_PROC_NR, local_m.REP_STATUS);
28620 }
28621
28622 /* The message received may be a reply to this call, or a REVIVE for some
28623 * other process.
28624 */
28625 for (;;) {
28626 if (r != OK) {
28627 if (r == EDEADDST) return; /* give up */
28628 else panic(__FILE__,"call_task: can’t send/receive", r);
28629 }
28630
28631 /* Did the process we did the sendrec() for get a result? */
28632 if (mess_ptr->REP_PROC_NR == proc_nr) {
28633 break;
28634 } else if (mess_ptr->m_type == REVIVE) {
28635 /* Otherwise it should be a REVIVE. */
28636 revive(mess_ptr->REP_PROC_NR, mess_ptr->REP_STATUS);
28637 } else {
28638 printf(
28639 "fs: strange device reply from %d, type = %d, proc = %d (2)\n",
28640 mess_ptr->m_source,
28641 mess_ptr->m_type, mess_ptr->REP_PROC_NR);
28642 return;
1028 File: servers/fs/device.c MINIX SOURCE CODE
28643 }
28644
28645 r = receive(task_nr, mess_ptr);
28646 }
28647 }
28649 /*===========================================================================*
28650 * ctty_io *
28651 *===========================================================================*/
28652 PUBLIC void ctty_io(task_nr, mess_ptr)
28653 int task_nr; /* not used - for compatibility with dmap_t */
28654 message *mess_ptr; /* pointer to message for task */
28655 {
28656 /* This routine is only called for one device, namely /dev/tty. Its job
28657 * is to change the message to use the controlling terminal, instead of the
28658 * major/minor pair for /dev/tty itself.
28659 */
28660
28661 struct dmap *dp;
28662
28663 if (fp->fp_tty == 0) {
28664 /* No controlling tty present anymore, return an I/O error. */
28665 mess_ptr->REP_STATUS = EIO;
28666 } else {
28667 /* Substitute the controlling terminal device. */
28668 dp = &dmap[(fp->fp_tty >> MAJOR) & BYTE];
28669 mess_ptr->DEVICE = (fp->fp_tty >> MINOR) & BYTE;
28670 (*dp->dmap_io)(dp->dmap_driver, mess_ptr);
28671 }
28672 }
28674 /*===========================================================================*
28675 * no_dev *
28676 *===========================================================================*/
28677 PUBLIC int no_dev(op, dev, proc, flags)
28678 int op; /* operation, DEV_OPEN or DEV_CLOSE */
28679 dev_t dev; /* device to open or close */
28680 int proc; /* process to open/close for */
28681 int flags; /* mode bits and flags */
28682 {
28683 /* Called when opening a nonexistent device. */
28684
28685 return(ENODEV);
28686 }
28688 /*===========================================================================*
28689 * clone_opcl *
28690 *===========================================================================*/
28691 PUBLIC int clone_opcl(op, dev, proc, flags)
28692 int op; /* operation, DEV_OPEN or DEV_CLOSE */
28693 dev_t dev; /* device to open or close */
28694 int proc; /* process to open/close for */
28695 int flags; /* mode bits and flags */
28696 {
28697 /* Some devices need special processing upon open. Such a device is "cloned",
28698 * i.e. on a succesful open it is replaced by a new device with a new unique
28699 * minor device number. This new device number identifies a new object (such
28700 * as a new network connection) that has been allocated within a task.
28701 */
28702 struct dmap *dp;
MINIX SOURCE CODE File: servers/fs/device.c 1029
28703 int minor;
28704 message dev_mess;
28705
28706 /* Determine task dmap. */
28707 dp = &dmap[(dev >> MAJOR) & BYTE];
28708 minor = (dev >> MINOR) & BYTE;
28709
28710 dev_mess.m_type = op;
28711 dev_mess.DEVICE = minor;
28712 dev_mess.PROC_NR = proc;
28713 dev_mess.COUNT = flags;
28714
28715 /* Call the task. */
28716 (*dp->dmap_io)(dp->dmap_driver, &dev_mess);
28717
28718 if (op == DEV_OPEN && dev_mess.REP_STATUS >= 0) {
28719 if (dev_mess.REP_STATUS != minor) {
28720 /* A new minor device number has been returned. Create a
28721 * temporary device file to hold it.
28722 */
28723 struct inode *ip;
28724
28725 /* Device number of the new device. */
28726 dev = (dev & ˜(BYTE << MINOR)) | (dev_mess.REP_STATUS << MINOR);
28727
28728 ip = alloc_inode(root_dev, ALL_MODES | I_CHAR_SPECIAL);
28729 if (ip == NIL_INODE) {
28730 /* Oops, that didn’t work. Undo open. */
28731 (void) clone_opcl(DEV_CLOSE, dev, proc, 0);
28732 return(err_code);
28733 }
28734 ip->i_zone[0] = dev;
28735
28736 put_inode(fp->fp_filp[m_in.fd]->filp_ino);
28737 fp->fp_filp[m_in.fd]->filp_ino = ip;
28738 }
28739 dev_mess.REP_STATUS = OK;
28740 }
28741 return(dev_mess.REP_STATUS);
28742 }