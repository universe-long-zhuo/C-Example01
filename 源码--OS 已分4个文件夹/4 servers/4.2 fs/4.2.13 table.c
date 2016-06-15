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