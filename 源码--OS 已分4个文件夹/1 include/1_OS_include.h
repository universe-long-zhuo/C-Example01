++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             include/ansi.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <ansi.h> header attempts to decide whether the compiler has enough
* conformance to Standard C for Minix to take advantage of. If so, the
* symbol _ANSI is defined (as ). Otherwise _ANSI is not defined
* here, but it may be defined by applications that want to bend the rules.
* The magic number in the definition is to inhibit unnecessary bending
* of the rules. (For consistency with the new ’#ifdef _ANSI" tests in
* the headers, _ANSI should really be defined as nothing, but that would
* break many library routines that use "#if _ANSI".)
* If _ANSI ends up being defined, a macro
*
* _PROTOTYPE(function, params)
*
* is defined. This macro expands in different ways, generating either
* ANSI Standard C prototypes or old-style K&R (Kernighan & Ritchie)
* prototypes, as needed. Finally, some programs use _CONST, _VOIDSTAR etc
* in such a way that they are portable over both ANSI and K&R compilers.
* The appropriate macros are defined here.
*/

/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*/

#ifndef _ANSI_H
#define _ANSI_H

#if __STDC__ == 1
#define _ANSI /* compiler claims full ANSI conformance */
#endif

#ifdef __GNUC__
#define _ANSI /* gcc conforms enough even in non-ANSI mode */
#endif

#ifdef _ANSI

/* Keep everything for ANSI prototypes. */
#define _PROTOTYPE(function, params) function params
#define _ARGS(params) params

#define _VOIDSTAR void *
#define _VOID void
#define _CONST const
#define _VOLATILE volatile
#define _SIZET size_t

#else

/* Throw away the parameters for K&R prototypes. */
#define _PROTOTYPE(function, params) function()
#define _ARGS(params) ()

#define _VOIDSTAR void *
#define _VOID void
#define _CONST
#define _VOLATILE
#define _SIZET int

640 File: include/ansi.h MINIX SOURCE CODE
#endif /* _ANSI */

/* This should be defined as restrict when a C99 compiler is used. */
#define _RESTRICT

/* Setting any of _MINIX, _POSIX_C_SOURCE or _POSIX2_SOURCE implies
* _POSIX_SOURCE. (Seems wrong to put this here in ANSI space.)
*/
#if defined(_MINIX) || _POSIX_C_SOURCE > 0 || defined(_POSIX2_SOURCE)
#undef _POSIX_SOURCE
#define _POSIX_SOURCE 1
#endif

#endif /* ANSI_H */

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             include/limits.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <limits.h> header defines some basic sizes, both of the language types
* (e.g., the number of bits in an integer), and of the operating system (e.g.
* the number of characters in a file name.
*/

#ifndef _LIMITS_H
#define _LIMITS_H

/* Definitions about chars (8 bits in MINIX, and signed). */
#define CHAR_BIT 8 /* # bits in a char */
#define CHAR_MIN -128 /* minimum value of a char */
#define CHAR_MAX 127 /* maximum value of a char */
#define SCHAR_MIN -128 /* minimum value of a signed char */
#define SCHAR_MAX 127 /* maximum value of a signed char */
#define UCHAR_MAX 255 /* maximum value of an unsigned char */
#define MB_LEN_MAX 1 /* maximum length of a multibyte char */

/* Definitions about shorts (16 bits in MINIX). */
#define SHRT_MIN (--1) /* minimum value of a short */
#define SHRT_MAX /* maximum value of a short */
#define USHRT_MAX 0xFFFF /* maximum value of unsigned short */

/* _EM_WSIZE is a compiler-generated symbol giving the word size in bytes. */
#define INT_MIN (--1) /* minimum value of a 32-bit int */
#define INT_MAX /* maximum value of a 32-bit int */
#define UINT_MAX 0xFFFFFFFF /* maximum value of an unsigned 32-bit int */

/*Definitions about longs (32 bits in MINIX). */
#define LONG_MIN (-L-1)/* minimum value of a long */
#define LONG_MAX L /* maximum value of a long */
#define ULONG_MAX 0xFFFFFFFFL /* maximum value of an unsigned long */

#include <sys/dir.h>

/* Minimum sizes required by the POSIX P1003.1 standard (Table 2-3). */
#ifdef _POSIX_SOURCE /* these are only visible for POSIX */
#define _POSIX_ARG_MAX 4096 /* exec() may have 4K worth of args */
#define _POSIX_CHILD_MAX 6 /* a process may have 6 children */
#define _POSIX_LINK_MAX 8 /* a file may have 8 links */
#define _POSIX_MAX_CANON 255 /* size of the canonical input queue */
MINIX SOURCE CODE File: include/limits.h 641
#define _POSIX_MAX_INPUT 255 /* you can type 255 chars ahead */
#define _POSIX_NAME_MAX DIRSIZ /* a file name may have 14 chars */
#define _POSIX_NGROUPS_MAX 0 /* supplementary group IDs are optional */
#define _POSIX_OPEN_MAX 16 /* a process may have 16 files open */
#define _POSIX_PATH_MAX 255 /* a pathname may contain 255 chars */
#define _POSIX_PIPE_BUF 512 /* pipes writes of 512 bytes must be atomic */
#define _POSIX_STREAM_MAX 8 /* at least 8 FILEs can be open at once */
#define _POSIX_TZNAME_MAX 3 /* time zone names can be at least 3 chars */
#define _POSIX_SSIZE_MAX /* read() must support byte reads */

/* Values actually implemented by MINIX (Tables 2-4, 2-5, 2-6, and 2-7). */
/* Some of these old names had better be defined when not POSIX. */
#define _NO_LIMIT 100 /* arbitrary number; limit not enforced */

#define NGROUPS_MAX 0 /* supplemental group IDs not available */
#define ARG_MAX /* # bytes of args + environ for exec() */
#define CHILD_MAX _NO_LIMIT /* MINIX does not limit children */
#define OPEN_MAX 20 /* # open files a process may have */
#define LINK_MAX SHRT_MAX /* # links a file may have */
#define MAX_CANON 255 /* size of the canonical input queue */
#define MAX_INPUT 255 /* size of the type-ahead buffer */
#define NAME_MAX DIRSIZ /* # chars in a file name */
#define PATH_MAX 255 /* # chars in a path name */
#define PIPE_BUF 7168 /* # bytes in atomic write to a pipe */
#define STREAM_MAX 20 /* must be the same as FOPEN_MAX in stdio.h */
#define TZNAME_MAX 3 /* maximum bytes in a time zone name is 3 */
#define SSIZE_MAX /* max defined byte count for read() */

#endif /* _POSIX_SOURCE */

#endif /* _LIMITS_H */

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             include/errno.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <errno.h> header defines the numbers of the various errors that can
* occur during program execution. They are visible to user programs and
* should be small positive integers. However, they are also used within
* MINIX, where they must be negative. For example, the READ system call is
* executed internally by calling do_read(). This function returns either a
* (negative) error number or a (positive) number of bytes actually read.
*
* To solve the problem of having the error numbers be negative inside the
* the system and positive outside, the following mechanism is used. All the
* definitions are are the form:
*
* #define EPERM (_SIGN 1)
*
* If the macro _SYSTEM is defined, then _SIGN is set to "-", otherwise it is
* set to "". Thus when compiling the operating system, the macro _SYSTEM
* will be defined, setting EPERM to (- 1), whereas when when this
* file is included in an ordinary user program, EPERM has the value ( 1).
*/

#ifndef _ERRNO_H /* check if <errno.h> is already included */
642 File: include/errno.h MINIX SOURCE CODE
#define _ERRNO_H /* it is not included; note that fact */

/* Now define _SIGN as "" or "-" depending on _SYSTEM. */
#ifdef _SYSTEM
# define _SIGN -
# define OK 0
#else
# define _SIGN
#endif

extern int errno; /* place where the error numbers go */

/* Here are the numerical values of the error numbers. */
#define _NERROR 70 /* number of errors */

#define EGENERIC (_SIGN 99) /* generic error */
#define EPERM (_SIGN 1) /* operation not permitted */
#define ENOENT (_SIGN 2) /* no such file or directory */
#define ESRCH (_SIGN 3) /* no such process */
#define EINTR (_SIGN 4) /* interrupted function call */
#define EIO (_SIGN 5) /* input/output error */
#define ENXIO (_SIGN 6) /* no such device or address */
#define E2BIG (_SIGN 7) /* arg list too long */
#define ENOEXEC (_SIGN 8) /* exec format error */
#define EBADF (_SIGN 9) /* bad file descriptor */
#define ECHILD (_SIGN 10) /* no child process */
#define EAGAIN (_SIGN 11) /* resource temporarily unavailable */
#define ENOMEM (_SIGN 12) /* not enough space */
#define EACCES (_SIGN 13) /* permission denied */
#define EFAULT (_SIGN 14) /* bad address */
#define ENOTBLK (_SIGN 15) /* Extension: not a block special file */
#define EBUSY (_SIGN 16) /* resource busy */
#define EEXIST (_SIGN 17) /* file exists */
#define EXDEV (_SIGN 18) /* improper link */
#define ENODEV (_SIGN 19) /* no such device */
#define ENOTDIR (_SIGN 20) /* not a directory */
#define EISDIR (_SIGN 21) /* is a directory */
#define EINVAL (_SIGN 22) /* invalid argument */
#define ENFILE (_SIGN 23) /* too many open files in system */
#define EMFILE (_SIGN 24) /* too many open files */
#define ENOTTY (_SIGN 25) /* inappropriate I/O control operation */
#define ETXTBSY (_SIGN 26) /* no longer used */
#define EFBIG (_SIGN 27) /* file too large */
#define ENOSPC (_SIGN 28) /* no space left on device */
#define ESPIPE (_SIGN 29) /* invalid seek */
#define EROFS (_SIGN 30) /* read-only file system */
#define EMLINK (_SIGN 31) /* too many links */
#define EPIPE (_SIGN 32) /* broken pipe */
#define EDOM (_SIGN 33) /* domain error (from ANSI C std) */
#define ERANGE (_SIGN 34) /* result too large (from ANSI C std) */
#define EDEADLK (_SIGN 35) /* resource deadlock avoided */
#define ENAMETOOLONG (_SIGN 36) /* file name too long */
#define ENOLCK (_SIGN 37) /* no locks available */
#define ENOSYS (_SIGN 38) /* function not implemented */
#define ENOTEMPTY (_SIGN 39) /* directory not empty */

/* The following errors relate to networking. */
#define EPACKSIZE (_SIGN 50) /* invalid packet size for some protocol */
#define EOUTOFBUFS (_SIGN 51) /* not enough buffers left */
#define EBADIOCTL (_SIGN 52) /* illegal ioctl for device */
MINIX SOURCE CODE File: include/errno.h 643
#define EBADMODE (_SIGN 53) /* badmode in ioctl */
#define EWOULDBLOCK (_SIGN 54)
#define EBADDEST (_SIGN 55) /* not a valid destination address */
#define EDSTNOTRCH (_SIGN 56) /* destination not reachable */
#define EISCONN (_SIGN 57) /* all ready connected */
#define EADDRINUSE (_SIGN 58) /* address in use */
#define ECONNREFUSED (_SIGN 59) /* connection refused */
#define ECONNRESET (_SIGN 60) /* connection reset */
#define ETIMEDOUT (_SIGN 61) /* connection timed out */
#define EURG (_SIGN 62) /* urgent data present */
#define ENOURG (_SIGN 63) /* no urgent data present */
#define ENOTCONN (_SIGN 64) /* no connection (yet or anymore) */
#define ESHUTDOWN (_SIGN 65) /* a write call to a shutdown connection */
#define ENOCONN (_SIGN 66) /* no such connection */
#define EAFNOSUPPORT (_SIGN 67) /* address family not supported */
#define EPROTONOSUPPORT (_SIGN 68) /* protocol not supported by AF */
#define EPROTOTYPE (_SIGN 69) /* Protocol wrong type for socket */
#define EINPROGRESS (_SIGN 70) /* Operation now in progress */
#define EADDRNOTAVAIL (_SIGN 71) /* Can’t assign requested address */
#define EALREADY (_SIGN 72) /* Connection already in progress */
#define EMSGSIZE (_SIGN 73) /* Message too long */

/* The following are not POSIX errors, but they can still happen.
* All of these are generated by the kernel and relate to message passing.
*/
#define ELOCKED (_SIGN 101) /* can’t send message due to deadlock */
#define EBADCALL (_SIGN 102) /* illegal system call number */
#define EBADSRCDST (_SIGN 103) /* bad source or destination process */
#define ECALLDENIED (_SIGN 104) /* no permission for system call */
#define EDEADDST (_SIGN 105) /* send destination is not alive */
#define ENOTREADY (_SIGN 106) /* source or destination is not ready */
#define EBADREQUEST (_SIGN 107) /* destination cannot handle request */
#define EDONTREPLY (_SIGN 201) /* pseudo-code: don’t send a reply */

#endif /* _ERRNO_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/unistd.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <unistd.h> header contains a few miscellaneous manifest constants. */

#ifndef _UNISTD_H
#define _UNISTD_H

#ifndef _TYPES_H
#include <sys/types.h>
#endif

/* Values used by access(). POSIX Table 2-8. */
#define F_OK 0 /* test if file exists */
#define X_OK 1 /* test if file is executable */
#define W_OK 2 /* test if file is writable */
#define R_OK 4 /* test if file is readable */

/* Values used for whence in lseek(fd, offset, whence). POSIX Table 2-9. */
#define SEEK_SET 0 /* offset is absolute */
#define SEEK_CUR 1 /* offset is relative to current position */
#define SEEK_END 2 /* offset is relative to end of file */

644 File: include/unistd.h MINIX SOURCE CODE
/* This value is required by POSIX Table 2-10. */
#define _POSIX_VERSION 9L /* which standard is being conformed to */

/* These three definitions are required by POSIX Sec. 8.2.1.2. */
#define STDIN_FILENO 0 /* file descriptor for stdin */
#define STDOUT_FILENO 1 /* file descriptor for stdout */
#define STDERR_FILENO 2 /* file descriptor for stderr */

#ifdef _MINIX
/* How to exit the system or stop a server process. */
#define RBT_HALT 0
#define RBT_REBOOT 1
#define RBT_PANIC 2 /* a server panics */
#define RBT_MONITOR 3 /* let the monitor do this */
#define RBT_RESET 4 /* hard reset the system */
#endif

/* What system info to retrieve with sysgetinfo(). */
#define SI_KINFO 0 /* get kernel info via PM */
#define SI_PROC_ADDR 1 /* address of process table */
#define SI_PROC_TAB 2 /* copy of entire process table */
#define SI_DMAP_TAB 3 /* get device <-> driver mappings */

/* NULL must be defined in <unistd.h> according to POSIX Sec. 2.7.1. */
#define NULL ((void *)0)

/* The following relate to configurable system variables. POSIX Table 4-2. */
#define _SC_ARG_MAX 1
#define _SC_CHILD_MAX 2
#define _SC_CLOCKS_PER_SEC 3
#define _SC_CLK_TCK 3
#define _SC_NGROUPS_MAX 4
#define _SC_OPEN_MAX 5
#define _SC_JOB_CONTROL 6
#define _SC_SAVED_IDS 7
#define _SC_VERSION 8
#define _SC_STREAM_MAX 9
#define _SC_TZNAME_MAX 10

/* The following relate to configurable pathname variables. POSIX Table 5-2. */
#define _PC_LINK_MAX 1 /* link count */
#define _PC_MAX_CANON 2 /* size of the canonical input queue */
#define _PC_MAX_INPUT 3 /* type-ahead buffer size */
#define _PC_NAME_MAX 4 /* file name size */
#define _PC_PATH_MAX 5 /* pathname size */
#define _PC_PIPE_BUF 6 /* pipe size */
#define _PC_NO_TRUNC 7 /* treatment of long name components */
#define _PC_VDISABLE 8 /* tty disable */
#define _PC_CHOWN_RESTRICTED 9 /* chown restricted or not */

/* POSIX defines several options that may be implemented or not, at the
* implementer’s whim. This implementer has made the following choices:
*
* _POSIX_JOB_CONTROL not defined: no job control
* _POSIX_SAVED_IDS not defined: no saved uid/gid
* _POSIX_NO_TRUNC defined as -1: long path names are truncated
* _POSIX_CHOWN_RESTRICTED defined: you can’t give away files
* _POSIX_VDISABLE defined: tty functions can be disabled
*/
#define _POSIX_NO_TRUNC (-1)
MINIX SOURCE CODE File: include/unistd.h 645
#define _POSIX_CHOWN_RESTRICTED 1

/* Function Prototypes. */
_PROTOTYPE( void _exit, (int _status) );
_PROTOTYPE( int access, (const char *_path, int _amode) );
_PROTOTYPE( unsigned int alarm, (unsigned int _seconds) );
_PROTOTYPE( int chdir, (const char *_path) );
_PROTOTYPE( int fchdir, (int fd) );
_PROTOTYPE( int chown, (const char *_path, _mnx_Uid_t _owner, _mnx_Gid_t _group) );
_PROTOTYPE( int close, (int _fd) );
_PROTOTYPE( char *ctermid, (char *_s) );
_PROTOTYPE( char *cuserid, (char *_s) );
_PROTOTYPE( int dup, (int _fd) );
_PROTOTYPE( int dup2, (int _fd, int _fd2) );
_PROTOTYPE( int execl, (const char *_path, const char *_arg, ...) );
_PROTOTYPE( int execle, (const char *_path, const char *_arg, ...) );
_PROTOTYPE( int execlp, (const char *_file, const char *arg, ...) );
_PROTOTYPE( int execv, (const char *_path, char *const _argv[]) );
_PROTOTYPE( int execve, (const char *_path, char *const _argv[],
char *const _envp[]) );
_PROTOTYPE( int execvp, (const char *_file, char *const _argv[]) );
_PROTOTYPE( pid_t fork, (void) );
_PROTOTYPE( long fpathconf, (int _fd, int _name) );
_PROTOTYPE( char *getcwd, (char *_buf, size_t _size) );
_PROTOTYPE( gid_t getegid, (void) );
_PROTOTYPE( uid_t geteuid, (void) );
_PROTOTYPE( gid_t getgid, (void) );
_PROTOTYPE( int getgroups, (int _gidsetsize, gid_t _grouplist[]) );
_PROTOTYPE( char *getlogin, (void) );
_PROTOTYPE( pid_t getpgrp, (void) );
_PROTOTYPE( pid_t getpid, (void) );
_PROTOTYPE( pid_t getppid, (void) );
_PROTOTYPE( uid_t getuid, (void) );
_PROTOTYPE( int isatty, (int _fd) );
_PROTOTYPE( int link, (const char *_existing, const char *_new) );
_PROTOTYPE( off_t lseek, (int _fd, off_t _offset, int _whence) );
_PROTOTYPE( long pathconf, (const char *_path, int _name) );
_PROTOTYPE( int pause, (void) );
_PROTOTYPE( int pipe, (int _fildes[2]) );
_PROTOTYPE( ssize_t read, (int _fd, void *_buf, size_t _n) );
_PROTOTYPE( int rmdir, (const char *_path) );
_PROTOTYPE( int setgid, (_mnx_Gid_t _gid) );
_PROTOTYPE( int setpgid, (pid_t _pid, pid_t _pgid) );
_PROTOTYPE( pid_t setsid, (void) );
_PROTOTYPE( int setuid, (_mnx_Uid_t _uid) );
_PROTOTYPE( unsigned int sleep, (unsigned int _seconds) );
_PROTOTYPE( long sysconf, (int _name) );
_PROTOTYPE( pid_t tcgetpgrp, (int _fd) );
_PROTOTYPE( int tcsetpgrp, (int _fd, pid_t _pgrp_id) );
_PROTOTYPE( char *ttyname, (int _fd) );
_PROTOTYPE( int unlink, (const char *_path) );
_PROTOTYPE( ssize_t write, (int _fd, const void *_buf, size_t _n) );

/* Open Group Base Specifications Issue 6 (not complete) */
_PROTOTYPE( int symlink, (const char *path1, const char *path2) );
_PROTOTYPE( int getopt, (int _argc, char **_argv, char *_opts) );
extern char *optarg;
extern int optind, opterr, optopt;
_PROTOTYPE( int usleep, (useconds_t _useconds) );

646 File: include/unistd.h MINIX SOURCE CODE
#ifdef _MINIX
#ifndef _TYPE_H
#include <minix/type.h>
#endif
_PROTOTYPE( int brk, (char *_addr) );
_PROTOTYPE( int chroot, (const char *_name) );
_PROTOTYPE( int mknod, (const char *_name, _mnx_Mode_t _mode, Dev_t _addr) );
_PROTOTYPE( int mknod4, (const char *_name, _mnx_Mode_t _mode, Dev_t _addr,
long _size) );
_PROTOTYPE( char *mktemp, (char *_template) );
_PROTOTYPE( int mount, (char *_spec, char *_name, int _flag) );
_PROTOTYPE( long ptrace, (int _req, pid_t _pid, long _addr, long _data) );
_PROTOTYPE( char *sbrk, (int _incr) );
_PROTOTYPE( int sync, (void) );
_PROTOTYPE( int fsync, (int fd) );
_PROTOTYPE( int umount, (const char *_name) );
_PROTOTYPE( int reboot, (int _how, ...) );
_PROTOTYPE( int gethostname, (char *_hostname, size_t _len) );
_PROTOTYPE( int getdomainname, (char *_domain, size_t _len) );
_PROTOTYPE( int ttyslot, (void) );
_PROTOTYPE( int fttyslot, (int _fd) );
_PROTOTYPE( char *crypt, (const char *_key, const char *_salt) );
_PROTOTYPE( int getsysinfo, (int who, int what, void *where) );
_PROTOTYPE( int getprocnr, (void) );
_PROTOTYPE( int findproc, (char *proc_name, int *proc_nr) );
_PROTOTYPE( int allocmem, (phys_bytes size, phys_bytes *base) );
_PROTOTYPE( int freemem, (phys_bytes size, phys_bytes base) );
#define DEV_MAP 1
#define DEV_UNMAP 2
#define mapdriver(driver, device, style) devctl(DEV_MAP, driver, device, style)
#define unmapdriver(device) devctl(DEV_UNMAP, 0, device, 0)
_PROTOTYPE( int devctl, (int ctl_req, int driver, int device, int style));

/* For compatibility with other Unix systems */
_PROTOTYPE( int getpagesize, (void) );
_PROTOTYPE( int setgroups, (int ngroups, const gid_t *gidset) );

#endif

_PROTOTYPE( int readlink, (const char *, char *, int));
_PROTOTYPE( int getopt, (int, char **, char *));
extern int optind, opterr, optopt;

#endif /* _UNISTD_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/string.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <string.h> header contains prototypes for the string handling
* functions.
*/

#ifndef _STRING_H
#define _STRING_H

#define NULL ((void *)0)

#ifndef _SIZE_T
MINIX SOURCE CODE File: include/string.h 647
#define _SIZE_T
typedef unsigned int size_t; /* type returned by sizeof */
#endif /*_SIZE_T */

/* Function Prototypes. */
#ifndef _ANSI_H
#include <ansi.h>
#endif

_PROTOTYPE( void *memchr, (const void *_s, int _c, size_t _n) );
_PROTOTYPE( int memcmp, (const void *_s1, const void *_s2, size_t _n) );
_PROTOTYPE( void *memcpy, (void *_s1, const void *_s2, size_t _n) );
_PROTOTYPE( void *memmove, (void *_s1, const void *_s2, size_t _n) );
_PROTOTYPE( void *memset, (void *_s, int _c, size_t _n) );
_PROTOTYPE( char *strcat, (char *_s1, const char *_s2) );
_PROTOTYPE( char *strchr, (const char *_s, int _c) );
_PROTOTYPE( int strncmp, (const char *_s1, const char *_s2, size_t _n) );
_PROTOTYPE( int strcmp, (const char *_s1, const char *_s2) );
_PROTOTYPE( int strcoll, (const char *_s1, const char *_s2) );
_PROTOTYPE( char *strcpy, (char *_s1, const char *_s2) );
_PROTOTYPE( size_t strcspn, (const char *_s1, const char *_s2) );
_PROTOTYPE( char *strerror, (int _errnum) );
_PROTOTYPE( size_t strlen, (const char *_s) );
_PROTOTYPE( char *strncat, (char *_s1, const char *_s2, size_t _n) );
_PROTOTYPE( char *strncpy, (char *_s1, const char *_s2, size_t _n) );
_PROTOTYPE( char *strpbrk, (const char *_s1, const char *_s2) );
_PROTOTYPE( char *strrchr, (const char *_s, int _c) );
_PROTOTYPE( size_t strspn, (const char *_s1, const char *_s2) );
_PROTOTYPE( char *strstr, (const char *_s1, const char *_s2) );
_PROTOTYPE( char *strtok, (char *_s1, const char *_s2) );
_PROTOTYPE( size_t strxfrm, (char *_s1, const char *_s2, size_t _n) );

#ifdef _POSIX_SOURCE
/* Open Group Base Specifications Issue 6 (not complete) */
char *strdup(const char *_s1);
#endif

#ifdef _MINIX
/* For backward compatibility. */
_PROTOTYPE( char *index, (const char *_s, int _charwanted) );
_PROTOTYPE( char *rindex, (const char *_s, int _charwanted) );
_PROTOTYPE( void bcopy, (const void *_src, void *_dst, size_t _length) );
_PROTOTYPE( int bcmp, (const void *_s1, const void *_s2, size_t _length));
_PROTOTYPE( void bzero, (void *_dst, size_t _length) );
_PROTOTYPE( void *memccpy, (char *_dst, const char *_src, int _ucharstop,
size_t _size) );

/* Misc. extra functions */
_PROTOTYPE( int strcasecmp, (const char *_s1, const char *_s2) );
_PROTOTYPE( int strncasecmp, (const char *_s1, const char *_s2,
size_t _len) );
_PROTOTYPE( size_t strnlen, (const char *_s, size_t _n) );
#endif

#endif /* _STRING_H */
648 File: include/signal.h MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/signal.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <signal.h> header defines all the ANSI and POSIX signals.
* MINIX supports all the signals required by POSIX. They are defined below.
* Some additional signals are also supported.
*/

#ifndef _SIGNAL_H
#define _SIGNAL_H

#ifndef _ANSI_H
#include <ansi.h>
#endif
#ifdef _POSIX_SOURCE
#ifndef _TYPES_H
#include <sys/types.h>
#endif
#endif

/* Here are types that are closely associated with signal handling. */
typedef int sig_atomic_t;

#ifdef _POSIX_SOURCE
#ifndef _SIGSET_T
#define _SIGSET_T
typedef unsigned long sigset_t;
#endif
#endif

#define _NSIG 20 /* number of signals used */

#define SIGHUP 1 /* hangup */
#define SIGINT 2 /* interrupt (DEL) */
#define SIGQUIT 3 /* quit (ASCII FS) */
#define SIGILL 4 /* illegal instruction */
#define SIGTRAP 5 /* trace trap (not reset when caught) */
#define SIGABRT 6 /* IOT instruction */
#define SIGIOT 6 /* SIGABRT for people who speak PDP-11 */
#define SIGUNUSED 7 /* spare code */
#define SIGFPE 8 /* floating point exception */
#define SIGKILL 9 /* kill (cannot be caught or ignored) */
#define SIGUSR1 10 /* user defined signal # 1 */
#define SIGSEGV 11 /* segmentation violation */
#define SIGUSR2 12 /* user defined signal # 2 */
#define SIGPIPE 13 /* write on a pipe with no one to read it */
#define SIGALRM 14 /* alarm clock */
#define SIGTERM 15 /* software termination signal from kill */
#define SIGCHLD 17 /* child process terminated or stopped */

#define SIGEMT 7 /* obsolete */
#define SIGBUS 10 /* obsolete */

/* MINIX specific signals. These signals are not used by user proceses,
* but meant to inform system processes, like the PM, about system events.
*/
#define SIGKMESS 18 /* new kernel message */
#define SIGKSIG 19 /* kernel signal pending */
MINIX SOURCE CODE File: include/signal.h 649
#define SIGKSTOP 20 /* kernel shutting down */

/* POSIX requires the following signals to be defined, even if they are
* not supported. Here are the definitions, but they are not supported.
*/
#define SIGCONT 18 /* continue if stopped */
#define SIGSTOP 19 /* stop signal */
#define SIGTSTP 20 /* interactive stop signal */
#define SIGTTIN 21 /* background process wants to read */
#define SIGTTOU 22 /* background process wants to write */

/* The sighandler_t type is not allowed unless _POSIX_SOURCE is defined. */
typedef void _PROTOTYPE( (*__sighandler_t), (int) );

/* Macros used as function pointers. */
#define SIG_ERR ((__sighandler_t) -1) /* error return */
#define SIG_DFL ((__sighandler_t) 0) /* default signal handling */
#define SIG_IGN ((__sighandler_t) 1) /* ignore signal */
#define SIG_HOLD ((__sighandler_t) 2) /* block signal */
#define SIG_CATCH ((__sighandler_t) 3) /* catch signal */
#define SIG_MESS ((__sighandler_t) 4) /* pass as message (MINIX) */

#ifdef _POSIX_SOURCE
struct sigaction {
__sighandler_t sa_handler; /* SIG_DFL, SIG_IGN, or pointer to function */
sigset_t sa_mask; /* signals to be blocked during handler */
int sa_flags; /* special flags */
};

/* Fields for sa_flags. */
#define SA_ONSTACK 0x0001 /* deliver signal on alternate stack */
#define SA_RESETHAND 0x0002 /* reset signal handler when signal caught */
#define SA_NODEFER 0x0004 /* don’t block signal while catching it */
#define SA_RESTART 0x0008 /* automatic system call restart */
#define SA_SIGINFO 0x0010 /* extended signal handling */
#define SA_NOCLDWAIT 0x0020 /* don’t create zombies */
#define SA_NOCLDSTOP 0x0040 /* don’t receive SIGCHLD when child stops */

/* POSIX requires these values for use with sigprocmask(2). */
#define SIG_BLOCK 0 /* for blocking signals */
#define SIG_UNBLOCK 1 /* for unblocking signals */
#define SIG_SETMASK 2 /* for setting the signal mask */
#define SIG_INQUIRE 4 /* for internal use only */
#endif /* _POSIX_SOURCE */

/* POSIX and ANSI function prototypes. */
_PROTOTYPE( int raise, (int _sig) );
_PROTOTYPE( __sighandler_t signal, (int _sig, __sighandler_t _func) );

#ifdef _POSIX_SOURCE
_PROTOTYPE( int kill, (pid_t _pid, int _sig) );
_PROTOTYPE( int sigaction,
(int _sig, const struct sigaction *_act, struct sigaction *_oact) );
_PROTOTYPE( int sigaddset, (sigset_t *_set, int _sig) );
_PROTOTYPE( int sigdelset, (sigset_t *_set, int _sig) );
_PROTOTYPE( int sigemptyset, (sigset_t *_set) );
_PROTOTYPE( int sigfillset, (sigset_t *_set) );
_PROTOTYPE( int sigismember, (const sigset_t *_set, int _sig) );
_PROTOTYPE( int sigpending, (sigset_t *_set) );
_PROTOTYPE( int sigprocmask,
650 File: include/signal.h MINIX SOURCE CODE
(int _how, const sigset_t *_set, sigset_t *_oset) );
_PROTOTYPE( int sigsuspend, (const sigset_t *_sigmask) );
#endif

#endif /* _SIGNAL_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/fcntl.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <fcntl.h> header is needed by the open() and fcntl() system calls,
* which have a variety of parameters and flags. They are described here.
* The formats of the calls to each of these are:
*
* open(path, oflag [,mode]) open a file
* fcntl(fd, cmd [,arg]) get or set file attributes
*
*/

#ifndef _FCNTL_H
#define _FCNTL_H

#ifndef _TYPES_H
#include <sys/types.h>
#endif

/* These values are used for cmd in fcntl(). POSIX Table 6-1. */
#define F_DUPFD 0 /* duplicate file descriptor */
#define F_GETFD 1 /* get file descriptor flags */
#define F_SETFD 2 /* set file descriptor flags */
#define F_GETFL 3 /* get file status flags */
#define F_SETFL 4 /* set file status flags */
#define F_GETLK 5 /* get record locking information */
#define F_SETLK 6 /* set record locking information */
#define F_SETLKW 7 /* set record locking info; wait if blocked */

/* File descriptor flags used for fcntl(). POSIX Table 6-2. */
#define FD_CLOEXEC 1 /* close on exec flag for third arg of fcntl */

/* L_type values for record locking with fcntl(). POSIX Table 6-3. */
#define F_RDLCK 1 /* shared or read lock */
#define F_WRLCK 2 /* exclusive or write lock */
#define F_UNLCK 3 /* unlock */

/* Oflag values for open(). POSIX Table 6-4. */
#define O_CREAT /* creat file if it doesn’t exist */
#define O_EXCL /* exclusive use flag */
#define O_NOCTTY /* do not assign a controlling terminal */
#define O_TRUNC /* truncate flag */

/* File status flags for open() and fcntl(). POSIX Table 6-5. */
#define O_APPEND /* set append mode */
#define O_NONBLOCK /* no delay */

/* File access modes for open() and fcntl(). POSIX Table 6-6. */
#define O_RDONLY 0 /* open(name, O_RDONLY) opens read only */
#define O_WRONLY 1 /* open(name, O_WRONLY) opens write only */
#define O_RDWR 2 /* open(name, O_RDWR) opens read/write */

/* Mask for use with file access modes. POSIX Table 6-7. */
MINIX SOURCE CODE File: include/fcntl.h 651
#define O_ACCMODE 03 /* mask for file access modes */

/* Struct used for locking. POSIX Table 6-8. */
struct flock {
short l_type; /* type: F_RDLCK, F_WRLCK, or F_UNLCK */
short l_whence; /* flag for starting offset */
off_t l_start; /* relative offset in bytes */
off_t l_len; /* size; if 0, then until EOF */
pid_t l_pid; /* process id of the locks’ owner */
};

/* Function Prototypes. */
_PROTOTYPE( int creat, (const char *_path, _mnx_Mode_t _mode) );
_PROTOTYPE( int fcntl, (int _filedes, int _cmd, ...) );
_PROTOTYPE( int open, (const char *_path, int _oflag, ...) );

#endif /* _FCNTL_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/termios.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <termios.h> header is used for controlling tty modes. */

#ifndef _TERMIOS_H
#define _TERMIOS_H

typedef unsigned short tcflag_t;
typedef unsigned char cc_t;
typedef unsigned int speed_t;

#define NCCS 20 /* size of cc_c array, some extra space
* for extensions. */

/* Primary terminal control structure. POSIX Table 7-1. */
struct termios {
tcflag_t c_iflag; /* input modes */
tcflag_t c_oflag; /* output modes */
tcflag_t c_cflag; /* control modes */
tcflag_t c_lflag; /* local modes */
speed_t c_ispeed; /* input speed */
speed_t c_ospeed; /* output speed */
cc_t c_cc[NCCS]; /* control characters */
};

/* Values for termios c_iflag bit map. POSIX Table 7-2. */
#define BRKINT 0x0001 /* signal interrupt on break */
#define ICRNL 0x0002 /* map CR to NL on input */
#define IGNBRK 0x0004 /* ignore break */
#define IGNCR 0x0008 /* ignore CR */
#define IGNPAR 0x0010 /* ignore characters with parity errors */
#define INLCR 0x0020 /* map NL to CR on input */
#define INPCK 0x0040 /* enable input parity check */
#define ISTRIP 0x0080 /* mask off 8th bit */
#define IXOFF 0x0100 /* enable start/stop input control */
#define IXON 0x0200 /* enable start/stop output control */
#define PARMRK 0x0400 /* mark parity errors in the input queue */
652 File: include/termios.h MINIX SOURCE CODE

/* Values for termios c_oflag bit map. POSIX Sec. 7.1.2.3. */
#define OPOST 0x0001 /* perform output processing */

/* Values for termios c_cflag bit map. POSIX Table 7-3. */
#define CLOCAL 0x0001 /* ignore modem status lines */
#define CREAD 0x0002 /* enable receiver */
#define CSIZE 0x000C /* number of bits per character */
#define CS5 0x0000 /* if CSIZE is CS5, characters are 5 bits */
#define CS6 0x0004 /* if CSIZE is CS6, characters are 6 bits */
#define CS7 0x0008 /* if CSIZE is CS7, characters are 7 bits */
#define CS8 0x000C /* if CSIZE is CS8, characters are 8 bits */
#define CSTOPB 0x0010 /* send 2 stop bits if set, else 1 */
#define HUPCL 0x0020 /* hang up on last close */
#define PARENB 0x0040 /* enable parity on output */
#define PARODD 0x0080 /* use odd parity if set, else even */

/* Values for termios c_lflag bit map. POSIX Table 7-4. */
#define ECHO 0x0001 /* enable echoing of input characters */
#define ECHOE 0x0002 /* echo ERASE as backspace */
#define ECHOK 0x0004 /* echo KILL */
#define ECHONL 0x0008 /* echo NL */
#define ICANON 0x0010 /* canonical input (erase and kill enabled) */
#define IEXTEN 0x0020 /* enable extended functions */
#define ISIG 0x0040 /* enable signals */
#define NOFLSH 0x0080 /* disable flush after interrupt or quit */
#define TOSTOP 0x0100 /* send SIGTTOU (job control, not implemented*/

/* Indices into c_cc array. Default values in parentheses. POSIX Table 7-5. */
#define VEOF 0 /* cc_c[VEOF] = EOF char (ˆD) */
#define VEOL 1 /* cc_c[VEOL] = EOL char (undef) */
#define VERASE 2 /* cc_c[VERASE] = ERASE char (ˆH) */
#define VINTR 3 /* cc_c[VINTR] = INTR char (DEL) */
#define VKILL 4 /* cc_c[VKILL] = KILL char (ˆU) */
#define VMIN 5 /* cc_c[VMIN] = MIN value for timer */
#define VQUIT 6 /* cc_c[VQUIT] = QUIT char (ˆ\) */
#define VTIME 7 /* cc_c[VTIME] = TIME value for timer */
#define VSUSP 8 /* cc_c[VSUSP] = SUSP (ˆZ, ignored) */
#define VSTART 9 /* cc_c[VSTART] = START char (ˆS) */
#define VSTOP 10 /* cc_c[VSTOP] = STOP char (ˆQ) */

#define _POSIX_VDISABLE (cc_t)0xFF /* You can’t even generate this
* character with ’normal’ keyboards.
* But some language specific keyboards
* can generate 0xFF. It seems that all
* 256 are used, so cc_t should be a
* short...
*/

/* Values for the baud rate settings. POSIX Table 7-6. */
#define B0 0x0000 /* hang up the line */
#define B50 0x1000 /* 50 baud */
#define B75 0x2000 /* 75 baud */
#define B110 0x3000 /* 110 baud */
#define B134 0x4000 /* 134.5 baud */
#define B150 0x5000 /* 150 baud */
#define B200 0x6000 /* 200 baud */
#define B300 0x7000 /* 300 baud */
#define B600 0x8000 /* 600 baud */
#define B1200 0x9000 /* 1200 baud */
MINIX SOURCE CODE File: include/termios.h 653
#define B1800 0xA000 /* 1800 baud */
#define B2400 0xB000 /* 2400 baud */
#define B4800 0xC000 /* 4800 baud */
#define B9600 0xD000 /* 9600 baud */
#define B0xE000 /* baud */
#define B0xF000 /* baud */

/* Optional actions for tcsetattr(). POSIX Sec. 7.2.1.2. */
#define TCSANOW 1 /* changes take effect immediately */
#define TCSADRAIN 2 /* changes take effect after output is done */
#define TCSAFLUSH 3 /* wait for output to finish and flush input */

/* Queue_selector values for tcflush(). POSIX Sec. 7.2.2.2. */
#define TCIFLUSH 1 /* flush accumulated input data */
#define TCOFLUSH 2 /* flush accumulated output data */
#define TCIOFLUSH 3 /* flush accumulated input and output data */

/* Action values for tcflow(). POSIX Sec. 7.2.2.2. */
#define TCOOFF 1 /* suspend output */
#define TCOON 2 /* restart suspended output */
#define TCIOFF 3 /* transmit a STOP character on the line */
#define TCION 4 /* transmit a START character on the line */

/* Function Prototypes. */
#ifndef _ANSI_H
#include <ansi.h>
#endif

_PROTOTYPE( int tcsendbreak, (int _fildes, int _duration) );
_PROTOTYPE( int tcdrain, (int _filedes) );
_PROTOTYPE( int tcflush, (int _filedes, int _queue_selector) );
_PROTOTYPE( int tcflow, (int _filedes, int _action) );
_PROTOTYPE( speed_t cfgetispeed, (const struct termios *_termios_p) );
_PROTOTYPE( speed_t cfgetospeed, (const struct termios *_termios_p) );
_PROTOTYPE( int cfsetispeed, (struct termios *_termios_p, speed_t _speed) );
_PROTOTYPE( int cfsetospeed, (struct termios *_termios_p, speed_t _speed) );
_PROTOTYPE( int tcgetattr, (int _filedes, struct termios *_termios_p) );
_PROTOTYPE( int tcsetattr, \
(int _filedes, int _opt_actions, const struct termios *_termios_p) );

#define cfgetispeed(termios_p) ((termios_p)->c_ispeed)
#define cfgetospeed(termios_p) ((termios_p)->c_ospeed)
#define cfsetispeed(termios_p, speed) ((termios_p)->c_ispeed = (speed), 0)
#define cfsetospeed(termios_p, speed) ((termios_p)->c_ospeed = (speed), 0)

#ifdef _MINIX
/* Here are the local extensions to the POSIX standard for Minix. Posix
* conforming programs are not able to access these, and therefore they are
* only defined when a Minix program is compiled.
*/

/* Extensions to the termios c_iflag bit map. */
#define IXANY 0x0800 /* allow any key to continue ouptut */

/* Extensions to the termios c_oflag bit map. They are only active iff
* OPOST is enabled. */
#define ONLCR 0x0002 /* Map NL to CR-NL on output */
#define XTABS 0x0004 /* Expand tabs to spaces */
#define ONOEOT 0x0008 /* discard EOT’s (ˆD) on output) */

654 File: include/termios.h MINIX SOURCE CODE
/* Extensions to the termios c_lflag bit map. */
#define LFLUSHO 0x0200 /* Flush output. */

/* Extensions to the c_cc array. */
#define VREPRINT 11 /* cc_c[VREPRINT] (ˆR) */
#define VLNEXT 12 /* cc_c[VLNEXT] (ˆV) */
#define VDISCARD 13 /* cc_c[VDISCARD] (ˆO) */

/* Extensions to baud rate settings. */
#define B0x0100 /* baud */
#define B10x0200 /* 1baud */

/* These are the default settings used by the kernel and by ’stty sane’ */

#define TCTRL_DEF (CREAD | CS8 | HUPCL)
#define TINPUT_DEF (BRKINT | ICRNL | IXON | IXANY)
#define TOUTPUT_DEF (OPOST | ONLCR)
#define TLOCAL_DEF (ISIG | IEXTEN | ICANON | ECHO | ECHOE)
#define TSPEED_DEF B9600

#define TEOF_DEF ’\4’ /* ˆD */
#define TEOL_DEF _POSIX_VDISABLE
#define TERASE_DEF ’\10’ /* ˆH */
#define TINTR_DEF ’\3’ /* ˆC */
#define TKILL_DEF ’\25’ /* ˆU */
#define TMIN_DEF 1
#define TQUIT_DEF ’\34’ /* ˆ\ */
#define TSTART_DEF ’\21’ /* ˆQ */
#define TSTOP_DEF ’\23’ /* ˆS */
#define TSUSP_DEF ’\32’ /* ˆZ */
#define TTIME_DEF 0
#define TREPRINT_DEF ’\22’ /* ˆR */
#define TLNEXT_DEF ’\26’ /* ˆV */
#define TDISCARD_DEF ’\17’ /* ˆO */

/* Window size. This information is stored in the TTY driver but not used.
* This can be used for screen based applications in a window environment.
* The ioctls TIOCGWINSZ and TIOCSWINSZ can be used to get and set this
* information.
*/

struct winsize
{
unsigned short ws_row; /* rows, in characters */
unsigned short ws_col; /* columns, in characters */
unsigned short ws_xpixel; /* horizontal size, pixels */
unsigned short ws_ypixel; /* vertical size, pixels */
};
#endif /* _MINIX */

#endif /* _TERMIOS_H */
MINIX SOURCE CODE File: include/timers.h 655
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/timers.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This library provides generic watchdog timer management functionality.
* The functions operate on a timer queue provided by the caller. Note that
* the timers must use absolute time to allow sorting. The library provides:
*
* tmrs_settimer: (re)set a new watchdog timer in the timers queue
* tmrs_clrtimer: remove a timer from both the timers queue
* tmrs_exptimers: check for expired timers and run watchdog functions
*
* Author:
* Jorrit N. Herder <jnherder@cs.vu.nl>
* Adapted from tmr_settimer and tmr_clrtimer in src/kernel/clock.c.
* Last modified: September 30, 2004.
*/

#ifndef _TIMERS_H
#define _TIMERS_H

#include <limits.h>
#include <sys/types.h>

struct timer;
typedef void (*tmr_func_t)(struct timer *tp);
typedef union { int ta_int; long ta_long; void *ta_ptr; } tmr_arg_t;

/* A timer_t variable must be declare for each distinct timer to be used.
* The timers watchdog function and expiration time are automatically set
* by the library function tmrs_settimer, but its argument is not.
*/
typedef struct timer
{
struct timer *tmr_next; /* next in a timer chain */
clock_t tmr_exp_time; /* expiration time */
tmr_func_t tmr_func; /* function to call when expired */
tmr_arg_t tmr_arg; /* random argument */
} timer_t;

/* Used when the timer is not active. */
#define TMR_NEVER ((clock_t) -1 < 0) ? ((clock_t) LONG_MAX) : ((clock_t) -1)
#undef TMR_NEVER
#define TMR_NEVER ((clock_t) LONG_MAX)

/* These definitions can be used to set or get data from a timer variable. */
#define tmr_arg(tp) (&(tp)->tmr_arg)
#define tmr_exp_time(tp) (&(tp)->tmr_exp_time)

/* Timers should be initialized once before they are being used. Be careful
* not to reinitialize a timer that is in a list of timers, or the chain
* will be broken.
*/
#define tmr_inittimer(tp) (void)((tp)->tmr_exp_time = TMR_NEVER, \
(tp)->tmr_next = NULL)

/* The following generic timer management functions are available. They
* can be used to operate on the lists of timers. Adding a timer to a list
* automatically takes care of removing it.
656 File: include/timers.h MINIX SOURCE CODE
*/
_PROTOTYPE( clock_t tmrs_clrtimer, (timer_t **tmrs, timer_t *tp, clock_t *new_head)
_PROTOTYPE( void tmrs_exptimers, (timer_t **tmrs, clock_t now, clock_t *new_head)
_PROTOTYPE( clock_t tmrs_settimer, (timer_t **tmrs, timer_t *tp,
clock_t exp_time, tmr_func_t watchdog, clock_t *new_head)

#endif /* _TIMERS_H */

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/types.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <sys/types.h> header contains important data type definitions.
* It is considered good programming practice to use these definitions,
* instead of the underlying base type. By convention, all type names end
* with _t.
*/

#ifndef _TYPES_H
#define _TYPES_H

#ifndef _ANSI_H
#include <ansi.h>
#endif

/* The type size_t holds all results of the sizeof operator. At first glance,
* it seems obvious that it should be an unsigned int, but this is not always
* the case. For example, MINIX-ST () has 32-bit pointers and 16-bit
* integers. When one asks for the size of a 70K struct or array, the result
* requires 17 bits to express, so size_t must be a long type. The type
* ssize_t is the signed version of size_t.
*/
#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#ifndef _SSIZE_T
#define _SSIZE_T
typedef int ssize_t;
#endif

#ifndef _TIME_T
#define _TIME_T
typedef long time_t; /* time in sec since 1 Jan 1970 0000 GMT */
#endif

#ifndef _CLOCK_T
#define _CLOCK_T
typedef long clock_t; /* unit for system accounting */
#endif

#ifndef _SIGSET_T
#define _SIGSET_T
typedef unsigned long sigset_t;
#endif

MINIX SOURCE CODE File: include/sys/types.h 657
/* Open Group Base Specifications Issue 6 (not complete) */
typedef long useconds_t; /* Time in microseconds */

/* Types used in disk, inode, etc. data structures. */
typedef short dev_t; /* holds (major|minor) device pair */
typedef char gid_t; /* group id */
typedef unsigned long ino_t; /* i-node number (V3 filesystem) */
typedef unsigned short mode_t; /* file type and permissions bits */
typedef short nlink_t; /* number of links to a file */
typedef unsigned long off_t; /* offset within a file */
typedef int pid_t; /* process id (must be signed) */
typedef short uid_t; /* user id */
typedef unsigned long zone_t; /* zone number */
typedef unsigned long block_t; /* block number */
typedef unsigned long bit_t; /* bit number in a bit map */
typedef unsigned short zone1_t; /* zone number for V1 file systems */
typedef unsigned short bitchunk_t; /* collection of bits in a bitmap */

typedef unsigned char u8_t; /* 8 bit type */
typedef unsigned short u16_t; /* 16 bit type */
typedef unsigned long u32_t; /* 32 bit type */

typedef char i8_t; /* 8 bit signed type */
typedef short i16_t; /* 16 bit signed type */
typedef long i32_t; /* 32 bit signed type */

typedef struct { u32_t _[2]; } u64_t;

/* The following types are needed because MINIX uses K&R style function
* definitions (for maximum portability). When a short, such as dev_t, is
* passed to a function with a K&R definition, the compiler automatically
* promotes it to an int. The prototype must contain an int as the parameter,
* not a short, because an int is what an old-style function definition
* expects. Thus using dev_t in a prototype would be incorrect. It would be
* sufficient to just use int instead of dev_t in the prototypes, but Dev_t
* is clearer.
*/
typedef int Dev_t;
typedef int _mnx_Gid_t;
typedef int Nlink_t;
typedef int _mnx_Uid_t;
typedef int U8_t;
typedef unsigned long U32_t;
typedef int I8_t;
typedef int I16_t;
typedef long I32_t;

/* ANSI C makes writing down the promotion of unsigned types very messy. When
* sizeof(short) == sizeof(int), there is no promotion, so the type stays
* unsigned. When the compiler is not ANSI, there is usually no loss of
* unsignedness, and there are usually no prototypes so the promoted type
* doesn’t matter. The use of types like Ino_t is an attempt to use ints
* (which are not promoted) while providing information to the reader.
*/

typedef unsigned long Ino_t;

#if _EM_WSIZE == 2
/*typedef unsigned int Ino_t; Ino_t is now 32 bits */
typedef unsigned int Zone1_t;
658 File: include/sys/types.h MINIX SOURCE CODE
typedef unsigned int Bitchunk_t;
typedef unsigned int U16_t;
typedef unsigned int _mnx_Mode_t;

#else /* _EM_WSIZE == 4, or _EM_WSIZE undefined */
/*typedef int Ino_t; Ino_t is now 32 bits */
typedef int Zone1_t;
typedef int Bitchunk_t;
typedef int U16_t;
typedef int _mnx_Mode_t;

#endif /* _EM_WSIZE == 2, etc */

/* Signal handler type, e.g. SIG_IGN */
typedef void _PROTOTYPE( (*sighandler_t), (int) );

/* Compatibility with other systems */
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef char *caddr_t;

#endif /* _TYPES_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/sigcontext.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _SIGCONTEXT_H
#define _SIGCONTEXT_H

/* The sigcontext structure is used by the sigreturn(2) system call.
* sigreturn() is seldom called by user programs, but it is used internally
* by the signal catching mechanism.
*/

#ifndef _ANSI_H
#include <ansi.h>
#endif

#ifndef _MINIX_SYS_CONFIG_H
#include <minix/sys_config.h>
#endif

#if !defined(_MINIX_CHIP)
#include "error, configuration is not known"
#endif

/* The following structure should match the stackframe_s structure used
* by the kernel’s context switching code. Floating point registers should
* be added in a different struct.
*/
struct sigregs {
short sr_gs;
short sr_fs;
short sr_es;
short sr_ds;
int sr_di;
MINIX SOURCE CODE File: include/sys/sigcontext.h 659
int sr_si;
int sr_bp;
int sr_st; /* stack top -- used in kernel */
int sr_bx;
int sr_dx;
int sr_cx;
int sr_retreg;
int sr_retadr; /* return address to caller of save -- used
* in kernel */
int sr_pc;
int sr_cs;
int sr_psw;
int sr_sp;
int sr_ss;
};

struct sigframe { /* stack frame created for signalled process */
_PROTOTYPE( void (*sf_retadr), (void) );
int sf_signo;
int sf_code;
struct sigcontext *sf_scp;
int sf_fp;
_PROTOTYPE( void (*sf_retadr2), (void) );
struct sigcontext *sf_scpcopy;
};

struct sigcontext {
int sc_flags; /* sigstack state to restore */
long sc_mask; /* signal mask to restore */
struct sigregs sc_regs; /* register set to restore */
};

#define sc_gs sc_regs.sr_gs
#define sc_fs sc_regs.sr_fs
#define sc_es sc_regs.sr_es
#define sc_ds sc_regs.sr_ds
#define sc_di sc_regs.sr_di
#define sc_si sc_regs.sr_si
#define sc_fp sc_regs.sr_bp
#define sc_st sc_regs.sr_st /* stack top -- used in kernel */
#define sc_bx sc_regs.sr_bx
#define sc_dx sc_regs.sr_dx
#define sc_cx sc_regs.sr_cx
#define sc_retreg sc_regs.sr_retreg
#define sc_retadr sc_regs.sr_retadr /* return address to caller of
save -- used in kernel */
#define sc_pc sc_regs.sr_pc
#define sc_cs sc_regs.sr_cs
#define sc_psw sc_regs.sr_psw
#define sc_sp sc_regs.sr_sp
#define sc_ss sc_regs.sr_ss

/* Values for sc_flags. Must agree with <minix/jmp_buf.h>. */
#define SC_SIGCONTEXT 2 /* nonzero when signal context is included */
#define SC_NOREGLOCALS 4 /* nonzero when registers are not to be
saved and restored */

_PROTOTYPE( int sigreturn, (struct sigcontext *_scp) );

#endif /* _SIGCONTEXT_H */
660 File: include/sys/stat.h MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/stat.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <sys/stat.h> header defines a struct that is used in the stat() and
* fstat functions. The information in this struct comes from the i-node of
* some file. These calls are the only approved way to inspect i-nodes.
*/

#ifndef _STAT_H
#define _STAT_H

#ifndef _TYPES_H
#include <sys/types.h>
#endif

struct stat {
dev_t st_dev; /* major/minor device number */
ino_t st_ino; /* i-node number */
mode_t st_mode; /* file mode, protection bits, etc. */
short int st_nlink; /* # links; TEMPORARY HACK: should be nlink_t*/
uid_t st_uid; /* uid of the file’s owner */
short int st_gid; /* gid; TEMPORARY HACK: should be gid_t */
dev_t st_rdev;
off_t st_size; /* file size */
time_t st_atime; /* time of last access */
time_t st_mtime; /* time of last data modification */
time_t st_ctime; /* time of last file status change */
};

/* Traditional mask definitions for st_mode. */
/* The ugly casts on only some of the definitions are to avoid suprising sign
* extensions such as S_IFREG != (mode_t) S_IFREG when ints are 32 bits.
*/
#define S_IFMT ((mode_t) 00) /* type of file */
#define S_IFLNK ((mode_t) 00) /* symbolic link, not implemented */
#define S_IFREG ((mode_t) 00) /* regular */
#define S_IFBLK 00/* block special */
#define S_IFDIR 00/* directory */
#define S_IFCHR 00/* character special */
#define S_IFIFO 00/* this is a FIFO */
#define S_ISUID 00/* set user id on execution */
#define S_ISGID 00/* set group id on execution */
/* next is reserved for future use */
#define S_ISVTX /* save swapped text even after use */

/* POSIX masks for st_mode. */
#define S_IRWXU /* owner: rwx------ */
#define S_IRUSR /* owner: r-------- */
#define S_IWUSR /* owner: -w------- */
#define S_IXUSR /* owner: --x------ */

#define S_IRWXG /* group: ---rwx--- */
#define S_IRGRP /* group: ---r----- */
#define S_IWGRP /* group: ----w---- */
#define S_IXGRP /* group: -----x--- */

#define S_IRWXO /* others: ------rwx */
#define S_IROTH /* others: ------r-- */
MINIX SOURCE CODE File: include/sys/stat.h 661
#define S_IWOTH /* others: -------w- */
#define S_IXOTH /* others: --------x */

/* The following macros test st_mode (from POSIX Sec. 5.6.1.1). */
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG) /* is a reg file */
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR) /* is a directory */
#define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR) /* is a char spec */
#define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK) /* is a block spec */
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO) /* is a pipe/FIFO */
#define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK) /* is a sym link */

/* Function Prototypes. */
_PROTOTYPE( int chmod, (const char *_path, _mnx_Mode_t _mode) );
_PROTOTYPE( int fstat, (int _fildes, struct stat *_buf) );
_PROTOTYPE( int mkdir, (const char *_path, _mnx_Mode_t _mode) );
_PROTOTYPE( int mkfifo, (const char *_path, _mnx_Mode_t _mode) );
_PROTOTYPE( int stat, (const char *_path, struct stat *_buf) );
_PROTOTYPE( mode_t umask, (_mnx_Mode_t _cmask) );

/* Open Group Base Specifications Issue 6 (not complete) */
_PROTOTYPE( int lstat, (const char *_path, struct stat *_buf) );

#endif /* _STAT_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/dir.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <dir.h> header gives the layout of a directory. */

#ifndef _DIR_H
#define _DIR_H

#include <sys/types.h>

#define DIRBLKSIZ 512 /* size of directory block */

#ifndef DIRSIZ
#define DIRSIZ 60
#endif

struct direct {
ino_t d_ino;
char d_name[DIRSIZ];
};

#endif /* _DIR_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/wait.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <sys/wait.h> header contains macros related to wait(). The value
* returned by wait() and waitpid() depends on whether the process
* terminated by an exit() call, was killed by a signal, or was stopped
* due to job control, as follows:
*
662 File: include/sys/wait.h MINIX SOURCE CODE
* High byte Low byte
* +---------------------+
* exit(status) | status | 0 |
* +---------------------+
* killed by signal | 0 | signal |
* +---------------------+
* stopped (job control) | signal | 0177 |
* +---------------------+
*/

#ifndef _WAIT_H
#define _WAIT_H

#ifndef _TYPES_H
#include <sys/types.h>
#endif

#define _LOW(v) ( (v) & 0377)
#define _HIGH(v) ( ((v) >> 8) & 0377)

#define WNOHANG 1 /* do not wait for child to exit */
#define WUNTRACED 2 /* for job control; not implemented */

#define WIFEXITED(s) (_LOW(s) == 0) /* normal exit */
#define WEXITSTATUS(s) (_HIGH(s)) /* exit status */
#define WTERMSIG(s) (_LOW(s) & 0177) /* sig value */
#define WIFSIGNALED(s) (((unsigned int)(s)-1 & 0xFFFF) < 0xFF) /* signaled */
#define WIFSTOPPED(s) (_LOW(s) == 0177) /* stopped */
#define WSTOPSIG(s) (_HIGH(s) & 0377) /* stop signal */

/* Function Prototypes. */
_PROTOTYPE( pid_t wait, (int *_stat_loc) );
_PROTOTYPE( pid_t waitpid, (pid_t _pid, int *_stat_loc, int _options) );

#endif /* _WAIT_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/ioctl.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* sys/ioctl.h - All ioctl() command codes. Author: Kees J. Bot
* 23 Nov 2002
*
* This header file includes all other ioctl command code headers.
*/

#ifndef _S_IOCTL_H
#define _S_IOCTL_H

/* A driver that uses ioctls claims a character for its series of commands.
* For instance: #define TCGETS _IOR(’T’, 8, struct termios)
* This is a terminal ioctl that uses the character ’T’. The character(s)
* used in each header file are shown in the comment following.
*/

#include <sys/ioc_tty.h> /* ’T’ ’t’ ’k’ */
#include <sys/ioc_disk.h> /* ’d’ */
#include <sys/ioc_memory.h> /* ’m’ */
#include <sys/ioc_cmos.h> /* ’c’ */

MINIX SOURCE CODE File: include/sys/ioctl.h 663
#endif /* _S_IOCTL_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/sys/ioc_disk.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* sys/ioc_disk.h - Disk ioctl() command codes. Author: Kees J. Bot
* 23 Nov 2002
*
*/

#ifndef _S_I_DISK_H
#define _S_I_DISK_H

#include <minix/ioctl.h>

#define DIOCSETP _IOW(’d’, 3, struct partition)
#define DIOCGETP _IOR(’d’, 4, struct partition)
#define DIOCEJECT _IO (’d’, 5)
#define DIOCTIMEOUT _IOW(’d’, 6, int)
#define DIOCOPENCT _IOR(’d’, 7, int)

#endif /* _S_I_DISK_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/ioctl.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* minix/ioctl.h - Ioctl helper definitions. Author: Kees J. Bot
* 23 Nov 2002
*
* This file is included by every header file that defines ioctl codes.
*/

#ifndef _M_IOCTL_H
#define _M_IOCTL_H

#ifndef _TYPES_H
#include <sys/types.h>
#endif

#if _EM_WSIZE >= 4
/* Ioctls have the command encoded in the low-order word, and the size
* of the parameter in the high-order word. The 3 high bits of the high-
* order word are used to encode the in/out/void status of the parameter.
*/
#define _IOCPARM_MASK 0x1FFF
#define _IOC_VOID 0x000
#define _IOCTYPE_MASK 0xFFFF
#define _IOC_IN 0x000
#define _IOC_OUT 0x000
#define _IOC_INOUT (_IOC_IN | _IOC_OUT)

664 File: include/minix/ioctl.h MINIX SOURCE CODE
#define _IO(x,y) ((x << 8) | y | _IOC_VOID)
#define _IOR(x,y,t) ((x << 8) | y | ((sizeof(t) & _IOCPARM_MASK) << 16) |\
_IOC_OUT)
#define _IOW(x,y,t) ((x << 8) | y | ((sizeof(t) & _IOCPARM_MASK) << 16) |\
_IOC_IN)
#define _IORW(x,y,t) ((x << 8) | y | ((sizeof(t) & _IOCPARM_MASK) << 16) |\
_IOC_INOUT)
#else
/* No fancy encoding on a 16-bit machine. */

#define _IO(x,y) ((x << 8) | y)
#define _IOR(x,y,t) _IO(x,y)
#define _IOW(x,y,t) _IO(x,y)
#define _IORW(x,y,t) _IO(x,y)
#endif

int ioctl(int _fd, int _request, void *_data);

#endif /* _M_IOCTL_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/config.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _CONFIG_H
#define _CONFIG_H

/* Minix release and version numbers. */
#define OS_RELEASE "3"
#define OS_VERSION "1.0"

/* This file sets configuration parameters for the MINIX kernel, FS, and PM.
* It is divided up into two main sections. The first section contains
* user-settable parameters. In the second section, various internal system
* parameters are set based on the user-settable parameters.
*
* Parts of config.h have been moved to sys_config.h, which can be included
* by other include files that wish to get at the configuration data, but
* don’t want to pollute the users namespace. Some editable values have
* gone there.
*
* This is a modified version of config.h for compiling a small Minix system
* with only the options described in the text, Operating Systems Design and
* Implementation, 3rd edition. See the version of config.h in the full
* source code directory for information on alternatives omitted here.
*/

/* The MACHINE (called _MINIX_MACHINE) setting can be done
* in <minix/machine.h>.
*/
#include <minix/sys_config.h>

#define MACHINE _MINIX_MACHINE

#define IBM_PC _MACHINE_IBM_PC
#define SUN_4 _MACHINE_SUN_4
#define SUN_4_60 _MACHINE_SUN_4_60
#define ATARI _MACHINE_ATARI
#define MACINTOSH _MACHINE_MACINTOSH
MINIX SOURCE CODE File: include/minix/config.h 665

/* Number of slots in the process table for non-kernel processes. The number
* of system processes defines how many processes with special privileges
* there can be. User processes share the same properties and count for one.
*
* These can be changed in sys_config.h.
*/
#define NR_PROCS _NR_PROCS
#define NR_SYS_PROCS _NR_SYS_PROCS

#define NR_BUFS 128
#define NR_BUF_HASH 128

/* Number of controller tasks (/dev/cN device classes). */
#define NR_CTRLRS 2

/* Enable or disable the second level file system cache on the RAM disk. */
#define ENABLE_CACHE2 0

/* Enable or disable swapping processes to disk. */
#define ENABLE_SWAP 0

/* Include or exclude an image of /dev/boot in the boot image.
* Please update the makefile in /usr/src/tools/ as well.
*/
#define ENABLE_BOOTDEV 0 /* load image of /dev/boot at boot time */

/* DMA_SECTORS may be increased to speed up DMA based drivers. */
#define DMA_SECTORS 1 /* DMA buffer size (must be >= 1) */

/* Include or exclude backwards compatibility code. */
#define ENABLE_BINCOMPAT 0 /* for binaries using obsolete calls */
#define ENABLE_SRCCOMPAT 0 /* for sources using obsolete calls */

/* Which process should receive diagnostics from the kernel and system?
* Directly sending it to TTY only displays the output. Sending it to the
* log driver will cause the diagnostics to be buffered and displayed.
*/
#define OUTPUT_PROC_NR LOG_PROC_NR /* TTY_PROC_NR or LOG_PROC_NR */

/* NR_CONS, NR_RS_LINES, and NR_PTYS determine the number of terminals the
* system can handle.
*/
#define NR_CONS 4 /* # system consoles (1 to 8) */
#define NR_RS_LINES 0 /* # rs232 terminals (0 to 4) */
#define NR_PTYS 0 /* # pseudo terminals (0 to 64) */

/*===========================================================================*
* There are no user-settable parameters after this line *
*===========================================================================*/
/* Set the CHIP type based on the machine selected. The symbol CHIP is actually
* indicative of more than just the CPU. For example, machines for which
* CHIP == INTEL are expected to have 8259A interrrupt controllers and the
* other properties of IBM PC/XT/AT/386 types machines in general. */
#define INTEL _CHIP_INTEL /* CHIP type for PC, XT, AT, 386 and clones */
#define M_CHIP_M/* CHIP type for Atari, Amiga, Macintosh */
#define SPARC _CHIP_SPARC /* CHIP type for SUN-4 (e.g. SPARCstation) */

/* Set the FP_FORMAT type based on the machine selected, either hw or sw */
#define FP_NONE _FP_NONE /* no floating point support */
666 File: include/minix/config.h MINIX SOURCE CODE
#define FP_IEEE _FP_IEEE /* conform IEEE floating point standard */

/* _MINIX_CHIP is defined in sys_config.h. */
#define CHIP _MINIX_CHIP

/* _MINIX_FP_FORMAT is defined in sys_config.h. */
#define FP_FORMAT _MINIX_FP_FORMAT

/* _ASKDEV and _FASTLOAD are defined in sys_config.h. */
#define ASKDEV _ASKDEV
#define FASTLOAD _FASTLOAD

#endif /* _CONFIG_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/sys_config.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _MINIX_SYS_CONFIG_H
#define _MINIX_SYS_CONFIG_H 1

/* This is a modified sys_config.h for compiling a small Minix system
* with only the options described in the text, Operating Systems Design and
* Implementation, 3rd edition. See the sys_config.h in the full
* source code directory for information on alternatives omitted here.
*/

/*===========================================================================*
* This section contains user-settable parameters *
*===========================================================================*/
#define _MINIX_MACHINE _MACHINE_IBM_PC

#define _MACHINE_IBM_PC 1 /* any 8088 or 80x86-based system */

/* Word size in bytes (a constant equal to sizeof(int)). */
#if __ACK__ || __GNUC__
#define _WORD_SIZE _EM_WSIZE
#define _PTR_SIZE _EM_WSIZE
#endif

#define _NR_PROCS 64
#define _NR_SYS_PROCS 32

/* Set the CHIP type based on the machine selected. The symbol CHIP is actually
* indicative of more than just the CPU. For example, machines for which
* CHIP == INTEL are expected to have 8259A interrrupt controllers and the
* other properties of IBM PC/XT/AT/386 types machines in general. */
#define _CHIP_INTEL 1 /* CHIP type for PC, XT, AT, 386 and clones */

/* Set the FP_FORMAT type based on the machine selected, either hw or sw */
#define _FP_NONE 0 /* no floating point support */
#define _FP_IEEE 1 /* conform IEEE floating point standard */

#define _MINIX_CHIP _CHIP_INTEL

#define _MINIX_FP_FORMAT _FP_NONE

#ifndef _MINIX_MACHINE
MINIX SOURCE CODE File: include/minix/sys_config.h 667
error "In <minix/sys_config.h> please define _MINIX_MACHINE"
#endif

#ifndef _MINIX_CHIP
error "In <minix/sys_config.h> please define _MINIX_MACHINE to have a legal value"
#endif

#if (_MINIX_MACHINE == 0)
error "_MINIX_MACHINE has incorrect value (0)"
#endif

#endif /* _MINIX_SYS_CONFIG_H */


++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* Copyright (C) 2001 by Prentice-Hall, Inc. See the copyright notice in
* the file /usr/src/LICENSE.
*/

#ifndef CHIP
#error CHIP is not defined
#endif

#define EXTERN extern /* used in *.h files */
#define PRIVATE static /* PRIVATE x limits the scope of x */
#define PUBLIC /* PUBLIC is the opposite of PRIVATE */
#define FORWARD static /* some compilers require this to be ’static’*/

#define TRUE 1 /* used for turning integers into Booleans */
#define FALSE 0 /* used for turning integers into Booleans */

#define HZ 60 /* clock freq (software settable on IBM-PC) */

#define SUPER_USER (uid_t) 0 /* uid_t of superuser */

/* Devices. */
#define MAJOR 8 /* major device = (dev>>MAJOR) & 0377 */
#define MINOR 0 /* minor device = (dev>>MINOR) & 0377 */

#define NULL ((void *)0) /* null pointer */
#define CPVEC_NR 16 /* max # of entries in a SYS_VCOPY request */
#define CPVVEC_NR 64 /* max # of entries in a SYS_VCOPY request */
#define NR_IOREQS MIN(NR_BUFS, 64)
/* maximum number of entries in an iorequest */

/* Message passing constants. */
#define MESS_SIZE (sizeof(message)) /* might need usizeof from FS here */
#define NIL_MESS ((message *) 0) /* null pointer */

/* Memory related constants. */
#define SEGMENT_TYPE 0xFF00 /* bit mask to get segment type */
#define SEGMENT_INDEX 0x00FF /* bit mask to get segment index */

#define LOCAL_SEG 0x0000 /* flags indicating local memory segment */
#define NR_LOCAL_SEGS 3 /* # local segments per process (fixed) */
668 File: include/minix/const.h MINIX SOURCE CODE
#define T 0 /* proc[i].mem_map[T] is for text */
#define D 1 /* proc[i].mem_map[D] is for data */
#define S 2 /* proc[i].mem_map[S] is for stack */

#define REMOTE_SEG 0x0100 /* flags indicating remote memory segment */
#define NR_REMOTE_SEGS 3 /* # remote memory regions (variable) */

#define BIOS_SEG 0x0200 /* flags indicating BIOS memory segment */
#define NR_BIOS_SEGS 3 /* # BIOS memory regions (variable) */

#define PHYS_SEG 0x0400 /* flag indicating entire physical memory */

/* Labels used to disable code sections for different reasons. */
#define DEAD_CODE 0 /* unused code in normal configuration */
#define FUTURE_CODE 0 /* new code to be activated + tested later */
#define TEMP_CODE 1 /* active code to be removed later */

/* Process name length in the PM process table, including ’\0’. */
#define PROC_NAME_LEN 16

/* Miscellaneous */
#define BYTE 0377 /* mask for 8 bits */
#define READING 0 /* copy data to user */
#define WRITING 1 /* copy data from user */
#define NO_NUM 0x8000 /* used as numerical argument to panic() */
#define NIL_PTR (char *) 0 /* generally useful expression */
#define HAVE_SCATTERED_IO 1 /* scattered I/O is now standard */

/* Macros. */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* Memory is allocated in clicks. */
#if (CHIP == INTEL)
#define CLICK_SIZE 1024 /* unit in which memory is allocated */
#define CLICK_SHIFT 10 /* log2 of CLICK_SIZE */
#endif

#if (CHIP == SPARC) || (CHIP == M)
#define CLICK_SIZE 4096 /* unit in which memory is allocated */
#define CLICK_SHIFT 12 /* log2 of CLICK_SIZE */
#endif

/* Click to byte conversions (and vice versa). */
#define HCLICK_SHIFT 4 /* log2 of HCLICK_SIZE */
#define HCLICK_SIZE 16 /* hardware segment conversion magic */
#if CLICK_SIZE >= HCLICK_SIZE
#define click_to_hclick(n) ((n) << (CLICK_SHIFT - HCLICK_SHIFT))
#else
#define click_to_hclick(n) ((n) >> (HCLICK_SHIFT - CLICK_SHIFT))
#endif
#define hclick_to_physb(n) ((phys_bytes) (n) << HCLICK_SHIFT)
#define physb_to_hclick(n) ((n) >> HCLICK_SHIFT)

#define ABS -999 /* this process means absolute memory */

/* Flag bits for i_mode in the inode. */
#define I_TYPE 01/* this field gives inode type */
#define I_REGULAR 01/* regular file, not dir or special */
#define I_BLOCK_SPECIAL 00/* block special file */
MINIX SOURCE CODE File: include/minix/const.h 669
#define I_DIRECTORY 00/* file is a directory */
#define I_CHAR_SPECIAL 00/* character special file */
#define I_NAMED_PIPE 00/* named pipe (FIFO) */
#define I_SET_UID_BIT 00/* set effective uid_t on exec */
#define I_SET_GID_BIT 00/* set effective gid_t on exec */
#define ALL_MODES 00/* all bits for user, group and others */
#define RWX_MODES 00/* mode bits for RWX only */
#define R_BIT 00/* Rwx protection bit */
#define W_BIT 00/* rWx protection bit */
#define X_BIT 00/* rwX protection bit */
#define I_NOT_ALLOC 00/* this inode is free */

/* Flag used only in flags argument of dev_open. */
#define RO_BIT 02/* Open device readonly; fail if writable. */

/* Some limits. */
#define MAX_BLOCK_NR ((block_t) 7777) /* largest block number */
#define HIGHEST_ZONE ((zone_t) 7777) /* largest zone number */
#define MAX_INODE_NR ((ino_t) 77) /* largest inode number */
#define MAX_FILE_POS ((off_t) 77) /* largest legal file offset */

#define NO_BLOCK ((block_t) 0) /* absence of a block number */
#define NO_ENTRY ((ino_t) 0) /* absence of a dir entry */
#define NO_ZONE ((zone_t) 0) /* absence of a zone number */
#define NO_DEV ((dev_t) 0) /* absence of a device numb */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/type.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _TYPE_H
#define _TYPE_H

#ifndef _MINIX_SYS_CONFIG_H
#include <minix/sys_config.h>
#endif

#ifndef _TYPES_H
#include <sys/types.h>
#endif

/* Type definitions. */
typedef unsigned int vir_clicks; /* virtual addr/length in clicks */
typedef unsigned long phys_bytes; /* physical addr/length in bytes */
typedef unsigned int phys_clicks; /* physical addr/length in clicks */

#if (_MINIX_CHIP == _CHIP_INTEL)
typedef unsigned int vir_bytes; /* virtual addresses and lengths in bytes */
#endif

#if (_MINIX_CHIP == _CHIP_M)
typedef unsigned long vir_bytes;/* virtual addresses and lengths in bytes */
#endif

#if (_MINIX_CHIP == _CHIP_SPARC)
typedef unsigned long vir_bytes;/* virtual addresses and lengths in bytes */
#endif

/* Memory map for local text, stack, data segments. */
struct mem_map {
670 File: include/minix/type.h MINIX SOURCE CODE
vir_clicks mem_vir; /* virtual address */
phys_clicks mem_phys; /* physical address */
vir_clicks mem_len; /* length */
};

/* Memory map for remote memory areas, e.g., for the RAM disk. */
struct far_mem {
int in_use; /* entry in use, unless zero */
phys_clicks mem_phys; /* physical address */
vir_clicks mem_len; /* length */
};

/* Structure for virtual copying by means of a vector with requests. */
struct vir_addr {
int proc_nr;
int segment;
vir_bytes offset;
};

#define phys_cp_req vir_cp_req
struct vir_cp_req {
struct vir_addr src;
struct vir_addr dst;
phys_bytes count;
};

typedef struct {
vir_bytes iov_addr; /* address of an I/O buffer */
vir_bytes iov_size; /* sizeof an I/O buffer */
} iovec_t;

/* PM passes the address of a structure of this type to KERNEL when
* sys_sendsig() is invoked as part of the signal catching mechanism.
* The structure contain all the information that KERNEL needs to build
* the signal stack.
*/
struct sigmsg {
int sm_signo; /* signal number being caught */
unsigned long sm_mask; /* mask to restore when handler returns */
vir_bytes sm_sighandler; /* address of handler */
vir_bytes sm_sigreturn; /* address of _sigreturn in C library */
vir_bytes sm_stkptr; /* user stack pointer */
};

/* This is used to obtain system information through SYS_GETINFO. */
struct kinfo {
phys_bytes code_base; /* base of kernel code */
phys_bytes code_size;
phys_bytes data_base; /* base of kernel data */
phys_bytes data_size;
vir_bytes proc_addr; /* virtual address of process table */
phys_bytes kmem_base; /* kernel memory layout (/dev/kmem) */
phys_bytes kmem_size;
phys_bytes bootdev_base; /* boot device from boot image (/dev/boot) */
phys_bytes bootdev_size;
phys_bytes bootdev_mem;
phys_bytes params_base; /* parameters passed by boot monitor */
phys_bytes params_size;
int nr_procs; /* number of user processes */
int nr_tasks; /* number of kernel tasks */
char release[6]; /* kernel release number */
char version[6]; /* kernel version number */
int relocking; /* relocking check (for debugging) */
};

struct machine {
int pc_at;
int ps_mca;
int processor;
int protected;
int vdu_ega;
int vdu_vga;
};

#endif /* _TYPE_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/ipc.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _IPC_H
#define _IPC_H

/*==========================================================================*
* Types relating to messages. *
*==========================================================================*/

#define M1 1
#define M3 3
#define M4 4
#define M3_STRING 14

typedef struct {int m1i1, m1i2, m1i3; char *m1p1, *m1p2, *m1p3;} mess_1;
typedef struct {int m2i1, m2i2, m2i3; long m2l1, m2l2; char *m2p1;} mess_2;
typedef struct {int m3i1, m3i2; char *m3p1; char m3ca1[M3_STRING];} mess_3;
typedef struct {long m4l1, m4l2, m4l3, m4l4, m4l5;} mess_4;
typedef struct {short m5c1, m5c2; int m5i1, m5i2; long m5l1, m5l2, m5l3;}mess_5;
typedef struct {int m7i1, m7i2, m7i3, m7i4; char *m7p1, *m7p2;} mess_7;
typedef struct {int m8i1, m8i2; char *m8p1, *m8p2, *m8p3, *m8p4;} mess_8;

typedef struct {
int m_source; /* who sent the message */
int m_type; /* what kind of message is it */
union {
mess_1 m_m1;
mess_2 m_m2;
mess_3 m_m3;
mess_4 m_m4;
mess_5 m_m5;
mess_7 m_m7;
mess_8 m_m8;
} m_u;
} message;

/* The following defines provide names for useful members. */
#define m1_i1 m_u.m_m1.m1i1
#define m1_i2 m_u.m_m1.m1i2
#define m1_i3 m_u.m_m1.m1i3
#define m1_p1 m_u.m_m1.m1p1
#define m1_p2 m_u.m_m1.m1p2
672 File: include/minix/ipc.h MINIX SOURCE CODE
#define m1_p3 m_u.m_m1.m1p3

#define m2_i1 m_u.m_m2.m2i1
#define m2_i2 m_u.m_m2.m2i2
#define m2_i3 m_u.m_m2.m2i3
#define m2_l1 m_u.m_m2.m2l1
#define m2_l2 m_u.m_m2.m2l2
#define m2_p1 m_u.m_m2.m2p1

#define m3_i1 m_u.m_m3.m3i1
#define m3_i2 m_u.m_m3.m3i2
#define m3_p1 m_u.m_m3.m3p1
#define m3_ca1 m_u.m_m3.m3ca1

#define m4_l1 m_u.m_m4.m4l1
#define m4_l2 m_u.m_m4.m4l2
#define m4_l3 m_u.m_m4.m4l3
#define m4_l4 m_u.m_m4.m4l4
#define m4_l5 m_u.m_m4.m4l5

#define m5_c1 m_u.m_m5.m5c1
#define m5_c2 m_u.m_m5.m5c2
#define m5_i1 m_u.m_m5.m5i1
#define m5_i2 m_u.m_m5.m5i2
#define m5_l1 m_u.m_m5.m5l1
#define m5_l2 m_u.m_m5.m5l2
#define m5_l3 m_u.m_m5.m5l3

#define m7_i1 m_u.m_m7.m7i1
#define m7_i2 m_u.m_m7.m7i2
#define m7_i3 m_u.m_m7.m7i3
#define m7_i4 m_u.m_m7.m7i4
#define m7_p1 m_u.m_m7.m7p1
#define m7_p2 m_u.m_m7.m7p2

#define m8_i1 m_u.m_m8.m8i1
#define m8_i2 m_u.m_m8.m8i2
#define m8_p1 m_u.m_m8.m8p1
#define m8_p2 m_u.m_m8.m8p2
#define m8_p3 m_u.m_m8.m8p3
#define m8_p4 m_u.m_m8.m8p4

/*==========================================================================*
* Minix run-time system (IPC). *
*==========================================================================*/

/* Hide names to avoid name space pollution. */
#define echo _echo
#define notify _notify
#define sendrec _sendrec
#define receive _receive
#define send _send
#define nb_receive _nb_receive
#define nb_send _nb_send

_PROTOTYPE( int echo, (message *m_ptr) );
_PROTOTYPE( int notify, (int dest) );
_PROTOTYPE( int sendrec, (int src_dest, message *m_ptr) );
_PROTOTYPE( int receive, (int src, message *m_ptr) );
_PROTOTYPE( int send, (int dest, message *m_ptr) );
MINIX SOURCE CODE File: include/minix/ipc.h 673
_PROTOTYPE( int nb_receive, (int src, message *m_ptr) );
_PROTOTYPE( int nb_send, (int dest, message *m_ptr) );

#endif /* _IPC_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/syslib.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* Prototypes for system library functions. */

#ifndef _SYSLIB_H
#define _SYSLIB_H

#ifndef _TYPES_H
#include <sys/types.h>
#endif

#ifndef _IPC_H
#include <minix/ipc.h>
#endif

#ifndef _DEVIO_H
#include <minix/devio.h>
#endif

/* Forward declaration */
struct reg86u;

#define SYSTASK SYSTEM

/*==========================================================================*
* Minix system library. *
*==========================================================================*/
_PROTOTYPE( int _taskcall, (int who, int syscallnr, message *msgptr));

_PROTOTYPE( int sys_abort, (int how, ...));
_PROTOTYPE( int sys_exec, (int proc, char *ptr,
char *aout, vir_bytes initpc));
_PROTOTYPE( int sys_fork, (int parent, int child));
_PROTOTYPE( int sys_newmap, (int proc, struct mem_map *ptr));
_PROTOTYPE( int sys_exit, (int proc));
_PROTOTYPE( int sys_trace, (int req, int proc, long addr, long *data_p));

_PROTOTYPE( int sys_svrctl, (int proc, int req, int priv,vir_bytes argp));
_PROTOTYPE( int sys_nice, (int proc, int priority));

_PROTOTYPE( int sys_int86, (struct reg86u *reg86p));

/* Shorthands for sys_sdevio() system call. */
#define sys_insb(port, proc_nr, buffer, count) \
sys_sdevio(DIO_INPUT, port, DIO_BYTE, proc_nr, buffer, count)
#define sys_insw(port, proc_nr, buffer, count) \
sys_sdevio(DIO_INPUT, port, DIO_WORD, proc_nr, buffer, count)
#define sys_outsb(port, proc_nr, buffer, count) \
sys_sdevio(DIO_OUTPUT, port, DIO_BYTE, proc_nr, buffer, count)
#define sys_outsw(port, proc_nr, buffer, count) \
sys_sdevio(DIO_OUTPUT, port, DIO_WORD, proc_nr, buffer, count)
_PROTOTYPE( int sys_sdevio, (int req, long port, int type, int proc_nr,
674 File: include/minix/syslib.h MINIX SOURCE CODE
void *buffer, int count));

/* Clock functionality: get system times or (un)schedule an alarm call. */
_PROTOTYPE( int sys_times, (int proc_nr, clock_t *ptr));
_PROTOTYPE(int sys_setalarm, (clock_t exp_time, int abs_time));

/* Shorthands for sys_irqctl() system call. */
#define sys_irqdisable(hook_id) \
sys_irqctl(IRQ_DISABLE, 0, 0, hook_id)
#define sys_irqenable(hook_id) \
sys_irqctl(IRQ_ENABLE, 0, 0, hook_id)
#define sys_irqsetpolicy(irq_vec, policy, hook_id) \
sys_irqctl(IRQ_SETPOLICY, irq_vec, policy, hook_id)
#define sys_irqrmpolicy(irq_vec, hook_id) \
sys_irqctl(IRQ_RMPOLICY, irq_vec, 0, hook_id)
_PROTOTYPE ( int sys_irqctl, (int request, int irq_vec, int policy,
int *irq_hook_id) );

/* Shorthands for sys_vircopy() and sys_physcopy() system calls. */
#define sys_biosin(bios_vir, dst_vir, bytes) \
sys_vircopy(SELF, BIOS_SEG, bios_vir, SELF, D, dst_vir, bytes)
#define sys_biosout(src_vir, bios_vir, bytes) \
sys_vircopy(SELF, D, src_vir, SELF, BIOS_SEG, bios_vir, bytes)
#define sys_datacopy(src_proc, src_vir, dst_proc, dst_vir, bytes) \
sys_vircopy(src_proc, D, src_vir, dst_proc, D, dst_vir, bytes)
#define sys_textcopy(src_proc, src_vir, dst_proc, dst_vir, bytes) \
sys_vircopy(src_proc, T, src_vir, dst_proc, T, dst_vir, bytes)
#define sys_stackcopy(src_proc, src_vir, dst_proc, dst_vir, bytes) \
sys_vircopy(src_proc, S, src_vir, dst_proc, S, dst_vir, bytes)
_PROTOTYPE(int sys_vircopy, (int src_proc, int src_seg, vir_bytes src_vir,
int dst_proc, int dst_seg, vir_bytes dst_vir, phys_bytes bytes));

#define sys_abscopy(src_phys, dst_phys, bytes) \
sys_physcopy(NONE, PHYS_SEG, src_phys, NONE, PHYS_SEG, dst_phys, bytes)
_PROTOTYPE(int sys_physcopy, (int src_proc, int src_seg, vir_bytes src_vir,
int dst_proc, int dst_seg, vir_bytes dst_vir, phys_bytes bytes));
_PROTOTYPE(int sys_memset, (unsigned long pattern,
phys_bytes base, phys_bytes bytes));

/* Vectored virtual / physical copy calls. */
#if DEAD_CODE /* library part not yet implemented */
_PROTOTYPE(int sys_virvcopy, (phys_cp_req *vec_ptr,int vec_size,int *nr_ok));
_PROTOTYPE(int sys_physvcopy, (phys_cp_req *vec_ptr,int vec_size,int *nr_ok));
#endif

_PROTOTYPE(int sys_umap, (int proc_nr, int seg, vir_bytes vir_addr,
vir_bytes bytes, phys_bytes *phys_addr));
_PROTOTYPE(int sys_segctl, (int *index, u16_t *seg, vir_bytes *off,
phys_bytes phys, vir_bytes size));

/* Shorthands for sys_getinfo() system call. */
#define sys_getkmessages(dst) sys_getinfo(GET_KMESSAGES, dst, 0,0,0)
#define sys_getkinfo(dst) sys_getinfo(GET_KINFO, dst, 0,0,0)
#define sys_getmachine(dst) sys_getinfo(GET_MACHINE, dst, 0,0,0)
#define sys_getproctab(dst) sys_getinfo(GET_PROCTAB, dst, 0,0,0)
#define sys_getprivtab(dst) sys_getinfo(GET_PRIVTAB, dst, 0,0,0)
#define sys_getproc(dst,nr) sys_getinfo(GET_PROC, dst, 0,0, nr)
#define sys_getrandomness(dst) sys_getinfo(GET_RANDOMNESS, dst, 0,0,0)
#define sys_getimage(dst) sys_getinfo(GET_IMAGE, dst, 0,0,0)
#define sys_getirqhooks(dst) sys_getinfo(GET_IRQHOOKS, dst, 0,0,0)
MINIX SOURCE CODE File: include/minix/syslib.h 675
#define sys_getmonparams(v,vl) sys_getinfo(GET_MONPARAMS, v,vl, 0,0)
#define sys_getschedinfo(v1,v2) sys_getinfo(GET_SCHEDINFO, v1,0, v2,0)
#define sys_getlocktimings(dst) sys_getinfo(GET_LOCKTIMING, dst, 0,0,0)
#define sys_getbiosbuffer(virp, sizep) sys_getinfo(GET_BIOSBUFFER, virp, \
sizeof(*virp), sizep, sizeof(*sizep))
_PROTOTYPE(int sys_getinfo, (int request, void *val_ptr, int val_len,
void *val_ptr2, int val_len2) );

/* Signal control. */
_PROTOTYPE(int sys_kill, (int proc, int sig) );
_PROTOTYPE(int sys_sigsend, (int proc_nr, struct sigmsg *sig_ctxt) );
_PROTOTYPE(int sys_sigreturn, (int proc_nr, struct sigmsg *sig_ctxt) );
_PROTOTYPE(int sys_getksig, (int *k_proc_nr, sigset_t *k_sig_map) );
_PROTOTYPE(int sys_endksig, (int proc_nr) );

/* NOTE: two different approaches were used to distinguish the device I/O
* types ’byte’, ’word’, ’long’: the latter uses #define and results in a
* smaller implementation, but looses the static type checking.
*/
_PROTOTYPE(int sys_voutb, (pvb_pair_t *pvb_pairs, int nr_ports) );
_PROTOTYPE(int sys_voutw, (pvw_pair_t *pvw_pairs, int nr_ports) );
_PROTOTYPE(int sys_voutl, (pvl_pair_t *pvl_pairs, int nr_ports) );
_PROTOTYPE(int sys_vinb, (pvb_pair_t *pvb_pairs, int nr_ports) );
_PROTOTYPE(int sys_vinw, (pvw_pair_t *pvw_pairs, int nr_ports) );
_PROTOTYPE(int sys_vinl, (pvl_pair_t *pvl_pairs, int nr_ports) );

/* Shorthands for sys_out() system call. */
#define sys_outb(p,v) sys_out((p), (unsigned long) (v), DIO_BYTE)
#define sys_outw(p,v) sys_out((p), (unsigned long) (v), DIO_WORD)
#define sys_outl(p,v) sys_out((p), (unsigned long) (v), DIO_LONG)
_PROTOTYPE(int sys_out, (int port, unsigned long value, int type) );

/* Shorthands for sys_in() system call. */
#define sys_inb(p,v) sys_in((p), (unsigned long*) (v), DIO_BYTE)
#define sys_inw(p,v) sys_in((p), (unsigned long*) (v), DIO_WORD)
#define sys_inl(p,v) sys_in((p), (unsigned long*) (v), DIO_LONG)
_PROTOTYPE(int sys_in, (int port, unsigned long *value, int type) );

#endif /* _SYSLIB_H */

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/sysutil.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _EXTRALIB_H
#define _EXTRALIB_H

/* Extra system library definitions to support device drivers and servers.
*
* Created:
* Mar 15, 2004 by Jorrit N. Herder
*
* Changes:
* May 31, 2005: added printf, kputc (relocated from syslib)
* May 31, 2005: added getuptime
* Mar 18, 2005: added tickdelay
* Oct 01, 2004: added env_parse, env_prefix, env_panic
* Jul 13, 2004: added fkey_ctl
* Apr 28, 2004: added report, panic
676 File: include/minix/sysutil.h MINIX SOURCE CODE
* Mar 31, 2004: setup like other libraries, such as syslib
*/

/*==========================================================================*
* Miscellaneous helper functions.
*==========================================================================*/

/* Environment parsing return values. */
#define EP_BUF_SIZE 128 /* local buffer for env value */
#define EP_UNSET 0 /* variable not set */
#define EP_OFF 1 /* var = off */
#define EP_ON 2 /* var = on (or field left blank) */
#define EP_SET 3 /* var = 1:2:3 (nonblank field) */
#define EP_EGETKENV 4 /* sys_getkenv() failed ... */

_PROTOTYPE( void env_setargs, (int argc, char *argv[]) );
_PROTOTYPE( int env_get_param, (char *key, char *value, int max_size) );
_PROTOTYPE( int env_prefix, (char *env, char *prefix) );
_PROTOTYPE( void env_panic, (char *key) );
_PROTOTYPE( int env_parse, (char *env, char *fmt, int field, long *param,
long min, long max) );

#define fkey_map(fkeys, sfkeys) fkey_ctl(FKEY_MAP, (fkeys), (sfkeys))
#define fkey_unmap(fkeys, sfkeys) fkey_ctl(FKEY_UNMAP, (fkeys), (sfkeys))
#define fkey_events(fkeys, sfkeys) fkey_ctl(FKEY_EVENTS, (fkeys), (sfkeys))
_PROTOTYPE( int fkey_ctl, (int req, int *fkeys, int *sfkeys) );

_PROTOTYPE( int printf, (const char *fmt, ...));
_PROTOTYPE( void kputc, (int c));
_PROTOTYPE( void report, (char *who, char *mess, int num));
_PROTOTYPE( void panic, (char *who, char *mess, int num));
_PROTOTYPE( int getuptime, (clock_t *ticks));
_PROTOTYPE( int tickdelay, (clock_t ticks));

#endif /* _EXTRALIB_H */

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/callnr.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define NCALLS 91 /* number of system calls allowed */

#define EXIT 1
#define FORK 2
#define READ 3
#define WRITE 4
#define OPEN 5
#define CLOSE 6
#define WAIT 7
#define CREAT 8
#define LINK 9
#define UNLINK 10
#define WAITPID 11
#define CHDIR 12
#define TIME 13
MINIX SOURCE CODE File: include/minix/callnr.h 677
#define MKNOD 14
#define CHMOD 15
#define CHOWN 16
#define BRK 17
#define STAT 18
#define LSEEK 19
#define GETPID 20
#define MOUNT 21
#define UMOUNT 22
#define SETUID 23
#define GETUID 24
#define STIME 25
#define PTRACE 26
#define ALARM 27
#define FSTAT 28
#define PAUSE 29
#define UTIME 30
#define ACCESS 33
#define SYNC 36
#define KILL 37
#define RENAME 38
#define MKDIR 39
#define RMDIR 40
#define DUP 41
#define PIPE 42
#define TIMES 43
#define SETGID 46
#define GETGID 47
#define SIGNAL 48
#define IOCTL 54
#define FCNTL 55
#define EXEC 59
#define UMASK 60
#define CHROOT 61
#define SETSID 62
#define GETPGRP 63

/* The following are not system calls, but are processed like them. */
#define UNPAUSE 65 /* to MM or FS: check for EINTR */
#define REVIVE 67 /* to FS: revive a sleeping process */
#define TASK_REPLY 68 /* to FS: reply code from tty task */

/* Posix signal handling. */
#define SIGACTION 71
#define SIGSUSPEND 72
#define SIGPENDING 73
#define SIGPROCMASK 74
#define SIGRETURN 75

#define REBOOT 76 /* to PM */

/* MINIX specific calls, e.g., to support system services. */
#define SVRCTL 77
/* unused */
#define GETSYSINFO 79 /* to PM or FS */
#define GETPROCNR 80 /* to PM */
#define DEVCTL 81 /* to FS */
#define FSTATFS 82 /* to FS */
#define ALLOCMEM 83 /* to PM */
#define FREEMEM 84 /* to PM */
678 File: include/minix/callnr.h MINIX SOURCE CODE
#define SELECT 85 /* to FS */
#define FCHDIR 86 /* to FS */
#define FSYNC 87 /* to FS */
#define GETPRIORITY 88 /* to PM */
#define SETPRIORITY 89 /* to PM */
#define GETTIMEOFDAY 90 /* to PM */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/com.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _MINIX_COM_H
#define _MINIX_COM_H

/*===========================================================================*
* Magic process numbers *
*===========================================================================*/

#define ANY 0x7ace /* used to indicate ’any process’ */
#define NONE 0x6ace /* used to indicate ’no process at all’ */
#define SELF 0x8ace /* used to indicate ’own process’ */

/*===========================================================================*
* Process numbers of processes in the system image *
*===========================================================================*/

/* The values of several task numbers depend on whether they or other tasks
* are enabled. They are defined as (PREVIOUS_TASK - ENABLE_TASK) in general.
* ENABLE_TASK is either 0 or 1, so a task either gets a new number, or gets
* the same number as the previous task and is further unused. Note that the
* order should correspond to the order in the task table defined in table.c.
*/

/* Kernel tasks. These all run in the same address space. */
#define IDLE -4 /* runs when no one else can run */
#define CLOCK -3 /* alarms and other clock functions */
#define SYSTEM -2 /* request system functionality */
#define KERNEL -1 /* pseudo-process for IPC and scheduling */
#define HARDWARE KERNEL /* for hardware interrupt handlers */

/* Number of tasks. Note that NR_PROCS is defined in <minix/config.h>. */
#define NR_TASKS 4

/* User-space processes, that is, device drivers, servers, and INIT. */
#define PM_PROC_NR 0 /* process manager */
#define FS_PROC_NR 1 /* file system */
#define RS_PROC_NR 2 /* reincarnation server */
#define MEM_PROC_NR 3 /* memory driver (RAM disk, null, etc.) */
#define LOG_PROC_NR 4 /* log device driver */
#define TTY_PROC_NR 5 /* terminal (TTY) driver */
#define DRVR_PROC_NR 6 /* device driver for boot medium */
#define INIT_PROC_NR 7 /* init -- goes multiuser */

/* Number of processes contained in the system image. */
#define NR_BOOT_PROCS (NR_TASKS + INIT_PROC_NR + 1)

MINIX SOURCE CODE File: include/minix/com.h 679
/*===========================================================================*
* Kernel notification types *
*===========================================================================*/

/* Kernel notification types. In principle, these can be sent to any process,
* so make sure that these types do not interfere with other message types.
* Notifications are prioritized because of the way they are unhold() and
* blocking notifications are delivered. The lowest numbers go first. The
* offset are used for the per-process notification bit maps.
*/
#define NOTIFY_MESSAGE 0x1000
#define NOTIFY_FROM(p_nr) (NOTIFY_MESSAGE | ((p_nr) + NR_TASKS))
# define SYN_ALARM NOTIFY_FROM(CLOCK) /* synchronous alarm */
# define SYS_SIG NOTIFY_FROM(SYSTEM) /* system signal */
# define HARD_INT NOTIFY_FROM(HARDWARE) /* hardware interrupt */
# define NEW_KSIG NOTIFY_FROM(HARDWARE) /* new kernel signal */
# define FKEY_PRESSED NOTIFY_FROM(TTY_PROC_NR)/* function key press */

/* Shorthands for message parameters passed with notifications. */
#define NOTIFY_SOURCE m_source
#define NOTIFY_TYPE m_type
#define NOTIFY_ARG m2_l1
#define NOTIFY_TIMESTAMP m2_l2
#define NOTIFY_FLAGS m2_i1

/*===========================================================================*
* Messages for BLOCK and CHARACTER device drivers *
*===========================================================================*/

/* Message types for device drivers. */
#define DEV_RQ_BASE 0x400 /* base for device request types */
#define DEV_RS_BASE 0x500 /* base for device response types */

#define CANCEL (DEV_RQ_BASE + 0) /* general req to force a task to cancel */
#define DEV_READ (DEV_RQ_BASE + 3) /* read from minor device */
#define DEV_WRITE (DEV_RQ_BASE + 4) /* write to minor device */
#define DEV_IOCTL (DEV_RQ_BASE + 5) /* I/O control code */
#define DEV_OPEN (DEV_RQ_BASE + 6) /* open a minor device */
#define DEV_CLOSE (DEV_RQ_BASE + 7) /* close a minor device */
#define DEV_SCATTER (DEV_RQ_BASE + 8) /* write from a vector */
#define DEV_GATHER (DEV_RQ_BASE + 9) /* read into a vector */
#define TTY_SETPGRP (DEV_RQ_BASE + 10) /* set process group */
#define TTY_EXIT (DEV_RQ_BASE + 11) /* process group leader exited */
#define DEV_SELECT (DEV_RQ_BASE + 12) /* request select() attention */
#define DEV_STATUS (DEV_RQ_BASE + 13) /* request driver status */

#define DEV_REPLY (DEV_RS_BASE + 0) /* general task reply */
#define DEV_CLONED (DEV_RS_BASE + 1) /* return cloned minor */
#define DEV_REVIVE (DEV_RS_BASE + 2) /* driver revives process */
#define DEV_IO_READY (DEV_RS_BASE + 3) /* selected device ready */
#define DEV_NO_STATUS (DEV_RS_BASE + 4) /* empty status reply */

/* Field names for messages to block and character device drivers. */
#define DEVICE m2_i1 /* major-minor device */
#define PROC_NR m2_i2 /* which (proc) wants I/O? */
#define COUNT m2_i3 /* how many bytes to transfer */
#define REQUEST m2_i3 /* ioctl request code */
#define POSITION m2_l1 /* file offset */
#define ADDRESS m2_p1 /* core buffer address */

680 File: include/minix/com.h MINIX SOURCE CODE
/* Field names for DEV_SELECT messages to device drivers. */
#define DEV_MINOR m2_i1 /* minor device */
#define DEV_SEL_OPS m2_i2 /* which select operations are requested */
#define DEV_SEL_WATCH m2_i3 /* request notify if no operations are ready */

/* Field names used in reply messages from tasks. */
#define REP_PROC_NR m2_i1 /* # of proc on whose behalf I/O was done */
#define REP_STATUS m2_i2 /* bytes transferred or error number */
# define SUSPEND -998 /* status to suspend caller, reply later */

/* Field names for messages to TTY driver. */
#define TTY_LINE DEVICE /* message parameter: terminal line */
#define TTY_REQUEST COUNT /* message parameter: ioctl request code */
#define TTY_SPEK POSITION/* message parameter: ioctl speed, erasing */
#define TTY_FLAGS m2_l2 /* message parameter: ioctl tty mode */
#define TTY_PGRP m2_i3 /* message parameter: process group */

/* Field names for the QIC 02 status reply from tape driver */
#define TAPE_STAT0 m2_l1
#define TAPE_STAT1 m2_l2

/*===========================================================================*
* Messages for networking layer *
*===========================================================================*/

/* Message types for network layer requests. This layer acts like a driver. */
#define NW_OPEN DEV_OPEN
#define NW_CLOSE DEV_CLOSE
#define NW_READ DEV_READ
#define NW_WRITE DEV_WRITE
#define NW_IOCTL DEV_IOCTL
#define NW_CANCEL CANCEL

/* Base type for data link layer requests and responses. */
#define DL_RQ_BASE 0x800
#define DL_RS_BASE 0x900

/* Message types for data link layer requests. */
#define DL_WRITE (DL_RQ_BASE + 3)
#define DL_WRITEV (DL_RQ_BASE + 4)
#define DL_READ (DL_RQ_BASE + 5)
#define DL_READV (DL_RQ_BASE + 6)
#define DL_INIT (DL_RQ_BASE + 7)
#define DL_STOP (DL_RQ_BASE + 8)
#define DL_GETSTAT (DL_RQ_BASE + 9)

/* Message type for data link layer replies. */
#define DL_INIT_REPLY (DL_RS_BASE + 20)
#define DL_TASK_REPLY (DL_RS_BASE + 21)

/* Field names for data link layer messages. */
#define DL_PORT m2_i1
#define DL_PROC m2_i2
#define DL_COUNT m2_i3
#define DL_MODE m2_l1
#define DL_CLCK m2_l2
#define DL_ADDR m2_p1
#define DL_STAT m2_l1

/* Bits in ’DL_STAT’ field of DL replies. */
MINIX SOURCE CODE File: include/minix/com.h 681
# define DL_PACK_SEND 0x01
# define DL_PACK_RECV 0x02
# define DL_READ_IP 0x04

/* Bits in ’DL_MODE’ field of DL requests. */
# define DL_NOMODE 0x0
# define DL_PROMISC_REQ 0x2
# define DL_MULTI_REQ 0x4
# define DL_BROAD_REQ 0x8

/*===========================================================================*
* SYSTASK request types and field names *
*===========================================================================*/

/* System library calls are dispatched via a call vector, so be careful when
* modifying the system call numbers. The numbers here determine which call
* is made from the call vector.
*/
#define KERNEL_CALL 0x600 /* base for kernel calls to SYSTEM */

# define SYS_FORK (KERNEL_CALL + 0) /* sys_fork() */
# define SYS_EXEC (KERNEL_CALL + 1) /* sys_exec() */
# define SYS_EXIT (KERNEL_CALL + 2) /* sys_exit() */
# define SYS_NICE (KERNEL_CALL + 3) /* sys_nice() */
# define SYS_PRIVCTL (KERNEL_CALL + 4) /* sys_privctl() */
# define SYS_TRACE (KERNEL_CALL + 5) /* sys_trace() */
# define SYS_KILL (KERNEL_CALL + 6) /* sys_kill() */

# define SYS_GETKSIG (KERNEL_CALL + 7) /* sys_getsig() */
# define SYS_ENDKSIG (KERNEL_CALL + 8) /* sys_endsig() */
# define SYS_SIGSEND (KERNEL_CALL + 9) /* sys_sigsend() */
# define SYS_SIGRETURN (KERNEL_CALL + 10) /* sys_sigreturn() */

# define SYS_NEWMAP (KERNEL_CALL + 11) /* sys_newmap() */
# define SYS_SEGCTL (KERNEL_CALL + 12) /* sys_segctl() */
# define SYS_MEMSET (KERNEL_CALL + 13) /* sys_memset() */

# define SYS_UMAP (KERNEL_CALL + 14) /* sys_umap() */
# define SYS_VIRCOPY (KERNEL_CALL + 15) /* sys_vircopy() */
# define SYS_PHYSCOPY (KERNEL_CALL + 16) /* sys_physcopy() */
# define SYS_VIRVCOPY (KERNEL_CALL + 17) /* sys_virvcopy() */
# define SYS_PHYSVCOPY (KERNEL_CALL + 18) /* sys_physvcopy() */

# define SYS_IRQCTL (KERNEL_CALL + 19) /* sys_irqctl() */
# define SYS_INT86 (KERNEL_CALL + 20) /* sys_int86() */
# define SYS_DEVIO (KERNEL_CALL + 21) /* sys_devio() */
# define SYS_SDEVIO (KERNEL_CALL + 22) /* sys_sdevio() */
# define SYS_VDEVIO (KERNEL_CALL + 23) /* sys_vdevio() */

# define SYS_SETALARM (KERNEL_CALL + 24) /* sys_setalarm() */
# define SYS_TIMES (KERNEL_CALL + 25) /* sys_times() */
# define SYS_GETINFO (KERNEL_CALL + 26) /* sys_getinfo() */
# define SYS_ABORT (KERNEL_CALL + 27) /* sys_abort() */

#define NR_SYS_CALLS 28 /* number of system calls */

/* Field names for SYS_MEMSET, SYS_SEGCTL. */
#define MEM_PTR m2_p1 /* base */
#define MEM_COUNT m2_l1 /* count */
#define MEM_PATTERN m2_l2 /* pattern to write */
682 File: include/minix/com.h MINIX SOURCE CODE
#define MEM_CHUNK_BASE m4_l1 /* physical base address */
#define MEM_CHUNK_SIZE m4_l2 /* size of mem chunk */
#define MEM_TOT_SIZE m4_l3 /* total memory size */
#define MEM_CHUNK_TAG m4_l4 /* tag to identify chunk of mem */

/* Field names for SYS_DEVIO, SYS_VDEVIO, SYS_SDEVIO. */
#define DIO_REQUEST m2_i3 /* device in or output */
# define DIO_INPUT 0 /* input */
# define DIO_OUTPUT 1 /* output */
#define DIO_TYPE m2_i1 /* flag indicating byte, word, or long */
# define DIO_BYTE ’b’ /* byte type values */
# define DIO_WORD ’w’ /* word type values */
# define DIO_LONG ’l’ /* long type values */
#define DIO_PORT m2_l1 /* single port address */
#define DIO_VALUE m2_l2 /* single I/O value */
#define DIO_VEC_ADDR m2_p1 /* address of buffer or (p,v)-pairs */
#define DIO_VEC_SIZE m2_l2 /* number of elements in vector */
#define DIO_VEC_PROC m2_i2 /* number of process where vector is */

/* Field names for SYS_SIGNARLM, SYS_FLAGARLM, SYS_SYNCALRM. */
#define ALRM_EXP_TIME m2_l1 /* expire time for the alarm call */
#define ALRM_ABS_TIME m2_i2 /* set to 1 to use absolute alarm time */
#define ALRM_TIME_LEFT m2_l1 /* how many ticks were remaining */
#define ALRM_PROC_NR m2_i1 /* which process wants the alarm? */
#define ALRM_FLAG_PTR m2_p1 /* virtual address of timeout flag */

/* Field names for SYS_IRQCTL. */
#define IRQ_REQUEST m5_c1 /* what to do? */
# define IRQ_SETPOLICY 1 /* manage a slot of the IRQ table */
# define IRQ_RMPOLICY 2 /* remove a slot of the IRQ table */
# define IRQ_ENABLE 3 /* enable interrupts */
# define IRQ_DISABLE 4 /* disable interrupts */
#define IRQ_VECTOR m5_c2 /* irq vector */
#define IRQ_POLICY m5_i1 /* options for IRQCTL request */
# define IRQ_REENABLE 0x001 /* reenable IRQ line after interrupt */
# define IRQ_BYTE 0x100 /* byte values */
# define IRQ_WORD 0x200 /* word values */
# define IRQ_LONG 0x400 /* long values */
#define IRQ_PROC_NR m5_i2 /* process number, SELF, NONE */
#define IRQ_HOOK_ID m5_l3 /* id of irq hook at kernel */

/* Field names for SYS_SEGCTL. */
#define SEG_SELECT m4_l1 /* segment selector returned */
#define SEG_OFFSET m4_l2 /* offset in segment returned */
#define SEG_PHYS m4_l3 /* physical address of segment */
#define SEG_SIZE m4_l4 /* segment size */
#define SEG_INDEX m4_l5 /* segment index in remote map */

/* Field names for SYS_VIDCOPY. */
#define VID_REQUEST m4_l1 /* what to do? */
# define VID_VID_COPY 1 /* request vid_vid_copy() */
# define MEM_VID_COPY 2 /* request mem_vid_copy() */
#define VID_SRC_ADDR m4_l2 /* virtual address in memory */
#define VID_SRC_OFFSET m4_l3 /* offset in video memory */
#define VID_DST_OFFSET m4_l4 /* offset in video memory */
#define VID_CP_COUNT m4_l5 /* number of words to be copied */

/* Field names for SYS_ABORT. */
#define ABRT_HOW m1_i1 /* RBT_REBOOT, RBT_HALT, etc. */
#define ABRT_MON_PROC m1_i2 /* process where monitor params are */
MINIX SOURCE CODE File: include/minix/com.h 683
#define ABRT_MON_LEN m1_i3 /* length of monitor params */
#define ABRT_MON_ADDR m1_p1 /* virtual address of monitor params */

/* Field names for _UMAP, _VIRCOPY, _PHYSCOPY. */
#define CP_SRC_SPACE m5_c1 /* T or D space (stack is also D) */
#define CP_SRC_PROC_NR m5_i1 /* process to copy from */
#define CP_SRC_ADDR m5_l1 /* address where data come from */
#define CP_DST_SPACE m5_c2 /* T or D space (stack is also D) */
#define CP_DST_PROC_NR m5_i2 /* process to copy to */
#define CP_DST_ADDR m5_l2 /* address where data go to */
#define CP_NR_BYTES m5_l3 /* number of bytes to copy */

/* Field names for SYS_VCOPY and SYS_VVIRCOPY. */
#define VCP_NR_OK m1_i2 /* number of successfull copies */
#define VCP_VEC_SIZE m1_i3 /* size of copy vector */
#define VCP_VEC_ADDR m1_p1 /* pointer to copy vector */

/* Field names for SYS_GETINFO. */
#define I_REQUEST m7_i3 /* what info to get */
# define GET_KINFO 0 /* get kernel information structure */
# define GET_IMAGE 1 /* get system image table */
# define GET_PROCTAB 2 /* get kernel process table */
# define GET_RANDOMNESS 3 /* get randomness buffer */
# define GET_MONPARAMS 4 /* get monitor parameters */
# define GET_KENV 5 /* get kernel environment string */
# define GET_IRQHOOKS 6 /* get the IRQ table */
# define GET_KMESSAGES 7 /* get kernel messages */
# define GET_PRIVTAB 8 /* get kernel privileges table */
# define GET_KADDRESSES 9 /* get various kernel addresses */
# define GET_SCHEDINFO 10 /* get scheduling queues */
# define GET_PROC 11 /* get process slot if given process */
# define GET_MACHINE 12 /* get machine information */
# define GET_LOCKTIMING 13 /* get lock()/unlock() latency timing */
# define GET_BIOSBUFFER 14 /* get a buffer for BIOS calls */
#define I_PROC_NR m7_i4 /* calling process */
#define I_VAL_PTR m7_p1 /* virtual address at caller */
#define I_VAL_LEN m7_i1 /* max length of value */
#define I_VAL_PTR2 m7_p2 /* second virtual address */
#define I_VAL_LEN2 m7_i2 /* second length, or proc nr */

/* Field names for SYS_TIMES. */
#define T_PROC_NR m4_l1 /* process to request time info for */
#define T_USER_TIME m4_l1 /* user time consumed by process */
#define T_SYSTEM_TIME m4_l2 /* system time consumed by process */
#define T_CHILD_UTIME m4_l3 /* user time consumed by process’ children */
#define T_CHILD_STIME m4_l4 /* sys time consumed by process’ children */
#define T_BOOT_TICKS m4_l5 /* number of clock ticks since boot time */

/* Field names for SYS_TRACE, SYS_SVRCTL. */
#define CTL_PROC_NR m2_i1 /* process number of the caller */
#define CTL_REQUEST m2_i2 /* server control request */
#define CTL_MM_PRIV m2_i3 /* privilege as seen by PM */
#define CTL_ARG_PTR m2_p1 /* pointer to argument */
#define CTL_ADDRESS m2_l1 /* address at traced process’ space */
#define CTL_DATA m2_l2 /* data field for tracing */

/* Field names for SYS_KILL, SYS_SIGCTL */
#define SIG_REQUEST m2_l2 /* PM signal control request */
#define S_GETSIG 0 /* get pending kernel signal */
#define S_ENDSIG 1 /* finish a kernel signal */
684 File: include/minix/com.h MINIX SOURCE CODE
#define S_SENDSIG 2 /* POSIX style signal handling */
#define S_SIGRETURN 3 /* return from POSIX handling */
#define S_KILL 4 /* servers kills process with signal */
#define SIG_PROC m2_i1 /* process number for inform */
#define SIG_NUMBER m2_i2 /* signal number to send */
#define SIG_FLAGS m2_i3 /* signal flags field */
#define SIG_MAP m2_l1 /* used by kernel to pass signal bit map */
#define SIG_CTXT_PTR m2_p1 /* pointer to info to restore signal context */

/* Field names for SYS_FORK, _EXEC, _EXIT, _NEWMAP. */
#define PR_PROC_NR m1_i1 /* indicates a (child) process */
#define PR_PRIORITY m1_i2 /* process priority */
#define PR_PPROC_NR m1_i2 /* indicates a (parent) process */
#define PR_PID m1_i3 /* process id at process manager */
#define PR_STACK_PTR m1_p1 /* used for stack ptr in sys_exec, sys_getsp */
#define PR_TRACING m1_i3 /* flag to indicate tracing is on/ off */
#define PR_NAME_PTR m1_p2 /* tells where program name is for dmp */
#define PR_IP_PTR m1_p3 /* initial value for ip after exec */
#define PR_MEM_PTR m1_p1 /* tells where memory map is for sys_newmap */

/* Field names for SYS_INT86 */
#define INT86_REG86 m1_p1 /* pointer to registers */

/* Field names for SELECT (FS). */
#define SEL_NFDS m8_i1
#define SEL_READFDS m8_p1
#define SEL_WRITEFDS m8_p2
#define SEL_ERRORFDS m8_p3
#define SEL_TIMEOUT m8_p4

/*===========================================================================*
* Messages for system management server *
*===========================================================================*/

#define SRV_RQ_BASE 0x700

#define SRV_UP (SRV_RQ_BASE + 0) /* start system service */
#define SRV_DOWN (SRV_RQ_BASE + 1) /* stop system service */
#define SRV_STATUS (SRV_RQ_BASE + 2) /* get service status */

# define SRV_PATH_ADDR m1_p1 /* path of binary */
# define SRV_PATH_LEN m1_i1 /* length of binary */
# define SRV_ARGS_ADDR m1_p2 /* arguments to be passed */
# define SRV_ARGS_LEN m1_i2 /* length of arguments */
# define SRV_DEV_MAJOR m1_i3 /* major device number */
# define SRV_PRIV_ADDR m1_p3 /* privileges string */
# define SRV_PRIV_LEN m1_i3 /* length of privileges */

/*===========================================================================*
* Miscellaneous messages used by TTY *
*===========================================================================*/

/* Miscellaneous request types and field names, e.g. used by IS server. */
#define PANIC_DUMPS 97 /* debug dumps at the TTY on RBT_PANIC */
#define FKEY_CONTROL 98 /* control a function key at the TTY */
# define FKEY_REQUEST m2_i1 /* request to perform at TTY */
# define FKEY_MAP 10 /* observe function key */
# define FKEY_UNMAP 11 /* stop observing function key */
# define FKEY_EVENTS 12 /* request open key presses */
# define FKEY_FKEYS m2_l1 /* F1-F12 keys pressed */
MINIX SOURCE CODE File: include/minix/com.h 685
# define FKEY_SFKEYS m2_l2 /* Shift-F1-F12 keys pressed */
#define DIAGNOSTICS 100 /* output a string without FS in between */
# define DIAG_PRINT_BUF m1_p1
# define DIAG_BUF_COUNT m1_i1
# define DIAG_PROC_NR m1_i2

#endif /* _MINIX_COM_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/devio.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This file provides basic types and some constants for the
* SYS_DEVIO and SYS_VDEVIO system calls, which allow user-level
* processes to perform device I/O.
*
* Created:
* Apr 08, 2004 by Jorrit N. Herder
*/

#ifndef _DEVIO_H
#define _DEVIO_H

#include <minix/sys_config.h> /* needed to include <minix/type.h> */
#include <sys/types.h> /* u8_t, u16_t, u32_t needed */

typedef u16_t port_t;
typedef U16_t Port_t;

/* We have different granularities of port I/O: 8, 16, 32 bits.
* Also see <ibm/portio.h>, which has functions for bytes, words,
* and longs. Hence, we need different (port,value)-pair types.
*/
typedef struct { u16_t port; u8_t value; } pvb_pair_t;
typedef struct { u16_t port; u16_t value; } pvw_pair_t;
typedef struct { u16_t port; u32_t value; } pvl_pair_t;

/* Macro shorthand to set (port,value)-pair. */
#define pv_set(pv, p, v) ((pv).port = (p), (pv).value = (v))
#define pv_ptr_set(pv_ptr, p, v) ((pv_ptr)->port = (p), (pv_ptr)->value = (v))

#endif /* _DEVIO_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/minix/dmap.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _DMAP_H
#define _DMAP_H

#include <minix/sys_config.h>
#include <minix/ipc.h>

686 File: include/minix/dmap.h MINIX SOURCE CODE
/*===========================================================================*
* Device <-> Driver Table *
*===========================================================================*/

/* Device table. This table is indexed by major device number. It provides
* the link between major device numbers and the routines that process them.
* The table can be update dynamically. The field ’dmap_flags’ describe an
* entry’s current status and determines what control options are possible.
*/
#define DMAP_MUTABLE 0x01 /* mapping can be overtaken */
#define DMAP_BUSY 0x02 /* driver busy with request */

enum dev_style { STYLE_DEV, STYLE_NDEV, STYLE_TTY, STYLE_CLONE };

extern struct dmap {
int _PROTOTYPE ((*dmap_opcl), (int, Dev_t, int, int) );
void _PROTOTYPE ((*dmap_io), (int, message *) );
int dmap_driver;
int dmap_flags;
} dmap[];

/*===========================================================================*
* Major and minor device numbers *
*===========================================================================*/

/* Total number of different devices. */
#define NR_DEVICES 32 /* number of (major) devices */

/* Major and minor device numbers for MEMORY driver. */
#define MEMORY_MAJOR 1 /* major device for memory devices */
# define RAM_DEV 0 /* minor device for /dev/ram */
# define MEM_DEV 1 /* minor device for /dev/mem */
# define KMEM_DEV 2 /* minor device for /dev/kmem */
# define NULL_DEV 3 /* minor device for /dev/null */
# define BOOT_DEV 4 /* minor device for /dev/boot */
# define ZERO_DEV 5 /* minor device for /dev/zero */

#define CTRLR(n) ((n)==0 ? 3 : (8 + 2*((n)-1))) /* magic formula */

/* Full device numbers that are special to the boot monitor and FS. */
# define DEV_RAM 0x0100 /* device number of /dev/ram */
# define DEV_BOOT 0x0104 /* device number of /dev/boot */

#define FLOPPY_MAJOR 2 /* major device for floppy disks */
#define TTY_MAJOR 4 /* major device for ttys */
#define CTTY_MAJOR 5 /* major device for /dev/tty */

#define INET_MAJOR 7 /* major device for inet */

#define LOG_MAJOR 15 /* major device for log driver */
# define IS_KLOG_DEV 0 /* minor device for /dev/klog */

#endif /* _DMAP_H */
MINIX SOURCE CODE File: include/ibm/portio.h 687
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/ibm/portio.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
ibm/portio.h

Created: Jan 15, 1992 by Philip Homburg
*/

#ifndef _PORTIO_H_
#define _PORTIO_H_

#ifndef _TYPES_H
#include <sys/types.h>
#endif

unsigned inb(U16_t _port);
unsigned inw(U16_t _port);
unsigned inl(U32_t _port);
void outb(U16_t _port, U8_t _value);
void outw(U16_t _port, U16_t _value);
void outl(U16_t _port, U32_t _value);
void insb(U16_t _port, void *_buf, size_t _count);
void insw(U16_t _port, void *_buf, size_t _count);
void insl(U16_t _port, void *_buf, size_t _count);
void outsb(U16_t _port, void *_buf, size_t _count);
void outsw(U16_t _port, void *_buf, size_t _count);
void outsl(U16_t _port, void *_buf, size_t _count);
void intr_disable(void);
void intr_enable(void);

#endif /* _PORTIO_H_ */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/ibm/interrupt.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* Interrupt numbers and hardware vectors. */

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#if (CHIP == INTEL)

/* 8259A interrupt controller ports. */
#define INT_CTL 0x20 /* I/O port for interrupt controller */
#define INT_CTLMASK 0x21 /* setting bits in this port disables ints */
#define INT2_CTL 0xA0 /* I/O port for second interrupt controller */
#define INT2_CTLMASK 0xA1 /* setting bits in this port disables ints */

/* Magic numbers for interrupt controller. */
#define END_OF_INT 0x20 /* code used to re-enable after an interrupt */

/* Interrupt vectors defined/reserved by processor. */
#define DIVIDE_VECTOR 0 /* divide error */
#define DEBUG_VECTOR 1 /* single step (trace) */
#define NMI_VECTOR 2 /* non-maskable interrupt */
688 File: include/ibm/interrupt.h MINIX SOURCE CODE
#define BREAKPOINT_VECTOR 3 /* software breakpoint */
#define OVERFLOW_VECTOR 4 /* from INTO */

/* Fixed system call vector. */
#define SYS_VECTOR 32 /* system calls are made with int SYSVEC */
#define SYS386_VECTOR 33 /* except 386 system calls use this */
#define LEVEL0_VECTOR 34 /* for execution of a function at level 0 */

/* Suitable irq bases for hardware interrupts. Reprogram the 8259(s) from
* the PC BIOS defaults since the BIOS doesn’t respect all the processor’s
* reserved vectors (0 to 31).
*/
#define BIOS_IRQ0_VEC 0x08 /* base of IRQ0-7 vectors used by BIOS */
#define BIOS_IRQ8_VEC 0x70 /* base of IRQ8-15 vectors used by BIOS */
#define IRQ0_VECTOR 0x50 /* nice vectors to relocate IRQ0-7 to */
#define IRQ8_VECTOR 0x70 /* no need to move IRQ8-15 */

/* Hardware interrupt numbers. */
#define NR_IRQ_VECTORS 16
#define CLOCK_IRQ 0
#define KEYBOARD_IRQ 1
#define CASCADE_IRQ 2 /* cascade enable for 2nd AT controller */
#define ETHER_IRQ 3 /* default ethernet interrupt vector */
#define SECONDARY_IRQ 3 /* RS232 interrupt vector for port 2 */
#define RS232_IRQ 4 /* RS232 interrupt vector for port 1 */
#define XT_WINI_IRQ 5 /* xt winchester */
#define FLOPPY_IRQ 6 /* floppy disk */
#define PRINTER_IRQ 7
#define AT_WINI_0_IRQ 14 /* at winchester controller 0 */
#define AT_WINI_1_IRQ 15 /* at winchester controller 1 */

/* Interrupt number to hardware vector. */
#define BIOS_VECTOR(irq) \
(((irq) < 8 ? BIOS_IRQ0_VEC : BIOS_IRQ8_VEC) + ((irq) & 0x07))
#define VECTOR(irq) \
(((irq) < 8 ? IRQ0_VECTOR : IRQ8_VECTOR) + ((irq) & 0x07))

#endif /* (CHIP == INTEL) */

#endif /* _INTERRUPT_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
include/ibm/ports.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* Addresses and magic numbers for miscellaneous ports. */

#ifndef _PORTS_H
#define _PORTS_H

#if (CHIP == INTEL)

/* Miscellaneous ports. */
#define PCR 0x65 /* Planar Control Register */
#define PORT_B 0x61 /* I/O port for 8255 port B (kbd, beeper...) */
#define TIMER0 0x40 /* I/O port for timer channel 0 */
#define TIMER2 0x42 /* I/O port for timer channel 2 */
#define TIMER_MODE 0x43 /* I/O port for timer mode control */

#endif /* (CHIP == INTEL) */
MINIX SOURCE CODE File: include/ibm/ports.h 689

#endif /* _PORTS_H */