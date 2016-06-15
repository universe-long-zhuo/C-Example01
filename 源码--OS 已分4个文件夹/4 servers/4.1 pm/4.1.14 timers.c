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