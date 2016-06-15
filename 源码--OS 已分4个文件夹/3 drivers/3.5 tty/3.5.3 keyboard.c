++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             drivers/tty/keyboard.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
15200 /* Keyboard driver for PC’s and AT’s.
15201 *
15202 * Changes:
15203 * Jul 13, 2004 processes can observe function keys (Jorrit N. Herder)
15204 * Jun 15, 2004 removed wreboot(), except panic dumps (Jorrit N. Herder)
15205 * Feb 04, 1994 loadable keymaps (Marcus Hampel)
15206 */
15207
15208 #include "../drivers.h"
15209 #include <sys/time.h>
15210 #include <sys/select.h>
15211 #include <termios.h>
15212 #include <signal.h>
15213 #include <unistd.h>
15214 #include <minix/callnr.h>
15215 #include <minix/com.h>
15216 #include <minix/keymap.h>
15217 #include "tty.h"
15218 #include "keymaps/us-std.src"
15219 #include "../../kernel/const.h"
15220 #include "../../kernel/config.h"
15221 #include "../../kernel/type.h"
15222 #include "../../kernel/proc.h"
15223
15224 int irq_hook_id = -1;
838 File: drivers/tty/keyboard.c MINIX SOURCE CODE
15225
15226 /* Standard and AT keyboard. (PS/2 MCA implies AT throughout.) */
15227 #define KEYBD 0x60 /* I/O port for keyboard data */
15228
15229 /* AT keyboard. */
15230 #define KB_COMMAND 0x64 /* I/O port for commands on AT */
15231 #define KB_STATUS 0x64 /* I/O port for status on AT */
15232 #define KB_ACK 0xFA /* keyboard ack response */
15233 #define KB_OUT_FULL 0x01 /* status bit set when keypress char pending */
15234 #define KB_IN_FULL 0x02 /* status bit set when not ready to receive */
15235 #define LED_CODE 0xED /* command to keyboard to set LEDs */
15236 #define MAX_KB_ACK_RETRIES 0x1000 /* max #times to wait for kb ack */
15237 #define MAX_KB_BUSY_RETRIES 0x1000 /* max #times to loop while kb busy */
15238 #define KBIT 0x80 /* bit used to ack characters to keyboard */
15239
15240 /* Miscellaneous. */
15241 #define ESC_SCAN 0x01 /* reboot key when panicking */
15242 #define SLASH_SCAN 0x35 /* to recognize numeric slash */
15243 #define RSHIFT_SCAN 0x36 /* to distinguish left and right shift */
15244 #define HOME_SCAN 0x47 /* first key on the numeric keypad */
15245 #define INS_SCAN 0x52 /* INS for use in CTRL-ALT-INS reboot */
15246 #define DEL_SCAN 0x53 /* DEL for use in CTRL-ALT-DEL reboot */
15247
15248 #define CONSOLE 0 /* line number for console */
15249 #define KB_IN_BYTES 32 /* size of keyboard input buffer */
15250 PRIVATE char ibuf[KB_IN_BYTES]; /* input buffer */
15251 PRIVATE char *ihead = ibuf; /* next free spot in input buffer */
15252 PRIVATE char *itail = ibuf; /* scan code to return to TTY */
15253 PRIVATE int icount; /* # codes in buffer */
15254
15255 PRIVATE int esc; /* escape scan code detected? */
15256 PRIVATE int alt_l; /* left alt key state */
15257 PRIVATE int alt_r; /* right alt key state */
15258 PRIVATE int alt; /* either alt key */
15259 PRIVATE int ctrl_l; /* left control key state */
15260 PRIVATE int ctrl_r; /* right control key state */
15261 PRIVATE int ctrl; /* either control key */
15262 PRIVATE int shift_l; /* left shift key state */
15263 PRIVATE int shift_r; /* right shift key state */
15264 PRIVATE int shift; /* either shift key */
15265 PRIVATE int num_down; /* num lock key depressed */
15266 PRIVATE int caps_down; /* caps lock key depressed */
15267 PRIVATE int scroll_down; /* scroll lock key depressed */
15268 PRIVATE int locks[NR_CONS]; /* per console lock keys state */
15269
15270 /* Lock key active bits. Chosen to be equal to the keyboard LED bits. */
15271 #define SCROLL_LOCK 0x01
15272 #define NUM_LOCK 0x02
15273 #define CAPS_LOCK 0x04
15274
15275 PRIVATE char numpad_map[] =
15276 {’H’, ’Y’, ’A’, ’B’, ’D’, ’C’, ’V’, ’U’, ’G’, ’S’, ’T’, ’@’};
15277
15278 /* Variables and definition for observed function keys. */
15279 typedef struct observer { int proc_nr; int events; } obs_t;
15280 PRIVATE obs_t fkey_obs[12]; /* observers for F1-F12 */
15281 PRIVATE obs_t sfkey_obs[12]; /* observers for SHIFT F1-F12 */
15282
15283 FORWARD _PROTOTYPE( int kb_ack, (void) );
15284 FORWARD _PROTOTYPE( int kb_wait, (void) );
MINIX SOURCE CODE File: drivers/tty/keyboard.c 839
15285 FORWARD _PROTOTYPE( int func_key, (int scode) );
15286 FORWARD _PROTOTYPE( int scan_keyboard, (void) );
15287 FORWARD _PROTOTYPE( unsigned make_break, (int scode) );
15288 FORWARD _PROTOTYPE( void set_leds, (void) );
15289 FORWARD _PROTOTYPE( void show_key_mappings, (void) );
15290 FORWARD _PROTOTYPE( int kb_read, (struct tty *tp, int try) );
15291 FORWARD _PROTOTYPE( unsigned map_key, (int scode) );
15292
15293 /*===========================================================================*
15294 * map_key0 *
15295 *===========================================================================*/
15296 /* Map a scan code to an ASCII code ignoring modifiers. */
15297 #define map_key0(scode) \
15298 ((unsigned) keymap[(scode) * MAP_COLS])
15299
15300 /*===========================================================================*
15301 * map_key *
15302 *===========================================================================*/
15303 PRIVATE unsigned map_key(scode)
15304 int scode;
15305 {
15306 /* Map a scan code to an ASCII code. */
15307
15308 int caps, column, lk;
15309 u16_t *keyrow;
15310
15311 if (scode == SLASH_SCAN && esc) return ’/’; /* don’t map numeric slash */
15312
15313 keyrow = &keymap[scode * MAP_COLS];
15314
15315 caps = shift;
15316 lk = locks[ccurrent];
15317 if ((lk & NUM_LOCK) && HOME_SCAN <= scode && scode <= DEL_SCAN) caps = !caps;
15318 if ((lk & CAPS_LOCK) && (keyrow[0] & HASCAPS)) caps = !caps;
15319
15320 if (alt) {
15321 column = 2;
15322 if (ctrl || alt_r) column = 3; /* Ctrl + Alt == AltGr */
15323 if (caps) column = 4;
15324 } else {
15325 column = 0;
15326 if (caps) column = 1;
15327 if (ctrl) column = 5;
15328 }
15329 return keyrow[column] & ˜HASCAPS;
15330 }
15332 /*===========================================================================*
15333 * kbd_interrupt *
15334 *===========================================================================*/
15335 PUBLIC void kbd_interrupt(m_ptr)
15336 message *m_ptr;
15337 {
15338 /* A keyboard interrupt has occurred. Process it. */
15339 int scode;
15340 static timer_t timer; /* timer must be static! */
15341
15342 /* Fetch the character from the keyboard hardware and acknowledge it. */
15343 scode = scan_keyboard();
15344
840 File: drivers/tty/keyboard.c MINIX SOURCE CODE
15345 /* Store the scancode in memory so the task can get at it later. */
15346 if (icount < KB_IN_BYTES) {
15347 *ihead++ = scode;
15348 if (ihead == ibuf + KB_IN_BYTES) ihead = ibuf;
15349 icount++;
15350 tty_table[ccurrent].tty_events = 1;
15351 if (tty_table[ccurrent].tty_select_ops & SEL_RD) {
15352 select_retry(&tty_table[ccurrent]);
15353 }
15354 }
15355 }
15357 /*===========================================================================*
15358 * kb_read *
15359 *===========================================================================*/
15360 PRIVATE int kb_read(tp, try)
15361 tty_t *tp;
15362 int try;
15363 {
15364 /* Process characters from the circular keyboard buffer. */
15365 char buf[3];
15366 int scode;
15367 unsigned ch;
15368
15369 tp = &tty_table[ccurrent]; /* always use the current console */
15370
15371 if (try) {
15372 if (icount > 0) return 1;
15373 return 0;
15374 }
15375
15376 while (icount > 0) {
15377 scode = *itail++; /* take one key scan code */
15378 if (itail == ibuf + KB_IN_BYTES) itail = ibuf;
15379 icount--;
15380
15381 /* Function keys are being used for debug dumps. */
15382 if (func_key(scode)) continue;
15383
15384 /* Perform make/break processing. */
15385 ch = make_break(scode);
15386
15387 if (ch <= 0xFF) {
15388 /* A normal character. */
15389 buf[0] = ch;
15390 (void) in_process(tp, buf, 1);
15391 } else
15392 if (HOME <= ch && ch <= INSRT) {
15393 /* An ASCII escape sequence generated by the numeric pad. */
15394 buf[0] = ESC;
15395 buf[1] = ’[’;
15396 buf[2] = numpad_map[ch - HOME];
15397 (void) in_process(tp, buf, 3);
15398 } else
15399 if (ch == ALEFT) {
15400 /* Choose lower numbered console as current console. */
15401 select_console(ccurrent - 1);
15402 set_leds();
15403 } else
15404 if (ch == ARIGHT) {
MINIX SOURCE CODE File: drivers/tty/keyboard.c 841
15405 /* Choose higher numbered console as current console. */
15406 select_console(ccurrent + 1);
15407 set_leds();
15408 } else
15409 if (AF1 <= ch && ch <= AF12) {
15410 /* Alt-F1 is console, Alt-F2 is ttyc1, etc. */
15411 select_console(ch - AF1);
15412 set_leds();
15413 } else
15414 if (CF1 <= ch && ch <= CF12) {
15415 switch(ch) {
15416 case CF1: show_key_mappings(); break;
15417 case CF3: toggle_scroll(); break; /* hardware <-> software */
15418 case CF7: sigchar(&tty_table[CONSOLE], SIGQUIT); break;
15419 case CF8: sigchar(&tty_table[CONSOLE], SIGINT); break;
15420 case CF9: sigchar(&tty_table[CONSOLE], SIGKILL); break;
15421 }
15422 }
15423 }
15424
15425 return 1;
15426 }
15428 /*===========================================================================*
15429 * make_break *
15430 *===========================================================================*/
15431 PRIVATE unsigned make_break(scode)
15432 int scode; /* scan code of key just struck or released */
15433 {
15434 /* This routine can handle keyboards that interrupt only on key depression,
15435 * as well as keyboards that interrupt on key depression and key release.
15436 * For efficiency, the interrupt routine filters out most key releases.
15437 */
15438 int ch, make, escape;
15439 static int CAD_count = 0;
15440
15441 /* Check for CTRL-ALT-DEL, and if found, halt the computer. This would
15442 * be better done in keyboard() in case TTY is hung, except control and
15443 * alt are set in the high level code.
15444 */
15445 if (ctrl && alt && (scode == DEL_SCAN || scode == INS_SCAN))
15446 {
15447 if (++CAD_count == 3) sys_abort(RBT_HALT);
15448 sys_kill(INIT_PROC_NR, SIGABRT);
15449 return -1;
15450 }
15451
15452 /* High-order bit set on key release. */
15453 make = (scode & KEY_RELEASE) == 0; /* true if pressed */
15454
15455 ch = map_key(scode &= ASCII_MASK); /* map to ASCII */
15456
15457 escape = esc; /* Key is escaped? (true if added since the XT) */
15458 esc = 0;
15459
15460 switch (ch) {
15461 case CTRL: /* Left or right control key */
15462 *(escape ? &ctrl_r : &ctrl_l) = make;
15463 ctrl = ctrl_l | ctrl_r;
15464 break;
842 File: drivers/tty/keyboard.c MINIX SOURCE CODE
15465 case SHIFT: /* Left or right shift key */
15466 *(scode == RSHIFT_SCAN ? &shift_r : &shift_l) = make;
15467 shift = shift_l | shift_r;
15468 break;
15469 case ALT: /* Left or right alt key */
15470 *(escape ? &alt_r : &alt_l) = make;
15471 alt = alt_l | alt_r;
15472 break;
15473 case CALOCK: /* Caps lock - toggle on 0 -> 1 transition */
15474 if (caps_down < make) {
15475 locks[ccurrent] ˆ= CAPS_LOCK;
15476 set_leds();
15477 }
15478 caps_down = make;
15479 break;
15480 case NLOCK: /* Num lock */
15481 if (num_down < make) {
15482 locks[ccurrent] ˆ= NUM_LOCK;
15483 set_leds();
15484 }
15485 num_down = make;
15486 break;
15487 case SLOCK: /* Scroll lock */
15488 if (scroll_down < make) {
15489 locks[ccurrent] ˆ= SCROLL_LOCK;
15490 set_leds();
15491 }
15492 scroll_down = make;
15493 break;
15494 case EXTKEY: /* Escape keycode */
15495 esc = 1; /* Next key is escaped */
15496 return(-1);
15497 default: /* A normal key */
15498 if (make) return(ch);
15499 }
15500
15501 /* Key release, or a shift type key. */
15502 return(-1);
15503 }
15505 /*===========================================================================*
15506 * set_leds *
15507 *===========================================================================*/
15508 PRIVATE void set_leds()
15509 {
15510 /* Set the LEDs on the caps, num, and scroll lock keys */
15511 int s;
15512 if (! machine.pc_at) return; /* PC/XT doesn’t have LEDs */
15513
15514 kb_wait(); /* wait for buffer empty */
15515 if ((s=sys_outb(KEYBD, LED_CODE)) != OK)
15516 printf("Warning, sys_outb couldn’t prepare for LED values: %d\n", s);
15517 /* prepare keyboard to accept LED values */
15518 kb_ack(); /* wait for ack response */
15519
15520 kb_wait(); /* wait for buffer empty */
15521 if ((s=sys_outb(KEYBD, locks[ccurrent])) != OK)
15522 printf("Warning, sys_outb couldn’t give LED values: %d\n", s);
15523 /* give keyboard LED values */
15524 kb_ack(); /* wait for ack response */
MINIX SOURCE CODE File: drivers/tty/keyboard.c 843
15525 }
15527 /*===========================================================================*
15528 * kb_wait *
15529 *===========================================================================*/
15530 PRIVATE int kb_wait()
15531 {
15532 /* Wait until the controller is ready; return zero if this times out. */
15533
15534 int retries, status, temp;
15535 int s;
15536
15537 retries = MAX_KB_BUSY_RETRIES + 1; /* wait until not busy */
15538 do {
15539 s = sys_inb(KB_STATUS, &status);
15540 if (status & KB_OUT_FULL) {
15541 s = sys_inb(KEYBD, &temp); /* discard value */
15542 }
15543 if (! (status & (KB_IN_FULL|KB_OUT_FULL)) )
15544 break; /* wait until ready */
15545 } while (--retries != 0); /* continue unless timeout */
15546 return(retries); /* zero on timeout, positive if ready */
15547 }
15549 /*===========================================================================*
15550 * kb_ack *
15551 *===========================================================================*/
15552 PRIVATE int kb_ack()
15553 {
15554 /* Wait until kbd acknowledges last command; return zero if this times out. */
15555
15556 int retries, s;
15557 u8_t u8val;
15558
15559 retries = MAX_KB_ACK_RETRIES + 1;
15560 do {
15561 s = sys_inb(KEYBD, &u8val);
15562 if (u8val == KB_ACK)
15563 break; /* wait for ack */
15564 } while(--retries != 0); /* continue unless timeout */
15565
15566 return(retries); /* nonzero if ack received */
15567 }
15569 /*===========================================================================*
15570 * kb_init *
15571 *===========================================================================*/
15572 PUBLIC void kb_init(tp)
15573 tty_t *tp;
15574 {
15575 /* Initialize the keyboard driver. */
15576
15577 tp->tty_devread = kb_read; /* input function */
15578 }
15580 /*===========================================================================*
15581 * kb_init_once *
15582 *===========================================================================*/
15583 PUBLIC void kb_init_once(void)
15584 {
844 File: drivers/tty/keyboard.c MINIX SOURCE CODE
15585 int i;
15586
15587 set_leds(); /* turn off numlock led */
15588 scan_keyboard(); /* discard leftover keystroke */
15589
15590 /* Clear the function key observers array. Also see func_key(). */
15591 for (i=0; i<12; i++) {
15592 fkey_obs[i].proc_nr = NONE; /* F1-F12 observers */
15593 fkey_obs[i].events = 0; /* F1-F12 observers */
15594 sfkey_obs[i].proc_nr = NONE; /* Shift F1-F12 observers */
15595 sfkey_obs[i].events = 0; /* Shift F1-F12 observers */
15596 }
15597
15598 /* Set interrupt handler and enable keyboard IRQ. */
15599 irq_hook_id = KEYBOARD_IRQ; /* id to be returned on interrupt */
15600 if ((i=sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE, &irq_hook_id)) != OK)
15601 panic("TTY", "Couldn’t set keyboard IRQ policy", i);
15602 if ((i=sys_irqenable(&irq_hook_id)) != OK)
15603 panic("TTY", "Couldn’t enable keyboard IRQs", i);
15604 kbd_irq_set |= (1 << KEYBOARD_IRQ);
15605 }
15607 /*===========================================================================*
15608 * kbd_loadmap *
15609 *===========================================================================*/
15610 PUBLIC int kbd_loadmap(m)
15611 message *m;
15612 {
15613 /* Load a new keymap. */
15614 int result;
15615 result = sys_vircopy(m->PROC_NR, D, (vir_bytes) m->ADDRESS,
15616 SELF, D, (vir_bytes) keymap,
15617 (vir_bytes) sizeof(keymap));
15618 return(result);
15619 }
15621 /*===========================================================================*
15622 * do_fkey_ctl *
15623 *===========================================================================*/
15624 PUBLIC void do_fkey_ctl(m_ptr)
15625 message *m_ptr; /* pointer to the request message */
15626 {
15627 /* This procedure allows processes to register a function key to receive
15628 * notifications if it is pressed. At most one binding per key can exist.
15629 */
15630 int i;
15631 int result;
15632
15633 switch (m_ptr->FKEY_REQUEST) { /* see what we must do */
15634 case FKEY_MAP: /* request for new mapping */
15635 result = OK; /* assume everything will be ok*/
15636 for (i=0; i < 12; i++) { /* check F1-F12 keys */
15637 if (bit_isset(m_ptr->FKEY_FKEYS, i+1) ) {
15638 if (fkey_obs[i].proc_nr == NONE) {
15639 fkey_obs[i].proc_nr = m_ptr->m_source;
15640 fkey_obs[i].events = 0;
15641 bit_unset(m_ptr->FKEY_FKEYS, i+1);
15642 } else {
15643 printf("WARNING, fkey_map failed F%d\n", i+1);
15644 result = EBUSY; /* report failure, but try rest */
MINIX SOURCE CODE File: drivers/tty/keyboard.c 845
15645 }
15646 }
15647 }
15648 for (i=0; i < 12; i++) { /* check Shift+F1-F12 keys */
15649 if (bit_isset(m_ptr->FKEY_SFKEYS, i+1) ) {
15650 if (sfkey_obs[i].proc_nr == NONE) {
15651 sfkey_obs[i].proc_nr = m_ptr->m_source;
15652 sfkey_obs[i].events = 0;
15653 bit_unset(m_ptr->FKEY_SFKEYS, i+1);
15654 } else {
15655 printf("WARNING, fkey_map failed Shift F%d\n", i+1);
15656 result = EBUSY; /* report failure but try rest */
15657 }
15658 }
15659 }
15660 break;
15661 case FKEY_UNMAP:
15662 result = OK; /* assume everything will be ok*/
15663 for (i=0; i < 12; i++) { /* check F1-F12 keys */
15664 if (bit_isset(m_ptr->FKEY_FKEYS, i+1) ) {
15665 if (fkey_obs[i].proc_nr == m_ptr->m_source) {
15666 fkey_obs[i].proc_nr = NONE;
15667 fkey_obs[i].events = 0;
15668 bit_unset(m_ptr->FKEY_FKEYS, i+1);
15669 } else {
15670 result = EPERM; /* report failure, but try rest */
15671 }
15672 }
15673 }
15674 for (i=0; i < 12; i++) { /* check Shift+F1-F12 keys */
15675 if (bit_isset(m_ptr->FKEY_SFKEYS, i+1) ) {
15676 if (sfkey_obs[i].proc_nr == m_ptr->m_source) {
15677 sfkey_obs[i].proc_nr = NONE;
15678 sfkey_obs[i].events = 0;
15679 bit_unset(m_ptr->FKEY_SFKEYS, i+1);
15680 } else {
15681 result = EPERM; /* report failure, but try rest */
15682 }
15683 }
15684 }
15685 break;
15686 case FKEY_EVENTS:
15687 m_ptr->FKEY_FKEYS = m_ptr->FKEY_SFKEYS = 0;
15688 for (i=0; i < 12; i++) { /* check (Shift+) F1-F12 keys */
15689 if (fkey_obs[i].proc_nr == m_ptr->m_source) {
15690 if (fkey_obs[i].events) {
15691 bit_set(m_ptr->FKEY_FKEYS, i+1);
15692 fkey_obs[i].events = 0;
15693 }
15694 }
15695 if (sfkey_obs[i].proc_nr == m_ptr->m_source) {
15696 if (sfkey_obs[i].events) {
15697 bit_set(m_ptr->FKEY_SFKEYS, i+1);
15698 sfkey_obs[i].events = 0;
15699 }
15700 }
15701 }
15702 break;
15703 default:
15704 result = EINVAL; /* key cannot be observed */
846 File: drivers/tty/keyboard.c MINIX SOURCE CODE
15705 }
15706
15707 /* Almost done, return result to caller. */
15708 m_ptr->m_type = result;
15709 send(m_ptr->m_source, m_ptr);
15710 }
15712 /*===========================================================================*
15713 * func_key *
15714 *===========================================================================*/
15715 PRIVATE int func_key(scode)
15716 int scode; /* scan code for a function key */
15717 {
15718 /* This procedure traps function keys for debugging purposes. Observers of
15719 * function keys are kept in a global array. If a subject (a key) is pressed
15720 * the observer is notified of the event. Initialization of the arrays is done
15721 * in kb_init, where NONE is set to indicate there is no interest in the key.
15722 * Returns FALSE on a key release or if the key is not observable.
15723 */
15724 message m;
15725 int key;
15726 int proc_nr;
15727 int i,s;
15728
15729 /* Ignore key releases. If this is a key press, get full key code. */
15730 if (scode & KEY_RELEASE) return(FALSE); /* key release */
15731 key = map_key(scode); /* include modifiers */
15732
15733 /* Key pressed, now see if there is an observer for the pressed key.
15734 * F1-F12 observers are in fkey_obs array.
15735 * SHIFT F1-F12 observers are in sfkey_req array.
15736 * CTRL F1-F12 reserved (see kb_read)
15737 * ALT F1-F12 reserved (see kb_read)
15738 * Other combinations are not in use. Note that Alt+Shift+F1-F12 is yet
15739 * defined in <minix/keymap.h>, and thus is easy for future extensions.
15740 */
15741 if (F1 <= key && key <= F12) { /* F1-F12 */
15742 proc_nr = fkey_obs[key - F1].proc_nr;
15743 fkey_obs[key - F1].events ++ ;
15744 } else if (SF1 <= key && key <= SF12) { /* Shift F2-F12 */
15745 proc_nr = sfkey_obs[key - SF1].proc_nr;
15746 sfkey_obs[key - SF1].events ++;
15747 }
15748 else {
15749 return(FALSE); /* not observable */
15750 }
15751
15752 /* See if an observer is registered and send it a message. */
15753 if (proc_nr != NONE) {
15754 m.NOTIFY_TYPE = FKEY_PRESSED;
15755 notify(proc_nr);
15756 }
15757 return(TRUE);
15758 }
15760 /*===========================================================================*
15761 * show_key_mappings *
15762 *===========================================================================*/
15763 PRIVATE void show_key_mappings()
15764 {
MINIX SOURCE CODE File: drivers/tty/keyboard.c 847
15765 int i,s;
15766 struct proc proc;
15767
15768 printf("\n");
15769 printf("System information. Known function key mappings to request debug dumps:\n");
15770 printf("-------------------------------------------------------------------------\n");
15771 for (i=0; i<12; i++) {
15772
15773 printf(" %sF%d: ", i+1<10? " ":"", i+1);
15774 if (fkey_obs[i].proc_nr != NONE) {
15775 if ((s=sys_getproc(&proc, fkey_obs[i].proc_nr))!=OK)
15776 printf("sys_getproc: %d\n", s);
15777 printf("%-14.14s", proc.p_name);
15778 } else {
15779 printf("%-14.14s", "<none>");
15780 }
15781
15782 printf(" %sShift-F%d: ", i+1<10? " ":"", i+1);
15783 if (sfkey_obs[i].proc_nr != NONE) {
15784 if ((s=sys_getproc(&proc, sfkey_obs[i].proc_nr))!=OK)
15785 printf("sys_getproc: %d\n", s);
15786 printf("%-14.14s", proc.p_name);
15787 } else {
15788 printf("%-14.14s", "<none>");
15789 }
15790 printf("\n");
15791 }
15792 printf("\n");
15793 printf("Press one of the registered function keys to trigger a debug dump.\n");
15794 printf("\n");
15795 }
15797 /*===========================================================================*
15798 * scan_keyboard *
15799 *===========================================================================*/
15800 PRIVATE int scan_keyboard()
15801 {
15802 /* Fetch the character from the keyboard hardware and acknowledge it. */
15803 pvb_pair_t byte_in[2], byte_out[2];
15804
15805 byte_in[0].port = KEYBD; /* get the scan code for the key struck */
15806 byte_in[1].port = PORT_B; /* strobe the keyboard to ack the char */
15807 sys_vinb(byte_in, 2); /* request actual input */
15808
15809 pv_set(byte_out[0], PORT_B, byte_in[1].value | KBIT); /* strobe bit high */
15810 pv_set(byte_out[1], PORT_B, byte_in[1].value); /* then strobe low */
15811 sys_voutb(byte_out, 2); /* request actual output */
15812
15813 return(byte_in[0].value); /* return scan code */
15814 }
15816 /*===========================================================================*
15817 * do_panic_dumps *
15818 *===========================================================================*/
15819 PUBLIC void do_panic_dumps(m)
15820 message *m; /* request message to TTY */
15821 {
15822 /* Wait for keystrokes for printing debugging info and reboot. */
15823 int quiet, code;
15824
848 File: drivers/tty/keyboard.c MINIX SOURCE CODE
15825 /* A panic! Allow debug dumps until user wants to shutdown. */
15826 printf("\nHit ESC to reboot, DEL to shutdown, F-keys for debug dumps\n");
15827
15828 (void) scan_keyboard(); /* ack any old input */
15829 quiet = scan_keyboard();/* quiescent value (0 on PC, last code on AT)*/
15830 for (;;) {
15831 tickdelay(10);
15832 /* See if there are pending request for output, but don’t block.
15833 * Diagnostics can span multiple printf()s, so do it in a loop.
15834 */
15835 while (nb_receive(ANY, m) == OK) {
15836 switch(m->m_type) {
15837 case FKEY_CONTROL: do_fkey_ctl(m); break;
15838 case SYS_SIG: do_new_kmess(m); break;
15839 case DIAGNOSTICS: do_diagnostics(m); break;
15840 default: ; /* do nothing */
15841 }
15842 tickdelay(1); /* allow more */
15843 }
15844 code = scan_keyboard();
15845 if (code != quiet) {
15846 /* A key has been pressed. */
15847 switch (code) { /* possibly abort MINIX */
15848 case ESC_SCAN: sys_abort(RBT_REBOOT); return;
15849 case DEL_SCAN: sys_abort(RBT_HALT); return;
15850 }
15851 (void) func_key(code); /* check for function key */
15852 quiet = scan_keyboard();
15853 }
15854 }
15855 }