MINIX SOURCE CODE File: servers/pm/pm.h 867
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/pm/pm.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
17000 /* This is the master header for PM. It includes some other files
17001 * and defines the principal constants.
17002 */
17003 #define _POSIX_SOURCE 1 /* tell headers to include POSIX stuff */
17004 #define _MINIX 1 /* tell headers to include MINIX stuff */
17005 #define _SYSTEM 1 /* tell headers that this is the kernel */
17006
17007 /* The following are so basic, all the *.c files get them automatically. */
17008 #include <minix/config.h> /* MUST be first */
17009 #include <ansi.h> /* MUST be second */
17010 #include <sys/types.h>
17011 #include <minix/const.h>
17012 #include <minix/type.h>
17013
17014 #include <fcntl.h>
17015 #include <unistd.h>
17016 #include <minix/syslib.h>
17017 #include <minix/sysutil.h>
17018
17019 #include <limits.h>
17020 #include <errno.h>
17021
17022 #include "const.h"
17023 #include "type.h"
17024 #include "proto.h"
17025 #include "glo.h"
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
17100 /* Constants used by the Process Manager. */
17101
17102 #define NO_MEM ((phys_clicks) 0) /* returned by alloc_mem() with mem is up */
17103
17104 #define NR_PIDS 30000 /* process ids range from 0 to NR_PIDS-1.
17105 * (magic constant: some old applications use
17106 * a ’short’ instead of pid_t.)
17107 */
17108
17109 #define PM_PID 0 /* PM’s process id number */
17110 #define INIT_PID 1 /* INIT’s process id number */
17111
868 File: servers/pm/type.h MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/type.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
17200 /* If there were any type definitions local to the Process Manager, they would
17201 * be here. This file is included only for symmetry with the kernel and File
17202 * System, which do have some local type definitions.
17203 */
17204
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/proto.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
17300 /* Function prototypes. */
17301
17302 struct mproc;
17303 struct stat;
17304 struct mem_map;
17305 struct memory;
17306
17307 #include <timers.h>
17308
17309 /* alloc.c */
17310 _PROTOTYPE( phys_clicks alloc_mem, (phys_clicks clicks) );
17311 _PROTOTYPE( void free_mem, (phys_clicks base, phys_clicks clicks) );
17312 _PROTOTYPE( void mem_init, (struct memory *chunks, phys_clicks *free) );
17313 #define swap_in() ((void)0)
17314 #define swap_inqueue(rmp) ((void)0)
17315
17316 /* break.c */
17317 _PROTOTYPE( int adjust, (struct mproc *rmp,
17318 vir_clicks data_clicks, vir_bytes sp) );
17319 _PROTOTYPE( int do_brk, (void) );
17320 _PROTOTYPE( int size_ok, (int file_type, vir_clicks tc, vir_clicks dc,
17321 vir_clicks sc, vir_clicks dvir, vir_clicks s_vir) );
17322
17323 /* devio.c */
17324 _PROTOTYPE( int do_dev_io, (void) );
17325 _PROTOTYPE( int do_dev_io, (void) );
17326
17327 /* dmp.c */
17328 _PROTOTYPE( int do_fkey_pressed, (void) );
17329
17330 /* exec.c */
17331 _PROTOTYPE( int do_exec, (void) );
17332 _PROTOTYPE( void rw_seg, (int rw, int fd, int proc, int seg,
17333 phys_bytes seg_bytes) );
17334 _PROTOTYPE( struct mproc *find_share, (struct mproc *mp_ign, Ino_t ino,
17335 Dev_t dev, time_t ctime) );
17336
17337 /* forkexit.c */
17338 _PROTOTYPE( int do_fork, (void) );
17339 _PROTOTYPE( int do_pm_exit, (void) );
17340 _PROTOTYPE( int do_waitpid, (void) );
17341 _PROTOTYPE( void pm_exit, (struct mproc *rmp, int exit_status) );
17342
17343 /* getset.c */
17344 _PROTOTYPE( int do_getset, (void) );
MINIX SOURCE CODE File: servers/pm/proto.h 869
17345
17346 /* main.c */
17347 _PROTOTYPE( int main, (void) );
17348
17349 /* misc.c */
17350 _PROTOTYPE( int do_reboot, (void) );
17351 _PROTOTYPE( int do_getsysinfo, (void) );
17352 _PROTOTYPE( int do_getprocnr, (void) );
17353 _PROTOTYPE( int do_svrctl, (void) );
17354 _PROTOTYPE( int do_allocmem, (void) );
17355 _PROTOTYPE( int do_freemem, (void) );
17356 _PROTOTYPE( int do_getsetpriority, (void) );
17357
17358 _PROTOTYPE( void setreply, (int proc_nr, int result) );
17359
17360 /* signal.c */
17361 _PROTOTYPE( int do_alarm, (void) );
17362 _PROTOTYPE( int do_kill, (void) );
17363 _PROTOTYPE( int ksig_pending, (void) );
17364 _PROTOTYPE( int do_pause, (void) );
17365 _PROTOTYPE( int set_alarm, (int proc_nr, int sec) );
17366 _PROTOTYPE( int check_sig, (pid_t proc_id, int signo) );
17367 _PROTOTYPE( void sig_proc, (struct mproc *rmp, int sig_nr) );
17368 _PROTOTYPE( int do_sigaction, (void) );
17369 _PROTOTYPE( int do_sigpending, (void) );
17370 _PROTOTYPE( int do_sigprocmask, (void) );
17371 _PROTOTYPE( int do_sigreturn, (void) );
17372 _PROTOTYPE( int do_sigsuspend, (void) );
17373 _PROTOTYPE( void check_pending, (struct mproc *rmp) );
17374
17375 /* time.c */
17376 _PROTOTYPE( int do_stime, (void) );
17377 _PROTOTYPE( int do_time, (void) );
17378 _PROTOTYPE( int do_times, (void) );
17379 _PROTOTYPE( int do_gettimeofday, (void) );
17380
17381 /* timers.c */
17382 _PROTOTYPE( void pm_set_timer, (timer_t *tp, int delta,
17383 tmr_func_t watchdog, int arg));
17384 _PROTOTYPE( void pm_expire_timers, (clock_t now));
17385 _PROTOTYPE( void pm_cancel_timer, (timer_t *tp));
17386
17387 /* trace.c */
17388 _PROTOTYPE( int do_trace, (void) );
17389 _PROTOTYPE( void stop_proc, (struct mproc *rmp, int sig_nr) );
17390
17391 /* utility.c */
17392 _PROTOTYPE( pid_t get_free_pid, (void) );
17393 _PROTOTYPE( int allowed, (char *name_buf, struct stat *s_buf, int mask) );
17394 _PROTOTYPE( int no_sys, (void) );
17395 _PROTOTYPE( void panic, (char *who, char *mess, int num) );
17396 _PROTOTYPE( void tell_fs, (int what, int p1, int p2, int p3) );
17397 _PROTOTYPE( int get_stack_ptr, (int proc_nr, vir_bytes *sp) );
17398 _PROTOTYPE( int get_mem_map, (int proc_nr, struct mem_map *mem_map) );
17399 _PROTOTYPE( char *find_param, (const char *key));
17400 _PROTOTYPE( int proc_from_pid, (pid_t p));
870 File: servers/pm/glo.h MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/glo.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
17500 /* EXTERN should be extern except in table.c */
17501 #ifdef _TABLE
17502 #undef EXTERN
17503 #define EXTERN
17504 #endif
17505
17506 /* Global variables. */
17507 EXTERN struct mproc *mp; /* ptr to ’mproc’ slot of current process */
17508 EXTERN int procs_in_use; /* how many processes are marked as IN_USE */
17509 EXTERN char monitor_params[128*sizeof(char *)]; /* boot monitor parameters */
17510 EXTERN struct kinfo kinfo; /* kernel information */
17511
17512 /* The parameters of the call are kept here. */
17513 EXTERN message m_in; /* the incoming message itself is kept here. */
17514 EXTERN int who; /* caller’s proc number */
17515 EXTERN int call_nr; /* system call number */
17516
17517 extern _PROTOTYPE (int (*call_vec[]), (void) ); /* system call handlers */
17518 extern char core_name[]; /* file name where core images are produced */
17519 EXTERN sigset_t core_sset; /* which signals cause core images */
17520 EXTERN sigset_t ign_sset; /* which signals are by default ignored */
17521
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
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/param.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
17700 /* The following names are synonyms for the variables in the input message. */
17701 #define addr m1_p1
17702 #define exec_name m1_p1
17703 #define exec_len m1_i1
17704 #define func m6_f1
872 File: servers/pm/param.h MINIX SOURCE CODE
17705 #define grp_id m1_i1
17706 #define namelen m1_i2
17707 #define pid m1_i1
17708 #define procnr m1_i1
17709 #define seconds m1_i1
17710 #define sig m6_i1
17711 #define stack_bytes m1_i2
17712 #define stack_ptr m1_p2
17713 #define status m1_i1
17714 #define usr_id m1_i1
17715 #define request m2_i2
17716 #define taddr m2_l1
17717 #define data m2_l2
17718 #define sig_nr m1_i2
17719 #define sig_nsa m1_p1
17720 #define sig_osa m1_p2
17721 #define sig_ret m1_p3
17722 #define sig_set m2_l1
17723 #define sig_how m2_i1
17724 #define sig_flags m2_i2
17725 #define sig_context m2_p1
17726 #define info_what m1_i1
17727 #define info_where m1_p1
17728 #define reboot_flag m1_i1
17729 #define reboot_code m1_p1
17730 #define reboot_strlen m1_i2
17731 #define svrctl_req m2_i1
17732 #define svrctl_argp m2_p1
17733 #define stime m2_l1
17734 #define memsize m4_l1
17735 #define membase m4_l2
17736
17737 /* The following names are synonyms for the variables in a reply message. */
17738 #define reply_res m_type
17739 #define reply_res2 m2_i1
17740 #define reply_ptr m2_p1
17741 #define reply_mask m2_l1
17742 #define reply_trace m2_l2
17743 #define reply_time m2_l1
17744 #define reply_utime m2_l2
17745 #define reply_t1 m4_l1
17746 #define reply_t2 m4_l2
17747 #define reply_t3 m4_l3
17748 #define reply_t4 m4_l4
17749 #define reply_t5 m4_l5
17750
17751 /* The following names are used to inform the FS about certain events. */
17752 #define tell_fs_arg1 m1_i1
17753 #define tell_fs_arg2 m1_i2
17754 #define tell_fs_arg3 m1_i3
17755
MINIX SOURCE CODE File: servers/pm/table.c 873
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/table.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
17800 /* This file contains the table used to map system call numbers onto the
17801 * routines that perform them.
17802 */
17803
17804 #define _TABLE
17805
17806 #include "pm.h"
17807 #include <minix/callnr.h>
17808 #include <signal.h>
17809 #include "mproc.h"
17810 #include "param.h"
17811
17812 /* Miscellaneous */
17813 char core_name[] = "core"; /* file name where core images are produced */
17814
17815 _PROTOTYPE (int (*call_vec[NCALLS]), (void) ) = {
17816 no_sys, /* 0 = unused */
17817 do_pm_exit, /* 1 = exit */
17818 do_fork, /* 2 = fork */
17819 no_sys, /* 3 = read */
17820 no_sys, /* 4 = write */
17821 no_sys, /* 5 = open */
17822 no_sys, /* 6 = close */
17823 do_waitpid, /* 7 = wait */
17824 no_sys, /* 8 = creat */
17825 no_sys, /* 9 = link */
17826 no_sys, /* 10 = unlink */
17827 do_waitpid, /* 11 = waitpid */
17828 no_sys, /* 12 = chdir */
17829 do_time, /* 13 = time */
17830 no_sys, /* 14 = mknod */
17831 no_sys, /* 15 = chmod */
17832 no_sys, /* 16 = chown */
17833 do_brk, /* 17 = break */
17834 no_sys, /* 18 = stat */
17835 no_sys, /* 19 = lseek */
17836 do_getset, /* 20 = getpid */
17837 no_sys, /* 21 = mount */
17838 no_sys, /* 22 = umount */
17839 do_getset, /* 23 = setuid */
17840 do_getset, /* 24 = getuid */
17841 do_stime, /* 25 = stime */
17842 do_trace, /* 26 = ptrace */
17843 do_alarm, /* 27 = alarm */
17844 no_sys, /* 28 = fstat */
17845 do_pause, /* 29 = pause */
17846 no_sys, /* 30 = utime */
17847 no_sys, /* 31 = (stty) */
17848 no_sys, /* 32 = (gtty) */
17849 no_sys, /* 33 = access */
17850 no_sys, /* 34 = (nice) */
17851 no_sys, /* 35 = (ftime) */
17852 no_sys, /* 36 = sync */
17853 do_kill, /* 37 = kill */
17854 no_sys, /* 38 = rename */
874 File: servers/pm/table.c MINIX SOURCE CODE
17855 no_sys, /* 39 = mkdir */
17856 no_sys, /* 40 = rmdir */
17857 no_sys, /* 41 = dup */
17858 no_sys, /* 42 = pipe */
17859 do_times, /* 43 = times */
17860 no_sys, /* 44 = (prof) */
17861 no_sys, /* 45 = unused */
17862 do_getset, /* 46 = setgid */
17863 do_getset, /* 47 = getgid */
17864 no_sys, /* 48 = (signal)*/
17865 no_sys, /* 49 = unused */
17866 no_sys, /* 50 = unused */
17867 no_sys, /* 51 = (acct) */
17868 no_sys, /* 52 = (phys) */
17869 no_sys, /* 53 = (lock) */
17870 no_sys, /* 54 = ioctl */
17871 no_sys, /* 55 = fcntl */
17872 no_sys, /* 56 = (mpx) */
17873 no_sys, /* 57 = unused */
17874 no_sys, /* 58 = unused */
17875 do_exec, /* 59 = execve */
17876 no_sys, /* 60 = umask */
17877 no_sys, /* 61 = chroot */
17878 do_getset, /* 62 = setsid */
17879 do_getset, /* 63 = getpgrp */
17880
17881 no_sys, /* 64 = unused */
17882 no_sys, /* 65 = UNPAUSE */
17883 no_sys, /* 66 = unused */
17884 no_sys, /* 67 = REVIVE */
17885 no_sys, /* 68 = TASK_REPLY */
17886 no_sys, /* 69 = unused */
17887 no_sys, /* 70 = unused */
17888 do_sigaction, /* 71 = sigaction */
17889 do_sigsuspend, /* 72 = sigsuspend */
17890 do_sigpending, /* 73 = sigpending */
17891 do_sigprocmask, /* 74 = sigprocmask */
17892 do_sigreturn, /* 75 = sigreturn */
17893 do_reboot, /* 76 = reboot */
17894 do_svrctl, /* 77 = svrctl */
17895
17896 no_sys, /* 78 = unused */
17897 do_getsysinfo, /* 79 = getsysinfo */
17898 do_getprocnr, /* 80 = getprocnr */
17899 no_sys, /* 81 = unused */
17900 no_sys, /* 82 = fstatfs */
17901 do_allocmem, /* 83 = memalloc */
17902 do_freemem, /* 84 = memfree */
17903 no_sys, /* 85 = select */
17904 no_sys, /* 86 = fchdir */
17905 no_sys, /* 87 = fsync */
17906 do_getsetpriority, /* 88 = getpriority */
17907 do_getsetpriority, /* 89 = setpriority */
17908 do_time, /* 90 = gettimeofday */
17909 };
17910 /* This should not fail with "array size is negative": */
17911 extern int dummy[sizeof(call_vec) == NCALLS * sizeof(call_vec[0]) ? 1 : -1];
MINIX SOURCE CODE File: servers/pm/main.c 875
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
18000 /* This file contains the main program of the process manager and some related
18001 * procedures. When MINIX starts up, the kernel runs for a little while,
18002 * initializing itself and its tasks, and then it runs PM and FS. Both PM
18003 * and FS initialize themselves as far as they can. PM asks the kernel for
18004 * all free memory and starts serving requests.
18005 *
18006 * The entry points into this file are:
18007 * main: starts PM running
18008 * setreply: set the reply to be sent to process making an PM system call
18009 */
18010
18011 #include "pm.h"
18012 #include <minix/keymap.h>
18013 #include <minix/callnr.h>
18014 #include <minix/com.h>
18015 #include <signal.h>
18016 #include <stdlib.h>
18017 #include <fcntl.h>
18018 #include <sys/resource.h>
18019 #include <string.h>
18020 #include "mproc.h"
18021 #include "param.h"
18022
18023 #include "../../kernel/const.h"
18024 #include "../../kernel/config.h"
18025 #include "../../kernel/type.h"
18026 #include "../../kernel/proc.h"
18027
18028 FORWARD _PROTOTYPE( void get_work, (void) );
18029 FORWARD _PROTOTYPE( void pm_init, (void) );
18030 FORWARD _PROTOTYPE( int get_nice_value, (int queue) );
18031 FORWARD _PROTOTYPE( void get_mem_chunks, (struct memory *mem_chunks) );
18032 FORWARD _PROTOTYPE( void patch_mem_chunks, (struct memory *mem_chunks,
18033 struct mem_map *map_ptr) );
18034
18035 #define click_to_round_k(n) \
18036 ((unsigned) ((((unsigned long) (n) << CLICK_SHIFT) + 512) / 1024))
18037
18038 /*===========================================================================*
18039 * main *
18040 *===========================================================================*/
18041 PUBLIC int main()
18042 {
18043 /* Main routine of the process manager. */
18044 int result, s, proc_nr;
18045 struct mproc *rmp;
18046 sigset_t sigset;
18047
18048 pm_init(); /* initialize process manager tables */
18049
18050 /* This is PM’s main loop- get work and do it, forever and forever. */
18051 while (TRUE) {
18052 get_work(); /* wait for an PM system call */
18053
18054 /* Check for system notifications first. Special cases. */
876 File: servers/pm/main.c MINIX SOURCE CODE
18055 if (call_nr == SYN_ALARM) {
18056 pm_expire_timers(m_in.NOTIFY_TIMESTAMP);
18057 result = SUSPEND; /* don’t reply */
18058 } else if (call_nr == SYS_SIG) { /* signals pending */
18059 sigset = m_in.NOTIFY_ARG;
18060 if (sigismember(&sigset, SIGKSIG)) (void) ksig_pending();
18061 result = SUSPEND; /* don’t reply */
18062 }
18063 /* Else, if the system call number is valid, perform the call. */
18064 else if ((unsigned) call_nr >= NCALLS) {
18065 result = ENOSYS;
18066 } else {
18067 result = (*call_vec[call_nr])();
18068 }
18069
18070 /* Send the results back to the user to indicate completion. */
18071 if (result != SUSPEND) setreply(who, result);
18072
18073 swap_in(); /* maybe a process can be swapped in? */
18074
18075 /* Send out all pending reply messages, including the answer to
18076 * the call just made above. The processes must not be swapped out.
18077 */
18078 for (proc_nr=0, rmp=mproc; proc_nr < NR_PROCS; proc_nr++, rmp++) {
18079 /* In the meantime, the process may have been killed by a
18080 * signal (e.g. if a lethal pending signal was unblocked)
18081 * without the PM realizing it. If the slot is no longer in
18082 * use or just a zombie, don’t try to reply.
18083 */
18084 if ((rmp->mp_flags & (REPLY | ONSWAP | IN_USE | ZOMBIE)) ==
18085 (REPLY | IN_USE)) {
18086 if ((s=send(proc_nr, &rmp->mp_reply)) != OK) {
18087 panic(__FILE__,"PM can’t reply to", proc_nr);
18088 }
18089 rmp->mp_flags &= ˜REPLY;
18090 }
18091 }
18092 }
18093 return(OK);
18094 }
18096 /*===========================================================================*
18097 * get_work *
18098 *===========================================================================*/
18099 PRIVATE void get_work()
18100 {
18101 /* Wait for the next message and extract useful information from it. */
18102 if (receive(ANY, &m_in) != OK) panic(__FILE__,"PM receive error", NO_NUM);
18103 who = m_in.m_source; /* who sent the message */
18104 call_nr = m_in.m_type; /* system call number */
18105
18106 /* Process slot of caller. Misuse PM’s own process slot if the kernel is
18107 * calling. This can happen in case of synchronous alarms (CLOCK) or or
18108 * event like pending kernel signals (SYSTEM).
18109 */
18110 mp = &mproc[who < 0 ? PM_PROC_NR : who];
18111 }
MINIX SOURCE CODE File: servers/pm/main.c 877
18113 /*===========================================================================*
18114 * setreply *
18115 *===========================================================================*/
18116 PUBLIC void setreply(proc_nr, result)
18117 int proc_nr; /* process to reply to */
18118 int result; /* result of call (usually OK or error #) */
18119 {
18120 /* Fill in a reply message to be sent later to a user process. System calls
18121 * may occasionally fill in other fields, this is only for the main return
18122 * value, and for setting the "must send reply" flag.
18123 */
18124 register struct mproc *rmp = &mproc[proc_nr];
18125
18126 rmp->mp_reply.reply_res = result;
18127 rmp->mp_flags |= REPLY; /* reply pending */
18128
18129 if (rmp->mp_flags & ONSWAP)
18130 swap_inqueue(rmp); /* must swap this process back in */
18131 }
18133 /*===========================================================================*
18134 * pm_init *
18135 *===========================================================================*/
18136 PRIVATE void pm_init()
18137 {
18138 /* Initialize the process manager.
18139 * Memory use info is collected from the boot monitor, the kernel, and
18140 * all processes compiled into the system image. Initially this information
18141 * is put into an array mem_chunks. Elements of mem_chunks are struct memory,
18142 * and hold base, size pairs in units of clicks. This array is small, there
18143 * should be no more than 8 chunks. After the array of chunks has been built
18144 * the contents are used to initialize the hole list. Space for the hole list
18145 * is reserved as an array with twice as many elements as the maximum number
18146 * of processes allowed. It is managed as a linked list, and elements of the
18147 * array are struct hole, which, in addition to storage for a base and size in
18148 * click units also contain space for a link, a pointer to another element.
18149 */
18150 int s;
18151 static struct boot_image image[NR_BOOT_PROCS];
18152 register struct boot_image *ip;
18153 static char core_sigs[] = { SIGQUIT, SIGILL, SIGTRAP, SIGABRT,
18154 SIGEMT, SIGFPE, SIGUSR1, SIGSEGV, SIGUSR2 };
18155 static char ign_sigs[] = { SIGCHLD };
18156 register struct mproc *rmp;
18157 register char *sig_ptr;
18158 phys_clicks total_clicks, minix_clicks, free_clicks;
18159 message mess;
18160 struct mem_map mem_map[NR_LOCAL_SEGS];
18161 struct memory mem_chunks[NR_MEMS];
18162
18163 /* Initialize process table, including timers. */
18164 for (rmp=&mproc[0]; rmp<&mproc[NR_PROCS]; rmp++) {
18165 tmr_inittimer(&rmp->mp_timer);
18166 }
18167
18168 /* Build the set of signals which cause core dumps, and the set of signals
18169 * that are by default ignored.
18170 */
18171 sigemptyset(&core_sset);
18172 for (sig_ptr = core_sigs; sig_ptr < core_sigs+sizeof(core_sigs); sig_ptr++)
878 File: servers/pm/main.c MINIX SOURCE CODE
18173 sigaddset(&core_sset, *sig_ptr);
18174 sigemptyset(&ign_sset);
18175 for (sig_ptr = ign_sigs; sig_ptr < ign_sigs+sizeof(ign_sigs); sig_ptr++)
18176 sigaddset(&ign_sset, *sig_ptr);
18177
18178 /* Obtain a copy of the boot monitor parameters and the kernel info struct.
18179 * Parse the list of free memory chunks. This list is what the boot monitor
18180 * reported, but it must be corrected for the kernel and system processes.
18181 */
18182 if ((s=sys_getmonparams(monitor_params, sizeof(monitor_params))) != OK)
18183 panic(__FILE__,"get monitor params failed",s);
18184 get_mem_chunks(mem_chunks);
18185 if ((s=sys_getkinfo(&kinfo)) != OK)
18186 panic(__FILE__,"get kernel info failed",s);
18187
18188 /* Get the memory map of the kernel to see how much memory it uses. */
18189 if ((s=get_mem_map(SYSTASK, mem_map)) != OK)
18190 panic(__FILE__,"couldn’t get memory map of SYSTASK",s);
18191 minix_clicks = (mem_map[S].mem_phys+mem_map[S].mem_len)-mem_map[T].mem_phys;
18192 patch_mem_chunks(mem_chunks, mem_map);
18193
18194 /* Initialize PM’s process table. Request a copy of the system image table
18195 * that is defined at the kernel level to see which slots to fill in.
18196 */
18197 if (OK != (s=sys_getimage(image)))
18198 panic(__FILE__,"couldn’t get image table: %d\n", s);
18199 procs_in_use = 0; /* start populating table */
18200 printf("Building process table:"); /* show what’s happening */
18201 for (ip = &image[0]; ip < &image[NR_BOOT_PROCS]; ip++) {
18202 if (ip->proc_nr >= 0) { /* task have negative nrs */
18203 procs_in_use += 1; /* found user process */
18204
18205 /* Set process details found in the image table. */
18206 rmp = &mproc[ip->proc_nr];
18207 strncpy(rmp->mp_name, ip->proc_name, PROC_NAME_LEN);
18208 rmp->mp_parent = RS_PROC_NR;
18209 rmp->mp_nice = get_nice_value(ip->priority);
18210 if (ip->proc_nr == INIT_PROC_NR) { /* user process */
18211 rmp->mp_pid = INIT_PID;
18212 rmp->mp_flags |= IN_USE;
18213 sigemptyset(&rmp->mp_ignore);
18214 }
18215 else { /* system process */
18216 rmp->mp_pid = get_free_pid();
18217 rmp->mp_flags |= IN_USE | DONT_SWAP | PRIV_PROC;
18218 sigfillset(&rmp->mp_ignore);
18219 }
18220 sigemptyset(&rmp->mp_sigmask);
18221 sigemptyset(&rmp->mp_catch);
18222 sigemptyset(&rmp->mp_sig2mess);
18223
18224 /* Get memory map for this process from the kernel. */
18225 if ((s=get_mem_map(ip->proc_nr, rmp->mp_seg)) != OK)
18226 panic(__FILE__,"couldn’t get process entry",s);
18227 if (rmp->mp_seg[T].mem_len != 0) rmp->mp_flags |= SEPARATE;
18228 minix_clicks += rmp->mp_seg[S].mem_phys +
18229 rmp->mp_seg[S].mem_len - rmp->mp_seg[T].mem_phys;
18230 patch_mem_chunks(mem_chunks, rmp->mp_seg);
18231
18232 /* Tell FS about this system process. */
MINIX SOURCE CODE File: servers/pm/main.c 879
18233 mess.PR_PROC_NR = ip->proc_nr;
18234 mess.PR_PID = rmp->mp_pid;
18235 if (OK != (s=send(FS_PROC_NR, &mess)))
18236 panic(__FILE__,"can’t sync up with FS", s);
18237 printf(" %s", ip->proc_name); /* display process name */
18238 }
18239 }
18240 printf(".\n"); /* last process done */
18241
18242 /* Override some details. PM is somewhat special. */
18243 mproc[PM_PROC_NR].mp_pid = PM_PID; /* magically override pid */
18244 mproc[PM_PROC_NR].mp_parent = PM_PROC_NR; /* PM doesn’t have parent */
18245
18246 /* Tell FS that no more system processes follow and synchronize. */
18247 mess.PR_PROC_NR = NONE;
18248 if (sendrec(FS_PROC_NR, &mess) != OK || mess.m_type != OK)
18249 panic(__FILE__,"can’t sync up with FS", NO_NUM);
18250
18251 /* Initialize tables to all physical memory and print memory information. */
18252 printf("Physical memory:");
18253 mem_init(mem_chunks, &free_clicks);
18254 total_clicks = minix_clicks + free_clicks;
18255 printf(" total %u KB,", click_to_round_k(total_clicks));
18256 printf(" system %u KB,", click_to_round_k(minix_clicks));
18257 printf(" free %u KB.\n", click_to_round_k(free_clicks));
18258 }
18260 /*===========================================================================*
18261 * get_nice_value *
18262 *===========================================================================*/
18263 PRIVATE int get_nice_value(queue)
18264 int queue; /* store mem chunks here */
18265 {
18266 /* Processes in the boot image have a priority assigned. The PM doesn’t know
18267 * about priorities, but uses ’nice’ values instead. The priority is between
18268 * MIN_USER_Q and MAX_USER_Q. We have to scale between PRIO_MIN and PRIO_MAX.
18269 */
18270 int nice_val = (queue - USER_Q) * (PRIO_MAX-PRIO_MIN+1) /
18271 (MIN_USER_Q-MAX_USER_Q+1);
18272 if (nice_val > PRIO_MAX) nice_val = PRIO_MAX; /* shouldn’t happen */
18273 if (nice_val < PRIO_MIN) nice_val = PRIO_MIN; /* shouldn’t happen */
18274 return nice_val;
18275 }
18277 /*===========================================================================*
18278 * get_mem_chunks *
18279 *===========================================================================*/
18280 PRIVATE void get_mem_chunks(mem_chunks)
18281 struct memory *mem_chunks; /* store mem chunks here */
18282 {
18283 /* Initialize the free memory list from the ’memory’ boot variable. Translate
18284 * the byte offsets and sizes in this list to clicks, properly truncated. Also
18285 * make sure that we don’t exceed the maximum address space of the 286 or the
18286 * 8086, i.e. when running in 16-bit protected mode or real mode.
18287 */
18288 long base, size, limit;
18289 char *s, *end; /* use to parse boot variable */
18290 int i, done = 0;
18291 struct memory *memp;
18292
880 File: servers/pm/main.c MINIX SOURCE CODE
18293 /* Initialize everything to zero. */
18294 for (i = 0; i < NR_MEMS; i++) {
18295 memp = &mem_chunks[i]; /* next mem chunk is stored here */
18296 memp->base = memp->size = 0;
18297 }
18298
18299 /* The available memory is determined by MINIX’ boot loader as a list of
18300 * (base:size)-pairs in boothead.s. The ’memory’ boot variable is set in
18301 * in boot.s. The format is "b0:s0,b1:s1,b2:s2", where b0:s0 is low mem,
18302 * b1:s1 is mem between 1M and 16M, b2:s2 is mem above 16M. Pairs b1:s1
18303 * and b2:s2 are combined if the memory is adjacent.
18304 */
18305 s = find_param("memory"); /* get memory boot variable */
18306 for (i = 0; i < NR_MEMS && !done; i++) {
18307 memp = &mem_chunks[i]; /* next mem chunk is stored here */
18308 base = size = 0; /* initialize next base:size pair */
18309 if (*s != 0) { /* get fresh data, unless at end */
18310
18311 /* Read fresh base and expect colon as next char. */
18312 base = strtoul(s, &end, 0x10); /* get number */
18313 if (end != s && *end == ’:’) s = ++end; /* skip ’:’ */
18314 else *s=0; /* terminate, should not happen */
18315
18316 /* Read fresh size and expect comma or assume end. */
18317 size = strtoul(s, &end, 0x10); /* get number */
18318 if (end != s && *end == ’,’) s = ++end; /* skip ’,’ */
18319 else done = 1;
18320 }
18321 limit = base + size;
18322 base = (base + CLICK_SIZE-1) & ˜(long)(CLICK_SIZE-1);
18323 limit &= ˜(long)(CLICK_SIZE-1);
18324 if (limit <= base) continue;
18325 memp->base = base >> CLICK_SHIFT;
18326 memp->size = (limit - base) >> CLICK_SHIFT;
18327 }
18328 }
18330 /*===========================================================================*
18331 * patch_mem_chunks *
18332 *===========================================================================*/
18333 PRIVATE void patch_mem_chunks(mem_chunks, map_ptr)
18334 struct memory *mem_chunks; /* store mem chunks here */
18335 struct mem_map *map_ptr; /* memory to remove */
18336 {
18337 /* Remove server memory from the free memory list. The boot monitor
18338 * promises to put processes at the start of memory chunks. The
18339 * tasks all use same base address, so only the first task changes
18340 * the memory lists. The servers and init have their own memory
18341 * spaces and their memory will be removed from the list.
18342 */
18343 struct memory *memp;
18344 for (memp = mem_chunks; memp < &mem_chunks[NR_MEMS]; memp++) {
18345 if (memp->base == map_ptr[T].mem_phys) {
18346 memp->base += map_ptr[T].mem_len + map_ptr[D].mem_len;
18347 memp->size -= map_ptr[T].mem_len + map_ptr[D].mem_len;
18348 }
18349 }
18350 }
MINIX SOURCE CODE File: servers/pm/forkexit.c 881
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/forkexit.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
18400 /* This file deals with creating processes (via FORK) and deleting them (via
18401 * EXIT/WAIT). When a process forks, a new slot in the ’mproc’ table is
18402 * allocated for it, and a copy of the parent’s core image is made for the
18403 * child. Then the kernel and file system are informed. A process is removed
18404 * from the ’mproc’ table when two events have occurred: (1) it has exited or
18405 * been killed by a signal, and (2) the parent has done a WAIT. If the process
18406 * exits first, it continues to occupy a slot until the parent does a WAIT.
18407 *
18408 * The entry points into this file are:
18409 * do_fork: perform the FORK system call
18410 * do_pm_exit: perform the EXIT system call (by calling pm_exit())
18411 * pm_exit: actually do the exiting
18412 * do_wait: perform the WAITPID or WAIT system call
18413 */
18414
18415 #include "pm.h"
18416 #include <sys/wait.h>
18417 #include <minix/callnr.h>
18418 #include <minix/com.h>
18419 #include <signal.h>
18420 #include "mproc.h"
18421 #include "param.h"
18422
18423 #define LAST_FEW 2 /* last few slots reserved for superuser */
18424
18425 FORWARD _PROTOTYPE (void cleanup, (register struct mproc *child) );
18426
18427 /*===========================================================================*
18428 * do_fork *
18429 *===========================================================================*/
18430 PUBLIC int do_fork()
18431 {
18432 /* The process pointed to by ’mp’ has forked. Create a child process. */
18433 register struct mproc *rmp; /* pointer to parent */
18434 register struct mproc *rmc; /* pointer to child */
18435 int child_nr, s;
18436 phys_clicks prog_clicks, child_base;
18437 phys_bytes prog_bytes, parent_abs, child_abs; /* Intel only */
18438 pid_t new_pid;
18439
18440 /* If tables might fill up during FORK, don’t even start since recovery half
18441 * way through is such a nuisance.
18442 */
18443 rmp = mp;
18444 if ((procs_in_use == NR_PROCS) ||
18445 (procs_in_use >= NR_PROCS-LAST_FEW && rmp->mp_effuid != 0))
18446 {
18447 printf("PM: warning, process table is full!\n");
18448 return(EAGAIN);
18449 }
18450
18451 /* Determine how much memory to allocate. Only the data and stack need to
18452 * be copied, because the text segment is either shared or of zero length.
18453 */
18454 prog_clicks = (phys_clicks) rmp->mp_seg[S].mem_len;
882 File: servers/pm/forkexit.c MINIX SOURCE CODE
18455 prog_clicks += (rmp->mp_seg[S].mem_vir - rmp->mp_seg[D].mem_vir);
18456 prog_bytes = (phys_bytes) prog_clicks << CLICK_SHIFT;
18457 if ( (child_base = alloc_mem(prog_clicks)) == NO_MEM) return(ENOMEM);
18458
18459 /* Create a copy of the parent’s core image for the child. */
18460 child_abs = (phys_bytes) child_base << CLICK_SHIFT;
18461 parent_abs = (phys_bytes) rmp->mp_seg[D].mem_phys << CLICK_SHIFT;
18462 s = sys_abscopy(parent_abs, child_abs, prog_bytes);
18463 if (s < 0) panic(__FILE__,"do_fork can’t copy", s);
18464
18465 /* Find a slot in ’mproc’ for the child process. A slot must exist. */
18466 for (rmc = &mproc[0]; rmc < &mproc[NR_PROCS]; rmc++)
18467 if ( (rmc->mp_flags & IN_USE) == 0) break;
18468
18469 /* Set up the child and its memory map; copy its ’mproc’ slot from parent. */
18470 child_nr = (int)(rmc - mproc); /* slot number of the child */
18471 procs_in_use++;
18472 *rmc = *rmp; /* copy parent’s process slot to child’s */
18473 rmc->mp_parent = who; /* record child’s parent */
18474 /* inherit only these flags */
18475 rmc->mp_flags &= (IN_USE|SEPARATE|PRIV_PROC|DONT_SWAP);
18476 rmc->mp_child_utime = 0; /* reset administration */
18477 rmc->mp_child_stime = 0; /* reset administration */
18478
18479 /* A separate I&D child keeps the parents text segment. The data and stack
18480 * segments must refer to the new copy.
18481 */
18482 if (!(rmc->mp_flags & SEPARATE)) rmc->mp_seg[T].mem_phys = child_base;
18483 rmc->mp_seg[D].mem_phys = child_base;
18484 rmc->mp_seg[S].mem_phys = rmc->mp_seg[D].mem_phys +
18485 (rmp->mp_seg[S].mem_vir - rmp->mp_seg[D].mem_vir);
18486 rmc->mp_exitstatus = 0;
18487 rmc->mp_sigstatus = 0;
18488
18489 /* Find a free pid for the child and put it in the table. */
18490 new_pid = get_free_pid();
18491 rmc->mp_pid = new_pid; /* assign pid to child */
18492
18493 /* Tell kernel and file system about the (now successful) FORK. */
18494 sys_fork(who, child_nr);
18495 tell_fs(FORK, who, child_nr, rmc->mp_pid);
18496
18497 /* Report child’s memory map to kernel. */
18498 sys_newmap(child_nr, rmc->mp_seg);
18499
18500 /* Reply to child to wake it up. */
18501 setreply(child_nr, 0); /* only parent gets details */
18502 rmp->mp_reply.procnr = child_nr; /* child’s process number */
18503 return(new_pid); /* child’s pid */
18504 }
18506 /*===========================================================================*
18507 * do_pm_exit *
18508 *===========================================================================*/
18509 PUBLIC int do_pm_exit()
18510 {
18511 /* Perform the exit(status) system call. The real work is done by pm_exit(),
18512 * which is also called when a process is killed by a signal.
18513 */
18514 pm_exit(mp, m_in.status);
MINIX SOURCE CODE File: servers/pm/forkexit.c 883
18515 return(SUSPEND); /* can’t communicate from beyond the grave */
18516 }
18518 /*===========================================================================*
18519 * pm_exit *
18520 *===========================================================================*/
18521 PUBLIC void pm_exit(rmp, exit_status)
18522 register struct mproc *rmp; /* pointer to the process to be terminated */
18523 int exit_status; /* the process’ exit status (for parent) */
18524 {
18525 /* A process is done. Release most of the process’ possessions. If its
18526 * parent is waiting, release the rest, else keep the process slot and
18527 * become a zombie.
18528 */
18529 register int proc_nr;
18530 int parent_waiting, right_child;
18531 pid_t pidarg, procgrp;
18532 struct mproc *p_mp;
18533 clock_t t[5];
18534
18535 proc_nr = (int) (rmp - mproc); /* get process slot number */
18536
18537 /* Remember a session leader’s process group. */
18538 procgrp = (rmp->mp_pid == mp->mp_procgrp) ? mp->mp_procgrp : 0;
18539
18540 /* If the exited process has a timer pending, kill it. */
18541 if (rmp->mp_flags & ALARM_ON) set_alarm(proc_nr, (unsigned) 0);
18542
18543 /* Do accounting: fetch usage times and accumulate at parent. */
18544 sys_times(proc_nr, t);
18545 p_mp = &mproc[rmp->mp_parent]; /* process’ parent */
18546 p_mp->mp_child_utime += t[0] + rmp->mp_child_utime; /* add user time */
18547 p_mp->mp_child_stime += t[1] + rmp->mp_child_stime; /* add system time */
18548
18549 /* Tell the kernel and FS that the process is no longer runnable. */
18550 tell_fs(EXIT, proc_nr, 0, 0); /* file system can free the proc slot */
18551 sys_exit(proc_nr);
18552
18553 /* Pending reply messages for the dead process cannot be delivered. */
18554 rmp->mp_flags &= ˜REPLY;
18555
18556 /* Release the memory occupied by the child. */
18557 if (find_share(rmp, rmp->mp_ino, rmp->mp_dev, rmp->mp_ctime) == NULL) {
18558 /* No other process shares the text segment, so free it. */
18559 free_mem(rmp->mp_seg[T].mem_phys, rmp->mp_seg[T].mem_len);
18560 }
18561 /* Free the data and stack segments. */
18562 free_mem(rmp->mp_seg[D].mem_phys,
18563 rmp->mp_seg[S].mem_vir
18564 + rmp->mp_seg[S].mem_len - rmp->mp_seg[D].mem_vir);
18565
18566 /* The process slot can only be freed if the parent has done a WAIT. */
18567 rmp->mp_exitstatus = (char) exit_status;
18568
18569 pidarg = p_mp->mp_wpid; /* who’s being waited for? */
18570 parent_waiting = p_mp->mp_flags & WAITING;
18571 right_child = /* child meets one of the 3 tests? */
18572 (pidarg == -1 || pidarg == rmp->mp_pid || -pidarg == rmp->mp_procgrp);
18573
18574 if (parent_waiting && right_child) {
884 File: servers/pm/forkexit.c MINIX SOURCE CODE
18575 cleanup(rmp); /* tell parent and release child slot */
18576 } else {
18577 rmp->mp_flags = IN_USE|ZOMBIE; /* parent not waiting, zombify child */
18578 sig_proc(p_mp, SIGCHLD); /* send parent a "child died" signal */
18579 }
18580
18581 /* If the process has children, disinherit them. INIT is the new parent. */
18582 for (rmp = &mproc[0]; rmp < &mproc[NR_PROCS]; rmp++) {
18583 if (rmp->mp_flags & IN_USE && rmp->mp_parent == proc_nr) {
18584 /* ’rmp’ now points to a child to be disinherited. */
18585 rmp->mp_parent = INIT_PROC_NR;
18586 parent_waiting = mproc[INIT_PROC_NR].mp_flags & WAITING;
18587 if (parent_waiting && (rmp->mp_flags & ZOMBIE)) cleanup(rmp);
18588 }
18589 }
18590
18591 /* Send a hangup to the process’ process group if it was a session leader. */
18592 if (procgrp != 0) check_sig(-procgrp, SIGHUP);
18593 }
18595 /*===========================================================================*
18596 * do_waitpid *
18597 *===========================================================================*/
18598 PUBLIC int do_waitpid()
18599 {
18600 /* A process wants to wait for a child to terminate. If a child is already
18601 * waiting, go clean it up and let this WAIT call terminate. Otherwise,
18602 * really wait.
18603 * A process calling WAIT never gets a reply in the usual way at the end
18604 * of the main loop (unless WNOHANG is set or no qualifying child exists).
18605 * If a child has already exited, the routine cleanup() sends the reply
18606 * to awaken the caller.
18607 * Both WAIT and WAITPID are handled by this code.
18608 */
18609 register struct mproc *rp;
18610 int pidarg, options, children;
18611
18612 /* Set internal variables, depending on whether this is WAIT or WAITPID. */
18613 pidarg = (call_nr == WAIT ? -1 : m_in.pid); /* 1st param of waitpid */
18614 options = (call_nr == WAIT ? 0 : m_in.sig_nr); /* 3rd param of waitpid */
18615 if (pidarg == 0) pidarg = -mp->mp_procgrp; /* pidarg < 0 ==> proc grp */
18616
18617 /* Is there a child waiting to be collected? At this point, pidarg != 0:
18618 * pidarg > 0 means pidarg is pid of a specific process to wait for
18619 * pidarg == -1 means wait for any child
18620 * pidarg < -1 means wait for any child whose process group = -pidarg
18621 */
18622 children = 0;
18623 for (rp = &mproc[0]; rp < &mproc[NR_PROCS]; rp++) {
18624 if ( (rp->mp_flags & IN_USE) && rp->mp_parent == who) {
18625 /* The value of pidarg determines which children qualify. */
18626 if (pidarg > 0 && pidarg != rp->mp_pid) continue;
18627 if (pidarg < -1 && -pidarg != rp->mp_procgrp) continue;
18628
18629 children++; /* this child is acceptable */
18630 if (rp->mp_flags & ZOMBIE) {
18631 /* This child meets the pid test and has exited. */
18632 cleanup(rp); /* this child has already exited */
18633 return(SUSPEND);
18634 }
MINIX SOURCE CODE File: servers/pm/forkexit.c 885
18635 if ((rp->mp_flags & STOPPED) && rp->mp_sigstatus) {
18636 /* This child meets the pid test and is being traced.*/
18637 mp->mp_reply.reply_res2 = 0177|(rp->mp_sigstatus << 8);
18638 rp->mp_sigstatus = 0;
18639 return(rp->mp_pid);
18640 }
18641 }
18642 }
18643
18644 /* No qualifying child has exited. Wait for one, unless none exists. */
18645 if (children > 0) {
18646 /* At least 1 child meets the pid test exists, but has not exited. */
18647 if (options & WNOHANG) return(0); /* parent does not want to wait */
18648 mp->mp_flags |= WAITING; /* parent wants to wait */
18649 mp->mp_wpid = (pid_t) pidarg; /* save pid for later */
18650 return(SUSPEND); /* do not reply, let it wait */
18651 } else {
18652 /* No child even meets the pid test. Return error immediately. */
18653 return(ECHILD); /* no - parent has no children */
18654 }
18655 }
18657 /*===========================================================================*
18658 * cleanup *
18659 *===========================================================================*/
18660 PRIVATE void cleanup(child)
18661 register struct mproc *child; /* tells which process is exiting */
18662 {
18663 /* Finish off the exit of a process. The process has exited or been killed
18664 * by a signal, and its parent is waiting.
18665 */
18666 struct mproc *parent = &mproc[child->mp_parent];
18667 int exitstatus;
18668
18669 /* Wake up the parent by sending the reply message. */
18670 exitstatus = (child->mp_exitstatus << 8) | (child->mp_sigstatus & 0377);
18671 parent->mp_reply.reply_res2 = exitstatus;
18672 setreply(child->mp_parent, child->mp_pid);
18673 parent->mp_flags &= ˜WAITING; /* parent no longer waiting */
18674
18675 /* Release the process table entry and reinitialize some field. */
18676 child->mp_pid = 0;
18677 child->mp_flags = 0;
18678 child->mp_child_utime = 0;
18679 child->mp_child_stime = 0;
18680 procs_in_use--;
18681 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/exec.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
18700 /* This file handles the EXEC system call. It performs the work as follows:
18701 * - see if the permissions allow the file to be executed
18702 * - read the header and extract the sizes
18703 * - fetch the initial args and environment from the user space
18704 * - allocate the memory for the new process
886 File: servers/pm/exec.c MINIX SOURCE CODE
18705 * - copy the initial stack from PM to the process
18706 * - read in the text and data segments and copy to the process
18707 * - take care of setuid and setgid bits
18708 * - fix up ’mproc’ table
18709 * - tell kernel about EXEC
18710 * - save offset to initial argc (for ps)
18711 *
18712 * The entry points into this file are:
18713 * do_exec: perform the EXEC system call
18714 * rw_seg: read or write a segment from or to a file
18715 * find_share: find a process whose text segment can be shared
18716 */
18717
18718 #include "pm.h"
18719 #include <sys/stat.h>
18720 #include <minix/callnr.h>
18721 #include <minix/com.h>
18722 #include <a.out.h>
18723 #include <signal.h>
18724 #include <string.h>
18725 #include "mproc.h"
18726 #include "param.h"
18727
18728 FORWARD _PROTOTYPE( int new_mem, (struct mproc *sh_mp, vir_bytes text_bytes,
18729 vir_bytes data_bytes, vir_bytes bss_bytes,
18730 vir_bytes stk_bytes, phys_bytes tot_bytes) );
18731 FORWARD _PROTOTYPE( void patch_ptr, (char stack[ARG_MAX], vir_bytes base) );
18732 FORWARD _PROTOTYPE( int insert_arg, (char stack[ARG_MAX],
18733 vir_bytes *stk_bytes, char *arg, int replace) );
18734 FORWARD _PROTOTYPE( char *patch_stack, (int fd, char stack[ARG_MAX],
18735 vir_bytes *stk_bytes, char *script) );
18736 FORWARD _PROTOTYPE( int read_header, (int fd, int *ft, vir_bytes *text_bytes,
18737 vir_bytes *data_bytes, vir_bytes *bss_bytes,
18738 phys_bytes *tot_bytes, long *sym_bytes, vir_clicks sc,
18739 vir_bytes *pc) );
18740
18741 #define ESCRIPT (-2000) /* Returned by read_header for a #! script. */
18742 #define PTRSIZE sizeof(char *) /* Size of pointers in argv[] and envp[]. */
18743
18744 /*===========================================================================*
18745 * do_exec *
18746 *===========================================================================*/
18747 PUBLIC int do_exec()
18748 {
18749 /* Perform the execve(name, argv, envp) call. The user library builds a
18750 * complete stack image, including pointers, args, environ, etc. The stack
18751 * is copied to a buffer inside PM, and then to the new core image.
18752 */
18753 register struct mproc *rmp;
18754 struct mproc *sh_mp;
18755 int m, r, fd, ft, sn;
18756 static char mbuf[ARG_MAX]; /* buffer for stack and zeroes */
18757 static char name_buf[PATH_MAX]; /* the name of the file to exec */
18758 char *new_sp, *name, *basename;
18759 vir_bytes src, dst, text_bytes, data_bytes, bss_bytes, stk_bytes, vsp;
18760 phys_bytes tot_bytes; /* total space for program, including gap */
18761 long sym_bytes;
18762 vir_clicks sc;
18763 struct stat s_buf[2], *s_p;
18764 vir_bytes pc;
MINIX SOURCE CODE File: servers/pm/exec.c 887
18765
18766 /* Do some validity checks. */
18767 rmp = mp;
18768 stk_bytes = (vir_bytes) m_in.stack_bytes;
18769 if (stk_bytes > ARG_MAX) return(ENOMEM); /* stack too big */
18770 if (m_in.exec_len <= 0 || m_in.exec_len > PATH_MAX) return(EINVAL);
18771
18772 /* Get the exec file name and see if the file is executable. */
18773 src = (vir_bytes) m_in.exec_name;
18774 dst = (vir_bytes) name_buf;
18775 r = sys_datacopy(who, (vir_bytes) src,
18776 PM_PROC_NR, (vir_bytes) dst, (phys_bytes) m_in.exec_len);
18777 if (r != OK) return(r); /* file name not in user data segment */
18778
18779 /* Fetch the stack from the user before destroying the old core image. */
18780 src = (vir_bytes) m_in.stack_ptr;
18781 dst = (vir_bytes) mbuf;
18782 r = sys_datacopy(who, (vir_bytes) src,
18783 PM_PROC_NR, (vir_bytes) dst, (phys_bytes)stk_bytes);
18784 /* can’t fetch stack (e.g. bad virtual addr) */
18785 if (r != OK) return(EACCES);
18786
18787 r = 0; /* r = 0 (first attempt), or 1 (interpreted script) */
18788 name = name_buf; /* name of file to exec. */
18789 do {
18790 s_p = &s_buf[r];
18791 tell_fs(CHDIR, who, FALSE, 0); /* switch to the user’s FS environ */
18792 fd = allowed(name, s_p, X_BIT); /* is file executable? */
18793 if (fd < 0) return(fd); /* file was not executable */
18794
18795 /* Read the file header and extract the segment sizes. */
18796 sc = (stk_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
18797
18798 m = read_header(fd, &ft, &text_bytes, &data_bytes, &bss_bytes,
18799 &tot_bytes, &sym_bytes, sc, &pc);
18800 if (m != ESCRIPT || ++r > 1) break;
18801 } while ((name = patch_stack(fd, mbuf, &stk_bytes, name_buf)) != NULL);
18802
18803 if (m < 0) {
18804 close(fd); /* something wrong with header */
18805 return(stk_bytes > ARG_MAX ? ENOMEM : ENOEXEC);
18806 }
18807
18808 /* Can the process’ text be shared with that of one already running? */
18809 sh_mp = find_share(rmp, s_p->st_ino, s_p->st_dev, s_p->st_ctime);
18810
18811 /* Allocate new memory and release old memory. Fix map and tell kernel. */
18812 r = new_mem(sh_mp, text_bytes, data_bytes, bss_bytes, stk_bytes, tot_bytes);
18813 if (r != OK) {
18814 close(fd); /* insufficient core or program too big */
18815 return(r);
18816 }
18817
18818 /* Save file identification to allow it to be shared. */
18819 rmp->mp_ino = s_p->st_ino;
18820 rmp->mp_dev = s_p->st_dev;
18821 rmp->mp_ctime = s_p->st_ctime;
18822
18823 /* Patch up stack and copy it from PM to new core image. */
18824 vsp = (vir_bytes) rmp->mp_seg[S].mem_vir << CLICK_SHIFT;
888 File: servers/pm/exec.c MINIX SOURCE CODE
18825 vsp += (vir_bytes) rmp->mp_seg[S].mem_len << CLICK_SHIFT;
18826 vsp -= stk_bytes;
18827 patch_ptr(mbuf, vsp);
18828 src = (vir_bytes) mbuf;
18829 r = sys_datacopy(PM_PROC_NR, (vir_bytes) src,
18830 who, (vir_bytes) vsp, (phys_bytes)stk_bytes);
18831 if (r != OK) panic(__FILE__,"do_exec stack copy err on", who);
18832
18833 /* Read in text and data segments. */
18834 if (sh_mp != NULL) {
18835 lseek(fd, (off_t) text_bytes, SEEK_CUR); /* shared: skip text */
18836 } else {
18837 rw_seg(0, fd, who, T, text_bytes);
18838 }
18839 rw_seg(0, fd, who, D, data_bytes);
18840
18841 close(fd); /* don’t need exec file any more */
18842
18843 /* Take care of setuid/setgid bits. */
18844 if ((rmp->mp_flags & TRACED) == 0) { /* suppress if tracing */
18845 if (s_buf[0].st_mode & I_SET_UID_BIT) {
18846 rmp->mp_effuid = s_buf[0].st_uid;
18847 tell_fs(SETUID,who, (int)rmp->mp_realuid, (int)rmp->mp_effuid);
18848 }
18849 if (s_buf[0].st_mode & I_SET_GID_BIT) {
18850 rmp->mp_effgid = s_buf[0].st_gid;
18851 tell_fs(SETGID,who, (int)rmp->mp_realgid, (int)rmp->mp_effgid);
18852 }
18853 }
18854
18855 /* Save offset to initial argc (for ps) */
18856 rmp->mp_procargs = vsp;
18857
18858 /* Fix ’mproc’ fields, tell kernel that exec is done, reset caught sigs. */
18859 for (sn = 1; sn <= _NSIG; sn++) {
18860 if (sigismember(&rmp->mp_catch, sn)) {
18861 sigdelset(&rmp->mp_catch, sn);
18862 rmp->mp_sigact[sn].sa_handler = SIG_DFL;
18863 sigemptyset(&rmp->mp_sigact[sn].sa_mask);
18864 }
18865 }
18866
18867 rmp->mp_flags &= ˜SEPARATE; /* turn off SEPARATE bit */
18868 rmp->mp_flags |= ft; /* turn it on for separate I & D files */
18869 new_sp = (char *) vsp;
18870
18871 tell_fs(EXEC, who, 0, 0); /* allow FS to handle FD_CLOEXEC files */
18872
18873 /* System will save command line for debugging, ps(1) output, etc. */
18874 basename = strrchr(name, ’/’);
18875 if (basename == NULL) basename = name; else basename++;
18876 strncpy(rmp->mp_name, basename, PROC_NAME_LEN-1);
18877 rmp->mp_name[PROC_NAME_LEN] = ’\0’;
18878 sys_exec(who, new_sp, basename, pc);
18879
18880 /* Cause a signal if this process is traced. */
18881 if (rmp->mp_flags & TRACED) check_sig(rmp->mp_pid, SIGTRAP);
18882
18883 return(SUSPEND); /* no reply, new program just runs */
18884 }
MINIX SOURCE CODE File: servers/pm/exec.c 889
18886 /*===========================================================================*
18887 * read_header *
18888 *===========================================================================*/
18889 PRIVATE int read_header(fd, ft, text_bytes, data_bytes, bss_bytes,
18890 tot_bytes, sym_bytes, sc, pc)
18891 int fd; /* file descriptor for reading exec file */
18892 int *ft; /* place to return ft number */
18893 vir_bytes *text_bytes; /* place to return text size */
18894 vir_bytes *data_bytes; /* place to return initialized data size */
18895 vir_bytes *bss_bytes; /* place to return bss size */
18896 phys_bytes *tot_bytes; /* place to return total size */
18897 long *sym_bytes; /* place to return symbol table size */
18898 vir_clicks sc; /* stack size in clicks */
18899 vir_bytes *pc; /* program entry point (initial PC) */
18900 {
18901 /* Read the header and extract the text, data, bss and total sizes from it. */
18902
18903 int m, ct;
18904 vir_clicks tc, dc, s_vir, dvir;
18905 phys_clicks totc;
18906 struct exec hdr; /* a.out header is read in here */
18907
18908 /* Read the header and check the magic number. The standard MINIX header
18909 * is defined in <a.out.h>. It consists of 8 chars followed by 6 longs.
18910 * Then come 4 more longs that are not used here.
18911 * Byte 0: magic number 0x01
18912 * Byte 1: magic number 0x03
18913 * Byte 2: normal = 0x10 (not checked, 0 is OK), separate I/D = 0x20
18914 * Byte 3: CPU type, Intel 16 bit = 0x04, Intel 32 bit = 0x10,
18915 * Motorola = 0x0B, Sun SPARC = 0x17
18916 * Byte 4: Header length = 0x20
18917 * Bytes 5-7 are not used.
18918 *
18919 * Now come the 6 longs
18920 * Bytes 8-11: size of text segments in bytes
18921 * Bytes 12-15: size of initialized data segment in bytes
18922 * Bytes 16-19: size of bss in bytes
18923 * Bytes 20-23: program entry point
18924 * Bytes 24-27: total memory allocated to program (text, data + stack)
18925 * Bytes 28-31: size of symbol table in bytes
18926 * The longs are represented in a machine dependent order,
18927 * little-endian on the 8088, big-endian on the 68000.
18928 * The header is followed directly by the text and data segments, and the
18929 * symbol table (if any). The sizes are given in the header. Only the
18930 * text and data segments are copied into memory by exec. The header is
18931 * used here only. The symbol table is for the benefit of a debugger and
18932 * is ignored here.
18933 */
18934
18935 if ((m= read(fd, &hdr, A_MINHDR)) < 2) return(ENOEXEC);
18936
18937 /* Interpreted script? */
18938 if (((char *) &hdr)[0] == ’#’ && ((char *) &hdr)[1] == ’!’) return(ESCRIPT);
18939
18940 if (m != A_MINHDR) return(ENOEXEC);
18941
18942 /* Check magic number, cpu type, and flags. */
18943 if (BADMAG(hdr)) return(ENOEXEC);
18944 if (hdr.a_cpu != A_I80386) return(ENOEXEC);
890 File: servers/pm/exec.c MINIX SOURCE CODE
18945 if ((hdr.a_flags & ˜(A_NSYM | A_EXEC | A_SEP)) != 0) return(ENOEXEC);
18946
18947 *ft = ( (hdr.a_flags & A_SEP) ? SEPARATE : 0); /* separate I & D or not */
18948
18949 /* Get text and data sizes. */
18950 *text_bytes = (vir_bytes) hdr.a_text; /* text size in bytes */
18951 *data_bytes = (vir_bytes) hdr.a_data; /* data size in bytes */
18952 *bss_bytes = (vir_bytes) hdr.a_bss; /* bss size in bytes */
18953 *tot_bytes = hdr.a_total; /* total bytes to allocate for prog */
18954 *sym_bytes = hdr.a_syms; /* symbol table size in bytes */
18955 if (*tot_bytes == 0) return(ENOEXEC);
18956
18957 if (*ft != SEPARATE) {
18958 /* If I & D space is not separated, it is all considered data. Text=0*/
18959 *data_bytes += *text_bytes;
18960 *text_bytes = 0;
18961 }
18962 *pc = hdr.a_entry; /* initial address to start execution */
18963
18964 /* Check to see if segment sizes are feasible. */
18965 tc = ((unsigned long) *text_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
18966 dc = (*data_bytes + *bss_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
18967 totc = (*tot_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
18968 if (dc >= totc) return(ENOEXEC); /* stack must be at least 1 click */
18969 dvir = (*ft == SEPARATE ? 0 : tc);
18970 s_vir = dvir + (totc - sc);
18971 m = (dvir + dc > s_vir) ? ENOMEM : OK;
18972 ct = hdr.a_hdrlen & BYTE; /* header length */
18973 if (ct > A_MINHDR) lseek(fd, (off_t) ct, SEEK_SET); /* skip unused hdr */
18974 return(m);
18975 }
18977 /*===========================================================================*
18978 * new_mem *
18979 *===========================================================================*/
18980 PRIVATE int new_mem(sh_mp, text_bytes, data_bytes,
18981 bss_bytes,stk_bytes,tot_bytes)
18982 struct mproc *sh_mp; /* text can be shared with this process */
18983 vir_bytes text_bytes; /* text segment size in bytes */
18984 vir_bytes data_bytes; /* size of initialized data in bytes */
18985 vir_bytes bss_bytes; /* size of bss in bytes */
18986 vir_bytes stk_bytes; /* size of initial stack segment in bytes */
18987 phys_bytes tot_bytes; /* total memory to allocate, including gap */
18988 {
18989 /* Allocate new memory and release the old memory. Change the map and report
18990 * the new map to the kernel. Zero the new core image’s bss, gap and stack.
18991 */
18992
18993 register struct mproc *rmp = mp;
18994 vir_clicks text_clicks, data_clicks, gap_clicks, stack_clicks, tot_clicks;
18995 phys_clicks new_base;
18996 phys_bytes bytes, base, bss_offset;
18997 int s;
18998
18999 /* No need to allocate text if it can be shared. */
19000 if (sh_mp != NULL) text_bytes = 0;
19001
19002 /* Allow the old data to be swapped out to make room. (Which is really a
19003 * waste of time, because we are going to throw it away anyway.)
19004 */
MINIX SOURCE CODE File: servers/pm/exec.c 891
19005 rmp->mp_flags |= WAITING;
19006
19007 /* Acquire the new memory. Each of the 4 parts: text, (data+bss), gap,
19008 * and stack occupies an integral number of clicks, starting at click
19009 * boundary. The data and bss parts are run together with no space.
19010 */
19011 text_clicks = ((unsigned long) text_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
19012 data_clicks = (data_bytes + bss_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
19013 stack_clicks = (stk_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
19014 tot_clicks = (tot_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
19015 gap_clicks = tot_clicks - data_clicks - stack_clicks;
19016 if ( (int) gap_clicks < 0) return(ENOMEM);
19017
19018 /* Try to allocate memory for the new process. */
19019 new_base = alloc_mem(text_clicks + tot_clicks);
19020 if (new_base == NO_MEM) return(ENOMEM);
19021
19022 /* We’ve got memory for the new core image. Release the old one. */
19023 rmp = mp;
19024
19025 if (find_share(rmp, rmp->mp_ino, rmp->mp_dev, rmp->mp_ctime) == NULL) {
19026 /* No other process shares the text segment, so free it. */
19027 free_mem(rmp->mp_seg[T].mem_phys, rmp->mp_seg[T].mem_len);
19028 }
19029 /* Free the data and stack segments. */
19030 free_mem(rmp->mp_seg[D].mem_phys,
19031 rmp->mp_seg[S].mem_vir + rmp->mp_seg[S].mem_len - rmp->mp_seg[D].mem_vir);
19032
19033 /* We have now passed the point of no return. The old core image has been
19034 * forever lost, memory for a new core image has been allocated. Set up
19035 * and report new map.
19036 */
19037 if (sh_mp != NULL) {
19038 /* Share the text segment. */
19039 rmp->mp_seg[T] = sh_mp->mp_seg[T];
19040 } else {
19041 rmp->mp_seg[T].mem_phys = new_base;
19042 rmp->mp_seg[T].mem_vir = 0;
19043 rmp->mp_seg[T].mem_len = text_clicks;
19044 }
19045 rmp->mp_seg[D].mem_phys = new_base + text_clicks;
19046 rmp->mp_seg[D].mem_vir = 0;
19047 rmp->mp_seg[D].mem_len = data_clicks;
19048 rmp->mp_seg[S].mem_phys = rmp->mp_seg[D].mem_phys + data_clicks + gap_clicks;
19049 rmp->mp_seg[S].mem_vir = rmp->mp_seg[D].mem_vir + data_clicks + gap_clicks;
19050 rmp->mp_seg[S].mem_len = stack_clicks;
19051
19052 sys_newmap(who, rmp->mp_seg); /* report new map to the kernel */
19053
19054 /* The old memory may have been swapped out, but the new memory is real. */
19055 rmp->mp_flags &= ˜(WAITING|ONSWAP|SWAPIN);
19056
19057 /* Zero the bss, gap, and stack segment. */
19058 bytes = (phys_bytes)(data_clicks + gap_clicks + stack_clicks) << CLICK_SHIFT;
19059 base = (phys_bytes) rmp->mp_seg[D].mem_phys << CLICK_SHIFT;
19060 bss_offset = (data_bytes >> CLICK_SHIFT) << CLICK_SHIFT;
19061 base += bss_offset;
19062 bytes -= bss_offset;
19063
19064 if ((s=sys_memset(0, base, bytes)) != OK) {
892 File: servers/pm/exec.c MINIX SOURCE CODE
19065 panic(__FILE__,"new_mem can’t zero", s);
19066 }
19067
19068 return(OK);
19069 }
19071 /*===========================================================================*
19072 * patch_ptr *
19073 *===========================================================================*/
19074 PRIVATE void patch_ptr(stack, base)
19075 char stack[ARG_MAX]; /* pointer to stack image within PM */
19076 vir_bytes base; /* virtual address of stack base inside user */
19077 {
19078 /* When doing an exec(name, argv, envp) call, the user builds up a stack
19079 * image with arg and env pointers relative to the start of the stack. Now
19080 * these pointers must be relocated, since the stack is not positioned at
19081 * address 0 in the user’s address space.
19082 */
19083
19084 char **ap, flag;
19085 vir_bytes v;
19086
19087 flag = 0; /* counts number of 0-pointers seen */
19088 ap = (char **) stack; /* points initially to ’nargs’ */
19089 ap++; /* now points to argv[0] */
19090 while (flag < 2) {
19091 if (ap >= (char **) &stack[ARG_MAX]) return; /* too bad */
19092 if (*ap != NULL) {
19093 v = (vir_bytes) *ap; /* v is relative pointer */
19094 v += base; /* relocate it */
19095 *ap = (char *) v; /* put it back */
19096 } else {
19097 flag++;
19098 }
19099 ap++;
19100 }
19101 }
19103 /*===========================================================================*
19104 * insert_arg *
19105 *===========================================================================*/
19106 PRIVATE int insert_arg(stack, stk_bytes, arg, replace)
19107 char stack[ARG_MAX]; /* pointer to stack image within PM */
19108 vir_bytes *stk_bytes; /* size of initial stack */
19109 char *arg; /* argument to prepend/replace as new argv[0] */
19110 int replace;
19111 {
19112 /* Patch the stack so that arg will become argv[0]. Be careful, the stack may
19113 * be filled with garbage, although it normally looks like this:
19114 * nargs argv[0] ... argv[nargs-1] NULL envp[0] ... NULL
19115 * followed by the strings "pointed" to by the argv[i] and the envp[i]. The
19116 * pointers are really offsets from the start of stack.
19117 * Return true iff the operation succeeded.
19118 */
19119 int offset, a0, a1, old_bytes = *stk_bytes;
19120
19121 /* Prepending arg adds at least one string and a zero byte. */
19122 offset = strlen(arg) + 1;
19123
19124 a0 = (int) ((char **) stack)[1]; /* argv[0] */
MINIX SOURCE CODE File: servers/pm/exec.c 893
19125 if (a0 < 4 * PTRSIZE || a0 >= old_bytes) return(FALSE);
19126
19127 a1 = a0; /* a1 will point to the strings to be moved */
19128 if (replace) {
19129 /* Move a1 to the end of argv[0][] (argv[1] if nargs > 1). */
19130 do {
19131 if (a1 == old_bytes) return(FALSE);
19132 --offset;
19133 } while (stack[a1++] != 0);
19134 } else {
19135 offset += PTRSIZE; /* new argv[0] needs new pointer in argv[] */
19136 a0 += PTRSIZE; /* location of new argv[0][]. */
19137 }
19138
19139 /* stack will grow by offset bytes (or shrink by -offset bytes) */
19140 if ((*stk_bytes += offset) > ARG_MAX) return(FALSE);
19141
19142 /* Reposition the strings by offset bytes */
19143 memmove(stack + a1 + offset, stack + a1, old_bytes - a1);
19144
19145 strcpy(stack + a0, arg); /* Put arg in the new space. */
19146
19147 if (!replace) {
19148 /* Make space for a new argv[0]. */
19149 memmove(stack + 2 * PTRSIZE, stack + 1 * PTRSIZE, a0 - 2 * PTRSIZE);
19150
19151 ((char **) stack)[0]++; /* nargs++; */
19152 }
19153 /* Now patch up argv[] and envp[] by offset. */
19154 patch_ptr(stack, (vir_bytes) offset);
19155 ((char **) stack)[1] = (char *) a0; /* set argv[0] correctly */
19156 return(TRUE);
19157 }
19159 /*===========================================================================*
19160 * patch_stack *
19161 *===========================================================================*/
19162 PRIVATE char *patch_stack(fd, stack, stk_bytes, script)
19163 int fd; /* file descriptor to open script file */
19164 char stack[ARG_MAX]; /* pointer to stack image within PM */
19165 vir_bytes *stk_bytes; /* size of initial stack */
19166 char *script; /* name of script to interpret */
19167 {
19168 /* Patch the argument vector to include the path name of the script to be
19169 * interpreted, and all strings on the #! line. Returns the path name of
19170 * the interpreter.
19171 */
19172 char *sp, *interp = NULL;
19173 int n;
19174 enum { INSERT=FALSE, REPLACE=TRUE };
19175
19176 /* Make script[] the new argv[0]. */
19177 if (!insert_arg(stack, stk_bytes, script, REPLACE)) return(NULL);
19178
19179 if (lseek(fd, 2L, 0) == -1 /* just behind the #! */
19180 || (n= read(fd, script, PATH_MAX)) < 0 /* read line one */
19181 || (sp= memchr(script, ’\n’, n)) == NULL) /* must be a proper line */
19182 return(NULL);
19183
19184 /* Move sp backwards through script[], prepending each string to stack. */
894 File: servers/pm/exec.c MINIX SOURCE CODE
19185 for (;;) {
19186 /* skip spaces behind argument. */
19187 while (sp > script && (*--sp == ’ ’ || *sp == ’\t’)) {}
19188 if (sp == script) break;
19189
19190 sp[1] = 0;
19191 /* Move to the start of the argument. */
19192 while (sp > script && sp[-1] != ’ ’ && sp[-1] != ’\t’) --sp;
19193
19194 interp = sp;
19195 if (!insert_arg(stack, stk_bytes, sp, INSERT)) return(NULL);
19196 }
19197
19198 /* Round *stk_bytes up to the size of a pointer for alignment contraints. */
19199 *stk_bytes= ((*stk_bytes + PTRSIZE - 1) / PTRSIZE) * PTRSIZE;
19200
19201 close(fd);
19202 return(interp);
19203 }
19205 /*===========================================================================*
19206 * rw_seg *
19207 *===========================================================================*/
19208 PUBLIC void rw_seg(rw, fd, proc, seg, seg_bytes0)
19209 int rw; /* 0 = read, 1 = write */
19210 int fd; /* file descriptor to read from / write to */
19211 int proc; /* process number */
19212 int seg; /* T, D, or S */
19213 phys_bytes seg_bytes0; /* how much is to be transferred? */
19214 {
19215 /* Transfer text or data from/to a file and copy to/from a process segment.
19216 * This procedure is a little bit tricky. The logical way to transfer a
19217 * segment would be block by block and copying each block to/from the user
19218 * space one at a time. This is too slow, so we do something dirty here,
19219 * namely send the user space and virtual address to the file system in the
19220 * upper 10 bits of the file descriptor, and pass it the user virtual address
19221 * instead of a PM address. The file system extracts these parameters when
19222 * gets a read or write call from the process manager, which is the only
19223 * process that is permitted to use this trick. The file system then copies
19224 * the whole segment directly to/from user space, bypassing PM completely.
19225 *
19226 * The byte count on read is usually smaller than the segment count, because
19227 * a segment is padded out to a click multiple, and the data segment is only
19228 * partially initialized.
19229 */
19230
19231 int new_fd, bytes, r;
19232 char *ubuf_ptr;
19233 struct mem_map *sp = &mproc[proc].mp_seg[seg];
19234 phys_bytes seg_bytes = seg_bytes0;
19235
19236 new_fd = (proc << 7) | (seg << 5) | fd;
19237 ubuf_ptr = (char *) ((vir_bytes) sp->mem_vir << CLICK_SHIFT);
19238
19239 while (seg_bytes != 0) {
19240 #define PM_CHUNK_SIZE 8192
19241 bytes = MIN((INT_MAX / PM_CHUNK_SIZE) * PM_CHUNK_SIZE, seg_bytes);
19242 if (rw == 0) {
19243 r = read(new_fd, ubuf_ptr, bytes);
19244 } else {
MINIX SOURCE CODE File: servers/pm/exec.c 895
19245 r = write(new_fd, ubuf_ptr, bytes);
19246 }
19247 if (r != bytes) break;
19248 ubuf_ptr += bytes;
19249 seg_bytes -= bytes;
19250 }
19251 }
19253 /*===========================================================================*
19254 * find_share *
19255 *===========================================================================*/
19256 PUBLIC struct mproc *find_share(mp_ign, ino, dev, ctime)
19257 struct mproc *mp_ign; /* process that should not be looked at */
19258 ino_t ino; /* parameters that uniquely identify a file */
19259 dev_t dev;
19260 time_t ctime;
19261 {
19262 /* Look for a process that is the file <ino, dev, ctime> in execution. Don’t
19263 * accidentally "find" mp_ign, because it is the process on whose behalf this
19264 * call is made.
19265 */
19266 struct mproc *sh_mp;
19267 for (sh_mp = &mproc[0]; sh_mp < &mproc[NR_PROCS]; sh_mp++) {
19268
19269 if (!(sh_mp->mp_flags & SEPARATE)) continue;
19270 if (sh_mp == mp_ign) continue;
19271 if (sh_mp->mp_ino != ino) continue;
19272 if (sh_mp->mp_dev != dev) continue;
19273 if (sh_mp->mp_ctime != ctime) continue;
19274 return sh_mp;
19275 }
19276 return(NULL);
19277 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/break.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
19300 /* The MINIX model of memory allocation reserves a fixed amount of memory for
19301 * the combined text, data, and stack segments. The amount used for a child
19302 * process created by FORK is the same as the parent had. If the child does
19303 * an EXEC later, the new size is taken from the header of the file EXEC’ed.
19304 *
19305 * The layout in memory consists of the text segment, followed by the data
19306 * segment, followed by a gap (unused memory), followed by the stack segment.
19307 * The data segment grows upward and the stack grows downward, so each can
19308 * take memory from the gap. If they meet, the process must be killed. The
19309 * procedures in this file deal with the growth of the data and stack segments.
19310 *
19311 * The entry points into this file are:
19312 * do_brk: BRK/SBRK system calls to grow or shrink the data segment
19313 * adjust: see if a proposed segment adjustment is allowed
19314 * size_ok: see if the segment sizes are feasible
19315 */
19316
19317 #include "pm.h"
19318 #include <signal.h>
19319 #include "mproc.h"
896 File: servers/pm/break.c MINIX SOURCE CODE
19320 #include "param.h"
19321
19322 #define DATA_CHANGED 1 /* flag value when data segment size changed */
19323 #define STACK_CHANGED 2 /* flag value when stack size changed */
19324
19325 /*===========================================================================*
19326 * do_brk *
19327 *===========================================================================*/
19328 PUBLIC int do_brk()
19329 {
19330 /* Perform the brk(addr) system call.
19331 *
19332 * The call is complicated by the fact that on some machines (e.g., 8088),
19333 * the stack pointer can grow beyond the base of the stack segment without
19334 * anybody noticing it.
19335 * The parameter, ’addr’ is the new virtual address in D space.
19336 */
19337
19338 register struct mproc *rmp;
19339 int r;
19340 vir_bytes v, new_sp;
19341 vir_clicks new_clicks;
19342
19343 rmp = mp;
19344 v = (vir_bytes) m_in.addr;
19345 new_clicks = (vir_clicks) ( ((long) v + CLICK_SIZE - 1) >> CLICK_SHIFT);
19346 if (new_clicks < rmp->mp_seg[D].mem_vir) {
19347 rmp->mp_reply.reply_ptr = (char *) -1;
19348 return(ENOMEM);
19349 }
19350 new_clicks -= rmp->mp_seg[D].mem_vir;
19351 if ((r=get_stack_ptr(who, &new_sp)) != OK) /* ask kernel for sp value */
19352 panic(__FILE__,"couldn’t get stack pointer", r);
19353 r = adjust(rmp, new_clicks, new_sp);
19354 rmp->mp_reply.reply_ptr = (r == OK ? m_in.addr : (char *) -1);
19355 return(r); /* return new address or -1 */
19356 }
19358 /*===========================================================================*
19359 * adjust *
19360 *===========================================================================*/
19361 PUBLIC int adjust(rmp, data_clicks, sp)
19362 register struct mproc *rmp; /* whose memory is being adjusted? */
19363 vir_clicks data_clicks; /* how big is data segment to become? */
19364 vir_bytes sp; /* new value of sp */
19365 {
19366 /* See if data and stack segments can coexist, adjusting them if need be.
19367 * Memory is never allocated or freed. Instead it is added or removed from the
19368 * gap between data segment and stack segment. If the gap size becomes
19369 * negative, the adjustment of data or stack fails and ENOMEM is returned.
19370 */
19371
19372 register struct mem_map *mem_sp, *mem_dp;
19373 vir_clicks sp_click, gap_base, lower, old_clicks;
19374 int changed, r, ft;
19375 long base_of_stack, delta; /* longs avoid certain problems */
19376
19377 mem_dp = &rmp->mp_seg[D]; /* pointer to data segment map */
19378 mem_sp = &rmp->mp_seg[S]; /* pointer to stack segment map */
19379 changed = 0; /* set when either segment changed */
MINIX SOURCE CODE File: servers/pm/break.c 897
19380
19381 if (mem_sp->mem_len == 0) return(OK); /* don’t bother init */
19382
19383 /* See if stack size has gone negative (i.e., sp too close to 0xFFFF...) */
19384 base_of_stack = (long) mem_sp->mem_vir + (long) mem_sp->mem_len;
19385 sp_click = sp >> CLICK_SHIFT; /* click containing sp */
19386 if (sp_click >= base_of_stack) return(ENOMEM); /* sp too high */
19387
19388 /* Compute size of gap between stack and data segments. */
19389 delta = (long) mem_sp->mem_vir - (long) sp_click;
19390 lower = (delta > 0 ? sp_click : mem_sp->mem_vir);
19391
19392 /* Add a safety margin for future stack growth. Impossible to do right. */
19393 #define SAFETY_BYTES (384 * sizeof(char *))
19394 #define SAFETY_CLICKS ((SAFETY_BYTES + CLICK_SIZE - 1) / CLICK_SIZE)
19395 gap_base = mem_dp->mem_vir + data_clicks + SAFETY_CLICKS;
19396 if (lower < gap_base) return(ENOMEM); /* data and stack collided */
19397
19398 /* Update data length (but not data orgin) on behalf of brk() system call. */
19399 old_clicks = mem_dp->mem_len;
19400 if (data_clicks != mem_dp->mem_len) {
19401 mem_dp->mem_len = data_clicks;
19402 changed |= DATA_CHANGED;
19403 }
19404
19405 /* Update stack length and origin due to change in stack pointer. */
19406 if (delta > 0) {
19407 mem_sp->mem_vir -= delta;
19408 mem_sp->mem_phys -= delta;
19409 mem_sp->mem_len += delta;
19410 changed |= STACK_CHANGED;
19411 }
19412
19413 /* Do the new data and stack segment sizes fit in the address space? */
19414 ft = (rmp->mp_flags & SEPARATE);
19415 r = (rmp->mp_seg[D].mem_vir + rmp->mp_seg[D].mem_len >
19416 rmp->mp_seg[S].mem_vir) ? ENOMEM : OK;
19417 if (r == OK) {
19418 if (changed) sys_newmap((int)(rmp - mproc), rmp->mp_seg);
19419 return(OK);
19420 }
19421
19422 /* New sizes don’t fit or require too many page/segment registers. Restore.*/
19423 if (changed & DATA_CHANGED) mem_dp->mem_len = old_clicks;
19424 if (changed & STACK_CHANGED) {
19425 mem_sp->mem_vir += delta;
19426 mem_sp->mem_phys += delta;
19427 mem_sp->mem_len -= delta;
19428 }
19429 return(ENOMEM);
19430 }
898 File: servers/pm/signal.c MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/signal.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
19500 /* This file handles signals, which are asynchronous events and are generally
19501 * a messy and unpleasant business. Signals can be generated by the KILL
19502 * system call, or from the keyboard (SIGINT) or from the clock (SIGALRM).
19503 * In all cases control eventually passes to check_sig() to see which processes
19504 * can be signaled. The actual signaling is done by sig_proc().
19505 *
19506 * The entry points into this file are:
19507 * do_sigaction: perform the SIGACTION system call
19508 * do_sigpending: perform the SIGPENDING system call
19509 * do_sigprocmask: perform the SIGPROCMASK system call
19510 * do_sigreturn: perform the SIGRETURN system call
19511 * do_sigsuspend: perform the SIGSUSPEND system call
19512 * do_kill: perform the KILL system call
19513 * do_alarm: perform the ALARM system call by calling set_alarm()
19514 * set_alarm: tell the clock task to start or stop a timer
19515 * do_pause: perform the PAUSE system call
19516 * ksig_pending: the kernel notified about pending signals
19517 * sig_proc: interrupt or terminate a signaled process
19518 * check_sig: check which processes to signal with sig_proc()
19519 * check_pending: check if a pending signal can now be delivered
19520 */
19521
19522 #include "pm.h"
19523 #include <sys/stat.h>
19524 #include <sys/ptrace.h>
19525 #include <minix/callnr.h>
19526 #include <minix/com.h>
19527 #include <signal.h>
19528 #include <sys/sigcontext.h>
19529 #include <string.h>
19530 #include "mproc.h"
19531 #include "param.h"
19532
19533 #define CORE_MODE 0777 /* mode to use on core image files */
19534 #define DUMPED 0200 /* bit set in status when core dumped */
19535
19536 FORWARD _PROTOTYPE( void dump_core, (struct mproc *rmp) );
19537 FORWARD _PROTOTYPE( void unpause, (int pro) );
19538 FORWARD _PROTOTYPE( void handle_sig, (int proc_nr, sigset_t sig_map) );
19539 FORWARD _PROTOTYPE( void cause_sigalrm, (struct timer *tp) );
19540
19541 /*===========================================================================*
19542 * do_sigaction *
19543 *===========================================================================*/
19544 PUBLIC int do_sigaction()
19545 {
19546 int r;
19547 struct sigaction svec;
19548 struct sigaction *svp;
19549
19550 if (m_in.sig_nr == SIGKILL) return(OK);
19551 if (m_in.sig_nr < 1 || m_in.sig_nr > _NSIG) return (EINVAL);
19552 svp = &mp->mp_sigact[m_in.sig_nr];
19553 if ((struct sigaction *) m_in.sig_osa != (struct sigaction *) NULL) {
19554 r = sys_datacopy(PM_PROC_NR,(vir_bytes) svp,
MINIX SOURCE CODE File: servers/pm/signal.c 899
19555 who, (vir_bytes) m_in.sig_osa, (phys_bytes) sizeof(svec));
19556 if (r != OK) return(r);
19557 }
19558
19559 if ((struct sigaction *) m_in.sig_nsa == (struct sigaction *) NULL)
19560 return(OK);
19561
19562 /* Read in the sigaction structure. */
19563 r = sys_datacopy(who, (vir_bytes) m_in.sig_nsa,
19564 PM_PROC_NR, (vir_bytes) &svec, (phys_bytes) sizeof(svec));
19565 if (r != OK) return(r);
19566
19567 if (svec.sa_handler == SIG_IGN) {
19568 sigaddset(&mp->mp_ignore, m_in.sig_nr);
19569 sigdelset(&mp->mp_sigpending, m_in.sig_nr);
19570 sigdelset(&mp->mp_catch, m_in.sig_nr);
19571 sigdelset(&mp->mp_sig2mess, m_in.sig_nr);
19572 } else if (svec.sa_handler == SIG_DFL) {
19573 sigdelset(&mp->mp_ignore, m_in.sig_nr);
19574 sigdelset(&mp->mp_catch, m_in.sig_nr);
19575 sigdelset(&mp->mp_sig2mess, m_in.sig_nr);
19576 } else if (svec.sa_handler == SIG_MESS) {
19577 if (! (mp->mp_flags & PRIV_PROC)) return(EPERM);
19578 sigdelset(&mp->mp_ignore, m_in.sig_nr);
19579 sigaddset(&mp->mp_sig2mess, m_in.sig_nr);
19580 sigdelset(&mp->mp_catch, m_in.sig_nr);
19581 } else {
19582 sigdelset(&mp->mp_ignore, m_in.sig_nr);
19583 sigaddset(&mp->mp_catch, m_in.sig_nr);
19584 sigdelset(&mp->mp_sig2mess, m_in.sig_nr);
19585 }
19586 mp->mp_sigact[m_in.sig_nr].sa_handler = svec.sa_handler;
19587 sigdelset(&svec.sa_mask, SIGKILL);
19588 mp->mp_sigact[m_in.sig_nr].sa_mask = svec.sa_mask;
19589 mp->mp_sigact[m_in.sig_nr].sa_flags = svec.sa_flags;
19590 mp->mp_sigreturn = (vir_bytes) m_in.sig_ret;
19591 return(OK);
19592 }
19594 /*===========================================================================*
19595 * do_sigpending *
19596 *===========================================================================*/
19597 PUBLIC int do_sigpending()
19598 {
19599 mp->mp_reply.reply_mask = (long) mp->mp_sigpending;
19600 return OK;
19601 }
19603 /*===========================================================================*
19604 * do_sigprocmask *
19605 *===========================================================================*/
19606 PUBLIC int do_sigprocmask()
19607 {
19608 /* Note that the library interface passes the actual mask in sigmask_set,
19609 * not a pointer to the mask, in order to save a copy. Similarly,
19610 * the old mask is placed in the return message which the library
19611 * interface copies (if requested) to the user specified address.
19612 *
19613 * The library interface must set SIG_INQUIRE if the ’act’ argument
19614 * is NULL.
900 File: servers/pm/signal.c MINIX SOURCE CODE
19615 */
19616
19617 int i;
19618
19619 mp->mp_reply.reply_mask = (long) mp->mp_sigmask;
19620
19621 switch (m_in.sig_how) {
19622 case SIG_BLOCK:
19623 sigdelset((sigset_t *)&m_in.sig_set, SIGKILL);
19624 for (i = 1; i <= _NSIG; i++) {
19625 if (sigismember((sigset_t *)&m_in.sig_set, i))
19626 sigaddset(&mp->mp_sigmask, i);
19627 }
19628 break;
19629
19630 case SIG_UNBLOCK:
19631 for (i = 1; i <= _NSIG; i++) {
19632 if (sigismember((sigset_t *)&m_in.sig_set, i))
19633 sigdelset(&mp->mp_sigmask, i);
19634 }
19635 check_pending(mp);
19636 break;
19637
19638 case SIG_SETMASK:
19639 sigdelset((sigset_t *) &m_in.sig_set, SIGKILL);
19640 mp->mp_sigmask = (sigset_t) m_in.sig_set;
19641 check_pending(mp);
19642 break;
19643
19644 case SIG_INQUIRE:
19645 break;
19646
19647 default:
19648 return(EINVAL);
19649 break;
19650 }
19651 return OK;
19652 }
19654 /*===========================================================================*
19655 * do_sigsuspend *
19656 *===========================================================================*/
19657 PUBLIC int do_sigsuspend()
19658 {
19659 mp->mp_sigmask2 = mp->mp_sigmask; /* save the old mask */
19660 mp->mp_sigmask = (sigset_t) m_in.sig_set;
19661 sigdelset(&mp->mp_sigmask, SIGKILL);
19662 mp->mp_flags |= SIGSUSPENDED;
19663 check_pending(mp);
19664 return(SUSPEND);
19665 }
19667 /*===========================================================================*
19668 * do_sigreturn *
19669 *===========================================================================*/
19670 PUBLIC int do_sigreturn()
19671 {
19672 /* A user signal handler is done. Restore context and check for
19673 * pending unblocked signals.
19674 */
MINIX SOURCE CODE File: servers/pm/signal.c 901
19675
19676 int r;
19677
19678 mp->mp_sigmask = (sigset_t) m_in.sig_set;
19679 sigdelset(&mp->mp_sigmask, SIGKILL);
19680
19681 r = sys_sigreturn(who, (struct sigmsg *) m_in.sig_context);
19682 check_pending(mp);
19683 return(r);
19684 }
19686 /*===========================================================================*
19687 * do_kill *
19688 *===========================================================================*/
19689 PUBLIC int do_kill()
19690 {
19691 /* Perform the kill(pid, signo) system call. */
19692
19693 return check_sig(m_in.pid, m_in.sig_nr);
19694 }
19696 /*===========================================================================*
19697 * ksig_pending *
19698 *===========================================================================*/
19699 PUBLIC int ksig_pending()
19700 {
19701 /* Certain signals, such as segmentation violations originate in the kernel.
19702 * When the kernel detects such signals, it notifies the PM to take further
19703 * action. The PM requests the kernel to send messages with the process
19704 * slot and bit map for all signaled processes. The File System, for example,
19705 * uses this mechanism to signal writing on broken pipes (SIGPIPE).
19706 *
19707 * The kernel has notified the PM about pending signals. Request pending
19708 * signals until all signals are handled. If there are no more signals,
19709 * NONE is returned in the process number field.
19710 */
19711 int proc_nr;
19712 sigset_t sig_map;
19713
19714 while (TRUE) {
19715 sys_getksig(&proc_nr, &sig_map); /* get an arbitrary pending signal */
19716 if (NONE == proc_nr) { /* stop if no more pending signals */
19717 break;
19718 } else {
19719 handle_sig(proc_nr, sig_map); /* handle the received signal */
19720 sys_endksig(proc_nr); /* tell kernel it’s done */
19721 }
19722 }
19723 return(SUSPEND); /* prevents sending reply */
19724 }
19726 /*===========================================================================*
19727 * handle_sig *
19728 *===========================================================================*/
19729 PRIVATE void handle_sig(proc_nr, sig_map)
19730 int proc_nr;
19731 sigset_t sig_map;
19732 {
19733 register struct mproc *rmp;
19734 int i;
902 File: servers/pm/signal.c MINIX SOURCE CODE
19735 pid_t proc_id, id;
19736
19737 rmp = &mproc[proc_nr];
19738 if ((rmp->mp_flags & (IN_USE | ZOMBIE)) != IN_USE) return;
19739 proc_id = rmp->mp_pid;
19740 mp = &mproc[0]; /* pretend signals are from PM */
19741 mp->mp_procgrp = rmp->mp_procgrp; /* get process group right */
19742
19743 /* Check each bit in turn to see if a signal is to be sent. Unlike
19744 * kill(), the kernel may collect several unrelated signals for a
19745 * process and pass them to PM in one blow. Thus loop on the bit
19746 * map. For SIGINT and SIGQUIT, use proc_id 0 to indicate a broadcast
19747 * to the recipient’s process group. For SIGKILL, use proc_id -1 to
19748 * indicate a systemwide broadcast.
19749 */
19750 for (i = 1; i <= _NSIG; i++) {
19751 if (!sigismember(&sig_map, i)) continue;
19752 switch (i) {
19753 case SIGINT:
19754 case SIGQUIT:
19755 id = 0; break; /* broadcast to process group */
19756 case SIGKILL:
19757 id = -1; break; /* broadcast to all except INIT */
19758 default:
19759 id = proc_id;
19760 break;
19761 }
19762 check_sig(id, i);
19763 }
19764 }
19766 /*===========================================================================*
19767 * do_alarm *
19768 *===========================================================================*/
19769 PUBLIC int do_alarm()
19770 {
19771 /* Perform the alarm(seconds) system call. */
19772 return(set_alarm(who, m_in.seconds));
19773 }
19775 /*===========================================================================*
19776 * set_alarm *
19777 *===========================================================================*/
19778 PUBLIC int set_alarm(proc_nr, sec)
19779 int proc_nr; /* process that wants the alarm */
19780 int sec; /* how many seconds delay before the signal */
19781 {
19782 /* This routine is used by do_alarm() to set the alarm timer. It is also used
19783 * to turn the timer off when a process exits with the timer still on.
19784 */
19785 clock_t ticks; /* number of ticks for alarm */
19786 clock_t exptime; /* needed for remaining time on previous alarm */
19787 clock_t uptime; /* current system time */
19788 int remaining; /* previous time left in seconds */
19789 int s;
19790
19791 /* First determine remaining time of previous alarm, if set. */
19792 if (mproc[proc_nr].mp_flags & ALARM_ON) {
19793 if ( (s=getuptime(&uptime)) != OK)
19794 panic(__FILE__,"set_alarm couldn’t get uptime", s);
MINIX SOURCE CODE File: servers/pm/signal.c 903
19795 exptime = *tmr_exp_time(&mproc[proc_nr].mp_timer);
19796 remaining = (int) ((exptime - uptime + (HZ-1))/HZ);
19797 if (remaining < 0) remaining = 0;
19798 } else {
19799 remaining = 0;
19800 }
19801
19802 /* Tell the clock task to provide a signal message when the time comes.
19803 *
19804 * Large delays cause a lot of problems. First, the alarm system call
19805 * takes an unsigned seconds count and the library has cast it to an int.
19806 * That probably works, but on return the library will convert "negative"
19807 * unsigneds to errors. Presumably no one checks for these errors, so
19808 * force this call through. Second, If unsigned and long have the same
19809 * size, converting from seconds to ticks can easily overflow. Finally,
19810 * the kernel has similar overflow bugs adding ticks.
19811 *
19812 * Fixing this requires a lot of ugly casts to fit the wrong interface
19813 * types and to avoid overflow traps. ALRM_EXP_TIME has the right type
19814 * (clock_t) although it is declared as long. How can variables like
19815 * this be declared properly without combinatorial explosion of message
19816 * types?
19817 */
19818 ticks = (clock_t) (HZ * (unsigned long) (unsigned) sec);
19819 if ( (unsigned long) ticks / HZ != (unsigned) sec)
19820 ticks = LONG_MAX; /* eternity (really TMR_NEVER) */
19821
19822 if (ticks != 0) {
19823 pm_set_timer(&mproc[proc_nr].mp_timer, ticks, cause_sigalrm, proc_nr);
19824 mproc[proc_nr].mp_flags |= ALARM_ON;
19825 } else if (mproc[proc_nr].mp_flags & ALARM_ON) {
19826 pm_cancel_timer(&mproc[proc_nr].mp_timer);
19827 mproc[proc_nr].mp_flags &= ˜ALARM_ON;
19828 }
19829 return(remaining);
19830 }
19832 /*===========================================================================*
19833 * cause_sigalrm *
19834 *===========================================================================*/
19835 PRIVATE void cause_sigalrm(tp)
19836 struct timer *tp;
19837 {
19838 int proc_nr;
19839 register struct mproc *rmp;
19840
19841 proc_nr = tmr_arg(tp)->ta_int; /* get process from timer */
19842 rmp = &mproc[proc_nr];
19843
19844 if ((rmp->mp_flags & (IN_USE | ZOMBIE)) != IN_USE) return;
19845 if ((rmp->mp_flags & ALARM_ON) == 0) return;
19846 rmp->mp_flags &= ˜ALARM_ON;
19847 check_sig(rmp->mp_pid, SIGALRM);
19848 }
19850 /*===========================================================================*
19851 * do_pause *
19852 *===========================================================================*/
19853 PUBLIC int do_pause()
19854 {
904 File: servers/pm/signal.c MINIX SOURCE CODE
19855 /* Perform the pause() system call. */
19856
19857 mp->mp_flags |= PAUSED;
19858 return(SUSPEND);
19859 }
19861 /*===========================================================================*
19862 * sig_proc *
19863 *===========================================================================*/
19864 PUBLIC void sig_proc(rmp, signo)
19865 register struct mproc *rmp; /* pointer to the process to be signaled */
19866 int signo; /* signal to send to process (1 to _NSIG) */
19867 {
19868 /* Send a signal to a process. Check to see if the signal is to be caught,
19869 * ignored, tranformed into a message (for system processes) or blocked.
19870 * - If the signal is to be transformed into a message, request the KERNEL to
19871 * send the target process a system notification with the pending signal as an
19872 * argument.
19873 * - If the signal is to be caught, request the KERNEL to push a sigcontext
19874 * structure and a sigframe structure onto the catcher’s stack. Also, KERNEL
19875 * will reset the program counter and stack pointer, so that when the process
19876 * next runs, it will be executing the signal handler. When the signal handler
19877 * returns, sigreturn(2) will be called. Then KERNEL will restore the signal
19878 * context from the sigcontext structure.
19879 * If there is insufficient stack space, kill the process.
19880 */
19881
19882 vir_bytes new_sp;
19883 int s;
19884 int slot;
19885 int sigflags;
19886 struct sigmsg sm;
19887
19888 slot = (int) (rmp - mproc);
19889 if ((rmp->mp_flags & (IN_USE | ZOMBIE)) != IN_USE) {
19890 printf("PM: signal %d sent to %s process %d\n",
19891 signo, (rmp->mp_flags & ZOMBIE) ? "zombie" : "dead", slot);
19892 panic(__FILE__,"", NO_NUM);
19893 }
19894 if ((rmp->mp_flags & TRACED) && signo != SIGKILL) {
19895 /* A traced process has special handling. */
19896 unpause(slot);
19897 stop_proc(rmp, signo); /* a signal causes it to stop */
19898 return;
19899 }
19900 /* Some signals are ignored by default. */
19901 if (sigismember(&rmp->mp_ignore, signo)) {
19902 return;
19903 }
19904 if (sigismember(&rmp->mp_sigmask, signo)) {
19905 /* Signal should be blocked. */
19906 sigaddset(&rmp->mp_sigpending, signo);
19907 return;
19908 }
19909 sigflags = rmp->mp_sigact[signo].sa_flags;
19910 if (sigismember(&rmp->mp_catch, signo)) {
19911 if (rmp->mp_flags & SIGSUSPENDED)
19912 sm.sm_mask = rmp->mp_sigmask2;
19913 else
19914 sm.sm_mask = rmp->mp_sigmask;
MINIX SOURCE CODE File: servers/pm/signal.c 905
19915 sm.sm_signo = signo;
19916 sm.sm_sighandler = (vir_bytes) rmp->mp_sigact[signo].sa_handler;
19917 sm.sm_sigreturn = rmp->mp_sigreturn;
19918 if ((s=get_stack_ptr(slot, &new_sp)) != OK)
19919 panic(__FILE__,"couldn’t get new stack pointer",s);
19920 sm.sm_stkptr = new_sp;
19921
19922 /* Make room for the sigcontext and sigframe struct. */
19923 new_sp -= sizeof(struct sigcontext)
19924 + 3 * sizeof(char *) + 2 * sizeof(int);
19925
19926 if (adjust(rmp, rmp->mp_seg[D].mem_len, new_sp) != OK)
19927 goto doterminate;
19928
19929 rmp->mp_sigmask |= rmp->mp_sigact[signo].sa_mask;
19930 if (sigflags & SA_NODEFER)
19931 sigdelset(&rmp->mp_sigmask, signo);
19932 else
19933 sigaddset(&rmp->mp_sigmask, signo);
19934
19935 if (sigflags & SA_RESETHAND) {
19936 sigdelset(&rmp->mp_catch, signo);
19937 rmp->mp_sigact[signo].sa_handler = SIG_DFL;
19938 }
19939
19940 if (OK == (s=sys_sigsend(slot, &sm))) {
19941
19942 sigdelset(&rmp->mp_sigpending, signo);
19943 /* If process is hanging on PAUSE, WAIT, SIGSUSPEND, tty,
19944 * pipe, etc., release it.
19945 */
19946 unpause(slot);
19947 return;
19948 }
19949 panic(__FILE__, "warning, sys_sigsend failed", s);
19950 }
19951 else if (sigismember(&rmp->mp_sig2mess, signo)) {
19952 if (OK != (s=sys_kill(slot,signo)))
19953 panic(__FILE__, "warning, sys_kill failed", s);
19954 return;
19955 }
19956
19957 doterminate:
19958 /* Signal should not or cannot be caught. Take default action. */
19959 if (sigismember(&ign_sset, signo)) return;
19960
19961 rmp->mp_sigstatus = (char) signo;
19962 if (sigismember(&core_sset, signo)) {
19963 /* Switch to the user’s FS environment and dump core. */
19964 tell_fs(CHDIR, slot, FALSE, 0);
19965 dump_core(rmp);
19966 }
19967 pm_exit(rmp, 0); /* terminate process */
19968 }
19970 /*===========================================================================*
19971 * check_sig *
19972 *===========================================================================*/
19973 PUBLIC int check_sig(proc_id, signo)
19974 pid_t proc_id; /* pid of proc to sig, or 0 or -1, or -pgrp */
906 File: servers/pm/signal.c MINIX SOURCE CODE
19975 int signo; /* signal to send to process (0 to _NSIG) */
19976 {
19977 /* Check to see if it is possible to send a signal. The signal may have to be
19978 * sent to a group of processes. This routine is invoked by the KILL system
19979 * call, and also when the kernel catches a DEL or other signal.
19980 */
19981
19982 register struct mproc *rmp;
19983 int count; /* count # of signals sent */
19984 int error_code;
19985
19986 if (signo < 0 || signo > _NSIG) return(EINVAL);
19987
19988 /* Return EINVAL for attempts to send SIGKILL to INIT alone. */
19989 if (proc_id == INIT_PID && signo == SIGKILL) return(EINVAL);
19990
19991 /* Search the proc table for processes to signal. (See forkexit.c about
19992 * pid magic.)
19993 */
19994 count = 0;
19995 error_code = ESRCH;
19996 for (rmp = &mproc[0]; rmp < &mproc[NR_PROCS]; rmp++) {
19997 if (!(rmp->mp_flags & IN_USE)) continue;
19998 if ((rmp->mp_flags & ZOMBIE) && signo != 0) continue;
19999
20000 /* Check for selection. */
20001 if (proc_id > 0 && proc_id != rmp->mp_pid) continue;
20002 if (proc_id == 0 && mp->mp_procgrp != rmp->mp_procgrp) continue;
20003 if (proc_id == -1 && rmp->mp_pid <= INIT_PID) continue;
20004 if (proc_id < -1 && rmp->mp_procgrp != -proc_id) continue;
20005
20006 /* Check for permission. */
20007 if (mp->mp_effuid != SUPER_USER
20008 && mp->mp_realuid != rmp->mp_realuid
20009 && mp->mp_effuid != rmp->mp_realuid
20010 && mp->mp_realuid != rmp->mp_effuid
20011 && mp->mp_effuid != rmp->mp_effuid) {
20012 error_code = EPERM;
20013 continue;
20014 }
20015
20016 count++;
20017 if (signo == 0) continue;
20018
20019 /* ’sig_proc’ will handle the disposition of the signal. The
20020 * signal may be caught, blocked, ignored, or cause process
20021 * termination, possibly with core dump.
20022 */
20023 sig_proc(rmp, signo);
20024
20025 if (proc_id > 0) break; /* only one process being signaled */
20026 }
20027
20028 /* If the calling process has killed itself, don’t reply. */
20029 if ((mp->mp_flags & (IN_USE | ZOMBIE)) != IN_USE) return(SUSPEND);
20030 return(count > 0 ? OK : error_code);
20031 }
MINIX SOURCE CODE File: servers/pm/signal.c 907
20033 /*===========================================================================*
20034 * check_pending *
20035 *===========================================================================*/
20036 PUBLIC void check_pending(rmp)
20037 register struct mproc *rmp;
20038 {
20039 /* Check to see if any pending signals have been unblocked. The
20040 * first such signal found is delivered.
20041 *
20042 * If multiple pending unmasked signals are found, they will be
20043 * delivered sequentially.
20044 *
20045 * There are several places in this file where the signal mask is
20046 * changed. At each such place, check_pending() should be called to
20047 * check for newly unblocked signals.
20048 */
20049
20050 int i;
20051
20052 for (i = 1; i <= _NSIG; i++) {
20053 if (sigismember(&rmp->mp_sigpending, i) &&
20054 !sigismember(&rmp->mp_sigmask, i)) {
20055 sigdelset(&rmp->mp_sigpending, i);
20056 sig_proc(rmp, i);
20057 break;
20058 }
20059 }
20060 }
20062 /*===========================================================================*
20063 * unpause *
20064 *===========================================================================*/
20065 PRIVATE void unpause(pro)
20066 int pro; /* which process number */
20067 {
20068 /* A signal is to be sent to a process. If that process is hanging on a
20069 * system call, the system call must be terminated with EINTR. Possible
20070 * calls are PAUSE, WAIT, READ and WRITE, the latter two for pipes and ttys.
20071 * First check if the process is hanging on an PM call. If not, tell FS,
20072 * so it can check for READs and WRITEs from pipes, ttys and the like.
20073 */
20074
20075 register struct mproc *rmp;
20076
20077 rmp = &mproc[pro];
20078
20079 /* Check to see if process is hanging on a PAUSE, WAIT or SIGSUSPEND call. */
20080 if (rmp->mp_flags & (PAUSED | WAITING | SIGSUSPENDED)) {
20081 rmp->mp_flags &= ˜(PAUSED | WAITING | SIGSUSPENDED);
20082 setreply(pro, EINTR);
20083 return;
20084 }
20085
20086 /* Process is not hanging on an PM call. Ask FS to take a look. */
20087 tell_fs(UNPAUSE, pro, 0, 0);
20088 }
908 File: servers/pm/signal.c MINIX SOURCE CODE
20090 /*===========================================================================*
20091 * dump_core *
20092 *===========================================================================*/
20093 PRIVATE void dump_core(rmp)
20094 register struct mproc *rmp; /* whose core is to be dumped */
20095 {
20096 /* Make a core dump on the file "core", if possible. */
20097
20098 int s, fd, seg, slot;
20099 vir_bytes current_sp;
20100 long trace_data, trace_off;
20101
20102 slot = (int) (rmp - mproc);
20103
20104 /* Can core file be written? We are operating in the user’s FS environment,
20105 * so no special permission checks are needed.
20106 */
20107 if (rmp->mp_realuid != rmp->mp_effuid) return;
20108 if ( (fd = open(core_name, O_WRONLY | O_CREAT | O_TRUNC | O_NONBLOCK,
20109 CORE_MODE)) < 0) return;
20110 rmp->mp_sigstatus |= DUMPED;
20111
20112 /* Make sure the stack segment is up to date.
20113 * We don’t want adjust() to fail unless current_sp is preposterous,
20114 * but it might fail due to safety checking. Also, we don’t really want
20115 * the adjust() for sending a signal to fail due to safety checking.
20116 * Maybe make SAFETY_BYTES a parameter.
20117 */
20118 if ((s=get_stack_ptr(slot, &current_sp)) != OK)
20119 panic(__FILE__,"couldn’t get new stack pointer",s);
20120 adjust(rmp, rmp->mp_seg[D].mem_len, current_sp);
20121
20122 /* Write the memory map of all segments to begin the core file. */
20123 if (write(fd, (char *) rmp->mp_seg, (unsigned) sizeof rmp->mp_seg)
20124 != (unsigned) sizeof rmp->mp_seg) {
20125 close(fd);
20126 return;
20127 }
20128
20129 /* Write out the whole kernel process table entry to get the regs. */
20130 trace_off = 0;
20131 while (sys_trace(T_GETUSER, slot, trace_off, &trace_data) == OK) {
20132 if (write(fd, (char *) &trace_data, (unsigned) sizeof (long))
20133 != (unsigned) sizeof (long)) {
20134 close(fd);
20135 return;
20136 }
20137 trace_off += sizeof (long);
20138 }
20139
20140 /* Loop through segments and write the segments themselves out. */
20141 for (seg = 0; seg < NR_LOCAL_SEGS; seg++) {
20142 rw_seg(1, fd, slot, seg,
20143 (phys_bytes) rmp->mp_seg[seg].mem_len << CLICK_SHIFT);
20144 }
20145 close(fd);
20146 }
MINIX SOURCE CODE File: servers/pm/timers.c 909
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/timers.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
20200 /* PM watchdog timer management. These functions in this file provide
20201 * a convenient interface to the timers library that manages a list of
20202 * watchdog timers. All details of scheduling an alarm at the CLOCK task
20203 * are hidden behind this interface.
20204 * Only system processes are allowed to set an alarm timer at the kernel.
20205 * Therefore, the PM maintains a local list of timers for user processes
20206 * that requested an alarm signal.
20207 *
20208 * The entry points into this file are:
20209 * pm_set_timer: reset and existing or set a new watchdog timer
20210 * pm_expire_timers: check for expired timers and run watchdog functions
20211 * pm_cancel_timer: remove a time from the list of timers
20212 *
20213 */
20214
20215 #include "pm.h"
20216
20217 #include <timers.h>
20218 #include <minix/syslib.h>
20219 #include <minix/com.h>
20220
20221 PRIVATE timer_t *pm_timers = NULL;
20222
20223 /*===========================================================================*
20224 * pm_set_timer *
20225 *===========================================================================*/
20226 PUBLIC void pm_set_timer(timer_t *tp, int ticks, tmr_func_t watchdog, int arg)
20227 {
20228 int r;
20229 clock_t now, prev_time = 0, next_time;
20230
20231 if ((r = getuptime(&now)) != OK)
20232 panic(__FILE__, "PM couldn’t get uptime", NO_NUM);
20233
20234 /* Set timer argument and add timer to the list. */
20235 tmr_arg(tp)->ta_int = arg;
20236 prev_time = tmrs_settimer(&pm_timers,tp,now+ticks,watchdog,&next_time);
20237
20238 /* Reschedule our synchronous alarm if necessary. */
20239 if (! prev_time || prev_time > next_time) {
20240 if (sys_setalarm(next_time, 1) != OK)
20241 panic(__FILE__, "PM set timer couldn’t set alarm.", NO_NUM);
20242 }
20243
20244 return;
20245 }
20247 /*===========================================================================*
20248 * pm_expire_timers *
20249 *===========================================================================*/
20250 PUBLIC void pm_expire_timers(clock_t now)
20251 {
20252 clock_t next_time;
20253
20254 /* Check for expired timers and possibly reschedule an alarm. */
910 File: servers/pm/timers.c MINIX SOURCE CODE
20255 tmrs_exptimers(&pm_timers, now, &next_time);
20256 if (next_time > 0) {
20257 if (sys_setalarm(next_time, 1) != OK)
20258 panic(__FILE__, "PM expire timer couldn’t set alarm.", NO_NUM);
20259 }
20260 }
20262 /*===========================================================================*
20263 * pm_cancel_timer *
20264 *===========================================================================*/
20265 PUBLIC void pm_cancel_timer(timer_t *tp)
20266 {
20267 clock_t next_time, prev_time;
20268 prev_time = tmrs_clrtimer(&pm_timers, tp, &next_time);
20269
20270 /* If the earliest timer has been removed, we have to set the alarm to
20271 * the next timer, or cancel the alarm altogether if the last timer has
20272 * been cancelled (next_time will be 0 then).
20273 */
20274 if (prev_time < next_time || ! next_time) {
20275 if (sys_setalarm(next_time, 1) != OK)
20276 panic(__FILE__, "PM expire timer couldn’t set alarm.", NO_NUM);
20277 }
20278 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/time.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
20300 /* This file takes care of those system calls that deal with time.
20301 *
20302 * The entry points into this file are
20303 * do_time: perform the TIME system call
20304 * do_stime: perform the STIME system call
20305 * do_times: perform the TIMES system call
20306 */
20307
20308 #include "pm.h"
20309 #include <minix/callnr.h>
20310 #include <minix/com.h>
20311 #include <signal.h>
20312 #include "mproc.h"
20313 #include "param.h"
20314
20315 PRIVATE time_t boottime;
20316
20317 /*===========================================================================*
20318 * do_time *
20319 *===========================================================================*/
20320 PUBLIC int do_time()
20321 {
20322 /* Perform the time(tp) system call. This returns the time in seconds since
20323 * 1.1.1970. MINIX is an astrophysically naive system that assumes the earth
20324 * rotates at a constant rate and that such things as leap seconds do not
20325 * exist.
20326 */
20327 clock_t uptime;
20328 int s;
20329
MINIX SOURCE CODE File: servers/pm/time.c 911
20330 if ( (s=getuptime(&uptime)) != OK)
20331 panic(__FILE__,"do_time couldn’t get uptime", s);
20332
20333 mp->mp_reply.reply_time = (time_t) (boottime + (uptime/HZ));
20334 mp->mp_reply.reply_utime = (uptime%HZ)*1000000/HZ;
20335 return(OK);
20336 }
20338 /*===========================================================================*
20339 * do_stime *
20340 *===========================================================================*/
20341 PUBLIC int do_stime()
20342 {
20343 /* Perform the stime(tp) system call. Retrieve the system’s uptime (ticks
20344 * since boot) and store the time in seconds at system boot in the global
20345 * variable ’boottime’.
20346 */
20347 clock_t uptime;
20348 int s;
20349
20350 if (mp->mp_effuid != SUPER_USER) {
20351 return(EPERM);
20352 }
20353 if ( (s=getuptime(&uptime)) != OK)
20354 panic(__FILE__,"do_stime couldn’t get uptime", s);
20355 boottime = (long) m_in.stime - (uptime/HZ);
20356
20357 /* Also inform FS about the new system time. */
20358 tell_fs(STIME, boottime, 0, 0);
20359
20360 return(OK);
20361 }
20363 /*===========================================================================*
20364 * do_times *
20365 *===========================================================================*/
20366 PUBLIC int do_times()
20367 {
20368 /* Perform the times(buffer) system call. */
20369 register struct mproc *rmp = mp;
20370 clock_t t[5];
20371 int s;
20372
20373 if (OK != (s=sys_times(who, t)))
20374 panic(__FILE__,"do_times couldn’t get times", s);
20375 rmp->mp_reply.reply_t1 = t[0]; /* user time */
20376 rmp->mp_reply.reply_t2 = t[1]; /* system time */
20377 rmp->mp_reply.reply_t3 = rmp->mp_child_utime; /* child user time */
20378 rmp->mp_reply.reply_t4 = rmp->mp_child_stime; /* child system time */
20379 rmp->mp_reply.reply_t5 = t[4]; /* uptime since boot */
20380
20381 return(OK);
20382 }
912 File: servers/pm/getset.c MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/getset.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
20400 /* This file handles the 4 system calls that get and set uids and gids.
20401 * It also handles getpid(), setsid(), and getpgrp(). The code for each
20402 * one is so tiny that it hardly seemed worthwhile to make each a separate
20403 * function.
20404 */
20405
20406 #include "pm.h"
20407 #include <minix/callnr.h>
20408 #include <signal.h>
20409 #include "mproc.h"
20410 #include "param.h"
20411
20412 /*===========================================================================*
20413 * do_getset *
20414 *===========================================================================*/
20415 PUBLIC int do_getset()
20416 {
20417 /* Handle GETUID, GETGID, GETPID, GETPGRP, SETUID, SETGID, SETSID. The four
20418 * GETs and SETSID return their primary results in ’r’. GETUID, GETGID, and
20419 * GETPID also return secondary results (the effective IDs, or the parent
20420 * process ID) in ’reply_res2’, which is returned to the user.
20421 */
20422
20423 register struct mproc *rmp = mp;
20424 register int r;
20425
20426 switch(call_nr) {
20427 case GETUID:
20428 r = rmp->mp_realuid;
20429 rmp->mp_reply.reply_res2 = rmp->mp_effuid;
20430 break;
20431
20432 case GETGID:
20433 r = rmp->mp_realgid;
20434 rmp->mp_reply.reply_res2 = rmp->mp_effgid;
20435 break;
20436
20437 case GETPID:
20438 r = mproc[who].mp_pid;
20439 rmp->mp_reply.reply_res2 = mproc[rmp->mp_parent].mp_pid;
20440 break;
20441
20442 case SETUID:
20443 if (rmp->mp_realuid != (uid_t) m_in.usr_id &&
20444 rmp->mp_effuid != SUPER_USER)
20445 return(EPERM);
20446 rmp->mp_realuid = (uid_t) m_in.usr_id;
20447 rmp->mp_effuid = (uid_t) m_in.usr_id;
20448 tell_fs(SETUID, who, rmp->mp_realuid, rmp->mp_effuid);
20449 r = OK;
20450 break;
20451
20452 case SETGID:
20453 if (rmp->mp_realgid != (gid_t) m_in.grp_id &&
20454 rmp->mp_effuid != SUPER_USER)
MINIX SOURCE CODE File: servers/pm/getset.c 913
20455 return(EPERM);
20456 rmp->mp_realgid = (gid_t) m_in.grp_id;
20457 rmp->mp_effgid = (gid_t) m_in.grp_id;
20458 tell_fs(SETGID, who, rmp->mp_realgid, rmp->mp_effgid);
20459 r = OK;
20460 break;
20461
20462 case SETSID:
20463 if (rmp->mp_procgrp == rmp->mp_pid) return(EPERM);
20464 rmp->mp_procgrp = rmp->mp_pid;
20465 tell_fs(SETSID, who, 0, 0);
20466 /* fall through */
20467
20468 case GETPGRP:
20469 r = rmp->mp_procgrp;
20470 break;
20471
20472 default:
20473 r = EINVAL;
20474 break;
20475 }
20476 return(r);
20477 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/pm/misc.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
20500 /* Miscellaneous system calls. Author: Kees J. Bot
20501 * 31 Mar 2000
20502 * The entry points into this file are:
20503 * do_reboot: kill all processes, then reboot system
20504 * do_svrctl: process manager control
20505 * do_getsysinfo: request copy of PM data structure (Jorrit N. Herder)
20506 * do_getprocnr: lookup process slot number (Jorrit N. Herder)
20507 * do_memalloc: allocate a chunk of memory (Jorrit N. Herder)
20508 * do_memfree: deallocate a chunk of memory (Jorrit N. Herder)
20509 * do_getsetpriority: get/set process priority
20510 */
20511
20512 #include "pm.h"
20513 #include <minix/callnr.h>
20514 #include <signal.h>
20515 #include <sys/svrctl.h>
20516 #include <sys/resource.h>
20517 #include <minix/com.h>
20518 #include <string.h>
20519 #include "mproc.h"
20520 #include "param.h"
20521
20522 /*===========================================================================*
20523 * do_allocmem *
20524 *===========================================================================*/
20525 PUBLIC int do_allocmem()
20526 {
20527 vir_clicks mem_clicks;
20528 phys_clicks mem_base;
20529
914 File: servers/pm/misc.c MINIX SOURCE CODE
20530 mem_clicks = (m_in.memsize + CLICK_SIZE -1 ) >> CLICK_SHIFT;
20531 mem_base = alloc_mem(mem_clicks);
20532 if (mem_base == NO_MEM) return(ENOMEM);
20533 mp->mp_reply.membase = (phys_bytes) (mem_base << CLICK_SHIFT);
20534 return(OK);
20535 }
20537 /*===========================================================================*
20538 * do_freemem *
20539 *===========================================================================*/
20540 PUBLIC int do_freemem()
20541 {
20542 vir_clicks mem_clicks;
20543 phys_clicks mem_base;
20544
20545 mem_clicks = (m_in.memsize + CLICK_SIZE -1 ) >> CLICK_SHIFT;
20546 mem_base = (m_in.membase + CLICK_SIZE -1 ) >> CLICK_SHIFT;
20547 free_mem(mem_base, mem_clicks);
20548 return(OK);
20549 }
20551 /*===========================================================================*
20552 * do_getsysinfo *
20553 *===========================================================================*/
20554 PUBLIC int do_getsysinfo()
20555 {
20556 struct mproc *proc_addr;
20557 vir_bytes src_addr, dst_addr;
20558 struct kinfo kinfo;
20559 size_t len;
20560 int s;
20561
20562 switch(m_in.info_what) {
20563 case SI_KINFO: /* kernel info is obtained via PM */
20564 sys_getkinfo(&kinfo);
20565 src_addr = (vir_bytes) &kinfo;
20566 len = sizeof(struct kinfo);
20567 break;
20568 case SI_PROC_ADDR: /* get address of PM process table */
20569 proc_addr = &mproc[0];
20570 src_addr = (vir_bytes) &proc_addr;
20571 len = sizeof(struct mproc *);
20572 break;
20573 case SI_PROC_TAB: /* copy entire process table */
20574 src_addr = (vir_bytes) mproc;
20575 len = sizeof(struct mproc) * NR_PROCS;
20576 break;
20577 default:
20578 return(EINVAL);
20579 }
20580
20581 dst_addr = (vir_bytes) m_in.info_where;
20582 if (OK != (s=sys_datacopy(SELF, src_addr, who, dst_addr, len)))
20583 return(s);
20584 return(OK);
20585 }
MINIX SOURCE CODE File: servers/pm/misc.c 915
20587 /*===========================================================================*
20588 * do_getprocnr *
20589 *===========================================================================*/
20590 PUBLIC int do_getprocnr()
20591 {
20592 register struct mproc *rmp;
20593 static char search_key[PROC_NAME_LEN+1];
20594 int key_len;
20595 int s;
20596
20597 if (m_in.pid >= 0) { /* lookup process by pid */
20598 for (rmp = &mproc[0]; rmp < &mproc[NR_PROCS]; rmp++) {
20599 if ((rmp->mp_flags & IN_USE) && (rmp->mp_pid==m_in.pid)) {
20600 mp->mp_reply.procnr = (int) (rmp - mproc);
20601 return(OK);
20602 }
20603 }
20604 return(ESRCH);
20605 } else if (m_in.namelen > 0) { /* lookup process by name */
20606 key_len = MIN(m_in.namelen, PROC_NAME_LEN);
20607 if (OK != (s=sys_datacopy(who, (vir_bytes) m_in.addr,
20608 SELF, (vir_bytes) search_key, key_len)))
20609 return(s);
20610 search_key[key_len] = ’\0’; /* terminate for safety */
20611 for (rmp = &mproc[0]; rmp < &mproc[NR_PROCS]; rmp++) {
20612 if ((rmp->mp_flags & IN_USE) &&
20613 strncmp(rmp->mp_name, search_key, key_len)==0) {
20614 mp->mp_reply.procnr = (int) (rmp - mproc);
20615 return(OK);
20616 }
20617 }
20618 return(ESRCH);
20619 } else { /* return own process number */
20620 mp->mp_reply.procnr = who;
20621 }
20622 return(OK);
20623 }
20625 /*===========================================================================*
20626 * do_reboot *
20627 *===========================================================================*/
20628 #define REBOOT_CODE "delay; boot"
20629 PUBLIC int do_reboot()
20630 {
20631 char monitor_code[32*sizeof(char *)];
20632 int code_len;
20633 int abort_flag;
20634
20635 if (mp->mp_effuid != SUPER_USER) return(EPERM);
20636
20637 switch (m_in.reboot_flag) {
20638 case RBT_HALT:
20639 case RBT_PANIC:
20640 case RBT_RESET:
20641 abort_flag = m_in.reboot_flag;
20642 break;
20643 case RBT_REBOOT:
20644 code_len = strlen(REBOOT_CODE) + 1;
20645 strncpy(monitor_code, REBOOT_CODE, code_len);
20646 abort_flag = RBT_MONITOR;
916 File: servers/pm/misc.c MINIX SOURCE CODE
20647 break;
20648 case RBT_MONITOR:
20649 code_len = m_in.reboot_strlen + 1;
20650 if (code_len > sizeof(monitor_code)) return(EINVAL);
20651 if (sys_datacopy(who, (vir_bytes) m_in.reboot_code,
20652 PM_PROC_NR, (vir_bytes) monitor_code,
20653 (phys_bytes) (code_len)) != OK) return(EFAULT);
20654 if (monitor_code[code_len-1] != 0) return(EINVAL);
20655 abort_flag = RBT_MONITOR;
20656 break;
20657 default:
20658 return(EINVAL);
20659 }
20660
20661 check_sig(-1, SIGKILL); /* kill all processes except init */
20662 tell_fs(REBOOT,0,0,0); /* tell FS to prepare for shutdown */
20663
20664 /* Ask the kernel to abort. All system services, including the PM, will
20665 * get a HARD_STOP notification. Await the notification in the main loop.
20666 */
20667 sys_abort(abort_flag, PM_PROC_NR, monitor_code, code_len);
20668 return(SUSPEND); /* don’t reply to killed process */
20669 }
20671 /*===========================================================================*
20672 * do_getsetpriority *
20673 *===========================================================================*/
20674 PUBLIC int do_getsetpriority()
20675 {
20676 int arg_which, arg_who, arg_pri;
20677 int rmp_nr;
20678 struct mproc *rmp;
20679
20680 arg_which = m_in.m1_i1;
20681 arg_who = m_in.m1_i2;
20682 arg_pri = m_in.m1_i3; /* for SETPRIORITY */
20683
20684 /* Code common to GETPRIORITY and SETPRIORITY. */
20685
20686 /* Only support PRIO_PROCESS for now. */
20687 if (arg_which != PRIO_PROCESS)
20688 return(EINVAL);
20689
20690 if (arg_who == 0)
20691 rmp_nr = who;
20692 else
20693 if ((rmp_nr = proc_from_pid(arg_who)) < 0)
20694 return(ESRCH);
20695
20696 rmp = &mproc[rmp_nr];
20697
20698 if (mp->mp_effuid != SUPER_USER &&
20699 mp->mp_effuid != rmp->mp_effuid && mp->mp_effuid != rmp->mp_realuid)
20700 return EPERM;
20701
20702 /* If GET, that’s it. */
20703 if (call_nr == GETPRIORITY) {
20704 return(rmp->mp_nice - PRIO_MIN);
20705 }
20706
MINIX SOURCE CODE File: servers/pm/misc.c 917
20707 /* Only root is allowed to reduce the nice level. */
20708 if (rmp->mp_nice > arg_pri && mp->mp_effuid != SUPER_USER)
20709 return(EACCES);
20710
20711 /* We’re SET, and it’s allowed. Do it and tell kernel. */
20712 rmp->mp_nice = arg_pri;
20713 return sys_nice(rmp_nr, arg_pri);
20714 }
20716 /*===========================================================================*
20717 * do_svrctl *
20718 *===========================================================================*/
20719 PUBLIC int do_svrctl()
20720 {
20721 int s, req;
20722 vir_bytes ptr;
20723 #define MAX_LOCAL_PARAMS 2
20724 static struct {
20725 char name[30];
20726 char value[30];
20727 } local_param_overrides[MAX_LOCAL_PARAMS];
20728 static int local_params = 0;
20729
20730 req = m_in.svrctl_req;
20731 ptr = (vir_bytes) m_in.svrctl_argp;
20732
20733 /* Is the request indeed for the MM? */
20734 if (((req >> 8) & 0xFF) != ’M’) return(EINVAL);
20735
20736 /* Control operations local to the PM. */
20737 switch(req) {
20738 case MMSETPARAM:
20739 case MMGETPARAM: {
20740 struct sysgetenv sysgetenv;
20741 char search_key[64];
20742 char *val_start;
20743 size_t val_len;
20744 size_t copy_len;
20745
20746 /* Copy sysgetenv structure to PM. */
20747 if (sys_datacopy(who, ptr, SELF, (vir_bytes) &sysgetenv,
20748 sizeof(sysgetenv)) != OK) return(EFAULT);
20749
20750 /* Set a param override? */
20751 if (req == MMSETPARAM) {
20752 if (local_params >= MAX_LOCAL_PARAMS) return ENOSPC;
20753 if (sysgetenv.keylen <= 0
20754 || sysgetenv.keylen >=
20755 sizeof(local_param_overrides[local_params].name)
20756 || sysgetenv.vallen <= 0
20757 || sysgetenv.vallen >=
20758 sizeof(local_param_overrides[local_params].value))
20759 return EINVAL;
20760
20761 if ((s = sys_datacopy(who, (vir_bytes) sysgetenv.key,
20762 SELF, (vir_bytes) local_param_overrides[local_params].name,
20763 sysgetenv.keylen)) != OK)
20764 return s;
20765 if ((s = sys_datacopy(who, (vir_bytes) sysgetenv.val,
20766 SELF, (vir_bytes) local_param_overrides[local_params].value,
918 File: servers/pm/misc.c MINIX SOURCE CODE
20767 sysgetenv.keylen)) != OK)
20768 return s;
20769 local_param_overrides[local_params].name[sysgetenv.keylen] = ’\0’;
20770 local_param_overrides[local_params].value[sysgetenv.vallen] = ’\0’;
20771
20772 local_params++;
20773
20774 return OK;
20775 }
20776
20777 if (sysgetenv.keylen == 0) { /* copy all parameters */
20778 val_start = monitor_params;
20779 val_len = sizeof(monitor_params);
20780 }
20781 else { /* lookup value for key */
20782 int p;
20783 /* Try to get a copy of the requested key. */
20784 if (sysgetenv.keylen > sizeof(search_key)) return(EINVAL);
20785 if ((s = sys_datacopy(who, (vir_bytes) sysgetenv.key,
20786 SELF, (vir_bytes) search_key, sysgetenv.keylen)) != OK)
20787 return(s);
20788
20789 /* Make sure key is null-terminated and lookup value.
20790 * First check local overrides.
20791 */
20792 search_key[sysgetenv.keylen-1]= ’\0’;
20793 for(p = 0; p < local_params; p++) {
20794 if (!strcmp(search_key, local_param_overrides[p].name)) {
20795 val_start = local_param_overrides[p].value;
20796 break;
20797 }
20798 }
20799 if (p >= local_params && (val_start = find_param(search_key)) == NULL)
20800 return(ESRCH);
20801 val_len = strlen(val_start) + 1;
20802 }
20803
20804 /* See if it fits in the client’s buffer. */
20805 if (val_len > sysgetenv.vallen)
20806 return E2BIG;
20807
20808 /* Value found, make the actual copy (as far as possible). */
20809 copy_len = MIN(val_len, sysgetenv.vallen);
20810 if ((s=sys_datacopy(SELF, (vir_bytes) val_start,
20811 who, (vir_bytes) sysgetenv.val, copy_len)) != OK)
20812 return(s);
20813
20814 return OK;
20815 }
20816 default:
20817 return(EINVAL);
20818 }
20819 }