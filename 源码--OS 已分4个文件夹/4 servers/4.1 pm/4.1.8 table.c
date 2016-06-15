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