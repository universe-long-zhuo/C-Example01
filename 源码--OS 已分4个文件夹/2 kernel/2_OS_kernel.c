++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             kernel/kernel.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
04600 #ifndef KERNEL_H
04601 #define KERNEL_H
04602
04603 /* This is the master header for the kernel. It includes some other files
04604 * and defines the principal constants.
04605 */
04606 #define _POSIX_SOURCE 1 /* tell headers to include POSIX stuff */
04607 #define _MINIX 1 /* tell headers to include MINIX stuff */
04608 #define _SYSTEM 1 /* tell headers that this is the kernel */
04609
04610 /* The following are so basic, all the *.c files get them automatically. */
04611 #include <minix/config.h> /* global configuration, MUST be first */
04612 #include <ansi.h> /* C style: ANSI or K&R, MUST be second */
04613 #include <sys/types.h> /* general system types */
04614 #include <minix/const.h> /* MINIX specific constants */
04615 #include <minix/type.h> /* MINIX specific types, e.g. message */
04616 #include <minix/ipc.h> /* MINIX run-time system */
04617 #include <timers.h> /* watchdog timer management */
04618 #include <errno.h> /* return codes and error numbers */
04619 #include <ibm/portio.h> /* device I/O and toggle interrupts */
04620
04621 /* Important kernel header files. */
04622 #include "config.h" /* configuration, MUST be first */
04623 #include "const.h" /* constants, MUST be second */
04624 #include "type.h" /* type definitions, MUST be third */
04625 #include "proto.h" /* function prototypes */
04626 #include "glo.h" /* global variables */
04627 #include "ipc.h" /* IPC constants */
04628 /* #include "debug.h" */ /* debugging, MUST be last kernel header */
04629
04630 #endif /* KERNEL_H */
04631
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/config.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
04700 #ifndef CONFIG_H
04701 #define CONFIG_H
04702
04703 /* This file defines the kernel configuration. It allows to set sizes of some
04704 * kernel buffers and to enable or disable debugging code, timing features,
04705 * and individual kernel calls.
04706 *
04707 * Changes:
04708 * Jul 11, 2005 Created. (Jorrit N. Herder)
04709 */
690 File: kernel/config.h MINIX SOURCE CODE
04710
04711 /* In embedded and sensor applications, not all the kernel calls may be
04712 * needed. In this section you can specify which kernel calls are needed
04713 * and which are not. The code for unneeded kernel calls is not included in
04714 * the system binary, making it smaller. If you are not sure, it is best
04715 * to keep all kernel calls enabled.
04716 */
04717 #define USE_FORK 1 /* fork a new process */
04718 #define USE_NEWMAP 1 /* set a new memory map */
04719 #define USE_EXEC 1 /* update process after execute */
04720 #define USE_EXIT 1 /* clean up after process exit */
04721 #define USE_TRACE 1 /* process information and tracing */
04722 #define USE_GETKSIG 1 /* retrieve pending kernel signals */
04723 #define USE_ENDKSIG 1 /* finish pending kernel signals */
04724 #define USE_KILL 1 /* send a signal to a process */
04725 #define USE_SIGSEND 1 /* send POSIX-style signal */
04726 #define USE_SIGRETURN 1 /* sys_sigreturn(proc_nr, ctxt_ptr, flags) */
04727 #define USE_ABORT 1 /* shut down MINIX */
04728 #define USE_GETINFO 1 /* retrieve a copy of kernel data */
04729 #define USE_TIMES 1 /* get process and system time info */
04730 #define USE_SETALARM 1 /* schedule a synchronous alarm */
04731 #define USE_DEVIO 1 /* read or write a single I/O port */
04732 #define USE_VDEVIO 1 /* process vector with I/O requests */
04733 #define USE_SDEVIO 1 /* perform I/O request on a buffer */
04734 #define USE_IRQCTL 1 /* set an interrupt policy */
04735 #define USE_SEGCTL 1 /* set up a remote segment */
04736 #define USE_PRIVCTL 1 /* system privileges control */
04737 #define USE_NICE 1 /* change scheduling priority */
04738 #define USE_UMAP 1 /* map virtual to physical address */
04739 #define USE_VIRCOPY 1 /* copy using virtual addressing */
04740 #define USE_VIRVCOPY 1 /* vector with virtual copy requests */
04741 #define USE_PHYSCOPY 1 /* copy using physical addressing */
04742 #define USE_PHYSVCOPY 1 /* vector with physical copy requests */
04743 #define USE_MEMSET 1 /* write char to a given memory area */
04744
04745 /* Length of program names stored in the process table. This is only used
04746 * for the debugging dumps that can be generated with the IS server. The PM
04747 * server keeps its own copy of the program name.
04748 */
04749 #define P_NAME_LEN 8
04750
04751 /* Kernel diagnostics are written to a circular buffer. After each message,
04752 * a system server is notified and a copy of the buffer can be retrieved to
04753 * display the message. The buffers size can safely be reduced.
04754 */
04755 #define KMESS_BUF_SIZE 256
04756
04757 /* Buffer to gather randomness. This is used to generate a random stream by
04758 * the MEMORY driver when reading from /dev/random.
04759 */
04760 #define RANDOM_ELEMENTS 32
04761
04762 /* This section contains defines for valuable system resources that are used
04763 * by device drivers. The number of elements of the vectors is determined by
04764 * the maximum needed by any given driver. The number of interrupt hooks may
04765 * be incremented on systems with many device drivers.
04766 */
04767 #define NR_IRQ_HOOKS 16 /* number of interrupt hooks */
04768 #define VDEVIO_BUF_SIZE 64 /* max elements per VDEVIO request */
04769 #define VCOPY_VEC_SIZE 16 /* max elements per VCOPY request */
MINIX SOURCE CODE File: kernel/config.h 691
04770
04771 /* How many bytes for the kernel stack. Space allocated in mpx.s. */
04772 #define K_STACK_BYTES 1024
04773
04774 /* This section allows to enable kernel debugging and timing functionality.
04775 * For normal operation all options should be disabled.
04776 */
04777 #define DEBUG_SCHED_CHECK 0 /* sanity check of scheduling queues */
04778 #define DEBUG_LOCK_CHECK 0 /* kernel lock() sanity check */
04779 #define DEBUG_TIME_LOCKS 0 /* measure time spent in locks */
04780
04781 #endif /* CONFIG_H */
04782
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
04800 /* General macros and constants used by the kernel. */
04801 #ifndef CONST_H
04802 #define CONST_H
04803
04804 #include <ibm/interrupt.h> /* interrupt numbers and hardware vectors */
04805 #include <ibm/ports.h> /* port addresses and magic numbers */
04806 #include <ibm/bios.h> /* BIOS addresses, sizes and magic numbers */
04807 #include <ibm/cpu.h> /* BIOS addresses, sizes and magic numbers */
04808 #include <minix/config.h>
04809 #include "config.h"
04810
04811 /* To translate an address in kernel space to a physical address. This is
04812 * the same as umap_local(proc_ptr, D, vir, sizeof(*vir)), but less costly.
04813 */
04814 #define vir2phys(vir) (kinfo.data_base + (vir_bytes) (vir))
04815
04816 /* Map a process number to a privilege structure id. */
04817 #define s_nr_to_id(n) (NR_TASKS + (n) + 1)
04818
04819 /* Translate a pointer to a field in a structure to a pointer to the structure
04820 * itself. So it translates ’&struct_ptr->field’ back to ’struct_ptr’.
04821 */
04822 #define structof(type, field, ptr) \
04823 ((type *) (((char *) (ptr)) - offsetof(type, field)))
04824
04825 /* Constants used in virtual_copy(). Values must be 0 and 1, respectively. */
04826 #define _SRC_ 0
04827 #define _DST_ 1
04828
04829 /* Number of random sources */
04830 #define RANDOM_SOURCES 16
04831
04832 /* Constants and macros for bit map manipulation. */
04833 #define BITCHUNK_BITS (sizeof(bitchunk_t) * CHAR_BIT)
04834 #define BITMAP_CHUNKS(nr_bits) (((nr_bits)+BITCHUNK_BITS-1)/BITCHUNK_BITS)
04835 #define MAP_CHUNK(map,bit) (map)[((bit)/BITCHUNK_BITS)]
04836 #define CHUNK_OFFSET(bit) ((bit)%BITCHUNK_BITS))
04837 #define GET_BIT(map,bit) ( MAP_CHUNK(map,bit) & (1 << CHUNK_OFFSET(bit) )
04838 #define SET_BIT(map,bit) ( MAP_CHUNK(map,bit) |= (1 << CHUNK_OFFSET(bit) )
04839 #define UNSET_BIT(map,bit) ( MAP_CHUNK(map,bit) &= ˜(1 << CHUNK_OFFSET(bit) )
692 File: kernel/const.h MINIX SOURCE CODE
04840
04841 #define get_sys_bit(map,bit) \
04842 ( MAP_CHUNK(map.chunk,bit) & (1 << CHUNK_OFFSET(bit) )
04843 #define set_sys_bit(map,bit) \
04844 ( MAP_CHUNK(map.chunk,bit) |= (1 << CHUNK_OFFSET(bit) )
04845 #define unset_sys_bit(map,bit) \
04846 ( MAP_CHUNK(map.chunk,bit) &= ˜(1 << CHUNK_OFFSET(bit) )
04847 #define NR_SYS_CHUNKS BITMAP_CHUNKS(NR_SYS_PROCS)
04848
04849 /* Program stack words and masks. */
04850 #define INIT_PSW 0x0200 /* initial psw */
04851 #define INIT_TASK_PSW 0x1200 /* initial psw for tasks (with IOPL 1) */
04852 #define TRACEBIT 0x0100 /* OR this with psw in proc[] for tracing */
04853 #define SETPSW(rp, new) /* permits only certain bits to be set */ \
04854 ((rp)->p_reg.psw = (rp)->p_reg.psw & ˜0xCD5 | (new) & 0xCD5)
04855 #define IF_MASK 0x00000200
04856 #define IOPL_MASK 0x003000
04857
04858 /* Disable/ enable hardware interrupts. The parameters of lock() and unlock()
04859 * are used when debugging is enabled. See debug.h for more information.
04860 */
04861 #define lock(c, v) intr_disable();
04862 #define unlock(c) intr_enable();
04863
04864 /* Sizes of memory tables. The boot monitor distinguishes three memory areas,
04865 * namely low mem below 1M, 1M-16M, and mem after 16M. More chunks are needed
04866 * for DOS MINIX.
04867 */
04868 #define NR_MEMS 8
04869
04870 #endif /* CONST_H */
04871
04872
04873
04874
04875
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/type.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
04900 #ifndef TYPE_H
04901 #define TYPE_H
04902
04903 typedef _PROTOTYPE( void task_t, (void) );
04904
04905 /* Process table and system property related types. */
04906 typedef int proc_nr_t; /* process table entry number */
04907 typedef short sys_id_t; /* system process index */
04908 typedef struct { /* bitmap for system indexes */
04909 bitchunk_t chunk[BITMAP_CHUNKS(NR_SYS_PROCS)];
04910 } sys_map_t;
04911
04912 struct boot_image {
04913 proc_nr_t proc_nr; /* process number to use */
04914 task_t *initial_pc; /* start function for tasks */
MINIX SOURCE CODE File: kernel/type.h 693
04915 int flags; /* process flags */
04916 unsigned char quantum; /* quantum (tick count) */
04917 int priority; /* scheduling priority */
04918 int stksize; /* stack size for tasks */
04919 short trap_mask; /* allowed system call traps */
04920 bitchunk_t ipc_to; /* send mask protection */
04921 long call_mask; /* system call protection */
04922 char proc_name[P_NAME_LEN]; /* name in process table */
04923 };
04924
04925 struct memory {
04926 phys_clicks base; /* start address of chunk */
04927 phys_clicks size; /* size of memory chunk */
04928 };
04929
04930 /* The kernel outputs diagnostic messages in a circular buffer. */
04931 struct kmessages {
04932 int km_next; /* next index to write */
04933 int km_size; /* current size in buffer */
04934 char km_buf[KMESS_BUF_SIZE]; /* buffer for messages */
04935 };
04936
04937 struct randomness {
04938 struct {
04939 int r_next; /* next index to write */
04940 int r_size; /* number of random elements */
04941 unsigned short r_buf[RANDOM_ELEMENTS]; /* buffer for random info */
04942 } bin[RANDOM_SOURCES];
04943 };
04944
04945 #if (CHIP == INTEL)
04946 typedef unsigned reg_t; /* machine register */
04947
04948 /* The stack frame layout is determined by the software, but for efficiency
04949 * it is laid out so the assembly code to use it is as simple as possible.
04950 * 80286 protected mode and all real modes use the same frame, built with
04951 * 16-bit registers. Real mode lacks an automatic stack switch, so little
04952 * is lost by using the 286 frame for it. The 386 frame differs only in
04953 * having 32-bit registers and more segment registers. The same names are
04954 * used for the larger registers to avoid differences in the code.
04955 */
04956 struct stackframe_s { /* proc_ptr points here */
04957 #if _WORD_SIZE == 4
04958 u16_t gs; /* last item pushed by save */
04959 u16_t fs; /* ˆ */
04960 #endif
04961 u16_t es; /* | */
04962 u16_t ds; /* | */
04963 reg_t di; /* di through cx are not accessed in C */
04964 reg_t si; /* order is to match pusha/popa */
04965 reg_t fp; /* bp */
04966 reg_t st; /* hole for another copy of sp */
04967 reg_t bx; /* | */
04968 reg_t dx; /* | */
04969 reg_t cx; /* | */
04970 reg_t retreg; /* ax and above are all pushed by save */
04971 reg_t retadr; /* return address for assembly code save() */
04972 reg_t pc; /* ˆ last item pushed by interrupt */
04973 reg_t cs; /* | */
04974 reg_t psw; /* | */
694 File: kernel/type.h MINIX SOURCE CODE
04975 reg_t sp; /* | */
04976 reg_t ss; /* these are pushed by CPU during interrupt */
04977 };
04978
04979 struct segdesc_s { /* segment descriptor for protected mode */
04980 u16_t limit_low;
04981 u16_t base_low;
04982 u8_t base_middle;
04983 u8_t access; /* |P|DL|1|X|E|R|A| */
04984 u8_t granularity; /* |G|X|0|A|LIMT| */
04985 u8_t base_high;
04986 };
04987
04988 typedef unsigned long irq_policy_t;
04989 typedef unsigned long irq_id_t;
04990
04991 typedef struct irq_hook {
04992 struct irq_hook *next; /* next hook in chain */
04993 int (*handler)(struct irq_hook *); /* interrupt handler */
04994 int irq; /* IRQ vector number */
04995 int id; /* id of this hook */
04996 int proc_nr; /* NONE if not in use */
04997 irq_id_t notify_id; /* id to return on interrupt */
04998 irq_policy_t policy; /* bit mask for policy */
04999 } irq_hook_t;
05000
05001 typedef int (*irq_handler_t)(struct irq_hook *);
05002
05003 #endif /* (CHIP == INTEL) */
05004
05005 #if (CHIP == M68000)
05006 /* M68000 specific types go here. */
05007 #endif /* (CHIP == M68000) */
05008
05009 #endif /* TYPE_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/proto.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
05100 /* Function prototypes. */
05101
05102 #ifndef PROTO_H
05103 #define PROTO_H
05104
05105 /* Struct declarations. */
05106 struct proc;
05107 struct timer;
05108
05109 /* clock.c */
05110 _PROTOTYPE( void clock_task, (void) );
05111 _PROTOTYPE( void clock_stop, (void) );
05112 _PROTOTYPE( clock_t get_uptime, (void) );
05113 _PROTOTYPE( unsigned long read_clock, (void) );
05114 _PROTOTYPE( void set_timer, (struct timer *tp, clock_t t, tmr_func_t f) );
05115 _PROTOTYPE( void reset_timer, (struct timer *tp) );
05116
05117 /* main.c */
05118 _PROTOTYPE( void main, (void) );
05119 _PROTOTYPE( void prepare_shutdown, (int how) );
MINIX SOURCE CODE File: kernel/proto.h 695
05120
05121 /* utility.c */
05122 _PROTOTYPE( void kprintf, (const char *fmt, ...) );
05123 _PROTOTYPE( void panic, (_CONST char *s, int n) );
05124
05125 /* proc.c */
05126 _PROTOTYPE( int sys_call, (int function, int src_dest, message *m_ptr) );
05127 _PROTOTYPE( int lock_notify, (int src, int dst) );
05128 _PROTOTYPE( int lock_send, (int dst, message *m_ptr) );
05129 _PROTOTYPE( void lock_enqueue, (struct proc *rp) );
05130 _PROTOTYPE( void lock_dequeue, (struct proc *rp) );
05131
05132 /* start.c */
05133 _PROTOTYPE( void cstart, (U16_t cs, U16_t ds, U16_t mds,
05134 U16_t parmoff, U16_t parmsize) );
05135
05136 /* system.c */
05137 _PROTOTYPE( int get_priv, (register struct proc *rc, int proc_type) );
05138 _PROTOTYPE( void send_sig, (int proc_nr, int sig_nr) );
05139 _PROTOTYPE( void cause_sig, (int proc_nr, int sig_nr) );
05140 _PROTOTYPE( void sys_task, (void) );
05141 _PROTOTYPE( void get_randomness, (int source) );
05142 _PROTOTYPE( int virtual_copy, (struct vir_addr *src, struct vir_addr *dst,
05143 vir_bytes bytes) );
05144 #define numap_local(proc_nr, vir_addr, bytes) \
05145 umap_local(proc_addr(proc_nr), D, (vir_addr), (bytes))
05146 _PROTOTYPE( phys_bytes umap_local, (struct proc *rp, int seg,
05147 vir_bytes vir_addr, vir_bytes bytes) );
05148 _PROTOTYPE( phys_bytes umap_remote, (struct proc *rp, int seg,
05149 vir_bytes vir_addr, vir_bytes bytes) );
05150 _PROTOTYPE( phys_bytes umap_bios, (struct proc *rp, vir_bytes vir_addr,
05151 vir_bytes bytes) );
05152
05153 /* exception.c */
05154 _PROTOTYPE( void exception, (unsigned vec_nr) );
05155
05156 /* i8259.c */
05157 _PROTOTYPE( void intr_init, (int mine) );
05158 _PROTOTYPE( void intr_handle, (irq_hook_t *hook) );
05159 _PROTOTYPE( void put_irq_handler, (irq_hook_t *hook, int irq,
05160 irq_handler_t handler) );
05161 _PROTOTYPE( void rm_irq_handler, (irq_hook_t *hook) );
05162
05163 /* klib*.s */
05164 _PROTOTYPE( void int86, (void) );
05165 _PROTOTYPE( void cp_mess, (int src,phys_clicks src_clicks,vir_bytes src_offset,
05166 phys_clicks dst_clicks, vir_bytes dst_offset) );
05167 _PROTOTYPE( void enable_irq, (irq_hook_t *hook) );
05168 _PROTOTYPE( int disable_irq, (irq_hook_t *hook) );
05169 _PROTOTYPE( u16_t mem_rdw, (U16_t segm, vir_bytes offset) );
05170 _PROTOTYPE( void phys_copy, (phys_bytes source, phys_bytes dest,
05171 phys_bytes count) );
05172 _PROTOTYPE( void phys_memset, (phys_bytes source, unsigned long pattern,
05173 phys_bytes count) );
05174 _PROTOTYPE( void phys_insb, (U16_t port, phys_bytes buf, size_t count) );
05175 _PROTOTYPE( void phys_insw, (U16_t port, phys_bytes buf, size_t count) );
05176 _PROTOTYPE( void phys_outsb, (U16_t port, phys_bytes buf, size_t count) );
05177 _PROTOTYPE( void phys_outsw, (U16_t port, phys_bytes buf, size_t count) );
05178 _PROTOTYPE( void reset, (void) );
05179 _PROTOTYPE( void level0, (void (*func)(void)) );
696 File: kernel/proto.h MINIX SOURCE CODE
05180 _PROTOTYPE( void monitor, (void) );
05181 _PROTOTYPE( void read_tsc, (unsigned long *high, unsigned long *low) );
05182 _PROTOTYPE( unsigned long read_cpu_flags, (void) );
05183
05184 /* mpx*.s */
05185 _PROTOTYPE( void idle_task, (void) );
05186 _PROTOTYPE( void restart, (void) );
05187
05188 /* The following are never called from C (pure asm procs). */
05189
05190 /* Exception handlers (real or protected mode), in numerical order. */
05191 void _PROTOTYPE( int00, (void) ), _PROTOTYPE( divide_error, (void) );
05192 void _PROTOTYPE( int01, (void) ), _PROTOTYPE( single_step_exception, (void) );
05193 void _PROTOTYPE( int02, (void) ), _PROTOTYPE( nmi, (void) );
05194 void _PROTOTYPE( int03, (void) ), _PROTOTYPE( breakpoint_exception, (void) );
05195 void _PROTOTYPE( int04, (void) ), _PROTOTYPE( overflow, (void) );
05196 void _PROTOTYPE( int05, (void) ), _PROTOTYPE( bounds_check, (void) );
05197 void _PROTOTYPE( int06, (void) ), _PROTOTYPE( inval_opcode, (void) );
05198 void _PROTOTYPE( int07, (void) ), _PROTOTYPE( copr_not_available, (void) );
05199 void _PROTOTYPE( double_fault, (void) );
05200 void _PROTOTYPE( copr_seg_overrun, (void) );
05201 void _PROTOTYPE( inval_tss, (void) );
05202 void _PROTOTYPE( segment_not_present, (void) );
05203 void _PROTOTYPE( stack_exception, (void) );
05204 void _PROTOTYPE( general_protection, (void) );
05205 void _PROTOTYPE( page_fault, (void) );
05206 void _PROTOTYPE( copr_error, (void) );
05207
05208 /* Hardware interrupt handlers. */
05209 _PROTOTYPE( void hwint00, (void) );
05210 _PROTOTYPE( void hwint01, (void) );
05211 _PROTOTYPE( void hwint02, (void) );
05212 _PROTOTYPE( void hwint03, (void) );
05213 _PROTOTYPE( void hwint04, (void) );
05214 _PROTOTYPE( void hwint05, (void) );
05215 _PROTOTYPE( void hwint06, (void) );
05216 _PROTOTYPE( void hwint07, (void) );
05217 _PROTOTYPE( void hwint08, (void) );
05218 _PROTOTYPE( void hwint09, (void) );
05219 _PROTOTYPE( void hwint10, (void) );
05220 _PROTOTYPE( void hwint11, (void) );
05221 _PROTOTYPE( void hwint12, (void) );
05222 _PROTOTYPE( void hwint13, (void) );
05223 _PROTOTYPE( void hwint14, (void) );
05224 _PROTOTYPE( void hwint15, (void) );
05225
05226 /* Software interrupt handlers, in numerical order. */
05227 _PROTOTYPE( void trp, (void) );
05228 _PROTOTYPE( void s_call, (void) ), _PROTOTYPE( p_s_call, (void) );
05229 _PROTOTYPE( void level0_call, (void) );
05230
05231 /* protect.c */
05232 _PROTOTYPE( void prot_init, (void) );
05233 _PROTOTYPE( void init_codeseg, (struct segdesc_s *segdp, phys_bytes base,
05234 vir_bytes size, int privilege) );
05235 _PROTOTYPE( void init_dataseg, (struct segdesc_s *segdp, phys_bytes base,
05236 vir_bytes size, int privilege) );
05237 _PROTOTYPE( phys_bytes seg2phys, (U16_t seg) );
05238 _PROTOTYPE( void phys2seg, (u16_t *seg, vir_bytes *off, phys_bytes phys));
05239 _PROTOTYPE( void enable_iop, (struct proc *pp) );
MINIX SOURCE CODE File: kernel/proto.h 697
05240 _PROTOTYPE( void alloc_segments, (struct proc *rp) );
05241
05242 #endif /* PROTO_H */
05243
05244
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/glo.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
05300 #ifndef GLO_H
05301 #define GLO_H
05302
05303 /* Global variables used in the kernel. This file contains the declarations;
05304 * storage space for the variables is allocated in table.c, because EXTERN is
05305 * defined as extern unless the _TABLE definition is seen. We rely on the
05306 * compiler’s default initialization (0) for several global variables.
05307 */
05308 #ifdef _TABLE
05309 #undef EXTERN
05310 #define EXTERN
05311 #endif
05312
05313 #include <minix/config.h>
05314 #include "config.h"
05315
05316 /* Variables relating to shutting down MINIX. */
05317 EXTERN char kernel_exception; /* TRUE after system exceptions */
05318 EXTERN char shutdown_started; /* TRUE after shutdowns / reboots */
05319
05320 /* Kernel information structures. This groups vital kernel information. */
05321 EXTERN phys_bytes aout; /* address of a.out headers */
05322 EXTERN struct kinfo kinfo; /* kernel information for users */
05323 EXTERN struct machine machine; /* machine information for users */
05324 EXTERN struct kmessages kmess; /* diagnostic messages in kernel */
05325 EXTERN struct randomness krandom; /* gather kernel random information */
05326
05327 /* Process scheduling information and the kernel reentry count. */
05328 EXTERN struct proc *prev_ptr; /* previously running process */
05329 EXTERN struct proc *proc_ptr; /* pointer to currently running process */
05330 EXTERN struct proc *next_ptr; /* next process to run after restart() */
05331 EXTERN struct proc *bill_ptr; /* process to bill for clock ticks */
05332 EXTERN char k_reenter; /* kernel reentry count (entry count less 1) */
05333 EXTERN unsigned lost_ticks; /* clock ticks counted outside clock task */
05334
05335 /* Interrupt related variables. */
05336 EXTERN irq_hook_t irq_hooks[NR_IRQ_HOOKS]; /* hooks for general use */
05337 EXTERN irq_hook_t *irq_handlers[NR_IRQ_VECTORS];/* list of IRQ handlers */
05338 EXTERN int irq_actids[NR_IRQ_VECTORS]; /* IRQ ID bits active */
05339 EXTERN int irq_use; /* map of all in-use irq’s */
05340
05341 /* Miscellaneous. */
05342 EXTERN reg_t mon_ss, mon_sp; /* boot monitor stack */
05343 EXTERN int mon_return; /* true if we can return to monitor */
05344
05345 /* Variables that are initialized elsewhere are just extern here. */
05346 extern struct boot_image image[]; /* system image processes */
05347 extern char *t_stack[]; /* task stack space */
05348 extern struct segdesc_s gdt[]; /* global descriptor table */
05349
698 File: kernel/glo.h MINIX SOURCE CODE
05350 EXTERN _PROTOTYPE( void (*level0_func), (void) );
05351
05352 #endif /* GLO_H */
05353
05354
05355
05356
05357
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/ipc.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
05400 #ifndef IPC_H
05401 #define IPC_H
05402
05403 /* This header file defines constants for MINIX inter-process communication.
05404 * These definitions are used in the file proc.c.
05405 */
05406 #include <minix/com.h>
05407
05408 /* Masks and flags for system calls. */
05409 #define SYSCALL_FUNC 0x0F /* mask for system call function */
05410 #define SYSCALL_FLAGS 0xF0 /* mask for system call flags */
05411 #define NON_BLOCKING 0x10 /* prevent blocking, return error */
05412
05413 /* System call numbers that are passed when trapping to the kernel. The
05414 * numbers are carefully defined so that it can easily be seen (based on
05415 * the bits that are on) which checks should be done in sys_call().
05416 */
05417 #define SEND 1 /* 0 0 0 1 : blocking send */
05418 #define RECEIVE 2 /* 0 0 1 0 : blocking receive */
05419 #define SENDREC 3 /* 0 0 1 1 : SEND + RECEIVE */
05420 #define NOTIFY 4 /* 0 1 0 0 : nonblocking notify */
05421 #define ECHO 8 /* 1 0 0 0 : echo a message */
05422
05423 /* The following bit masks determine what checks that should be done. */
05424 #define CHECK_PTR 0x0B /* 1 0 1 1 : validate message buffer */
05425 #define CHECK_DST 0x05 /* 0 1 0 1 : validate message destination */
05426 #define CHECK_SRC 0x02 /* 0 0 1 0 : validate message source */
05427
05428 #endif /* IPC_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/proc.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
05500 #ifndef PROC_H
05501 #define PROC_H
05502
05503 /* Here is the declaration of the process table. It contains all process
05504 * data, including registers, flags, scheduling priority, memory map,
05505 * accounting, message passing (IPC) information, and so on.
05506 *
05507 * Many assembly code routines reference fields in it. The offsets to these
05508 * fields are defined in the assembler include file sconst.h. When changing
05509 * struct proc, be sure to change sconst.h to match.
MINIX SOURCE CODE File: kernel/proc.h 699
05510 */
05511 #include <minix/com.h>
05512 #include "protect.h"
05513 #include "const.h"
05514 #include "priv.h"
05515
05516 struct proc {
05517 struct stackframe_s p_reg; /* process’ registers saved in stack frame */
05518 reg_t p_ldt_sel; /* selector in gdt with ldt base and limit */
05519 struct segdesc_s p_ldt[2+NR_REMOTE_SEGS]; /* CS, DS and remote segments */
05520
05521 proc_nr_t p_nr; /* number of this process (for fast access) */
05522 struct priv *p_priv; /* system privileges structure */
05523 char p_rts_flags; /* SENDING, RECEIVING, etc. */
05524
05525 char p_priority; /* current scheduling priority */
05526 char p_max_priority; /* maximum scheduling priority */
05527 char p_ticks_left; /* number of scheduling ticks left */
05528 char p_quantum_size; /* quantum size in ticks */
05529
05530 struct mem_map p_memmap[NR_LOCAL_SEGS]; /* memory map (T, D, S) */
05531
05532 clock_t p_user_time; /* user time in ticks */
05533 clock_t p_sys_time; /* sys time in ticks */
05534
05535 struct proc *p_nextready; /* pointer to next ready process */
05536 struct proc *p_caller_q; /* head of list of procs wishing to send */
05537 struct proc *p_q_link; /* link to next proc wishing to send */
05538 message *p_messbuf; /* pointer to passed message buffer */
05539 proc_nr_t p_getfrom; /* from whom does process want to receive? */
05540 proc_nr_t p_sendto; /* to whom does process want to send? */
05541
05542 sigset_t p_pending; /* bit map for pending kernel signals */
05543
05544 char p_name[P_NAME_LEN]; /* name of the process, including \0 */
05545 };
05546
05547 /* Bits for the runtime flags. A process is runnable iff p_rts_flags == 0. */
05548 #define SLOT_FREE 0x01 /* process slot is free */
05549 #define NO_MAP 0x02 /* keeps unmapped forked child from running */
05550 #define SENDING 0x04 /* process blocked trying to SEND */
05551 #define RECEIVING 0x08 /* process blocked trying to RECEIVE */
05552 #define SIGNALED 0x10 /* set when new kernel signal arrives */
05553 #define SIG_PENDING 0x20 /* unready while signal being processed */
05554 #define P_STOP 0x40 /* set when process is being traced */
05555 #define NO_PRIV 0x80 /* keep forked system process from running */
05556
05557 /* Scheduling priorities for p_priority. Values must start at zero (highest
05558 * priority) and increment. Priorities of the processes in the boot image
05559 * can be set in table.c. IDLE must have a queue for itself, to prevent low
05560 * priority user processes to run round-robin with IDLE.
05561 */
05562 #define NR_SCHED_QUEUES 16 /* MUST equal minimum priority + 1 */
05563 #define TASK_Q 0 /* highest, used for kernel tasks */
05564 #define MAX_USER_Q 0 /* highest priority for user processes */
05565 #define USER_Q 7 /* default (should correspond to nice 0) */
05566 #define MIN_USER_Q 14 /* minimum priority for user processes */
05567 #define IDLE_Q 15 /* lowest, only IDLE process goes here */
05568
05569 /* Magic process table addresses. */
700 File: kernel/proc.h MINIX SOURCE CODE
05570 #define BEG_PROC_ADDR (&proc[0])
05571 #define BEG_USER_ADDR (&proc[NR_TASKS])
05572 #define END_PROC_ADDR (&proc[NR_TASKS + NR_PROCS])
05573
05574 #define NIL_PROC ((struct proc *) 0)
05575 #define NIL_SYS_PROC ((struct proc *) 1)
05576 #define cproc_addr(n) (&(proc + NR_TASKS)[(n)])
05577 #define proc_addr(n) (pproc_addr + NR_TASKS)[(n)]
05578 #define proc_nr(p) ((p)->p_nr)
05579
05580 #define isokprocn(n) ((unsigned) ((n) + NR_TASKS) < NR_PROCS + NR_TASKS)
05581 #define isemptyn(n) isemptyp(proc_addr(n))
05582 #define isemptyp(p) ((p)->p_rts_flags == SLOT_FREE)
05583 #define iskernelp(p) iskerneln((p)->p_nr)
05584 #define iskerneln(n) ((n) < 0)
05585 #define isuserp(p) isusern((p)->p_nr)
05586 #define isusern(n) ((n) >= 0)
05587
05588 /* The process table and pointers to process table slots. The pointers allow
05589 * faster access because now a process entry can be found by indexing the
05590 * pproc_addr array, while accessing an element i requires a multiplication
05591 * with sizeof(struct proc) to determine the address.
05592 */
05593 EXTERN struct proc proc[NR_TASKS + NR_PROCS]; /* process table */
05594 EXTERN struct proc *pproc_addr[NR_TASKS + NR_PROCS];
05595 EXTERN struct proc *rdy_head[NR_SCHED_QUEUES]; /* ptrs to ready list headers */
05596 EXTERN struct proc *rdy_tail[NR_SCHED_QUEUES]; /* ptrs to ready list tails */
05597
05598 #endif /* PROC_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/sconst.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
05600 ! Miscellaneous constants used in assembler code.
05601 W = _WORD_SIZE ! Machine word size.
05602
05603 ! Offsets in struct proc. They MUST match proc.h.
05604 P_STACKBASE = 0
05605 GSREG = P_STACKBASE
05606 FSREG = GSREG + 2 ! 386 introduces FS and GS segments
05607 ESREG = FSREG + 2
05608 DSREG = ESREG + 2
05609 DIREG = DSREG + 2
05610 SIREG = DIREG + W
05611 BPREG = SIREG + W
05612 STREG = BPREG + W ! hole for another SP
05613 BXREG = STREG + W
05614 DXREG = BXREG + W
05615 CXREG = DXREG + W
05616 AXREG = CXREG + W
05617 RETADR = AXREG + W ! return address for save() call
05618 PCREG = RETADR + W
05619 CSREG = PCREG + W
05620 PSWREG = CSREG + W
05621 SPREG = PSWREG + W
05622 SSREG = SPREG + W
05623 P_STACKTOP = SSREG + W
05624 P_LDT_SEL = P_STACKTOP
MINIX SOURCE CODE File: kernel/sconst.h 701
05625 P_LDT = P_LDT_SEL + W
05626
05627 Msize = 9 ! size of a message in 32-bit words
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/priv.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
05700 #ifndef PRIV_H
05701 #define PRIV_H
05702
05703 /* Declaration of the system privileges structure. It defines flags, system
05704 * call masks, an synchronous alarm timer, I/O privileges, pending hardware
05705 * interrupts and notifications, and so on.
05706 * System processes each get their own structure with properties, whereas all
05707 * user processes share one structure. This setup provides a clear separation
05708 * between common and privileged process fields and is very space efficient.
05709 *
05710 * Changes:
05711 * Jul 01, 2005 Created. (Jorrit N. Herder)
05712 */
05713 #include <minix/com.h>
05714 #include "protect.h"
05715 #include "const.h"
05716 #include "type.h"
05717
05718 struct priv {
05719 proc_nr_t s_proc_nr; /* number of associated process */
05720 sys_id_t s_id; /* index of this system structure */
05721 short s_flags; /* PREEMTIBLE, BILLABLE, etc. */
05722
05723 short s_trap_mask; /* allowed system call traps */
05724 sys_map_t s_ipc_from; /* allowed callers to receive from */
05725 sys_map_t s_ipc_to; /* allowed destination processes */
05726 long s_call_mask; /* allowed kernel calls */
05727
05728 sys_map_t s_notify_pending; /* bit map with pending notifications */
05729 irq_id_t s_int_pending; /* pending hardware interrupts */
05730 sigset_t s_sig_pending; /* pending signals */
05731
05732 timer_t s_alarm_timer; /* synchronous alarm timer */
05733 struct far_mem s_farmem[NR_REMOTE_SEGS]; /* remote memory map */
05734 reg_t *s_stack_guard; /* stack guard word for kernel tasks */
05735 };
05736
05737 /* Guard word for task stacks. */
05738 #define STACK_GUARD ((reg_t) (sizeof(reg_t) == 2 ? 0xBEEF : 0xDEADBEEF))
05739
05740 /* Bits for the system property flags. */
05741 #define PREEMPTIBLE 0x01 /* kernel tasks are not preemptible */
05742 #define BILLABLE 0x04 /* some processes are not billable */
05743 #define SYS_PROC 0x10 /* system processes are privileged */
05744 #define SENDREC_BUSY 0x20 /* sendrec() in progress */
05745
05746 /* Magic system structure table addresses. */
05747 #define BEG_PRIV_ADDR (&priv[0])
05748 #define END_PRIV_ADDR (&priv[NR_SYS_PROCS])
05749
702 File: kernel/priv.h MINIX SOURCE CODE
05750 #define priv_addr(i) (ppriv_addr)[(i)]
05751 #define priv_id(rp) ((rp)->p_priv->s_id)
05752 #define priv(rp) ((rp)->p_priv)
05753
05754 #define id_to_nr(id) priv_addr(id)->s_proc_nr
05755 #define nr_to_id(nr) priv(proc_addr(nr))->s_id
05756
05757 /* The system structures table and pointers to individual table slots. The
05758 * pointers allow faster access because now a process entry can be found by
05759 * indexing the psys_addr array, while accessing an element i requires a
05760 * multiplication with sizeof(struct sys) to determine the address.
05761 */
05762 EXTERN struct priv priv[NR_SYS_PROCS]; /* system properties table */
05763 EXTERN struct priv *ppriv_addr[NR_SYS_PROCS]; /* direct slot pointers */
05764
05765 /* Unprivileged user processes all share the same privilege structure.
05766 * This id must be fixed because it is used to check send mask entries.
05767 */
05768 #define USER_PRIV_ID 0
05769
05770 /* Make sure the system can boot. The following sanity check verifies that
05771 * the system privileges table is large enough for the number of processes
05772 * in the boot image.
05773 */
05774 #if (NR_BOOT_PROCS > NR_SYS_PROCS)
05775 #error NR_SYS_PROCS must be larger than NR_BOOT_PROCS
05776 #endif
05777
05778 #endif /* PRIV_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/protect.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
05800 /* Constants for protected mode. */
05801
05802 /* Table sizes. */
05803 #define GDT_SIZE (FIRST_LDT_INDEX + NR_TASKS + NR_PROCS)
05804 /* spec. and LDT’s */
05805 #define IDT_SIZE (IRQ8_VECTOR + 8) /* only up to the highest vector */
05806 #define LDT_SIZE (2 + NR_REMOTE_SEGS) /* CS, DS and remote segments */
05807
05808 /* Fixed global descriptors. 1 to 7 are prescribed by the BIOS. */
05809 #define GDT_INDEX 1 /* GDT descriptor */
05810 #define IDT_INDEX 2 /* IDT descriptor */
05811 #define DS_INDEX 3 /* kernel DS */
05812 #define ES_INDEX 4 /* kernel ES (386: flag 4 Gb at startup) */
05813 #define SS_INDEX 5 /* kernel SS (386: monitor SS at startup) */
05814 #define CS_INDEX 6 /* kernel CS */
05815 #define MON_CS_INDEX 7 /* temp for BIOS (386: monitor CS at startup) */
05816 #define TSS_INDEX 8 /* kernel TSS */
05817 #define DS_286_INDEX 9 /* scratch 16-bit source segment */
05818 #define ES_286_INDEX 10 /* scratch 16-bit destination segment */
05819 #define A_INDEX 11 /* 64K memory segment at A0000 */
05820 #define B_INDEX 12 /* 64K memory segment at B0000 */
05821 #define C_INDEX 13 /* 64K memory segment at C0000 */
05822 #define D_INDEX 14 /* 64K memory segment at D0000 */
05823 #define FIRST_LDT_INDEX 15 /* rest of descriptors are LDT’s */
05824
MINIX SOURCE CODE File: kernel/protect.h 703
05825 #define GDT_SELECTOR 0x08 /* (GDT_INDEX * DESC_SIZE) bad for asld */
05826 #define IDT_SELECTOR 0x10 /* (IDT_INDEX * DESC_SIZE) */
05827 #define DS_SELECTOR 0x18 /* (DS_INDEX * DESC_SIZE) */
05828 #define ES_SELECTOR 0x20 /* (ES_INDEX * DESC_SIZE) */
05829 #define FLAT_DS_SELECTOR 0x21 /* less privileged ES */
05830 #define SS_SELECTOR 0x28 /* (SS_INDEX * DESC_SIZE) */
05831 #define CS_SELECTOR 0x30 /* (CS_INDEX * DESC_SIZE) */
05832 #define MON_CS_SELECTOR 0x38 /* (MON_CS_INDEX * DESC_SIZE) */
05833 #define TSS_SELECTOR 0x40 /* (TSS_INDEX * DESC_SIZE) */
05834 #define DS_286_SELECTOR 0x49 /* (DS_286_INDEX*DESC_SIZE+TASK_PRIVILEGE) */
05835 #define ES_286_SELECTOR 0x51 /* (ES_286_INDEX*DESC_SIZE+TASK_PRIVILEGE) */
05836
05837 /* Fixed local descriptors. */
05838 #define CS_LDT_INDEX 0 /* process CS */
05839 #define DS_LDT_INDEX 1 /* process DS=ES=FS=GS=SS */
05840 #define EXTRA_LDT_INDEX 2 /* first of the extra LDT entries */
05841
05842 /* Privileges. */
05843 #define INTR_PRIVILEGE 0 /* kernel and interrupt handlers */
05844 #define TASK_PRIVILEGE 1 /* kernel tasks */
05845 #define USER_PRIVILEGE 3 /* servers and user processes */
05846
05847 /* 286 hardware constants. */
05848
05849 /* Exception vector numbers. */
05850 #define BOUNDS_VECTOR 5 /* bounds check failed */
05851 #define INVAL_OP_VECTOR 6 /* invalid opcode */
05852 #define COPROC_NOT_VECTOR 7 /* coprocessor not available */
05853 #define DOUBLE_FAULT_VECTOR 8
05854 #define COPROC_SEG_VECTOR 9 /* coprocessor segment overrun */
05855 #define INVAL_TSS_VECTOR 10 /* invalid TSS */
05856 #define SEG_NOT_VECTOR 11 /* segment not present */
05857 #define STACK_FAULT_VECTOR 12 /* stack exception */
05858 #define PROTECTION_VECTOR 13 /* general protection */
05859
05860 /* Selector bits. */
05861 #define TI 0x04 /* table indicator */
05862 #define RPL 0x03 /* requester privilege level */
05863
05864 /* Descriptor structure offsets. */
05865 #define DESC_BASE 2 /* to base_low */
05866 #define DESC_BASE_MIDDLE 4 /* to base_middle */
05867 #define DESC_ACCESS 5 /* to access byte */
05868 #define DESC_SIZE 8 /* sizeof (struct segdesc_s) */
05869
05870 /* Base and limit sizes and shifts. */
05871 #define BASE_MIDDLE_SHIFT 16 /* shift for base --> base_middle */
05872
05873 /* Access-byte and type-byte bits. */
05874 #define PRESENT 0x80 /* set for descriptor present */
05875 #define DPL 0x60 /* descriptor privilege level mask */
05876 #define DPL_SHIFT 5
05877 #define SEGMENT 0x10 /* set for segment-type descriptors */
05878
05879 /* Access-byte bits. */
05880 #define EXECUTABLE 0x08 /* set for executable segment */
05881 #define CONFORMING 0x04 /* set for conforming segment if executable */
05882 #define EXPAND_DOWN 0x04 /* set for expand-down segment if !executable*/
05883 #define READABLE 0x02 /* set for readable segment if executable */
05884 #define WRITEABLE 0x02 /* set for writeable segment if !executable */
704 File: kernel/protect.h MINIX SOURCE CODE
05885 #define TSS_BUSY 0x02 /* set if TSS descriptor is busy */
05886 #define ACCESSED 0x01 /* set if segment accessed */
05887
05888 /* Special descriptor types. */
05889 #define AVL_286_TSS 1 /* available 286 TSS */
05890 #define LDT 2 /* local descriptor table */
05891 #define BUSY_286_TSS 3 /* set transparently to the software */
05892 #define CALL_286_GATE 4 /* not used */
05893 #define TASK_GATE 5 /* only used by debugger */
05894 #define INT_286_GATE 6 /* interrupt gate, used for all vectors */
05895 #define TRAP_286_GATE 7 /* not used */
05896
05897 /* Extra 386 hardware constants. */
05898
05899 /* Exception vector numbers. */
05900 #define PAGE_FAULT_VECTOR 14
05901 #define COPROC_ERR_VECTOR 16 /* coprocessor error */
05902
05903 /* Descriptor structure offsets. */
05904 #define DESC_GRANULARITY 6 /* to granularity byte */
05905 #define DESC_BASE_HIGH 7 /* to base_high */
05906
05907 /* Base and limit sizes and shifts. */
05908 #define BASE_HIGH_SHIFT 24 /* shift for base --> base_high */
05909 #define BYTE_GRAN_MAX 0xFFFFFL /* maximum size for byte granular segment */
05910 #define GRANULARITY_SHIFT 16 /* shift for limit --> granularity */
05911 #define OFFSET_HIGH_SHIFT 16 /* shift for (gate) offset --> offset_high */
05912 #define PAGE_GRAN_SHIFT 12 /* extra shift for page granular limits */
05913
05914 /* Type-byte bits. */
05915 #define DESC_386_BIT 0x08 /* 386 types are obtained by ORing with this */
05916 /* LDT’s and TASK_GATE’s don’t need it */
05917
05918 /* Granularity byte. */
05919 #define GRANULAR 0x80 /* set for 4K granularilty */
05920 #define DEFAULT 0x40 /* set for 32-bit defaults (executable seg) */
05921 #define BIG 0x40 /* set for "BIG" (expand-down seg) */
05922 #define AVL 0x10 /* 0 for available */
05923 #define LIMIT_HIGH 0x0F /* mask for high bits of limit */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/table.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
06000 /* The object file of "table.c" contains most kernel data. Variables that
06001 * are declared in the *.h files appear with EXTERN in front of them, as in
06002 *
06003 * EXTERN int x;
06004 *
06005 * Normally EXTERN is defined as extern, so when they are included in another
06006 * file, no storage is allocated. If EXTERN were not present, but just say,
06007 *
06008 * int x;
06009 *
06010 * then including this file in several source files would cause ’x’ to be
06011 * declared several times. While some linkers accept this, others do not,
06012 * so they are declared extern when included normally. However, it must be
06013 * declared for real somewhere. That is done here, by redefining EXTERN as
06014 * the null string, so that inclusion of all *.h files in table.c actually
MINIX SOURCE CODE File: kernel/table.c 705
06015 * generates storage for them.
06016 *
06017 * Various variables could not be declared EXTERN, but are declared PUBLIC
06018 * or PRIVATE. The reason for this is that extern variables cannot have a
06019 * default initialization. If such variables are shared, they must also be
06020 * declared in one of the *.h files without the initialization. Examples
06021 * include ’boot_image’ (this file) and ’idt’ and ’gdt’ (protect.c).
06022 *
06023 * Changes:
06024 * Aug 02, 2005 set privileges and minimal boot image (Jorrit N. Herder)
06025 * Oct 17, 2004 updated above and tasktab comments (Jorrit N. Herder)
06026 * May 01, 2004 changed struct for system image (Jorrit N. Herder)
06027 */
06028 #define _TABLE
06029
06030 #include "kernel.h"
06031 #include "proc.h"
06032 #include "ipc.h"
06033 #include <minix/com.h>
06034 #include <ibm/int86.h>
06035
06036 /* Define stack sizes for the kernel tasks included in the system image. */
06037 #define NO_STACK 0
06038 #define SMALL_STACK (128 * sizeof(char *))
06039 #define IDL_S SMALL_STACK /* 3 intr, 3 temps, 4 db for Intel */
06040 #define HRD_S NO_STACK /* dummy task, uses kernel stack */
06041 #define TSK_S SMALL_STACK /* system and clock task */
06042
06043 /* Stack space for all the task stacks. Declared as (char *) to align it. */
06044 #define TOT_STACK_SPACE (IDL_S + HRD_S + (2 * TSK_S))
06045 PUBLIC char *t_stack[TOT_STACK_SPACE / sizeof(char *)];
06046
06047 /* Define flags for the various process types. */
06048 #define IDL_F (SYS_PROC | PREEMPTIBLE | BILLABLE) /* idle task */
06049 #define TSK_F (SYS_PROC) /* kernel tasks */
06050 #define SRV_F (SYS_PROC | PREEMPTIBLE) /* system services */
06051 #define USR_F (BILLABLE | PREEMPTIBLE) /* user processes */
06052
06053 /* Define system call traps for the various process types. These call masks
06054 * determine what system call traps a process is allowed to make.
06055 */
06056 #define TSK_T (1 << RECEIVE) /* clock and system */
06057 #define SRV_T (˜0) /* system services */
06058 #define USR_T ((1 << SENDREC) | (1 << ECHO)) /* user processes */
06059
06060 /* Send masks determine to whom processes can send messages or notifications.
06061 * The values here are used for the processes in the boot image. We rely on
06062 * the initialization code in main() to match the s_nr_to_id() mapping for the
06063 * processes in the boot image, so that the send mask that is defined here
06064 * can be directly copied onto map[0] of the actual send mask. Privilege
06065 * structure 0 is shared by user processes.
06066 */
06067 #define s(n) (1 << s_nr_to_id(n))
06068 #define SRV_M (˜0)
06069 #define SYS_M (˜0)
06070 #define USR_M (s(PM_PROC_NR) | s(FS_PROC_NR) | s(RS_PROC_NR))
06071 #define DRV_M (USR_M | s(SYSTEM) | s(CLOCK) | s(LOG_PROC_NR) | s(TTY_PROC_NR))
06072
06073 /* Define kernel calls that processes are allowed to make. This is not looking
06074 * very nice, but we need to define the access rights on a per call basis.
706 File: kernel/table.c MINIX SOURCE CODE
06075 * Note that the reincarnation server has all bits on, because it should
06076 * be allowed to distribute rights to services that it starts.
06077 */
06078 #define c(n) (1 << ((n)-KERNEL_CALL))
06079 #define RS_C ˜0
06080 #define PM_C ˜(c(SYS_DEVIO) | c(SYS_SDEVIO) | c(SYS_VDEVIO) \
06081 | c(SYS_IRQCTL) | c(SYS_INT86))
06082 #define FS_C (c(SYS_KILL) | c(SYS_VIRCOPY) | c(SYS_VIRVCOPY) | c(SYS_UMAP) \
06083 | c(SYS_GETINFO) | c(SYS_EXIT) | c(SYS_TIMES) | c(SYS_SETALARM))
06084 #define DRV_C (FS_C | c(SYS_SEGCTL) | c(SYS_IRQCTL) | c(SYS_INT86) \
06085 | c(SYS_DEVIO) | c(SYS_VDEVIO) | c(SYS_SDEVIO))
06086 #define MEM_C (DRV_C | c(SYS_PHYSCOPY) | c(SYS_PHYSVCOPY))
06087
06088 /* The system image table lists all programs that are part of the boot image.
06089 * The order of the entries here MUST agree with the order of the programs
06090 * in the boot image and all kernel tasks must come first.
06091 * Each entry provides the process number, flags, quantum size (qs), scheduling
06092 * queue, allowed traps, ipc mask, and a name for the process table. The
06093 * initial program counter and stack size is also provided for kernel tasks.
06094 */
06095 PUBLIC struct boot_image image[] = {
06096 /* process nr, pc, flags, qs, queue, stack, traps, ipcto, call, name */
06097 { IDLE, idle_task, IDL_F, 8, IDLE_Q, IDL_S, 0, 0, 0, "IDLE" },
06098 { CLOCK,clock_task, TSK_F, 64, TASK_Q, TSK_S, TSK_T, 0, 0, "CLOCK" },
06099 { SYSTEM, sys_task, TSK_F, 64, TASK_Q, TSK_S, TSK_T, 0, 0, "SYSTEM"},
06100 { HARDWARE, 0, TSK_F, 64, TASK_Q, HRD_S, 0, 0, 0, "KERNEL"},
06101 { PM_PROC_NR, 0, SRV_F, 32, 3, 0, SRV_T, SRV_M, PM_C, "pm" },
06102 { FS_PROC_NR, 0, SRV_F, 32, 4, 0, SRV_T, SRV_M, FS_C, "fs" },
06103 { RS_PROC_NR, 0, SRV_F, 4, 3, 0, SRV_T, SYS_M, RS_C, "rs" },
06104 { TTY_PROC_NR, 0, SRV_F, 4, 1, 0, SRV_T, SYS_M, DRV_C, "tty" },
06105 { MEM_PROC_NR, 0, SRV_F, 4, 2, 0, SRV_T, DRV_M, MEM_C, "memory"},
06106 { LOG_PROC_NR, 0, SRV_F, 4, 2, 0, SRV_T, SYS_M, DRV_C, "log" },
06107 { DRVR_PROC_NR, 0, SRV_F, 4, 2, 0, SRV_T, SYS_M, DRV_C, "driver"},
06108 { INIT_PROC_NR, 0, USR_F, 8, USER_Q, 0, USR_T, USR_M, 0, "init" },
06109 };
06110
06111 /* Verify the size of the system image table at compile time. Also verify that
06112 * the first chunk of the ipc mask has enough bits to accommodate the processes
06113 * in the image.
06114 * If a problem is detected, the size of the ’dummy’ array will be negative,
06115 * causing a compile time error. Note that no space is actually allocated
06116 * because ’dummy’ is declared extern.
06117 */
06118 extern int dummy[(NR_BOOT_PROCS==sizeof(image)/
06119 sizeof(struct boot_image))?1:-1];
06120 extern int dummy[(BITCHUNK_BITS > NR_BOOT_PROCS - 1) ? 1 : -1];
06121
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/mpx.s
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
06200 #
06201 ! Chooses between the 8086 and 386 versions of the Minix startup code.
06202
06203 #include <minix/config.h>
06204 #if _WORD_SIZE == 2
MINIX SOURCE CODE File: kernel/mpx.s 707
06205 #include "mpx88.s"
06206 #else
06207 #include "mpx386.s"
06208 #endif
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/mpx386.s
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
06300 #
06301 ! This file, mpx386.s, is included by mpx.s when Minix is compiled for
06302 ! 32-bit Intel CPUs. The alternative mpx88.s is compiled for 16-bit CPUs.
06303
06304 ! This file is part of the lowest layer of the MINIX kernel. (The other part
06305 ! is "proc.c".) The lowest layer does process switching and message handling.
06306 ! Furthermore it contains the assembler startup code for Minix and the 32-bit
06307 ! interrupt handlers. It cooperates with the code in "start.c" to set up a
06308 ! good environment for main().
06309
06310 ! Every transition to the kernel goes through this file. Transitions to the
06311 ! kernel may be nested. The initial entry may be with a system call (i.e.,
06312 ! send or receive a message), an exception or a hardware interrupt; kernel
06313 ! reentries may only be made by hardware interrupts. The count of reentries
06314 ! is kept in "k_reenter". It is important for deciding whether to switch to
06315 ! the kernel stack and for protecting the message passing code in "proc.c".
06316
06317 ! For the message passing trap, most of the machine state is saved in the
06318 ! proc table. (Some of the registers need not be saved.) Then the stack is
06319 ! switched to "k_stack", and interrupts are reenabled. Finally, the system
06320 ! call handler (in C) is called. When it returns, interrupts are disabled
06321 ! again and the code falls into the restart routine, to finish off held-up
06322 ! interrupts and run the process or task whose pointer is in "proc_ptr".
06323
06324 ! Hardware interrupt handlers do the same, except (1) The entire state must
06325 ! be saved. (2) There are too many handlers to do this inline, so the save
06326 ! routine is called. A few cycles are saved by pushing the address of the
06327 ! appropiate restart routine for a return later. (3) A stack switch is
06328 ! avoided when the stack is already switched. (4) The (master) 8259 interrupt
06329 ! controller is reenabled centrally in save(). (5) Each interrupt handler
06330 ! masks its interrupt line using the 8259 before enabling (other unmasked)
06331 ! interrupts, and unmasks it after servicing the interrupt. This limits the
06332 ! nest level to the number of lines and protects the handler from itself.
06333
06334 ! For communication with the boot monitor at startup time some constant
06335 ! data are compiled into the beginning of the text segment. This facilitates
06336 ! reading the data at the start of the boot process, since only the first
06337 ! sector of the file needs to be read.
06338
06339 ! Some data storage is also allocated at the end of this file. This data
06340 ! will be at the start of the data segment of the kernel and will be read
06341 ! and modified by the boot monitor before the kernel starts.
06342
06343 ! sections
06344
06345 .sect .text
06346 begtext:
06347 .sect .rom
06348 begrom:
06349 .sect .data
708 File: kernel/mpx386.s MINIX SOURCE CODE
06350 begdata:
06351 .sect .bss
06352 begbss:
06353
06354 #include <minix/config.h>
06355 #include <minix/const.h>
06356 #include <minix/com.h>
06357 #include <ibm/interrupt.h>
06358 #include "const.h"
06359 #include "protect.h"
06360 #include "sconst.h"
06361
06362 /* Selected 386 tss offsets. */
06363 #define TSS3_S_SP0 4
06364
06365 ! Exported functions
06366 ! Note: in assembly language the .define statement applied to a function name
06367 ! is loosely equivalent to a prototype in C code -- it makes it possible to
06368 ! link to an entity declared in the assembly code but does not create
06369 ! the entity.
06370
06371 .define _restart
06372 .define save
06373
06374 .define _divide_error
06375 .define _single_step_exception
06376 .define _nmi
06377 .define _breakpoint_exception
06378 .define _overflow
06379 .define _bounds_check
06380 .define _inval_opcode
06381 .define _copr_not_available
06382 .define _double_fault
06383 .define _copr_seg_overrun
06384 .define _inval_tss
06385 .define _segment_not_present
06386 .define _stack_exception
06387 .define _general_protection
06388 .define _page_fault
06389 .define _copr_error
06390
06391 .define _hwint00 ! handlers for hardware interrupts
06392 .define _hwint01
06393 .define _hwint02
06394 .define _hwint03
06395 .define _hwint04
06396 .define _hwint05
06397 .define _hwint06
06398 .define _hwint07
06399 .define _hwint08
06400 .define _hwint09
06401 .define _hwint10
06402 .define _hwint11
06403 .define _hwint12
06404 .define _hwint13
06405 .define _hwint14
06406 .define _hwint15
06407
06408 .define _s_call
06409 .define _p_s_call
MINIX SOURCE CODE File: kernel/mpx386.s 709
06410 .define _level0_call
06411
06412 ! Exported variables.
06413 .define begbss
06414 .define begdata
06415
06416 .sect .text
06417 !*===========================================================================*
06418 !* MINIX *
06419 !*===========================================================================*
06420 MINIX: ! this is the entry point for the MINIX kernel
06421 jmp over_flags ! skip over the next few bytes
06422 .data2 CLICK_SHIFT ! for the monitor: memory granularity
06423 flags:
06424 .data2 0x01FD ! boot monitor flags:
06425 ! call in 386 mode, make bss, make stack,
06426 ! load high, don’t patch, will return,
06427 ! uses generic INT, memory vector,
06428 ! new boot code return
06429 nop ! extra byte to sync up disassembler
06430 over_flags:
06431
06432 ! Set up a C stack frame on the monitor stack. (The monitor sets cs and ds
06433 ! right. The ss descriptor still references the monitor data segment.)
06434 movzx esp, sp ! monitor stack is a 16 bit stack
06435 push ebp
06436 mov ebp, esp
06437 push esi
06438 push edi
06439 cmp 4(ebp), 0 ! monitor return vector is
06440 jz noret ! nonzero if return possible
06441 inc (_mon_return)
06442 noret: mov (_mon_sp), esp ! save stack pointer for later return
06443
06444 ! Copy the monitor global descriptor table to the address space of kernel and
06445 ! switch over to it. Prot_init() can then update it with immediate effect.
06446
06447 sgdt (_gdt+GDT_SELECTOR) ! get the monitor gdtr
06448 mov esi, (_gdt+GDT_SELECTOR+2) ! absolute address of GDT
06449 mov ebx, _gdt ! address of kernel GDT
06450 mov ecx, 8*8 ! copying eight descriptors
06451 copygdt:
06452 eseg movb al, (esi)
06453 movb (ebx), al
06454 inc esi
06455 inc ebx
06456 loop copygdt
06457 mov eax, (_gdt+DS_SELECTOR+2) ! base of kernel data
06458 and eax, 0x00FFFFFF ! only 24 bits
06459 add eax, _gdt ! eax = vir2phys(gdt)
06460 mov (_gdt+GDT_SELECTOR+2), eax ! set base of GDT
06461 lgdt (_gdt+GDT_SELECTOR) ! switch over to kernel GDT
06462
06463 ! Locate boot parameters, set up kernel segment registers and stack.
06464 mov ebx, 8(ebp) ! boot parameters offset
06465 mov edx, 12(ebp) ! boot parameters length
06466 mov eax, 16(ebp) ! address of a.out headers
06467 mov (_aout), eax
06468 mov ax, ds ! kernel data
06469 mov es, ax
710 File: kernel/mpx386.s MINIX SOURCE CODE
06470 mov fs, ax
06471 mov gs, ax
06472 mov ss, ax
06473 mov esp, k_stktop ! set sp to point to the top of kernel stack
06474
06475 ! Call C startup code to set up a proper environment to run main().
06476 push edx
06477 push ebx
06478 push SS_SELECTOR
06479 push DS_SELECTOR
06480 push CS_SELECTOR
06481 call _cstart ! cstart(cs, ds, mds, parmoff, parmlen)
06482 add esp, 5*4
06483
06484 ! Reload gdtr, idtr and the segment registers to global descriptor table set
06485 ! up by prot_init().
06486
06487 lgdt (_gdt+GDT_SELECTOR)
06488 lidt (_gdt+IDT_SELECTOR)
06489
06490 jmpf CS_SELECTOR:csinit
06491 csinit:
06492 o16 mov ax, DS_SELECTOR
06493 mov ds, ax
06494 mov es, ax
06495 mov fs, ax
06496 mov gs, ax
06497 mov ss, ax
06498 o16 mov ax, TSS_SELECTOR ! no other TSS is used
06499 ltr ax
06500 push 0 ! set flags to known good state
06501 popf ! esp, clear nested task and int enable
06502
06503 jmp _main ! main()
06504
06505
06506 !*===========================================================================*
06507 !* interrupt handlers *
06508 !* interrupt handlers for 386 32-bit protected mode *
06509 !*===========================================================================*
06510
06511 !*===========================================================================*
06512 !* hwint00 - 07 *
06513 !*===========================================================================*
06514 ! Note this is a macro, it just looks like a subroutine.
06515 #define hwint_master(irq) \
06516 call save /* save interrupted process state */;\
06517 push (_irq_handlers+4*irq) /* irq_handlers[irq] */;\
06518 call _intr_handle /* intr_handle(irq_handlers[irq]) */;\
06519 pop ecx ;\
06520 cmp (_irq_actids+4*irq), 0 /* interrupt still active? */;\
06521 jz 0f ;\
06522 inb INT_CTLMASK /* get current mask */ ;\
06523 orb al, [1<<irq] /* mask irq */ ;\
06524 outb INT_CTLMASK /* disable the irq */;\
06525 0: movb al, END_OF_INT ;\
06526 outb INT_CTL /* reenable master 8259 */;\
06527 ret /* restart (another) process */
06528
06529 ! Each of these entry points is an expansion of the hwint_master macro
MINIX SOURCE CODE File: kernel/mpx386.s 711
06530 .align 16
06531 _hwint00: ! Interrupt routine for irq 0 (the clock).
06532 hwint_master(0)
06533
06534 .align 16
06535 _hwint01: ! Interrupt routine for irq 1 (keyboard)
06536 hwint_master(1)
06537
06538 .align 16
06539 _hwint02: ! Interrupt routine for irq 2 (cascade!)
06540 hwint_master(2)
06541
06542 .align 16
06543 _hwint03: ! Interrupt routine for irq 3 (second serial)
06544 hwint_master(3)
06545
06546 .align 16
06547 _hwint04: ! Interrupt routine for irq 4 (first serial)
06548 hwint_master(4)
06549
06550 .align 16
06551 _hwint05: ! Interrupt routine for irq 5 (XT winchester)
06552 hwint_master(5)
06553
06554 .align 16
06555 _hwint06: ! Interrupt routine for irq 6 (floppy)
06556 hwint_master(6)
06557
06558 .align 16
06559 _hwint07: ! Interrupt routine for irq 7 (printer)
06560 hwint_master(7)
06561
06562 !*===========================================================================*
06563 !* hwint08 - 15 *
06564 !*===========================================================================*
06565 ! Note this is a macro, it just looks like a subroutine.
06566 #define hwint_slave(irq) \
06567 call save /* save interrupted process state */;\
06568 push (_irq_handlers+4*irq) /* irq_handlers[irq] */;\
06569 call _intr_handle /* intr_handle(irq_handlers[irq]) */;\
06570 pop ecx ;\
06571 cmp (_irq_actids+4*irq), 0 /* interrupt still active? */;\
06572 jz 0f ;\
06573 inb INT2_CTLMASK ;\
06574 orb al, [1<<[irq-8]] ;\
06575 outb INT2_CTLMASK /* disable the irq */;\
06576 0: movb al, END_OF_INT ;\
06577 outb INT_CTL /* reenable master 8259 */;\
06578 outb INT2_CTL /* reenable slave 8259 */;\
06579 ret /* restart (another) process */
06580
06581 ! Each of these entry points is an expansion of the hwint_slave macro
06582 .align 16
06583 _hwint08: ! Interrupt routine for irq 8 (realtime clock)
06584 hwint_slave(8)
06585
06586 .align 16
06587 _hwint09: ! Interrupt routine for irq 9 (irq 2 redirected)
06588 hwint_slave(9)
06589
712 File: kernel/mpx386.s MINIX SOURCE CODE
06590 .align 16
06591 _hwint10: ! Interrupt routine for irq 10
06592 hwint_slave(10)
06593
06594 .align 16
06595 _hwint11: ! Interrupt routine for irq 11
06596 hwint_slave(11)
06597
06598 .align 16
06599 _hwint12: ! Interrupt routine for irq 12
06600 hwint_slave(12)
06601
06602 .align 16
06603 _hwint13: ! Interrupt routine for irq 13 (FPU exception)
06604 hwint_slave(13)
06605
06606 .align 16
06607 _hwint14: ! Interrupt routine for irq 14 (AT winchester)
06608 hwint_slave(14)
06609
06610 .align 16
06611 _hwint15: ! Interrupt routine for irq 15
06612 hwint_slave(15)
06613
06614 !*===========================================================================*
06615 !* save *
06616 !*===========================================================================*
06617 ! Save for protected mode.
06618 ! This is much simpler than for 8086 mode, because the stack already points
06619 ! into the process table, or has already been switched to the kernel stack.
06620
06621 .align 16
06622 save:
06623 cld ! set direction flag to a known value
06624 pushad ! save "general" registers
06625 o16 push ds ! save ds
06626 o16 push es ! save es
06627 o16 push fs ! save fs
06628 o16 push gs ! save gs
06629 mov dx, ss ! ss is kernel data segment
06630 mov ds, dx ! load rest of kernel segments
06631 mov es, dx ! kernel does not use fs, gs
06632 mov eax, esp ! prepare to return
06633 incb (_k_reenter) ! from -1 if not reentering
06634 jnz set_restart1 ! stack is already kernel stack
06635 mov esp, k_stktop
06636 push _restart ! build return address for int handler
06637 xor ebp, ebp ! for stacktrace
06638 jmp RETADR-P_STACKBASE(eax)
06639
06640 .align 4
06641 set_restart1:
06642 push restart1
06643 jmp RETADR-P_STACKBASE(eax)
06644
06645 !*===========================================================================*
06646 !* _s_call *
06647 !*===========================================================================*
06648 .align 16
06649 _s_call:
MINIX SOURCE CODE File: kernel/mpx386.s 713
06650 _p_s_call:
06651 cld ! set direction flag to a known value
06652 sub esp, 6*4 ! skip RETADR, eax, ecx, edx, ebx, est
06653 push ebp ! stack already points into proc table
06654 push esi
06655 push edi
06656 o16 push ds
06657 o16 push es
06658 o16 push fs
06659 o16 push gs
06660 mov dx, ss
06661 mov ds, dx
06662 mov es, dx
06663 incb (_k_reenter)
06664 mov esi, esp ! assumes P_STACKBASE == 0
06665 mov esp, k_stktop
06666 xor ebp, ebp ! for stacktrace
06667 ! end of inline save
06668 ! now set up parameters for sys_call()
06669 push ebx ! pointer to user message
06670 push eax ! src/dest
06671 push ecx ! SEND/RECEIVE/BOTH
06672 call _sys_call ! sys_call(function, src_dest, m_ptr)
06673 ! caller is now explicitly in proc_ptr
06674 mov AXREG(esi), eax ! sys_call MUST PRESERVE si
06675
06676 ! Fall into code to restart proc/task running.
06677
06678 !*===========================================================================*
06679 !* restart *
06680 !*===========================================================================*
06681 _restart:
06682
06683 ! Restart the current process or the next process if it is set.
06684
06685 cmp (_next_ptr), 0 ! see if another process is scheduled
06686 jz 0f
06687 mov eax, (_next_ptr)
06688 mov (_proc_ptr), eax ! schedule new process
06689 mov (_next_ptr), 0
06690 0: mov esp, (_proc_ptr) ! will assume P_STACKBASE == 0
06691 lldt P_LDT_SEL(esp) ! enable process’ segment descriptors
06692 lea eax, P_STACKTOP(esp) ! arrange for next interrupt
06693 mov (_tss+TSS3_S_SP0), eax ! to save state in process table
06694 restart1:
06695 decb (_k_reenter)
06696 o16 pop gs
06697 o16 pop fs
06698 o16 pop es
06699 o16 pop ds
06700 popad
06701 add esp, 4 ! skip return adr
06702 iretd ! continue process
06703
06704 !*===========================================================================*
06705 !* exception handlers *
06706 !*===========================================================================*
06707 _divide_error:
06708 push DIVIDE_VECTOR
06709 jmp exception
714 File: kernel/mpx386.s MINIX SOURCE CODE
06710
06711 _single_step_exception:
06712 push DEBUG_VECTOR
06713 jmp exception
06714
06715 _nmi:
06716 push NMI_VECTOR
06717 jmp exception
06718
06719 _breakpoint_exception:
06720 push BREAKPOINT_VECTOR
06721 jmp exception
06722
06723 _overflow:
06724 push OVERFLOW_VECTOR
06725 jmp exception
06726
06727 _bounds_check:
06728 push BOUNDS_VECTOR
06729 jmp exception
06730
06731 _inval_opcode:
06732 push INVAL_OP_VECTOR
06733 jmp exception
06734
06735 _copr_not_available:
06736 push COPROC_NOT_VECTOR
06737 jmp exception
06738
06739 _double_fault:
06740 push DOUBLE_FAULT_VECTOR
06741 jmp errexception
06742
06743 _copr_seg_overrun:
06744 push COPROC_SEG_VECTOR
06745 jmp exception
06746
06747 _inval_tss:
06748 push INVAL_TSS_VECTOR
06749 jmp errexception
06750
06751 _segment_not_present:
06752 push SEG_NOT_VECTOR
06753 jmp errexception
06754
06755 _stack_exception:
06756 push STACK_FAULT_VECTOR
06757 jmp errexception
06758
06759 _general_protection:
06760 push PROTECTION_VECTOR
06761 jmp errexception
06762
06763 _page_fault:
06764 push PAGE_FAULT_VECTOR
06765 jmp errexception
06766
06767 _copr_error:
06768 push COPROC_ERR_VECTOR
06769 jmp exception
MINIX SOURCE CODE File: kernel/mpx386.s 715
06770
06771 !*===========================================================================*
06772 !* exception *
06773 !*===========================================================================*
06774 ! This is called for all exceptions which do not push an error code.
06775
06776 .align 16
06777 exception:
06778 sseg mov (trap_errno), 0 ! clear trap_errno
06779 sseg pop (ex_number)
06780 jmp exception1
06781
06782 !*===========================================================================*
06783 !* errexception *
06784 !*===========================================================================*
06785 ! This is called for all exceptions which push an error code.
06786
06787 .align 16
06788 errexception:
06789 sseg pop (ex_number)
06790 sseg pop (trap_errno)
06791 exception1: ! Common for all exceptions.
06792 push eax ! eax is scratch register
06793 mov eax, 0+4(esp) ! old eip
06794 sseg mov (old_eip), eax
06795 movzx eax, 4+4(esp) ! old cs
06796 sseg mov (old_cs), eax
06797 mov eax, 8+4(esp) ! old eflags
06798 sseg mov (old_eflags), eax
06799 pop eax
06800 call save
06801 push (old_eflags)
06802 push (old_cs)
06803 push (old_eip)
06804 push (trap_errno)
06805 push (ex_number)
06806 call _exception ! (ex_number, trap_errno, old_eip,
06807 ! old_cs, old_eflags)
06808 add esp, 5*4
06809 ret
06810
06811 !*===========================================================================*
06812 !* level0_call *
06813 !*===========================================================================*
06814 _level0_call:
06815 call save
06816 jmp (_level0_func)
06817
06818 !*===========================================================================*
06819 !* data *
06820 !*===========================================================================*
06821
06822 .sect .rom ! Before the string table please
06823 .data2 0x526F ! this must be the first data entry (magic #)
06824
06825 .sect .bss
06826 k_stack:
06827 .space K_STACK_BYTES ! kernel stack
06828 k_stktop: ! top of kernel stack
06829 .comm ex_number, 4
716 File: kernel/mpx386.s MINIX SOURCE CODE
06830 .comm trap_errno, 4
06831 .comm old_eip, 4
06832 .comm old_cs, 4
06833 .comm old_eflags, 4
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/start.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
06900 /* This file contains the C startup code for Minix on Intel processors.
06901 * It cooperates with mpx.s to set up a good environment for main().
06902 *
06903 * This code runs in real mode for a 16 bit kernel and may have to switch
06904 * to protected mode for a 286.
06905 * For a 32 bit kernel this already runs in protected mode, but the selectors
06906 * are still those given by the BIOS with interrupts disabled, so the
06907 * descriptors need to be reloaded and interrupt descriptors made.
06908 */
06909
06910 #include "kernel.h"
06911 #include "protect.h"
06912 #include "proc.h"
06913 #include <stdlib.h>
06914 #include <string.h>
06915
06916 FORWARD _PROTOTYPE( char *get_value, (_CONST char *params, _CONST char *key));
06917 /*===========================================================================*
06918 * cstart *
06919 *===========================================================================*/
06920 PUBLIC void cstart(cs, ds, mds, parmoff, parmsize)
06921 U16_t cs, ds; /* kernel code and data segment */
06922 U16_t mds; /* monitor data segment */
06923 U16_t parmoff, parmsize; /* boot parameters offset and length */
06924 {
06925 /* Perform system initializations prior to calling main(). Most settings are
06926 * determined with help of the environment strings passed by MINIX’ loader.
06927 */
06928 char params[128*sizeof(char *)]; /* boot monitor parameters */
06929 register char *value; /* value in key=value pair */
06930 extern int etext, end;
06931
06932 /* Decide if mode is protected; 386 or higher implies protected mode.
06933 * This must be done first, because it is needed for, e.g., seg2phys().
06934 * For 286 machines we cannot decide on protected mode, yet. This is
06935 * done below.
06936 */
06937 #if _WORD_SIZE != 2
06938 machine.protected = 1;
06939 #endif
06940
06941 /* Record where the kernel and the monitor are. */
06942 kinfo.code_base = seg2phys(cs);
06943 kinfo.code_size = (phys_bytes) &etext; /* size of code segment */
06944 kinfo.data_base = seg2phys(ds);
06945 kinfo.data_size = (phys_bytes) &end; /* size of data segment */
06946
06947 /* Initialize protected mode descriptors. */
06948 prot_init();
06949
MINIX SOURCE CODE File: kernel/start.c 717
06950 /* Copy the boot parameters to the local buffer. */
06951 kinfo.params_base = seg2phys(mds) + parmoff;
06952 kinfo.params_size = MIN(parmsize,sizeof(params)-2);
06953 phys_copy(kinfo.params_base, vir2phys(params), kinfo.params_size);
06954
06955 /* Record miscellaneous information for user-space servers. */
06956 kinfo.nr_procs = NR_PROCS;
06957 kinfo.nr_tasks = NR_TASKS;
06958 strncpy(kinfo.release, OS_RELEASE, sizeof(kinfo.release));
06959 kinfo.release[sizeof(kinfo.release)-1] = ’\0’;
06960 strncpy(kinfo.version, OS_VERSION, sizeof(kinfo.version));
06961 kinfo.version[sizeof(kinfo.version)-1] = ’\0’;
06962 kinfo.proc_addr = (vir_bytes) proc;
06963 kinfo.kmem_base = vir2phys(0);
06964 kinfo.kmem_size = (phys_bytes) &end;
06965
06966 /* Processor? 86, 186, 286, 386, ...
06967 * Decide if mode is protected for older machines.
06968 */
06969 machine.processor=atoi(get_value(params, "processor"));
06970 #if _WORD_SIZE == 2
06971 machine.protected = machine.processor >= 286;
06972 #endif
06973 if (! machine.protected) mon_return = 0;
06974
06975 /* XT, AT or MCA bus? */
06976 value = get_value(params, "bus");
06977 if (value == NIL_PTR || strcmp(value, "at") == 0) {
06978 machine.pc_at = TRUE; /* PC-AT compatible hardware */
06979 } else if (strcmp(value, "mca") == 0) {
06980 machine.pc_at = machine.ps_mca = TRUE; /* PS/2 with micro channel */
06981 }
06982
06983 /* Type of VDU: */
06984 value = get_value(params, "video"); /* EGA or VGA video unit */
06985 if (strcmp(value, "ega") == 0) machine.vdu_ega = TRUE;
06986 if (strcmp(value, "vga") == 0) machine.vdu_vga = machine.vdu_ega = TRUE;
06987
06988 /* Return to assembler code to switch to protected mode (if 286),
06989 * reload selectors and call main().
06990 */
06991 }
06993 /*===========================================================================*
06994 * get_value *
06995 *===========================================================================*/
06996
06997 PRIVATE char *get_value(params, name)
06998 _CONST char *params; /* boot monitor parameters */
06999 _CONST char *name; /* key to look up */
07000 {
07001 /* Get environment value - kernel version of getenv to avoid setting up the
07002 * usual environment array.
07003 */
07004 register _CONST char *namep;
07005 register char *envp;
07006
07007 for (envp = (char *) params; *envp != 0;) {
07008 for (namep = name; *namep != 0 && *namep == *envp; namep++, envp++)
07009 ;
718 File: kernel/start.c MINIX SOURCE CODE
07010 if (*namep == ’\0’ && *envp == ’=’) return(envp + 1);
07011 while (*envp++ != 0)
07012 ;
07013 }
07014 return(NIL_PTR);
07015 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
07100 /* This file contains the main program of MINIX as well as its shutdown code.
07101 * The routine main() initializes the system and starts the ball rolling by
07102 * setting up the process table, interrupt vectors, and scheduling each task
07103 * to run to initialize itself.
07104 * The routine shutdown() does the opposite and brings down MINIX.
07105 *
07106 * The entries into this file are:
07107 * main: MINIX main program
07108 * prepare_shutdown: prepare to take MINIX down
07109 *
07110 * Changes:
07111 * Nov 24, 2004 simplified main() with system image (Jorrit N. Herder)
07112 * Aug 20, 2004 new prepare_shutdown() and shutdown() (Jorrit N. Herder)
07113 */
07114 #include "kernel.h"
07115 #include <signal.h>
07116 #include <string.h>
07117 #include <unistd.h>
07118 #include <a.out.h>
07119 #include <minix/callnr.h>
07120 #include <minix/com.h>
07121 #include "proc.h"
07122
07123 /* Prototype declarations for PRIVATE functions. */
07124 FORWARD _PROTOTYPE( void announce, (void));
07125 FORWARD _PROTOTYPE( void shutdown, (timer_t *tp));
07126
07127 /*===========================================================================*
07128 * main *
07129 *===========================================================================*/
07130 PUBLIC void main()
07131 {
07132 /* Start the ball rolling. */
07133 struct boot_image *ip; /* boot image pointer */
07134 register struct proc *rp; /* process pointer */
07135 register struct priv *sp; /* privilege structure pointer */
07136 register int i, s;
07137 int hdrindex; /* index to array of a.out headers */
07138 phys_clicks text_base;
07139 vir_clicks text_clicks, data_clicks;
07140 reg_t ktsb; /* kernel task stack base */
07141 struct exec e_hdr; /* for a copy of an a.out header */
07142
07143 /* Initialize the interrupt controller. */
07144 intr_init(1);
MINIX SOURCE CODE File: kernel/main.c 719
07145
07146 /* Clear the process table. Anounce each slot as empty and set up mappings
07147 * for proc_addr() and proc_nr() macros. Do the same for the table with
07148 * privilege structures for the system processes.
07149 */
07150 for (rp = BEG_PROC_ADDR, i = -NR_TASKS; rp < END_PROC_ADDR; ++rp, ++i) {
07151 rp->p_rts_flags = SLOT_FREE; /* initialize free slot */
07152 rp->p_nr = i; /* proc number from ptr */
07153 (pproc_addr + NR_TASKS)[i] = rp; /* proc ptr from number */
07154 }
07155 for (sp = BEG_PRIV_ADDR, i = 0; sp < END_PRIV_ADDR; ++sp, ++i) {
07156 sp->s_proc_nr = NONE; /* initialize as free */
07157 sp->s_id = i; /* priv structure index */
07158 ppriv_addr[i] = sp; /* priv ptr from number */
07159 }
07160
07161 /* Set up proc table entries for tasks and servers. The stacks of the
07162 * kernel tasks are initialized to an array in data space. The stacks
07163 * of the servers have been added to the data segment by the monitor, so
07164 * the stack pointer is set to the end of the data segment. All the
07165 * processes are in low memory on the 8086. On the 386 only the kernel
07166 * is in low memory, the rest is loaded in extended memory.
07167 */
07168
07169 /* Task stacks. */
07170 ktsb = (reg_t) t_stack;
07171
07172 for (i=0; i < NR_BOOT_PROCS; ++i) {
07173 ip = &image[i]; /* process’ attributes */
07174 rp = proc_addr(ip->proc_nr); /* get process pointer */
07175 rp->p_max_priority = ip->priority; /* max scheduling priority */
07176 rp->p_priority = ip->priority; /* current priority */
07177 rp->p_quantum_size = ip->quantum; /* quantum size in ticks */
07178 rp->p_ticks_left = ip->quantum; /* current credit */
07179 strncpy(rp->p_name, ip->proc_name, P_NAME_LEN); /* set process name */
07180 (void) get_priv(rp, (ip->flags & SYS_PROC)); /* assign structure */
07181 priv(rp)->s_flags = ip->flags; /* process flags */
07182 priv(rp)->s_trap_mask = ip->trap_mask; /* allowed traps */
07183 priv(rp)->s_call_mask = ip->call_mask; /* kernel call mask */
07184 priv(rp)->s_ipc_to.chunk[0] = ip->ipc_to; /* restrict targets */
07185 if (iskerneln(proc_nr(rp))) { /* part of the kernel? */
07186 if (ip->stksize > 0) { /* HARDWARE stack size is 0 */
07187 rp->p_priv->s_stack_guard = (reg_t *) ktsb;
07188 *rp->p_priv->s_stack_guard = STACK_GUARD;
07189 }
07190 ktsb += ip->stksize; /* point to high end of stack */
07191 rp->p_reg.sp = ktsb; /* this task’s initial stack ptr */
07192 text_base = kinfo.code_base >> CLICK_SHIFT;
07193 /* processes that are in the kernel */
07194 hdrindex = 0; /* all use the first a.out header */
07195 } else {
07196 hdrindex = 1 + i-NR_TASKS; /* servers, drivers, INIT */
07197 }
07198
07199 /* The bootstrap loader created an array of the a.out headers at
07200 * absolute address ’aout’. Get one element to e_hdr.
07201 */
07202 phys_copy(aout + hdrindex * A_MINHDR, vir2phys(&e_hdr),
07203 (phys_bytes) A_MINHDR);
07204 /* Convert addresses to clicks and build process memory map */
720 File: kernel/main.c MINIX SOURCE CODE
07205 text_base = e_hdr.a_syms >> CLICK_SHIFT;
07206 text_clicks = (e_hdr.a_text + CLICK_SIZE-1) >> CLICK_SHIFT;
07207 if (!(e_hdr.a_flags & A_SEP)) text_clicks = 0; /* common I&D */
07208 data_clicks = (e_hdr.a_total + CLICK_SIZE-1) >> CLICK_SHIFT;
07209 rp->p_memmap[T].mem_phys = text_base;
07210 rp->p_memmap[T].mem_len = text_clicks;
07211 rp->p_memmap[D].mem_phys = text_base + text_clicks;
07212 rp->p_memmap[D].mem_len = data_clicks;
07213 rp->p_memmap[S].mem_phys = text_base + text_clicks + data_clicks;
07214 rp->p_memmap[S].mem_vir = data_clicks; /* empty - stack is in data */
07215
07216 /* Set initial register values. The processor status word for tasks
07217 * is different from that of other processes because tasks can
07218 * access I/O; this is not allowed to less-privileged processes
07219 */
07220 rp->p_reg.pc = (reg_t) ip->initial_pc;
07221 rp->p_reg.psw = (iskernelp(rp)) ? INIT_TASK_PSW : INIT_PSW;
07222
07223 /* Initialize the server stack pointer. Take it down one word
07224 * to give crtso.s something to use as "argc".
07225 */
07226 if (isusern(proc_nr(rp))) { /* user-space process? */
07227 rp->p_reg.sp = (rp->p_memmap[S].mem_vir +
07228 rp->p_memmap[S].mem_len) << CLICK_SHIFT;
07229 rp->p_reg.sp -= sizeof(reg_t);
07230 }
07231
07232 /* Set ready. The HARDWARE task is never ready. */
07233 if (rp->p_nr != HARDWARE) {
07234 rp->p_rts_flags = 0; /* runnable if no flags */
07235 lock_enqueue(rp); /* add to scheduling queues */
07236 } else {
07237 rp->p_rts_flags = NO_MAP; /* prevent from running */
07238 }
07239
07240 /* Code and data segments must be allocated in protected mode. */
07241 alloc_segments(rp);
07242 }
07243
07244 /* We’re definitely not shutting down. */
07245 shutdown_started = 0;
07246
07247 /* MINIX is now ready. All boot image processes are on the ready queue.
07248 * Return to the assembly code to start running the current process.
07249 */
07250 bill_ptr = proc_addr(IDLE); /* it has to point somewhere */
07251 announce(); /* print MINIX startup banner */
07252 restart();
07253 }
07255 /*===========================================================================*
07256 * announce *
07257 *===========================================================================*/
07258 PRIVATE void announce(void)
07259 {
07260 /* Display the MINIX startup banner. */
07261 kprintf("MINIX %s.%s."
07262 "Copyright 2006, Vrije Universiteit, Amsterdam, The Netherlands\n",
07263 OS_RELEASE, OS_VERSION);
07264
MINIX SOURCE CODE File: kernel/main.c 721
07265 /* Real mode, or 16/32-bit protected mode? */
07266 kprintf("Executing in %s mode.\n\n",
07267 machine.protected ? "32-bit protected" : "real");
07268 }
07270 /*===========================================================================*
07271 * prepare_shutdown *
07272 *===========================================================================*/
07273 PUBLIC void prepare_shutdown(how)
07274 int how;
07275 {
07276 /* This function prepares to shutdown MINIX. */
07277 static timer_t shutdown_timer;
07278 register struct proc *rp;
07279 message m;
07280
07281 /* Show debugging dumps on panics. Make sure that the TTY task is still
07282 * available to handle them. This is done with help of a non-blocking send.
07283 * We rely on TTY to call sys_abort() when it is done with the dumps.
07284 */
07285 if (how == RBT_PANIC) {
07286 m.m_type = PANIC_DUMPS;
07287 if (nb_send(TTY_PROC_NR,&m)==OK) /* don’t block if TTY isn’t ready */
07288 return; /* await sys_abort() from TTY */
07289 }
07290
07291 /* Send a signal to all system processes that are still alive to inform
07292 * them that the MINIX kernel is shutting down. A proper shutdown sequence
07293 * should be implemented by a user-space server. This mechanism is useful
07294 * as a backup in case of system panics, so that system processes can still
07295 * run their shutdown code, e.g, to synchronize the FS or to let the TTY
07296 * switch to the first console.
07297 */
07298 kprintf("Sending SIGKSTOP to system processes ...\n");
07299 for (rp=BEG_PROC_ADDR; rp<END_PROC_ADDR; rp++) {
07300 if (!isemptyp(rp) && (priv(rp)->s_flags & SYS_PROC) && !iskernelp(rp))
07301 send_sig(proc_nr(rp), SIGKSTOP);
07302 }
07303
07304 /* We’re shutting down. Diagnostics may behave differently now. */
07305 shutdown_started = 1;
07306
07307 /* Notify system processes of the upcoming shutdown and allow them to be
07308 * scheduled by setting a watchog timer that calls shutdown(). The timer
07309 * argument passes the shutdown status.
07310 */
07311 kprintf("MINIX will now be shut down ...\n");
07312 tmr_arg(&shutdown_timer)->ta_int = how;
07313
07314 /* Continue after 1 second, to give processes a chance to get
07315 * scheduled to do shutdown work.
07316 */
07317 set_timer(&shutdown_timer, get_uptime() + HZ, shutdown);
07318 }
07320 /*===========================================================================*
07321 * shutdown *
07322 *===========================================================================*/
07323 PRIVATE void shutdown(tp)
07324 timer_t *tp;
722 File: kernel/main.c MINIX SOURCE CODE
07325 {
07326 /* This function is called from prepare_shutdown or stop_sequence to bring
07327 * down MINIX. How to shutdown is in the argument: RBT_HALT (return to the
07328 * monitor), RBT_MONITOR (execute given code), RBT_RESET (hard reset).
07329 */
07330 int how = tmr_arg(tp)->ta_int;
07331 u16_t magic;
07332
07333 /* Now mask all interrupts, including the clock, and stop the clock. */
07334 outb(INT_CTLMASK, ˜0);
07335 clock_stop();
07336
07337 if (mon_return && how != RBT_RESET) {
07338 /* Reinitialize the interrupt controllers to the BIOS defaults. */
07339 intr_init(0);
07340 outb(INT_CTLMASK, 0);
07341 outb(INT2_CTLMASK, 0);
07342
07343 /* Return to the boot monitor. Set the program if not already done. */
07344 if (how != RBT_MONITOR) phys_copy(vir2phys(""), kinfo.params_base, 1);
07345 level0(monitor);
07346 }
07347
07348 /* Reset the system by jumping to the reset address (real mode), or by
07349 * forcing a processor shutdown (protected mode). First stop the BIOS
07350 * memory test by setting a soft reset flag.
07351 */
07352 magic = STOP_MEM_CHECK;
07353 phys_copy(vir2phys(&magic), SOFT_RESET_FLAG_ADDR, SOFT_RESET_FLAG_SIZE);
07354 level0(reset);
07355 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/proc.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
07400 /* This file contains essentially all of the process and message handling.
07401 * Together with "mpx.s" it forms the lowest layer of the MINIX kernel.
07402 * There is one entry point from the outside:
07403 *
07404 * sys_call: a system call, i.e., the kernel is trapped with an INT
07405 *
07406 * As well as several entry points used from the interrupt and task level:
07407 *
07408 * lock_notify: notify a process of a system event
07409 * lock_send: send a message to a process
07410 * lock_enqueue: put a process on one of the scheduling queues
07411 * lock_dequeue: remove a process from the scheduling queues
07412 *
07413 * Changes:
07414 * Aug 19, 2005 rewrote scheduling code (Jorrit N. Herder)
07415 * Jul 25, 2005 rewrote system call handling (Jorrit N. Herder)
07416 * May 26, 2005 rewrote message passing functions (Jorrit N. Herder)
07417 * May 24, 2005 new notification system call (Jorrit N. Herder)
07418 * Oct 28, 2004 nonblocking send and receive calls (Jorrit N. Herder)
07419 *
MINIX SOURCE CODE File: kernel/proc.c 723
07420 * The code here is critical to make everything work and is important for the
07421 * overall performance of the system. A large fraction of the code deals with
07422 * list manipulation. To make this both easy to understand and fast to execute
07423 * pointer pointers are used throughout the code. Pointer pointers prevent
07424 * exceptions for the head or tail of a linked list.
07425 *
07426 * node_t *queue, *new_node; // assume these as global variables
07427 * node_t **xpp = &queue; // get pointer pointer to head of queue
07428 * while (*xpp != NULL) // find last pointer of the linked list
07429 * xpp = &(*xpp)->next; // get pointer to next pointer
07430 * *xpp = new_node; // now replace the end (the NULL pointer)
07431 * new_node->next = NULL; // and mark the new end of the list
07432 *
07433 * For example, when adding a new node to the end of the list, one normally
07434 * makes an exception for an empty list and looks up the end of the list for
07435 * nonempty lists. As shown above, this is not required with pointer pointers.
07436 */
07437
07438 #include <minix/com.h>
07439 #include <minix/callnr.h>
07440 #include "kernel.h"
07441 #include "proc.h"
07442
07443 /* Scheduling and message passing functions. The functions are available to
07444 * other parts of the kernel through lock_...(). The lock temporarily disables
07445 * interrupts to prevent race conditions.
07446 */
07447 FORWARD _PROTOTYPE( int mini_send, (struct proc *caller_ptr, int dst,
07448 message *m_ptr, unsigned flags) );
07449 FORWARD _PROTOTYPE( int mini_receive, (struct proc *caller_ptr, int src,
07450 message *m_ptr, unsigned flags) );
07451 FORWARD _PROTOTYPE( int mini_notify, (struct proc *caller_ptr, int dst) );
07452
07453 FORWARD _PROTOTYPE( void enqueue, (struct proc *rp) );
07454 FORWARD _PROTOTYPE( void dequeue, (struct proc *rp) );
07455 FORWARD _PROTOTYPE( void sched, (struct proc *rp, int *queue, int *front) );
07456 FORWARD _PROTOTYPE( void pick_proc, (void) );
07457
07458 #define BuildMess(m_ptr, src, dst_ptr) \
07459 (m_ptr)->m_source = (src); \
07460 (m_ptr)->m_type = NOTIFY_FROM(src); \
07461 (m_ptr)->NOTIFY_TIMESTAMP = get_uptime(); \
07462 switch (src) { \
07463 case HARDWARE: \
07464 (m_ptr)->NOTIFY_ARG = priv(dst_ptr)->s_int_pending; \
07465 priv(dst_ptr)->s_int_pending = 0; \
07466 break; \
07467 case SYSTEM: \
07468 (m_ptr)->NOTIFY_ARG = priv(dst_ptr)->s_sig_pending; \
07469 priv(dst_ptr)->s_sig_pending = 0; \
07470 break; \
07471 }
07472
07473 #define CopyMess(s,sp,sm,dp,dm) \
07474 cp_mess(s, (sp)->p_memmap[D].mem_phys, \
07475 (vir_bytes)sm, (dp)->p_memmap[D].mem_phys, (vir_bytes)dm)
07476
724 File: kernel/proc.c MINIX SOURCE CODE
07477 /*===========================================================================*
07478 * sys_call *
07479 *===========================================================================*/
07480 PUBLIC int sys_call(call_nr, src_dst, m_ptr)
07481 int call_nr; /* system call number and flags */
07482 int src_dst; /* src to receive from or dst to send to */
07483 message *m_ptr; /* pointer to message in the caller’s space */
07484 {
07485 /* System calls are done by trapping to the kernel with an INT instruction.
07486 * The trap is caught and sys_call() is called to send or receive a message
07487 * (or both). The caller is always given by ’proc_ptr’.
07488 */
07489 register struct proc *caller_ptr = proc_ptr; /* get pointer to caller */
07490 int function = call_nr & SYSCALL_FUNC; /* get system call function */
07491 unsigned flags = call_nr & SYSCALL_FLAGS; /* get flags */
07492 int mask_entry; /* bit to check in send mask */
07493 int result; /* the system call’s result */
07494 vir_clicks vlo, vhi; /* virtual clicks containing message to send */
07495
07496 /* Check if the process has privileges for the requested call. Calls to the
07497 * kernel may only be SENDREC, because tasks always reply and may not block
07498 * if the caller doesn’t do receive().
07499 */
07500 if (! (priv(caller_ptr)->s_trap_mask & (1 << function)) ||
07501 (iskerneln(src_dst) && function != SENDREC
07502 && function != RECEIVE)) {
07503 kprintf("sys_call: trap %d not allowed, caller %d, src_dst %d\n",
07504 function, proc_nr(caller_ptr), src_dst);
07505 return(ECALLDENIED); /* trap denied by mask or kernel */
07506 }
07507
07508 /* Require a valid source and/ or destination process, unless echoing. */
07509 if (! (isokprocn(src_dst) || src_dst == ANY || function == ECHO)) {
07510 kprintf("sys_call: invalid src_dst, src_dst %d, caller %d\n",
07511 src_dst, proc_nr(caller_ptr));
07512 return(EBADSRCDST); /* invalid process number */
07513 }
07514
07515 /* If the call involves a message buffer, i.e., for SEND, RECEIVE, SENDREC,
07516 * or ECHO, check the message pointer. This check allows a message to be
07517 * anywhere in data or stack or gap. It will have to be made more elaborate
07518 * for machines which don’t have the gap mapped.
07519 */
07520 if (function & CHECK_PTR) {
07521 vlo = (vir_bytes) m_ptr >> CLICK_SHIFT;
07522 vhi = ((vir_bytes) m_ptr + MESS_SIZE - 1) >> CLICK_SHIFT;
07523 if (vlo < caller_ptr->p_memmap[D].mem_vir || vlo > vhi ||
07524 vhi >= caller_ptr->p_memmap[S].mem_vir +
07525 caller_ptr->p_memmap[S].mem_len) {
07526 kprintf("sys_call: invalid message pointer, trap %d, caller %d\n",
07527 function, proc_nr(caller_ptr));
07528 return(EFAULT); /* invalid message pointer */
07529 }
07530 }
07531
07532 /* If the call is to send to a process, i.e., for SEND, SENDREC or NOTIFY,
07533 * verify that the caller is allowed to send to the given destination and
07534 * that the destination is still alive.
07535 */
07536 if (function & CHECK_DST) {
MINIX SOURCE CODE File: kernel/proc.c 725
07537 if (! get_sys_bit(priv(caller_ptr)->s_ipc_to, nr_to_id(src_dst))) {
07538 kprintf("sys_call: ipc mask denied %d sending to %d\n",
07539 proc_nr(caller_ptr), src_dst);
07540 return(ECALLDENIED); /* call denied by ipc mask */
07541 }
07542
07543 if (isemptyn(src_dst) && !shutdown_started) {
07544 kprintf("sys_call: dead dest; %d, %d, %d\n",
07545 function, proc_nr(caller_ptr), src_dst);
07546 return(EDEADDST); /* cannot send to the dead */
07547 }
07548 }
07549
07550 /* Now check if the call is known and try to perform the request. The only
07551 * system calls that exist in MINIX are sending and receiving messages.
07552 * - SENDREC: combines SEND and RECEIVE in a single system call
07553 * - SEND: sender blocks until its message has been delivered
07554 * - RECEIVE: receiver blocks until an acceptable message has arrived
07555 * - NOTIFY: nonblocking call; deliver notification or mark pending
07556 * - ECHO: nonblocking call; directly echo back the message
07557 */
07558 switch(function) {
07559 case SENDREC:
07560 /* A flag is set so that notifications cannot interrupt SENDREC. */
07561 priv(caller_ptr)->s_flags |= SENDREC_BUSY;
07562 /* fall through */
07563 case SEND:
07564 result = mini_send(caller_ptr, src_dst, m_ptr, flags);
07565 if (function == SEND || result != OK) {
07566 break; /* done, or SEND failed */
07567 } /* fall through for SENDREC */
07568 case RECEIVE:
07569 if (function == RECEIVE)
07570 priv(caller_ptr)->s_flags &= ˜SENDREC_BUSY;
07571 result = mini_receive(caller_ptr, src_dst, m_ptr, flags);
07572 break;
07573 case NOTIFY:
07574 result = mini_notify(caller_ptr, src_dst);
07575 break;
07576 case ECHO:
07577 CopyMess(caller_ptr->p_nr, caller_ptr, m_ptr, caller_ptr, m_ptr);
07578 result = OK;
07579 break;
07580 default:
07581 result = EBADCALL; /* illegal system call */
07582 }
07583
07584 /* Now, return the result of the system call to the caller. */
07585 return(result);
07586 }
07588 /*===========================================================================*
07589 * mini_send *
07590 *===========================================================================*/
07591 PRIVATE int mini_send(caller_ptr, dst, m_ptr, flags)
07592 register struct proc *caller_ptr; /* who is trying to send a message? */
07593 int dst; /* to whom is message being sent? */
07594 message *m_ptr; /* pointer to message buffer */
07595 unsigned flags; /* system call flags */
07596 {
726 File: kernel/proc.c MINIX SOURCE CODE
07597 /* Send a message from ’caller_ptr’ to ’dst’. If ’dst’ is blocked waiting
07598 * for this message, copy the message to it and unblock ’dst’. If ’dst’ is
07599 * not waiting at all, or is waiting for another source, queue ’caller_ptr’.
07600 */
07601 register struct proc *dst_ptr = proc_addr(dst);
07602 register struct proc **xpp;
07603 register struct proc *xp;
07604
07605 /* Check for deadlock by ’caller_ptr’ and ’dst’ sending to each other. */
07606 xp = dst_ptr;
07607 while (xp->p_rts_flags & SENDING) { /* check while sending */
07608 xp = proc_addr(xp->p_sendto); /* get xp’s destination */
07609 if (xp == caller_ptr) return(ELOCKED); /* deadlock if cyclic */
07610 }
07611
07612 /* Check if ’dst’ is blocked waiting for this message. The destination’s
07613 * SENDING flag may be set when its SENDREC call blocked while sending.
07614 */
07615 if ( (dst_ptr->p_rts_flags & (RECEIVING | SENDING)) == RECEIVING &&
07616 (dst_ptr->p_getfrom == ANY || dst_ptr->p_getfrom == caller_ptr->p_nr)) {
07617 /* Destination is indeed waiting for this message. */
07618 CopyMess(caller_ptr->p_nr, caller_ptr, m_ptr, dst_ptr,
07619 dst_ptr->p_messbuf);
07620 if ((dst_ptr->p_rts_flags &= ˜RECEIVING) == 0) enqueue(dst_ptr);
07621 } else if ( ! (flags & NON_BLOCKING)) {
07622 /* Destination is not waiting. Block and dequeue caller. */
07623 caller_ptr->p_messbuf = m_ptr;
07624 if (caller_ptr->p_rts_flags == 0) dequeue(caller_ptr);
07625 caller_ptr->p_rts_flags |= SENDING;
07626 caller_ptr->p_sendto = dst;
07627
07628 /* Process is now blocked. Put in on the destination’s queue. */
07629 xpp = &dst_ptr->p_caller_q; /* find end of list */
07630 while (*xpp != NIL_PROC) xpp = &(*xpp)->p_q_link;
07631 *xpp = caller_ptr; /* add caller to end */
07632 caller_ptr->p_q_link = NIL_PROC; /* mark new end of list */
07633 } else {
07634 return(ENOTREADY);
07635 }
07636 return(OK);
07637 }
07639 /*===========================================================================*
07640 * mini_receive *
07641 *===========================================================================*/
07642 PRIVATE int mini_receive(caller_ptr, src, m_ptr, flags)
07643 register struct proc *caller_ptr; /* process trying to get message */
07644 int src; /* which message source is wanted */
07645 message *m_ptr; /* pointer to message buffer */
07646 unsigned flags; /* system call flags */
07647 {
07648 /* A process or task wants to get a message. If a message is already queued,
07649 * acquire it and deblock the sender. If no message from the desired source
07650 * is available block the caller, unless the flags don’t allow blocking.
07651 */
07652 register struct proc **xpp;
07653 register struct notification **ntf_q_pp;
07654 message m;
07655 int bit_nr;
07656 sys_map_t *map;
MINIX SOURCE CODE File: kernel/proc.c 727
07657 bitchunk_t *chunk;
07658 int i, src_id, src_proc_nr;
07659
07660 /* Check to see if a message from desired source is already available.
07661 * The caller’s SENDING flag may be set if SENDREC couldn’t send. If it is
07662 * set, the process should be blocked.
07663 */
07664 if (!(caller_ptr->p_rts_flags & SENDING)) {
07665
07666 /* Check if there are pending notifications, except for SENDREC. */
07667 if (! (priv(caller_ptr)->s_flags & SENDREC_BUSY)) {
07668
07669 map = &priv(caller_ptr)->s_notify_pending;
07670 for (chunk=&map->chunk[0]; chunk<&map->chunk[NR_SYS_CHUNKS]; chunk++) {
07671
07672 /* Find a pending notification from the requested source. */
07673 if (! *chunk) continue; /* no bits in chunk */
07674 for (i=0; ! (*chunk & (1<<i)); ++i) {} /* look up the bit */
07675 src_id = (chunk - &map->chunk[0]) * BITCHUNK_BITS + i;
07676 if (src_id >= NR_SYS_PROCS) break; /* out of range */
07677 src_proc_nr = id_to_nr(src_id); /* get source proc */
07678 if (src!=ANY && src!=src_proc_nr) continue; /* source not ok */
07679 *chunk &= ˜(1 << i); /* no longer pending */
07680
07681 /* Found a suitable source, deliver the notification message. */
07682 BuildMess(&m, src_proc_nr, caller_ptr); /* assemble message */
07683 CopyMess(src_proc_nr, proc_addr(HARDWARE), &m, caller_ptr, m_ptr);
07684 return(OK); /* report success */
07685 }
07686 }
07687
07688 /* Check caller queue. Use pointer pointers to keep code simple. */
07689 xpp = &caller_ptr->p_caller_q;
07690 while (*xpp != NIL_PROC) {
07691 if (src == ANY || src == proc_nr(*xpp)) {
07692 /* Found acceptable message. Copy it and update status. */
07693 CopyMess((*xpp)->p_nr, *xpp, (*xpp)->p_messbuf, caller_ptr, m_ptr);
07694 if (((*xpp)->p_rts_flags &= ˜SENDING) == 0) enqueue(*xpp);
07695 *xpp = (*xpp)->p_q_link; /* remove from queue */
07696 return(OK); /* report success */
07697 }
07698 xpp = &(*xpp)->p_q_link; /* proceed to next */
07699 }
07700 }
07701
07702 /* No suitable message is available or the caller couldn’t send in SENDREC.
07703 * Block the process trying to receive, unless the flags tell otherwise.
07704 */
07705 if ( ! (flags & NON_BLOCKING)) {
07706 caller_ptr->p_getfrom = src;
07707 caller_ptr->p_messbuf = m_ptr;
07708 if (caller_ptr->p_rts_flags == 0) dequeue(caller_ptr);
07709 caller_ptr->p_rts_flags |= RECEIVING;
07710 return(OK);
07711 } else {
07712 return(ENOTREADY);
07713 }
07714 }
728 File: kernel/proc.c MINIX SOURCE CODE
07716 /*===========================================================================*
07717 * mini_notify *
07718 *===========================================================================*/
07719 PRIVATE int mini_notify(caller_ptr, dst)
07720 register struct proc *caller_ptr; /* sender of the notification */
07721 int dst; /* which process to notify */
07722 {
07723 register struct proc *dst_ptr = proc_addr(dst);
07724 int src_id; /* source id for late delivery */
07725 message m; /* the notification message */
07726
07727 /* Check to see if target is blocked waiting for this message. A process
07728 * can be both sending and receiving during a SENDREC system call.
07729 */
07730 if ((dst_ptr->p_rts_flags & (RECEIVING|SENDING)) == RECEIVING &&
07731 ! (priv(dst_ptr)->s_flags & SENDREC_BUSY) &&
07732 (dst_ptr->p_getfrom == ANY || dst_ptr->p_getfrom == caller_ptr->p_nr)) {
07733
07734 /* Destination is indeed waiting for a message. Assemble a notification
07735 * message and deliver it. Copy from pseudo-source HARDWARE, since the
07736 * message is in the kernel’s address space.
07737 */
07738 BuildMess(&m, proc_nr(caller_ptr), dst_ptr);
07739 CopyMess(proc_nr(caller_ptr), proc_addr(HARDWARE), &m,
07740 dst_ptr, dst_ptr->p_messbuf);
07741 dst_ptr->p_rts_flags &= ˜RECEIVING; /* deblock destination */
07742 if (dst_ptr->p_rts_flags == 0) enqueue(dst_ptr);
07743 return(OK);
07744 }
07745
07746 /* Destination is not ready to receive the notification. Add it to the
07747 * bit map with pending notifications. Note the indirectness: the system id
07748 * instead of the process number is used in the pending bit map.
07749 */
07750 src_id = priv(caller_ptr)->s_id;
07751 set_sys_bit(priv(dst_ptr)->s_notify_pending, src_id);
07752 return(OK);
07753 }
07755 /*===========================================================================*
07756 * lock_notify *
07757 *===========================================================================*/
07758 PUBLIC int lock_notify(src, dst)
07759 int src; /* sender of the notification */
07760 int dst; /* who is to be notified */
07761 {
07762 /* Safe gateway to mini_notify() for tasks and interrupt handlers. The sender
07763 * is explicitly given to prevent confusion where the call comes from. MINIX
07764 * kernel is not reentrant, which means to interrupts are disabled after
07765 * the first kernel entry (hardware interrupt, trap, or exception). Locking
07766 * is done by temporarily disabling interrupts.
07767 */
07768 int result;
07769
07770 /* Exception or interrupt occurred, thus already locked. */
07771 if (k_reenter >= 0) {
07772 result = mini_notify(proc_addr(src), dst);
07773 }
07774
07775 /* Call from task level, locking is required. */
MINIX SOURCE CODE File: kernel/proc.c 729
07776 else {
07777 lock(0, "notify");
07778 result = mini_notify(proc_addr(src), dst);
07779 unlock(0);
07780 }
07781 return(result);
07782 }
07784 /*===========================================================================*
07785 * enqueue *
07786 *===========================================================================*/
07787 PRIVATE void enqueue(rp)
07788 register struct proc *rp; /* this process is now runnable */
07789 {
07790 /* Add ’rp’ to one of the queues of runnable processes. This function is
07791 * responsible for inserting a process into one of the scheduling queues.
07792 * The mechanism is implemented here. The actual scheduling policy is
07793 * defined in sched() and pick_proc().
07794 */
07795 int q; /* scheduling queue to use */
07796 int front; /* add to front or back */
07797
07798 /* Determine where to insert to process. */
07799 sched(rp, &q, &front);
07800
07801 /* Now add the process to the queue. */
07802 if (rdy_head[q] == NIL_PROC) { /* add to empty queue */
07803 rdy_head[q] = rdy_tail[q] = rp; /* create a new queue */
07804 rp->p_nextready = NIL_PROC; /* mark new end */
07805 }
07806 else if (front) { /* add to head of queue */
07807 rp->p_nextready = rdy_head[q]; /* chain head of queue */
07808 rdy_head[q] = rp; /* set new queue head */
07809 }
07810 else { /* add to tail of queue */
07811 rdy_tail[q]->p_nextready = rp; /* chain tail of queue */
07812 rdy_tail[q] = rp; /* set new queue tail */
07813 rp->p_nextready = NIL_PROC; /* mark new end */
07814 }
07815
07816 /* Now select the next process to run. */
07817 pick_proc();
07818 }
07820 /*===========================================================================*
07821 * dequeue *
07822 *===========================================================================*/
07823 PRIVATE void dequeue(rp)
07824 register struct proc *rp; /* this process is no longer runnable */
07825 {
07826 /* A process must be removed from the scheduling queues, for example, because
07827 * it has blocked. If the currently active process is removed, a new process
07828 * is picked to run by calling pick_proc().
07829 */
07830 register int q = rp->p_priority; /* queue to use */
07831 register struct proc **xpp; /* iterate over queue */
07832 register struct proc *prev_xp;
07833
07834 /* Side-effect for kernel: check if the task’s stack still is ok? */
07835 if (iskernelp(rp)) {
730 File: kernel/proc.c MINIX SOURCE CODE
07836 if (*priv(rp)->s_stack_guard != STACK_GUARD)
07837 panic("stack overrun by task", proc_nr(rp));
07838 }
07839
07840 /* Now make sure that the process is not in its ready queue. Remove the
07841 * process if it is found. A process can be made unready even if it is not
07842 * running by being sent a signal that kills it.
07843 */
07844 prev_xp = NIL_PROC;
07845 for (xpp = &rdy_head[q]; *xpp != NIL_PROC; xpp = &(*xpp)->p_nextready) {
07846
07847 if (*xpp == rp) { /* found process to remove */
07848 *xpp = (*xpp)->p_nextready; /* replace with next chain */
07849 if (rp == rdy_tail[q]) /* queue tail removed */
07850 rdy_tail[q] = prev_xp; /* set new tail */
07851 if (rp == proc_ptr || rp == next_ptr) /* active process removed */
07852 pick_proc(); /* pick new process to run */
07853 break;
07854 }
07855 prev_xp = *xpp; /* save previous in chain */
07856 }
07857 }
07859 /*===========================================================================*
07860 * sched *
07861 *===========================================================================*/
07862 PRIVATE void sched(rp, queue, front)
07863 register struct proc *rp; /* process to be scheduled */
07864 int *queue; /* return: queue to use */
07865 int *front; /* return: front or back */
07866 {
07867 /* This function determines the scheduling policy. It is called whenever a
07868 * process must be added to one of the scheduling queues to decide where to
07869 * insert it. As a side-effect the process’ priority may be updated.
07870 */
07871 static struct proc *prev_ptr = NIL_PROC; /* previous without time */
07872 int time_left = (rp->p_ticks_left > 0); /* quantum fully consumed */
07873 int penalty = 0; /* change in priority */
07874
07875 /* Check whether the process has time left. Otherwise give a new quantum
07876 * and possibly raise the priority. Processes using multiple quantums
07877 * in a row get a lower priority to catch infinite loops in high priority
07878 * processes (system servers and drivers).
07879 */
07880 if ( ! time_left) { /* quantum consumed ? */
07881 rp->p_ticks_left = rp->p_quantum_size; /* give new quantum */
07882 if (prev_ptr == rp) penalty ++; /* catch infinite loops */
07883 else penalty --; /* give slow way back */
07884 prev_ptr = rp; /* store ptr for next */
07885 }
07886
07887 /* Determine the new priority of this process. The bounds are determined
07888 * by IDLE’s queue and the maximum priority of this process. Kernel tasks
07889 * and the idle process are never changed in priority.
07890 */
07891 if (penalty != 0 && ! iskernelp(rp)) {
07892 rp->p_priority += penalty; /* update with penalty */
07893 if (rp->p_priority < rp->p_max_priority) /* check upper bound */
07894 rp->p_priority=rp->p_max_priority;
07895 else if (rp->p_priority > IDLE_Q-1) /* check lower bound */
MINIX SOURCE CODE File: kernel/proc.c 731
07896 rp->p_priority = IDLE_Q-1;
07897 }
07898
07899 /* If there is time left, the process is added to the front of its queue,
07900 * so that it can immediately run. The queue to use simply is always the
07901 * process’ current priority.
07902 */
07903 *queue = rp->p_priority;
07904 *front = time_left;
07905 }
07907 /*===========================================================================*
07908 * pick_proc *
07909 *===========================================================================*/
07910 PRIVATE void pick_proc()
07911 {
07912 /* Decide who to run now. A new process is selected by setting ’next_ptr’.
07913 * When a billable process is selected, record it in ’bill_ptr’, so that the
07914 * clock task can tell who to bill for system time.
07915 */
07916 register struct proc *rp; /* process to run */
07917 int q; /* iterate over queues */
07918
07919 /* Check each of the scheduling queues for ready processes. The number of
07920 * queues is defined in proc.h, and priorities are set in the image table.
07921 * The lowest queue contains IDLE, which is always ready.
07922 */
07923 for (q=0; q < NR_SCHED_QUEUES; q++) {
07924 if ( (rp = rdy_head[q]) != NIL_PROC) {
07925 next_ptr = rp; /* run process ’rp’ next */
07926 if (priv(rp)->s_flags & BILLABLE)
07927 bill_ptr = rp; /* bill for system time */
07928 return;
07929 }
07930 }
07931 }
07933 /*===========================================================================*
07934 * lock_send *
07935 *===========================================================================*/
07936 PUBLIC int lock_send(dst, m_ptr)
07937 int dst; /* to whom is message being sent? */
07938 message *m_ptr; /* pointer to message buffer */
07939 {
07940 /* Safe gateway to mini_send() for tasks. */
07941 int result;
07942 lock(2, "send");
07943 result = mini_send(proc_ptr, dst, m_ptr, NON_BLOCKING);
07944 unlock(2);
07945 return(result);
07946 }
07948 /*===========================================================================*
07949 * lock_enqueue *
07950 *===========================================================================*/
07951 PUBLIC void lock_enqueue(rp)
07952 struct proc *rp; /* this process is now runnable */
07953 {
07954 /* Safe gateway to enqueue() for tasks. */
07955 lock(3, "enqueue");
732 File: kernel/proc.c MINIX SOURCE CODE
07956 enqueue(rp);
07957 unlock(3);
07958 }
07960 /*===========================================================================*
07961 * lock_dequeue *
07962 *===========================================================================*/
07963 PUBLIC void lock_dequeue(rp)
07964 struct proc *rp; /* this process is no longer runnable */
07965 {
07966 /* Safe gateway to dequeue() for tasks. */
07967 lock(4, "dequeue");
07968 dequeue(rp);
07969 unlock(4);
07970 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/exception.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
08000 /* This file contains a simple exception handler. Exceptions in user
08001 * processes are converted to signals. Exceptions in a kernel task cause
08002 * a panic.
08003 */
08004
08005 #include "kernel.h"
08006 #include <signal.h>
08007 #include "proc.h"
08008
08009 /*===========================================================================*
08010 * exception *
08011 *===========================================================================*/
08012 PUBLIC void exception(vec_nr)
08013 unsigned vec_nr;
08014 {
08015 /* An exception or unexpected interrupt has occurred. */
08016
08017 struct ex_s {
08018 char *msg;
08019 int signum;
08020 int minprocessor;
08021 };
08022 static struct ex_s ex_data[] = {
08023 { "Divide error", SIGFPE, 86 },
08024 { "Debug exception", SIGTRAP, 86 },
08025 { "Nonmaskable interrupt", SIGBUS, 86 },
08026 { "Breakpoint", SIGEMT, 86 },
08027 { "Overflow", SIGFPE, 86 },
08028 { "Bounds check", SIGFPE, 186 },
08029 { "Invalid opcode", SIGILL, 186 },
08030 { "Coprocessor not available", SIGFPE, 186 },
08031 { "Double fault", SIGBUS, 286 },
08032 { "Copressor segment overrun", SIGSEGV, 286 },
08033 { "Invalid TSS", SIGSEGV, 286 },
08034 { "Segment not present", SIGSEGV, 286 },
MINIX SOURCE CODE File: kernel/exception.c 733
08035 { "Stack exception", SIGSEGV, 286 }, /* STACK_FAULT already used */
08036 { "General protection", SIGSEGV, 286 },
08037 { "Page fault", SIGSEGV, 386 }, /* not close */
08038 { NIL_PTR, SIGILL, 0 }, /* probably software trap */
08039 { "Coprocessor error", SIGFPE, 386 },
08040 };
08041 register struct ex_s *ep;
08042 struct proc *saved_proc;
08043
08044 /* Save proc_ptr, because it may be changed by debug statements. */
08045 saved_proc = proc_ptr;
08046
08047 ep = &ex_data[vec_nr];
08048
08049 if (vec_nr == 2) { /* spurious NMI on some machines */
08050 kprintf("got spurious NMI\n");
08051 return;
08052 }
08053
08054 /* If an exception occurs while running a process, the k_reenter variable
08055 * will be zero. Exceptions in interrupt handlers or system traps will make
08056 * k_reenter larger than zero.
08057 */
08058 if (k_reenter == 0 && ! iskernelp(saved_proc)) {
08059 cause_sig(proc_nr(saved_proc), ep->signum);
08060 return;
08061 }
08062
08063 /* Exception in system code. This is not supposed to happen. */
08064 if (ep->msg == NIL_PTR || machine.processor < ep->minprocessor)
08065 kprintf("\nIntel-reserved exception %d\n", vec_nr);
08066 else
08067 kprintf("\n%s\n", ep->msg);
08068 kprintf("k_reenter = %d ", k_reenter);
08069 kprintf("process %d (%s), ", proc_nr(saved_proc), saved_proc->p_name);
08070 kprintf("pc = %u:0x%x", (unsigned) saved_proc->p_reg.cs,
08071 (unsigned) saved_proc->p_reg.pc);
08072
08073 panic("exception in a kernel task", NO_NUM);
08074 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/i8259.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
08100 /* This file contains routines for initializing the 8259 interrupt controller:
08101 * put_irq_handler: register an interrupt handler
08102 * rm_irq_handler: deregister an interrupt handler
08103 * intr_handle: handle a hardware interrupt
08104 * intr_init: initialize the interrupt controller(s)
08105 */
08106
08107 #include "kernel.h"
08108 #include "proc.h"
08109 #include <minix/com.h>
734 File: kernel/i8259.c MINIX SOURCE CODE
08110
08111 #define ICW1_AT 0x11 /* edge triggered, cascade, need ICW4 */
08112 #define ICW1_PC 0x13 /* edge triggered, no cascade, need ICW4 */
08113 #define ICW1_PS 0x19 /* level triggered, cascade, need ICW4 */
08114 #define ICW4_AT_SLAVE 0x01 /* not SFNM, not buffered, normal EOI, 8086 */
08115 #define ICW4_AT_MASTER 0x05 /* not SFNM, not buffered, normal EOI, 8086 */
08116 #define ICW4_PC_SLAVE 0x09 /* not SFNM, buffered, normal EOI, 8086 */
08117 #define ICW4_PC_MASTER 0x0D /* not SFNM, buffered, normal EOI, 8086 */
08118
08119 #define set_vec(nr, addr) ((void)0)
08120
08121 /*===========================================================================*
08122 * intr_init *
08123 *===========================================================================*/
08124 PUBLIC void intr_init(mine)
08125 int mine;
08126 {
08127 /* Initialize the 8259s, finishing with all interrupts disabled. This is
08128 * only done in protected mode, in real mode we don’t touch the 8259s, but
08129 * use the BIOS locations instead. The flag "mine" is set if the 8259s are
08130 * to be programmed for MINIX, or to be reset to what the BIOS expects.
08131 */
08132 int i;
08133
08134 intr_disable();
08135
08136 /* The AT and newer PS/2 have two interrupt controllers, one master,
08137 * one slaved at IRQ 2. (We don’t have to deal with the PC that
08138 * has just one controller, because it must run in real mode.)
08139 */
08140 outb(INT_CTL, machine.ps_mca ? ICW1_PS : ICW1_AT);
08141 outb(INT_CTLMASK, mine ? IRQ0_VECTOR : BIOS_IRQ0_VEC);
08142 /* ICW2 for master */
08143 outb(INT_CTLMASK, (1 << CASCADE_IRQ)); /* ICW3 tells slaves */
08144 outb(INT_CTLMASK, ICW4_AT_MASTER);
08145 outb(INT_CTLMASK, ˜(1 << CASCADE_IRQ)); /* IRQ 0-7 mask */
08146 outb(INT2_CTL, machine.ps_mca ? ICW1_PS : ICW1_AT);
08147 outb(INT2_CTLMASK, mine ? IRQ8_VECTOR : BIOS_IRQ8_VEC);
08148 /* ICW2 for slave */
08149 outb(INT2_CTLMASK, CASCADE_IRQ); /* ICW3 is slave nr */
08150 outb(INT2_CTLMASK, ICW4_AT_SLAVE);
08151 outb(INT2_CTLMASK, ˜0); /* IRQ 8-15 mask */
08152
08153 /* Copy the BIOS vectors from the BIOS to the Minix location, so we
08154 * can still make BIOS calls without reprogramming the i8259s.
08155 */
08156 phys_copy(BIOS_VECTOR(0) * 4L, VECTOR(0) * 4L, 8 * 4L);
08157 }
08159 /*===========================================================================*
08160 * put_irq_handler *
08161 *===========================================================================*/
08162 PUBLIC void put_irq_handler(hook, irq, handler)
08163 irq_hook_t *hook;
08164 int irq;
08165 irq_handler_t handler;
08166 {
08167 /* Register an interrupt handler. */
08168 int id;
08169 irq_hook_t **line;
MINIX SOURCE CODE File: kernel/i8259.c 735
08170
08171 if (irq < 0 || irq >= NR_IRQ_VECTORS)
08172 panic("invalid call to put_irq_handler", irq);
08173
08174 line = &irq_handlers[irq];
08175 id = 1;
08176 while (*line != NULL) {
08177 if (hook == *line) return; /* extra initialization */
08178 line = &(*line)->next;
08179 id <<= 1;
08180 }
08181 if (id == 0) panic("Too many handlers for irq", irq);
08182
08183 hook->next = NULL;
08184 hook->handler = handler;
08185 hook->irq = irq;
08186 hook->id = id;
08187 *line = hook;
08188
08189 irq_use |= 1 << irq;
08190 }
08192 /*===========================================================================*
08193 * rm_irq_handler *
08194 *===========================================================================*/
08195 PUBLIC void rm_irq_handler(hook)
08196 irq_hook_t *hook;
08197 {
08198 /* Unregister an interrupt handler. */
08199 int irq = hook->irq;
08200 int id = hook->id;
08201 irq_hook_t **line;
08202
08203 if (irq < 0 || irq >= NR_IRQ_VECTORS)
08204 panic("invalid call to rm_irq_handler", irq);
08205
08206 line = &irq_handlers[irq];
08207 while (*line != NULL) {
08208 if ((*line)->id == id) {
08209 (*line) = (*line)->next;
08210 if (! irq_handlers[irq]) irq_use &= ˜(1 << irq);
08211 return;
08212 }
08213 line = &(*line)->next;
08214 }
08215 /* When the handler is not found, normally return here. */
08216 }
08218 /*===========================================================================*
08219 * intr_handle *
08220 *===========================================================================*/
08221 PUBLIC void intr_handle(hook)
08222 irq_hook_t *hook;
08223 {
08224 /* Call the interrupt handlers for an interrupt with the given hook list.
08225 * The assembly part of the handler has already masked the IRQ, reenabled the
08226 * controller(s) and enabled interrupts.
08227 */
08228
08229 /* Call list of handlers for an IRQ. */
736 File: kernel/i8259.c MINIX SOURCE CODE
08230 while (hook != NULL) {
08231 /* For each handler in the list, mark it active by setting its ID bit,
08232 * call the function, and unmark it if the function returns true.
08233 */
08234 irq_actids[hook->irq] |= hook->id;
08235 if ((*hook->handler)(hook)) irq_actids[hook->irq] &= ˜hook->id;
08236 hook = hook->next;
08237 }
08238
08239 /* The assembly code will now disable interrupts, unmask the IRQ if and only
08240 * if all active ID bits are cleared, and restart a process.
08241 */
08242 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/protect.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
08300 /* This file contains code for initialization of protected mode, to initialize
08301 * code and data segment descriptors, and to initialize global descriptors
08302 * for local descriptors in the process table.
08303 */
08304
08305 #include "kernel.h"
08306 #include "proc.h"
08307 #include "protect.h"
08308
08309 #define INT_GATE_TYPE (INT_286_GATE | DESC_386_BIT)
08310 #define TSS_TYPE (AVL_286_TSS | DESC_386_BIT)
08311
08312 struct desctableptr_s {
08313 char limit[sizeof(u16_t)];
08314 char base[sizeof(u32_t)]; /* really u24_t + pad for 286 */
08315 };
08316
08317 struct gatedesc_s {
08318 u16_t offset_low;
08319 u16_t selector;
08320 u8_t pad; /* |000|XXXXX| ig & trpg, |XXXXXXXX| task g */
08321 u8_t p_dpl_type; /* |P|DL|0|TYPE| */
08322 u16_t offset_high;
08323 };
08324
08325 struct tss_s {
08326 reg_t backlink;
08327 reg_t sp0; /* stack pointer to use during interrupt */
08328 reg_t ss0; /* " segment " " " " */
08329 reg_t sp1;
08330 reg_t ss1;
08331 reg_t sp2;
08332 reg_t ss2;
08333 reg_t cr3;
08334 reg_t ip;
08335 reg_t flags;
08336 reg_t ax;
08337 reg_t cx;
08338 reg_t dx;
08339 reg_t bx;
MINIX SOURCE CODE File: kernel/protect.c 737
08340 reg_t sp;
08341 reg_t bp;
08342 reg_t si;
08343 reg_t di;
08344 reg_t es;
08345 reg_t cs;
08346 reg_t ss;
08347 reg_t ds;
08348 reg_t fs;
08349 reg_t gs;
08350 reg_t ldt;
08351 u16_t trap;
08352 u16_t iobase;
08353 /* u8_t iomap[0]; */
08354 };
08355
08356 PUBLIC struct segdesc_s gdt[GDT_SIZE]; /* used in klib.s and mpx.s */
08357 PRIVATE struct gatedesc_s idt[IDT_SIZE]; /* zero-init so none present */
08358 PUBLIC struct tss_s tss; /* zero init */
08359
08360 FORWARD _PROTOTYPE( void int_gate, (unsigned vec_nr, vir_bytes offset,
08361 unsigned dpl_type) );
08362 FORWARD _PROTOTYPE( void sdesc, (struct segdesc_s *segdp, phys_bytes base,
08363 vir_bytes size) );
08364
08365 /*===========================================================================*
08366 * prot_init *
08367 *===========================================================================*/
08368 PUBLIC void prot_init()
08369 {
08370 /* Set up tables for protected mode.
08371 * All GDT slots are allocated at compile time.
08372 */
08373 struct gate_table_s *gtp;
08374 struct desctableptr_s *dtp;
08375 unsigned ldt_index;
08376 register struct proc *rp;
08377
08378 static struct gate_table_s {
08379 _PROTOTYPE( void (*gate), (void) );
08380 unsigned char vec_nr;
08381 unsigned char privilege;
08382 }
08383 gate_table[] = {
08384 { divide_error, DIVIDE_VECTOR, INTR_PRIVILEGE },
08385 { single_step_exception, DEBUG_VECTOR, INTR_PRIVILEGE },
08386 { nmi, NMI_VECTOR, INTR_PRIVILEGE },
08387 { breakpoint_exception, BREAKPOINT_VECTOR, USER_PRIVILEGE },
08388 { overflow, OVERFLOW_VECTOR, USER_PRIVILEGE },
08389 { bounds_check, BOUNDS_VECTOR, INTR_PRIVILEGE },
08390 { inval_opcode, INVAL_OP_VECTOR, INTR_PRIVILEGE },
08391 { copr_not_available, COPROC_NOT_VECTOR, INTR_PRIVILEGE },
08392 { double_fault, DOUBLE_FAULT_VECTOR, INTR_PRIVILEGE },
08393 { copr_seg_overrun, COPROC_SEG_VECTOR, INTR_PRIVILEGE },
08394 { inval_tss, INVAL_TSS_VECTOR, INTR_PRIVILEGE },
08395 { segment_not_present, SEG_NOT_VECTOR, INTR_PRIVILEGE },
08396 { stack_exception, STACK_FAULT_VECTOR, INTR_PRIVILEGE },
08397 { general_protection, PROTECTION_VECTOR, INTR_PRIVILEGE },
08398 { page_fault, PAGE_FAULT_VECTOR, INTR_PRIVILEGE },
08399 { copr_error, COPROC_ERR_VECTOR, INTR_PRIVILEGE },
738 File: kernel/protect.c MINIX SOURCE CODE
08400 { hwint00, VECTOR( 0), INTR_PRIVILEGE },
08401 { hwint01, VECTOR( 1), INTR_PRIVILEGE },
08402 { hwint02, VECTOR( 2), INTR_PRIVILEGE },
08403 { hwint03, VECTOR( 3), INTR_PRIVILEGE },
08404 { hwint04, VECTOR( 4), INTR_PRIVILEGE },
08405 { hwint05, VECTOR( 5), INTR_PRIVILEGE },
08406 { hwint06, VECTOR( 6), INTR_PRIVILEGE },
08407 { hwint07, VECTOR( 7), INTR_PRIVILEGE },
08408 { hwint08, VECTOR( 8), INTR_PRIVILEGE },
08409 { hwint09, VECTOR( 9), INTR_PRIVILEGE },
08410 { hwint10, VECTOR(10), INTR_PRIVILEGE },
08411 { hwint11, VECTOR(11), INTR_PRIVILEGE },
08412 { hwint12, VECTOR(12), INTR_PRIVILEGE },
08413 { hwint13, VECTOR(13), INTR_PRIVILEGE },
08414 { hwint14, VECTOR(14), INTR_PRIVILEGE },
08415 { hwint15, VECTOR(15), INTR_PRIVILEGE },
08416 { s_call, SYS386_VECTOR, USER_PRIVILEGE }, /* 386 system call */
08417 { level0_call, LEVEL0_VECTOR, TASK_PRIVILEGE },
08418 };
08419
08420 /* Build gdt and idt pointers in GDT where the BIOS expects them. */
08421 dtp= (struct desctableptr_s *) &gdt[GDT_INDEX];
08422 * (u16_t *) dtp->limit = (sizeof gdt) - 1;
08423 * (u32_t *) dtp->base = vir2phys(gdt);
08424
08425 dtp= (struct desctableptr_s *) &gdt[IDT_INDEX];
08426 * (u16_t *) dtp->limit = (sizeof idt) - 1;
08427 * (u32_t *) dtp->base = vir2phys(idt);
08428
08429 /* Build segment descriptors for tasks and interrupt handlers. */
08430 init_codeseg(&gdt[CS_INDEX],
08431 kinfo.code_base, kinfo.code_size, INTR_PRIVILEGE);
08432 init_dataseg(&gdt[DS_INDEX],
08433 kinfo.data_base, kinfo.data_size, INTR_PRIVILEGE);
08434 init_dataseg(&gdt[ES_INDEX], 0L, 0, TASK_PRIVILEGE);
08435
08436 /* Build scratch descriptors for functions in klib88. */
08437 init_dataseg(&gdt[DS_286_INDEX], 0L, 0, TASK_PRIVILEGE);
08438 init_dataseg(&gdt[ES_286_INDEX], 0L, 0, TASK_PRIVILEGE);
08439
08440 /* Build local descriptors in GDT for LDT’s in process table.
08441 * The LDT’s are allocated at compile time in the process table, and
08442 * initialized whenever a process’ map is initialized or changed.
08443 */
08444 for (rp = BEG_PROC_ADDR, ldt_index = FIRST_LDT_INDEX;
08445 rp < END_PROC_ADDR; ++rp, ldt_index++) {
08446 init_dataseg(&gdt[ldt_index], vir2phys(rp->p_ldt),
08447 sizeof(rp->p_ldt), INTR_PRIVILEGE);
08448 gdt[ldt_index].access = PRESENT | LDT;
08449 rp->p_ldt_sel = ldt_index * DESC_SIZE;
08450 }
08451
08452 /* Build main TSS.
08453 * This is used only to record the stack pointer to be used after an
08454 * interrupt.
08455 * The pointer is set up so that an interrupt automatically saves the
08456 * current process’s registers ip:cs:f:sp:ss in the correct slots in the
08457 * process table.
08458 */
08459 tss.ss0 = DS_SELECTOR;
MINIX SOURCE CODE File: kernel/protect.c 739
08460 init_dataseg(&gdt[TSS_INDEX], vir2phys(&tss), sizeof(tss), INTR_PRIVILEGE);
08461 gdt[TSS_INDEX].access = PRESENT | (INTR_PRIVILEGE << DPL_SHIFT) | TSS_TYPE;
08462
08463 /* Build descriptors for interrupt gates in IDT. */
08464 for (gtp = &gate_table[0];
08465 gtp < &gate_table[sizeof gate_table / sizeof gate_table[0]]; ++gtp) {
08466 int_gate(gtp->vec_nr, (vir_bytes) gtp->gate,
08467 PRESENT | INT_GATE_TYPE | (gtp->privilege << DPL_SHIFT));
08468 }
08469
08470 /* Complete building of main TSS. */
08471 tss.iobase = sizeof tss; /* empty i/o permissions map */
08472 }
08474 /*===========================================================================*
08475 * init_codeseg *
08476 *===========================================================================*/
08477 PUBLIC void init_codeseg(segdp, base, size, privilege)
08478 register struct segdesc_s *segdp;
08479 phys_bytes base;
08480 vir_bytes size;
08481 int privilege;
08482 {
08483 /* Build descriptor for a code segment. */
08484 sdesc(segdp, base, size);
08485 segdp->access = (privilege << DPL_SHIFT)
08486 | (PRESENT | SEGMENT | EXECUTABLE | READABLE);
08487 /* CONFORMING = 0, ACCESSED = 0 */
08488 }
08490 /*===========================================================================*
08491 * init_dataseg *
08492 *===========================================================================*/
08493 PUBLIC void init_dataseg(segdp, base, size, privilege)
08494 register struct segdesc_s *segdp;
08495 phys_bytes base;
08496 vir_bytes size;
08497 int privilege;
08498 {
08499 /* Build descriptor for a data segment. */
08500 sdesc(segdp, base, size);
08501 segdp->access = (privilege << DPL_SHIFT) | (PRESENT | SEGMENT | WRITEABLE);
08502 /* EXECUTABLE = 0, EXPAND_DOWN = 0, ACCESSED = 0 */
08503 }
08505 /*===========================================================================*
08506 * sdesc *
08507 *===========================================================================*/
08508 PRIVATE void sdesc(segdp, base, size)
08509 register struct segdesc_s *segdp;
08510 phys_bytes base;
08511 vir_bytes size;
08512 {
08513 /* Fill in the size fields (base, limit and granularity) of a descriptor. */
08514 segdp->base_low = base;
08515 segdp->base_middle = base >> BASE_MIDDLE_SHIFT;
08516 segdp->base_high = base >> BASE_HIGH_SHIFT;
08517
08518 --size; /* convert to a limit, 0 size means 4G */
08519 if (size > BYTE_GRAN_MAX) {
740 File: kernel/protect.c MINIX SOURCE CODE
08520 segdp->limit_low = size >> PAGE_GRAN_SHIFT;
08521 segdp->granularity = GRANULAR | (size >>
08522 (PAGE_GRAN_SHIFT + GRANULARITY_SHIFT));
08523 } else {
08524 segdp->limit_low = size;
08525 segdp->granularity = size >> GRANULARITY_SHIFT;
08526 }
08527 segdp->granularity |= DEFAULT; /* means BIG for data seg */
08528 }
08530 /*===========================================================================*
08531 * seg2phys *
08532 *===========================================================================*/
08533 PUBLIC phys_bytes seg2phys(seg)
08534 U16_t seg;
08535 {
08536 /* Return the base address of a segment, with seg being either a 8086 segment
08537 * register, or a 286/386 segment selector.
08538 */
08539 phys_bytes base;
08540 struct segdesc_s *segdp;
08541
08542 if (! machine.protected) {
08543 base = hclick_to_physb(seg);
08544 } else {
08545 segdp = &gdt[seg >> 3];
08546 base = ((u32_t) segdp->base_low << 0)
08547 | ((u32_t) segdp->base_middle << 16)
08548 | ((u32_t) segdp->base_high << 24);
08549 }
08550 return base;
08551 }
08553 /*===========================================================================*
08554 * phys2seg *
08555 *===========================================================================*/
08556 PUBLIC void phys2seg(seg, off, phys)
08557 u16_t *seg;
08558 vir_bytes *off;
08559 phys_bytes phys;
08560 {
08561 /* Return a segment selector and offset that can be used to reach a physical
08562 * address, for use by a driver doing memory I/O in the A0000 - DFFFF range.
08563 */
08564 *seg = FLAT_DS_SELECTOR;
08565 *off = phys;
08566 }
08568 /*===========================================================================*
08569 * int_gate *
08570 *===========================================================================*/
08571 PRIVATE void int_gate(vec_nr, offset, dpl_type)
08572 unsigned vec_nr;
08573 vir_bytes offset;
08574 unsigned dpl_type;
08575 {
08576 /* Build descriptor for an interrupt gate. */
08577 register struct gatedesc_s *idp;
08578
08579 idp = &idt[vec_nr];
MINIX SOURCE CODE File: kernel/protect.c 741
08580 idp->offset_low = offset;
08581 idp->selector = CS_SELECTOR;
08582 idp->p_dpl_type = dpl_type;
08583 idp->offset_high = offset >> OFFSET_HIGH_SHIFT;
08584 }
08586 /*===========================================================================*
08587 * enable_iop *
08588 *===========================================================================*/
08589 PUBLIC void enable_iop(pp)
08590 struct proc *pp;
08591 {
08592 /* Allow a user process to use I/O instructions. Change the I/O Permission
08593 * Level bits in the psw. These specify least-privileged Current Permission
08594 * Level allowed to execute I/O instructions. Users and servers have CPL 3.
08595 * You can’t have less privilege than that. Kernel has CPL 0, tasks CPL 1.
08596 */
08597 pp->p_reg.psw |= 0x3000;
08598 }
08600 /*===========================================================================*
08601 * alloc_segments *
08602 *===========================================================================*/
08603 PUBLIC void alloc_segments(rp)
08604 register struct proc *rp;
08605 {
08606 /* This is called at system initialization from main() and by do_newmap().
08607 * The code has a separate function because of all hardware-dependencies.
08608 * Note that IDLE is part of the kernel and gets TASK_PRIVILEGE here.
08609 */
08610 phys_bytes code_bytes;
08611 phys_bytes data_bytes;
08612 int privilege;
08613
08614 if (machine.protected) {
08615 data_bytes = (phys_bytes) (rp->p_memmap[S].mem_vir +
08616 rp->p_memmap[S].mem_len) << CLICK_SHIFT;
08617 if (rp->p_memmap[T].mem_len == 0)
08618 code_bytes = data_bytes; /* common I&D, poor protect */
08619 else
08620 code_bytes = (phys_bytes) rp->p_memmap[T].mem_len << CLICK_SHIFT;
08621 privilege = (iskernelp(rp)) ? TASK_PRIVILEGE : USER_PRIVILEGE;
08622 init_codeseg(&rp->p_ldt[CS_LDT_INDEX],
08623 (phys_bytes) rp->p_memmap[T].mem_phys << CLICK_SHIFT,
08624 code_bytes, privilege);
08625 init_dataseg(&rp->p_ldt[DS_LDT_INDEX],
08626 (phys_bytes) rp->p_memmap[D].mem_phys << CLICK_SHIFT,
08627 data_bytes, privilege);
08628 rp->p_reg.cs = (CS_LDT_INDEX * DESC_SIZE) | TI | privilege;
08629 rp->p_reg.gs =
08630 rp->p_reg.fs =
08631 rp->p_reg.ss =
08632 rp->p_reg.es =
08633 rp->p_reg.ds = (DS_LDT_INDEX*DESC_SIZE) | TI | privilege;
08634 } else {
08635 rp->p_reg.cs = click_to_hclick(rp->p_memmap[T].mem_phys);
08636 rp->p_reg.ss =
08637 rp->p_reg.es =
08638 rp->p_reg.ds = click_to_hclick(rp->p_memmap[D].mem_phys);
08639 }
742 File: kernel/protect.c MINIX SOURCE CODE
08640 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/klib.s
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
08700 #
08701 ! Chooses between the 8086 and 386 versions of the low level kernel code.
08702
08703 #include <minix/config.h>
08704 #if _WORD_SIZE == 2
08705 #include "klib88.s"
08706 #else
08707 #include "klib386.s"
08708 #endif
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/klib386.s
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
08800 #
08801 ! sections
08802
08803 .sect .text; .sect .rom; .sect .data; .sect .bss
08804
08805 #include <minix/config.h>
08806 #include <minix/const.h>
08807 #include "const.h"
08808 #include "sconst.h"
08809 #include "protect.h"
08810
08811 ! This file contains a number of assembly code utility routines needed by the
08812 ! kernel. They are:
08813
08814 .define _monitor ! exit Minix and return to the monitor
08815 .define _int86 ! let the monitor make an 8086 interrupt call
08816 .define _cp_mess ! copies messages from source to destination
08817 .define _exit ! dummy for library routines
08818 .define __exit ! dummy for library routines
08819 .define ___exit ! dummy for library routines
08820 .define ___main ! dummy for GCC
08821 .define _phys_insw ! transfer data from (disk controller) port to memory
08822 .define _phys_insb ! likewise byte by byte
08823 .define _phys_outsw ! transfer data from memory to (disk controller) port
08824 .define _phys_outsb ! likewise byte by byte
08825 .define _enable_irq ! enable an irq at the 8259 controller
08826 .define _disable_irq ! disable an irq
08827 .define _phys_copy ! copy data from anywhere to anywhere in memory
08828 .define _phys_memset ! write pattern anywhere in memory
08829 .define _mem_rdw ! copy one word from [segment:offset]
08830 .define _reset ! reset the system
08831 .define _idle_task ! task executed when there is no work
08832 .define _level0 ! call a function at level 0
08833 .define _read_tsc ! read the cycle counter (Pentium and up)
08834 .define _read_cpu_flags ! read the cpu flags
MINIX SOURCE CODE File: kernel/klib386.s 743
08835
08836 ! The routines only guarantee to preserve the registers the C compiler
08837 ! expects to be preserved (ebx, esi, edi, ebp, esp, segment registers, and
08838 ! direction bit in the flags).
08839
08840 .sect .text
08841 !*===========================================================================*
08842 !* monitor *
08843 !*===========================================================================*
08844 ! PUBLIC void monitor();
08845 ! Return to the monitor.
08846
08847 _monitor:
08848 mov esp, (_mon_sp) ! restore monitor stack pointer
08849 o16 mov dx, SS_SELECTOR ! monitor data segment
08850 mov ds, dx
08851 mov es, dx
08852 mov fs, dx
08853 mov gs, dx
08854 mov ss, dx
08855 pop edi
08856 pop esi
08857 pop ebp
08858 o16 retf ! return to the monitor
08859
08860
08861 !*===========================================================================*
08862 !* int86 *
08863 !*===========================================================================*
08864 ! PUBLIC void int86();
08865 _int86:
08866 cmpb (_mon_return), 0 ! is the monitor there?
08867 jnz 0f
08868 movb ah, 0x01 ! an int 13 error seems appropriate
08869 movb (_reg86+ 0), ah ! reg86.w.f = 1 (set carry flag)
08870 movb (_reg86+13), ah ! reg86.b.ah = 0x01 = "invalid command"
08871 ret
08872 0: push ebp ! save C registers
08873 push esi
08874 push edi
08875 push ebx
08876 pushf ! save flags
08877 cli ! no interruptions
08878
08879 inb INT2_CTLMASK
08880 movb ah, al
08881 inb INT_CTLMASK
08882 push eax ! save interrupt masks
08883 mov eax, (_irq_use) ! map of in-use IRQ’s
08884 and eax, ˜[1<<CLOCK_IRQ] ! keep the clock ticking
08885 outb INT_CTLMASK ! enable all unused IRQ’s and vv.
08886 movb al, ah
08887 outb INT2_CTLMASK
08888
08889 mov eax, SS_SELECTOR ! monitor data segment
08890 mov ss, ax
08891 xchg esp, (_mon_sp) ! switch stacks
08892 push (_reg86+36) ! parameters used in INT call
08893 push (_reg86+32)
08894 push (_reg86+28)
744 File: kernel/klib386.s MINIX SOURCE CODE
08895 push (_reg86+24)
08896 push (_reg86+20)
08897 push (_reg86+16)
08898 push (_reg86+12)
08899 push (_reg86+ 8)
08900 push (_reg86+ 4)
08901 push (_reg86+ 0)
08902 mov ds, ax ! remaining data selectors
08903 mov es, ax
08904 mov fs, ax
08905 mov gs, ax
08906 push cs
08907 push return ! kernel return address and selector
08908 o16 jmpf 20+2*4+10*4+2*4(esp) ! make the call
08909 return:
08910 pop (_reg86+ 0)
08911 pop (_reg86+ 4)
08912 pop (_reg86+ 8)
08913 pop (_reg86+12)
08914 pop (_reg86+16)
08915 pop (_reg86+20)
08916 pop (_reg86+24)
08917 pop (_reg86+28)
08918 pop (_reg86+32)
08919 pop (_reg86+36)
08920 lgdt (_gdt+GDT_SELECTOR) ! reload global descriptor table
08921 jmpf CS_SELECTOR:csinit ! restore everything
08922 csinit: mov eax, DS_SELECTOR
08923 mov ds, ax
08924 mov es, ax
08925 mov fs, ax
08926 mov gs, ax
08927 mov ss, ax
08928 xchg esp, (_mon_sp) ! unswitch stacks
08929 lidt (_gdt+IDT_SELECTOR) ! reload interrupt descriptor table
08930 andb (_gdt+TSS_SELECTOR+DESC_ACCESS), ˜0x02 ! clear TSS busy bit
08931 mov eax, TSS_SELECTOR
08932 ltr ax ! set TSS register
08933
08934 pop eax
08935 outb INT_CTLMASK ! restore interrupt masks
08936 movb al, ah
08937 outb INT2_CTLMASK
08938
08939 add (_lost_ticks), ecx ! record lost clock ticks
08940
08941 popf ! restore flags
08942 pop ebx ! restore C registers
08943 pop edi
08944 pop esi
08945 pop ebp
08946 ret
08947
08948
08949 !*===========================================================================*
08950 !* cp_mess *
08951 !*===========================================================================*
08952 ! PUBLIC void cp_mess(int src, phys_clicks src_clicks, vir_bytes src_offset,
08953 ! phys_clicks dst_clicks, vir_bytes dst_offset);
08954 ! This routine makes a fast copy of a message from anywhere in the address
MINIX SOURCE CODE File: kernel/klib386.s 745
08955 ! space to anywhere else. It also copies the source address provided as a
08956 ! parameter to the call into the first word of the destination message.
08957 !
08958 ! Note that the message size, "Msize" is in DWORDS (not bytes) and must be set
08959 ! correctly. Changing the definition of message in the type file and not
08960 ! changing it here will lead to total disaster.
08961
08962 CM_ARGS = 4 + 4 + 4 + 4 + 4 ! 4 + 4 + 4 + 4 + 4
08963 ! es ds edi esi eip proc scl sof dcl dof
08964
08965 .align 16
08966 _cp_mess:
08967 cld
08968 push esi
08969 push edi
08970 push ds
08971 push es
08972
08973 mov eax, FLAT_DS_SELECTOR
08974 mov ds, ax
08975 mov es, ax
08976
08977 mov esi, CM_ARGS+4(esp) ! src clicks
08978 shl esi, CLICK_SHIFT
08979 add esi, CM_ARGS+4+4(esp) ! src offset
08980 mov edi, CM_ARGS+4+4+4(esp) ! dst clicks
08981 shl edi, CLICK_SHIFT
08982 add edi, CM_ARGS+4+4+4+4(esp) ! dst offset
08983
08984 mov eax, CM_ARGS(esp) ! process number of sender
08985 stos ! copy number of sender to dest message
08986 add esi, 4 ! do not copy first word
08987 mov ecx, Msize - 1 ! remember, first word does not count
08988 rep
08989 movs ! copy the message
08990
08991 pop es
08992 pop ds
08993 pop edi
08994 pop esi
08995 ret ! that is all folks!
08996
08997
08998 !*===========================================================================*
08999 !* exit *
09000 !*===========================================================================*
09001 ! PUBLIC void exit();
09002 ! Some library routines use exit, so provide a dummy version.
09003 ! Actual calls to exit cannot occur in the kernel.
09004 ! GNU CC likes to call ___main from main() for nonobvious reasons.
09005
09006 _exit:
09007 __exit:
09008 ___exit:
09009 sti
09010 jmp ___exit
09011
09012 ___main:
09013 ret
09014
746 File: kernel/klib386.s MINIX SOURCE CODE
09015
09016 !*===========================================================================*
09017 !* phys_insw *
09018 !*===========================================================================*
09019 ! PUBLIC void phys_insw(Port_t port, phys_bytes buf, size_t count);
09020 ! Input an array from an I/O port. Absolute address version of insw().
09021
09022 _phys_insw:
09023 push ebp
09024 mov ebp, esp
09025 cld
09026 push edi
09027 push es
09028 mov ecx, FLAT_DS_SELECTOR
09029 mov es, cx
09030 mov edx, 8(ebp) ! port to read from
09031 mov edi, 12(ebp) ! destination addr
09032 mov ecx, 16(ebp) ! byte count
09033 shr ecx, 1 ! word count
09034 rep o16 ins ! input many words
09035 pop es
09036 pop edi
09037 pop ebp
09038 ret
09039
09040
09041 !*===========================================================================*
09042 !* phys_insb *
09043 !*===========================================================================*
09044 ! PUBLIC void phys_insb(Port_t port, phys_bytes buf, size_t count);
09045 ! Input an array from an I/O port. Absolute address version of insb().
09046
09047 _phys_insb:
09048 push ebp
09049 mov ebp, esp
09050 cld
09051 push edi
09052 push es
09053 mov ecx, FLAT_DS_SELECTOR
09054 mov es, cx
09055 mov edx, 8(ebp) ! port to read from
09056 mov edi, 12(ebp) ! destination addr
09057 mov ecx, 16(ebp) ! byte count
09058 ! shr ecx, 1 ! word count
09059 rep insb ! input many bytes
09060 pop es
09061 pop edi
09062 pop ebp
09063 ret
09064
09065
09066 !*===========================================================================*
09067 !* phys_outsw *
09068 !*===========================================================================*
09069 ! PUBLIC void phys_outsw(Port_t port, phys_bytes buf, size_t count);
09070 ! Output an array to an I/O port. Absolute address version of outsw().
09071
09072 .align 16
09073 _phys_outsw:
09074 push ebp
MINIX SOURCE CODE File: kernel/klib386.s 747
09075 mov ebp, esp
09076 cld
09077 push esi
09078 push ds
09079 mov ecx, FLAT_DS_SELECTOR
09080 mov ds, cx
09081 mov edx, 8(ebp) ! port to write to
09082 mov esi, 12(ebp) ! source addr
09083 mov ecx, 16(ebp) ! byte count
09084 shr ecx, 1 ! word count
09085 rep o16 outs ! output many words
09086 pop ds
09087 pop esi
09088 pop ebp
09089 ret
09090
09091
09092 !*===========================================================================*
09093 !* phys_outsb *
09094 !*===========================================================================*
09095 ! PUBLIC void phys_outsb(Port_t port, phys_bytes buf, size_t count);
09096 ! Output an array to an I/O port. Absolute address version of outsb().
09097
09098 .align 16
09099 _phys_outsb:
09100 push ebp
09101 mov ebp, esp
09102 cld
09103 push esi
09104 push ds
09105 mov ecx, FLAT_DS_SELECTOR
09106 mov ds, cx
09107 mov edx, 8(ebp) ! port to write to
09108 mov esi, 12(ebp) ! source addr
09109 mov ecx, 16(ebp) ! byte count
09110 rep outsb ! output many bytes
09111 pop ds
09112 pop esi
09113 pop ebp
09114 ret
09115
09116
09117 !*==========================================================================*
09118 !* enable_irq *
09119 !*==========================================================================*/
09120 ! PUBLIC void enable_irq(irq_hook_t *hook)
09121 ! Enable an interrupt request line by clearing an 8259 bit.
09122 ! Equivalent C code for hook->irq < 8:
09123 ! if ((irq_actids[hook->irq] &= ˜hook->id) == 0)
09124 ! outb(INT_CTLMASK, inb(INT_CTLMASK) & ˜(1 << irq));
09125
09126 .align 16
09127 _enable_irq:
09128 push ebp
09129 mov ebp, esp
09130 pushf
09131 cli
09132 mov eax, 8(ebp) ! hook
09133 mov ecx, 8(eax) ! irq
09134 mov eax, 12(eax) ! id bit
748 File: kernel/klib386.s MINIX SOURCE CODE
09135 not eax
09136 and _irq_actids(ecx*4), eax ! clear this id bit
09137 jnz en_done ! still masked by other handlers?
09138 movb ah, ˜1
09139 rolb ah, cl ! ah = ˜(1 << (irq % 8))
09140 mov edx, INT_CTLMASK ! enable irq < 8 at the master 8259
09141 cmpb cl, 8
09142 jb 0f
09143 mov edx, INT2_CTLMASK ! enable irq >= 8 at the slave 8259
09144 0: inb dx
09145 andb al, ah
09146 outb dx ! clear bit at the 8259
09147 en_done:popf
09148 leave
09149 ret
09150
09151
09152 !*==========================================================================*
09153 !* disable_irq *
09154 !*==========================================================================*/
09155 ! PUBLIC int disable_irq(irq_hook_t *hook)
09156 ! Disable an interrupt request line by setting an 8259 bit.
09157 ! Equivalent C code for irq < 8:
09158 ! irq_actids[hook->irq] |= hook->id;
09159 ! outb(INT_CTLMASK, inb(INT_CTLMASK) | (1 << irq));
09160 ! Returns true iff the interrupt was not already disabled.
09161
09162 .align 16
09163 _disable_irq:
09164 push ebp
09165 mov ebp, esp
09166 pushf
09167 cli
09168 mov eax, 8(ebp) ! hook
09169 mov ecx, 8(eax) ! irq
09170 mov eax, 12(eax) ! id bit
09171 or _irq_actids(ecx*4), eax ! set this id bit
09172 movb ah, 1
09173 rolb ah, cl ! ah = (1 << (irq % 8))
09174 mov edx, INT_CTLMASK ! disable irq < 8 at the master 8259
09175 cmpb cl, 8
09176 jb 0f
09177 mov edx, INT2_CTLMASK ! disable irq >= 8 at the slave 8259
09178 0: inb dx
09179 testb al, ah
09180 jnz dis_already ! already disabled?
09181 orb al, ah
09182 outb dx ! set bit at the 8259
09183 mov eax, 1 ! disabled by this function
09184 popf
09185 leave
09186 ret
09187 dis_already:
09188 xor eax, eax ! already disabled
09189 popf
09190 leave
09191 ret
09192
09193
MINIX SOURCE CODE File: kernel/klib386.s 749
09194 !*===========================================================================*
09195 !* phys_copy *
09196 !*===========================================================================*
09197 ! PUBLIC void phys_copy(phys_bytes source, phys_bytes destination,
09198 ! phys_bytes bytecount);
09199 ! Copy a block of physical memory.
09200
09201 PC_ARGS = 4 + 4 + 4 + 4 ! 4 + 4 + 4
09202 ! es edi esi eip src dst len
09203
09204 .align 16
09205 _phys_copy:
09206 cld
09207 push esi
09208 push edi
09209 push es
09210
09211 mov eax, FLAT_DS_SELECTOR
09212 mov es, ax
09213
09214 mov esi, PC_ARGS(esp)
09215 mov edi, PC_ARGS+4(esp)
09216 mov eax, PC_ARGS+4+4(esp)
09217
09218 cmp eax, 10 ! avoid align overhead for small counts
09219 jb pc_small
09220 mov ecx, esi ! align source, hope target is too
09221 neg ecx
09222 and ecx, 3 ! count for alignment
09223 sub eax, ecx
09224 rep
09225 eseg movsb
09226 mov ecx, eax
09227 shr ecx, 2 ! count of dwords
09228 rep
09229 eseg movs
09230 and eax, 3
09231 pc_small:
09232 xchg ecx, eax ! remainder
09233 rep
09234 eseg movsb
09235
09236 pop es
09237 pop edi
09238 pop esi
09239 ret
09240
09241 !*===========================================================================*
09242 !* phys_memset *
09243 !*===========================================================================*
09244 ! PUBLIC void phys_memset(phys_bytes source, unsigned long pattern,
09245 ! phys_bytes bytecount);
09246 ! Fill a block of physical memory with pattern.
09247
09248 .align 16
09249 _phys_memset:
09250 push ebp
09251 mov ebp, esp
09252 push esi
09253 push ebx
750 File: kernel/klib386.s MINIX SOURCE CODE
09254 push ds
09255 mov esi, 8(ebp)
09256 mov eax, 16(ebp)
09257 mov ebx, FLAT_DS_SELECTOR
09258 mov ds, bx
09259 mov ebx, 12(ebp)
09260 shr eax, 2
09261 fill_start:
09262 mov (esi), ebx
09263 add esi, 4
09264 dec eax
09265 jnz fill_start
09266 ! Any remaining bytes?
09267 mov eax, 16(ebp)
09268 and eax, 3
09269 remain_fill:
09270 cmp eax, 0
09271 jz fill_done
09272 movb bl, 12(ebp)
09273 movb (esi), bl
09274 add esi, 1
09275 inc ebp
09276 dec eax
09277 jmp remain_fill
09278 fill_done:
09279 pop ds
09280 pop ebx
09281 pop esi
09282 pop ebp
09283 ret
09284
09285 !*===========================================================================*
09286 !* mem_rdw *
09287 !*===========================================================================*
09288 ! PUBLIC u16_t mem_rdw(U16_t segment, u16_t *offset);
09289 ! Load and return word at far pointer segment:offset.
09290
09291 .align 16
09292 _mem_rdw:
09293 mov cx, ds
09294 mov ds, 4(esp) ! segment
09295 mov eax, 4+4(esp) ! offset
09296 movzx eax, (eax) ! word to return
09297 mov ds, cx
09298 ret
09299
09300
09301 !*===========================================================================*
09302 !* reset *
09303 !*===========================================================================*
09304 ! PUBLIC void reset();
09305 ! Reset the system by loading IDT with offset 0 and interrupting.
09306
09307 _reset:
09308 lidt (idt_zero)
09309 int 3 ! anything goes, the 386 will not like it
09310 .sect .data
09311 idt_zero: .data4 0, 0
09312 .sect .text
09313
MINIX SOURCE CODE File: kernel/klib386.s 751
09314
09315 !*===========================================================================*
09316 !* idle_task *
09317 !*===========================================================================*
09318 _idle_task:
09319 ! This task is called when the system has nothing else to do. The HLT
09320 ! instruction puts the processor in a state where it draws minimum power.
09321 push halt
09322 call _level0 ! level0(halt)
09323 pop eax
09324 jmp _idle_task
09325 halt:
09326 sti
09327 hlt
09328 cli
09329 ret
09330
09331 !*===========================================================================*
09332 !* level0 *
09333 !*===========================================================================*
09334 ! PUBLIC void level0(void (*func)(void))
09335 ! Call a function at permission level 0. This allows kernel tasks to do
09336 ! things that are only possible at the most privileged CPU level.
09337 !
09338 _level0:
09339 mov eax, 4(esp)
09340 mov (_level0_func), eax
09341 int LEVEL0_VECTOR
09342 ret
09343
09344
09345 !*===========================================================================*
09346 !* read_tsc *
09347 !*===========================================================================*
09348 ! PUBLIC void read_tsc(unsigned long *high, unsigned long *low);
09349 ! Read the cycle counter of the CPU. Pentium and up.
09350 .align 16
09351 _read_tsc:
09352 .data1 0x0f ! this is the RDTSC instruction
09353 .data1 0x31 ! it places the TSC in EDX:EAX
09354 push ebp
09355 mov ebp, 8(esp)
09356 mov (ebp), edx
09357 mov ebp, 12(esp)
09358 mov (ebp), eax
09359 pop ebp
09360 ret
09361
09362 !*===========================================================================*
09363 !* read_flags *
09364 !*===========================================================================*
09365 ! PUBLIC unsigned long read_cpu_flags(void);
09366 ! Read CPU status flags from C.
09367 .align 16
09368 _read_cpu_flags:
09369 pushf
09370 mov eax, (esp)
09371 popf
09372 ret
09373
752 File: kernel/utility.c MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/utility.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
09400 /* This file contains a collection of miscellaneous procedures:
09401 * panic: abort MINIX due to a fatal error
09402 * kprintf: diagnostic output for the kernel
09403 *
09404 * Changes:
09405 * Dec 10, 2004 kernel printing to circular buffer (Jorrit N. Herder)
09406 *
09407 * This file contains the routines that take care of kernel messages, i.e.,
09408 * diagnostic output within the kernel. Kernel messages are not directly
09409 * displayed on the console, because this must be done by the output driver.
09410 * Instead, the kernel accumulates characters in a buffer and notifies the
09411 * output driver when a new message is ready.
09412 */
09413
09414 #include <minix/com.h>
09415 #include "kernel.h"
09416 #include <stdarg.h>
09417 #include <unistd.h>
09418 #include <stddef.h>
09419 #include <stdlib.h>
09420 #include <signal.h>
09421 #include "proc.h"
09422
09423 #define END_OF_KMESS -1
09424 FORWARD _PROTOTYPE(void kputc, (int c));
09425
09426 /*===========================================================================*
09427 * panic *
09428 *===========================================================================*/
09429 PUBLIC void panic(mess,nr)
09430 _CONST char *mess;
09431 int nr;
09432 {
09433 /* The system has run aground of a fatal kernel error. Terminate execution. */
09434 static int panicking = 0;
09435 if (panicking ++) return; /* prevent recursive panics */
09436
09437 if (mess != NULL) {
09438 kprintf("\nKernel panic: %s", mess);
09439 if (nr != NO_NUM) kprintf(" %d", nr);
09440 kprintf("\n",NO_NUM);
09441 }
09442
09443 /* Abort MINIX. */
09444 prepare_shutdown(RBT_PANIC);
09445 }
09447 /*===========================================================================*
09448 * kprintf *
09449 *===========================================================================*/
09450 PUBLIC void kprintf(const char *fmt, ...) /* format to be printed */
09451 {
09452 int c; /* next character in fmt */
09453 int d;
09454 unsigned long u; /* hold number argument */
MINIX SOURCE CODE File: kernel/utility.c 753
09455 int base; /* base of number arg */
09456 int negative = 0; /* print minus sign */
09457 static char x2c[] = "0123456789ABCDEF"; /* nr conversion table */
09458 char ascii[8 * sizeof(long) / 3 + 2]; /* string for ascii number */
09459 char *s = NULL; /* string to be printed */
09460 va_list argp; /* optional arguments */
09461
09462 va_start(argp, fmt); /* init variable arguments */
09463
09464 while((c=*fmt++) != 0) {
09465
09466 if (c == ’%’) { /* expect format ’%key’ */
09467 switch(c = *fmt++) { /* determine what to do */
09468
09469 /* Known keys are %d, %u, %x, %s, and %%. This is easily extended
09470 * with number types like %b and %o by providing a different base.
09471 * Number type keys don’t set a string to ’s’, but use the general
09472 * conversion after the switch statement.
09473 */
09474 case ’d’: /* output decimal */
09475 d = va_arg(argp, signed int);
09476 if (d < 0) { negative = 1; u = -d; } else { u = d; }
09477 base = 10;
09478 break;
09479 case ’u’: /* output unsigned long */
09480 u = va_arg(argp, unsigned long);
09481 base = 10;
09482 break;
09483 case ’x’: /* output hexadecimal */
09484 u = va_arg(argp, unsigned long);
09485 base = 0x10;
09486 break;
09487 case ’s’: /* output string */
09488 s = va_arg(argp, char *);
09489 if (s == NULL) s = "(null)";
09490 break;
09491 case ’%’: /* output percent */
09492 s = "%";
09493 break;
09494
09495 /* Unrecognized key. */
09496 default: /* echo back %key */
09497 s = "%?";
09498 s[1] = c; /* set unknown key */
09499 }
09500
09501 /* Assume a number if no string is set. Convert to ascii. */
09502 if (s == NULL) {
09503 s = ascii + sizeof(ascii)-1;
09504 *s = 0;
09505 do { *--s = x2c[(u % base)]; } /* work backwards */
09506 while ((u /= base) > 0);
09507 }
09508
09509 /* This is where the actual output for format "%key" is done. */
09510 if (negative) kputc(’-’); /* print sign if negative */
09511 while(*s != 0) { kputc(*s++); } /* print string/ number */
09512 s = NULL; /* reset for next round */
09513 }
09514 else {
754 File: kernel/utility.c MINIX SOURCE CODE
09515 kputc(c); /* print and continue */
09516 }
09517 }
09518 kputc(END_OF_KMESS); /* terminate output */
09519 va_end(argp); /* end variable arguments */
09520 }
09522 /*===========================================================================*
09523 * kputc *
09524 *===========================================================================*/
09525 PRIVATE void kputc(c)
09526 int c; /* character to append */
09527 {
09528 /* Accumulate a single character for a kernel message. Send a notification
09529 * to the output driver if an END_OF_KMESS is encountered.
09530 */
09531 if (c != END_OF_KMESS) {
09532 kmess.km_buf[kmess.km_next] = c; /* put normal char in buffer */
09533 if (kmess.km_size < KMESS_BUF_SIZE)
09534 kmess.km_size += 1;
09535 kmess.km_next = (kmess.km_next + 1) % KMESS_BUF_SIZE;
09536 } else {
09537 send_sig(OUTPUT_PROC_NR, SIGKMESS);
09538 }
09539 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/system.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
09600 /* Function prototypes for the system library.
09601 * The implementation is contained in src/kernel/system/.
09602 *
09603 * The system library allows access to system services by doing a kernel call.
09604 * Kernel calls are transformed into request messages to the SYS task that is
09605 * responsible for handling the call. By convention, sys_call() is transformed
09606 * into a message with type SYS_CALL that is handled in a function do_call().
09607 */
09608
09609 #ifndef SYSTEM_H
09610 #define SYSTEM_H
09611
09612 /* Common includes for the system library. */
09613 #include "kernel.h"
09614 #include "proto.h"
09615 #include "proc.h"
09616
09617 /* Default handler for unused kernel calls. */
09618 _PROTOTYPE( int do_unused, (message *m_ptr) );
09619 _PROTOTYPE( int do_exec, (message *m_ptr) );
09620 _PROTOTYPE( int do_fork, (message *m_ptr) );
09621 _PROTOTYPE( int do_newmap, (message *m_ptr) );
09622 _PROTOTYPE( int do_exit, (message *m_ptr) );
09623 _PROTOTYPE( int do_trace, (message *m_ptr) );
09624 _PROTOTYPE( int do_nice, (message *m_ptr) );
MINIX SOURCE CODE File: kernel/system.h 755
09625 _PROTOTYPE( int do_copy, (message *m_ptr) );
09626 #define do_vircopy do_copy
09627 #define do_physcopy do_copy
09628 _PROTOTYPE( int do_vcopy, (message *m_ptr) );
09629 #define do_virvcopy do_vcopy
09630 #define do_physvcopy do_vcopy
09631 _PROTOTYPE( int do_umap, (message *m_ptr) );
09632 _PROTOTYPE( int do_memset, (message *m_ptr) );
09633 _PROTOTYPE( int do_abort, (message *m_ptr) );
09634 _PROTOTYPE( int do_getinfo, (message *m_ptr) );
09635 _PROTOTYPE( int do_privctl, (message *m_ptr) );
09636 _PROTOTYPE( int do_segctl, (message *m_ptr) );
09637 _PROTOTYPE( int do_irqctl, (message *m_ptr) );
09638 _PROTOTYPE( int do_devio, (message *m_ptr) );
09639 _PROTOTYPE( int do_vdevio, (message *m_ptr) );
09640 _PROTOTYPE( int do_int86, (message *m_ptr) );
09641 _PROTOTYPE( int do_sdevio, (message *m_ptr) );
09642 _PROTOTYPE( int do_kill, (message *m_ptr) );
09643 _PROTOTYPE( int do_getksig, (message *m_ptr) );
09644 _PROTOTYPE( int do_endksig, (message *m_ptr) );
09645 _PROTOTYPE( int do_sigsend, (message *m_ptr) );
09646 _PROTOTYPE( int do_sigreturn, (message *m_ptr) );
09647 _PROTOTYPE( int do_times, (message *m_ptr) );
09648 _PROTOTYPE( int do_setalarm, (message *m_ptr) );
09649
09650 #endif /* SYSTEM_H */
09651
09652
09653
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/system.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
09700 /* This task provides an interface between the kernel and user-space system
09701 * processes. System services can be accessed by doing a kernel call. Kernel
09702 * calls are transformed into request messages, which are handled by this
09703 * task. By convention, a sys_call() is transformed in a SYS_CALL request
09704 * message that is handled in a function named do_call().
09705 *
09706 * A private call vector is used to map all kernel calls to the functions that
09707 * handle them. The actual handler functions are contained in separate files
09708 * to keep this file clean. The call vector is used in the system task’s main
09709 * loop to handle all incoming requests.
09710 *
09711 * In addition to the main sys_task() entry point, which starts the main loop,
09712 * there are several other minor entry points:
09713 * get_priv: assign privilege structure to user or system process
09714 * send_sig: send a signal directly to a system process
09715 * cause_sig: take action to cause a signal to occur via PM
09716 * umap_local: map virtual address in LOCAL_SEG to physical
09717 * umap_remote: map virtual address in REMOTE_SEG to physical
09718 * umap_bios: map virtual address in BIOS_SEG to physical
09719 * virtual_copy: copy bytes from one virtual address to another
09720 * get_randomness: accumulate randomness in a buffer
09721 *
09722 * Changes:
09723 * Aug 04, 2005 check if kernel call is allowed (Jorrit N. Herder)
09724 * Jul 20, 2005 send signal to services with message (Jorrit N. Herder)
756 File: kernel/system.c MINIX SOURCE CODE
09725 * Jan 15, 2005 new, generalized virtual copy function (Jorrit N. Herder)
09726 * Oct 10, 2004 dispatch system calls from call vector (Jorrit N. Herder)
09727 * Sep 30, 2004 source code documentation updated (Jorrit N. Herder)
09728 */
09729
09730 #include "kernel.h"
09731 #include "system.h"
09732 #include <stdlib.h>
09733 #include <signal.h>
09734 #include <unistd.h>
09735 #include <sys/sigcontext.h>
09736 #include <ibm/memory.h>
09737 #include "protect.h"
09738
09739 /* Declaration of the call vector that defines the mapping of kernel calls
09740 * to handler functions. The vector is initialized in sys_init() with map(),
09741 * which makes sure the kernel call numbers are ok. No space is allocated,
09742 * because the dummy is declared extern. If an illegal call is given, the
09743 * array size will be negative and this won’t compile.
09744 */
09745 PUBLIC int (*call_vec[NR_SYS_CALLS])(message *m_ptr);
09746
09747 #define map(call_nr, handler) \
09748 {extern int dummy[NR_SYS_CALLS>(unsigned)(call_nr-KERNEL_CALL) ? 1:-1];} \
09749 call_vec[(call_nr-KERNEL_CALL)] = (handler)
09750
09751 FORWARD _PROTOTYPE( void initialize, (void));
09752
09753 /*===========================================================================*
09754 * sys_task *
09755 *===========================================================================*/
09756 PUBLIC void sys_task()
09757 {
09758 /* Main entry point of sys_task. Get the message and dispatch on type. */
09759 static message m;
09760 register int result;
09761 register struct proc *caller_ptr;
09762 unsigned int call_nr;
09763 int s;
09764
09765 /* Initialize the system task. */
09766 initialize();
09767
09768 while (TRUE) {
09769 /* Get work. Block and wait until a request message arrives. */
09770 receive(ANY, &m);
09771 call_nr = (unsigned) m.m_type - KERNEL_CALL;
09772 caller_ptr = proc_addr(m.m_source);
09773
09774 /* See if the caller made a valid request and try to handle it. */
09775 if (! (priv(caller_ptr)->s_call_mask & (1<<call_nr))) {
09776 kprintf("SYSTEM: request %d from %d denied.\n", call_nr,m.m_source);
09777 result = ECALLDENIED; /* illegal message type */
09778 } else if (call_nr >= NR_SYS_CALLS) { /* check call number */
09779 kprintf("SYSTEM: illegal request %d from %d.\n", call_nr,m.m_source);
09780 result = EBADREQUEST; /* illegal message type */
09781 }
09782 else {
09783 result = (*call_vec[call_nr])(&m); /* handle the kernel call */
09784 }
MINIX SOURCE CODE File: kernel/system.c 757
09785
09786 /* Send a reply, unless inhibited by a handler function. Use the kernel
09787 * function lock_send() to prevent a system call trap. The destination
09788 * is known to be blocked waiting for a message.
09789 */
09790 if (result != EDONTREPLY) {
09791 m.m_type = result; /* report status of call */
09792 if (OK != (s=lock_send(m.m_source, &m))) {
09793 kprintf("SYSTEM, reply to %d failed: %d\n", m.m_source, s);
09794 }
09795 }
09796 }
09797 }
09799 /*===========================================================================*
09800 * initialize *
09801 *===========================================================================*/
09802 PRIVATE void initialize(void)
09803 {
09804 register struct priv *sp;
09805 int i;
09806
09807 /* Initialize IRQ handler hooks. Mark all hooks available. */
09808 for (i=0; i<NR_IRQ_HOOKS; i++) {
09809 irq_hooks[i].proc_nr = NONE;
09810 }
09811
09812 /* Initialize all alarm timers for all processes. */
09813 for (sp=BEG_PRIV_ADDR; sp < END_PRIV_ADDR; sp++) {
09814 tmr_inittimer(&(sp->s_alarm_timer));
09815 }
09816
09817 /* Initialize the call vector to a safe default handler. Some kernel calls
09818 * may be disabled or nonexistant. Then explicitly map known calls to their
09819 * handler functions. This is done with a macro that gives a compile error
09820 * if an illegal call number is used. The ordering is not important here.
09821 */
09822 for (i=0; i<NR_SYS_CALLS; i++) {
09823 call_vec[i] = do_unused;
09824 }
09825
09826 /* Process management. */
09827 map(SYS_FORK, do_fork); /* a process forked a new process */
09828 map(SYS_EXEC, do_exec); /* update process after execute */
09829 map(SYS_EXIT, do_exit); /* clean up after process exit */
09830 map(SYS_NICE, do_nice); /* set scheduling priority */
09831 map(SYS_PRIVCTL, do_privctl); /* system privileges control */
09832 map(SYS_TRACE, do_trace); /* request a trace operation */
09833
09834 /* Signal handling. */
09835 map(SYS_KILL, do_kill); /* cause a process to be signaled */
09836 map(SYS_GETKSIG, do_getksig); /* PM checks for pending signals */
09837 map(SYS_ENDKSIG, do_endksig); /* PM finished processing signal */
09838 map(SYS_SIGSEND, do_sigsend); /* start POSIX-style signal */
09839 map(SYS_SIGRETURN, do_sigreturn); /* return from POSIX-style signal */
09840
09841 /* Device I/O. */
09842 map(SYS_IRQCTL, do_irqctl); /* interrupt control operations */
09843 map(SYS_DEVIO, do_devio); /* inb, inw, inl, outb, outw, outl */
09844 map(SYS_SDEVIO, do_sdevio); /* phys_insb, _insw, _outsb, _outsw */
758 File: kernel/system.c MINIX SOURCE CODE
09845 map(SYS_VDEVIO, do_vdevio); /* vector with devio requests */
09846 map(SYS_INT86, do_int86); /* real-mode BIOS calls */
09847
09848 /* Memory management. */
09849 map(SYS_NEWMAP, do_newmap); /* set up a process memory map */
09850 map(SYS_SEGCTL, do_segctl); /* add segment and get selector */
09851 map(SYS_MEMSET, do_memset); /* write char to memory area */
09852
09853 /* Copying. */
09854 map(SYS_UMAP, do_umap); /* map virtual to physical address */
09855 map(SYS_VIRCOPY, do_vircopy); /* use pure virtual addressing */
09856 map(SYS_PHYSCOPY, do_physcopy); /* use physical addressing */
09857 map(SYS_VIRVCOPY, do_virvcopy); /* vector with copy requests */
09858 map(SYS_PHYSVCOPY, do_physvcopy); /* vector with copy requests */
09859
09860 /* Clock functionality. */
09861 map(SYS_TIMES, do_times); /* get uptime and process times */
09862 map(SYS_SETALARM, do_setalarm); /* schedule a synchronous alarm */
09863
09864 /* System control. */
09865 map(SYS_ABORT, do_abort); /* abort MINIX */
09866 map(SYS_GETINFO, do_getinfo); /* request system information */
09867 }
09869 /*===========================================================================*
09870 * get_priv *
09871 *===========================================================================*/
09872 PUBLIC int get_priv(rc, proc_type)
09873 register struct proc *rc; /* new (child) process pointer */
09874 int proc_type; /* system or user process flag */
09875 {
09876 /* Get a privilege structure. All user processes share the same privilege
09877 * structure. System processes get their own privilege structure.
09878 */
09879 register struct priv *sp; /* privilege structure */
09880
09881 if (proc_type == SYS_PROC) { /* find a new slot */
09882 for (sp = BEG_PRIV_ADDR; sp < END_PRIV_ADDR; ++sp)
09883 if (sp->s_proc_nr == NONE && sp->s_id != USER_PRIV_ID) break;
09884 if (sp->s_proc_nr != NONE) return(ENOSPC);
09885 rc->p_priv = sp; /* assign new slot */
09886 rc->p_priv->s_proc_nr = proc_nr(rc); /* set association */
09887 rc->p_priv->s_flags = SYS_PROC; /* mark as privileged */
09888 } else {
09889 rc->p_priv = &priv[USER_PRIV_ID]; /* use shared slot */
09890 rc->p_priv->s_proc_nr = INIT_PROC_NR; /* set association */
09891 rc->p_priv->s_flags = 0; /* no initial flags */
09892 }
09893 return(OK);
09894 }
09896 /*===========================================================================*
09897 * get_randomness *
09898 *===========================================================================*/
09899 PUBLIC void get_randomness(source)
09900 int source;
09901 {
09902 /* On machines with the RDTSC (cycle counter read instruction - pentium
09903 * and up), use that for high-resolution raw entropy gathering. Otherwise,
09904 * use the realtime clock (tick resolution).
MINIX SOURCE CODE File: kernel/system.c 759
09905 *
09906 * Unfortunately this test is run-time - we don’t want to bother with
09907 * compiling different kernels for different machines.
09908 *
09909 * On machines without RDTSC, we use read_clock().
09910 */
09911 int r_next;
09912 unsigned long tsc_high, tsc_low;
09913
09914 source %= RANDOM_SOURCES;
09915 r_next= krandom.bin[source].r_next;
09916 if (machine.processor > 486) {
09917 read_tsc(&tsc_high, &tsc_low);
09918 krandom.bin[source].r_buf[r_next] = tsc_low;
09919 } else {
09920 krandom.bin[source].r_buf[r_next] = read_clock();
09921 }
09922 if (krandom.bin[source].r_size < RANDOM_ELEMENTS) {
09923 krandom.bin[source].r_size ++;
09924 }
09925 krandom.bin[source].r_next = (r_next + 1 ) % RANDOM_ELEMENTS;
09926 }
09928 /*===========================================================================*
09929 * send_sig *
09930 *===========================================================================*/
09931 PUBLIC void send_sig(proc_nr, sig_nr)
09932 int proc_nr; /* system process to be signalled */
09933 int sig_nr; /* signal to be sent, 1 to _NSIG */
09934 {
09935 /* Notify a system process about a signal. This is straightforward. Simply
09936 * set the signal that is to be delivered in the pending signals map and
09937 * send a notification with source SYSTEM.
09938 */
09939 register struct proc *rp;
09940
09941 rp = proc_addr(proc_nr);
09942 sigaddset(&priv(rp)->s_sig_pending, sig_nr);
09943 lock_notify(SYSTEM, proc_nr);
09944 }
09946 /*===========================================================================*
09947 * cause_sig *
09948 *===========================================================================*/
09949 PUBLIC void cause_sig(proc_nr, sig_nr)
09950 int proc_nr; /* process to be signalled */
09951 int sig_nr; /* signal to be sent, 1 to _NSIG */
09952 {
09953 /* A system process wants to send a signal to a process. Examples are:
09954 * - HARDWARE wanting to cause a SIGSEGV after a CPU exception
09955 * - TTY wanting to cause SIGINT upon getting a DEL
09956 * - FS wanting to cause SIGPIPE for a broken pipe
09957 * Signals are handled by sending a message to PM. This function handles the
09958 * signals and makes sure the PM gets them by sending a notification. The
09959 * process being signaled is blocked while PM has not finished all signals
09960 * for it.
09961 * Race conditions between calls to this function and the system calls that
09962 * process pending kernel signals cannot exist. Signal related functions are
09963 * only called when a user process causes a CPU exception and from the kernel
09964 * process level, which runs to completion.
760 File: kernel/system.c MINIX SOURCE CODE
09965 */
09966 register struct proc *rp;
09967
09968 /* Check if the signal is already pending. Process it otherwise. */
09969 rp = proc_addr(proc_nr);
09970 if (! sigismember(&rp->p_pending, sig_nr)) {
09971 sigaddset(&rp->p_pending, sig_nr);
09972 if (! (rp->p_rts_flags & SIGNALED)) { /* other pending */
09973 if (rp->p_rts_flags == 0) lock_dequeue(rp); /* make not ready */
09974 rp->p_rts_flags |= SIGNALED | SIG_PENDING; /* update flags */
09975 send_sig(PM_PROC_NR, SIGKSIG);
09976 }
09977 }
09978 }
09980 /*===========================================================================*
09981 * umap_local *
09982 *===========================================================================*/
09983 PUBLIC phys_bytes umap_local(rp, seg, vir_addr, bytes)
09984 register struct proc *rp; /* pointer to proc table entry for process */
09985 int seg; /* T, D, or S segment */
09986 vir_bytes vir_addr; /* virtual address in bytes within the seg */
09987 vir_bytes bytes; /* # of bytes to be copied */
09988 {
09989 /* Calculate the physical memory address for a given virtual address. */
09990 vir_clicks vc; /* the virtual address in clicks */
09991 phys_bytes pa; /* intermediate variables as phys_bytes */
09992 phys_bytes seg_base;
09993
09994 /* If ’seg’ is D it could really be S and vice versa. T really means T.
09995 * If the virtual address falls in the gap, it causes a problem. On the
09996 * 8088 it is probably a legal stack reference, since "stackfaults" are
09997 * not detected by the hardware. On 8088s, the gap is called S and
09998 * accepted, but on other machines it is called D and rejected.
09999 * The Atari ST behaves like the 8088 in this respect.
10000 */
10001
10002 if (bytes <= 0) return( (phys_bytes) 0);
10003 if (vir_addr + bytes <= vir_addr) return 0; /* overflow */
10004 vc = (vir_addr + bytes - 1) >> CLICK_SHIFT; /* last click of data */
10005
10006 if (seg != T)
10007 seg = (vc < rp->p_memmap[D].mem_vir + rp->p_memmap[D].mem_len ? D : S);
10008
10009 if ((vir_addr>>CLICK_SHIFT) >= rp->p_memmap[seg].mem_vir +
10010 rp->p_memmap[seg].mem_len) return( (phys_bytes) 0 );
10011
10012 if (vc >= rp->p_memmap[seg].mem_vir +
10013 rp->p_memmap[seg].mem_len) return( (phys_bytes) 0 );
10014
10015 seg_base = (phys_bytes) rp->p_memmap[seg].mem_phys;
10016 seg_base = seg_base << CLICK_SHIFT; /* segment origin in bytes */
10017 pa = (phys_bytes) vir_addr;
10018 pa -= rp->p_memmap[seg].mem_vir << CLICK_SHIFT;
10019 return(seg_base + pa);
10020 }
MINIX SOURCE CODE File: kernel/system.c 761
10022 /*===========================================================================*
10023 * umap_remote *
10024 *===========================================================================*/
10025 PUBLIC phys_bytes umap_remote(rp, seg, vir_addr, bytes)
10026 register struct proc *rp; /* pointer to proc table entry for process */
10027 int seg; /* index of remote segment */
10028 vir_bytes vir_addr; /* virtual address in bytes within the seg */
10029 vir_bytes bytes; /* # of bytes to be copied */
10030 {
10031 /* Calculate the physical memory address for a given virtual address. */
10032 struct far_mem *fm;
10033
10034 if (bytes <= 0) return( (phys_bytes) 0);
10035 if (seg < 0 || seg >= NR_REMOTE_SEGS) return( (phys_bytes) 0);
10036
10037 fm = &rp->p_priv->s_farmem[seg];
10038 if (! fm->in_use) return( (phys_bytes) 0);
10039 if (vir_addr + bytes > fm->mem_len) return( (phys_bytes) 0);
10040
10041 return(fm->mem_phys + (phys_bytes) vir_addr);
10042 }
10044 /*===========================================================================*
10045 * umap_bios *
10046 *===========================================================================*/
10047 PUBLIC phys_bytes umap_bios(rp, vir_addr, bytes)
10048 register struct proc *rp; /* pointer to proc table entry for process */
10049 vir_bytes vir_addr; /* virtual address in BIOS segment */
10050 vir_bytes bytes; /* # of bytes to be copied */
10051 {
10052 /* Calculate the physical memory address at the BIOS. Note: currently, BIOS
10053 * address zero (the first BIOS interrupt vector) is not considered as an
10054 * error here, but since the physical address will be zero as well, the
10055 * calling function will think an error occurred. This is not a problem,
10056 * since no one uses the first BIOS interrupt vector.
10057 */
10058
10059 /* Check all acceptable ranges. */
10060 if (vir_addr >= BIOS_MEM_BEGIN && vir_addr + bytes <= BIOS_MEM_END)
10061 return (phys_bytes) vir_addr;
10062 else if (vir_addr >= BASE_MEM_TOP && vir_addr + bytes <= UPPER_MEM_END)
10063 return (phys_bytes) vir_addr;
10064 kprintf("Warning, error in umap_bios, virtual address 0x%x\n", vir_addr);
10065 return 0;
10066 }
10068 /*===========================================================================*
10069 * virtual_copy *
10070 *===========================================================================*/
10071 PUBLIC int virtual_copy(src_addr, dst_addr, bytes)
10072 struct vir_addr *src_addr; /* source virtual address */
10073 struct vir_addr *dst_addr; /* destination virtual address */
10074 vir_bytes bytes; /* # of bytes to copy */
10075 {
10076 /* Copy bytes from virtual address src_addr to virtual address dst_addr.
10077 * Virtual addresses can be in ABS, LOCAL_SEG, REMOTE_SEG, or BIOS_SEG.
10078 */
10079 struct vir_addr *vir_addr[2]; /* virtual source and destination address */
10080 phys_bytes phys_addr[2]; /* absolute source and destination */
10081 int seg_index;
762 File: kernel/system.c MINIX SOURCE CODE
10082 int i;
10083
10084 /* Check copy count. */
10085 if (bytes <= 0) return(EDOM);
10086
10087 /* Do some more checks and map virtual addresses to physical addresses. */
10088 vir_addr[_SRC_] = src_addr;
10089 vir_addr[_DST_] = dst_addr;
10090 for (i=_SRC_; i<=_DST_; i++) {
10091
10092 /* Get physical address. */
10093 switch((vir_addr[i]->segment & SEGMENT_TYPE)) {
10094 case LOCAL_SEG:
10095 seg_index = vir_addr[i]->segment & SEGMENT_INDEX;
10096 phys_addr[i] = umap_local( proc_addr(vir_addr[i]->proc_nr),
10097 seg_index, vir_addr[i]->offset, bytes );
10098 break;
10099 case REMOTE_SEG:
10100 seg_index = vir_addr[i]->segment & SEGMENT_INDEX;
10101 phys_addr[i] = umap_remote( proc_addr(vir_addr[i]->proc_nr),
10102 seg_index, vir_addr[i]->offset, bytes );
10103 break;
10104 case BIOS_SEG:
10105 phys_addr[i] = umap_bios( proc_addr(vir_addr[i]->proc_nr),
10106 vir_addr[i]->offset, bytes );
10107 break;
10108 case PHYS_SEG:
10109 phys_addr[i] = vir_addr[i]->offset;
10110 break;
10111 default:
10112 return(EINVAL);
10113 }
10114
10115 /* Check if mapping succeeded. */
10116 if (phys_addr[i] <= 0 && vir_addr[i]->segment != PHYS_SEG)
10117 return(EFAULT);
10118 }
10119
10120 /* Now copy bytes between physical addresseses. */
10121 phys_copy(phys_addr[_SRC_], phys_addr[_DST_], (phys_bytes) bytes);
10122 return(OK);
10123 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/system/do_setalarm.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
10200 /* The kernel call implemented in this file:
10201 * m_type: SYS_SETALARM
10202 *
10203 * The parameters for this kernel call are:
10204 * m2_l1: ALRM_EXP_TIME (alarm’s expiration time)
10205 * m2_i2: ALRM_ABS_TIME (expiration time is absolute?)
10206 * m2_l1: ALRM_TIME_LEFT (return seconds left of previous)
10207 */
10208
10209 #include "../system.h"
MINIX SOURCE CODE File: kernel/system/do_setalarm.c 763
10210
10211 #if USE_SETALARM
10212
10213 FORWARD _PROTOTYPE( void cause_alarm, (timer_t *tp) );
10214
10215 /*===========================================================================*
10216 * do_setalarm *
10217 *===========================================================================*/
10218 PUBLIC int do_setalarm(m_ptr)
10219 message *m_ptr; /* pointer to request message */
10220 {
10221 /* A process requests a synchronous alarm, or wants to cancel its alarm. */
10222 register struct proc *rp; /* pointer to requesting process */
10223 int proc_nr; /* which process wants the alarm */
10224 long exp_time; /* expiration time for this alarm */
10225 int use_abs_time; /* use absolute or relative time */
10226 timer_t *tp; /* the process’ timer structure */
10227 clock_t uptime; /* placeholder for current uptime */
10228
10229 /* Extract shared parameters from the request message. */
10230 exp_time = m_ptr->ALRM_EXP_TIME; /* alarm’s expiration time */
10231 use_abs_time = m_ptr->ALRM_ABS_TIME; /* flag for absolute time */
10232 proc_nr = m_ptr->m_source; /* process to interrupt later */
10233 rp = proc_addr(proc_nr);
10234 if (! (priv(rp)->s_flags & SYS_PROC)) return(EPERM);
10235
10236 /* Get the timer structure and set the parameters for this alarm. */
10237 tp = &(priv(rp)->s_alarm_timer);
10238 tmr_arg(tp)->ta_int = proc_nr;
10239 tp->tmr_func = cause_alarm;
10240
10241 /* Return the ticks left on the previous alarm. */
10242 uptime = get_uptime();
10243 if ((tp->tmr_exp_time != TMR_NEVER) && (uptime < tp->tmr_exp_time) ) {
10244 m_ptr->ALRM_TIME_LEFT = (tp->tmr_exp_time - uptime);
10245 } else {
10246 m_ptr->ALRM_TIME_LEFT = 0;
10247 }
10248
10249 /* Finally, (re)set the timer depending on the expiration time. */
10250 if (exp_time == 0) {
10251 reset_timer(tp);
10252 } else {
10253 tp->tmr_exp_time = (use_abs_time) ? exp_time : exp_time + get_uptime();
10254 set_timer(tp, tp->tmr_exp_time, tp->tmr_func);
10255 }
10256 return(OK);
10257 }
10259 /*===========================================================================*
10260 * cause_alarm *
10261 *===========================================================================*/
10262 PRIVATE void cause_alarm(tp)
10263 timer_t *tp;
10264 {
10265 /* Routine called if a timer goes off and the process requested a synchronous
10266 * alarm. The process number is stored in timer argument ’ta_int’. Notify that
10267 * process with a notification message from CLOCK.
10268 */
10269 int proc_nr = tmr_arg(tp)->ta_int; /* get process number */
764 File: kernel/system/do_setalarm.c MINIX SOURCE CODE
10270 lock_notify(CLOCK, proc_nr); /* notify process */
10271 }
10273 #endif /* USE_SETALARM */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/system/do_exec.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
10300 /* The kernel call implemented in this file:
10301 * m_type: SYS_EXEC
10302 *
10303 * The parameters for this kernel call are:
10304 * m1_i1: PR_PROC_NR (process that did exec call)
10305 * m1_p1: PR_STACK_PTR (new stack pointer)
10306 * m1_p2: PR_NAME_PTR (pointer to program name)
10307 * m1_p3: PR_IP_PTR (new instruction pointer)
10308 */
10309 #include "../system.h"
10310 #include <string.h>
10311 #include <signal.h>
10312
10313 #if USE_EXEC
10314
10315 /*===========================================================================*
10316 * do_exec *
10317 *===========================================================================*/
10318 PUBLIC int do_exec(m_ptr)
10319 register message *m_ptr; /* pointer to request message */
10320 {
10321 /* Handle sys_exec(). A process has done a successful EXEC. Patch it up. */
10322 register struct proc *rp;
10323 reg_t sp; /* new sp */
10324 phys_bytes phys_name;
10325 char *np;
10326
10327 rp = proc_addr(m_ptr->PR_PROC_NR);
10328 sp = (reg_t) m_ptr->PR_STACK_PTR;
10329 rp->p_reg.sp = sp; /* set the stack pointer */
10330 phys_memset(vir2phys(&rp->p_ldt[EXTRA_LDT_INDEX]), 0,
10331 (LDT_SIZE - EXTRA_LDT_INDEX) * sizeof(rp->p_ldt[0]));
10332 rp->p_reg.pc = (reg_t) m_ptr->PR_IP_PTR; /* set pc */
10333 rp->p_rts_flags &= ˜RECEIVING; /* PM does not reply to EXEC call */
10334 if (rp->p_rts_flags == 0) lock_enqueue(rp);
10335
10336 /* Save command name for debugging, ps(1) output, etc. */
10337 phys_name = numap_local(m_ptr->m_source, (vir_bytes) m_ptr->PR_NAME_PTR,
10338 (vir_bytes) P_NAME_LEN - 1);
10339 if (phys_name != 0) {
10340 phys_copy(phys_name, vir2phys(rp->p_name), (phys_bytes) P_NAME_LEN - 1);
10341 for (np = rp->p_name; (*np & BYTE) >= ’ ’; np++) {}
10342 *np = 0; /* mark end */
10343 } else {
10344 strncpy(rp->p_name, "<unset>", P_NAME_LEN);
10345 }
10346 return(OK);
10347 }
10348 #endif /* USE_EXEC */
MINIX SOURCE CODE File: kernel/clock.c 765
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/clock.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
10400 /* This file contains the clock task, which handles time related functions.
10401 * Important events that are handled by the CLOCK include setting and
10402 * monitoring alarm timers and deciding when to (re)schedule processes.
10403 * The CLOCK offers a direct interface to kernel processes. System services
10404 * can access its services through system calls, such as sys_setalarm(). The
10405 * CLOCK task thus is hidden from the outside world.
10406 *
10407 * Changes:
10408 * Oct 08, 2005 reordering and comment editing (A. S. Woodhull)
10409 * Mar 18, 2004 clock interface moved to SYSTEM task (Jorrit N. Herder)
10410 * Sep 30, 2004 source code documentation updated (Jorrit N. Herder)
10411 * Sep 24, 2004 redesigned alarm timers (Jorrit N. Herder)
10412 *
10413 * The function do_clocktick() is triggered by the clock’s interrupt
10414 * handler when a watchdog timer has expired or a process must be scheduled.
10415 *
10416 * In addition to the main clock_task() entry point, which starts the main
10417 * loop, there are several other minor entry points:
10418 * clock_stop: called just before MINIX shutdown
10419 * get_uptime: get realtime since boot in clock ticks
10420 * set_timer: set a watchdog timer (+)
10421 * reset_timer: reset a watchdog timer (+)
10422 * read_clock: read the counter of channel 0 of the 8253A timer
10423 *
10424 * (+) The CLOCK task keeps tracks of watchdog timers for the entire kernel.
10425 * The watchdog functions of expired timers are executed in do_clocktick().
10426 * It is crucial that watchdog functions not block, or the CLOCK task may
10427 * be blocked. Do not send() a message when the receiver is not expecting it.
10428 * Instead, notify(), which always returns, should be used.
10429 */
10430
10431 #include "kernel.h"
10432 #include "proc.h"
10433 #include <signal.h>
10434 #include <minix/com.h>
10435
10436 /* Function prototype for PRIVATE functions. */
10437 FORWARD _PROTOTYPE( void init_clock, (void) );
10438 FORWARD _PROTOTYPE( int clock_handler, (irq_hook_t *hook) );
10439 FORWARD _PROTOTYPE( int do_clocktick, (message *m_ptr) );
10440
10441 /* Clock parameters. */
10442 #define COUNTER_FREQ (2*TIMER_FREQ) /* counter frequency using square wave */
10443 #define LATCH_COUNT 0x00 /* cc00xxxx, c = channel, x = any */
10444 #define SQUARE_WAVE 0x36 /* ccaammmb, a = access, m = mode, b = BCD */
10445 /* 11x11, 11 = LSB then MSB, x11 = sq wave */
10446 #define TIMER_COUNT ((unsigned) (TIMER_FREQ/HZ)) /* initial value for counter*/
10447 #define TIMER_FREQ 1193182L /* clock frequency for timer in PC and AT */
10448
10449 #define CLOCK_ACK_BIT 0x80 /* PS/2 clock interrupt acknowledge bit */
10450
10451 /* The CLOCK’s timers queue. The functions in <timers.h> operate on this.
10452 * Each system process possesses a single synchronous alarm timer. If other
10453 * kernel parts want to use additional timers, they must declare their own
10454 * persistent (static) timer structure, which can be passed to the clock
766 File: kernel/clock.c MINIX SOURCE CODE
10455 * via (re)set_timer().
10456 * When a timer expires its watchdog function is run by the CLOCK task.
10457 */
10458 PRIVATE timer_t *clock_timers; /* queue of CLOCK timers */
10459 PRIVATE clock_t next_timeout; /* realtime that next timer expires */
10460
10461 /* The time is incremented by the interrupt handler on each clock tick. */
10462 PRIVATE clock_t realtime; /* real time clock */
10463 PRIVATE irq_hook_t clock_hook; /* interrupt handler hook */
10464
10465 /*===========================================================================*
10466 * clock_task *
10467 *===========================================================================*/
10468 PUBLIC void clock_task()
10469 {
10470 /* Main program of clock task. If the call is not HARD_INT it is an error.
10471 */
10472 message m; /* message buffer for both input and output */
10473 int result; /* result returned by the handler */
10474
10475 init_clock(); /* initialize clock task */
10476
10477 /* Main loop of the clock task. Get work, process it. Never reply. */
10478 while (TRUE) {
10479
10480 /* Go get a message. */
10481 receive(ANY, &m);
10482
10483 /* Handle the request. Only clock ticks are expected. */
10484 switch (m.m_type) {
10485 case HARD_INT:
10486 result = do_clocktick(&m); /* handle clock tick */
10487 break;
10488 default: /* illegal request type */
10489 kprintf("CLOCK: illegal request %d from %d.\n", m.m_type,m.m_source);
10490 }
10491 }
10492 }
10494 /*===========================================================================*
10495 * do_clocktick *
10496 *===========================================================================*/
10497 PRIVATE int do_clocktick(m_ptr)
10498 message *m_ptr; /* pointer to request message */
10499 {
10500 /* Despite its name, this routine is not called on every clock tick. It
10501 * is called on those clock ticks when a lot of work needs to be done.
10502 */
10503
10504 /* A process used up a full quantum. The interrupt handler stored this
10505 * process in ’prev_ptr’. First make sure that the process is not on the
10506 * scheduling queues. Then announce the process ready again. Since it has
10507 * no more time left, it gets a new quantum and is inserted at the right
10508 * place in the queues. As a side-effect a new process will be scheduled.
10509 */
10510 if (prev_ptr->p_ticks_left <= 0 && priv(prev_ptr)->s_flags & PREEMPTIBLE) {
10511 lock_dequeue(prev_ptr); /* take it off the queues */
10512 lock_enqueue(prev_ptr); /* and reinsert it again */
10513 }
10514
MINIX SOURCE CODE File: kernel/clock.c 767
10515 /* Check if a clock timer expired and run its watchdog function. */
10516 if (next_timeout <= realtime) {
10517 tmrs_exptimers(&clock_timers, realtime, NULL);
10518 next_timeout = clock_timers == NULL ?
10519 TMR_NEVER : clock_timers->tmr_exp_time;
10520 }
10521
10522 /* Inhibit sending a reply. */
10523 return(EDONTREPLY);
10524 }
10526 /*===========================================================================*
10527 * init_clock *
10528 *===========================================================================*/
10529 PRIVATE void init_clock()
10530 {
10531 /* Initialize the CLOCK’s interrupt hook. */
10532 clock_hook.proc_nr = CLOCK;
10533
10534 /* Initialize channel 0 of the 8253A timer to, e.g., 60 Hz. */
10535 outb(TIMER_MODE, SQUARE_WAVE); /* set timer to run continuously */
10536 outb(TIMER0, TIMER_COUNT); /* load timer low byte */
10537 outb(TIMER0, TIMER_COUNT >> 8); /* load timer high byte */
10538 put_irq_handler(&clock_hook, CLOCK_IRQ, clock_handler);/* register handler */
10539 enable_irq(&clock_hook); /* ready for clock interrupts */
10540 }
10542 /*===========================================================================*
10543 * clock_stop *
10544 *===========================================================================*/
10545 PUBLIC void clock_stop()
10546 {
10547 /* Reset the clock to the BIOS rate. (For rebooting) */
10548 outb(TIMER_MODE, 0x36);
10549 outb(TIMER0, 0);
10550 outb(TIMER0, 0);
10551 }
10553 /*===========================================================================*
10554 * clock_handler *
10555 *===========================================================================*/
10556 PRIVATE int clock_handler(hook)
10557 irq_hook_t *hook;
10558 {
10559 /* This executes on each clock tick (i.e., every time the timer chip generates
10560 * an interrupt). It does a little bit of work so the clock task does not have
10561 * to be called on every tick. The clock task is called when:
10562 *
10563 * (1) the scheduling quantum of the running process has expired, or
10564 * (2) a timer has expired and the watchdog function should be run.
10565 *
10566 * Many global global and static variables are accessed here. The safety of
10567 * this must be justified. All scheduling and message passing code acquires a
10568 * lock by temporarily disabling interrupts, so no conflicts with calls from
10569 * the task level can occur. Furthermore, interrupts are not reentrant, the
10570 * interrupt handler cannot be bothered by other interrupts.
10571 *
10572 * Variables that are updated in the clock’s interrupt handler:
10573 * lost_ticks:
10574 * Clock ticks counted outside the clock task. This for example
768 File: kernel/clock.c MINIX SOURCE CODE
10575 * is used when the boot monitor processes a real mode interrupt.
10576 * realtime:
10577 * The current uptime is incremented with all outstanding ticks.
10578 * proc_ptr, bill_ptr:
10579 * These are used for accounting. It does not matter if proc.c
10580 * is changing them, provided they are always valid pointers,
10581 * since at worst the previous process would be billed.
10582 */
10583 register unsigned ticks;
10584
10585 /* Acknowledge the PS/2 clock interrupt. */
10586 if (machine.ps_mca) outb(PORT_B, inb(PORT_B) | CLOCK_ACK_BIT);
10587
10588 /* Get number of ticks and update realtime. */
10589 ticks = lost_ticks + 1;
10590 lost_ticks = 0;
10591 realtime += ticks;
10592
10593 /* Update user and system accounting times. Charge the current process for
10594 * user time. If the current process is not billable, that is, if a non-user
10595 * process is running, charge the billable process for system time as well.
10596 * Thus the unbillable process’ user time is the billable user’s system time.
10597 */
10598 proc_ptr->p_user_time += ticks;
10599 if (priv(proc_ptr)->s_flags & PREEMPTIBLE) {
10600 proc_ptr->p_ticks_left -= ticks;
10601 }
10602 if (! (priv(proc_ptr)->s_flags & BILLABLE)) {
10603 bill_ptr->p_sys_time += ticks;
10604 bill_ptr->p_ticks_left -= ticks;
10605 }
10606
10607 /* Check if do_clocktick() must be called. Done for alarms and scheduling.
10608 * Some processes, such as the kernel tasks, cannot be preempted.
10609 */
10610 if ((next_timeout <= realtime) || (proc_ptr->p_ticks_left <= 0)) {
10611 prev_ptr = proc_ptr; /* store running process */
10612 lock_notify(HARDWARE, CLOCK); /* send notification */
10613 }
10614 return(1); /* reenable interrupts */
10615 }
10617 /*===========================================================================*
10618 * get_uptime *
10619 *===========================================================================*/
10620 PUBLIC clock_t get_uptime()
10621 {
10622 /* Get and return the current clock uptime in ticks. */
10623 return(realtime);
10624 }
10626 /*===========================================================================*
10627 * set_timer *
10628 *===========================================================================*/
10629 PUBLIC void set_timer(tp, exp_time, watchdog)
10630 struct timer *tp; /* pointer to timer structure */
10631 clock_t exp_time; /* expiration realtime */
10632 tmr_func_t watchdog; /* watchdog to be called */
10633 {
10634 /* Insert the new timer in the active timers list. Always update the
MINIX SOURCE CODE File: kernel/clock.c 769
10635 * next timeout time by setting it to the front of the active list.
10636 */
10637 tmrs_settimer(&clock_timers, tp, exp_time, watchdog, NULL);
10638 next_timeout = clock_timers->tmr_exp_time;
10639 }
10641 /*===========================================================================*
10642 * reset_timer *
10643 *===========================================================================*/
10644 PUBLIC void reset_timer(tp)
10645 struct timer *tp; /* pointer to timer structure */
10646 {
10647 /* The timer pointed to by ’tp’ is no longer needed. Remove it from both the
10648 * active and expired lists. Always update the next timeout time by setting
10649 * it to the front of the active list.
10650 */
10651 tmrs_clrtimer(&clock_timers, tp, NULL);
10652 next_timeout = (clock_timers == NULL) ?
10653 TMR_NEVER : clock_timers->tmr_exp_time;
10654 }
10656 /*===========================================================================*
10657 * read_clock *
10658 *===========================================================================*/
10659 PUBLIC unsigned long read_clock()
10660 {
10661 /* Read the counter of channel 0 of the 8253A timer. This counter counts
10662 * down at a rate of TIMER_FREQ and restarts at TIMER_COUNT-1 when it
10663 * reaches zero. A hardware interrupt (clock tick) occurs when the counter
10664 * gets to zero and restarts its cycle.
10665 */
10666 unsigned count;
10667
10668 outb(TIMER_MODE, LATCH_COUNT);
10669 count = inb(TIMER0);
10670 count |= (inb(TIMER0) << 8);
10671
10672 return count;
10673 }