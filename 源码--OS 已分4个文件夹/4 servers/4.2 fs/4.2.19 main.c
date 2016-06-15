++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
24000 /* This file contains the main program of the File System. It consists of
24001 * a loop that gets messages requesting work, carries out the work, and sends
24002 * replies.
24003 *
24004 * The entry points into this file are:
24005 * main: main program of the File System
24006 * reply: send a reply to a process after the requested work is done
24007 *
24008 */
24009
24010 struct super_block; /* proto.h needs to know this */
24011
24012 #include "fs.h"
24013 #include <fcntl.h>
24014 #include <string.h>
24015 #include <stdio.h>
24016 #include <signal.h>
24017 #include <stdlib.h>
24018 #include <sys/ioc_memory.h>
24019 #include <sys/svrctl.h>
24020 #include <minix/callnr.h>
24021 #include <minix/com.h>
24022 #include <minix/keymap.h>
24023 #include <minix/const.h>
24024 #include "buf.h"
24025 #include "file.h"
24026 #include "fproc.h"
24027 #include "inode.h"
24028 #include "param.h"
24029 #include "super.h"
958 File: servers/fs/main.c MINIX SOURCE CODE
24030
24031 FORWARD _PROTOTYPE( void fs_init, (void) );
24032 FORWARD _PROTOTYPE( int igetenv, (char *var, int optional) );
24033 FORWARD _PROTOTYPE( void get_work, (void) );
24034 FORWARD _PROTOTYPE( void load_ram, (void) );
24035 FORWARD _PROTOTYPE( void load_super, (Dev_t super_dev) );
24036
24037 /*===========================================================================*
24038 * main *
24039 *===========================================================================*/
24040 PUBLIC int main()
24041 {
24042 /* This is the main program of the file system. The main loop consists of
24043 * three major activities: getting new work, processing the work, and sending
24044 * the reply. This loop never terminates as long as the file system runs.
24045 */
24046 sigset_t sigset;
24047 int error;
24048
24049 fs_init();
24050
24051 /* This is the main loop that gets work, processes it, and sends replies. */
24052 while (TRUE) {
24053 get_work(); /* sets who and call_nr */
24054
24055 fp = &fproc[who]; /* pointer to proc table struct */
24056 super_user = (fp->fp_effuid == SU_UID ? TRUE : FALSE); /* su? */
24057
24058 /* Check for special control messages first. */
24059 if (call_nr == SYS_SIG) {
24060 sigset = m_in.NOTIFY_ARG;
24061 if (sigismember(&sigset, SIGKSTOP)) {
24062 do_sync();
24063 sys_exit(0); /* never returns */
24064 }
24065 } else if (call_nr == SYN_ALARM) {
24066 /* Not a user request; system has expired one of our timers,
24067 * currently only in use for select(). Check it.
24068 */
24069 fs_expire_timers(m_in.NOTIFY_TIMESTAMP);
24070 } else if ((call_nr & NOTIFY_MESSAGE)) {
24071 /* Device notifies us of an event. */
24072 dev_status(&m_in);
24073 } else {
24074 /* Call the internal function that does the work. */
24075 if (call_nr < 0 || call_nr >= NCALLS) {
24076 error = ENOSYS;
24077 printf("FS,warning illegal %d system call by %d\n",call_nr,who);
24078 } else if (fp->fp_pid == PID_FREE) {
24079 error = ENOSYS;
24080 printf("FS, bad process, who = %d, call_nr = %d, slot1 = %d\n",
24081 who, call_nr, m_in.slot1);
24082 } else {
24083 error = (*call_vec[call_nr])();
24084 }
24085
24086 /* Copy the results back to the user and send reply. */
24087 if (error != SUSPEND) { reply(who, error); }
24088 if (rdahed_inode != NIL_INODE) {
24089 read_ahead(); /* do block read ahead */
MINIX SOURCE CODE File: servers/fs/main.c 959
24090 }
24091 }
24092 }
24093 return(OK); /* shouldn’t come here */
24094 }
24096 /*===========================================================================*
24097 * get_work *
24098 *===========================================================================*/
24099 PRIVATE void get_work()
24100 {
24101 /* Normally wait for new input. However, if ’reviving’ is
24102 * nonzero, a suspended process must be awakened.
24103 */
24104 register struct fproc *rp;
24105
24106 if (reviving != 0) {
24107 /* Revive a suspended process. */
24108 for (rp = &fproc[0]; rp < &fproc[NR_PROCS]; rp++)
24109 if (rp->fp_revived == REVIVING) {
24110 who = (int)(rp - fproc);
24111 call_nr = rp->fp_fd & BYTE;
24112 m_in.fd = (rp->fp_fd >>8) & BYTE;
24113 m_in.buffer = rp->fp_buffer;
24114 m_in.nbytes = rp->fp_nbytes;
24115 rp->fp_suspended = NOT_SUSPENDED; /*no longer hanging*/
24116 rp->fp_revived = NOT_REVIVING;
24117 reviving--;
24118 return;
24119 }
24120 panic(__FILE__,"get_work couldn’t revive anyone", NO_NUM);
24121 }
24122
24123 /* Normal case. No one to revive. */
24124 if (receive(ANY, &m_in) != OK) panic(__FILE__,"fs receive error", NO_NUM);
24125 who = m_in.m_source;
24126 call_nr = m_in.m_type;
24127 }
24129 /*===========================================================================*
24130 * buf_pool *
24131 *===========================================================================*/
24132 PRIVATE void buf_pool(void)
24133 {
24134 /* Initialize the buffer pool. */
24135
24136 register struct buf *bp;
24137
24138 bufs_in_use = 0;
24139 front = &buf[0];
24140 rear = &buf[NR_BUFS - 1];
24141
24142 for (bp = &buf[0]; bp < &buf[NR_BUFS]; bp++) {
24143 bp->b_blocknr = NO_BLOCK;
24144 bp->b_dev = NO_DEV;
24145 bp->b_next = bp + 1;
24146 bp->b_prev = bp - 1;
24147 }
24148 buf[0].b_prev = NIL_BUF;
24149 buf[NR_BUFS - 1].b_next = NIL_BUF;
960 File: servers/fs/main.c MINIX SOURCE CODE
24150
24151 for (bp = &buf[0]; bp < &buf[NR_BUFS]; bp++) bp->b_hash = bp->b_next;
24152 buf_hash[0] = front;
24153
24154 }
24156 /*===========================================================================*
24157 * reply *
24158 *===========================================================================*/
24159 PUBLIC void reply(whom, result)
24160 int whom; /* process to reply to */
24161 int result; /* result of the call (usually OK or error #) */
24162 {
24163 /* Send a reply to a user process. It may fail (if the process has just
24164 * been killed by a signal), so don’t check the return code. If the send
24165 * fails, just ignore it.
24166 */
24167 int s;
24168 m_out.reply_type = result;
24169 s = send(whom, &m_out);
24170 if (s != OK) printf("FS: couldn’t send reply %d: %d\n", result, s);
24171 }
24173 /*===========================================================================*
24174 * fs_init *
24175 *===========================================================================*/
24176 PRIVATE void fs_init()
24177 {
24178 /* Initialize global variables, tables, etc. */
24179 register struct inode *rip;
24180 register struct fproc *rfp;
24181 message mess;
24182 int s;
24183
24184 /* Initialize the process table with help of the process manager messages.
24185 * Expect one message for each system process with its slot number and pid.
24186 * When no more processes follow, the magic process number NONE is sent.
24187 * Then, stop and synchronize with the PM.
24188 */
24189 do {
24190 if (OK != (s=receive(PM_PROC_NR, &mess)))
24191 panic(__FILE__,"FS couldn’t receive from PM", s);
24192 if (NONE == mess.PR_PROC_NR) break;
24193
24194 rfp = &fproc[mess.PR_PROC_NR];
24195 rfp->fp_pid = mess.PR_PID;
24196 rfp->fp_realuid = (uid_t) SYS_UID;
24197 rfp->fp_effuid = (uid_t) SYS_UID;
24198 rfp->fp_realgid = (gid_t) SYS_GID;
24199 rfp->fp_effgid = (gid_t) SYS_GID;
24200 rfp->fp_umask = ˜0;
24201
24202 } while (TRUE); /* continue until process NONE */
24203 mess.m_type = OK; /* tell PM that we succeeded */
24204 s=send(PM_PROC_NR, &mess); /* send synchronization message */
24205
24206 /* All process table entries have been set. Continue with FS initialization.
24207 * Certain relations must hold for the file system to work at all. Some
24208 * extra block_size requirements are checked at super-block-read-in time.
24209 */
MINIX SOURCE CODE File: servers/fs/main.c 961
24210 if (OPEN_MAX > 127) panic(__FILE__,"OPEN_MAX > 127", NO_NUM);
24211 if (NR_BUFS < 6) panic(__FILE__,"NR_BUFS < 6", NO_NUM);
24212 if (V1_INODE_SIZE != 32) panic(__FILE__,"V1 inode size != 32", NO_NUM);
24213 if (V2_INODE_SIZE != 64) panic(__FILE__,"V2 inode size != 64", NO_NUM);
24214 if (OPEN_MAX > 8 * sizeof(long))
24215 panic(__FILE__,"Too few bits in fp_cloexec", NO_NUM);
24216
24217 /* The following initializations are needed to let dev_opcl succeed .*/
24218 fp = (struct fproc *) NULL;
24219 who = FS_PROC_NR;
24220
24221 buf_pool(); /* initialize buffer pool */
24222 build_dmap(); /* build device table and map boot driver */
24223 load_ram(); /* init RAM disk, load if it is root */
24224 load_super(root_dev); /* load super block for root device */
24225 init_select(); /* init select() structures */
24226
24227 /* The root device can now be accessed; set process directories. */
24228 for (rfp=&fproc[0]; rfp < &fproc[NR_PROCS]; rfp++) {
24229 if (rfp->fp_pid != PID_FREE) {
24230 rip = get_inode(root_dev, ROOT_INODE);
24231 dup_inode(rip);
24232 rfp->fp_rootdir = rip;
24233 rfp->fp_workdir = rip;
24234 }
24235 }
24236 }
24238 /*===========================================================================*
24239 * igetenv *
24240 *===========================================================================*/
24241 PRIVATE int igetenv(key, optional)
24242 char *key;
24243 int optional;
24244 {
24245 /* Ask kernel for an integer valued boot environment variable. */
24246 char value[64];
24247 int i;
24248
24249 if ((i = env_get_param(key, value, sizeof(value))) != OK) {
24250 if (!optional)
24251 printf("FS: Warning, couldn’t get monitor param: %d\n", i);
24252 return 0;
24253 }
24254 return(atoi(value));
24255 }
24257 /*===========================================================================*
24258 * load_ram *
24259 *===========================================================================*/
24260 PRIVATE void load_ram(void)
24261 {
24262 /* Allocate a RAM disk with size given in the boot parameters. If a RAM disk
24263 * image is given, the copy the entire image device block-by-block to a RAM
24264 * disk with the same size as the image.
24265 * If the root device is not set, the RAM disk will be used as root instead.
24266 */
24267 register struct buf *bp, *bp1;
24268 u32_t lcount, ram_size_kb;
24269 zone_t zones;
962 File: servers/fs/main.c MINIX SOURCE CODE
24270 struct super_block *sp, *dsp;
24271 block_t b;
24272 Dev_t image_dev;
24273 static char sbbuf[MIN_BLOCK_SIZE];
24274 int block_size_image, block_size_ram, ramfs_block_size;
24275 int s;
24276
24277 /* Get some boot environment variables. */
24278 root_dev = igetenv("rootdev", 0);
24279 image_dev = igetenv("ramimagedev", 0);
24280 ram_size_kb = igetenv("ramsize", 0);
24281
24282 /* Open the root device. */
24283 if (dev_open(root_dev, FS_PROC_NR, R_BIT|W_BIT) != OK)
24284 panic(__FILE__,"Cannot open root device",NO_NUM);
24285
24286 /* If we must initialize a ram disk, get details from the image device. */
24287 if (root_dev == DEV_RAM) {
24288 u32_t fsmax, probedev;
24289
24290 /* If we are running from CD, see if we can find it. */
24291 if (igetenv("cdproberoot", 1) && (probedev=cdprobe()) != NO_DEV) {
24292 char devnum[10];
24293 struct sysgetenv env;
24294
24295 /* If so, this is our new RAM image device. */
24296 image_dev = probedev;
24297
24298 /* Tell PM about it, so userland can find out about it
24299 * with sysenv interface.
24300 */
24301 env.key = "cdproberoot";
24302 env.keylen = strlen(env.key);
24303 sprintf(devnum, "%d", (int) probedev);
24304 env.val = devnum;
24305 env.vallen = strlen(devnum);
24306 svrctl(MMSETPARAM, &env);
24307 }
24308
24309 /* Open image device for RAM root. */
24310 if (dev_open(image_dev, FS_PROC_NR, R_BIT) != OK)
24311 panic(__FILE__,"Cannot open RAM image device", NO_NUM);
24312
24313 /* Get size of RAM disk image from the super block. */
24314 sp = &super_block[0];
24315 sp->s_dev = image_dev;
24316 if (read_super(sp) != OK)
24317 panic(__FILE__,"Bad RAM disk image FS", NO_NUM);
24318
24319 lcount = sp->s_zones << sp->s_log_zone_size; /* # blks on root dev*/
24320
24321 /* Stretch the RAM disk file system to the boot parameters size, but
24322 * no further than the last zone bit map block allows.
24323 */
24324 if (ram_size_kb*1024 < lcount*sp->s_block_size)
24325 ram_size_kb = lcount*sp->s_block_size/1024;
24326 fsmax = (u32_t) sp->s_zmap_blocks * CHAR_BIT * sp->s_block_size;
24327 fsmax = (fsmax + (sp->s_firstdatazone-1)) << sp->s_log_zone_size;
24328 if (ram_size_kb*1024 > fsmax*sp->s_block_size)
24329 ram_size_kb = fsmax*sp->s_block_size/1024;
MINIX SOURCE CODE File: servers/fs/main.c 963
24330 }
24331
24332 /* Tell RAM driver how big the RAM disk must be. */
24333 m_out.m_type = DEV_IOCTL;
24334 m_out.PROC_NR = FS_PROC_NR;
24335 m_out.DEVICE = RAM_DEV;
24336 m_out.REQUEST = MIOCRAMSIZE; /* I/O control to use */
24337 m_out.POSITION = (ram_size_kb * 1024); /* request in bytes */
24338 if ((s=sendrec(MEM_PROC_NR, &m_out)) != OK)
24339 panic("FS","sendrec from MEM failed", s);
24340 else if (m_out.REP_STATUS != OK) {
24341 /* Report and continue, unless RAM disk is required as root FS. */
24342 if (root_dev != DEV_RAM) {
24343 report("FS","can’t set RAM disk size", m_out.REP_STATUS);
24344 return;
24345 } else {
24346 panic(__FILE__,"can’t set RAM disk size", m_out.REP_STATUS);
24347 }
24348 }
24349
24350 /* See if we must load the RAM disk image, otherwise return. */
24351 if (root_dev != DEV_RAM)
24352 return;
24353
24354 /* Copy the blocks one at a time from the image to the RAM disk. */
24355 printf("Loading RAM disk onto /dev/ram:\33[23CLoaded: 0 KB");
24356
24357 inode[0].i_mode = I_BLOCK_SPECIAL; /* temp inode for rahead() */
24358 inode[0].i_size = LONG_MAX;
24359 inode[0].i_dev = image_dev;
24360 inode[0].i_zone[0] = image_dev;
24361
24362 block_size_ram = get_block_size(DEV_RAM);
24363 block_size_image = get_block_size(image_dev);
24364
24365 /* RAM block size has to be a multiple of the root image block
24366 * size to make copying easier.
24367 */
24368 if (block_size_image % block_size_ram) {
24369 printf("\nram block size: %d image block size: %d\n",
24370 block_size_ram, block_size_image);
24371 panic(__FILE__, "ram disk block size must be a multiple of "
24372 "the image disk block size", NO_NUM);
24373 }
24374
24375 /* Loading blocks from image device. */
24376 for (b = 0; b < (block_t) lcount; b++) {
24377 int rb, factor;
24378 bp = rahead(&inode[0], b, (off_t)block_size_image * b, block_size_image);
24379 factor = block_size_image/block_size_ram;
24380 for(rb = 0; rb < factor; rb++) {
24381 bp1 = get_block(root_dev, b * factor + rb, NO_READ);
24382 memcpy(bp1->b_data, bp->b_data + rb * block_size_ram,
24383 (size_t) block_size_ram);
24384 bp1->b_dirt = DIRTY;
24385 put_block(bp1, FULL_DATA_BLOCK);
24386 }
24387 put_block(bp, FULL_DATA_BLOCK);
24388 if (b % 11 == 0)
24389 printf("\b\b\b\b\b\b\b\b\b%6ld KB", ((long) b * block_size_image)/1024L);
964 File: servers/fs/main.c MINIX SOURCE CODE
24390 }
24391
24392 /* Commit changes to RAM so dev_io will see it. */
24393 do_sync();
24394
24395 printf("\rRAM disk of %u KB loaded onto /dev/ram.", (unsigned) ram_size_kb);
24396 if (root_dev == DEV_RAM) printf(" Using RAM disk as root FS.");
24397 printf(" \n");
24398
24399 /* Invalidate and close the image device. */
24400 invalidate(image_dev);
24401 dev_close(image_dev);
24402
24403 /* Resize the RAM disk root file system. */
24404 if (dev_io(DEV_READ, root_dev, FS_PROC_NR,
24405 sbbuf, SUPER_BLOCK_BYTES, MIN_BLOCK_SIZE, 0) != MIN_BLOCK_SIZE) {
24406 printf("WARNING: ramdisk read for resizing failed\n");
24407 }
24408 dsp = (struct super_block *) sbbuf;
24409 if (dsp->s_magic == SUPER_V3)
24410 ramfs_block_size = dsp->s_block_size;
24411 else
24412 ramfs_block_size = STATIC_BLOCK_SIZE;
24413 zones = (ram_size_kb * 1024 / ramfs_block_size) >> sp->s_log_zone_size;
24414
24415 dsp->s_nzones = conv2(sp->s_native, (u16_t) zones);
24416 dsp->s_zones = conv4(sp->s_native, zones);
24417 if (dev_io(DEV_WRITE, root_dev, FS_PROC_NR,
24418 sbbuf, SUPER_BLOCK_BYTES, MIN_BLOCK_SIZE, 0) != MIN_BLOCK_SIZE) {
24419 printf("WARNING: ramdisk write for resizing failed\n");
24420 }
24421 }
24423 /*===========================================================================*
24424 * load_super *
24425 *===========================================================================*/
24426 PRIVATE void load_super(super_dev)
24427 dev_t super_dev; /* place to get superblock from */
24428 {
24429 int bad;
24430 register struct super_block *sp;
24431 register struct inode *rip;
24432
24433 /* Initialize the super_block table. */
24434 for (sp = &super_block[0]; sp < &super_block[NR_SUPERS]; sp++)
24435 sp->s_dev = NO_DEV;
24436
24437 /* Read in super_block for the root file system. */
24438 sp = &super_block[0];
24439 sp->s_dev = super_dev;
24440
24441 /* Check super_block for consistency. */
24442 bad = (read_super(sp) != OK);
24443 if (!bad) {
24444 rip = get_inode(super_dev, ROOT_INODE); /* inode for root dir */
24445 if ( (rip->i_mode & I_TYPE) != I_DIRECTORY || rip->i_nlinks < 3) bad++;
24446 }
24447 if (bad) panic(__FILE__,"Invalid root file system", NO_NUM);
24448
24449 sp->s_imount = rip;
MINIX SOURCE CODE File: servers/fs/main.c 965
24450 dup_inode(rip);
24451 sp->s_isup = rip;
24452 sp->s_rd_only = 0;
24453 return;
24454 }