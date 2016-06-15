MINIX SOURCE CODE File: servers/pm/forkexit.c 881
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/pm/forkexit.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
18400 /* This file deals with creating processes (via FORK) and deleting them (via
18401 * EXIT/WAIT). When a process forks, a new slot in the ’mproc’ table is
18402 * allocated for it, and a copy of the parent’s core image is made for the
18403 * child. Then the kernel and file system are informed. A process is removed
18404 * from the ’mproc’ table when two events have occurred: (1) it has exited or
18405 * been killed by a signal, and (2) the parent has done a WAIT. If the process
18406 * exits first, it continues to occupy a slot until the parent does a WAIT.
18407 *
18408 * The entry points into this file are:
18409 * do_fork: perform the FORK system call
18410 * do_pm_exit: perform the EXIT system call (by calling pm_exit())
18411 * pm_exit: actually do the exiting
18412 * do_wait: perform the WAITPID or WAIT system call
18413 */
18414
18415 #include "pm.h"
18416 #include <sys/wait.h>
18417 #include <minix/callnr.h>
18418 #include <minix/com.h>
18419 #include <signal.h>
18420 #include "mproc.h"
18421 #include "param.h"
18422
18423 #define LAST_FEW 2 /* last few slots reserved for superuser */
18424
18425 FORWARD _PROTOTYPE (void cleanup, (register struct mproc *child) );
18426
18427 /*===========================================================================*
18428 * do_fork *
18429 *===========================================================================*/
18430 PUBLIC int do_fork()
18431 {
18432 /* The process pointed to by ’mp’ has forked. Create a child process. */
18433 register struct mproc *rmp; /* pointer to parent */
18434 register struct mproc *rmc; /* pointer to child */
18435 int child_nr, s;
18436 phys_clicks prog_clicks, child_base;
18437 phys_bytes prog_bytes, parent_abs, child_abs; /* Intel only */
18438 pid_t new_pid;
18439
18440 /* If tables might fill up during FORK, don’t even start since recovery half
18441 * way through is such a nuisance.
18442 */
18443 rmp = mp;
18444 if ((procs_in_use == NR_PROCS) ||
18445 (procs_in_use >= NR_PROCS-LAST_FEW && rmp->mp_effuid != 0))
18446 {
18447 printf("PM: warning, process table is full!\n");
18448 return(EAGAIN);
18449 }
18450
18451 /* Determine how much memory to allocate. Only the data and stack need to
18452 * be copied, because the text segment is either shared or of zero length.
18453 */
18454 prog_clicks = (phys_clicks) rmp->mp_seg[S].mem_len;
882 File: servers/pm/forkexit.c MINIX SOURCE CODE
18455 prog_clicks += (rmp->mp_seg[S].mem_vir - rmp->mp_seg[D].mem_vir);
18456 prog_bytes = (phys_bytes) prog_clicks << CLICK_SHIFT;
18457 if ( (child_base = alloc_mem(prog_clicks)) == NO_MEM) return(ENOMEM);
18458
18459 /* Create a copy of the parent’s core image for the child. */
18460 child_abs = (phys_bytes) child_base << CLICK_SHIFT;
18461 parent_abs = (phys_bytes) rmp->mp_seg[D].mem_phys << CLICK_SHIFT;
18462 s = sys_abscopy(parent_abs, child_abs, prog_bytes);
18463 if (s < 0) panic(__FILE__,"do_fork can’t copy", s);
18464
18465 /* Find a slot in ’mproc’ for the child process. A slot must exist. */
18466 for (rmc = &mproc[0]; rmc < &mproc[NR_PROCS]; rmc++)
18467 if ( (rmc->mp_flags & IN_USE) == 0) break;
18468
18469 /* Set up the child and its memory map; copy its ’mproc’ slot from parent. */
18470 child_nr = (int)(rmc - mproc); /* slot number of the child */
18471 procs_in_use++;
18472 *rmc = *rmp; /* copy parent’s process slot to child’s */
18473 rmc->mp_parent = who; /* record child’s parent */
18474 /* inherit only these flags */
18475 rmc->mp_flags &= (IN_USE|SEPARATE|PRIV_PROC|DONT_SWAP);
18476 rmc->mp_child_utime = 0; /* reset administration */
18477 rmc->mp_child_stime = 0; /* reset administration */
18478
18479 /* A separate I&D child keeps the parents text segment. The data and stack
18480 * segments must refer to the new copy.
18481 */
18482 if (!(rmc->mp_flags & SEPARATE)) rmc->mp_seg[T].mem_phys = child_base;
18483 rmc->mp_seg[D].mem_phys = child_base;
18484 rmc->mp_seg[S].mem_phys = rmc->mp_seg[D].mem_phys +
18485 (rmp->mp_seg[S].mem_vir - rmp->mp_seg[D].mem_vir);
18486 rmc->mp_exitstatus = 0;
18487 rmc->mp_sigstatus = 0;
18488
18489 /* Find a free pid for the child and put it in the table. */
18490 new_pid = get_free_pid();
18491 rmc->mp_pid = new_pid; /* assign pid to child */
18492
18493 /* Tell kernel and file system about the (now successful) FORK. */
18494 sys_fork(who, child_nr);
18495 tell_fs(FORK, who, child_nr, rmc->mp_pid);
18496
18497 /* Report child’s memory map to kernel. */
18498 sys_newmap(child_nr, rmc->mp_seg);
18499
18500 /* Reply to child to wake it up. */
18501 setreply(child_nr, 0); /* only parent gets details */
18502 rmp->mp_reply.procnr = child_nr; /* child’s process number */
18503 return(new_pid); /* child’s pid */
18504 }
18506 /*===========================================================================*
18507 * do_pm_exit *
18508 *===========================================================================*/
18509 PUBLIC int do_pm_exit()
18510 {
18511 /* Perform the exit(status) system call. The real work is done by pm_exit(),
18512 * which is also called when a process is killed by a signal.
18513 */
18514 pm_exit(mp, m_in.status);
MINIX SOURCE CODE File: servers/pm/forkexit.c 883
18515 return(SUSPEND); /* can’t communicate from beyond the grave */
18516 }
18518 /*===========================================================================*
18519 * pm_exit *
18520 *===========================================================================*/
18521 PUBLIC void pm_exit(rmp, exit_status)
18522 register struct mproc *rmp; /* pointer to the process to be terminated */
18523 int exit_status; /* the process’ exit status (for parent) */
18524 {
18525 /* A process is done. Release most of the process’ possessions. If its
18526 * parent is waiting, release the rest, else keep the process slot and
18527 * become a zombie.
18528 */
18529 register int proc_nr;
18530 int parent_waiting, right_child;
18531 pid_t pidarg, procgrp;
18532 struct mproc *p_mp;
18533 clock_t t[5];
18534
18535 proc_nr = (int) (rmp - mproc); /* get process slot number */
18536
18537 /* Remember a session leader’s process group. */
18538 procgrp = (rmp->mp_pid == mp->mp_procgrp) ? mp->mp_procgrp : 0;
18539
18540 /* If the exited process has a timer pending, kill it. */
18541 if (rmp->mp_flags & ALARM_ON) set_alarm(proc_nr, (unsigned) 0);
18542
18543 /* Do accounting: fetch usage times and accumulate at parent. */
18544 sys_times(proc_nr, t);
18545 p_mp = &mproc[rmp->mp_parent]; /* process’ parent */
18546 p_mp->mp_child_utime += t[0] + rmp->mp_child_utime; /* add user time */
18547 p_mp->mp_child_stime += t[1] + rmp->mp_child_stime; /* add system time */
18548
18549 /* Tell the kernel and FS that the process is no longer runnable. */
18550 tell_fs(EXIT, proc_nr, 0, 0); /* file system can free the proc slot */
18551 sys_exit(proc_nr);
18552
18553 /* Pending reply messages for the dead process cannot be delivered. */
18554 rmp->mp_flags &= ˜REPLY;
18555
18556 /* Release the memory occupied by the child. */
18557 if (find_share(rmp, rmp->mp_ino, rmp->mp_dev, rmp->mp_ctime) == NULL) {
18558 /* No other process shares the text segment, so free it. */
18559 free_mem(rmp->mp_seg[T].mem_phys, rmp->mp_seg[T].mem_len);
18560 }
18561 /* Free the data and stack segments. */
18562 free_mem(rmp->mp_seg[D].mem_phys,
18563 rmp->mp_seg[S].mem_vir
18564 + rmp->mp_seg[S].mem_len - rmp->mp_seg[D].mem_vir);
18565
18566 /* The process slot can only be freed if the parent has done a WAIT. */
18567 rmp->mp_exitstatus = (char) exit_status;
18568
18569 pidarg = p_mp->mp_wpid; /* who’s being waited for? */
18570 parent_waiting = p_mp->mp_flags & WAITING;
18571 right_child = /* child meets one of the 3 tests? */
18572 (pidarg == -1 || pidarg == rmp->mp_pid || -pidarg == rmp->mp_procgrp);
18573
18574 if (parent_waiting && right_child) {
884 File: servers/pm/forkexit.c MINIX SOURCE CODE
18575 cleanup(rmp); /* tell parent and release child slot */
18576 } else {
18577 rmp->mp_flags = IN_USE|ZOMBIE; /* parent not waiting, zombify child */
18578 sig_proc(p_mp, SIGCHLD); /* send parent a "child died" signal */
18579 }
18580
18581 /* If the process has children, disinherit them. INIT is the new parent. */
18582 for (rmp = &mproc[0]; rmp < &mproc[NR_PROCS]; rmp++) {
18583 if (rmp->mp_flags & IN_USE && rmp->mp_parent == proc_nr) {
18584 /* ’rmp’ now points to a child to be disinherited. */
18585 rmp->mp_parent = INIT_PROC_NR;
18586 parent_waiting = mproc[INIT_PROC_NR].mp_flags & WAITING;
18587 if (parent_waiting && (rmp->mp_flags & ZOMBIE)) cleanup(rmp);
18588 }
18589 }
18590
18591 /* Send a hangup to the process’ process group if it was a session leader. */
18592 if (procgrp != 0) check_sig(-procgrp, SIGHUP);
18593 }
18595 /*===========================================================================*
18596 * do_waitpid *
18597 *===========================================================================*/
18598 PUBLIC int do_waitpid()
18599 {
18600 /* A process wants to wait for a child to terminate. If a child is already
18601 * waiting, go clean it up and let this WAIT call terminate. Otherwise,
18602 * really wait.
18603 * A process calling WAIT never gets a reply in the usual way at the end
18604 * of the main loop (unless WNOHANG is set or no qualifying child exists).
18605 * If a child has already exited, the routine cleanup() sends the reply
18606 * to awaken the caller.
18607 * Both WAIT and WAITPID are handled by this code.
18608 */
18609 register struct mproc *rp;
18610 int pidarg, options, children;
18611
18612 /* Set internal variables, depending on whether this is WAIT or WAITPID. */
18613 pidarg = (call_nr == WAIT ? -1 : m_in.pid); /* 1st param of waitpid */
18614 options = (call_nr == WAIT ? 0 : m_in.sig_nr); /* 3rd param of waitpid */
18615 if (pidarg == 0) pidarg = -mp->mp_procgrp; /* pidarg < 0 ==> proc grp */
18616
18617 /* Is there a child waiting to be collected? At this point, pidarg != 0:
18618 * pidarg > 0 means pidarg is pid of a specific process to wait for
18619 * pidarg == -1 means wait for any child
18620 * pidarg < -1 means wait for any child whose process group = -pidarg
18621 */
18622 children = 0;
18623 for (rp = &mproc[0]; rp < &mproc[NR_PROCS]; rp++) {
18624 if ( (rp->mp_flags & IN_USE) && rp->mp_parent == who) {
18625 /* The value of pidarg determines which children qualify. */
18626 if (pidarg > 0 && pidarg != rp->mp_pid) continue;
18627 if (pidarg < -1 && -pidarg != rp->mp_procgrp) continue;
18628
18629 children++; /* this child is acceptable */
18630 if (rp->mp_flags & ZOMBIE) {
18631 /* This child meets the pid test and has exited. */
18632 cleanup(rp); /* this child has already exited */
18633 return(SUSPEND);
18634 }
MINIX SOURCE CODE File: servers/pm/forkexit.c 885
18635 if ((rp->mp_flags & STOPPED) && rp->mp_sigstatus) {
18636 /* This child meets the pid test and is being traced.*/
18637 mp->mp_reply.reply_res2 = 0177|(rp->mp_sigstatus << 8);
18638 rp->mp_sigstatus = 0;
18639 return(rp->mp_pid);
18640 }
18641 }
18642 }
18643
18644 /* No qualifying child has exited. Wait for one, unless none exists. */
18645 if (children > 0) {
18646 /* At least 1 child meets the pid test exists, but has not exited. */
18647 if (options & WNOHANG) return(0); /* parent does not want to wait */
18648 mp->mp_flags |= WAITING; /* parent wants to wait */
18649 mp->mp_wpid = (pid_t) pidarg; /* save pid for later */
18650 return(SUSPEND); /* do not reply, let it wait */
18651 } else {
18652 /* No child even meets the pid test. Return error immediately. */
18653 return(ECHILD); /* no - parent has no children */
18654 }
18655 }
18657 /*===========================================================================*
18658 * cleanup *
18659 *===========================================================================*/
18660 PRIVATE void cleanup(child)
18661 register struct mproc *child; /* tells which process is exiting */
18662 {
18663 /* Finish off the exit of a process. The process has exited or been killed
18664 * by a signal, and its parent is waiting.
18665 */
18666 struct mproc *parent = &mproc[child->mp_parent];
18667 int exitstatus;
18668
18669 /* Wake up the parent by sending the reply message. */
18670 exitstatus = (child->mp_exitstatus << 8) | (child->mp_sigstatus & 0377);
18671 parent->mp_reply.reply_res2 = exitstatus;
18672 setreply(child->mp_parent, child->mp_pid);
18673 parent->mp_flags &= ˜WAITING; /* parent no longer waiting */
18674
18675 /* Release the process table entry and reinitialize some field. */
18676 child->mp_pid = 0;
18677 child->mp_flags = 0;
18678 child->mp_child_utime = 0;
18679 child->mp_child_stime = 0;
18680 procs_in_use--;
18681 }