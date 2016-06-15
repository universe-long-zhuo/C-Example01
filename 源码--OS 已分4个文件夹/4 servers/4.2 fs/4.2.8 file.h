++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/file.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21700 /* This is the filp table. It is an intermediary between file descriptors and
21701 * inodes. A slot is free if filp_count == 0.
21702 */
21703
21704 EXTERN struct filp {
21705 mode_t filp_mode; /* RW bits, telling how file is opened */
21706 int filp_flags; /* flags from open and fcntl */
21707 int filp_count; /* how many file descriptors share this slot?*/
21708 struct inode *filp_ino; /* pointer to the inode */
21709 off_t filp_pos; /* file position */
928 File: servers/fs/file.h MINIX SOURCE CODE
21710
21711 /* the following fields are for select() and are owned by the generic
21712 * select() code (i.e., fd-type-specific select() code can’t touch these).
21713 */
21714 int filp_selectors; /* select()ing processes blocking on this fd */
21715 int filp_select_ops; /* interested in these SEL_* operations */
21716
21717 /* following are for fd-type-specific select() */
21718 int filp_pipe_select_ops;
21719 } filp[NR_FILPS];
21720
21721 #define FILP_CLOSED 0 /* filp_mode: associated device closed */
21722
21723 #define NIL_FILP (struct filp *) 0 /* indicates absence of a filp slot */