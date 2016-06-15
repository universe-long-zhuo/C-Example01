++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/pm/mproc.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
17600 /* This table has one slot per process. It contains all the process management
17601 * information for each process. Among other things, it defines the text, data
17602 * and stack segments, uids and gids, and various flags. The kernel and file
17603 * systems have tables that are also indexed by process, with the contents
17604 * of corresponding slots referring to the same process in all three.
17605 */
17606 #include <timers.h>
17607
17608 EXTERN struct mproc {
17609 struct mem_map mp_seg[NR_LOCAL_SEGS]; /* points to text, data, stack */
17610 char mp_exitstatus; /* storage for status when process exits */
17611 char mp_sigstatus; /* storage for signal # for killed procs */
17612 pid_t mp_pid; /* process id */
17613 pid_t mp_procgrp; /* pid of process group (used for signals) */
17614 pid_t mp_wpid; /* pid this process is waiting for */
17615 int mp_parent; /* index of parent process */
17616
17617 /* Child user and system times. Accounting done on child exit. */
17618 clock_t mp_child_utime; /* cumulative user time of children */
17619 clock_t mp_child_stime; /* cumulative sys time of children */
17620
17621 /* Real and effective uids and gids. */
17622 uid_t mp_realuid; /* process’ real uid */
17623 uid_t mp_effuid; /* process’ effective uid */
17624 gid_t mp_realgid; /* process’ real gid */
MINIX SOURCE CODE File: servers/pm/mproc.h 871
17625 gid_t mp_effgid; /* process’ effective gid */
17626
17627 /* File identification for sharing. */
17628 ino_t mp_ino; /* inode number of file */
17629 dev_t mp_dev; /* device number of file system */
17630 time_t mp_ctime; /* inode changed time */
17631
17632 /* Signal handling information. */
17633 sigset_t mp_ignore; /* 1 means ignore the signal, 0 means don’t */
17634 sigset_t mp_catch; /* 1 means catch the signal, 0 means don’t */
17635 sigset_t mp_sig2mess; /* 1 means transform into notify message */
17636 sigset_t mp_sigmask; /* signals to be blocked */
17637 sigset_t mp_sigmask2; /* saved copy of mp_sigmask */
17638 sigset_t mp_sigpending; /* pending signals to be handled */
17639 struct sigaction mp_sigact[_NSIG + 1]; /* as in sigaction(2) */
17640 vir_bytes mp_sigreturn; /* address of C library __sigreturn function */
17641 struct timer mp_timer; /* watchdog timer for alarm(2) */
17642
17643 /* Backwards compatibility for signals. */
17644 sighandler_t mp_func; /* all sigs vectored to a single user fcn */
17645
17646 unsigned mp_flags; /* flag bits */
17647 vir_bytes mp_procargs; /* ptr to proc’s initial stack arguments */
17648 struct mproc *mp_swapq; /* queue of procs waiting to be swapped in */
17649 message mp_reply; /* reply message to be sent to one */
17650
17651 /* Scheduling priority. */
17652 signed int mp_nice; /* nice is PRIO_MIN..PRIO_MAX, standard 0. */
17653
17654 char mp_name[PROC_NAME_LEN]; /* process name */
17655 } mproc[NR_PROCS];
17656
17657 /* Flag values */
17658 #define IN_USE 0x001 /* set when ’mproc’ slot in use */
17659 #define WAITING 0x002 /* set by WAIT system call */
17660 #define ZOMBIE 0x004 /* set by EXIT, cleared by WAIT */
17661 #define PAUSED 0x008 /* set by PAUSE system call */
17662 #define ALARM_ON 0x010 /* set when SIGALRM timer started */
17663 #define SEPARATE 0x020 /* set if file is separate I & D space */
17664 #define TRACED 0x040 /* set if process is to be traced */
17665 #define STOPPED 0x080 /* set if process stopped for tracing */
17666 #define SIGSUSPENDED 0x100 /* set by SIGSUSPEND system call */
17667 #define REPLY 0x200 /* set if a reply message is pending */
17668 #define ONSWAP 0x400 /* set if data segment is swapped out */
17669 #define SWAPIN 0x800 /* set if on the "swap this in" queue */
17670 #define DONT_SWAP 0x1000 /* never swap out this process */
17671 #define PRIV_PROC 0x2000 /* system process, special privileges */
17672
17673 #define NIL_MPROC ((struct mproc *) 0)
17674