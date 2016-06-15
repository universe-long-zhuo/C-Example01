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