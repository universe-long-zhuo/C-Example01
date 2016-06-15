++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/pm/misc.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
20500 /* Miscellaneous system calls. Author: Kees J. Bot
20501 * 31 Mar 2000
20502 * The entry points into this file are:
20503 * do_reboot: kill all processes, then reboot system
20504 * do_svrctl: process manager control
20505 * do_getsysinfo: request copy of PM data structure (Jorrit N. Herder)
20506 * do_getprocnr: lookup process slot number (Jorrit N. Herder)
20507 * do_memalloc: allocate a chunk of memory (Jorrit N. Herder)
20508 * do_memfree: deallocate a chunk of memory (Jorrit N. Herder)
20509 * do_getsetpriority: get/set process priority
20510 */
20511
20512 #include "pm.h"
20513 #include <minix/callnr.h>
20514 #include <signal.h>
20515 #include <sys/svrctl.h>
20516 #include <sys/resource.h>
20517 #include <minix/com.h>
20518 #include <string.h>
20519 #include "mproc.h"
20520 #include "param.h"
20521
20522 /*===========================================================================*
20523 * do_allocmem *
20524 *===========================================================================*/
20525 PUBLIC int do_allocmem()
20526 {
20527 vir_clicks mem_clicks;
20528 phys_clicks mem_base;
20529
914 File: servers/pm/misc.c MINIX SOURCE CODE
20530 mem_clicks = (m_in.memsize + CLICK_SIZE -1 ) >> CLICK_SHIFT;
20531 mem_base = alloc_mem(mem_clicks);
20532 if (mem_base == NO_MEM) return(ENOMEM);
20533 mp->mp_reply.membase = (phys_bytes) (mem_base << CLICK_SHIFT);
20534 return(OK);
20535 }
20537 /*===========================================================================*
20538 * do_freemem *
20539 *===========================================================================*/
20540 PUBLIC int do_freemem()
20541 {
20542 vir_clicks mem_clicks;
20543 phys_clicks mem_base;
20544
20545 mem_clicks = (m_in.memsize + CLICK_SIZE -1 ) >> CLICK_SHIFT;
20546 mem_base = (m_in.membase + CLICK_SIZE -1 ) >> CLICK_SHIFT;
20547 free_mem(mem_base, mem_clicks);
20548 return(OK);
20549 }
20551 /*===========================================================================*
20552 * do_getsysinfo *
20553 *===========================================================================*/
20554 PUBLIC int do_getsysinfo()
20555 {
20556 struct mproc *proc_addr;
20557 vir_bytes src_addr, dst_addr;
20558 struct kinfo kinfo;
20559 size_t len;
20560 int s;
20561
20562 switch(m_in.info_what) {
20563 case SI_KINFO: /* kernel info is obtained via PM */
20564 sys_getkinfo(&kinfo);
20565 src_addr = (vir_bytes) &kinfo;
20566 len = sizeof(struct kinfo);
20567 break;
20568 case SI_PROC_ADDR: /* get address of PM process table */
20569 proc_addr = &mproc[0];
20570 src_addr = (vir_bytes) &proc_addr;
20571 len = sizeof(struct mproc *);
20572 break;
20573 case SI_PROC_TAB: /* copy entire process table */
20574 src_addr = (vir_bytes) mproc;
20575 len = sizeof(struct mproc) * NR_PROCS;
20576 break;
20577 default:
20578 return(EINVAL);
20579 }
20580
20581 dst_addr = (vir_bytes) m_in.info_where;
20582 if (OK != (s=sys_datacopy(SELF, src_addr, who, dst_addr, len)))
20583 return(s);
20584 return(OK);
20585 }
MINIX SOURCE CODE File: servers/pm/misc.c 915
20587 /*===========================================================================*
20588 * do_getprocnr *
20589 *===========================================================================*/
20590 PUBLIC int do_getprocnr()
20591 {
20592 register struct mproc *rmp;
20593 static char search_key[PROC_NAME_LEN+1];
20594 int key_len;
20595 int s;
20596
20597 if (m_in.pid >= 0) { /* lookup process by pid */
20598 for (rmp = &mproc[0]; rmp < &mproc[NR_PROCS]; rmp++) {
20599 if ((rmp->mp_flags & IN_USE) && (rmp->mp_pid==m_in.pid)) {
20600 mp->mp_reply.procnr = (int) (rmp - mproc);
20601 return(OK);
20602 }
20603 }
20604 return(ESRCH);
20605 } else if (m_in.namelen > 0) { /* lookup process by name */
20606 key_len = MIN(m_in.namelen, PROC_NAME_LEN);
20607 if (OK != (s=sys_datacopy(who, (vir_bytes) m_in.addr,
20608 SELF, (vir_bytes) search_key, key_len)))
20609 return(s);
20610 search_key[key_len] = ’\0’; /* terminate for safety */
20611 for (rmp = &mproc[0]; rmp < &mproc[NR_PROCS]; rmp++) {
20612 if ((rmp->mp_flags & IN_USE) &&
20613 strncmp(rmp->mp_name, search_key, key_len)==0) {
20614 mp->mp_reply.procnr = (int) (rmp - mproc);
20615 return(OK);
20616 }
20617 }
20618 return(ESRCH);
20619 } else { /* return own process number */
20620 mp->mp_reply.procnr = who;
20621 }
20622 return(OK);
20623 }
20625 /*===========================================================================*
20626 * do_reboot *
20627 *===========================================================================*/
20628 #define REBOOT_CODE "delay; boot"
20629 PUBLIC int do_reboot()
20630 {
20631 char monitor_code[32*sizeof(char *)];
20632 int code_len;
20633 int abort_flag;
20634
20635 if (mp->mp_effuid != SUPER_USER) return(EPERM);
20636
20637 switch (m_in.reboot_flag) {
20638 case RBT_HALT:
20639 case RBT_PANIC:
20640 case RBT_RESET:
20641 abort_flag = m_in.reboot_flag;
20642 break;
20643 case RBT_REBOOT:
20644 code_len = strlen(REBOOT_CODE) + 1;
20645 strncpy(monitor_code, REBOOT_CODE, code_len);
20646 abort_flag = RBT_MONITOR;
916 File: servers/pm/misc.c MINIX SOURCE CODE
20647 break;
20648 case RBT_MONITOR:
20649 code_len = m_in.reboot_strlen + 1;
20650 if (code_len > sizeof(monitor_code)) return(EINVAL);
20651 if (sys_datacopy(who, (vir_bytes) m_in.reboot_code,
20652 PM_PROC_NR, (vir_bytes) monitor_code,
20653 (phys_bytes) (code_len)) != OK) return(EFAULT);
20654 if (monitor_code[code_len-1] != 0) return(EINVAL);
20655 abort_flag = RBT_MONITOR;
20656 break;
20657 default:
20658 return(EINVAL);
20659 }
20660
20661 check_sig(-1, SIGKILL); /* kill all processes except init */
20662 tell_fs(REBOOT,0,0,0); /* tell FS to prepare for shutdown */
20663
20664 /* Ask the kernel to abort. All system services, including the PM, will
20665 * get a HARD_STOP notification. Await the notification in the main loop.
20666 */
20667 sys_abort(abort_flag, PM_PROC_NR, monitor_code, code_len);
20668 return(SUSPEND); /* don’t reply to killed process */
20669 }
20671 /*===========================================================================*
20672 * do_getsetpriority *
20673 *===========================================================================*/
20674 PUBLIC int do_getsetpriority()
20675 {
20676 int arg_which, arg_who, arg_pri;
20677 int rmp_nr;
20678 struct mproc *rmp;
20679
20680 arg_which = m_in.m1_i1;
20681 arg_who = m_in.m1_i2;
20682 arg_pri = m_in.m1_i3; /* for SETPRIORITY */
20683
20684 /* Code common to GETPRIORITY and SETPRIORITY. */
20685
20686 /* Only support PRIO_PROCESS for now. */
20687 if (arg_which != PRIO_PROCESS)
20688 return(EINVAL);
20689
20690 if (arg_who == 0)
20691 rmp_nr = who;
20692 else
20693 if ((rmp_nr = proc_from_pid(arg_who)) < 0)
20694 return(ESRCH);
20695
20696 rmp = &mproc[rmp_nr];
20697
20698 if (mp->mp_effuid != SUPER_USER &&
20699 mp->mp_effuid != rmp->mp_effuid && mp->mp_effuid != rmp->mp_realuid)
20700 return EPERM;
20701
20702 /* If GET, that’s it. */
20703 if (call_nr == GETPRIORITY) {
20704 return(rmp->mp_nice - PRIO_MIN);
20705 }
20706
MINIX SOURCE CODE File: servers/pm/misc.c 917
20707 /* Only root is allowed to reduce the nice level. */
20708 if (rmp->mp_nice > arg_pri && mp->mp_effuid != SUPER_USER)
20709 return(EACCES);
20710
20711 /* We’re SET, and it’s allowed. Do it and tell kernel. */
20712 rmp->mp_nice = arg_pri;
20713 return sys_nice(rmp_nr, arg_pri);
20714 }
20716 /*===========================================================================*
20717 * do_svrctl *
20718 *===========================================================================*/
20719 PUBLIC int do_svrctl()
20720 {
20721 int s, req;
20722 vir_bytes ptr;
20723 #define MAX_LOCAL_PARAMS 2
20724 static struct {
20725 char name[30];
20726 char value[30];
20727 } local_param_overrides[MAX_LOCAL_PARAMS];
20728 static int local_params = 0;
20729
20730 req = m_in.svrctl_req;
20731 ptr = (vir_bytes) m_in.svrctl_argp;
20732
20733 /* Is the request indeed for the MM? */
20734 if (((req >> 8) & 0xFF) != ’M’) return(EINVAL);
20735
20736 /* Control operations local to the PM. */
20737 switch(req) {
20738 case MMSETPARAM:
20739 case MMGETPARAM: {
20740 struct sysgetenv sysgetenv;
20741 char search_key[64];
20742 char *val_start;
20743 size_t val_len;
20744 size_t copy_len;
20745
20746 /* Copy sysgetenv structure to PM. */
20747 if (sys_datacopy(who, ptr, SELF, (vir_bytes) &sysgetenv,
20748 sizeof(sysgetenv)) != OK) return(EFAULT);
20749
20750 /* Set a param override? */
20751 if (req == MMSETPARAM) {
20752 if (local_params >= MAX_LOCAL_PARAMS) return ENOSPC;
20753 if (sysgetenv.keylen <= 0
20754 || sysgetenv.keylen >=
20755 sizeof(local_param_overrides[local_params].name)
20756 || sysgetenv.vallen <= 0
20757 || sysgetenv.vallen >=
20758 sizeof(local_param_overrides[local_params].value))
20759 return EINVAL;
20760
20761 if ((s = sys_datacopy(who, (vir_bytes) sysgetenv.key,
20762 SELF, (vir_bytes) local_param_overrides[local_params].name,
20763 sysgetenv.keylen)) != OK)
20764 return s;
20765 if ((s = sys_datacopy(who, (vir_bytes) sysgetenv.val,
20766 SELF, (vir_bytes) local_param_overrides[local_params].value,
918 File: servers/pm/misc.c MINIX SOURCE CODE
20767 sysgetenv.keylen)) != OK)
20768 return s;
20769 local_param_overrides[local_params].name[sysgetenv.keylen] = ’\0’;
20770 local_param_overrides[local_params].value[sysgetenv.vallen] = ’\0’;
20771
20772 local_params++;
20773
20774 return OK;
20775 }
20776
20777 if (sysgetenv.keylen == 0) { /* copy all parameters */
20778 val_start = monitor_params;
20779 val_len = sizeof(monitor_params);
20780 }
20781 else { /* lookup value for key */
20782 int p;
20783 /* Try to get a copy of the requested key. */
20784 if (sysgetenv.keylen > sizeof(search_key)) return(EINVAL);
20785 if ((s = sys_datacopy(who, (vir_bytes) sysgetenv.key,
20786 SELF, (vir_bytes) search_key, sysgetenv.keylen)) != OK)
20787 return(s);
20788
20789 /* Make sure key is null-terminated and lookup value.
20790 * First check local overrides.
20791 */
20792 search_key[sysgetenv.keylen-1]= ’\0’;
20793 for(p = 0; p < local_params; p++) {
20794 if (!strcmp(search_key, local_param_overrides[p].name)) {
20795 val_start = local_param_overrides[p].value;
20796 break;
20797 }
20798 }
20799 if (p >= local_params && (val_start = find_param(search_key)) == NULL)
20800 return(ESRCH);
20801 val_len = strlen(val_start) + 1;
20802 }
20803
20804 /* See if it fits in the client’s buffer. */
20805 if (val_len > sysgetenv.vallen)
20806 return E2BIG;
20807
20808 /* Value found, make the actual copy (as far as possible). */
20809 copy_len = MIN(val_len, sysgetenv.vallen);
20810 if ((s=sys_datacopy(SELF, (vir_bytes) val_start,
20811 who, (vir_bytes) sysgetenv.val, copy_len)) != OK)
20812 return(s);
20813
20814 return OK;
20815 }
20816 default:
20817 return(EINVAL);
20818 }
20819 }