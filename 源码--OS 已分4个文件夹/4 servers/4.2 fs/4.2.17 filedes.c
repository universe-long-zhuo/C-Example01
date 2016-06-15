++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/filedes.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
23700 /* This file contains the procedures that manipulate file descriptors.
23701 *
23702 * The entry points into this file are
23703 * get_fd: look for free file descriptor and free filp slots
23704 * get_filp: look up the filp entry for a given file descriptor
23705 * find_filp: find a filp slot that points to a given inode
23706 */
23707
23708 #include "fs.h"
23709 #include "file.h"
23710 #include "fproc.h"
23711 #include "inode.h"
23712
23713 /*===========================================================================*
23714 * get_fd *
23715 *===========================================================================*/
23716 PUBLIC int get_fd(int start, mode_t bits, int *k, struct filp **fpt)
23717 {
23718 /* Look for a free file descriptor and a free filp slot. Fill in the mode word
23719 * in the latter, but don’t claim either one yet, since the open() or creat()
23720 * may yet fail.
23721 */
23722
23723 register struct filp *f;
23724 register int i;
MINIX SOURCE CODE File: servers/fs/filedes.c 953
23725
23726 *k = -1; /* we need a way to tell if file desc found */
23727
23728 /* Search the fproc fp_filp table for a free file descriptor. */
23729 for (i = start; i < OPEN_MAX; i++) {
23730 if (fp->fp_filp[i] == NIL_FILP) {
23731 /* A file descriptor has been located. */
23732 *k = i;
23733 break;
23734 }
23735 }
23736
23737 /* Check to see if a file descriptor has been found. */
23738 if (*k < 0) return(EMFILE); /* this is why we initialized k to -1 */
23739
23740 /* Now that a file descriptor has been found, look for a free filp slot. */
23741 for (f = &filp[0]; f < &filp[NR_FILPS]; f++) {
23742 if (f->filp_count == 0) {
23743 f->filp_mode = bits;
23744 f->filp_pos = 0L;
23745 f->filp_selectors = 0;
23746 f->filp_select_ops = 0;
23747 f->filp_pipe_select_ops = 0;
23748 f->filp_flags = 0;
23749 *fpt = f;
23750 return(OK);
23751 }
23752 }
23753
23754 /* If control passes here, the filp table must be full. Report that back. */
23755 return(ENFILE);
23756 }
23758 /*===========================================================================*
23759 * get_filp *
23760 *===========================================================================*/
23761 PUBLIC struct filp *get_filp(fild)
23762 int fild; /* file descriptor */
23763 {
23764 /* See if ’fild’ refers to a valid file descr. If so, return its filp ptr. */
23765
23766 err_code = EBADF;
23767 if (fild < 0 || fild >= OPEN_MAX ) return(NIL_FILP);
23768 return(fp->fp_filp[fild]); /* may also be NIL_FILP */
23769 }
23771 /*===========================================================================*
23772 * find_filp *
23773 *===========================================================================*/
23774 PUBLIC struct filp *find_filp(register struct inode *rip, mode_t bits)
23775 {
23776 /* Find a filp slot that refers to the inode ’rip’ in a way as described
23777 * by the mode bit ’bits’. Used for determining whether somebody is still
23778 * interested in either end of a pipe. Also used when opening a FIFO to
23779 * find partners to share a filp field with (to shared the file position).
23780 * Like ’get_fd’ it performs its job by linear search through the filp table.
23781 */
23782
23783 register struct filp *f;
23784
954 File: servers/fs/filedes.c MINIX SOURCE CODE
23785 for (f = &filp[0]; f < &filp[NR_FILPS]; f++) {
23786 if (f->filp_count != 0 && f->filp_ino == rip && (f->filp_mode & bits)){
23787 return(f);
23788 }
23789 }
23790
23791 /* If control passes here, the filp wasn’t there. Report that back. */
23792 return(NIL_FILP);
23793 }