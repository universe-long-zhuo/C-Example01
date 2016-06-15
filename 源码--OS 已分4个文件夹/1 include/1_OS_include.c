blank
MINIX SOURCE CODE File: include/ansi.h 639
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             include/ansi.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
00000 /* The <ansi.h> header attempts to decide whether the compiler has enough
00001 * conformance to Standard C for Minix to take advantage of. If so, the
00002 * symbol _ANSI is defined (as 31459). Otherwise _ANSI is not defined
00003 * here, but it may be defined by applications that want to bend the rules.
00004 * The magic number in the definition is to inhibit unnecessary bending
00005 * of the rules. (For consistency with the new ’#ifdef _ANSI" tests in
00006 * the headers, _ANSI should really be defined as nothing, but that would
00007 * break many library routines that use "#if _ANSI".)
00008
00009 * If _ANSI ends up being defined, a macro
00010 *
00011 * _PROTOTYPE(function, params)
00012 *
00013 * is defined. This macro expands in different ways, generating either
00014 * ANSI Standard C prototypes or old-style K&R (Kernighan & Ritchie)
00015 * prototypes, as needed. Finally, some programs use _CONST, _VOIDSTAR etc
00016 * in such a way that they are portable over both ANSI and K&R compilers.
00017 * The appropriate macros are defined here.
00018 */
00019
00020 #ifndef _ANSI_H
00021 #define _ANSI_H
00022
00023 #if __STDC__ == 1
00024 #define _ANSI 31459 /* compiler claims full ANSI conformance */
00025 #endif
00026
00027 #ifdef __GNUC__
00028 #define _ANSI 31459 /* gcc conforms enough even in non-ANSI mode */
00029 #endif
00030
00031 #ifdef _ANSI
00032
00033 /* Keep everything for ANSI prototypes. */
00034 #define _PROTOTYPE(function, params) function params
00035 #define _ARGS(params) params
00036
00037 #define _VOIDSTAR void *
00038 #define _VOID void
00039 #define _CONST const
00040 #define _VOLATILE volatile
00041 #define _SIZET size_t
00042
00043 #else
00044
00045 /* Throw away the parameters for K&R prototypes. */
00046 #define _PROTOTYPE(function, params) function()
00047 #define _ARGS(params) ()
00048
00049 #define _VOIDSTAR void *
00050 #define _VOID void
00051 #define _CONST
00052 #define _VOLATILE
00053 #define _SIZET int
00054
640 File: include/ansi.h MINIX SOURCE CODE
00055 #endif /* _ANSI */
00056
00057 /* This should be defined as restrict when a C99 compiler is used. */
00058 #define _RESTRICT
00059
00060 /* Setting any of _MINIX, _POSIX_C_SOURCE or _POSIX2_SOURCE implies
00061 * _POSIX_SOURCE. (Seems wrong to put this here in ANSI space.)
00062 */
00063 #if defined(_MINIX) || _POSIX_C_SOURCE > 0 || defined(_POSIX2_SOURCE)
00064 #undef _POSIX_SOURCE
00065 #define _POSIX_SOURCE 1
00066 #endif
00067
00068 #endif /* ANSI_H */

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             include/limits.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
00100 /* The <limits.h> header defines some basic sizes, both of the language types
00101 * (e.g., the number of bits in an integer), and of the operating system (e.g.
00102 * the number of characters in a file name.
00103 */
00104
00105 #ifndef _LIMITS_H
00106 #define _LIMITS_H
00107
00108 /* Definitions about chars (8 bits in MINIX, and signed). */
00109 #define CHAR_BIT 8 /* # bits in a char */
00110 #define CHAR_MIN -128 /* minimum value of a char */
00111 #define CHAR_MAX 127 /* maximum value of a char */
00112 #define SCHAR_MIN -128 /* minimum value of a signed char */
00113 #define SCHAR_MAX 127 /* maximum value of a signed char */
00114 #define UCHAR_MAX 255 /* maximum value of an unsigned char */
00115 #define MB_LEN_MAX 1 /* maximum length of a multibyte char */
00116
00117 /* Definitions about shorts (16 bits in MINIX). */
00118 #define SHRT_MIN (-32767-1) /* minimum value of a short */
00119 #define SHRT_MAX 32767 /* maximum value of a short */
00120 #define USHRT_MAX 0xFFFF /* maximum value of unsigned short */
00121
00122 /* _EM_WSIZE is a compiler-generated symbol giving the word size in bytes. */
00123 #define INT_MIN (-2147483647-1) /* minimum value of a 32-bit int */
00124 #define INT_MAX 2147483647 /* maximum value of a 32-bit int */
00125 #define UINT_MAX 0xFFFFFFFF /* maximum value of an unsigned 32-bit int */
00126
00127 /*Definitions about longs (32 bits in MINIX). */
00128 #define LONG_MIN (-2147483647L-1)/* minimum value of a long */
00129 #define LONG_MAX 2147483647L /* maximum value of a long */
00130 #define ULONG_MAX 0xFFFFFFFFL /* maximum value of an unsigned long */
00131
00132 #include <sys/dir.h>
00133
00134 /* Minimum sizes required by the POSIX P1003.1 standard (Table 2-3). */
00135 #ifdef _POSIX_SOURCE /* these are only visible for POSIX */
00136 #define _POSIX_ARG_MAX 4096 /* exec() may have 4K worth of args */
00137 #define _POSIX_CHILD_MAX 6 /* a process may have 6 children */
00138 #define _POSIX_LINK_MAX 8 /* a file may have 8 links */
00139 #define _POSIX_MAX_CANON 255 /* size of the canonical input queue */
MINIX SOURCE CODE File: include/limits.h 641
00140 #define _POSIX_MAX_INPUT 255 /* you can type 255 chars ahead */
00141 #define _POSIX_NAME_MAX DIRSIZ /* a file name may have 14 chars */
00142 #define _POSIX_NGROUPS_MAX 0 /* supplementary group IDs are optional */
00143 #define _POSIX_OPEN_MAX 16 /* a process may have 16 files open */
00144 #define _POSIX_PATH_MAX 255 /* a pathname may contain 255 chars */
00145 #define _POSIX_PIPE_BUF 512 /* pipes writes of 512 bytes must be atomic */
00146 #define _POSIX_STREAM_MAX 8 /* at least 8 FILEs can be open at once */
00147 #define _POSIX_TZNAME_MAX 3 /* time zone names can be at least 3 chars */
00148 #define _POSIX_SSIZE_MAX 32767 /* read() must support 32767 byte reads */
00149
00150 /* Values actually implemented by MINIX (Tables 2-4, 2-5, 2-6, and 2-7). */
00151 /* Some of these old names had better be defined when not POSIX. */
00152 #define _NO_LIMIT 100 /* arbitrary number; limit not enforced */
00153
00154 #define NGROUPS_MAX 0 /* supplemental group IDs not available */
00155 #define ARG_MAX 16384 /* # bytes of args + environ for exec() */
00156 #define CHILD_MAX _NO_LIMIT /* MINIX does not limit children */
00157 #define OPEN_MAX 20 /* # open files a process may have */
00158 #define LINK_MAX SHRT_MAX /* # links a file may have */
00159 #define MAX_CANON 255 /* size of the canonical input queue */
00160 #define MAX_INPUT 255 /* size of the type-ahead buffer */
00161 #define NAME_MAX DIRSIZ /* # chars in a file name */
00162 #define PATH_MAX 255 /* # chars in a path name */
00163 #define PIPE_BUF 7168 /* # bytes in atomic write to a pipe */
00164 #define STREAM_MAX 20 /* must be the same as FOPEN_MAX in stdio.h */
00165 #define TZNAME_MAX 3 /* maximum bytes in a time zone name is 3 */
00166 #define SSIZE_MAX 32767 /* max defined byte count for read() */
00167
00168 #endif /* _POSIX_SOURCE */
00169
00170 #endif /* _LIMITS_H */

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             include/errno.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
00200 /* The <errno.h> header defines the numbers of the various errors that can
00201 * occur during program execution. They are visible to user programs and
00202 * should be small positive integers. However, they are also used within
00203 * MINIX, where they must be negative. For example, the READ system call is
00204 * executed internally by calling do_read(). This function returns either a
00205 * (negative) error number or a (positive) number of bytes actually read.
00206 *
00207 * To solve the problem of having the error numbers be negative inside the
00208 * the system and positive outside, the following mechanism is used. All the
00209 * definitions are are the form:
00210 *
00211 * #define EPERM (_SIGN 1)
00212 *
00213 * If the macro _SYSTEM is defined, then _SIGN is set to "-", otherwise it is
00214 * set to "". Thus when compiling the operating system, the macro _SYSTEM
00215 * will be defined, setting EPERM to (- 1), whereas when when this
00216 * file is included in an ordinary user program, EPERM has the value ( 1).
00217 */
00218
00219 #ifndef _ERRNO_H /* check if <errno.h> is already included */
642 File: include/errno.h MINIX SOURCE CODE
00220 #define _ERRNO_H /* it is not included; note that fact */
00221
00222 /* Now define _SIGN as "" or "-" depending on _SYSTEM. */
00223 #ifdef _SYSTEM
00224 # define _SIGN -
00225 # define OK 0
00226 #else
00227 # define _SIGN
00228 #endif
00229
00230 extern int errno; /* place where the error numbers go */
00231
00232 /* Here are the numerical values of the error numbers. */
00233 #define _NERROR 70 /* number of errors */
00234
00235 #define EGENERIC (_SIGN 99) /* generic error */
00236 #define EPERM (_SIGN 1) /* operation not permitted */
00237 #define ENOENT (_SIGN 2) /* no such file or directory */
00238 #define ESRCH (_SIGN 3) /* no such process */
00239 #define EINTR (_SIGN 4) /* interrupted function call */
00240 #define EIO (_SIGN 5) /* input/output error */
00241 #define ENXIO (_SIGN 6) /* no such device or address */
00242 #define E2BIG (_SIGN 7) /* arg list too long */
00243 #define ENOEXEC (_SIGN 8) /* exec format error */
00244 #define EBADF (_SIGN 9) /* bad file descriptor */
00245 #define ECHILD (_SIGN 10) /* no child process */
00246 #define EAGAIN (_SIGN 11) /* resource temporarily unavailable */
00247 #define ENOMEM (_SIGN 12) /* not enough space */
00248 #define EACCES (_SIGN 13) /* permission denied */
00249 #define EFAULT (_SIGN 14) /* bad address */
00250 #define ENOTBLK (_SIGN 15) /* Extension: not a block special file */
00251 #define EBUSY (_SIGN 16) /* resource busy */
00252 #define EEXIST (_SIGN 17) /* file exists */
00253 #define EXDEV (_SIGN 18) /* improper link */
00254 #define ENODEV (_SIGN 19) /* no such device */
00255 #define ENOTDIR (_SIGN 20) /* not a directory */
00256 #define EISDIR (_SIGN 21) /* is a directory */
00257 #define EINVAL (_SIGN 22) /* invalid argument */
00258 #define ENFILE (_SIGN 23) /* too many open files in system */
00259 #define EMFILE (_SIGN 24) /* too many open files */
00260 #define ENOTTY (_SIGN 25) /* inappropriate I/O control operation */
00261 #define ETXTBSY (_SIGN 26) /* no longer used */
00262 #define EFBIG (_SIGN 27) /* file too large */
00263 #define ENOSPC (_SIGN 28) /* no space left on device */
00264 #define ESPIPE (_SIGN 29) /* invalid seek */
00265 #define EROFS (_SIGN 30) /* read-only file system */
00266 #define EMLINK (_SIGN 31) /* too many links */
00267 #define EPIPE (_SIGN 32) /* broken pipe */
00268 #define EDOM (_SIGN 33) /* domain error (from ANSI C std) */
00269 #define ERANGE (_SIGN 34) /* result too large (from ANSI C std) */
00270 #define EDEADLK (_SIGN 35) /* resource deadlock avoided */
00271 #define ENAMETOOLONG (_SIGN 36) /* file name too long */
00272 #define ENOLCK (_SIGN 37) /* no locks available */
00273 #define ENOSYS (_SIGN 38) /* function not implemented */
00274 #define ENOTEMPTY (_SIGN 39) /* directory not empty */
00275
00276 /* The following errors relate to networking. */
00277 #define EPACKSIZE (_SIGN 50) /* invalid packet size for some protocol */
00278 #define EOUTOFBUFS (_SIGN 51) /* not enough buffers left */
00279 #define EBADIOCTL (_SIGN 52) /* illegal ioctl for device */
MINIX SOURCE CODE File: include/errno.h 643
00280 #define EBADMODE (_SIGN 53) /* badmode in ioctl */
00281 #define EWOULDBLOCK (_SIGN 54)
00282 #define EBADDEST (_SIGN 55) /* not a valid destination address */
00283 #define EDSTNOTRCH (_SIGN 56) /* destination not reachable */
00284 #define EISCONN (_SIGN 57) /* all ready connected */
00285 #define EADDRINUSE (_SIGN 58) /* address in use */
00286 #define ECONNREFUSED (_SIGN 59) /* connection refused */
00287 #define ECONNRESET (_SIGN 60) /* connection reset */
00288 #define ETIMEDOUT (_SIGN 61) /* connection timed out */
00289 #define EURG (_SIGN 62) /* urgent data present */
00290 #define ENOURG (_SIGN 63) /* no urgent data present */
00291 #define ENOTCONN (_SIGN 64) /* no connection (yet or anymore) */
00292 #define ESHUTDOWN (_SIGN 65) /* a write call to a shutdown connection */
00293 #define ENOCONN (_SIGN 66) /* no such connection */
00294 #define EAFNOSUPPORT (_SIGN 67) /* address family not supported */
00295 #define EPROTONOSUPPORT (_SIGN 68) /* protocol not supported by AF */
00296 #define EPROTOTYPE (_SIGN 69) /* Protocol wrong type for socket */
00297 #define EINPROGRESS (_SIGN 70) /* Operation now in progress */
00298 #define EADDRNOTAVAIL (_SIGN 71) /* Can’t assign requested address */
00299 #define EALREADY (_SIGN 72) /* Connection already in progress */
00300 #define EMSGSIZE (_SIGN 73) /* Message too long */
00301
00302 /* The following are not POSIX errors, but they can still happen.
00303 * All of these are generated by the kernel and relate to message passing.
00304 */
00305 #define ELOCKED (_SIGN 101) /* can’t send message due to deadlock */
00306 #define EBADCALL (_SIGN 102) /* illegal system call number */
00307 #define EBADSRCDST (_SIGN 103) /* bad source or destination process */
00308 #define ECALLDENIED (_SIGN 104) /* no permission for system call */
00309 #define EDEADDST (_SIGN 105) /* send destination is not alive */
00310 #define ENOTREADY (_SIGN 106) /* source or destination is not ready */
00311 #define EBADREQUEST (_SIGN 107) /* destination cannot handle request */
00312 #define EDONTREPLY (_SIGN 201) /* pseudo-code: don’t send a reply */
00313
00314 #endif /* _ERRNO_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/unistd.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
00400 /* The <unistd.h> header contains a few miscellaneous manifest constants. */
00401
00402 #ifndef _UNISTD_H
00403 #define _UNISTD_H
00404
00405 #ifndef _TYPES_H
00406 #include <sys/types.h>
00407 #endif
00408
00409 /* Values used by access(). POSIX Table 2-8. */
00410 #define F_OK 0 /* test if file exists */
00411 #define X_OK 1 /* test if file is executable */
00412 #define W_OK 2 /* test if file is writable */
00413 #define R_OK 4 /* test if file is readable */
00414
00415 /* Values used for whence in lseek(fd, offset, whence). POSIX Table 2-9. */
00416 #define SEEK_SET 0 /* offset is absolute */
00417 #define SEEK_CUR 1 /* offset is relative to current position */
00418 #define SEEK_END 2 /* offset is relative to end of file */
00419
644 File: include/unistd.h MINIX SOURCE CODE
00420 /* This value is required by POSIX Table 2-10. */
00421 #define _POSIX_VERSION 199009L /* which standard is being conformed to */
00422
00423 /* These three definitions are required by POSIX Sec. 8.2.1.2. */
00424 #define STDIN_FILENO 0 /* file descriptor for stdin */
00425 #define STDOUT_FILENO 1 /* file descriptor for stdout */
00426 #define STDERR_FILENO 2 /* file descriptor for stderr */
00427
00428 #ifdef _MINIX
00429 /* How to exit the system or stop a server process. */
00430 #define RBT_HALT 0
00431 #define RBT_REBOOT 1
00432 #define RBT_PANIC 2 /* a server panics */
00433 #define RBT_MONITOR 3 /* let the monitor do this */
00434 #define RBT_RESET 4 /* hard reset the system */
00435 #endif
00436
00437 /* What system info to retrieve with sysgetinfo(). */
00438 #define SI_KINFO 0 /* get kernel info via PM */
00439 #define SI_PROC_ADDR 1 /* address of process table */
00440 #define SI_PROC_TAB 2 /* copy of entire process table */
00441 #define SI_DMAP_TAB 3 /* get device <-> driver mappings */
00442
00443 /* NULL must be defined in <unistd.h> according to POSIX Sec. 2.7.1. */
00444 #define NULL ((void *)0)
00445
00446 /* The following relate to configurable system variables. POSIX Table 4-2. */
00447 #define _SC_ARG_MAX 1
00448 #define _SC_CHILD_MAX 2
00449 #define _SC_CLOCKS_PER_SEC 3
00450 #define _SC_CLK_TCK 3
00451 #define _SC_NGROUPS_MAX 4
00452 #define _SC_OPEN_MAX 5
00453 #define _SC_JOB_CONTROL 6
00454 #define _SC_SAVED_IDS 7
00455 #define _SC_VERSION 8
00456 #define _SC_STREAM_MAX 9
00457 #define _SC_TZNAME_MAX 10
00458
00459 /* The following relate to configurable pathname variables. POSIX Table 5-2. */
00460 #define _PC_LINK_MAX 1 /* link count */
00461 #define _PC_MAX_CANON 2 /* size of the canonical input queue */
00462 #define _PC_MAX_INPUT 3 /* type-ahead buffer size */
00463 #define _PC_NAME_MAX 4 /* file name size */
00464 #define _PC_PATH_MAX 5 /* pathname size */
00465 #define _PC_PIPE_BUF 6 /* pipe size */
00466 #define _PC_NO_TRUNC 7 /* treatment of long name components */
00467 #define _PC_VDISABLE 8 /* tty disable */
00468 #define _PC_CHOWN_RESTRICTED 9 /* chown restricted or not */
00469
00470 /* POSIX defines several options that may be implemented or not, at the
00471 * implementer’s whim. This implementer has made the following choices:
00472 *
00473 * _POSIX_JOB_CONTROL not defined: no job control
00474 * _POSIX_SAVED_IDS not defined: no saved uid/gid
00475 * _POSIX_NO_TRUNC defined as -1: long path names are truncated
00476 * _POSIX_CHOWN_RESTRICTED defined: you can’t give away files
00477 * _POSIX_VDISABLE defined: tty functions can be disabled
00478 */
00479 #define _POSIX_NO_TRUNC (-1)
MINIX SOURCE CODE File: include/unistd.h 645
00480 #define _POSIX_CHOWN_RESTRICTED 1
00481
00482 /* Function Prototypes. */
00483 _PROTOTYPE( void _exit, (int _status) );
00484 _PROTOTYPE( int access, (const char *_path, int _amode) );
00485 _PROTOTYPE( unsigned int alarm, (unsigned int _seconds) );
00486 _PROTOTYPE( int chdir, (const char *_path) );
00487 _PROTOTYPE( int fchdir, (int fd) );
00488 _PROTOTYPE( int chown, (const char *_path, _mnx_Uid_t _owner, _mnx_Gid_t _group) );
00489 _PROTOTYPE( int close, (int _fd) );
00490 _PROTOTYPE( char *ctermid, (char *_s) );
00491 _PROTOTYPE( char *cuserid, (char *_s) );
00492 _PROTOTYPE( int dup, (int _fd) );
00493 _PROTOTYPE( int dup2, (int _fd, int _fd2) );
00494 _PROTOTYPE( int execl, (const char *_path, const char *_arg, ...) );
00495 _PROTOTYPE( int execle, (const char *_path, const char *_arg, ...) );
00496 _PROTOTYPE( int execlp, (const char *_file, const char *arg, ...) );
00497 _PROTOTYPE( int execv, (const char *_path, char *const _argv[]) );
00498 _PROTOTYPE( int execve, (const char *_path, char *const _argv[],
00499 char *const _envp[]) );
00500 _PROTOTYPE( int execvp, (const char *_file, char *const _argv[]) );
00501 _PROTOTYPE( pid_t fork, (void) );
00502 _PROTOTYPE( long fpathconf, (int _fd, int _name) );
00503 _PROTOTYPE( char *getcwd, (char *_buf, size_t _size) );
00504 _PROTOTYPE( gid_t getegid, (void) );
00505 _PROTOTYPE( uid_t geteuid, (void) );
00506 _PROTOTYPE( gid_t getgid, (void) );
00507 _PROTOTYPE( int getgroups, (int _gidsetsize, gid_t _grouplist[]) );
00508 _PROTOTYPE( char *getlogin, (void) );
00509 _PROTOTYPE( pid_t getpgrp, (void) );
00510 _PROTOTYPE( pid_t getpid, (void) );
00511 _PROTOTYPE( pid_t getppid, (void) );
00512 _PROTOTYPE( uid_t getuid, (void) );
00513 _PROTOTYPE( int isatty, (int _fd) );
00514 _PROTOTYPE( int link, (const char *_existing, const char *_new) );
00515 _PROTOTYPE( off_t lseek, (int _fd, off_t _offset, int _whence) );
00516 _PROTOTYPE( long pathconf, (const char *_path, int _name) );
00517 _PROTOTYPE( int pause, (void) );
00518 _PROTOTYPE( int pipe, (int _fildes[2]) );
00519 _PROTOTYPE( ssize_t read, (int _fd, void *_buf, size_t _n) );
00520 _PROTOTYPE( int rmdir, (const char *_path) );
00521 _PROTOTYPE( int setgid, (_mnx_Gid_t _gid) );
00522 _PROTOTYPE( int setpgid, (pid_t _pid, pid_t _pgid) );
00523 _PROTOTYPE( pid_t setsid, (void) );
00524 _PROTOTYPE( int setuid, (_mnx_Uid_t _uid) );
00525 _PROTOTYPE( unsigned int sleep, (unsigned int _seconds) );
00526 _PROTOTYPE( long sysconf, (int _name) );
00527 _PROTOTYPE( pid_t tcgetpgrp, (int _fd) );
00528 _PROTOTYPE( int tcsetpgrp, (int _fd, pid_t _pgrp_id) );
00529 _PROTOTYPE( char *ttyname, (int _fd) );
00530 _PROTOTYPE( int unlink, (const char *_path) );
00531 _PROTOTYPE( ssize_t write, (int _fd, const void *_buf, size_t _n) );
00532
00533 /* Open Group Base Specifications Issue 6 (not complete) */
00534 _PROTOTYPE( int symlink, (const char *path1, const char *path2) );
00535 _PROTOTYPE( int getopt, (int _argc, char **_argv, char *_opts) );
00536 extern char *optarg;
00537 extern int optind, opterr, optopt;
00538 _PROTOTYPE( int usleep, (useconds_t _useconds) );
00539
646 File: include/unistd.h MINIX SOURCE CODE
00540 #ifdef _MINIX
00541 #ifndef _TYPE_H
00542 #include <minix/type.h>
00543 #endif
00544 _PROTOTYPE( int brk, (char *_addr) );
00545 _PROTOTYPE( int chroot, (const char *_name) );
00546 _PROTOTYPE( int mknod, (const char *_name, _mnx_Mode_t _mode, Dev_t _addr) );
00547 _PROTOTYPE( int mknod4, (const char *_name, _mnx_Mode_t _mode, Dev_t _addr,
00548 long _size) );
00549 _PROTOTYPE( char *mktemp, (char *_template) );
00550 _PROTOTYPE( int mount, (char *_spec, char *_name, int _flag) );
00551 _PROTOTYPE( long ptrace, (int _req, pid_t _pid, long _addr, long _data) );
00552 _PROTOTYPE( char *sbrk, (int _incr) );
00553 _PROTOTYPE( int sync, (void) );
00554 _PROTOTYPE( int fsync, (int fd) );
00555 _PROTOTYPE( int umount, (const char *_name) );
00556 _PROTOTYPE( int reboot, (int _how, ...) );
00557 _PROTOTYPE( int gethostname, (char *_hostname, size_t _len) );
00558 _PROTOTYPE( int getdomainname, (char *_domain, size_t _len) );
00559 _PROTOTYPE( int ttyslot, (void) );
00560 _PROTOTYPE( int fttyslot, (int _fd) );
00561 _PROTOTYPE( char *crypt, (const char *_key, const char *_salt) );
00562 _PROTOTYPE( int getsysinfo, (int who, int what, void *where) );
00563 _PROTOTYPE( int getprocnr, (void) );
00564 _PROTOTYPE( int findproc, (char *proc_name, int *proc_nr) );
00565 _PROTOTYPE( int allocmem, (phys_bytes size, phys_bytes *base) );
00566 _PROTOTYPE( int freemem, (phys_bytes size, phys_bytes base) );
00567 #define DEV_MAP 1
00568 #define DEV_UNMAP 2
00569 #define mapdriver(driver, device, style) devctl(DEV_MAP, driver, device, style)
00570 #define unmapdriver(device) devctl(DEV_UNMAP, 0, device, 0)
00571 _PROTOTYPE( int devctl, (int ctl_req, int driver, int device, int style));
00572
00573 /* For compatibility with other Unix systems */
00574 _PROTOTYPE( int getpagesize, (void) );
00575 _PROTOTYPE( int setgroups, (int ngroups, const gid_t *gidset) );
00576
00577 #endif
00578
00579 _PROTOTYPE( int readlink, (const char *, char *, int));
00580 _PROTOTYPE( int getopt, (int, char **, char *));
00581 extern int optind, opterr, optopt;
00582
00583 #endif /* _UNISTD_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/string.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
00600 /* The <string.h> header contains prototypes for the string handling
00601 * functions.
00602 */
00603
00604 #ifndef _STRING_H
00605 #define _STRING_H
00606
00607 #define NULL ((void *)0)
00608
00609 #ifndef _SIZE_T
MINIX SOURCE CODE File: include/string.h 647
00610 #define _SIZE_T
00611 typedef unsigned int size_t; /* type returned by sizeof */
00612 #endif /*_SIZE_T */
00613
00614 /* Function Prototypes. */
00615 #ifndef _ANSI_H
00616 #include <ansi.h>
00617 #endif
00618
00619 _PROTOTYPE( void *memchr, (const void *_s, int _c, size_t _n) );
00620 _PROTOTYPE( int memcmp, (const void *_s1, const void *_s2, size_t _n) );
00621 _PROTOTYPE( void *memcpy, (void *_s1, const void *_s2, size_t _n) );
00622 _PROTOTYPE( void *memmove, (void *_s1, const void *_s2, size_t _n) );
00623 _PROTOTYPE( void *memset, (void *_s, int _c, size_t _n) );
00624 _PROTOTYPE( char *strcat, (char *_s1, const char *_s2) );
00625 _PROTOTYPE( char *strchr, (const char *_s, int _c) );
00626 _PROTOTYPE( int strncmp, (const char *_s1, const char *_s2, size_t _n) );
00627 _PROTOTYPE( int strcmp, (const char *_s1, const char *_s2) );
00628 _PROTOTYPE( int strcoll, (const char *_s1, const char *_s2) );
00629 _PROTOTYPE( char *strcpy, (char *_s1, const char *_s2) );
00630 _PROTOTYPE( size_t strcspn, (const char *_s1, const char *_s2) );
00631 _PROTOTYPE( char *strerror, (int _errnum) );
00632 _PROTOTYPE( size_t strlen, (const char *_s) );
00633 _PROTOTYPE( char *strncat, (char *_s1, const char *_s2, size_t _n) );
00634 _PROTOTYPE( char *strncpy, (char *_s1, const char *_s2, size_t _n) );
00635 _PROTOTYPE( char *strpbrk, (const char *_s1, const char *_s2) );
00636 _PROTOTYPE( char *strrchr, (const char *_s, int _c) );
00637 _PROTOTYPE( size_t strspn, (const char *_s1, const char *_s2) );
00638 _PROTOTYPE( char *strstr, (const char *_s1, const char *_s2) );
00639 _PROTOTYPE( char *strtok, (char *_s1, const char *_s2) );
00640 _PROTOTYPE( size_t strxfrm, (char *_s1, const char *_s2, size_t _n) );
00641
00642 #ifdef _POSIX_SOURCE
00643 /* Open Group Base Specifications Issue 6 (not complete) */
00644 char *strdup(const char *_s1);
00645 #endif
00646
00647 #ifdef _MINIX
00648 /* For backward compatibility. */
00649 _PROTOTYPE( char *index, (const char *_s, int _charwanted) );
00650 _PROTOTYPE( char *rindex, (const char *_s, int _charwanted) );
00651 _PROTOTYPE( void bcopy, (const void *_src, void *_dst, size_t _length) );
00652 _PROTOTYPE( int bcmp, (const void *_s1, const void *_s2, size_t _length));
00653 _PROTOTYPE( void bzero, (void *_dst, size_t _length) );
00654 _PROTOTYPE( void *memccpy, (char *_dst, const char *_src, int _ucharstop,
00655 size_t _size) );
00656
00657 /* Misc. extra functions */
00658 _PROTOTYPE( int strcasecmp, (const char *_s1, const char *_s2) );
00659 _PROTOTYPE( int strncasecmp, (const char *_s1, const char *_s2,
00660 size_t _len) );
00661 _PROTOTYPE( size_t strnlen, (const char *_s, size_t _n) );
00662 #endif
00663
00664 #endif /* _STRING_H */
648 File: include/signal.h MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/signal.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
00700 /* The <signal.h> header defines all the ANSI and POSIX signals.
00701 * MINIX supports all the signals required by POSIX. They are defined below.
00702 * Some additional signals are also supported.
00703 */
00704
00705 #ifndef _SIGNAL_H
00706 #define _SIGNAL_H
00707
00708 #ifndef _ANSI_H
00709 #include <ansi.h>
00710 #endif
00711 #ifdef _POSIX_SOURCE
00712 #ifndef _TYPES_H
00713 #include <sys/types.h>
00714 #endif
00715 #endif
00716
00717 /* Here are types that are closely associated with signal handling. */
00718 typedef int sig_atomic_t;
00719
00720 #ifdef _POSIX_SOURCE
00721 #ifndef _SIGSET_T
00722 #define _SIGSET_T
00723 typedef unsigned long sigset_t;
00724 #endif
00725 #endif
00726
00727 #define _NSIG 20 /* number of signals used */
00728
00729 #define SIGHUP 1 /* hangup */
00730 #define SIGINT 2 /* interrupt (DEL) */
00731 #define SIGQUIT 3 /* quit (ASCII FS) */
00732 #define SIGILL 4 /* illegal instruction */
00733 #define SIGTRAP 5 /* trace trap (not reset when caught) */
00734 #define SIGABRT 6 /* IOT instruction */
00735 #define SIGIOT 6 /* SIGABRT for people who speak PDP-11 */
00736 #define SIGUNUSED 7 /* spare code */
00737 #define SIGFPE 8 /* floating point exception */
00738 #define SIGKILL 9 /* kill (cannot be caught or ignored) */
00739 #define SIGUSR1 10 /* user defined signal # 1 */
00740 #define SIGSEGV 11 /* segmentation violation */
00741 #define SIGUSR2 12 /* user defined signal # 2 */
00742 #define SIGPIPE 13 /* write on a pipe with no one to read it */
00743 #define SIGALRM 14 /* alarm clock */
00744 #define SIGTERM 15 /* software termination signal from kill */
00745 #define SIGCHLD 17 /* child process terminated or stopped */
00746
00747 #define SIGEMT 7 /* obsolete */
00748 #define SIGBUS 10 /* obsolete */
00749
00750 /* MINIX specific signals. These signals are not used by user proceses,
00751 * but meant to inform system processes, like the PM, about system events.
00752 */
00753 #define SIGKMESS 18 /* new kernel message */
00754 #define SIGKSIG 19 /* kernel signal pending */
MINIX SOURCE CODE File: include/signal.h 649
00755 #define SIGKSTOP 20 /* kernel shutting down */
00756
00757 /* POSIX requires the following signals to be defined, even if they are
00758 * not supported. Here are the definitions, but they are not supported.
00759 */
00760 #define SIGCONT 18 /* continue if stopped */
00761 #define SIGSTOP 19 /* stop signal */
00762 #define SIGTSTP 20 /* interactive stop signal */
00763 #define SIGTTIN 21 /* background process wants to read */
00764 #define SIGTTOU 22 /* background process wants to write */
00765
00766 /* The sighandler_t type is not allowed unless _POSIX_SOURCE is defined. */
00767 typedef void _PROTOTYPE( (*__sighandler_t), (int) );
00768
00769 /* Macros used as function pointers. */
00770 #define SIG_ERR ((__sighandler_t) -1) /* error return */
00771 #define SIG_DFL ((__sighandler_t) 0) /* default signal handling */
00772 #define SIG_IGN ((__sighandler_t) 1) /* ignore signal */
00773 #define SIG_HOLD ((__sighandler_t) 2) /* block signal */
00774 #define SIG_CATCH ((__sighandler_t) 3) /* catch signal */
00775 #define SIG_MESS ((__sighandler_t) 4) /* pass as message (MINIX) */
00776
00777 #ifdef _POSIX_SOURCE
00778 struct sigaction {
00779 __sighandler_t sa_handler; /* SIG_DFL, SIG_IGN, or pointer to function */
00780 sigset_t sa_mask; /* signals to be blocked during handler */
00781 int sa_flags; /* special flags */
00782 };
00783
00784 /* Fields for sa_flags. */
00785 #define SA_ONSTACK 0x0001 /* deliver signal on alternate stack */
00786 #define SA_RESETHAND 0x0002 /* reset signal handler when signal caught */
00787 #define SA_NODEFER 0x0004 /* don’t block signal while catching it */
00788 #define SA_RESTART 0x0008 /* automatic system call restart */
00789 #define SA_SIGINFO 0x0010 /* extended signal handling */
00790 #define SA_NOCLDWAIT 0x0020 /* don’t create zombies */
00791 #define SA_NOCLDSTOP 0x0040 /* don’t receive SIGCHLD when child stops */
00792
00793 /* POSIX requires these values for use with sigprocmask(2). */
00794 #define SIG_BLOCK 0 /* for blocking signals */
00795 #define SIG_UNBLOCK 1 /* for unblocking signals */
00796 #define SIG_SETMASK 2 /* for setting the signal mask */
00797 #define SIG_INQUIRE 4 /* for internal use only */
00798 #endif /* _POSIX_SOURCE */
00799
00800 /* POSIX and ANSI function prototypes. */
00801 _PROTOTYPE( int raise, (int _sig) );
00802 _PROTOTYPE( __sighandler_t signal, (int _sig, __sighandler_t _func) );
00803
00804 #ifdef _POSIX_SOURCE
00805 _PROTOTYPE( int kill, (pid_t _pid, int _sig) );
00806 _PROTOTYPE( int sigaction,
00807 (int _sig, const struct sigaction *_act, struct sigaction *_oact) );
00808 _PROTOTYPE( int sigaddset, (sigset_t *_set, int _sig) );
00809 _PROTOTYPE( int sigdelset, (sigset_t *_set, int _sig) );
00810 _PROTOTYPE( int sigemptyset, (sigset_t *_set) );
00811 _PROTOTYPE( int sigfillset, (sigset_t *_set) );
00812 _PROTOTYPE( int sigismember, (const sigset_t *_set, int _sig) );
00813 _PROTOTYPE( int sigpending, (sigset_t *_set) );
00814 _PROTOTYPE( int sigprocmask,
650 File: include/signal.h MINIX SOURCE CODE
00815 (int _how, const sigset_t *_set, sigset_t *_oset) );
00816 _PROTOTYPE( int sigsuspend, (const sigset_t *_sigmask) );
00817 #endif
00818
00819 #endif /* _SIGNAL_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/fcntl.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
00900 /* The <fcntl.h> header is needed by the open() and fcntl() system calls,
00901 * which have a variety of parameters and flags. They are described here.
00902 * The formats of the calls to each of these are:
00903 *
00904 * open(path, oflag [,mode]) open a file
00905 * fcntl(fd, cmd [,arg]) get or set file attributes
00906 *
00907 */
00908
00909 #ifndef _FCNTL_H
00910 #define _FCNTL_H
00911
00912 #ifndef _TYPES_H
00913 #include <sys/types.h>
00914 #endif
00915
00916 /* These values are used for cmd in fcntl(). POSIX Table 6-1. */
00917 #define F_DUPFD 0 /* duplicate file descriptor */
00918 #define F_GETFD 1 /* get file descriptor flags */
00919 #define F_SETFD 2 /* set file descriptor flags */
00920 #define F_GETFL 3 /* get file status flags */
00921 #define F_SETFL 4 /* set file status flags */
00922 #define F_GETLK 5 /* get record locking information */
00923 #define F_SETLK 6 /* set record locking information */
00924 #define F_SETLKW 7 /* set record locking info; wait if blocked */
00925
00926 /* File descriptor flags used for fcntl(). POSIX Table 6-2. */
00927 #define FD_CLOEXEC 1 /* close on exec flag for third arg of fcntl */
00928
00929 /* L_type values for record locking with fcntl(). POSIX Table 6-3. */
00930 #define F_RDLCK 1 /* shared or read lock */
00931 #define F_WRLCK 2 /* exclusive or write lock */
00932 #define F_UNLCK 3 /* unlock */
00933
00934 /* Oflag values for open(). POSIX Table 6-4. */
00935 #define O_CREAT 00100 /* creat file if it doesn’t exist */
00936 #define O_EXCL 00200 /* exclusive use flag */
00937 #define O_NOCTTY 00400 /* do not assign a controlling terminal */
00938 #define O_TRUNC 01000 /* truncate flag */
00939
00940 /* File status flags for open() and fcntl(). POSIX Table 6-5. */
00941 #define O_APPEND 02000 /* set append mode */
00942 #define O_NONBLOCK 04000 /* no delay */
00943
00944 /* File access modes for open() and fcntl(). POSIX Table 6-6. */
00945 #define O_RDONLY 0 /* open(name, O_RDONLY) opens read only */
00946 #define O_WRONLY 1 /* open(name, O_WRONLY) opens write only */
00947 #define O_RDWR 2 /* open(name, O_RDWR) opens read/write */
00948
00949 /* Mask for use with file access modes. POSIX Table 6-7. */
MINIX SOURCE CODE File: include/fcntl.h 651
00950 #define O_ACCMODE 03 /* mask for file access modes */
00951
00952 /* Struct used for locking. POSIX Table 6-8. */
00953 struct flock {
00954 short l_type; /* type: F_RDLCK, F_WRLCK, or F_UNLCK */
00955 short l_whence; /* flag for starting offset */
00956 off_t l_start; /* relative offset in bytes */
00957 off_t l_len; /* size; if 0, then until EOF */
00958 pid_t l_pid; /* process id of the locks’ owner */
00959 };
00960
00961 /* Function Prototypes. */
00962 _PROTOTYPE( int creat, (const char *_path, _mnx_Mode_t _mode) );
00963 _PROTOTYPE( int fcntl, (int _filedes, int _cmd, ...) );
00964 _PROTOTYPE( int open, (const char *_path, int _oflag, ...) );
00965
00966 #endif /* _FCNTL_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/termios.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
01000 /* The <termios.h> header is used for controlling tty modes. */
01001
01002 #ifndef _TERMIOS_H
01003 #define _TERMIOS_H
01004
01005 typedef unsigned short tcflag_t;
01006 typedef unsigned char cc_t;
01007 typedef unsigned int speed_t;
01008
01009 #define NCCS 20 /* size of cc_c array, some extra space
01010 * for extensions. */
01011
01012 /* Primary terminal control structure. POSIX Table 7-1. */
01013 struct termios {
01014 tcflag_t c_iflag; /* input modes */
01015 tcflag_t c_oflag; /* output modes */
01016 tcflag_t c_cflag; /* control modes */
01017 tcflag_t c_lflag; /* local modes */
01018 speed_t c_ispeed; /* input speed */
01019 speed_t c_ospeed; /* output speed */
01020 cc_t c_cc[NCCS]; /* control characters */
01021 };
01022
01023 /* Values for termios c_iflag bit map. POSIX Table 7-2. */
01024 #define BRKINT 0x0001 /* signal interrupt on break */
01025 #define ICRNL 0x0002 /* map CR to NL on input */
01026 #define IGNBRK 0x0004 /* ignore break */
01027 #define IGNCR 0x0008 /* ignore CR */
01028 #define IGNPAR 0x0010 /* ignore characters with parity errors */
01029 #define INLCR 0x0020 /* map NL to CR on input */
01030 #define INPCK 0x0040 /* enable input parity check */
01031 #define ISTRIP 0x0080 /* mask off 8th bit */
01032 #define IXOFF 0x0100 /* enable start/stop input control */
01033 #define IXON 0x0200 /* enable start/stop output control */
01034 #define PARMRK 0x0400 /* mark parity errors in the input queue */
652 File: include/termios.h MINIX SOURCE CODE
01035
01036 /* Values for termios c_oflag bit map. POSIX Sec. 7.1.2.3. */
01037 #define OPOST 0x0001 /* perform output processing */
01038
01039 /* Values for termios c_cflag bit map. POSIX Table 7-3. */
01040 #define CLOCAL 0x0001 /* ignore modem status lines */
01041 #define CREAD 0x0002 /* enable receiver */
01042 #define CSIZE 0x000C /* number of bits per character */
01043 #define CS5 0x0000 /* if CSIZE is CS5, characters are 5 bits */
01044 #define CS6 0x0004 /* if CSIZE is CS6, characters are 6 bits */
01045 #define CS7 0x0008 /* if CSIZE is CS7, characters are 7 bits */
01046 #define CS8 0x000C /* if CSIZE is CS8, characters are 8 bits */
01047 #define CSTOPB 0x0010 /* send 2 stop bits if set, else 1 */
01048 #define HUPCL 0x0020 /* hang up on last close */
01049 #define PARENB 0x0040 /* enable parity on output */
01050 #define PARODD 0x0080 /* use odd parity if set, else even */
01051
01052 /* Values for termios c_lflag bit map. POSIX Table 7-4. */
01053 #define ECHO 0x0001 /* enable echoing of input characters */
01054 #define ECHOE 0x0002 /* echo ERASE as backspace */
01055 #define ECHOK 0x0004 /* echo KILL */
01056 #define ECHONL 0x0008 /* echo NL */
01057 #define ICANON 0x0010 /* canonical input (erase and kill enabled) */
01058 #define IEXTEN 0x0020 /* enable extended functions */
01059 #define ISIG 0x0040 /* enable signals */
01060 #define NOFLSH 0x0080 /* disable flush after interrupt or quit */
01061 #define TOSTOP 0x0100 /* send SIGTTOU (job control, not implemented*/
01062
01063 /* Indices into c_cc array. Default values in parentheses. POSIX Table 7-5. */
01064 #define VEOF 0 /* cc_c[VEOF] = EOF char (ˆD) */
01065 #define VEOL 1 /* cc_c[VEOL] = EOL char (undef) */
01066 #define VERASE 2 /* cc_c[VERASE] = ERASE char (ˆH) */
01067 #define VINTR 3 /* cc_c[VINTR] = INTR char (DEL) */
01068 #define VKILL 4 /* cc_c[VKILL] = KILL char (ˆU) */
01069 #define VMIN 5 /* cc_c[VMIN] = MIN value for timer */
01070 #define VQUIT 6 /* cc_c[VQUIT] = QUIT char (ˆ\) */
01071 #define VTIME 7 /* cc_c[VTIME] = TIME value for timer */
01072 #define VSUSP 8 /* cc_c[VSUSP] = SUSP (ˆZ, ignored) */
01073 #define VSTART 9 /* cc_c[VSTART] = START char (ˆS) */
01074 #define VSTOP 10 /* cc_c[VSTOP] = STOP char (ˆQ) */
01075
01076 #define _POSIX_VDISABLE (cc_t)0xFF /* You can’t even generate this
01077 * character with ’normal’ keyboards.
01078 * But some language specific keyboards
01079 * can generate 0xFF. It seems that all
01080 * 256 are used, so cc_t should be a
01081 * short...
01082 */
01083
01084 /* Values for the baud rate settings. POSIX Table 7-6. */
01085 #define B0 0x0000 /* hang up the line */
01086 #define B50 0x1000 /* 50 baud */
01087 #define B75 0x2000 /* 75 baud */
01088 #define B110 0x3000 /* 110 baud */
01089 #define B134 0x4000 /* 134.5 baud */
01090 #define B150 0x5000 /* 150 baud */
01091 #define B200 0x6000 /* 200 baud */
01092 #define B300 0x7000 /* 300 baud */
01093 #define B600 0x8000 /* 600 baud */
01094 #define B1200 0x9000 /* 1200 baud */
MINIX SOURCE CODE File: include/termios.h 653
01095 #define B1800 0xA000 /* 1800 baud */
01096 #define B2400 0xB000 /* 2400 baud */
01097 #define B4800 0xC000 /* 4800 baud */
01098 #define B9600 0xD000 /* 9600 baud */
01099 #define B19200 0xE000 /* 19200 baud */
01100 #define B38400 0xF000 /* 38400 baud */
01101
01102 /* Optional actions for tcsetattr(). POSIX Sec. 7.2.1.2. */
01103 #define TCSANOW 1 /* changes take effect immediately */
01104 #define TCSADRAIN 2 /* changes take effect after output is done */
01105 #define TCSAFLUSH 3 /* wait for output to finish and flush input */
01106
01107 /* Queue_selector values for tcflush(). POSIX Sec. 7.2.2.2. */
01108 #define TCIFLUSH 1 /* flush accumulated input data */
01109 #define TCOFLUSH 2 /* flush accumulated output data */
01110 #define TCIOFLUSH 3 /* flush accumulated input and output data */
01111
01112 /* Action values for tcflow(). POSIX Sec. 7.2.2.2. */
01113 #define TCOOFF 1 /* suspend output */
01114 #define TCOON 2 /* restart suspended output */
01115 #define TCIOFF 3 /* transmit a STOP character on the line */
01116 #define TCION 4 /* transmit a START character on the line */
01117
01118 /* Function Prototypes. */
01119 #ifndef _ANSI_H
01120 #include <ansi.h>
01121 #endif
01122
01123 _PROTOTYPE( int tcsendbreak, (int _fildes, int _duration) );
01124 _PROTOTYPE( int tcdrain, (int _filedes) );
01125 _PROTOTYPE( int tcflush, (int _filedes, int _queue_selector) );
01126 _PROTOTYPE( int tcflow, (int _filedes, int _action) );
01127 _PROTOTYPE( speed_t cfgetispeed, (const struct termios *_termios_p) );
01128 _PROTOTYPE( speed_t cfgetospeed, (const struct termios *_termios_p) );
01129 _PROTOTYPE( int cfsetispeed, (struct termios *_termios_p, speed_t _speed) );
01130 _PROTOTYPE( int cfsetospeed, (struct termios *_termios_p, speed_t _speed) );
01131 _PROTOTYPE( int tcgetattr, (int _filedes, struct termios *_termios_p) );
01132 _PROTOTYPE( int tcsetattr, \
01133 (int _filedes, int _opt_actions, const struct termios *_termios_p) );
01134
01135 #define cfgetispeed(termios_p) ((termios_p)->c_ispeed)
01136 #define cfgetospeed(termios_p) ((termios_p)->c_ospeed)
01137 #define cfsetispeed(termios_p, speed) ((termios_p)->c_ispeed = (speed), 0)
01138 #define cfsetospeed(termios_p, speed) ((termios_p)->c_ospeed = (speed), 0)
01139
01140 #ifdef _MINIX
01141 /* Here are the local extensions to the POSIX standard for Minix. Posix
01142 * conforming programs are not able to access these, and therefore they are
01143 * only defined when a Minix program is compiled.
01144 */
01145
01146 /* Extensions to the termios c_iflag bit map. */
01147 #define IXANY 0x0800 /* allow any key to continue ouptut */
01148
01149 /* Extensions to the termios c_oflag bit map. They are only active iff
01150 * OPOST is enabled. */
01151 #define ONLCR 0x0002 /* Map NL to CR-NL on output */
01152 #define XTABS 0x0004 /* Expand tabs to spaces */
01153 #define ONOEOT 0x0008 /* discard EOT’s (ˆD) on output) */
01154
654 File: include/termios.h MINIX SOURCE CODE
01155 /* Extensions to the termios c_lflag bit map. */
01156 #define LFLUSHO 0x0200 /* Flush output. */
01157
01158 /* Extensions to the c_cc array. */
01159 #define VREPRINT 11 /* cc_c[VREPRINT] (ˆR) */
01160 #define VLNEXT 12 /* cc_c[VLNEXT] (ˆV) */
01161 #define VDISCARD 13 /* cc_c[VDISCARD] (ˆO) */
01162
01163 /* Extensions to baud rate settings. */
01164 #define B57600 0x0100 /* 57600 baud */
01165 #define B115200 0x0200 /* 115200 baud */
01166
01167 /* These are the default settings used by the kernel and by ’stty sane’ */
01168
01169 #define TCTRL_DEF (CREAD | CS8 | HUPCL)
01170 #define TINPUT_DEF (BRKINT | ICRNL | IXON | IXANY)
01171 #define TOUTPUT_DEF (OPOST | ONLCR)
01172 #define TLOCAL_DEF (ISIG | IEXTEN | ICANON | ECHO | ECHOE)
01173 #define TSPEED_DEF B9600
01174
01175 #define TEOF_DEF ’\4’ /* ˆD */
01176 #define TEOL_DEF _POSIX_VDISABLE
01177 #define TERASE_DEF ’\10’ /* ˆH */
01178 #define TINTR_DEF ’\3’ /* ˆC */
01179 #define TKILL_DEF ’\25’ /* ˆU */
01180 #define TMIN_DEF 1
01181 #define TQUIT_DEF ’\34’ /* ˆ\ */
01182 #define TSTART_DEF ’\21’ /* ˆQ */
01183 #define TSTOP_DEF ’\23’ /* ˆS */
01184 #define TSUSP_DEF ’\32’ /* ˆZ */
01185 #define TTIME_DEF 0
01186 #define TREPRINT_DEF ’\22’ /* ˆR */
01187 #define TLNEXT_DEF ’\26’ /* ˆV */
01188 #define TDISCARD_DEF ’\17’ /* ˆO */
01189
01190 /* Window size. This information is stored in the TTY driver but not used.
01191 * This can be used for screen based applications in a window environment.
01192 * The ioctls TIOCGWINSZ and TIOCSWINSZ can be used to get and set this
01193 * information.
01194 */
01195
01196 struct winsize
01197 {
01198 unsigned short ws_row; /* rows, in characters */
01199 unsigned short ws_col; /* columns, in characters */
01200 unsigned short ws_xpixel; /* horizontal size, pixels */
01201 unsigned short ws_ypixel; /* vertical size, pixels */
01202 };
01203 #endif /* _MINIX */
01204
01205 #endif /* _TERMIOS_H */
MINIX SOURCE CODE File: include/timers.h 655
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/timers.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
01300 /* This library provides generic watchdog timer management functionality.
01301 * The functions operate on a timer queue provided by the caller. Note that
01302 * the timers must use absolute time to allow sorting. The library provides:
01303 *
01304 * tmrs_settimer: (re)set a new watchdog timer in the timers queue
01305 * tmrs_clrtimer: remove a timer from both the timers queue
01306 * tmrs_exptimers: check for expired timers and run watchdog functions
01307 *
01308 * Author:
01309 * Jorrit N. Herder <jnherder@cs.vu.nl>
01310 * Adapted from tmr_settimer and tmr_clrtimer in src/kernel/clock.c.
01311 * Last modified: September 30, 2004.
01312 */
01313
01314 #ifndef _TIMERS_H
01315 #define _TIMERS_H
01316
01317 #include <limits.h>
01318 #include <sys/types.h>
01319
01320 struct timer;
01321 typedef void (*tmr_func_t)(struct timer *tp);
01322 typedef union { int ta_int; long ta_long; void *ta_ptr; } tmr_arg_t;
01323
01324 /* A timer_t variable must be declare for each distinct timer to be used.
01325 * The timers watchdog function and expiration time are automatically set
01326 * by the library function tmrs_settimer, but its argument is not.
01327 */
01328 typedef struct timer
01329 {
01330 struct timer *tmr_next; /* next in a timer chain */
01331 clock_t tmr_exp_time; /* expiration time */
01332 tmr_func_t tmr_func; /* function to call when expired */
01333 tmr_arg_t tmr_arg; /* random argument */
01334 } timer_t;
01335
01336 /* Used when the timer is not active. */
01337 #define TMR_NEVER ((clock_t) -1 < 0) ? ((clock_t) LONG_MAX) : ((clock_t) -1)
01338 #undef TMR_NEVER
01339 #define TMR_NEVER ((clock_t) LONG_MAX)
01340
01341 /* These definitions can be used to set or get data from a timer variable. */
01342 #define tmr_arg(tp) (&(tp)->tmr_arg)
01343 #define tmr_exp_time(tp) (&(tp)->tmr_exp_time)
01344
01345 /* Timers should be initialized once before they are being used. Be careful
01346 * not to reinitialize a timer that is in a list of timers, or the chain
01347 * will be broken.
01348 */
01349 #define tmr_inittimer(tp) (void)((tp)->tmr_exp_time = TMR_NEVER, \
01350 (tp)->tmr_next = NULL)
01351
01352 /* The following generic timer management functions are available. They
01353 * can be used to operate on the lists of timers. Adding a timer to a list
01354 * automatically takes care of removing it.
656 File: include/timers.h MINIX SOURCE CODE
01355 */
01356 _PROTOTYPE( clock_t tmrs_clrtimer, (timer_t **tmrs, timer_t *tp, clock_t *new_head)
01357 _PROTOTYPE( void tmrs_exptimers, (timer_t **tmrs, clock_t now, clock_t *new_head)
01358 _PROTOTYPE( clock_t tmrs_settimer, (timer_t **tmrs, timer_t *tp,
01359 clock_t exp_time, tmr_func_t watchdog, clock_t *new_head)
01360
01361 #endif /* _TIMERS_H */
01362
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/types.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
01400 /* The <sys/types.h> header contains important data type definitions.
01401 * It is considered good programming practice to use these definitions,
01402 * instead of the underlying base type. By convention, all type names end
01403 * with _t.
01404 */
01405
01406 #ifndef _TYPES_H
01407 #define _TYPES_H
01408
01409 #ifndef _ANSI_H
01410 #include <ansi.h>
01411 #endif
01412
01413 /* The type size_t holds all results of the sizeof operator. At first glance,
01414 * it seems obvious that it should be an unsigned int, but this is not always
01415 * the case. For example, MINIX-ST (68000) has 32-bit pointers and 16-bit
01416 * integers. When one asks for the size of a 70K struct or array, the result
01417 * requires 17 bits to express, so size_t must be a long type. The type
01418 * ssize_t is the signed version of size_t.
01419 */
01420 #ifndef _SIZE_T
01421 #define _SIZE_T
01422 typedef unsigned int size_t;
01423 #endif
01424
01425 #ifndef _SSIZE_T
01426 #define _SSIZE_T
01427 typedef int ssize_t;
01428 #endif
01429
01430 #ifndef _TIME_T
01431 #define _TIME_T
01432 typedef long time_t; /* time in sec since 1 Jan 1970 0000 GMT */
01433 #endif
01434
01435 #ifndef _CLOCK_T
01436 #define _CLOCK_T
01437 typedef long clock_t; /* unit for system accounting */
01438 #endif
01439
01440 #ifndef _SIGSET_T
01441 #define _SIGSET_T
01442 typedef unsigned long sigset_t;
01443 #endif
01444
MINIX SOURCE CODE File: include/sys/types.h 657
01445 /* Open Group Base Specifications Issue 6 (not complete) */
01446 typedef long useconds_t; /* Time in microseconds */
01447
01448 /* Types used in disk, inode, etc. data structures. */
01449 typedef short dev_t; /* holds (major|minor) device pair */
01450 typedef char gid_t; /* group id */
01451 typedef unsigned long ino_t; /* i-node number (V3 filesystem) */
01452 typedef unsigned short mode_t; /* file type and permissions bits */
01453 typedef short nlink_t; /* number of links to a file */
01454 typedef unsigned long off_t; /* offset within a file */
01455 typedef int pid_t; /* process id (must be signed) */
01456 typedef short uid_t; /* user id */
01457 typedef unsigned long zone_t; /* zone number */
01458 typedef unsigned long block_t; /* block number */
01459 typedef unsigned long bit_t; /* bit number in a bit map */
01460 typedef unsigned short zone1_t; /* zone number for V1 file systems */
01461 typedef unsigned short bitchunk_t; /* collection of bits in a bitmap */
01462
01463 typedef unsigned char u8_t; /* 8 bit type */
01464 typedef unsigned short u16_t; /* 16 bit type */
01465 typedef unsigned long u32_t; /* 32 bit type */
01466
01467 typedef char i8_t; /* 8 bit signed type */
01468 typedef short i16_t; /* 16 bit signed type */
01469 typedef long i32_t; /* 32 bit signed type */
01470
01471 typedef struct { u32_t _[2]; } u64_t;
01472
01473 /* The following types are needed because MINIX uses K&R style function
01474 * definitions (for maximum portability). When a short, such as dev_t, is
01475 * passed to a function with a K&R definition, the compiler automatically
01476 * promotes it to an int. The prototype must contain an int as the parameter,
01477 * not a short, because an int is what an old-style function definition
01478 * expects. Thus using dev_t in a prototype would be incorrect. It would be
01479 * sufficient to just use int instead of dev_t in the prototypes, but Dev_t
01480 * is clearer.
01481 */
01482 typedef int Dev_t;
01483 typedef int _mnx_Gid_t;
01484 typedef int Nlink_t;
01485 typedef int _mnx_Uid_t;
01486 typedef int U8_t;
01487 typedef unsigned long U32_t;
01488 typedef int I8_t;
01489 typedef int I16_t;
01490 typedef long I32_t;
01491
01492 /* ANSI C makes writing down the promotion of unsigned types very messy. When
01493 * sizeof(short) == sizeof(int), there is no promotion, so the type stays
01494 * unsigned. When the compiler is not ANSI, there is usually no loss of
01495 * unsignedness, and there are usually no prototypes so the promoted type
01496 * doesn’t matter. The use of types like Ino_t is an attempt to use ints
01497 * (which are not promoted) while providing information to the reader.
01498 */
01499
01500 typedef unsigned long Ino_t;
01501
01502 #if _EM_WSIZE == 2
01503 /*typedef unsigned int Ino_t; Ino_t is now 32 bits */
01504 typedef unsigned int Zone1_t;
658 File: include/sys/types.h MINIX SOURCE CODE
01505 typedef unsigned int Bitchunk_t;
01506 typedef unsigned int U16_t;
01507 typedef unsigned int _mnx_Mode_t;
01508
01509 #else /* _EM_WSIZE == 4, or _EM_WSIZE undefined */
01510 /*typedef int Ino_t; Ino_t is now 32 bits */
01511 typedef int Zone1_t;
01512 typedef int Bitchunk_t;
01513 typedef int U16_t;
01514 typedef int _mnx_Mode_t;
01515
01516 #endif /* _EM_WSIZE == 2, etc */
01517
01518 /* Signal handler type, e.g. SIG_IGN */
01519 typedef void _PROTOTYPE( (*sighandler_t), (int) );
01520
01521 /* Compatibility with other systems */
01522 typedef unsigned char u_char;
01523 typedef unsigned short u_short;
01524 typedef unsigned int u_int;
01525 typedef unsigned long u_long;
01526 typedef char *caddr_t;
01527
01528 #endif /* _TYPES_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/sigcontext.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
01600 #ifndef _SIGCONTEXT_H
01601 #define _SIGCONTEXT_H
01602
01603 /* The sigcontext structure is used by the sigreturn(2) system call.
01604 * sigreturn() is seldom called by user programs, but it is used internally
01605 * by the signal catching mechanism.
01606 */
01607
01608 #ifndef _ANSI_H
01609 #include <ansi.h>
01610 #endif
01611
01612 #ifndef _MINIX_SYS_CONFIG_H
01613 #include <minix/sys_config.h>
01614 #endif
01615
01616 #if !defined(_MINIX_CHIP)
01617 #include "error, configuration is not known"
01618 #endif
01619
01620 /* The following structure should match the stackframe_s structure used
01621 * by the kernel’s context switching code. Floating point registers should
01622 * be added in a different struct.
01623 */
01624 struct sigregs {
01625 short sr_gs;
01626 short sr_fs;
01627 short sr_es;
01628 short sr_ds;
01629 int sr_di;
MINIX SOURCE CODE File: include/sys/sigcontext.h 659
01630 int sr_si;
01631 int sr_bp;
01632 int sr_st; /* stack top -- used in kernel */
01633 int sr_bx;
01634 int sr_dx;
01635 int sr_cx;
01636 int sr_retreg;
01637 int sr_retadr; /* return address to caller of save -- used
01638 * in kernel */
01639 int sr_pc;
01640 int sr_cs;
01641 int sr_psw;
01642 int sr_sp;
01643 int sr_ss;
01644 };
01645
01646 struct sigframe { /* stack frame created for signalled process */
01647 _PROTOTYPE( void (*sf_retadr), (void) );
01648 int sf_signo;
01649 int sf_code;
01650 struct sigcontext *sf_scp;
01651 int sf_fp;
01652 _PROTOTYPE( void (*sf_retadr2), (void) );
01653 struct sigcontext *sf_scpcopy;
01654 };
01655
01656 struct sigcontext {
01657 int sc_flags; /* sigstack state to restore */
01658 long sc_mask; /* signal mask to restore */
01659 struct sigregs sc_regs; /* register set to restore */
01660 };
01661
01662 #define sc_gs sc_regs.sr_gs
01663 #define sc_fs sc_regs.sr_fs
01664 #define sc_es sc_regs.sr_es
01665 #define sc_ds sc_regs.sr_ds
01666 #define sc_di sc_regs.sr_di
01667 #define sc_si sc_regs.sr_si
01668 #define sc_fp sc_regs.sr_bp
01669 #define sc_st sc_regs.sr_st /* stack top -- used in kernel */
01670 #define sc_bx sc_regs.sr_bx
01671 #define sc_dx sc_regs.sr_dx
01672 #define sc_cx sc_regs.sr_cx
01673 #define sc_retreg sc_regs.sr_retreg
01674 #define sc_retadr sc_regs.sr_retadr /* return address to caller of
01675 save -- used in kernel */
01676 #define sc_pc sc_regs.sr_pc
01677 #define sc_cs sc_regs.sr_cs
01678 #define sc_psw sc_regs.sr_psw
01679 #define sc_sp sc_regs.sr_sp
01680 #define sc_ss sc_regs.sr_ss
01681
01682 /* Values for sc_flags. Must agree with <minix/jmp_buf.h>. */
01683 #define SC_SIGCONTEXT 2 /* nonzero when signal context is included */
01684 #define SC_NOREGLOCALS 4 /* nonzero when registers are not to be
01685 saved and restored */
01686
01687 _PROTOTYPE( int sigreturn, (struct sigcontext *_scp) );
01688
01689 #endif /* _SIGCONTEXT_H */
660 File: include/sys/stat.h MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/stat.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
01700 /* The <sys/stat.h> header defines a struct that is used in the stat() and
01701 * fstat functions. The information in this struct comes from the i-node of
01702 * some file. These calls are the only approved way to inspect i-nodes.
01703 */
01704
01705 #ifndef _STAT_H
01706 #define _STAT_H
01707
01708 #ifndef _TYPES_H
01709 #include <sys/types.h>
01710 #endif
01711
01712 struct stat {
01713 dev_t st_dev; /* major/minor device number */
01714 ino_t st_ino; /* i-node number */
01715 mode_t st_mode; /* file mode, protection bits, etc. */
01716 short int st_nlink; /* # links; TEMPORARY HACK: should be nlink_t*/
01717 uid_t st_uid; /* uid of the file’s owner */
01718 short int st_gid; /* gid; TEMPORARY HACK: should be gid_t */
01719 dev_t st_rdev;
01720 off_t st_size; /* file size */
01721 time_t st_atime; /* time of last access */
01722 time_t st_mtime; /* time of last data modification */
01723 time_t st_ctime; /* time of last file status change */
01724 };
01725
01726 /* Traditional mask definitions for st_mode. */
01727 /* The ugly casts on only some of the definitions are to avoid suprising sign
01728 * extensions such as S_IFREG != (mode_t) S_IFREG when ints are 32 bits.
01729 */
01730 #define S_IFMT ((mode_t) 0170000) /* type of file */
01731 #define S_IFLNK ((mode_t) 0120000) /* symbolic link, not implemented */
01732 #define S_IFREG ((mode_t) 0100000) /* regular */
01733 #define S_IFBLK 0060000 /* block special */
01734 #define S_IFDIR 0040000 /* directory */
01735 #define S_IFCHR 0020000 /* character special */
01736 #define S_IFIFO 0010000 /* this is a FIFO */
01737 #define S_ISUID 0004000 /* set user id on execution */
01738 #define S_ISGID 0002000 /* set group id on execution */
01739 /* next is reserved for future use */
01740 #define S_ISVTX 01000 /* save swapped text even after use */
01741
01742 /* POSIX masks for st_mode. */
01743 #define S_IRWXU 00700 /* owner: rwx------ */
01744 #define S_IRUSR 00400 /* owner: r-------- */
01745 #define S_IWUSR 00200 /* owner: -w------- */
01746 #define S_IXUSR 00100 /* owner: --x------ */
01747
01748 #define S_IRWXG 00070 /* group: ---rwx--- */
01749 #define S_IRGRP 00040 /* group: ---r----- */
01750 #define S_IWGRP 00020 /* group: ----w---- */
01751 #define S_IXGRP 00010 /* group: -----x--- */
01752
01753 #define S_IRWXO 00007 /* others: ------rwx */
01754 #define S_IROTH 00004 /* others: ------r-- */
MINIX SOURCE CODE File: include/sys/stat.h 661
01755 #define S_IWOTH 00002 /* others: -------w- */
01756 #define S_IXOTH 00001 /* others: --------x */
01757
01758 /* The following macros test st_mode (from POSIX Sec. 5.6.1.1). */
01759 #define S_ISREG(m) (((m) & S_IFMT) == S_IFREG) /* is a reg file */
01760 #define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR) /* is a directory */
01761 #define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR) /* is a char spec */
01762 #define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK) /* is a block spec */
01763 #define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO) /* is a pipe/FIFO */
01764 #define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK) /* is a sym link */
01765
01766 /* Function Prototypes. */
01767 _PROTOTYPE( int chmod, (const char *_path, _mnx_Mode_t _mode) );
01768 _PROTOTYPE( int fstat, (int _fildes, struct stat *_buf) );
01769 _PROTOTYPE( int mkdir, (const char *_path, _mnx_Mode_t _mode) );
01770 _PROTOTYPE( int mkfifo, (const char *_path, _mnx_Mode_t _mode) );
01771 _PROTOTYPE( int stat, (const char *_path, struct stat *_buf) );
01772 _PROTOTYPE( mode_t umask, (_mnx_Mode_t _cmask) );
01773
01774 /* Open Group Base Specifications Issue 6 (not complete) */
01775 _PROTOTYPE( int lstat, (const char *_path, struct stat *_buf) );
01776
01777 #endif /* _STAT_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/dir.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
01800 /* The <dir.h> header gives the layout of a directory. */
01801
01802 #ifndef _DIR_H
01803 #define _DIR_H
01804
01805 #include <sys/types.h>
01806
01807 #define DIRBLKSIZ 512 /* size of directory block */
01808
01809 #ifndef DIRSIZ
01810 #define DIRSIZ 60
01811 #endif
01812
01813 struct direct {
01814 ino_t d_ino;
01815 char d_name[DIRSIZ];
01816 };
01817
01818 #endif /* _DIR_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/wait.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
01900 /* The <sys/wait.h> header contains macros related to wait(). The value
01901 * returned by wait() and waitpid() depends on whether the process
01902 * terminated by an exit() call, was killed by a signal, or was stopped
01903 * due to job control, as follows:
01904 *
662 File: include/sys/wait.h MINIX SOURCE CODE
01905 * High byte Low byte
01906 * +---------------------+
01907 * exit(status) | status | 0 |
01908 * +---------------------+
01909 * killed by signal | 0 | signal |
01910 * +---------------------+
01911 * stopped (job control) | signal | 0177 |
01912 * +---------------------+
01913 */
01914
01915 #ifndef _WAIT_H
01916 #define _WAIT_H
01917
01918 #ifndef _TYPES_H
01919 #include <sys/types.h>
01920 #endif
01921
01922 #define _LOW(v) ( (v) & 0377)
01923 #define _HIGH(v) ( ((v) >> 8) & 0377)
01924
01925 #define WNOHANG 1 /* do not wait for child to exit */
01926 #define WUNTRACED 2 /* for job control; not implemented */
01927
01928 #define WIFEXITED(s) (_LOW(s) == 0) /* normal exit */
01929 #define WEXITSTATUS(s) (_HIGH(s)) /* exit status */
01930 #define WTERMSIG(s) (_LOW(s) & 0177) /* sig value */
01931 #define WIFSIGNALED(s) (((unsigned int)(s)-1 & 0xFFFF) < 0xFF) /* signaled */
01932 #define WIFSTOPPED(s) (_LOW(s) == 0177) /* stopped */
01933 #define WSTOPSIG(s) (_HIGH(s) & 0377) /* stop signal */
01934
01935 /* Function Prototypes. */
01936 _PROTOTYPE( pid_t wait, (int *_stat_loc) );
01937 _PROTOTYPE( pid_t waitpid, (pid_t _pid, int *_stat_loc, int _options) );
01938
01939 #endif /* _WAIT_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/ioctl.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
02000 /* sys/ioctl.h - All ioctl() command codes. Author: Kees J. Bot
02001 * 23 Nov 2002
02002 *
02003 * This header file includes all other ioctl command code headers.
02004 */
02005
02006 #ifndef _S_IOCTL_H
02007 #define _S_IOCTL_H
02008
02009 /* A driver that uses ioctls claims a character for its series of commands.
02010 * For instance: #define TCGETS _IOR(’T’, 8, struct termios)
02011 * This is a terminal ioctl that uses the character ’T’. The character(s)
02012 * used in each header file are shown in the comment following.
02013 */
02014
02015 #include <sys/ioc_tty.h> /* ’T’ ’t’ ’k’ */
02016 #include <sys/ioc_disk.h> /* ’d’ */
02017 #include <sys/ioc_memory.h> /* ’m’ */
02018 #include <sys/ioc_cmos.h> /* ’c’ */
02019
MINIX SOURCE CODE File: include/sys/ioctl.h 663
02020 #endif /* _S_IOCTL_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/ioc_disk.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
02100 /* sys/ioc_disk.h - Disk ioctl() command codes. Author: Kees J. Bot
02101 * 23 Nov 2002
02102 *
02103 */
02104
02105 #ifndef _S_I_DISK_H
02106 #define _S_I_DISK_H
02107
02108 #include <minix/ioctl.h>
02109
02110 #define DIOCSETP _IOW(’d’, 3, struct partition)
02111 #define DIOCGETP _IOR(’d’, 4, struct partition)
02112 #define DIOCEJECT _IO (’d’, 5)
02113 #define DIOCTIMEOUT _IOW(’d’, 6, int)
02114 #define DIOCOPENCT _IOR(’d’, 7, int)
02115
02116 #endif /* _S_I_DISK_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/ioctl.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
02200 /* minix/ioctl.h - Ioctl helper definitions. Author: Kees J. Bot
02201 * 23 Nov 2002
02202 *
02203 * This file is included by every header file that defines ioctl codes.
02204 */
02205
02206 #ifndef _M_IOCTL_H
02207 #define _M_IOCTL_H
02208
02209 #ifndef _TYPES_H
02210 #include <sys/types.h>
02211 #endif
02212
02213 #if _EM_WSIZE >= 4
02214 /* Ioctls have the command encoded in the low-order word, and the size
02215 * of the parameter in the high-order word. The 3 high bits of the high-
02216 * order word are used to encode the in/out/void status of the parameter.
02217 */
02218 #define _IOCPARM_MASK 0x1FFF
02219 #define _IOC_VOID 0x20000000
02220 #define _IOCTYPE_MASK 0xFFFF
02221 #define _IOC_IN 0x40000000
02222 #define _IOC_OUT 0x80000000
02223 #define _IOC_INOUT (_IOC_IN | _IOC_OUT)
02224
664 File: include/minix/ioctl.h MINIX SOURCE CODE
02225 #define _IO(x,y) ((x << 8) | y | _IOC_VOID)
02226 #define _IOR(x,y,t) ((x << 8) | y | ((sizeof(t) & _IOCPARM_MASK) << 16) |\
02227 _IOC_OUT)
02228 #define _IOW(x,y,t) ((x << 8) | y | ((sizeof(t) & _IOCPARM_MASK) << 16) |\
02229 _IOC_IN)
02230 #define _IORW(x,y,t) ((x << 8) | y | ((sizeof(t) & _IOCPARM_MASK) << 16) |\
02231 _IOC_INOUT)
02232 #else
02233 /* No fancy encoding on a 16-bit machine. */
02234
02235 #define _IO(x,y) ((x << 8) | y)
02236 #define _IOR(x,y,t) _IO(x,y)
02237 #define _IOW(x,y,t) _IO(x,y)
02238 #define _IORW(x,y,t) _IO(x,y)
02239 #endif
02240
02241 int ioctl(int _fd, int _request, void *_data);
02242
02243 #endif /* _M_IOCTL_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/config.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
02300 #ifndef _CONFIG_H
02301 #define _CONFIG_H
02302
02303 /* Minix release and version numbers. */
02304 #define OS_RELEASE "3"
02305 #define OS_VERSION "1.0"
02306
02307 /* This file sets configuration parameters for the MINIX kernel, FS, and PM.
02308 * It is divided up into two main sections. The first section contains
02309 * user-settable parameters. In the second section, various internal system
02310 * parameters are set based on the user-settable parameters.
02311 *
02312 * Parts of config.h have been moved to sys_config.h, which can be included
02313 * by other include files that wish to get at the configuration data, but
02314 * don’t want to pollute the users namespace. Some editable values have
02315 * gone there.
02316 *
02317 * This is a modified version of config.h for compiling a small Minix system
02318 * with only the options described in the text, Operating Systems Design and
02319 * Implementation, 3rd edition. See the version of config.h in the full
02320 * source code directory for information on alternatives omitted here.
02321 */
02322
02323 /* The MACHINE (called _MINIX_MACHINE) setting can be done
02324 * in <minix/machine.h>.
02325 */
02326 #include <minix/sys_config.h>
02327
02328 #define MACHINE _MINIX_MACHINE
02329
02330 #define IBM_PC _MACHINE_IBM_PC
02331 #define SUN_4 _MACHINE_SUN_4
02332 #define SUN_4_60 _MACHINE_SUN_4_60
02333 #define ATARI _MACHINE_ATARI
02334 #define MACINTOSH _MACHINE_MACINTOSH
MINIX SOURCE CODE File: include/minix/config.h 665
02335
02336 /* Number of slots in the process table for non-kernel processes. The number
02337 * of system processes defines how many processes with special privileges
02338 * there can be. User processes share the same properties and count for one.
02339 *
02340 * These can be changed in sys_config.h.
02341 */
02342 #define NR_PROCS _NR_PROCS
02343 #define NR_SYS_PROCS _NR_SYS_PROCS
02344
02345 #define NR_BUFS 128
02346 #define NR_BUF_HASH 128
02347
02348 /* Number of controller tasks (/dev/cN device classes). */
02349 #define NR_CTRLRS 2
02350
02351 /* Enable or disable the second level file system cache on the RAM disk. */
02352 #define ENABLE_CACHE2 0
02353
02354 /* Enable or disable swapping processes to disk. */
02355 #define ENABLE_SWAP 0
02356
02357 /* Include or exclude an image of /dev/boot in the boot image.
02358 * Please update the makefile in /usr/src/tools/ as well.
02359 */
02360 #define ENABLE_BOOTDEV 0 /* load image of /dev/boot at boot time */
02361
02362 /* DMA_SECTORS may be increased to speed up DMA based drivers. */
02363 #define DMA_SECTORS 1 /* DMA buffer size (must be >= 1) */
02364
02365 /* Include or exclude backwards compatibility code. */
02366 #define ENABLE_BINCOMPAT 0 /* for binaries using obsolete calls */
02367 #define ENABLE_SRCCOMPAT 0 /* for sources using obsolete calls */
02368
02369 /* Which process should receive diagnostics from the kernel and system?
02370 * Directly sending it to TTY only displays the output. Sending it to the
02371 * log driver will cause the diagnostics to be buffered and displayed.
02372 */
02373 #define OUTPUT_PROC_NR LOG_PROC_NR /* TTY_PROC_NR or LOG_PROC_NR */
02374
02375 /* NR_CONS, NR_RS_LINES, and NR_PTYS determine the number of terminals the
02376 * system can handle.
02377 */
02378 #define NR_CONS 4 /* # system consoles (1 to 8) */
02379 #define NR_RS_LINES 0 /* # rs232 terminals (0 to 4) */
02380 #define NR_PTYS 0 /* # pseudo terminals (0 to 64) */
02381
02382 /*===========================================================================*
02383 * There are no user-settable parameters after this line *
02384 *===========================================================================*/
02385 /* Set the CHIP type based on the machine selected. The symbol CHIP is actually
02386 * indicative of more than just the CPU. For example, machines for which
02387 * CHIP == INTEL are expected to have 8259A interrrupt controllers and the
02388 * other properties of IBM PC/XT/AT/386 types machines in general. */
02389 #define INTEL _CHIP_INTEL /* CHIP type for PC, XT, AT, 386 and clones */
02390 #define M68000 _CHIP_M68000 /* CHIP type for Atari, Amiga, Macintosh */
02391 #define SPARC _CHIP_SPARC /* CHIP type for SUN-4 (e.g. SPARCstation) */
02392
02393 /* Set the FP_FORMAT type based on the machine selected, either hw or sw */
02394 #define FP_NONE _FP_NONE /* no floating point support */
666 File: include/minix/config.h MINIX SOURCE CODE
02395 #define FP_IEEE _FP_IEEE /* conform IEEE floating point standard */
02396
02397 /* _MINIX_CHIP is defined in sys_config.h. */
02398 #define CHIP _MINIX_CHIP
02399
02400 /* _MINIX_FP_FORMAT is defined in sys_config.h. */
02401 #define FP_FORMAT _MINIX_FP_FORMAT
02402
02403 /* _ASKDEV and _FASTLOAD are defined in sys_config.h. */
02404 #define ASKDEV _ASKDEV
02405 #define FASTLOAD _FASTLOAD
02406
02407 #endif /* _CONFIG_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/sys_config.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
02500 #ifndef _MINIX_SYS_CONFIG_H
02501 #define _MINIX_SYS_CONFIG_H 1
02502
02503 /* This is a modified sys_config.h for compiling a small Minix system
02504 * with only the options described in the text, Operating Systems Design and
02505 * Implementation, 3rd edition. See the sys_config.h in the full
02506 * source code directory for information on alternatives omitted here.
02507 */
02508
02509 /*===========================================================================*
02510 * This section contains user-settable parameters *
02511 *===========================================================================*/
02512 #define _MINIX_MACHINE _MACHINE_IBM_PC
02513
02514 #define _MACHINE_IBM_PC 1 /* any 8088 or 80x86-based system */
02515
02516 /* Word size in bytes (a constant equal to sizeof(int)). */
02517 #if __ACK__ || __GNUC__
02518 #define _WORD_SIZE _EM_WSIZE
02519 #define _PTR_SIZE _EM_WSIZE
02520 #endif
02521
02522 #define _NR_PROCS 64
02523 #define _NR_SYS_PROCS 32
02524
02525 /* Set the CHIP type based on the machine selected. The symbol CHIP is actually
02526 * indicative of more than just the CPU. For example, machines for which
02527 * CHIP == INTEL are expected to have 8259A interrrupt controllers and the
02528 * other properties of IBM PC/XT/AT/386 types machines in general. */
02529 #define _CHIP_INTEL 1 /* CHIP type for PC, XT, AT, 386 and clones */
02530
02531 /* Set the FP_FORMAT type based on the machine selected, either hw or sw */
02532 #define _FP_NONE 0 /* no floating point support */
02533 #define _FP_IEEE 1 /* conform IEEE floating point standard */
02534
02535 #define _MINIX_CHIP _CHIP_INTEL
02536
02537 #define _MINIX_FP_FORMAT _FP_NONE
02538
02539 #ifndef _MINIX_MACHINE
MINIX SOURCE CODE File: include/minix/sys_config.h 667
02540 error "In <minix/sys_config.h> please define _MINIX_MACHINE"
02541 #endif
02542
02543 #ifndef _MINIX_CHIP
02544 error "In <minix/sys_config.h> please define _MINIX_MACHINE to have a legal value"
02545 #endif
02546
02547 #if (_MINIX_MACHINE == 0)
02548 error "_MINIX_MACHINE has incorrect value (0)"
02549 #endif
02550
02551 #endif /* _MINIX_SYS_CONFIG_H */
02552
02553
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
02600 /* Copyright (C) 2001 by Prentice-Hall, Inc. See the copyright notice in
02601 * the file /usr/src/LICENSE.
02602 */
02603
02604 #ifndef CHIP
02605 #error CHIP is not defined
02606 #endif
02607
02608 #define EXTERN extern /* used in *.h files */
02609 #define PRIVATE static /* PRIVATE x limits the scope of x */
02610 #define PUBLIC /* PUBLIC is the opposite of PRIVATE */
02611 #define FORWARD static /* some compilers require this to be ’static’*/
02612
02613 #define TRUE 1 /* used for turning integers into Booleans */
02614 #define FALSE 0 /* used for turning integers into Booleans */
02615
02616 #define HZ 60 /* clock freq (software settable on IBM-PC) */
02617
02618 #define SUPER_USER (uid_t) 0 /* uid_t of superuser */
02619
02620 /* Devices. */
02621 #define MAJOR 8 /* major device = (dev>>MAJOR) & 0377 */
02622 #define MINOR 0 /* minor device = (dev>>MINOR) & 0377 */
02623
02624 #define NULL ((void *)0) /* null pointer */
02625 #define CPVEC_NR 16 /* max # of entries in a SYS_VCOPY request */
02626 #define CPVVEC_NR 64 /* max # of entries in a SYS_VCOPY request */
02627 #define NR_IOREQS MIN(NR_BUFS, 64)
02628 /* maximum number of entries in an iorequest */
02629
02630 /* Message passing constants. */
02631 #define MESS_SIZE (sizeof(message)) /* might need usizeof from FS here */
02632 #define NIL_MESS ((message *) 0) /* null pointer */
02633
02634 /* Memory related constants. */
02635 #define SEGMENT_TYPE 0xFF00 /* bit mask to get segment type */
02636 #define SEGMENT_INDEX 0x00FF /* bit mask to get segment index */
02637
02638 #define LOCAL_SEG 0x0000 /* flags indicating local memory segment */
02639 #define NR_LOCAL_SEGS 3 /* # local segments per process (fixed) */
668 File: include/minix/const.h MINIX SOURCE CODE
02640 #define T 0 /* proc[i].mem_map[T] is for text */
02641 #define D 1 /* proc[i].mem_map[D] is for data */
02642 #define S 2 /* proc[i].mem_map[S] is for stack */
02643
02644 #define REMOTE_SEG 0x0100 /* flags indicating remote memory segment */
02645 #define NR_REMOTE_SEGS 3 /* # remote memory regions (variable) */
02646
02647 #define BIOS_SEG 0x0200 /* flags indicating BIOS memory segment */
02648 #define NR_BIOS_SEGS 3 /* # BIOS memory regions (variable) */
02649
02650 #define PHYS_SEG 0x0400 /* flag indicating entire physical memory */
02651
02652 /* Labels used to disable code sections for different reasons. */
02653 #define DEAD_CODE 0 /* unused code in normal configuration */
02654 #define FUTURE_CODE 0 /* new code to be activated + tested later */
02655 #define TEMP_CODE 1 /* active code to be removed later */
02656
02657 /* Process name length in the PM process table, including ’\0’. */
02658 #define PROC_NAME_LEN 16
02659
02660 /* Miscellaneous */
02661 #define BYTE 0377 /* mask for 8 bits */
02662 #define READING 0 /* copy data to user */
02663 #define WRITING 1 /* copy data from user */
02664 #define NO_NUM 0x8000 /* used as numerical argument to panic() */
02665 #define NIL_PTR (char *) 0 /* generally useful expression */
02666 #define HAVE_SCATTERED_IO 1 /* scattered I/O is now standard */
02667
02668 /* Macros. */
02669 #define MAX(a, b) ((a) > (b) ? (a) : (b))
02670 #define MIN(a, b) ((a) < (b) ? (a) : (b))
02671
02672 /* Memory is allocated in clicks. */
02673 #if (CHIP == INTEL)
02674 #define CLICK_SIZE 1024 /* unit in which memory is allocated */
02675 #define CLICK_SHIFT 10 /* log2 of CLICK_SIZE */
02676 #endif
02677
02678 #if (CHIP == SPARC) || (CHIP == M68000)
02679 #define CLICK_SIZE 4096 /* unit in which memory is allocated */
02680 #define CLICK_SHIFT 12 /* log2 of CLICK_SIZE */
02681 #endif
02682
02683 /* Click to byte conversions (and vice versa). */
02684 #define HCLICK_SHIFT 4 /* log2 of HCLICK_SIZE */
02685 #define HCLICK_SIZE 16 /* hardware segment conversion magic */
02686 #if CLICK_SIZE >= HCLICK_SIZE
02687 #define click_to_hclick(n) ((n) << (CLICK_SHIFT - HCLICK_SHIFT))
02688 #else
02689 #define click_to_hclick(n) ((n) >> (HCLICK_SHIFT - CLICK_SHIFT))
02690 #endif
02691 #define hclick_to_physb(n) ((phys_bytes) (n) << HCLICK_SHIFT)
02692 #define physb_to_hclick(n) ((n) >> HCLICK_SHIFT)
02693
02694 #define ABS -999 /* this process means absolute memory */
02695
02696 /* Flag bits for i_mode in the inode. */
02697 #define I_TYPE 0170000 /* this field gives inode type */
02698 #define I_REGULAR 0100000 /* regular file, not dir or special */
02699 #define I_BLOCK_SPECIAL 0060000 /* block special file */
MINIX SOURCE CODE File: include/minix/const.h 669
02700 #define I_DIRECTORY 0040000 /* file is a directory */
02701 #define I_CHAR_SPECIAL 0020000 /* character special file */
02702 #define I_NAMED_PIPE 0010000 /* named pipe (FIFO) */
02703 #define I_SET_UID_BIT 0004000 /* set effective uid_t on exec */
02704 #define I_SET_GID_BIT 0002000 /* set effective gid_t on exec */
02705 #define ALL_MODES 0006777 /* all bits for user, group and others */
02706 #define RWX_MODES 0000777 /* mode bits for RWX only */
02707 #define R_BIT 0000004 /* Rwx protection bit */
02708 #define W_BIT 0000002 /* rWx protection bit */
02709 #define X_BIT 0000001 /* rwX protection bit */
02710 #define I_NOT_ALLOC 0000000 /* this inode is free */
02711
02712 /* Flag used only in flags argument of dev_open. */
02713 #define RO_BIT 0200000 /* Open device readonly; fail if writable. */
02714
02715 /* Some limits. */
02716 #define MAX_BLOCK_NR ((block_t) 077777777) /* largest block number */
02717 #define HIGHEST_ZONE ((zone_t) 077777777) /* largest zone number */
02718 #define MAX_INODE_NR ((ino_t) 037777777777) /* largest inode number */
02719 #define MAX_FILE_POS ((off_t) 037777777777) /* largest legal file offset */
02720
02721 #define NO_BLOCK ((block_t) 0) /* absence of a block number */
02722 #define NO_ENTRY ((ino_t) 0) /* absence of a dir entry */
02723 #define NO_ZONE ((zone_t) 0) /* absence of a zone number */
02724 #define NO_DEV ((dev_t) 0) /* absence of a device numb */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/type.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
02800 #ifndef _TYPE_H
02801 #define _TYPE_H
02802
02803 #ifndef _MINIX_SYS_CONFIG_H
02804 #include <minix/sys_config.h>
02805 #endif
02806
02807 #ifndef _TYPES_H
02808 #include <sys/types.h>
02809 #endif
02810
02811 /* Type definitions. */
02812 typedef unsigned int vir_clicks; /* virtual addr/length in clicks */
02813 typedef unsigned long phys_bytes; /* physical addr/length in bytes */
02814 typedef unsigned int phys_clicks; /* physical addr/length in clicks */
02815
02816 #if (_MINIX_CHIP == _CHIP_INTEL)
02817 typedef unsigned int vir_bytes; /* virtual addresses and lengths in bytes */
02818 #endif
02819
02820 #if (_MINIX_CHIP == _CHIP_M68000)
02821 typedef unsigned long vir_bytes;/* virtual addresses and lengths in bytes */
02822 #endif
02823
02824 #if (_MINIX_CHIP == _CHIP_SPARC)
02825 typedef unsigned long vir_bytes;/* virtual addresses and lengths in bytes */
02826 #endif
02827
02828 /* Memory map for local text, stack, data segments. */
02829 struct mem_map {
670 File: include/minix/type.h MINIX SOURCE CODE
02830 vir_clicks mem_vir; /* virtual address */
02831 phys_clicks mem_phys; /* physical address */
02832 vir_clicks mem_len; /* length */
02833 };
02834
02835 /* Memory map for remote memory areas, e.g., for the RAM disk. */
02836 struct far_mem {
02837 int in_use; /* entry in use, unless zero */
02838 phys_clicks mem_phys; /* physical address */
02839 vir_clicks mem_len; /* length */
02840 };
02841
02842 /* Structure for virtual copying by means of a vector with requests. */
02843 struct vir_addr {
02844 int proc_nr;
02845 int segment;
02846 vir_bytes offset;
02847 };
02848
02849 #define phys_cp_req vir_cp_req
02850 struct vir_cp_req {
02851 struct vir_addr src;
02852 struct vir_addr dst;
02853 phys_bytes count;
02854 };
02855
02856 typedef struct {
02857 vir_bytes iov_addr; /* address of an I/O buffer */
02858 vir_bytes iov_size; /* sizeof an I/O buffer */
02859 } iovec_t;
02860
02861 /* PM passes the address of a structure of this type to KERNEL when
02862 * sys_sendsig() is invoked as part of the signal catching mechanism.
02863 * The structure contain all the information that KERNEL needs to build
02864 * the signal stack.
02865 */
02866 struct sigmsg {
02867 int sm_signo; /* signal number being caught */
02868 unsigned long sm_mask; /* mask to restore when handler returns */
02869 vir_bytes sm_sighandler; /* address of handler */
02870 vir_bytes sm_sigreturn; /* address of _sigreturn in C library */
02871 vir_bytes sm_stkptr; /* user stack pointer */
02872 };
02873
02874 /* This is used to obtain system information through SYS_GETINFO. */
02875 struct kinfo {
02876 phys_bytes code_base; /* base of kernel code */
02877 phys_bytes code_size;
02878 phys_bytes data_base; /* base of kernel data */
02879 phys_bytes data_size;
02880 vir_bytes proc_addr; /* virtual address of process table */
02881 phys_bytes kmem_base; /* kernel memory layout (/dev/kmem) */
02882 phys_bytes kmem_size;
02883 phys_bytes bootdev_base; /* boot device from boot image (/dev/boot) */
02884 phys_bytes bootdev_size;
02885 phys_bytes bootdev_mem;
02886 phys_bytes params_base; /* parameters passed by boot monitor */
02887 phys_bytes params_size;
02888 int nr_procs; /* number of user processes */
02889 int nr_tasks; /* number of kernel tasks */
MINIX SOURCE CODE File: include/minix/type.h 671
02890 char release[6]; /* kernel release number */
02891 char version[6]; /* kernel version number */
02892 int relocking; /* relocking check (for debugging) */
02893 };
02894
02895 struct machine {
02896 int pc_at;
02897 int ps_mca;
02898 int processor;
02899 int protected;
02900 int vdu_ega;
02901 int vdu_vga;
02902 };
02903
02904 #endif /* _TYPE_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/ipc.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
03000 #ifndef _IPC_H
03001 #define _IPC_H
03002
03003 /*==========================================================================*
03004 * Types relating to messages. *
03005 *==========================================================================*/
03006
03007 #define M1 1
03008 #define M3 3
03009 #define M4 4
03010 #define M3_STRING 14
03011
03012 typedef struct {int m1i1, m1i2, m1i3; char *m1p1, *m1p2, *m1p3;} mess_1;
03013 typedef struct {int m2i1, m2i2, m2i3; long m2l1, m2l2; char *m2p1;} mess_2;
03014 typedef struct {int m3i1, m3i2; char *m3p1; char m3ca1[M3_STRING];} mess_3;
03015 typedef struct {long m4l1, m4l2, m4l3, m4l4, m4l5;} mess_4;
03016 typedef struct {short m5c1, m5c2; int m5i1, m5i2; long m5l1, m5l2, m5l3;}mess_5;
03017 typedef struct {int m7i1, m7i2, m7i3, m7i4; char *m7p1, *m7p2;} mess_7;
03018 typedef struct {int m8i1, m8i2; char *m8p1, *m8p2, *m8p3, *m8p4;} mess_8;
03019
03020 typedef struct {
03021 int m_source; /* who sent the message */
03022 int m_type; /* what kind of message is it */
03023 union {
03024 mess_1 m_m1;
03025 mess_2 m_m2;
03026 mess_3 m_m3;
03027 mess_4 m_m4;
03028 mess_5 m_m5;
03029 mess_7 m_m7;
03030 mess_8 m_m8;
03031 } m_u;
03032 } message;
03033
03034 /* The following defines provide names for useful members. */
03035 #define m1_i1 m_u.m_m1.m1i1
03036 #define m1_i2 m_u.m_m1.m1i2
03037 #define m1_i3 m_u.m_m1.m1i3
03038 #define m1_p1 m_u.m_m1.m1p1
03039 #define m1_p2 m_u.m_m1.m1p2
672 File: include/minix/ipc.h MINIX SOURCE CODE
03040 #define m1_p3 m_u.m_m1.m1p3
03041
03042 #define m2_i1 m_u.m_m2.m2i1
03043 #define m2_i2 m_u.m_m2.m2i2
03044 #define m2_i3 m_u.m_m2.m2i3
03045 #define m2_l1 m_u.m_m2.m2l1
03046 #define m2_l2 m_u.m_m2.m2l2
03047 #define m2_p1 m_u.m_m2.m2p1
03048
03049 #define m3_i1 m_u.m_m3.m3i1
03050 #define m3_i2 m_u.m_m3.m3i2
03051 #define m3_p1 m_u.m_m3.m3p1
03052 #define m3_ca1 m_u.m_m3.m3ca1
03053
03054 #define m4_l1 m_u.m_m4.m4l1
03055 #define m4_l2 m_u.m_m4.m4l2
03056 #define m4_l3 m_u.m_m4.m4l3
03057 #define m4_l4 m_u.m_m4.m4l4
03058 #define m4_l5 m_u.m_m4.m4l5
03059
03060 #define m5_c1 m_u.m_m5.m5c1
03061 #define m5_c2 m_u.m_m5.m5c2
03062 #define m5_i1 m_u.m_m5.m5i1
03063 #define m5_i2 m_u.m_m5.m5i2
03064 #define m5_l1 m_u.m_m5.m5l1
03065 #define m5_l2 m_u.m_m5.m5l2
03066 #define m5_l3 m_u.m_m5.m5l3
03067
03068 #define m7_i1 m_u.m_m7.m7i1
03069 #define m7_i2 m_u.m_m7.m7i2
03070 #define m7_i3 m_u.m_m7.m7i3
03071 #define m7_i4 m_u.m_m7.m7i4
03072 #define m7_p1 m_u.m_m7.m7p1
03073 #define m7_p2 m_u.m_m7.m7p2
03074
03075 #define m8_i1 m_u.m_m8.m8i1
03076 #define m8_i2 m_u.m_m8.m8i2
03077 #define m8_p1 m_u.m_m8.m8p1
03078 #define m8_p2 m_u.m_m8.m8p2
03079 #define m8_p3 m_u.m_m8.m8p3
03080 #define m8_p4 m_u.m_m8.m8p4
03081
03082 /*==========================================================================*
03083 * Minix run-time system (IPC). *
03084 *==========================================================================*/
03085
03086 /* Hide names to avoid name space pollution. */
03087 #define echo _echo
03088 #define notify _notify
03089 #define sendrec _sendrec
03090 #define receive _receive
03091 #define send _send
03092 #define nb_receive _nb_receive
03093 #define nb_send _nb_send
03094
03095 _PROTOTYPE( int echo, (message *m_ptr) );
03096 _PROTOTYPE( int notify, (int dest) );
03097 _PROTOTYPE( int sendrec, (int src_dest, message *m_ptr) );
03098 _PROTOTYPE( int receive, (int src, message *m_ptr) );
03099 _PROTOTYPE( int send, (int dest, message *m_ptr) );
MINIX SOURCE CODE File: include/minix/ipc.h 673
03100 _PROTOTYPE( int nb_receive, (int src, message *m_ptr) );
03101 _PROTOTYPE( int nb_send, (int dest, message *m_ptr) );
03102
03103 #endif /* _IPC_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/syslib.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
03200 /* Prototypes for system library functions. */
03201
03202 #ifndef _SYSLIB_H
03203 #define _SYSLIB_H
03204
03205 #ifndef _TYPES_H
03206 #include <sys/types.h>
03207 #endif
03208
03209 #ifndef _IPC_H
03210 #include <minix/ipc.h>
03211 #endif
03212
03213 #ifndef _DEVIO_H
03214 #include <minix/devio.h>
03215 #endif
03216
03217 /* Forward declaration */
03218 struct reg86u;
03219
03220 #define SYSTASK SYSTEM
03221
03222 /*==========================================================================*
03223 * Minix system library. *
03224 *==========================================================================*/
03225 _PROTOTYPE( int _taskcall, (int who, int syscallnr, message *msgptr));
03226
03227 _PROTOTYPE( int sys_abort, (int how, ...));
03228 _PROTOTYPE( int sys_exec, (int proc, char *ptr,
03229 char *aout, vir_bytes initpc));
03230 _PROTOTYPE( int sys_fork, (int parent, int child));
03231 _PROTOTYPE( int sys_newmap, (int proc, struct mem_map *ptr));
03232 _PROTOTYPE( int sys_exit, (int proc));
03233 _PROTOTYPE( int sys_trace, (int req, int proc, long addr, long *data_p));
03234
03235 _PROTOTYPE( int sys_svrctl, (int proc, int req, int priv,vir_bytes argp));
03236 _PROTOTYPE( int sys_nice, (int proc, int priority));
03237
03238 _PROTOTYPE( int sys_int86, (struct reg86u *reg86p));
03239
03240 /* Shorthands for sys_sdevio() system call. */
03241 #define sys_insb(port, proc_nr, buffer, count) \
03242 sys_sdevio(DIO_INPUT, port, DIO_BYTE, proc_nr, buffer, count)
03243 #define sys_insw(port, proc_nr, buffer, count) \
03244 sys_sdevio(DIO_INPUT, port, DIO_WORD, proc_nr, buffer, count)
03245 #define sys_outsb(port, proc_nr, buffer, count) \
03246 sys_sdevio(DIO_OUTPUT, port, DIO_BYTE, proc_nr, buffer, count)
03247 #define sys_outsw(port, proc_nr, buffer, count) \
03248 sys_sdevio(DIO_OUTPUT, port, DIO_WORD, proc_nr, buffer, count)
03249 _PROTOTYPE( int sys_sdevio, (int req, long port, int type, int proc_nr,
674 File: include/minix/syslib.h MINIX SOURCE CODE
03250 void *buffer, int count));
03251
03252 /* Clock functionality: get system times or (un)schedule an alarm call. */
03253 _PROTOTYPE( int sys_times, (int proc_nr, clock_t *ptr));
03254 _PROTOTYPE(int sys_setalarm, (clock_t exp_time, int abs_time));
03255
03256 /* Shorthands for sys_irqctl() system call. */
03257 #define sys_irqdisable(hook_id) \
03258 sys_irqctl(IRQ_DISABLE, 0, 0, hook_id)
03259 #define sys_irqenable(hook_id) \
03260 sys_irqctl(IRQ_ENABLE, 0, 0, hook_id)
03261 #define sys_irqsetpolicy(irq_vec, policy, hook_id) \
03262 sys_irqctl(IRQ_SETPOLICY, irq_vec, policy, hook_id)
03263 #define sys_irqrmpolicy(irq_vec, hook_id) \
03264 sys_irqctl(IRQ_RMPOLICY, irq_vec, 0, hook_id)
03265 _PROTOTYPE ( int sys_irqctl, (int request, int irq_vec, int policy,
03266 int *irq_hook_id) );
03267
03268 /* Shorthands for sys_vircopy() and sys_physcopy() system calls. */
03269 #define sys_biosin(bios_vir, dst_vir, bytes) \
03270 sys_vircopy(SELF, BIOS_SEG, bios_vir, SELF, D, dst_vir, bytes)
03271 #define sys_biosout(src_vir, bios_vir, bytes) \
03272 sys_vircopy(SELF, D, src_vir, SELF, BIOS_SEG, bios_vir, bytes)
03273 #define sys_datacopy(src_proc, src_vir, dst_proc, dst_vir, bytes) \
03274 sys_vircopy(src_proc, D, src_vir, dst_proc, D, dst_vir, bytes)
03275 #define sys_textcopy(src_proc, src_vir, dst_proc, dst_vir, bytes) \
03276 sys_vircopy(src_proc, T, src_vir, dst_proc, T, dst_vir, bytes)
03277 #define sys_stackcopy(src_proc, src_vir, dst_proc, dst_vir, bytes) \
03278 sys_vircopy(src_proc, S, src_vir, dst_proc, S, dst_vir, bytes)
03279 _PROTOTYPE(int sys_vircopy, (int src_proc, int src_seg, vir_bytes src_vir,
03280 int dst_proc, int dst_seg, vir_bytes dst_vir, phys_bytes bytes));
03281
03282 #define sys_abscopy(src_phys, dst_phys, bytes) \
03283 sys_physcopy(NONE, PHYS_SEG, src_phys, NONE, PHYS_SEG, dst_phys, bytes)
03284 _PROTOTYPE(int sys_physcopy, (int src_proc, int src_seg, vir_bytes src_vir,
03285 int dst_proc, int dst_seg, vir_bytes dst_vir, phys_bytes bytes));
03286 _PROTOTYPE(int sys_memset, (unsigned long pattern,
03287 phys_bytes base, phys_bytes bytes));
03288
03289 /* Vectored virtual / physical copy calls. */
03290 #if DEAD_CODE /* library part not yet implemented */
03291 _PROTOTYPE(int sys_virvcopy, (phys_cp_req *vec_ptr,int vec_size,int *nr_ok));
03292 _PROTOTYPE(int sys_physvcopy, (phys_cp_req *vec_ptr,int vec_size,int *nr_ok));
03293 #endif
03294
03295 _PROTOTYPE(int sys_umap, (int proc_nr, int seg, vir_bytes vir_addr,
03296 vir_bytes bytes, phys_bytes *phys_addr));
03297 _PROTOTYPE(int sys_segctl, (int *index, u16_t *seg, vir_bytes *off,
03298 phys_bytes phys, vir_bytes size));
03299
03300 /* Shorthands for sys_getinfo() system call. */
03301 #define sys_getkmessages(dst) sys_getinfo(GET_KMESSAGES, dst, 0,0,0)
03302 #define sys_getkinfo(dst) sys_getinfo(GET_KINFO, dst, 0,0,0)
03303 #define sys_getmachine(dst) sys_getinfo(GET_MACHINE, dst, 0,0,0)
03304 #define sys_getproctab(dst) sys_getinfo(GET_PROCTAB, dst, 0,0,0)
03305 #define sys_getprivtab(dst) sys_getinfo(GET_PRIVTAB, dst, 0,0,0)
03306 #define sys_getproc(dst,nr) sys_getinfo(GET_PROC, dst, 0,0, nr)
03307 #define sys_getrandomness(dst) sys_getinfo(GET_RANDOMNESS, dst, 0,0,0)
03308 #define sys_getimage(dst) sys_getinfo(GET_IMAGE, dst, 0,0,0)
03309 #define sys_getirqhooks(dst) sys_getinfo(GET_IRQHOOKS, dst, 0,0,0)
MINIX SOURCE CODE File: include/minix/syslib.h 675
03310 #define sys_getmonparams(v,vl) sys_getinfo(GET_MONPARAMS, v,vl, 0,0)
03311 #define sys_getschedinfo(v1,v2) sys_getinfo(GET_SCHEDINFO, v1,0, v2,0)
03312 #define sys_getlocktimings(dst) sys_getinfo(GET_LOCKTIMING, dst, 0,0,0)
03313 #define sys_getbiosbuffer(virp, sizep) sys_getinfo(GET_BIOSBUFFER, virp, \
03314 sizeof(*virp), sizep, sizeof(*sizep))
03315 _PROTOTYPE(int sys_getinfo, (int request, void *val_ptr, int val_len,
03316 void *val_ptr2, int val_len2) );
03317
03318 /* Signal control. */
03319 _PROTOTYPE(int sys_kill, (int proc, int sig) );
03320 _PROTOTYPE(int sys_sigsend, (int proc_nr, struct sigmsg *sig_ctxt) );
03321 _PROTOTYPE(int sys_sigreturn, (int proc_nr, struct sigmsg *sig_ctxt) );
03322 _PROTOTYPE(int sys_getksig, (int *k_proc_nr, sigset_t *k_sig_map) );
03323 _PROTOTYPE(int sys_endksig, (int proc_nr) );
03324
03325 /* NOTE: two different approaches were used to distinguish the device I/O
03326 * types ’byte’, ’word’, ’long’: the latter uses #define and results in a
03327 * smaller implementation, but looses the static type checking.
03328 */
03329 _PROTOTYPE(int sys_voutb, (pvb_pair_t *pvb_pairs, int nr_ports) );
03330 _PROTOTYPE(int sys_voutw, (pvw_pair_t *pvw_pairs, int nr_ports) );
03331 _PROTOTYPE(int sys_voutl, (pvl_pair_t *pvl_pairs, int nr_ports) );
03332 _PROTOTYPE(int sys_vinb, (pvb_pair_t *pvb_pairs, int nr_ports) );
03333 _PROTOTYPE(int sys_vinw, (pvw_pair_t *pvw_pairs, int nr_ports) );
03334 _PROTOTYPE(int sys_vinl, (pvl_pair_t *pvl_pairs, int nr_ports) );
03335
03336 /* Shorthands for sys_out() system call. */
03337 #define sys_outb(p,v) sys_out((p), (unsigned long) (v), DIO_BYTE)
03338 #define sys_outw(p,v) sys_out((p), (unsigned long) (v), DIO_WORD)
03339 #define sys_outl(p,v) sys_out((p), (unsigned long) (v), DIO_LONG)
03340 _PROTOTYPE(int sys_out, (int port, unsigned long value, int type) );
03341
03342 /* Shorthands for sys_in() system call. */
03343 #define sys_inb(p,v) sys_in((p), (unsigned long*) (v), DIO_BYTE)
03344 #define sys_inw(p,v) sys_in((p), (unsigned long*) (v), DIO_WORD)
03345 #define sys_inl(p,v) sys_in((p), (unsigned long*) (v), DIO_LONG)
03346 _PROTOTYPE(int sys_in, (int port, unsigned long *value, int type) );
03347
03348 #endif /* _SYSLIB_H */
03349
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/sysutil.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
03400 #ifndef _EXTRALIB_H
03401 #define _EXTRALIB_H
03402
03403 /* Extra system library definitions to support device drivers and servers.
03404 *
03405 * Created:
03406 * Mar 15, 2004 by Jorrit N. Herder
03407 *
03408 * Changes:
03409 * May 31, 2005: added printf, kputc (relocated from syslib)
03410 * May 31, 2005: added getuptime
03411 * Mar 18, 2005: added tickdelay
03412 * Oct 01, 2004: added env_parse, env_prefix, env_panic
03413 * Jul 13, 2004: added fkey_ctl
03414 * Apr 28, 2004: added report, panic
676 File: include/minix/sysutil.h MINIX SOURCE CODE
03415 * Mar 31, 2004: setup like other libraries, such as syslib
03416 */
03417
03418 /*==========================================================================*
03419 * Miscellaneous helper functions.
03420 *==========================================================================*/
03421
03422 /* Environment parsing return values. */
03423 #define EP_BUF_SIZE 128 /* local buffer for env value */
03424 #define EP_UNSET 0 /* variable not set */
03425 #define EP_OFF 1 /* var = off */
03426 #define EP_ON 2 /* var = on (or field left blank) */
03427 #define EP_SET 3 /* var = 1:2:3 (nonblank field) */
03428 #define EP_EGETKENV 4 /* sys_getkenv() failed ... */
03429
03430 _PROTOTYPE( void env_setargs, (int argc, char *argv[]) );
03431 _PROTOTYPE( int env_get_param, (char *key, char *value, int max_size) );
03432 _PROTOTYPE( int env_prefix, (char *env, char *prefix) );
03433 _PROTOTYPE( void env_panic, (char *key) );
03434 _PROTOTYPE( int env_parse, (char *env, char *fmt, int field, long *param,
03435 long min, long max) );
03436
03437 #define fkey_map(fkeys, sfkeys) fkey_ctl(FKEY_MAP, (fkeys), (sfkeys))
03438 #define fkey_unmap(fkeys, sfkeys) fkey_ctl(FKEY_UNMAP, (fkeys), (sfkeys))
03439 #define fkey_events(fkeys, sfkeys) fkey_ctl(FKEY_EVENTS, (fkeys), (sfkeys))
03440 _PROTOTYPE( int fkey_ctl, (int req, int *fkeys, int *sfkeys) );
03441
03442 _PROTOTYPE( int printf, (const char *fmt, ...));
03443 _PROTOTYPE( void kputc, (int c));
03444 _PROTOTYPE( void report, (char *who, char *mess, int num));
03445 _PROTOTYPE( void panic, (char *who, char *mess, int num));
03446 _PROTOTYPE( int getuptime, (clock_t *ticks));
03447 _PROTOTYPE( int tickdelay, (clock_t ticks));
03448
03449 #endif /* _EXTRALIB_H */
03450
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/callnr.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
03500 #define NCALLS 91 /* number of system calls allowed */
03501
03502 #define EXIT 1
03503 #define FORK 2
03504 #define READ 3
03505 #define WRITE 4
03506 #define OPEN 5
03507 #define CLOSE 6
03508 #define WAIT 7
03509 #define CREAT 8
03510 #define LINK 9
03511 #define UNLINK 10
03512 #define WAITPID 11
03513 #define CHDIR 12
03514 #define TIME 13
MINIX SOURCE CODE File: include/minix/callnr.h 677
03515 #define MKNOD 14
03516 #define CHMOD 15
03517 #define CHOWN 16
03518 #define BRK 17
03519 #define STAT 18
03520 #define LSEEK 19
03521 #define GETPID 20
03522 #define MOUNT 21
03523 #define UMOUNT 22
03524 #define SETUID 23
03525 #define GETUID 24
03526 #define STIME 25
03527 #define PTRACE 26
03528 #define ALARM 27
03529 #define FSTAT 28
03530 #define PAUSE 29
03531 #define UTIME 30
03532 #define ACCESS 33
03533 #define SYNC 36
03534 #define KILL 37
03535 #define RENAME 38
03536 #define MKDIR 39
03537 #define RMDIR 40
03538 #define DUP 41
03539 #define PIPE 42
03540 #define TIMES 43
03541 #define SETGID 46
03542 #define GETGID 47
03543 #define SIGNAL 48
03544 #define IOCTL 54
03545 #define FCNTL 55
03546 #define EXEC 59
03547 #define UMASK 60
03548 #define CHROOT 61
03549 #define SETSID 62
03550 #define GETPGRP 63
03551
03552 /* The following are not system calls, but are processed like them. */
03553 #define UNPAUSE 65 /* to MM or FS: check for EINTR */
03554 #define REVIVE 67 /* to FS: revive a sleeping process */
03555 #define TASK_REPLY 68 /* to FS: reply code from tty task */
03556
03557 /* Posix signal handling. */
03558 #define SIGACTION 71
03559 #define SIGSUSPEND 72
03560 #define SIGPENDING 73
03561 #define SIGPROCMASK 74
03562 #define SIGRETURN 75
03563
03564 #define REBOOT 76 /* to PM */
03565
03566 /* MINIX specific calls, e.g., to support system services. */
03567 #define SVRCTL 77
03568 /* unused */
03569 #define GETSYSINFO 79 /* to PM or FS */
03570 #define GETPROCNR 80 /* to PM */
03571 #define DEVCTL 81 /* to FS */
03572 #define FSTATFS 82 /* to FS */
03573 #define ALLOCMEM 83 /* to PM */
03574 #define FREEMEM 84 /* to PM */
678 File: include/minix/callnr.h MINIX SOURCE CODE
03575 #define SELECT 85 /* to FS */
03576 #define FCHDIR 86 /* to FS */
03577 #define FSYNC 87 /* to FS */
03578 #define GETPRIORITY 88 /* to PM */
03579 #define SETPRIORITY 89 /* to PM */
03580 #define GETTIMEOFDAY 90 /* to PM */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/com.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
03600 #ifndef _MINIX_COM_H
03601 #define _MINIX_COM_H
03602
03603 /*===========================================================================*
03604 * Magic process numbers *
03605 *===========================================================================*/
03606
03607 #define ANY 0x7ace /* used to indicate ’any process’ */
03608 #define NONE 0x6ace /* used to indicate ’no process at all’ */
03609 #define SELF 0x8ace /* used to indicate ’own process’ */
03610
03611 /*===========================================================================*
03612 * Process numbers of processes in the system image *
03613 *===========================================================================*/
03614
03615 /* The values of several task numbers depend on whether they or other tasks
03616 * are enabled. They are defined as (PREVIOUS_TASK - ENABLE_TASK) in general.
03617 * ENABLE_TASK is either 0 or 1, so a task either gets a new number, or gets
03618 * the same number as the previous task and is further unused. Note that the
03619 * order should correspond to the order in the task table defined in table.c.
03620 */
03621
03622 /* Kernel tasks. These all run in the same address space. */
03623 #define IDLE -4 /* runs when no one else can run */
03624 #define CLOCK -3 /* alarms and other clock functions */
03625 #define SYSTEM -2 /* request system functionality */
03626 #define KERNEL -1 /* pseudo-process for IPC and scheduling */
03627 #define HARDWARE KERNEL /* for hardware interrupt handlers */
03628
03629 /* Number of tasks. Note that NR_PROCS is defined in <minix/config.h>. */
03630 #define NR_TASKS 4
03631
03632 /* User-space processes, that is, device drivers, servers, and INIT. */
03633 #define PM_PROC_NR 0 /* process manager */
03634 #define FS_PROC_NR 1 /* file system */
03635 #define RS_PROC_NR 2 /* reincarnation server */
03636 #define MEM_PROC_NR 3 /* memory driver (RAM disk, null, etc.) */
03637 #define LOG_PROC_NR 4 /* log device driver */
03638 #define TTY_PROC_NR 5 /* terminal (TTY) driver */
03639 #define DRVR_PROC_NR 6 /* device driver for boot medium */
03640 #define INIT_PROC_NR 7 /* init -- goes multiuser */
03641
03642 /* Number of processes contained in the system image. */
03643 #define NR_BOOT_PROCS (NR_TASKS + INIT_PROC_NR + 1)
03644
MINIX SOURCE CODE File: include/minix/com.h 679
03645 /*===========================================================================*
03646 * Kernel notification types *
03647 *===========================================================================*/
03648
03649 /* Kernel notification types. In principle, these can be sent to any process,
03650 * so make sure that these types do not interfere with other message types.
03651 * Notifications are prioritized because of the way they are unhold() and
03652 * blocking notifications are delivered. The lowest numbers go first. The
03653 * offset are used for the per-process notification bit maps.
03654 */
03655 #define NOTIFY_MESSAGE 0x1000
03656 #define NOTIFY_FROM(p_nr) (NOTIFY_MESSAGE | ((p_nr) + NR_TASKS))
03657 # define SYN_ALARM NOTIFY_FROM(CLOCK) /* synchronous alarm */
03658 # define SYS_SIG NOTIFY_FROM(SYSTEM) /* system signal */
03659 # define HARD_INT NOTIFY_FROM(HARDWARE) /* hardware interrupt */
03660 # define NEW_KSIG NOTIFY_FROM(HARDWARE) /* new kernel signal */
03661 # define FKEY_PRESSED NOTIFY_FROM(TTY_PROC_NR)/* function key press */
03662
03663 /* Shorthands for message parameters passed with notifications. */
03664 #define NOTIFY_SOURCE m_source
03665 #define NOTIFY_TYPE m_type
03666 #define NOTIFY_ARG m2_l1
03667 #define NOTIFY_TIMESTAMP m2_l2
03668 #define NOTIFY_FLAGS m2_i1
03669
03670 /*===========================================================================*
03671 * Messages for BLOCK and CHARACTER device drivers *
03672 *===========================================================================*/
03673
03674 /* Message types for device drivers. */
03675 #define DEV_RQ_BASE 0x400 /* base for device request types */
03676 #define DEV_RS_BASE 0x500 /* base for device response types */
03677
03678 #define CANCEL (DEV_RQ_BASE + 0) /* general req to force a task to cancel */
03679 #define DEV_READ (DEV_RQ_BASE + 3) /* read from minor device */
03680 #define DEV_WRITE (DEV_RQ_BASE + 4) /* write to minor device */
03681 #define DEV_IOCTL (DEV_RQ_BASE + 5) /* I/O control code */
03682 #define DEV_OPEN (DEV_RQ_BASE + 6) /* open a minor device */
03683 #define DEV_CLOSE (DEV_RQ_BASE + 7) /* close a minor device */
03684 #define DEV_SCATTER (DEV_RQ_BASE + 8) /* write from a vector */
03685 #define DEV_GATHER (DEV_RQ_BASE + 9) /* read into a vector */
03686 #define TTY_SETPGRP (DEV_RQ_BASE + 10) /* set process group */
03687 #define TTY_EXIT (DEV_RQ_BASE + 11) /* process group leader exited */
03688 #define DEV_SELECT (DEV_RQ_BASE + 12) /* request select() attention */
03689 #define DEV_STATUS (DEV_RQ_BASE + 13) /* request driver status */
03690
03691 #define DEV_REPLY (DEV_RS_BASE + 0) /* general task reply */
03692 #define DEV_CLONED (DEV_RS_BASE + 1) /* return cloned minor */
03693 #define DEV_REVIVE (DEV_RS_BASE + 2) /* driver revives process */
03694 #define DEV_IO_READY (DEV_RS_BASE + 3) /* selected device ready */
03695 #define DEV_NO_STATUS (DEV_RS_BASE + 4) /* empty status reply */
03696
03697 /* Field names for messages to block and character device drivers. */
03698 #define DEVICE m2_i1 /* major-minor device */
03699 #define PROC_NR m2_i2 /* which (proc) wants I/O? */
03700 #define COUNT m2_i3 /* how many bytes to transfer */
03701 #define REQUEST m2_i3 /* ioctl request code */
03702 #define POSITION m2_l1 /* file offset */
03703 #define ADDRESS m2_p1 /* core buffer address */
03704
680 File: include/minix/com.h MINIX SOURCE CODE
03705 /* Field names for DEV_SELECT messages to device drivers. */
03706 #define DEV_MINOR m2_i1 /* minor device */
03707 #define DEV_SEL_OPS m2_i2 /* which select operations are requested */
03708 #define DEV_SEL_WATCH m2_i3 /* request notify if no operations are ready */
03709
03710 /* Field names used in reply messages from tasks. */
03711 #define REP_PROC_NR m2_i1 /* # of proc on whose behalf I/O was done */
03712 #define REP_STATUS m2_i2 /* bytes transferred or error number */
03713 # define SUSPEND -998 /* status to suspend caller, reply later */
03714
03715 /* Field names for messages to TTY driver. */
03716 #define TTY_LINE DEVICE /* message parameter: terminal line */
03717 #define TTY_REQUEST COUNT /* message parameter: ioctl request code */
03718 #define TTY_SPEK POSITION/* message parameter: ioctl speed, erasing */
03719 #define TTY_FLAGS m2_l2 /* message parameter: ioctl tty mode */
03720 #define TTY_PGRP m2_i3 /* message parameter: process group */
03721
03722 /* Field names for the QIC 02 status reply from tape driver */
03723 #define TAPE_STAT0 m2_l1
03724 #define TAPE_STAT1 m2_l2
03725
03726 /*===========================================================================*
03727 * Messages for networking layer *
03728 *===========================================================================*/
03729
03730 /* Message types for network layer requests. This layer acts like a driver. */
03731 #define NW_OPEN DEV_OPEN
03732 #define NW_CLOSE DEV_CLOSE
03733 #define NW_READ DEV_READ
03734 #define NW_WRITE DEV_WRITE
03735 #define NW_IOCTL DEV_IOCTL
03736 #define NW_CANCEL CANCEL
03737
03738 /* Base type for data link layer requests and responses. */
03739 #define DL_RQ_BASE 0x800
03740 #define DL_RS_BASE 0x900
03741
03742 /* Message types for data link layer requests. */
03743 #define DL_WRITE (DL_RQ_BASE + 3)
03744 #define DL_WRITEV (DL_RQ_BASE + 4)
03745 #define DL_READ (DL_RQ_BASE + 5)
03746 #define DL_READV (DL_RQ_BASE + 6)
03747 #define DL_INIT (DL_RQ_BASE + 7)
03748 #define DL_STOP (DL_RQ_BASE + 8)
03749 #define DL_GETSTAT (DL_RQ_BASE + 9)
03750
03751 /* Message type for data link layer replies. */
03752 #define DL_INIT_REPLY (DL_RS_BASE + 20)
03753 #define DL_TASK_REPLY (DL_RS_BASE + 21)
03754
03755 /* Field names for data link layer messages. */
03756 #define DL_PORT m2_i1
03757 #define DL_PROC m2_i2
03758 #define DL_COUNT m2_i3
03759 #define DL_MODE m2_l1
03760 #define DL_CLCK m2_l2
03761 #define DL_ADDR m2_p1
03762 #define DL_STAT m2_l1
03763
03764 /* Bits in ’DL_STAT’ field of DL replies. */
MINIX SOURCE CODE File: include/minix/com.h 681
03765 # define DL_PACK_SEND 0x01
03766 # define DL_PACK_RECV 0x02
03767 # define DL_READ_IP 0x04
03768
03769 /* Bits in ’DL_MODE’ field of DL requests. */
03770 # define DL_NOMODE 0x0
03771 # define DL_PROMISC_REQ 0x2
03772 # define DL_MULTI_REQ 0x4
03773 # define DL_BROAD_REQ 0x8
03774
03775 /*===========================================================================*
03776 * SYSTASK request types and field names *
03777 *===========================================================================*/
03778
03779 /* System library calls are dispatched via a call vector, so be careful when
03780 * modifying the system call numbers. The numbers here determine which call
03781 * is made from the call vector.
03782 */
03783 #define KERNEL_CALL 0x600 /* base for kernel calls to SYSTEM */
03784
03785 # define SYS_FORK (KERNEL_CALL + 0) /* sys_fork() */
03786 # define SYS_EXEC (KERNEL_CALL + 1) /* sys_exec() */
03787 # define SYS_EXIT (KERNEL_CALL + 2) /* sys_exit() */
03788 # define SYS_NICE (KERNEL_CALL + 3) /* sys_nice() */
03789 # define SYS_PRIVCTL (KERNEL_CALL + 4) /* sys_privctl() */
03790 # define SYS_TRACE (KERNEL_CALL + 5) /* sys_trace() */
03791 # define SYS_KILL (KERNEL_CALL + 6) /* sys_kill() */
03792
03793 # define SYS_GETKSIG (KERNEL_CALL + 7) /* sys_getsig() */
03794 # define SYS_ENDKSIG (KERNEL_CALL + 8) /* sys_endsig() */
03795 # define SYS_SIGSEND (KERNEL_CALL + 9) /* sys_sigsend() */
03796 # define SYS_SIGRETURN (KERNEL_CALL + 10) /* sys_sigreturn() */
03797
03798 # define SYS_NEWMAP (KERNEL_CALL + 11) /* sys_newmap() */
03799 # define SYS_SEGCTL (KERNEL_CALL + 12) /* sys_segctl() */
03800 # define SYS_MEMSET (KERNEL_CALL + 13) /* sys_memset() */
03801
03802 # define SYS_UMAP (KERNEL_CALL + 14) /* sys_umap() */
03803 # define SYS_VIRCOPY (KERNEL_CALL + 15) /* sys_vircopy() */
03804 # define SYS_PHYSCOPY (KERNEL_CALL + 16) /* sys_physcopy() */
03805 # define SYS_VIRVCOPY (KERNEL_CALL + 17) /* sys_virvcopy() */
03806 # define SYS_PHYSVCOPY (KERNEL_CALL + 18) /* sys_physvcopy() */
03807
03808 # define SYS_IRQCTL (KERNEL_CALL + 19) /* sys_irqctl() */
03809 # define SYS_INT86 (KERNEL_CALL + 20) /* sys_int86() */
03810 # define SYS_DEVIO (KERNEL_CALL + 21) /* sys_devio() */
03811 # define SYS_SDEVIO (KERNEL_CALL + 22) /* sys_sdevio() */
03812 # define SYS_VDEVIO (KERNEL_CALL + 23) /* sys_vdevio() */
03813
03814 # define SYS_SETALARM (KERNEL_CALL + 24) /* sys_setalarm() */
03815 # define SYS_TIMES (KERNEL_CALL + 25) /* sys_times() */
03816 # define SYS_GETINFO (KERNEL_CALL + 26) /* sys_getinfo() */
03817 # define SYS_ABORT (KERNEL_CALL + 27) /* sys_abort() */
03818
03819 #define NR_SYS_CALLS 28 /* number of system calls */
03820
03821 /* Field names for SYS_MEMSET, SYS_SEGCTL. */
03822 #define MEM_PTR m2_p1 /* base */
03823 #define MEM_COUNT m2_l1 /* count */
03824 #define MEM_PATTERN m2_l2 /* pattern to write */
682 File: include/minix/com.h MINIX SOURCE CODE
03825 #define MEM_CHUNK_BASE m4_l1 /* physical base address */
03826 #define MEM_CHUNK_SIZE m4_l2 /* size of mem chunk */
03827 #define MEM_TOT_SIZE m4_l3 /* total memory size */
03828 #define MEM_CHUNK_TAG m4_l4 /* tag to identify chunk of mem */
03829
03830 /* Field names for SYS_DEVIO, SYS_VDEVIO, SYS_SDEVIO. */
03831 #define DIO_REQUEST m2_i3 /* device in or output */
03832 # define DIO_INPUT 0 /* input */
03833 # define DIO_OUTPUT 1 /* output */
03834 #define DIO_TYPE m2_i1 /* flag indicating byte, word, or long */
03835 # define DIO_BYTE ’b’ /* byte type values */
03836 # define DIO_WORD ’w’ /* word type values */
03837 # define DIO_LONG ’l’ /* long type values */
03838 #define DIO_PORT m2_l1 /* single port address */
03839 #define DIO_VALUE m2_l2 /* single I/O value */
03840 #define DIO_VEC_ADDR m2_p1 /* address of buffer or (p,v)-pairs */
03841 #define DIO_VEC_SIZE m2_l2 /* number of elements in vector */
03842 #define DIO_VEC_PROC m2_i2 /* number of process where vector is */
03843
03844 /* Field names for SYS_SIGNARLM, SYS_FLAGARLM, SYS_SYNCALRM. */
03845 #define ALRM_EXP_TIME m2_l1 /* expire time for the alarm call */
03846 #define ALRM_ABS_TIME m2_i2 /* set to 1 to use absolute alarm time */
03847 #define ALRM_TIME_LEFT m2_l1 /* how many ticks were remaining */
03848 #define ALRM_PROC_NR m2_i1 /* which process wants the alarm? */
03849 #define ALRM_FLAG_PTR m2_p1 /* virtual address of timeout flag */
03850
03851 /* Field names for SYS_IRQCTL. */
03852 #define IRQ_REQUEST m5_c1 /* what to do? */
03853 # define IRQ_SETPOLICY 1 /* manage a slot of the IRQ table */
03854 # define IRQ_RMPOLICY 2 /* remove a slot of the IRQ table */
03855 # define IRQ_ENABLE 3 /* enable interrupts */
03856 # define IRQ_DISABLE 4 /* disable interrupts */
03857 #define IRQ_VECTOR m5_c2 /* irq vector */
03858 #define IRQ_POLICY m5_i1 /* options for IRQCTL request */
03859 # define IRQ_REENABLE 0x001 /* reenable IRQ line after interrupt */
03860 # define IRQ_BYTE 0x100 /* byte values */
03861 # define IRQ_WORD 0x200 /* word values */
03862 # define IRQ_LONG 0x400 /* long values */
03863 #define IRQ_PROC_NR m5_i2 /* process number, SELF, NONE */
03864 #define IRQ_HOOK_ID m5_l3 /* id of irq hook at kernel */
03865
03866 /* Field names for SYS_SEGCTL. */
03867 #define SEG_SELECT m4_l1 /* segment selector returned */
03868 #define SEG_OFFSET m4_l2 /* offset in segment returned */
03869 #define SEG_PHYS m4_l3 /* physical address of segment */
03870 #define SEG_SIZE m4_l4 /* segment size */
03871 #define SEG_INDEX m4_l5 /* segment index in remote map */
03872
03873 /* Field names for SYS_VIDCOPY. */
03874 #define VID_REQUEST m4_l1 /* what to do? */
03875 # define VID_VID_COPY 1 /* request vid_vid_copy() */
03876 # define MEM_VID_COPY 2 /* request mem_vid_copy() */
03877 #define VID_SRC_ADDR m4_l2 /* virtual address in memory */
03878 #define VID_SRC_OFFSET m4_l3 /* offset in video memory */
03879 #define VID_DST_OFFSET m4_l4 /* offset in video memory */
03880 #define VID_CP_COUNT m4_l5 /* number of words to be copied */
03881
03882 /* Field names for SYS_ABORT. */
03883 #define ABRT_HOW m1_i1 /* RBT_REBOOT, RBT_HALT, etc. */
03884 #define ABRT_MON_PROC m1_i2 /* process where monitor params are */
MINIX SOURCE CODE File: include/minix/com.h 683
03885 #define ABRT_MON_LEN m1_i3 /* length of monitor params */
03886 #define ABRT_MON_ADDR m1_p1 /* virtual address of monitor params */
03887
03888 /* Field names for _UMAP, _VIRCOPY, _PHYSCOPY. */
03889 #define CP_SRC_SPACE m5_c1 /* T or D space (stack is also D) */
03890 #define CP_SRC_PROC_NR m5_i1 /* process to copy from */
03891 #define CP_SRC_ADDR m5_l1 /* address where data come from */
03892 #define CP_DST_SPACE m5_c2 /* T or D space (stack is also D) */
03893 #define CP_DST_PROC_NR m5_i2 /* process to copy to */
03894 #define CP_DST_ADDR m5_l2 /* address where data go to */
03895 #define CP_NR_BYTES m5_l3 /* number of bytes to copy */
03896
03897 /* Field names for SYS_VCOPY and SYS_VVIRCOPY. */
03898 #define VCP_NR_OK m1_i2 /* number of successfull copies */
03899 #define VCP_VEC_SIZE m1_i3 /* size of copy vector */
03900 #define VCP_VEC_ADDR m1_p1 /* pointer to copy vector */
03901
03902 /* Field names for SYS_GETINFO. */
03903 #define I_REQUEST m7_i3 /* what info to get */
03904 # define GET_KINFO 0 /* get kernel information structure */
03905 # define GET_IMAGE 1 /* get system image table */
03906 # define GET_PROCTAB 2 /* get kernel process table */
03907 # define GET_RANDOMNESS 3 /* get randomness buffer */
03908 # define GET_MONPARAMS 4 /* get monitor parameters */
03909 # define GET_KENV 5 /* get kernel environment string */
03910 # define GET_IRQHOOKS 6 /* get the IRQ table */
03911 # define GET_KMESSAGES 7 /* get kernel messages */
03912 # define GET_PRIVTAB 8 /* get kernel privileges table */
03913 # define GET_KADDRESSES 9 /* get various kernel addresses */
03914 # define GET_SCHEDINFO 10 /* get scheduling queues */
03915 # define GET_PROC 11 /* get process slot if given process */
03916 # define GET_MACHINE 12 /* get machine information */
03917 # define GET_LOCKTIMING 13 /* get lock()/unlock() latency timing */
03918 # define GET_BIOSBUFFER 14 /* get a buffer for BIOS calls */
03919 #define I_PROC_NR m7_i4 /* calling process */
03920 #define I_VAL_PTR m7_p1 /* virtual address at caller */
03921 #define I_VAL_LEN m7_i1 /* max length of value */
03922 #define I_VAL_PTR2 m7_p2 /* second virtual address */
03923 #define I_VAL_LEN2 m7_i2 /* second length, or proc nr */
03924
03925 /* Field names for SYS_TIMES. */
03926 #define T_PROC_NR m4_l1 /* process to request time info for */
03927 #define T_USER_TIME m4_l1 /* user time consumed by process */
03928 #define T_SYSTEM_TIME m4_l2 /* system time consumed by process */
03929 #define T_CHILD_UTIME m4_l3 /* user time consumed by process’ children */
03930 #define T_CHILD_STIME m4_l4 /* sys time consumed by process’ children */
03931 #define T_BOOT_TICKS m4_l5 /* number of clock ticks since boot time */
03932
03933 /* Field names for SYS_TRACE, SYS_SVRCTL. */
03934 #define CTL_PROC_NR m2_i1 /* process number of the caller */
03935 #define CTL_REQUEST m2_i2 /* server control request */
03936 #define CTL_MM_PRIV m2_i3 /* privilege as seen by PM */
03937 #define CTL_ARG_PTR m2_p1 /* pointer to argument */
03938 #define CTL_ADDRESS m2_l1 /* address at traced process’ space */
03939 #define CTL_DATA m2_l2 /* data field for tracing */
03940
03941 /* Field names for SYS_KILL, SYS_SIGCTL */
03942 #define SIG_REQUEST m2_l2 /* PM signal control request */
03943 #define S_GETSIG 0 /* get pending kernel signal */
03944 #define S_ENDSIG 1 /* finish a kernel signal */
684 File: include/minix/com.h MINIX SOURCE CODE
03945 #define S_SENDSIG 2 /* POSIX style signal handling */
03946 #define S_SIGRETURN 3 /* return from POSIX handling */
03947 #define S_KILL 4 /* servers kills process with signal */
03948 #define SIG_PROC m2_i1 /* process number for inform */
03949 #define SIG_NUMBER m2_i2 /* signal number to send */
03950 #define SIG_FLAGS m2_i3 /* signal flags field */
03951 #define SIG_MAP m2_l1 /* used by kernel to pass signal bit map */
03952 #define SIG_CTXT_PTR m2_p1 /* pointer to info to restore signal context */
03953
03954 /* Field names for SYS_FORK, _EXEC, _EXIT, _NEWMAP. */
03955 #define PR_PROC_NR m1_i1 /* indicates a (child) process */
03956 #define PR_PRIORITY m1_i2 /* process priority */
03957 #define PR_PPROC_NR m1_i2 /* indicates a (parent) process */
03958 #define PR_PID m1_i3 /* process id at process manager */
03959 #define PR_STACK_PTR m1_p1 /* used for stack ptr in sys_exec, sys_getsp */
03960 #define PR_TRACING m1_i3 /* flag to indicate tracing is on/ off */
03961 #define PR_NAME_PTR m1_p2 /* tells where program name is for dmp */
03962 #define PR_IP_PTR m1_p3 /* initial value for ip after exec */
03963 #define PR_MEM_PTR m1_p1 /* tells where memory map is for sys_newmap */
03964
03965 /* Field names for SYS_INT86 */
03966 #define INT86_REG86 m1_p1 /* pointer to registers */
03967
03968 /* Field names for SELECT (FS). */
03969 #define SEL_NFDS m8_i1
03970 #define SEL_READFDS m8_p1
03971 #define SEL_WRITEFDS m8_p2
03972 #define SEL_ERRORFDS m8_p3
03973 #define SEL_TIMEOUT m8_p4
03974
03975 /*===========================================================================*
03976 * Messages for system management server *
03977 *===========================================================================*/
03978
03979 #define SRV_RQ_BASE 0x700
03980
03981 #define SRV_UP (SRV_RQ_BASE + 0) /* start system service */
03982 #define SRV_DOWN (SRV_RQ_BASE + 1) /* stop system service */
03983 #define SRV_STATUS (SRV_RQ_BASE + 2) /* get service status */
03984
03985 # define SRV_PATH_ADDR m1_p1 /* path of binary */
03986 # define SRV_PATH_LEN m1_i1 /* length of binary */
03987 # define SRV_ARGS_ADDR m1_p2 /* arguments to be passed */
03988 # define SRV_ARGS_LEN m1_i2 /* length of arguments */
03989 # define SRV_DEV_MAJOR m1_i3 /* major device number */
03990 # define SRV_PRIV_ADDR m1_p3 /* privileges string */
03991 # define SRV_PRIV_LEN m1_i3 /* length of privileges */
03992
03993 /*===========================================================================*
03994 * Miscellaneous messages used by TTY *
03995 *===========================================================================*/
03996
03997 /* Miscellaneous request types and field names, e.g. used by IS server. */
03998 #define PANIC_DUMPS 97 /* debug dumps at the TTY on RBT_PANIC */
03999 #define FKEY_CONTROL 98 /* control a function key at the TTY */
04000 # define FKEY_REQUEST m2_i1 /* request to perform at TTY */
04001 # define FKEY_MAP 10 /* observe function key */
04002 # define FKEY_UNMAP 11 /* stop observing function key */
04003 # define FKEY_EVENTS 12 /* request open key presses */
04004 # define FKEY_FKEYS m2_l1 /* F1-F12 keys pressed */
MINIX SOURCE CODE File: include/minix/com.h 685
04005 # define FKEY_SFKEYS m2_l2 /* Shift-F1-F12 keys pressed */
04006 #define DIAGNOSTICS 100 /* output a string without FS in between */
04007 # define DIAG_PRINT_BUF m1_p1
04008 # define DIAG_BUF_COUNT m1_i1
04009 # define DIAG_PROC_NR m1_i2
04010
04011 #endif /* _MINIX_COM_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/devio.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
04100 /* This file provides basic types and some constants for the
04101 * SYS_DEVIO and SYS_VDEVIO system calls, which allow user-level
04102 * processes to perform device I/O.
04103 *
04104 * Created:
04105 * Apr 08, 2004 by Jorrit N. Herder
04106 */
04107
04108 #ifndef _DEVIO_H
04109 #define _DEVIO_H
04110
04111 #include <minix/sys_config.h> /* needed to include <minix/type.h> */
04112 #include <sys/types.h> /* u8_t, u16_t, u32_t needed */
04113
04114 typedef u16_t port_t;
04115 typedef U16_t Port_t;
04116
04117 /* We have different granularities of port I/O: 8, 16, 32 bits.
04118 * Also see <ibm/portio.h>, which has functions for bytes, words,
04119 * and longs. Hence, we need different (port,value)-pair types.
04120 */
04121 typedef struct { u16_t port; u8_t value; } pvb_pair_t;
04122 typedef struct { u16_t port; u16_t value; } pvw_pair_t;
04123 typedef struct { u16_t port; u32_t value; } pvl_pair_t;
04124
04125 /* Macro shorthand to set (port,value)-pair. */
04126 #define pv_set(pv, p, v) ((pv).port = (p), (pv).value = (v))
04127 #define pv_ptr_set(pv_ptr, p, v) ((pv_ptr)->port = (p), (pv_ptr)->value = (v))
04128
04129 #endif /* _DEVIO_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/dmap.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
04200 #ifndef _DMAP_H
04201 #define _DMAP_H
04202
04203 #include <minix/sys_config.h>
04204 #include <minix/ipc.h>
04205
686 File: include/minix/dmap.h MINIX SOURCE CODE
04206 /*===========================================================================*
04207 * Device <-> Driver Table *
04208 *===========================================================================*/
04209
04210 /* Device table. This table is indexed by major device number. It provides
04211 * the link between major device numbers and the routines that process them.
04212 * The table can be update dynamically. The field ’dmap_flags’ describe an
04213 * entry’s current status and determines what control options are possible.
04214 */
04215 #define DMAP_MUTABLE 0x01 /* mapping can be overtaken */
04216 #define DMAP_BUSY 0x02 /* driver busy with request */
04217
04218 enum dev_style { STYLE_DEV, STYLE_NDEV, STYLE_TTY, STYLE_CLONE };
04219
04220 extern struct dmap {
04221 int _PROTOTYPE ((*dmap_opcl), (int, Dev_t, int, int) );
04222 void _PROTOTYPE ((*dmap_io), (int, message *) );
04223 int dmap_driver;
04224 int dmap_flags;
04225 } dmap[];
04226
04227 /*===========================================================================*
04228 * Major and minor device numbers *
04229 *===========================================================================*/
04230
04231 /* Total number of different devices. */
04232 #define NR_DEVICES 32 /* number of (major) devices */
04233
04234 /* Major and minor device numbers for MEMORY driver. */
04235 #define MEMORY_MAJOR 1 /* major device for memory devices */
04236 # define RAM_DEV 0 /* minor device for /dev/ram */
04237 # define MEM_DEV 1 /* minor device for /dev/mem */
04238 # define KMEM_DEV 2 /* minor device for /dev/kmem */
04239 # define NULL_DEV 3 /* minor device for /dev/null */
04240 # define BOOT_DEV 4 /* minor device for /dev/boot */
04241 # define ZERO_DEV 5 /* minor device for /dev/zero */
04242
04243 #define CTRLR(n) ((n)==0 ? 3 : (8 + 2*((n)-1))) /* magic formula */
04244
04245 /* Full device numbers that are special to the boot monitor and FS. */
04246 # define DEV_RAM 0x0100 /* device number of /dev/ram */
04247 # define DEV_BOOT 0x0104 /* device number of /dev/boot */
04248
04249 #define FLOPPY_MAJOR 2 /* major device for floppy disks */
04250 #define TTY_MAJOR 4 /* major device for ttys */
04251 #define CTTY_MAJOR 5 /* major device for /dev/tty */
04252
04253 #define INET_MAJOR 7 /* major device for inet */
04254
04255 #define LOG_MAJOR 15 /* major device for log driver */
04256 # define IS_KLOG_DEV 0 /* minor device for /dev/klog */
04257
04258 #endif /* _DMAP_H */
MINIX SOURCE CODE File: include/ibm/portio.h 687
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/ibm/portio.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
04300 /*
04301 ibm/portio.h
04302
04303 Created: Jan 15, 1992 by Philip Homburg
04304 */
04305
04306 #ifndef _PORTIO_H_
04307 #define _PORTIO_H_
04308
04309 #ifndef _TYPES_H
04310 #include <sys/types.h>
04311 #endif
04312
04313 unsigned inb(U16_t _port);
04314 unsigned inw(U16_t _port);
04315 unsigned inl(U32_t _port);
04316 void outb(U16_t _port, U8_t _value);
04317 void outw(U16_t _port, U16_t _value);
04318 void outl(U16_t _port, U32_t _value);
04319 void insb(U16_t _port, void *_buf, size_t _count);
04320 void insw(U16_t _port, void *_buf, size_t _count);
04321 void insl(U16_t _port, void *_buf, size_t _count);
04322 void outsb(U16_t _port, void *_buf, size_t _count);
04323 void outsw(U16_t _port, void *_buf, size_t _count);
04324 void outsl(U16_t _port, void *_buf, size_t _count);
04325 void intr_disable(void);
04326 void intr_enable(void);
04327
04328 #endif /* _PORTIO_H_ */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/ibm/interrupt.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
04400 /* Interrupt numbers and hardware vectors. */
04401
04402 #ifndef _INTERRUPT_H
04403 #define _INTERRUPT_H
04404
04405 #if (CHIP == INTEL)
04406
04407 /* 8259A interrupt controller ports. */
04408 #define INT_CTL 0x20 /* I/O port for interrupt controller */
04409 #define INT_CTLMASK 0x21 /* setting bits in this port disables ints */
04410 #define INT2_CTL 0xA0 /* I/O port for second interrupt controller */
04411 #define INT2_CTLMASK 0xA1 /* setting bits in this port disables ints */
04412
04413 /* Magic numbers for interrupt controller. */
04414 #define END_OF_INT 0x20 /* code used to re-enable after an interrupt */
04415
04416 /* Interrupt vectors defined/reserved by processor. */
04417 #define DIVIDE_VECTOR 0 /* divide error */
04418 #define DEBUG_VECTOR 1 /* single step (trace) */
04419 #define NMI_VECTOR 2 /* non-maskable interrupt */
688 File: include/ibm/interrupt.h MINIX SOURCE CODE
04420 #define BREAKPOINT_VECTOR 3 /* software breakpoint */
04421 #define OVERFLOW_VECTOR 4 /* from INTO */
04422
04423 /* Fixed system call vector. */
04424 #define SYS_VECTOR 32 /* system calls are made with int SYSVEC */
04425 #define SYS386_VECTOR 33 /* except 386 system calls use this */
04426 #define LEVEL0_VECTOR 34 /* for execution of a function at level 0 */
04427
04428 /* Suitable irq bases for hardware interrupts. Reprogram the 8259(s) from
04429 * the PC BIOS defaults since the BIOS doesn’t respect all the processor’s
04430 * reserved vectors (0 to 31).
04431 */
04432 #define BIOS_IRQ0_VEC 0x08 /* base of IRQ0-7 vectors used by BIOS */
04433 #define BIOS_IRQ8_VEC 0x70 /* base of IRQ8-15 vectors used by BIOS */
04434 #define IRQ0_VECTOR 0x50 /* nice vectors to relocate IRQ0-7 to */
04435 #define IRQ8_VECTOR 0x70 /* no need to move IRQ8-15 */
04436
04437 /* Hardware interrupt numbers. */
04438 #define NR_IRQ_VECTORS 16
04439 #define CLOCK_IRQ 0
04440 #define KEYBOARD_IRQ 1
04441 #define CASCADE_IRQ 2 /* cascade enable for 2nd AT controller */
04442 #define ETHER_IRQ 3 /* default ethernet interrupt vector */
04443 #define SECONDARY_IRQ 3 /* RS232 interrupt vector for port 2 */
04444 #define RS232_IRQ 4 /* RS232 interrupt vector for port 1 */
04445 #define XT_WINI_IRQ 5 /* xt winchester */
04446 #define FLOPPY_IRQ 6 /* floppy disk */
04447 #define PRINTER_IRQ 7
04448 #define AT_WINI_0_IRQ 14 /* at winchester controller 0 */
04449 #define AT_WINI_1_IRQ 15 /* at winchester controller 1 */
04450
04451 /* Interrupt number to hardware vector. */
04452 #define BIOS_VECTOR(irq) \
04453 (((irq) < 8 ? BIOS_IRQ0_VEC : BIOS_IRQ8_VEC) + ((irq) & 0x07))
04454 #define VECTOR(irq) \
04455 (((irq) < 8 ? IRQ0_VECTOR : IRQ8_VECTOR) + ((irq) & 0x07))
04456
04457 #endif /* (CHIP == INTEL) */
04458
04459 #endif /* _INTERRUPT_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/ibm/ports.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
04500 /* Addresses and magic numbers for miscellaneous ports. */
04501
04502 #ifndef _PORTS_H
04503 #define _PORTS_H
04504
04505 #if (CHIP == INTEL)
04506
04507 /* Miscellaneous ports. */
04508 #define PCR 0x65 /* Planar Control Register */
04509 #define PORT_B 0x61 /* I/O port for 8255 port B (kbd, beeper...) */
04510 #define TIMER0 0x40 /* I/O port for timer channel 0 */
04511 #define TIMER2 0x42 /* I/O port for timer channel 2 */
04512 #define TIMER_MODE 0x43 /* I/O port for timer mode control */
04513
04514 #endif /* (CHIP == INTEL) */
MINIX SOURCE CODE File: include/ibm/ports.h 689
04515
04516 #endif /* _PORTS_H */