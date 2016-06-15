++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/pm/break.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
19300 /* The MINIX model of memory allocation reserves a fixed amount of memory for
19301 * the combined text, data, and stack segments. The amount used for a child
19302 * process created by FORK is the same as the parent had. If the child does
19303 * an EXEC later, the new size is taken from the header of the file EXEC’ed.
19304 *
19305 * The layout in memory consists of the text segment, followed by the data
19306 * segment, followed by a gap (unused memory), followed by the stack segment.
19307 * The data segment grows upward and the stack grows downward, so each can
19308 * take memory from the gap. If they meet, the process must be killed. The
19309 * procedures in this file deal with the growth of the data and stack segments.
19310 *
19311 * The entry points into this file are:
19312 * do_brk: BRK/SBRK system calls to grow or shrink the data segment
19313 * adjust: see if a proposed segment adjustment is allowed
19314 * size_ok: see if the segment sizes are feasible
19315 */
19316
19317 #include "pm.h"
19318 #include <signal.h>
19319 #include "mproc.h"
896 File: servers/pm/break.c MINIX SOURCE CODE
19320 #include "param.h"
19321
19322 #define DATA_CHANGED 1 /* flag value when data segment size changed */
19323 #define STACK_CHANGED 2 /* flag value when stack size changed */
19324
19325 /*===========================================================================*
19326 * do_brk *
19327 *===========================================================================*/
19328 PUBLIC int do_brk()
19329 {
19330 /* Perform the brk(addr) system call.
19331 *
19332 * The call is complicated by the fact that on some machines (e.g., 8088),
19333 * the stack pointer can grow beyond the base of the stack segment without
19334 * anybody noticing it.
19335 * The parameter, ’addr’ is the new virtual address in D space.
19336 */
19337
19338 register struct mproc *rmp;
19339 int r;
19340 vir_bytes v, new_sp;
19341 vir_clicks new_clicks;
19342
19343 rmp = mp;
19344 v = (vir_bytes) m_in.addr;
19345 new_clicks = (vir_clicks) ( ((long) v + CLICK_SIZE - 1) >> CLICK_SHIFT);
19346 if (new_clicks < rmp->mp_seg[D].mem_vir) {
19347 rmp->mp_reply.reply_ptr = (char *) -1;
19348 return(ENOMEM);
19349 }
19350 new_clicks -= rmp->mp_seg[D].mem_vir;
19351 if ((r=get_stack_ptr(who, &new_sp)) != OK) /* ask kernel for sp value */
19352 panic(__FILE__,"couldn’t get stack pointer", r);
19353 r = adjust(rmp, new_clicks, new_sp);
19354 rmp->mp_reply.reply_ptr = (r == OK ? m_in.addr : (char *) -1);
19355 return(r); /* return new address or -1 */
19356 }
19358 /*===========================================================================*
19359 * adjust *
19360 *===========================================================================*/
19361 PUBLIC int adjust(rmp, data_clicks, sp)
19362 register struct mproc *rmp; /* whose memory is being adjusted? */
19363 vir_clicks data_clicks; /* how big is data segment to become? */
19364 vir_bytes sp; /* new value of sp */
19365 {
19366 /* See if data and stack segments can coexist, adjusting them if need be.
19367 * Memory is never allocated or freed. Instead it is added or removed from the
19368 * gap between data segment and stack segment. If the gap size becomes
19369 * negative, the adjustment of data or stack fails and ENOMEM is returned.
19370 */
19371
19372 register struct mem_map *mem_sp, *mem_dp;
19373 vir_clicks sp_click, gap_base, lower, old_clicks;
19374 int changed, r, ft;
19375 long base_of_stack, delta; /* longs avoid certain problems */
19376
19377 mem_dp = &rmp->mp_seg[D]; /* pointer to data segment map */
19378 mem_sp = &rmp->mp_seg[S]; /* pointer to stack segment map */
19379 changed = 0; /* set when either segment changed */
MINIX SOURCE CODE File: servers/pm/break.c 897
19380
19381 if (mem_sp->mem_len == 0) return(OK); /* don’t bother init */
19382
19383 /* See if stack size has gone negative (i.e., sp too close to 0xFFFF...) */
19384 base_of_stack = (long) mem_sp->mem_vir + (long) mem_sp->mem_len;
19385 sp_click = sp >> CLICK_SHIFT; /* click containing sp */
19386 if (sp_click >= base_of_stack) return(ENOMEM); /* sp too high */
19387
19388 /* Compute size of gap between stack and data segments. */
19389 delta = (long) mem_sp->mem_vir - (long) sp_click;
19390 lower = (delta > 0 ? sp_click : mem_sp->mem_vir);
19391
19392 /* Add a safety margin for future stack growth. Impossible to do right. */
19393 #define SAFETY_BYTES (384 * sizeof(char *))
19394 #define SAFETY_CLICKS ((SAFETY_BYTES + CLICK_SIZE - 1) / CLICK_SIZE)
19395 gap_base = mem_dp->mem_vir + data_clicks + SAFETY_CLICKS;
19396 if (lower < gap_base) return(ENOMEM); /* data and stack collided */
19397
19398 /* Update data length (but not data orgin) on behalf of brk() system call. */
19399 old_clicks = mem_dp->mem_len;
19400 if (data_clicks != mem_dp->mem_len) {
19401 mem_dp->mem_len = data_clicks;
19402 changed |= DATA_CHANGED;
19403 }
19404
19405 /* Update stack length and origin due to change in stack pointer. */
19406 if (delta > 0) {
19407 mem_sp->mem_vir -= delta;
19408 mem_sp->mem_phys -= delta;
19409 mem_sp->mem_len += delta;
19410 changed |= STACK_CHANGED;
19411 }
19412
19413 /* Do the new data and stack segment sizes fit in the address space? */
19414 ft = (rmp->mp_flags & SEPARATE);
19415 r = (rmp->mp_seg[D].mem_vir + rmp->mp_seg[D].mem_len >
19416 rmp->mp_seg[S].mem_vir) ? ENOMEM : OK;
19417 if (r == OK) {
19418 if (changed) sys_newmap((int)(rmp - mproc), rmp->mp_seg);
19419 return(OK);
19420 }
19421
19422 /* New sizes don’t fit or require too many page/segment registers. Restore.*/
19423 if (changed & DATA_CHANGED) mem_dp->mem_len = old_clicks;
19424 if (changed & STACK_CHANGED) {
19425 mem_sp->mem_vir += delta;
19426 mem_sp->mem_phys += delta;
19427 mem_sp->mem_len -= delta;
19428 }
19429 return(ENOMEM);
19430 }