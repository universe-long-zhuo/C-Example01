++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             drivers/tty/tty.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
13400 /* tty.h - Terminals */
13401
13402 #include <timers.h>
13403
13404 /* First minor numbers for the various classes of TTY devices. */
13405 #define CONS_MINOR 0
13406 #define LOG_MINOR 15
13407 #define RS232_MINOR 16
13408 #define TTYPX_MINOR 128
13409 #define PTYPX_MINOR 192
13410
13411 #define LINEWRAP 1 /* console.c - wrap lines at column 80 */
13412
13413 #define TTY_IN_BYTES 256 /* tty input queue size */
13414 #define TAB_SIZE 8 /* distance between tab stops */
13415 #define TAB_MASK 7 /* mask to compute a tab stop position */
13416
13417 #define ESC ’\33’ /* escape */
13418
13419 #define O_NOCTTY 00400 /* from <fcntl.h>, or cc will choke */
MINIX SOURCE CODE File: drivers/tty/tty.h 809
13420 #define O_NONBLOCK 04000
13421
13422 struct tty;
13423 typedef _PROTOTYPE( int (*devfun_t), (struct tty *tp, int try_only) );
13424 typedef _PROTOTYPE( void (*devfunarg_t), (struct tty *tp, int c) );
13425
13426 typedef struct tty {
13427 int tty_events; /* set when TTY should inspect this line */
13428 int tty_index; /* index into TTY table */
13429 int tty_minor; /* device minor number */
13430
13431 /* Input queue. Typed characters are stored here until read by a program. */
13432 u16_t *tty_inhead; /* pointer to place where next char goes */
13433 u16_t *tty_intail; /* pointer to next char to be given to prog */
13434 int tty_incount; /* # chars in the input queue */
13435 int tty_eotct; /* number of "line breaks" in input queue */
13436 devfun_t tty_devread; /* routine to read from low level buffers */
13437 devfun_t tty_icancel; /* cancel any device input */
13438 int tty_min; /* minimum requested #chars in input queue */
13439 timer_t tty_tmr; /* the timer for this tty */
13440
13441 /* Output section. */
13442 devfun_t tty_devwrite; /* routine to start actual device output */
13443 devfunarg_t tty_echo; /* routine to echo characters input */
13444 devfun_t tty_ocancel; /* cancel any ongoing device output */
13445 devfun_t tty_break; /* let the device send a break */
13446
13447 /* Terminal parameters and status. */
13448 int tty_position; /* current position on the screen for echoing */
13449 char tty_reprint; /* 1 when echoed input messed up, else 0 */
13450 char tty_escaped; /* 1 when LNEXT (ˆV) just seen, else 0 */
13451 char tty_inhibited; /* 1 when STOP (ˆS) just seen (stops output) */
13452 char tty_pgrp; /* slot number of controlling process */
13453 char tty_openct; /* count of number of opens of this tty */
13454
13455 /* Information about incomplete I/O requests is stored here. */
13456 char tty_inrepcode; /* reply code, TASK_REPLY or REVIVE */
13457 char tty_inrevived; /* set to 1 if revive callback is pending */
13458 char tty_incaller; /* process that made the call (usually FS) */
13459 char tty_inproc; /* process that wants to read from tty */
13460 vir_bytes tty_in_vir; /* virtual address where data is to go */
13461 int tty_inleft; /* how many chars are still needed */
13462 int tty_incum; /* # chars input so far */
13463 char tty_outrepcode; /* reply code, TASK_REPLY or REVIVE */
13464 char tty_outrevived; /* set to 1 if revive callback is pending */
13465 char tty_outcaller; /* process that made the call (usually FS) */
13466 char tty_outproc; /* process that wants to write to tty */
13467 vir_bytes tty_out_vir; /* virtual address where data comes from */
13468 int tty_outleft; /* # chars yet to be output */
13469 int tty_outcum; /* # chars output so far */
13470 char tty_iocaller; /* process that made the call (usually FS) */
13471 char tty_ioproc; /* process that wants to do an ioctl */
13472 int tty_ioreq; /* ioctl request code */
13473 vir_bytes tty_iovir; /* virtual address of ioctl buffer */
13474
13475 /* select() data */
13476 int tty_select_ops; /* which operations are interesting */
13477 int tty_select_proc; /* which process wants notification */
13478
13479 /* Miscellaneous. */
810 File: drivers/tty/tty.h MINIX SOURCE CODE
13480 devfun_t tty_ioctl; /* set line speed, etc. at the device level */
13481 devfun_t tty_close; /* tell the device that the tty is closed */
13482 void *tty_priv; /* pointer to per device private data */
13483 struct termios tty_termios; /* terminal attributes */
13484 struct winsize tty_winsize; /* window size (#lines and #columns) */
13485
13486 u16_t tty_inbuf[TTY_IN_BYTES];/* tty input buffer */
13487
13488 } tty_t;
13489
13490 /* Memory allocated in tty.c, so extern here. */
13491 extern tty_t tty_table[NR_CONS+NR_RS_LINES+NR_PTYS];
13492 extern int ccurrent; /* currently visible console */
13493 extern int irq_hook_id; /* hook id for keyboard irq */
13494
13495 extern unsigned long kbd_irq_set;
13496 extern unsigned long rs_irq_set;
13497
13498 /* Values for the fields. */
13499 #define NOT_ESCAPED 0 /* previous character is not LNEXT (ˆV) */
13500 #define ESCAPED 1 /* previous character was LNEXT (ˆV) */
13501 #define RUNNING 0 /* no STOP (ˆS) has been typed to stop output */
13502 #define STOPPED 1 /* STOP (ˆS) has been typed to stop output */
13503
13504 /* Fields and flags on characters in the input queue. */
13505 #define IN_CHAR 0x00FF /* low 8 bits are the character itself */
13506 #define IN_LEN 0x0F00 /* length of char if it has been echoed */
13507 #define IN_LSHIFT 8 /* length = (c & IN_LEN) >> IN_LSHIFT */
13508 #define IN_EOT 0x1000 /* char is a line break (ˆD, LF) */
13509 #define IN_EOF 0x2000 /* char is EOF (ˆD), do not return to user */
13510 #define IN_ESC 0x4000 /* escaped by LNEXT (ˆV), no interpretation */
13511
13512 /* Times and timeouts. */
13513 #define force_timeout() ((void) (0))
13514
13515 /* Memory allocated in tty.c, so extern here. */
13516 extern timer_t *tty_timers; /* queue of TTY timers */
13517 extern clock_t tty_next_timeout; /* next TTY timeout */
13518
13519 /* Number of elements and limit of a buffer. */
13520 #define buflen(buf) (sizeof(buf) / sizeof((buf)[0]))
13521 #define bufend(buf) ((buf) + buflen(buf))
13522
13523 /* Memory allocated in tty.c, so extern here. */
13524 extern struct machine machine; /* machine information (a.o.: pc_at, ega) */
13525
13526 /* Function prototypes for TTY driver. */
13527 /* tty.c */
13528 _PROTOTYPE( void handle_events, (struct tty *tp) );
13529 _PROTOTYPE( void sigchar, (struct tty *tp, int sig) );
13530 _PROTOTYPE( void tty_task, (void) );
13531 _PROTOTYPE( int in_process, (struct tty *tp, char *buf, int count) );
13532 _PROTOTYPE( void out_process, (struct tty *tp, char *bstart, char *bpos,
13533 char *bend, int *icount, int *ocount) );
13534 _PROTOTYPE( void tty_wakeup, (clock_t now) );
13535 _PROTOTYPE( void tty_reply, (int code, int replyee, int proc_nr,
13536 int status) );
13537 _PROTOTYPE( int tty_devnop, (struct tty *tp, int try) );
13538 _PROTOTYPE( int select_try, (struct tty *tp, int ops) );
13539 _PROTOTYPE( int select_retry, (struct tty *tp) );
MINIX SOURCE CODE File: drivers/tty/tty.h 811
13540
13541 /* console.c */
13542 _PROTOTYPE( void kputc, (int c) );
13543 _PROTOTYPE( void cons_stop, (void) );
13544 _PROTOTYPE( void do_new_kmess, (message *m) );
13545 _PROTOTYPE( void do_diagnostics, (message *m) );
13546 _PROTOTYPE( void scr_init, (struct tty *tp) );
13547 _PROTOTYPE( void toggle_scroll, (void) );
13548 _PROTOTYPE( int con_loadfont, (message *m) );
13549 _PROTOTYPE( void select_console, (int cons_line) );
13550
13551 /* keyboard.c */
13552 _PROTOTYPE( void kb_init, (struct tty *tp) );
13553 _PROTOTYPE( void kb_init_once, (void) );
13554 _PROTOTYPE( int kbd_loadmap, (message *m) );
13555 _PROTOTYPE( void do_panic_dumps, (message *m) );
13556 _PROTOTYPE( void do_fkey_ctl, (message *m) );
13557 _PROTOTYPE( void kbd_interrupt, (message *m) );
13558
13559 /* vidcopy.s */
13560 _PROTOTYPE( void vid_vid_copy, (unsigned src, unsigned dst, unsigned count));
13561 _PROTOTYPE( void mem_vid_copy, (u16_t *src, unsigned dst, unsigned count));