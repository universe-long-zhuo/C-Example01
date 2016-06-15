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
MINIX SOURCE CODE File: servers/fs/fs.h 919
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/fs.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
20900 /* This is the master header for fs. It includes some other files
20901 * and defines the principal constants.
20902 */
20903 #define _POSIX_SOURCE 1 /* tell headers to include POSIX stuff */
20904 #define _MINIX 1 /* tell headers to include MINIX stuff */
20905 #define _SYSTEM 1 /* tell headers that this is the kernel */
20906
20907 #define VERBOSE 0 /* show messages during initialization? */
20908
20909 /* The following are so basic, all the *.c files get them automatically. */
20910 #include <minix/config.h> /* MUST be first */
20911 #include <ansi.h> /* MUST be second */
20912 #include <sys/types.h>
20913 #include <minix/const.h>
20914 #include <minix/type.h>
20915 #include <minix/dmap.h>
20916
20917 #include <limits.h>
20918 #include <errno.h>
20919
20920 #include <minix/syslib.h>
20921 #include <minix/sysutil.h>
20922
20923 #include "const.h"
20924 #include "type.h"
20925 #include "proto.h"
20926 #include "glo.h"
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21000 /* Tables sizes */
21001 #define V1_NR_DZONES 7 /* # direct zone numbers in a V1 inode */
21002 #define V1_NR_TZONES 9 /* total # zone numbers in a V1 inode */
21003 #define V2_NR_DZONES 7 /* # direct zone numbers in a V2 inode */
21004 #define V2_NR_TZONES 10 /* total # zone numbers in a V2 inode */
21005
21006 #define NR_FILPS 128 /* # slots in filp table */
21007 #define NR_INODES 64 /* # slots in "in core" inode table */
21008 #define NR_SUPERS 8 /* # slots in super block table */
21009 #define NR_LOCKS 8 /* # slots in the file locking table */
21010
21011 /* The type of sizeof may be (unsigned) long. Use the following macro for
21012 * taking the sizes of small objects so that there are no surprises like
21013 * (small) long constants being passed to routines expecting an int.
21014 */
21015 #define usizeof(t) ((unsigned) sizeof(t))
21016
21017 /* File system types. */
21018 #define SUPER_MAGIC 0x137F /* magic number contained in super-block */
21019 #define SUPER_REV 0x7F13 /* magic # when 68000 disk read on PC or vv */
920 File: servers/fs/const.h MINIX SOURCE CODE
21020 #define SUPER_V2 0x2468 /* magic # for V2 file systems */
21021 #define SUPER_V2_REV 0x6824 /* V2 magic written on PC, read on 68K or vv */
21022 #define SUPER_V3 0x4d5a /* magic # for V3 file systems */
21023
21024 #define V1 1 /* version number of V1 file systems */
21025 #define V2 2 /* version number of V2 file systems */
21026 #define V3 3 /* version number of V3 file systems */
21027
21028 /* Miscellaneous constants */
21029 #define SU_UID ((uid_t) 0) /* super_user’s uid_t */
21030 #define SYS_UID ((uid_t) 0) /* uid_t for processes MM and INIT */
21031 #define SYS_GID ((gid_t) 0) /* gid_t for processes MM and INIT */
21032 #define NORMAL 0 /* forces get_block to do disk read */
21033 #define NO_READ 1 /* prevents get_block from doing disk read */
21034 #define PREFETCH 2 /* tells get_block not to read or mark dev */
21035
21036 #define XPIPE (-NR_TASKS-1) /* used in fp_task when susp’d on pipe */
21037 #define XLOCK (-NR_TASKS-2) /* used in fp_task when susp’d on lock */
21038 #define XPOPEN (-NR_TASKS-3) /* used in fp_task when susp’d on pipe open */
21039 #define XSELECT (-NR_TASKS-4) /* used in fp_task when susp’d on select */
21040
21041 #define NO_BIT ((bit_t) 0) /* returned by alloc_bit() to signal failure */
21042
21043 #define DUP_MASK 0100 /* mask to distinguish dup2 from dup */
21044
21045 #define LOOK_UP 0 /* tells search_dir to lookup string */
21046 #define ENTER 1 /* tells search_dir to make dir entry */
21047 #define DELETE 2 /* tells search_dir to delete entry */
21048 #define IS_EMPTY 3 /* tells search_dir to ret. OK or ENOTEMPTY */
21049
21050 #define CLEAN 0 /* disk and memory copies identical */
21051 #define DIRTY 1 /* disk and memory copies differ */
21052 #define ATIME 002 /* set if atime field needs updating */
21053 #define CTIME 004 /* set if ctime field needs updating */
21054 #define MTIME 010 /* set if mtime field needs updating */
21055
21056 #define BYTE_SWAP 0 /* tells conv2/conv4 to swap bytes */
21057
21058 #define END_OF_FILE (-104) /* eof detected */
21059
21060 #define ROOT_INODE 1 /* inode number for root directory */
21061 #define BOOT_BLOCK ((block_t) 0) /* block number of boot block */
21062 #define SUPER_BLOCK_BYTES (1024) /* bytes offset */
21063 #define START_BLOCK 2 /* first block of FS (not counting SB) */
21064
21065 #define DIR_ENTRY_SIZE usizeof (struct direct) /* # bytes/dir entry */
21066 #define NR_DIR_ENTRIES(b) ((b)/DIR_ENTRY_SIZE) /* # dir entries/blk */
21067 #define SUPER_SIZE usizeof (struct super_block) /* super_block size */
21068 #define PIPE_SIZE(b) (V1_NR_DZONES*(b)) /* pipe size in bytes */
21069
21070 #define FS_BITMAP_CHUNKS(b) ((b)/usizeof (bitchunk_t))/* # map chunks/blk */
21071 #define FS_BITCHUNK_BITS (usizeof(bitchunk_t) * CHAR_BIT)
21072 #define FS_BITS_PER_BLOCK(b) (FS_BITMAP_CHUNKS(b) * FS_BITCHUNK_BITS)
21073
21074 /* Derived sizes pertaining to the V1 file system. */
21075 #define V1_ZONE_NUM_SIZE usizeof (zone1_t) /* # bytes in V1 zone */
21076 #define V1_INODE_SIZE usizeof (d1_inode) /* bytes in V1 dsk ino */
21077
21078 /* # zones/indir block */
21079 #define V1_INDIRECTS (STATIC_BLOCK_SIZE/V1_ZONE_NUM_SIZE)
MINIX SOURCE CODE File: servers/fs/const.h 921
21080
21081 /* # V1 dsk inodes/blk */
21082 #define V1_INODES_PER_BLOCK (STATIC_BLOCK_SIZE/V1_INODE_SIZE)
21083
21084 /* Derived sizes pertaining to the V2 file system. */
21085 #define V2_ZONE_NUM_SIZE usizeof (zone_t) /* # bytes in V2 zone */
21086 #define V2_INODE_SIZE usizeof (d2_inode) /* bytes in V2 dsk ino */
21087 #define V2_INDIRECTS(b) ((b)/V2_ZONE_NUM_SIZE) /* # zones/indir block */
21088 #define V2_INODES_PER_BLOCK(b) ((b)/V2_INODE_SIZE)/* # V2 dsk inodes/blk */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/type.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21100 /* Declaration of the V1 inode as it is on the disk (not in core). */
21101 typedef struct { /* V1.x disk inode */
21102 mode_t d1_mode; /* file type, protection, etc. */
21103 uid_t d1_uid; /* user id of the file’s owner */
21104 off_t d1_size; /* current file size in bytes */
21105 time_t d1_mtime; /* when was file data last changed */
21106 u8_t d1_gid; /* group number */
21107 u8_t d1_nlinks; /* how many links to this file */
21108 u16_t d1_zone[V1_NR_TZONES]; /* block nums for direct, ind, and dbl ind */
21109 } d1_inode;
21110
21111 /* Declaration of the V2 inode as it is on the disk (not in core). */
21112 typedef struct { /* V2.x disk inode */
21113 mode_t d2_mode; /* file type, protection, etc. */
21114 u16_t d2_nlinks; /* how many links to this file. HACK! */
21115 uid_t d2_uid; /* user id of the file’s owner. */
21116 u16_t d2_gid; /* group number HACK! */
21117 off_t d2_size; /* current file size in bytes */
21118 time_t d2_atime; /* when was file data last accessed */
21119 time_t d2_mtime; /* when was file data last changed */
21120 time_t d2_ctime; /* when was inode data last changed */
21121 zone_t d2_zone[V2_NR_TZONES]; /* block nums for direct, ind, and dbl ind */
21122 } d2_inode;
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/proto.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21200 /* Function prototypes. */
21201
21202 #include "timers.h"
21203
21204 /* Structs used in prototypes must be declared as such first. */
21205 struct buf;
21206 struct filp;
21207 struct inode;
21208 struct super_block;
21209
21210 /* cache.c */
21211 _PROTOTYPE( zone_t alloc_zone, (Dev_t dev, zone_t z) );
21212 _PROTOTYPE( void flushall, (Dev_t dev) );
21213 _PROTOTYPE( void free_zone, (Dev_t dev, zone_t numb) );
21214 _PROTOTYPE( struct buf *get_block, (Dev_t dev, block_t block,int only_search));
922 File: servers/fs/proto.h MINIX SOURCE CODE
21215 _PROTOTYPE( void invalidate, (Dev_t device) );
21216 _PROTOTYPE( void put_block, (struct buf *bp, int block_type) );
21217 _PROTOTYPE( void rw_block, (struct buf *bp, int rw_flag) );
21218 _PROTOTYPE( void rw_scattered, (Dev_t dev,
21219 struct buf **bufq, int bufqsize, int rw_flag) );
21220
21221 /* device.c */
21222 _PROTOTYPE( int dev_open, (Dev_t dev, int proc, int flags) );
21223 _PROTOTYPE( void dev_close, (Dev_t dev) );
21224 _PROTOTYPE( int dev_io, (int op, Dev_t dev, int proc, void *buf,
21225 off_t pos, int bytes, int flags) );
21226 _PROTOTYPE( int gen_opcl, (int op, Dev_t dev, int proc, int flags) );
21227 _PROTOTYPE( void gen_io, (int task_nr, message *mess_ptr) );
21228 _PROTOTYPE( int no_dev, (int op, Dev_t dev, int proc, int flags) );
21229 _PROTOTYPE( int tty_opcl, (int op, Dev_t dev, int proc, int flags) );
21230 _PROTOTYPE( int ctty_opcl, (int op, Dev_t dev, int proc, int flags) );
21231 _PROTOTYPE( int clone_opcl, (int op, Dev_t dev, int proc, int flags) );
21232 _PROTOTYPE( void ctty_io, (int task_nr, message *mess_ptr) );
21233 _PROTOTYPE( int do_ioctl, (void) );
21234 _PROTOTYPE( int do_setsid, (void) );
21235 _PROTOTYPE( void dev_status, (message *) );
21236
21237 /* dmp.c */
21238 _PROTOTYPE( int do_fkey_pressed, (void) );
21239
21240 /* dmap.c */
21241 _PROTOTYPE( int do_devctl, (void) );
21242 _PROTOTYPE( void build_dmap, (void) );
21243 _PROTOTYPE( int map_driver, (int major, int proc_nr, int dev_style) );
21244
21245 /* filedes.c */
21246 _PROTOTYPE( struct filp *find_filp, (struct inode *rip, mode_t bits) );
21247 _PROTOTYPE( int get_fd, (int start, mode_t bits, int *k, struct filp **fpt) );
21248 _PROTOTYPE( struct filp *get_filp, (int fild) );
21249
21250 /* inode.c */
21251 _PROTOTYPE( struct inode *alloc_inode, (dev_t dev, mode_t bits) );
21252 _PROTOTYPE( void dup_inode, (struct inode *ip) );
21253 _PROTOTYPE( void free_inode, (Dev_t dev, Ino_t numb) );
21254 _PROTOTYPE( struct inode *get_inode, (Dev_t dev, int numb) );
21255 _PROTOTYPE( void put_inode, (struct inode *rip) );
21256 _PROTOTYPE( void update_times, (struct inode *rip) );
21257 _PROTOTYPE( void rw_inode, (struct inode *rip, int rw_flag) );
21258 _PROTOTYPE( void wipe_inode, (struct inode *rip) );
21259
21260 /* link.c */
21261 _PROTOTYPE( int do_link, (void) );
21262 _PROTOTYPE( int do_unlink, (void) );
21263 _PROTOTYPE( int do_rename, (void) );
21264 _PROTOTYPE( void truncate, (struct inode *rip) );
21265
21266 /* lock.c */
21267 _PROTOTYPE( int lock_op, (struct filp *f, int req) );
21268 _PROTOTYPE( void lock_revive, (void) );
21269
21270 /* main.c */
21271 _PROTOTYPE( int main, (void) );
21272 _PROTOTYPE( void reply, (int whom, int result) );
21273
21274 /* misc.c */
MINIX SOURCE CODE File: servers/fs/proto.h 923
21275 _PROTOTYPE( int do_dup, (void) );
21276 _PROTOTYPE( int do_exit, (void) );
21277 _PROTOTYPE( int do_fcntl, (void) );
21278 _PROTOTYPE( int do_fork, (void) );
21279 _PROTOTYPE( int do_exec, (void) );
21280 _PROTOTYPE( int do_revive, (void) );
21281 _PROTOTYPE( int do_set, (void) );
21282 _PROTOTYPE( int do_sync, (void) );
21283 _PROTOTYPE( int do_fsync, (void) );
21284 _PROTOTYPE( int do_reboot, (void) );
21285 _PROTOTYPE( int do_svrctl, (void) );
21286 _PROTOTYPE( int do_getsysinfo, (void) );
21287
21288 /* mount.c */
21289 _PROTOTYPE( int do_mount, (void) );
21290 _PROTOTYPE( int do_umount, (void) );
21291 _PROTOTYPE( int unmount, (Dev_t dev) );
21292
21293 /* open.c */
21294 _PROTOTYPE( int do_close, (void) );
21295 _PROTOTYPE( int do_creat, (void) );
21296 _PROTOTYPE( int do_lseek, (void) );
21297 _PROTOTYPE( int do_mknod, (void) );
21298 _PROTOTYPE( int do_mkdir, (void) );
21299 _PROTOTYPE( int do_open, (void) );
21300
21301 /* path.c */
21302 _PROTOTYPE( struct inode *advance,(struct inode *dirp, char string[NAME_MAX]));
21303 _PROTOTYPE( int search_dir, (struct inode *ldir_ptr,
21304 char string [NAME_MAX], ino_t *numb, int flag) );
21305 _PROTOTYPE( struct inode *eat_path, (char *path) );
21306 _PROTOTYPE( struct inode *last_dir, (char *path, char string [NAME_MAX]));
21307
21308 /* pipe.c */
21309 _PROTOTYPE( int do_pipe, (void) );
21310 _PROTOTYPE( int do_unpause, (void) );
21311 _PROTOTYPE( int pipe_check, (struct inode *rip, int rw_flag,
21312 int oflags, int bytes, off_t position, int *canwrite, int notouch));
21313 _PROTOTYPE( void release, (struct inode *ip, int call_nr, int count) );
21314 _PROTOTYPE( void revive, (int proc_nr, int bytes) );
21315 _PROTOTYPE( void suspend, (int task) );
21316 _PROTOTYPE( int select_request_pipe, (struct filp *f, int *ops, int bl) );
21317 _PROTOTYPE( int select_cancel_pipe, (struct filp *f) );
21318 _PROTOTYPE( int select_match_pipe, (struct filp *f) );
21319
21320 /* protect.c */
21321 _PROTOTYPE( int do_access, (void) );
21322 _PROTOTYPE( int do_chmod, (void) );
21323 _PROTOTYPE( int do_chown, (void) );
21324 _PROTOTYPE( int do_umask, (void) );
21325 _PROTOTYPE( int forbidden, (struct inode *rip, mode_t access_desired) );
21326 _PROTOTYPE( int read_only, (struct inode *ip) );
21327
21328 /* read.c */
21329 _PROTOTYPE( int do_read, (void) );
21330 _PROTOTYPE( struct buf *rahead, (struct inode *rip, block_t baseblock,
21331 off_t position, unsigned bytes_ahead) );
21332 _PROTOTYPE( void read_ahead, (void) );
21333 _PROTOTYPE( block_t read_map, (struct inode *rip, off_t position) );
21334 _PROTOTYPE( int read_write, (int rw_flag) );
924 File: servers/fs/proto.h MINIX SOURCE CODE
21335 _PROTOTYPE( zone_t rd_indir, (struct buf *bp, int index) );
21336
21337 /* stadir.c */
21338 _PROTOTYPE( int do_chdir, (void) );
21339 _PROTOTYPE( int do_fchdir, (void) );
21340 _PROTOTYPE( int do_chroot, (void) );
21341 _PROTOTYPE( int do_fstat, (void) );
21342 _PROTOTYPE( int do_stat, (void) );
21343 _PROTOTYPE( int do_fstatfs, (void) );
21344
21345 /* super.c */
21346 _PROTOTYPE( bit_t alloc_bit, (struct super_block *sp, int map, bit_t origin));
21347 _PROTOTYPE( void free_bit, (struct super_block *sp, int map,
21348 bit_t bit_returned) );
21349 _PROTOTYPE( struct super_block *get_super, (Dev_t dev) );
21350 _PROTOTYPE( int mounted, (struct inode *rip) );
21351 _PROTOTYPE( int read_super, (struct super_block *sp) );
21352 _PROTOTYPE( int get_block_size, (dev_t dev) );
21353
21354 /* time.c */
21355 _PROTOTYPE( int do_stime, (void) );
21356 _PROTOTYPE( int do_utime, (void) );
21357
21358 /* utility.c */
21359 _PROTOTYPE( time_t clock_time, (void) );
21360 _PROTOTYPE( unsigned conv2, (int norm, int w) );
21361 _PROTOTYPE( long conv4, (int norm, long x) );
21362 _PROTOTYPE( int fetch_name, (char *path, int len, int flag) );
21363 _PROTOTYPE( int no_sys, (void) );
21364 _PROTOTYPE( void panic, (char *who, char *mess, int num) );
21365
21366 /* write.c */
21367 _PROTOTYPE( void clear_zone, (struct inode *rip, off_t pos, int flag) );
21368 _PROTOTYPE( int do_write, (void) );
21369 _PROTOTYPE( struct buf *new_block, (struct inode *rip, off_t position) );
21370 _PROTOTYPE( void zero_block, (struct buf *bp) );
21371
21372 /* select.c */
21373 _PROTOTYPE( int do_select, (void) );
21374 _PROTOTYPE( int select_callback, (struct filp *, int ops) );
21375 _PROTOTYPE( void select_forget, (int fproc) );
21376 _PROTOTYPE( void select_timeout_check, (timer_t *) );
21377 _PROTOTYPE( void init_select, (void) );
21378 _PROTOTYPE( int select_notified, (int major, int minor, int ops) );
21379
21380 /* timers.c */
21381 _PROTOTYPE( void fs_set_timer, (timer_t *tp, int delta, tmr_func_t watchdog, int arg));
21382 _PROTOTYPE( void fs_expire_timers, (clock_t now) );
21383 _PROTOTYPE( void fs_cancel_timer, (timer_t *tp) );
21384 _PROTOTYPE( void fs_init_timer, (timer_t *tp) );
21385
21386 /* cdprobe.c */
21387 _PROTOTYPE( int cdprobe, (void) );
MINIX SOURCE CODE File: servers/fs/glo.h 925
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/glo.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21400 /* EXTERN should be extern except for the table file */
21401 #ifdef _TABLE
21402 #undef EXTERN
21403 #define EXTERN
21404 #endif
21405
21406 /* File System global variables */
21407 EXTERN struct fproc *fp; /* pointer to caller’s fproc struct */
21408 EXTERN int super_user; /* 1 if caller is super_user, else 0 */
21409 EXTERN int susp_count; /* number of procs suspended on pipe */
21410 EXTERN int nr_locks; /* number of locks currently in place */
21411 EXTERN int reviving; /* number of pipe processes to be revived */
21412 EXTERN off_t rdahedpos; /* position to read ahead */
21413 EXTERN struct inode *rdahed_inode; /* pointer to inode to read ahead */
21414 EXTERN Dev_t root_dev; /* device number of the root device */
21415 EXTERN time_t boottime; /* time in seconds at system boot */
21416
21417 /* The parameters of the call are kept here. */
21418 EXTERN message m_in; /* the input message itself */
21419 EXTERN message m_out; /* the output message used for reply */
21420 EXTERN int who; /* caller’s proc number */
21421 EXTERN int call_nr; /* system call number */
21422 EXTERN char user_path[PATH_MAX];/* storage for user path name */
21423
21424 /* The following variables are used for returning results to the caller. */
21425 EXTERN int err_code; /* temporary storage for error number */
21426 EXTERN int rdwt_err; /* status of last disk i/o request */
21427
21428 /* Data initialized elsewhere. */
21429 extern _PROTOTYPE (int (*call_vec[]), (void) ); /* sys call table */
21430 extern char dot1[2]; /* dot1 (&dot1[0]) and dot2 (&dot2[0]) have a special */
21431 extern char dot2[3]; /* meaning to search_dir: no access permission check. */
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
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/lock.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21800 /* This is the file locking table. Like the filp table, it points to the
21801 * inode table, however, in this case to achieve advisory locking.
21802 */
21803 EXTERN struct file_lock {
21804 short lock_type; /* F_RDLOCK or F_WRLOCK; 0 means unused slot */
21805 pid_t lock_pid; /* pid of the process holding the lock */
21806 struct inode *lock_inode; /* pointer to the inode locked */
21807 off_t lock_first; /* offset of first byte locked */
21808 off_t lock_last; /* offset of last byte locked */
21809 } file_lock[NR_LOCKS];
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/inode.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21900 /* Inode table. This table holds inodes that are currently in use. In some
21901 * cases they have been opened by an open() or creat() system call, in other
21902 * cases the file system itself needs the inode for one reason or another,
21903 * such as to search a directory for a path name.
21904 * The first part of the struct holds fields that are present on the
21905 * disk; the second part holds fields not present on the disk.
21906 * The disk inode part is also declared in "type.h" as ’d1_inode’ for V1
21907 * file systems and ’d2_inode’ for V2 file systems.
21908 */
21909
21910 EXTERN struct inode {
21911 mode_t i_mode; /* file type, protection, etc. */
21912 nlink_t i_nlinks; /* how many links to this file */
21913 uid_t i_uid; /* user id of the file’s owner */
21914 gid_t i_gid; /* group number */
21915 off_t i_size; /* current file size in bytes */
21916 time_t i_atime; /* time of last access (V2 only) */
21917 time_t i_mtime; /* when was file data last changed */
21918 time_t i_ctime; /* when was inode itself changed (V2 only)*/
21919 zone_t i_zone[V2_NR_TZONES]; /* zone numbers for direct, ind, and dbl ind */
21920
21921 /* The following items are not present on the disk. */
21922 dev_t i_dev; /* which device is the inode on */
21923 ino_t i_num; /* inode number on its (minor) device */
21924 int i_count; /* # times inode used; 0 means slot is free */
MINIX SOURCE CODE File: servers/fs/inode.h 929
21925 int i_ndzones; /* # direct zones (Vx_NR_DZONES) */
21926 int i_nindirs; /* # indirect zones per indirect block */
21927 struct super_block *i_sp; /* pointer to super block for inode’s device */
21928 char i_dirt; /* CLEAN or DIRTY */
21929 char i_pipe; /* set to I_PIPE if pipe */
21930 char i_mount; /* this bit is set if file mounted on */
21931 char i_seek; /* set on LSEEK, cleared on READ/WRITE */
21932 char i_update; /* the ATIME, CTIME, and MTIME bits are here */
21933 } inode[NR_INODES];
21934
21935 #define NIL_INODE (struct inode *) 0 /* indicates absence of inode slot */
21936
21937 /* Field values. Note that CLEAN and DIRTY are defined in "const.h" */
21938 #define NO_PIPE 0 /* i_pipe is NO_PIPE if inode is not a pipe */
21939 #define I_PIPE 1 /* i_pipe is I_PIPE if inode is a pipe */
21940 #define NO_MOUNT 0 /* i_mount is NO_MOUNT if file not mounted on*/
21941 #define I_MOUNT 1 /* i_mount is I_MOUNT if file mounted on */
21942 #define NO_SEEK 0 /* i_seek = NO_SEEK if last op was not SEEK */
21943 #define ISEEK 1 /* i_seek = ISEEK if last op was SEEK */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/param.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
22000 /* The following names are synonyms for the variables in the input message. */
22001 #define acc_time m2_l1
22002 #define addr m1_i3
22003 #define buffer m1_p1
22004 #define child m1_i2
22005 #define co_mode m1_i1
22006 #define eff_grp_id m1_i3
22007 #define eff_user_id m1_i3
22008 #define erki m1_p1
22009 #define fd m1_i1
22010 #define fd2 m1_i2
22011 #define ioflags m1_i3
22012 #define group m1_i3
22013 #define real_grp_id m1_i2
22014 #define ls_fd m2_i1
22015 #define mk_mode m1_i2
22016 #define mk_z0 m1_i3
22017 #define mode m3_i2
22018 #define c_mode m1_i3
22019 #define c_name m1_p1
22020 #define name m3_p1
22021 #define name1 m1_p1
22022 #define name2 m1_p2
22023 #define name_length m3_i1
22024 #define name1_length m1_i1
22025 #define name2_length m1_i2
22026 #define nbytes m1_i2
22027 #define owner m1_i2
22028 #define parent m1_i1
22029 #define pathname m3_ca1
22030 #define pid m1_i3
22031 #define pro m1_i1
22032 #define ctl_req m4_l1
22033 #define driver_nr m4_l2
22034 #define dev_nr m4_l3
930 File: servers/fs/param.h MINIX SOURCE CODE
22035 #define dev_style m4_l4
22036 #define rd_only m1_i3
22037 #define real_user_id m1_i2
22038 #define request m1_i2
22039 #define sig m1_i2
22040 #define slot1 m1_i1
22041 #define tp m2_l1
22042 #define utime_actime m2_l1
22043 #define utime_modtime m2_l2
22044 #define utime_file m2_p1
22045 #define utime_length m2_i1
22046 #define utime_strlen m2_i2
22047 #define whence m2_i2
22048 #define svrctl_req m2_i1
22049 #define svrctl_argp m2_p1
22050 #define pm_stime m1_i1
22051 #define info_what m1_i1
22052 #define info_where m1_p1
22053
22054 /* The following names are synonyms for the variables in the output message. */
22055 #define reply_type m_type
22056 #define reply_l1 m2_l1
22057 #define reply_i1 m1_i1
22058 #define reply_i2 m1_i2
22059 #define reply_t1 m4_l1
22060 #define reply_t2 m4_l2
22061 #define reply_t3 m4_l3
22062 #define reply_t4 m4_l4
22063 #define reply_t5 m4_l5
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/super.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
22100 /* Super block table. The root file system and every mounted file system
22101 * has an entry here. The entry holds information about the sizes of the bit
22102 * maps and inodes. The s_ninodes field gives the number of inodes available
22103 * for files and directories, including the root directory. Inode 0 is
22104 * on the disk, but not used. Thus s_ninodes = 4 means that 5 bits will be
22105 * used in the bit map, bit 0, which is always 1 and not used, and bits 1-4
22106 * for files and directories. The disk layout is:
22107 *
22108 * Item # blocks
22109 * boot block 1
22110 * super block 1 (offset 1kB)
22111 * inode map s_imap_blocks
22112 * zone map s_zmap_blocks
22113 * inodes (s_ninodes + ’inodes per block’ - 1)/’inodes per block’
22114 * unused whatever is needed to fill out the current zone
22115 * data zones (s_zones - s_firstdatazone) << s_log_zone_size
22116 *
22117 * A super_block slot is free if s_dev == NO_DEV.
22118 */
22119
22120 EXTERN struct super_block {
22121 ino_t s_ninodes; /* # usable inodes on the minor device */
22122 zone1_t s_nzones; /* total device size, including bit maps etc */
22123 short s_imap_blocks; /* # of blocks used by inode bit map */
22124 short s_zmap_blocks; /* # of blocks used by zone bit map */
MINIX SOURCE CODE File: servers/fs/super.h 931
22125 zone1_t s_firstdatazone; /* number of first data zone */
22126 short s_log_zone_size; /* log2 of blocks/zone */
22127 short s_pad; /* try to avoid compiler-dependent padding */
22128 off_t s_max_size; /* maximum file size on this device */
22129 zone_t s_zones; /* number of zones (replaces s_nzones in V2) */
22130 short s_magic; /* magic number to recognize super-blocks */
22131
22132 /* The following items are valid on disk only for V3 and above */
22133
22134 /* The block size in bytes. Minimum MIN_BLOCK SIZE. SECTOR_SIZE
22135 * multiple. If V1 or V2 filesystem, this should be
22136 * initialised to STATIC_BLOCK_SIZE. Maximum MAX_BLOCK_SIZE.
22137 */
22138 short s_pad2; /* try to avoid compiler-dependent padding */
22139 unsigned short s_block_size; /* block size in bytes. */
22140 char s_disk_version; /* filesystem format sub-version */
22141
22142 /* The following items are only used when the super_block is in memory. */
22143 struct inode *s_isup; /* inode for root dir of mounted file sys */
22144 struct inode *s_imount; /* inode mounted on */
22145 unsigned s_inodes_per_block; /* precalculated from magic number */
22146 dev_t s_dev; /* whose super block is this? */
22147 int s_rd_only; /* set to 1 iff file sys mounted read only */
22148 int s_native; /* set to 1 iff not byte swapped file system */
22149 int s_version; /* file system version, zero means bad magic */
22150 int s_ndzones; /* # direct zones in an inode */
22151 int s_nindirs; /* # indirect zones per indirect block */
22152 bit_t s_isearch; /* inodes below this bit number are in use */
22153 bit_t s_zsearch; /* all zones below this bit number are in use*/
22154 } super_block[NR_SUPERS];
22155
22156 #define NIL_SUPER (struct super_block *) 0
22157 #define IMAP 0 /* operating on the inode bit map */
22158 #define ZMAP 1 /* operating on the zone bit map */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/table.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
22200 /* This file contains the table used to map system call numbers onto the
22201 * routines that perform them.
22202 */
22203
22204 #define _TABLE
22205
22206 #include "fs.h"
22207 #include <minix/callnr.h>
22208 #include <minix/com.h>
22209 #include "buf.h"
22210 #include "file.h"
22211 #include "fproc.h"
22212 #include "inode.h"
22213 #include "lock.h"
22214 #include "super.h"
22215
22216 PUBLIC _PROTOTYPE (int (*call_vec[]), (void) ) = {
22217 no_sys, /* 0 = unused */
22218 do_exit, /* 1 = exit */
22219 do_fork, /* 2 = fork */
932 File: servers/fs/table.c MINIX SOURCE CODE
22220 do_read, /* 3 = read */
22221 do_write, /* 4 = write */
22222 do_open, /* 5 = open */
22223 do_close, /* 6 = close */
22224 no_sys, /* 7 = wait */
22225 do_creat, /* 8 = creat */
22226 do_link, /* 9 = link */
22227 do_unlink, /* 10 = unlink */
22228 no_sys, /* 11 = waitpid */
22229 do_chdir, /* 12 = chdir */
22230 no_sys, /* 13 = time */
22231 do_mknod, /* 14 = mknod */
22232 do_chmod, /* 15 = chmod */
22233 do_chown, /* 16 = chown */
22234 no_sys, /* 17 = break */
22235 do_stat, /* 18 = stat */
22236 do_lseek, /* 19 = lseek */
22237 no_sys, /* 20 = getpid */
22238 do_mount, /* 21 = mount */
22239 do_umount, /* 22 = umount */
22240 do_set, /* 23 = setuid */
22241 no_sys, /* 24 = getuid */
22242 do_stime, /* 25 = stime */
22243 no_sys, /* 26 = ptrace */
22244 no_sys, /* 27 = alarm */
22245 do_fstat, /* 28 = fstat */
22246 no_sys, /* 29 = pause */
22247 do_utime, /* 30 = utime */
22248 no_sys, /* 31 = (stty) */
22249 no_sys, /* 32 = (gtty) */
22250 do_access, /* 33 = access */
22251 no_sys, /* 34 = (nice) */
22252 no_sys, /* 35 = (ftime) */
22253 do_sync, /* 36 = sync */
22254 no_sys, /* 37 = kill */
22255 do_rename, /* 38 = rename */
22256 do_mkdir, /* 39 = mkdir */
22257 do_unlink, /* 40 = rmdir */
22258 do_dup, /* 41 = dup */
22259 do_pipe, /* 42 = pipe */
22260 no_sys, /* 43 = times */
22261 no_sys, /* 44 = (prof) */
22262 no_sys, /* 45 = unused */
22263 do_set, /* 46 = setgid */
22264 no_sys, /* 47 = getgid */
22265 no_sys, /* 48 = (signal)*/
22266 no_sys, /* 49 = unused */
22267 no_sys, /* 50 = unused */
22268 no_sys, /* 51 = (acct) */
22269 no_sys, /* 52 = (phys) */
22270 no_sys, /* 53 = (lock) */
22271 do_ioctl, /* 54 = ioctl */
22272 do_fcntl, /* 55 = fcntl */
22273 no_sys, /* 56 = (mpx) */
22274 no_sys, /* 57 = unused */
22275 no_sys, /* 58 = unused */
22276 do_exec, /* 59 = execve */
22277 do_umask, /* 60 = umask */
22278 do_chroot, /* 61 = chroot */
22279 do_setsid, /* 62 = setsid */
MINIX SOURCE CODE File: servers/fs/table.c 933
22280 no_sys, /* 63 = getpgrp */
22281
22282 no_sys, /* 64 = KSIG: signals originating in the kernel */
22283 do_unpause, /* 65 = UNPAUSE */
22284 no_sys, /* 66 = unused */
22285 do_revive, /* 67 = REVIVE */
22286 no_sys, /* 68 = TASK_REPLY */
22287 no_sys, /* 69 = unused */
22288 no_sys, /* 70 = unused */
22289 no_sys, /* 71 = si */
22290 no_sys, /* 72 = sigsuspend */
22291 no_sys, /* 73 = sigpending */
22292 no_sys, /* 74 = sigprocmask */
22293 no_sys, /* 75 = sigreturn */
22294 do_reboot, /* 76 = reboot */
22295 do_svrctl, /* 77 = svrctl */
22296
22297 no_sys, /* 78 = unused */
22298 do_getsysinfo, /* 79 = getsysinfo */
22299 no_sys, /* 80 = unused */
22300 do_devctl, /* 81 = devctl */
22301 do_fstatfs, /* 82 = fstatfs */
22302 no_sys, /* 83 = memalloc */
22303 no_sys, /* 84 = memfree */
22304 do_select, /* 85 = select */
22305 do_fchdir, /* 86 = fchdir */
22306 do_fsync, /* 87 = fsync */
22307 no_sys, /* 88 = getpriority */
22308 no_sys, /* 89 = setpriority */
22309 no_sys, /* 90 = gettimeofday */
22310 };
22311 /* This should not fail with "array size is negative": */
22312 extern int dummy[sizeof(call_vec) == NCALLS * sizeof(call_vec[0]) ? 1 : -1];
22313
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/cache.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
22400 /* The file system maintains a buffer cache to reduce the number of disk
22401 * accesses needed. Whenever a read or write to the disk is done, a check is
22402 * first made to see if the block is in the cache. This file manages the
22403 * cache.
22404 *
22405 * The entry points into this file are:
22406 * get_block: request to fetch a block for reading or writing from cache
22407 * put_block: return a block previously requested with get_block
22408 * alloc_zone: allocate a new zone (to increase the length of a file)
22409 * free_zone: release a zone (when a file is removed)
22410 * rw_block: read or write a block from the disk itself
22411 * invalidate: remove all the cache blocks on some device
22412 */
22413
22414 #include "fs.h"
22415 #include <minix/com.h>
22416 #include "buf.h"
22417 #include "file.h"
22418 #include "fproc.h"
22419 #include "super.h"
934 File: servers/fs/cache.c MINIX SOURCE CODE
22420
22421 FORWARD _PROTOTYPE( void rm_lru, (struct buf *bp) );
22422
22423 /*===========================================================================*
22424 * get_block *
22425 *===========================================================================*/
22426 PUBLIC struct buf *get_block(dev, block, only_search)
22427 register dev_t dev; /* on which device is the block? */
22428 register block_t block; /* which block is wanted? */
22429 int only_search; /* if NO_READ, don’t read, else act normal */
22430 {
22431 /* Check to see if the requested block is in the block cache. If so, return
22432 * a pointer to it. If not, evict some other block and fetch it (unless
22433 * ’only_search’ is 1). All the blocks in the cache that are not in use
22434 * are linked together in a chain, with ’front’ pointing to the least recently
22435 * used block and ’rear’ to the most recently used block. If ’only_search’ is
22436 * 1, the block being requested will be overwritten in its entirety, so it is
22437 * only necessary to see if it is in the cache; if it is not, any free buffer
22438 * will do. It is not necessary to actually read the block in from disk.
22439 * If ’only_search’ is PREFETCH, the block need not be read from the disk,
22440 * and the device is not to be marked on the block, so callers can tell if
22441 * the block returned is valid.
22442 * In addition to the LRU chain, there is also a hash chain to link together
22443 * blocks whose block numbers end with the same bit strings, for fast lookup.
22444 */
22445
22446 int b;
22447 register struct buf *bp, *prev_ptr;
22448
22449 /* Search the hash chain for (dev, block). Do_read() can use
22450 * get_block(NO_DEV ...) to get an unnamed block to fill with zeros when
22451 * someone wants to read from a hole in a file, in which case this search
22452 * is skipped
22453 */
22454 if (dev != NO_DEV) {
22455 b = (int) block & HASH_MASK;
22456 bp = buf_hash[b];
22457 while (bp != NIL_BUF) {
22458 if (bp->b_blocknr == block && bp->b_dev == dev) {
22459 /* Block needed has been found. */
22460 if (bp->b_count == 0) rm_lru(bp);
22461 bp->b_count++; /* record that block is in use */
22462
22463 return(bp);
22464 } else {
22465 /* This block is not the one sought. */
22466 bp = bp->b_hash; /* move to next block on hash chain */
22467 }
22468 }
22469 }
22470
22471 /* Desired block is not on available chain. Take oldest block (’front’). */
22472 if ((bp = front) == NIL_BUF) panic(__FILE__,"all buffers in use", NR_BUFS);
22473 rm_lru(bp);
22474
22475 /* Remove the block that was just taken from its hash chain. */
22476 b = (int) bp->b_blocknr & HASH_MASK;
22477 prev_ptr = buf_hash[b];
22478 if (prev_ptr == bp) {
22479 buf_hash[b] = bp->b_hash;
MINIX SOURCE CODE File: servers/fs/cache.c 935
22480 } else {
22481 /* The block just taken is not on the front of its hash chain. */
22482 while (prev_ptr->b_hash != NIL_BUF)
22483 if (prev_ptr->b_hash == bp) {
22484 prev_ptr->b_hash = bp->b_hash; /* found it */
22485 break;
22486 } else {
22487 prev_ptr = prev_ptr->b_hash; /* keep looking */
22488 }
22489 }
22490
22491 /* If the block taken is dirty, make it clean by writing it to the disk.
22492 * Avoid hysteresis by flushing all other dirty blocks for the same device.
22493 */
22494 if (bp->b_dev != NO_DEV) {
22495 if (bp->b_dirt == DIRTY) flushall(bp->b_dev);
22496 }
22497
22498 /* Fill in block’s parameters and add it to the hash chain where it goes. */
22499 bp->b_dev = dev; /* fill in device number */
22500 bp->b_blocknr = block; /* fill in block number */
22501 bp->b_count++; /* record that block is being used */
22502 b = (int) bp->b_blocknr & HASH_MASK;
22503 bp->b_hash = buf_hash[b];
22504 buf_hash[b] = bp; /* add to hash list */
22505
22506 /* Go get the requested block unless searching or prefetching. */
22507 if (dev != NO_DEV) {
22508 if (only_search == PREFETCH) bp->b_dev = NO_DEV;
22509 else
22510 if (only_search == NORMAL) {
22511 rw_block(bp, READING);
22512 }
22513 }
22514 return(bp); /* return the newly acquired block */
22515 }
22517 /*===========================================================================*
22518 * put_block *
22519 *===========================================================================*/
22520 PUBLIC void put_block(bp, block_type)
22521 register struct buf *bp; /* pointer to the buffer to be released */
22522 int block_type; /* INODE_BLOCK, DIRECTORY_BLOCK, or whatever */
22523 {
22524 /* Return a block to the list of available blocks. Depending on ’block_type’
22525 * it may be put on the front or rear of the LRU chain. Blocks that are
22526 * expected to be needed again shortly (e.g., partially full data blocks)
22527 * go on the rear; blocks that are unlikely to be needed again shortly
22528 * (e.g., full data blocks) go on the front. Blocks whose loss can hurt
22529 * the integrity of the file system (e.g., inode blocks) are written to
22530 * disk immediately if they are dirty.
22531 */
22532 if (bp == NIL_BUF) return; /* it is easier to check here than in caller */
22533
22534 bp->b_count--; /* there is one use fewer now */
22535 if (bp->b_count != 0) return; /* block is still in use */
22536
22537 bufs_in_use--; /* one fewer block buffers in use */
22538
22539 /* Put this block back on the LRU chain. If the ONE_SHOT bit is set in
936 File: servers/fs/cache.c MINIX SOURCE CODE
22540 * ’block_type’, the block is not likely to be needed again shortly, so put
22541 * it on the front of the LRU chain where it will be the first one to be
22542 * taken when a free buffer is needed later.
22543 */
22544 if (bp->b_dev == DEV_RAM || block_type & ONE_SHOT) {
22545 /* Block probably won’t be needed quickly. Put it on front of chain.
22546 * It will be the next block to be evicted from the cache.
22547 */
22548 bp->b_prev = NIL_BUF;
22549 bp->b_next = front;
22550 if (front == NIL_BUF)
22551 rear = bp; /* LRU chain was empty */
22552 else
22553 front->b_prev = bp;
22554 front = bp;
22555 } else {
22556 /* Block probably will be needed quickly. Put it on rear of chain.
22557 * It will not be evicted from the cache for a long time.
22558 */
22559 bp->b_prev = rear;
22560 bp->b_next = NIL_BUF;
22561 if (rear == NIL_BUF)
22562 front = bp;
22563 else
22564 rear->b_next = bp;
22565 rear = bp;
22566 }
22567
22568 /* Some blocks are so important (e.g., inodes, indirect blocks) that they
22569 * should be written to the disk immediately to avoid messing up the file
22570 * system in the event of a crash.
22571 */
22572 if ((block_type & WRITE_IMMED) && bp->b_dirt==DIRTY && bp->b_dev != NO_DEV) {
22573 rw_block(bp, WRITING);
22574 }
22575 }
22577 /*===========================================================================*
22578 * alloc_zone *
22579 *===========================================================================*/
22580 PUBLIC zone_t alloc_zone(dev, z)
22581 dev_t dev; /* device where zone wanted */
22582 zone_t z; /* try to allocate new zone near this one */
22583 {
22584 /* Allocate a new zone on the indicated device and return its number. */
22585
22586 int major, minor;
22587 bit_t b, bit;
22588 struct super_block *sp;
22589
22590 /* Note that the routine alloc_bit() returns 1 for the lowest possible
22591 * zone, which corresponds to sp->s_firstdatazone. To convert a value
22592 * between the bit number, ’b’, used by alloc_bit() and the zone number, ’z’,
22593 * stored in the inode, use the formula:
22594 * z = b + sp->s_firstdatazone - 1
22595 * Alloc_bit() never returns 0, since this is used for NO_BIT (failure).
22596 */
22597 sp = get_super(dev);
22598
22599 /* If z is 0, skip initial part of the map known to be fully in use. */
MINIX SOURCE CODE File: servers/fs/cache.c 937
22600 if (z == sp->s_firstdatazone) {
22601 bit = sp->s_zsearch;
22602 } else {
22603 bit = (bit_t) z - (sp->s_firstdatazone - 1);
22604 }
22605 b = alloc_bit(sp, ZMAP, bit);
22606 if (b == NO_BIT) {
22607 err_code = ENOSPC;
22608 major = (int) (sp->s_dev >> MAJOR) & BYTE;
22609 minor = (int) (sp->s_dev >> MINOR) & BYTE;
22610 printf("No space on %sdevice %d/%d\n",
22611 sp->s_dev == root_dev ? "root " : "", major, minor);
22612 return(NO_ZONE);
22613 }
22614 if (z == sp->s_firstdatazone) sp->s_zsearch = b; /* for next time */
22615 return(sp->s_firstdatazone - 1 + (zone_t) b);
22616 }
22618 /*===========================================================================*
22619 * free_zone *
22620 *===========================================================================*/
22621 PUBLIC void free_zone(dev, numb)
22622 dev_t dev; /* device where zone located */
22623 zone_t numb; /* zone to be returned */
22624 {
22625 /* Return a zone. */
22626
22627 register struct super_block *sp;
22628 bit_t bit;
22629
22630 /* Locate the appropriate super_block and return bit. */
22631 sp = get_super(dev);
22632 if (numb < sp->s_firstdatazone || numb >= sp->s_zones) return;
22633 bit = (bit_t) (numb - (sp->s_firstdatazone - 1));
22634 free_bit(sp, ZMAP, bit);
22635 if (bit < sp->s_zsearch) sp->s_zsearch = bit;
22636 }
22638 /*===========================================================================*
22639 * rw_block *
22640 *===========================================================================*/
22641 PUBLIC void rw_block(bp, rw_flag)
22642 register struct buf *bp; /* buffer pointer */
22643 int rw_flag; /* READING or WRITING */
22644 {
22645 /* Read or write a disk block. This is the only routine in which actual disk
22646 * I/O is invoked. If an error occurs, a message is printed here, but the error
22647 * is not reported to the caller. If the error occurred while purging a block
22648 * from the cache, it is not clear what the caller could do about it anyway.
22649 */
22650
22651 int r, op;
22652 off_t pos;
22653 dev_t dev;
22654 int block_size;
22655
22656 block_size = get_block_size(bp->b_dev);
22657
22658 if ( (dev = bp->b_dev) != NO_DEV) {
22659 pos = (off_t) bp->b_blocknr * block_size;
938 File: servers/fs/cache.c MINIX SOURCE CODE
22660 op = (rw_flag == READING ? DEV_READ : DEV_WRITE);
22661 r = dev_io(op, dev, FS_PROC_NR, bp->b_data, pos, block_size, 0);
22662 if (r != block_size) {
22663 if (r >= 0) r = END_OF_FILE;
22664 if (r != END_OF_FILE)
22665 printf("Unrecoverable disk error on device %d/%d, block %ld\n",
22666 (dev>>MAJOR)&BYTE, (dev>>MINOR)&BYTE, bp->b_blocknr);
22667 bp->b_dev = NO_DEV; /* invalidate block */
22668
22669 /* Report read errors to interested parties. */
22670 if (rw_flag == READING) rdwt_err = r;
22671 }
22672 }
22673
22674 bp->b_dirt = CLEAN;
22675 }
22677 /*===========================================================================*
22678 * invalidate *
22679 *===========================================================================*/
22680 PUBLIC void invalidate(device)
22681 dev_t device; /* device whose blocks are to be purged */
22682 {
22683 /* Remove all the blocks belonging to some device from the cache. */
22684
22685 register struct buf *bp;
22686
22687 for (bp = &buf[0]; bp < &buf[NR_BUFS]; bp++)
22688 if (bp->b_dev == device) bp->b_dev = NO_DEV;
22689 }
22691 /*===========================================================================*
22692 * flushall *
22693 *===========================================================================*/
22694 PUBLIC void flushall(dev)
22695 dev_t dev; /* device to flush */
22696 {
22697 /* Flush all dirty blocks for one device. */
22698
22699 register struct buf *bp;
22700 static struct buf *dirty[NR_BUFS]; /* static so it isn’t on stack */
22701 int ndirty;
22702
22703 for (bp = &buf[0], ndirty = 0; bp < &buf[NR_BUFS]; bp++)
22704 if (bp->b_dirt == DIRTY && bp->b_dev == dev) dirty[ndirty++] = bp;
22705 rw_scattered(dev, dirty, ndirty, WRITING);
22706 }
22708 /*===========================================================================*
22709 * rw_scattered *
22710 *===========================================================================*/
22711 PUBLIC void rw_scattered(dev, bufq, bufqsize, rw_flag)
22712 dev_t dev; /* major-minor device number */
22713 struct buf **bufq; /* pointer to array of buffers */
22714 int bufqsize; /* number of buffers */
22715 int rw_flag; /* READING or WRITING */
22716 {
22717 /* Read or write scattered data from a device. */
22718
22719 register struct buf *bp;
MINIX SOURCE CODE File: servers/fs/cache.c 939
22720 int gap;
22721 register int i;
22722 register iovec_t *iop;
22723 static iovec_t iovec[NR_IOREQS]; /* static so it isn’t on stack */
22724 int j, r;
22725 int block_size;
22726
22727 block_size = get_block_size(dev);
22728
22729 /* (Shell) sort buffers on b_blocknr. */
22730 gap = 1;
22731 do
22732 gap = 3 * gap + 1;
22733 while (gap <= bufqsize);
22734 while (gap != 1) {
22735 gap /= 3;
22736 for (j = gap; j < bufqsize; j++) {
22737 for (i = j - gap;
22738 i >= 0 && bufq[i]->b_blocknr > bufq[i + gap]->b_blocknr;
22739 i -= gap) {
22740 bp = bufq[i];
22741 bufq[i] = bufq[i + gap];
22742 bufq[i + gap] = bp;
22743 }
22744 }
22745 }
22746
22747 /* Set up I/O vector and do I/O. The result of dev_io is OK if everything
22748 * went fine, otherwise the error code for the first failed transfer.
22749 */
22750 while (bufqsize > 0) {
22751 for (j = 0, iop = iovec; j < NR_IOREQS && j < bufqsize; j++, iop++) {
22752 bp = bufq[j];
22753 if (bp->b_blocknr != bufq[0]->b_blocknr + j) break;
22754 iop->iov_addr = (vir_bytes) bp->b_data;
22755 iop->iov_size = block_size;
22756 }
22757 r = dev_io(rw_flag == WRITING ? DEV_SCATTER : DEV_GATHER,
22758 dev, FS_PROC_NR, iovec,
22759 (off_t) bufq[0]->b_blocknr * block_size, j, 0);
22760
22761 /* Harvest the results. Dev_io reports the first error it may have
22762 * encountered, but we only care if it’s the first block that failed.
22763 */
22764 for (i = 0, iop = iovec; i < j; i++, iop++) {
22765 bp = bufq[i];
22766 if (iop->iov_size != 0) {
22767 /* Transfer failed. An error? Do we care? */
22768 if (r != OK && i == 0) {
22769 printf(
22770 "fs: I/O error on device %d/%d, block %lu\n",
22771 (dev>>MAJOR)&BYTE, (dev>>MINOR)&BYTE,
22772 bp->b_blocknr);
22773 bp->b_dev = NO_DEV; /* invalidate block */
22774 }
22775 break;
22776 }
22777 if (rw_flag == READING) {
22778 bp->b_dev = dev; /* validate block */
22779 put_block(bp, PARTIAL_DATA_BLOCK);
940 File: servers/fs/cache.c MINIX SOURCE CODE
22780 } else {
22781 bp->b_dirt = CLEAN;
22782 }
22783 }
22784 bufq += i;
22785 bufqsize -= i;
22786 if (rw_flag == READING) {
22787 /* Don’t bother reading more than the device is willing to
22788 * give at this time. Don’t forget to release those extras.
22789 */
22790 while (bufqsize > 0) {
22791 put_block(*bufq++, PARTIAL_DATA_BLOCK);
22792 bufqsize--;
22793 }
22794 }
22795 if (rw_flag == WRITING && i == 0) {
22796 /* We’re not making progress, this means we might keep
22797 * looping. Buffers remain dirty if un-written. Buffers are
22798 * lost if invalidate()d or LRU-removed while dirty. This
22799 * is better than keeping unwritable blocks around forever..
22800 */
22801 break;
22802 }
22803 }
22804 }
22806 /*===========================================================================*
22807 * rm_lru *
22808 *===========================================================================*/
22809 PRIVATE void rm_lru(bp)
22810 struct buf *bp;
22811 {
22812 /* Remove a block from its LRU chain. */
22813 struct buf *next_ptr, *prev_ptr;
22814
22815 bufs_in_use++;
22816 next_ptr = bp->b_next; /* successor on LRU chain */
22817 prev_ptr = bp->b_prev; /* predecessor on LRU chain */
22818 if (prev_ptr != NIL_BUF)
22819 prev_ptr->b_next = next_ptr;
22820 else
22821 front = next_ptr; /* this block was at front of chain */
22822
22823 if (next_ptr != NIL_BUF)
22824 next_ptr->b_prev = prev_ptr;
22825 else
22826 rear = prev_ptr; /* this block was at rear of chain */
22827 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/inode.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
22900 /* This file manages the inode table. There are procedures to allocate and
22901 * deallocate inodes, acquire, erase, and release them, and read and write
22902 * them from the disk.
22903 *
22904 * The entry points into this file are
MINIX SOURCE CODE File: servers/fs/inode.c 941
22905 * get_inode: search inode table for a given inode; if not there,
22906 * read it
22907 * put_inode: indicate that an inode is no longer needed in memory
22908 * alloc_inode: allocate a new, unused inode
22909 * wipe_inode: erase some fields of a newly allocated inode
22910 * free_inode: mark an inode as available for a new file
22911 * update_times: update atime, ctime, and mtime
22912 * rw_inode: read a disk block and extract an inode, or corresp. write
22913 * old_icopy: copy to/from in-core inode struct and disk inode (V1.x)
22914 * new_icopy: copy to/from in-core inode struct and disk inode (V2.x)
22915 * dup_inode: indicate that someone else is using an inode table entry
22916 */
22917
22918 #include "fs.h"
22919 #include "buf.h"
22920 #include "file.h"
22921 #include "fproc.h"
22922 #include "inode.h"
22923 #include "super.h"
22924
22925 FORWARD _PROTOTYPE( void old_icopy, (struct inode *rip, d1_inode *dip,
22926 int direction, int norm));
22927 FORWARD _PROTOTYPE( void new_icopy, (struct inode *rip, d2_inode *dip,
22928 int direction, int norm));
22929
22930 /*===========================================================================*
22931 * get_inode *
22932 *===========================================================================*/
22933 PUBLIC struct inode *get_inode(dev, numb)
22934 dev_t dev; /* device on which inode resides */
22935 int numb; /* inode number (ANSI: may not be unshort) */
22936 {
22937 /* Find a slot in the inode table, load the specified inode into it, and
22938 * return a pointer to the slot. If ’dev’ == NO_DEV, just return a free slot.
22939 */
22940
22941 register struct inode *rip, *xp;
22942
22943 /* Search the inode table both for (dev, numb) and a free slot. */
22944 xp = NIL_INODE;
22945 for (rip = &inode[0]; rip < &inode[NR_INODES]; rip++) {
22946 if (rip->i_count > 0) { /* only check used slots for (dev, numb) */
22947 if (rip->i_dev == dev && rip->i_num == numb) {
22948 /* This is the inode that we are looking for. */
22949 rip->i_count++;
22950 return(rip); /* (dev, numb) found */
22951 }
22952 } else {
22953 xp = rip; /* remember this free slot for later */
22954 }
22955 }
22956
22957 /* Inode we want is not currently in use. Did we find a free slot? */
22958 if (xp == NIL_INODE) { /* inode table completely full */
22959 err_code = ENFILE;
22960 return(NIL_INODE);
22961 }
22962
22963 /* A free inode slot has been located. Load the inode into it. */
22964 xp->i_dev = dev;
942 File: servers/fs/inode.c MINIX SOURCE CODE
22965 xp->i_num = numb;
22966 xp->i_count = 1;
22967 if (dev != NO_DEV) rw_inode(xp, READING); /* get inode from disk */
22968 xp->i_update = 0; /* all the times are initially up-to-date */
22969
22970 return(xp);
22971 }
22973 /*===========================================================================*
22974 * put_inode *
22975 *===========================================================================*/
22976 PUBLIC void put_inode(rip)
22977 register struct inode *rip; /* pointer to inode to be released */
22978 {
22979 /* The caller is no longer using this inode. If no one else is using it either
22980 * write it back to the disk immediately. If it has no links, truncate it and
22981 * return it to the pool of available inodes.
22982 */
22983
22984 if (rip == NIL_INODE) return; /* checking here is easier than in caller */
22985 if (--rip->i_count == 0) { /* i_count == 0 means no one is using it now */
22986 if (rip->i_nlinks == 0) {
22987 /* i_nlinks == 0 means free the inode. */
22988 truncate(rip); /* return all the disk blocks */
22989 rip->i_mode = I_NOT_ALLOC; /* clear I_TYPE field */
22990 rip->i_dirt = DIRTY;
22991 free_inode(rip->i_dev, rip->i_num);
22992 } else {
22993 if (rip->i_pipe == I_PIPE) truncate(rip);
22994 }
22995 rip->i_pipe = NO_PIPE; /* should always be cleared */
22996 if (rip->i_dirt == DIRTY) rw_inode(rip, WRITING);
22997 }
22998 }
23000 /*===========================================================================*
23001 * alloc_inode *
23002 *===========================================================================*/
23003 PUBLIC struct inode *alloc_inode(dev_t dev, mode_t bits)
23004 {
23005 /* Allocate a free inode on ’dev’, and return a pointer to it. */
23006
23007 register struct inode *rip;
23008 register struct super_block *sp;
23009 int major, minor, inumb;
23010 bit_t b;
23011
23012 sp = get_super(dev); /* get pointer to super_block */
23013 if (sp->s_rd_only) { /* can’t allocate an inode on a read only device. */
23014 err_code = EROFS;
23015 return(NIL_INODE);
23016 }
23017
23018 /* Acquire an inode from the bit map. */
23019 b = alloc_bit(sp, IMAP, sp->s_isearch);
23020 if (b == NO_BIT) {
23021 err_code = ENFILE;
23022 major = (int) (sp->s_dev >> MAJOR) & BYTE;
23023 minor = (int) (sp->s_dev >> MINOR) & BYTE;
23024 printf("Out of i-nodes on %sdevice %d/%d\n",
MINIX SOURCE CODE File: servers/fs/inode.c 943
23025 sp->s_dev == root_dev ? "root " : "", major, minor);
23026 return(NIL_INODE);
23027 }
23028 sp->s_isearch = b; /* next time start here */
23029 inumb = (int) b; /* be careful not to pass unshort as param */
23030
23031 /* Try to acquire a slot in the inode table. */
23032 if ((rip = get_inode(NO_DEV, inumb)) == NIL_INODE) {
23033 /* No inode table slots available. Free the inode just allocated. */
23034 free_bit(sp, IMAP, b);
23035 } else {
23036 /* An inode slot is available. Put the inode just allocated into it. */
23037 rip->i_mode = bits; /* set up RWX bits */
23038 rip->i_nlinks = 0; /* initial no links */
23039 rip->i_uid = fp->fp_effuid; /* file’s uid is owner’s */
23040 rip->i_gid = fp->fp_effgid; /* ditto group id */
23041 rip->i_dev = dev; /* mark which device it is on */
23042 rip->i_ndzones = sp->s_ndzones; /* number of direct zones */
23043 rip->i_nindirs = sp->s_nindirs; /* number of indirect zones per blk*/
23044 rip->i_sp = sp; /* pointer to super block */
23045
23046 /* Fields not cleared already are cleared in wipe_inode(). They have
23047 * been put there because truncate() needs to clear the same fields if
23048 * the file happens to be open while being truncated. It saves space
23049 * not to repeat the code twice.
23050 */
23051 wipe_inode(rip);
23052 }
23053
23054 return(rip);
23055 }
23057 /*===========================================================================*
23058 * wipe_inode *
23059 *===========================================================================*/
23060 PUBLIC void wipe_inode(rip)
23061 register struct inode *rip; /* the inode to be erased */
23062 {
23063 /* Erase some fields in the inode. This function is called from alloc_inode()
23064 * when a new inode is to be allocated, and from truncate(), when an existing
23065 * inode is to be truncated.
23066 */
23067
23068 register int i;
23069
23070 rip->i_size = 0;
23071 rip->i_update = ATIME | CTIME | MTIME; /* update all times later */
23072 rip->i_dirt = DIRTY;
23073 for (i = 0; i < V2_NR_TZONES; i++) rip->i_zone[i] = NO_ZONE;
23074 }
23076 /*===========================================================================*
23077 * free_inode *
23078 *===========================================================================*/
23079 PUBLIC void free_inode(dev, inumb)
23080 dev_t dev; /* on which device is the inode */
23081 ino_t inumb; /* number of inode to be freed */
23082 {
23083 /* Return an inode to the pool of unallocated inodes. */
23084
944 File: servers/fs/inode.c MINIX SOURCE CODE
23085 register struct super_block *sp;
23086 bit_t b;
23087
23088 /* Locate the appropriate super_block. */
23089 sp = get_super(dev);
23090 if (inumb <= 0 || inumb > sp->s_ninodes) return;
23091 b = inumb;
23092 free_bit(sp, IMAP, b);
23093 if (b < sp->s_isearch) sp->s_isearch = b;
23094 }
23096 /*===========================================================================*
23097 * update_times *
23098 *===========================================================================*/
23099 PUBLIC void update_times(rip)
23100 register struct inode *rip; /* pointer to inode to be read/written */
23101 {
23102 /* Various system calls are required by the standard to update atime, ctime,
23103 * or mtime. Since updating a time requires sending a message to the clock
23104 * task--an expensive business--the times are marked for update by setting
23105 * bits in i_update. When a stat, fstat, or sync is done, or an inode is
23106 * released, update_times() may be called to actually fill in the times.
23107 */
23108
23109 time_t cur_time;
23110 struct super_block *sp;
23111
23112 sp = rip->i_sp; /* get pointer to super block. */
23113 if (sp->s_rd_only) return; /* no updates for read-only file systems */
23114
23115 cur_time = clock_time();
23116 if (rip->i_update & ATIME) rip->i_atime = cur_time;
23117 if (rip->i_update & CTIME) rip->i_ctime = cur_time;
23118 if (rip->i_update & MTIME) rip->i_mtime = cur_time;
23119 rip->i_update = 0; /* they are all up-to-date now */
23120 }
23122 /*===========================================================================*
23123 * rw_inode *
23124 *===========================================================================*/
23125 PUBLIC void rw_inode(rip, rw_flag)
23126 register struct inode *rip; /* pointer to inode to be read/written */
23127 int rw_flag; /* READING or WRITING */
23128 {
23129 /* An entry in the inode table is to be copied to or from the disk. */
23130
23131 register struct buf *bp;
23132 register struct super_block *sp;
23133 d1_inode *dip;
23134 d2_inode *dip2;
23135 block_t b, offset;
23136
23137 /* Get the block where the inode resides. */
23138 sp = get_super(rip->i_dev); /* get pointer to super block */
23139 rip->i_sp = sp; /* inode must contain super block pointer */
23140 offset = sp->s_imap_blocks + sp->s_zmap_blocks + 2;
23141 b = (block_t) (rip->i_num - 1)/sp->s_inodes_per_block + offset;
23142 bp = get_block(rip->i_dev, b, NORMAL);
23143 dip = bp->b_v1_ino + (rip->i_num - 1) % V1_INODES_PER_BLOCK;
23144 dip2 = bp->b_v2_ino + (rip->i_num - 1) %
MINIX SOURCE CODE File: servers/fs/inode.c 945
23145 V2_INODES_PER_BLOCK(sp->s_block_size);
23146
23147 /* Do the read or write. */
23148 if (rw_flag == WRITING) {
23149 if (rip->i_update) update_times(rip); /* times need updating */
23150 if (sp->s_rd_only == FALSE) bp->b_dirt = DIRTY;
23151 }
23152
23153 /* Copy the inode from the disk block to the in-core table or vice versa.
23154 * If the fourth parameter below is FALSE, the bytes are swapped.
23155 */
23156 if (sp->s_version == V1)
23157 old_icopy(rip, dip, rw_flag, sp->s_native);
23158 else
23159 new_icopy(rip, dip2, rw_flag, sp->s_native);
23160
23161 put_block(bp, INODE_BLOCK);
23162 rip->i_dirt = CLEAN;
23163 }
23165 /*===========================================================================*
23166 * old_icopy *
23167 *===========================================================================*/
23168 PRIVATE void old_icopy(rip, dip, direction, norm)
23169 register struct inode *rip; /* pointer to the in-core inode struct */
23170 register d1_inode *dip; /* pointer to the d1_inode inode struct */
23171 int direction; /* READING (from disk) or WRITING (to disk) */
23172 int norm; /* TRUE = do not swap bytes; FALSE = swap */
23173
23174 {
23175 /* The V1.x IBM disk, the V1.x 68000 disk, and the V2 disk (same for IBM and
23176 * 68000) all have different inode layouts. When an inode is read or written
23177 * this routine handles the conversions so that the information in the inode
23178 * table is independent of the disk structure from which the inode came.
23179 * The old_icopy routine copies to and from V1 disks.
23180 */
23181
23182 int i;
23183
23184 if (direction == READING) {
23185 /* Copy V1.x inode to the in-core table, swapping bytes if need be. */
23186 rip->i_mode = conv2(norm, (int) dip->d1_mode);
23187 rip->i_uid = conv2(norm, (int) dip->d1_uid );
23188 rip->i_size = conv4(norm, dip->d1_size);
23189 rip->i_mtime = conv4(norm, dip->d1_mtime);
23190 rip->i_atime = rip->i_mtime;
23191 rip->i_ctime = rip->i_mtime;
23192 rip->i_nlinks = dip->d1_nlinks; /* 1 char */
23193 rip->i_gid = dip->d1_gid; /* 1 char */
23194 rip->i_ndzones = V1_NR_DZONES;
23195 rip->i_nindirs = V1_INDIRECTS;
23196 for (i = 0; i < V1_NR_TZONES; i++)
23197 rip->i_zone[i] = conv2(norm, (int) dip->d1_zone[i]);
23198 } else {
23199 /* Copying V1.x inode to disk from the in-core table. */
23200 dip->d1_mode = conv2(norm, (int) rip->i_mode);
23201 dip->d1_uid = conv2(norm, (int) rip->i_uid );
23202 dip->d1_size = conv4(norm, rip->i_size);
23203 dip->d1_mtime = conv4(norm, rip->i_mtime);
23204 dip->d1_nlinks = rip->i_nlinks; /* 1 char */
946 File: servers/fs/inode.c MINIX SOURCE CODE
23205 dip->d1_gid = rip->i_gid; /* 1 char */
23206 for (i = 0; i < V1_NR_TZONES; i++)
23207 dip->d1_zone[i] = conv2(norm, (int) rip->i_zone[i]);
23208 }
23209 }
23211 /*===========================================================================*
23212 * new_icopy *
23213 *===========================================================================*/
23214 PRIVATE void new_icopy(rip, dip, direction, norm)
23215 register struct inode *rip; /* pointer to the in-core inode struct */
23216 register d2_inode *dip; /* pointer to the d2_inode struct */
23217 int direction; /* READING (from disk) or WRITING (to disk) */
23218 int norm; /* TRUE = do not swap bytes; FALSE = swap */
23219
23220 {
23221 /* Same as old_icopy, but to/from V2 disk layout. */
23222
23223 int i;
23224
23225 if (direction == READING) {
23226 /* Copy V2.x inode to the in-core table, swapping bytes if need be. */
23227 rip->i_mode = conv2(norm,dip->d2_mode);
23228 rip->i_uid = conv2(norm,dip->d2_uid);
23229 rip->i_nlinks = conv2(norm,dip->d2_nlinks);
23230 rip->i_gid = conv2(norm,dip->d2_gid);
23231 rip->i_size = conv4(norm,dip->d2_size);
23232 rip->i_atime = conv4(norm,dip->d2_atime);
23233 rip->i_ctime = conv4(norm,dip->d2_ctime);
23234 rip->i_mtime = conv4(norm,dip->d2_mtime);
23235 rip->i_ndzones = V2_NR_DZONES;
23236 rip->i_nindirs = V2_INDIRECTS(rip->i_sp->s_block_size);
23237 for (i = 0; i < V2_NR_TZONES; i++)
23238 rip->i_zone[i] = conv4(norm, (long) dip->d2_zone[i]);
23239 } else {
23240 /* Copying V2.x inode to disk from the in-core table. */
23241 dip->d2_mode = conv2(norm,rip->i_mode);
23242 dip->d2_uid = conv2(norm,rip->i_uid);
23243 dip->d2_nlinks = conv2(norm,rip->i_nlinks);
23244 dip->d2_gid = conv2(norm,rip->i_gid);
23245 dip->d2_size = conv4(norm,rip->i_size);
23246 dip->d2_atime = conv4(norm,rip->i_atime);
23247 dip->d2_ctime = conv4(norm,rip->i_ctime);
23248 dip->d2_mtime = conv4(norm,rip->i_mtime);
23249 for (i = 0; i < V2_NR_TZONES; i++)
23250 dip->d2_zone[i] = conv4(norm, (long) rip->i_zone[i]);
23251 }
23252 }
23254 /*===========================================================================*
23255 * dup_inode *
23256 *===========================================================================*/
23257 PUBLIC void dup_inode(ip)
23258 struct inode *ip; /* The inode to be duplicated. */
23259 {
23260 /* This routine is a simplified form of get_inode() for the case where
23261 * the inode pointer is already known.
23262 */
23263
23264 ip->i_count++;
MINIX SOURCE CODE File: servers/fs/inode.c 947
23265 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/super.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
23300 /* This file manages the super block table and the related data structures,
23301 * namely, the bit maps that keep track of which zones and which inodes are
23302 * allocated and which are free. When a new inode or zone is needed, the
23303 * appropriate bit map is searched for a free entry.
23304 *
23305 * The entry points into this file are
23306 * alloc_bit: somebody wants to allocate a zone or inode; find one
23307 * free_bit: indicate that a zone or inode is available for allocation
23308 * get_super: search the ’superblock’ table for a device
23309 * mounted: tells if file inode is on mounted (or ROOT) file system
23310 * read_super: read a superblock
23311 */
23312
23313 #include "fs.h"
23314 #include <string.h>
23315 #include <minix/com.h>
23316 #include "buf.h"
23317 #include "inode.h"
23318 #include "super.h"
23319 #include "const.h"
23320
23321 /*===========================================================================*
23322 * alloc_bit *
23323 *===========================================================================*/
23324 PUBLIC bit_t alloc_bit(sp, map, origin)
23325 struct super_block *sp; /* the filesystem to allocate from */
23326 int map; /* IMAP (inode map) or ZMAP (zone map) */
23327 bit_t origin; /* number of bit to start searching at */
23328 {
23329 /* Allocate a bit from a bit map and return its bit number. */
23330
23331 block_t start_block; /* first bit block */
23332 bit_t map_bits; /* how many bits are there in the bit map? */
23333 unsigned bit_blocks; /* how many blocks are there in the bit map? */
23334 unsigned block, word, bcount;
23335 struct buf *bp;
23336 bitchunk_t *wptr, *wlim, k;
23337 bit_t i, b;
23338
23339 if (sp->s_rd_only)
23340 panic(__FILE__,"can’t allocate bit on read-only filesys.", NO_NUM);
23341
23342 if (map == IMAP) {
23343 start_block = START_BLOCK;
23344 map_bits = sp->s_ninodes + 1;
23345 bit_blocks = sp->s_imap_blocks;
23346 } else {
23347 start_block = START_BLOCK + sp->s_imap_blocks;
23348 map_bits = sp->s_zones - (sp->s_firstdatazone - 1);
23349 bit_blocks = sp->s_zmap_blocks;
948 File: servers/fs/super.c MINIX SOURCE CODE
23350 }
23351
23352 /* Figure out where to start the bit search (depends on ’origin’). */
23353 if (origin >= map_bits) origin = 0; /* for robustness */
23354
23355 /* Locate the starting place. */
23356 block = origin / FS_BITS_PER_BLOCK(sp->s_block_size);
23357 word = (origin % FS_BITS_PER_BLOCK(sp->s_block_size)) / FS_BITCHUNK_BITS;
23358
23359 /* Iterate over all blocks plus one, because we start in the middle. */
23360 bcount = bit_blocks + 1;
23361 do {
23362 bp = get_block(sp->s_dev, start_block + block, NORMAL);
23363 wlim = &bp->b_bitmap[FS_BITMAP_CHUNKS(sp->s_block_size)];
23364
23365 /* Iterate over the words in block. */
23366 for (wptr = &bp->b_bitmap[word]; wptr < wlim; wptr++) {
23367
23368 /* Does this word contain a free bit? */
23369 if (*wptr == (bitchunk_t) ˜0) continue;
23370
23371 /* Find and allocate the free bit. */
23372 k = conv2(sp->s_native, (int) *wptr);
23373 for (i = 0; (k & (1 << i)) != 0; ++i) {}
23374
23375 /* Bit number from the start of the bit map. */
23376 b = ((bit_t) block * FS_BITS_PER_BLOCK(sp->s_block_size))
23377 + (wptr - &bp->b_bitmap[0]) * FS_BITCHUNK_BITS
23378 + i;
23379
23380 /* Don’t allocate bits beyond the end of the map. */
23381 if (b >= map_bits) break;
23382
23383 /* Allocate and return bit number. */
23384 k |= 1 << i;
23385 *wptr = conv2(sp->s_native, (int) k);
23386 bp->b_dirt = DIRTY;
23387 put_block(bp, MAP_BLOCK);
23388 return(b);
23389 }
23390 put_block(bp, MAP_BLOCK);
23391 if (++block >= bit_blocks) block = 0; /* last block, wrap around */
23392 word = 0;
23393 } while (--bcount > 0);
23394 return(NO_BIT); /* no bit could be allocated */
23395 }
23397 /*===========================================================================*
23398 * free_bit *
23399 *===========================================================================*/
23400 PUBLIC void free_bit(sp, map, bit_returned)
23401 struct super_block *sp; /* the filesystem to operate on */
23402 int map; /* IMAP (inode map) or ZMAP (zone map) */
23403 bit_t bit_returned; /* number of bit to insert into the map */
23404 {
23405 /* Return a zone or inode by turning off its bitmap bit. */
23406
23407 unsigned block, word, bit;
23408 struct buf *bp;
23409 bitchunk_t k, mask;
MINIX SOURCE CODE File: servers/fs/super.c 949
23410 block_t start_block;
23411
23412 if (sp->s_rd_only)
23413 panic(__FILE__,"can’t free bit on read-only filesys.", NO_NUM);
23414
23415 if (map == IMAP) {
23416 start_block = START_BLOCK;
23417 } else {
23418 start_block = START_BLOCK + sp->s_imap_blocks;
23419 }
23420 block = bit_returned / FS_BITS_PER_BLOCK(sp->s_block_size);
23421 word = (bit_returned % FS_BITS_PER_BLOCK(sp->s_block_size))
23422 / FS_BITCHUNK_BITS;
23423
23424 bit = bit_returned % FS_BITCHUNK_BITS;
23425 mask = 1 << bit;
23426
23427 bp = get_block(sp->s_dev, start_block + block, NORMAL);
23428
23429 k = conv2(sp->s_native, (int) bp->b_bitmap[word]);
23430 if (!(k & mask)) {
23431 panic(__FILE__,map == IMAP ? "tried to free unused inode" :
23432 "tried to free unused block", NO_NUM);
23433 }
23434
23435 k &= ˜mask;
23436 bp->b_bitmap[word] = conv2(sp->s_native, (int) k);
23437 bp->b_dirt = DIRTY;
23438
23439 put_block(bp, MAP_BLOCK);
23440 }
23442 /*===========================================================================*
23443 * get_super *
23444 *===========================================================================*/
23445 PUBLIC struct super_block *get_super(dev)
23446 dev_t dev; /* device number whose super_block is sought */
23447 {
23448 /* Search the superblock table for this device. It is supposed to be there. */
23449
23450 register struct super_block *sp;
23451
23452 if (dev == NO_DEV)
23453 panic(__FILE__,"request for super_block of NO_DEV", NO_NUM);
23454
23455 for (sp = &super_block[0]; sp < &super_block[NR_SUPERS]; sp++)
23456 if (sp->s_dev == dev) return(sp);
23457
23458 /* Search failed. Something wrong. */
23459 panic(__FILE__,"can’t find superblock for device (in decimal)", (int) dev);
23460
23461 return(NIL_SUPER); /* to keep the compiler and lint quiet */
23462 }
23464 /*===========================================================================*
23465 * get_block_size *
23466 *===========================================================================*/
23467 PUBLIC int get_block_size(dev_t dev)
23468 {
23469 /* Search the superblock table for this device. */
950 File: servers/fs/super.c MINIX SOURCE CODE
23470
23471 register struct super_block *sp;
23472
23473 if (dev == NO_DEV)
23474 panic(__FILE__,"request for block size of NO_DEV", NO_NUM);
23475
23476 for (sp = &super_block[0]; sp < &super_block[NR_SUPERS]; sp++) {
23477 if (sp->s_dev == dev) {
23478 return(sp->s_block_size);
23479 }
23480 }
23481
23482 /* no mounted filesystem? use this block size then. */
23483 return MIN_BLOCK_SIZE;
23484 }
23486 /*===========================================================================*
23487 * mounted *
23488 *===========================================================================*/
23489 PUBLIC int mounted(rip)
23490 register struct inode *rip; /* pointer to inode */
23491 {
23492 /* Report on whether the given inode is on a mounted (or ROOT) file system. */
23493
23494 register struct super_block *sp;
23495 register dev_t dev;
23496
23497 dev = (dev_t) rip->i_zone[0];
23498 if (dev == root_dev) return(TRUE); /* inode is on root file system */
23499
23500 for (sp = &super_block[0]; sp < &super_block[NR_SUPERS]; sp++)
23501 if (sp->s_dev == dev) return(TRUE);
23502
23503 return(FALSE);
23504 }
23506 /*===========================================================================*
23507 * read_super *
23508 *===========================================================================*/
23509 PUBLIC int read_super(sp)
23510 register struct super_block *sp; /* pointer to a superblock */
23511 {
23512 /* Read a superblock. */
23513 dev_t dev;
23514 int magic;
23515 int version, native, r;
23516 static char sbbuf[MIN_BLOCK_SIZE];
23517
23518 dev = sp->s_dev; /* save device (will be overwritten by copy) */
23519 if (dev == NO_DEV)
23520 panic(__FILE__,"request for super_block of NO_DEV", NO_NUM);
23521 r = dev_io(DEV_READ, dev, FS_PROC_NR,
23522 sbbuf, SUPER_BLOCK_BYTES, MIN_BLOCK_SIZE, 0);
23523 if (r != MIN_BLOCK_SIZE) {
23524 return EINVAL;
23525 }
23526 memcpy(sp, sbbuf, sizeof(*sp));
23527 sp->s_dev = NO_DEV; /* restore later */
23528 magic = sp->s_magic; /* determines file system type */
23529
MINIX SOURCE CODE File: servers/fs/super.c 951
23530 /* Get file system version and type. */
23531 if (magic == SUPER_MAGIC || magic == conv2(BYTE_SWAP, SUPER_MAGIC)) {
23532 version = V1;
23533 native = (magic == SUPER_MAGIC);
23534 } else if (magic == SUPER_V2 || magic == conv2(BYTE_SWAP, SUPER_V2)) {
23535 version = V2;
23536 native = (magic == SUPER_V2);
23537 } else if (magic == SUPER_V3) {
23538 version = V3;
23539 native = 1;
23540 } else {
23541 return(EINVAL);
23542 }
23543
23544 /* If the super block has the wrong byte order, swap the fields; the magic
23545 * number doesn’t need conversion. */
23546 sp->s_ninodes = conv4(native, sp->s_ninodes);
23547 sp->s_nzones = conv2(native, (int) sp->s_nzones);
23548 sp->s_imap_blocks = conv2(native, (int) sp->s_imap_blocks);
23549 sp->s_zmap_blocks = conv2(native, (int) sp->s_zmap_blocks);
23550 sp->s_firstdatazone = conv2(native, (int) sp->s_firstdatazone);
23551 sp->s_log_zone_size = conv2(native, (int) sp->s_log_zone_size);
23552 sp->s_max_size = conv4(native, sp->s_max_size);
23553 sp->s_zones = conv4(native, sp->s_zones);
23554
23555 /* In V1, the device size was kept in a short, s_nzones, which limited
23556 * devices to 32K zones. For V2, it was decided to keep the size as a
23557 * long. However, just changing s_nzones to a long would not work, since
23558 * then the position of s_magic in the super block would not be the same
23559 * in V1 and V2 file systems, and there would be no way to tell whether
23560 * a newly mounted file system was V1 or V2. The solution was to introduce
23561 * a new variable, s_zones, and copy the size there.
23562 *
23563 * Calculate some other numbers that depend on the version here too, to
23564 * hide some of the differences.
23565 */
23566 if (version == V1) {
23567 sp->s_block_size = STATIC_BLOCK_SIZE;
23568 sp->s_zones = sp->s_nzones; /* only V1 needs this copy */
23569 sp->s_inodes_per_block = V1_INODES_PER_BLOCK;
23570 sp->s_ndzones = V1_NR_DZONES;
23571 sp->s_nindirs = V1_INDIRECTS;
23572 } else {
23573 if (version == V2)
23574 sp->s_block_size = STATIC_BLOCK_SIZE;
23575 if (sp->s_block_size < MIN_BLOCK_SIZE)
23576 return EINVAL;
23577 sp->s_inodes_per_block = V2_INODES_PER_BLOCK(sp->s_block_size);
23578 sp->s_ndzones = V2_NR_DZONES;
23579 sp->s_nindirs = V2_INDIRECTS(sp->s_block_size);
23580 }
23581
23582 if (sp->s_block_size < MIN_BLOCK_SIZE) {
23583 return EINVAL;
23584 }
23585 if (sp->s_block_size > MAX_BLOCK_SIZE) {
23586 printf("Filesystem block size is %d kB; maximum filesystem\n"
23587 "block size is %d kB. This limit can be increased by recompiling.\n",
23588 sp->s_block_size/1024, MAX_BLOCK_SIZE/1024);
23589 return EINVAL;
952 File: servers/fs/super.c MINIX SOURCE CODE
23590 }
23591 if ((sp->s_block_size % 512) != 0) {
23592 return EINVAL;
23593 }
23594 if (SUPER_SIZE > sp->s_block_size) {
23595 return EINVAL;
23596 }
23597 if ((sp->s_block_size % V2_INODE_SIZE) != 0 ||
23598 (sp->s_block_size % V1_INODE_SIZE) != 0) {
23599 return EINVAL;
23600 }
23601
23602 sp->s_isearch = 0; /* inode searches initially start at 0 */
23603 sp->s_zsearch = 0; /* zone searches initially start at 0 */
23604 sp->s_version = version;
23605 sp->s_native = native;
23606
23607 /* Make a few basic checks to see if super block looks reasonable. */
23608 if (sp->s_imap_blocks < 1 || sp->s_zmap_blocks < 1
23609 || sp->s_ninodes < 1 || sp->s_zones < 1
23610 || (unsigned) sp->s_log_zone_size > 4) {
23611 printf("not enough imap or zone map blocks, \n");
23612 printf("or not enough inodes, or not enough zones, "
23613 "or zone size too large\n");
23614 return(EINVAL);
23615 }
23616 sp->s_dev = dev; /* restore device number */
23617 return(OK);
23618 }
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
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/lock.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
23800 /* This file handles advisory file locking as required by POSIX.
23801 *
23802 * The entry points into this file are
23803 * lock_op: perform locking operations for FCNTL system call
23804 * lock_revive: revive processes when a lock is released
23805 */
23806
23807 #include "fs.h"
23808 #include <minix/com.h>
23809 #include <fcntl.h>
23810 #include <unistd.h>
23811 #include "file.h"
23812 #include "fproc.h"
23813 #include "inode.h"
23814 #include "lock.h"
23815 #include "param.h"
23816
23817 /*===========================================================================*
23818 * lock_op *
23819 *===========================================================================*/
23820 PUBLIC int lock_op(f, req)
23821 struct filp *f;
23822 int req; /* either F_SETLK or F_SETLKW */
23823 {
23824 /* Perform the advisory locking required by POSIX. */
23825
23826 int r, ltype, i, conflict = 0, unlocking = 0;
23827 mode_t mo;
23828 off_t first, last;
23829 struct flock flock;
23830 vir_bytes user_flock;
23831 struct file_lock *flp, *flp2, *empty;
23832
23833 /* Fetch the flock structure from user space. */
23834 user_flock = (vir_bytes) m_in.name1;
23835 r = sys_datacopy(who, (vir_bytes) user_flock,
23836 FS_PROC_NR, (vir_bytes) &flock, (phys_bytes) sizeof(flock));
23837 if (r != OK) return(EINVAL);
23838
23839 /* Make some error checks. */
23840 ltype = flock.l_type;
23841 mo = f->filp_mode;
23842 if (ltype != F_UNLCK && ltype != F_RDLCK && ltype != F_WRLCK) return(EINVAL);
23843 if (req == F_GETLK && ltype == F_UNLCK) return(EINVAL);
23844 if ( (f->filp_ino->i_mode & I_TYPE) != I_REGULAR) return(EINVAL);
MINIX SOURCE CODE File: servers/fs/lock.c 955
23845 if (req != F_GETLK && ltype == F_RDLCK && (mo & R_BIT) == 0) return(EBADF);
23846 if (req != F_GETLK && ltype == F_WRLCK && (mo & W_BIT) == 0) return(EBADF);
23847
23848 /* Compute the first and last bytes in the lock region. */
23849 switch (flock.l_whence) {
23850 case SEEK_SET: first = 0; break;
23851 case SEEK_CUR: first = f->filp_pos; break;
23852 case SEEK_END: first = f->filp_ino->i_size; break;
23853 default: return(EINVAL);
23854 }
23855 /* Check for overflow. */
23856 if (((long)flock.l_start > 0) && ((first + flock.l_start) < first))
23857 return(EINVAL);
23858 if (((long)flock.l_start < 0) && ((first + flock.l_start) > first))
23859 return(EINVAL);
23860 first = first + flock.l_start;
23861 last = first + flock.l_len - 1;
23862 if (flock.l_len == 0) last = MAX_FILE_POS;
23863 if (last < first) return(EINVAL);
23864
23865 /* Check if this region conflicts with any existing lock. */
23866 empty = (struct file_lock *) 0;
23867 for (flp = &file_lock[0]; flp < & file_lock[NR_LOCKS]; flp++) {
23868 if (flp->lock_type == 0) {
23869 if (empty == (struct file_lock *) 0) empty = flp;
23870 continue; /* 0 means unused slot */
23871 }
23872 if (flp->lock_inode != f->filp_ino) continue; /* different file */
23873 if (last < flp->lock_first) continue; /* new one is in front */
23874 if (first > flp->lock_last) continue; /* new one is afterwards */
23875 if (ltype == F_RDLCK && flp->lock_type == F_RDLCK) continue;
23876 if (ltype != F_UNLCK && flp->lock_pid == fp->fp_pid) continue;
23877
23878 /* There might be a conflict. Process it. */
23879 conflict = 1;
23880 if (req == F_GETLK) break;
23881
23882 /* If we are trying to set a lock, it just failed. */
23883 if (ltype == F_RDLCK || ltype == F_WRLCK) {
23884 if (req == F_SETLK) {
23885 /* For F_SETLK, just report back failure. */
23886 return(EAGAIN);
23887 } else {
23888 /* For F_SETLKW, suspend the process. */
23889 suspend(XLOCK);
23890 return(SUSPEND);
23891 }
23892 }
23893
23894 /* We are clearing a lock and we found something that overlaps. */
23895 unlocking = 1;
23896 if (first <= flp->lock_first && last >= flp->lock_last) {
23897 flp->lock_type = 0; /* mark slot as unused */
23898 nr_locks--; /* number of locks is now 1 less */
23899 continue;
23900 }
23901
23902 /* Part of a locked region has been unlocked. */
23903 if (first <= flp->lock_first) {
23904 flp->lock_first = last + 1;
956 File: servers/fs/lock.c MINIX SOURCE CODE
23905 continue;
23906 }
23907
23908 if (last >= flp->lock_last) {
23909 flp->lock_last = first - 1;
23910 continue;
23911 }
23912
23913 /* Bad luck. A lock has been split in two by unlocking the middle. */
23914 if (nr_locks == NR_LOCKS) return(ENOLCK);
23915 for (i = 0; i < NR_LOCKS; i++)
23916 if (file_lock[i].lock_type == 0) break;
23917 flp2 = &file_lock[i];
23918 flp2->lock_type = flp->lock_type;
23919 flp2->lock_pid = flp->lock_pid;
23920 flp2->lock_inode = flp->lock_inode;
23921 flp2->lock_first = last + 1;
23922 flp2->lock_last = flp->lock_last;
23923 flp->lock_last = first - 1;
23924 nr_locks++;
23925 }
23926 if (unlocking) lock_revive();
23927
23928 if (req == F_GETLK) {
23929 if (conflict) {
23930 /* GETLK and conflict. Report on the conflicting lock. */
23931 flock.l_type = flp->lock_type;
23932 flock.l_whence = SEEK_SET;
23933 flock.l_start = flp->lock_first;
23934 flock.l_len = flp->lock_last - flp->lock_first + 1;
23935 flock.l_pid = flp->lock_pid;
23936
23937 } else {
23938 /* It is GETLK and there is no conflict. */
23939 flock.l_type = F_UNLCK;
23940 }
23941
23942 /* Copy the flock structure back to the caller. */
23943 r = sys_datacopy(FS_PROC_NR, (vir_bytes) &flock,
23944 who, (vir_bytes) user_flock, (phys_bytes) sizeof(flock));
23945 return(r);
23946 }
23947
23948 if (ltype == F_UNLCK) return(OK); /* unlocked a region with no locks */
23949
23950 /* There is no conflict. If space exists, store new lock in the table. */
23951 if (empty == (struct file_lock *) 0) return(ENOLCK); /* table full */
23952 empty->lock_type = ltype;
23953 empty->lock_pid = fp->fp_pid;
23954 empty->lock_inode = f->filp_ino;
23955 empty->lock_first = first;
23956 empty->lock_last = last;
23957 nr_locks++;
23958 return(OK);
23959 }
MINIX SOURCE CODE File: servers/fs/lock.c 957
23961 /*===========================================================================*
23962 * lock_revive *
23963 *===========================================================================*/
23964 PUBLIC void lock_revive()
23965 {
23966 /* Go find all the processes that are waiting for any kind of lock and
23967 * revive them all. The ones that are still blocked will block again when
23968 * they run. The others will complete. This strategy is a space-time
23969 * tradeoff. Figuring out exactly which ones to unblock now would take
23970 * extra code, and the only thing it would win would be some performance in
23971 * extremely rare circumstances (namely, that somebody actually used
23972 * locking).
23973 */
23974
23975 int task;
23976 struct fproc *fptr;
23977
23978 for (fptr = &fproc[INIT_PROC_NR + 1]; fptr < &fproc[NR_PROCS]; fptr++){
23979 task = -fptr->fp_task;
23980 if (fptr->fp_suspended == SUSPENDED && task == XLOCK) {
23981 revive( (int) (fptr - fproc), 0);
23982 }
23983 }
23984 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
24000 /* This file contains the main program of the File System. It consists of
24001 * a loop that gets messages requesting work, carries out the work, and sends
24002 * replies.
24003 *
24004 * The entry points into this file are:
24005 * main: main program of the File System
24006 * reply: send a reply to a process after the requested work is done
24007 *
24008 */
24009
24010 struct super_block; /* proto.h needs to know this */
24011
24012 #include "fs.h"
24013 #include <fcntl.h>
24014 #include <string.h>
24015 #include <stdio.h>
24016 #include <signal.h>
24017 #include <stdlib.h>
24018 #include <sys/ioc_memory.h>
24019 #include <sys/svrctl.h>
24020 #include <minix/callnr.h>
24021 #include <minix/com.h>
24022 #include <minix/keymap.h>
24023 #include <minix/const.h>
24024 #include "buf.h"
24025 #include "file.h"
24026 #include "fproc.h"
24027 #include "inode.h"
24028 #include "param.h"
24029 #include "super.h"
958 File: servers/fs/main.c MINIX SOURCE CODE
24030
24031 FORWARD _PROTOTYPE( void fs_init, (void) );
24032 FORWARD _PROTOTYPE( int igetenv, (char *var, int optional) );
24033 FORWARD _PROTOTYPE( void get_work, (void) );
24034 FORWARD _PROTOTYPE( void load_ram, (void) );
24035 FORWARD _PROTOTYPE( void load_super, (Dev_t super_dev) );
24036
24037 /*===========================================================================*
24038 * main *
24039 *===========================================================================*/
24040 PUBLIC int main()
24041 {
24042 /* This is the main program of the file system. The main loop consists of
24043 * three major activities: getting new work, processing the work, and sending
24044 * the reply. This loop never terminates as long as the file system runs.
24045 */
24046 sigset_t sigset;
24047 int error;
24048
24049 fs_init();
24050
24051 /* This is the main loop that gets work, processes it, and sends replies. */
24052 while (TRUE) {
24053 get_work(); /* sets who and call_nr */
24054
24055 fp = &fproc[who]; /* pointer to proc table struct */
24056 super_user = (fp->fp_effuid == SU_UID ? TRUE : FALSE); /* su? */
24057
24058 /* Check for special control messages first. */
24059 if (call_nr == SYS_SIG) {
24060 sigset = m_in.NOTIFY_ARG;
24061 if (sigismember(&sigset, SIGKSTOP)) {
24062 do_sync();
24063 sys_exit(0); /* never returns */
24064 }
24065 } else if (call_nr == SYN_ALARM) {
24066 /* Not a user request; system has expired one of our timers,
24067 * currently only in use for select(). Check it.
24068 */
24069 fs_expire_timers(m_in.NOTIFY_TIMESTAMP);
24070 } else if ((call_nr & NOTIFY_MESSAGE)) {
24071 /* Device notifies us of an event. */
24072 dev_status(&m_in);
24073 } else {
24074 /* Call the internal function that does the work. */
24075 if (call_nr < 0 || call_nr >= NCALLS) {
24076 error = ENOSYS;
24077 printf("FS,warning illegal %d system call by %d\n",call_nr,who);
24078 } else if (fp->fp_pid == PID_FREE) {
24079 error = ENOSYS;
24080 printf("FS, bad process, who = %d, call_nr = %d, slot1 = %d\n",
24081 who, call_nr, m_in.slot1);
24082 } else {
24083 error = (*call_vec[call_nr])();
24084 }
24085
24086 /* Copy the results back to the user and send reply. */
24087 if (error != SUSPEND) { reply(who, error); }
24088 if (rdahed_inode != NIL_INODE) {
24089 read_ahead(); /* do block read ahead */
MINIX SOURCE CODE File: servers/fs/main.c 959
24090 }
24091 }
24092 }
24093 return(OK); /* shouldn’t come here */
24094 }
24096 /*===========================================================================*
24097 * get_work *
24098 *===========================================================================*/
24099 PRIVATE void get_work()
24100 {
24101 /* Normally wait for new input. However, if ’reviving’ is
24102 * nonzero, a suspended process must be awakened.
24103 */
24104 register struct fproc *rp;
24105
24106 if (reviving != 0) {
24107 /* Revive a suspended process. */
24108 for (rp = &fproc[0]; rp < &fproc[NR_PROCS]; rp++)
24109 if (rp->fp_revived == REVIVING) {
24110 who = (int)(rp - fproc);
24111 call_nr = rp->fp_fd & BYTE;
24112 m_in.fd = (rp->fp_fd >>8) & BYTE;
24113 m_in.buffer = rp->fp_buffer;
24114 m_in.nbytes = rp->fp_nbytes;
24115 rp->fp_suspended = NOT_SUSPENDED; /*no longer hanging*/
24116 rp->fp_revived = NOT_REVIVING;
24117 reviving--;
24118 return;
24119 }
24120 panic(__FILE__,"get_work couldn’t revive anyone", NO_NUM);
24121 }
24122
24123 /* Normal case. No one to revive. */
24124 if (receive(ANY, &m_in) != OK) panic(__FILE__,"fs receive error", NO_NUM);
24125 who = m_in.m_source;
24126 call_nr = m_in.m_type;
24127 }
24129 /*===========================================================================*
24130 * buf_pool *
24131 *===========================================================================*/
24132 PRIVATE void buf_pool(void)
24133 {
24134 /* Initialize the buffer pool. */
24135
24136 register struct buf *bp;
24137
24138 bufs_in_use = 0;
24139 front = &buf[0];
24140 rear = &buf[NR_BUFS - 1];
24141
24142 for (bp = &buf[0]; bp < &buf[NR_BUFS]; bp++) {
24143 bp->b_blocknr = NO_BLOCK;
24144 bp->b_dev = NO_DEV;
24145 bp->b_next = bp + 1;
24146 bp->b_prev = bp - 1;
24147 }
24148 buf[0].b_prev = NIL_BUF;
24149 buf[NR_BUFS - 1].b_next = NIL_BUF;
960 File: servers/fs/main.c MINIX SOURCE CODE
24150
24151 for (bp = &buf[0]; bp < &buf[NR_BUFS]; bp++) bp->b_hash = bp->b_next;
24152 buf_hash[0] = front;
24153
24154 }
24156 /*===========================================================================*
24157 * reply *
24158 *===========================================================================*/
24159 PUBLIC void reply(whom, result)
24160 int whom; /* process to reply to */
24161 int result; /* result of the call (usually OK or error #) */
24162 {
24163 /* Send a reply to a user process. It may fail (if the process has just
24164 * been killed by a signal), so don’t check the return code. If the send
24165 * fails, just ignore it.
24166 */
24167 int s;
24168 m_out.reply_type = result;
24169 s = send(whom, &m_out);
24170 if (s != OK) printf("FS: couldn’t send reply %d: %d\n", result, s);
24171 }
24173 /*===========================================================================*
24174 * fs_init *
24175 *===========================================================================*/
24176 PRIVATE void fs_init()
24177 {
24178 /* Initialize global variables, tables, etc. */
24179 register struct inode *rip;
24180 register struct fproc *rfp;
24181 message mess;
24182 int s;
24183
24184 /* Initialize the process table with help of the process manager messages.
24185 * Expect one message for each system process with its slot number and pid.
24186 * When no more processes follow, the magic process number NONE is sent.
24187 * Then, stop and synchronize with the PM.
24188 */
24189 do {
24190 if (OK != (s=receive(PM_PROC_NR, &mess)))
24191 panic(__FILE__,"FS couldn’t receive from PM", s);
24192 if (NONE == mess.PR_PROC_NR) break;
24193
24194 rfp = &fproc[mess.PR_PROC_NR];
24195 rfp->fp_pid = mess.PR_PID;
24196 rfp->fp_realuid = (uid_t) SYS_UID;
24197 rfp->fp_effuid = (uid_t) SYS_UID;
24198 rfp->fp_realgid = (gid_t) SYS_GID;
24199 rfp->fp_effgid = (gid_t) SYS_GID;
24200 rfp->fp_umask = ˜0;
24201
24202 } while (TRUE); /* continue until process NONE */
24203 mess.m_type = OK; /* tell PM that we succeeded */
24204 s=send(PM_PROC_NR, &mess); /* send synchronization message */
24205
24206 /* All process table entries have been set. Continue with FS initialization.
24207 * Certain relations must hold for the file system to work at all. Some
24208 * extra block_size requirements are checked at super-block-read-in time.
24209 */
MINIX SOURCE CODE File: servers/fs/main.c 961
24210 if (OPEN_MAX > 127) panic(__FILE__,"OPEN_MAX > 127", NO_NUM);
24211 if (NR_BUFS < 6) panic(__FILE__,"NR_BUFS < 6", NO_NUM);
24212 if (V1_INODE_SIZE != 32) panic(__FILE__,"V1 inode size != 32", NO_NUM);
24213 if (V2_INODE_SIZE != 64) panic(__FILE__,"V2 inode size != 64", NO_NUM);
24214 if (OPEN_MAX > 8 * sizeof(long))
24215 panic(__FILE__,"Too few bits in fp_cloexec", NO_NUM);
24216
24217 /* The following initializations are needed to let dev_opcl succeed .*/
24218 fp = (struct fproc *) NULL;
24219 who = FS_PROC_NR;
24220
24221 buf_pool(); /* initialize buffer pool */
24222 build_dmap(); /* build device table and map boot driver */
24223 load_ram(); /* init RAM disk, load if it is root */
24224 load_super(root_dev); /* load super block for root device */
24225 init_select(); /* init select() structures */
24226
24227 /* The root device can now be accessed; set process directories. */
24228 for (rfp=&fproc[0]; rfp < &fproc[NR_PROCS]; rfp++) {
24229 if (rfp->fp_pid != PID_FREE) {
24230 rip = get_inode(root_dev, ROOT_INODE);
24231 dup_inode(rip);
24232 rfp->fp_rootdir = rip;
24233 rfp->fp_workdir = rip;
24234 }
24235 }
24236 }
24238 /*===========================================================================*
24239 * igetenv *
24240 *===========================================================================*/
24241 PRIVATE int igetenv(key, optional)
24242 char *key;
24243 int optional;
24244 {
24245 /* Ask kernel for an integer valued boot environment variable. */
24246 char value[64];
24247 int i;
24248
24249 if ((i = env_get_param(key, value, sizeof(value))) != OK) {
24250 if (!optional)
24251 printf("FS: Warning, couldn’t get monitor param: %d\n", i);
24252 return 0;
24253 }
24254 return(atoi(value));
24255 }
24257 /*===========================================================================*
24258 * load_ram *
24259 *===========================================================================*/
24260 PRIVATE void load_ram(void)
24261 {
24262 /* Allocate a RAM disk with size given in the boot parameters. If a RAM disk
24263 * image is given, the copy the entire image device block-by-block to a RAM
24264 * disk with the same size as the image.
24265 * If the root device is not set, the RAM disk will be used as root instead.
24266 */
24267 register struct buf *bp, *bp1;
24268 u32_t lcount, ram_size_kb;
24269 zone_t zones;
962 File: servers/fs/main.c MINIX SOURCE CODE
24270 struct super_block *sp, *dsp;
24271 block_t b;
24272 Dev_t image_dev;
24273 static char sbbuf[MIN_BLOCK_SIZE];
24274 int block_size_image, block_size_ram, ramfs_block_size;
24275 int s;
24276
24277 /* Get some boot environment variables. */
24278 root_dev = igetenv("rootdev", 0);
24279 image_dev = igetenv("ramimagedev", 0);
24280 ram_size_kb = igetenv("ramsize", 0);
24281
24282 /* Open the root device. */
24283 if (dev_open(root_dev, FS_PROC_NR, R_BIT|W_BIT) != OK)
24284 panic(__FILE__,"Cannot open root device",NO_NUM);
24285
24286 /* If we must initialize a ram disk, get details from the image device. */
24287 if (root_dev == DEV_RAM) {
24288 u32_t fsmax, probedev;
24289
24290 /* If we are running from CD, see if we can find it. */
24291 if (igetenv("cdproberoot", 1) && (probedev=cdprobe()) != NO_DEV) {
24292 char devnum[10];
24293 struct sysgetenv env;
24294
24295 /* If so, this is our new RAM image device. */
24296 image_dev = probedev;
24297
24298 /* Tell PM about it, so userland can find out about it
24299 * with sysenv interface.
24300 */
24301 env.key = "cdproberoot";
24302 env.keylen = strlen(env.key);
24303 sprintf(devnum, "%d", (int) probedev);
24304 env.val = devnum;
24305 env.vallen = strlen(devnum);
24306 svrctl(MMSETPARAM, &env);
24307 }
24308
24309 /* Open image device for RAM root. */
24310 if (dev_open(image_dev, FS_PROC_NR, R_BIT) != OK)
24311 panic(__FILE__,"Cannot open RAM image device", NO_NUM);
24312
24313 /* Get size of RAM disk image from the super block. */
24314 sp = &super_block[0];
24315 sp->s_dev = image_dev;
24316 if (read_super(sp) != OK)
24317 panic(__FILE__,"Bad RAM disk image FS", NO_NUM);
24318
24319 lcount = sp->s_zones << sp->s_log_zone_size; /* # blks on root dev*/
24320
24321 /* Stretch the RAM disk file system to the boot parameters size, but
24322 * no further than the last zone bit map block allows.
24323 */
24324 if (ram_size_kb*1024 < lcount*sp->s_block_size)
24325 ram_size_kb = lcount*sp->s_block_size/1024;
24326 fsmax = (u32_t) sp->s_zmap_blocks * CHAR_BIT * sp->s_block_size;
24327 fsmax = (fsmax + (sp->s_firstdatazone-1)) << sp->s_log_zone_size;
24328 if (ram_size_kb*1024 > fsmax*sp->s_block_size)
24329 ram_size_kb = fsmax*sp->s_block_size/1024;
MINIX SOURCE CODE File: servers/fs/main.c 963
24330 }
24331
24332 /* Tell RAM driver how big the RAM disk must be. */
24333 m_out.m_type = DEV_IOCTL;
24334 m_out.PROC_NR = FS_PROC_NR;
24335 m_out.DEVICE = RAM_DEV;
24336 m_out.REQUEST = MIOCRAMSIZE; /* I/O control to use */
24337 m_out.POSITION = (ram_size_kb * 1024); /* request in bytes */
24338 if ((s=sendrec(MEM_PROC_NR, &m_out)) != OK)
24339 panic("FS","sendrec from MEM failed", s);
24340 else if (m_out.REP_STATUS != OK) {
24341 /* Report and continue, unless RAM disk is required as root FS. */
24342 if (root_dev != DEV_RAM) {
24343 report("FS","can’t set RAM disk size", m_out.REP_STATUS);
24344 return;
24345 } else {
24346 panic(__FILE__,"can’t set RAM disk size", m_out.REP_STATUS);
24347 }
24348 }
24349
24350 /* See if we must load the RAM disk image, otherwise return. */
24351 if (root_dev != DEV_RAM)
24352 return;
24353
24354 /* Copy the blocks one at a time from the image to the RAM disk. */
24355 printf("Loading RAM disk onto /dev/ram:\33[23CLoaded: 0 KB");
24356
24357 inode[0].i_mode = I_BLOCK_SPECIAL; /* temp inode for rahead() */
24358 inode[0].i_size = LONG_MAX;
24359 inode[0].i_dev = image_dev;
24360 inode[0].i_zone[0] = image_dev;
24361
24362 block_size_ram = get_block_size(DEV_RAM);
24363 block_size_image = get_block_size(image_dev);
24364
24365 /* RAM block size has to be a multiple of the root image block
24366 * size to make copying easier.
24367 */
24368 if (block_size_image % block_size_ram) {
24369 printf("\nram block size: %d image block size: %d\n",
24370 block_size_ram, block_size_image);
24371 panic(__FILE__, "ram disk block size must be a multiple of "
24372 "the image disk block size", NO_NUM);
24373 }
24374
24375 /* Loading blocks from image device. */
24376 for (b = 0; b < (block_t) lcount; b++) {
24377 int rb, factor;
24378 bp = rahead(&inode[0], b, (off_t)block_size_image * b, block_size_image);
24379 factor = block_size_image/block_size_ram;
24380 for(rb = 0; rb < factor; rb++) {
24381 bp1 = get_block(root_dev, b * factor + rb, NO_READ);
24382 memcpy(bp1->b_data, bp->b_data + rb * block_size_ram,
24383 (size_t) block_size_ram);
24384 bp1->b_dirt = DIRTY;
24385 put_block(bp1, FULL_DATA_BLOCK);
24386 }
24387 put_block(bp, FULL_DATA_BLOCK);
24388 if (b % 11 == 0)
24389 printf("\b\b\b\b\b\b\b\b\b%6ld KB", ((long) b * block_size_image)/1024L);
964 File: servers/fs/main.c MINIX SOURCE CODE
24390 }
24391
24392 /* Commit changes to RAM so dev_io will see it. */
24393 do_sync();
24394
24395 printf("\rRAM disk of %u KB loaded onto /dev/ram.", (unsigned) ram_size_kb);
24396 if (root_dev == DEV_RAM) printf(" Using RAM disk as root FS.");
24397 printf(" \n");
24398
24399 /* Invalidate and close the image device. */
24400 invalidate(image_dev);
24401 dev_close(image_dev);
24402
24403 /* Resize the RAM disk root file system. */
24404 if (dev_io(DEV_READ, root_dev, FS_PROC_NR,
24405 sbbuf, SUPER_BLOCK_BYTES, MIN_BLOCK_SIZE, 0) != MIN_BLOCK_SIZE) {
24406 printf("WARNING: ramdisk read for resizing failed\n");
24407 }
24408 dsp = (struct super_block *) sbbuf;
24409 if (dsp->s_magic == SUPER_V3)
24410 ramfs_block_size = dsp->s_block_size;
24411 else
24412 ramfs_block_size = STATIC_BLOCK_SIZE;
24413 zones = (ram_size_kb * 1024 / ramfs_block_size) >> sp->s_log_zone_size;
24414
24415 dsp->s_nzones = conv2(sp->s_native, (u16_t) zones);
24416 dsp->s_zones = conv4(sp->s_native, zones);
24417 if (dev_io(DEV_WRITE, root_dev, FS_PROC_NR,
24418 sbbuf, SUPER_BLOCK_BYTES, MIN_BLOCK_SIZE, 0) != MIN_BLOCK_SIZE) {
24419 printf("WARNING: ramdisk write for resizing failed\n");
24420 }
24421 }
24423 /*===========================================================================*
24424 * load_super *
24425 *===========================================================================*/
24426 PRIVATE void load_super(super_dev)
24427 dev_t super_dev; /* place to get superblock from */
24428 {
24429 int bad;
24430 register struct super_block *sp;
24431 register struct inode *rip;
24432
24433 /* Initialize the super_block table. */
24434 for (sp = &super_block[0]; sp < &super_block[NR_SUPERS]; sp++)
24435 sp->s_dev = NO_DEV;
24436
24437 /* Read in super_block for the root file system. */
24438 sp = &super_block[0];
24439 sp->s_dev = super_dev;
24440
24441 /* Check super_block for consistency. */
24442 bad = (read_super(sp) != OK);
24443 if (!bad) {
24444 rip = get_inode(super_dev, ROOT_INODE); /* inode for root dir */
24445 if ( (rip->i_mode & I_TYPE) != I_DIRECTORY || rip->i_nlinks < 3) bad++;
24446 }
24447 if (bad) panic(__FILE__,"Invalid root file system", NO_NUM);
24448
24449 sp->s_imount = rip;
MINIX SOURCE CODE File: servers/fs/main.c 965
24450 dup_inode(rip);
24451 sp->s_isup = rip;
24452 sp->s_rd_only = 0;
24453 return;
24454 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/open.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
24500 /* This file contains the procedures for creating, opening, closing, and
24501 * seeking on files.
24502 *
24503 * The entry points into this file are
24504 * do_creat: perform the CREAT system call
24505 * do_open: perform the OPEN system call
24506 * do_mknod: perform the MKNOD system call
24507 * do_mkdir: perform the MKDIR system call
24508 * do_close: perform the CLOSE system call
24509 * do_lseek: perform the LSEEK system call
24510 */
24511
24512 #include "fs.h"
24513 #include <sys/stat.h>
24514 #include <fcntl.h>
24515 #include <minix/callnr.h>
24516 #include <minix/com.h>
24517 #include "buf.h"
24518 #include "file.h"
24519 #include "fproc.h"
24520 #include "inode.h"
24521 #include "lock.h"
24522 #include "param.h"
24523 #include "super.h"
24524
24525 #define offset m2_l1
24526
24527 PRIVATE char mode_map[] = {R_BIT, W_BIT, R_BIT|W_BIT, 0};
24528
24529 FORWARD _PROTOTYPE( int common_open, (int oflags, mode_t omode) );
24530 FORWARD _PROTOTYPE( int pipe_open, (struct inode *rip,mode_t bits,int oflags));
24531 FORWARD _PROTOTYPE( struct inode *new_node, (char *path, mode_t bits,
24532 zone_t z0) );
24533
24534 /*===========================================================================*
24535 * do_creat *
24536 *===========================================================================*/
24537 PUBLIC int do_creat()
24538 {
24539 /* Perform the creat(name, mode) system call. */
24540 int r;
24541
24542 if (fetch_name(m_in.name, m_in.name_length, M3) != OK) return(err_code);
24543 r = common_open(O_WRONLY | O_CREAT | O_TRUNC, (mode_t) m_in.mode);
24544 return(r);
24545 }
966 File: servers/fs/open.c MINIX SOURCE CODE
24547 /*===========================================================================*
24548 * do_open *
24549 *===========================================================================*/
24550 PUBLIC int do_open()
24551 {
24552 /* Perform the open(name, flags,...) system call. */
24553
24554 int create_mode = 0; /* is really mode_t but this gives problems */
24555 int r;
24556
24557 /* If O_CREAT is set, open has three parameters, otherwise two. */
24558 if (m_in.mode & O_CREAT) {
24559 create_mode = m_in.c_mode;
24560 r = fetch_name(m_in.c_name, m_in.name1_length, M1);
24561 } else {
24562 r = fetch_name(m_in.name, m_in.name_length, M3);
24563 }
24564
24565 if (r != OK) return(err_code); /* name was bad */
24566 r = common_open(m_in.mode, create_mode);
24567 return(r);
24568 }
24570 /*===========================================================================*
24571 * common_open *
24572 *===========================================================================*/
24573 PRIVATE int common_open(register int oflags, mode_t omode)
24574 {
24575 /* Common code from do_creat and do_open. */
24576
24577 register struct inode *rip;
24578 int r, b, exist = TRUE;
24579 dev_t dev;
24580 mode_t bits;
24581 off_t pos;
24582 struct filp *fil_ptr, *filp2;
24583
24584 /* Remap the bottom two bits of oflags. */
24585 bits = (mode_t) mode_map[oflags & O_ACCMODE];
24586
24587 /* See if file descriptor and filp slots are available. */
24588 if ( (r = get_fd(0, bits, &m_in.fd, &fil_ptr)) != OK) return(r);
24589
24590 /* If O_CREATE is set, try to make the file. */
24591 if (oflags & O_CREAT) {
24592 /* Create a new inode by calling new_node(). */
24593 omode = I_REGULAR | (omode & ALL_MODES & fp->fp_umask);
24594 rip = new_node(user_path, omode, NO_ZONE);
24595 r = err_code;
24596 if (r == OK) exist = FALSE; /* we just created the file */
24597 else if (r != EEXIST) return(r); /* other error */
24598 else exist = !(oflags & O_EXCL); /* file exists, if the O_EXCL
24599 flag is set this is an error */
24600 } else {
24601 /* Scan path name. */
24602 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
24603 }
24604
24605 /* Claim the file descriptor and filp slot and fill them in. */
24606 fp->fp_filp[m_in.fd] = fil_ptr;
MINIX SOURCE CODE File: servers/fs/open.c 967
24607 fil_ptr->filp_count = 1;
24608 fil_ptr->filp_ino = rip;
24609 fil_ptr->filp_flags = oflags;
24610
24611 /* Only do the normal open code if we didn’t just create the file. */
24612 if (exist) {
24613 /* Check protections. */
24614 if ((r = forbidden(rip, bits)) == OK) {
24615 /* Opening reg. files directories and special files differ. */
24616 switch (rip->i_mode & I_TYPE) {
24617 case I_REGULAR:
24618 /* Truncate regular file if O_TRUNC. */
24619 if (oflags & O_TRUNC) {
24620 if ((r = forbidden(rip, W_BIT)) !=OK) break;
24621 truncate(rip);
24622 wipe_inode(rip);
24623 /* Send the inode from the inode cache to the
24624 * block cache, so it gets written on the next
24625 * cache flush.
24626 */
24627 rw_inode(rip, WRITING);
24628 }
24629 break;
24630
24631 case I_DIRECTORY:
24632 /* Directories may be read but not written. */
24633 r = (bits & W_BIT ? EISDIR : OK);
24634 break;
24635
24636 case I_CHAR_SPECIAL:
24637 case I_BLOCK_SPECIAL:
24638 /* Invoke the driver for special processing. */
24639 dev = (dev_t) rip->i_zone[0];
24640 r = dev_open(dev, who, bits | (oflags & ˜O_ACCMODE));
24641 break;
24642
24643 case I_NAMED_PIPE:
24644 oflags |= O_APPEND; /* force append mode */
24645 fil_ptr->filp_flags = oflags;
24646 r = pipe_open(rip, bits, oflags);
24647 if (r != ENXIO) {
24648 /* See if someone else is doing a rd or wt on
24649 * the FIFO. If so, use its filp entry so the
24650 * file position will be automatically shared.
24651 */
24652 b = (bits & R_BIT ? R_BIT : W_BIT);
24653 fil_ptr->filp_count = 0; /* don’t find self */
24654 if ((filp2 = find_filp(rip, b)) != NIL_FILP) {
24655 /* Co-reader or writer found. Use it.*/
24656 fp->fp_filp[m_in.fd] = filp2;
24657 filp2->filp_count++;
24658 filp2->filp_ino = rip;
24659 filp2->filp_flags = oflags;
24660
24661 /* i_count was incremented incorrectly
24662 * by eatpath above, not knowing that
24663 * we were going to use an existing
24664 * filp entry. Correct this error.
24665 */
24666 rip->i_count--;
968 File: servers/fs/open.c MINIX SOURCE CODE
24667 } else {
24668 /* Nobody else found. Restore filp. */
24669 fil_ptr->filp_count = 1;
24670 if (b == R_BIT)
24671 pos = rip->i_zone[V2_NR_DZONES+0];
24672 else
24673 pos = rip->i_zone[V2_NR_DZONES+1];
24674 fil_ptr->filp_pos = pos;
24675 }
24676 }
24677 break;
24678 }
24679 }
24680 }
24681
24682 /* If error, release inode. */
24683 if (r != OK) {
24684 if (r == SUSPEND) return(r); /* Oops, just suspended */
24685 fp->fp_filp[m_in.fd] = NIL_FILP;
24686 fil_ptr->filp_count= 0;
24687 put_inode(rip);
24688 return(r);
24689 }
24690
24691 return(m_in.fd);
24692 }
24694 /*===========================================================================*
24695 * new_node *
24696 *===========================================================================*/
24697 PRIVATE struct inode *new_node(char *path, mode_t bits, zone_t z0)
24698 {
24699 /* New_node() is called by common_open(), do_mknod(), and do_mkdir().
24700 * In all cases it allocates a new inode, makes a directory entry for it on
24701 * the path ’path’, and initializes it. It returns a pointer to the inode if
24702 * it can do this; otherwise it returns NIL_INODE. It always sets ’err_code’
24703 * to an appropriate value (OK or an error code).
24704 */
24705
24706 register struct inode *rlast_dir_ptr, *rip;
24707 register int r;
24708 char string[NAME_MAX];
24709
24710 /* See if the path can be opened down to the last directory. */
24711 if ((rlast_dir_ptr = last_dir(path, string)) == NIL_INODE) return(NIL_INODE);
24712
24713 /* The final directory is accessible. Get final component of the path. */
24714 rip = advance(rlast_dir_ptr, string);
24715 if ( rip == NIL_INODE && err_code == ENOENT) {
24716 /* Last path component does not exist. Make new directory entry. */
24717 if ( (rip = alloc_inode(rlast_dir_ptr->i_dev, bits)) == NIL_INODE) {
24718 /* Can’t creat new inode: out of inodes. */
24719 put_inode(rlast_dir_ptr);
24720 return(NIL_INODE);
24721 }
24722
24723 /* Force inode to the disk before making directory entry to make
24724 * the system more robust in the face of a crash: an inode with
24725 * no directory entry is much better than the opposite.
24726 */
MINIX SOURCE CODE File: servers/fs/open.c 969
24727 rip->i_nlinks++;
24728 rip->i_zone[0] = z0; /* major/minor device numbers */
24729 rw_inode(rip, WRITING); /* force inode to disk now */
24730
24731 /* New inode acquired. Try to make directory entry. */
24732 if ((r = search_dir(rlast_dir_ptr, string, &rip->i_num,ENTER)) != OK) {
24733 put_inode(rlast_dir_ptr);
24734 rip->i_nlinks--; /* pity, have to free disk inode */
24735 rip->i_dirt = DIRTY; /* dirty inodes are written out */
24736 put_inode(rip); /* this call frees the inode */
24737 err_code = r;
24738 return(NIL_INODE);
24739 }
24740
24741 } else {
24742 /* Either last component exists, or there is some problem. */
24743 if (rip != NIL_INODE)
24744 r = EEXIST;
24745 else
24746 r = err_code;
24747 }
24748
24749 /* Return the directory inode and exit. */
24750 put_inode(rlast_dir_ptr);
24751 err_code = r;
24752 return(rip);
24753 }
24755 /*===========================================================================*
24756 * pipe_open *
24757 *===========================================================================*/
24758 PRIVATE int pipe_open(register struct inode *rip, register mode_t bits,
24759 register int oflags)
24760 {
24761 /* This function is called from common_open. It checks if
24762 * there is at least one reader/writer pair for the pipe, if not
24763 * it suspends the caller, otherwise it revives all other blocked
24764 * processes hanging on the pipe.
24765 */
24766
24767 rip->i_pipe = I_PIPE;
24768 if (find_filp(rip, bits & W_BIT ? R_BIT : W_BIT) == NIL_FILP) {
24769 if (oflags & O_NONBLOCK) {
24770 if (bits & W_BIT) return(ENXIO);
24771 } else {
24772 suspend(XPOPEN); /* suspend caller */
24773 return(SUSPEND);
24774 }
24775 } else if (susp_count > 0) {/* revive blocked processes */
24776 release(rip, OPEN, susp_count);
24777 release(rip, CREAT, susp_count);
24778 }
24779 return(OK);
24780 }
24782 /*===========================================================================*
24783 * do_mknod *
24784 *===========================================================================*/
24785 PUBLIC int do_mknod()
24786 {
970 File: servers/fs/open.c MINIX SOURCE CODE
24787 /* Perform the mknod(name, mode, addr) system call. */
24788
24789 register mode_t bits, mode_bits;
24790 struct inode *ip;
24791
24792 /* Only the super_user may make nodes other than fifos. */
24793 mode_bits = (mode_t) m_in.mk_mode; /* mode of the inode */
24794 if (!super_user && ((mode_bits & I_TYPE) != I_NAMED_PIPE)) return(EPERM);
24795 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
24796 bits = (mode_bits & I_TYPE) | (mode_bits & ALL_MODES & fp->fp_umask);
24797 ip = new_node(user_path, bits, (zone_t) m_in.mk_z0);
24798 put_inode(ip);
24799 return(err_code);
24800 }
24802 /*===========================================================================*
24803 * do_mkdir *
24804 *===========================================================================*/
24805 PUBLIC int do_mkdir()
24806 {
24807 /* Perform the mkdir(name, mode) system call. */
24808
24809 int r1, r2; /* status codes */
24810 ino_t dot, dotdot; /* inode numbers for . and .. */
24811 mode_t bits; /* mode bits for the new inode */
24812 char string[NAME_MAX]; /* last component of the new dir’s path name */
24813 register struct inode *rip, *ldirp;
24814
24815 /* Check to see if it is possible to make another link in the parent dir. */
24816 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
24817 ldirp = last_dir(user_path, string); /* pointer to new dir’s parent */
24818 if (ldirp == NIL_INODE) return(err_code);
24819 if (ldirp->i_nlinks >= (ldirp->i_sp->s_version == V1 ?
24820 CHAR_MAX : SHRT_MAX)) {
24821 put_inode(ldirp); /* return parent */
24822 return(EMLINK);
24823 }
24824
24825 /* Next make the inode. If that fails, return error code. */
24826 bits = I_DIRECTORY | (m_in.mode & RWX_MODES & fp->fp_umask);
24827 rip = new_node(user_path, bits, (zone_t) 0);
24828 if (rip == NIL_INODE || err_code == EEXIST) {
24829 put_inode(rip); /* can’t make dir: it already exists */
24830 put_inode(ldirp); /* return parent too */
24831 return(err_code);
24832 }
24833
24834 /* Get the inode numbers for . and .. to enter in the directory. */
24835 dotdot = ldirp->i_num; /* parent’s inode number */
24836 dot = rip->i_num; /* inode number of the new dir itself */
24837
24838 /* Now make dir entries for . and .. unless the disk is completely full. */
24839 /* Use dot1 and dot2, so the mode of the directory isn’t important. */
24840 rip->i_mode = bits; /* set mode */
24841 r1 = search_dir(rip, dot1, &dot, ENTER); /* enter . in the new dir */
24842 r2 = search_dir(rip, dot2, &dotdot, ENTER); /* enter .. in the new dir */
24843
24844 /* If both . and .. were successfully entered, increment the link counts. */
24845 if (r1 == OK && r2 == OK) {
24846 /* Normal case. It was possible to enter . and .. in the new dir. */
MINIX SOURCE CODE File: servers/fs/open.c 971
24847 rip->i_nlinks++; /* this accounts for . */
24848 ldirp->i_nlinks++; /* this accounts for .. */
24849 ldirp->i_dirt = DIRTY; /* mark parent’s inode as dirty */
24850 } else {
24851 /* It was not possible to enter . or .. probably disk was full. */
24852 (void) search_dir(ldirp, string, (ino_t *) 0, DELETE);
24853 rip->i_nlinks--; /* undo the increment done in new_node() */
24854 }
24855 rip->i_dirt = DIRTY; /* either way, i_nlinks has changed */
24856
24857 put_inode(ldirp); /* return the inode of the parent dir */
24858 put_inode(rip); /* return the inode of the newly made dir */
24859 return(err_code); /* new_node() always sets ’err_code’ */
24860 }
24862 /*===========================================================================*
24863 * do_close *
24864 *===========================================================================*/
24865 PUBLIC int do_close()
24866 {
24867 /* Perform the close(fd) system call. */
24868
24869 register struct filp *rfilp;
24870 register struct inode *rip;
24871 struct file_lock *flp;
24872 int rw, mode_word, lock_count;
24873 dev_t dev;
24874
24875 /* First locate the inode that belongs to the file descriptor. */
24876 if ( (rfilp = get_filp(m_in.fd)) == NIL_FILP) return(err_code);
24877 rip = rfilp->filp_ino; /* ’rip’ points to the inode */
24878
24879 if (rfilp->filp_count - 1 == 0 && rfilp->filp_mode != FILP_CLOSED) {
24880 /* Check to see if the file is special. */
24881 mode_word = rip->i_mode & I_TYPE;
24882 if (mode_word == I_CHAR_SPECIAL || mode_word == I_BLOCK_SPECIAL) {
24883 dev = (dev_t) rip->i_zone[0];
24884 if (mode_word == I_BLOCK_SPECIAL) {
24885 /* Invalidate cache entries unless special is mounted
24886 * or ROOT
24887 */
24888 if (!mounted(rip)) {
24889 (void) do_sync(); /* purge cache */
24890 invalidate(dev);
24891 }
24892 }
24893 /* Do any special processing on device close. */
24894 dev_close(dev);
24895 }
24896 }
24897
24898 /* If the inode being closed is a pipe, release everyone hanging on it. */
24899 if (rip->i_pipe == I_PIPE) {
24900 rw = (rfilp->filp_mode & R_BIT ? WRITE : READ);
24901 release(rip, rw, NR_PROCS);
24902 }
24903
24904 /* If a write has been done, the inode is already marked as DIRTY. */
24905 if (--rfilp->filp_count == 0) {
24906 if (rip->i_pipe == I_PIPE && rip->i_count > 1) {
972 File: servers/fs/open.c MINIX SOURCE CODE
24907 /* Save the file position in the i-node in case needed later.
24908 * The read and write positions are saved separately. The
24909 * last 3 zones in the i-node are not used for (named) pipes.
24910 */
24911 if (rfilp->filp_mode == R_BIT)
24912 rip->i_zone[V2_NR_DZONES+0] = (zone_t) rfilp->filp_pos;
24913 else
24914 rip->i_zone[V2_NR_DZONES+1] = (zone_t) rfilp->filp_pos;
24915 }
24916 put_inode(rip);
24917 }
24918
24919 fp->fp_cloexec &= ˜(1L << m_in.fd); /* turn off close-on-exec bit */
24920 fp->fp_filp[m_in.fd] = NIL_FILP;
24921
24922 /* Check to see if the file is locked. If so, release all locks. */
24923 if (nr_locks == 0) return(OK);
24924 lock_count = nr_locks; /* save count of locks */
24925 for (flp = &file_lock[0]; flp < &file_lock[NR_LOCKS]; flp++) {
24926 if (flp->lock_type == 0) continue; /* slot not in use */
24927 if (flp->lock_inode == rip && flp->lock_pid == fp->fp_pid) {
24928 flp->lock_type = 0;
24929 nr_locks--;
24930 }
24931 }
24932 if (nr_locks < lock_count) lock_revive(); /* lock released */
24933 return(OK);
24934 }
24936 /*===========================================================================*
24937 * do_lseek *
24938 *===========================================================================*/
24939 PUBLIC int do_lseek()
24940 {
24941 /* Perform the lseek(ls_fd, offset, whence) system call. */
24942
24943 register struct filp *rfilp;
24944 register off_t pos;
24945
24946 /* Check to see if the file descriptor is valid. */
24947 if ( (rfilp = get_filp(m_in.ls_fd)) == NIL_FILP) return(err_code);
24948
24949 /* No lseek on pipes. */
24950 if (rfilp->filp_ino->i_pipe == I_PIPE) return(ESPIPE);
24951
24952 /* The value of ’whence’ determines the start position to use. */
24953 switch(m_in.whence) {
24954 case 0: pos = 0; break;
24955 case 1: pos = rfilp->filp_pos; break;
24956 case 2: pos = rfilp->filp_ino->i_size; break;
24957 default: return(EINVAL);
24958 }
24959
24960 /* Check for overflow. */
24961 if (((long)m_in.offset > 0) && ((long)(pos + m_in.offset) < (long)pos))
24962 return(EINVAL);
24963 if (((long)m_in.offset < 0) && ((long)(pos + m_in.offset) > (long)pos))
24964 return(EINVAL);
24965 pos = pos + m_in.offset;
24966
MINIX SOURCE CODE File: servers/fs/open.c 973
24967 if (pos != rfilp->filp_pos)
24968 rfilp->filp_ino->i_seek = ISEEK; /* inhibit read ahead */
24969 rfilp->filp_pos = pos;
24970 m_out.reply_l1 = pos; /* insert the long into the output message */
24971 return(OK);
24972 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/read.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
25000 /* This file contains the heart of the mechanism used to read (and write)
25001 * files. Read and write requests are split up into chunks that do not cross
25002 * block boundaries. Each chunk is then processed in turn. Reads on special
25003 * files are also detected and handled.
25004 *
25005 * The entry points into this file are
25006 * do_read: perform the READ system call by calling read_write
25007 * read_write: actually do the work of READ and WRITE
25008 * read_map: given an inode and file position, look up its zone number
25009 * rd_indir: read an entry in an indirect block
25010 * read_ahead: manage the block read ahead business
25011 */
25012
25013 #include "fs.h"
25014 #include <fcntl.h>
25015 #include <minix/com.h>
25016 #include "buf.h"
25017 #include "file.h"
25018 #include "fproc.h"
25019 #include "inode.h"
25020 #include "param.h"
25021 #include "super.h"
25022
25023 FORWARD _PROTOTYPE( int rw_chunk, (struct inode *rip, off_t position,
25024 unsigned off, int chunk, unsigned left, int rw_flag,
25025 char *buff, int seg, int usr, int block_size, int *completed));
25026
25027 /*===========================================================================*
25028 * do_read *
25029 *===========================================================================*/
25030 PUBLIC int do_read()
25031 {
25032 return(read_write(READING));
25033 }
25035 /*===========================================================================*
25036 * read_write *
25037 *===========================================================================*/
25038 PUBLIC int read_write(rw_flag)
25039 int rw_flag; /* READING or WRITING */
25040 {
25041 /* Perform read(fd, buffer, nbytes) or write(fd, buffer, nbytes) call. */
25042
25043 register struct inode *rip;
25044 register struct filp *f;
974 File: servers/fs/read.c MINIX SOURCE CODE
25045 off_t bytes_left, f_size, position;
25046 unsigned int off, cum_io;
25047 int op, oflags, r, chunk, usr, seg, block_spec, char_spec;
25048 int regular, partial_pipe = 0, partial_cnt = 0;
25049 mode_t mode_word;
25050 struct filp *wf;
25051 int block_size;
25052 int completed, r2 = OK;
25053 phys_bytes p;
25054
25055 /* left unfinished rw_chunk()s from previous call! this can’t happen.
25056 * it means something has gone wrong we can’t repair now.
25057 */
25058 if (bufs_in_use < 0) {
25059 panic(__FILE__,"start - bufs_in_use negative", bufs_in_use);
25060 }
25061
25062 /* MM loads segments by putting funny things in upper 10 bits of ’fd’. */
25063 if (who == PM_PROC_NR && (m_in.fd & (˜BYTE)) ) {
25064 usr = m_in.fd >> 7;
25065 seg = (m_in.fd >> 5) & 03;
25066 m_in.fd &= 037; /* get rid of user and segment bits */
25067 } else {
25068 usr = who; /* normal case */
25069 seg = D;
25070 }
25071
25072 /* If the file descriptor is valid, get the inode, size and mode. */
25073 if (m_in.nbytes < 0) return(EINVAL);
25074 if ((f = get_filp(m_in.fd)) == NIL_FILP) return(err_code);
25075 if (((f->filp_mode) & (rw_flag == READING ? R_BIT : W_BIT)) == 0) {
25076 return(f->filp_mode == FILP_CLOSED ? EIO : EBADF);
25077 }
25078 if (m_in.nbytes == 0)
25079 return(0); /* so char special files need not check for 0*/
25080
25081 /* check if user process has the memory it needs.
25082 * if not, copying will fail later.
25083 * do this after 0-check above because umap doesn’t want to map 0 bytes.
25084 */
25085 if ((r = sys_umap(usr, seg, (vir_bytes) m_in.buffer, m_in.nbytes, &p)) != OK)
25086 return r;
25087 position = f->filp_pos;
25088 oflags = f->filp_flags;
25089 rip = f->filp_ino;
25090 f_size = rip->i_size;
25091 r = OK;
25092 if (rip->i_pipe == I_PIPE) {
25093 /* fp->fp_cum_io_partial is only nonzero when doing partial writes */
25094 cum_io = fp->fp_cum_io_partial;
25095 } else {
25096 cum_io = 0;
25097 }
25098 op = (rw_flag == READING ? DEV_READ : DEV_WRITE);
25099 mode_word = rip->i_mode & I_TYPE;
25100 regular = mode_word == I_REGULAR || mode_word == I_NAMED_PIPE;
25101
25102 if ((char_spec = (mode_word == I_CHAR_SPECIAL ? 1 : 0))) {
25103 if (rip->i_zone[0] == NO_DEV)
25104 panic(__FILE__,"read_write tries to read from "
MINIX SOURCE CODE File: servers/fs/read.c 975
25105 "character device NO_DEV", NO_NUM);
25106 block_size = get_block_size(rip->i_zone[0]);
25107 }
25108 if ((block_spec = (mode_word == I_BLOCK_SPECIAL ? 1 : 0))) {
25109 f_size = ULONG_MAX;
25110 if (rip->i_zone[0] == NO_DEV)
25111 panic(__FILE__,"read_write tries to read from "
25112 " block device NO_DEV", NO_NUM);
25113 block_size = get_block_size(rip->i_zone[0]);
25114 }
25115
25116 if (!char_spec && !block_spec)
25117 block_size = rip->i_sp->s_block_size;
25118
25119 rdwt_err = OK; /* set to EIO if disk error occurs */
25120
25121 /* Check for character special files. */
25122 if (char_spec) {
25123 dev_t dev;
25124 dev = (dev_t) rip->i_zone[0];
25125 r = dev_io(op, dev, usr, m_in.buffer, position, m_in.nbytes, oflags);
25126 if (r >= 0) {
25127 cum_io = r;
25128 position += r;
25129 r = OK;
25130 }
25131 } else {
25132 if (rw_flag == WRITING && block_spec == 0) {
25133 /* Check in advance to see if file will grow too big. */
25134 if (position > rip->i_sp->s_max_size - m_in.nbytes)
25135 return(EFBIG);
25136
25137 /* Check for O_APPEND flag. */
25138 if (oflags & O_APPEND) position = f_size;
25139
25140 /* Clear the zone containing present EOF if hole about
25141 * to be created. This is necessary because all unwritten
25142 * blocks prior to the EOF must read as zeros.
25143 */
25144 if (position > f_size) clear_zone(rip, f_size, 0);
25145 }
25146
25147 /* Pipes are a little different. Check. */
25148 if (rip->i_pipe == I_PIPE) {
25149 r = pipe_check(rip, rw_flag, oflags,
25150 m_in.nbytes, position, &partial_cnt, 0);
25151 if (r <= 0) return(r);
25152 }
25153
25154 if (partial_cnt > 0) partial_pipe = 1;
25155
25156 /* Split the transfer into chunks that don’t span two blocks. */
25157 while (m_in.nbytes != 0) {
25158
25159 off = (unsigned int) (position % block_size);/* offset in blk*/
25160 if (partial_pipe) { /* pipes only */
25161 chunk = MIN(partial_cnt, block_size - off);
25162 } else
25163 chunk = MIN(m_in.nbytes, block_size - off);
25164 if (chunk < 0) chunk = block_size - off;
976 File: servers/fs/read.c MINIX SOURCE CODE
25165
25166 if (rw_flag == READING) {
25167 bytes_left = f_size - position;
25168 if (position >= f_size) break; /* we are beyond EOF */
25169 if (chunk > bytes_left) chunk = (int) bytes_left;
25170 }
25171
25172 /* Read or write ’chunk’ bytes. */
25173 r = rw_chunk(rip, position, off, chunk, (unsigned) m_in.nbytes,
25174 rw_flag, m_in.buffer, seg, usr, block_size, &completed);
25175
25176 if (r != OK) break; /* EOF reached */
25177 if (rdwt_err < 0) break;
25178
25179 /* Update counters and pointers. */
25180 m_in.buffer += chunk; /* user buffer address */
25181 m_in.nbytes -= chunk; /* bytes yet to be read */
25182 cum_io += chunk; /* bytes read so far */
25183 position += chunk; /* position within the file */
25184
25185 if (partial_pipe) {
25186 partial_cnt -= chunk;
25187 if (partial_cnt <= 0) break;
25188 }
25189 }
25190 }
25191
25192 /* On write, update file size and access time. */
25193 if (rw_flag == WRITING) {
25194 if (regular || mode_word == I_DIRECTORY) {
25195 if (position > f_size) rip->i_size = position;
25196 }
25197 } else {
25198 if (rip->i_pipe == I_PIPE) {
25199 if ( position >= rip->i_size) {
25200 /* Reset pipe pointers. */
25201 rip->i_size = 0; /* no data left */
25202 position = 0; /* reset reader(s) */
25203 wf = find_filp(rip, W_BIT);
25204 if (wf != NIL_FILP) wf->filp_pos = 0;
25205 }
25206 }
25207 }
25208 f->filp_pos = position;
25209
25210 /* Check to see if read-ahead is called for, and if so, set it up. */
25211 if (rw_flag == READING && rip->i_seek == NO_SEEK && position % block_size== 0
25212 && (regular || mode_word == I_DIRECTORY)) {
25213 rdahed_inode = rip;
25214 rdahedpos = position;
25215 }
25216 rip->i_seek = NO_SEEK;
25217
25218 if (rdwt_err != OK) r = rdwt_err; /* check for disk error */
25219 if (rdwt_err == END_OF_FILE) r = OK;
25220
25221 /* if user-space copying failed, read/write failed. */
25222 if (r == OK && r2 != OK) {
25223 r = r2;
25224 }
MINIX SOURCE CODE File: servers/fs/read.c 977
25225 if (r == OK) {
25226 if (rw_flag == READING) rip->i_update |= ATIME;
25227 if (rw_flag == WRITING) rip->i_update |= CTIME | MTIME;
25228 rip->i_dirt = DIRTY; /* inode is thus now dirty */
25229 if (partial_pipe) {
25230 partial_pipe = 0;
25231 /* partial write on pipe with */
25232 /* O_NONBLOCK, return write count */
25233 if (!(oflags & O_NONBLOCK)) {
25234 fp->fp_cum_io_partial = cum_io;
25235 suspend(XPIPE); /* partial write on pipe with */
25236 return(SUSPEND); /* nbyte > PIPE_SIZE - non-atomic */
25237 }
25238 }
25239 fp->fp_cum_io_partial = 0;
25240 return(cum_io);
25241 }
25242 if (bufs_in_use < 0) {
25243 panic(__FILE__,"end - bufs_in_use negative", bufs_in_use);
25244 }
25245 return(r);
25246 }
25248 /*===========================================================================*
25249 * rw_chunk *
25250 *===========================================================================*/
25251 PRIVATE int rw_chunk(rip, position, off, chunk, left, rw_flag, buff,
25252 seg, usr, block_size, completed)
25253 register struct inode *rip; /* pointer to inode for file to be rd/wr */
25254 off_t position; /* position within file to read or write */
25255 unsigned off; /* off within the current block */
25256 int chunk; /* number of bytes to read or write */
25257 unsigned left; /* max number of bytes wanted after position */
25258 int rw_flag; /* READING or WRITING */
25259 char *buff; /* virtual address of the user buffer */
25260 int seg; /* T or D segment in user space */
25261 int usr; /* which user process */
25262 int block_size; /* block size of FS operating on */
25263 int *completed; /* number of bytes copied */
25264 {
25265 /* Read or write (part of) a block. */
25266
25267 register struct buf *bp;
25268 register int r = OK;
25269 int n, block_spec;
25270 block_t b;
25271 dev_t dev;
25272
25273 *completed = 0;
25274
25275 block_spec = (rip->i_mode & I_TYPE) == I_BLOCK_SPECIAL;
25276 if (block_spec) {
25277 b = position/block_size;
25278 dev = (dev_t) rip->i_zone[0];
25279 } else {
25280 b = read_map(rip, position);
25281 dev = rip->i_dev;
25282 }
25283
25284 if (!block_spec && b == NO_BLOCK) {
978 File: servers/fs/read.c MINIX SOURCE CODE
25285 if (rw_flag == READING) {
25286 /* Reading from a nonexistent block. Must read as all zeros.*/
25287 bp = get_block(NO_DEV, NO_BLOCK, NORMAL); /* get a buffer */
25288 zero_block(bp);
25289 } else {
25290 /* Writing to a nonexistent block. Create and enter in inode.*/
25291 if ((bp= new_block(rip, position)) == NIL_BUF)return(err_code);
25292 }
25293 } else if (rw_flag == READING) {
25294 /* Read and read ahead if convenient. */
25295 bp = rahead(rip, b, position, left);
25296 } else {
25297 /* Normally an existing block to be partially overwritten is first read
25298 * in. However, a full block need not be read in. If it is already in
25299 * the cache, acquire it, otherwise just acquire a free buffer.
25300 */
25301 n = (chunk == block_size ? NO_READ : NORMAL);
25302 if (!block_spec && off == 0 && position >= rip->i_size) n = NO_READ;
25303 bp = get_block(dev, b, n);
25304 }
25305
25306 /* In all cases, bp now points to a valid buffer. */
25307 if (bp == NIL_BUF) {
25308 panic(__FILE__,"bp not valid in rw_chunk, this can’t happen", NO_NUM);
25309 }
25310 if (rw_flag == WRITING && chunk != block_size && !block_spec &&
25311 position >= rip->i_size && off == 0) {
25312 zero_block(bp);
25313 }
25314
25315 if (rw_flag == READING) {
25316 /* Copy a chunk from the block buffer to user space. */
25317 r = sys_vircopy(FS_PROC_NR, D, (phys_bytes) (bp->b_data+off),
25318 usr, seg, (phys_bytes) buff,
25319 (phys_bytes) chunk);
25320 } else {
25321 /* Copy a chunk from user space to the block buffer. */
25322 r = sys_vircopy(usr, seg, (phys_bytes) buff,
25323 FS_PROC_NR, D, (phys_bytes) (bp->b_data+off),
25324 (phys_bytes) chunk);
25325 bp->b_dirt = DIRTY;
25326 }
25327 n = (off + chunk == block_size ? FULL_DATA_BLOCK : PARTIAL_DATA_BLOCK);
25328 put_block(bp, n);
25329
25330 return(r);
25331 }
25334 /*===========================================================================*
25335 * read_map *
25336 *===========================================================================*/
25337 PUBLIC block_t read_map(rip, position)
25338 register struct inode *rip; /* ptr to inode to map from */
25339 off_t position; /* position in file whose blk wanted */
25340 {
25341 /* Given an inode and a position within the corresponding file, locate the
25342 * block (not zone) number in which that position is to be found and return it.
25343 */
25344
MINIX SOURCE CODE File: servers/fs/read.c 979
25345 register struct buf *bp;
25346 register zone_t z;
25347 int scale, boff, dzones, nr_indirects, index, zind, ex;
25348 block_t b;
25349 long excess, zone, block_pos;
25350
25351 scale = rip->i_sp->s_log_zone_size; /* for block-zone conversion */
25352 block_pos = position/rip->i_sp->s_block_size; /* relative blk # in file */
25353 zone = block_pos >> scale; /* position’s zone */
25354 boff = (int) (block_pos - (zone << scale) ); /* relative blk # within zone */
25355 dzones = rip->i_ndzones;
25356 nr_indirects = rip->i_nindirs;
25357
25358 /* Is ’position’ to be found in the inode itself? */
25359 if (zone < dzones) {
25360 zind = (int) zone; /* index should be an int */
25361 z = rip->i_zone[zind];
25362 if (z == NO_ZONE) return(NO_BLOCK);
25363 b = ((block_t) z << scale) + boff;
25364 return(b);
25365 }
25366
25367 /* It is not in the inode, so it must be single or double indirect. */
25368 excess = zone - dzones; /* first Vx_NR_DZONES don’t count */
25369
25370 if (excess < nr_indirects) {
25371 /* ’position’ can be located via the single indirect block. */
25372 z = rip->i_zone[dzones];
25373 } else {
25374 /* ’position’ can be located via the double indirect block. */
25375 if ( (z = rip->i_zone[dzones+1]) == NO_ZONE) return(NO_BLOCK);
25376 excess -= nr_indirects; /* single indir doesn’t count*/
25377 b = (block_t) z << scale;
25378 bp = get_block(rip->i_dev, b, NORMAL); /* get double indirect block */
25379 index = (int) (excess/nr_indirects);
25380 z = rd_indir(bp, index); /* z= zone for single*/
25381 put_block(bp, INDIRECT_BLOCK); /* release double ind block */
25382 excess = excess % nr_indirects; /* index into single ind blk */
25383 }
25384
25385 /* ’z’ is zone num for single indirect block; ’excess’ is index into it. */
25386 if (z == NO_ZONE) return(NO_BLOCK);
25387 b = (block_t) z << scale; /* b is blk # for single ind */
25388 bp = get_block(rip->i_dev, b, NORMAL); /* get single indirect block */
25389 ex = (int) excess; /* need an integer */
25390 z = rd_indir(bp, ex); /* get block pointed to */
25391 put_block(bp, INDIRECT_BLOCK); /* release single indir blk */
25392 if (z == NO_ZONE) return(NO_BLOCK);
25393 b = ((block_t) z << scale) + boff;
25394 return(b);
25395 }
25397 /*===========================================================================*
25398 * rd_indir *
25399 *===========================================================================*/
25400 PUBLIC zone_t rd_indir(bp, index)
25401 struct buf *bp; /* pointer to indirect block */
25402 int index; /* index into *bp */
25403 {
25404 /* Given a pointer to an indirect block, read one entry. The reason for
980 File: servers/fs/read.c MINIX SOURCE CODE
25405 * making a separate routine out of this is that there are four cases:
25406 * V1 (IBM and 68000), and V2 (IBM and 68000).
25407 */
25408
25409 struct super_block *sp;
25410 zone_t zone; /* V2 zones are longs (shorts in V1) */
25411
25412 sp = get_super(bp->b_dev); /* need super block to find file sys type */
25413
25414 /* read a zone from an indirect block */
25415 if (sp->s_version == V1)
25416 zone = (zone_t) conv2(sp->s_native, (int) bp->b_v1_ind[index]);
25417 else
25418 zone = (zone_t) conv4(sp->s_native, (long) bp->b_v2_ind[index]);
25419
25420 if (zone != NO_ZONE &&
25421 (zone < (zone_t) sp->s_firstdatazone || zone >= sp->s_zones)) {
25422 printf("Illegal zone number %ld in indirect block, index %d\n",
25423 (long) zone, index);
25424 panic(__FILE__,"check file system", NO_NUM);
25425 }
25426 return(zone);
25427 }
25429 /*===========================================================================*
25430 * read_ahead *
25431 *===========================================================================*/
25432 PUBLIC void read_ahead()
25433 {
25434 /* Read a block into the cache before it is needed. */
25435 int block_size;
25436 register struct inode *rip;
25437 struct buf *bp;
25438 block_t b;
25439
25440 rip = rdahed_inode; /* pointer to inode to read ahead from */
25441 block_size = get_block_size(rip->i_dev);
25442 rdahed_inode = NIL_INODE; /* turn off read ahead */
25443 if ( (b = read_map(rip, rdahedpos)) == NO_BLOCK) return; /* at EOF */
25444 bp = rahead(rip, b, rdahedpos, block_size);
25445 put_block(bp, PARTIAL_DATA_BLOCK);
25446 }
25448 /*===========================================================================*
25449 * rahead *
25450 *===========================================================================*/
25451 PUBLIC struct buf *rahead(rip, baseblock, position, bytes_ahead)
25452 register struct inode *rip; /* pointer to inode for file to be read */
25453 block_t baseblock; /* block at current position */
25454 off_t position; /* position within file */
25455 unsigned bytes_ahead; /* bytes beyond position for immediate use */
25456 {
25457 /* Fetch a block from the cache or the device. If a physical read is
25458 * required, prefetch as many more blocks as convenient into the cache.
25459 * This usually covers bytes_ahead and is at least BLOCKS_MINIMUM.
25460 * The device driver may decide it knows better and stop reading at a
25461 * cylinder boundary (or after an error). Rw_scattered() puts an optional
25462 * flag on all reads to allow this.
25463 */
25464 int block_size;
MINIX SOURCE CODE File: servers/fs/read.c 981
25465 /* Minimum number of blocks to prefetch. */
25466 # define BLOCKS_MINIMUM (NR_BUFS < 50 ? 18 : 32)
25467 int block_spec, scale, read_q_size;
25468 unsigned int blocks_ahead, fragment;
25469 block_t block, blocks_left;
25470 off_t ind1_pos;
25471 dev_t dev;
25472 struct buf *bp;
25473 static struct buf *read_q[NR_BUFS];
25474
25475 block_spec = (rip->i_mode & I_TYPE) == I_BLOCK_SPECIAL;
25476 if (block_spec) {
25477 dev = (dev_t) rip->i_zone[0];
25478 } else {
25479 dev = rip->i_dev;
25480 }
25481 block_size = get_block_size(dev);
25482
25483 block = baseblock;
25484 bp = get_block(dev, block, PREFETCH);
25485 if (bp->b_dev != NO_DEV) return(bp);
25486
25487 /* The best guess for the number of blocks to prefetch: A lot.
25488 * It is impossible to tell what the device looks like, so we don’t even
25489 * try to guess the geometry, but leave it to the driver.
25490 *
25491 * The floppy driver can read a full track with no rotational delay, and it
25492 * avoids reading partial tracks if it can, so handing it enough buffers to
25493 * read two tracks is perfect. (Two, because some diskette types have
25494 * an odd number of sectors per track, so a block may span tracks.)
25495 *
25496 * The disk drivers don’t try to be smart. With todays disks it is
25497 * impossible to tell what the real geometry looks like, so it is best to
25498 * read as much as you can. With luck the caching on the drive allows
25499 * for a little time to start the next read.
25500 *
25501 * The current solution below is a bit of a hack, it just reads blocks from
25502 * the current file position hoping that more of the file can be found. A
25503 * better solution must look at the already available zone pointers and
25504 * indirect blocks (but don’t call read_map!).
25505 */
25506
25507 fragment = position % block_size;
25508 position -= fragment;
25509 bytes_ahead += fragment;
25510
25511 blocks_ahead = (bytes_ahead + block_size - 1) / block_size;
25512
25513 if (block_spec && rip->i_size == 0) {
25514 blocks_left = NR_IOREQS;
25515 } else {
25516 blocks_left = (rip->i_size - position + block_size - 1) / block_size;
25517
25518 /* Go for the first indirect block if we are in its neighborhood. */
25519 if (!block_spec) {
25520 scale = rip->i_sp->s_log_zone_size;
25521 ind1_pos = (off_t) rip->i_ndzones * (block_size << scale);
25522 if (position <= ind1_pos && rip->i_size > ind1_pos) {
25523 blocks_ahead++;
25524 blocks_left++;
982 File: servers/fs/read.c MINIX SOURCE CODE
25525 }
25526 }
25527 }
25528
25529 /* No more than the maximum request. */
25530 if (blocks_ahead > NR_IOREQS) blocks_ahead = NR_IOREQS;
25531
25532 /* Read at least the minimum number of blocks, but not after a seek. */
25533 if (blocks_ahead < BLOCKS_MINIMUM && rip->i_seek == NO_SEEK)
25534 blocks_ahead = BLOCKS_MINIMUM;
25535
25536 /* Can’t go past end of file. */
25537 if (blocks_ahead > blocks_left) blocks_ahead = blocks_left;
25538
25539 read_q_size = 0;
25540
25541 /* Acquire block buffers. */
25542 for (;;) {
25543 read_q[read_q_size++] = bp;
25544
25545 if (--blocks_ahead == 0) break;
25546
25547 /* Don’t trash the cache, leave 4 free. */
25548 if (bufs_in_use >= NR_BUFS - 4) break;
25549
25550 block++;
25551
25552 bp = get_block(dev, block, PREFETCH);
25553 if (bp->b_dev != NO_DEV) {
25554 /* Oops, block already in the cache, get out. */
25555 put_block(bp, FULL_DATA_BLOCK);
25556 break;
25557 }
25558 }
25559 rw_scattered(dev, read_q, read_q_size, READING);
25560 return(get_block(dev, baseblock, NORMAL));
25561 }
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
25629 return(read_write(WRITING));
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
25657 zindex = (int) zone; /* we need an integer here */
25658 rip->i_zone[zindex] = new_zone;
25659 return(OK);
25660 }
25661
25662 /* It is not in the inode, so it must be single or double indirect. */
25663 excess = zone - zones; /* first Vx_NR_DZONES don’t count */
25664 new_ind = FALSE;
25665 new_dbl = FALSE;
25666
25667 if (excess < nr_indirects) {
25668 /* ’position’ can be located via the single indirect block. */
25669 z1 = rip->i_zone[zones]; /* single indirect zone */
25670 single = TRUE;
25671 } else {
25672 /* ’position’ can be located via the double indirect block. */
25673 if ( (z = rip->i_zone[zones+1]) == NO_ZONE) {
25674 /* Create the double indirect block. */
984 File: servers/fs/write.c MINIX SOURCE CODE
25675 if ( (z = alloc_zone(rip->i_dev, rip->i_zone[0])) == NO_ZONE)
25676 return(err_code);
25677 rip->i_zone[zones+1] = z;
25678 new_dbl = TRUE; /* set flag for later */
25679 }
25680
25681 /* Either way, ’z’ is zone number for double indirect block. */
25682 excess -= nr_indirects; /* single indirect doesn’t count */
25683 ind_ex = (int) (excess / nr_indirects);
25684 excess = excess % nr_indirects;
25685 if (ind_ex >= nr_indirects) return(EFBIG);
25686 b = (block_t) z << scale;
25687 bp = get_block(rip->i_dev, b, (new_dbl ? NO_READ : NORMAL));
25688 if (new_dbl) zero_block(bp);
25689 z1 = rd_indir(bp, ind_ex);
25690 single = FALSE;
25691 }
25692
25693 /* z1 is now single indirect zone; ’excess’ is index. */
25694 if (z1 == NO_ZONE) {
25695 /* Create indirect block and store zone # in inode or dbl indir blk. */
25696 z1 = alloc_zone(rip->i_dev, rip->i_zone[0]);
25697 if (single)
25698 rip->i_zone[zones] = z1; /* update inode */
25699 else
25700 wr_indir(bp, ind_ex, z1); /* update dbl indir */
25701
25702 new_ind = TRUE;
25703 if (bp != NIL_BUF) bp->b_dirt = DIRTY; /* if double ind, it is dirty*/
25704 if (z1 == NO_ZONE) {
25705 put_block(bp, INDIRECT_BLOCK); /* release dbl indirect blk */
25706 return(err_code); /* couldn’t create single ind */
25707 }
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
25738 if (sp->s_version == V1)
25739 bp->b_v1_ind[index] = (zone1_t) conv2(sp->s_native, (int) zone);
25740 else
25741 bp->b_v2_ind[index] = (zone_t) conv4(sp->s_native, (long) zone);
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
25778 bp = get_block(rip->i_dev, b, NO_READ);
25779 zero_block(bp);
25780 put_block(bp, FULL_DATA_BLOCK);
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
25810 if (rip->i_zone[0] == NO_ZONE) {
25811 sp = rip->i_sp;
25812 z = sp->s_firstdatazone;
25813 } else {
25814 z = rip->i_zone[0]; /* hunt near first zone */
25815 }
25816 if ( (z = alloc_zone(rip->i_dev, z)) == NO_ZONE) return(NIL_BUF);
25817 if ( (r = write_map(rip, position, z)) != OK) {
25818 free_zone(rip->i_dev, z);
25819 err_code = r;
25820 return(NIL_BUF);
25821 }
25822
25823 /* If we are not writing at EOF, clear the zone, just to be safe. */
25824 if ( position != rip->i_size) clear_zone(rip, position, 1);
25825 scale = rip->i_sp->s_log_zone_size;
25826 base_block = (block_t) z << scale;
25827 zone_size = (zone_t) rip->i_sp->s_block_size << scale;
25828 b = base_block + (block_t)((position % zone_size)/rip->i_sp->s_block_size);
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
25842 /* Zero a block. */
25843 memset(bp->b_data, 0, MAX_BLOCK_SIZE);
25844 bp->b_dirt = DIRTY;
25845 }
MINIX SOURCE CODE File: servers/fs/pipe.c 987
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/pipe.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
25900 /* This file deals with the suspension and revival of processes. A process can
25901 * be suspended because it wants to read or write from a pipe and can’t, or
25902 * because it wants to read or write from a special file and can’t. When a
25903 * process can’t continue it is suspended, and revived later when it is able
25904 * to continue.
25905 *
25906 * The entry points into this file are
25907 * do_pipe: perform the PIPE system call
25908 * pipe_check: check to see that a read or write on a pipe is feasible now
25909 * suspend: suspend a process that cannot do a requested read or write
25910 * release: check to see if a suspended process can be released and do
25911 * it
25912 * revive: mark a suspended process as able to run again
25913 * do_unpause: a signal has been sent to a process; see if it suspended
25914 */
25915
25916 #include "fs.h"
25917 #include <fcntl.h>
25918 #include <signal.h>
25919 #include <minix/callnr.h>
25920 #include <minix/com.h>
25921 #include <sys/select.h>
25922 #include <sys/time.h>
25923 #include "file.h"
25924 #include "fproc.h"
25925 #include "inode.h"
25926 #include "param.h"
25927 #include "super.h"
25928 #include "select.h"
25929
25930 /*===========================================================================*
25931 * do_pipe *
25932 *===========================================================================*/
25933 PUBLIC int do_pipe()
25934 {
25935 /* Perform the pipe(fil_des) system call. */
25936
25937 register struct fproc *rfp;
25938 register struct inode *rip;
25939 int r;
25940 struct filp *fil_ptr0, *fil_ptr1;
25941 int fil_des[2]; /* reply goes here */
25942
25943 /* Acquire two file descriptors. */
25944 rfp = fp;
25945 if ( (r = get_fd(0, R_BIT, &fil_des[0], &fil_ptr0)) != OK) return(r);
25946 rfp->fp_filp[fil_des[0]] = fil_ptr0;
25947 fil_ptr0->filp_count = 1;
25948 if ( (r = get_fd(0, W_BIT, &fil_des[1], &fil_ptr1)) != OK) {
25949 rfp->fp_filp[fil_des[0]] = NIL_FILP;
25950 fil_ptr0->filp_count = 0;
25951 return(r);
25952 }
25953 rfp->fp_filp[fil_des[1]] = fil_ptr1;
25954 fil_ptr1->filp_count = 1;
988 File: servers/fs/pipe.c MINIX SOURCE CODE
25955
25956 /* Make the inode on the pipe device. */
25957 if ( (rip = alloc_inode(root_dev, I_REGULAR) ) == NIL_INODE) {
25958 rfp->fp_filp[fil_des[0]] = NIL_FILP;
25959 fil_ptr0->filp_count = 0;
25960 rfp->fp_filp[fil_des[1]] = NIL_FILP;
25961 fil_ptr1->filp_count = 0;
25962 return(err_code);
25963 }
25964
25965 if (read_only(rip) != OK)
25966 panic(__FILE__,"pipe device is read only", NO_NUM);
25967
25968 rip->i_pipe = I_PIPE;
25969 rip->i_mode &= ˜I_REGULAR;
25970 rip->i_mode |= I_NAMED_PIPE; /* pipes and FIFOs have this bit set */
25971 fil_ptr0->filp_ino = rip;
25972 fil_ptr0->filp_flags = O_RDONLY;
25973 dup_inode(rip); /* for double usage */
25974 fil_ptr1->filp_ino = rip;
25975 fil_ptr1->filp_flags = O_WRONLY;
25976 rw_inode(rip, WRITING); /* mark inode as allocated */
25977 m_out.reply_i1 = fil_des[0];
25978 m_out.reply_i2 = fil_des[1];
25979 rip->i_update = ATIME | CTIME | MTIME;
25980 return(OK);
25981 }
25983 /*===========================================================================*
25984 * pipe_check *
25985 *===========================================================================*/
25986 PUBLIC int pipe_check(rip, rw_flag, oflags, bytes, position, canwrite, notouch)
25987 register struct inode *rip; /* the inode of the pipe */
25988 int rw_flag; /* READING or WRITING */
25989 int oflags; /* flags set by open or fcntl */
25990 register int bytes; /* bytes to be read or written (all chunks) */
25991 register off_t position; /* current file position */
25992 int *canwrite; /* return: number of bytes we can write */
25993 int notouch; /* check only */
25994 {
25995 /* Pipes are a little different. If a process reads from an empty pipe for
25996 * which a writer still exists, suspend the reader. If the pipe is empty
25997 * and there is no writer, return 0 bytes. If a process is writing to a
25998 * pipe and no one is reading from it, give a broken pipe error.
25999 */
26000
26001 /* If reading, check for empty pipe. */
26002 if (rw_flag == READING) {
26003 if (position >= rip->i_size) {
26004 /* Process is reading from an empty pipe. */
26005 int r = 0;
26006 if (find_filp(rip, W_BIT) != NIL_FILP) {
26007 /* Writer exists */
26008 if (oflags & O_NONBLOCK) {
26009 r = EAGAIN;
26010 } else {
26011 if (!notouch)
26012 suspend(XPIPE); /* block reader */
26013 r = SUSPEND;
26014 }
MINIX SOURCE CODE File: servers/fs/pipe.c 989
26015 /* If need be, activate sleeping writers. */
26016 if (susp_count > 0 && !notouch)
26017 release(rip, WRITE, susp_count);
26018 }
26019 return(r);
26020 }
26021 } else {
26022 /* Process is writing to a pipe. */
26023 if (find_filp(rip, R_BIT) == NIL_FILP) {
26024 /* Tell kernel to generate a SIGPIPE signal. */
26025 if (!notouch)
26026 sys_kill((int)(fp - fproc), SIGPIPE);
26027 return(EPIPE);
26028 }
26029
26030 if (position + bytes > PIPE_SIZE(rip->i_sp->s_block_size)) {
26031 if ((oflags & O_NONBLOCK)
26032 && bytes < PIPE_SIZE(rip->i_sp->s_block_size))
26033 return(EAGAIN);
26034 else if ((oflags & O_NONBLOCK)
26035 && bytes > PIPE_SIZE(rip->i_sp->s_block_size)) {
26036 if ( (*canwrite = (PIPE_SIZE(rip->i_sp->s_block_size)
26037 - position)) > 0) {
26038 /* Do a partial write. Need to wakeup reader */
26039 if (!notouch)
26040 release(rip, READ, susp_count);
26041 return(1);
26042 } else {
26043 return(EAGAIN);
26044 }
26045 }
26046 if (bytes > PIPE_SIZE(rip->i_sp->s_block_size)) {
26047 if ((*canwrite = PIPE_SIZE(rip->i_sp->s_block_size)
26048 - position) > 0) {
26049 /* Do a partial write. Need to wakeup reader
26050 * since we’ll suspend ourself in read_write()
26051 */
26052 release(rip, READ, susp_count);
26053 return(1);
26054 }
26055 }
26056 if (!notouch)
26057 suspend(XPIPE); /* stop writer -- pipe full */
26058 return(SUSPEND);
26059 }
26060
26061 /* Writing to an empty pipe. Search for suspended reader. */
26062 if (position == 0 && !notouch)
26063 release(rip, READ, susp_count);
26064 }
26065
26066 *canwrite = 0;
26067 return(1);
26068 }
26070 /*===========================================================================*
26071 * suspend *
26072 *===========================================================================*/
26073 PUBLIC void suspend(task)
26074 int task; /* who is proc waiting for? (PIPE = pipe) */
990 File: servers/fs/pipe.c MINIX SOURCE CODE
26075 {
26076 /* Take measures to suspend the processing of the present system call.
26077 * Store the parameters to be used upon resuming in the process table.
26078 * (Actually they are not used when a process is waiting for an I/O device,
26079 * but they are needed for pipes, and it is not worth making the distinction.)
26080 * The SUSPEND pseudo error should be returned after calling suspend().
26081 */
26082
26083 if (task == XPIPE || task == XPOPEN) susp_count++;/* #procs susp’ed on pipe*/
26084 fp->fp_suspended = SUSPENDED;
26085 fp->fp_fd = m_in.fd << 8 | call_nr;
26086 fp->fp_task = -task;
26087 if (task == XLOCK) {
26088 fp->fp_buffer = (char *) m_in.name1; /* third arg to fcntl() */
26089 fp->fp_nbytes = m_in.request; /* second arg to fcntl() */
26090 } else {
26091 fp->fp_buffer = m_in.buffer; /* for reads and writes */
26092 fp->fp_nbytes = m_in.nbytes;
26093 }
26094 }
26096 /*===========================================================================*
26097 * release *
26098 *===========================================================================*/
26099 PUBLIC void release(ip, call_nr, count)
26100 register struct inode *ip; /* inode of pipe */
26101 int call_nr; /* READ, WRITE, OPEN or CREAT */
26102 int count; /* max number of processes to release */
26103 {
26104 /* Check to see if any process is hanging on the pipe whose inode is in ’ip’.
26105 * If one is, and it was trying to perform the call indicated by ’call_nr’,
26106 * release it.
26107 */
26108
26109 register struct fproc *rp;
26110 struct filp *f;
26111
26112 /* Trying to perform the call also includes SELECTing on it with that
26113 * operation.
26114 */
26115 if (call_nr == READ || call_nr == WRITE) {
26116 int op;
26117 if (call_nr == READ)
26118 op = SEL_RD;
26119 else
26120 op = SEL_WR;
26121 for(f = &filp[0]; f < &filp[NR_FILPS]; f++) {
26122 if (f->filp_count < 1 || !(f->filp_pipe_select_ops & op) ||
26123 f->filp_ino != ip)
26124 continue;
26125 select_callback(f, op);
26126 f->filp_pipe_select_ops &= ˜op;
26127 }
26128 }
26129
26130 /* Search the proc table. */
26131 for (rp = &fproc[0]; rp < &fproc[NR_PROCS]; rp++) {
26132 if (rp->fp_suspended == SUSPENDED &&
26133 rp->fp_revived == NOT_REVIVING &&
26134 (rp->fp_fd & BYTE) == call_nr &&
MINIX SOURCE CODE File: servers/fs/pipe.c 991
26135 rp->fp_filp[rp->fp_fd>>8]->filp_ino == ip) {
26136 revive((int)(rp - fproc), 0);
26137 susp_count--; /* keep track of who is suspended */
26138 if (--count == 0) return;
26139 }
26140 }
26141 }
26143 /*===========================================================================*
26144 * revive *
26145 *===========================================================================*/
26146 PUBLIC void revive(proc_nr, returned)
26147 int proc_nr; /* process to revive */
26148 int returned; /* if hanging on task, how many bytes read */
26149 {
26150 /* Revive a previously blocked process. When a process hangs on tty, this
26151 * is the way it is eventually released.
26152 */
26153
26154 register struct fproc *rfp;
26155 register int task;
26156
26157 if (proc_nr < 0 || proc_nr >= NR_PROCS)
26158 panic(__FILE__,"revive err", proc_nr);
26159 rfp = &fproc[proc_nr];
26160 if (rfp->fp_suspended == NOT_SUSPENDED || rfp->fp_revived == REVIVING)return;
26161
26162 /* The ’reviving’ flag only applies to pipes. Processes waiting for TTY get
26163 * a message right away. The revival process is different for TTY and pipes.
26164 * For select and TTY revival, the work is already done, for pipes it is not:
26165 * the proc must be restarted so it can try again.
26166 */
26167 task = -rfp->fp_task;
26168 if (task == XPIPE || task == XLOCK) {
26169 /* Revive a process suspended on a pipe or lock. */
26170 rfp->fp_revived = REVIVING;
26171 reviving++; /* process was waiting on pipe or lock */
26172 } else {
26173 rfp->fp_suspended = NOT_SUSPENDED;
26174 if (task == XPOPEN) /* process blocked in open or create */
26175 reply(proc_nr, rfp->fp_fd>>8);
26176 else if (task == XSELECT) {
26177 reply(proc_nr, returned);
26178 } else {
26179 /* Revive a process suspended on TTY or other device. */
26180 rfp->fp_nbytes = returned; /*pretend it wants only what there is*/
26181 reply(proc_nr, returned); /* unblock the process */
26182 }
26183 }
26184 }
26186 /*===========================================================================*
26187 * do_unpause *
26188 *===========================================================================*/
26189 PUBLIC int do_unpause()
26190 {
26191 /* A signal has been sent to a user who is paused on the file system.
26192 * Abort the system call with the EINTR error message.
26193 */
26194
992 File: servers/fs/pipe.c MINIX SOURCE CODE
26195 register struct fproc *rfp;
26196 int proc_nr, task, fild;
26197 struct filp *f;
26198 dev_t dev;
26199 message mess;
26200
26201 if (who > PM_PROC_NR) return(EPERM);
26202 proc_nr = m_in.pro;
26203 if (proc_nr < 0 || proc_nr >= NR_PROCS)
26204 panic(__FILE__,"unpause err 1", proc_nr);
26205 rfp = &fproc[proc_nr];
26206 if (rfp->fp_suspended == NOT_SUSPENDED) return(OK);
26207 task = -rfp->fp_task;
26208
26209 switch (task) {
26210 case XPIPE: /* process trying to read or write a pipe */
26211 break;
26212
26213 case XLOCK: /* process trying to set a lock with FCNTL */
26214 break;
26215
26216 case XSELECT: /* process blocking on select() */
26217 select_forget(proc_nr);
26218 break;
26219
26220 case XPOPEN: /* process trying to open a fifo */
26221 break;
26222
26223 default: /* process trying to do device I/O (e.g. tty)*/
26224 fild = (rfp->fp_fd >> 8) & BYTE;/* extract file descriptor */
26225 if (fild < 0 || fild >= OPEN_MAX)
26226 panic(__FILE__,"unpause err 2",NO_NUM);
26227 f = rfp->fp_filp[fild];
26228 dev = (dev_t) f->filp_ino->i_zone[0]; /* device hung on */
26229 mess.TTY_LINE = (dev >> MINOR) & BYTE;
26230 mess.PROC_NR = proc_nr;
26231
26232 /* Tell kernel R or W. Mode is from current call, not open. */
26233 mess.COUNT = (rfp->fp_fd & BYTE) == READ ? R_BIT : W_BIT;
26234 mess.m_type = CANCEL;
26235 fp = rfp; /* hack - ctty_io uses fp */
26236 (*dmap[(dev >> MAJOR) & BYTE].dmap_io)(task, &mess);
26237 }
26238
26239 rfp->fp_suspended = NOT_SUSPENDED;
26240 reply(proc_nr, EINTR); /* signal interrupted call */
26241 return(OK);
26242 }
26244 /*===========================================================================*
26245 * select_request_pipe *
26246 *===========================================================================*/
26247 PUBLIC int select_request_pipe(struct filp *f, int *ops, int block)
26248 {
26249 int orig_ops, r = 0, err, canwrite;
26250 orig_ops = *ops;
26251 if ((*ops & SEL_RD)) {
26252 if ((err = pipe_check(f->filp_ino, READING, 0,
26253 1, f->filp_pos, &canwrite, 1)) != SUSPEND)
26254 r |= SEL_RD;
MINIX SOURCE CODE File: servers/fs/pipe.c 993
26255 if (err < 0 && err != SUSPEND && (*ops & SEL_ERR))
26256 r |= SEL_ERR;
26257 }
26258 if ((*ops & SEL_WR)) {
26259 if ((err = pipe_check(f->filp_ino, WRITING, 0,
26260 1, f->filp_pos, &canwrite, 1)) != SUSPEND)
26261 r |= SEL_WR;
26262 if (err < 0 && err != SUSPEND && (*ops & SEL_ERR))
26263 r |= SEL_ERR;
26264 }
26265
26266 *ops = r;
26267
26268 if (!r && block) {
26269 f->filp_pipe_select_ops |= orig_ops;
26270 }
26271
26272 return SEL_OK;
26273 }
26275 /*===========================================================================*
26276 * select_match_pipe *
26277 *===========================================================================*/
26278 PUBLIC int select_match_pipe(struct filp *f)
26279 {
26280 /* recognize either pipe or named pipe (FIFO) */
26281 if (f && f->filp_ino && (f->filp_ino->i_mode & I_NAMED_PIPE))
26282 return 1;
26283 return 0;
26284 }
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
1000 File: servers/fs/mount.c MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/mount.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
26700 /* This file performs the MOUNT and UMOUNT system calls.
26701 *
26702 * The entry points into this file are
26703 * do_mount: perform the MOUNT system call
26704 * do_umount: perform the UMOUNT system call
26705 */
26706
26707 #include "fs.h"
26708 #include <fcntl.h>
26709 #include <minix/com.h>
26710 #include <sys/stat.h>
26711 #include "buf.h"
26712 #include "file.h"
26713 #include "fproc.h"
26714 #include "inode.h"
26715 #include "param.h"
26716 #include "super.h"
26717
26718 FORWARD _PROTOTYPE( dev_t name_to_dev, (char *path) );
26719
26720 /*===========================================================================*
26721 * do_mount *
26722 *===========================================================================*/
26723 PUBLIC int do_mount()
26724 {
26725 /* Perform the mount(name, mfile, rd_only) system call. */
26726
26727 register struct inode *rip, *root_ip;
26728 struct super_block *xp, *sp;
26729 dev_t dev;
26730 mode_t bits;
26731 int rdir, mdir; /* TRUE iff {root|mount} file is dir */
26732 int r, found;
26733
26734 /* Only the super-user may do MOUNT. */
26735 if (!super_user) return(EPERM);
26736
26737 /* If ’name’ is not for a block special file, return error. */
26738 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
26739 if ( (dev = name_to_dev(user_path)) == NO_DEV) return(err_code);
26740
26741 /* Scan super block table to see if dev already mounted & find a free slot.*/
26742 sp = NIL_SUPER;
26743 found = FALSE;
26744 for (xp = &super_block[0]; xp < &super_block[NR_SUPERS]; xp++) {
26745 if (xp->s_dev == dev) found = TRUE; /* is it mounted already? */
26746 if (xp->s_dev == NO_DEV) sp = xp; /* record free slot */
26747 }
26748 if (found) return(EBUSY); /* already mounted */
26749 if (sp == NIL_SUPER) return(ENFILE); /* no super block available */
26750
26751 /* Open the device the file system lives on. */
26752 if (dev_open(dev, who, m_in.rd_only ? R_BIT : (R_BIT|W_BIT)) != OK)
26753 return(EINVAL);
26754
MINIX SOURCE CODE File: servers/fs/mount.c 1001
26755 /* Make the cache forget about blocks it has open on the filesystem */
26756 (void) do_sync();
26757 invalidate(dev);
26758
26759 /* Fill in the super block. */
26760 sp->s_dev = dev; /* read_super() needs to know which dev */
26761 r = read_super(sp);
26762
26763 /* Is it recognized as a Minix filesystem? */
26764 if (r != OK) {
26765 dev_close(dev);
26766 sp->s_dev = NO_DEV;
26767 return(r);
26768 }
26769
26770 /* Now get the inode of the file to be mounted on. */
26771 if (fetch_name(m_in.name2, m_in.name2_length, M1) != OK) {
26772 dev_close(dev);
26773 sp->s_dev = NO_DEV;
26774 return(err_code);
26775 }
26776 if ( (rip = eat_path(user_path)) == NIL_INODE) {
26777 dev_close(dev);
26778 sp->s_dev = NO_DEV;
26779 return(err_code);
26780 }
26781
26782 /* It may not be busy. */
26783 r = OK;
26784 if (rip->i_count > 1) r = EBUSY;
26785
26786 /* It may not be special. */
26787 bits = rip->i_mode & I_TYPE;
26788 if (bits == I_BLOCK_SPECIAL || bits == I_CHAR_SPECIAL) r = ENOTDIR;
26789
26790 /* Get the root inode of the mounted file system. */
26791 root_ip = NIL_INODE; /* if ’r’ not OK, make sure this is defined */
26792 if (r == OK) {
26793 if ( (root_ip = get_inode(dev, ROOT_INODE)) == NIL_INODE) r = err_code;
26794 }
26795 if (root_ip != NIL_INODE && root_ip->i_mode == 0) {
26796 r = EINVAL;
26797 }
26798
26799 /* File types of ’rip’ and ’root_ip’ may not conflict. */
26800 if (r == OK) {
26801 mdir = ((rip->i_mode & I_TYPE) == I_DIRECTORY); /* TRUE iff dir */
26802 rdir = ((root_ip->i_mode & I_TYPE) == I_DIRECTORY);
26803 if (!mdir && rdir) r = EISDIR;
26804 }
26805
26806 /* If error, return the super block and both inodes; release the maps. */
26807 if (r != OK) {
26808 put_inode(rip);
26809 put_inode(root_ip);
26810 (void) do_sync();
26811 invalidate(dev);
26812 dev_close(dev);
26813 sp->s_dev = NO_DEV;
26814 return(r);
1002 File: servers/fs/mount.c MINIX SOURCE CODE
26815 }
26816
26817 /* Nothing else can go wrong. Perform the mount. */
26818 rip->i_mount = I_MOUNT; /* this bit says the inode is mounted on */
26819 sp->s_imount = rip;
26820 sp->s_isup = root_ip;
26821 sp->s_rd_only = m_in.rd_only;
26822 return(OK);
26823 }
26825 /*===========================================================================*
26826 * do_umount *
26827 *===========================================================================*/
26828 PUBLIC int do_umount()
26829 {
26830 /* Perform the umount(name) system call. */
26831 dev_t dev;
26832
26833 /* Only the super-user may do UMOUNT. */
26834 if (!super_user) return(EPERM);
26835
26836 /* If ’name’ is not for a block special file, return error. */
26837 if (fetch_name(m_in.name, m_in.name_length, M3) != OK) return(err_code);
26838 if ( (dev = name_to_dev(user_path)) == NO_DEV) return(err_code);
26839
26840 return(unmount(dev));
26841 }
26843 /*===========================================================================*
26844 * unmount *
26845 *===========================================================================*/
26846 PUBLIC int unmount(dev)
26847 Dev_t dev;
26848 {
26849 /* Unmount a file system by device number. */
26850 register struct inode *rip;
26851 struct super_block *sp, *sp1;
26852 int count;
26853
26854 /* See if the mounted device is busy. Only 1 inode using it should be
26855 * open -- the root inode -- and that inode only 1 time.
26856 */
26857 count = 0;
26858 for (rip = &inode[0]; rip< &inode[NR_INODES]; rip++)
26859 if (rip->i_count > 0 && rip->i_dev == dev) count += rip->i_count;
26860 if (count > 1) return(EBUSY); /* can’t umount a busy file system */
26861
26862 /* Find the super block. */
26863 sp = NIL_SUPER;
26864 for (sp1 = &super_block[0]; sp1 < &super_block[NR_SUPERS]; sp1++) {
26865 if (sp1->s_dev == dev) {
26866 sp = sp1;
26867 break;
26868 }
26869 }
26870
26871 /* Sync the disk, and invalidate cache. */
26872 (void) do_sync(); /* force any cached blocks out of memory */
26873 invalidate(dev); /* invalidate cache entries for this dev */
26874 if (sp == NIL_SUPER) {
MINIX SOURCE CODE File: servers/fs/mount.c 1003
26875 return(EINVAL);
26876 }
26877
26878 /* Close the device the file system lives on. */
26879 dev_close(dev);
26880
26881 /* Finish off the unmount. */
26882 sp->s_imount->i_mount = NO_MOUNT; /* inode returns to normal */
26883 put_inode(sp->s_imount); /* release the inode mounted on */
26884 put_inode(sp->s_isup); /* release the root inode of the mounted fs */
26885 sp->s_imount = NIL_INODE;
26886 sp->s_dev = NO_DEV;
26887 return(OK);
26888 }
26890 /*===========================================================================*
26891 * name_to_dev *
26892 *===========================================================================*/
26893 PRIVATE dev_t name_to_dev(path)
26894 char *path; /* pointer to path name */
26895 {
26896 /* Convert the block special file ’path’ to a device number. If ’path’
26897 * is not a block special file, return error code in ’err_code’.
26898 */
26899
26900 register struct inode *rip;
26901 register dev_t dev;
26902
26903 /* If ’path’ can’t be opened, give up immediately. */
26904 if ( (rip = eat_path(path)) == NIL_INODE) return(NO_DEV);
26905
26906 /* If ’path’ is not a block special file, return error. */
26907 if ( (rip->i_mode & I_TYPE) != I_BLOCK_SPECIAL) {
26908 err_code = ENOTBLK;
26909 put_inode(rip);
26910 return(NO_DEV);
26911 }
26912
26913 /* Extract the device number. */
26914 dev = (dev_t) rip->i_zone[0];
26915 put_inode(rip);
26916 return(dev);
26917 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/link.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
27000 /* This file handles the LINK and UNLINK system calls. It also deals with
27001 * deallocating the storage used by a file when the last UNLINK is done to a
27002 * file and the blocks must be returned to the free block pool.
27003 *
27004 * The entry points into this file are
27005 * do_link: perform the LINK system call
27006 * do_unlink: perform the UNLINK and RMDIR system calls
27007 * do_rename: perform the RENAME system call
27008 * truncate: release all the blocks associated with an inode
27009 */
1004 File: servers/fs/link.c MINIX SOURCE CODE
27010
27011 #include "fs.h"
27012 #include <sys/stat.h>
27013 #include <string.h>
27014 #include <minix/com.h>
27015 #include <minix/callnr.h>
27016 #include "buf.h"
27017 #include "file.h"
27018 #include "fproc.h"
27019 #include "inode.h"
27020 #include "param.h"
27021 #include "super.h"
27022
27023 #define SAME 1000
27024
27025 FORWARD _PROTOTYPE( int remove_dir, (struct inode *rldirp, struct inode *rip,
27026 char dir_name[NAME_MAX]) );
27027
27028 FORWARD _PROTOTYPE( int unlink_file, (struct inode *dirp, struct inode *rip,
27029 char file_name[NAME_MAX]) );
27030
27031 /*===========================================================================*
27032 * do_link *
27033 *===========================================================================*/
27034 PUBLIC int do_link()
27035 {
27036 /* Perform the link(name1, name2) system call. */
27037
27038 register struct inode *ip, *rip;
27039 register int r;
27040 char string[NAME_MAX];
27041 struct inode *new_ip;
27042
27043 /* See if ’name’ (file to be linked) exists. */
27044 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
27045 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
27046
27047 /* Check to see if the file has maximum number of links already. */
27048 r = OK;
27049 if (rip->i_nlinks >= (rip->i_sp->s_version == V1 ? CHAR_MAX : SHRT_MAX))
27050 r = EMLINK;
27051
27052 /* Only super_user may link to directories. */
27053 if (r == OK)
27054 if ( (rip->i_mode & I_TYPE) == I_DIRECTORY && !super_user) r = EPERM;
27055
27056 /* If error with ’name’, return the inode. */
27057 if (r != OK) {
27058 put_inode(rip);
27059 return(r);
27060 }
27061
27062 /* Does the final directory of ’name2’ exist? */
27063 if (fetch_name(m_in.name2, m_in.name2_length, M1) != OK) {
27064 put_inode(rip);
27065 return(err_code);
27066 }
27067 if ( (ip = last_dir(user_path, string)) == NIL_INODE) r = err_code;
27068
27069 /* If ’name2’ exists in full (even if no space) set ’r’ to error. */
MINIX SOURCE CODE File: servers/fs/link.c 1005
27070 if (r == OK) {
27071 if ( (new_ip = advance(ip, string)) == NIL_INODE) {
27072 r = err_code;
27073 if (r == ENOENT) r = OK;
27074 } else {
27075 put_inode(new_ip);
27076 r = EEXIST;
27077 }
27078 }
27079
27080 /* Check for links across devices. */
27081 if (r == OK)
27082 if (rip->i_dev != ip->i_dev) r = EXDEV;
27083
27084 /* Try to link. */
27085 if (r == OK)
27086 r = search_dir(ip, string, &rip->i_num, ENTER);
27087
27088 /* If success, register the linking. */
27089 if (r == OK) {
27090 rip->i_nlinks++;
27091 rip->i_update |= CTIME;
27092 rip->i_dirt = DIRTY;
27093 }
27094
27095 /* Done. Release both inodes. */
27096 put_inode(rip);
27097 put_inode(ip);
27098 return(r);
27099 }
27101 /*===========================================================================*
27102 * do_unlink *
27103 *===========================================================================*/
27104 PUBLIC int do_unlink()
27105 {
27106 /* Perform the unlink(name) or rmdir(name) system call. The code for these two
27107 * is almost the same. They differ only in some condition testing. Unlink()
27108 * may be used by the superuser to do dangerous things; rmdir() may not.
27109 */
27110
27111 register struct inode *rip;
27112 struct inode *rldirp;
27113 int r;
27114 char string[NAME_MAX];
27115
27116 /* Get the last directory in the path. */
27117 if (fetch_name(m_in.name, m_in.name_length, M3) != OK) return(err_code);
27118 if ( (rldirp = last_dir(user_path, string)) == NIL_INODE)
27119 return(err_code);
27120
27121 /* The last directory exists. Does the file also exist? */
27122 r = OK;
27123 if ( (rip = advance(rldirp, string)) == NIL_INODE) r = err_code;
27124
27125 /* If error, return inode. */
27126 if (r != OK) {
27127 put_inode(rldirp);
27128 return(r);
27129 }
1006 File: servers/fs/link.c MINIX SOURCE CODE
27130
27131 /* Do not remove a mount point. */
27132 if (rip->i_num == ROOT_INODE) {
27133 put_inode(rldirp);
27134 put_inode(rip);
27135 return(EBUSY);
27136 }
27137
27138 /* Now test if the call is allowed, separately for unlink() and rmdir(). */
27139 if (call_nr == UNLINK) {
27140 /* Only the su may unlink directories, but the su can unlink any dir.*/
27141 if ( (rip->i_mode & I_TYPE) == I_DIRECTORY && !super_user) r = EPERM;
27142
27143 /* Don’t unlink a file if it is the root of a mounted file system. */
27144 if (rip->i_num == ROOT_INODE) r = EBUSY;
27145
27146 /* Actually try to unlink the file; fails if parent is mode 0 etc. */
27147 if (r == OK) r = unlink_file(rldirp, rip, string);
27148
27149 } else {
27150 r = remove_dir(rldirp, rip, string); /* call is RMDIR */
27151 }
27152
27153 /* If unlink was possible, it has been done, otherwise it has not. */
27154 put_inode(rip);
27155 put_inode(rldirp);
27156 return(r);
27157 }
27159 /*===========================================================================*
27160 * do_rename *
27161 *===========================================================================*/
27162 PUBLIC int do_rename()
27163 {
27164 /* Perform the rename(name1, name2) system call. */
27165
27166 struct inode *old_dirp, *old_ip; /* ptrs to old dir, file inodes */
27167 struct inode *new_dirp, *new_ip; /* ptrs to new dir, file inodes */
27168 struct inode *new_superdirp, *next_new_superdirp;
27169 int r = OK; /* error flag; initially no error */
27170 int odir, ndir; /* TRUE iff {old|new} file is dir */
27171 int same_pdir; /* TRUE iff parent dirs are the same */
27172 char old_name[NAME_MAX], new_name[NAME_MAX];
27173 ino_t numb;
27174 int r1;
27175
27176 /* See if ’name1’ (existing file) exists. Get dir and file inodes. */
27177 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
27178 if ( (old_dirp = last_dir(user_path, old_name))==NIL_INODE) return(err_code);
27179
27180 if ( (old_ip = advance(old_dirp, old_name)) == NIL_INODE) r = err_code;
27181
27182 /* See if ’name2’ (new name) exists. Get dir and file inodes. */
27183 if (fetch_name(m_in.name2, m_in.name2_length, M1) != OK) r = err_code;
27184 if ( (new_dirp = last_dir(user_path, new_name)) == NIL_INODE) r = err_code;
27185 new_ip = advance(new_dirp, new_name); /* not required to exist */
27186
27187 if (old_ip != NIL_INODE)
27188 odir = ((old_ip->i_mode & I_TYPE) == I_DIRECTORY); /* TRUE iff dir */
27189
MINIX SOURCE CODE File: servers/fs/link.c 1007
27190 /* If it is ok, check for a variety of possible errors. */
27191 if (r == OK) {
27192 same_pdir = (old_dirp == new_dirp);
27193
27194 /* The old inode must not be a superdirectory of the new last dir. */
27195 if (odir && !same_pdir) {
27196 dup_inode(new_superdirp = new_dirp);
27197 while (TRUE) { /* may hang in a file system loop */
27198 if (new_superdirp == old_ip) {
27199 r = EINVAL;
27200 break;
27201 }
27202 next_new_superdirp = advance(new_superdirp, dot2);
27203 put_inode(new_superdirp);
27204 if (next_new_superdirp == new_superdirp)
27205 break; /* back at system root directory */
27206 new_superdirp = next_new_superdirp;
27207 if (new_superdirp == NIL_INODE) {
27208 /* Missing ".." entry. Assume the worst. */
27209 r = EINVAL;
27210 break;
27211 }
27212 }
27213 put_inode(new_superdirp);
27214 }
27215
27216 /* The old or new name must not be . or .. */
27217 if (strcmp(old_name, ".")==0 || strcmp(old_name, "..")==0 ||
27218 strcmp(new_name, ".")==0 || strcmp(new_name, "..")==0) r = EINVAL;
27219
27220 /* Both parent directories must be on the same device. */
27221 if (old_dirp->i_dev != new_dirp->i_dev) r = EXDEV;
27222
27223 /* Parent dirs must be writable, searchable and on a writable device */
27224 if ((r1 = forbidden(old_dirp, W_BIT | X_BIT)) != OK ||
27225 (r1 = forbidden(new_dirp, W_BIT | X_BIT)) != OK) r = r1;
27226
27227 /* Some tests apply only if the new path exists. */
27228 if (new_ip == NIL_INODE) {
27229 /* don’t rename a file with a file system mounted on it. */
27230 if (old_ip->i_dev != old_dirp->i_dev) r = EXDEV;
27231 if (odir && new_dirp->i_nlinks >=
27232 (new_dirp->i_sp->s_version == V1 ? CHAR_MAX : SHRT_MAX) &&
27233 !same_pdir && r == OK) r = EMLINK;
27234 } else {
27235 if (old_ip == new_ip) r = SAME; /* old=new */
27236
27237 /* has the old file or new file a file system mounted on it? */
27238 if (old_ip->i_dev != new_ip->i_dev) r = EXDEV;
27239
27240 ndir = ((new_ip->i_mode & I_TYPE) == I_DIRECTORY); /* dir ? */
27241 if (odir == TRUE && ndir == FALSE) r = ENOTDIR;
27242 if (odir == FALSE && ndir == TRUE) r = EISDIR;
27243 }
27244 }
27245
27246 /* If a process has another root directory than the system root, we might
27247 * "accidently" be moving it’s working directory to a place where it’s
27248 * root directory isn’t a super directory of it anymore. This can make
27249 * the function chroot useless. If chroot will be used often we should
1008 File: servers/fs/link.c MINIX SOURCE CODE
27250 * probably check for it here.
27251 */
27252
27253 /* The rename will probably work. Only two things can go wrong now:
27254 * 1. being unable to remove the new file. (when new file already exists)
27255 * 2. being unable to make the new directory entry. (new file doesn’t exists)
27256 * [directory has to grow by one block and cannot because the disk
27257 * is completely full].
27258 */
27259 if (r == OK) {
27260 if (new_ip != NIL_INODE) {
27261 /* There is already an entry for ’new’. Try to remove it. */
27262 if (odir)
27263 r = remove_dir(new_dirp, new_ip, new_name);
27264 else
27265 r = unlink_file(new_dirp, new_ip, new_name);
27266 }
27267 /* if r is OK, the rename will succeed, while there is now an
27268 * unused entry in the new parent directory.
27269 */
27270 }
27271
27272 if (r == OK) {
27273 /* If the new name will be in the same parent directory as the old one,
27274 * first remove the old name to free an entry for the new name,
27275 * otherwise first try to create the new name entry to make sure
27276 * the rename will succeed.
27277 */
27278 numb = old_ip->i_num; /* inode number of old file */
27279
27280 if (same_pdir) {
27281 r = search_dir(old_dirp, old_name, (ino_t *) 0, DELETE);
27282 /* shouldn’t go wrong. */
27283 if (r==OK) (void) search_dir(old_dirp, new_name, &numb, ENTER);
27284 } else {
27285 r = search_dir(new_dirp, new_name, &numb, ENTER);
27286 if (r == OK)
27287 (void) search_dir(old_dirp, old_name, (ino_t *) 0, DELETE);
27288 }
27289 }
27290 /* If r is OK, the ctime and mtime of old_dirp and new_dirp have been marked
27291 * for update in search_dir.
27292 */
27293
27294 if (r == OK && odir && !same_pdir) {
27295 /* Update the .. entry in the directory (still points to old_dirp). */
27296 numb = new_dirp->i_num;
27297 (void) unlink_file(old_ip, NIL_INODE, dot2);
27298 if (search_dir(old_ip, dot2, &numb, ENTER) == OK) {
27299 /* New link created. */
27300 new_dirp->i_nlinks++;
27301 new_dirp->i_dirt = DIRTY;
27302 }
27303 }
27304
27305 /* Release the inodes. */
27306 put_inode(old_dirp);
27307 put_inode(old_ip);
27308 put_inode(new_dirp);
27309 put_inode(new_ip);
MINIX SOURCE CODE File: servers/fs/link.c 1009
27310 return(r == SAME ? OK : r);
27311 }
27313 /*===========================================================================*
27314 * truncate *
27315 *===========================================================================*/
27316 PUBLIC void truncate(rip)
27317 register struct inode *rip; /* pointer to inode to be truncated */
27318 {
27319 /* Remove all the zones from the inode ’rip’ and mark it dirty. */
27320
27321 register block_t b;
27322 zone_t z, zone_size, z1;
27323 off_t position;
27324 int i, scale, file_type, waspipe, single, nr_indirects;
27325 struct buf *bp;
27326 dev_t dev;
27327
27328 file_type = rip->i_mode & I_TYPE; /* check to see if file is special */
27329 if (file_type == I_CHAR_SPECIAL || file_type == I_BLOCK_SPECIAL) return;
27330 dev = rip->i_dev; /* device on which inode resides */
27331 scale = rip->i_sp->s_log_zone_size;
27332 zone_size = (zone_t) rip->i_sp->s_block_size << scale;
27333 nr_indirects = rip->i_nindirs;
27334
27335 /* Pipes can shrink, so adjust size to make sure all zones are removed. */
27336 waspipe = rip->i_pipe == I_PIPE; /* TRUE is this was a pipe */
27337 if (waspipe) rip->i_size = PIPE_SIZE(rip->i_sp->s_block_size);
27338
27339 /* Step through the file a zone at a time, finding and freeing the zones. */
27340 for (position = 0; position < rip->i_size; position += zone_size) {
27341 if ( (b = read_map(rip, position)) != NO_BLOCK) {
27342 z = (zone_t) b >> scale;
27343 free_zone(dev, z);
27344 }
27345 }
27346
27347 /* All the data zones have been freed. Now free the indirect zones. */
27348 rip->i_dirt = DIRTY;
27349 if (waspipe) {
27350 wipe_inode(rip); /* clear out inode for pipes */
27351 return; /* indirect slots contain file positions */
27352 }
27353 single = rip->i_ndzones;
27354 free_zone(dev, rip->i_zone[single]); /* single indirect zone */
27355 if ( (z = rip->i_zone[single+1]) != NO_ZONE) {
27356 /* Free all the single indirect zones pointed to by the double. */
27357 b = (block_t) z << scale;
27358 bp = get_block(dev, b, NORMAL); /* get double indirect zone */
27359 for (i = 0; i < nr_indirects; i++) {
27360 z1 = rd_indir(bp, i);
27361 free_zone(dev, z1);
27362 }
27363
27364 /* Now free the double indirect zone itself. */
27365 put_block(bp, INDIRECT_BLOCK);
27366 free_zone(dev, z);
27367 }
27368
27369 /* Leave zone numbers for de(1) to recover file after an unlink(2). */
1010 File: servers/fs/link.c MINIX SOURCE CODE
27370 }
27372 /*===========================================================================*
27373 * remove_dir *
27374 *===========================================================================*/
27375 PRIVATE int remove_dir(rldirp, rip, dir_name)
27376 struct inode *rldirp; /* parent directory */
27377 struct inode *rip; /* directory to be removed */
27378 char dir_name[NAME_MAX]; /* name of directory to be removed */
27379 {
27380 /* A directory file has to be removed. Five conditions have to met:
27381 * - The file must be a directory
27382 * - The directory must be empty (except for . and ..)
27383 * - The final component of the path must not be . or ..
27384 * - The directory must not be the root of a mounted file system
27385 * - The directory must not be anybody’s root/working directory
27386 */
27387
27388 int r;
27389 register struct fproc *rfp;
27390
27391 /* search_dir checks that rip is a directory too. */
27392 if ((r = search_dir(rip, "", (ino_t *) 0, IS_EMPTY)) != OK) return r;
27393
27394 if (strcmp(dir_name, ".") == 0 || strcmp(dir_name, "..") == 0)return(EINVAL);
27395 if (rip->i_num == ROOT_INODE) return(EBUSY); /* can’t remove ’root’ */
27396
27397 for (rfp = &fproc[INIT_PROC_NR + 1]; rfp < &fproc[NR_PROCS]; rfp++)
27398 if (rfp->fp_workdir == rip || rfp->fp_rootdir == rip) return(EBUSY);
27399 /* can’t remove anybody’s working dir */
27400
27401 /* Actually try to unlink the file; fails if parent is mode 0 etc. */
27402 if ((r = unlink_file(rldirp, rip, dir_name)) != OK) return r;
27403
27404 /* Unlink . and .. from the dir. The super user can link and unlink any dir,
27405 * so don’t make too many assumptions about them.
27406 */
27407 (void) unlink_file(rip, NIL_INODE, dot1);
27408 (void) unlink_file(rip, NIL_INODE, dot2);
27409 return(OK);
27410 }
27412 /*===========================================================================*
27413 * unlink_file *
27414 *===========================================================================*/
27415 PRIVATE int unlink_file(dirp, rip, file_name)
27416 struct inode *dirp; /* parent directory of file */
27417 struct inode *rip; /* inode of file, may be NIL_INODE too. */
27418 char file_name[NAME_MAX]; /* name of file to be removed */
27419 {
27420 /* Unlink ’file_name’; rip must be the inode of ’file_name’ or NIL_INODE. */
27421
27422 ino_t numb; /* inode number */
27423 int r;
27424
27425 /* If rip is not NIL_INODE, it is used to get faster access to the inode. */
27426 if (rip == NIL_INODE) {
27427 /* Search for file in directory and try to get its inode. */
27428 err_code = search_dir(dirp, file_name, &numb, LOOK_UP);
27429 if (err_code == OK) rip = get_inode(dirp->i_dev, (int) numb);
MINIX SOURCE CODE File: servers/fs/link.c 1011
27430 if (err_code != OK || rip == NIL_INODE) return(err_code);
27431 } else {
27432 dup_inode(rip); /* inode will be returned with put_inode */
27433 }
27434
27435 r = search_dir(dirp, file_name, (ino_t *) 0, DELETE);
27436
27437 if (r == OK) {
27438 rip->i_nlinks--; /* entry deleted from parent’s dir */
27439 rip->i_update |= CTIME;
27440 rip->i_dirt = DIRTY;
27441 }
27442
27443 put_inode(rip);
27444 return(r);
27445 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/stadir.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
27500 /* This file contains the code for performing four system calls relating to
27501 * status and directories.
27502 *
27503 * The entry points into this file are
27504 * do_chdir: perform the CHDIR system call
27505 * do_chroot: perform the CHROOT system call
27506 * do_stat: perform the STAT system call
27507 * do_fstat: perform the FSTAT system call
27508 * do_fstatfs: perform the FSTATFS system call
27509 */
27510
27511 #include "fs.h"
27512 #include <sys/stat.h>
27513 #include <sys/statfs.h>
27514 #include <minix/com.h>
27515 #include "file.h"
27516 #include "fproc.h"
27517 #include "inode.h"
27518 #include "param.h"
27519 #include "super.h"
27520
27521 FORWARD _PROTOTYPE( int change, (struct inode **iip, char *name_ptr, int len));
27522 FORWARD _PROTOTYPE( int change_into, (struct inode **iip, struct inode *ip));
27523 FORWARD _PROTOTYPE( int stat_inode, (struct inode *rip, struct filp *fil_ptr,
27524 char *user_addr) );
27525
27526 /*===========================================================================*
27527 * do_fchdir *
27528 *===========================================================================*/
27529 PUBLIC int do_fchdir()
27530 {
27531 /* Change directory on already-opened fd. */
27532 struct filp *rfilp;
27533
27534 /* Is the file descriptor valid? */
1012 File: servers/fs/stadir.c MINIX SOURCE CODE
27535 if ( (rfilp = get_filp(m_in.fd)) == NIL_FILP) return(err_code);
27536 return change_into(&fp->fp_workdir, rfilp->filp_ino);
27537 }
27539 /*===========================================================================*
27540 * do_chdir *
27541 *===========================================================================*/
27542 PUBLIC int do_chdir()
27543 {
27544 /* Change directory. This function is also called by MM to simulate a chdir
27545 * in order to do EXEC, etc. It also changes the root directory, the uids and
27546 * gids, and the umask.
27547 */
27548
27549 int r;
27550 register struct fproc *rfp;
27551
27552 if (who == PM_PROC_NR) {
27553 rfp = &fproc[m_in.slot1];
27554 put_inode(fp->fp_rootdir);
27555 dup_inode(fp->fp_rootdir = rfp->fp_rootdir);
27556 put_inode(fp->fp_workdir);
27557 dup_inode(fp->fp_workdir = rfp->fp_workdir);
27558
27559 /* MM uses access() to check permissions. To make this work, pretend
27560 * that the user’s real ids are the same as the user’s effective ids.
27561 * FS calls other than access() do not use the real ids, so are not
27562 * affected.
27563 */
27564 fp->fp_realuid =
27565 fp->fp_effuid = rfp->fp_effuid;
27566 fp->fp_realgid =
27567 fp->fp_effgid = rfp->fp_effgid;
27568 fp->fp_umask = rfp->fp_umask;
27569 return(OK);
27570 }
27571
27572 /* Perform the chdir(name) system call. */
27573 r = change(&fp->fp_workdir, m_in.name, m_in.name_length);
27574 return(r);
27575 }
27577 /*===========================================================================*
27578 * do_chroot *
27579 *===========================================================================*/
27580 PUBLIC int do_chroot()
27581 {
27582 /* Perform the chroot(name) system call. */
27583
27584 register int r;
27585
27586 if (!super_user) return(EPERM); /* only su may chroot() */
27587 r = change(&fp->fp_rootdir, m_in.name, m_in.name_length);
27588 return(r);
27589 }
MINIX SOURCE CODE File: servers/fs/stadir.c 1013
27591 /*===========================================================================*
27592 * change *
27593 *===========================================================================*/
27594 PRIVATE int change(iip, name_ptr, len)
27595 struct inode **iip; /* pointer to the inode pointer for the dir */
27596 char *name_ptr; /* pointer to the directory name to change to */
27597 int len; /* length of the directory name string */
27598 {
27599 /* Do the actual work for chdir() and chroot(). */
27600 struct inode *rip;
27601
27602 /* Try to open the new directory. */
27603 if (fetch_name(name_ptr, len, M3) != OK) return(err_code);
27604 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
27605 return change_into(iip, rip);
27606 }
27608 /*===========================================================================*
27609 * change_into *
27610 *===========================================================================*/
27611 PRIVATE int change_into(iip, rip)
27612 struct inode **iip; /* pointer to the inode pointer for the dir */
27613 struct inode *rip; /* this is what the inode has to become */
27614 {
27615 register int r;
27616
27617 /* It must be a directory and also be searchable. */
27618 if ( (rip->i_mode & I_TYPE) != I_DIRECTORY)
27619 r = ENOTDIR;
27620 else
27621 r = forbidden(rip, X_BIT); /* check if dir is searchable */
27622
27623 /* If error, return inode. */
27624 if (r != OK) {
27625 put_inode(rip);
27626 return(r);
27627 }
27628
27629 /* Everything is OK. Make the change. */
27630 put_inode(*iip); /* release the old directory */
27631 *iip = rip; /* acquire the new one */
27632 return(OK);
27633 }
27635 /*===========================================================================*
27636 * do_stat *
27637 *===========================================================================*/
27638 PUBLIC int do_stat()
27639 {
27640 /* Perform the stat(name, buf) system call. */
27641
27642 register struct inode *rip;
27643 register int r;
27644
27645 /* Both stat() and fstat() use the same routine to do the real work. That
27646 * routine expects an inode, so acquire it temporarily.
27647 */
27648 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
27649 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
27650 r = stat_inode(rip, NIL_FILP, m_in.name2); /* actually do the work.*/
1014 File: servers/fs/stadir.c MINIX SOURCE CODE
27651 put_inode(rip); /* release the inode */
27652 return(r);
27653 }
27655 /*===========================================================================*
27656 * do_fstat *
27657 *===========================================================================*/
27658 PUBLIC int do_fstat()
27659 {
27660 /* Perform the fstat(fd, buf) system call. */
27661
27662 register struct filp *rfilp;
27663
27664 /* Is the file descriptor valid? */
27665 if ( (rfilp = get_filp(m_in.fd)) == NIL_FILP) return(err_code);
27666
27667 return(stat_inode(rfilp->filp_ino, rfilp, m_in.buffer));
27668 }
27670 /*===========================================================================*
27671 * stat_inode *
27672 *===========================================================================*/
27673 PRIVATE int stat_inode(rip, fil_ptr, user_addr)
27674 register struct inode *rip; /* pointer to inode to stat */
27675 struct filp *fil_ptr; /* filp pointer, supplied by ’fstat’ */
27676 char *user_addr; /* user space address where stat buf goes */
27677 {
27678 /* Common code for stat and fstat system calls. */
27679
27680 struct stat statbuf;
27681 mode_t mo;
27682 int r, s;
27683
27684 /* Update the atime, ctime, and mtime fields in the inode, if need be. */
27685 if (rip->i_update) update_times(rip);
27686
27687 /* Fill in the statbuf struct. */
27688 mo = rip->i_mode & I_TYPE;
27689
27690 /* true iff special */
27691 s = (mo == I_CHAR_SPECIAL || mo == I_BLOCK_SPECIAL);
27692
27693 statbuf.st_dev = rip->i_dev;
27694 statbuf.st_ino = rip->i_num;
27695 statbuf.st_mode = rip->i_mode;
27696 statbuf.st_nlink = rip->i_nlinks;
27697 statbuf.st_uid = rip->i_uid;
27698 statbuf.st_gid = rip->i_gid;
27699 statbuf.st_rdev = (dev_t) (s ? rip->i_zone[0] : NO_DEV);
27700 statbuf.st_size = rip->i_size;
27701
27702 if (rip->i_pipe == I_PIPE) {
27703 statbuf.st_mode &= ˜I_REGULAR; /* wipe out I_REGULAR bit for pipes */
27704 if (fil_ptr != NIL_FILP && fil_ptr->filp_mode & R_BIT)
27705 statbuf.st_size -= fil_ptr->filp_pos;
27706 }
27707
27708 statbuf.st_atime = rip->i_atime;
27709 statbuf.st_mtime = rip->i_mtime;
27710 statbuf.st_ctime = rip->i_ctime;
MINIX SOURCE CODE File: servers/fs/stadir.c 1015
27711
27712 /* Copy the struct to user space. */
27713 r = sys_datacopy(FS_PROC_NR, (vir_bytes) &statbuf,
27714 who, (vir_bytes) user_addr, (phys_bytes) sizeof(statbuf));
27715 return(r);
27716 }
27718 /*===========================================================================*
27719 * do_fstatfs *
27720 *===========================================================================*/
27721 PUBLIC int do_fstatfs()
27722 {
27723 /* Perform the fstatfs(fd, buf) system call. */
27724 struct statfs st;
27725 register struct filp *rfilp;
27726 int r;
27727
27728 /* Is the file descriptor valid? */
27729 if ( (rfilp = get_filp(m_in.fd)) == NIL_FILP) return(err_code);
27730
27731 st.f_bsize = rfilp->filp_ino->i_sp->s_block_size;
27732
27733 r = sys_datacopy(FS_PROC_NR, (vir_bytes) &st,
27734 who, (vir_bytes) m_in.buffer, (phys_bytes) sizeof(st));
27735
27736 return(r);
27737 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/protect.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
27800 /* This file deals with protection in the file system. It contains the code
27801 * for four system calls that relate to protection.
27802 *
27803 * The entry points into this file are
27804 * do_chmod: perform the CHMOD system call
27805 * do_chown: perform the CHOWN system call
27806 * do_umask: perform the UMASK system call
27807 * do_access: perform the ACCESS system call
27808 * forbidden: check to see if a given access is allowed on a given inode
27809 */
27810
27811 #include "fs.h"
27812 #include <unistd.h>
27813 #include <minix/callnr.h>
27814 #include "buf.h"
27815 #include "file.h"
27816 #include "fproc.h"
27817 #include "inode.h"
27818 #include "param.h"
27819 #include "super.h"
27820
1016 File: servers/fs/protect.c MINIX SOURCE CODE
27821 /*===========================================================================*
27822 * do_chmod *
27823 *===========================================================================*/
27824 PUBLIC int do_chmod()
27825 {
27826 /* Perform the chmod(name, mode) system call. */
27827
27828 register struct inode *rip;
27829 register int r;
27830
27831 /* Temporarily open the file. */
27832 if (fetch_name(m_in.name, m_in.name_length, M3) != OK) return(err_code);
27833 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
27834
27835 /* Only the owner or the super_user may change the mode of a file.
27836 * No one may change the mode of a file on a read-only file system.
27837 */
27838 if (rip->i_uid != fp->fp_effuid && !super_user)
27839 r = EPERM;
27840 else
27841 r = read_only(rip);
27842
27843 /* If error, return inode. */
27844 if (r != OK) {
27845 put_inode(rip);
27846 return(r);
27847 }
27848
27849 /* Now make the change. Clear setgid bit if file is not in caller’s grp */
27850 rip->i_mode = (rip->i_mode & ˜ALL_MODES) | (m_in.mode & ALL_MODES);
27851 if (!super_user && rip->i_gid != fp->fp_effgid)rip->i_mode &= ˜I_SET_GID_BIT;
27852 rip->i_update |= CTIME;
27853 rip->i_dirt = DIRTY;
27854
27855 put_inode(rip);
27856 return(OK);
27857 }
27859 /*===========================================================================*
27860 * do_chown *
27861 *===========================================================================*/
27862 PUBLIC int do_chown()
27863 {
27864 /* Perform the chown(name, owner, group) system call. */
27865
27866 register struct inode *rip;
27867 register int r;
27868
27869 /* Temporarily open the file. */
27870 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
27871 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
27872
27873 /* Not permitted to change the owner of a file on a read-only file sys. */
27874 r = read_only(rip);
27875 if (r == OK) {
27876 /* FS is R/W. Whether call is allowed depends on ownership, etc. */
27877 if (super_user) {
27878 /* The super user can do anything. */
27879 rip->i_uid = m_in.owner; /* others later */
27880 } else {
MINIX SOURCE CODE File: servers/fs/protect.c 1017
27881 /* Regular users can only change groups of their own files. */
27882 if (rip->i_uid != fp->fp_effuid) r = EPERM;
27883 if (rip->i_uid != m_in.owner) r = EPERM; /* no giving away */
27884 if (fp->fp_effgid != m_in.group) r = EPERM;
27885 }
27886 }
27887 if (r == OK) {
27888 rip->i_gid = m_in.group;
27889 rip->i_mode &= ˜(I_SET_UID_BIT | I_SET_GID_BIT);
27890 rip->i_update |= CTIME;
27891 rip->i_dirt = DIRTY;
27892 }
27893
27894 put_inode(rip);
27895 return(r);
27896 }
27898 /*===========================================================================*
27899 * do_umask *
27900 *===========================================================================*/
27901 PUBLIC int do_umask()
27902 {
27903 /* Perform the umask(co_mode) system call. */
27904 register mode_t r;
27905
27906 r = ˜fp->fp_umask; /* set ’r’ to complement of old mask */
27907 fp->fp_umask = ˜(m_in.co_mode & RWX_MODES);
27908 return(r); /* return complement of old mask */
27909 }
27911 /*===========================================================================*
27912 * do_access *
27913 *===========================================================================*/
27914 PUBLIC int do_access()
27915 {
27916 /* Perform the access(name, mode) system call. */
27917
27918 struct inode *rip;
27919 register int r;
27920
27921 /* First check to see if the mode is correct. */
27922 if ( (m_in.mode & ˜(R_OK | W_OK | X_OK)) != 0 && m_in.mode != F_OK)
27923 return(EINVAL);
27924
27925 /* Temporarily open the file whose access is to be checked. */
27926 if (fetch_name(m_in.name, m_in.name_length, M3) != OK) return(err_code);
27927 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
27928
27929 /* Now check the permissions. */
27930 r = forbidden(rip, (mode_t) m_in.mode);
27931 put_inode(rip);
27932 return(r);
27933 }
27935 /*===========================================================================*
27936 * forbidden *
27937 *===========================================================================*/
27938 PUBLIC int forbidden(register struct inode *rip, mode_t access_desired)
27939 {
27940 /* Given a pointer to an inode, ’rip’, and the access desired, determine
1018 File: servers/fs/protect.c MINIX SOURCE CODE
27941 * if the access is allowed, and if not why not. The routine looks up the
27942 * caller’s uid in the ’fproc’ table. If access is allowed, OK is returned
27943 * if it is forbidden, EACCES is returned.
27944 */
27945
27946 register struct inode *old_rip = rip;
27947 register struct super_block *sp;
27948 register mode_t bits, perm_bits;
27949 int r, shift, test_uid, test_gid, type;
27950
27951 if (rip->i_mount == I_MOUNT) /* The inode is mounted on. */
27952 for (sp = &super_block[1]; sp < &super_block[NR_SUPERS]; sp++)
27953 if (sp->s_imount == rip) {
27954 rip = get_inode(sp->s_dev, ROOT_INODE);
27955 break;
27956 } /* if */
27957
27958 /* Isolate the relevant rwx bits from the mode. */
27959 bits = rip->i_mode;
27960 test_uid = (call_nr == ACCESS ? fp->fp_realuid : fp->fp_effuid);
27961 test_gid = (call_nr == ACCESS ? fp->fp_realgid : fp->fp_effgid);
27962 if (test_uid == SU_UID) {
27963 /* Grant read and write permission. Grant search permission for
27964 * directories. Grant execute permission (for non-directories) if
27965 * and only if one of the ’X’ bits is set.
27966 */
27967 if ( (bits & I_TYPE) == I_DIRECTORY ||
27968 bits & ((X_BIT << 6) | (X_BIT << 3) | X_BIT))
27969 perm_bits = R_BIT | W_BIT | X_BIT;
27970 else
27971 perm_bits = R_BIT | W_BIT;
27972 } else {
27973 if (test_uid == rip->i_uid) shift = 6; /* owner */
27974 else if (test_gid == rip->i_gid ) shift = 3; /* group */
27975 else shift = 0; /* other */
27976 perm_bits = (bits >> shift) & (R_BIT | W_BIT | X_BIT);
27977 }
27978
27979 /* If access desired is not a subset of what is allowed, it is refused. */
27980 r = OK;
27981 if ((perm_bits | access_desired) != perm_bits) r = EACCES;
27982
27983 /* Check to see if someone is trying to write on a file system that is
27984 * mounted read-only.
27985 */
27986 type = rip->i_mode & I_TYPE;
27987 if (r == OK)
27988 if (access_desired & W_BIT)
27989 r = read_only(rip);
27990
27991 if (rip != old_rip) put_inode(rip);
27992
27993 return(r);
27994 }
27996 /*===========================================================================*
27997 * read_only *
27998 *===========================================================================*/
27999 PUBLIC int read_only(ip)
28000 struct inode *ip; /* ptr to inode whose file sys is to be cked */
MINIX SOURCE CODE File: servers/fs/protect.c 1019
28001 {
28002 /* Check to see if the file system on which the inode ’ip’ resides is mounted
28003 * read only. If so, return EROFS, else return OK.
28004 */
28005
28006 register struct super_block *sp;
28007
28008 sp = ip->i_sp;
28009 return(sp->s_rd_only ? EROFS : OK);
28010 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/dmap.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
28100 /* This file contains the table with device <-> driver mappings. It also
28101 * contains some routines to dynamically add and/ or remove device drivers
28102 * or change mappings.
28103 */
28104
28105 #include "fs.h"
28106 #include "fproc.h"
28107 #include <string.h>
28108 #include <stdlib.h>
28109 #include <ctype.h>
28110 #include <unistd.h>
28111 #include <minix/com.h>
28112 #include "param.h"
28113
28114 /* Some devices may or may not be there in the next table. */
28115 #define DT(enable, opcl, io, driver, flags) \
28116 { (enable?(opcl):no_dev), (enable?(io):0), \
28117 (enable?(driver):0), (flags) },
28118 #define NC(x) (NR_CTRLRS >= (x))
28119
28120 /* The order of the entries here determines the mapping between major device
28121 * numbers and tasks. The first entry (major device 0) is not used. The
28122 * next entry is major device 1, etc. Character and block devices can be
28123 * intermixed at random. The ordering determines the device numbers in /dev/.
28124 * Note that FS knows the device number of /dev/ram/ to load the RAM disk.
28125 * Also note that the major device numbers used in /dev/ are NOT the same as
28126 * the process numbers of the device drivers.
28127 */
28128 /*
28129 Driver enabled Open/Cls I/O Driver # Flags Device File
28130 -------------- -------- ------ ----------- ----- ------ ----
28131 */
28132 struct dmap dmap[NR_DEVICES]; /* actual map */
28133 PRIVATE struct dmap init_dmap[] = {
28134 DT(1, no_dev, 0, 0, 0) /* 0 = not used */
28135 DT(1, gen_opcl, gen_io, MEM_PROC_NR, 0) /* 1 = /dev/mem */
28136 DT(0, no_dev, 0, 0, DMAP_MUTABLE) /* 2 = /dev/fd0 */
28137 DT(0, no_dev, 0, 0, DMAP_MUTABLE) /* 3 = /dev/c0 */
28138 DT(1, tty_opcl, gen_io, TTY_PROC_NR, 0) /* 4 = /dev/tty00 */
28139 DT(1, ctty_opcl,ctty_io, TTY_PROC_NR, 0) /* 5 = /dev/tty */
28140 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /* 6 = /dev/lp */
28141 DT(1, no_dev, 0, 0, DMAP_MUTABLE) /* 7 = /dev/ip */
28142 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /* 8 = /dev/c1 */
28143 DT(0, 0, 0, 0, DMAP_MUTABLE) /* 9 = not used */
28144 DT(0, no_dev, 0, 0, DMAP_MUTABLE) /*10 = /dev/c2 */
1020 File: servers/fs/dmap.c MINIX SOURCE CODE
28145 DT(0, 0, 0, 0, DMAP_MUTABLE) /*11 = not used */
28146 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /*12 = /dev/c3 */
28147 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /*13 = /dev/audio */
28148 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /*14 = /dev/mixer */
28149 DT(1, gen_opcl, gen_io, LOG_PROC_NR, 0) /*15 = /dev/klog */
28150 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /*16 = /dev/random*/
28151 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /*17 = /dev/cmos */
28152 };
28153
28154 /*===========================================================================*
28155 * do_devctl *
28156 *===========================================================================*/
28157 PUBLIC int do_devctl()
28158 {
28159 int result;
28160
28161 switch(m_in.ctl_req) {
28162 case DEV_MAP:
28163 /* Try to update device mapping. */
28164 result = map_driver(m_in.dev_nr, m_in.driver_nr, m_in.dev_style);
28165 break;
28166 case DEV_UNMAP:
28167 result = ENOSYS;
28168 break;
28169 default:
28170 result = EINVAL;
28171 }
28172 return(result);
28173 }
28175 /*===========================================================================*
28176 * map_driver *
28177 *===========================================================================*/
28178 PUBLIC int map_driver(major, proc_nr, style)
28179 int major; /* major number of the device */
28180 int proc_nr; /* process number of the driver */
28181 int style; /* style of the device */
28182 {
28183 /* Set a new device driver mapping in the dmap table. Given that correct
28184 * arguments are given, this only works if the entry is mutable and the
28185 * current driver is not busy.
28186 * Normal error codes are returned so that this function can be used from
28187 * a system call that tries to dynamically install a new driver.
28188 */
28189 struct dmap *dp;
28190
28191 /* Get pointer to device entry in the dmap table. */
28192 if (major >= NR_DEVICES) return(ENODEV);
28193 dp = &dmap[major];
28194
28195 /* See if updating the entry is allowed. */
28196 if (! (dp->dmap_flags & DMAP_MUTABLE)) return(EPERM);
28197 if (dp->dmap_flags & DMAP_BUSY) return(EBUSY);
28198
28199 /* Check process number of new driver. */
28200 if (! isokprocnr(proc_nr)) return(EINVAL);
28201
28202 /* Try to update the entry. */
28203 switch (style) {
28204 case STYLE_DEV: dp->dmap_opcl = gen_opcl; break;
MINIX SOURCE CODE File: servers/fs/dmap.c 1021
28205 case STYLE_TTY: dp->dmap_opcl = tty_opcl; break;
28206 case STYLE_CLONE: dp->dmap_opcl = clone_opcl; break;
28207 default: return(EINVAL);
28208 }
28209 dp->dmap_io = gen_io;
28210 dp->dmap_driver = proc_nr;
28211 return(OK);
28212 }
28214 /*===========================================================================*
28215 * build_dmap *
28216 *===========================================================================*/
28217 PUBLIC void build_dmap()
28218 {
28219 /* Initialize the table with all device <-> driver mappings. Then, map
28220 * the boot driver to a controller and update the dmap table to that
28221 * selection. The boot driver and the controller it handles are set at
28222 * the boot monitor.
28223 */
28224 char driver[16];
28225 char *controller = "c##";
28226 int nr, major = -1;
28227 int i,s;
28228 struct dmap *dp;
28229
28230 /* Build table with device <-> driver mappings. */
28231 for (i=0; i<NR_DEVICES; i++) {
28232 dp = &dmap[i];
28233 if (i < sizeof(init_dmap)/sizeof(struct dmap) &&
28234 init_dmap[i].dmap_opcl != no_dev) { /* a preset driver */
28235 dp->dmap_opcl = init_dmap[i].dmap_opcl;
28236 dp->dmap_io = init_dmap[i].dmap_io;
28237 dp->dmap_driver = init_dmap[i].dmap_driver;
28238 dp->dmap_flags = init_dmap[i].dmap_flags;
28239 } else { /* no default */
28240 dp->dmap_opcl = no_dev;
28241 dp->dmap_io = 0;
28242 dp->dmap_driver = 0;
28243 dp->dmap_flags = DMAP_MUTABLE;
28244 }
28245 }
28246
28247 /* Get settings of ’controller’ and ’driver’ at the boot monitor. */
28248 if ((s = env_get_param("label", driver, sizeof(driver))) != OK)
28249 panic(__FILE__,"couldn’t get boot monitor parameter ’driver’", s);
28250 if ((s = env_get_param("controller", controller, sizeof(controller))) != OK)
28251 panic(__FILE__,"couldn’t get boot monitor parameter ’controller’", s);
28252
28253 /* Determine major number to map driver onto. */
28254 if (controller[0] == ’f’ && controller[1] == ’d’) {
28255 major = FLOPPY_MAJOR;
28256 }
28257 else if (controller[0] == ’c’ && isdigit(controller[1])) {
28258 if ((nr = (unsigned) atoi(&controller[1])) > NR_CTRLRS)
28259 panic(__FILE__,"monitor ’controller’ maximum ’c#’ is", NR_CTRLRS);
28260 major = CTRLR(nr);
28261 }
28262 else {
28263 panic(__FILE__,"monitor ’controller’ syntax is ’c#’ of ’fd’", NO_NUM);
28264 }
1022 File: servers/fs/dmap.c MINIX SOURCE CODE
28265
28266 /* Now try to set the actual mapping and report to the user. */
28267 if ((s=map_driver(major, DRVR_PROC_NR, STYLE_DEV)) != OK)
28268 panic(__FILE__,"map_driver failed",s);
28269 printf("Boot medium driver: %s driver mapped onto controller %s.\n",
28270 driver, controller);
28271 }
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
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
servers/fs/time.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
28800 /* This file takes care of those system calls that deal with time.
28801 *
28802 * The entry points into this file are
28803 * do_utime: perform the UTIME system call
28804 * do_stime: PM informs FS about STIME system call
28805 */
28806
28807 #include "fs.h"
28808 #include <minix/callnr.h>
28809 #include <minix/com.h>
1030 File: servers/fs/time.c MINIX SOURCE CODE
28810 #include "file.h"
28811 #include "fproc.h"
28812 #include "inode.h"
28813 #include "param.h"
28814
28815 /*===========================================================================*
28816 * do_utime *
28817 *===========================================================================*/
28818 PUBLIC int do_utime()
28819 {
28820 /* Perform the utime(name, timep) system call. */
28821
28822 register struct inode *rip;
28823 register int len, r;
28824
28825 /* Adjust for case of ’timep’ being NULL;
28826 * utime_strlen then holds the actual size: strlen(name)+1.
28827 */
28828 len = m_in.utime_length;
28829 if (len == 0) len = m_in.utime_strlen;
28830
28831 /* Temporarily open the file. */
28832 if (fetch_name(m_in.utime_file, len, M1) != OK) return(err_code);
28833 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
28834
28835 /* Only the owner of a file or the super_user can change its time. */
28836 r = OK;
28837 if (rip->i_uid != fp->fp_effuid && !super_user) r = EPERM;
28838 if (m_in.utime_length == 0 && r != OK) r = forbidden(rip, W_BIT);
28839 if (read_only(rip) != OK) r = EROFS; /* not even su can touch if R/O */
28840 if (r == OK) {
28841 if (m_in.utime_length == 0) {
28842 rip->i_atime = clock_time();
28843 rip->i_mtime = rip->i_atime;
28844 } else {
28845 rip->i_atime = m_in.utime_actime;
28846 rip->i_mtime = m_in.utime_modtime;
28847 }
28848 rip->i_update = CTIME; /* discard any stale ATIME and MTIME flags */
28849 rip->i_dirt = DIRTY;
28850 }
28851
28852 put_inode(rip);
28853 return(r);
28854 }
28856 /*===========================================================================*
28857 * do_stime *
28858 *===========================================================================*/
28859 PUBLIC int do_stime()
28860 {
28861 /* Perform the stime(tp) system call. */
28862 boottime = (long) m_in.pm_stime;
28863 return(OK);
28864 }