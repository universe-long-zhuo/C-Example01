++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/fproc.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21500 /* This is the per-process information. A slot is reserved for each potential
21501 * process. Thus NR_PROCS must be the same as in the kernel. It is not
21502 * possible or even necessary to tell when a slot is free here.
21503 */
21504 EXTERN struct fproc {
21505 mode_t fp_umask; /* mask set by umask system call */
21506 struct inode *fp_workdir; /* pointer to working directory’s inode */
21507 struct inode *fp_rootdir; /* pointer to current root dir (see chroot) */
21508 struct filp *fp_filp[OPEN_MAX];/* the file descriptor table */
21509 uid_t fp_realuid; /* real user id */
21510 uid_t fp_effuid; /* effective user id */
21511 gid_t fp_realgid; /* real group id */
21512 gid_t fp_effgid; /* effective group id */
21513 dev_t fp_tty; /* major/minor of controlling tty */
21514 int fp_fd; /* place to save fd if rd/wr can’t finish */
926 File: servers/fs/fproc.h MINIX SOURCE CODE
21515 char *fp_buffer; /* place to save buffer if rd/wr can’t finish*/
21516 int fp_nbytes; /* place to save bytes if rd/wr can’t finish */
21517 int fp_cum_io_partial; /* partial byte count if rd/wr can’t finish */
21518 char fp_suspended; /* set to indicate process hanging */
21519 char fp_revived; /* set to indicate process being revived */
21520 char fp_task; /* which task is proc suspended on */
21521 char fp_sesldr; /* true if proc is a session leader */
21522 pid_t fp_pid; /* process id */
21523 long fp_cloexec; /* bit map for POSIX Table 6-2 FD_CLOEXEC */
21524 } fproc[NR_PROCS];
21525
21526 /* Field values. */
21527 #define NOT_SUSPENDED 0 /* process is not suspended on pipe or task */
21528 #define SUSPENDED 1 /* process is suspended on pipe or task */
21529 #define NOT_REVIVING 0 /* process is not being revived */
21530 #define REVIVING 1 /* process is being revived from suspension */
21531 #define PID_FREE 0 /* process slot free */
21532
21533 /* Check is process number is acceptable - includes system processes. */
21534 #define isokprocnr(n) ((unsigned)((n)+NR_TASKS) < NR_PROCS + NR_TASKS)
21535