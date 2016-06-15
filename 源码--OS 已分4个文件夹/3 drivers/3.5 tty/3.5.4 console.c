++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             drivers/tty/console.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
15900 /* Code and data for the IBM console driver.
15901 *
15902 * The 6845 video controller used by the IBM PC shares its video memory with
15903 * the CPU somewhere in the 0xB0000 memory bank. To the 6845 this memory
15904 * consists of 16-bit words. Each word has a character code in the low byte
15905 * and a so-called attribute byte in the high byte. The CPU directly modifies
15906 * video memory to display characters, and sets two registers on the 6845 that
15907 * specify the video origin and the cursor position. The video origin is the
15908 * place in video memory where the first character (upper left corner) can
15909 * be found. Moving the origin is a fast way to scroll the screen. Some
15910 * video adapters wrap around the top of video memory, so the origin can
15911 * move without bounds. For other adapters screen memory must sometimes be
15912 * moved to reset the origin. All computations on video memory use character
15913 * (word) addresses for simplicity and assume there is no wrapping. The
15914 * assembly support functions translate the word addresses to byte addresses
15915 * and the scrolling function worries about wrapping.
15916 */
15917
15918 #include "../drivers.h"
15919 #include <termios.h>
MINIX SOURCE CODE File: drivers/tty/console.c 849
15920 #include <minix/callnr.h>
15921 #include <minix/com.h>
15922 #include "tty.h"
15923
15924 #include "../../kernel/const.h"
15925 #include "../../kernel/config.h"
15926 #include "../../kernel/type.h"
15927
15928 /* Definitions used by the console driver. */
15929 #define MONO_BASE 0xB0000L /* base of mono video memory */
15930 #define COLOR_BASE 0xB8000L /* base of color video memory */
15931 #define MONO_SIZE 0x1000 /* 4K mono video memory */
15932 #define COLOR_SIZE 0x4000 /* 16K color video memory */
15933 #define EGA_SIZE 0x8000 /* EGA & VGA have at least 32K */
15934 #define BLANK_COLOR 0x0700 /* determines cursor color on blank screen */
15935 #define SCROLL_UP 0 /* scroll forward */
15936 #define SCROLL_DOWN 1 /* scroll backward */
15937 #define BLANK_MEM ((u16_t *) 0) /* tells mem_vid_copy() to blank the screen */
15938 #define CONS_RAM_WORDS 80 /* video ram buffer size */
15939 #define MAX_ESC_PARMS 4 /* number of escape sequence params allowed */
15940
15941 /* Constants relating to the controller chips. */
15942 #define M_6845 0x3B4 /* port for 6845 mono */
15943 #define C_6845 0x3D4 /* port for 6845 color */
15944 #define INDEX 0 /* 6845’s index register */
15945 #define DATA 1 /* 6845’s data register */
15946 #define STATUS 6 /* 6845’s status register */
15947 #define VID_ORG 12 /* 6845’s origin register */
15948 #define CURSOR 14 /* 6845’s cursor register */
15949
15950 /* Beeper. */
15951 #define BEEP_FREQ 0x0533 /* value to put into timer to set beep freq */
15952 #define B_TIME 3 /* length of CTRL-G beep is ticks */
15953
15954 /* definitions used for font management */
15955 #define GA_SEQUENCER_INDEX 0x3C4
15956 #define GA_SEQUENCER_DATA 0x3C5
15957 #define GA_GRAPHICS_INDEX 0x3CE
15958 #define GA_GRAPHICS_DATA 0x3CF
15959 #define GA_VIDEO_ADDRESS 0xA0000L
15960 #define GA_FONT_SIZE 8192
15961
15962 /* Global variables used by the console driver and assembly support. */
15963 PUBLIC int vid_index; /* index of video segment in remote mem map */
15964 PUBLIC u16_t vid_seg;
15965 PUBLIC vir_bytes vid_off; /* video ram is found at vid_seg:vid_off */
15966 PUBLIC unsigned vid_size; /* 0x2000 for color or 0x0800 for mono */
15967 PUBLIC unsigned vid_mask; /* 0x1FFF for color or 0x07FF for mono */
15968 PUBLIC unsigned blank_color = BLANK_COLOR; /* display code for blank */
15969
15970 /* Private variables used by the console driver. */
15971 PRIVATE int vid_port; /* I/O port for accessing 6845 */
15972 PRIVATE int wrap; /* hardware can wrap? */
15973 PRIVATE int softscroll; /* 1 = software scrolling, 0 = hardware */
15974 PRIVATE int beeping; /* speaker is beeping? */
15975 PRIVATE unsigned font_lines; /* font lines per character */
15976 PRIVATE unsigned scr_width; /* # characters on a line */
15977 PRIVATE unsigned scr_lines; /* # lines on the screen */
15978 PRIVATE unsigned scr_size; /* # characters on the screen */
15979
850 File: drivers/tty/console.c MINIX SOURCE CODE
15980 /* Per console data. */
15981 typedef struct console {
15982 tty_t *c_tty; /* associated TTY struct */
15983 int c_column; /* current column number (0-origin) */
15984 int c_row; /* current row (0 at top of screen) */
15985 int c_rwords; /* number of WORDS (not bytes) in outqueue */
15986 unsigned c_start; /* start of video memory of this console */
15987 unsigned c_limit; /* limit of this console’s video memory */
15988 unsigned c_org; /* location in RAM where 6845 base points */
15989 unsigned c_cur; /* current position of cursor in video RAM */
15990 unsigned c_attr; /* character attribute */
15991 unsigned c_blank; /* blank attribute */
15992 char c_reverse; /* reverse video */
15993 char c_esc_state; /* 0=normal, 1=ESC, 2=ESC[ */
15994 char c_esc_intro; /* Distinguishing character following ESC */
15995 int *c_esc_parmp; /* pointer to current escape parameter */
15996 int c_esc_parmv[MAX_ESC_PARMS]; /* list of escape parameters */
15997 u16_t c_ramqueue[CONS_RAM_WORDS]; /* buffer for video RAM */
15998 } console_t;
15999
16000 PRIVATE int nr_cons= 1; /* actual number of consoles */
16001 PRIVATE console_t cons_table[NR_CONS];
16002 PRIVATE console_t *curcons; /* currently visible */
16003
16004 /* Color if using a color controller. */
16005 #define color (vid_port == C_6845)
16006
16007 /* Map from ANSI colors to the attributes used by the PC */
16008 PRIVATE int ansi_colors[8] = {0, 4, 2, 6, 1, 5, 3, 7};
16009
16010 /* Structure used for font management */
16011 struct sequence {
16012 unsigned short index;
16013 unsigned char port;
16014 unsigned char value;
16015 };
16016
16017 FORWARD _PROTOTYPE( int cons_write, (struct tty *tp, int try) );
16018 FORWARD _PROTOTYPE( void cons_echo, (tty_t *tp, int c) );
16019 FORWARD _PROTOTYPE( void out_char, (console_t *cons, int c) );
16020 FORWARD _PROTOTYPE( void putk, (int c) );
16021 FORWARD _PROTOTYPE( void beep, (void) );
16022 FORWARD _PROTOTYPE( void do_escape, (console_t *cons, int c) );
16023 FORWARD _PROTOTYPE( void flush, (console_t *cons) );
16024 FORWARD _PROTOTYPE( void parse_escape, (console_t *cons, int c) );
16025 FORWARD _PROTOTYPE( void scroll_screen, (console_t *cons, int dir) );
16026 FORWARD _PROTOTYPE( void set_6845, (int reg, unsigned val) );
16027 FORWARD _PROTOTYPE( void get_6845, (int reg, unsigned *val) );
16028 FORWARD _PROTOTYPE( void stop_beep, (timer_t *tmrp) );
16029 FORWARD _PROTOTYPE( void cons_org0, (void) );
16030 FORWARD _PROTOTYPE( int ga_program, (struct sequence *seq) );
16031 FORWARD _PROTOTYPE( int cons_ioctl, (tty_t *tp, int) );
16032
16033 /*===========================================================================*
16034 * cons_write *
16035 *===========================================================================*/
16036 PRIVATE int cons_write(tp, try)
16037 register struct tty *tp; /* tells which terminal is to be used */
16038 int try;
16039 {
MINIX SOURCE CODE File: drivers/tty/console.c 851
16040 /* Copy as much data as possible to the output queue, then start I/O. On
16041 * memory-mapped terminals, such as the IBM console, the I/O will also be
16042 * finished, and the counts updated. Keep repeating until all I/O done.
16043 */
16044
16045 int count;
16046 int result;
16047 register char *tbuf;
16048 char buf[64];
16049 console_t *cons = tp->tty_priv;
16050
16051 if (try) return 1; /* we can always write to console */
16052
16053 /* Check quickly for nothing to do, so this can be called often without
16054 * unmodular tests elsewhere.
16055 */
16056 if ((count = tp->tty_outleft) == 0 || tp->tty_inhibited) return;
16057
16058 /* Copy the user bytes to buf[] for decent addressing. Loop over the
16059 * copies, since the user buffer may be much larger than buf[].
16060 */
16061 do {
16062 if (count > sizeof(buf)) count = sizeof(buf);
16063 if ((result = sys_vircopy(tp->tty_outproc, D, tp->tty_out_vir,
16064 SELF, D, (vir_bytes) buf, (vir_bytes) count)) != OK)
16065 break;
16066 tbuf = buf;
16067
16068 /* Update terminal data structure. */
16069 tp->tty_out_vir += count;
16070 tp->tty_outcum += count;
16071 tp->tty_outleft -= count;
16072
16073 /* Output each byte of the copy to the screen. Avoid calling
16074 * out_char() for the "easy" characters, put them into the buffer
16075 * directly.
16076 */
16077 do {
16078 if ((unsigned) *tbuf < ’ ’ || cons->c_esc_state > 0
16079 || cons->c_column >= scr_width
16080 || cons->c_rwords >= buflen(cons->c_ramqueue))
16081 {
16082 out_char(cons, *tbuf++);
16083 } else {
16084 cons->c_ramqueue[cons->c_rwords++] =
16085 cons->c_attr | (*tbuf++ & BYTE);
16086 cons->c_column++;
16087 }
16088 } while (--count != 0);
16089 } while ((count = tp->tty_outleft) != 0 && !tp->tty_inhibited);
16090
16091 flush(cons); /* transfer anything buffered to the screen */
16092
16093 /* Reply to the writer if all output is finished or if an error occured. */
16094 if (tp->tty_outleft == 0 || result != OK) {
16095 /* REVIVE is not possible. I/O on memory mapped consoles finishes. */
16096 tty_reply(tp->tty_outrepcode, tp->tty_outcaller, tp->tty_outproc,
16097 tp->tty_outcum);
16098 tp->tty_outcum = 0;
16099 }
852 File: drivers/tty/console.c MINIX SOURCE CODE
16100 }
16102 /*===========================================================================*
16103 * cons_echo *
16104 *===========================================================================*/
16105 PRIVATE void cons_echo(tp, c)
16106 register tty_t *tp; /* pointer to tty struct */
16107 int c; /* character to be echoed */
16108 {
16109 /* Echo keyboard input (print & flush). */
16110 console_t *cons = tp->tty_priv;
16111
16112 out_char(cons, c);
16113 flush(cons);
16114 }
16116 /*===========================================================================*
16117 * out_char *
16118 *===========================================================================*/
16119 PRIVATE void out_char(cons, c)
16120 register console_t *cons; /* pointer to console struct */
16121 int c; /* character to be output */
16122 {
16123 /* Output a character on the console. Check for escape sequences first. */
16124 if (cons->c_esc_state > 0) {
16125 parse_escape(cons, c);
16126 return;
16127 }
16128
16129 switch(c) {
16130 case 000: /* null is typically used for padding */
16131 return; /* better not do anything */
16132
16133 case 007: /* ring the bell */
16134 flush(cons); /* print any chars queued for output */
16135 beep();
16136 return;
16137
16138 case ’\b’: /* backspace */
16139 if (--cons->c_column < 0) {
16140 if (--cons->c_row >= 0) cons->c_column += scr_width;
16141 }
16142 flush(cons);
16143 return;
16144
16145 case ’\n’: /* line feed */
16146 if ((cons->c_tty->tty_termios.c_oflag & (OPOST|ONLCR))
16147 == (OPOST|ONLCR)) {
16148 cons->c_column = 0;
16149 }
16150 /*FALL THROUGH*/
16151 case 013: /* CTRL-K */
16152 case 014: /* CTRL-L */
16153 if (cons->c_row == scr_lines-1) {
16154 scroll_screen(cons, SCROLL_UP);
16155 } else {
16156 cons->c_row++;
16157 }
16158 flush(cons);
16159 return;
MINIX SOURCE CODE File: drivers/tty/console.c 853
16160
16161 case ’\r’: /* carriage return */
16162 cons->c_column = 0;
16163 flush(cons);
16164 return;
16165
16166 case ’\t’: /* tab */
16167 cons->c_column = (cons->c_column + TAB_SIZE) & ˜TAB_MASK;
16168 if (cons->c_column > scr_width) {
16169 cons->c_column -= scr_width;
16170 if (cons->c_row == scr_lines-1) {
16171 scroll_screen(cons, SCROLL_UP);
16172 } else {
16173 cons->c_row++;
16174 }
16175 }
16176 flush(cons);
16177 return;
16178
16179 case 033: /* ESC - start of an escape sequence */
16180 flush(cons); /* print any chars queued for output */
16181 cons->c_esc_state = 1; /* mark ESC as seen */
16182 return;
16183
16184 default: /* printable chars are stored in ramqueue */
16185 if (cons->c_column >= scr_width) {
16186 if (!LINEWRAP) return;
16187 if (cons->c_row == scr_lines-1) {
16188 scroll_screen(cons, SCROLL_UP);
16189 } else {
16190 cons->c_row++;
16191 }
16192 cons->c_column = 0;
16193 flush(cons);
16194 }
16195 if (cons->c_rwords == buflen(cons->c_ramqueue)) flush(cons);
16196 cons->c_ramqueue[cons->c_rwords++] = cons->c_attr | (c & BYTE);
16197 cons->c_column++; /* next column */
16198 return;
16199 }
16200 }
16202 /*===========================================================================*
16203 * scroll_screen *
16204 *===========================================================================*/
16205 PRIVATE void scroll_screen(cons, dir)
16206 register console_t *cons; /* pointer to console struct */
16207 int dir; /* SCROLL_UP or SCROLL_DOWN */
16208 {
16209 unsigned new_line, new_org, chars;
16210
16211 flush(cons);
16212 chars = scr_size - scr_width; /* one screen minus one line */
16213
16214 /* Scrolling the screen is a real nuisance due to the various incompatible
16215 * video cards. This driver supports software scrolling (Hercules?),
16216 * hardware scrolling (mono and CGA cards) and hardware scrolling without
16217 * wrapping (EGA cards). In the latter case we must make sure that
16218 * c_start <= c_org && c_org + scr_size <= c_limit
16219 * holds, because EGA doesn’t wrap around the end of video memory.
854 File: drivers/tty/console.c MINIX SOURCE CODE
16220 */
16221 if (dir == SCROLL_UP) {
16222 /* Scroll one line up in 3 ways: soft, avoid wrap, use origin. */
16223 if (softscroll) {
16224 vid_vid_copy(cons->c_start + scr_width, cons->c_start, chars);
16225 } else
16226 if (!wrap && cons->c_org + scr_size + scr_width >= cons->c_limit) {
16227 vid_vid_copy(cons->c_org + scr_width, cons->c_start, chars);
16228 cons->c_org = cons->c_start;
16229 } else {
16230 cons->c_org = (cons->c_org + scr_width) & vid_mask;
16231 }
16232 new_line = (cons->c_org + chars) & vid_mask;
16233 } else {
16234 /* Scroll one line down in 3 ways: soft, avoid wrap, use origin. */
16235 if (softscroll) {
16236 vid_vid_copy(cons->c_start, cons->c_start + scr_width, chars);
16237 } else
16238 if (!wrap && cons->c_org < cons->c_start + scr_width) {
16239 new_org = cons->c_limit - scr_size;
16240 vid_vid_copy(cons->c_org, new_org + scr_width, chars);
16241 cons->c_org = new_org;
16242 } else {
16243 cons->c_org = (cons->c_org - scr_width) & vid_mask;
16244 }
16245 new_line = cons->c_org;
16246 }
16247 /* Blank the new line at top or bottom. */
16248 blank_color = cons->c_blank;
16249 mem_vid_copy(BLANK_MEM, new_line, scr_width);
16250
16251 /* Set the new video origin. */
16252 if (cons == curcons) set_6845(VID_ORG, cons->c_org);
16253 flush(cons);
16254 }
16256 /*===========================================================================*
16257 * flush *
16258 *===========================================================================*/
16259 PRIVATE void flush(cons)
16260 register console_t *cons; /* pointer to console struct */
16261 {
16262 /* Send characters buffered in ’ramqueue’ to screen memory, check the new
16263 * cursor position, compute the new hardware cursor position and set it.
16264 */
16265 unsigned cur;
16266 tty_t *tp = cons->c_tty;
16267
16268 /* Have the characters in ’ramqueue’ transferred to the screen. */
16269 if (cons->c_rwords > 0) {
16270 mem_vid_copy(cons->c_ramqueue, cons->c_cur, cons->c_rwords);
16271 cons->c_rwords = 0;
16272
16273 /* TTY likes to know the current column and if echoing messed up. */
16274 tp->tty_position = cons->c_column;
16275 tp->tty_reprint = TRUE;
16276 }
16277
16278 /* Check and update the cursor position. */
16279 if (cons->c_column < 0) cons->c_column = 0;
MINIX SOURCE CODE File: drivers/tty/console.c 855
16280 if (cons->c_column > scr_width) cons->c_column = scr_width;
16281 if (cons->c_row < 0) cons->c_row = 0;
16282 if (cons->c_row >= scr_lines) cons->c_row = scr_lines - 1;
16283 cur = cons->c_org + cons->c_row * scr_width + cons->c_column;
16284 if (cur != cons->c_cur) {
16285 if (cons == curcons) set_6845(CURSOR, cur);
16286 cons->c_cur = cur;
16287 }
16288 }
16290 /*===========================================================================*
16291 * parse_escape *
16292 *===========================================================================*/
16293 PRIVATE void parse_escape(cons, c)
16294 register console_t *cons; /* pointer to console struct */
16295 char c; /* next character in escape sequence */
16296 {
16297 /* The following ANSI escape sequences are currently supported.
16298 * If n and/or m are omitted, they default to 1.
16299 * ESC [nA moves up n lines
16300 * ESC [nB moves down n lines
16301 * ESC [nC moves right n spaces
16302 * ESC [nD moves left n spaces
16303 * ESC [m;nH" moves cursor to (m,n)
16304 * ESC [J clears screen from cursor
16305 * ESC [K clears line from cursor
16306 * ESC [nL inserts n lines ar cursor
16307 * ESC [nM deletes n lines at cursor
16308 * ESC [nP deletes n chars at cursor
16309 * ESC [n@ inserts n chars at cursor
16310 * ESC [nm enables rendition n (0=normal, 4=bold, 5=blinking, 7=reverse)
16311 * ESC M scrolls the screen backwards if the cursor is on the top line
16312 */
16313
16314 switch (cons->c_esc_state) {
16315 case 1: /* ESC seen */
16316 cons->c_esc_intro = ’\0’;
16317 cons->c_esc_parmp = bufend(cons->c_esc_parmv);
16318 do {
16319 *--cons->c_esc_parmp = 0;
16320 } while (cons->c_esc_parmp > cons->c_esc_parmv);
16321 switch (c) {
16322 case ’[’: /* Control Sequence Introducer */
16323 cons->c_esc_intro = c;
16324 cons->c_esc_state = 2;
16325 break;
16326 case ’M’: /* Reverse Index */
16327 do_escape(cons, c);
16328 break;
16329 default:
16330 cons->c_esc_state = 0;
16331 }
16332 break;
16333
16334 case 2: /* ESC [ seen */
16335 if (c >= ’0’ && c <= ’9’) {
16336 if (cons->c_esc_parmp < bufend(cons->c_esc_parmv))
16337 *cons->c_esc_parmp = *cons->c_esc_parmp * 10 + (c-’0’);
16338 } else
16339 if (c == ’;’) {
856 File: drivers/tty/console.c MINIX SOURCE CODE
16340 if (cons->c_esc_parmp < bufend(cons->c_esc_parmv))
16341 cons->c_esc_parmp++;
16342 } else {
16343 do_escape(cons, c);
16344 }
16345 break;
16346 }
16347 }
16349 /*===========================================================================*
16350 * do_escape *
16351 *===========================================================================*/
16352 PRIVATE void do_escape(cons, c)
16353 register console_t *cons; /* pointer to console struct */
16354 char c; /* next character in escape sequence */
16355 {
16356 int value, n;
16357 unsigned src, dst, count;
16358 int *parmp;
16359
16360 /* Some of these things hack on screen RAM, so it had better be up to date */
16361 flush(cons);
16362
16363 if (cons->c_esc_intro == ’\0’) {
16364 /* Handle a sequence beginning with just ESC */
16365 switch (c) {
16366 case ’M’: /* Reverse Index */
16367 if (cons->c_row == 0) {
16368 scroll_screen(cons, SCROLL_DOWN);
16369 } else {
16370 cons->c_row--;
16371 }
16372 flush(cons);
16373 break;
16374
16375 default: break;
16376 }
16377 } else
16378 if (cons->c_esc_intro == ’[’) {
16379 /* Handle a sequence beginning with ESC [ and parameters */
16380 value = cons->c_esc_parmv[0];
16381 switch (c) {
16382 case ’A’: /* ESC [nA moves up n lines */
16383 n = (value == 0 ? 1 : value);
16384 cons->c_row -= n;
16385 flush(cons);
16386 break;
16387
16388 case ’B’: /* ESC [nB moves down n lines */
16389 n = (value == 0 ? 1 : value);
16390 cons->c_row += n;
16391 flush(cons);
16392 break;
16393
16394 case ’C’: /* ESC [nC moves right n spaces */
16395 n = (value == 0 ? 1 : value);
16396 cons->c_column += n;
16397 flush(cons);
16398 break;
16399
MINIX SOURCE CODE File: drivers/tty/console.c 857
16400 case ’D’: /* ESC [nD moves left n spaces */
16401 n = (value == 0 ? 1 : value);
16402 cons->c_column -= n;
16403 flush(cons);
16404 break;
16405
16406 case ’H’: /* ESC [m;nH" moves cursor to (m,n) */
16407 cons->c_row = cons->c_esc_parmv[0] - 1;
16408 cons->c_column = cons->c_esc_parmv[1] - 1;
16409 flush(cons);
16410 break;
16411
16412 case ’J’: /* ESC [sJ clears in display */
16413 switch (value) {
16414 case 0: /* Clear from cursor to end of screen */
16415 count = scr_size - (cons->c_cur - cons->c_org);
16416 dst = cons->c_cur;
16417 break;
16418 case 1: /* Clear from start of screen to cursor */
16419 count = cons->c_cur - cons->c_org;
16420 dst = cons->c_org;
16421 break;
16422 case 2: /* Clear entire screen */
16423 count = scr_size;
16424 dst = cons->c_org;
16425 break;
16426 default: /* Do nothing */
16427 count = 0;
16428 dst = cons->c_org;
16429 }
16430 blank_color = cons->c_blank;
16431 mem_vid_copy(BLANK_MEM, dst, count);
16432 break;
16433
16434 case ’K’: /* ESC [sK clears line from cursor */
16435 switch (value) {
16436 case 0: /* Clear from cursor to end of line */
16437 count = scr_width - cons->c_column;
16438 dst = cons->c_cur;
16439 break;
16440 case 1: /* Clear from beginning of line to cursor */
16441 count = cons->c_column;
16442 dst = cons->c_cur - cons->c_column;
16443 break;
16444 case 2: /* Clear entire line */
16445 count = scr_width;
16446 dst = cons->c_cur - cons->c_column;
16447 break;
16448 default: /* Do nothing */
16449 count = 0;
16450 dst = cons->c_cur;
16451 }
16452 blank_color = cons->c_blank;
16453 mem_vid_copy(BLANK_MEM, dst, count);
16454 break;
16455
16456 case ’L’: /* ESC [nL inserts n lines at cursor */
16457 n = value;
16458 if (n < 1) n = 1;
16459 if (n > (scr_lines - cons->c_row))
858 File: drivers/tty/console.c MINIX SOURCE CODE
16460 n = scr_lines - cons->c_row;
16461
16462 src = cons->c_org + cons->c_row * scr_width;
16463 dst = src + n * scr_width;
16464 count = (scr_lines - cons->c_row - n) * scr_width;
16465 vid_vid_copy(src, dst, count);
16466 blank_color = cons->c_blank;
16467 mem_vid_copy(BLANK_MEM, src, n * scr_width);
16468 break;
16469
16470 case ’M’: /* ESC [nM deletes n lines at cursor */
16471 n = value;
16472 if (n < 1) n = 1;
16473 if (n > (scr_lines - cons->c_row))
16474 n = scr_lines - cons->c_row;
16475
16476 dst = cons->c_org + cons->c_row * scr_width;
16477 src = dst + n * scr_width;
16478 count = (scr_lines - cons->c_row - n) * scr_width;
16479 vid_vid_copy(src, dst, count);
16480 blank_color = cons->c_blank;
16481 mem_vid_copy(BLANK_MEM, dst + count, n * scr_width);
16482 break;
16483
16484 case ’@’: /* ESC [n@ inserts n chars at cursor */
16485 n = value;
16486 if (n < 1) n = 1;
16487 if (n > (scr_width - cons->c_column))
16488 n = scr_width - cons->c_column;
16489
16490 src = cons->c_cur;
16491 dst = src + n;
16492 count = scr_width - cons->c_column - n;
16493 vid_vid_copy(src, dst, count);
16494 blank_color = cons->c_blank;
16495 mem_vid_copy(BLANK_MEM, src, n);
16496 break;
16497
16498 case ’P’: /* ESC [nP deletes n chars at cursor */
16499 n = value;
16500 if (n < 1) n = 1;
16501 if (n > (scr_width - cons->c_column))
16502 n = scr_width - cons->c_column;
16503
16504 dst = cons->c_cur;
16505 src = dst + n;
16506 count = scr_width - cons->c_column - n;
16507 vid_vid_copy(src, dst, count);
16508 blank_color = cons->c_blank;
16509 mem_vid_copy(BLANK_MEM, dst + count, n);
16510 break;
16511
16512 case ’m’: /* ESC [nm enables rendition n */
16513 for (parmp = cons->c_esc_parmv; parmp <= cons->c_esc_parmp
16514 && parmp < bufend(cons->c_esc_parmv); parmp++) {
16515 if (cons->c_reverse) {
16516 /* Unswap fg and bg colors */
16517 cons->c_attr = ((cons->c_attr & 0x7000) >> 4) |
16518 ((cons->c_attr & 0x0700) << 4) |
16519 ((cons->c_attr & 0x8800));
MINIX SOURCE CODE File: drivers/tty/console.c 859
16520 }
16521 switch (n = *parmp) {
16522 case 0: /* NORMAL */
16523 cons->c_attr = cons->c_blank = BLANK_COLOR;
16524 cons->c_reverse = FALSE;
16525 break;
16526
16527 case 1: /* BOLD */
16528 /* Set intensity bit */
16529 cons->c_attr |= 0x0800;
16530 break;
16531
16532 case 4: /* UNDERLINE */
16533 if (color) {
16534 /* Change white to cyan, i.e. lose red
16535 */
16536 cons->c_attr = (cons->c_attr & 0xBBFF);
16537 } else {
16538 /* Set underline attribute */
16539 cons->c_attr = (cons->c_attr & 0x99FF);
16540 }
16541 break;
16542
16543 case 5: /* BLINKING */
16544 /* Set the blink bit */
16545 cons->c_attr |= 0x8000;
16546 break;
16547
16548 case 7: /* REVERSE */
16549 cons->c_reverse = TRUE;
16550 break;
16551
16552 default: /* COLOR */
16553 if (n == 39) n = 37; /* set default color */
16554 if (n == 49) n = 40;
16555
16556 if (!color) {
16557 /* Don’t mess up a monochrome screen */
16558 } else
16559 if (30 <= n && n <= 37) {
16560 /* Foreground color */
16561 cons->c_attr =
16562 (cons->c_attr & 0xF8FF) |
16563 (ansi_colors[(n - 30)] << 8);
16564 cons->c_blank =
16565 (cons->c_blank & 0xF8FF) |
16566 (ansi_colors[(n - 30)] << 8);
16567 } else
16568 if (40 <= n && n <= 47) {
16569 /* Background color */
16570 cons->c_attr =
16571 (cons->c_attr & 0x8FFF) |
16572 (ansi_colors[(n - 40)] << 12);
16573 cons->c_blank =
16574 (cons->c_blank & 0x8FFF) |
16575 (ansi_colors[(n - 40)] << 12);
16576 }
16577 }
16578 if (cons->c_reverse) {
16579 /* Swap fg and bg colors */
860 File: drivers/tty/console.c MINIX SOURCE CODE
16580 cons->c_attr = ((cons->c_attr & 0x7000) >> 4) |
16581 ((cons->c_attr & 0x0700) << 4) |
16582 ((cons->c_attr & 0x8800));
16583 }
16584 }
16585 break;
16586 }
16587 }
16588 cons->c_esc_state = 0;
16589 }
16591 /*===========================================================================*
16592 * set_6845 *
16593 *===========================================================================*/
16594 PRIVATE void set_6845(reg, val)
16595 int reg; /* which register pair to set */
16596 unsigned val; /* 16-bit value to set it to */
16597 {
16598 /* Set a register pair inside the 6845.
16599 * Registers 12-13 tell the 6845 where in video ram to start
16600 * Registers 14-15 tell the 6845 where to put the cursor
16601 */
16602 pvb_pair_t char_out[4];
16603 pv_set(char_out[0], vid_port + INDEX, reg); /* set index register */
16604 pv_set(char_out[1], vid_port + DATA, (val>>8) & BYTE); /* high byte */
16605 pv_set(char_out[2], vid_port + INDEX, reg + 1); /* again */
16606 pv_set(char_out[3], vid_port + DATA, val&BYTE); /* low byte */
16607 sys_voutb(char_out, 4); /* do actual output */
16608 }
16610 /*===========================================================================*
16611 * get_6845 *
16612 *===========================================================================*/
16613 PRIVATE void get_6845(reg, val)
16614 int reg; /* which register pair to set */
16615 unsigned *val; /* 16-bit value to set it to */
16616 {
16617 char v1, v2;
16618 /* Get a register pair inside the 6845. */
16619 sys_outb(vid_port + INDEX, reg);
16620 sys_inb(vid_port + DATA, &v1);
16621 sys_outb(vid_port + INDEX, reg+1);
16622 sys_inb(vid_port + DATA, &v2);
16623 *val = (v1 << 8) | v2;
16624 }
16626 /*===========================================================================*
16627 * beep *
16628 *===========================================================================*/
16629 PRIVATE void beep()
16630 {
16631 /* Making a beeping sound on the speaker (output for CRTL-G).
16632 * This routine works by turning on the bits 0 and 1 in port B of the 8255
16633 * chip that drive the speaker.
16634 */
16635 static timer_t tmr_stop_beep;
16636 pvb_pair_t char_out[3];
16637 clock_t now;
16638 int port_b_val, s;
16639
MINIX SOURCE CODE File: drivers/tty/console.c 861
16640 /* Fetch current time in advance to prevent beeping delay. */
16641 if ((s=getuptime(&now)) != OK)
16642 panic("TTY","Console couldn’t get clock’s uptime.", s);
16643 if (!beeping) {
16644 /* Set timer channel 2, square wave, with given frequency. */
16645 pv_set(char_out[0], TIMER_MODE, 0xB6);
16646 pv_set(char_out[1], TIMER2, (BEEP_FREQ >> 0) & BYTE);
16647 pv_set(char_out[2], TIMER2, (BEEP_FREQ >> 8) & BYTE);
16648 if (sys_voutb(char_out, 3)==OK) {
16649 if (sys_inb(PORT_B, &port_b_val)==OK &&
16650 sys_outb(PORT_B, (port_b_val|3))==OK)
16651 beeping = TRUE;
16652 }
16653 }
16654 /* Add a timer to the timers list. Possibly reschedule the alarm. */
16655 tmrs_settimer(&tty_timers, &tmr_stop_beep, now+B_TIME, stop_beep, NULL);
16656 if (tty_timers->tmr_exp_time != tty_next_timeout) {
16657 tty_next_timeout = tty_timers->tmr_exp_time;
16658 if ((s=sys_setalarm(tty_next_timeout, 1)) != OK)
16659 panic("TTY","Console couldn’t set alarm.", s);
16660 }
16661 }
16663 /*===========================================================================*
16664 * stop_beep *
16665 *===========================================================================*/
16666 PRIVATE void stop_beep(tmrp)
16667 timer_t *tmrp;
16668 {
16669 /* Turn off the beeper by turning off bits 0 and 1 in PORT_B. */
16670 int port_b_val;
16671 if (sys_inb(PORT_B, &port_b_val)==OK &&
16672 sys_outb(PORT_B, (port_b_val & ˜3))==OK)
16673 beeping = FALSE;
16674 }
16676 /*===========================================================================*
16677 * scr_init *
16678 *===========================================================================*/
16679 PUBLIC void scr_init(tp)
16680 tty_t *tp;
16681 {
16682 /* Initialize the screen driver. */
16683 console_t *cons;
16684 phys_bytes vid_base;
16685 u16_t bios_columns, bios_crtbase, bios_fontlines;
16686 u8_t bios_rows;
16687 int line;
16688 int s;
16689 static int vdu_initialized = 0;
16690 unsigned page_size;
16691
16692 /* Associate console and TTY. */
16693 line = tp - &tty_table[0];
16694 if (line >= nr_cons) return;
16695 cons = &cons_table[line];
16696 cons->c_tty = tp;
16697 tp->tty_priv = cons;
16698
16699 /* Initialize the keyboard driver. */
862 File: drivers/tty/console.c MINIX SOURCE CODE
16700 kb_init(tp);
16701
16702 /* Fill in TTY function hooks. */
16703 tp->tty_devwrite = cons_write;
16704 tp->tty_echo = cons_echo;
16705 tp->tty_ioctl = cons_ioctl;
16706
16707 /* Get the BIOS parameters that describe the VDU. */
16708 if (! vdu_initialized++) {
16709
16710 /* How about error checking? What to do on failure??? */
16711 s=sys_vircopy(SELF, BIOS_SEG, (vir_bytes) VDU_SCREEN_COLS_ADDR,
16712 SELF, D, (vir_bytes) &bios_columns, VDU_SCREEN_COLS_SIZE);
16713 s=sys_vircopy(SELF, BIOS_SEG, (vir_bytes) VDU_CRT_BASE_ADDR,
16714 SELF, D, (vir_bytes) &bios_crtbase, VDU_CRT_BASE_SIZE);
16715 s=sys_vircopy(SELF, BIOS_SEG, (vir_bytes) VDU_SCREEN_ROWS_ADDR,
16716 SELF, D, (vir_bytes) &bios_rows, VDU_SCREEN_ROWS_SIZE);
16717 s=sys_vircopy(SELF, BIOS_SEG, (vir_bytes) VDU_FONTLINES_ADDR,
16718 SELF, D, (vir_bytes) &bios_fontlines, VDU_FONTLINES_SIZE);
16719
16720 vid_port = bios_crtbase;
16721 scr_width = bios_columns;
16722 font_lines = bios_fontlines;
16723 scr_lines = machine.vdu_ega ? bios_rows+1 : 25;
16724
16725 if (color) {
16726 vid_base = COLOR_BASE;
16727 vid_size = COLOR_SIZE;
16728 } else {
16729 vid_base = MONO_BASE;
16730 vid_size = MONO_SIZE;
16731 }
16732 if (machine.vdu_ega) vid_size = EGA_SIZE;
16733 wrap = ! machine.vdu_ega;
16734
16735 s = sys_segctl(&vid_index, &vid_seg, &vid_off, vid_base, vid_size);
16736
16737 vid_size >>= 1; /* word count */
16738 vid_mask = vid_size - 1;
16739
16740 /* Size of the screen (number of displayed characters.) */
16741 scr_size = scr_lines * scr_width;
16742
16743 /* There can be as many consoles as video memory allows. */
16744 nr_cons = vid_size / scr_size;
16745 if (nr_cons > NR_CONS) nr_cons = NR_CONS;
16746 if (nr_cons > 1) wrap = 0;
16747 page_size = vid_size / nr_cons;
16748 }
16749
16750 cons->c_start = line * page_size;
16751 cons->c_limit = cons->c_start + page_size;
16752 cons->c_cur = cons->c_org = cons->c_start;
16753 cons->c_attr = cons->c_blank = BLANK_COLOR;
16754
16755 if (line != 0) {
16756 /* Clear the non-console vtys. */
16757 blank_color = BLANK_COLOR;
16758 mem_vid_copy(BLANK_MEM, cons->c_start, scr_size);
16759 } else {
MINIX SOURCE CODE File: drivers/tty/console.c 863
16760 int i, n;
16761 /* Set the cursor of the console vty at the bottom. c_cur
16762 * is updated automatically later.
16763 */
16764 scroll_screen(cons, SCROLL_UP);
16765 cons->c_row = scr_lines - 1;
16766 cons->c_column = 0;
16767 }
16768 select_console(0);
16769 cons_ioctl(tp, 0);
16770 }
16772 /*===========================================================================*
16773 * kputc *
16774 *===========================================================================*/
16775 PUBLIC void kputc(c)
16776 int c;
16777 {
16778 putk(c);
16779 }
16781 /*===========================================================================*
16782 * do_new_kmess *
16783 *===========================================================================*/
16784 PUBLIC void do_new_kmess(m)
16785 message *m;
16786 {
16787 /* Notification for a new kernel message. */
16788 struct kmessages kmess; /* kmessages structure */
16789 static int prev_next = 0; /* previous next seen */
16790 int size, next;
16791 int bytes;
16792 int r;
16793
16794 /* Try to get a fresh copy of the buffer with kernel messages. */
16795 sys_getkmessages(&kmess);
16796
16797 /* Print only the new part. Determine how many new bytes there are with
16798 * help of the current and previous ’next’ index. Note that the kernel
16799 * buffer is circular. This works fine if less then KMESS_BUF_SIZE bytes
16800 * is new data; else we miss % KMESS_BUF_SIZE here.
16801 * Check for size being positive, the buffer might as well be emptied!
16802 */
16803 if (kmess.km_size > 0) {
16804 bytes = ((kmess.km_next + KMESS_BUF_SIZE) - prev_next) % KMESS_BUF_SIZE;
16805 r=prev_next; /* start at previous old */
16806 while (bytes > 0) {
16807 putk( kmess.km_buf[(r%KMESS_BUF_SIZE)] );
16808 bytes --;
16809 r ++;
16810 }
16811 putk(0); /* terminate to flush output */
16812 }
16813
16814 /* Almost done, store ’next’ so that we can determine what part of the
16815 * kernel messages buffer to print next time a notification arrives.
16816 */
16817 prev_next = kmess.km_next;
16818 }
864 File: drivers/tty/console.c MINIX SOURCE CODE
16820 /*===========================================================================*
16821 * do_diagnostics *
16822 *===========================================================================*/
16823 PUBLIC void do_diagnostics(m_ptr)
16824 message *m_ptr; /* pointer to request message */
16825 {
16826 /* Print a string for a server. */
16827 char c;
16828 vir_bytes src;
16829 int count;
16830 int result = OK;
16831 int proc_nr = m_ptr->DIAG_PROC_NR;
16832 if (proc_nr == SELF) proc_nr = m_ptr->m_source;
16833
16834 src = (vir_bytes) m_ptr->DIAG_PRINT_BUF;
16835 for (count = m_ptr->DIAG_BUF_COUNT; count > 0; count--) {
16836 if (sys_vircopy(proc_nr, D, src++, SELF, D, (vir_bytes) &c, 1) != OK) {
16837 result = EFAULT;
16838 break;
16839 }
16840 putk(c);
16841 }
16842 putk(0); /* always terminate, even with EFAULT */
16843 m_ptr->m_type = result;
16844 send(m_ptr->m_source, m_ptr);
16845 }
16847 /*===========================================================================*
16848 * putk *
16849 *===========================================================================*/
16850 PRIVATE void putk(c)
16851 int c; /* character to print */
16852 {
16853 /* This procedure is used by the version of printf() that is linked with
16854 * the TTY driver. The one in the library sends a message to FS, which is
16855 * not what is needed for printing within the TTY. This version just queues
16856 * the character and starts the output.
16857 */
16858 if (c != 0) {
16859 if (c == ’\n’) putk(’\r’);
16860 out_char(&cons_table[0], (int) c);
16861 } else {
16862 flush(&cons_table[0]);
16863 }
16864 }
16866 /*===========================================================================*
16867 * toggle_scroll *
16868 *===========================================================================*/
16869 PUBLIC void toggle_scroll()
16870 {
16871 /* Toggle between hardware and software scroll. */
16872
16873 cons_org0();
16874 softscroll = !softscroll;
16875 printf("%sware scrolling enabled.\n", softscroll ? "Soft" : "Hard");
16876 }
MINIX SOURCE CODE File: drivers/tty/console.c 865
16878 /*===========================================================================*
16879 * cons_stop *
16880 *===========================================================================*/
16881 PUBLIC void cons_stop()
16882 {
16883 /* Prepare for halt or reboot. */
16884 cons_org0();
16885 softscroll = 1;
16886 select_console(0);
16887 cons_table[0].c_attr = cons_table[0].c_blank = BLANK_COLOR;
16888 }
16890 /*===========================================================================*
16891 * cons_org0 *
16892 *===========================================================================*/
16893 PRIVATE void cons_org0()
16894 {
16895 /* Scroll video memory back to put the origin at 0. */
16896 int cons_line;
16897 console_t *cons;
16898 unsigned n;
16899
16900 for (cons_line = 0; cons_line < nr_cons; cons_line++) {
16901 cons = &cons_table[cons_line];
16902 while (cons->c_org > cons->c_start) {
16903 n = vid_size - scr_size; /* amount of unused memory */
16904 if (n > cons->c_org - cons->c_start)
16905 n = cons->c_org - cons->c_start;
16906 vid_vid_copy(cons->c_org, cons->c_org - n, scr_size);
16907 cons->c_org -= n;
16908 }
16909 flush(cons);
16910 }
16911 select_console(ccurrent);
16912 }
16914 /*===========================================================================*
16915 * select_console *
16916 *===========================================================================*/
16917 PUBLIC void select_console(int cons_line)
16918 {
16919 /* Set the current console to console number ’cons_line’. */
16920
16921 if (cons_line < 0 || cons_line >= nr_cons) return;
16922 ccurrent = cons_line;
16923 curcons = &cons_table[cons_line];
16924 set_6845(VID_ORG, curcons->c_org);
16925 set_6845(CURSOR, curcons->c_cur);
16926 }
16928 /*===========================================================================*
16929 * con_loadfont *
16930 *===========================================================================*/
16931 PUBLIC int con_loadfont(m)
16932 message *m;
16933 {
16934 /* Load a font into the EGA or VGA adapter. */
16935 int result;
16936 static struct sequence seq1[7] = {
16937 { GA_SEQUENCER_INDEX, 0x00, 0x01 },
866 File: drivers/tty/console.c MINIX SOURCE CODE
16938 { GA_SEQUENCER_INDEX, 0x02, 0x04 },
16939 { GA_SEQUENCER_INDEX, 0x04, 0x07 },
16940 { GA_SEQUENCER_INDEX, 0x00, 0x03 },
16941 { GA_GRAPHICS_INDEX, 0x04, 0x02 },
16942 { GA_GRAPHICS_INDEX, 0x05, 0x00 },
16943 { GA_GRAPHICS_INDEX, 0x06, 0x00 },
16944 };
16945 static struct sequence seq2[7] = {
16946 { GA_SEQUENCER_INDEX, 0x00, 0x01 },
16947 { GA_SEQUENCER_INDEX, 0x02, 0x03 },
16948 { GA_SEQUENCER_INDEX, 0x04, 0x03 },
16949 { GA_SEQUENCER_INDEX, 0x00, 0x03 },
16950 { GA_GRAPHICS_INDEX, 0x04, 0x00 },
16951 { GA_GRAPHICS_INDEX, 0x05, 0x10 },
16952 { GA_GRAPHICS_INDEX, 0x06, 0 },
16953 };
16954
16955 seq2[6].value= color ? 0x0E : 0x0A;
16956
16957 if (!machine.vdu_ega) return(ENOTTY);
16958 result = ga_program(seq1); /* bring font memory into view */
16959
16960 result = sys_physcopy(m->PROC_NR, D, (vir_bytes) m->ADDRESS,
16961 NONE, PHYS_SEG, (phys_bytes) GA_VIDEO_ADDRESS, (phys_bytes)GA_FONT_SIZE);
16962
16963 result = ga_program(seq2); /* restore */
16964
16965 return(result);
16966 }
16968 /*===========================================================================*
16969 * ga_program *
16970 *===========================================================================*/
16971 PRIVATE int ga_program(seq)
16972 struct sequence *seq;
16973 {
16974 pvb_pair_t char_out[14];
16975 int i;
16976 for (i=0; i<7; i++) {
16977 pv_set(char_out[2*i], seq->index, seq->port);
16978 pv_set(char_out[2*i+1], seq->index+1, seq->value);
16979 seq++;
16980 }
16981 return sys_voutb(char_out, 14);
16982 }
16984 /*===========================================================================*
16985 * cons_ioctl *
16986 *===========================================================================*/
16987 PRIVATE int cons_ioctl(tp, try)
16988 tty_t *tp;
16989 int try;
16990 {
16991 /* Set the screen dimensions. */
16992
16993 tp->tty_winsize.ws_row= scr_lines;
16994 tp->tty_winsize.ws_col= scr_width;
16995 tp->tty_winsize.ws_xpixel= scr_width * 8;
16996 tp->tty_winsize.ws_ypixel= scr_lines * font_lines;
16997 }