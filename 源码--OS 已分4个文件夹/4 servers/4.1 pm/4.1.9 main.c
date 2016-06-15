MINIX SOURCE CODE File: servers/pm/main.c 875
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/pm/main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
18000 /* This file contains the main program of the process manager and some related
18001 * procedures. When MINIX starts up, the kernel runs for a little while,
18002 * initializing itself and its tasks, and then it runs PM and FS. Both PM
18003 * and FS initialize themselves as far as they can. PM asks the kernel for
18004 * all free memory and starts serving requests.
18005 *
18006 * The entry points into this file are:
18007 * main: starts PM running
18008 * setreply: set the reply to be sent to process making an PM system call
18009 */
18010
18011 #include "pm.h"
18012 #include <minix/keymap.h>
18013 #include <minix/callnr.h>
18014 #include <minix/com.h>
18015 #include <signal.h>
18016 #include <stdlib.h>
18017 #include <fcntl.h>
18018 #include <sys/resource.h>
18019 #include <string.h>
18020 #include "mproc.h"
18021 #include "param.h"
18022
18023 #include "../../kernel/const.h"
18024 #include "../../kernel/config.h"
18025 #include "../../kernel/type.h"
18026 #include "../../kernel/proc.h"
18027
18028 FORWARD _PROTOTYPE( void get_work, (void) );
18029 FORWARD _PROTOTYPE( void pm_init, (void) );
18030 FORWARD _PROTOTYPE( int get_nice_value, (int queue) );
18031 FORWARD _PROTOTYPE( void get_mem_chunks, (struct memory *mem_chunks) );
18032 FORWARD _PROTOTYPE( void patch_mem_chunks, (struct memory *mem_chunks,
18033 struct mem_map *map_ptr) );
18034
18035 #define click_to_round_k(n) \
18036 ((unsigned) ((((unsigned long) (n) << CLICK_SHIFT) + 512) / 1024))
18037
18038 /*===========================================================================*
18039 * main *
18040 *===========================================================================*/
18041 PUBLIC int main()
18042 {
18043 /* Main routine of the process manager. */
18044 int result, s, proc_nr;
18045 struct mproc *rmp;
18046 sigset_t sigset;
18047
18048 pm_init(); /* initialize process manager tables */
18049
18050 /* This is PM’s main loop- get work and do it, forever and forever. */
18051 while (TRUE) {
18052 get_work(); /* wait for an PM system call */
18053
18054 /* Check for system notifications first. Special cases. */
876 File: servers/pm/main.c MINIX SOURCE CODE
18055 if (call_nr == SYN_ALARM) {
18056 pm_expire_timers(m_in.NOTIFY_TIMESTAMP);
18057 result = SUSPEND; /* don’t reply */
18058 } else if (call_nr == SYS_SIG) { /* signals pending */
18059 sigset = m_in.NOTIFY_ARG;
18060 if (sigismember(&sigset, SIGKSIG)) (void) ksig_pending();
18061 result = SUSPEND; /* don’t reply */
18062 }
18063 /* Else, if the system call number is valid, perform the call. */
18064 else if ((unsigned) call_nr >= NCALLS) {
18065 result = ENOSYS;
18066 } else {
18067 result = (*call_vec[call_nr])();
18068 }
18069
18070 /* Send the results back to the user to indicate completion. */
18071 if (result != SUSPEND) setreply(who, result);
18072
18073 swap_in(); /* maybe a process can be swapped in? */
18074
18075 /* Send out all pending reply messages, including the answer to
18076 * the call just made above. The processes must not be swapped out.
18077 */
18078 for (proc_nr=0, rmp=mproc; proc_nr < NR_PROCS; proc_nr++, rmp++) {
18079 /* In the meantime, the process may have been killed by a
18080 * signal (e.g. if a lethal pending signal was unblocked)
18081 * without the PM realizing it. If the slot is no longer in
18082 * use or just a zombie, don’t try to reply.
18083 */
18084 if ((rmp->mp_flags & (REPLY | ONSWAP | IN_USE | ZOMBIE)) ==
18085 (REPLY | IN_USE)) {
18086 if ((s=send(proc_nr, &rmp->mp_reply)) != OK) {
18087 panic(__FILE__,"PM can’t reply to", proc_nr);
18088 }
18089 rmp->mp_flags &= ˜REPLY;
18090 }
18091 }
18092 }
18093 return(OK);
18094 }
18096 /*===========================================================================*
18097 * get_work *
18098 *===========================================================================*/
18099 PRIVATE void get_work()
18100 {
18101 /* Wait for the next message and extract useful information from it. */
18102 if (receive(ANY, &m_in) != OK) panic(__FILE__,"PM receive error", NO_NUM);
18103 who = m_in.m_source; /* who sent the message */
18104 call_nr = m_in.m_type; /* system call number */
18105
18106 /* Process slot of caller. Misuse PM’s own process slot if the kernel is
18107 * calling. This can happen in case of synchronous alarms (CLOCK) or or
18108 * event like pending kernel signals (SYSTEM).
18109 */
18110 mp = &mproc[who < 0 ? PM_PROC_NR : who];
18111 }
MINIX SOURCE CODE File: servers/pm/main.c 877
18113 /*===========================================================================*
18114 * setreply *
18115 *===========================================================================*/
18116 PUBLIC void setreply(proc_nr, result)
18117 int proc_nr; /* process to reply to */
18118 int result; /* result of call (usually OK or error #) */
18119 {
18120 /* Fill in a reply message to be sent later to a user process. System calls
18121 * may occasionally fill in other fields, this is only for the main return
18122 * value, and for setting the "must send reply" flag.
18123 */
18124 register struct mproc *rmp = &mproc[proc_nr];
18125
18126 rmp->mp_reply.reply_res = result;
18127 rmp->mp_flags |= REPLY; /* reply pending */
18128
18129 if (rmp->mp_flags & ONSWAP)
18130 swap_inqueue(rmp); /* must swap this process back in */
18131 }
18133 /*===========================================================================*
18134 * pm_init *
18135 *===========================================================================*/
18136 PRIVATE void pm_init()
18137 {
18138 /* Initialize the process manager.
18139 * Memory use info is collected from the boot monitor, the kernel, and
18140 * all processes compiled into the system image. Initially this information
18141 * is put into an array mem_chunks. Elements of mem_chunks are struct memory,
18142 * and hold base, size pairs in units of clicks. This array is small, there
18143 * should be no more than 8 chunks. After the array of chunks has been built
18144 * the contents are used to initialize the hole list. Space for the hole list
18145 * is reserved as an array with twice as many elements as the maximum number
18146 * of processes allowed. It is managed as a linked list, and elements of the
18147 * array are struct hole, which, in addition to storage for a base and size in
18148 * click units also contain space for a link, a pointer to another element.
18149 */
18150 int s;
18151 static struct boot_image image[NR_BOOT_PROCS];
18152 register struct boot_image *ip;
18153 static char core_sigs[] = { SIGQUIT, SIGILL, SIGTRAP, SIGABRT,
18154 SIGEMT, SIGFPE, SIGUSR1, SIGSEGV, SIGUSR2 };
18155 static char ign_sigs[] = { SIGCHLD };
18156 register struct mproc *rmp;
18157 register char *sig_ptr;
18158 phys_clicks total_clicks, minix_clicks, free_clicks;
18159 message mess;
18160 struct mem_map mem_map[NR_LOCAL_SEGS];
18161 struct memory mem_chunks[NR_MEMS];
18162
18163 /* Initialize process table, including timers. */
18164 for (rmp=&mproc[0]; rmp<&mproc[NR_PROCS]; rmp++) {
18165 tmr_inittimer(&rmp->mp_timer);
18166 }
18167
18168 /* Build the set of signals which cause core dumps, and the set of signals
18169 * that are by default ignored.
18170 */
18171 sigemptyset(&core_sset);
18172 for (sig_ptr = core_sigs; sig_ptr < core_sigs+sizeof(core_sigs); sig_ptr++)
878 File: servers/pm/main.c MINIX SOURCE CODE
18173 sigaddset(&core_sset, *sig_ptr);
18174 sigemptyset(&ign_sset);
18175 for (sig_ptr = ign_sigs; sig_ptr < ign_sigs+sizeof(ign_sigs); sig_ptr++)
18176 sigaddset(&ign_sset, *sig_ptr);
18177
18178 /* Obtain a copy of the boot monitor parameters and the kernel info struct.
18179 * Parse the list of free memory chunks. This list is what the boot monitor
18180 * reported, but it must be corrected for the kernel and system processes.
18181 */
18182 if ((s=sys_getmonparams(monitor_params, sizeof(monitor_params))) != OK)
18183 panic(__FILE__,"get monitor params failed",s);
18184 get_mem_chunks(mem_chunks);
18185 if ((s=sys_getkinfo(&kinfo)) != OK)
18186 panic(__FILE__,"get kernel info failed",s);
18187
18188 /* Get the memory map of the kernel to see how much memory it uses. */
18189 if ((s=get_mem_map(SYSTASK, mem_map)) != OK)
18190 panic(__FILE__,"couldn’t get memory map of SYSTASK",s);
18191 minix_clicks = (mem_map[S].mem_phys+mem_map[S].mem_len)-mem_map[T].mem_phys;
18192 patch_mem_chunks(mem_chunks, mem_map);
18193
18194 /* Initialize PM’s process table. Request a copy of the system image table
18195 * that is defined at the kernel level to see which slots to fill in.
18196 */
18197 if (OK != (s=sys_getimage(image)))
18198 panic(__FILE__,"couldn’t get image table: %d\n", s);
18199 procs_in_use = 0; /* start populating table */
18200 printf("Building process table:"); /* show what’s happening */
18201 for (ip = &image[0]; ip < &image[NR_BOOT_PROCS]; ip++) {
18202 if (ip->proc_nr >= 0) { /* task have negative nrs */
18203 procs_in_use += 1; /* found user process */
18204
18205 /* Set process details found in the image table. */
18206 rmp = &mproc[ip->proc_nr];
18207 strncpy(rmp->mp_name, ip->proc_name, PROC_NAME_LEN);
18208 rmp->mp_parent = RS_PROC_NR;
18209 rmp->mp_nice = get_nice_value(ip->priority);
18210 if (ip->proc_nr == INIT_PROC_NR) { /* user process */
18211 rmp->mp_pid = INIT_PID;
18212 rmp->mp_flags |= IN_USE;
18213 sigemptyset(&rmp->mp_ignore);
18214 }
18215 else { /* system process */
18216 rmp->mp_pid = get_free_pid();
18217 rmp->mp_flags |= IN_USE | DONT_SWAP | PRIV_PROC;
18218 sigfillset(&rmp->mp_ignore);
18219 }
18220 sigemptyset(&rmp->mp_sigmask);
18221 sigemptyset(&rmp->mp_catch);
18222 sigemptyset(&rmp->mp_sig2mess);
18223
18224 /* Get memory map for this process from the kernel. */
18225 if ((s=get_mem_map(ip->proc_nr, rmp->mp_seg)) != OK)
18226 panic(__FILE__,"couldn’t get process entry",s);
18227 if (rmp->mp_seg[T].mem_len != 0) rmp->mp_flags |= SEPARATE;
18228 minix_clicks += rmp->mp_seg[S].mem_phys +
18229 rmp->mp_seg[S].mem_len - rmp->mp_seg[T].mem_phys;
18230 patch_mem_chunks(mem_chunks, rmp->mp_seg);
18231
18232 /* Tell FS about this system process. */
MINIX SOURCE CODE File: servers/pm/main.c 879
18233 mess.PR_PROC_NR = ip->proc_nr;
18234 mess.PR_PID = rmp->mp_pid;
18235 if (OK != (s=send(FS_PROC_NR, &mess)))
18236 panic(__FILE__,"can’t sync up with FS", s);
18237 printf(" %s", ip->proc_name); /* display process name */
18238 }
18239 }
18240 printf(".\n"); /* last process done */
18241
18242 /* Override some details. PM is somewhat special. */
18243 mproc[PM_PROC_NR].mp_pid = PM_PID; /* magically override pid */
18244 mproc[PM_PROC_NR].mp_parent = PM_PROC_NR; /* PM doesn’t have parent */
18245
18246 /* Tell FS that no more system processes follow and synchronize. */
18247 mess.PR_PROC_NR = NONE;
18248 if (sendrec(FS_PROC_NR, &mess) != OK || mess.m_type != OK)
18249 panic(__FILE__,"can’t sync up with FS", NO_NUM);
18250
18251 /* Initialize tables to all physical memory and print memory information. */
18252 printf("Physical memory:");
18253 mem_init(mem_chunks, &free_clicks);
18254 total_clicks = minix_clicks + free_clicks;
18255 printf(" total %u KB,", click_to_round_k(total_clicks));
18256 printf(" system %u KB,", click_to_round_k(minix_clicks));
18257 printf(" free %u KB.\n", click_to_round_k(free_clicks));
18258 }
18260 /*===========================================================================*
18261 * get_nice_value *
18262 *===========================================================================*/
18263 PRIVATE int get_nice_value(queue)
18264 int queue; /* store mem chunks here */
18265 {
18266 /* Processes in the boot image have a priority assigned. The PM doesn’t know
18267 * about priorities, but uses ’nice’ values instead. The priority is between
18268 * MIN_USER_Q and MAX_USER_Q. We have to scale between PRIO_MIN and PRIO_MAX.
18269 */
18270 int nice_val = (queue - USER_Q) * (PRIO_MAX-PRIO_MIN+1) /
18271 (MIN_USER_Q-MAX_USER_Q+1);
18272 if (nice_val > PRIO_MAX) nice_val = PRIO_MAX; /* shouldn’t happen */
18273 if (nice_val < PRIO_MIN) nice_val = PRIO_MIN; /* shouldn’t happen */
18274 return nice_val;
18275 }
18277 /*===========================================================================*
18278 * get_mem_chunks *
18279 *===========================================================================*/
18280 PRIVATE void get_mem_chunks(mem_chunks)
18281 struct memory *mem_chunks; /* store mem chunks here */
18282 {
18283 /* Initialize the free memory list from the ’memory’ boot variable. Translate
18284 * the byte offsets and sizes in this list to clicks, properly truncated. Also
18285 * make sure that we don’t exceed the maximum address space of the 286 or the
18286 * 8086, i.e. when running in 16-bit protected mode or real mode.
18287 */
18288 long base, size, limit;
18289 char *s, *end; /* use to parse boot variable */
18290 int i, done = 0;
18291 struct memory *memp;
18292
880 File: servers/pm/main.c MINIX SOURCE CODE
18293 /* Initialize everything to zero. */
18294 for (i = 0; i < NR_MEMS; i++) {
18295 memp = &mem_chunks[i]; /* next mem chunk is stored here */
18296 memp->base = memp->size = 0;
18297 }
18298
18299 /* The available memory is determined by MINIX’ boot loader as a list of
18300 * (base:size)-pairs in boothead.s. The ’memory’ boot variable is set in
18301 * in boot.s. The format is "b0:s0,b1:s1,b2:s2", where b0:s0 is low mem,
18302 * b1:s1 is mem between 1M and 16M, b2:s2 is mem above 16M. Pairs b1:s1
18303 * and b2:s2 are combined if the memory is adjacent.
18304 */
18305 s = find_param("memory"); /* get memory boot variable */
18306 for (i = 0; i < NR_MEMS && !done; i++) {
18307 memp = &mem_chunks[i]; /* next mem chunk is stored here */
18308 base = size = 0; /* initialize next base:size pair */
18309 if (*s != 0) { /* get fresh data, unless at end */
18310
18311 /* Read fresh base and expect colon as next char. */
18312 base = strtoul(s, &end, 0x10); /* get number */
18313 if (end != s && *end == ’:’) s = ++end; /* skip ’:’ */
18314 else *s=0; /* terminate, should not happen */
18315
18316 /* Read fresh size and expect comma or assume end. */
18317 size = strtoul(s, &end, 0x10); /* get number */
18318 if (end != s && *end == ’,’) s = ++end; /* skip ’,’ */
18319 else done = 1;
18320 }
18321 limit = base + size;
18322 base = (base + CLICK_SIZE-1) & ˜(long)(CLICK_SIZE-1);
18323 limit &= ˜(long)(CLICK_SIZE-1);
18324 if (limit <= base) continue;
18325 memp->base = base >> CLICK_SHIFT;
18326 memp->size = (limit - base) >> CLICK_SHIFT;
18327 }
18328 }
18330 /*===========================================================================*
18331 * patch_mem_chunks *
18332 *===========================================================================*/
18333 PRIVATE void patch_mem_chunks(mem_chunks, map_ptr)
18334 struct memory *mem_chunks; /* store mem chunks here */
18335 struct mem_map *map_ptr; /* memory to remove */
18336 {
18337 /* Remove server memory from the free memory list. The boot monitor
18338 * promises to put processes at the start of memory chunks. The
18339 * tasks all use same base address, so only the first task changes
18340 * the memory lists. The servers and init have their own memory
18341 * spaces and their memory will be removed from the list.
18342 */
18343 struct memory *memp;
18344 for (memp = mem_chunks; memp < &mem_chunks[NR_MEMS]; memp++) {
18345 if (memp->base == map_ptr[T].mem_phys) {
18346 memp->base += map_ptr[T].mem_len + map_ptr[D].mem_len;
18347 memp->size -= map_ptr[T].mem_len + map_ptr[D].mem_len;
18348 }
18349 }
18350 }