912 File: servers/pm/getset.c MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/pm/getset.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
20400 /* This file handles the 4 system calls that get and set uids and gids.
20401 * It also handles getpid(), setsid(), and getpgrp(). The code for each
20402 * one is so tiny that it hardly seemed worthwhile to make each a separate
20403 * function.
20404 */
20405
20406 #include "pm.h"
20407 #include <minix/callnr.h>
20408 #include <signal.h>
20409 #include "mproc.h"
20410 #include "param.h"
20411
20412 /*===========================================================================*
20413 * do_getset *
20414 *===========================================================================*/
20415 PUBLIC int do_getset()
20416 {
20417 /* Handle GETUID, GETGID, GETPID, GETPGRP, SETUID, SETGID, SETSID. The four
20418 * GETs and SETSID return their primary results in ’r’. GETUID, GETGID, and
20419 * GETPID also return secondary results (the effective IDs, or the parent
20420 * process ID) in ’reply_res2’, which is returned to the user.
20421 */
20422
20423 register struct mproc *rmp = mp;
20424 register int r;
20425
20426 switch(call_nr) {
20427 case GETUID:
20428 r = rmp->mp_realuid;
20429 rmp->mp_reply.reply_res2 = rmp->mp_effuid;
20430 break;
20431
20432 case GETGID:
20433 r = rmp->mp_realgid;
20434 rmp->mp_reply.reply_res2 = rmp->mp_effgid;
20435 break;
20436
20437 case GETPID:
20438 r = mproc[who].mp_pid;
20439 rmp->mp_reply.reply_res2 = mproc[rmp->mp_parent].mp_pid;
20440 break;
20441
20442 case SETUID:
20443 if (rmp->mp_realuid != (uid_t) m_in.usr_id &&
20444 rmp->mp_effuid != SUPER_USER)
20445 return(EPERM);
20446 rmp->mp_realuid = (uid_t) m_in.usr_id;
20447 rmp->mp_effuid = (uid_t) m_in.usr_id;
20448 tell_fs(SETUID, who, rmp->mp_realuid, rmp->mp_effuid);
20449 r = OK;
20450 break;
20451
20452 case SETGID:
20453 if (rmp->mp_realgid != (gid_t) m_in.grp_id &&
20454 rmp->mp_effuid != SUPER_USER)
MINIX SOURCE CODE File: servers/pm/getset.c 913
20455 return(EPERM);
20456 rmp->mp_realgid = (gid_t) m_in.grp_id;
20457 rmp->mp_effgid = (gid_t) m_in.grp_id;
20458 tell_fs(SETGID, who, rmp->mp_realgid, rmp->mp_effgid);
20459 r = OK;
20460 break;
20461
20462 case SETSID:
20463 if (rmp->mp_procgrp == rmp->mp_pid) return(EPERM);
20464 rmp->mp_procgrp = rmp->mp_pid;
20465 tell_fs(SETSID, who, 0, 0);
20466 /* fall through */
20467
20468 case GETPGRP:
20469 r = rmp->mp_procgrp;
20470 break;
20471
20472 default:
20473 r = EINVAL;
20474 break;
20475 }
20476 return(r);
20477 }