++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             kernel/kernel.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef KERNEL_H
#define KERNEL_H

/* This is the master header for the kernel. It includes some other files
* and defines the principal constants.
*/
#define _POSIX_SOURCE 1 /* tell headers to include POSIX stuff */
#define _MINIX 1 /* tell headers to include MINIX stuff */
#define _SYSTEM 1 /* tell headers that this is the kernel */

/* The following are so basic, all the *.c files get them automatically. */
#include <minix/config.h> /* global configuration, MUST be first */
#include <ansi.h> /* C style: ANSI or K&R, MUST be second */
#include <sys/types.h> /* general system types */
#include <minix/const.h> /* MINIX specific constants */
#include <minix/type.h> /* MINIX specific types, e.g. message */
#include <minix/ipc.h> /* MINIX run-time system */
#include <timers.h> /* watchdog timer management */
#include <errno.h> /* return codes and error numbers */
#include <ibm/portio.h> /* device I/O and toggle interrupts */

/* Important kernel header files. */
#include "config.h" /* configuration, MUST be first */
#include "const.h" /* constants, MUST be second */
#include "type.h" /* type definitions, MUST be third */
#include "proto.h" /* function prototypes */
#include "glo.h" /* global variables */
#include "ipc.h" /* IPC constants */
/* #include "debug.h" */ /* debugging, MUST be last kernel header */

#endif /* KERNEL_H */

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/config.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef CONFIG_H
#define CONFIG_H

/* This file defines the kernel configuration. It allows to set sizes of some
* kernel buffers and to enable or disable debugging code, timing features,
* and individual kernel calls.
*
* Changes:
* Jul 11, 2005 Created. (Jorrit N. Herder)
*/
690 File: kernel/config.h MINIX SOURCE CODE

/* In embedded and sensor applications, not all the kernel calls may be
* needed. In this section you can specify which kernel calls are needed
* and which are not. The code for unneeded kernel calls is not included in
* the system binary, making it smaller. If you are not sure, it is best
* to keep all kernel calls enabled.
*/
#define USE_FORK 1 /* fork a new process */
#define USE_NEWMAP 1 /* set a new memory map */
#define USE_EXEC 1 /* update process after execute */
#define USE_EXIT 1 /* clean up after process exit */
#define USE_TRACE 1 /* process information and tracing */
#define USE_GETKSIG 1 /* retrieve pending kernel signals */
#define USE_ENDKSIG 1 /* finish pending kernel signals */
#define USE_KILL 1 /* send a signal to a process */
#define USE_SIGSEND 1 /* send POSIX-style signal */
#define USE_SIGRETURN 1 /* sys_sigreturn(proc_nr, ctxt_ptr, flags) */
#define USE_ABORT 1 /* shut down MINIX */
#define USE_GETINFO 1 /* retrieve a copy of kernel data */
#define USE_TIMES 1 /* get process and system time info */
#define USE_SETALARM 1 /* schedule a synchronous alarm */
#define USE_DEVIO 1 /* read or write a single I/O port */
#define USE_VDEVIO 1 /* process vector with I/O requests */
#define USE_SDEVIO 1 /* perform I/O request on a buffer */
#define USE_IRQCTL 1 /* set an interrupt policy */
#define USE_SEGCTL 1 /* set up a remote segment */
#define USE_PRIVCTL 1 /* system privileges control */
#define USE_NICE 1 /* change scheduling priority */
#define USE_UMAP 1 /* map virtual to physical address */
#define USE_VIRCOPY 1 /* copy using virtual addressing */
#define USE_VIRVCOPY 1 /* vector with virtual copy requests */
#define USE_PHYSCOPY 1 /* copy using physical addressing */
#define USE_PHYSVCOPY 1 /* vector with physical copy requests */
#define USE_MEMSET 1 /* write char to a given memory area */

/* Length of program names stored in the process table. This is only used
* for the debugging dumps that can be generated with the IS server. The PM
* server keeps its own copy of the program name.
*/
#define P_NAME_LEN 8

/* Kernel diagnostics are written to a circular buffer. After each message,
* a system server is notified and a copy of the buffer can be retrieved to
* display the message. The buffers size can safely be reduced.
*/
#define KMESS_BUF_SIZE 256

/* Buffer to gather randomness. This is used to generate a random stream by
* the MEMORY driver when reading from /dev/random.
*/
#define RANDOM_ELEMENTS 32

/* This section contains defines for valuable system resources that are used
* by device drivers. The number of elements of the vectors is determined by
* the maximum needed by any given driver. The number of interrupt hooks may
* be incremented on systems with many device drivers.
*/
#define NR_IRQ_HOOKS 16 /* number of interrupt hooks */
#define VDEVIO_BUF_SIZE 64 /* max elements per VDEVIO request */
#define VCOPY_VEC_SIZE 16 /* max elements per VCOPY request */
MINIX SOURCE CODE File: kernel/config.h 691

/* How many bytes for the kernel stack. Space allocated in mpx.s. */
#define K_STACK_BYTES 1024

/* This section allows to enable kernel debugging and timing functionality.
* For normal operation all options should be disabled.
*/
#define DEBUG_SCHED_CHECK 0 /* sanity check of scheduling queues */
#define DEBUG_LOCK_CHECK 0 /* kernel lock() sanity check */
#define DEBUG_TIME_LOCKS 0 /* measure time spent in locks */

#endif /* CONFIG_H */

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* General macros and constants used by the kernel. */
#ifndef CONST_H
#define CONST_H

#include <ibm/interrupt.h> /* interrupt numbers and hardware vectors */
#include <ibm/ports.h> /* port addresses and magic numbers */
#include <ibm/bios.h> /* BIOS addresses, sizes and magic numbers */
#include <ibm/cpu.h> /* BIOS addresses, sizes and magic numbers */
#include <minix/config.h>
#include "config.h"

/* To translate an address in kernel space to a physical address. This is
* the same as umap_local(proc_ptr, D, vir, sizeof(*vir)), but less costly.
*/
#define vir2phys(vir) (kinfo.data_base + (vir_bytes) (vir))

/* Map a process number to a privilege structure id. */
#define s_nr_to_id(n) (NR_TASKS + (n) + 1)

/* Translate a pointer to a field in a structure to a pointer to the structure
* itself. So it translates ’&struct_ptr->field’ back to ’struct_ptr’.
*/
#define structof(type, field, ptr) \
((type *) (((char *) (ptr)) - offsetof(type, field)))

/* Constants used in virtual_copy(). Values must be 0 and 1, respectively. */
#define _SRC_ 0
#define _DST_ 1

/* Number of random sources */
#define RANDOM_SOURCES 16

/* Constants and macros for bit map manipulation. */
#define BITCHUNK_BITS (sizeof(bitchunk_t) * CHAR_BIT)
#define BITMAP_CHUNKS(nr_bits) (((nr_bits)+BITCHUNK_BITS-1)/BITCHUNK_BITS)
#define MAP_CHUNK(map,bit) (map)[((bit)/BITCHUNK_BITS)]
#define CHUNK_OFFSET(bit) ((bit)%BITCHUNK_BITS))
#define GET_BIT(map,bit) ( MAP_CHUNK(map,bit) & (1 << CHUNK_OFFSET(bit) )
#define SET_BIT(map,bit) ( MAP_CHUNK(map,bit) |= (1 << CHUNK_OFFSET(bit) )
#define UNSET_BIT(map,bit) ( MAP_CHUNK(map,bit) &= ˜(1 << CHUNK_OFFSET(bit) )
692 File: kernel/const.h MINIX SOURCE CODE

#define get_sys_bit(map,bit) \
( MAP_CHUNK(map.chunk,bit) & (1 << CHUNK_OFFSET(bit) )
#define set_sys_bit(map,bit) \
( MAP_CHUNK(map.chunk,bit) |= (1 << CHUNK_OFFSET(bit) )
#define unset_sys_bit(map,bit) \
( MAP_CHUNK(map.chunk,bit) &= ˜(1 << CHUNK_OFFSET(bit) )
#define NR_SYS_CHUNKS BITMAP_CHUNKS(NR_SYS_PROCS)

/* Program stack words and masks. */
#define INIT_PSW 0x0200 /* initial psw */
#define INIT_TASK_PSW 0x1200 /* initial psw for tasks (with IOPL 1) */
#define TRACEBIT 0x0100 /* OR this with psw in proc[] for tracing */
#define SETPSW(rp, new) /* permits only certain bits to be set */ \
((rp)->p_reg.psw = (rp)->p_reg.psw & ˜0xCD5 | (new) & 0xCD5)
#define IF_MASK 0x200
#define IOPL_MASK 0x0

/* Disable/ enable hardware interrupts. The parameters of lock() and unlock()
* are used when debugging is enabled. See debug.h for more information.
*/
#define lock(c, v) intr_disable();
#define unlock(c) intr_enable();

/* Sizes of memory tables. The boot monitor distinguishes three memory areas,
* namely low mem below 1M, 1M-16M, and mem after 16M. More chunks are needed
* for DOS MINIX.
*/
#define NR_MEMS 8

#endif /* CONST_H */





++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/type.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef TYPE_H
#define TYPE_H

typedef _PROTOTYPE( void task_t, (void) );

/* Process table and system property related types. */
typedef int proc_nr_t; /* process table entry number */
typedef short sys_id_t; /* system process index */
typedef struct { /* bitmap for system indexes */
bitchunk_t chunk[BITMAP_CHUNKS(NR_SYS_PROCS)];
} sys_map_t;

struct boot_image {
proc_nr_t proc_nr; /* process number to use */
task_t *initial_pc; /* start function for tasks */
MINIX SOURCE CODE File: kernel/type.h 693
int flags; /* process flags */
unsigned char quantum; /* quantum (tick count) */
int priority; /* scheduling priority */
int stksize; /* stack size for tasks */
short trap_mask; /* allowed system call traps */
bitchunk_t ipc_to; /* send mask protection */
long call_mask; /* system call protection */
char proc_name[P_NAME_LEN]; /* name in process table */
};

struct memory {
phys_clicks base; /* start address of chunk */
phys_clicks size; /* size of memory chunk */
};

/* The kernel outputs diagnostic messages in a circular buffer. */
struct kmessages {
int km_next; /* next index to write */
int km_size; /* current size in buffer */
char km_buf[KMESS_BUF_SIZE]; /* buffer for messages */
};

struct randomness {
struct {
int r_next; /* next index to write */
int r_size; /* number of random elements */
unsigned short r_buf[RANDOM_ELEMENTS]; /* buffer for random info */
} bin[RANDOM_SOURCES];
};

#if (CHIP == INTEL)
typedef unsigned reg_t; /* machine register */

/* The stack frame layout is determined by the software, but for efficiency
* it is laid out so the assembly code to use it is as simple as possible.
* protected mode and all real modes use the same frame, built with
* 16-bit registers. Real mode lacks an automatic stack switch, so little
* is lost by using the 286 frame for it. The 386 frame differs only in
* having 32-bit registers and more segment registers. The same names are
* used for the larger registers to avoid differences in the code.
*/
struct stackframe_s { /* proc_ptr points here */
#if _WORD_SIZE == 4
u16_t gs; /* last item pushed by save */
u16_t fs; /* ˆ */
#endif
u16_t es; /* | */
u16_t ds; /* | */
reg_t di; /* di through cx are not accessed in C */
reg_t si; /* order is to match pusha/popa */
reg_t fp; /* bp */
reg_t st; /* hole for another copy of sp */
reg_t bx; /* | */
reg_t dx; /* | */
reg_t cx; /* | */
reg_t retreg; /* ax and above are all pushed by save */
reg_t retadr; /* return address for assembly code save() */
reg_t pc; /* ˆ last item pushed by interrupt */
reg_t cs; /* | */
reg_t psw; /* | */
694 File: kernel/type.h MINIX SOURCE CODE
reg_t sp; /* | */
reg_t ss; /* these are pushed by CPU during interrupt */
};

struct segdesc_s { /* segment descriptor for protected mode */
u16_t limit_low;
u16_t base_low;
u8_t base_middle;
u8_t access; /* |P|DL|1|X|E|R|A| */
u8_t granularity; /* |G|X|0|A|LIMT| */
u8_t base_high;
};

typedef unsigned long irq_policy_t;
typedef unsigned long irq_id_t;

typedef struct irq_hook {
struct irq_hook *next; /* next hook in chain */
int (*handler)(struct irq_hook *); /* interrupt handler */
int irq; /* IRQ vector number */
int id; /* id of this hook */
int proc_nr; /* NONE if not in use */
irq_id_t notify_id; /* id to return on interrupt */
irq_policy_t policy; /* bit mask for policy */
} irq_hook_t;

typedef int (*irq_handler_t)(struct irq_hook *);

#endif /* (CHIP == INTEL) */

#if (CHIP == M)
/* Mspecific types go here. */
#endif /* (CHIP == M) */

#endif /* TYPE_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/proto.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* Function prototypes. */

#ifndef PROTO_H
#define PROTO_H

/* Struct declarations. */
struct proc;
struct timer;

/* clock.c */
_PROTOTYPE( void clock_task, (void) );
_PROTOTYPE( void clock_stop, (void) );
_PROTOTYPE( clock_t get_uptime, (void) );
_PROTOTYPE( unsigned long read_clock, (void) );
_PROTOTYPE( void set_timer, (struct timer *tp, clock_t t, tmr_func_t f) );
_PROTOTYPE( void reset_timer, (struct timer *tp) );

/* main.c */
_PROTOTYPE( void main, (void) );
_PROTOTYPE( void prepare_shutdown, (int how) );
MINIX SOURCE CODE File: kernel/proto.h 695

/* utility.c */
_PROTOTYPE( void kprintf, (const char *fmt, ...) );
_PROTOTYPE( void panic, (_CONST char *s, int n) );

/* proc.c */
_PROTOTYPE( int sys_call, (int function, int src_dest, message *m_ptr) );
_PROTOTYPE( int lock_notify, (int src, int dst) );
_PROTOTYPE( int lock_send, (int dst, message *m_ptr) );
_PROTOTYPE( void lock_enqueue, (struct proc *rp) );
_PROTOTYPE( void lock_dequeue, (struct proc *rp) );

/* start.c */
_PROTOTYPE( void cstart, (U16_t cs, U16_t ds, U16_t mds,
U16_t parmoff, U16_t parmsize) );

/* system.c */
_PROTOTYPE( int get_priv, (register struct proc *rc, int proc_type) );
_PROTOTYPE( void send_sig, (int proc_nr, int sig_nr) );
_PROTOTYPE( void cause_sig, (int proc_nr, int sig_nr) );
_PROTOTYPE( void sys_task, (void) );
_PROTOTYPE( void get_randomness, (int source) );
_PROTOTYPE( int virtual_copy, (struct vir_addr *src, struct vir_addr *dst,
vir_bytes bytes) );
#define numap_local(proc_nr, vir_addr, bytes) \
umap_local(proc_addr(proc_nr), D, (vir_addr), (bytes))
_PROTOTYPE( phys_bytes umap_local, (struct proc *rp, int seg,
vir_bytes vir_addr, vir_bytes bytes) );
_PROTOTYPE( phys_bytes umap_remote, (struct proc *rp, int seg,
vir_bytes vir_addr, vir_bytes bytes) );
_PROTOTYPE( phys_bytes umap_bios, (struct proc *rp, vir_bytes vir_addr,
vir_bytes bytes) );

/* exception.c */
_PROTOTYPE( void exception, (unsigned vec_nr) );

/* i8259.c */
_PROTOTYPE( void intr_init, (int mine) );
_PROTOTYPE( void intr_handle, (irq_hook_t *hook) );
_PROTOTYPE( void put_irq_handler, (irq_hook_t *hook, int irq,
irq_handler_t handler) );
_PROTOTYPE( void rm_irq_handler, (irq_hook_t *hook) );

/* klib*.s */
_PROTOTYPE( void int86, (void) );
_PROTOTYPE( void cp_mess, (int src,phys_clicks src_clicks,vir_bytes src_offset,
phys_clicks dst_clicks, vir_bytes dst_offset) );
_PROTOTYPE( void enable_irq, (irq_hook_t *hook) );
_PROTOTYPE( int disable_irq, (irq_hook_t *hook) );
_PROTOTYPE( u16_t mem_rdw, (U16_t segm, vir_bytes offset) );
_PROTOTYPE( void phys_copy, (phys_bytes source, phys_bytes dest,
phys_bytes count) );
_PROTOTYPE( void phys_memset, (phys_bytes source, unsigned long pattern,
phys_bytes count) );
_PROTOTYPE( void phys_insb, (U16_t port, phys_bytes buf, size_t count) );
_PROTOTYPE( void phys_insw, (U16_t port, phys_bytes buf, size_t count) );
_PROTOTYPE( void phys_outsb, (U16_t port, phys_bytes buf, size_t count) );
_PROTOTYPE( void phys_outsw, (U16_t port, phys_bytes buf, size_t count) );
_PROTOTYPE( void reset, (void) );
_PROTOTYPE( void level0, (void (*func)(void)) );
696 File: kernel/proto.h MINIX SOURCE CODE
_PROTOTYPE( void monitor, (void) );
_PROTOTYPE( void read_tsc, (unsigned long *high, unsigned long *low) );
_PROTOTYPE( unsigned long read_cpu_flags, (void) );

/* mpx*.s */
_PROTOTYPE( void idle_task, (void) );
_PROTOTYPE( void restart, (void) );

/* The following are never called from C (pure asm procs). */

/* Exception handlers (real or protected mode), in numerical order. */
void _PROTOTYPE( int00, (void) ), _PROTOTYPE( divide_error, (void) );
void _PROTOTYPE( int01, (void) ), _PROTOTYPE( single_step_exception, (void) );
void _PROTOTYPE( int02, (void) ), _PROTOTYPE( nmi, (void) );
void _PROTOTYPE( int03, (void) ), _PROTOTYPE( breakpoint_exception, (void) );
void _PROTOTYPE( int04, (void) ), _PROTOTYPE( overflow, (void) );
void _PROTOTYPE( int05, (void) ), _PROTOTYPE( bounds_check, (void) );
void _PROTOTYPE( int06, (void) ), _PROTOTYPE( inval_opcode, (void) );
void _PROTOTYPE( int07, (void) ), _PROTOTYPE( copr_not_available, (void) );
void _PROTOTYPE( double_fault, (void) );
void _PROTOTYPE( copr_seg_overrun, (void) );
void _PROTOTYPE( inval_tss, (void) );
void _PROTOTYPE( segment_not_present, (void) );
void _PROTOTYPE( stack_exception, (void) );
void _PROTOTYPE( general_protection, (void) );
void _PROTOTYPE( page_fault, (void) );
void _PROTOTYPE( copr_error, (void) );

/* Hardware interrupt handlers. */
_PROTOTYPE( void hwint00, (void) );
_PROTOTYPE( void hwint01, (void) );
_PROTOTYPE( void hwint02, (void) );
_PROTOTYPE( void hwint03, (void) );
_PROTOTYPE( void hwint04, (void) );
_PROTOTYPE( void hwint05, (void) );
_PROTOTYPE( void hwint06, (void) );
_PROTOTYPE( void hwint07, (void) );
_PROTOTYPE( void hwint08, (void) );
_PROTOTYPE( void hwint09, (void) );
_PROTOTYPE( void hwint10, (void) );
_PROTOTYPE( void hwint11, (void) );
_PROTOTYPE( void hwint12, (void) );
_PROTOTYPE( void hwint13, (void) );
_PROTOTYPE( void hwint14, (void) );
_PROTOTYPE( void hwint15, (void) );

/* Software interrupt handlers, in numerical order. */
_PROTOTYPE( void trp, (void) );
_PROTOTYPE( void s_call, (void) ), _PROTOTYPE( p_s_call, (void) );
_PROTOTYPE( void level0_call, (void) );

/* protect.c */
_PROTOTYPE( void prot_init, (void) );
_PROTOTYPE( void init_codeseg, (struct segdesc_s *segdp, phys_bytes base,
vir_bytes size, int privilege) );
_PROTOTYPE( void init_dataseg, (struct segdesc_s *segdp, phys_bytes base,
vir_bytes size, int privilege) );
_PROTOTYPE( phys_bytes seg2phys, (U16_t seg) );
_PROTOTYPE( void phys2seg, (u16_t *seg, vir_bytes *off, phys_bytes phys));
_PROTOTYPE( void enable_iop, (struct proc *pp) );
MINIX SOURCE CODE File: kernel/proto.h 697
_PROTOTYPE( void alloc_segments, (struct proc *rp) );

#endif /* PROTO_H */


++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/glo.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef GLO_H
#define GLO_H

/* Global variables used in the kernel. This file contains the declarations;
* storage space for the variables is allocated in table.c, because EXTERN is
* defined as extern unless the _TABLE definition is seen. We rely on the
* compiler’s default initialization (0) for several global variables.
*/
#ifdef _TABLE
#undef EXTERN
#define EXTERN
#endif

#include <minix/config.h>
#include "config.h"

/* Variables relating to shutting down MINIX. */
EXTERN char kernel_exception; /* TRUE after system exceptions */
EXTERN char shutdown_started; /* TRUE after shutdowns / reboots */

/* Kernel information structures. This groups vital kernel information. */
EXTERN phys_bytes aout; /* address of a.out headers */
EXTERN struct kinfo kinfo; /* kernel information for users */
EXTERN struct machine machine; /* machine information for users */
EXTERN struct kmessages kmess; /* diagnostic messages in kernel */
EXTERN struct randomness krandom; /* gather kernel random information */

/* Process scheduling information and the kernel reentry count. */
EXTERN struct proc *prev_ptr; /* previously running process */
EXTERN struct proc *proc_ptr; /* pointer to currently running process */
EXTERN struct proc *next_ptr; /* next process to run after restart() */
EXTERN struct proc *bill_ptr; /* process to bill for clock ticks */
EXTERN char k_reenter; /* kernel reentry count (entry count less 1) */
EXTERN unsigned lost_ticks; /* clock ticks counted outside clock task */

/* Interrupt related variables. */
EXTERN irq_hook_t irq_hooks[NR_IRQ_HOOKS]; /* hooks for general use */
EXTERN irq_hook_t *irq_handlers[NR_IRQ_VECTORS];/* list of IRQ handlers */
EXTERN int irq_actids[NR_IRQ_VECTORS]; /* IRQ ID bits active */
EXTERN int irq_use; /* map of all in-use irq’s */

/* Miscellaneous. */
EXTERN reg_t mon_ss, mon_sp; /* boot monitor stack */
EXTERN int mon_return; /* true if we can return to monitor */

/* Variables that are initialized elsewhere are just extern here. */
extern struct boot_image image[]; /* system image processes */
extern char *t_stack[]; /* task stack space */
extern struct segdesc_s gdt[]; /* global descriptor table */

698 File: kernel/glo.h MINIX SOURCE CODE
EXTERN _PROTOTYPE( void (*level0_func), (void) );

#endif /* GLO_H */





++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/ipc.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef IPC_H
#define IPC_H

/* This header file defines constants for MINIX inter-process communication.
* These definitions are used in the file proc.c.
*/
#include <minix/com.h>

/* Masks and flags for system calls. */
#define SYSCALL_FUNC 0x0F /* mask for system call function */
#define SYSCALL_FLAGS 0xF0 /* mask for system call flags */
#define NON_BLOCKING 0x10 /* prevent blocking, return error */

/* System call numbers that are passed when trapping to the kernel. The
* numbers are carefully defined so that it can easily be seen (based on
* the bits that are on) which checks should be done in sys_call().
*/
#define SEND 1 /* 0 0 0 1 : blocking send */
#define RECEIVE 2 /* 0 0 1 0 : blocking receive */
#define SENDREC 3 /* 0 0 1 1 : SEND + RECEIVE */
#define NOTIFY 4 /* 0 1 0 0 : nonblocking notify */
#define ECHO 8 /* 1 0 0 0 : echo a message */

/* The following bit masks determine what checks that should be done. */
#define CHECK_PTR 0x0B /* 1 0 1 1 : validate message buffer */
#define CHECK_DST 0x05 /* 0 1 0 1 : validate message destination */
#define CHECK_SRC 0x02 /* 0 0 1 0 : validate message source */

#endif /* IPC_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/proc.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef PROC_H
#define PROC_H

/* Here is the declaration of the process table. It contains all process
* data, including registers, flags, scheduling priority, memory map,
* accounting, message passing (IPC) information, and so on.
*
* Many assembly code routines reference fields in it. The offsets to these
* fields are defined in the assembler include file sconst.h. When changing
* struct proc, be sure to change sconst.h to match.
MINIX SOURCE CODE File: kernel/proc.h 699
*/
#include <minix/com.h>
#include "protect.h"
#include "const.h"
#include "priv.h"

struct proc {
struct stackframe_s p_reg; /* process’ registers saved in stack frame */
reg_t p_ldt_sel; /* selector in gdt with ldt base and limit */
struct segdesc_s p_ldt[2+NR_REMOTE_SEGS]; /* CS, DS and remote segments */

proc_nr_t p_nr; /* number of this process (for fast access) */
struct priv *p_priv; /* system privileges structure */
char p_rts_flags; /* SENDING, RECEIVING, etc. */

char p_priority; /* current scheduling priority */
char p_max_priority; /* maximum scheduling priority */
char p_ticks_left; /* number of scheduling ticks left */
char p_quantum_size; /* quantum size in ticks */

struct mem_map p_memmap[NR_LOCAL_SEGS]; /* memory map (T, D, S) */

clock_t p_user_time; /* user time in ticks */
clock_t p_sys_time; /* sys time in ticks */

struct proc *p_nextready; /* pointer to next ready process */
struct proc *p_caller_q; /* head of list of procs wishing to send */
struct proc *p_q_link; /* link to next proc wishing to send */
message *p_messbuf; /* pointer to passed message buffer */
proc_nr_t p_getfrom; /* from whom does process want to receive? */
proc_nr_t p_sendto; /* to whom does process want to send? */

sigset_t p_pending; /* bit map for pending kernel signals */

char p_name[P_NAME_LEN]; /* name of the process, including \0 */
};

/* Bits for the runtime flags. A process is runnable iff p_rts_flags == 0. */
#define SLOT_FREE 0x01 /* process slot is free */
#define NO_MAP 0x02 /* keeps unmapped forked child from running */
#define SENDING 0x04 /* process blocked trying to SEND */
#define RECEIVING 0x08 /* process blocked trying to RECEIVE */
#define SIGNALED 0x10 /* set when new kernel signal arrives */
#define SIG_PENDING 0x20 /* unready while signal being processed */
#define P_STOP 0x40 /* set when process is being traced */
#define NO_PRIV 0x80 /* keep forked system process from running */

/* Scheduling priorities for p_priority. Values must start at zero (highest
* priority) and increment. Priorities of the processes in the boot image
* can be set in table.c. IDLE must have a queue for itself, to prevent low
* priority user processes to run round-robin with IDLE.
*/
#define NR_SCHED_QUEUES 16 /* MUST equal minimum priority + 1 */
#define TASK_Q 0 /* highest, used for kernel tasks */
#define MAX_USER_Q 0 /* highest priority for user processes */
#define USER_Q 7 /* default (should correspond to nice 0) */
#define MIN_USER_Q 14 /* minimum priority for user processes */
#define IDLE_Q 15 /* lowest, only IDLE process goes here */

/* Magic process table addresses. */
700 File: kernel/proc.h MINIX SOURCE CODE
#define BEG_PROC_ADDR (&proc[0])
#define BEG_USER_ADDR (&proc[NR_TASKS])
#define END_PROC_ADDR (&proc[NR_TASKS + NR_PROCS])

#define NIL_PROC ((struct proc *) 0)
#define NIL_SYS_PROC ((struct proc *) 1)
#define cproc_addr(n) (&(proc + NR_TASKS)[(n)])
#define proc_addr(n) (pproc_addr + NR_TASKS)[(n)]
#define proc_nr(p) ((p)->p_nr)

#define isokprocn(n) ((unsigned) ((n) + NR_TASKS) < NR_PROCS + NR_TASKS)
#define isemptyn(n) isemptyp(proc_addr(n))
#define isemptyp(p) ((p)->p_rts_flags == SLOT_FREE)
#define iskernelp(p) iskerneln((p)->p_nr)
#define iskerneln(n) ((n) < 0)
#define isuserp(p) isusern((p)->p_nr)
#define isusern(n) ((n) >= 0)

/* The process table and pointers to process table slots. The pointers allow
* faster access because now a process entry can be found by indexing the
* pproc_addr array, while accessing an element i requires a multiplication
* with sizeof(struct proc) to determine the address.
*/
EXTERN struct proc proc[NR_TASKS + NR_PROCS]; /* process table */
EXTERN struct proc *pproc_addr[NR_TASKS + NR_PROCS];
EXTERN struct proc *rdy_head[NR_SCHED_QUEUES]; /* ptrs to ready list headers */
EXTERN struct proc *rdy_tail[NR_SCHED_QUEUES]; /* ptrs to ready list tails */

#endif /* PROC_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/sconst.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! Miscellaneous constants used in assembler code.
W = _WORD_SIZE ! Machine word size.

! Offsets in struct proc. They MUST match proc.h.
P_STACKBASE = 0
GSREG = P_STACKBASE
FSREG = GSREG + 2 ! 386 introduces FS and GS segments
ESREG = FSREG + 2
DSREG = ESREG + 2
DIREG = DSREG + 2
SIREG = DIREG + W
BPREG = SIREG + W
STREG = BPREG + W ! hole for another SP
BXREG = STREG + W
DXREG = BXREG + W
CXREG = DXREG + W
AXREG = CXREG + W
RETADR = AXREG + W ! return address for save() call
PCREG = RETADR + W
CSREG = PCREG + W
PSWREG = CSREG + W
SPREG = PSWREG + W
SSREG = SPREG + W
P_STACKTOP = SSREG + W
P_LDT_SEL = P_STACKTOP
MINIX SOURCE CODE File: kernel/sconst.h 701
P_LDT = P_LDT_SEL + W

Msize = 9 ! size of a message in 32-bit words
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/priv.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef PRIV_H
#define PRIV_H

/* Declaration of the system privileges structure. It defines flags, system
* call masks, an synchronous alarm timer, I/O privileges, pending hardware
* interrupts and notifications, and so on.
* System processes each get their own structure with properties, whereas all
* user processes share one structure. This setup provides a clear separation
* between common and privileged process fields and is very space efficient.
*
* Changes:
* Jul 01, 2005 Created. (Jorrit N. Herder)
*/
#include <minix/com.h>
#include "protect.h"
#include "const.h"
#include "type.h"

struct priv {
proc_nr_t s_proc_nr; /* number of associated process */
sys_id_t s_id; /* index of this system structure */
short s_flags; /* PREEMTIBLE, BILLABLE, etc. */

short s_trap_mask; /* allowed system call traps */
sys_map_t s_ipc_from; /* allowed callers to receive from */
sys_map_t s_ipc_to; /* allowed destination processes */
long s_call_mask; /* allowed kernel calls */

sys_map_t s_notify_pending; /* bit map with pending notifications */
irq_id_t s_int_pending; /* pending hardware interrupts */
sigset_t s_sig_pending; /* pending signals */

timer_t s_alarm_timer; /* synchronous alarm timer */
struct far_mem s_farmem[NR_REMOTE_SEGS]; /* remote memory map */
reg_t *s_stack_guard; /* stack guard word for kernel tasks */
};

/* Guard word for task stacks. */
#define STACK_GUARD ((reg_t) (sizeof(reg_t) == 2 ? 0xBEEF : 0xDEADBEEF))

/* Bits for the system property flags. */
#define PREEMPTIBLE 0x01 /* kernel tasks are not preemptible */
#define BILLABLE 0x04 /* some processes are not billable */
#define SYS_PROC 0x10 /* system processes are privileged */
#define SENDREC_BUSY 0x20 /* sendrec() in progress */

/* Magic system structure table addresses. */
#define BEG_PRIV_ADDR (&priv[0])
#define END_PRIV_ADDR (&priv[NR_SYS_PROCS])

702 File: kernel/priv.h MINIX SOURCE CODE
#define priv_addr(i) (ppriv_addr)[(i)]
#define priv_id(rp) ((rp)->p_priv->s_id)
#define priv(rp) ((rp)->p_priv)

#define id_to_nr(id) priv_addr(id)->s_proc_nr
#define nr_to_id(nr) priv(proc_addr(nr))->s_id

/* The system structures table and pointers to individual table slots. The
* pointers allow faster access because now a process entry can be found by
* indexing the psys_addr array, while accessing an element i requires a
* multiplication with sizeof(struct sys) to determine the address.
*/
EXTERN struct priv priv[NR_SYS_PROCS]; /* system properties table */
EXTERN struct priv *ppriv_addr[NR_SYS_PROCS]; /* direct slot pointers */

/* Unprivileged user processes all share the same privilege structure.
* This id must be fixed because it is used to check send mask entries.
*/
#define USER_PRIV_ID 0

/* Make sure the system can boot. The following sanity check verifies that
* the system privileges table is large enough for the number of processes
* in the boot image.
*/
#if (NR_BOOT_PROCS > NR_SYS_PROCS)
#error NR_SYS_PROCS must be larger than NR_BOOT_PROCS
#endif

#endif /* PRIV_H */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/protect.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* Constants for protected mode. */

/* Table sizes. */
#define GDT_SIZE (FIRST_LDT_INDEX + NR_TASKS + NR_PROCS)
/* spec. and LDT’s */
#define IDT_SIZE (IRQ8_VECTOR + 8) /* only up to the highest vector */
#define LDT_SIZE (2 + NR_REMOTE_SEGS) /* CS, DS and remote segments */

/* Fixed global descriptors. 1 to 7 are prescribed by the BIOS. */
#define GDT_INDEX 1 /* GDT descriptor */
#define IDT_INDEX 2 /* IDT descriptor */
#define DS_INDEX 3 /* kernel DS */
#define ES_INDEX 4 /* kernel ES (386: flag 4 Gb at startup) */
#define SS_INDEX 5 /* kernel SS (386: monitor SS at startup) */
#define CS_INDEX 6 /* kernel CS */
#define MON_CS_INDEX 7 /* temp for BIOS (386: monitor CS at startup) */
#define TSS_INDEX 8 /* kernel TSS */
#define DS_286_INDEX 9 /* scratch 16-bit source segment */
#define ES_286_INDEX 10 /* scratch 16-bit destination segment */
#define A_INDEX 11 /* 64K memory segment at A0000 */
#define B_INDEX 12 /* 64K memory segment at B0000 */
#define C_INDEX 13 /* 64K memory segment at C0000 */
#define D_INDEX 14 /* 64K memory segment at D0000 */
#define FIRST_LDT_INDEX 15 /* rest of descriptors are LDT’s */

MINIX SOURCE CODE File: kernel/protect.h 703
#define GDT_SELECTOR 0x08 /* (GDT_INDEX * DESC_SIZE) bad for asld */
#define IDT_SELECTOR 0x10 /* (IDT_INDEX * DESC_SIZE) */
#define DS_SELECTOR 0x18 /* (DS_INDEX * DESC_SIZE) */
#define ES_SELECTOR 0x20 /* (ES_INDEX * DESC_SIZE) */
#define FLAT_DS_SELECTOR 0x21 /* less privileged ES */
#define SS_SELECTOR 0x28 /* (SS_INDEX * DESC_SIZE) */
#define CS_SELECTOR 0x30 /* (CS_INDEX * DESC_SIZE) */
#define MON_CS_SELECTOR 0x38 /* (MON_CS_INDEX * DESC_SIZE) */
#define TSS_SELECTOR 0x40 /* (TSS_INDEX * DESC_SIZE) */
#define DS_286_SELECTOR 0x49 /* (DS_286_INDEX*DESC_SIZE+TASK_PRIVILEGE) */
#define ES_286_SELECTOR 0x51 /* (ES_286_INDEX*DESC_SIZE+TASK_PRIVILEGE) */

/* Fixed local descriptors. */
#define CS_LDT_INDEX 0 /* process CS */
#define DS_LDT_INDEX 1 /* process DS=ES=FS=GS=SS */
#define EXTRA_LDT_INDEX 2 /* first of the extra LDT entries */

/* Privileges. */
#define INTR_PRIVILEGE 0 /* kernel and interrupt handlers */
#define TASK_PRIVILEGE 1 /* kernel tasks */
#define USER_PRIVILEGE 3 /* servers and user processes */

/* 286 hardware constants. */

/* Exception vector numbers. */
#define BOUNDS_VECTOR 5 /* bounds check failed */
#define INVAL_OP_VECTOR 6 /* invalid opcode */
#define COPROC_NOT_VECTOR 7 /* coprocessor not available */
#define DOUBLE_FAULT_VECTOR 8
#define COPROC_SEG_VECTOR 9 /* coprocessor segment overrun */
#define INVAL_TSS_VECTOR 10 /* invalid TSS */
#define SEG_NOT_VECTOR 11 /* segment not present */
#define STACK_FAULT_VECTOR 12 /* stack exception */
#define PROTECTION_VECTOR 13 /* general protection */

/* Selector bits. */
#define TI 0x04 /* table indicator */
#define RPL 0x03 /* requester privilege level */

/* Descriptor structure offsets. */
#define DESC_BASE 2 /* to base_low */
#define DESC_BASE_MIDDLE 4 /* to base_middle */
#define DESC_ACCESS 5 /* to access byte */
#define DESC_SIZE 8 /* sizeof (struct segdesc_s) */

/* Base and limit sizes and shifts. */
#define BASE_MIDDLE_SHIFT 16 /* shift for base --> base_middle */

/* Access-byte and type-byte bits. */
#define PRESENT 0x80 /* set for descriptor present */
#define DPL 0x60 /* descriptor privilege level mask */
#define DPL_SHIFT 5
#define SEGMENT 0x10 /* set for segment-type descriptors */

/* Access-byte bits. */
#define EXECUTABLE 0x08 /* set for executable segment */
#define CONFORMING 0x04 /* set for conforming segment if executable */
#define EXPAND_DOWN 0x04 /* set for expand-down segment if !executable*/
#define READABLE 0x02 /* set for readable segment if executable */
#define WRITEABLE 0x02 /* set for writeable segment if !executable */
704 File: kernel/protect.h MINIX SOURCE CODE
#define TSS_BUSY 0x02 /* set if TSS descriptor is busy */
#define ACCESSED 0x01 /* set if segment accessed */

/* Special descriptor types. */
#define AVL_286_TSS 1 /* available 286 TSS */
#define LDT 2 /* local descriptor table */
#define BUSY_286_TSS 3 /* set transparently to the software */
#define CALL_286_GATE 4 /* not used */
#define TASK_GATE 5 /* only used by debugger */
#define INT_286_GATE 6 /* interrupt gate, used for all vectors */
#define TRAP_286_GATE 7 /* not used */

/* Extra 386 hardware constants. */

/* Exception vector numbers. */
#define PAGE_FAULT_VECTOR 14
#define COPROC_ERR_VECTOR 16 /* coprocessor error */

/* Descriptor structure offsets. */
#define DESC_GRANULARITY 6 /* to granularity byte */
#define DESC_BASE_HIGH 7 /* to base_high */

/* Base and limit sizes and shifts. */
#define BASE_HIGH_SHIFT 24 /* shift for base --> base_high */
#define BYTE_GRAN_MAX 0xFFFFFL /* maximum size for byte granular segment */
#define GRANULARITY_SHIFT 16 /* shift for limit --> granularity */
#define OFFSET_HIGH_SHIFT 16 /* shift for (gate) offset --> offset_high */
#define PAGE_GRAN_SHIFT 12 /* extra shift for page granular limits */

/* Type-byte bits. */
#define DESC_386_BIT 0x08 /* 386 types are obtained by ORing with this */
/* LDT’s and TASK_GATE’s don’t need it */

/* Granularity byte. */
#define GRANULAR 0x80 /* set for 4K granularilty */
#define DEFAULT 0x40 /* set for 32-bit defaults (executable seg) */
#define BIG 0x40 /* set for "BIG" (expand-down seg) */
#define AVL 0x10 /* 0 for available */
#define LIMIT_HIGH 0x0F /* mask for high bits of limit */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/table.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The object file of "table.c" contains most kernel data. Variables that
* are declared in the *.h files appear with EXTERN in front of them, as in
*
* EXTERN int x;
*
* Normally EXTERN is defined as extern, so when they are included in another
* file, no storage is allocated. If EXTERN were not present, but just say,
*
* int x;
*
* then including this file in several source files would cause ’x’ to be
* declared several times. While some linkers accept this, others do not,
* so they are declared extern when included normally. However, it must be
* declared for real somewhere. That is done here, by redefining EXTERN as
* the null string, so that inclusion of all *.h files in table.c actually
MINIX SOURCE CODE File: kernel/table.c 705
* generates storage for them.
*
* Various variables could not be declared EXTERN, but are declared PUBLIC
* or PRIVATE. The reason for this is that extern variables cannot have a
* default initialization. If such variables are shared, they must also be
* declared in one of the *.h files without the initialization. Examples
* include ’boot_image’ (this file) and ’idt’ and ’gdt’ (protect.c).
*
* Changes:
* Aug 02, 2005 set privileges and minimal boot image (Jorrit N. Herder)
* Oct 17, 2004 updated above and tasktab comments (Jorrit N. Herder)
* May 01, 2004 changed struct for system image (Jorrit N. Herder)
*/
#define _TABLE

#include "kernel.h"
#include "proc.h"
#include "ipc.h"
#include <minix/com.h>
#include <ibm/int86.h>

/* Define stack sizes for the kernel tasks included in the system image. */
#define NO_STACK 0
#define SMALL_STACK (128 * sizeof(char *))
#define IDL_S SMALL_STACK /* 3 intr, 3 temps, 4 db for Intel */
#define HRD_S NO_STACK /* dummy task, uses kernel stack */
#define TSK_S SMALL_STACK /* system and clock task */

/* Stack space for all the task stacks. Declared as (char *) to align it. */
#define TOT_STACK_SPACE (IDL_S + HRD_S + (2 * TSK_S))
PUBLIC char *t_stack[TOT_STACK_SPACE / sizeof(char *)];

/* Define flags for the various process types. */
#define IDL_F (SYS_PROC | PREEMPTIBLE | BILLABLE) /* idle task */
#define TSK_F (SYS_PROC) /* kernel tasks */
#define SRV_F (SYS_PROC | PREEMPTIBLE) /* system services */
#define USR_F (BILLABLE | PREEMPTIBLE) /* user processes */

/* Define system call traps for the various process types. These call masks
* determine what system call traps a process is allowed to make.
*/
#define TSK_T (1 << RECEIVE) /* clock and system */
#define SRV_T (˜0) /* system services */
#define USR_T ((1 << SENDREC) | (1 << ECHO)) /* user processes */

/* Send masks determine to whom processes can send messages or notifications.
* The values here are used for the processes in the boot image. We rely on
* the initialization code in main() to match the s_nr_to_id() mapping for the
* processes in the boot image, so that the send mask that is defined here
* can be directly copied onto map[0] of the actual send mask. Privilege
* structure 0 is shared by user processes.
*/
#define s(n) (1 << s_nr_to_id(n))
#define SRV_M (˜0)
#define SYS_M (˜0)
#define USR_M (s(PM_PROC_NR) | s(FS_PROC_NR) | s(RS_PROC_NR))
#define DRV_M (USR_M | s(SYSTEM) | s(CLOCK) | s(LOG_PROC_NR) | s(TTY_PROC_NR))

/* Define kernel calls that processes are allowed to make. This is not looking
* very nice, but we need to define the access rights on a per call basis.
706 File: kernel/table.c MINIX SOURCE CODE
* Note that the reincarnation server has all bits on, because it should
* be allowed to distribute rights to services that it starts.
*/
#define c(n) (1 << ((n)-KERNEL_CALL))
#define RS_C ˜0
#define PM_C ˜(c(SYS_DEVIO) | c(SYS_SDEVIO) | c(SYS_VDEVIO) \
| c(SYS_IRQCTL) | c(SYS_INT86))
#define FS_C (c(SYS_KILL) | c(SYS_VIRCOPY) | c(SYS_VIRVCOPY) | c(SYS_UMAP) \
| c(SYS_GETINFO) | c(SYS_EXIT) | c(SYS_TIMES) | c(SYS_SETALARM))
#define DRV_C (FS_C | c(SYS_SEGCTL) | c(SYS_IRQCTL) | c(SYS_INT86) \
| c(SYS_DEVIO) | c(SYS_VDEVIO) | c(SYS_SDEVIO))
#define MEM_C (DRV_C | c(SYS_PHYSCOPY) | c(SYS_PHYSVCOPY))

/* The system image table lists all programs that are part of the boot image.
* The order of the entries here MUST agree with the order of the programs
* in the boot image and all kernel tasks must come first.
* Each entry provides the process number, flags, quantum size (qs), scheduling
* queue, allowed traps, ipc mask, and a name for the process table. The
* initial program counter and stack size is also provided for kernel tasks.
*/
PUBLIC struct boot_image image[] = {
/* process nr, pc, flags, qs, queue, stack, traps, ipcto, call, name */
{ IDLE, idle_task, IDL_F, 8, IDLE_Q, IDL_S, 0, 0, 0, "IDLE" },
{ CLOCK,clock_task, TSK_F, 64, TASK_Q, TSK_S, TSK_T, 0, 0, "CLOCK" },
{ SYSTEM, sys_task, TSK_F, 64, TASK_Q, TSK_S, TSK_T, 0, 0, "SYSTEM"},
{ HARDWARE, 0, TSK_F, 64, TASK_Q, HRD_S, 0, 0, 0, "KERNEL"},
{ PM_PROC_NR, 0, SRV_F, 32, 3, 0, SRV_T, SRV_M, PM_C, "pm" },
{ FS_PROC_NR, 0, SRV_F, 32, 4, 0, SRV_T, SRV_M, FS_C, "fs" },
{ RS_PROC_NR, 0, SRV_F, 4, 3, 0, SRV_T, SYS_M, RS_C, "rs" },
{ TTY_PROC_NR, 0, SRV_F, 4, 1, 0, SRV_T, SYS_M, DRV_C, "tty" },
{ MEM_PROC_NR, 0, SRV_F, 4, 2, 0, SRV_T, DRV_M, MEM_C, "memory"},
{ LOG_PROC_NR, 0, SRV_F, 4, 2, 0, SRV_T, SYS_M, DRV_C, "log" },
{ DRVR_PROC_NR, 0, SRV_F, 4, 2, 0, SRV_T, SYS_M, DRV_C, "driver"},
{ INIT_PROC_NR, 0, USR_F, 8, USER_Q, 0, USR_T, USR_M, 0, "init" },
};

/* Verify the size of the system image table at compile time. Also verify that
* the first chunk of the ipc mask has enough bits to accommodate the processes
* in the image.
* If a problem is detected, the size of the ’dummy’ array will be negative,
* causing a compile time error. Note that no space is actually allocated
* because ’dummy’ is declared extern.
*/
extern int dummy[(NR_BOOT_PROCS==sizeof(image)/
sizeof(struct boot_image))?1:-1];
extern int dummy[(BITCHUNK_BITS > NR_BOOT_PROCS - 1) ? 1 : -1];

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/mpx.s
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
! Chooses between the 8086 and 386 versions of the Minix startup code.

#include <minix/config.h>
#if _WORD_SIZE == 2
MINIX SOURCE CODE File: kernel/mpx.s 707
#include "mpx88.s"
#else
#include "mpx386.s"
#endif
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/mpx386.s
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
! This file, mpx386.s, is included by mpx.s when Minix is compiled for
! 32-bit Intel CPUs. The alternative mpx88.s is compiled for 16-bit CPUs.

! This file is part of the lowest layer of the MINIX kernel. (The other part
! is "proc.c".) The lowest layer does process switching and message handling.
! Furthermore it contains the assembler startup code for Minix and the 32-bit
! interrupt handlers. It cooperates with the code in "start.c" to set up a
! good environment for main().

! Every transition to the kernel goes through this file. Transitions to the
! kernel may be nested. The initial entry may be with a system call (i.e.,
! send or receive a message), an exception or a hardware interrupt; kernel
! reentries may only be made by hardware interrupts. The count of reentries
! is kept in "k_reenter". It is important for deciding whether to switch to
! the kernel stack and for protecting the message passing code in "proc.c".

! For the message passing trap, most of the machine state is saved in the
! proc table. (Some of the registers need not be saved.) Then the stack is
! switched to "k_stack", and interrupts are reenabled. Finally, the system
! call handler (in C) is called. When it returns, interrupts are disabled
! again and the code falls into the restart routine, to finish off held-up
! interrupts and run the process or task whose pointer is in "proc_ptr".

! Hardware interrupt handlers do the same, except (1) The entire state must
! be saved. (2) There are too many handlers to do this inline, so the save
! routine is called. A few cycles are saved by pushing the address of the
! appropiate restart routine for a return later. (3) A stack switch is
! avoided when the stack is already switched. (4) The (master) 8259 interrupt
! controller is reenabled centrally in save(). (5) Each interrupt handler
! masks its interrupt line using the 8259 before enabling (other unmasked)
! interrupts, and unmasks it after servicing the interrupt. This limits the
! nest level to the number of lines and protects the handler from itself.

! For communication with the boot monitor at startup time some constant
! data are compiled into the beginning of the text segment. This facilitates
! reading the data at the start of the boot process, since only the first
! sector of the file needs to be read.

! Some data storage is also allocated at the end of this file. This data
! will be at the start of the data segment of the kernel and will be read
! and modified by the boot monitor before the kernel starts.

! sections

.sect .text
begtext:
.sect .rom
begrom:
.sect .data
708 File: kernel/mpx386.s MINIX SOURCE CODE
begdata:
.sect .bss
begbss:

#include <minix/config.h>
#include <minix/const.h>
#include <minix/com.h>
#include <ibm/interrupt.h>
#include "const.h"
#include "protect.h"
#include "sconst.h"

/* Selected 386 tss offsets. */
#define TSS3_S_SP0 4

! Exported functions
! Note: in assembly language the .define statement applied to a function name
! is loosely equivalent to a prototype in C code -- it makes it possible to
! link to an entity declared in the assembly code but does not create
! the entity.

.define _restart
.define save

.define _divide_error
.define _single_step_exception
.define _nmi
.define _breakpoint_exception
.define _overflow
.define _bounds_check
.define _inval_opcode
.define _copr_not_available
.define _double_fault
.define _copr_seg_overrun
.define _inval_tss
.define _segment_not_present
.define _stack_exception
.define _general_protection
.define _page_fault
.define _copr_error

.define _hwint00 ! handlers for hardware interrupts
.define _hwint01
.define _hwint02
.define _hwint03
.define _hwint04
.define _hwint05
.define _hwint06
.define _hwint07
.define _hwint08
.define _hwint09
.define _hwint10
.define _hwint11
.define _hwint12
.define _hwint13
.define _hwint14
.define _hwint15

.define _s_call
.define _p_s_call
MINIX SOURCE CODE File: kernel/mpx386.s 709
.define _level0_call

! Exported variables.
.define begbss
.define begdata

.sect .text
!*===========================================================================*
!* MINIX *
!*===========================================================================*
MINIX: ! this is the entry point for the MINIX kernel
jmp over_flags ! skip over the next few bytes
.data2 CLICK_SHIFT ! for the monitor: memory granularity
flags:
.data2 0x01FD ! boot monitor flags:
! call in 386 mode, make bss, make stack,
! load high, don’t patch, will return,
! uses generic INT, memory vector,
! new boot code return
nop ! extra byte to sync up disassembler
over_flags:

! Set up a C stack frame on the monitor stack. (The monitor sets cs and ds
! right. The ss descriptor still references the monitor data segment.)
movzx esp, sp ! monitor stack is a 16 bit stack
push ebp
mov ebp, esp
push esi
push edi
cmp 4(ebp), 0 ! monitor return vector is
jz noret ! nonzero if return possible
inc (_mon_return)
noret: mov (_mon_sp), esp ! save stack pointer for later return

! Copy the monitor global descriptor table to the address space of kernel and
! switch over to it. Prot_init() can then update it with immediate effect.

sgdt (_gdt+GDT_SELECTOR) ! get the monitor gdtr
mov esi, (_gdt+GDT_SELECTOR+2) ! absolute address of GDT
mov ebx, _gdt ! address of kernel GDT
mov ecx, 8*8 ! copying eight descriptors
copygdt:
eseg movb al, (esi)
movb (ebx), al
inc esi
inc ebx
loop copygdt
mov eax, (_gdt+DS_SELECTOR+2) ! base of kernel data
and eax, 0x00FFFFFF ! only 24 bits
add eax, _gdt ! eax = vir2phys(gdt)
mov (_gdt+GDT_SELECTOR+2), eax ! set base of GDT
lgdt (_gdt+GDT_SELECTOR) ! switch over to kernel GDT

! Locate boot parameters, set up kernel segment registers and stack.
mov ebx, 8(ebp) ! boot parameters offset
mov edx, 12(ebp) ! boot parameters length
mov eax, 16(ebp) ! address of a.out headers
mov (_aout), eax
mov ax, ds ! kernel data
mov es, ax
710 File: kernel/mpx386.s MINIX SOURCE CODE
mov fs, ax
mov gs, ax
mov ss, ax
mov esp, k_stktop ! set sp to point to the top of kernel stack

! Call C startup code to set up a proper environment to run main().
push edx
push ebx
push SS_SELECTOR
push DS_SELECTOR
push CS_SELECTOR
call _cstart ! cstart(cs, ds, mds, parmoff, parmlen)
add esp, 5*4

! Reload gdtr, idtr and the segment registers to global descriptor table set
! up by prot_init().

lgdt (_gdt+GDT_SELECTOR)
lidt (_gdt+IDT_SELECTOR)

jmpf CS_SELECTOR:csinit
csinit:
o16 mov ax, DS_SELECTOR
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
o16 mov ax, TSS_SELECTOR ! no other TSS is used
ltr ax
push 0 ! set flags to known good state
popf ! esp, clear nested task and int enable

jmp _main ! main()


!*===========================================================================*
!* interrupt handlers *
!* interrupt handlers for 386 32-bit protected mode *
!*===========================================================================*

!*===========================================================================*
!* hwint00 - 07 *
!*===========================================================================*
! Note this is a macro, it just looks like a subroutine.
#define hwint_master(irq) \
call save /* save interrupted process state */;\
push (_irq_handlers+4*irq) /* irq_handlers[irq] */;\
call _intr_handle /* intr_handle(irq_handlers[irq]) */;\
pop ecx ;\
cmp (_irq_actids+4*irq), 0 /* interrupt still active? */;\
jz 0f ;\
inb INT_CTLMASK /* get current mask */ ;\
orb al, [1<<irq] /* mask irq */ ;\
outb INT_CTLMASK /* disable the irq */;\
0: movb al, END_OF_INT ;\
outb INT_CTL /* reenable master 8259 */;\
ret /* restart (another) process */

! Each of these entry points is an expansion of the hwint_master macro
MINIX SOURCE CODE File: kernel/mpx386.s 711
.align 16
_hwint00: ! Interrupt routine for irq 0 (the clock).
hwint_master(0)

.align 16
_hwint01: ! Interrupt routine for irq 1 (keyboard)
hwint_master(1)

.align 16
_hwint02: ! Interrupt routine for irq 2 (cascade!)
hwint_master(2)

.align 16
_hwint03: ! Interrupt routine for irq 3 (second serial)
hwint_master(3)

.align 16
_hwint04: ! Interrupt routine for irq 4 (first serial)
hwint_master(4)

.align 16
_hwint05: ! Interrupt routine for irq 5 (XT winchester)
hwint_master(5)

.align 16
_hwint06: ! Interrupt routine for irq 6 (floppy)
hwint_master(6)

.align 16
_hwint07: ! Interrupt routine for irq 7 (printer)
hwint_master(7)

!*===========================================================================*
!* hwint08 - 15 *
!*===========================================================================*
! Note this is a macro, it just looks like a subroutine.
#define hwint_slave(irq) \
call save /* save interrupted process state */;\
push (_irq_handlers+4*irq) /* irq_handlers[irq] */;\
call _intr_handle /* intr_handle(irq_handlers[irq]) */;\
pop ecx ;\
cmp (_irq_actids+4*irq), 0 /* interrupt still active? */;\
jz 0f ;\
inb INT2_CTLMASK ;\
orb al, [1<<[irq-8]] ;\
outb INT2_CTLMASK /* disable the irq */;\
0: movb al, END_OF_INT ;\
outb INT_CTL /* reenable master 8259 */;\
outb INT2_CTL /* reenable slave 8259 */;\
ret /* restart (another) process */

! Each of these entry points is an expansion of the hwint_slave macro
.align 16
_hwint08: ! Interrupt routine for irq 8 (realtime clock)
hwint_slave(8)

.align 16
_hwint09: ! Interrupt routine for irq 9 (irq 2 redirected)
hwint_slave(9)

712 File: kernel/mpx386.s MINIX SOURCE CODE
.align 16
_hwint10: ! Interrupt routine for irq 10
hwint_slave(10)

.align 16
_hwint11: ! Interrupt routine for irq 11
hwint_slave(11)

.align 16
_hwint12: ! Interrupt routine for irq 12
hwint_slave(12)

.align 16
_hwint13: ! Interrupt routine for irq 13 (FPU exception)
hwint_slave(13)

.align 16
_hwint14: ! Interrupt routine for irq 14 (AT winchester)
hwint_slave(14)

.align 16
_hwint15: ! Interrupt routine for irq 15
hwint_slave(15)

!*===========================================================================*
!* save *
!*===========================================================================*
! Save for protected mode.
! This is much simpler than for 8086 mode, because the stack already points
! into the process table, or has already been switched to the kernel stack.

.align 16
save:
cld ! set direction flag to a known value
pushad ! save "general" registers
o16 push ds ! save ds
o16 push es ! save es
o16 push fs ! save fs
o16 push gs ! save gs
mov dx, ss ! ss is kernel data segment
mov ds, dx ! load rest of kernel segments
mov es, dx ! kernel does not use fs, gs
mov eax, esp ! prepare to return
incb (_k_reenter) ! from -1 if not reentering
jnz set_restart1 ! stack is already kernel stack
mov esp, k_stktop
push _restart ! build return address for int handler
xor ebp, ebp ! for stacktrace
jmp RETADR-P_STACKBASE(eax)

.align 4
set_restart1:
push restart1
jmp RETADR-P_STACKBASE(eax)

!*===========================================================================*
!* _s_call *
!*===========================================================================*
.align 16
_s_call:
MINIX SOURCE CODE File: kernel/mpx386.s 713
_p_s_call:
cld ! set direction flag to a known value
sub esp, 6*4 ! skip RETADR, eax, ecx, edx, ebx, est
push ebp ! stack already points into proc table
push esi
push edi
o16 push ds
o16 push es
o16 push fs
o16 push gs
mov dx, ss
mov ds, dx
mov es, dx
incb (_k_reenter)
mov esi, esp ! assumes P_STACKBASE == 0
mov esp, k_stktop
xor ebp, ebp ! for stacktrace
! end of inline save
! now set up parameters for sys_call()
push ebx ! pointer to user message
push eax ! src/dest
push ecx ! SEND/RECEIVE/BOTH
call _sys_call ! sys_call(function, src_dest, m_ptr)
! caller is now explicitly in proc_ptr
mov AXREG(esi), eax ! sys_call MUST PRESERVE si

! Fall into code to restart proc/task running.

!*===========================================================================*
!* restart *
!*===========================================================================*
_restart:

! Restart the current process or the next process if it is set.

cmp (_next_ptr), 0 ! see if another process is scheduled
jz 0f
mov eax, (_next_ptr)
mov (_proc_ptr), eax ! schedule new process
mov (_next_ptr), 0
0: mov esp, (_proc_ptr) ! will assume P_STACKBASE == 0
lldt P_LDT_SEL(esp) ! enable process’ segment descriptors
lea eax, P_STACKTOP(esp) ! arrange for next interrupt
mov (_tss+TSS3_S_SP0), eax ! to save state in process table
restart1:
decb (_k_reenter)
o16 pop gs
o16 pop fs
o16 pop es
o16 pop ds
popad
add esp, 4 ! skip return adr
iretd ! continue process

!*===========================================================================*
!* exception handlers *
!*===========================================================================*
_divide_error:
push DIVIDE_VECTOR
jmp exception
714 File: kernel/mpx386.s MINIX SOURCE CODE

_single_step_exception:
push DEBUG_VECTOR
jmp exception

_nmi:
push NMI_VECTOR
jmp exception

_breakpoint_exception:
push BREAKPOINT_VECTOR
jmp exception

_overflow:
push OVERFLOW_VECTOR
jmp exception

_bounds_check:
push BOUNDS_VECTOR
jmp exception

_inval_opcode:
push INVAL_OP_VECTOR
jmp exception

_copr_not_available:
push COPROC_NOT_VECTOR
jmp exception

_double_fault:
push DOUBLE_FAULT_VECTOR
jmp errexception

_copr_seg_overrun:
push COPROC_SEG_VECTOR
jmp exception

_inval_tss:
push INVAL_TSS_VECTOR
jmp errexception

_segment_not_present:
push SEG_NOT_VECTOR
jmp errexception

_stack_exception:
push STACK_FAULT_VECTOR
jmp errexception

_general_protection:
push PROTECTION_VECTOR
jmp errexception

_page_fault:
push PAGE_FAULT_VECTOR
jmp errexception

_copr_error:
push COPROC_ERR_VECTOR
jmp exception
MINIX SOURCE CODE File: kernel/mpx386.s 715

!*===========================================================================*
!* exception *
!*===========================================================================*
! This is called for all exceptions which do not push an error code.

.align 16
exception:
sseg mov (trap_errno), 0 ! clear trap_errno
sseg pop (ex_number)
jmp exception1

!*===========================================================================*
!* errexception *
!*===========================================================================*
! This is called for all exceptions which push an error code.

.align 16
errexception:
sseg pop (ex_number)
sseg pop (trap_errno)
exception1: ! Common for all exceptions.
push eax ! eax is scratch register
mov eax, 0+4(esp) ! old eip
sseg mov (old_eip), eax
movzx eax, 4+4(esp) ! old cs
sseg mov (old_cs), eax
mov eax, 8+4(esp) ! old eflags
sseg mov (old_eflags), eax
pop eax
call save
push (old_eflags)
push (old_cs)
push (old_eip)
push (trap_errno)
push (ex_number)
call _exception ! (ex_number, trap_errno, old_eip,
! old_cs, old_eflags)
add esp, 5*4
ret

!*===========================================================================*
!* level0_call *
!*===========================================================================*
_level0_call:
call save
jmp (_level0_func)

!*===========================================================================*
!* data *
!*===========================================================================*

.sect .rom ! Before the string table please
.data2 0x526F ! this must be the first data entry (magic #)

.sect .bss
k_stack:
.space K_STACK_BYTES ! kernel stack
k_stktop: ! top of kernel stack
.comm ex_number, 4
716 File: kernel/mpx386.s MINIX SOURCE CODE
.comm trap_errno, 4
.comm old_eip, 4
.comm old_cs, 4
.comm old_eflags, 4
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/start.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This file contains the C startup code for Minix on Intel processors.
* It cooperates with mpx.s to set up a good environment for main().
*
* This code runs in real mode for a 16 bit kernel and may have to switch
* to protected mode for a 286.
* For a 32 bit kernel this already runs in protected mode, but the selectors
* are still those given by the BIOS with interrupts disabled, so the
* descriptors need to be reloaded and interrupt descriptors made.
*/

#include "kernel.h"
#include "protect.h"
#include "proc.h"
#include <stdlib.h>
#include <string.h>

FORWARD _PROTOTYPE( char *get_value, (_CONST char *params, _CONST char *key));
/*===========================================================================*
* cstart *
*===========================================================================*/
PUBLIC void cstart(cs, ds, mds, parmoff, parmsize)
U16_t cs, ds; /* kernel code and data segment */
U16_t mds; /* monitor data segment */
U16_t parmoff, parmsize; /* boot parameters offset and length */
{
/* Perform system initializations prior to calling main(). Most settings are
* determined with help of the environment strings passed by MINIX’ loader.
*/
char params[128*sizeof(char *)]; /* boot monitor parameters */
register char *value; /* value in key=value pair */
extern int etext, end;

/* Decide if mode is protected; 386 or higher implies protected mode.
* This must be done first, because it is needed for, e.g., seg2phys().
* For 286 machines we cannot decide on protected mode, yet. This is
* done below.
*/
#if _WORD_SIZE != 2
machine.protected = 1;
#endif

/* Record where the kernel and the monitor are. */
kinfo.code_base = seg2phys(cs);
kinfo.code_size = (phys_bytes) &etext; /* size of code segment */
kinfo.data_base = seg2phys(ds);
kinfo.data_size = (phys_bytes) &end; /* size of data segment */

/* Initialize protected mode descriptors. */
prot_init();

MINIX SOURCE CODE File: kernel/start.c 717
/* Copy the boot parameters to the local buffer. */
kinfo.params_base = seg2phys(mds) + parmoff;
kinfo.params_size = MIN(parmsize,sizeof(params)-2);
phys_copy(kinfo.params_base, vir2phys(params), kinfo.params_size);

/* Record miscellaneous information for user-space servers. */
kinfo.nr_procs = NR_PROCS;
kinfo.nr_tasks = NR_TASKS;
strncpy(kinfo.release, OS_RELEASE, sizeof(kinfo.release));
kinfo.release[sizeof(kinfo.release)-1] = ’\0’;
strncpy(kinfo.version, OS_VERSION, sizeof(kinfo.version));
kinfo.version[sizeof(kinfo.version)-1] = ’\0’;
kinfo.proc_addr = (vir_bytes) proc;
kinfo.kmem_base = vir2phys(0);
kinfo.kmem_size = (phys_bytes) &end;

/* Processor? 86, 186, 286, 386, ...
* Decide if mode is protected for older machines.
*/
machine.processor=atoi(get_value(params, "processor"));
#if _WORD_SIZE == 2
machine.protected = machine.processor >= 286;
#endif
if (! machine.protected) mon_return = 0;

/* XT, AT or MCA bus? */
value = get_value(params, "bus");
if (value == NIL_PTR || strcmp(value, "at") == 0) {
machine.pc_at = TRUE; /* PC-AT compatible hardware */
} else if (strcmp(value, "mca") == 0) {
machine.pc_at = machine.ps_mca = TRUE; /* PS/2 with micro channel */
}

/* Type of VDU: */
value = get_value(params, "video"); /* EGA or VGA video unit */
if (strcmp(value, "ega") == 0) machine.vdu_ega = TRUE;
if (strcmp(value, "vga") == 0) machine.vdu_vga = machine.vdu_ega = TRUE;

/* Return to assembler code to switch to protected mode (if 286),
* reload selectors and call main().
*/
}
/*===========================================================================*
* get_value *
*===========================================================================*/

PRIVATE char *get_value(params, name)
_CONST char *params; /* boot monitor parameters */
_CONST char *name; /* key to look up */
{
/* Get environment value - kernel version of getenv to avoid setting up the
* usual environment array.
*/
register _CONST char *namep;
register char *envp;

for (envp = (char *) params; *envp != 0;) {
for (namep = name; *namep != 0 && *namep == *envp; namep++, envp++)
;
718 File: kernel/start.c MINIX SOURCE CODE
if (*namep == ’\0’ && *envp == ’=’) return(envp + 1);
while (*envp++ != 0)
;
}
return(NIL_PTR);
}
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This file contains the main program of MINIX as well as its shutdown code.
* The routine main() initializes the system and starts the ball rolling by
* setting up the process table, interrupt vectors, and scheduling each task
* to run to initialize itself.
* The routine shutdown() does the opposite and brings down MINIX.
*
* The entries into this file are:
* main: MINIX main program
* prepare_shutdown: prepare to take MINIX down
*
* Changes:
* Nov 24, 2004 simplified main() with system image (Jorrit N. Herder)
* Aug 20, 2004 new prepare_shutdown() and shutdown() (Jorrit N. Herder)
*/
#include "kernel.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <a.out.h>
#include <minix/callnr.h>
#include <minix/com.h>
#include "proc.h"

/* Prototype declarations for PRIVATE functions. */
FORWARD _PROTOTYPE( void announce, (void));
FORWARD _PROTOTYPE( void shutdown, (timer_t *tp));

/*===========================================================================*
* main *
*===========================================================================*/
PUBLIC void main()
{
/* Start the ball rolling. */
struct boot_image *ip; /* boot image pointer */
register struct proc *rp; /* process pointer */
register struct priv *sp; /* privilege structure pointer */
register int i, s;
int hdrindex; /* index to array of a.out headers */
phys_clicks text_base;
vir_clicks text_clicks, data_clicks;
reg_t ktsb; /* kernel task stack base */
struct exec e_hdr; /* for a copy of an a.out header */

/* Initialize the interrupt controller. */
intr_init(1);
MINIX SOURCE CODE File: kernel/main.c 719

/* Clear the process table. Anounce each slot as empty and set up mappings
* for proc_addr() and proc_nr() macros. Do the same for the table with
* privilege structures for the system processes.
*/
for (rp = BEG_PROC_ADDR, i = -NR_TASKS; rp < END_PROC_ADDR; ++rp, ++i) {
rp->p_rts_flags = SLOT_FREE; /* initialize free slot */
rp->p_nr = i; /* proc number from ptr */
(pproc_addr + NR_TASKS)[i] = rp; /* proc ptr from number */
}
for (sp = BEG_PRIV_ADDR, i = 0; sp < END_PRIV_ADDR; ++sp, ++i) {
sp->s_proc_nr = NONE; /* initialize as free */
sp->s_id = i; /* priv structure index */
ppriv_addr[i] = sp; /* priv ptr from number */
}

/* Set up proc table entries for tasks and servers. The stacks of the
* kernel tasks are initialized to an array in data space. The stacks
* of the servers have been added to the data segment by the monitor, so
* the stack pointer is set to the end of the data segment. All the
* processes are in low memory on the 8086. On the 386 only the kernel
* is in low memory, the rest is loaded in extended memory.
*/

/* Task stacks. */
ktsb = (reg_t) t_stack;

for (i=0; i < NR_BOOT_PROCS; ++i) {
ip = &image[i]; /* process’ attributes */
rp = proc_addr(ip->proc_nr); /* get process pointer */
rp->p_max_priority = ip->priority; /* max scheduling priority */
rp->p_priority = ip->priority; /* current priority */
rp->p_quantum_size = ip->quantum; /* quantum size in ticks */
rp->p_ticks_left = ip->quantum; /* current credit */
strncpy(rp->p_name, ip->proc_name, P_NAME_LEN); /* set process name */
(void) get_priv(rp, (ip->flags & SYS_PROC)); /* assign structure */
priv(rp)->s_flags = ip->flags; /* process flags */
priv(rp)->s_trap_mask = ip->trap_mask; /* allowed traps */
priv(rp)->s_call_mask = ip->call_mask; /* kernel call mask */
priv(rp)->s_ipc_to.chunk[0] = ip->ipc_to; /* restrict targets */
if (iskerneln(proc_nr(rp))) { /* part of the kernel? */
if (ip->stksize > 0) { /* HARDWARE stack size is 0 */
rp->p_priv->s_stack_guard = (reg_t *) ktsb;
*rp->p_priv->s_stack_guard = STACK_GUARD;
}
ktsb += ip->stksize; /* point to high end of stack */
rp->p_reg.sp = ktsb; /* this task’s initial stack ptr */
text_base = kinfo.code_base >> CLICK_SHIFT;
/* processes that are in the kernel */
hdrindex = 0; /* all use the first a.out header */
} else {
hdrindex = 1 + i-NR_TASKS; /* servers, drivers, INIT */
}

/* The bootstrap loader created an array of the a.out headers at
* absolute address ’aout’. Get one element to e_hdr.
*/
phys_copy(aout + hdrindex * A_MINHDR, vir2phys(&e_hdr),
(phys_bytes) A_MINHDR);
/* Convert addresses to clicks and build process memory map */
720 File: kernel/main.c MINIX SOURCE CODE
text_base = e_hdr.a_syms >> CLICK_SHIFT;
text_clicks = (e_hdr.a_text + CLICK_SIZE-1) >> CLICK_SHIFT;
if (!(e_hdr.a_flags & A_SEP)) text_clicks = 0; /* common I&D */
data_clicks = (e_hdr.a_total + CLICK_SIZE-1) >> CLICK_SHIFT;
rp->p_memmap[T].mem_phys = text_base;
rp->p_memmap[T].mem_len = text_clicks;
rp->p_memmap[D].mem_phys = text_base + text_clicks;
rp->p_memmap[D].mem_len = data_clicks;
rp->p_memmap[S].mem_phys = text_base + text_clicks + data_clicks;
rp->p_memmap[S].mem_vir = data_clicks; /* empty - stack is in data */

/* Set initial register values. The processor status word for tasks
* is different from that of other processes because tasks can
* access I/O; this is not allowed to less-privileged processes
*/
rp->p_reg.pc = (reg_t) ip->initial_pc;
rp->p_reg.psw = (iskernelp(rp)) ? INIT_TASK_PSW : INIT_PSW;

/* Initialize the server stack pointer. Take it down one word
* to give crtso.s something to use as "argc".
*/
if (isusern(proc_nr(rp))) { /* user-space process? */
rp->p_reg.sp = (rp->p_memmap[S].mem_vir +
rp->p_memmap[S].mem_len) << CLICK_SHIFT;
rp->p_reg.sp -= sizeof(reg_t);
}

/* Set ready. The HARDWARE task is never ready. */
if (rp->p_nr != HARDWARE) {
rp->p_rts_flags = 0; /* runnable if no flags */
lock_enqueue(rp); /* add to scheduling queues */
} else {
rp->p_rts_flags = NO_MAP; /* prevent from running */
}

/* Code and data segments must be allocated in protected mode. */
alloc_segments(rp);
}

/* We’re definitely not shutting down. */
shutdown_started = 0;

/* MINIX is now ready. All boot image processes are on the ready queue.
* Return to the assembly code to start running the current process.
*/
bill_ptr = proc_addr(IDLE); /* it has to point somewhere */
announce(); /* print MINIX startup banner */
restart();
}
/*===========================================================================*
* announce *
*===========================================================================*/
PRIVATE void announce(void)
{
/* Display the MINIX startup banner. */
kprintf("MINIX %s.%s."
"Copyright 2006, Vrije Universiteit, Amsterdam, The Netherlands\n",
OS_RELEASE, OS_VERSION);

MINIX SOURCE CODE File: kernel/main.c 721
/* Real mode, or 16/32-bit protected mode? */
kprintf("Executing in %s mode.\n\n",
machine.protected ? "32-bit protected" : "real");
}
/*===========================================================================*
* prepare_shutdown *
*===========================================================================*/
PUBLIC void prepare_shutdown(how)
int how;
{
/* This function prepares to shutdown MINIX. */
static timer_t shutdown_timer;
register struct proc *rp;
message m;

/* Show debugging dumps on panics. Make sure that the TTY task is still
* available to handle them. This is done with help of a non-blocking send.
* We rely on TTY to call sys_abort() when it is done with the dumps.
*/
if (how == RBT_PANIC) {
m.m_type = PANIC_DUMPS;
if (nb_send(TTY_PROC_NR,&m)==OK) /* don’t block if TTY isn’t ready */
return; /* await sys_abort() from TTY */
}

/* Send a signal to all system processes that are still alive to inform
* them that the MINIX kernel is shutting down. A proper shutdown sequence
* should be implemented by a user-space server. This mechanism is useful
* as a backup in case of system panics, so that system processes can still
* run their shutdown code, e.g, to synchronize the FS or to let the TTY
* switch to the first console.
*/
kprintf("Sending SIGKSTOP to system processes ...\n");
for (rp=BEG_PROC_ADDR; rp<END_PROC_ADDR; rp++) {
if (!isemptyp(rp) && (priv(rp)->s_flags & SYS_PROC) && !iskernelp(rp))
send_sig(proc_nr(rp), SIGKSTOP);
}

/* We’re shutting down. Diagnostics may behave differently now. */
shutdown_started = 1;

/* Notify system processes of the upcoming shutdown and allow them to be
* scheduled by setting a watchog timer that calls shutdown(). The timer
* argument passes the shutdown status.
*/
kprintf("MINIX will now be shut down ...\n");
tmr_arg(&shutdown_timer)->ta_int = how;

/* Continue after 1 second, to give processes a chance to get
* scheduled to do shutdown work.
*/
set_timer(&shutdown_timer, get_uptime() + HZ, shutdown);
}
/*===========================================================================*
* shutdown *
*===========================================================================*/
PRIVATE void shutdown(tp)
timer_t *tp;
722 File: kernel/main.c MINIX SOURCE CODE
{
/* This function is called from prepare_shutdown or stop_sequence to bring
* down MINIX. How to shutdown is in the argument: RBT_HALT (return to the
* monitor), RBT_MONITOR (execute given code), RBT_RESET (hard reset).
*/
int how = tmr_arg(tp)->ta_int;
u16_t magic;

/* Now mask all interrupts, including the clock, and stop the clock. */
outb(INT_CTLMASK, ˜0);
clock_stop();

if (mon_return && how != RBT_RESET) {
/* Reinitialize the interrupt controllers to the BIOS defaults. */
intr_init(0);
outb(INT_CTLMASK, 0);
outb(INT2_CTLMASK, 0);

/* Return to the boot monitor. Set the program if not already done. */
if (how != RBT_MONITOR) phys_copy(vir2phys(""), kinfo.params_base, 1);
level0(monitor);
}

/* Reset the system by jumping to the reset address (real mode), or by
* forcing a processor shutdown (protected mode). First stop the BIOS
* memory test by setting a soft reset flag.
*/
magic = STOP_MEM_CHECK;
phys_copy(vir2phys(&magic), SOFT_RESET_FLAG_ADDR, SOFT_RESET_FLAG_SIZE);
level0(reset);
}
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/proc.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This file contains essentially all of the process and message handling.
* Together with "mpx.s" it forms the lowest layer of the MINIX kernel.
* There is one entry point from the outside:
*
* sys_call: a system call, i.e., the kernel is trapped with an INT
*
* As well as several entry points used from the interrupt and task level:
*
* lock_notify: notify a process of a system event
* lock_send: send a message to a process
* lock_enqueue: put a process on one of the scheduling queues
* lock_dequeue: remove a process from the scheduling queues
*
* Changes:
* Aug 19, 2005 rewrote scheduling code (Jorrit N. Herder)
* Jul 25, 2005 rewrote system call handling (Jorrit N. Herder)
* May 26, 2005 rewrote message passing functions (Jorrit N. Herder)
* May 24, 2005 new notification system call (Jorrit N. Herder)
* Oct 28, 2004 nonblocking send and receive calls (Jorrit N. Herder)
*
MINIX SOURCE CODE File: kernel/proc.c 723
* The code here is critical to make everything work and is important for the
* overall performance of the system. A large fraction of the code deals with
* list manipulation. To make this both easy to understand and fast to execute
* pointer pointers are used throughout the code. Pointer pointers prevent
* exceptions for the head or tail of a linked list.
*
* node_t *queue, *new_node; // assume these as global variables
* node_t **xpp = &queue; // get pointer pointer to head of queue
* while (*xpp != NULL) // find last pointer of the linked list
* xpp = &(*xpp)->next; // get pointer to next pointer
* *xpp = new_node; // now replace the end (the NULL pointer)
* new_node->next = NULL; // and mark the new end of the list
*
* For example, when adding a new node to the end of the list, one normally
* makes an exception for an empty list and looks up the end of the list for
* nonempty lists. As shown above, this is not required with pointer pointers.
*/

#include <minix/com.h>
#include <minix/callnr.h>
#include "kernel.h"
#include "proc.h"

/* Scheduling and message passing functions. The functions are available to
* other parts of the kernel through lock_...(). The lock temporarily disables
* interrupts to prevent race conditions.
*/
FORWARD _PROTOTYPE( int mini_send, (struct proc *caller_ptr, int dst,
message *m_ptr, unsigned flags) );
FORWARD _PROTOTYPE( int mini_receive, (struct proc *caller_ptr, int src,
message *m_ptr, unsigned flags) );
FORWARD _PROTOTYPE( int mini_notify, (struct proc *caller_ptr, int dst) );

FORWARD _PROTOTYPE( void enqueue, (struct proc *rp) );
FORWARD _PROTOTYPE( void dequeue, (struct proc *rp) );
FORWARD _PROTOTYPE( void sched, (struct proc *rp, int *queue, int *front) );
FORWARD _PROTOTYPE( void pick_proc, (void) );

#define BuildMess(m_ptr, src, dst_ptr) \
(m_ptr)->m_source = (src); \
(m_ptr)->m_type = NOTIFY_FROM(src); \
(m_ptr)->NOTIFY_TIMESTAMP = get_uptime(); \
switch (src) { \
case HARDWARE: \
(m_ptr)->NOTIFY_ARG = priv(dst_ptr)->s_int_pending; \
priv(dst_ptr)->s_int_pending = 0; \
break; \
case SYSTEM: \
(m_ptr)->NOTIFY_ARG = priv(dst_ptr)->s_sig_pending; \
priv(dst_ptr)->s_sig_pending = 0; \
break; \
}

#define CopyMess(s,sp,sm,dp,dm) \
cp_mess(s, (sp)->p_memmap[D].mem_phys, \
(vir_bytes)sm, (dp)->p_memmap[D].mem_phys, (vir_bytes)dm)

724 File: kernel/proc.c MINIX SOURCE CODE
/*===========================================================================*
* sys_call *
*===========================================================================*/
PUBLIC int sys_call(call_nr, src_dst, m_ptr)
int call_nr; /* system call number and flags */
int src_dst; /* src to receive from or dst to send to */
message *m_ptr; /* pointer to message in the caller’s space */
{
/* System calls are done by trapping to the kernel with an INT instruction.
* The trap is caught and sys_call() is called to send or receive a message
* (or both). The caller is always given by ’proc_ptr’.
*/
register struct proc *caller_ptr = proc_ptr; /* get pointer to caller */
int function = call_nr & SYSCALL_FUNC; /* get system call function */
unsigned flags = call_nr & SYSCALL_FLAGS; /* get flags */
int mask_entry; /* bit to check in send mask */
int result; /* the system call’s result */
vir_clicks vlo, vhi; /* virtual clicks containing message to send */

/* Check if the process has privileges for the requested call. Calls to the
* kernel may only be SENDREC, because tasks always reply and may not block
* if the caller doesn’t do receive().
*/
if (! (priv(caller_ptr)->s_trap_mask & (1 << function)) ||
(iskerneln(src_dst) && function != SENDREC
&& function != RECEIVE)) {
kprintf("sys_call: trap %d not allowed, caller %d, src_dst %d\n",
function, proc_nr(caller_ptr), src_dst);
return(ECALLDENIED); /* trap denied by mask or kernel */
}

/* Require a valid source and/ or destination process, unless echoing. */
if (! (isokprocn(src_dst) || src_dst == ANY || function == ECHO)) {
kprintf("sys_call: invalid src_dst, src_dst %d, caller %d\n",
src_dst, proc_nr(caller_ptr));
return(EBADSRCDST); /* invalid process number */
}

/* If the call involves a message buffer, i.e., for SEND, RECEIVE, SENDREC,
* or ECHO, check the message pointer. This check allows a message to be
* anywhere in data or stack or gap. It will have to be made more elaborate
* for machines which don’t have the gap mapped.
*/
if (function & CHECK_PTR) {
vlo = (vir_bytes) m_ptr >> CLICK_SHIFT;
vhi = ((vir_bytes) m_ptr + MESS_SIZE - 1) >> CLICK_SHIFT;
if (vlo < caller_ptr->p_memmap[D].mem_vir || vlo > vhi ||
vhi >= caller_ptr->p_memmap[S].mem_vir +
caller_ptr->p_memmap[S].mem_len) {
kprintf("sys_call: invalid message pointer, trap %d, caller %d\n",
function, proc_nr(caller_ptr));
return(EFAULT); /* invalid message pointer */
}
}

/* If the call is to send to a process, i.e., for SEND, SENDREC or NOTIFY,
* verify that the caller is allowed to send to the given destination and
* that the destination is still alive.
*/
if (function & CHECK_DST) {
MINIX SOURCE CODE File: kernel/proc.c 725
if (! get_sys_bit(priv(caller_ptr)->s_ipc_to, nr_to_id(src_dst))) {
kprintf("sys_call: ipc mask denied %d sending to %d\n",
proc_nr(caller_ptr), src_dst);
return(ECALLDENIED); /* call denied by ipc mask */
}

if (isemptyn(src_dst) && !shutdown_started) {
kprintf("sys_call: dead dest; %d, %d, %d\n",
function, proc_nr(caller_ptr), src_dst);
return(EDEADDST); /* cannot send to the dead */
}
}

/* Now check if the call is known and try to perform the request. The only
* system calls that exist in MINIX are sending and receiving messages.
* - SENDREC: combines SEND and RECEIVE in a single system call
* - SEND: sender blocks until its message has been delivered
* - RECEIVE: receiver blocks until an acceptable message has arrived
* - NOTIFY: nonblocking call; deliver notification or mark pending
* - ECHO: nonblocking call; directly echo back the message
*/
switch(function) {
case SENDREC:
/* A flag is set so that notifications cannot interrupt SENDREC. */
priv(caller_ptr)->s_flags |= SENDREC_BUSY;
/* fall through */
case SEND:
result = mini_send(caller_ptr, src_dst, m_ptr, flags);
if (function == SEND || result != OK) {
break; /* done, or SEND failed */
} /* fall through for SENDREC */
case RECEIVE:
if (function == RECEIVE)
priv(caller_ptr)->s_flags &= ˜SENDREC_BUSY;
result = mini_receive(caller_ptr, src_dst, m_ptr, flags);
break;
case NOTIFY:
result = mini_notify(caller_ptr, src_dst);
break;
case ECHO:
CopyMess(caller_ptr->p_nr, caller_ptr, m_ptr, caller_ptr, m_ptr);
result = OK;
break;
default:
result = EBADCALL; /* illegal system call */
}

/* Now, return the result of the system call to the caller. */
return(result);
}
/*===========================================================================*
* mini_send *
*===========================================================================*/
PRIVATE int mini_send(caller_ptr, dst, m_ptr, flags)
register struct proc *caller_ptr; /* who is trying to send a message? */
int dst; /* to whom is message being sent? */
message *m_ptr; /* pointer to message buffer */
unsigned flags; /* system call flags */
{
726 File: kernel/proc.c MINIX SOURCE CODE
/* Send a message from ’caller_ptr’ to ’dst’. If ’dst’ is blocked waiting
* for this message, copy the message to it and unblock ’dst’. If ’dst’ is
* not waiting at all, or is waiting for another source, queue ’caller_ptr’.
*/
register struct proc *dst_ptr = proc_addr(dst);
register struct proc **xpp;
register struct proc *xp;

/* Check for deadlock by ’caller_ptr’ and ’dst’ sending to each other. */
xp = dst_ptr;
while (xp->p_rts_flags & SENDING) { /* check while sending */
xp = proc_addr(xp->p_sendto); /* get xp’s destination */
if (xp == caller_ptr) return(ELOCKED); /* deadlock if cyclic */
}

/* Check if ’dst’ is blocked waiting for this message. The destination’s
* SENDING flag may be set when its SENDREC call blocked while sending.
*/
if ( (dst_ptr->p_rts_flags & (RECEIVING | SENDING)) == RECEIVING &&
(dst_ptr->p_getfrom == ANY || dst_ptr->p_getfrom == caller_ptr->p_nr)) {
/* Destination is indeed waiting for this message. */
CopyMess(caller_ptr->p_nr, caller_ptr, m_ptr, dst_ptr,
dst_ptr->p_messbuf);
if ((dst_ptr->p_rts_flags &= ˜RECEIVING) == 0) enqueue(dst_ptr);
} else if ( ! (flags & NON_BLOCKING)) {
/* Destination is not waiting. Block and dequeue caller. */
caller_ptr->p_messbuf = m_ptr;
if (caller_ptr->p_rts_flags == 0) dequeue(caller_ptr);
caller_ptr->p_rts_flags |= SENDING;
caller_ptr->p_sendto = dst;

/* Process is now blocked. Put in on the destination’s queue. */
xpp = &dst_ptr->p_caller_q; /* find end of list */
while (*xpp != NIL_PROC) xpp = &(*xpp)->p_q_link;
*xpp = caller_ptr; /* add caller to end */
caller_ptr->p_q_link = NIL_PROC; /* mark new end of list */
} else {
return(ENOTREADY);
}
return(OK);
}
/*===========================================================================*
* mini_receive *
*===========================================================================*/
PRIVATE int mini_receive(caller_ptr, src, m_ptr, flags)
register struct proc *caller_ptr; /* process trying to get message */
int src; /* which message source is wanted */
message *m_ptr; /* pointer to message buffer */
unsigned flags; /* system call flags */
{
/* A process or task wants to get a message. If a message is already queued,
* acquire it and deblock the sender. If no message from the desired source
* is available block the caller, unless the flags don’t allow blocking.
*/
register struct proc **xpp;
register struct notification **ntf_q_pp;
message m;
int bit_nr;
sys_map_t *map;
MINIX SOURCE CODE File: kernel/proc.c 727
bitchunk_t *chunk;
int i, src_id, src_proc_nr;

/* Check to see if a message from desired source is already available.
* The caller’s SENDING flag may be set if SENDREC couldn’t send. If it is
* set, the process should be blocked.
*/
if (!(caller_ptr->p_rts_flags & SENDING)) {

/* Check if there are pending notifications, except for SENDREC. */
if (! (priv(caller_ptr)->s_flags & SENDREC_BUSY)) {

map = &priv(caller_ptr)->s_notify_pending;
for (chunk=&map->chunk[0]; chunk<&map->chunk[NR_SYS_CHUNKS]; chunk++) {

/* Find a pending notification from the requested source. */
if (! *chunk) continue; /* no bits in chunk */
for (i=0; ! (*chunk & (1<<i)); ++i) {} /* look up the bit */
src_id = (chunk - &map->chunk[0]) * BITCHUNK_BITS + i;
if (src_id >= NR_SYS_PROCS) break; /* out of range */
src_proc_nr = id_to_nr(src_id); /* get source proc */
if (src!=ANY && src!=src_proc_nr) continue; /* source not ok */
*chunk &= ˜(1 << i); /* no longer pending */

/* Found a suitable source, deliver the notification message. */
BuildMess(&m, src_proc_nr, caller_ptr); /* assemble message */
CopyMess(src_proc_nr, proc_addr(HARDWARE), &m, caller_ptr, m_ptr);
return(OK); /* report success */
}
}

/* Check caller queue. Use pointer pointers to keep code simple. */
xpp = &caller_ptr->p_caller_q;
while (*xpp != NIL_PROC) {
if (src == ANY || src == proc_nr(*xpp)) {
/* Found acceptable message. Copy it and update status. */
CopyMess((*xpp)->p_nr, *xpp, (*xpp)->p_messbuf, caller_ptr, m_ptr);
if (((*xpp)->p_rts_flags &= ˜SENDING) == 0) enqueue(*xpp);
*xpp = (*xpp)->p_q_link; /* remove from queue */
return(OK); /* report success */
}
xpp = &(*xpp)->p_q_link; /* proceed to next */
}
}

/* No suitable message is available or the caller couldn’t send in SENDREC.
* Block the process trying to receive, unless the flags tell otherwise.
*/
if ( ! (flags & NON_BLOCKING)) {
caller_ptr->p_getfrom = src;
caller_ptr->p_messbuf = m_ptr;
if (caller_ptr->p_rts_flags == 0) dequeue(caller_ptr);
caller_ptr->p_rts_flags |= RECEIVING;
return(OK);
} else {
return(ENOTREADY);
}
}
728 File: kernel/proc.c MINIX SOURCE CODE
/*===========================================================================*
* mini_notify *
*===========================================================================*/
PRIVATE int mini_notify(caller_ptr, dst)
register struct proc *caller_ptr; /* sender of the notification */
int dst; /* which process to notify */
{
register struct proc *dst_ptr = proc_addr(dst);
int src_id; /* source id for late delivery */
message m; /* the notification message */

/* Check to see if target is blocked waiting for this message. A process
* can be both sending and receiving during a SENDREC system call.
*/
if ((dst_ptr->p_rts_flags & (RECEIVING|SENDING)) == RECEIVING &&
! (priv(dst_ptr)->s_flags & SENDREC_BUSY) &&
(dst_ptr->p_getfrom == ANY || dst_ptr->p_getfrom == caller_ptr->p_nr)) {

/* Destination is indeed waiting for a message. Assemble a notification
* message and deliver it. Copy from pseudo-source HARDWARE, since the
* message is in the kernel’s address space.
*/
BuildMess(&m, proc_nr(caller_ptr), dst_ptr);
CopyMess(proc_nr(caller_ptr), proc_addr(HARDWARE), &m,
dst_ptr, dst_ptr->p_messbuf);
dst_ptr->p_rts_flags &= ˜RECEIVING; /* deblock destination */
if (dst_ptr->p_rts_flags == 0) enqueue(dst_ptr);
return(OK);
}

/* Destination is not ready to receive the notification. Add it to the
* bit map with pending notifications. Note the indirectness: the system id
* instead of the process number is used in the pending bit map.
*/
src_id = priv(caller_ptr)->s_id;
set_sys_bit(priv(dst_ptr)->s_notify_pending, src_id);
return(OK);
}
/*===========================================================================*
* lock_notify *
*===========================================================================*/
PUBLIC int lock_notify(src, dst)
int src; /* sender of the notification */
int dst; /* who is to be notified */
{
/* Safe gateway to mini_notify() for tasks and interrupt handlers. The sender
* is explicitly given to prevent confusion where the call comes from. MINIX
* kernel is not reentrant, which means to interrupts are disabled after
* the first kernel entry (hardware interrupt, trap, or exception). Locking
* is done by temporarily disabling interrupts.
*/
int result;

/* Exception or interrupt occurred, thus already locked. */
if (k_reenter >= 0) {
result = mini_notify(proc_addr(src), dst);
}

/* Call from task level, locking is required. */
MINIX SOURCE CODE File: kernel/proc.c 729
else {
lock(0, "notify");
result = mini_notify(proc_addr(src), dst);
unlock(0);
}
return(result);
}
/*===========================================================================*
* enqueue *
*===========================================================================*/
PRIVATE void enqueue(rp)
register struct proc *rp; /* this process is now runnable */
{
/* Add ’rp’ to one of the queues of runnable processes. This function is
* responsible for inserting a process into one of the scheduling queues.
* The mechanism is implemented here. The actual scheduling policy is
* defined in sched() and pick_proc().
*/
int q; /* scheduling queue to use */
int front; /* add to front or back */

/* Determine where to insert to process. */
sched(rp, &q, &front);

/* Now add the process to the queue. */
if (rdy_head[q] == NIL_PROC) { /* add to empty queue */
rdy_head[q] = rdy_tail[q] = rp; /* create a new queue */
rp->p_nextready = NIL_PROC; /* mark new end */
}
else if (front) { /* add to head of queue */
rp->p_nextready = rdy_head[q]; /* chain head of queue */
rdy_head[q] = rp; /* set new queue head */
}
else { /* add to tail of queue */
rdy_tail[q]->p_nextready = rp; /* chain tail of queue */
rdy_tail[q] = rp; /* set new queue tail */
rp->p_nextready = NIL_PROC; /* mark new end */
}

/* Now select the next process to run. */
pick_proc();
}
/*===========================================================================*
* dequeue *
*===========================================================================*/
PRIVATE void dequeue(rp)
register struct proc *rp; /* this process is no longer runnable */
{
/* A process must be removed from the scheduling queues, for example, because
* it has blocked. If the currently active process is removed, a new process
* is picked to run by calling pick_proc().
*/
register int q = rp->p_priority; /* queue to use */
register struct proc **xpp; /* iterate over queue */
register struct proc *prev_xp;

/* Side-effect for kernel: check if the task’s stack still is ok? */
if (iskernelp(rp)) {
730 File: kernel/proc.c MINIX SOURCE CODE
if (*priv(rp)->s_stack_guard != STACK_GUARD)
panic("stack overrun by task", proc_nr(rp));
}

/* Now make sure that the process is not in its ready queue. Remove the
* process if it is found. A process can be made unready even if it is not
* running by being sent a signal that kills it.
*/
prev_xp = NIL_PROC;
for (xpp = &rdy_head[q]; *xpp != NIL_PROC; xpp = &(*xpp)->p_nextready) {

if (*xpp == rp) { /* found process to remove */
*xpp = (*xpp)->p_nextready; /* replace with next chain */
if (rp == rdy_tail[q]) /* queue tail removed */
rdy_tail[q] = prev_xp; /* set new tail */
if (rp == proc_ptr || rp == next_ptr) /* active process removed */
pick_proc(); /* pick new process to run */
break;
}
prev_xp = *xpp; /* save previous in chain */
}
}
/*===========================================================================*
* sched *
*===========================================================================*/
PRIVATE void sched(rp, queue, front)
register struct proc *rp; /* process to be scheduled */
int *queue; /* return: queue to use */
int *front; /* return: front or back */
{
/* This function determines the scheduling policy. It is called whenever a
* process must be added to one of the scheduling queues to decide where to
* insert it. As a side-effect the process’ priority may be updated.
*/
static struct proc *prev_ptr = NIL_PROC; /* previous without time */
int time_left = (rp->p_ticks_left > 0); /* quantum fully consumed */
int penalty = 0; /* change in priority */

/* Check whether the process has time left. Otherwise give a new quantum
* and possibly raise the priority. Processes using multiple quantums
* in a row get a lower priority to catch infinite loops in high priority
* processes (system servers and drivers).
*/
if ( ! time_left) { /* quantum consumed ? */
rp->p_ticks_left = rp->p_quantum_size; /* give new quantum */
if (prev_ptr == rp) penalty ++; /* catch infinite loops */
else penalty --; /* give slow way back */
prev_ptr = rp; /* store ptr for next */
}

/* Determine the new priority of this process. The bounds are determined
* by IDLE’s queue and the maximum priority of this process. Kernel tasks
* and the idle process are never changed in priority.
*/
if (penalty != 0 && ! iskernelp(rp)) {
rp->p_priority += penalty; /* update with penalty */
if (rp->p_priority < rp->p_max_priority) /* check upper bound */
rp->p_priority=rp->p_max_priority;
else if (rp->p_priority > IDLE_Q-1) /* check lower bound */
MINIX SOURCE CODE File: kernel/proc.c 731
rp->p_priority = IDLE_Q-1;
}

/* If there is time left, the process is added to the front of its queue,
* so that it can immediately run. The queue to use simply is always the
* process’ current priority.
*/
*queue = rp->p_priority;
*front = time_left;
}
/*===========================================================================*
* pick_proc *
*===========================================================================*/
PRIVATE void pick_proc()
{
/* Decide who to run now. A new process is selected by setting ’next_ptr’.
* When a billable process is selected, record it in ’bill_ptr’, so that the
* clock task can tell who to bill for system time.
*/
register struct proc *rp; /* process to run */
int q; /* iterate over queues */

/* Check each of the scheduling queues for ready processes. The number of
* queues is defined in proc.h, and priorities are set in the image table.
* The lowest queue contains IDLE, which is always ready.
*/
for (q=0; q < NR_SCHED_QUEUES; q++) {
if ( (rp = rdy_head[q]) != NIL_PROC) {
next_ptr = rp; /* run process ’rp’ next */
if (priv(rp)->s_flags & BILLABLE)
bill_ptr = rp; /* bill for system time */
return;
}
}
}
/*===========================================================================*
* lock_send *
*===========================================================================*/
PUBLIC int lock_send(dst, m_ptr)
int dst; /* to whom is message being sent? */
message *m_ptr; /* pointer to message buffer */
{
/* Safe gateway to mini_send() for tasks. */
int result;
lock(2, "send");
result = mini_send(proc_ptr, dst, m_ptr, NON_BLOCKING);
unlock(2);
return(result);
}
/*===========================================================================*
* lock_enqueue *
*===========================================================================*/
PUBLIC void lock_enqueue(rp)
struct proc *rp; /* this process is now runnable */
{
/* Safe gateway to enqueue() for tasks. */
lock(3, "enqueue");
732 File: kernel/proc.c MINIX SOURCE CODE
enqueue(rp);
unlock(3);
}
/*===========================================================================*
* lock_dequeue *
*===========================================================================*/
PUBLIC void lock_dequeue(rp)
struct proc *rp; /* this process is no longer runnable */
{
/* Safe gateway to dequeue() for tasks. */
lock(4, "dequeue");
dequeue(rp);
unlock(4);
}
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/exception.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This file contains a simple exception handler. Exceptions in user
* processes are converted to signals. Exceptions in a kernel task cause
* a panic.
*/

#include "kernel.h"
#include <signal.h>
#include "proc.h"

/*===========================================================================*
* exception *
*===========================================================================*/
PUBLIC void exception(vec_nr)
unsigned vec_nr;
{
/* An exception or unexpected interrupt has occurred. */

struct ex_s {
char *msg;
int signum;
int minprocessor;
};
static struct ex_s ex_data[] = {
{ "Divide error", SIGFPE, 86 },
{ "Debug exception", SIGTRAP, 86 },
{ "Nonmaskable interrupt", SIGBUS, 86 },
{ "Breakpoint", SIGEMT, 86 },
{ "Overflow", SIGFPE, 86 },
{ "Bounds check", SIGFPE, 186 },
{ "Invalid opcode", SIGILL, 186 },
{ "Coprocessor not available", SIGFPE, 186 },
{ "Double fault", SIGBUS, 286 },
{ "Copressor segment overrun", SIGSEGV, 286 },
{ "Invalid TSS", SIGSEGV, 286 },
{ "Segment not present", SIGSEGV, 286 },
MINIX SOURCE CODE File: kernel/exception.c 733
{ "Stack exception", SIGSEGV, 286 }, /* STACK_FAULT already used */
{ "General protection", SIGSEGV, 286 },
{ "Page fault", SIGSEGV, 386 }, /* not close */
{ NIL_PTR, SIGILL, 0 }, /* probably software trap */
{ "Coprocessor error", SIGFPE, 386 },
};
register struct ex_s *ep;
struct proc *saved_proc;

/* Save proc_ptr, because it may be changed by debug statements. */
saved_proc = proc_ptr;

ep = &ex_data[vec_nr];

if (vec_nr == 2) { /* spurious NMI on some machines */
kprintf("got spurious NMI\n");
return;
}

/* If an exception occurs while running a process, the k_reenter variable
* will be zero. Exceptions in interrupt handlers or system traps will make
* k_reenter larger than zero.
*/
if (k_reenter == 0 && ! iskernelp(saved_proc)) {
cause_sig(proc_nr(saved_proc), ep->signum);
return;
}

/* Exception in system code. This is not supposed to happen. */
if (ep->msg == NIL_PTR || machine.processor < ep->minprocessor)
kprintf("\nIntel-reserved exception %d\n", vec_nr);
else
kprintf("\n%s\n", ep->msg);
kprintf("k_reenter = %d ", k_reenter);
kprintf("process %d (%s), ", proc_nr(saved_proc), saved_proc->p_name);
kprintf("pc = %u:0x%x", (unsigned) saved_proc->p_reg.cs,
(unsigned) saved_proc->p_reg.pc);

panic("exception in a kernel task", NO_NUM);
}
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/i8259.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This file contains routines for initializing the 8259 interrupt controller:
* put_irq_handler: register an interrupt handler
* rm_irq_handler: deregister an interrupt handler
* intr_handle: handle a hardware interrupt
* intr_init: initialize the interrupt controller(s)
*/

#include "kernel.h"
#include "proc.h"
#include <minix/com.h>
734 File: kernel/i8259.c MINIX SOURCE CODE

#define ICW1_AT 0x11 /* edge triggered, cascade, need ICW4 */
#define ICW1_PC 0x13 /* edge triggered, no cascade, need ICW4 */
#define ICW1_PS 0x19 /* level triggered, cascade, need ICW4 */
#define ICW4_AT_SLAVE 0x01 /* not SFNM, not buffered, normal EOI, 8086 */
#define ICW4_AT_MASTER 0x05 /* not SFNM, not buffered, normal EOI, 8086 */
#define ICW4_PC_SLAVE 0x09 /* not SFNM, buffered, normal EOI, 8086 */
#define ICW4_PC_MASTER 0x0D /* not SFNM, buffered, normal EOI, 8086 */

#define set_vec(nr, addr) ((void)0)

/*===========================================================================*
* intr_init *
*===========================================================================*/
PUBLIC void intr_init(mine)
int mine;
{
/* Initialize the 8259s, finishing with all interrupts disabled. This is
* only done in protected mode, in real mode we don’t touch the 8259s, but
* use the BIOS locations instead. The flag "mine" is set if the 8259s are
* to be programmed for MINIX, or to be reset to what the BIOS expects.
*/
int i;

intr_disable();

/* The AT and newer PS/2 have two interrupt controllers, one master,
* one slaved at IRQ 2. (We don’t have to deal with the PC that
* has just one controller, because it must run in real mode.)
*/
outb(INT_CTL, machine.ps_mca ? ICW1_PS : ICW1_AT);
outb(INT_CTLMASK, mine ? IRQ0_VECTOR : BIOS_IRQ0_VEC);
/* ICW2 for master */
outb(INT_CTLMASK, (1 << CASCADE_IRQ)); /* ICW3 tells slaves */
outb(INT_CTLMASK, ICW4_AT_MASTER);
outb(INT_CTLMASK, ˜(1 << CASCADE_IRQ)); /* IRQ 0-7 mask */
outb(INT2_CTL, machine.ps_mca ? ICW1_PS : ICW1_AT);
outb(INT2_CTLMASK, mine ? IRQ8_VECTOR : BIOS_IRQ8_VEC);
/* ICW2 for slave */
outb(INT2_CTLMASK, CASCADE_IRQ); /* ICW3 is slave nr */
outb(INT2_CTLMASK, ICW4_AT_SLAVE);
outb(INT2_CTLMASK, ˜0); /* IRQ 8-15 mask */

/* Copy the BIOS vectors from the BIOS to the Minix location, so we
* can still make BIOS calls without reprogramming the i8259s.
*/
phys_copy(BIOS_VECTOR(0) * 4L, VECTOR(0) * 4L, 8 * 4L);
}
/*===========================================================================*
* put_irq_handler *
*===========================================================================*/
PUBLIC void put_irq_handler(hook, irq, handler)
irq_hook_t *hook;
int irq;
irq_handler_t handler;
{
/* Register an interrupt handler. */
int id;
irq_hook_t **line;
MINIX SOURCE CODE File: kernel/i8259.c 735

if (irq < 0 || irq >= NR_IRQ_VECTORS)
panic("invalid call to put_irq_handler", irq);

line = &irq_handlers[irq];
id = 1;
while (*line != NULL) {
if (hook == *line) return; /* extra initialization */
line = &(*line)->next;
id <<= 1;
}
if (id == 0) panic("Too many handlers for irq", irq);

hook->next = NULL;
hook->handler = handler;
hook->irq = irq;
hook->id = id;
*line = hook;

irq_use |= 1 << irq;
}
/*===========================================================================*
* rm_irq_handler *
*===========================================================================*/
PUBLIC void rm_irq_handler(hook)
irq_hook_t *hook;
{
/* Unregister an interrupt handler. */
int irq = hook->irq;
int id = hook->id;
irq_hook_t **line;

if (irq < 0 || irq >= NR_IRQ_VECTORS)
panic("invalid call to rm_irq_handler", irq);

line = &irq_handlers[irq];
while (*line != NULL) {
if ((*line)->id == id) {
(*line) = (*line)->next;
if (! irq_handlers[irq]) irq_use &= ˜(1 << irq);
return;
}
line = &(*line)->next;
}
/* When the handler is not found, normally return here. */
}
/*===========================================================================*
* intr_handle *
*===========================================================================*/
PUBLIC void intr_handle(hook)
irq_hook_t *hook;
{
/* Call the interrupt handlers for an interrupt with the given hook list.
* The assembly part of the handler has already masked the IRQ, reenabled the
* controller(s) and enabled interrupts.
*/

/* Call list of handlers for an IRQ. */
736 File: kernel/i8259.c MINIX SOURCE CODE
while (hook != NULL) {
/* For each handler in the list, mark it active by setting its ID bit,
* call the function, and unmark it if the function returns true.
*/
irq_actids[hook->irq] |= hook->id;
if ((*hook->handler)(hook)) irq_actids[hook->irq] &= ˜hook->id;
hook = hook->next;
}

/* The assembly code will now disable interrupts, unmask the IRQ if and only
* if all active ID bits are cleared, and restart a process.
*/
}
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/protect.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This file contains code for initialization of protected mode, to initialize
* code and data segment descriptors, and to initialize global descriptors
* for local descriptors in the process table.
*/

#include "kernel.h"
#include "proc.h"
#include "protect.h"

#define INT_GATE_TYPE (INT_286_GATE | DESC_386_BIT)
#define TSS_TYPE (AVL_286_TSS | DESC_386_BIT)

struct desctableptr_s {
char limit[sizeof(u16_t)];
char base[sizeof(u32_t)]; /* really u24_t + pad for 286 */
};

struct gatedesc_s {
u16_t offset_low;
u16_t selector;
u8_t pad; /* |000|XXXXX| ig & trpg, |XXXXXXXX| task g */
u8_t p_dpl_type; /* |P|DL|0|TYPE| */
u16_t offset_high;
};

struct tss_s {
reg_t backlink;
reg_t sp0; /* stack pointer to use during interrupt */
reg_t ss0; /* " segment " " " " */
reg_t sp1;
reg_t ss1;
reg_t sp2;
reg_t ss2;
reg_t cr3;
reg_t ip;
reg_t flags;
reg_t ax;
reg_t cx;
reg_t dx;
reg_t bx;
MINIX SOURCE CODE File: kernel/protect.c 737
reg_t sp;
reg_t bp;
reg_t si;
reg_t di;
reg_t es;
reg_t cs;
reg_t ss;
reg_t ds;
reg_t fs;
reg_t gs;
reg_t ldt;
u16_t trap;
u16_t iobase;
/* u8_t iomap[0]; */
};

PUBLIC struct segdesc_s gdt[GDT_SIZE]; /* used in klib.s and mpx.s */
PRIVATE struct gatedesc_s idt[IDT_SIZE]; /* zero-init so none present */
PUBLIC struct tss_s tss; /* zero init */

FORWARD _PROTOTYPE( void int_gate, (unsigned vec_nr, vir_bytes offset,
unsigned dpl_type) );
FORWARD _PROTOTYPE( void sdesc, (struct segdesc_s *segdp, phys_bytes base,
vir_bytes size) );

/*===========================================================================*
* prot_init *
*===========================================================================*/
PUBLIC void prot_init()
{
/* Set up tables for protected mode.
* All GDT slots are allocated at compile time.
*/
struct gate_table_s *gtp;
struct desctableptr_s *dtp;
unsigned ldt_index;
register struct proc *rp;

static struct gate_table_s {
_PROTOTYPE( void (*gate), (void) );
unsigned char vec_nr;
unsigned char privilege;
}
gate_table[] = {
{ divide_error, DIVIDE_VECTOR, INTR_PRIVILEGE },
{ single_step_exception, DEBUG_VECTOR, INTR_PRIVILEGE },
{ nmi, NMI_VECTOR, INTR_PRIVILEGE },
{ breakpoint_exception, BREAKPOINT_VECTOR, USER_PRIVILEGE },
{ overflow, OVERFLOW_VECTOR, USER_PRIVILEGE },
{ bounds_check, BOUNDS_VECTOR, INTR_PRIVILEGE },
{ inval_opcode, INVAL_OP_VECTOR, INTR_PRIVILEGE },
{ copr_not_available, COPROC_NOT_VECTOR, INTR_PRIVILEGE },
{ double_fault, DOUBLE_FAULT_VECTOR, INTR_PRIVILEGE },
{ copr_seg_overrun, COPROC_SEG_VECTOR, INTR_PRIVILEGE },
{ inval_tss, INVAL_TSS_VECTOR, INTR_PRIVILEGE },
{ segment_not_present, SEG_NOT_VECTOR, INTR_PRIVILEGE },
{ stack_exception, STACK_FAULT_VECTOR, INTR_PRIVILEGE },
{ general_protection, PROTECTION_VECTOR, INTR_PRIVILEGE },
{ page_fault, PAGE_FAULT_VECTOR, INTR_PRIVILEGE },
{ copr_error, COPROC_ERR_VECTOR, INTR_PRIVILEGE },
738 File: kernel/protect.c MINIX SOURCE CODE
{ hwint00, VECTOR( 0), INTR_PRIVILEGE },
{ hwint01, VECTOR( 1), INTR_PRIVILEGE },
{ hwint02, VECTOR( 2), INTR_PRIVILEGE },
{ hwint03, VECTOR( 3), INTR_PRIVILEGE },
{ hwint04, VECTOR( 4), INTR_PRIVILEGE },
{ hwint05, VECTOR( 5), INTR_PRIVILEGE },
{ hwint06, VECTOR( 6), INTR_PRIVILEGE },
{ hwint07, VECTOR( 7), INTR_PRIVILEGE },
{ hwint08, VECTOR( 8), INTR_PRIVILEGE },
{ hwint09, VECTOR( 9), INTR_PRIVILEGE },
{ hwint10, VECTOR(10), INTR_PRIVILEGE },
{ hwint11, VECTOR(11), INTR_PRIVILEGE },
{ hwint12, VECTOR(12), INTR_PRIVILEGE },
{ hwint13, VECTOR(13), INTR_PRIVILEGE },
{ hwint14, VECTOR(14), INTR_PRIVILEGE },
{ hwint15, VECTOR(15), INTR_PRIVILEGE },
{ s_call, SYS386_VECTOR, USER_PRIVILEGE }, /* 386 system call */
{ level0_call, LEVEL0_VECTOR, TASK_PRIVILEGE },
};

/* Build gdt and idt pointers in GDT where the BIOS expects them. */
dtp= (struct desctableptr_s *) &gdt[GDT_INDEX];
* (u16_t *) dtp->limit = (sizeof gdt) - 1;
* (u32_t *) dtp->base = vir2phys(gdt);

dtp= (struct desctableptr_s *) &gdt[IDT_INDEX];
* (u16_t *) dtp->limit = (sizeof idt) - 1;
* (u32_t *) dtp->base = vir2phys(idt);

/* Build segment descriptors for tasks and interrupt handlers. */
init_codeseg(&gdt[CS_INDEX],
kinfo.code_base, kinfo.code_size, INTR_PRIVILEGE);
init_dataseg(&gdt[DS_INDEX],
kinfo.data_base, kinfo.data_size, INTR_PRIVILEGE);
init_dataseg(&gdt[ES_INDEX], 0L, 0, TASK_PRIVILEGE);

/* Build scratch descriptors for functions in klib88. */
init_dataseg(&gdt[DS_286_INDEX], 0L, 0, TASK_PRIVILEGE);
init_dataseg(&gdt[ES_286_INDEX], 0L, 0, TASK_PRIVILEGE);

/* Build local descriptors in GDT for LDT’s in process table.
* The LDT’s are allocated at compile time in the process table, and
* initialized whenever a process’ map is initialized or changed.
*/
for (rp = BEG_PROC_ADDR, ldt_index = FIRST_LDT_INDEX;
rp < END_PROC_ADDR; ++rp, ldt_index++) {
init_dataseg(&gdt[ldt_index], vir2phys(rp->p_ldt),
sizeof(rp->p_ldt), INTR_PRIVILEGE);
gdt[ldt_index].access = PRESENT | LDT;
rp->p_ldt_sel = ldt_index * DESC_SIZE;
}

/* Build main TSS.
* This is used only to record the stack pointer to be used after an
* interrupt.
* The pointer is set up so that an interrupt automatically saves the
* current process’s registers ip:cs:f:sp:ss in the correct slots in the
* process table.
*/
tss.ss0 = DS_SELECTOR;
MINIX SOURCE CODE File: kernel/protect.c 739
init_dataseg(&gdt[TSS_INDEX], vir2phys(&tss), sizeof(tss), INTR_PRIVILEGE);
gdt[TSS_INDEX].access = PRESENT | (INTR_PRIVILEGE << DPL_SHIFT) | TSS_TYPE;

/* Build descriptors for interrupt gates in IDT. */
for (gtp = &gate_table[0];
gtp < &gate_table[sizeof gate_table / sizeof gate_table[0]]; ++gtp) {
int_gate(gtp->vec_nr, (vir_bytes) gtp->gate,
PRESENT | INT_GATE_TYPE | (gtp->privilege << DPL_SHIFT));
}

/* Complete building of main TSS. */
tss.iobase = sizeof tss; /* empty i/o permissions map */
}
/*===========================================================================*
* init_codeseg *
*===========================================================================*/
PUBLIC void init_codeseg(segdp, base, size, privilege)
register struct segdesc_s *segdp;
phys_bytes base;
vir_bytes size;
int privilege;
{
/* Build descriptor for a code segment. */
sdesc(segdp, base, size);
segdp->access = (privilege << DPL_SHIFT)
| (PRESENT | SEGMENT | EXECUTABLE | READABLE);
/* CONFORMING = 0, ACCESSED = 0 */
}
/*===========================================================================*
* init_dataseg *
*===========================================================================*/
PUBLIC void init_dataseg(segdp, base, size, privilege)
register struct segdesc_s *segdp;
phys_bytes base;
vir_bytes size;
int privilege;
{
/* Build descriptor for a data segment. */
sdesc(segdp, base, size);
segdp->access = (privilege << DPL_SHIFT) | (PRESENT | SEGMENT | WRITEABLE);
/* EXECUTABLE = 0, EXPAND_DOWN = 0, ACCESSED = 0 */
}
/*===========================================================================*
* sdesc *
*===========================================================================*/
PRIVATE void sdesc(segdp, base, size)
register struct segdesc_s *segdp;
phys_bytes base;
vir_bytes size;
{
/* Fill in the size fields (base, limit and granularity) of a descriptor. */
segdp->base_low = base;
segdp->base_middle = base >> BASE_MIDDLE_SHIFT;
segdp->base_high = base >> BASE_HIGH_SHIFT;

--size; /* convert to a limit, 0 size means 4G */
if (size > BYTE_GRAN_MAX) {
740 File: kernel/protect.c MINIX SOURCE CODE
segdp->limit_low = size >> PAGE_GRAN_SHIFT;
segdp->granularity = GRANULAR | (size >>
(PAGE_GRAN_SHIFT + GRANULARITY_SHIFT));
} else {
segdp->limit_low = size;
segdp->granularity = size >> GRANULARITY_SHIFT;
}
segdp->granularity |= DEFAULT; /* means BIG for data seg */
}
/*===========================================================================*
* seg2phys *
*===========================================================================*/
PUBLIC phys_bytes seg2phys(seg)
U16_t seg;
{
/* Return the base address of a segment, with seg being either a 8086 segment
* register, or a 286/386 segment selector.
*/
phys_bytes base;
struct segdesc_s *segdp;

if (! machine.protected) {
base = hclick_to_physb(seg);
} else {
segdp = &gdt[seg >> 3];
base = ((u32_t) segdp->base_low << 0)
| ((u32_t) segdp->base_middle << 16)
| ((u32_t) segdp->base_high << 24);
}
return base;
}
/*===========================================================================*
* phys2seg *
*===========================================================================*/
PUBLIC void phys2seg(seg, off, phys)
u16_t *seg;
vir_bytes *off;
phys_bytes phys;
{
/* Return a segment selector and offset that can be used to reach a physical
* address, for use by a driver doing memory I/O in the A0000 - DFFFF range.
*/
*seg = FLAT_DS_SELECTOR;
*off = phys;
}
/*===========================================================================*
* int_gate *
*===========================================================================*/
PRIVATE void int_gate(vec_nr, offset, dpl_type)
unsigned vec_nr;
vir_bytes offset;
unsigned dpl_type;
{
/* Build descriptor for an interrupt gate. */
register struct gatedesc_s *idp;

idp = &idt[vec_nr];
MINIX SOURCE CODE File: kernel/protect.c 741
idp->offset_low = offset;
idp->selector = CS_SELECTOR;
idp->p_dpl_type = dpl_type;
idp->offset_high = offset >> OFFSET_HIGH_SHIFT;
}
/*===========================================================================*
* enable_iop *
*===========================================================================*/
PUBLIC void enable_iop(pp)
struct proc *pp;
{
/* Allow a user process to use I/O instructions. Change the I/O Permission
* Level bits in the psw. These specify least-privileged Current Permission
* Level allowed to execute I/O instructions. Users and servers have CPL 3.
* You can’t have less privilege than that. Kernel has CPL 0, tasks CPL 1.
*/
pp->p_reg.psw |= 0x3000;
}
/*===========================================================================*
* alloc_segments *
*===========================================================================*/
PUBLIC void alloc_segments(rp)
register struct proc *rp;
{
/* This is called at system initialization from main() and by do_newmap().
* The code has a separate function because of all hardware-dependencies.
* Note that IDLE is part of the kernel and gets TASK_PRIVILEGE here.
*/
phys_bytes code_bytes;
phys_bytes data_bytes;
int privilege;

if (machine.protected) {
data_bytes = (phys_bytes) (rp->p_memmap[S].mem_vir +
rp->p_memmap[S].mem_len) << CLICK_SHIFT;
if (rp->p_memmap[T].mem_len == 0)
code_bytes = data_bytes; /* common I&D, poor protect */
else
code_bytes = (phys_bytes) rp->p_memmap[T].mem_len << CLICK_SHIFT;
privilege = (iskernelp(rp)) ? TASK_PRIVILEGE : USER_PRIVILEGE;
init_codeseg(&rp->p_ldt[CS_LDT_INDEX],
(phys_bytes) rp->p_memmap[T].mem_phys << CLICK_SHIFT,
code_bytes, privilege);
init_dataseg(&rp->p_ldt[DS_LDT_INDEX],
(phys_bytes) rp->p_memmap[D].mem_phys << CLICK_SHIFT,
data_bytes, privilege);
rp->p_reg.cs = (CS_LDT_INDEX * DESC_SIZE) | TI | privilege;
rp->p_reg.gs =
rp->p_reg.fs =
rp->p_reg.ss =
rp->p_reg.es =
rp->p_reg.ds = (DS_LDT_INDEX*DESC_SIZE) | TI | privilege;
} else {
rp->p_reg.cs = click_to_hclick(rp->p_memmap[T].mem_phys);
rp->p_reg.ss =
rp->p_reg.es =
rp->p_reg.ds = click_to_hclick(rp->p_memmap[D].mem_phys);
}
742 File: kernel/protect.c MINIX SOURCE CODE
}
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/klib.s
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
! Chooses between the 8086 and 386 versions of the low level kernel code.

#include <minix/config.h>
#if _WORD_SIZE == 2
#include "klib88.s"
#else
#include "klib386.s"
#endif
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/klib386.s
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
! sections

.sect .text; .sect .rom; .sect .data; .sect .bss

#include <minix/config.h>
#include <minix/const.h>
#include "const.h"
#include "sconst.h"
#include "protect.h"

! This file contains a number of assembly code utility routines needed by the
! kernel. They are:

.define _monitor ! exit Minix and return to the monitor
.define _int86 ! let the monitor make an 8086 interrupt call
.define _cp_mess ! copies messages from source to destination
.define _exit ! dummy for library routines
.define __exit ! dummy for library routines
.define ___exit ! dummy for library routines
.define ___main ! dummy for GCC
.define _phys_insw ! transfer data from (disk controller) port to memory
.define _phys_insb ! likewise byte by byte
.define _phys_outsw ! transfer data from memory to (disk controller) port
.define _phys_outsb ! likewise byte by byte
.define _enable_irq ! enable an irq at the 8259 controller
.define _disable_irq ! disable an irq
.define _phys_copy ! copy data from anywhere to anywhere in memory
.define _phys_memset ! write pattern anywhere in memory
.define _mem_rdw ! copy one word from [segment:offset]
.define _reset ! reset the system
.define _idle_task ! task executed when there is no work
.define _level0 ! call a function at level 0
.define _read_tsc ! read the cycle counter (Pentium and up)
.define _read_cpu_flags ! read the cpu flags
MINIX SOURCE CODE File: kernel/klib386.s 743

! The routines only guarantee to preserve the registers the C compiler
! expects to be preserved (ebx, esi, edi, ebp, esp, segment registers, and
! direction bit in the flags).

.sect .text
!*===========================================================================*
!* monitor *
!*===========================================================================*
! PUBLIC void monitor();
! Return to the monitor.

_monitor:
mov esp, (_mon_sp) ! restore monitor stack pointer
o16 mov dx, SS_SELECTOR ! monitor data segment
mov ds, dx
mov es, dx
mov fs, dx
mov gs, dx
mov ss, dx
pop edi
pop esi
pop ebp
o16 retf ! return to the monitor


!*===========================================================================*
!* int86 *
!*===========================================================================*
! PUBLIC void int86();
_int86:
cmpb (_mon_return), 0 ! is the monitor there?
jnz 0f
movb ah, 0x01 ! an int 13 error seems appropriate
movb (_reg86+ 0), ah ! reg86.w.f = 1 (set carry flag)
movb (_reg86+13), ah ! reg86.b.ah = 0x01 = "invalid command"
ret
0: push ebp ! save C registers
push esi
push edi
push ebx
pushf ! save flags
cli ! no interruptions

inb INT2_CTLMASK
movb ah, al
inb INT_CTLMASK
push eax ! save interrupt masks
mov eax, (_irq_use) ! map of in-use IRQ’s
and eax, ˜[1<<CLOCK_IRQ] ! keep the clock ticking
outb INT_CTLMASK ! enable all unused IRQ’s and vv.
movb al, ah
outb INT2_CTLMASK

mov eax, SS_SELECTOR ! monitor data segment
mov ss, ax
xchg esp, (_mon_sp) ! switch stacks
push (_reg86+36) ! parameters used in INT call
push (_reg86+32)
push (_reg86+28)
744 File: kernel/klib386.s MINIX SOURCE CODE
push (_reg86+24)
push (_reg86+20)
push (_reg86+16)
push (_reg86+12)
push (_reg86+ 8)
push (_reg86+ 4)
push (_reg86+ 0)
mov ds, ax ! remaining data selectors
mov es, ax
mov fs, ax
mov gs, ax
push cs
push return ! kernel return address and selector
o16 jmpf 20+2*4+10*4+2*4(esp) ! make the call
return:
pop (_reg86+ 0)
pop (_reg86+ 4)
pop (_reg86+ 8)
pop (_reg86+12)
pop (_reg86+16)
pop (_reg86+20)
pop (_reg86+24)
pop (_reg86+28)
pop (_reg86+32)
pop (_reg86+36)
lgdt (_gdt+GDT_SELECTOR) ! reload global descriptor table
jmpf CS_SELECTOR:csinit ! restore everything
csinit: mov eax, DS_SELECTOR
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
xchg esp, (_mon_sp) ! unswitch stacks
lidt (_gdt+IDT_SELECTOR) ! reload interrupt descriptor table
andb (_gdt+TSS_SELECTOR+DESC_ACCESS), ˜0x02 ! clear TSS busy bit
mov eax, TSS_SELECTOR
ltr ax ! set TSS register

pop eax
outb INT_CTLMASK ! restore interrupt masks
movb al, ah
outb INT2_CTLMASK

add (_lost_ticks), ecx ! record lost clock ticks

popf ! restore flags
pop ebx ! restore C registers
pop edi
pop esi
pop ebp
ret


!*===========================================================================*
!* cp_mess *
!*===========================================================================*
! PUBLIC void cp_mess(int src, phys_clicks src_clicks, vir_bytes src_offset,
! phys_clicks dst_clicks, vir_bytes dst_offset);
! This routine makes a fast copy of a message from anywhere in the address
MINIX SOURCE CODE File: kernel/klib386.s 745
! space to anywhere else. It also copies the source address provided as a
! parameter to the call into the first word of the destination message.
!
! Note that the message size, "Msize" is in DWORDS (not bytes) and must be set
! correctly. Changing the definition of message in the type file and not
! changing it here will lead to total disaster.

CM_ARGS = 4 + 4 + 4 + 4 + 4 ! 4 + 4 + 4 + 4 + 4
! es ds edi esi eip proc scl sof dcl dof

.align 16
_cp_mess:
cld
push esi
push edi
push ds
push es

mov eax, FLAT_DS_SELECTOR
mov ds, ax
mov es, ax

mov esi, CM_ARGS+4(esp) ! src clicks
shl esi, CLICK_SHIFT
add esi, CM_ARGS+4+4(esp) ! src offset
mov edi, CM_ARGS+4+4+4(esp) ! dst clicks
shl edi, CLICK_SHIFT
add edi, CM_ARGS+4+4+4+4(esp) ! dst offset

mov eax, CM_ARGS(esp) ! process number of sender
stos ! copy number of sender to dest message
add esi, 4 ! do not copy first word
mov ecx, Msize - 1 ! remember, first word does not count
rep
movs ! copy the message

pop es
pop ds
pop edi
pop esi
ret ! that is all folks!


!*===========================================================================*
!* exit *
!*===========================================================================*
! PUBLIC void exit();
! Some library routines use exit, so provide a dummy version.
! Actual calls to exit cannot occur in the kernel.
! GNU CC likes to call ___main from main() for nonobvious reasons.

_exit:
__exit:
___exit:
sti
jmp ___exit

___main:
ret

746 File: kernel/klib386.s MINIX SOURCE CODE

!*===========================================================================*
!* phys_insw *
!*===========================================================================*
! PUBLIC void phys_insw(Port_t port, phys_bytes buf, size_t count);
! Input an array from an I/O port. Absolute address version of insw().

_phys_insw:
push ebp
mov ebp, esp
cld
push edi
push es
mov ecx, FLAT_DS_SELECTOR
mov es, cx
mov edx, 8(ebp) ! port to read from
mov edi, 12(ebp) ! destination addr
mov ecx, 16(ebp) ! byte count
shr ecx, 1 ! word count
rep o16 ins ! input many words
pop es
pop edi
pop ebp
ret


!*===========================================================================*
!* phys_insb *
!*===========================================================================*
! PUBLIC void phys_insb(Port_t port, phys_bytes buf, size_t count);
! Input an array from an I/O port. Absolute address version of insb().

_phys_insb:
push ebp
mov ebp, esp
cld
push edi
push es
mov ecx, FLAT_DS_SELECTOR
mov es, cx
mov edx, 8(ebp) ! port to read from
mov edi, 12(ebp) ! destination addr
mov ecx, 16(ebp) ! byte count
! shr ecx, 1 ! word count
rep insb ! input many bytes
pop es
pop edi
pop ebp
ret


!*===========================================================================*
!* phys_outsw *
!*===========================================================================*
! PUBLIC void phys_outsw(Port_t port, phys_bytes buf, size_t count);
! Output an array to an I/O port. Absolute address version of outsw().

.align 16
_phys_outsw:
push ebp
MINIX SOURCE CODE File: kernel/klib386.s 747
mov ebp, esp
cld
push esi
push ds
mov ecx, FLAT_DS_SELECTOR
mov ds, cx
mov edx, 8(ebp) ! port to write to
mov esi, 12(ebp) ! source addr
mov ecx, 16(ebp) ! byte count
shr ecx, 1 ! word count
rep o16 outs ! output many words
pop ds
pop esi
pop ebp
ret


!*===========================================================================*
!* phys_outsb *
!*===========================================================================*
! PUBLIC void phys_outsb(Port_t port, phys_bytes buf, size_t count);
! Output an array to an I/O port. Absolute address version of outsb().

.align 16
_phys_outsb:
push ebp
mov ebp, esp
cld
push esi
push ds
mov ecx, FLAT_DS_SELECTOR
mov ds, cx
mov edx, 8(ebp) ! port to write to
mov esi, 12(ebp) ! source addr
mov ecx, 16(ebp) ! byte count
rep outsb ! output many bytes
pop ds
pop esi
pop ebp
ret


!*==========================================================================*
!* enable_irq *
!*==========================================================================*/
! PUBLIC void enable_irq(irq_hook_t *hook)
! Enable an interrupt request line by clearing an 8259 bit.
! Equivalent C code for hook->irq < 8:
! if ((irq_actids[hook->irq] &= ˜hook->id) == 0)
! outb(INT_CTLMASK, inb(INT_CTLMASK) & ˜(1 << irq));

.align 16
_enable_irq:
push ebp
mov ebp, esp
pushf
cli
mov eax, 8(ebp) ! hook
mov ecx, 8(eax) ! irq
mov eax, 12(eax) ! id bit
748 File: kernel/klib386.s MINIX SOURCE CODE
not eax
and _irq_actids(ecx*4), eax ! clear this id bit
jnz en_done ! still masked by other handlers?
movb ah, ˜1
rolb ah, cl ! ah = ˜(1 << (irq % 8))
mov edx, INT_CTLMASK ! enable irq < 8 at the master 8259
cmpb cl, 8
jb 0f
mov edx, INT2_CTLMASK ! enable irq >= 8 at the slave 8259
0: inb dx
andb al, ah
outb dx ! clear bit at the 8259
en_done:popf
leave
ret


!*==========================================================================*
!* disable_irq *
!*==========================================================================*/
! PUBLIC int disable_irq(irq_hook_t *hook)
! Disable an interrupt request line by setting an 8259 bit.
! Equivalent C code for irq < 8:
! irq_actids[hook->irq] |= hook->id;
! outb(INT_CTLMASK, inb(INT_CTLMASK) | (1 << irq));
! Returns true iff the interrupt was not already disabled.

.align 16
_disable_irq:
push ebp
mov ebp, esp
pushf
cli
mov eax, 8(ebp) ! hook
mov ecx, 8(eax) ! irq
mov eax, 12(eax) ! id bit
or _irq_actids(ecx*4), eax ! set this id bit
movb ah, 1
rolb ah, cl ! ah = (1 << (irq % 8))
mov edx, INT_CTLMASK ! disable irq < 8 at the master 8259
cmpb cl, 8
jb 0f
mov edx, INT2_CTLMASK ! disable irq >= 8 at the slave 8259
0: inb dx
testb al, ah
jnz dis_already ! already disabled?
orb al, ah
outb dx ! set bit at the 8259
mov eax, 1 ! disabled by this function
popf
leave
ret
dis_already:
xor eax, eax ! already disabled
popf
leave
ret


MINIX SOURCE CODE File: kernel/klib386.s 749
!*===========================================================================*
!* phys_copy *
!*===========================================================================*
! PUBLIC void phys_copy(phys_bytes source, phys_bytes destination,
! phys_bytes bytecount);
! Copy a block of physical memory.

PC_ARGS = 4 + 4 + 4 + 4 ! 4 + 4 + 4
! es edi esi eip src dst len

.align 16
_phys_copy:
cld
push esi
push edi
push es

mov eax, FLAT_DS_SELECTOR
mov es, ax

mov esi, PC_ARGS(esp)
mov edi, PC_ARGS+4(esp)
mov eax, PC_ARGS+4+4(esp)

cmp eax, 10 ! avoid align overhead for small counts
jb pc_small
mov ecx, esi ! align source, hope target is too
neg ecx
and ecx, 3 ! count for alignment
sub eax, ecx
rep
eseg movsb
mov ecx, eax
shr ecx, 2 ! count of dwords
rep
eseg movs
and eax, 3
pc_small:
xchg ecx, eax ! remainder
rep
eseg movsb

pop es
pop edi
pop esi
ret

!*===========================================================================*
!* phys_memset *
!*===========================================================================*
! PUBLIC void phys_memset(phys_bytes source, unsigned long pattern,
! phys_bytes bytecount);
! Fill a block of physical memory with pattern.

.align 16
_phys_memset:
push ebp
mov ebp, esp
push esi
push ebx
750 File: kernel/klib386.s MINIX SOURCE CODE
push ds
mov esi, 8(ebp)
mov eax, 16(ebp)
mov ebx, FLAT_DS_SELECTOR
mov ds, bx
mov ebx, 12(ebp)
shr eax, 2
fill_start:
mov (esi), ebx
add esi, 4
dec eax
jnz fill_start
! Any remaining bytes?
mov eax, 16(ebp)
and eax, 3
remain_fill:
cmp eax, 0
jz fill_done
movb bl, 12(ebp)
movb (esi), bl
add esi, 1
inc ebp
dec eax
jmp remain_fill
fill_done:
pop ds
pop ebx
pop esi
pop ebp
ret

!*===========================================================================*
!* mem_rdw *
!*===========================================================================*
! PUBLIC u16_t mem_rdw(U16_t segment, u16_t *offset);
! Load and return word at far pointer segment:offset.

.align 16
_mem_rdw:
mov cx, ds
mov ds, 4(esp) ! segment
mov eax, 4+4(esp) ! offset
movzx eax, (eax) ! word to return
mov ds, cx
ret


!*===========================================================================*
!* reset *
!*===========================================================================*
! PUBLIC void reset();
! Reset the system by loading IDT with offset 0 and interrupting.

_reset:
lidt (idt_zero)
int 3 ! anything goes, the 386 will not like it
.sect .data
idt_zero: .data4 0, 0
.sect .text

MINIX SOURCE CODE File: kernel/klib386.s 751

!*===========================================================================*
!* idle_task *
!*===========================================================================*
_idle_task:
! This task is called when the system has nothing else to do. The HLT
! instruction puts the processor in a state where it draws minimum power.
push halt
call _level0 ! level0(halt)
pop eax
jmp _idle_task
halt:
sti
hlt
cli
ret

!*===========================================================================*
!* level0 *
!*===========================================================================*
! PUBLIC void level0(void (*func)(void))
! Call a function at permission level 0. This allows kernel tasks to do
! things that are only possible at the most privileged CPU level.
!
_level0:
mov eax, 4(esp)
mov (_level0_func), eax
int LEVEL0_VECTOR
ret


!*===========================================================================*
!* read_tsc *
!*===========================================================================*
! PUBLIC void read_tsc(unsigned long *high, unsigned long *low);
! Read the cycle counter of the CPU. Pentium and up.
.align 16
_read_tsc:
.data1 0x0f ! this is the RDTSC instruction
.data1 0x31 ! it places the TSC in EDX:EAX
push ebp
mov ebp, 8(esp)
mov (ebp), edx
mov ebp, 12(esp)
mov (ebp), eax
pop ebp
ret

!*===========================================================================*
!* read_flags *
!*===========================================================================*
! PUBLIC unsigned long read_cpu_flags(void);
! Read CPU status flags from C.
.align 16
_read_cpu_flags:
pushf
mov eax, (esp)
popf
ret

752 File: kernel/utility.c MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/utility.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This file contains a collection of miscellaneous procedures:
* panic: abort MINIX due to a fatal error
* kprintf: diagnostic output for the kernel
*
* Changes:
* Dec 10, 2004 kernel printing to circular buffer (Jorrit N. Herder)
*
* This file contains the routines that take care of kernel messages, i.e.,
* diagnostic output within the kernel. Kernel messages are not directly
* displayed on the console, because this must be done by the output driver.
* Instead, the kernel accumulates characters in a buffer and notifies the
* output driver when a new message is ready.
*/

#include <minix/com.h>
#include "kernel.h"
#include <stdarg.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <signal.h>
#include "proc.h"

#define END_OF_KMESS -1
FORWARD _PROTOTYPE(void kputc, (int c));

/*===========================================================================*
* panic *
*===========================================================================*/
PUBLIC void panic(mess,nr)
_CONST char *mess;
int nr;
{
/* The system has run aground of a fatal kernel error. Terminate execution. */
static int panicking = 0;
if (panicking ++) return; /* prevent recursive panics */

if (mess != NULL) {
kprintf("\nKernel panic: %s", mess);
if (nr != NO_NUM) kprintf(" %d", nr);
kprintf("\n",NO_NUM);
}

/* Abort MINIX. */
prepare_shutdown(RBT_PANIC);
}
/*===========================================================================*
* kprintf *
*===========================================================================*/
PUBLIC void kprintf(const char *fmt, ...) /* format to be printed */
{
int c; /* next character in fmt */
int d;
unsigned long u; /* hold number argument */
MINIX SOURCE CODE File: kernel/utility.c 753
int base; /* base of number arg */
int negative = 0; /* print minus sign */
static char x2c[] = "ABCDEF"; /* nr conversion table */
char ascii[8 * sizeof(long) / 3 + 2]; /* string for ascii number */
char *s = NULL; /* string to be printed */
va_list argp; /* optional arguments */

va_start(argp, fmt); /* init variable arguments */

while((c=*fmt++) != 0) {

if (c == ’%’) { /* expect format ’%key’ */
switch(c = *fmt++) { /* determine what to do */

/* Known keys are %d, %u, %x, %s, and %%. This is easily extended
* with number types like %b and %o by providing a different base.
* Number type keys don’t set a string to ’s’, but use the general
* conversion after the switch statement.
*/
case ’d’: /* output decimal */
d = va_arg(argp, signed int);
if (d < 0) { negative = 1; u = -d; } else { u = d; }
base = 10;
break;
case ’u’: /* output unsigned long */
u = va_arg(argp, unsigned long);
base = 10;
break;
case ’x’: /* output hexadecimal */
u = va_arg(argp, unsigned long);
base = 0x10;
break;
case ’s’: /* output string */
s = va_arg(argp, char *);
if (s == NULL) s = "(null)";
break;
case ’%’: /* output percent */
s = "%";
break;

/* Unrecognized key. */
default: /* echo back %key */
s = "%?";
s[1] = c; /* set unknown key */
}

/* Assume a number if no string is set. Convert to ascii. */
if (s == NULL) {
s = ascii + sizeof(ascii)-1;
*s = 0;
do { *--s = x2c[(u % base)]; } /* work backwards */
while ((u /= base) > 0);
}

/* This is where the actual output for format "%key" is done. */
if (negative) kputc(’-’); /* print sign if negative */
while(*s != 0) { kputc(*s++); } /* print string/ number */
s = NULL; /* reset for next round */
}
else {
754 File: kernel/utility.c MINIX SOURCE CODE
kputc(c); /* print and continue */
}
}
kputc(END_OF_KMESS); /* terminate output */
va_end(argp); /* end variable arguments */
}
/*===========================================================================*
* kputc *
*===========================================================================*/
PRIVATE void kputc(c)
int c; /* character to append */
{
/* Accumulate a single character for a kernel message. Send a notification
* to the output driver if an END_OF_KMESS is encountered.
*/
if (c != END_OF_KMESS) {
kmess.km_buf[kmess.km_next] = c; /* put normal char in buffer */
if (kmess.km_size < KMESS_BUF_SIZE)
kmess.km_size += 1;
kmess.km_next = (kmess.km_next + 1) % KMESS_BUF_SIZE;
} else {
send_sig(OUTPUT_PROC_NR, SIGKMESS);
}
}
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/system.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* Function prototypes for the system library.
* The implementation is contained in src/kernel/system/.
*
* The system library allows access to system services by doing a kernel call.
* Kernel calls are transformed into request messages to the SYS task that is
* responsible for handling the call. By convention, sys_call() is transformed
* into a message with type SYS_CALL that is handled in a function do_call().
*/

#ifndef SYSTEM_H
#define SYSTEM_H

/* Common includes for the system library. */
#include "kernel.h"
#include "proto.h"
#include "proc.h"

/* Default handler for unused kernel calls. */
_PROTOTYPE( int do_unused, (message *m_ptr) );
_PROTOTYPE( int do_exec, (message *m_ptr) );
_PROTOTYPE( int do_fork, (message *m_ptr) );
_PROTOTYPE( int do_newmap, (message *m_ptr) );
_PROTOTYPE( int do_exit, (message *m_ptr) );
_PROTOTYPE( int do_trace, (message *m_ptr) );
_PROTOTYPE( int do_nice, (message *m_ptr) );
MINIX SOURCE CODE File: kernel/system.h 755
_PROTOTYPE( int do_copy, (message *m_ptr) );
#define do_vircopy do_copy
#define do_physcopy do_copy
_PROTOTYPE( int do_vcopy, (message *m_ptr) );
#define do_virvcopy do_vcopy
#define do_physvcopy do_vcopy
_PROTOTYPE( int do_umap, (message *m_ptr) );
_PROTOTYPE( int do_memset, (message *m_ptr) );
_PROTOTYPE( int do_abort, (message *m_ptr) );
_PROTOTYPE( int do_getinfo, (message *m_ptr) );
_PROTOTYPE( int do_privctl, (message *m_ptr) );
_PROTOTYPE( int do_segctl, (message *m_ptr) );
_PROTOTYPE( int do_irqctl, (message *m_ptr) );
_PROTOTYPE( int do_devio, (message *m_ptr) );
_PROTOTYPE( int do_vdevio, (message *m_ptr) );
_PROTOTYPE( int do_int86, (message *m_ptr) );
_PROTOTYPE( int do_sdevio, (message *m_ptr) );
_PROTOTYPE( int do_kill, (message *m_ptr) );
_PROTOTYPE( int do_getksig, (message *m_ptr) );
_PROTOTYPE( int do_endksig, (message *m_ptr) );
_PROTOTYPE( int do_sigsend, (message *m_ptr) );
_PROTOTYPE( int do_sigreturn, (message *m_ptr) );
_PROTOTYPE( int do_times, (message *m_ptr) );
_PROTOTYPE( int do_setalarm, (message *m_ptr) );

#endif /* SYSTEM_H */



++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/system.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This task provides an interface between the kernel and user-space system
* processes. System services can be accessed by doing a kernel call. Kernel
* calls are transformed into request messages, which are handled by this
* task. By convention, a sys_call() is transformed in a SYS_CALL request
* message that is handled in a function named do_call().
*
* A private call vector is used to map all kernel calls to the functions that
* handle them. The actual handler functions are contained in separate files
* to keep this file clean. The call vector is used in the system task’s main
* loop to handle all incoming requests.
*
* In addition to the main sys_task() entry point, which starts the main loop,
* there are several other minor entry points:
* get_priv: assign privilege structure to user or system process
* send_sig: send a signal directly to a system process
* cause_sig: take action to cause a signal to occur via PM
* umap_local: map virtual address in LOCAL_SEG to physical
* umap_remote: map virtual address in REMOTE_SEG to physical
* umap_bios: map virtual address in BIOS_SEG to physical
* virtual_copy: copy bytes from one virtual address to another
* get_randomness: accumulate randomness in a buffer
*
* Changes:
* Aug 04, 2005 check if kernel call is allowed (Jorrit N. Herder)
* Jul 20, 2005 send signal to services with message (Jorrit N. Herder)
756 File: kernel/system.c MINIX SOURCE CODE
* Jan 15, 2005 new, generalized virtual copy function (Jorrit N. Herder)
* Oct 10, 2004 dispatch system calls from call vector (Jorrit N. Herder)
* Sep 30, 2004 source code documentation updated (Jorrit N. Herder)
*/

#include "kernel.h"
#include "system.h"
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/sigcontext.h>
#include <ibm/memory.h>
#include "protect.h"

/* Declaration of the call vector that defines the mapping of kernel calls
* to handler functions. The vector is initialized in sys_init() with map(),
* which makes sure the kernel call numbers are ok. No space is allocated,
* because the dummy is declared extern. If an illegal call is given, the
* array size will be negative and this won’t compile.
*/
PUBLIC int (*call_vec[NR_SYS_CALLS])(message *m_ptr);

#define map(call_nr, handler) \
{extern int dummy[NR_SYS_CALLS>(unsigned)(call_nr-KERNEL_CALL) ? 1:-1];} \
call_vec[(call_nr-KERNEL_CALL)] = (handler)

FORWARD _PROTOTYPE( void initialize, (void));

/*===========================================================================*
* sys_task *
*===========================================================================*/
PUBLIC void sys_task()
{
/* Main entry point of sys_task. Get the message and dispatch on type. */
static message m;
register int result;
register struct proc *caller_ptr;
unsigned int call_nr;
int s;

/* Initialize the system task. */
initialize();

while (TRUE) {
/* Get work. Block and wait until a request message arrives. */
receive(ANY, &m);
call_nr = (unsigned) m.m_type - KERNEL_CALL;
caller_ptr = proc_addr(m.m_source);

/* See if the caller made a valid request and try to handle it. */
if (! (priv(caller_ptr)->s_call_mask & (1<<call_nr))) {
kprintf("SYSTEM: request %d from %d denied.\n", call_nr,m.m_source);
result = ECALLDENIED; /* illegal message type */
} else if (call_nr >= NR_SYS_CALLS) { /* check call number */
kprintf("SYSTEM: illegal request %d from %d.\n", call_nr,m.m_source);
result = EBADREQUEST; /* illegal message type */
}
else {
result = (*call_vec[call_nr])(&m); /* handle the kernel call */
}
MINIX SOURCE CODE File: kernel/system.c 757

/* Send a reply, unless inhibited by a handler function. Use the kernel
* function lock_send() to prevent a system call trap. The destination
* is known to be blocked waiting for a message.
*/
if (result != EDONTREPLY) {
m.m_type = result; /* report status of call */
if (OK != (s=lock_send(m.m_source, &m))) {
kprintf("SYSTEM, reply to %d failed: %d\n", m.m_source, s);
}
}
}
}
/*===========================================================================*
* initialize *
*===========================================================================*/
PRIVATE void initialize(void)
{
register struct priv *sp;
int i;

/* Initialize IRQ handler hooks. Mark all hooks available. */
for (i=0; i<NR_IRQ_HOOKS; i++) {
irq_hooks[i].proc_nr = NONE;
}

/* Initialize all alarm timers for all processes. */
for (sp=BEG_PRIV_ADDR; sp < END_PRIV_ADDR; sp++) {
tmr_inittimer(&(sp->s_alarm_timer));
}

/* Initialize the call vector to a safe default handler. Some kernel calls
* may be disabled or nonexistant. Then explicitly map known calls to their
* handler functions. This is done with a macro that gives a compile error
* if an illegal call number is used. The ordering is not important here.
*/
for (i=0; i<NR_SYS_CALLS; i++) {
call_vec[i] = do_unused;
}

/* Process management. */
map(SYS_FORK, do_fork); /* a process forked a new process */
map(SYS_EXEC, do_exec); /* update process after execute */
map(SYS_EXIT, do_exit); /* clean up after process exit */
map(SYS_NICE, do_nice); /* set scheduling priority */
map(SYS_PRIVCTL, do_privctl); /* system privileges control */
map(SYS_TRACE, do_trace); /* request a trace operation */

/* Signal handling. */
map(SYS_KILL, do_kill); /* cause a process to be signaled */
map(SYS_GETKSIG, do_getksig); /* PM checks for pending signals */
map(SYS_ENDKSIG, do_endksig); /* PM finished processing signal */
map(SYS_SIGSEND, do_sigsend); /* start POSIX-style signal */
map(SYS_SIGRETURN, do_sigreturn); /* return from POSIX-style signal */

/* Device I/O. */
map(SYS_IRQCTL, do_irqctl); /* interrupt control operations */
map(SYS_DEVIO, do_devio); /* inb, inw, inl, outb, outw, outl */
map(SYS_SDEVIO, do_sdevio); /* phys_insb, _insw, _outsb, _outsw */
758 File: kernel/system.c MINIX SOURCE CODE
map(SYS_VDEVIO, do_vdevio); /* vector with devio requests */
map(SYS_INT86, do_int86); /* real-mode BIOS calls */

/* Memory management. */
map(SYS_NEWMAP, do_newmap); /* set up a process memory map */
map(SYS_SEGCTL, do_segctl); /* add segment and get selector */
map(SYS_MEMSET, do_memset); /* write char to memory area */

/* Copying. */
map(SYS_UMAP, do_umap); /* map virtual to physical address */
map(SYS_VIRCOPY, do_vircopy); /* use pure virtual addressing */
map(SYS_PHYSCOPY, do_physcopy); /* use physical addressing */
map(SYS_VIRVCOPY, do_virvcopy); /* vector with copy requests */
map(SYS_PHYSVCOPY, do_physvcopy); /* vector with copy requests */

/* Clock functionality. */
map(SYS_TIMES, do_times); /* get uptime and process times */
map(SYS_SETALARM, do_setalarm); /* schedule a synchronous alarm */

/* System control. */
map(SYS_ABORT, do_abort); /* abort MINIX */
map(SYS_GETINFO, do_getinfo); /* request system information */
}
/*===========================================================================*
* get_priv *
*===========================================================================*/
PUBLIC int get_priv(rc, proc_type)
register struct proc *rc; /* new (child) process pointer */
int proc_type; /* system or user process flag */
{
/* Get a privilege structure. All user processes share the same privilege
* structure. System processes get their own privilege structure.
*/
register struct priv *sp; /* privilege structure */

if (proc_type == SYS_PROC) { /* find a new slot */
for (sp = BEG_PRIV_ADDR; sp < END_PRIV_ADDR; ++sp)
if (sp->s_proc_nr == NONE && sp->s_id != USER_PRIV_ID) break;
if (sp->s_proc_nr != NONE) return(ENOSPC);
rc->p_priv = sp; /* assign new slot */
rc->p_priv->s_proc_nr = proc_nr(rc); /* set association */
rc->p_priv->s_flags = SYS_PROC; /* mark as privileged */
} else {
rc->p_priv = &priv[USER_PRIV_ID]; /* use shared slot */
rc->p_priv->s_proc_nr = INIT_PROC_NR; /* set association */
rc->p_priv->s_flags = 0; /* no initial flags */
}
return(OK);
}
/*===========================================================================*
* get_randomness *
*===========================================================================*/
PUBLIC void get_randomness(source)
int source;
{
/* On machines with the RDTSC (cycle counter read instruction - pentium
* and up), use that for high-resolution raw entropy gathering. Otherwise,
* use the realtime clock (tick resolution).
MINIX SOURCE CODE File: kernel/system.c 759
*
* Unfortunately this test is run-time - we don’t want to bother with
* compiling different kernels for different machines.
*
* On machines without RDTSC, we use read_clock().
*/
int r_next;
unsigned long tsc_high, tsc_low;

source %= RANDOM_SOURCES;
r_next= krandom.bin[source].r_next;
if (machine.processor > 486) {
read_tsc(&tsc_high, &tsc_low);
krandom.bin[source].r_buf[r_next] = tsc_low;
} else {
krandom.bin[source].r_buf[r_next] = read_clock();
}
if (krandom.bin[source].r_size < RANDOM_ELEMENTS) {
krandom.bin[source].r_size ++;
}
krandom.bin[source].r_next = (r_next + 1 ) % RANDOM_ELEMENTS;
}
/*===========================================================================*
* send_sig *
*===========================================================================*/
PUBLIC void send_sig(proc_nr, sig_nr)
int proc_nr; /* system process to be signalled */
int sig_nr; /* signal to be sent, 1 to _NSIG */
{
/* Notify a system process about a signal. This is straightforward. Simply
* set the signal that is to be delivered in the pending signals map and
* send a notification with source SYSTEM.
*/
register struct proc *rp;

rp = proc_addr(proc_nr);
sigaddset(&priv(rp)->s_sig_pending, sig_nr);
lock_notify(SYSTEM, proc_nr);
}
/*===========================================================================*
* cause_sig *
*===========================================================================*/
PUBLIC void cause_sig(proc_nr, sig_nr)
int proc_nr; /* process to be signalled */
int sig_nr; /* signal to be sent, 1 to _NSIG */
{
/* A system process wants to send a signal to a process. Examples are:
* - HARDWARE wanting to cause a SIGSEGV after a CPU exception
* - TTY wanting to cause SIGINT upon getting a DEL
* - FS wanting to cause SIGPIPE for a broken pipe
* Signals are handled by sending a message to PM. This function handles the
* signals and makes sure the PM gets them by sending a notification. The
* process being signaled is blocked while PM has not finished all signals
* for it.
* Race conditions between calls to this function and the system calls that
* process pending kernel signals cannot exist. Signal related functions are
* only called when a user process causes a CPU exception and from the kernel
* process level, which runs to completion.
760 File: kernel/system.c MINIX SOURCE CODE
*/
register struct proc *rp;

/* Check if the signal is already pending. Process it otherwise. */
rp = proc_addr(proc_nr);
if (! sigismember(&rp->p_pending, sig_nr)) {
sigaddset(&rp->p_pending, sig_nr);
if (! (rp->p_rts_flags & SIGNALED)) { /* other pending */
if (rp->p_rts_flags == 0) lock_dequeue(rp); /* make not ready */
rp->p_rts_flags |= SIGNALED | SIG_PENDING; /* update flags */
send_sig(PM_PROC_NR, SIGKSIG);
}
}
}
/*===========================================================================*
* umap_local *
*===========================================================================*/
PUBLIC phys_bytes umap_local(rp, seg, vir_addr, bytes)
register struct proc *rp; /* pointer to proc table entry for process */
int seg; /* T, D, or S segment */
vir_bytes vir_addr; /* virtual address in bytes within the seg */
vir_bytes bytes; /* # of bytes to be copied */
{
/* Calculate the physical memory address for a given virtual address. */
vir_clicks vc; /* the virtual address in clicks */
phys_bytes pa; /* intermediate variables as phys_bytes */
phys_bytes seg_base;

/* If ’seg’ is D it could really be S and vice versa. T really means T.
* If the virtual address falls in the gap, it causes a problem. On the
* 8088 it is probably a legal stack reference, since "stackfaults" are
* not detected by the hardware. On 8088s, the gap is called S and
* accepted, but on other machines it is called D and rejected.
* The Atari ST behaves like the 8088 in this respect.
*/

if (bytes <= 0) return( (phys_bytes) 0);
if (vir_addr + bytes <= vir_addr) return 0; /* overflow */
vc = (vir_addr + bytes - 1) >> CLICK_SHIFT; /* last click of data */

if (seg != T)
seg = (vc < rp->p_memmap[D].mem_vir + rp->p_memmap[D].mem_len ? D : S);

if ((vir_addr>>CLICK_SHIFT) >= rp->p_memmap[seg].mem_vir +
rp->p_memmap[seg].mem_len) return( (phys_bytes) 0 );

if (vc >= rp->p_memmap[seg].mem_vir +
rp->p_memmap[seg].mem_len) return( (phys_bytes) 0 );

seg_base = (phys_bytes) rp->p_memmap[seg].mem_phys;
seg_base = seg_base << CLICK_SHIFT; /* segment origin in bytes */
pa = (phys_bytes) vir_addr;
pa -= rp->p_memmap[seg].mem_vir << CLICK_SHIFT;
return(seg_base + pa);
}
MINIX SOURCE CODE File: kernel/system.c 761
/*===========================================================================*
* umap_remote *
*===========================================================================*/
PUBLIC phys_bytes umap_remote(rp, seg, vir_addr, bytes)
register struct proc *rp; /* pointer to proc table entry for process */
int seg; /* index of remote segment */
vir_bytes vir_addr; /* virtual address in bytes within the seg */
vir_bytes bytes; /* # of bytes to be copied */
{
/* Calculate the physical memory address for a given virtual address. */
struct far_mem *fm;

if (bytes <= 0) return( (phys_bytes) 0);
if (seg < 0 || seg >= NR_REMOTE_SEGS) return( (phys_bytes) 0);

fm = &rp->p_priv->s_farmem[seg];
if (! fm->in_use) return( (phys_bytes) 0);
if (vir_addr + bytes > fm->mem_len) return( (phys_bytes) 0);

return(fm->mem_phys + (phys_bytes) vir_addr);
}
/*===========================================================================*
* umap_bios *
*===========================================================================*/
PUBLIC phys_bytes umap_bios(rp, vir_addr, bytes)
register struct proc *rp; /* pointer to proc table entry for process */
vir_bytes vir_addr; /* virtual address in BIOS segment */
vir_bytes bytes; /* # of bytes to be copied */
{
/* Calculate the physical memory address at the BIOS. Note: currently, BIOS
* address zero (the first BIOS interrupt vector) is not considered as an
* error here, but since the physical address will be zero as well, the
* calling function will think an error occurred. This is not a problem,
* since no one uses the first BIOS interrupt vector.
*/

/* Check all acceptable ranges. */
if (vir_addr >= BIOS_MEM_BEGIN && vir_addr + bytes <= BIOS_MEM_END)
return (phys_bytes) vir_addr;
else if (vir_addr >= BASE_MEM_TOP && vir_addr + bytes <= UPPER_MEM_END)
return (phys_bytes) vir_addr;
kprintf("Warning, error in umap_bios, virtual address 0x%x\n", vir_addr);
return 0;
}
/*===========================================================================*
* virtual_copy *
*===========================================================================*/
PUBLIC int virtual_copy(src_addr, dst_addr, bytes)
struct vir_addr *src_addr; /* source virtual address */
struct vir_addr *dst_addr; /* destination virtual address */
vir_bytes bytes; /* # of bytes to copy */
{
/* Copy bytes from virtual address src_addr to virtual address dst_addr.
* Virtual addresses can be in ABS, LOCAL_SEG, REMOTE_SEG, or BIOS_SEG.
*/
struct vir_addr *vir_addr[2]; /* virtual source and destination address */
phys_bytes phys_addr[2]; /* absolute source and destination */
int seg_index;
762 File: kernel/system.c MINIX SOURCE CODE
int i;

/* Check copy count. */
if (bytes <= 0) return(EDOM);

/* Do some more checks and map virtual addresses to physical addresses. */
vir_addr[_SRC_] = src_addr;
vir_addr[_DST_] = dst_addr;
for (i=_SRC_; i<=_DST_; i++) {

/* Get physical address. */
switch((vir_addr[i]->segment & SEGMENT_TYPE)) {
case LOCAL_SEG:
seg_index = vir_addr[i]->segment & SEGMENT_INDEX;
phys_addr[i] = umap_local( proc_addr(vir_addr[i]->proc_nr),
seg_index, vir_addr[i]->offset, bytes );
break;
case REMOTE_SEG:
seg_index = vir_addr[i]->segment & SEGMENT_INDEX;
phys_addr[i] = umap_remote( proc_addr(vir_addr[i]->proc_nr),
seg_index, vir_addr[i]->offset, bytes );
break;
case BIOS_SEG:
phys_addr[i] = umap_bios( proc_addr(vir_addr[i]->proc_nr),
vir_addr[i]->offset, bytes );
break;
case PHYS_SEG:
phys_addr[i] = vir_addr[i]->offset;
break;
default:
return(EINVAL);
}

/* Check if mapping succeeded. */
if (phys_addr[i] <= 0 && vir_addr[i]->segment != PHYS_SEG)
return(EFAULT);
}

/* Now copy bytes between physical addresseses. */
phys_copy(phys_addr[_SRC_], phys_addr[_DST_], (phys_bytes) bytes);
return(OK);
}
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/system/do_setalarm.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The kernel call implemented in this file:
* m_type: SYS_SETALARM
*
* The parameters for this kernel call are:
* m2_l1: ALRM_EXP_TIME (alarm’s expiration time)
* m2_i2: ALRM_ABS_TIME (expiration time is absolute?)
* m2_l1: ALRM_TIME_LEFT (return seconds left of previous)
*/

#include "../system.h"
MINIX SOURCE CODE File: kernel/system/do_setalarm.c 763

#if USE_SETALARM

FORWARD _PROTOTYPE( void cause_alarm, (timer_t *tp) );

/*===========================================================================*
* do_setalarm *
*===========================================================================*/
PUBLIC int do_setalarm(m_ptr)
message *m_ptr; /* pointer to request message */
{
/* A process requests a synchronous alarm, or wants to cancel its alarm. */
register struct proc *rp; /* pointer to requesting process */
int proc_nr; /* which process wants the alarm */
long exp_time; /* expiration time for this alarm */
int use_abs_time; /* use absolute or relative time */
timer_t *tp; /* the process’ timer structure */
clock_t uptime; /* placeholder for current uptime */

/* Extract shared parameters from the request message. */
exp_time = m_ptr->ALRM_EXP_TIME; /* alarm’s expiration time */
use_abs_time = m_ptr->ALRM_ABS_TIME; /* flag for absolute time */
proc_nr = m_ptr->m_source; /* process to interrupt later */
rp = proc_addr(proc_nr);
if (! (priv(rp)->s_flags & SYS_PROC)) return(EPERM);

/* Get the timer structure and set the parameters for this alarm. */
tp = &(priv(rp)->s_alarm_timer);
tmr_arg(tp)->ta_int = proc_nr;
tp->tmr_func = cause_alarm;

/* Return the ticks left on the previous alarm. */
uptime = get_uptime();
if ((tp->tmr_exp_time != TMR_NEVER) && (uptime < tp->tmr_exp_time) ) {
m_ptr->ALRM_TIME_LEFT = (tp->tmr_exp_time - uptime);
} else {
m_ptr->ALRM_TIME_LEFT = 0;
}

/* Finally, (re)set the timer depending on the expiration time. */
if (exp_time == 0) {
reset_timer(tp);
} else {
tp->tmr_exp_time = (use_abs_time) ? exp_time : exp_time + get_uptime();
set_timer(tp, tp->tmr_exp_time, tp->tmr_func);
}
return(OK);
}
/*===========================================================================*
* cause_alarm *
*===========================================================================*/
PRIVATE void cause_alarm(tp)
timer_t *tp;
{
/* Routine called if a timer goes off and the process requested a synchronous
* alarm. The process number is stored in timer argument ’ta_int’. Notify that
* process with a notification message from CLOCK.
*/
int proc_nr = tmr_arg(tp)->ta_int; /* get process number */
764 File: kernel/system/do_setalarm.c MINIX SOURCE CODE
lock_notify(CLOCK, proc_nr); /* notify process */
}
#endif /* USE_SETALARM */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/system/do_exec.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The kernel call implemented in this file:
* m_type: SYS_EXEC
*
* The parameters for this kernel call are:
* m1_i1: PR_PROC_NR (process that did exec call)
* m1_p1: PR_STACK_PTR (new stack pointer)
* m1_p2: PR_NAME_PTR (pointer to program name)
* m1_p3: PR_IP_PTR (new instruction pointer)
*/
#include "../system.h"
#include <string.h>
#include <signal.h>

#if USE_EXEC

/*===========================================================================*
* do_exec *
*===========================================================================*/
PUBLIC int do_exec(m_ptr)
register message *m_ptr; /* pointer to request message */
{
/* Handle sys_exec(). A process has done a successful EXEC. Patch it up. */
register struct proc *rp;
reg_t sp; /* new sp */
phys_bytes phys_name;
char *np;

rp = proc_addr(m_ptr->PR_PROC_NR);
sp = (reg_t) m_ptr->PR_STACK_PTR;
rp->p_reg.sp = sp; /* set the stack pointer */
phys_memset(vir2phys(&rp->p_ldt[EXTRA_LDT_INDEX]), 0,
(LDT_SIZE - EXTRA_LDT_INDEX) * sizeof(rp->p_ldt[0]));
rp->p_reg.pc = (reg_t) m_ptr->PR_IP_PTR; /* set pc */
rp->p_rts_flags &= ˜RECEIVING; /* PM does not reply to EXEC call */
if (rp->p_rts_flags == 0) lock_enqueue(rp);

/* Save command name for debugging, ps(1) output, etc. */
phys_name = numap_local(m_ptr->m_source, (vir_bytes) m_ptr->PR_NAME_PTR,
(vir_bytes) P_NAME_LEN - 1);
if (phys_name != 0) {
phys_copy(phys_name, vir2phys(rp->p_name), (phys_bytes) P_NAME_LEN - 1);
for (np = rp->p_name; (*np & BYTE) >= ’ ’; np++) {}
*np = 0; /* mark end */
} else {
strncpy(rp->p_name, "<unset>", P_NAME_LEN);
}
return(OK);
}
#endif /* USE_EXEC */
MINIX SOURCE CODE File: kernel/clock.c 765
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kernel/clock.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This file contains the clock task, which handles time related functions.
* Important events that are handled by the CLOCK include setting and
* monitoring alarm timers and deciding when to (re)schedule processes.
* The CLOCK offers a direct interface to kernel processes. System services
* can access its services through system calls, such as sys_setalarm(). The
* CLOCK task thus is hidden from the outside world.
*
* Changes:
* Oct 08, 2005 reordering and comment editing (A. S. Woodhull)
* Mar 18, 2004 clock interface moved to SYSTEM task (Jorrit N. Herder)
* Sep 30, 2004 source code documentation updated (Jorrit N. Herder)
* Sep 24, 2004 redesigned alarm timers (Jorrit N. Herder)
*
* The function do_clocktick() is triggered by the clock’s interrupt
* handler when a watchdog timer has expired or a process must be scheduled.
*
* In addition to the main clock_task() entry point, which starts the main
* loop, there are several other minor entry points:
* clock_stop: called just before MINIX shutdown
* get_uptime: get realtime since boot in clock ticks
* set_timer: set a watchdog timer (+)
* reset_timer: reset a watchdog timer (+)
* read_clock: read the counter of channel 0 of the 8253A timer
*
* (+) The CLOCK task keeps tracks of watchdog timers for the entire kernel.
* The watchdog functions of expired timers are executed in do_clocktick().
* It is crucial that watchdog functions not block, or the CLOCK task may
* be blocked. Do not send() a message when the receiver is not expecting it.
* Instead, notify(), which always returns, should be used.
*/

#include "kernel.h"
#include "proc.h"
#include <signal.h>
#include <minix/com.h>

/* Function prototype for PRIVATE functions. */
FORWARD _PROTOTYPE( void init_clock, (void) );
FORWARD _PROTOTYPE( int clock_handler, (irq_hook_t *hook) );
FORWARD _PROTOTYPE( int do_clocktick, (message *m_ptr) );

/* Clock parameters. */
#define COUNTER_FREQ (2*TIMER_FREQ) /* counter frequency using square wave */
#define LATCH_COUNT 0x00 /* cc00xxxx, c = channel, x = any */
#define SQUARE_WAVE 0x36 /* ccaammmb, a = access, m = mode, b = BCD */
/* 11x11, 11 = LSB then MSB, x11 = sq wave */
#define TIMER_COUNT ((unsigned) (TIMER_FREQ/HZ)) /* initial value for counter*/
#define TIMER_FREQ 82L /* clock frequency for timer in PC and AT */

#define CLOCK_ACK_BIT 0x80 /* PS/2 clock interrupt acknowledge bit */

/* The CLOCK’s timers queue. The functions in <timers.h> operate on this.
* Each system process possesses a single synchronous alarm timer. If other
* kernel parts want to use additional timers, they must declare their own
* persistent (static) timer structure, which can be passed to the clock
766 File: kernel/clock.c MINIX SOURCE CODE
* via (re)set_timer().
* When a timer expires its watchdog function is run by the CLOCK task.
*/
PRIVATE timer_t *clock_timers; /* queue of CLOCK timers */
PRIVATE clock_t next_timeout; /* realtime that next timer expires */

/* The time is incremented by the interrupt handler on each clock tick. */
PRIVATE clock_t realtime; /* real time clock */
PRIVATE irq_hook_t clock_hook; /* interrupt handler hook */

/*===========================================================================*
* clock_task *
*===========================================================================*/
PUBLIC void clock_task()
{
/* Main program of clock task. If the call is not HARD_INT it is an error.
*/
message m; /* message buffer for both input and output */
int result; /* result returned by the handler */

init_clock(); /* initialize clock task */

/* Main loop of the clock task. Get work, process it. Never reply. */
while (TRUE) {

/* Go get a message. */
receive(ANY, &m);

/* Handle the request. Only clock ticks are expected. */
switch (m.m_type) {
case HARD_INT:
result = do_clocktick(&m); /* handle clock tick */
break;
default: /* illegal request type */
kprintf("CLOCK: illegal request %d from %d.\n", m.m_type,m.m_source);
}
}
}
/*===========================================================================*
* do_clocktick *
*===========================================================================*/
PRIVATE int do_clocktick(m_ptr)
message *m_ptr; /* pointer to request message */
{
/* Despite its name, this routine is not called on every clock tick. It
* is called on those clock ticks when a lot of work needs to be done.
*/

/* A process used up a full quantum. The interrupt handler stored this
* process in ’prev_ptr’. First make sure that the process is not on the
* scheduling queues. Then announce the process ready again. Since it has
* no more time left, it gets a new quantum and is inserted at the right
* place in the queues. As a side-effect a new process will be scheduled.
*/
if (prev_ptr->p_ticks_left <= 0 && priv(prev_ptr)->s_flags & PREEMPTIBLE) {
lock_dequeue(prev_ptr); /* take it off the queues */
lock_enqueue(prev_ptr); /* and reinsert it again */
}

MINIX SOURCE CODE File: kernel/clock.c 767
/* Check if a clock timer expired and run its watchdog function. */
if (next_timeout <= realtime) {
tmrs_exptimers(&clock_timers, realtime, NULL);
next_timeout = clock_timers == NULL ?
TMR_NEVER : clock_timers->tmr_exp_time;
}

/* Inhibit sending a reply. */
return(EDONTREPLY);
}
/*===========================================================================*
* init_clock *
*===========================================================================*/
PRIVATE void init_clock()
{
/* Initialize the CLOCK’s interrupt hook. */
clock_hook.proc_nr = CLOCK;

/* Initialize channel 0 of the 8253A timer to, e.g., 60 Hz. */
outb(TIMER_MODE, SQUARE_WAVE); /* set timer to run continuously */
outb(TIMER0, TIMER_COUNT); /* load timer low byte */
outb(TIMER0, TIMER_COUNT >> 8); /* load timer high byte */
put_irq_handler(&clock_hook, CLOCK_IRQ, clock_handler);/* register handler */
enable_irq(&clock_hook); /* ready for clock interrupts */
}
/*===========================================================================*
* clock_stop *
*===========================================================================*/
PUBLIC void clock_stop()
{
/* Reset the clock to the BIOS rate. (For rebooting) */
outb(TIMER_MODE, 0x36);
outb(TIMER0, 0);
outb(TIMER0, 0);
}
/*===========================================================================*
* clock_handler *
*===========================================================================*/
PRIVATE int clock_handler(hook)
irq_hook_t *hook;
{
/* This executes on each clock tick (i.e., every time the timer chip generates
* an interrupt). It does a little bit of work so the clock task does not have
* to be called on every tick. The clock task is called when:
*
* (1) the scheduling quantum of the running process has expired, or
* (2) a timer has expired and the watchdog function should be run.
*
* Many global global and static variables are accessed here. The safety of
* this must be justified. All scheduling and message passing code acquires a
* lock by temporarily disabling interrupts, so no conflicts with calls from
* the task level can occur. Furthermore, interrupts are not reentrant, the
* interrupt handler cannot be bothered by other interrupts.
*
* Variables that are updated in the clock’s interrupt handler:
* lost_ticks:
* Clock ticks counted outside the clock task. This for example
768 File: kernel/clock.c MINIX SOURCE CODE
* is used when the boot monitor processes a real mode interrupt.
* realtime:
* The current uptime is incremented with all outstanding ticks.
* proc_ptr, bill_ptr:
* These are used for accounting. It does not matter if proc.c
* is changing them, provided they are always valid pointers,
* since at worst the previous process would be billed.
*/
register unsigned ticks;

/* Acknowledge the PS/2 clock interrupt. */
if (machine.ps_mca) outb(PORT_B, inb(PORT_B) | CLOCK_ACK_BIT);

/* Get number of ticks and update realtime. */
ticks = lost_ticks + 1;
lost_ticks = 0;
realtime += ticks;

/* Update user and system accounting times. Charge the current process for
* user time. If the current process is not billable, that is, if a non-user
* process is running, charge the billable process for system time as well.
* Thus the unbillable process’ user time is the billable user’s system time.
*/
proc_ptr->p_user_time += ticks;
if (priv(proc_ptr)->s_flags & PREEMPTIBLE) {
proc_ptr->p_ticks_left -= ticks;
}
if (! (priv(proc_ptr)->s_flags & BILLABLE)) {
bill_ptr->p_sys_time += ticks;
bill_ptr->p_ticks_left -= ticks;
}

/* Check if do_clocktick() must be called. Done for alarms and scheduling.
* Some processes, such as the kernel tasks, cannot be preempted.
*/
if ((next_timeout <= realtime) || (proc_ptr->p_ticks_left <= 0)) {
prev_ptr = proc_ptr; /* store running process */
lock_notify(HARDWARE, CLOCK); /* send notification */
}
return(1); /* reenable interrupts */
}
/*===========================================================================*
* get_uptime *
*===========================================================================*/
PUBLIC clock_t get_uptime()
{
/* Get and return the current clock uptime in ticks. */
return(realtime);
}
/*===========================================================================*
* set_timer *
*===========================================================================*/
PUBLIC void set_timer(tp, exp_time, watchdog)
struct timer *tp; /* pointer to timer structure */
clock_t exp_time; /* expiration realtime */
tmr_func_t watchdog; /* watchdog to be called */
{
/* Insert the new timer in the active timers list. Always update the
MINIX SOURCE CODE File: kernel/clock.c 769
* next timeout time by setting it to the front of the active list.
*/
tmrs_settimer(&clock_timers, tp, exp_time, watchdog, NULL);
next_timeout = clock_timers->tmr_exp_time;
}
/*===========================================================================*
* reset_timer *
*===========================================================================*/
PUBLIC void reset_timer(tp)
struct timer *tp; /* pointer to timer structure */
{
/* The timer pointed to by ’tp’ is no longer needed. Remove it from both the
* active and expired lists. Always update the next timeout time by setting
* it to the front of the active list.
*/
tmrs_clrtimer(&clock_timers, tp, NULL);
next_timeout = (clock_timers == NULL) ?
TMR_NEVER : clock_timers->tmr_exp_time;
}
/*===========================================================================*
* read_clock *
*===========================================================================*/
PUBLIC unsigned long read_clock()
{
/* Read the counter of channel 0 of the 8253A timer. This counter counts
* down at a rate of TIMER_FREQ and restarts at TIMER_COUNT-1 when it
* reaches zero. A hardware interrupt (clock tick) occurs when the counter
* gets to zero and restarts its cycle.
*/
unsigned count;

outb(TIMER_MODE, LATCH_COUNT);
count = inb(TIMER0);
count |= (inb(TIMER0) << 8);

return count;
}