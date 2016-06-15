++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             drivers/tty/tty.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
13600 /* This file contains the terminal driver, both for the IBM console and regular
13601 * ASCII terminals. It handles only the device-independent part of a TTY, the
13602 * device dependent parts are in console.c, rs232.c, etc. This file contains
13603 * two main entry points, tty_task() and tty_wakeup(), and several minor entry
13604 * points for use by the device-dependent code.
13605 *
13606 * The device-independent part accepts "keyboard" input from the device-
13607 * dependent part, performs input processing (special key interpretation),
13608 * and sends the input to a process reading from the TTY. Output to a TTY
13609 * is sent to the device-dependent code for output processing and "screen"
13610 * display. Input processing is done by the device by calling ’in_process’
13611 * on the input characters, output processing may be done by the device itself
13612 * or by calling ’out_process’. The TTY takes care of input queuing, the
13613 * device does the output queuing. If a device receives an external signal,
13614 * like an interrupt, then it causes tty_wakeup() to be run by the CLOCK task
13615 * to, you guessed it, wake up the TTY to check if input or output can
13616 * continue.
13617 *
13618 * The valid messages and their parameters are:
13619 *
13620 * HARD_INT: output has been completed or input has arrived
13621 * SYS_SIG: e.g., MINIX wants to shutdown; run code to cleanly stop
13622 * DEV_READ: a process wants to read from a terminal
13623 * DEV_WRITE: a process wants to write on a terminal
13624 * DEV_IOCTL: a process wants to change a terminal’s parameters
13625 * DEV_OPEN: a tty line has been opened
13626 * DEV_CLOSE: a tty line has been closed
13627 * DEV_SELECT: start select notification request
13628 * DEV_STATUS: FS wants to know status for SELECT or REVIVE
13629 * CANCEL: terminate a previous incomplete system call immediately
812 File: drivers/tty/tty.c MINIX SOURCE CODE
13630 *
13631 * m_type TTY_LINE PROC_NR COUNT TTY_SPEK TTY_FLAGS ADDRESS
13632 * ---------------------------------------------------------------------------
13633 * | HARD_INT | | | | | | |
13634 * |-------------+---------+---------+---------+---------+---------+---------|
13635 * | SYS_SIG | sig set| | | | | |
13636 * |-------------+---------+---------+---------+---------+---------+---------|
13637 * | DEV_READ |minor dev| proc nr | count | O_NONBLOCK| buf ptr |
13638 * |-------------+---------+---------+---------+---------+---------+---------|
13639 * | DEV_WRITE |minor dev| proc nr | count | | | buf ptr |
13640 * |-------------+---------+---------+---------+---------+---------+---------|
13641 * | DEV_IOCTL |minor dev| proc nr |func code|erase etc| flags | |
13642 * |-------------+---------+---------+---------+---------+---------+---------|
13643 * | DEV_OPEN |minor dev| proc nr | O_NOCTTY| | | |
13644 * |-------------+---------+---------+---------+---------+---------+---------|
13645 * | DEV_CLOSE |minor dev| proc nr| | | | |
13646 * |-------------+---------+---------+---------+---------+---------+---------|
13647 * | DEV_STATUS| | | | | | |
13648 * |-------------+---------+---------+---------+---------+---------+---------|
13649 * | CANCEL |minor dev| proc nr| | | | |
13650 * ---------------------------------------------------------------------------
13651 *
13652 * Changes:
13653 * Jan 20, 2004 moved TTY driver to user-space (Jorrit N. Herder)
13654 * Sep 20, 2004 local timer management/ sync alarms (Jorrit N. Herder)
13655 * Jul 13, 2004 support for function key observers (Jorrit N. Herder)
13656 */
13657
13658 #include "../drivers.h"
13659 #include "../drivers.h"
13660 #include <termios.h>
13661 #include <sys/ioc_tty.h>
13662 #include <signal.h>
13663 #include <minix/callnr.h>
13664 #include <minix/keymap.h>
13665 #include "tty.h"
13666
13667 #include <sys/time.h>
13668 #include <sys/select.h>
13669
13670 extern int irq_hook_id;
13671
13672 unsigned long kbd_irq_set = 0;
13673 unsigned long rs_irq_set = 0;
13674
13675 /* Address of a tty structure. */
13676 #define tty_addr(line) (&tty_table[line])
13677
13678 /* Macros for magic tty types. */
13679 #define isconsole(tp) ((tp) < tty_addr(NR_CONS))
13680 #define ispty(tp) ((tp) >= tty_addr(NR_CONS+NR_RS_LINES))
13681
13682 /* Macros for magic tty structure pointers. */
13683 #define FIRST_TTY tty_addr(0)
13684 #define END_TTY tty_addr(sizeof(tty_table) / sizeof(tty_table[0]))
13685
13686 /* A device exists if at least its ’devread’ function is defined. */
13687 #define tty_active(tp) ((tp)->tty_devread != NULL)
13688
13689 /* RS232 lines or pseudo terminals can be completely configured out. */
MINIX SOURCE CODE File: drivers/tty/tty.c 813
13690 #if NR_RS_LINES == 0
13691 #define rs_init(tp) ((void) 0)
13692 #endif
13693 #if NR_PTYS == 0
13694 #define pty_init(tp) ((void) 0)
13695 #define do_pty(tp, mp) ((void) 0)
13696 #endif
13697
13698 FORWARD _PROTOTYPE( void tty_timed_out, (timer_t *tp) );
13699 FORWARD _PROTOTYPE( void expire_timers, (void) );
13700 FORWARD _PROTOTYPE( void settimer, (tty_t *tty_ptr, int enable) );
13701 FORWARD _PROTOTYPE( void do_cancel, (tty_t *tp, message *m_ptr) );
13702 FORWARD _PROTOTYPE( void do_ioctl, (tty_t *tp, message *m_ptr) );
13703 FORWARD _PROTOTYPE( void do_open, (tty_t *tp, message *m_ptr) );
13704 FORWARD _PROTOTYPE( void do_close, (tty_t *tp, message *m_ptr) );
13705 FORWARD _PROTOTYPE( void do_read, (tty_t *tp, message *m_ptr) );
13706 FORWARD _PROTOTYPE( void do_write, (tty_t *tp, message *m_ptr) );
13707 FORWARD _PROTOTYPE( void do_select, (tty_t *tp, message *m_ptr) );
13708 FORWARD _PROTOTYPE( void do_status, (message *m_ptr) );
13709 FORWARD _PROTOTYPE( void in_transfer, (tty_t *tp) );
13710 FORWARD _PROTOTYPE( int tty_echo, (tty_t *tp, int ch) );
13711 FORWARD _PROTOTYPE( void rawecho, (tty_t *tp, int ch) );
13712 FORWARD _PROTOTYPE( int back_over, (tty_t *tp) );
13713 FORWARD _PROTOTYPE( void reprint, (tty_t *tp) );
13714 FORWARD _PROTOTYPE( void dev_ioctl, (tty_t *tp) );
13715 FORWARD _PROTOTYPE( void setattr, (tty_t *tp) );
13716 FORWARD _PROTOTYPE( void tty_icancel, (tty_t *tp) );
13717 FORWARD _PROTOTYPE( void tty_init, (void) );
13718
13719 /* Default attributes. */
13720 PRIVATE struct termios termios_defaults = {
13721 TINPUT_DEF, TOUTPUT_DEF, TCTRL_DEF, TLOCAL_DEF, TSPEED_DEF, TSPEED_DEF,
13722 {
13723 TEOF_DEF, TEOL_DEF, TERASE_DEF, TINTR_DEF, TKILL_DEF, TMIN_DEF,
13724 TQUIT_DEF, TTIME_DEF, TSUSP_DEF, TSTART_DEF, TSTOP_DEF,
13725 TREPRINT_DEF, TLNEXT_DEF, TDISCARD_DEF,
13726 },
13727 };
13728 PRIVATE struct winsize winsize_defaults; /* = all zeroes */
13729
13730 /* Global variables for the TTY task (declared extern in tty.h). */
13731 PUBLIC tty_t tty_table[NR_CONS+NR_RS_LINES+NR_PTYS];
13732 PUBLIC int ccurrent; /* currently active console */
13733 PUBLIC timer_t *tty_timers; /* queue of TTY timers */
13734 PUBLIC clock_t tty_next_timeout; /* time that the next alarm is due */
13735 PUBLIC struct machine machine; /* kernel environment variables */
13736
13737 /*===========================================================================*
13738 * tty_task *
13739 *===========================================================================*/
13740 PUBLIC void main(void)
13741 {
13742 /* Main routine of the terminal task. */
13743
13744 message tty_mess; /* buffer for all incoming messages */
13745 unsigned line;
13746 int s;
13747 char *types[] = {"task","driver","server", "user"};
13748 register struct proc *rp;
13749 register tty_t *tp;
814 File: drivers/tty/tty.c MINIX SOURCE CODE
13750
13751 /* Initialize the TTY driver. */
13752 tty_init();
13753
13754 /* Get kernel environment (protected_mode, pc_at and ega are needed). */
13755 if (OK != (s=sys_getmachine(&machine))) {
13756 panic("TTY","Couldn’t obtain kernel environment.", s);
13757 }
13758
13759 /* Final one-time keyboard initialization. */
13760 kb_init_once();
13761
13762 printf("\n");
13763
13764 while (TRUE) {
13765
13766 /* Check for and handle any events on any of the ttys. */
13767 for (tp = FIRST_TTY; tp < END_TTY; tp++) {
13768 if (tp->tty_events) handle_events(tp);
13769 }
13770
13771 /* Get a request message. */
13772 receive(ANY, &tty_mess);
13773
13774 /* First handle all kernel notification types that the TTY supports.
13775 * - An alarm went off, expire all timers and handle the events.
13776 * - A hardware interrupt also is an invitation to check for events.
13777 * - A new kernel message is available for printing.
13778 * - Reset the console on system shutdown.
13779 * Then see if this message is different from a normal device driver
13780 * request and should be handled separately. These extra functions
13781 * do not operate on a device, in constrast to the driver requests.
13782 */
13783 switch (tty_mess.m_type) {
13784 case SYN_ALARM: /* fall through */
13785 expire_timers(); /* run watchdogs of expired timers */
13786 continue; /* contine to check for events */
13787 case HARD_INT: { /* hardware interrupt notification */
13788 if (tty_mess.NOTIFY_ARG & kbd_irq_set)
13789 kbd_interrupt(&tty_mess);/* fetch chars from keyboard */
13790 #if NR_RS_LINES > 0
13791 if (tty_mess.NOTIFY_ARG & rs_irq_set)
13792 rs_interrupt(&tty_mess);/* serial I/O */
13793 #endif
13794 expire_timers(); /* run watchdogs of expired timers */
13795 continue; /* contine to check for events */
13796 }
13797 case SYS_SIG: { /* system signal */
13798 sigset_t sigset = (sigset_t) tty_mess.NOTIFY_ARG;
13799
13800 if (sigismember(&sigset, SIGKSTOP)) {
13801 cons_stop(); /* switch to primary console */
13802 if (irq_hook_id != -1) {
13803 sys_irqdisable(&irq_hook_id);
13804 sys_irqrmpolicy(KEYBOARD_IRQ, &irq_hook_id);
13805 }
13806 }
13807 if (sigismember(&sigset, SIGTERM)) cons_stop();
13808 if (sigismember(&sigset, SIGKMESS)) do_new_kmess(&tty_mess);
13809 continue;
MINIX SOURCE CODE File: drivers/tty/tty.c 815
13810 }
13811 case PANIC_DUMPS: /* allow panic dumps */
13812 cons_stop(); /* switch to primary console */
13813 do_panic_dumps(&tty_mess);
13814 continue;
13815 case DIAGNOSTICS: /* a server wants to print some */
13816 do_diagnostics(&tty_mess);
13817 continue;
13818 case FKEY_CONTROL: /* (un)register a fkey observer */
13819 do_fkey_ctl(&tty_mess);
13820 continue;
13821 default: /* should be a driver request */
13822 ; /* do nothing; end switch */
13823 }
13824
13825 /* Only device requests should get to this point. All requests,
13826 * except DEV_STATUS, have a minor device number. Check this
13827 * exception and get the minor device number otherwise.
13828 */
13829 if (tty_mess.m_type == DEV_STATUS) {
13830 do_status(&tty_mess);
13831 continue;
13832 }
13833 line = tty_mess.TTY_LINE;
13834 if ((line - CONS_MINOR) < NR_CONS) {
13835 tp = tty_addr(line - CONS_MINOR);
13836 } else if (line == LOG_MINOR) {
13837 tp = tty_addr(0);
13838 } else if ((line - RS232_MINOR) < NR_RS_LINES) {
13839 tp = tty_addr(line - RS232_MINOR + NR_CONS);
13840 } else if ((line - TTYPX_MINOR) < NR_PTYS) {
13841 tp = tty_addr(line - TTYPX_MINOR + NR_CONS + NR_RS_LINES);
13842 } else if ((line - PTYPX_MINOR) < NR_PTYS) {
13843 tp = tty_addr(line - PTYPX_MINOR + NR_CONS + NR_RS_LINES);
13844 if (tty_mess.m_type != DEV_IOCTL) {
13845 do_pty(tp, &tty_mess);
13846 continue;
13847 }
13848 } else {
13849 tp = NULL;
13850 }
13851
13852 /* If the device doesn’t exist or is not configured return ENXIO. */
13853 if (tp == NULL || ! tty_active(tp)) {
13854 printf("Warning, TTY got illegal request %d from %d\n",
13855 tty_mess.m_type, tty_mess.m_source);
13856 tty_reply(TASK_REPLY, tty_mess.m_source,
13857 tty_mess.PROC_NR, ENXIO);
13858 continue;
13859 }
13860
13861 /* Execute the requested device driver function. */
13862 switch (tty_mess.m_type) {
13863 case DEV_READ: do_read(tp, &tty_mess); break;
13864 case DEV_WRITE: do_write(tp, &tty_mess); break;
13865 case DEV_IOCTL: do_ioctl(tp, &tty_mess); break;
13866 case DEV_OPEN: do_open(tp, &tty_mess); break;
13867 case DEV_CLOSE: do_close(tp, &tty_mess); break;
13868 case DEV_SELECT: do_select(tp, &tty_mess); break;
13869 case CANCEL: do_cancel(tp, &tty_mess); break;
816 File: drivers/tty/tty.c MINIX SOURCE CODE
13870 default:
13871 printf("Warning, TTY got unexpected request %d from %d\n",
13872 tty_mess.m_type, tty_mess.m_source);
13873 tty_reply(TASK_REPLY, tty_mess.m_source,
13874 tty_mess.PROC_NR, EINVAL);
13875 }
13876 }
13877 }
13879 /*===========================================================================*
13880 * do_status *
13881 *===========================================================================*/
13882 PRIVATE void do_status(m_ptr)
13883 message *m_ptr;
13884 {
13885 register struct tty *tp;
13886 int event_found;
13887 int status;
13888 int ops;
13889
13890 /* Check for select or revive events on any of the ttys. If we found an,
13891 * event return a single status message for it. The FS will make another
13892 * call to see if there is more.
13893 */
13894 event_found = 0;
13895 for (tp = FIRST_TTY; tp < END_TTY; tp++) {
13896 if ((ops = select_try(tp, tp->tty_select_ops)) &&
13897 tp->tty_select_proc == m_ptr->m_source) {
13898
13899 /* I/O for a selected minor device is ready. */
13900 m_ptr->m_type = DEV_IO_READY;
13901 m_ptr->DEV_MINOR = tp->tty_index;
13902 m_ptr->DEV_SEL_OPS = ops;
13903
13904 tp->tty_select_ops &= ˜ops; /* unmark select event */
13905 event_found = 1;
13906 break;
13907 }
13908 else if (tp->tty_inrevived && tp->tty_incaller == m_ptr->m_source) {
13909
13910 /* Suspended request finished. Send a REVIVE. */
13911 m_ptr->m_type = DEV_REVIVE;
13912 m_ptr->REP_PROC_NR = tp->tty_inproc;
13913 m_ptr->REP_STATUS = tp->tty_incum;
13914
13915 tp->tty_inleft = tp->tty_incum = 0;
13916 tp->tty_inrevived = 0; /* unmark revive event */
13917 event_found = 1;
13918 break;
13919 }
13920 else if (tp->tty_outrevived && tp->tty_outcaller == m_ptr->m_source) {
13921
13922 /* Suspended request finished. Send a REVIVE. */
13923 m_ptr->m_type = DEV_REVIVE;
13924 m_ptr->REP_PROC_NR = tp->tty_outproc;
13925 m_ptr->REP_STATUS = tp->tty_outcum;
13926
13927 tp->tty_outcum = 0;
13928 tp->tty_outrevived = 0; /* unmark revive event */
13929 event_found = 1;
MINIX SOURCE CODE File: drivers/tty/tty.c 817
13930 break;
13931 }
13932 }
13933
13934 #if NR_PTYS > 0
13935 if (!event_found)
13936 event_found = pty_status(m_ptr);
13937 #endif
13938
13939 if (! event_found) {
13940 /* No events of interest were found. Return an empty message. */
13941 m_ptr->m_type = DEV_NO_STATUS;
13942 }
13943
13944 /* Almost done. Send back the reply message to the caller. */
13945 if ((status = send(m_ptr->m_source, m_ptr)) != OK) {
13946 panic("TTY","send in do_status failed, status\n", status);
13947 }
13948 }
13950 /*===========================================================================*
13951 * do_read *
13952 *===========================================================================*/
13953 PRIVATE void do_read(tp, m_ptr)
13954 register tty_t *tp; /* pointer to tty struct */
13955 register message *m_ptr; /* pointer to message sent to the task */
13956 {
13957 /* A process wants to read from a terminal. */
13958 int r, status;
13959 phys_bytes phys_addr;
13960
13961 /* Check if there is already a process hanging in a read, check if the
13962 * parameters are correct, do I/O.
13963 */
13964 if (tp->tty_inleft > 0) {
13965 r = EIO;
13966 } else
13967 if (m_ptr->COUNT <= 0) {
13968 r = EINVAL;
13969 } else
13970 if (sys_umap(m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS, m_ptr->COUNT,
13971 &phys_addr) != OK) {
13972 r = EFAULT;
13973 } else {
13974 /* Copy information from the message to the tty struct. */
13975 tp->tty_inrepcode = TASK_REPLY;
13976 tp->tty_incaller = m_ptr->m_source;
13977 tp->tty_inproc = m_ptr->PROC_NR;
13978 tp->tty_in_vir = (vir_bytes) m_ptr->ADDRESS;
13979 tp->tty_inleft = m_ptr->COUNT;
13980
13981 if (!(tp->tty_termios.c_lflag & ICANON)
13982 && tp->tty_termios.c_cc[VTIME] > 0) {
13983 if (tp->tty_termios.c_cc[VMIN] == 0) {
13984 /* MIN & TIME specify a read timer that finishes the
13985 * read in TIME/10 seconds if no bytes are available.
13986 */
13987 settimer(tp, TRUE);
13988 tp->tty_min = 1;
13989 } else {
818 File: drivers/tty/tty.c MINIX SOURCE CODE
13990 /* MIN & TIME specify an inter-byte timer that may
13991 * have to be cancelled if there are no bytes yet.
13992 */
13993 if (tp->tty_eotct == 0) {
13994 settimer(tp, FALSE);
13995 tp->tty_min = tp->tty_termios.c_cc[VMIN];
13996 }
13997 }
13998 }
13999
14000 /* Anything waiting in the input buffer? Clear it out... */
14001 in_transfer(tp);
14002 /* ...then go back for more. */
14003 handle_events(tp);
14004 if (tp->tty_inleft == 0) {
14005 if (tp->tty_select_ops)
14006 select_retry(tp);
14007 return; /* already done */
14008 }
14009
14010 /* There were no bytes in the input queue available, so either suspend
14011 * the caller or break off the read if nonblocking.
14012 */
14013 if (m_ptr->TTY_FLAGS & O_NONBLOCK) {
14014 r = EAGAIN; /* cancel the read */
14015 tp->tty_inleft = tp->tty_incum = 0;
14016 } else {
14017 r = SUSPEND; /* suspend the caller */
14018 tp->tty_inrepcode = REVIVE;
14019 }
14020 }
14021 tty_reply(TASK_REPLY, m_ptr->m_source, m_ptr->PROC_NR, r);
14022 if (tp->tty_select_ops)
14023 select_retry(tp);
14024 }
14026 /*===========================================================================*
14027 * do_write *
14028 *===========================================================================*/
14029 PRIVATE void do_write(tp, m_ptr)
14030 register tty_t *tp;
14031 register message *m_ptr; /* pointer to message sent to the task */
14032 {
14033 /* A process wants to write on a terminal. */
14034 int r;
14035 phys_bytes phys_addr;
14036
14037 /* Check if there is already a process hanging in a write, check if the
14038 * parameters are correct, do I/O.
14039 */
14040 if (tp->tty_outleft > 0) {
14041 r = EIO;
14042 } else
14043 if (m_ptr->COUNT <= 0) {
14044 r = EINVAL;
14045 } else
14046 if (sys_umap(m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS, m_ptr->COUNT,
14047 &phys_addr) != OK) {
14048 r = EFAULT;
14049 } else {
MINIX SOURCE CODE File: drivers/tty/tty.c 819
14050 /* Copy message parameters to the tty structure. */
14051 tp->tty_outrepcode = TASK_REPLY;
14052 tp->tty_outcaller = m_ptr->m_source;
14053 tp->tty_outproc = m_ptr->PROC_NR;
14054 tp->tty_out_vir = (vir_bytes) m_ptr->ADDRESS;
14055 tp->tty_outleft = m_ptr->COUNT;
14056
14057 /* Try to write. */
14058 handle_events(tp);
14059 if (tp->tty_outleft == 0)
14060 return; /* already done */
14061
14062 /* None or not all the bytes could be written, so either suspend the
14063 * caller or break off the write if nonblocking.
14064 */
14065 if (m_ptr->TTY_FLAGS & O_NONBLOCK) { /* cancel the write */
14066 r = tp->tty_outcum > 0 ? tp->tty_outcum : EAGAIN;
14067 tp->tty_outleft = tp->tty_outcum = 0;
14068 } else {
14069 r = SUSPEND; /* suspend the caller */
14070 tp->tty_outrepcode = REVIVE;
14071 }
14072 }
14073 tty_reply(TASK_REPLY, m_ptr->m_source, m_ptr->PROC_NR, r);
14074 }
14076 /*===========================================================================*
14077 * do_ioctl *
14078 *===========================================================================*/
14079 PRIVATE void do_ioctl(tp, m_ptr)
14080 register tty_t *tp;
14081 message *m_ptr; /* pointer to message sent to task */
14082 {
14083 /* Perform an IOCTL on this terminal. Posix termios calls are handled
14084 * by the IOCTL system call
14085 */
14086
14087 int r;
14088 union {
14089 int i;
14090 } param;
14091 size_t size;
14092
14093 /* Size of the ioctl parameter. */
14094 switch (m_ptr->TTY_REQUEST) {
14095 case TCGETS: /* Posix tcgetattr function */
14096 case TCSETS: /* Posix tcsetattr function, TCSANOW option */
14097 case TCSETSW: /* Posix tcsetattr function, TCSADRAIN option */
14098 case TCSETSF: /* Posix tcsetattr function, TCSAFLUSH option */
14099 size = sizeof(struct termios);
14100 break;
14101
14102 case TCSBRK: /* Posix tcsendbreak function */
14103 case TCFLOW: /* Posix tcflow function */
14104 case TCFLSH: /* Posix tcflush function */
14105 case TIOCGPGRP: /* Posix tcgetpgrp function */
14106 case TIOCSPGRP: /* Posix tcsetpgrp function */
14107 size = sizeof(int);
14108 break;
14109
820 File: drivers/tty/tty.c MINIX SOURCE CODE
14110 case TIOCGWINSZ: /* get window size (not Posix) */
14111 case TIOCSWINSZ: /* set window size (not Posix) */
14112 size = sizeof(struct winsize);
14113 break;
14114
14115 case KIOCSMAP: /* load keymap (Minix extension) */
14116 size = sizeof(keymap_t);
14117 break;
14118
14119 case TIOCSFON: /* load font (Minix extension) */
14120 size = sizeof(u8_t [8192]);
14121 break;
14122
14123 case TCDRAIN: /* Posix tcdrain function -- no parameter */
14124 default: size = 0;
14125 }
14126
14127 r = OK;
14128 switch (m_ptr->TTY_REQUEST) {
14129 case TCGETS:
14130 /* Get the termios attributes. */
14131 r = sys_vircopy(SELF, D, (vir_bytes) &tp->tty_termios,
14132 m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS,
14133 (vir_bytes) size);
14134 break;
14135
14136 case TCSETSW:
14137 case TCSETSF:
14138 case TCDRAIN:
14139 if (tp->tty_outleft > 0) {
14140 /* Wait for all ongoing output processing to finish. */
14141 tp->tty_iocaller = m_ptr->m_source;
14142 tp->tty_ioproc = m_ptr->PROC_NR;
14143 tp->tty_ioreq = m_ptr->REQUEST;
14144 tp->tty_iovir = (vir_bytes) m_ptr->ADDRESS;
14145 r = SUSPEND;
14146 break;
14147 }
14148 if (m_ptr->TTY_REQUEST == TCDRAIN) break;
14149 if (m_ptr->TTY_REQUEST == TCSETSF) tty_icancel(tp);
14150 /*FALL THROUGH*/
14151 case TCSETS:
14152 /* Set the termios attributes. */
14153 r = sys_vircopy( m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS,
14154 SELF, D, (vir_bytes) &tp->tty_termios, (vir_bytes) size);
14155 if (r != OK) break;
14156 setattr(tp);
14157 break;
14158
14159 case TCFLSH:
14160 r = sys_vircopy( m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS,
14161 SELF, D, (vir_bytes) &param.i, (vir_bytes) size);
14162 if (r != OK) break;
14163 switch (param.i) {
14164 case TCIFLUSH: tty_icancel(tp); break;
14165 case TCOFLUSH: (*tp->tty_ocancel)(tp, 0); break;
14166 case TCIOFLUSH: tty_icancel(tp); (*tp->tty_ocancel)(tp, 0); break;
14167 default: r = EINVAL;
14168 }
14169 break;
MINIX SOURCE CODE File: drivers/tty/tty.c 821
14170
14171 case TCFLOW:
14172 r = sys_vircopy( m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS,
14173 SELF, D, (vir_bytes) &param.i, (vir_bytes) size);
14174 if (r != OK) break;
14175 switch (param.i) {
14176 case TCOOFF:
14177 case TCOON:
14178 tp->tty_inhibited = (param.i == TCOOFF);
14179 tp->tty_events = 1;
14180 break;
14181 case TCIOFF:
14182 (*tp->tty_echo)(tp, tp->tty_termios.c_cc[VSTOP]);
14183 break;
14184 case TCION:
14185 (*tp->tty_echo)(tp, tp->tty_termios.c_cc[VSTART]);
14186 break;
14187 default:
14188 r = EINVAL;
14189 }
14190 break;
14191
14192 case TCSBRK:
14193 if (tp->tty_break != NULL) (*tp->tty_break)(tp,0);
14194 break;
14195
14196 case TIOCGWINSZ:
14197 r = sys_vircopy(SELF, D, (vir_bytes) &tp->tty_winsize,
14198 m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS,
14199 (vir_bytes) size);
14200 break;
14201
14202 case TIOCSWINSZ:
14203 r = sys_vircopy( m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS,
14204 SELF, D, (vir_bytes) &tp->tty_winsize, (vir_bytes) size);
14205 /* SIGWINCH... */
14206 break;
14207
14208 case KIOCSMAP:
14209 /* Load a new keymap (only /dev/console). */
14210 if (isconsole(tp)) r = kbd_loadmap(m_ptr);
14211 break;
14212
14213 case TIOCSFON:
14214 /* Load a font into an EGA or VGA card (hs@hck.hr) */
14215 if (isconsole(tp)) r = con_loadfont(m_ptr);
14216 break;
14217
14218 /* These Posix functions are allowed to fail if _POSIX_JOB_CONTROL is
14219 * not defined.
14220 */
14221 case TIOCGPGRP:
14222 case TIOCSPGRP:
14223 default:
14224 r = ENOTTY;
14225 }
14226
14227 /* Send the reply. */
14228 tty_reply(TASK_REPLY, m_ptr->m_source, m_ptr->PROC_NR, r);
14229 }
822 File: drivers/tty/tty.c MINIX SOURCE CODE
14231 /*===========================================================================*
14232 * do_open *
14233 *===========================================================================*/
14234 PRIVATE void do_open(tp, m_ptr)
14235 register tty_t *tp;
14236 message *m_ptr; /* pointer to message sent to task */
14237 {
14238 /* A tty line has been opened. Make it the callers controlling tty if
14239 * O_NOCTTY is *not* set and it is not the log device. 1 is returned if
14240 * the tty is made the controlling tty, otherwise OK or an error code.
14241 */
14242 int r = OK;
14243
14244 if (m_ptr->TTY_LINE == LOG_MINOR) {
14245 /* The log device is a write-only diagnostics device. */
14246 if (m_ptr->COUNT & R_BIT) r = EACCES;
14247 } else {
14248 if (!(m_ptr->COUNT & O_NOCTTY)) {
14249 tp->tty_pgrp = m_ptr->PROC_NR;
14250 r = 1;
14251 }
14252 tp->tty_openct++;
14253 }
14254 tty_reply(TASK_REPLY, m_ptr->m_source, m_ptr->PROC_NR, r);
14255 }
14257 /*===========================================================================*
14258 * do_close *
14259 *===========================================================================*/
14260 PRIVATE void do_close(tp, m_ptr)
14261 register tty_t *tp;
14262 message *m_ptr; /* pointer to message sent to task */
14263 {
14264 /* A tty line has been closed. Clean up the line if it is the last close. */
14265
14266 if (m_ptr->TTY_LINE != LOG_MINOR && --tp->tty_openct == 0) {
14267 tp->tty_pgrp = 0;
14268 tty_icancel(tp);
14269 (*tp->tty_ocancel)(tp, 0);
14270 (*tp->tty_close)(tp, 0);
14271 tp->tty_termios = termios_defaults;
14272 tp->tty_winsize = winsize_defaults;
14273 setattr(tp);
14274 }
14275 tty_reply(TASK_REPLY, m_ptr->m_source, m_ptr->PROC_NR, OK);
14276 }
14278 /*===========================================================================*
14279 * do_cancel *
14280 *===========================================================================*/
14281 PRIVATE void do_cancel(tp, m_ptr)
14282 register tty_t *tp;
14283 message *m_ptr; /* pointer to message sent to task */
14284 {
14285 /* A signal has been sent to a process that is hanging trying to read or write.
14286 * The pending read or write must be finished off immediately.
14287 */
14288
14289 int proc_nr;
MINIX SOURCE CODE File: drivers/tty/tty.c 823
14290 int mode;
14291
14292 /* Check the parameters carefully, to avoid cancelling twice. */
14293 proc_nr = m_ptr->PROC_NR;
14294 mode = m_ptr->COUNT;
14295 if ((mode & R_BIT) && tp->tty_inleft != 0 && proc_nr == tp->tty_inproc) {
14296 /* Process was reading when killed. Clean up input. */
14297 tty_icancel(tp);
14298 tp->tty_inleft = tp->tty_incum = 0;
14299 }
14300 if ((mode & W_BIT) && tp->tty_outleft != 0 && proc_nr == tp->tty_outproc) {
14301 /* Process was writing when killed. Clean up output. */
14302 (*tp->tty_ocancel)(tp, 0);
14303 tp->tty_outleft = tp->tty_outcum = 0;
14304 }
14305 if (tp->tty_ioreq != 0 && proc_nr == tp->tty_ioproc) {
14306 /* Process was waiting for output to drain. */
14307 tp->tty_ioreq = 0;
14308 }
14309 tp->tty_events = 1;
14310 tty_reply(TASK_REPLY, m_ptr->m_source, proc_nr, EINTR);
14311 }
14313 PUBLIC int select_try(struct tty *tp, int ops)
14314 {
14315 int ready_ops = 0;
14316
14317 /* Special case. If line is hung up, no operations will block.
14318 * (and it can be seen as an exceptional condition.)
14319 */
14320 if (tp->tty_termios.c_ospeed == B0) {
14321 ready_ops |= ops;
14322 }
14323
14324 if (ops & SEL_RD) {
14325 /* will i/o not block on read? */
14326 if (tp->tty_inleft > 0) {
14327 ready_ops |= SEL_RD; /* EIO - no blocking */
14328 } else if (tp->tty_incount > 0) {
14329 /* Is a regular read possible? tty_incount
14330 * says there is data. But a read will only succeed
14331 * in canonical mode if a newline has been seen.
14332 */
14333 if (!(tp->tty_termios.c_lflag & ICANON) ||
14334 tp->tty_eotct > 0) {
14335 ready_ops |= SEL_RD;
14336 }
14337 }
14338 }
14339
14340 if (ops & SEL_WR) {
14341 if (tp->tty_outleft > 0) ready_ops |= SEL_WR;
14342 else if ((*tp->tty_devwrite)(tp, 1)) ready_ops |= SEL_WR;
14343 }
14344
14345 return ready_ops;
14346 }
14348 PUBLIC int select_retry(struct tty *tp)
14349 {
824 File: drivers/tty/tty.c MINIX SOURCE CODE
14350 if (select_try(tp, tp->tty_select_ops))
14351 notify(tp->tty_select_proc);
14352 return OK;
14353 }
14355 /*===========================================================================*
14356 * handle_events *
14357 *===========================================================================*/
14358 PUBLIC void handle_events(tp)
14359 tty_t *tp; /* TTY to check for events. */
14360 {
14361 /* Handle any events pending on a TTY. These events are usually device
14362 * interrupts.
14363 *
14364 * Two kinds of events are prominent:
14365 * - a character has been received from the console or an RS232 line.
14366 * - an RS232 line has completed a write request (on behalf of a user).
14367 * The interrupt handler may delay the interrupt message at its discretion
14368 * to avoid swamping the TTY task. Messages may be overwritten when the
14369 * lines are fast or when there are races between different lines, input
14370 * and output, because MINIX only provides single buffering for interrupt
14371 * messages (in proc.c). This is handled by explicitly checking each line
14372 * for fresh input and completed output on each interrupt.
14373 */
14374 char *buf;
14375 unsigned count;
14376 int status;
14377
14378 do {
14379 tp->tty_events = 0;
14380
14381 /* Read input and perform input processing. */
14382 (*tp->tty_devread)(tp, 0);
14383
14384 /* Perform output processing and write output. */
14385 (*tp->tty_devwrite)(tp, 0);
14386
14387 /* Ioctl waiting for some event? */
14388 if (tp->tty_ioreq != 0) dev_ioctl(tp);
14389 } while (tp->tty_events);
14390
14391 /* Transfer characters from the input queue to a waiting process. */
14392 in_transfer(tp);
14393
14394 /* Reply if enough bytes are available. */
14395 if (tp->tty_incum >= tp->tty_min && tp->tty_inleft > 0) {
14396 if (tp->tty_inrepcode == REVIVE) {
14397 notify(tp->tty_incaller);
14398 tp->tty_inrevived = 1;
14399 } else {
14400 tty_reply(tp->tty_inrepcode, tp->tty_incaller,
14401 tp->tty_inproc, tp->tty_incum);
14402 tp->tty_inleft = tp->tty_incum = 0;
14403 }
14404 }
14405 if (tp->tty_select_ops)
14406 select_retry(tp);
14407 #if NR_PTYS > 0
14408 if (ispty(tp))
14409 select_retry_pty(tp);
MINIX SOURCE CODE File: drivers/tty/tty.c 825
14410 #endif
14411 }
14413 /*===========================================================================*
14414 * in_transfer *
14415 *===========================================================================*/
14416 PRIVATE void in_transfer(tp)
14417 register tty_t *tp; /* pointer to terminal to read from */
14418 {
14419 /* Transfer bytes from the input queue to a process reading from a terminal. */
14420
14421 int ch;
14422 int count;
14423 char buf[64], *bp;
14424
14425 /* Force read to succeed if the line is hung up, looks like EOF to reader. */
14426 if (tp->tty_termios.c_ospeed == B0) tp->tty_min = 0;
14427
14428 /* Anything to do? */
14429 if (tp->tty_inleft == 0 || tp->tty_eotct < tp->tty_min) return;
14430
14431 bp = buf;
14432 while (tp->tty_inleft > 0 && tp->tty_eotct > 0) {
14433 ch = *tp->tty_intail;
14434
14435 if (!(ch & IN_EOF)) {
14436 /* One character to be delivered to the user. */
14437 *bp = ch & IN_CHAR;
14438 tp->tty_inleft--;
14439 if (++bp == bufend(buf)) {
14440 /* Temp buffer full, copy to user space. */
14441 sys_vircopy(SELF, D, (vir_bytes) buf,
14442 tp->tty_inproc, D, tp->tty_in_vir,
14443 (vir_bytes) buflen(buf));
14444 tp->tty_in_vir += buflen(buf);
14445 tp->tty_incum += buflen(buf);
14446 bp = buf;
14447 }
14448 }
14449
14450 /* Remove the character from the input queue. */
14451 if (++tp->tty_intail == bufend(tp->tty_inbuf))
14452 tp->tty_intail = tp->tty_inbuf;
14453 tp->tty_incount--;
14454 if (ch & IN_EOT) {
14455 tp->tty_eotct--;
14456 /* Don’t read past a line break in canonical mode. */
14457 if (tp->tty_termios.c_lflag & ICANON) tp->tty_inleft = 0;
14458 }
14459 }
14460
14461 if (bp > buf) {
14462 /* Leftover characters in the buffer. */
14463 count = bp - buf;
14464 sys_vircopy(SELF, D, (vir_bytes) buf,
14465 tp->tty_inproc, D, tp->tty_in_vir, (vir_bytes) count);
14466 tp->tty_in_vir += count;
14467 tp->tty_incum += count;
14468 }
14469
826 File: drivers/tty/tty.c MINIX SOURCE CODE
14470 /* Usually reply to the reader, possibly even if incum == 0 (EOF). */
14471 if (tp->tty_inleft == 0) {
14472 if (tp->tty_inrepcode == REVIVE) {
14473 notify(tp->tty_incaller);
14474 tp->tty_inrevived = 1;
14475 } else {
14476 tty_reply(tp->tty_inrepcode, tp->tty_incaller,
14477 tp->tty_inproc, tp->tty_incum);
14478 tp->tty_inleft = tp->tty_incum = 0;
14479 }
14480 }
14481 }
14483 /*===========================================================================*
14484 * in_process *
14485 *===========================================================================*/
14486 PUBLIC int in_process(tp, buf, count)
14487 register tty_t *tp; /* terminal on which character has arrived */
14488 char *buf; /* buffer with input characters */
14489 int count; /* number of input characters */
14490 {
14491 /* Characters have just been typed in. Process, save, and echo them. Return
14492 * the number of characters processed.
14493 */
14494
14495 int ch, sig, ct;
14496 int timeset = FALSE;
14497 static unsigned char csize_mask[] = { 0x1F, 0x3F, 0x7F, 0xFF };
14498
14499 for (ct = 0; ct < count; ct++) {
14500 /* Take one character. */
14501 ch = *buf++ & BYTE;
14502
14503 /* Strip to seven bits? */
14504 if (tp->tty_termios.c_iflag & ISTRIP) ch &= 0x7F;
14505
14506 /* Input extensions? */
14507 if (tp->tty_termios.c_lflag & IEXTEN) {
14508
14509 /* Previous character was a character escape? */
14510 if (tp->tty_escaped) {
14511 tp->tty_escaped = NOT_ESCAPED;
14512 ch |= IN_ESC; /* protect character */
14513 }
14514
14515 /* LNEXT (ˆV) to escape the next character? */
14516 if (ch == tp->tty_termios.c_cc[VLNEXT]) {
14517 tp->tty_escaped = ESCAPED;
14518 rawecho(tp, ’ˆ’);
14519 rawecho(tp, ’\b’);
14520 continue; /* do not store the escape */
14521 }
14522
14523 /* REPRINT (ˆR) to reprint echoed characters? */
14524 if (ch == tp->tty_termios.c_cc[VREPRINT]) {
14525 reprint(tp);
14526 continue;
14527 }
14528 }
14529
MINIX SOURCE CODE File: drivers/tty/tty.c 827
14530 /* _POSIX_VDISABLE is a normal character value, so better escape it. */
14531 if (ch == _POSIX_VDISABLE) ch |= IN_ESC;
14532
14533 /* Map CR to LF, ignore CR, or map LF to CR. */
14534 if (ch == ’\r’) {
14535 if (tp->tty_termios.c_iflag & IGNCR) continue;
14536 if (tp->tty_termios.c_iflag & ICRNL) ch = ’\n’;
14537 } else
14538 if (ch == ’\n’) {
14539 if (tp->tty_termios.c_iflag & INLCR) ch = ’\r’;
14540 }
14541
14542 /* Canonical mode? */
14543 if (tp->tty_termios.c_lflag & ICANON) {
14544
14545 /* Erase processing (rub out of last character). */
14546 if (ch == tp->tty_termios.c_cc[VERASE]) {
14547 (void) back_over(tp);
14548 if (!(tp->tty_termios.c_lflag & ECHOE)) {
14549 (void) tty_echo(tp, ch);
14550 }
14551 continue;
14552 }
14553
14554 /* Kill processing (remove current line). */
14555 if (ch == tp->tty_termios.c_cc[VKILL]) {
14556 while (back_over(tp)) {}
14557 if (!(tp->tty_termios.c_lflag & ECHOE)) {
14558 (void) tty_echo(tp, ch);
14559 if (tp->tty_termios.c_lflag & ECHOK)
14560 rawecho(tp, ’\n’);
14561 }
14562 continue;
14563 }
14564
14565 /* EOF (ˆD) means end-of-file, an invisible "line break". */
14566 if (ch == tp->tty_termios.c_cc[VEOF]) ch |= IN_EOT | IN_EOF;
14567
14568 /* The line may be returned to the user after an LF. */
14569 if (ch == ’\n’) ch |= IN_EOT;
14570
14571 /* Same thing with EOL, whatever it may be. */
14572 if (ch == tp->tty_termios.c_cc[VEOL]) ch |= IN_EOT;
14573 }
14574
14575 /* Start/stop input control? */
14576 if (tp->tty_termios.c_iflag & IXON) {
14577
14578 /* Output stops on STOP (ˆS). */
14579 if (ch == tp->tty_termios.c_cc[VSTOP]) {
14580 tp->tty_inhibited = STOPPED;
14581 tp->tty_events = 1;
14582 continue;
14583 }
14584
14585 /* Output restarts on START (ˆQ) or any character if IXANY. */
14586 if (tp->tty_inhibited) {
14587 if (ch == tp->tty_termios.c_cc[VSTART]
14588 || (tp->tty_termios.c_iflag & IXANY)) {
14589 tp->tty_inhibited = RUNNING;
828 File: drivers/tty/tty.c MINIX SOURCE CODE
14590 tp->tty_events = 1;
14591 if (ch == tp->tty_termios.c_cc[VSTART])
14592 continue;
14593 }
14594 }
14595 }
14596
14597 if (tp->tty_termios.c_lflag & ISIG) {
14598 /* Check for INTR (ˆ?) and QUIT (ˆ\) characters. */
14599 if (ch == tp->tty_termios.c_cc[VINTR]
14600 || ch == tp->tty_termios.c_cc[VQUIT]) {
14601 sig = SIGINT;
14602 if (ch == tp->tty_termios.c_cc[VQUIT]) sig = SIGQUIT;
14603 sigchar(tp, sig);
14604 (void) tty_echo(tp, ch);
14605 continue;
14606 }
14607 }
14608
14609 /* Is there space in the input buffer? */
14610 if (tp->tty_incount == buflen(tp->tty_inbuf)) {
14611 /* No space; discard in canonical mode, keep in raw mode. */
14612 if (tp->tty_termios.c_lflag & ICANON) continue;
14613 break;
14614 }
14615
14616 if (!(tp->tty_termios.c_lflag & ICANON)) {
14617 /* In raw mode all characters are "line breaks". */
14618 ch |= IN_EOT;
14619
14620 /* Start an inter-byte timer? */
14621 if (!timeset && tp->tty_termios.c_cc[VMIN] > 0
14622 && tp->tty_termios.c_cc[VTIME] > 0) {
14623 settimer(tp, TRUE);
14624 timeset = TRUE;
14625 }
14626 }
14627
14628 /* Perform the intricate function of echoing. */
14629 if (tp->tty_termios.c_lflag & (ECHO|ECHONL)) ch = tty_echo(tp, ch);
14630
14631 /* Save the character in the input queue. */
14632 *tp->tty_inhead++ = ch;
14633 if (tp->tty_inhead == bufend(tp->tty_inbuf))
14634 tp->tty_inhead = tp->tty_inbuf;
14635 tp->tty_incount++;
14636 if (ch & IN_EOT) tp->tty_eotct++;
14637
14638 /* Try to finish input if the queue threatens to overflow. */
14639 if (tp->tty_incount == buflen(tp->tty_inbuf)) in_transfer(tp);
14640 }
14641 return ct;
14642 }
14644 /*===========================================================================*
14645 * echo *
14646 *===========================================================================*/
14647 PRIVATE int tty_echo(tp, ch)
14648 register tty_t *tp; /* terminal on which to echo */
14649 register int ch; /* pointer to character to echo */
MINIX SOURCE CODE File: drivers/tty/tty.c 829
14650 {
14651 /* Echo the character if echoing is on. Some control characters are echoed
14652 * with their normal effect, other control characters are echoed as "ˆX",
14653 * normal characters are echoed normally. EOF (ˆD) is echoed, but immediately
14654 * backspaced over. Return the character with the echoed length added to its
14655 * attributes.
14656 */
14657 int len, rp;
14658
14659 ch &= ˜IN_LEN;
14660 if (!(tp->tty_termios.c_lflag & ECHO)) {
14661 if (ch == (’\n’ | IN_EOT) && (tp->tty_termios.c_lflag
14662 & (ICANON|ECHONL)) == (ICANON|ECHONL))
14663 (*tp->tty_echo)(tp, ’\n’);
14664 return(ch);
14665 }
14666
14667 /* "Reprint" tells if the echo output has been messed up by other output. */
14668 rp = tp->tty_incount == 0 ? FALSE : tp->tty_reprint;
14669
14670 if ((ch & IN_CHAR) < ’ ’) {
14671 switch (ch & (IN_ESC|IN_EOF|IN_EOT|IN_CHAR)) {
14672 case ’\t’:
14673 len = 0;
14674 do {
14675 (*tp->tty_echo)(tp, ’ ’);
14676 len++;
14677 } while (len < TAB_SIZE && (tp->tty_position & TAB_MASK) != 0);
14678 break;
14679 case ’\r’ | IN_EOT:
14680 case ’\n’ | IN_EOT:
14681 (*tp->tty_echo)(tp, ch & IN_CHAR);
14682 len = 0;
14683 break;
14684 default:
14685 (*tp->tty_echo)(tp, ’ˆ’);
14686 (*tp->tty_echo)(tp, ’@’ + (ch & IN_CHAR));
14687 len = 2;
14688 }
14689 } else
14690 if ((ch & IN_CHAR) == ’\177’) {
14691 /* A DEL prints as "ˆ?". */
14692 (*tp->tty_echo)(tp, ’ˆ’);
14693 (*tp->tty_echo)(tp, ’?’);
14694 len = 2;
14695 } else {
14696 (*tp->tty_echo)(tp, ch & IN_CHAR);
14697 len = 1;
14698 }
14699 if (ch & IN_EOF) while (len > 0) { (*tp->tty_echo)(tp, ’\b’); len--; }
14700
14701 tp->tty_reprint = rp;
14702 return(ch | (len << IN_LSHIFT));
14703 }
14705 /*===========================================================================*
14706 * rawecho *
14707 *===========================================================================*/
14708 PRIVATE void rawecho(tp, ch)
14709 register tty_t *tp;
830 File: drivers/tty/tty.c MINIX SOURCE CODE
14710 int ch;
14711 {
14712 /* Echo without interpretation if ECHO is set. */
14713 int rp = tp->tty_reprint;
14714 if (tp->tty_termios.c_lflag & ECHO) (*tp->tty_echo)(tp, ch);
14715 tp->tty_reprint = rp;
14716 }
14718 /*===========================================================================*
14719 * back_over *
14720 *===========================================================================*/
14721 PRIVATE int back_over(tp)
14722 register tty_t *tp;
14723 {
14724 /* Backspace to previous character on screen and erase it. */
14725 u16_t *head;
14726 int len;
14727
14728 if (tp->tty_incount == 0) return(0); /* queue empty */
14729 head = tp->tty_inhead;
14730 if (head == tp->tty_inbuf) head = bufend(tp->tty_inbuf);
14731 if (*--head & IN_EOT) return(0); /* can’t erase "line breaks" */
14732 if (tp->tty_reprint) reprint(tp); /* reprint if messed up */
14733 tp->tty_inhead = head;
14734 tp->tty_incount--;
14735 if (tp->tty_termios.c_lflag & ECHOE) {
14736 len = (*head & IN_LEN) >> IN_LSHIFT;
14737 while (len > 0) {
14738 rawecho(tp, ’\b’);
14739 rawecho(tp, ’ ’);
14740 rawecho(tp, ’\b’);
14741 len--;
14742 }
14743 }
14744 return(1); /* one character erased */
14745 }
14747 /*===========================================================================*
14748 * reprint *
14749 *===========================================================================*/
14750 PRIVATE void reprint(tp)
14751 register tty_t *tp; /* pointer to tty struct */
14752 {
14753 /* Restore what has been echoed to screen before if the user input has been
14754 * messed up by output, or if REPRINT (ˆR) is typed.
14755 */
14756 int count;
14757 u16_t *head;
14758
14759 tp->tty_reprint = FALSE;
14760
14761 /* Find the last line break in the input. */
14762 head = tp->tty_inhead;
14763 count = tp->tty_incount;
14764 while (count > 0) {
14765 if (head == tp->tty_inbuf) head = bufend(tp->tty_inbuf);
14766 if (head[-1] & IN_EOT) break;
14767 head--;
14768 count--;
14769 }
MINIX SOURCE CODE File: drivers/tty/tty.c 831
14770 if (count == tp->tty_incount) return; /* no reason to reprint */
14771
14772 /* Show REPRINT (ˆR) and move to a new line. */
14773 (void) tty_echo(tp, tp->tty_termios.c_cc[VREPRINT] | IN_ESC);
14774 rawecho(tp, ’\r’);
14775 rawecho(tp, ’\n’);
14776
14777 /* Reprint from the last break onwards. */
14778 do {
14779 if (head == bufend(tp->tty_inbuf)) head = tp->tty_inbuf;
14780 *head = tty_echo(tp, *head);
14781 head++;
14782 count++;
14783 } while (count < tp->tty_incount);
14784 }
14786 /*===========================================================================*
14787 * out_process *
14788 *===========================================================================*/
14789 PUBLIC void out_process(tp, bstart, bpos, bend, icount, ocount)
14790 tty_t *tp;
14791 char *bstart, *bpos, *bend; /* start/pos/end of circular buffer */
14792 int *icount; /* # input chars / input chars used */
14793 int *ocount; /* max output chars / output chars used */
14794 {
14795 /* Perform output processing on a circular buffer. *icount is the number of
14796 * bytes to process, and the number of bytes actually processed on return.
14797 * *ocount is the space available on input and the space used on output.
14798 * (Naturally *icount < *ocount.) The column position is updated modulo
14799 * the TAB size, because we really only need it for tabs.
14800 */
14801
14802 int tablen;
14803 int ict = *icount;
14804 int oct = *ocount;
14805 int pos = tp->tty_position;
14806
14807 while (ict > 0) {
14808 switch (*bpos) {
14809 case ’\7’:
14810 break;
14811 case ’\b’:
14812 pos--;
14813 break;
14814 case ’\r’:
14815 pos = 0;
14816 break;
14817 case ’\n’:
14818 if ((tp->tty_termios.c_oflag & (OPOST|ONLCR))
14819 == (OPOST|ONLCR)) {
14820 /* Map LF to CR+LF if there is space. Note that the
14821 * next character in the buffer is overwritten, so
14822 * we stop at this point.
14823 */
14824 if (oct >= 2) {
14825 *bpos = ’\r’;
14826 if (++bpos == bend) bpos = bstart;
14827 *bpos = ’\n’;
14828 pos = 0;
14829 ict--;
832 File: drivers/tty/tty.c MINIX SOURCE CODE
14830 oct -= 2;
14831 }
14832 goto out_done; /* no space or buffer got changed */
14833 }
14834 break;
14835 case ’\t’:
14836 /* Best guess for the tab length. */
14837 tablen = TAB_SIZE - (pos & TAB_MASK);
14838
14839 if ((tp->tty_termios.c_oflag & (OPOST|XTABS))
14840 == (OPOST|XTABS)) {
14841 /* Tabs must be expanded. */
14842 if (oct >= tablen) {
14843 pos += tablen;
14844 ict--;
14845 oct -= tablen;
14846 do {
14847 *bpos = ’ ’;
14848 if (++bpos == bend) bpos = bstart;
14849 } while (--tablen != 0);
14850 }
14851 goto out_done;
14852 }
14853 /* Tabs are output directly. */
14854 pos += tablen;
14855 break;
14856 default:
14857 /* Assume any other character prints as one character. */
14858 pos++;
14859 }
14860 if (++bpos == bend) bpos = bstart;
14861 ict--;
14862 oct--;
14863 }
14864 out_done:
14865 tp->tty_position = pos & TAB_MASK;
14866
14867 *icount -= ict; /* [io]ct are the number of chars not used */
14868 *ocount -= oct; /* *[io]count are the number of chars that are used */
14869 }
14871 /*===========================================================================*
14872 * dev_ioctl *
14873 *===========================================================================*/
14874 PRIVATE void dev_ioctl(tp)
14875 tty_t *tp;
14876 {
14877 /* The ioctl’s TCSETSW, TCSETSF and TCDRAIN wait for output to finish to make
14878 * sure that an attribute change doesn’t affect the processing of current
14879 * output. Once output finishes the ioctl is executed as in do_ioctl().
14880 */
14881 int result;
14882
14883 if (tp->tty_outleft > 0) return; /* output not finished */
14884
14885 if (tp->tty_ioreq != TCDRAIN) {
14886 if (tp->tty_ioreq == TCSETSF) tty_icancel(tp);
14887 result = sys_vircopy(tp->tty_ioproc, D, tp->tty_iovir,
14888 SELF, D, (vir_bytes) &tp->tty_termios,
14889 (vir_bytes) sizeof(tp->tty_termios));
MINIX SOURCE CODE File: drivers/tty/tty.c 833
14890 setattr(tp);
14891 }
14892 tp->tty_ioreq = 0;
14893 tty_reply(REVIVE, tp->tty_iocaller, tp->tty_ioproc, result);
14894 }
14896 /*===========================================================================*
14897 * setattr *
14898 *===========================================================================*/
14899 PRIVATE void setattr(tp)
14900 tty_t *tp;
14901 {
14902 /* Apply the new line attributes (raw/canonical, line speed, etc.) */
14903 u16_t *inp;
14904 int count;
14905
14906 if (!(tp->tty_termios.c_lflag & ICANON)) {
14907 /* Raw mode; put a "line break" on all characters in the input queue.
14908 * It is undefined what happens to the input queue when ICANON is
14909 * switched off, a process should use TCSAFLUSH to flush the queue.
14910 * Keeping the queue to preserve typeahead is the Right Thing, however
14911 * when a process does use TCSANOW to switch to raw mode.
14912 */
14913 count = tp->tty_eotct = tp->tty_incount;
14914 inp = tp->tty_intail;
14915 while (count > 0) {
14916 *inp |= IN_EOT;
14917 if (++inp == bufend(tp->tty_inbuf)) inp = tp->tty_inbuf;
14918 --count;
14919 }
14920 }
14921
14922 /* Inspect MIN and TIME. */
14923 settimer(tp, FALSE);
14924 if (tp->tty_termios.c_lflag & ICANON) {
14925 /* No MIN & TIME in canonical mode. */
14926 tp->tty_min = 1;
14927 } else {
14928 /* In raw mode MIN is the number of chars wanted, and TIME how long
14929 * to wait for them. With interesting exceptions if either is zero.
14930 */
14931 tp->tty_min = tp->tty_termios.c_cc[VMIN];
14932 if (tp->tty_min == 0 && tp->tty_termios.c_cc[VTIME] > 0)
14933 tp->tty_min = 1;
14934 }
14935
14936 if (!(tp->tty_termios.c_iflag & IXON)) {
14937 /* No start/stop output control, so don’t leave output inhibited. */
14938 tp->tty_inhibited = RUNNING;
14939 tp->tty_events = 1;
14940 }
14941
14942 /* Setting the output speed to zero hangs up the phone. */
14943 if (tp->tty_termios.c_ospeed == B0) sigchar(tp, SIGHUP);
14944
14945 /* Set new line speed, character size, etc at the device level. */
14946 (*tp->tty_ioctl)(tp, 0);
14947 }
834 File: drivers/tty/tty.c MINIX SOURCE CODE
14949 /*===========================================================================*
14950 * tty_reply *
14951 *===========================================================================*/
14952 PUBLIC void tty_reply(code, replyee, proc_nr, status)
14953 int code; /* TASK_REPLY or REVIVE */
14954 int replyee; /* destination address for the reply */
14955 int proc_nr; /* to whom should the reply go? */
14956 int status; /* reply code */
14957 {
14958 /* Send a reply to a process that wanted to read or write data. */
14959 message tty_mess;
14960
14961 tty_mess.m_type = code;
14962 tty_mess.REP_PROC_NR = proc_nr;
14963 tty_mess.REP_STATUS = status;
14964
14965 if ((status = send(replyee, &tty_mess)) != OK) {
14966 panic("TTY","tty_reply failed, status\n", status);
14967 }
14968 }
14970 /*===========================================================================*
14971 * sigchar *
14972 *===========================================================================*/
14973 PUBLIC void sigchar(tp, sig)
14974 register tty_t *tp;
14975 int sig; /* SIGINT, SIGQUIT, SIGKILL or SIGHUP */
14976 {
14977 /* Process a SIGINT, SIGQUIT or SIGKILL char from the keyboard or SIGHUP from
14978 * a tty close, "stty 0", or a real RS-232 hangup. MM will send the signal to
14979 * the process group (INT, QUIT), all processes (KILL), or the session leader
14980 * (HUP).
14981 */
14982 int status;
14983
14984 if (tp->tty_pgrp != 0)
14985 if (OK != (status = sys_kill(tp->tty_pgrp, sig)))
14986 panic("TTY","Error, call to sys_kill failed", status);
14987
14988 if (!(tp->tty_termios.c_lflag & NOFLSH)) {
14989 tp->tty_incount = tp->tty_eotct = 0; /* kill earlier input */
14990 tp->tty_intail = tp->tty_inhead;
14991 (*tp->tty_ocancel)(tp, 0); /* kill all output */
14992 tp->tty_inhibited = RUNNING;
14993 tp->tty_events = 1;
14994 }
14995 }
14997 /*===========================================================================*
14998 * tty_icancel *
14999 *===========================================================================*/
15000 PRIVATE void tty_icancel(tp)
15001 register tty_t *tp;
15002 {
15003 /* Discard all pending input, tty buffer or device. */
15004
15005 tp->tty_incount = tp->tty_eotct = 0;
15006 tp->tty_intail = tp->tty_inhead;
15007 (*tp->tty_icancel)(tp, 0);
15008 }
MINIX SOURCE CODE File: drivers/tty/tty.c 835
15010 /*===========================================================================*
15011 * tty_init *
15012 *===========================================================================*/
15013 PRIVATE void tty_init()
15014 {
15015 /* Initialize tty structure and call device initialization routines. */
15016
15017 register tty_t *tp;
15018 int s;
15019 struct sigaction sigact;
15020
15021 /* Initialize the terminal lines. */
15022 for (tp = FIRST_TTY,s=0; tp < END_TTY; tp++,s++) {
15023
15024 tp->tty_index = s;
15025
15026 tmr_inittimer(&tp->tty_tmr);
15027
15028 tp->tty_intail = tp->tty_inhead = tp->tty_inbuf;
15029 tp->tty_min = 1;
15030 tp->tty_termios = termios_defaults;
15031 tp->tty_icancel = tp->tty_ocancel = tp->tty_ioctl = tp->tty_close =
15032 tty_devnop;
15033 if (tp < tty_addr(NR_CONS)) {
15034 scr_init(tp);
15035 tp->tty_minor = CONS_MINOR + s;
15036 } else
15037 if (tp < tty_addr(NR_CONS+NR_RS_LINES)) {
15038 rs_init(tp);
15039 tp->tty_minor = RS232_MINOR + s-NR_CONS;
15040 } else {
15041 pty_init(tp);
15042 tp->tty_minor = s - (NR_CONS+NR_RS_LINES) + TTYPX_MINOR;
15043 }
15044 }
15045 }
15047 /*===========================================================================*
15048 * tty_timed_out *
15049 *===========================================================================*/
15050 PRIVATE void tty_timed_out(timer_t *tp)
15051 {
15052 /* This timer has expired. Set the events flag, to force processing. */
15053 tty_t *tty_ptr;
15054 tty_ptr = &tty_table[tmr_arg(tp)->ta_int];
15055 tty_ptr->tty_min = 0; /* force read to succeed */
15056 tty_ptr->tty_events = 1;
15057 }
15059 /*===========================================================================*
15060 * expire_timers *
15061 *===========================================================================*/
15062 PRIVATE void expire_timers(void)
15063 {
15064 /* A synchronous alarm message was received. Check if there are any expired
15065 * timers. Possibly set the event flag and reschedule another alarm.
15066 */
15067 clock_t now; /* current time */
15068 int s;
836 File: drivers/tty/tty.c MINIX SOURCE CODE
15069
15070 /* Get the current time to compare the timers against. */
15071 if ((s=getuptime(&now)) != OK)
15072 panic("TTY","Couldn’t get uptime from clock.", s);
15073
15074 /* Scan the queue of timers for expired timers. This dispatch the watchdog
15075 * functions of expired timers. Possibly a new alarm call must be scheduled.
15076 */
15077 tmrs_exptimers(&tty_timers, now, NULL);
15078 if (tty_timers == NULL) tty_next_timeout = TMR_NEVER;
15079 else { /* set new sync alarm */
15080 tty_next_timeout = tty_timers->tmr_exp_time;
15081 if ((s=sys_setalarm(tty_next_timeout, 1)) != OK)
15082 panic("TTY","Couldn’t set synchronous alarm.", s);
15083 }
15084 }
15086 /*===========================================================================*
15087 * settimer *
15088 *===========================================================================*/
15089 PRIVATE void settimer(tty_ptr, enable)
15090 tty_t *tty_ptr; /* line to set or unset a timer on */
15091 int enable; /* set timer if true, otherwise unset */
15092 {
15093 clock_t now; /* current time */
15094 clock_t exp_time;
15095 int s;
15096
15097 /* Get the current time to calculate the timeout time. */
15098 if ((s=getuptime(&now)) != OK)
15099 panic("TTY","Couldn’t get uptime from clock.", s);
15100 if (enable) {
15101 exp_time = now + tty_ptr->tty_termios.c_cc[VTIME] * (HZ/10);
15102 /* Set a new timer for enabling the TTY events flags. */
15103 tmrs_settimer(&tty_timers, &tty_ptr->tty_tmr,
15104 exp_time, tty_timed_out, NULL);
15105 } else {
15106 /* Remove the timer from the active and expired lists. */
15107 tmrs_clrtimer(&tty_timers, &tty_ptr->tty_tmr, NULL);
15108 }
15109
15110 /* Now check if a new alarm must be scheduled. This happens when the front
15111 * of the timers queue was disabled or reinserted at another position, or
15112 * when a new timer was added to the front.
15113 */
15114 if (tty_timers == NULL) tty_next_timeout = TMR_NEVER;
15115 else if (tty_timers->tmr_exp_time != tty_next_timeout) {
15116 tty_next_timeout = tty_timers->tmr_exp_time;
15117 if ((s=sys_setalarm(tty_next_timeout, 1)) != OK)
15118 panic("TTY","Couldn’t set synchronous alarm.", s);
15119 }
15120 }
15122 /*===========================================================================*
15123 * tty_devnop *
15124 *===========================================================================*/
15125 PUBLIC int tty_devnop(tp, try)
15126 tty_t *tp;
15127 int try;
15128 {
MINIX SOURCE CODE File: drivers/tty/tty.c 837
15129 /* Some functions need not be implemented at the device level. */
15130 }
15132 /*===========================================================================*
15133 * do_select *
15134 *===========================================================================*/
15135 PRIVATE void do_select(tp, m_ptr)
15136 register tty_t *tp; /* pointer to tty struct */
15137 register message *m_ptr; /* pointer to message sent to the task */
15138 {
15139 int ops, ready_ops = 0, watch;
15140
15141 ops = m_ptr->PROC_NR & (SEL_RD|SEL_WR|SEL_ERR);
15142 watch = (m_ptr->PROC_NR & SEL_NOTIFY) ? 1 : 0;
15143
15144 ready_ops = select_try(tp, ops);
15145
15146 if (!ready_ops && ops && watch) {
15147 tp->tty_select_ops |= ops;
15148 tp->tty_select_proc = m_ptr->m_source;
15149 }
15150
15151 tty_reply(TASK_REPLY, m_ptr->m_source, m_ptr->PROC_NR, ready_ops);
15152
15153 return;
15154 }