++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             drivers/drivers.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
10700 /* This is the master header for all device drivers. It includes some other
10701 * files and defines the principal constants.
10702 */
10703 #define _POSIX_SOURCE 1 /* tell headers to include POSIX stuff */
10704 #define _MINIX 1 /* tell headers to include MINIX stuff */
10705 #define _SYSTEM 1 /* get negative error number in <errno.h> */
10706
10707 /* The following are so basic, all the *.c files get them automatically. */
10708 #include <minix/config.h> /* MUST be first */
10709 #include <ansi.h> /* MUST be second */
10710 #include <minix/type.h>
10711 #include <minix/com.h>
10712 #include <minix/dmap.h>
10713 #include <minix/callnr.h>
10714 #include <sys/types.h>
770 File: drivers/drivers.h MINIX SOURCE CODE
10715 #include <minix/const.h>
10716 #include <minix/devio.h>
10717 #include <minix/syslib.h>
10718 #include <minix/sysutil.h>
10719 #include <minix/bitmap.h>
10720
10721 #include <ibm/interrupt.h> /* IRQ vectors and miscellaneous ports */
10722 #include <ibm/bios.h> /* BIOS index numbers */
10723 #include <ibm/ports.h> /* Well-known ports */
10724
10725 #include <string.h>
10726 #include <signal.h>
10727 #include <stdlib.h>
10728 #include <limits.h>
10729 #include <stddef.h>
10730 #include <errno.h>
10731 #include <unistd.h>
10732
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
drivers/libdriver/driver.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
10800 /* Types and constants shared between the generic and device dependent
10801 * device driver code.
10802 */
10803
10804 #define _POSIX_SOURCE 1 /* tell headers to include POSIX stuff */
10805 #define _MINIX 1 /* tell headers to include MINIX stuff */
10806 #define _SYSTEM 1 /* get negative error number in <errno.h> */
10807
10808 /* The following are so basic, all the *.c files get them automatically. */
10809 #include <minix/config.h> /* MUST be first */
10810 #include <ansi.h> /* MUST be second */
10811 #include <minix/type.h>
10812 #include <minix/ipc.h>
10813 #include <minix/com.h>
10814 #include <minix/callnr.h>
10815 #include <sys/types.h>
10816 #include <minix/const.h>
10817 #include <minix/syslib.h>
10818 #include <minix/sysutil.h>
10819
10820 #include <string.h>
10821 #include <limits.h>
10822 #include <stddef.h>
10823 #include <errno.h>
10824
10825 #include <minix/partition.h>
10826 #include <minix/u64.h>
10827
10828 /* Info about and entry points into the device dependent code. */
10829 struct driver {
10830 _PROTOTYPE( char *(*dr_name), (void) );
10831 _PROTOTYPE( int (*dr_open), (struct driver *dp, message *m_ptr) );
10832 _PROTOTYPE( int (*dr_close), (struct driver *dp, message *m_ptr) );
10833 _PROTOTYPE( int (*dr_ioctl), (struct driver *dp, message *m_ptr) );
10834 _PROTOTYPE( struct device *(*dr_prepare), (int device) );
MINIX SOURCE CODE File: drivers/libdriver/driver.h 771
10835 _PROTOTYPE( int (*dr_transfer), (int proc_nr, int opcode, off_t position,
10836 iovec_t *iov, unsigned nr_req) );
10837 _PROTOTYPE( void (*dr_cleanup), (void) );
10838 _PROTOTYPE( void (*dr_geometry), (struct partition *entry) );
10839 _PROTOTYPE( void (*dr_signal), (struct driver *dp, message *m_ptr) );
10840 _PROTOTYPE( void (*dr_alarm), (struct driver *dp, message *m_ptr) );
10841 _PROTOTYPE( int (*dr_cancel), (struct driver *dp, message *m_ptr) );
10842 _PROTOTYPE( int (*dr_select), (struct driver *dp, message *m_ptr) );
10843 _PROTOTYPE( int (*dr_other), (struct driver *dp, message *m_ptr) );
10844 _PROTOTYPE( int (*dr_hw_int), (struct driver *dp, message *m_ptr) );
10845 };
10846
10847 #if (CHIP == INTEL)
10848
10849 /* Number of bytes you can DMA before hitting a 64K boundary: */
10850 #define dma_bytes_left(phys) \
10851 ((unsigned) (sizeof(int) == 2 ? 0 : 0x10000) - (unsigned) ((phys) & 0xFFFF))
10852
10853 #endif /* CHIP == INTEL */
10854
10855 /* Base and size of a partition in bytes. */
10856 struct device {
10857 u64_t dv_base;
10858 u64_t dv_size;
10859 };
10860
10861 #define NIL_DEV ((struct device *) 0)
10862
10863 /* Functions defined by driver.c: */
10864 _PROTOTYPE( void driver_task, (struct driver *dr) );
10865 _PROTOTYPE( char *no_name, (void) );
10866 _PROTOTYPE( int do_nop, (struct driver *dp, message *m_ptr) );
10867 _PROTOTYPE( struct device *nop_prepare, (int device) );
10868 _PROTOTYPE( void nop_cleanup, (void) );
10869 _PROTOTYPE( void nop_task, (void) );
10870 _PROTOTYPE( void nop_signal, (struct driver *dp, message *m_ptr) );
10871 _PROTOTYPE( void nop_alarm, (struct driver *dp, message *m_ptr) );
10872 _PROTOTYPE( int nop_cancel, (struct driver *dp, message *m_ptr) );
10873 _PROTOTYPE( int nop_select, (struct driver *dp, message *m_ptr) );
10874 _PROTOTYPE( int do_diocntl, (struct driver *dp, message *m_ptr) );
10875
10876 /* Parameters for the disk drive. */
10877 #define SECTOR_SIZE 512 /* physical sector size in bytes */
10878 #define SECTOR_SHIFT 9 /* for division */
10879 #define SECTOR_MASK 511 /* and remainder */
10880
10881 /* Size of the DMA buffer buffer in bytes. */
10882 #define USE_EXTRA_DMA_BUF 0 /* usually not needed */
10883 #define DMA_BUF_SIZE (DMA_SECTORS * SECTOR_SIZE)
10884
10885 #if (CHIP == INTEL)
10886 extern u8_t *tmp_buf; /* the DMA buffer */
10887 #else
10888 extern u8_t tmp_buf[]; /* the DMA buffer */
10889 #endif
10890 extern phys_bytes tmp_phys; /* phys address of DMA buffer */
772 File: drivers/libdriver/drvlib.h MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
drivers/libdriver/drvlib.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
10900 /* IBM device driver definitions Author: Kees J. Bot
10901 * 7 Dec 1995
10902 */
10903
10904 #include <ibm/partition.h>
10905
10906 _PROTOTYPE( void partition, (struct driver *dr, int device, int style, int atapi) );
10907
10908 /* BIOS parameter table layout. */
10909 #define bp_cylinders(t) (* (u16_t *) (&(t)[0]))
10910 #define bp_heads(t) (* (u8_t *) (&(t)[2]))
10911 #define bp_reduced_wr(t) (* (u16_t *) (&(t)[3]))
10912 #define bp_precomp(t) (* (u16_t *) (&(t)[5]))
10913 #define bp_max_ecc(t) (* (u8_t *) (&(t)[7]))
10914 #define bp_ctlbyte(t) (* (u8_t *) (&(t)[8]))
10915 #define bp_landingzone(t) (* (u16_t *) (&(t)[12]))
10916 #define bp_sectors(t) (* (u8_t *) (&(t)[14]))
10917
10918 /* Miscellaneous. */
10919 #define DEV_PER_DRIVE (1 + NR_PARTITIONS)
10920 #define MINOR_t0 64
10921 #define MINOR_r0 120
10922 #define MINOR_d0p0s0 128
10923 #define MINOR_fd0p0 (28<<2)
10924 #define P_FLOPPY 0
10925 #define P_PRIMARY 1
10926 #define P_SUB 2
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
drivers/libdriver/driver.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
11000 /* This file contains device independent device driver interface.
11001 *
11002 * Changes:
11003 * Jul 25, 2005 added SYS_SIG type for signals (Jorrit N. Herder)
11004 * Sep 15, 2004 added SYN_ALARM type for timeouts (Jorrit N. Herder)
11005 * Jul 23, 2004 removed kernel dependencies (Jorrit N. Herder)
11006 * Apr 02, 1992 constructed from AT wini and floppy driver (Kees J. Bot)
11007 *
11008 *
11009 * The drivers support the following operations (using message format m2):
11010 *
11011 * m_type DEVICE PROC_NR COUNT POSITION ADRRESS
11012 * ----------------------------------------------------------------
11013 * | DEV_OPEN | device | proc nr| | | |
11014 * |------------+---------+---------+---------+---------+---------|
11015 * | DEV_CLOSE | device | proc nr| | | |
11016 * |------------+---------+---------+---------+---------+---------|
11017 * | DEV_READ | device | proc nr | bytes | offset | buf ptr |
11018 * |------------+---------+---------+---------+---------+---------|
11019 * | DEV_WRITE | device | proc nr | bytes | offset | buf ptr |
MINIX SOURCE CODE File: drivers/libdriver/driver.c 773
11020 * |------------+---------+---------+---------+---------+---------|
11021 * | DEV_GATHER | device | proc nr | iov len | offset | iov ptr |
11022 * |------------+---------+---------+---------+---------+---------|
11023 * | DEV_SCATTER| device | proc nr | iov len | offset | iov ptr |
11024 * |------------+---------+---------+---------+---------+---------|
11025 * | DEV_IOCTL | device | proc nr |func code| | buf ptr |
11026 * |------------+---------+---------+---------+---------+---------|
11027 * | CANCEL | device | proc nr | r/w | | |
11028 * |------------+---------+---------+---------+---------+---------|
11029 * | HARD_STOP| | | | | |
11030 * ----------------------------------------------------------------
11031 *
11032 * The file contains one entry point:
11033 *
11034 * driver_task: called by the device dependent task entry
11035 */
11036
11037 #include "../drivers.h"
11038 #include <sys/ioc_disk.h>
11039 #include "driver.h"
11040
11041 #define BUF_EXTRA 0
11042
11043 /* Claim space for variables. */
11044 PRIVATE u8_t buffer[(unsigned) 2 * DMA_BUF_SIZE + BUF_EXTRA];
11045 u8_t *tmp_buf; /* the DMA buffer eventually */
11046 phys_bytes tmp_phys; /* phys address of DMA buffer */
11047
11048 FORWARD _PROTOTYPE( void init_buffer, (void) );
11049 FORWARD _PROTOTYPE( int do_rdwt, (struct driver *dr, message *mp) );
11050 FORWARD _PROTOTYPE( int do_vrdwt, (struct driver *dr, message *mp) );
11051
11052 int device_caller;
11053
11054 /*===========================================================================*
11055 * driver_task *
11056 *===========================================================================*/
11057 PUBLIC void driver_task(dp)
11058 struct driver *dp; /* Device dependent entry points. */
11059 {
11060 /* Main program of any device driver task. */
11061
11062 int r, proc_nr;
11063 message mess;
11064
11065 /* Get a DMA buffer. */
11066 init_buffer();
11067
11068 /* Here is the main loop of the disk task. It waits for a message, carries
11069 * it out, and sends a reply.
11070 */
11071 while (TRUE) {
11072
11073 /* Wait for a request to read or write a disk block. */
11074 if(receive(ANY, &mess) != OK) continue;
11075
11076 device_caller = mess.m_source;
11077 proc_nr = mess.PROC_NR;
11078
11079 /* Now carry out the work. */
774 File: drivers/libdriver/driver.c MINIX SOURCE CODE
11080 switch(mess.m_type) {
11081 case DEV_OPEN: r = (*dp->dr_open)(dp, &mess); break;
11082 case DEV_CLOSE: r = (*dp->dr_close)(dp, &mess); break;
11083 case DEV_IOCTL: r = (*dp->dr_ioctl)(dp, &mess); break;
11084 case CANCEL: r = (*dp->dr_cancel)(dp, &mess);break;
11085 case DEV_SELECT: r = (*dp->dr_select)(dp, &mess);break;
11086
11087 case DEV_READ:
11088 case DEV_WRITE: r = do_rdwt(dp, &mess); break;
11089 case DEV_GATHER:
11090 case DEV_SCATTER: r = do_vrdwt(dp, &mess); break;
11091
11092 case HARD_INT: /* leftover interrupt or expired timer. */
11093 if(dp->dr_hw_int) {
11094 (*dp->dr_hw_int)(dp, &mess);
11095 }
11096 continue;
11097 case SYS_SIG: (*dp->dr_signal)(dp, &mess);
11098 continue; /* don’t reply */
11099 case SYN_ALARM: (*dp->dr_alarm)(dp, &mess);
11100 continue; /* don’t reply */
11101 default:
11102 if(dp->dr_other)
11103 r = (*dp->dr_other)(dp, &mess);
11104 else
11105 r = EINVAL;
11106 break;
11107 }
11108
11109 /* Clean up leftover state. */
11110 (*dp->dr_cleanup)();
11111
11112 /* Finally, prepare and send the reply message. */
11113 if (r != EDONTREPLY) {
11114 mess.m_type = TASK_REPLY;
11115 mess.REP_PROC_NR = proc_nr;
11116 /* Status is # of bytes transferred or error code. */
11117 mess.REP_STATUS = r;
11118 send(device_caller, &mess);
11119 }
11120 }
11121 }
11123 /*===========================================================================*
11124 * init_buffer *
11125 *===========================================================================*/
11126 PRIVATE void init_buffer()
11127 {
11128 /* Select a buffer that can safely be used for DMA transfers. It may also
11129 * be used to read partition tables and such. Its absolute address is
11130 * ’tmp_phys’, the normal address is ’tmp_buf’.
11131 */
11132
11133 unsigned left;
11134
11135 tmp_buf = buffer;
11136 sys_umap(SELF, D, (vir_bytes)buffer, (phys_bytes)sizeof(buffer), &tmp_phys);
11137
11138 if ((left = dma_bytes_left(tmp_phys)) < DMA_BUF_SIZE) {
11139 /* First half of buffer crosses a 64K boundary, can’t DMA into that */
MINIX SOURCE CODE File: drivers/libdriver/driver.c 775
11140 tmp_buf += left;
11141 tmp_phys += left;
11142 }
11143 }
11145 /*===========================================================================*
11146 * do_rdwt *
11147 *===========================================================================*/
11148 PRIVATE int do_rdwt(dp, mp)
11149 struct driver *dp; /* device dependent entry points */
11150 message *mp; /* pointer to read or write message */
11151 {
11152 /* Carry out a single read or write request. */
11153 iovec_t iovec1;
11154 int r, opcode;
11155 phys_bytes phys_addr;
11156
11157 /* Disk address? Address and length of the user buffer? */
11158 if (mp->COUNT < 0) return(EINVAL);
11159
11160 /* Check the user buffer. */
11161 sys_umap(mp->PROC_NR, D, (vir_bytes) mp->ADDRESS, mp->COUNT, &phys_addr);
11162 if (phys_addr == 0) return(EFAULT);
11163
11164 /* Prepare for I/O. */
11165 if ((*dp->dr_prepare)(mp->DEVICE) == NIL_DEV) return(ENXIO);
11166
11167 /* Create a one element scatter/gather vector for the buffer. */
11168 opcode = mp->m_type == DEV_READ ? DEV_GATHER : DEV_SCATTER;
11169 iovec1.iov_addr = (vir_bytes) mp->ADDRESS;
11170 iovec1.iov_size = mp->COUNT;
11171
11172 /* Transfer bytes from/to the device. */
11173 r = (*dp->dr_transfer)(mp->PROC_NR, opcode, mp->POSITION, &iovec1, 1);
11174
11175 /* Return the number of bytes transferred or an error code. */
11176 return(r == OK ? (mp->COUNT - iovec1.iov_size) : r);
11177 }
11179 /*==========================================================================*
11180 * do_vrdwt *
11181 *==========================================================================*/
11182 PRIVATE int do_vrdwt(dp, mp)
11183 struct driver *dp; /* device dependent entry points */
11184 message *mp; /* pointer to read or write message */
11185 {
11186 /* Carry out an device read or write to/from a vector of user addresses.
11187 * The "user addresses" are assumed to be safe, i.e. FS transferring to/from
11188 * its own buffers, so they are not checked.
11189 */
11190 static iovec_t iovec[NR_IOREQS];
11191 iovec_t *iov;
11192 phys_bytes iovec_size;
11193 unsigned nr_req;
11194 int r;
11195
11196 nr_req = mp->COUNT; /* Length of I/O vector */
11197
11198 if (mp->m_source < 0) {
11199 /* Called by a task, no need to copy vector. */
776 File: drivers/libdriver/driver.c MINIX SOURCE CODE
11200 iov = (iovec_t *) mp->ADDRESS;
11201 } else {
11202 /* Copy the vector from the caller to kernel space. */
11203 if (nr_req > NR_IOREQS) nr_req = NR_IOREQS;
11204 iovec_size = (phys_bytes) (nr_req * sizeof(iovec[0]));
11205
11206 if (OK != sys_datacopy(mp->m_source, (vir_bytes) mp->ADDRESS,
11207 SELF, (vir_bytes) iovec, iovec_size))
11208 panic((*dp->dr_name)(),"bad I/O vector by", mp->m_source);
11209 iov = iovec;
11210 }
11211
11212 /* Prepare for I/O. */
11213 if ((*dp->dr_prepare)(mp->DEVICE) == NIL_DEV) return(ENXIO);
11214
11215 /* Transfer bytes from/to the device. */
11216 r = (*dp->dr_transfer)(mp->PROC_NR, mp->m_type, mp->POSITION, iov, nr_req);
11217
11218 /* Copy the I/O vector back to the caller. */
11219 if (mp->m_source >= 0) {
11220 sys_datacopy(SELF, (vir_bytes) iovec,
11221 mp->m_source, (vir_bytes) mp->ADDRESS, iovec_size);
11222 }
11223 return(r);
11224 }
11226 /*===========================================================================*
11227 * no_name *
11228 *===========================================================================*/
11229 PUBLIC char *no_name()
11230 {
11231 /* Use this default name if there is no specific name for the device. This was
11232 * originally done by fetching the name from the task table for this process:
11233 * "return(tasktab[proc_number(proc_ptr) + NR_TASKS].name);", but currently a
11234 * real "noname" is returned. Perhaps, some system information service can be
11235 * queried for a name at a later time.
11236 */
11237 static char name[] = "noname";
11238 return name;
11239 }
11241 /*============================================================================*
11242 * do_nop *
11243 *============================================================================*/
11244 PUBLIC int do_nop(dp, mp)
11245 struct driver *dp;
11246 message *mp;
11247 {
11248 /* Nothing there, or nothing to do. */
11249
11250 switch (mp->m_type) {
11251 case DEV_OPEN: return(ENODEV);
11252 case DEV_CLOSE: return(OK);
11253 case DEV_IOCTL: return(ENOTTY);
11254 default: return(EIO);
11255 }
11256 }
MINIX SOURCE CODE File: drivers/libdriver/driver.c 777
11258 /*============================================================================*
11259 * nop_signal *
11260 *============================================================================*/
11261 PUBLIC void nop_signal(dp, mp)
11262 struct driver *dp;
11263 message *mp;
11264 {
11265 /* Default action for signal is to ignore. */
11266 }
11268 /*============================================================================*
11269 * nop_alarm *
11270 *============================================================================*/
11271 PUBLIC void nop_alarm(dp, mp)
11272 struct driver *dp;
11273 message *mp;
11274 {
11275 /* Ignore the leftover alarm. */
11276 }
11278 /*===========================================================================*
11279 * nop_prepare *
11280 *===========================================================================*/
11281 PUBLIC struct device *nop_prepare(device)
11282 {
11283 /* Nothing to prepare for. */
11284 return(NIL_DEV);
11285 }
11287 /*===========================================================================*
11288 * nop_cleanup *
11289 *===========================================================================*/
11290 PUBLIC void nop_cleanup()
11291 {
11292 /* Nothing to clean up. */
11293 }
11295 /*===========================================================================*
11296 * nop_cancel *
11297 *===========================================================================*/
11298 PUBLIC int nop_cancel(struct driver *dr, message *m)
11299 {
11300 /* Nothing to do for cancel. */
11301 return(OK);
11302 }
11304 /*===========================================================================*
11305 * nop_select *
11306 *===========================================================================*/
11307 PUBLIC int nop_select(struct driver *dr, message *m)
11308 {
11309 /* Nothing to do for select. */
11310 return(OK);
11311 }
11313 /*============================================================================*
11314 * do_diocntl *
11315 *============================================================================*/
11316 PUBLIC int do_diocntl(dp, mp)
11317 struct driver *dp;
778 File: drivers/libdriver/driver.c MINIX SOURCE CODE
11318 message *mp; /* pointer to ioctl request */
11319 {
11320 /* Carry out a partition setting/getting request. */
11321 struct device *dv;
11322 struct partition entry;
11323 int s;
11324
11325 if (mp->REQUEST != DIOCSETP && mp->REQUEST != DIOCGETP) {
11326 if(dp->dr_other) {
11327 return dp->dr_other(dp, mp);
11328 } else return(ENOTTY);
11329 }
11330
11331 /* Decode the message parameters. */
11332 if ((dv = (*dp->dr_prepare)(mp->DEVICE)) == NIL_DEV) return(ENXIO);
11333
11334 if (mp->REQUEST == DIOCSETP) {
11335 /* Copy just this one partition table entry. */
11336 if (OK != (s=sys_datacopy(mp->PROC_NR, (vir_bytes) mp->ADDRESS,
11337 SELF, (vir_bytes) &entry, sizeof(entry))))
11338 return s;
11339 dv->dv_base = entry.base;
11340 dv->dv_size = entry.size;
11341 } else {
11342 /* Return a partition table entry and the geometry of the drive. */
11343 entry.base = dv->dv_base;
11344 entry.size = dv->dv_size;
11345 (*dp->dr_geometry)(&entry);
11346 if (OK != (s=sys_datacopy(SELF, (vir_bytes) &entry,
11347 mp->PROC_NR, (vir_bytes) mp->ADDRESS, sizeof(entry))))
11348 return s;
11349 }
11350 return(OK);
11351 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
drivers/libdriver/drvlib.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
11400 /* IBM device driver utility functions. Author: Kees J. Bot
11401 * 7 Dec 1995
11402 * Entry point:
11403 * partition: partition a disk to the partition table(s) on it.
11404 */
11405
11406 #include "driver.h"
11407 #include "drvlib.h"
11408 #include <unistd.h>
11409
11410 /* Extended partition? */
11411 #define ext_part(s) ((s) == 0x05 || (s) == 0x0F)
11412
11413 FORWARD _PROTOTYPE( void extpartition, (struct driver *dp, int extdev,
11414 unsigned long extbase) );
11415 FORWARD _PROTOTYPE( int get_part_table, (struct driver *dp, int device,
11416 unsigned long offset, struct part_entry *table));
11417 FORWARD _PROTOTYPE( void sort, (struct part_entry *table) );
11418
11419 #ifndef CD_SECTOR_SIZE
MINIX SOURCE CODE File: drivers/libdriver/drvlib.c 779
11420 #define CD_SECTOR_SIZE 2048
11421 #endif
11422
11423 /*============================================================================*
11424 * partition *
11425 *============================================================================*/
11426 PUBLIC void partition(dp, device, style, atapi)
11427 struct driver *dp; /* device dependent entry points */
11428 int device; /* device to partition */
11429 int style; /* partitioning style: floppy, primary, sub. */
11430 int atapi; /* atapi device */
11431 {
11432 /* This routine is called on first open to initialize the partition tables
11433 * of a device. It makes sure that each partition falls safely within the
11434 * device’s limits. Depending on the partition style we are either making
11435 * floppy partitions, primary partitions or subpartitions. Only primary
11436 * partitions are sorted, because they are shared with other operating
11437 * systems that expect this.
11438 */
11439 struct part_entry table[NR_PARTITIONS], *pe;
11440 int disk, par;
11441 struct device *dv;
11442 unsigned long base, limit, part_limit;
11443
11444 /* Get the geometry of the device to partition */
11445 if ((dv = (*dp->dr_prepare)(device)) == NIL_DEV
11446 || cmp64u(dv->dv_size, 0) == 0) return;
11447 base = div64u(dv->dv_base, SECTOR_SIZE);
11448 limit = base + div64u(dv->dv_size, SECTOR_SIZE);
11449
11450 /* Read the partition table for the device. */
11451 if(!get_part_table(dp, device, 0L, table)) {
11452 return;
11453 }
11454
11455 /* Compute the device number of the first partition. */
11456 switch (style) {
11457 case P_FLOPPY:
11458 device += MINOR_fd0p0;
11459 break;
11460 case P_PRIMARY:
11461 sort(table); /* sort a primary partition table */
11462 device += 1;
11463 break;
11464 case P_SUB:
11465 disk = device / DEV_PER_DRIVE;
11466 par = device % DEV_PER_DRIVE - 1;
11467 device = MINOR_d0p0s0 + (disk * NR_PARTITIONS + par) * NR_PARTITIONS;
11468 }
11469
11470 /* Find an array of devices. */
11471 if ((dv = (*dp->dr_prepare)(device)) == NIL_DEV) return;
11472
11473 /* Set the geometry of the partitions from the partition table. */
11474 for (par = 0; par < NR_PARTITIONS; par++, dv++) {
11475 /* Shrink the partition to fit within the device. */
11476 pe = &table[par];
11477 part_limit = pe->lowsec + pe->size;
11478 if (part_limit < pe->lowsec) part_limit = limit;
11479 if (part_limit > limit) part_limit = limit;
780 File: drivers/libdriver/drvlib.c MINIX SOURCE CODE
11480 if (pe->lowsec < base) pe->lowsec = base;
11481 if (part_limit < pe->lowsec) part_limit = pe->lowsec;
11482
11483 dv->dv_base = mul64u(pe->lowsec, SECTOR_SIZE);
11484 dv->dv_size = mul64u(part_limit - pe->lowsec, SECTOR_SIZE);
11485
11486 if (style == P_PRIMARY) {
11487 /* Each Minix primary partition can be subpartitioned. */
11488 if (pe->sysind == MINIX_PART)
11489 partition(dp, device + par, P_SUB, atapi);
11490
11491 /* An extended partition has logical partitions. */
11492 if (ext_part(pe->sysind))
11493 extpartition(dp, device + par, pe->lowsec);
11494 }
11495 }
11496 }
11498 /*============================================================================*
11499 * extpartition *
11500 *============================================================================*/
11501 PRIVATE void extpartition(dp, extdev, extbase)
11502 struct driver *dp; /* device dependent entry points */
11503 int extdev; /* extended partition to scan */
11504 unsigned long extbase; /* sector offset of the base extended partition */
11505 {
11506 /* Extended partitions cannot be ignored alas, because people like to move
11507 * files to and from DOS partitions. Avoid reading this code, it’s no fun.
11508 */
11509 struct part_entry table[NR_PARTITIONS], *pe;
11510 int subdev, disk, par;
11511 struct device *dv;
11512 unsigned long offset, nextoffset;
11513
11514 disk = extdev / DEV_PER_DRIVE;
11515 par = extdev % DEV_PER_DRIVE - 1;
11516 subdev = MINOR_d0p0s0 + (disk * NR_PARTITIONS + par) * NR_PARTITIONS;
11517
11518 offset = 0;
11519 do {
11520 if (!get_part_table(dp, extdev, offset, table)) return;
11521 sort(table);
11522
11523 /* The table should contain one logical partition and optionally
11524 * another extended partition. (It’s a linked list.)
11525 */
11526 nextoffset = 0;
11527 for (par = 0; par < NR_PARTITIONS; par++) {
11528 pe = &table[par];
11529 if (ext_part(pe->sysind)) {
11530 nextoffset = pe->lowsec;
11531 } else
11532 if (pe->sysind != NO_PART) {
11533 if ((dv = (*dp->dr_prepare)(subdev)) == NIL_DEV) return;
11534
11535 dv->dv_base = mul64u(extbase + offset + pe->lowsec,
11536 SECTOR_SIZE);
11537 dv->dv_size = mul64u(pe->size, SECTOR_SIZE);
11538
11539 /* Out of devices? */
MINIX SOURCE CODE File: drivers/libdriver/drvlib.c 781
11540 if (++subdev % NR_PARTITIONS == 0) return;
11541 }
11542 }
11543 } while ((offset = nextoffset) != 0);
11544 }
11546 /*============================================================================*
11547 * get_part_table *
11548 *============================================================================*/
11549 PRIVATE int get_part_table(dp, device, offset, table)
11550 struct driver *dp;
11551 int device;
11552 unsigned long offset; /* sector offset to the table */
11553 struct part_entry *table; /* four entries */
11554 {
11555 /* Read the partition table for the device, return true iff there were no
11556 * errors.
11557 */
11558 iovec_t iovec1;
11559 off_t position;
11560 static unsigned char partbuf[CD_SECTOR_SIZE];
11561
11562 position = offset << SECTOR_SHIFT;
11563 iovec1.iov_addr = (vir_bytes) partbuf;
11564 iovec1.iov_size = CD_SECTOR_SIZE;
11565 if ((*dp->dr_prepare)(device) != NIL_DEV) {
11566 (void) (*dp->dr_transfer)(SELF, DEV_GATHER, position, &iovec1, 1);
11567 }
11568 if (iovec1.iov_size != 0) {
11569 return 0;
11570 }
11571 if (partbuf[510] != 0x55 || partbuf[511] != 0xAA) {
11572 /* Invalid partition table. */
11573 return 0;
11574 }
11575 memcpy(table, (partbuf + PART_TABLE_OFF), NR_PARTITIONS * sizeof(table[0]));
11576 return 1;
11577 }
11579 /*===========================================================================*
11580 * sort *
11581 *===========================================================================*/
11582 PRIVATE void sort(table)
11583 struct part_entry *table;
11584 {
11585 /* Sort a partition table. */
11586 struct part_entry *pe, tmp;
11587 int n = NR_PARTITIONS;
11588
11589 do {
11590 for (pe = table; pe < table + NR_PARTITIONS-1; pe++) {
11591 if (pe[0].sysind == NO_PART
11592 || (pe[0].lowsec > pe[1].lowsec
11593 && pe[1].sysind != NO_PART)) {
11594 tmp = pe[0]; pe[0] = pe[1]; pe[1] = tmp;
11595 }
11596 }
11597 } while (--n > 0);
11598 }
782 File: drivers/memory/memory.c MINIX SOURCE CODE
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
drivers/memory/memory.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
11600 /* This file contains the device dependent part of the drivers for the
11601 * following special files:
11602 * /dev/ram - RAM disk
11603 * /dev/mem - absolute memory
11604 * /dev/kmem - kernel virtual memory
11605 * /dev/null - null device (data sink)
11606 * /dev/boot - boot device loaded from boot image
11607 * /dev/zero - null byte stream generator
11608 *
11609 * Changes:
11610 * Apr 29, 2005 added null byte generator (Jorrit N. Herder)
11611 * Apr 09, 2005 added support for boot device (Jorrit N. Herder)
11612 * Jul 26, 2004 moved RAM driver to user-space (Jorrit N. Herder)
11613 * Apr 20, 1992 device dependent/independent split (Kees J. Bot)
11614 */
11615
11616 #include "../drivers.h"
11617 #include "../libdriver/driver.h"
11618 #include <sys/ioc_memory.h>
11619 #include "../../kernel/const.h"
11620 #include "../../kernel/config.h"
11621 #include "../../kernel/type.h"
11622
11623 #include "assert.h"
11624
11625 #define NR_DEVS 6 /* number of minor devices */
11626
11627 PRIVATE struct device m_geom[NR_DEVS]; /* base and size of each device */
11628 PRIVATE int m_seg[NR_DEVS]; /* segment index of each device */
11629 PRIVATE int m_device; /* current device */
11630 PRIVATE struct kinfo kinfo; /* kernel information */
11631 PRIVATE struct machine machine; /* machine information */
11632
11633 extern int errno; /* error number for PM calls */
11634
11635 FORWARD _PROTOTYPE( char *m_name, (void) );
11636 FORWARD _PROTOTYPE( struct device *m_prepare, (int device) );
11637 FORWARD _PROTOTYPE( int m_transfer, (int proc_nr, int opcode, off_t position,
11638 iovec_t *iov, unsigned nr_req) );
11639 FORWARD _PROTOTYPE( int m_do_open, (struct driver *dp, message *m_ptr) );
11640 FORWARD _PROTOTYPE( void m_init, (void) );
11641 FORWARD _PROTOTYPE( int m_ioctl, (struct driver *dp, message *m_ptr) );
11642 FORWARD _PROTOTYPE( void m_geometry, (struct partition *entry) );
11643
11644 /* Entry points to this driver. */
11645 PRIVATE struct driver m_dtab = {
11646 m_name, /* current device’s name */
11647 m_do_open, /* open or mount */
11648 do_nop, /* nothing on a close */
11649 m_ioctl, /* specify ram disk geometry */
11650 m_prepare, /* prepare for I/O on a given minor device */
11651 m_transfer, /* do the I/O */
11652 nop_cleanup, /* no need to clean up */
11653 m_geometry, /* memory device "geometry" */
11654 nop_signal, /* system signals */
MINIX SOURCE CODE File: drivers/memory/memory.c 783
11655 nop_alarm,
11656 nop_cancel,
11657 nop_select,
11658 NULL,
11659 NULL
11660 };
11661
11662 /* Buffer for the /dev/zero null byte feed. */
11663 #define ZERO_BUF_SIZE 1024
11664 PRIVATE char dev_zero[ZERO_BUF_SIZE];
11665
11666 #define click_to_round_k(n) \
11667 ((unsigned) ((((unsigned long) (n) << CLICK_SHIFT) + 512) / 1024))
11668
11669 /*===========================================================================*
11670 * main *
11671 *===========================================================================*/
11672 PUBLIC int main(void)
11673 {
11674 /* Main program. Initialize the memory driver and start the main loop. */
11675 m_init();
11676 driver_task(&m_dtab);
11677 return(OK);
11678 }
11680 /*===========================================================================*
11681 * m_name *
11682 *===========================================================================*/
11683 PRIVATE char *m_name()
11684 {
11685 /* Return a name for the current device. */
11686 static char name[] = "memory";
11687 return name;
11688 }
11690 /*===========================================================================*
11691 * m_prepare *
11692 *===========================================================================*/
11693 PRIVATE struct device *m_prepare(device)
11694 int device;
11695 {
11696 /* Prepare for I/O on a device: check if the minor device number is ok. */
11697 if (device < 0 || device >= NR_DEVS) return(NIL_DEV);
11698 m_device = device;
11699
11700 return(&m_geom[device]);
11701 }
11703 /*===========================================================================*
11704 * m_transfer *
11705 *===========================================================================*/
11706 PRIVATE int m_transfer(proc_nr, opcode, position, iov, nr_req)
11707 int proc_nr; /* process doing the request */
11708 int opcode; /* DEV_GATHER or DEV_SCATTER */
11709 off_t position; /* offset on device to read or write */
11710 iovec_t *iov; /* pointer to read or write request vector */
11711 unsigned nr_req; /* length of request vector */
11712 {
11713 /* Read or write one the driver’s minor devices. */
11714 phys_bytes mem_phys;
784 File: drivers/memory/memory.c MINIX SOURCE CODE
11715 int seg;
11716 unsigned count, left, chunk;
11717 vir_bytes user_vir;
11718 struct device *dv;
11719 unsigned long dv_size;
11720 int s;
11721
11722 /* Get minor device number and check for /dev/null. */
11723 dv = &m_geom[m_device];
11724 dv_size = cv64ul(dv->dv_size);
11725
11726 while (nr_req > 0) {
11727
11728 /* How much to transfer and where to / from. */
11729 count = iov->iov_size;
11730 user_vir = iov->iov_addr;
11731
11732 switch (m_device) {
11733
11734 /* No copying; ignore request. */
11735 case NULL_DEV:
11736 if (opcode == DEV_GATHER) return(OK); /* always at EOF */
11737 break;
11738
11739 /* Virtual copying. For RAM disk, kernel memory and boot device. */
11740 case RAM_DEV:
11741 case KMEM_DEV:
11742 case BOOT_DEV:
11743 if (position >= dv_size) return(OK); /* check for EOF */
11744 if (position + count > dv_size) count = dv_size - position;
11745 seg = m_seg[m_device];
11746
11747 if (opcode == DEV_GATHER) { /* copy actual data */
11748 sys_vircopy(SELF,seg,position, proc_nr,D,user_vir, count);
11749 } else {
11750 sys_vircopy(proc_nr,D,user_vir, SELF,seg,position, count);
11751 }
11752 break;
11753
11754 /* Physical copying. Only used to access entire memory. */
11755 case MEM_DEV:
11756 if (position >= dv_size) return(OK); /* check for EOF */
11757 if (position + count > dv_size) count = dv_size - position;
11758 mem_phys = cv64ul(dv->dv_base) + position;
11759
11760 if (opcode == DEV_GATHER) { /* copy data */
11761 sys_physcopy(NONE, PHYS_SEG, mem_phys,
11762 proc_nr, D, user_vir, count);
11763 } else {
11764 sys_physcopy(proc_nr, D, user_vir,
11765 NONE, PHYS_SEG, mem_phys, count);
11766 }
11767 break;
11768
11769 /* Null byte stream generator. */
11770 case ZERO_DEV:
11771 if (opcode == DEV_GATHER) {
11772 left = count;
11773 while (left > 0) {
11774 chunk = (left > ZERO_BUF_SIZE) ? ZERO_BUF_SIZE : left;
MINIX SOURCE CODE File: drivers/memory/memory.c 785
11775 if (OK != (s=sys_vircopy(SELF, D, (vir_bytes) dev_zero,
11776 proc_nr, D, user_vir, chunk)))
11777 report("MEM","sys_vircopy failed", s);
11778 left -= chunk;
11779 user_vir += chunk;
11780 }
11781 }
11782 break;
11783
11784 /* Unknown (illegal) minor device. */
11785 default:
11786 return(EINVAL);
11787 }
11788
11789 /* Book the number of bytes transferred. */
11790 position += count;
11791 iov->iov_addr += count;
11792 if ((iov->iov_size -= count) == 0) { iov++; nr_req--; }
11793
11794 }
11795 return(OK);
11796 }
11798 /*===========================================================================*
11799 * m_do_open *
11800 *===========================================================================*/
11801 PRIVATE int m_do_open(dp, m_ptr)
11802 struct driver *dp;
11803 message *m_ptr;
11804 {
11805 /* Check device number on open. (This used to give I/O privileges to a
11806 * process opening /dev/mem or /dev/kmem. This may be needed in case of
11807 * memory mapped I/O. With system calls to do I/O this is no longer needed.)
11808 */
11809 if (m_prepare(m_ptr->DEVICE) == NIL_DEV) return(ENXIO);
11810
11811 return(OK);
11812 }
11814 /*===========================================================================*
11815 * m_init *
11816 *===========================================================================*/
11817 PRIVATE void m_init()
11818 {
11819 /* Initialize this task. All minor devices are initialized one by one. */
11820 int i, s;
11821
11822 if (OK != (s=sys_getkinfo(&kinfo))) {
11823 panic("MEM","Couldn’t get kernel information.",s);
11824 }
11825
11826 /* Install remote segment for /dev/kmem memory. */
11827 m_geom[KMEM_DEV].dv_base = cvul64(kinfo.kmem_base);
11828 m_geom[KMEM_DEV].dv_size = cvul64(kinfo.kmem_size);
11829 if (OK != (s=sys_segctl(&m_seg[KMEM_DEV], (u16_t *) &s, (vir_bytes *) &s,
11830 kinfo.kmem_base, kinfo.kmem_size))) {
11831 panic("MEM","Couldn’t install remote segment.",s);
11832 }
11833
11834 /* Install remote segment for /dev/boot memory, if enabled. */
786 File: drivers/memory/memory.c MINIX SOURCE CODE
11835 m_geom[BOOT_DEV].dv_base = cvul64(kinfo.bootdev_base);
11836 m_geom[BOOT_DEV].dv_size = cvul64(kinfo.bootdev_size);
11837 if (kinfo.bootdev_base > 0) {
11838 if (OK != (s=sys_segctl(&m_seg[BOOT_DEV], (u16_t *) &s, (vir_bytes *) &s,
11839 kinfo.bootdev_base, kinfo.bootdev_size))) {
11840 panic("MEM","Couldn’t install remote segment.",s);
11841 }
11842 }
11843
11844 /* Initialize /dev/zero. Simply write zeros into the buffer. */
11845 for (i=0; i<ZERO_BUF_SIZE; i++) {
11846 dev_zero[i] = ’\0’;
11847 }
11848
11849 /* Set up memory ranges for /dev/mem. */
11850 if (OK != (s=sys_getmachine(&machine))) {
11851 panic("MEM","Couldn’t get machine information.",s);
11852 }
11853 if (! machine.protected) {
11854 m_geom[MEM_DEV].dv_size = cvul64(0x100000); /* 1M for 8086 systems */
11855 } else {
11856 m_geom[MEM_DEV].dv_size = cvul64(0xFFFFFFFF); /* 4G-1 for 386 systems */
11857 }
11858 }
11860 /*===========================================================================*
11861 * m_ioctl *
11862 *===========================================================================*/
11863 PRIVATE int m_ioctl(dp, m_ptr)
11864 struct driver *dp; /* pointer to driver structure */
11865 message *m_ptr; /* pointer to control message */
11866 {
11867 /* I/O controls for the memory driver. Currently there is one I/O control:
11868 * - MIOCRAMSIZE: to set the size of the RAM disk.
11869 */
11870 struct device *dv;
11871 if ((dv = m_prepare(m_ptr->DEVICE)) == NIL_DEV) return(ENXIO);
11872
11873 switch (m_ptr->REQUEST) {
11874 case MIOCRAMSIZE: {
11875 /* FS wants to create a new RAM disk with the given size. */
11876 phys_bytes ramdev_size;
11877 phys_bytes ramdev_base;
11878 int s;
11879
11880 if (m_ptr->PROC_NR != FS_PROC_NR) {
11881 report("MEM", "warning, MIOCRAMSIZE called by", m_ptr->PROC_NR);
11882 return(EPERM);
11883 }
11884
11885 /* Try to allocate a piece of memory for the RAM disk. */
11886 ramdev_size = m_ptr->POSITION;
11887 if (allocmem(ramdev_size, &ramdev_base) < 0) {
11888 report("MEM", "warning, allocmem failed", errno);
11889 return(ENOMEM);
11890 }
11891 dv->dv_base = cvul64(ramdev_base);
11892 dv->dv_size = cvul64(ramdev_size);
11893
11894 if (OK != (s=sys_segctl(&m_seg[RAM_DEV], (u16_t *) &s, (vir_bytes *) &s,
MINIX SOURCE CODE File: drivers/memory/memory.c 787
11895 ramdev_base, ramdev_size))) {
11896 panic("MEM","Couldn’t install remote segment.",s);
11897 }
11898 break;
11899 }
11900
11901 default:
11902 return(do_diocntl(&m_dtab, m_ptr));
11903 }
11904 return(OK);
11905 }
11907 /*===========================================================================*
11908 * m_geometry *
11909 *===========================================================================*/
11910 PRIVATE void m_geometry(entry)
11911 struct partition *entry;
11912 {
11913 /* Memory devices don’t have a geometry, but the outside world insists. */
11914 entry->cylinders = div64u(m_geom[m_device].dv_size, SECTOR_SIZE) / (64 * 32);
11915 entry->heads = 64;
11916 entry->sectors = 32;
11917 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
drivers/at_wini/at_wini.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
12000 #include "../drivers.h"
12001 #include "../libdriver/driver.h"
12002 #include "../libdriver/drvlib.h"
12003
12004 _PROTOTYPE(int main, (void));
12005
12006 #define VERBOSE 0 /* display identify messages during boot */
12007 #define ENABLE_ATAPI 0 /* add ATAPI cd-rom support to driver */
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
drivers/at_wini/at_wini.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
12100 /* This file contains the device dependent part of a driver for the IBM-AT
12101 * winchester controller. Written by Adri Koppes.
12102 *
12103 * The file contains one entry point:
12104 *
12105 * at_winchester_task: main entry when system is brought up
12106 *
12107 * Changes:
12108 * Aug 19, 2005 ata pci support, supports SATA (Ben Gras)
12109 * Nov 18, 2004 moved AT disk driver to user-space (Jorrit N. Herder)
12110 * Aug 20, 2004 watchdogs replaced by sync alarms (Jorrit N. Herder)
12111 * Mar 23, 2000 added ATAPI CDROM support (Michael Temari)
12112 * May 14, 2000 d-d/i rewrite (Kees J. Bot)
12113 * Apr 13, 1992 device dependent/independent split (Kees J. Bot)
12114 */
788 File: drivers/at_wini/at_wini.c MINIX SOURCE CODE
12115
12116 #include "at_wini.h"
12117 #include "../libpci/pci.h"
12118
12119 #include <minix/sysutil.h>
12120 #include <minix/keymap.h>
12121 #include <sys/ioc_disk.h>
12122
12123 #define ATAPI_DEBUG 0 /* To debug ATAPI code. */
12124
12125 /* I/O Ports used by winchester disk controllers. */
12126
12127 /* Read and write registers */
12128 #define REG_CMD_BASE0 0x1F0 /* command base register of controller 0 */
12129 #define REG_CMD_BASE1 0x170 /* command base register of controller 1 */
12130 #define REG_CTL_BASE0 0x3F6 /* control base register of controller 0 */
12131 #define REG_CTL_BASE1 0x376 /* control base register of controller 1 */
12132
12133 #define REG_DATA 0 /* data register (offset from the base reg.) */
12134 #define REG_PRECOMP 1 /* start of write precompensation */
12135 #define REG_COUNT 2 /* sectors to transfer */
12136 #define REG_SECTOR 3 /* sector number */
12137 #define REG_CYL_LO 4 /* low byte of cylinder number */
12138 #define REG_CYL_HI 5 /* high byte of cylinder number */
12139 #define REG_LDH 6 /* lba, drive and head */
12140 #define LDH_DEFAULT 0xA0 /* ECC enable, 512 bytes per sector */
12141 #define LDH_LBA 0x40 /* Use LBA addressing */
12142 #define ldh_init(drive) (LDH_DEFAULT | ((drive) << 4))
12143
12144 /* Read only registers */
12145 #define REG_STATUS 7 /* status */
12146 #define STATUS_BSY 0x80 /* controller busy */
12147 #define STATUS_RDY 0x40 /* drive ready */
12148 #define STATUS_WF 0x20 /* write fault */
12149 #define STATUS_SC 0x10 /* seek complete (obsolete) */
12150 #define STATUS_DRQ 0x08 /* data transfer request */
12151 #define STATUS_CRD 0x04 /* corrected data */
12152 #define STATUS_IDX 0x02 /* index pulse */
12153 #define STATUS_ERR 0x01 /* error */
12154 #define STATUS_ADMBSY 0x100 /* administratively busy (software) */
12155 #define REG_ERROR 1 /* error code */
12156 #define ERROR_BB 0x80 /* bad block */
12157 #define ERROR_ECC 0x40 /* bad ecc bytes */
12158 #define ERROR_ID 0x10 /* id not found */
12159 #define ERROR_AC 0x04 /* aborted command */
12160 #define ERROR_TK 0x02 /* track zero error */
12161 #define ERROR_DM 0x01 /* no data address mark */
12162
12163 /* Write only registers */
12164 #define REG_COMMAND 7 /* command */
12165 #define CMD_IDLE 0x00 /* for w_command: drive idle */
12166 #define CMD_RECALIBRATE 0x10 /* recalibrate drive */
12167 #define CMD_READ 0x20 /* read data */
12168 #define CMD_READ_EXT 0x24 /* read data (LBA48 addressed) */
12169 #define CMD_WRITE 0x30 /* write data */
12170 #define CMD_WRITE_EXT 0x34 /* write data (LBA48 addressed) */
12171 #define CMD_READVERIFY 0x40 /* read verify */
12172 #define CMD_FORMAT 0x50 /* format track */
12173 #define CMD_SEEK 0x70 /* seek cylinder */
12174 #define CMD_DIAG 0x90 /* execute device diagnostics */
MINIX SOURCE CODE File: drivers/at_wini/at_wini.c 789
12175 #define CMD_SPECIFY 0x91 /* specify parameters */
12176 #define ATA_IDENTIFY 0xEC /* identify drive */
12177 /* #define REG_CTL 0x206 */ /* control register */
12178 #define REG_CTL 0 /* control register */
12179 #define CTL_NORETRY 0x80 /* disable access retry */
12180 #define CTL_NOECC 0x40 /* disable ecc retry */
12181 #define CTL_EIGHTHEADS 0x08 /* more than eight heads */
12182 #define CTL_RESET 0x04 /* reset controller */
12183 #define CTL_INTDISABLE 0x02 /* disable interrupts */
12184
12185 #define REG_STATUS 7 /* status */
12186 #define STATUS_BSY 0x80 /* controller busy */
12187 #define STATUS_DRDY 0x40 /* drive ready */
12188 #define STATUS_DMADF 0x20 /* dma ready/drive fault */
12189 #define STATUS_SRVCDSC 0x10 /* service or dsc */
12190 #define STATUS_DRQ 0x08 /* data transfer request */
12191 #define STATUS_CORR 0x04 /* correctable error occurred */
12192 #define STATUS_CHECK 0x01 /* check error */
12193
12194 /* Interrupt request lines. */
12195 #define NO_IRQ 0 /* no IRQ set yet */
12196
12197 #define ATAPI_PACKETSIZE 12
12198 #define SENSE_PACKETSIZE 18
12199
12200 /* Common command block */
12201 struct command {
12202 u8_t precomp; /* REG_PRECOMP, etc. */
12203 u8_t count;
12204 u8_t sector;
12205 u8_t cyl_lo;
12206 u8_t cyl_hi;
12207 u8_t ldh;
12208 u8_t command;
12209 };
12210
12211 /* Error codes */
12212 #define ERR (-1) /* general error */
12213 #define ERR_BAD_SECTOR (-2) /* block marked bad detected */
12214
12215 /* Some controllers don’t interrupt, the clock will wake us up. */
12216 #define WAKEUP (32*HZ) /* drive may be out for 31 seconds max */
12217
12218 /* Miscellaneous. */
12219 #define MAX_DRIVES 8
12220 #define COMPAT_DRIVES 4
12221 #define MAX_SECS 256 /* controller can transfer this many sectors */
12222 #define MAX_ERRORS 4 /* how often to try rd/wt before quitting */
12223 #define NR_MINORS (MAX_DRIVES * DEV_PER_DRIVE)
12224 #define SUB_PER_DRIVE (NR_PARTITIONS * NR_PARTITIONS)
12225 #define NR_SUBDEVS (MAX_DRIVES * SUB_PER_DRIVE)
12226 #define DELAY_USECS 1000 /* controller timeout in microseconds */
12227 #define DELAY_TICKS 1 /* controller timeout in ticks */
12228 #define DEF_TIMEOUT_TICKS 300 /* controller timeout in ticks */
12229 #define RECOVERY_USECS 500000 /* controller recovery time in microseconds */
12230 #define RECOVERY_TICKS 30 /* controller recovery time in ticks */
12231 #define INITIALIZED 0x01 /* drive is initialized */
12232 #define DEAF 0x02 /* controller must be reset */
12233 #define SMART 0x04 /* drive supports ATA commands */
12234 #define ATAPI 0 /* don’t bother with ATAPI; optimise out */
790 File: drivers/at_wini/at_wini.c MINIX SOURCE CODE
12235 #define IDENTIFIED 0x10 /* w_identify done successfully */
12236 #define IGNORING 0x20 /* w_identify failed once */
12237
12238 /* Timeouts and max retries. */
12239 int timeout_ticks = DEF_TIMEOUT_TICKS, max_errors = MAX_ERRORS;
12240 int wakeup_ticks = WAKEUP;
12241 long w_standard_timeouts = 0, w_pci_debug = 0, w_instance = 0,
12242 w_lba48 = 0, atapi_debug = 0;
12243
12244 int w_testing = 0, w_silent = 0;
12245
12246 int w_next_drive = 0;
12247
12248 /* Variables. */
12249
12250 /* wini is indexed by controller first, then drive (0-3).
12251 * controller 0 is always the ’compatability’ ide controller, at
12252 * the fixed locations, whether present or not.
12253 */
12254 PRIVATE struct wini { /* main drive struct, one entry per drive */
12255 unsigned state; /* drive state: deaf, initialized, dead */
12256 unsigned w_status; /* device status register */
12257 unsigned base_cmd; /* command base register */
12258 unsigned base_ctl; /* control base register */
12259 unsigned irq; /* interrupt request line */
12260 unsigned irq_mask; /* 1 << irq */
12261 unsigned irq_need_ack; /* irq needs to be acknowledged */
12262 int irq_hook_id; /* id of irq hook at the kernel */
12263 int lba48; /* supports lba48 */
12264 unsigned lcylinders; /* logical number of cylinders (BIOS) */
12265 unsigned lheads; /* logical number of heads */
12266 unsigned lsectors; /* logical number of sectors per track */
12267 unsigned pcylinders; /* physical number of cylinders (translated) */
12268 unsigned pheads; /* physical number of heads */
12269 unsigned psectors; /* physical number of sectors per track */
12270 unsigned ldhpref; /* top four bytes of the LDH (head) register */
12271 unsigned precomp; /* write precompensation cylinder / 4 */
12272 unsigned max_count; /* max request for this drive */
12273 unsigned open_ct; /* in-use count */
12274 struct device part[DEV_PER_DRIVE]; /* disks and partitions */
12275 struct device subpart[SUB_PER_DRIVE]; /* subpartitions */
12276 } wini[MAX_DRIVES], *w_wn;
12277
12278 PRIVATE int w_device = -1;
12279 PRIVATE int w_controller = -1;
12280 PRIVATE int w_major = -1;
12281 PRIVATE char w_id_string[40];
12282
12283 PRIVATE int win_tasknr; /* my task number */
12284 PRIVATE int w_command; /* current command in execution */
12285 PRIVATE u8_t w_byteval; /* used for SYS_IRQCTL */
12286 PRIVATE int w_drive; /* selected drive */
12287 PRIVATE int w_controller; /* selected controller */
12288 PRIVATE struct device *w_dv; /* device’s base and size */
12289
12290 FORWARD _PROTOTYPE( void init_params, (void) );
12291 FORWARD _PROTOTYPE( void init_drive, (struct wini *, int, int, int, int, int, int));
12292 FORWARD _PROTOTYPE( void init_params_pci, (int) );
12293 FORWARD _PROTOTYPE( int w_do_open, (struct driver *dp, message *m_ptr) );
12294 FORWARD _PROTOTYPE( struct device *w_prepare, (int dev) );
MINIX SOURCE CODE File: drivers/at_wini/at_wini.c 791
12295 FORWARD _PROTOTYPE( int w_identify, (void) );
12296 FORWARD _PROTOTYPE( char *w_name, (void) );
12297 FORWARD _PROTOTYPE( int w_specify, (void) );
12298 FORWARD _PROTOTYPE( int w_io_test, (void) );
12299 FORWARD _PROTOTYPE( int w_transfer, (int proc_nr, int opcode, off_t position,
12300 iovec_t *iov, unsigned nr_req) );
12301 FORWARD _PROTOTYPE( int com_out, (struct command *cmd) );
12302 FORWARD _PROTOTYPE( void w_need_reset, (void) );
12303 FORWARD _PROTOTYPE( void ack_irqs, (unsigned int) );
12304 FORWARD _PROTOTYPE( int w_do_close, (struct driver *dp, message *m_ptr) );
12305 FORWARD _PROTOTYPE( int w_other, (struct driver *dp, message *m_ptr) );
12306 FORWARD _PROTOTYPE( int w_hw_int, (struct driver *dp, message *m_ptr) );
12307 FORWARD _PROTOTYPE( int com_simple, (struct command *cmd) );
12308 FORWARD _PROTOTYPE( void w_timeout, (void) );
12309 FORWARD _PROTOTYPE( int w_reset, (void) );
12310 FORWARD _PROTOTYPE( void w_intr_wait, (void) );
12311 FORWARD _PROTOTYPE( int at_intr_wait, (void) );
12312 FORWARD _PROTOTYPE( int w_waitfor, (int mask, int value) );
12313 FORWARD _PROTOTYPE( void w_geometry, (struct partition *entry) );
12314
12315 /* Entry points to this driver. */
12316 PRIVATE struct driver w_dtab = {
12317 w_name, /* current device’s name */
12318 w_do_open, /* open or mount request, initialize device */
12319 w_do_close, /* release device */
12320 do_diocntl, /* get or set a partition’s geometry */
12321 w_prepare, /* prepare for I/O on a given minor device */
12322 w_transfer, /* do the I/O */
12323 nop_cleanup, /* nothing to clean up */
12324 w_geometry, /* tell the geometry of the disk */
12325 nop_signal, /* no cleanup needed on shutdown */
12326 nop_alarm, /* ignore leftover alarms */
12327 nop_cancel, /* ignore CANCELs */
12328 nop_select, /* ignore selects */
12329 w_other, /* catch-all for unrecognized commands and ioctls */
12330 w_hw_int /* leftover hardware interrupts */
12331 };
12332
12333 /*===========================================================================*
12334 * at_winchester_task *
12335 *===========================================================================*/
12336 PUBLIC int main()
12337 {
12338 /* Set special disk parameters then call the generic main loop. */
12339 init_params();
12340 driver_task(&w_dtab);
12341 return(OK);
12342 }
12344 /*===========================================================================*
12345 * init_params *
12346 *===========================================================================*/
12347 PRIVATE void init_params()
12348 {
12349 /* This routine is called at startup to initialize the drive parameters. */
12350
12351 u16_t parv[2];
12352 unsigned int vector, size;
12353 int drive, nr_drives;
12354 struct wini *wn;
792 File: drivers/at_wini/at_wini.c MINIX SOURCE CODE
12355 u8_t params[16];
12356 int s;
12357
12358 /* Boot variables. */
12359 env_parse("ata_std_timeout", "d", 0, &w_standard_timeouts, 0, 1);
12360 env_parse("ata_pci_debug", "d", 0, &w_pci_debug, 0, 1);
12361 env_parse("ata_instance", "d", 0, &w_instance, 0, 8);
12362 env_parse("ata_lba48", "d", 0, &w_lba48, 0, 1);
12363 env_parse("atapi_debug", "d", 0, &atapi_debug, 0, 1);
12364
12365 if (w_instance == 0) {
12366 /* Get the number of drives from the BIOS data area */
12367 if ((s=sys_vircopy(SELF, BIOS_SEG, NR_HD_DRIVES_ADDR,
12368 SELF, D, (vir_bytes) params, NR_HD_DRIVES_SIZE)) != OK)
12369 panic(w_name(), "Couldn’t read BIOS", s);
12370 if ((nr_drives = params[0]) > 2) nr_drives = 2;
12371
12372 for (drive = 0, wn = wini; drive < COMPAT_DRIVES; drive++, wn++) {
12373 if (drive < nr_drives) {
12374 /* Copy the BIOS parameter vector */
12375 vector = (drive == 0) ? BIOS_HD0_PARAMS_ADDR:BIOS_HD1_PARAMS_ADDR;
12376 size = (drive == 0) ? BIOS_HD0_PARAMS_SIZE:BIOS_HD1_PARAMS_SIZE;
12377 if ((s=sys_vircopy(SELF, BIOS_SEG, vector,
12378 SELF, D, (vir_bytes) parv, size)) != OK)
12379 panic(w_name(), "Couldn’t read BIOS", s);
12380
12381 /* Calculate the address of the parameters and copy them */
12382 if ((s=sys_vircopy(
12383 SELF, BIOS_SEG, hclick_to_physb(parv[1]) + parv[0],
12384 SELF, D, (phys_bytes) params, 16L))!=OK)
12385 panic(w_name(),"Couldn’t copy parameters", s);
12386
12387 /* Copy the parameters to the structures of the drive */
12388 wn->lcylinders = bp_cylinders(params);
12389 wn->lheads = bp_heads(params);
12390 wn->lsectors = bp_sectors(params);
12391 wn->precomp = bp_precomp(params) >> 2;
12392 }
12393
12394 /* Fill in non-BIOS parameters. */
12395 init_drive(wn,
12396 drive < 2 ? REG_CMD_BASE0 : REG_CMD_BASE1,
12397 drive < 2 ? REG_CTL_BASE0 : REG_CTL_BASE1,
12398 NO_IRQ, 0, 0, drive);
12399 w_next_drive++;
12400 }
12401 }
12402
12403 /* Look for controllers on the pci bus. Skip none the first instance,
12404 * skip one and then 2 for every instance, for every next instance.
12405 */
12406 if (w_instance == 0)
12407 init_params_pci(0);
12408 else
12409 init_params_pci(w_instance*2-1);
12410
12411 }
12413 #define ATA_IF_NOTCOMPAT1 (1L << 0)
12414 #define ATA_IF_NOTCOMPAT2 (1L << 2)
MINIX SOURCE CODE File: drivers/at_wini/at_wini.c 793
12415
12416 /*===========================================================================*
12417 * init_drive *
12418 *===========================================================================*/
12419 PRIVATE void init_drive(struct wini *w int base_cmd int base_ctl int irq int ack ...
12420 {
12421 w->state = 0;
12422 w->w_status = 0;
12423 w->base_cmd = base_cmd;
12424 w->base_ctl = base_ctl;
12425 w->irq = irq;
12426 w->irq_mask = 1 << irq;
12427 w->irq_need_ack = ack;
12428 w->irq_hook_id = hook;
12429 w->ldhpref = ldh_init(drive);
12430 w->max_count = MAX_SECS << SECTOR_SHIFT;
12431 w->lba48 = 0;
12432 }
12434 /*===========================================================================*
12435 * init_params_pci *
12436 *===========================================================================*/
12437 PRIVATE void init_params_pci(int skip)
12438 {
12439 int r, devind, drive;
12440 u16_t vid, did;
12441 pci_init();
12442 for(drive = w_next_drive; drive < MAX_DRIVES; drive++)
12443 wini[drive].state = IGNORING;
12444 for(r = pci_first_dev(&devind, &vid, &did);
12445 r!=0&&w_next_drive<MAX_DRIVES; r=pci_next_dev(&devind,&vid, &did)) {
12446 int interface, irq, irq_hook;
12447 /* Base class must be 01h (mass storage), subclass must
12448 * be 01h (ATA).
12449 */
12450 if (pci_attr_r8(devind, PCI_BCR) != 0x01 ||
12451 pci_attr_r8(devind, PCI_SCR) != 0x01) {
12452 continue;
12453 }
12454 /* Found a controller.
12455 * Programming interface register tells us more.
12456 */
12457 interface = pci_attr_r8(devind, PCI_PIFR);
12458 irq = pci_attr_r8(devind, PCI_ILR);
12459
12460 /* Any non-compat drives? */
12461 if (interface & (ATA_IF_NOTCOMPAT1 | ATA_IF_NOTCOMPAT2)) {
12462 int s;
12463 irq_hook = irq;
12464 if (skip > 0) {
12465 if(w_pci_debug)printf("atapci skipping contr. (remain %d)\n",skip);
12466 skip--;
12467 continue;
12468 }
12469 if ((s=sys_irqsetpolicy(irq, 0, &irq_hook)) != OK) {
12470 printf("atapci: couldn’t set IRQ policy %d\n", irq);
12471 continue;
12472 }
12473 if ((s=sys_irqenable(&irq_hook)) != OK) {
12474 printf("atapci: couldn’t enable IRQ line %d\n", irq);
794 File: drivers/at_wini/at_wini.c MINIX SOURCE CODE
12475 continue;
12476 }
12477 } else {
12478 /* If not.. this is not the ata-pci controller we’re
12479 * looking for.
12480 */
12481 if (w_pci_debug) printf("atapci skipping compatability controller\n");
12482 continue;
12483 }
12484
12485 /* Primary channel not in compatability mode? */
12486 if (interface & ATA_IF_NOTCOMPAT1) {
12487 u32_t base_cmd, base_ctl;
12488 base_cmd = pci_attr_r32(devind, PCI_BAR) & 0xffffffe0;
12489 base_ctl = pci_attr_r32(devind, PCI_BAR_2) & 0xffffffe0;
12490 if (base_cmd != REG_CMD_BASE0 && base_cmd != REG_CMD_BASE1) {
12491 init_drive(&wini[w_next_drive],
12492 base_cmd, base_ctl, irq, 1, irq_hook, 0);
12493 init_drive(&wini[w_next_drive+1],
12494 base_cmd, base_ctl, irq, 1, irq_hook, 1);
12495 if (w_pci_debug)
12496 printf("atapci %d: 0x%x 0x%x irq %d\n",devind,base_cmd,base_ctl,irq)
12497 } else printf("atapci: ignored drives on pri, base: %x\n",base_cmd);
12498 }
12499
12500 /* Secondary channel not in compatability mode? */
12501 if (interface & ATA_IF_NOTCOMPAT2) {
12502 u32_t base_cmd, base_ctl;
12503 base_cmd = pci_attr_r32(devind, PCI_BAR_3) & 0xffffffe0;
12504 base_ctl = pci_attr_r32(devind, PCI_BAR_4) & 0xffffffe0;
12505 if (base_cmd != REG_CMD_BASE0 && base_cmd != REG_CMD_BASE1) {
12506 init_drive(&wini[w_next_drive+2],
12507 base_cmd, base_ctl, irq, 1, irq_hook, 2);
12508 init_drive(&wini[w_next_drive+3],
12509 base_cmd, base_ctl, irq, 1, irq_hook, 3);
12510 if (w_pci_debug)
12511 printf("atapci %d: 0x%x 0x%x irq %d\n",devind,base_cmd,base_ctl,irq);
12512 } else printf("atapci: ignored drives on secondary %x\n", base_cmd);
12513 }
12514 w_next_drive += 4;
12515 }
12516 }
12518 /*===========================================================================*
12519 * w_do_open *
12520 *===========================================================================*/
12521 PRIVATE int w_do_open(dp, m_ptr)
12522 struct driver *dp;
12523 message *m_ptr;
12524 {
12525 /* Device open: Initialize the controller and read the partition table. */
12526
12527 struct wini *wn;
12528
12529 if (w_prepare(m_ptr->DEVICE) == NIL_DEV) return(ENXIO);
12530
12531 wn = w_wn;
12532
12533 /* If we’ve probed it before and it failed, don’t probe it again. */
12534 if (wn->state & IGNORING) return ENXIO;
MINIX SOURCE CODE File: drivers/at_wini/at_wini.c 795
12535
12536 /* If we haven’t identified it yet, or it’s gone deaf,
12537 * (re-)identify it.
12538 */
12539 if (!(wn->state & IDENTIFIED) || (wn->state & DEAF)) {
12540 /* Try to identify the device. */
12541 if (w_identify() != OK) {
12542 if (wn->state & DEAF) w_reset();
12543 wn->state = IGNORING;
12544 return(ENXIO);
12545 }
12546 /* Do a test transaction unless it’s a CD drive (then
12547 * we can believe the controller, and a test may fail
12548 * due to no CD being in the drive). If it fails, ignore
12549 * the device forever.
12550 */
12551 if (!(wn->state & ATAPI) && w_io_test() != OK) {
12552 wn->state |= IGNORING;
12553 return(ENXIO);
12554 }
12555 }
12556
12557 /* If it’s not an ATAPI device, then don’t open with RO_BIT. */
12558 if (!(wn->state & ATAPI) && (m_ptr->COUNT & RO_BIT)) return EACCES;
12559
12560 /* Partition the drive if it’s being opened for the first time,
12561 * or being opened after being closed.
12562 */
12563 if (wn->open_ct == 0) {
12564
12565 /* Partition the disk. */
12566 memset(wn->part, sizeof(wn->part), 0);
12567 memset(wn->subpart, sizeof(wn->subpart), 0);
12568 partition(&w_dtab, w_drive * DEV_PER_DRIVE, P_PRIMARY, wn->state & ATAPI);
12569 }
12570 wn->open_ct++;
12571 return(OK);
12572 }
12574 /*===========================================================================*
12575 * w_prepare *
12576 *===========================================================================*/
12577 PRIVATE struct device *w_prepare(int device)
12578 {
12579 /* Prepare for I/O on a device. */
12580 struct wini *prev_wn;
12581 prev_wn = w_wn;
12582 w_device = device;
12583
12584 if (device < NR_MINORS) { /* d0, d0p[0-3], d1, ... */
12585 w_drive = device / DEV_PER_DRIVE; /* save drive number */
12586 w_wn = &wini[w_drive];
12587 w_dv = &w_wn->part[device % DEV_PER_DRIVE];
12588 } else
12589 if ((unsigned) (device -= MINOR_d0p0s0) < NR_SUBDEVS) {/*d[0-7]p[0-3]s[0-3]*/
12590 w_drive = device / SUB_PER_DRIVE;
12591 w_wn = &wini[w_drive];
12592 w_dv = &w_wn->subpart[device % SUB_PER_DRIVE];
12593 } else {
12594 w_device = -1;
796 File: drivers/at_wini/at_wini.c MINIX SOURCE CODE
12595 return(NIL_DEV);
12596 }
12597 return(w_dv);
12598 }
12600 /*===========================================================================*
12601 * w_identify *
12602 *===========================================================================*/
12603 PRIVATE int w_identify()
12604 {
12605 /* Find out if a device exists, if it is an old AT disk, or a newer ATA
12606 * drive, a removable media device, etc.
12607 */
12608
12609 struct wini *wn = w_wn;
12610 struct command cmd;
12611 int i, s;
12612 unsigned long size;
12613 #define id_byte(n) (&tmp_buf[2 * (n)])
12614 #define id_word(n) (((u16_t) id_byte(n)[0] << 0) \
12615 |((u16_t) id_byte(n)[1] << 8))
12616 #define id_longword(n) (((u32_t) id_byte(n)[0] << 0) \
12617 |((u32_t) id_byte(n)[1] << 8) \
12618 |((u32_t) id_byte(n)[2] << 16) \
12619 |((u32_t) id_byte(n)[3] << 24))
12620
12621 /* Try to identify the device. */
12622 cmd.ldh = wn->ldhpref;
12623 cmd.command = ATA_IDENTIFY;
12624 if (com_simple(&cmd) == OK) {
12625 /* This is an ATA device. */
12626 wn->state |= SMART;
12627
12628 /* Device information. */
12629 if ((s=sys_insw(wn->base_cmd + REG_DATA, SELF, tmp_buf, SECTOR_SIZE)) != OK)
12630 panic(w_name(),"Call to sys_insw() failed", s);
12631
12632 /* Why are the strings byte swapped??? */
12633 for (i = 0; i < 40; i++) w_id_string[i] = id_byte(27)[iˆ1];
12634
12635 /* Preferred CHS translation mode. */
12636 wn->pcylinders = id_word(1);
12637 wn->pheads = id_word(3);
12638 wn->psectors = id_word(6);
12639 size = (u32_t) wn->pcylinders * wn->pheads * wn->psectors;
12640
12641 if ((id_byte(49)[1] & 0x02) && size > 512L*1024*2) {
12642 /* Drive is LBA capable and is big enough to trust it to
12643 * not make a mess of it.
12644 */
12645 wn->ldhpref |= LDH_LBA;
12646 size = id_longword(60);
12647
12648 if (w_lba48 && ((id_word(83)) & (1L << 10))) {
12649 /* Drive is LBA48 capable (and LBA48 is turned on). */
12650 if (id_word(102) || id_word(103)) {
12651 /* If no. of sectors doesn’t fit in 32 bits,
12652 * trunacte to this. So it’s LBA32 for now.
12653 * This can still address devices up to 2TB
12654 * though.
MINIX SOURCE CODE File: drivers/at_wini/at_wini.c 797
12655 */
12656 size = ULONG_MAX;
12657 } else {
12658 /* Actual number of sectors fits in 32 bits. */
12659 size = id_longword(100);
12660 }
12661
12662 wn->lba48 = 1;
12663 }
12664 }
12665
12666 if (wn->lcylinders == 0) {
12667 /* No BIOS parameters? Then make some up. */
12668 wn->lcylinders = wn->pcylinders;
12669 wn->lheads = wn->pheads;
12670 wn->lsectors = wn->psectors;
12671 while (wn->lcylinders > 1024) {
12672 wn->lheads *= 2;
12673 wn->lcylinders /= 2;
12674 }
12675 }
12676 } else {
12677 /* Not an ATA device; no translations, no special features. Don’t
12678 * touch it unless the BIOS knows about it.
12679 */
12680 if (wn->lcylinders == 0) { return(ERR); } /* no BIOS parameters */
12681 wn->pcylinders = wn->lcylinders;
12682 wn->pheads = wn->lheads;
12683 wn->psectors = wn->lsectors;
12684 size = (u32_t) wn->pcylinders * wn->pheads * wn->psectors;
12685 }
12686
12687 /* Size of the whole drive */
12688 wn->part[0].dv_size = mul64u(size, SECTOR_SIZE);
12689
12690 /* Reset/calibrate (where necessary) */
12691 if (w_specify() != OK && w_specify() != OK) {
12692 return(ERR);
12693 }
12694
12695 if (wn->irq == NO_IRQ) {
12696 /* Everything looks OK; register IRQ so we can stop polling. */
12697 wn->irq = w_drive < 2 ? AT_WINI_0_IRQ : AT_WINI_1_IRQ;
12698 wn->irq_hook_id = wn->irq; /* id to be returned if interrupt occurs */
12699 if ((s=sys_irqsetpolicy(wn->irq, IRQ_REENABLE, &wn->irq_hook_id)) != OK)
12700 panic(w_name(), "couldn’t set IRQ policy", s);
12701 if ((s=sys_irqenable(&wn->irq_hook_id)) != OK)
12702 panic(w_name(), "couldn’t enable IRQ line", s);
12703 }
12704 wn->state |= IDENTIFIED;
12705 return(OK);
12706 }
12708 /*===========================================================================*
12709 * w_name *
12710 *===========================================================================*/
12711 PRIVATE char *w_name()
12712 {
12713 /* Return a name for the current device. */
12714 static char name[] = "AT-D0";
798 File: drivers/at_wini/at_wini.c MINIX SOURCE CODE
12715
12716 name[4] = ’0’ + w_drive;
12717 return name;
12718 }
12720 /*===========================================================================*
12721 * w_io_test *
12722 *===========================================================================*/
12723 PRIVATE int w_io_test(void)
12724 {
12725 int r, save_dev;
12726 int save_timeout, save_errors, save_wakeup;
12727 iovec_t iov;
12728 static char buf[SECTOR_SIZE];
12729 iov.iov_addr = (vir_bytes) buf;
12730 iov.iov_size = sizeof(buf);
12731 save_dev = w_device;
12732
12733 /* Reduce timeout values for this test transaction. */
12734 save_timeout = timeout_ticks;
12735 save_errors = max_errors;
12736 save_wakeup = wakeup_ticks;
12737
12738 if (!w_standard_timeouts) {
12739 timeout_ticks = HZ * 4;
12740 wakeup_ticks = HZ * 6;
12741 max_errors = 3;
12742 }
12743
12744 w_testing = 1;
12745
12746 /* Try I/O on the actual drive (not any (sub)partition). */
12747 if (w_prepare(w_drive * DEV_PER_DRIVE) == NIL_DEV)
12748 panic(w_name(), "Couldn’t switch devices", NO_NUM);
12749
12750 r = w_transfer(SELF, DEV_GATHER, 0, &iov, 1);
12751
12752 /* Switch back. */
12753 if (w_prepare(save_dev) == NIL_DEV)
12754 panic(w_name(), "Couldn’t switch back devices", NO_NUM);
12755
12756 /* Restore parameters. */
12757 timeout_ticks = save_timeout;
12758 max_errors = save_errors;
12759 wakeup_ticks = save_wakeup;
12760 w_testing = 0;
12761
12762 /* Test if everything worked. */
12763 if (r != OK || iov.iov_size != 0) {
12764 return ERR;
12765 }
12766
12767 /* Everything worked. */
12768
12769 return OK;
12770 }

12772 /*===========================================================================*
12773 * w_specify *
12774 *===========================================================================*/
12775 PRIVATE int w_specify()
12776 {
12777 /* Routine to initialize the drive after boot or when a reset is needed. */
12778
12779 struct wini *wn = w_wn;
12780 struct command cmd;
12781
12782 if ((wn->state & DEAF) && w_reset() != OK) {
12783 return(ERR);
12784 }
12785
12786 if (!(wn->state & ATAPI)) {
12787 /* Specify parameters: precompensation, number of heads and sectors. */
12788 cmd.precomp = wn->precomp;
12789 cmd.count = wn->psectors;
12790 cmd.ldh = w_wn->ldhpref | (wn->pheads - 1);
12791 cmd.command = CMD_SPECIFY; /* Specify some parameters */
12792
12793 /* Output command block and see if controller accepts the parameters. */
12794 if (com_simple(&cmd) != OK) return(ERR);
12795
12796 if (!(wn->state & SMART)) {
12797 /* Calibrate an old disk. */
12798 cmd.sector = 0;
12799 cmd.cyl_lo = 0;
12800 cmd.cyl_hi = 0;
12801 cmd.ldh = w_wn->ldhpref;
12802 cmd.command = CMD_RECALIBRATE;
12803
12804 if (com_simple(&cmd) != OK) return(ERR);
12805 }
12806 }
12807 wn->state |= INITIALIZED;
12808 return(OK);
12809 }
12811 /*===========================================================================*
12812 * do_transfer *
12813 *===========================================================================*/
12814 PRIVATE int do_transfer(struct wini *wn, unsigned int precomp, unsigned int count,
12815 unsigned int sector, unsigned int opcode)
12816 {
12817 struct command cmd;
12818 unsigned secspcyl = wn->pheads * wn->psectors;
12819
12820 cmd.precomp = precomp;
12821 cmd.count = count;
12822 cmd.command = opcode == DEV_SCATTER ? CMD_WRITE : CMD_READ;
12823 /*
12824 if (w_lba48 && wn->lba48) {
12825 } else */
12826 if (wn->ldhpref & LDH_LBA) {
12827 cmd.sector = (sector >> 0) & 0xFF;
12828 cmd.cyl_lo = (sector >> 8) & 0xFF;
12829 cmd.cyl_hi = (sector >> 16) & 0xFF;
12830 cmd.ldh = wn->ldhpref | ((sector >> 24) & 0xF);
12831 } else {
800 File: drivers/at_wini/at_wini.c MINIX SOURCE CODE
12832 int cylinder, head, sec;
12833 cylinder = sector / secspcyl;
12834 head = (sector % secspcyl) / wn->psectors;
12835 sec = sector % wn->psectors;
12836 cmd.sector = sec + 1;
12837 cmd.cyl_lo = cylinder & BYTE;
12838 cmd.cyl_hi = (cylinder >> 8) & BYTE;
12839 cmd.ldh = wn->ldhpref | head;
12840 }
12841
12842 return com_out(&cmd);
12843 }
12845 /*===========================================================================*
12846 * w_transfer *
12847 *===========================================================================*/
12848 PRIVATE int w_transfer(proc_nr, opcode, position, iov, nr_req)
12849 int proc_nr; /* process doing the request */
12850 int opcode; /* DEV_GATHER or DEV_SCATTER */
12851 off_t position; /* offset on device to read or write */
12852 iovec_t *iov; /* pointer to read or write request vector */
12853 unsigned nr_req; /* length of request vector */
12854 {
12855 struct wini *wn = w_wn;
12856 iovec_t *iop, *iov_end = iov + nr_req;
12857 int r, s, errors;
12858 unsigned long block;
12859 unsigned long dv_size = cv64ul(w_dv->dv_size);
12860 unsigned cylinder, head, sector, nbytes;
12861
12862 /* Check disk address. */
12863 if ((position & SECTOR_MASK) != 0) return(EINVAL);
12864
12865 errors = 0;
12866
12867 while (nr_req > 0) {
12868 /* How many bytes to transfer? */
12869 nbytes = 0;
12870 for (iop = iov; iop < iov_end; iop++) nbytes += iop->iov_size;
12871 if ((nbytes & SECTOR_MASK) != 0) return(EINVAL);
12872
12873 /* Which block on disk and how close to EOF? */
12874 if (position >= dv_size) return(OK); /* At EOF */
12875 if (position + nbytes > dv_size) nbytes = dv_size - position;
12876 block = div64u(add64ul(w_dv->dv_base, position), SECTOR_SIZE);
12877
12878 if (nbytes >= wn->max_count) {
12879 /* The drive can’t do more then max_count at once. */
12880 nbytes = wn->max_count;
12881 }
12882
12883 /* First check to see if a reinitialization is needed. */
12884 if (!(wn->state & INITIALIZED) && w_specify() != OK) return(EIO);
12885
12886 /* Tell the controller to transfer nbytes bytes. */
12887 r = do_transfer(wn, wn->precomp, ((nbytes >> SECTOR_SHIFT) & BYTE),
12888 block, opcode);
12889
12890 while (r == OK && nbytes > 0) {
12891 /* For each sector, wait for an interrupt and fetch the data
MINIX SOURCE CODE File: drivers/at_wini/at_wini.c 801
12892 * (read), or supply data to the controller and wait for an
12893 * interrupt (write).
12894 */
12895
12896 if (opcode == DEV_GATHER) {
12897 /* First an interrupt, then data. */
12898 if ((r = at_intr_wait()) != OK) {
12899 /* An error, send data to the bit bucket. */
12900 if (w_wn->w_status & STATUS_DRQ) {
12901 if ((s=sys_insw(wn->base_cmd + REG_DATA, SELF, tmp_buf, SECTOR_SIZE)) != OK)
12902 panic(w_name(),"Call to sys_insw() failed", s);
12903 }
12904 break;
12905 }
12906 }
12907
12908 /* Wait for data transfer requested. */
12909 if (!w_waitfor(STATUS_DRQ, STATUS_DRQ)) { r = ERR; break; }
12910
12911 /* Copy bytes to or from the device’s buffer. */
12912 if (opcode == DEV_GATHER) { if((s=sys_insw(wn->base_cmd+REG_DATA,
12913 proc_nr,(void*)iov->iov_addr,SECTOR_SIZE))!=OK)
12914 panic(w_name(),"Call to sys_insw() failed", s);
12915 } else { if((s=sys_outsw(wn->base_cmd+REG_DATA,proc_nr,
12916 (void *) iov->iov_addr,SECTOR_SIZE))!=OK)
12917 panic(w_name(),"Call to sys_insw() failed", s);
12918
12919 /* Data sent, wait for an interrupt. */
12920 if ((r = at_intr_wait()) != OK) break;
12921 }
12922
12923 /* Book the bytes successfully transferred. */
12924 nbytes -= SECTOR_SIZE;
12925 position += SECTOR_SIZE;
12926 iov->iov_addr += SECTOR_SIZE;
12927 if ((iov->iov_size -= SECTOR_SIZE) == 0) { iov++; nr_req--; }
12928 }
12929
12930 /* Any errors? */
12931 if (r != OK) {
12932 /* Don’t retry if sector marked bad or too many errors. */
12933 if (r == ERR_BAD_SECTOR || ++errors == max_errors) {
12934 w_command = CMD_IDLE;
12935 return(EIO);
12936 }
12937 }
12938 }
12939
12940 w_command = CMD_IDLE;
12941 return(OK);
12942 }
12944 /*===========================================================================*
12945 * com_out *
12946 *===========================================================================*/
12947 PRIVATE int com_out(cmd)
12948 struct command *cmd; /* Command block */
12949 {
12950 /* Output the command block to the winchester controller and return status */
12951
802 File: drivers/at_wini/at_wini.c MINIX SOURCE CODE
12952 struct wini *wn = w_wn;
12953 unsigned base_cmd = wn->base_cmd;
12954 unsigned base_ctl = wn->base_ctl;
12955 pvb_pair_t outbyte[7]; /* vector for sys_voutb() */
12956 int s; /* status for sys_(v)outb() */
12957
12958 if (w_wn->state & IGNORING) return ERR;
12959
12960 if (!w_waitfor(STATUS_BSY, 0)) {
12961 printf("%s: controller not ready\n", w_name());
12962 return(ERR);
12963 }
12964
12965 /* Select drive. */
12966 if ((s=sys_outb(base_cmd + REG_LDH, cmd->ldh)) != OK)
12967 panic(w_name(),"Couldn’t write register to select drive",s);
12968
12969 if (!w_waitfor(STATUS_BSY, 0)) {
12970 printf("%s: com_out: drive not ready\n", w_name());
12971 return(ERR);
12972 }
12973
12974 /* Schedule a wakeup call, some controllers are flaky. This is done with
12975 * a synchronous alarm. If a timeout occurs a SYN_ALARM message is sent
12976 * from HARDWARE, so that w_intr_wait() can call w_timeout() in case the
12977 * controller was not able to execute the command. Leftover timeouts are
12978 * simply ignored by the main loop.
12979 */
12980 sys_setalarm(wakeup_ticks, 0);
12981
12982 wn->w_status = STATUS_ADMBSY;
12983 w_command = cmd->command;
12984 pv_set(outbyte[0], base_ctl + REG_CTL, wn->pheads >= 8 ? CTL_EIGHTHEADS : 0);
12985 pv_set(outbyte[1], base_cmd + REG_PRECOMP, cmd->precomp);
12986 pv_set(outbyte[2], base_cmd + REG_COUNT, cmd->count);
12987 pv_set(outbyte[3], base_cmd + REG_SECTOR, cmd->sector);
12988 pv_set(outbyte[4], base_cmd + REG_CYL_LO, cmd->cyl_lo);
12989 pv_set(outbyte[5], base_cmd + REG_CYL_HI, cmd->cyl_hi);
12990 pv_set(outbyte[6], base_cmd + REG_COMMAND, cmd->command);
12991 if ((s=sys_voutb(outbyte,7)) != OK)
12992 panic(w_name(),"Couldn’t write registers with sys_voutb()",s);
12993 return(OK);
12994 }
12996 /*===========================================================================*
12997 * w_need_reset *
12998 *===========================================================================*/
12999 PRIVATE void w_need_reset()
13000 {
13001 /* The controller needs to be reset. */
13002 struct wini *wn;
13003 int dr = 0;
13004
13005 for (wn = wini; wn < &wini[MAX_DRIVES]; wn++, dr++) {
13006 if (wn->base_cmd == w_wn->base_cmd) {
13007 wn->state |= DEAF;
13008 wn->state &= ˜INITIALIZED;
13009 }
13010 }
13011 }
MINIX SOURCE CODE File: drivers/at_wini/at_wini.c 803
13013 /*===========================================================================*
13014 * w_do_close *
13015 *===========================================================================*/
13016 PRIVATE int w_do_close(dp, m_ptr)
13017 struct driver *dp;
13018 message *m_ptr;
13019 {
13020 /* Device close: Release a device. */
13021 if (w_prepare(m_ptr->DEVICE) == NIL_DEV)
13022 return(ENXIO);
13023 w_wn->open_ct--;
13024 return(OK);
13025 }
13027 /*===========================================================================*
13028 * com_simple *
13029 *===========================================================================*/
13030 PRIVATE int com_simple(cmd)
13031 struct command *cmd; /* Command block */
13032 {
13033 /* A simple controller command, only one interrupt and no data-out phase. */
13034 int r;
13035
13036 if (w_wn->state & IGNORING) return ERR;
13037
13038 if ((r = com_out(cmd)) == OK) r = at_intr_wait();
13039 w_command = CMD_IDLE;
13040 return(r);
13041 }
13043 /*===========================================================================*
13044 * w_timeout *
13045 *===========================================================================*/
13046 PRIVATE void w_timeout(void)
13047 {
13048 struct wini *wn = w_wn;
13049
13050 switch (w_command) {
13051 case CMD_IDLE:
13052 break; /* fine */
13053 case CMD_READ:
13054 case CMD_WRITE:
13055 /* Impossible, but not on PC’s: The controller does not respond. */
13056
13057 /* Limiting multisector I/O seems to help. */
13058 if (wn->max_count > 8 * SECTOR_SIZE) {
13059 wn->max_count = 8 * SECTOR_SIZE;
13060 } else {
13061 wn->max_count = SECTOR_SIZE;
13062 }
13063 /*FALL THROUGH*/
13064 default:
13065 /* Some other command. */
13066 if (w_testing) wn->state |= IGNORING; /* Kick out this drive. */
13067 else if (!w_silent) printf("%s: timeout on command %02x\n", w_name(), w_command);
13068 w_need_reset();
13069 wn->w_status = 0;
13070 }
13071 }
804 File: drivers/at_wini/at_wini.c MINIX SOURCE CODE
13073 /*===========================================================================*
13074 * w_reset *
13075 *===========================================================================*/
13076 PRIVATE int w_reset()
13077 {
13078 /* Issue a reset to the controller. This is done after any catastrophe,
13079 * like the controller refusing to respond.
13080 */
13081 int s;
13082 struct wini *wn = w_wn;
13083
13084 /* Don’t bother if this drive is forgotten. */
13085 if (w_wn->state & IGNORING) return ERR;
13086
13087 /* Wait for any internal drive recovery. */
13088 tickdelay(RECOVERY_TICKS);
13089
13090 /* Strobe reset bit */
13091 if ((s=sys_outb(wn->base_ctl + REG_CTL, CTL_RESET)) != OK)
13092 panic(w_name(),"Couldn’t strobe reset bit",s);
13093 tickdelay(DELAY_TICKS);
13094 if ((s=sys_outb(wn->base_ctl + REG_CTL, 0)) != OK)
13095 panic(w_name(),"Couldn’t strobe reset bit",s);
13096 tickdelay(DELAY_TICKS);
13097
13098 /* Wait for controller ready */
13099 if (!w_waitfor(STATUS_BSY, 0)) {
13100 printf("%s: reset failed, drive busy\n", w_name());
13101 return(ERR);
13102 }
13103
13104 /* The error register should be checked now, but some drives mess it up. */
13105
13106 for (wn = wini; wn < &wini[MAX_DRIVES]; wn++) {
13107 if (wn->base_cmd == w_wn->base_cmd) {
13108 wn->state &= ˜DEAF;
13109 if (w_wn->irq_need_ack) {
13110 /* Make sure irq is actually enabled.. */
13111 sys_irqenable(&w_wn->irq_hook_id);
13112 }
13113 }
13114 }
13115
13116
13117 return(OK);
13118 }
13120 /*===========================================================================*
13121 * w_intr_wait *
13122 *===========================================================================*/
13123 PRIVATE void w_intr_wait()
13124 {
13125 /* Wait for a task completion interrupt. */
13126
13127 message m;
13128
13129 if (w_wn->irq != NO_IRQ) {
13130 /* Wait for an interrupt that sets w_status to "not busy". */
13131 while (w_wn->w_status & (STATUS_ADMBSY|STATUS_BSY)) {
MINIX SOURCE CODE File: drivers/at_wini/at_wini.c 805
13132 receive(ANY, &m); /* expect HARD_INT message */
13133 if (m.m_type == SYN_ALARM) { /* but check for timeout */
13134 w_timeout(); /* a.o. set w_status */
13135 } else if (m.m_type == HARD_INT) {
13136 sys_inb(w_wn->base_cmd + REG_STATUS, &w_wn->w_status);
13137 ack_irqs(m.NOTIFY_ARG);
13138 } else {
13139 printf("AT_WINI got unexpected message %d from %d\n",
13140 m.m_type, m.m_source);
13141 }
13142 }
13143 } else {
13144 /* Interrupt not yet allocated; use polling. */
13145 (void) w_waitfor(STATUS_BSY, 0);
13146 }
13147 }
13149 /*===========================================================================*
13150 * at_intr_wait *
13151 *===========================================================================*/
13152 PRIVATE int at_intr_wait()
13153 {
13154 /* Wait for an interrupt, study the status bits and return error/success. */
13155 int r;
13156 int s,inbval; /* read value with sys_inb */
13157
13158 w_intr_wait();
13159 if ((w_wn->w_status & (STATUS_BSY | STATUS_WF | STATUS_ERR)) == 0) {
13160 r = OK;
13161 } else {
13162 if ((s=sys_inb(w_wn->base_cmd + REG_ERROR, &inbval)) != OK)
13163 panic(w_name(),"Couldn’t read register",s);
13164 if ((w_wn->w_status & STATUS_ERR) && (inbval & ERROR_BB)) {
13165 r = ERR_BAD_SECTOR; /* sector marked bad, retries won’t help */
13166 } else {
13167 r = ERR; /* any other error */
13168 }
13169 }
13170 w_wn->w_status |= STATUS_ADMBSY; /* assume still busy with I/O */
13171 return(r);
13172 }
13174 /*===========================================================================*
13175 * w_waitfor *
13176 *===========================================================================*/
13177 PRIVATE int w_waitfor(mask, value)
13178 int mask; /* status mask */
13179 int value; /* required status */
13180 {
13181 /* Wait until controller is in the required state. Return zero on timeout.
13182 * An alarm that set a timeout flag is used. TIMEOUT is in micros, we need
13183 * ticks. Disabling the alarm is not needed, because a static flag is used
13184 * and a leftover timeout cannot do any harm.
13185 */
13186 clock_t t0, t1;
13187 int s;
13188 getuptime(&t0);
13189 do {
13190 if ((s=sys_inb(w_wn->base_cmd + REG_STATUS, &w_wn->w_status)) != OK)
13191 panic(w_name(),"Couldn’t read register",s);
806 File: drivers/at_wini/at_wini.c MINIX SOURCE CODE
13192 if ((w_wn->w_status & mask) == value) {
13193 return 1;
13194 }
13195 } while ((s=getuptime(&t1)) == OK && (t1-t0) < timeout_ticks );
13196 if (OK != s) printf("AT_WINI: warning, get_uptime failed: %d\n",s);
13197
13198 w_need_reset(); /* controller gone deaf */
13199 return(0);
13200 }
13202 /*===========================================================================*
13203 * w_geometry *
13204 *===========================================================================*/
13205 PRIVATE void w_geometry(entry)
13206 struct partition *entry;
13207 {
13208 struct wini *wn = w_wn;
13209
13210 if (wn->state & ATAPI) { /* Make up some numbers. */
13211 entry->cylinders = div64u(wn->part[0].dv_size, SECTOR_SIZE) / (64*32);
13212 entry->heads = 64;
13213 entry->sectors = 32;
13214 } else { /* Return logical geometry. */
13215 entry->cylinders = wn->lcylinders;
13216 entry->heads = wn->lheads;
13217 entry->sectors = wn->lsectors;
13218 }
13219 }
13221 /*===========================================================================*
13222 * w_other *
13223 *===========================================================================*/
13224 PRIVATE int w_other(dr, m)
13225 struct driver *dr;
13226 message *m;
13227 {
13228 int r, timeout, prev;
13229
13230 if (m->m_type != DEV_IOCTL ) {
13231 return EINVAL;
13232 }
13233
13234 if (m->REQUEST == DIOCTIMEOUT) {
13235 if ((r=sys_datacopy(m->PROC_NR, (vir_bytes)m->ADDRESS,
13236 SELF, (vir_bytes)&timeout, sizeof(timeout))) != OK)
13237 return r;
13238
13239 if (timeout == 0) {
13240 /* Restore defaults. */
13241 timeout_ticks = DEF_TIMEOUT_TICKS;
13242 max_errors = MAX_ERRORS;
13243 wakeup_ticks = WAKEUP;
13244 w_silent = 0;
13245 } else if (timeout < 0) {
13246 return EINVAL;
13247 } else {
13248 prev = wakeup_ticks;
13249
13250 if (!w_standard_timeouts) {
13251 /* Set (lower) timeout, lower error
MINIX SOURCE CODE File: drivers/at_wini/at_wini.c 807
13252 * tolerance and set silent mode.
13253 */
13254 wakeup_ticks = timeout;
13255 max_errors = 3;
13256 w_silent = 1;
13257
13258 if (timeout_ticks > timeout)
13259 timeout_ticks = timeout;
13260 }
13261
13262 if ((r=sys_datacopy(SELF, (vir_bytes)&prev,
13263 m->PROC_NR,(vir_bytes)m->ADDRESS,sizeof(prev)))!=OK)
13264 return r;
13265 }
13266
13267 return OK;
13268 } else if (m->REQUEST == DIOCOPENCT) {
13269 int count;
13270 if (w_prepare(m->DEVICE) == NIL_DEV) return ENXIO;
13271 count = w_wn->open_ct;
13272 if ((r=sys_datacopy(SELF, (vir_bytes)&count,
13273 m->PROC_NR, (vir_bytes)m->ADDRESS, sizeof(count))) != OK)
13274 return r;
13275 return OK;
13276 }
13277 return EINVAL;
13278 }
13280 /*===========================================================================*
13281 * w_hw_int *
13282 *===========================================================================*/
13283 PRIVATE int w_hw_int(dr, m)
13284 struct driver *dr;
13285 message *m;
13286 {
13287 /* Leftover interrupt(s) received; ack it/them. */
13288 ack_irqs(m->NOTIFY_ARG);
13289
13290 return OK;
13291 }
13294 /*===========================================================================*
13295 * ack_irqs *
13296 *===========================================================================*/
13297 PRIVATE void ack_irqs(unsigned int irqs)
13298 {
13299 unsigned int drive;
13300 for (drive = 0; drive < MAX_DRIVES && irqs; drive++) {
13301 if (!(wini[drive].state & IGNORING) && wini[drive].irq_need_ack &&
13302 (wini[drive].irq_mask & irqs)) {
13303 if (sys_inb((wini[drive].base_cmd+REG_STATUS),&wini[drive].w_status)!=OK)
13304 printf("couldn’t ack irq on drive %d\n", drive);
13305 if (sys_irqenable(&wini[drive].irq_hook_id) != OK)
13306 printf("couldn’t re-enable drive %d\n", drive);
13307 irqs &= ˜wini[drive].irq_mask;
13308 }
13309 }
13310 }
808 File: drivers/at_wini/at_wini.c MINIX SOURCE CODE
13313 #define STSTR(a) if (status & STATUS_ ## a) { strcat(str, #a); strcat(str, " "); }
13314 #define ERRSTR(a) if (e & ERROR_ ## a) { strcat(str, #a); strcat(str, " "); }
13315 char *strstatus(int status)
13316 {
13317 static char str[200];
13318 str[0] = ’\0’;
13319
13320 STSTR(BSY);
13321 STSTR(DRDY);
13322 STSTR(DMADF);
13323 STSTR(SRVCDSC);
13324 STSTR(DRQ);
13325 STSTR(CORR);
13326 STSTR(CHECK);
13327 return str;
13328 }
13330 char *strerr(int e)
13331 {
13332 static char str[200];
13333 str[0] = ’\0’;
13334
13335 ERRSTR(BB);
13336 ERRSTR(ECC);
13337 ERRSTR(ID);
13338 ERRSTR(AC);
13339 ERRSTR(TK);
13340 ERRSTR(DM);
13341
13342 return str;
13343 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
drivers/tty/tty.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
13400 /* tty.h - Terminals */
13401
13402 #include <timers.h>
13403
13404 /* First minor numbers for the various classes of TTY devices. */
13405 #define CONS_MINOR 0
13406 #define LOG_MINOR 15
13407 #define RS232_MINOR 16
13408 #define TTYPX_MINOR 128
13409 #define PTYPX_MINOR 192
13410
13411 #define LINEWRAP 1 /* console.c - wrap lines at column 80 */
13412
13413 #define TTY_IN_BYTES 256 /* tty input queue size */
13414 #define TAB_SIZE 8 /* distance between tab stops */
13415 #define TAB_MASK 7 /* mask to compute a tab stop position */
13416
13417 #define ESC ’\33’ /* escape */
13418
13419 #define O_NOCTTY 00400 /* from <fcntl.h>, or cc will choke */
MINIX SOURCE CODE File: drivers/tty/tty.h 809
13420 #define O_NONBLOCK 04000
13421
13422 struct tty;
13423 typedef _PROTOTYPE( int (*devfun_t), (struct tty *tp, int try_only) );
13424 typedef _PROTOTYPE( void (*devfunarg_t), (struct tty *tp, int c) );
13425
13426 typedef struct tty {
13427 int tty_events; /* set when TTY should inspect this line */
13428 int tty_index; /* index into TTY table */
13429 int tty_minor; /* device minor number */
13430
13431 /* Input queue. Typed characters are stored here until read by a program. */
13432 u16_t *tty_inhead; /* pointer to place where next char goes */
13433 u16_t *tty_intail; /* pointer to next char to be given to prog */
13434 int tty_incount; /* # chars in the input queue */
13435 int tty_eotct; /* number of "line breaks" in input queue */
13436 devfun_t tty_devread; /* routine to read from low level buffers */
13437 devfun_t tty_icancel; /* cancel any device input */
13438 int tty_min; /* minimum requested #chars in input queue */
13439 timer_t tty_tmr; /* the timer for this tty */
13440
13441 /* Output section. */
13442 devfun_t tty_devwrite; /* routine to start actual device output */
13443 devfunarg_t tty_echo; /* routine to echo characters input */
13444 devfun_t tty_ocancel; /* cancel any ongoing device output */
13445 devfun_t tty_break; /* let the device send a break */
13446
13447 /* Terminal parameters and status. */
13448 int tty_position; /* current position on the screen for echoing */
13449 char tty_reprint; /* 1 when echoed input messed up, else 0 */
13450 char tty_escaped; /* 1 when LNEXT (ˆV) just seen, else 0 */
13451 char tty_inhibited; /* 1 when STOP (ˆS) just seen (stops output) */
13452 char tty_pgrp; /* slot number of controlling process */
13453 char tty_openct; /* count of number of opens of this tty */
13454
13455 /* Information about incomplete I/O requests is stored here. */
13456 char tty_inrepcode; /* reply code, TASK_REPLY or REVIVE */
13457 char tty_inrevived; /* set to 1 if revive callback is pending */
13458 char tty_incaller; /* process that made the call (usually FS) */
13459 char tty_inproc; /* process that wants to read from tty */
13460 vir_bytes tty_in_vir; /* virtual address where data is to go */
13461 int tty_inleft; /* how many chars are still needed */
13462 int tty_incum; /* # chars input so far */
13463 char tty_outrepcode; /* reply code, TASK_REPLY or REVIVE */
13464 char tty_outrevived; /* set to 1 if revive callback is pending */
13465 char tty_outcaller; /* process that made the call (usually FS) */
13466 char tty_outproc; /* process that wants to write to tty */
13467 vir_bytes tty_out_vir; /* virtual address where data comes from */
13468 int tty_outleft; /* # chars yet to be output */
13469 int tty_outcum; /* # chars output so far */
13470 char tty_iocaller; /* process that made the call (usually FS) */
13471 char tty_ioproc; /* process that wants to do an ioctl */
13472 int tty_ioreq; /* ioctl request code */
13473 vir_bytes tty_iovir; /* virtual address of ioctl buffer */
13474
13475 /* select() data */
13476 int tty_select_ops; /* which operations are interesting */
13477 int tty_select_proc; /* which process wants notification */
13478
13479 /* Miscellaneous. */
810 File: drivers/tty/tty.h MINIX SOURCE CODE
13480 devfun_t tty_ioctl; /* set line speed, etc. at the device level */
13481 devfun_t tty_close; /* tell the device that the tty is closed */
13482 void *tty_priv; /* pointer to per device private data */
13483 struct termios tty_termios; /* terminal attributes */
13484 struct winsize tty_winsize; /* window size (#lines and #columns) */
13485
13486 u16_t tty_inbuf[TTY_IN_BYTES];/* tty input buffer */
13487
13488 } tty_t;
13489
13490 /* Memory allocated in tty.c, so extern here. */
13491 extern tty_t tty_table[NR_CONS+NR_RS_LINES+NR_PTYS];
13492 extern int ccurrent; /* currently visible console */
13493 extern int irq_hook_id; /* hook id for keyboard irq */
13494
13495 extern unsigned long kbd_irq_set;
13496 extern unsigned long rs_irq_set;
13497
13498 /* Values for the fields. */
13499 #define NOT_ESCAPED 0 /* previous character is not LNEXT (ˆV) */
13500 #define ESCAPED 1 /* previous character was LNEXT (ˆV) */
13501 #define RUNNING 0 /* no STOP (ˆS) has been typed to stop output */
13502 #define STOPPED 1 /* STOP (ˆS) has been typed to stop output */
13503
13504 /* Fields and flags on characters in the input queue. */
13505 #define IN_CHAR 0x00FF /* low 8 bits are the character itself */
13506 #define IN_LEN 0x0F00 /* length of char if it has been echoed */
13507 #define IN_LSHIFT 8 /* length = (c & IN_LEN) >> IN_LSHIFT */
13508 #define IN_EOT 0x1000 /* char is a line break (ˆD, LF) */
13509 #define IN_EOF 0x2000 /* char is EOF (ˆD), do not return to user */
13510 #define IN_ESC 0x4000 /* escaped by LNEXT (ˆV), no interpretation */
13511
13512 /* Times and timeouts. */
13513 #define force_timeout() ((void) (0))
13514
13515 /* Memory allocated in tty.c, so extern here. */
13516 extern timer_t *tty_timers; /* queue of TTY timers */
13517 extern clock_t tty_next_timeout; /* next TTY timeout */
13518
13519 /* Number of elements and limit of a buffer. */
13520 #define buflen(buf) (sizeof(buf) / sizeof((buf)[0]))
13521 #define bufend(buf) ((buf) + buflen(buf))
13522
13523 /* Memory allocated in tty.c, so extern here. */
13524 extern struct machine machine; /* machine information (a.o.: pc_at, ega) */
13525
13526 /* Function prototypes for TTY driver. */
13527 /* tty.c */
13528 _PROTOTYPE( void handle_events, (struct tty *tp) );
13529 _PROTOTYPE( void sigchar, (struct tty *tp, int sig) );
13530 _PROTOTYPE( void tty_task, (void) );
13531 _PROTOTYPE( int in_process, (struct tty *tp, char *buf, int count) );
13532 _PROTOTYPE( void out_process, (struct tty *tp, char *bstart, char *bpos,
13533 char *bend, int *icount, int *ocount) );
13534 _PROTOTYPE( void tty_wakeup, (clock_t now) );
13535 _PROTOTYPE( void tty_reply, (int code, int replyee, int proc_nr,
13536 int status) );
13537 _PROTOTYPE( int tty_devnop, (struct tty *tp, int try) );
13538 _PROTOTYPE( int select_try, (struct tty *tp, int ops) );
13539 _PROTOTYPE( int select_retry, (struct tty *tp) );
MINIX SOURCE CODE File: drivers/tty/tty.h 811
13540
13541 /* console.c */
13542 _PROTOTYPE( void kputc, (int c) );
13543 _PROTOTYPE( void cons_stop, (void) );
13544 _PROTOTYPE( void do_new_kmess, (message *m) );
13545 _PROTOTYPE( void do_diagnostics, (message *m) );
13546 _PROTOTYPE( void scr_init, (struct tty *tp) );
13547 _PROTOTYPE( void toggle_scroll, (void) );
13548 _PROTOTYPE( int con_loadfont, (message *m) );
13549 _PROTOTYPE( void select_console, (int cons_line) );
13550
13551 /* keyboard.c */
13552 _PROTOTYPE( void kb_init, (struct tty *tp) );
13553 _PROTOTYPE( void kb_init_once, (void) );
13554 _PROTOTYPE( int kbd_loadmap, (message *m) );
13555 _PROTOTYPE( void do_panic_dumps, (message *m) );
13556 _PROTOTYPE( void do_fkey_ctl, (message *m) );
13557 _PROTOTYPE( void kbd_interrupt, (message *m) );
13558
13559 /* vidcopy.s */
13560 _PROTOTYPE( void vid_vid_copy, (unsigned src, unsigned dst, unsigned count));
13561 _PROTOTYPE( void mem_vid_copy, (u16_t *src, unsigned dst, unsigned count));
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
drivers/tty/tty.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
13600 /* This file contains the terminal driver, both for the IBM console and regular
13601 * ASCII terminals. It handles only the device-independent part of a TTY, the
13602 * device dependent parts are in console.c, rs232.c, etc. This file contains
13603 * two main entry points, tty_task() and tty_wakeup(), and several minor entry
13604 * points for use by the device-dependent code.
13605 *
13606 * The device-independent part accepts "keyboard" input from the device-
13607 * dependent part, performs input processing (special key interpretation),
13608 * and sends the input to a process reading from the TTY. Output to a TTY
13609 * is sent to the device-dependent code for output processing and "screen"
13610 * display. Input processing is done by the device by calling ’in_process’
13611 * on the input characters, output processing may be done by the device itself
13612 * or by calling ’out_process’. The TTY takes care of input queuing, the
13613 * device does the output queuing. If a device receives an external signal,
13614 * like an interrupt, then it causes tty_wakeup() to be run by the CLOCK task
13615 * to, you guessed it, wake up the TTY to check if input or output can
13616 * continue.
13617 *
13618 * The valid messages and their parameters are:
13619 *
13620 * HARD_INT: output has been completed or input has arrived
13621 * SYS_SIG: e.g., MINIX wants to shutdown; run code to cleanly stop
13622 * DEV_READ: a process wants to read from a terminal
13623 * DEV_WRITE: a process wants to write on a terminal
13624 * DEV_IOCTL: a process wants to change a terminal’s parameters
13625 * DEV_OPEN: a tty line has been opened
13626 * DEV_CLOSE: a tty line has been closed
13627 * DEV_SELECT: start select notification request
13628 * DEV_STATUS: FS wants to know status for SELECT or REVIVE
13629 * CANCEL: terminate a previous incomplete system call immediately
812 File: drivers/tty/tty.c MINIX SOURCE CODE
13630 *
13631 * m_type TTY_LINE PROC_NR COUNT TTY_SPEK TTY_FLAGS ADDRESS
13632 * ---------------------------------------------------------------------------
13633 * | HARD_INT | | | | | | |
13634 * |-------------+---------+---------+---------+---------+---------+---------|
13635 * | SYS_SIG | sig set| | | | | |
13636 * |-------------+---------+---------+---------+---------+---------+---------|
13637 * | DEV_READ |minor dev| proc nr | count | O_NONBLOCK| buf ptr |
13638 * |-------------+---------+---------+---------+---------+---------+---------|
13639 * | DEV_WRITE |minor dev| proc nr | count | | | buf ptr |
13640 * |-------------+---------+---------+---------+---------+---------+---------|
13641 * | DEV_IOCTL |minor dev| proc nr |func code|erase etc| flags | |
13642 * |-------------+---------+---------+---------+---------+---------+---------|
13643 * | DEV_OPEN |minor dev| proc nr | O_NOCTTY| | | |
13644 * |-------------+---------+---------+---------+---------+---------+---------|
13645 * | DEV_CLOSE |minor dev| proc nr| | | | |
13646 * |-------------+---------+---------+---------+---------+---------+---------|
13647 * | DEV_STATUS| | | | | | |
13648 * |-------------+---------+---------+---------+---------+---------+---------|
13649 * | CANCEL |minor dev| proc nr| | | | |
13650 * ---------------------------------------------------------------------------
13651 *
13652 * Changes:
13653 * Jan 20, 2004 moved TTY driver to user-space (Jorrit N. Herder)
13654 * Sep 20, 2004 local timer management/ sync alarms (Jorrit N. Herder)
13655 * Jul 13, 2004 support for function key observers (Jorrit N. Herder)
13656 */
13657
13658 #include "../drivers.h"
13659 #include "../drivers.h"
13660 #include <termios.h>
13661 #include <sys/ioc_tty.h>
13662 #include <signal.h>
13663 #include <minix/callnr.h>
13664 #include <minix/keymap.h>
13665 #include "tty.h"
13666
13667 #include <sys/time.h>
13668 #include <sys/select.h>
13669
13670 extern int irq_hook_id;
13671
13672 unsigned long kbd_irq_set = 0;
13673 unsigned long rs_irq_set = 0;
13674
13675 /* Address of a tty structure. */
13676 #define tty_addr(line) (&tty_table[line])
13677
13678 /* Macros for magic tty types. */
13679 #define isconsole(tp) ((tp) < tty_addr(NR_CONS))
13680 #define ispty(tp) ((tp) >= tty_addr(NR_CONS+NR_RS_LINES))
13681
13682 /* Macros for magic tty structure pointers. */
13683 #define FIRST_TTY tty_addr(0)
13684 #define END_TTY tty_addr(sizeof(tty_table) / sizeof(tty_table[0]))
13685
13686 /* A device exists if at least its ’devread’ function is defined. */
13687 #define tty_active(tp) ((tp)->tty_devread != NULL)
13688
13689 /* RS232 lines or pseudo terminals can be completely configured out. */
MINIX SOURCE CODE File: drivers/tty/tty.c 813
13690 #if NR_RS_LINES == 0
13691 #define rs_init(tp) ((void) 0)
13692 #endif
13693 #if NR_PTYS == 0
13694 #define pty_init(tp) ((void) 0)
13695 #define do_pty(tp, mp) ((void) 0)
13696 #endif
13697
13698 FORWARD _PROTOTYPE( void tty_timed_out, (timer_t *tp) );
13699 FORWARD _PROTOTYPE( void expire_timers, (void) );
13700 FORWARD _PROTOTYPE( void settimer, (tty_t *tty_ptr, int enable) );
13701 FORWARD _PROTOTYPE( void do_cancel, (tty_t *tp, message *m_ptr) );
13702 FORWARD _PROTOTYPE( void do_ioctl, (tty_t *tp, message *m_ptr) );
13703 FORWARD _PROTOTYPE( void do_open, (tty_t *tp, message *m_ptr) );
13704 FORWARD _PROTOTYPE( void do_close, (tty_t *tp, message *m_ptr) );
13705 FORWARD _PROTOTYPE( void do_read, (tty_t *tp, message *m_ptr) );
13706 FORWARD _PROTOTYPE( void do_write, (tty_t *tp, message *m_ptr) );
13707 FORWARD _PROTOTYPE( void do_select, (tty_t *tp, message *m_ptr) );
13708 FORWARD _PROTOTYPE( void do_status, (message *m_ptr) );
13709 FORWARD _PROTOTYPE( void in_transfer, (tty_t *tp) );
13710 FORWARD _PROTOTYPE( int tty_echo, (tty_t *tp, int ch) );
13711 FORWARD _PROTOTYPE( void rawecho, (tty_t *tp, int ch) );
13712 FORWARD _PROTOTYPE( int back_over, (tty_t *tp) );
13713 FORWARD _PROTOTYPE( void reprint, (tty_t *tp) );
13714 FORWARD _PROTOTYPE( void dev_ioctl, (tty_t *tp) );
13715 FORWARD _PROTOTYPE( void setattr, (tty_t *tp) );
13716 FORWARD _PROTOTYPE( void tty_icancel, (tty_t *tp) );
13717 FORWARD _PROTOTYPE( void tty_init, (void) );
13718
13719 /* Default attributes. */
13720 PRIVATE struct termios termios_defaults = {
13721 TINPUT_DEF, TOUTPUT_DEF, TCTRL_DEF, TLOCAL_DEF, TSPEED_DEF, TSPEED_DEF,
13722 {
13723 TEOF_DEF, TEOL_DEF, TERASE_DEF, TINTR_DEF, TKILL_DEF, TMIN_DEF,
13724 TQUIT_DEF, TTIME_DEF, TSUSP_DEF, TSTART_DEF, TSTOP_DEF,
13725 TREPRINT_DEF, TLNEXT_DEF, TDISCARD_DEF,
13726 },
13727 };
13728 PRIVATE struct winsize winsize_defaults; /* = all zeroes */
13729
13730 /* Global variables for the TTY task (declared extern in tty.h). */
13731 PUBLIC tty_t tty_table[NR_CONS+NR_RS_LINES+NR_PTYS];
13732 PUBLIC int ccurrent; /* currently active console */
13733 PUBLIC timer_t *tty_timers; /* queue of TTY timers */
13734 PUBLIC clock_t tty_next_timeout; /* time that the next alarm is due */
13735 PUBLIC struct machine machine; /* kernel environment variables */
13736
13737 /*===========================================================================*
13738 * tty_task *
13739 *===========================================================================*/
13740 PUBLIC void main(void)
13741 {
13742 /* Main routine of the terminal task. */
13743
13744 message tty_mess; /* buffer for all incoming messages */
13745 unsigned line;
13746 int s;
13747 char *types[] = {"task","driver","server", "user"};
13748 register struct proc *rp;
13749 register tty_t *tp;
814 File: drivers/tty/tty.c MINIX SOURCE CODE
13750
13751 /* Initialize the TTY driver. */
13752 tty_init();
13753
13754 /* Get kernel environment (protected_mode, pc_at and ega are needed). */
13755 if (OK != (s=sys_getmachine(&machine))) {
13756 panic("TTY","Couldn’t obtain kernel environment.", s);
13757 }
13758
13759 /* Final one-time keyboard initialization. */
13760 kb_init_once();
13761
13762 printf("\n");
13763
13764 while (TRUE) {
13765
13766 /* Check for and handle any events on any of the ttys. */
13767 for (tp = FIRST_TTY; tp < END_TTY; tp++) {
13768 if (tp->tty_events) handle_events(tp);
13769 }
13770
13771 /* Get a request message. */
13772 receive(ANY, &tty_mess);
13773
13774 /* First handle all kernel notification types that the TTY supports.
13775 * - An alarm went off, expire all timers and handle the events.
13776 * - A hardware interrupt also is an invitation to check for events.
13777 * - A new kernel message is available for printing.
13778 * - Reset the console on system shutdown.
13779 * Then see if this message is different from a normal device driver
13780 * request and should be handled separately. These extra functions
13781 * do not operate on a device, in constrast to the driver requests.
13782 */
13783 switch (tty_mess.m_type) {
13784 case SYN_ALARM: /* fall through */
13785 expire_timers(); /* run watchdogs of expired timers */
13786 continue; /* contine to check for events */
13787 case HARD_INT: { /* hardware interrupt notification */
13788 if (tty_mess.NOTIFY_ARG & kbd_irq_set)
13789 kbd_interrupt(&tty_mess);/* fetch chars from keyboard */
13790 #if NR_RS_LINES > 0
13791 if (tty_mess.NOTIFY_ARG & rs_irq_set)
13792 rs_interrupt(&tty_mess);/* serial I/O */
13793 #endif
13794 expire_timers(); /* run watchdogs of expired timers */
13795 continue; /* contine to check for events */
13796 }
13797 case SYS_SIG: { /* system signal */
13798 sigset_t sigset = (sigset_t) tty_mess.NOTIFY_ARG;
13799
13800 if (sigismember(&sigset, SIGKSTOP)) {
13801 cons_stop(); /* switch to primary console */
13802 if (irq_hook_id != -1) {
13803 sys_irqdisable(&irq_hook_id);
13804 sys_irqrmpolicy(KEYBOARD_IRQ, &irq_hook_id);
13805 }
13806 }
13807 if (sigismember(&sigset, SIGTERM)) cons_stop();
13808 if (sigismember(&sigset, SIGKMESS)) do_new_kmess(&tty_mess);
13809 continue;
MINIX SOURCE CODE File: drivers/tty/tty.c 815
13810 }
13811 case PANIC_DUMPS: /* allow panic dumps */
13812 cons_stop(); /* switch to primary console */
13813 do_panic_dumps(&tty_mess);
13814 continue;
13815 case DIAGNOSTICS: /* a server wants to print some */
13816 do_diagnostics(&tty_mess);
13817 continue;
13818 case FKEY_CONTROL: /* (un)register a fkey observer */
13819 do_fkey_ctl(&tty_mess);
13820 continue;
13821 default: /* should be a driver request */
13822 ; /* do nothing; end switch */
13823 }
13824
13825 /* Only device requests should get to this point. All requests,
13826 * except DEV_STATUS, have a minor device number. Check this
13827 * exception and get the minor device number otherwise.
13828 */
13829 if (tty_mess.m_type == DEV_STATUS) {
13830 do_status(&tty_mess);
13831 continue;
13832 }
13833 line = tty_mess.TTY_LINE;
13834 if ((line - CONS_MINOR) < NR_CONS) {
13835 tp = tty_addr(line - CONS_MINOR);
13836 } else if (line == LOG_MINOR) {
13837 tp = tty_addr(0);
13838 } else if ((line - RS232_MINOR) < NR_RS_LINES) {
13839 tp = tty_addr(line - RS232_MINOR + NR_CONS);
13840 } else if ((line - TTYPX_MINOR) < NR_PTYS) {
13841 tp = tty_addr(line - TTYPX_MINOR + NR_CONS + NR_RS_LINES);
13842 } else if ((line - PTYPX_MINOR) < NR_PTYS) {
13843 tp = tty_addr(line - PTYPX_MINOR + NR_CONS + NR_RS_LINES);
13844 if (tty_mess.m_type != DEV_IOCTL) {
13845 do_pty(tp, &tty_mess);
13846 continue;
13847 }
13848 } else {
13849 tp = NULL;
13850 }
13851
13852 /* If the device doesn’t exist or is not configured return ENXIO. */
13853 if (tp == NULL || ! tty_active(tp)) {
13854 printf("Warning, TTY got illegal request %d from %d\n",
13855 tty_mess.m_type, tty_mess.m_source);
13856 tty_reply(TASK_REPLY, tty_mess.m_source,
13857 tty_mess.PROC_NR, ENXIO);
13858 continue;
13859 }
13860
13861 /* Execute the requested device driver function. */
13862 switch (tty_mess.m_type) {
13863 case DEV_READ: do_read(tp, &tty_mess); break;
13864 case DEV_WRITE: do_write(tp, &tty_mess); break;
13865 case DEV_IOCTL: do_ioctl(tp, &tty_mess); break;
13866 case DEV_OPEN: do_open(tp, &tty_mess); break;
13867 case DEV_CLOSE: do_close(tp, &tty_mess); break;
13868 case DEV_SELECT: do_select(tp, &tty_mess); break;
13869 case CANCEL: do_cancel(tp, &tty_mess); break;
816 File: drivers/tty/tty.c MINIX SOURCE CODE
13870 default:
13871 printf("Warning, TTY got unexpected request %d from %d\n",
13872 tty_mess.m_type, tty_mess.m_source);
13873 tty_reply(TASK_REPLY, tty_mess.m_source,
13874 tty_mess.PROC_NR, EINVAL);
13875 }
13876 }
13877 }
13879 /*===========================================================================*
13880 * do_status *
13881 *===========================================================================*/
13882 PRIVATE void do_status(m_ptr)
13883 message *m_ptr;
13884 {
13885 register struct tty *tp;
13886 int event_found;
13887 int status;
13888 int ops;
13889
13890 /* Check for select or revive events on any of the ttys. If we found an,
13891 * event return a single status message for it. The FS will make another
13892 * call to see if there is more.
13893 */
13894 event_found = 0;
13895 for (tp = FIRST_TTY; tp < END_TTY; tp++) {
13896 if ((ops = select_try(tp, tp->tty_select_ops)) &&
13897 tp->tty_select_proc == m_ptr->m_source) {
13898
13899 /* I/O for a selected minor device is ready. */
13900 m_ptr->m_type = DEV_IO_READY;
13901 m_ptr->DEV_MINOR = tp->tty_index;
13902 m_ptr->DEV_SEL_OPS = ops;
13903
13904 tp->tty_select_ops &= ˜ops; /* unmark select event */
13905 event_found = 1;
13906 break;
13907 }
13908 else if (tp->tty_inrevived && tp->tty_incaller == m_ptr->m_source) {
13909
13910 /* Suspended request finished. Send a REVIVE. */
13911 m_ptr->m_type = DEV_REVIVE;
13912 m_ptr->REP_PROC_NR = tp->tty_inproc;
13913 m_ptr->REP_STATUS = tp->tty_incum;
13914
13915 tp->tty_inleft = tp->tty_incum = 0;
13916 tp->tty_inrevived = 0; /* unmark revive event */
13917 event_found = 1;
13918 break;
13919 }
13920 else if (tp->tty_outrevived && tp->tty_outcaller == m_ptr->m_source) {
13921
13922 /* Suspended request finished. Send a REVIVE. */
13923 m_ptr->m_type = DEV_REVIVE;
13924 m_ptr->REP_PROC_NR = tp->tty_outproc;
13925 m_ptr->REP_STATUS = tp->tty_outcum;
13926
13927 tp->tty_outcum = 0;
13928 tp->tty_outrevived = 0; /* unmark revive event */
13929 event_found = 1;
MINIX SOURCE CODE File: drivers/tty/tty.c 817
13930 break;
13931 }
13932 }
13933
13934 #if NR_PTYS > 0
13935 if (!event_found)
13936 event_found = pty_status(m_ptr);
13937 #endif
13938
13939 if (! event_found) {
13940 /* No events of interest were found. Return an empty message. */
13941 m_ptr->m_type = DEV_NO_STATUS;
13942 }
13943
13944 /* Almost done. Send back the reply message to the caller. */
13945 if ((status = send(m_ptr->m_source, m_ptr)) != OK) {
13946 panic("TTY","send in do_status failed, status\n", status);
13947 }
13948 }
13950 /*===========================================================================*
13951 * do_read *
13952 *===========================================================================*/
13953 PRIVATE void do_read(tp, m_ptr)
13954 register tty_t *tp; /* pointer to tty struct */
13955 register message *m_ptr; /* pointer to message sent to the task */
13956 {
13957 /* A process wants to read from a terminal. */
13958 int r, status;
13959 phys_bytes phys_addr;
13960
13961 /* Check if there is already a process hanging in a read, check if the
13962 * parameters are correct, do I/O.
13963 */
13964 if (tp->tty_inleft > 0) {
13965 r = EIO;
13966 } else
13967 if (m_ptr->COUNT <= 0) {
13968 r = EINVAL;
13969 } else
13970 if (sys_umap(m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS, m_ptr->COUNT,
13971 &phys_addr) != OK) {
13972 r = EFAULT;
13973 } else {
13974 /* Copy information from the message to the tty struct. */
13975 tp->tty_inrepcode = TASK_REPLY;
13976 tp->tty_incaller = m_ptr->m_source;
13977 tp->tty_inproc = m_ptr->PROC_NR;
13978 tp->tty_in_vir = (vir_bytes) m_ptr->ADDRESS;
13979 tp->tty_inleft = m_ptr->COUNT;
13980
13981 if (!(tp->tty_termios.c_lflag & ICANON)
13982 && tp->tty_termios.c_cc[VTIME] > 0) {
13983 if (tp->tty_termios.c_cc[VMIN] == 0) {
13984 /* MIN & TIME specify a read timer that finishes the
13985 * read in TIME/10 seconds if no bytes are available.
13986 */
13987 settimer(tp, TRUE);
13988 tp->tty_min = 1;
13989 } else {
818 File: drivers/tty/tty.c MINIX SOURCE CODE
13990 /* MIN & TIME specify an inter-byte timer that may
13991 * have to be cancelled if there are no bytes yet.
13992 */
13993 if (tp->tty_eotct == 0) {
13994 settimer(tp, FALSE);
13995 tp->tty_min = tp->tty_termios.c_cc[VMIN];
13996 }
13997 }
13998 }
13999
14000 /* Anything waiting in the input buffer? Clear it out... */
14001 in_transfer(tp);
14002 /* ...then go back for more. */
14003 handle_events(tp);
14004 if (tp->tty_inleft == 0) {
14005 if (tp->tty_select_ops)
14006 select_retry(tp);
14007 return; /* already done */
14008 }
14009
14010 /* There were no bytes in the input queue available, so either suspend
14011 * the caller or break off the read if nonblocking.
14012 */
14013 if (m_ptr->TTY_FLAGS & O_NONBLOCK) {
14014 r = EAGAIN; /* cancel the read */
14015 tp->tty_inleft = tp->tty_incum = 0;
14016 } else {
14017 r = SUSPEND; /* suspend the caller */
14018 tp->tty_inrepcode = REVIVE;
14019 }
14020 }
14021 tty_reply(TASK_REPLY, m_ptr->m_source, m_ptr->PROC_NR, r);
14022 if (tp->tty_select_ops)
14023 select_retry(tp);
14024 }
14026 /*===========================================================================*
14027 * do_write *
14028 *===========================================================================*/
14029 PRIVATE void do_write(tp, m_ptr)
14030 register tty_t *tp;
14031 register message *m_ptr; /* pointer to message sent to the task */
14032 {
14033 /* A process wants to write on a terminal. */
14034 int r;
14035 phys_bytes phys_addr;
14036
14037 /* Check if there is already a process hanging in a write, check if the
14038 * parameters are correct, do I/O.
14039 */
14040 if (tp->tty_outleft > 0) {
14041 r = EIO;
14042 } else
14043 if (m_ptr->COUNT <= 0) {
14044 r = EINVAL;
14045 } else
14046 if (sys_umap(m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS, m_ptr->COUNT,
14047 &phys_addr) != OK) {
14048 r = EFAULT;
14049 } else {
MINIX SOURCE CODE File: drivers/tty/tty.c 819
14050 /* Copy message parameters to the tty structure. */
14051 tp->tty_outrepcode = TASK_REPLY;
14052 tp->tty_outcaller = m_ptr->m_source;
14053 tp->tty_outproc = m_ptr->PROC_NR;
14054 tp->tty_out_vir = (vir_bytes) m_ptr->ADDRESS;
14055 tp->tty_outleft = m_ptr->COUNT;
14056
14057 /* Try to write. */
14058 handle_events(tp);
14059 if (tp->tty_outleft == 0)
14060 return; /* already done */
14061
14062 /* None or not all the bytes could be written, so either suspend the
14063 * caller or break off the write if nonblocking.
14064 */
14065 if (m_ptr->TTY_FLAGS & O_NONBLOCK) { /* cancel the write */
14066 r = tp->tty_outcum > 0 ? tp->tty_outcum : EAGAIN;
14067 tp->tty_outleft = tp->tty_outcum = 0;
14068 } else {
14069 r = SUSPEND; /* suspend the caller */
14070 tp->tty_outrepcode = REVIVE;
14071 }
14072 }
14073 tty_reply(TASK_REPLY, m_ptr->m_source, m_ptr->PROC_NR, r);
14074 }
14076 /*===========================================================================*
14077 * do_ioctl *
14078 *===========================================================================*/
14079 PRIVATE void do_ioctl(tp, m_ptr)
14080 register tty_t *tp;
14081 message *m_ptr; /* pointer to message sent to task */
14082 {
14083 /* Perform an IOCTL on this terminal. Posix termios calls are handled
14084 * by the IOCTL system call
14085 */
14086
14087 int r;
14088 union {
14089 int i;
14090 } param;
14091 size_t size;
14092
14093 /* Size of the ioctl parameter. */
14094 switch (m_ptr->TTY_REQUEST) {
14095 case TCGETS: /* Posix tcgetattr function */
14096 case TCSETS: /* Posix tcsetattr function, TCSANOW option */
14097 case TCSETSW: /* Posix tcsetattr function, TCSADRAIN option */
14098 case TCSETSF: /* Posix tcsetattr function, TCSAFLUSH option */
14099 size = sizeof(struct termios);
14100 break;
14101
14102 case TCSBRK: /* Posix tcsendbreak function */
14103 case TCFLOW: /* Posix tcflow function */
14104 case TCFLSH: /* Posix tcflush function */
14105 case TIOCGPGRP: /* Posix tcgetpgrp function */
14106 case TIOCSPGRP: /* Posix tcsetpgrp function */
14107 size = sizeof(int);
14108 break;
14109
820 File: drivers/tty/tty.c MINIX SOURCE CODE
14110 case TIOCGWINSZ: /* get window size (not Posix) */
14111 case TIOCSWINSZ: /* set window size (not Posix) */
14112 size = sizeof(struct winsize);
14113 break;
14114
14115 case KIOCSMAP: /* load keymap (Minix extension) */
14116 size = sizeof(keymap_t);
14117 break;
14118
14119 case TIOCSFON: /* load font (Minix extension) */
14120 size = sizeof(u8_t [8192]);
14121 break;
14122
14123 case TCDRAIN: /* Posix tcdrain function -- no parameter */
14124 default: size = 0;
14125 }
14126
14127 r = OK;
14128 switch (m_ptr->TTY_REQUEST) {
14129 case TCGETS:
14130 /* Get the termios attributes. */
14131 r = sys_vircopy(SELF, D, (vir_bytes) &tp->tty_termios,
14132 m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS,
14133 (vir_bytes) size);
14134 break;
14135
14136 case TCSETSW:
14137 case TCSETSF:
14138 case TCDRAIN:
14139 if (tp->tty_outleft > 0) {
14140 /* Wait for all ongoing output processing to finish. */
14141 tp->tty_iocaller = m_ptr->m_source;
14142 tp->tty_ioproc = m_ptr->PROC_NR;
14143 tp->tty_ioreq = m_ptr->REQUEST;
14144 tp->tty_iovir = (vir_bytes) m_ptr->ADDRESS;
14145 r = SUSPEND;
14146 break;
14147 }
14148 if (m_ptr->TTY_REQUEST == TCDRAIN) break;
14149 if (m_ptr->TTY_REQUEST == TCSETSF) tty_icancel(tp);
14150 /*FALL THROUGH*/
14151 case TCSETS:
14152 /* Set the termios attributes. */
14153 r = sys_vircopy( m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS,
14154 SELF, D, (vir_bytes) &tp->tty_termios, (vir_bytes) size);
14155 if (r != OK) break;
14156 setattr(tp);
14157 break;
14158
14159 case TCFLSH:
14160 r = sys_vircopy( m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS,
14161 SELF, D, (vir_bytes) &param.i, (vir_bytes) size);
14162 if (r != OK) break;
14163 switch (param.i) {
14164 case TCIFLUSH: tty_icancel(tp); break;
14165 case TCOFLUSH: (*tp->tty_ocancel)(tp, 0); break;
14166 case TCIOFLUSH: tty_icancel(tp); (*tp->tty_ocancel)(tp, 0); break;
14167 default: r = EINVAL;
14168 }
14169 break;
MINIX SOURCE CODE File: drivers/tty/tty.c 821
14170
14171 case TCFLOW:
14172 r = sys_vircopy( m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS,
14173 SELF, D, (vir_bytes) &param.i, (vir_bytes) size);
14174 if (r != OK) break;
14175 switch (param.i) {
14176 case TCOOFF:
14177 case TCOON:
14178 tp->tty_inhibited = (param.i == TCOOFF);
14179 tp->tty_events = 1;
14180 break;
14181 case TCIOFF:
14182 (*tp->tty_echo)(tp, tp->tty_termios.c_cc[VSTOP]);
14183 break;
14184 case TCION:
14185 (*tp->tty_echo)(tp, tp->tty_termios.c_cc[VSTART]);
14186 break;
14187 default:
14188 r = EINVAL;
14189 }
14190 break;
14191
14192 case TCSBRK:
14193 if (tp->tty_break != NULL) (*tp->tty_break)(tp,0);
14194 break;
14195
14196 case TIOCGWINSZ:
14197 r = sys_vircopy(SELF, D, (vir_bytes) &tp->tty_winsize,
14198 m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS,
14199 (vir_bytes) size);
14200 break;
14201
14202 case TIOCSWINSZ:
14203 r = sys_vircopy( m_ptr->PROC_NR, D, (vir_bytes) m_ptr->ADDRESS,
14204 SELF, D, (vir_bytes) &tp->tty_winsize, (vir_bytes) size);
14205 /* SIGWINCH... */
14206 break;
14207
14208 case KIOCSMAP:
14209 /* Load a new keymap (only /dev/console). */
14210 if (isconsole(tp)) r = kbd_loadmap(m_ptr);
14211 break;
14212
14213 case TIOCSFON:
14214 /* Load a font into an EGA or VGA card (hs@hck.hr) */
14215 if (isconsole(tp)) r = con_loadfont(m_ptr);
14216 break;
14217
14218 /* These Posix functions are allowed to fail if _POSIX_JOB_CONTROL is
14219 * not defined.
14220 */
14221 case TIOCGPGRP:
14222 case TIOCSPGRP:
14223 default:
14224 r = ENOTTY;
14225 }
14226
14227 /* Send the reply. */
14228 tty_reply(TASK_REPLY, m_ptr->m_source, m_ptr->PROC_NR, r);
14229 }
822 File: drivers/tty/tty.c MINIX SOURCE CODE
14231 /*===========================================================================*
14232 * do_open *
14233 *===========================================================================*/
14234 PRIVATE void do_open(tp, m_ptr)
14235 register tty_t *tp;
14236 message *m_ptr; /* pointer to message sent to task */
14237 {
14238 /* A tty line has been opened. Make it the callers controlling tty if
14239 * O_NOCTTY is *not* set and it is not the log device. 1 is returned if
14240 * the tty is made the controlling tty, otherwise OK or an error code.
14241 */
14242 int r = OK;
14243
14244 if (m_ptr->TTY_LINE == LOG_MINOR) {
14245 /* The log device is a write-only diagnostics device. */
14246 if (m_ptr->COUNT & R_BIT) r = EACCES;
14247 } else {
14248 if (!(m_ptr->COUNT & O_NOCTTY)) {
14249 tp->tty_pgrp = m_ptr->PROC_NR;
14250 r = 1;
14251 }
14252 tp->tty_openct++;
14253 }
14254 tty_reply(TASK_REPLY, m_ptr->m_source, m_ptr->PROC_NR, r);
14255 }
14257 /*===========================================================================*
14258 * do_close *
14259 *===========================================================================*/
14260 PRIVATE void do_close(tp, m_ptr)
14261 register tty_t *tp;
14262 message *m_ptr; /* pointer to message sent to task */
14263 {
14264 /* A tty line has been closed. Clean up the line if it is the last close. */
14265
14266 if (m_ptr->TTY_LINE != LOG_MINOR && --tp->tty_openct == 0) {
14267 tp->tty_pgrp = 0;
14268 tty_icancel(tp);
14269 (*tp->tty_ocancel)(tp, 0);
14270 (*tp->tty_close)(tp, 0);
14271 tp->tty_termios = termios_defaults;
14272 tp->tty_winsize = winsize_defaults;
14273 setattr(tp);
14274 }
14275 tty_reply(TASK_REPLY, m_ptr->m_source, m_ptr->PROC_NR, OK);
14276 }
14278 /*===========================================================================*
14279 * do_cancel *
14280 *===========================================================================*/
14281 PRIVATE void do_cancel(tp, m_ptr)
14282 register tty_t *tp;
14283 message *m_ptr; /* pointer to message sent to task */
14284 {
14285 /* A signal has been sent to a process that is hanging trying to read or write.
14286 * The pending read or write must be finished off immediately.
14287 */
14288
14289 int proc_nr;
MINIX SOURCE CODE File: drivers/tty/tty.c 823
14290 int mode;
14291
14292 /* Check the parameters carefully, to avoid cancelling twice. */
14293 proc_nr = m_ptr->PROC_NR;
14294 mode = m_ptr->COUNT;
14295 if ((mode & R_BIT) && tp->tty_inleft != 0 && proc_nr == tp->tty_inproc) {
14296 /* Process was reading when killed. Clean up input. */
14297 tty_icancel(tp);
14298 tp->tty_inleft = tp->tty_incum = 0;
14299 }
14300 if ((mode & W_BIT) && tp->tty_outleft != 0 && proc_nr == tp->tty_outproc) {
14301 /* Process was writing when killed. Clean up output. */
14302 (*tp->tty_ocancel)(tp, 0);
14303 tp->tty_outleft = tp->tty_outcum = 0;
14304 }
14305 if (tp->tty_ioreq != 0 && proc_nr == tp->tty_ioproc) {
14306 /* Process was waiting for output to drain. */
14307 tp->tty_ioreq = 0;
14308 }
14309 tp->tty_events = 1;
14310 tty_reply(TASK_REPLY, m_ptr->m_source, proc_nr, EINTR);
14311 }
14313 PUBLIC int select_try(struct tty *tp, int ops)
14314 {
14315 int ready_ops = 0;
14316
14317 /* Special case. If line is hung up, no operations will block.
14318 * (and it can be seen as an exceptional condition.)
14319 */
14320 if (tp->tty_termios.c_ospeed == B0) {
14321 ready_ops |= ops;
14322 }
14323
14324 if (ops & SEL_RD) {
14325 /* will i/o not block on read? */
14326 if (tp->tty_inleft > 0) {
14327 ready_ops |= SEL_RD; /* EIO - no blocking */
14328 } else if (tp->tty_incount > 0) {
14329 /* Is a regular read possible? tty_incount
14330 * says there is data. But a read will only succeed
14331 * in canonical mode if a newline has been seen.
14332 */
14333 if (!(tp->tty_termios.c_lflag & ICANON) ||
14334 tp->tty_eotct > 0) {
14335 ready_ops |= SEL_RD;
14336 }
14337 }
14338 }
14339
14340 if (ops & SEL_WR) {
14341 if (tp->tty_outleft > 0) ready_ops |= SEL_WR;
14342 else if ((*tp->tty_devwrite)(tp, 1)) ready_ops |= SEL_WR;
14343 }
14344
14345 return ready_ops;
14346 }
14348 PUBLIC int select_retry(struct tty *tp)
14349 {
824 File: drivers/tty/tty.c MINIX SOURCE CODE
14350 if (select_try(tp, tp->tty_select_ops))
14351 notify(tp->tty_select_proc);
14352 return OK;
14353 }
14355 /*===========================================================================*
14356 * handle_events *
14357 *===========================================================================*/
14358 PUBLIC void handle_events(tp)
14359 tty_t *tp; /* TTY to check for events. */
14360 {
14361 /* Handle any events pending on a TTY. These events are usually device
14362 * interrupts.
14363 *
14364 * Two kinds of events are prominent:
14365 * - a character has been received from the console or an RS232 line.
14366 * - an RS232 line has completed a write request (on behalf of a user).
14367 * The interrupt handler may delay the interrupt message at its discretion
14368 * to avoid swamping the TTY task. Messages may be overwritten when the
14369 * lines are fast or when there are races between different lines, input
14370 * and output, because MINIX only provides single buffering for interrupt
14371 * messages (in proc.c). This is handled by explicitly checking each line
14372 * for fresh input and completed output on each interrupt.
14373 */
14374 char *buf;
14375 unsigned count;
14376 int status;
14377
14378 do {
14379 tp->tty_events = 0;
14380
14381 /* Read input and perform input processing. */
14382 (*tp->tty_devread)(tp, 0);
14383
14384 /* Perform output processing and write output. */
14385 (*tp->tty_devwrite)(tp, 0);
14386
14387 /* Ioctl waiting for some event? */
14388 if (tp->tty_ioreq != 0) dev_ioctl(tp);
14389 } while (tp->tty_events);
14390
14391 /* Transfer characters from the input queue to a waiting process. */
14392 in_transfer(tp);
14393
14394 /* Reply if enough bytes are available. */
14395 if (tp->tty_incum >= tp->tty_min && tp->tty_inleft > 0) {
14396 if (tp->tty_inrepcode == REVIVE) {
14397 notify(tp->tty_incaller);
14398 tp->tty_inrevived = 1;
14399 } else {
14400 tty_reply(tp->tty_inrepcode, tp->tty_incaller,
14401 tp->tty_inproc, tp->tty_incum);
14402 tp->tty_inleft = tp->tty_incum = 0;
14403 }
14404 }
14405 if (tp->tty_select_ops)
14406 select_retry(tp);
14407 #if NR_PTYS > 0
14408 if (ispty(tp))
14409 select_retry_pty(tp);
MINIX SOURCE CODE File: drivers/tty/tty.c 825
14410 #endif
14411 }
14413 /*===========================================================================*
14414 * in_transfer *
14415 *===========================================================================*/
14416 PRIVATE void in_transfer(tp)
14417 register tty_t *tp; /* pointer to terminal to read from */
14418 {
14419 /* Transfer bytes from the input queue to a process reading from a terminal. */
14420
14421 int ch;
14422 int count;
14423 char buf[64], *bp;
14424
14425 /* Force read to succeed if the line is hung up, looks like EOF to reader. */
14426 if (tp->tty_termios.c_ospeed == B0) tp->tty_min = 0;
14427
14428 /* Anything to do? */
14429 if (tp->tty_inleft == 0 || tp->tty_eotct < tp->tty_min) return;
14430
14431 bp = buf;
14432 while (tp->tty_inleft > 0 && tp->tty_eotct > 0) {
14433 ch = *tp->tty_intail;
14434
14435 if (!(ch & IN_EOF)) {
14436 /* One character to be delivered to the user. */
14437 *bp = ch & IN_CHAR;
14438 tp->tty_inleft--;
14439 if (++bp == bufend(buf)) {
14440 /* Temp buffer full, copy to user space. */
14441 sys_vircopy(SELF, D, (vir_bytes) buf,
14442 tp->tty_inproc, D, tp->tty_in_vir,
14443 (vir_bytes) buflen(buf));
14444 tp->tty_in_vir += buflen(buf);
14445 tp->tty_incum += buflen(buf);
14446 bp = buf;
14447 }
14448 }
14449
14450 /* Remove the character from the input queue. */
14451 if (++tp->tty_intail == bufend(tp->tty_inbuf))
14452 tp->tty_intail = tp->tty_inbuf;
14453 tp->tty_incount--;
14454 if (ch & IN_EOT) {
14455 tp->tty_eotct--;
14456 /* Don’t read past a line break in canonical mode. */
14457 if (tp->tty_termios.c_lflag & ICANON) tp->tty_inleft = 0;
14458 }
14459 }
14460
14461 if (bp > buf) {
14462 /* Leftover characters in the buffer. */
14463 count = bp - buf;
14464 sys_vircopy(SELF, D, (vir_bytes) buf,
14465 tp->tty_inproc, D, tp->tty_in_vir, (vir_bytes) count);
14466 tp->tty_in_vir += count;
14467 tp->tty_incum += count;
14468 }
14469
826 File: drivers/tty/tty.c MINIX SOURCE CODE
14470 /* Usually reply to the reader, possibly even if incum == 0 (EOF). */
14471 if (tp->tty_inleft == 0) {
14472 if (tp->tty_inrepcode == REVIVE) {
14473 notify(tp->tty_incaller);
14474 tp->tty_inrevived = 1;
14475 } else {
14476 tty_reply(tp->tty_inrepcode, tp->tty_incaller,
14477 tp->tty_inproc, tp->tty_incum);
14478 tp->tty_inleft = tp->tty_incum = 0;
14479 }
14480 }
14481 }
14483 /*===========================================================================*
14484 * in_process *
14485 *===========================================================================*/
14486 PUBLIC int in_process(tp, buf, count)
14487 register tty_t *tp; /* terminal on which character has arrived */
14488 char *buf; /* buffer with input characters */
14489 int count; /* number of input characters */
14490 {
14491 /* Characters have just been typed in. Process, save, and echo them. Return
14492 * the number of characters processed.
14493 */
14494
14495 int ch, sig, ct;
14496 int timeset = FALSE;
14497 static unsigned char csize_mask[] = { 0x1F, 0x3F, 0x7F, 0xFF };
14498
14499 for (ct = 0; ct < count; ct++) {
14500 /* Take one character. */
14501 ch = *buf++ & BYTE;
14502
14503 /* Strip to seven bits? */
14504 if (tp->tty_termios.c_iflag & ISTRIP) ch &= 0x7F;
14505
14506 /* Input extensions? */
14507 if (tp->tty_termios.c_lflag & IEXTEN) {
14508
14509 /* Previous character was a character escape? */
14510 if (tp->tty_escaped) {
14511 tp->tty_escaped = NOT_ESCAPED;
14512 ch |= IN_ESC; /* protect character */
14513 }
14514
14515 /* LNEXT (ˆV) to escape the next character? */
14516 if (ch == tp->tty_termios.c_cc[VLNEXT]) {
14517 tp->tty_escaped = ESCAPED;
14518 rawecho(tp, ’ˆ’);
14519 rawecho(tp, ’\b’);
14520 continue; /* do not store the escape */
14521 }
14522
14523 /* REPRINT (ˆR) to reprint echoed characters? */
14524 if (ch == tp->tty_termios.c_cc[VREPRINT]) {
14525 reprint(tp);
14526 continue;
14527 }
14528 }
14529
MINIX SOURCE CODE File: drivers/tty/tty.c 827
14530 /* _POSIX_VDISABLE is a normal character value, so better escape it. */
14531 if (ch == _POSIX_VDISABLE) ch |= IN_ESC;
14532
14533 /* Map CR to LF, ignore CR, or map LF to CR. */
14534 if (ch == ’\r’) {
14535 if (tp->tty_termios.c_iflag & IGNCR) continue;
14536 if (tp->tty_termios.c_iflag & ICRNL) ch = ’\n’;
14537 } else
14538 if (ch == ’\n’) {
14539 if (tp->tty_termios.c_iflag & INLCR) ch = ’\r’;
14540 }
14541
14542 /* Canonical mode? */
14543 if (tp->tty_termios.c_lflag & ICANON) {
14544
14545 /* Erase processing (rub out of last character). */
14546 if (ch == tp->tty_termios.c_cc[VERASE]) {
14547 (void) back_over(tp);
14548 if (!(tp->tty_termios.c_lflag & ECHOE)) {
14549 (void) tty_echo(tp, ch);
14550 }
14551 continue;
14552 }
14553
14554 /* Kill processing (remove current line). */
14555 if (ch == tp->tty_termios.c_cc[VKILL]) {
14556 while (back_over(tp)) {}
14557 if (!(tp->tty_termios.c_lflag & ECHOE)) {
14558 (void) tty_echo(tp, ch);
14559 if (tp->tty_termios.c_lflag & ECHOK)
14560 rawecho(tp, ’\n’);
14561 }
14562 continue;
14563 }
14564
14565 /* EOF (ˆD) means end-of-file, an invisible "line break". */
14566 if (ch == tp->tty_termios.c_cc[VEOF]) ch |= IN_EOT | IN_EOF;
14567
14568 /* The line may be returned to the user after an LF. */
14569 if (ch == ’\n’) ch |= IN_EOT;
14570
14571 /* Same thing with EOL, whatever it may be. */
14572 if (ch == tp->tty_termios.c_cc[VEOL]) ch |= IN_EOT;
14573 }
14574
14575 /* Start/stop input control? */
14576 if (tp->tty_termios.c_iflag & IXON) {
14577
14578 /* Output stops on STOP (ˆS). */
14579 if (ch == tp->tty_termios.c_cc[VSTOP]) {
14580 tp->tty_inhibited = STOPPED;
14581 tp->tty_events = 1;
14582 continue;
14583 }
14584
14585 /* Output restarts on START (ˆQ) or any character if IXANY. */
14586 if (tp->tty_inhibited) {
14587 if (ch == tp->tty_termios.c_cc[VSTART]
14588 || (tp->tty_termios.c_iflag & IXANY)) {
14589 tp->tty_inhibited = RUNNING;
828 File: drivers/tty/tty.c MINIX SOURCE CODE
14590 tp->tty_events = 1;
14591 if (ch == tp->tty_termios.c_cc[VSTART])
14592 continue;
14593 }
14594 }
14595 }
14596
14597 if (tp->tty_termios.c_lflag & ISIG) {
14598 /* Check for INTR (ˆ?) and QUIT (ˆ\) characters. */
14599 if (ch == tp->tty_termios.c_cc[VINTR]
14600 || ch == tp->tty_termios.c_cc[VQUIT]) {
14601 sig = SIGINT;
14602 if (ch == tp->tty_termios.c_cc[VQUIT]) sig = SIGQUIT;
14603 sigchar(tp, sig);
14604 (void) tty_echo(tp, ch);
14605 continue;
14606 }
14607 }
14608
14609 /* Is there space in the input buffer? */
14610 if (tp->tty_incount == buflen(tp->tty_inbuf)) {
14611 /* No space; discard in canonical mode, keep in raw mode. */
14612 if (tp->tty_termios.c_lflag & ICANON) continue;
14613 break;
14614 }
14615
14616 if (!(tp->tty_termios.c_lflag & ICANON)) {
14617 /* In raw mode all characters are "line breaks". */
14618 ch |= IN_EOT;
14619
14620 /* Start an inter-byte timer? */
14621 if (!timeset && tp->tty_termios.c_cc[VMIN] > 0
14622 && tp->tty_termios.c_cc[VTIME] > 0) {
14623 settimer(tp, TRUE);
14624 timeset = TRUE;
14625 }
14626 }
14627
14628 /* Perform the intricate function of echoing. */
14629 if (tp->tty_termios.c_lflag & (ECHO|ECHONL)) ch = tty_echo(tp, ch);
14630
14631 /* Save the character in the input queue. */
14632 *tp->tty_inhead++ = ch;
14633 if (tp->tty_inhead == bufend(tp->tty_inbuf))
14634 tp->tty_inhead = tp->tty_inbuf;
14635 tp->tty_incount++;
14636 if (ch & IN_EOT) tp->tty_eotct++;
14637
14638 /* Try to finish input if the queue threatens to overflow. */
14639 if (tp->tty_incount == buflen(tp->tty_inbuf)) in_transfer(tp);
14640 }
14641 return ct;
14642 }
14644 /*===========================================================================*
14645 * echo *
14646 *===========================================================================*/
14647 PRIVATE int tty_echo(tp, ch)
14648 register tty_t *tp; /* terminal on which to echo */
14649 register int ch; /* pointer to character to echo */
MINIX SOURCE CODE File: drivers/tty/tty.c 829
14650 {
14651 /* Echo the character if echoing is on. Some control characters are echoed
14652 * with their normal effect, other control characters are echoed as "ˆX",
14653 * normal characters are echoed normally. EOF (ˆD) is echoed, but immediately
14654 * backspaced over. Return the character with the echoed length added to its
14655 * attributes.
14656 */
14657 int len, rp;
14658
14659 ch &= ˜IN_LEN;
14660 if (!(tp->tty_termios.c_lflag & ECHO)) {
14661 if (ch == (’\n’ | IN_EOT) && (tp->tty_termios.c_lflag
14662 & (ICANON|ECHONL)) == (ICANON|ECHONL))
14663 (*tp->tty_echo)(tp, ’\n’);
14664 return(ch);
14665 }
14666
14667 /* "Reprint" tells if the echo output has been messed up by other output. */
14668 rp = tp->tty_incount == 0 ? FALSE : tp->tty_reprint;
14669
14670 if ((ch & IN_CHAR) < ’ ’) {
14671 switch (ch & (IN_ESC|IN_EOF|IN_EOT|IN_CHAR)) {
14672 case ’\t’:
14673 len = 0;
14674 do {
14675 (*tp->tty_echo)(tp, ’ ’);
14676 len++;
14677 } while (len < TAB_SIZE && (tp->tty_position & TAB_MASK) != 0);
14678 break;
14679 case ’\r’ | IN_EOT:
14680 case ’\n’ | IN_EOT:
14681 (*tp->tty_echo)(tp, ch & IN_CHAR);
14682 len = 0;
14683 break;
14684 default:
14685 (*tp->tty_echo)(tp, ’ˆ’);
14686 (*tp->tty_echo)(tp, ’@’ + (ch & IN_CHAR));
14687 len = 2;
14688 }
14689 } else
14690 if ((ch & IN_CHAR) == ’\177’) {
14691 /* A DEL prints as "ˆ?". */
14692 (*tp->tty_echo)(tp, ’ˆ’);
14693 (*tp->tty_echo)(tp, ’?’);
14694 len = 2;
14695 } else {
14696 (*tp->tty_echo)(tp, ch & IN_CHAR);
14697 len = 1;
14698 }
14699 if (ch & IN_EOF) while (len > 0) { (*tp->tty_echo)(tp, ’\b’); len--; }
14700
14701 tp->tty_reprint = rp;
14702 return(ch | (len << IN_LSHIFT));
14703 }
14705 /*===========================================================================*
14706 * rawecho *
14707 *===========================================================================*/
14708 PRIVATE void rawecho(tp, ch)
14709 register tty_t *tp;
830 File: drivers/tty/tty.c MINIX SOURCE CODE
14710 int ch;
14711 {
14712 /* Echo without interpretation if ECHO is set. */
14713 int rp = tp->tty_reprint;
14714 if (tp->tty_termios.c_lflag & ECHO) (*tp->tty_echo)(tp, ch);
14715 tp->tty_reprint = rp;
14716 }
14718 /*===========================================================================*
14719 * back_over *
14720 *===========================================================================*/
14721 PRIVATE int back_over(tp)
14722 register tty_t *tp;
14723 {
14724 /* Backspace to previous character on screen and erase it. */
14725 u16_t *head;
14726 int len;
14727
14728 if (tp->tty_incount == 0) return(0); /* queue empty */
14729 head = tp->tty_inhead;
14730 if (head == tp->tty_inbuf) head = bufend(tp->tty_inbuf);
14731 if (*--head & IN_EOT) return(0); /* can’t erase "line breaks" */
14732 if (tp->tty_reprint) reprint(tp); /* reprint if messed up */
14733 tp->tty_inhead = head;
14734 tp->tty_incount--;
14735 if (tp->tty_termios.c_lflag & ECHOE) {
14736 len = (*head & IN_LEN) >> IN_LSHIFT;
14737 while (len > 0) {
14738 rawecho(tp, ’\b’);
14739 rawecho(tp, ’ ’);
14740 rawecho(tp, ’\b’);
14741 len--;
14742 }
14743 }
14744 return(1); /* one character erased */
14745 }
14747 /*===========================================================================*
14748 * reprint *
14749 *===========================================================================*/
14750 PRIVATE void reprint(tp)
14751 register tty_t *tp; /* pointer to tty struct */
14752 {
14753 /* Restore what has been echoed to screen before if the user input has been
14754 * messed up by output, or if REPRINT (ˆR) is typed.
14755 */
14756 int count;
14757 u16_t *head;
14758
14759 tp->tty_reprint = FALSE;
14760
14761 /* Find the last line break in the input. */
14762 head = tp->tty_inhead;
14763 count = tp->tty_incount;
14764 while (count > 0) {
14765 if (head == tp->tty_inbuf) head = bufend(tp->tty_inbuf);
14766 if (head[-1] & IN_EOT) break;
14767 head--;
14768 count--;
14769 }
MINIX SOURCE CODE File: drivers/tty/tty.c 831
14770 if (count == tp->tty_incount) return; /* no reason to reprint */
14771
14772 /* Show REPRINT (ˆR) and move to a new line. */
14773 (void) tty_echo(tp, tp->tty_termios.c_cc[VREPRINT] | IN_ESC);
14774 rawecho(tp, ’\r’);
14775 rawecho(tp, ’\n’);
14776
14777 /* Reprint from the last break onwards. */
14778 do {
14779 if (head == bufend(tp->tty_inbuf)) head = tp->tty_inbuf;
14780 *head = tty_echo(tp, *head);
14781 head++;
14782 count++;
14783 } while (count < tp->tty_incount);
14784 }
14786 /*===========================================================================*
14787 * out_process *
14788 *===========================================================================*/
14789 PUBLIC void out_process(tp, bstart, bpos, bend, icount, ocount)
14790 tty_t *tp;
14791 char *bstart, *bpos, *bend; /* start/pos/end of circular buffer */
14792 int *icount; /* # input chars / input chars used */
14793 int *ocount; /* max output chars / output chars used */
14794 {
14795 /* Perform output processing on a circular buffer. *icount is the number of
14796 * bytes to process, and the number of bytes actually processed on return.
14797 * *ocount is the space available on input and the space used on output.
14798 * (Naturally *icount < *ocount.) The column position is updated modulo
14799 * the TAB size, because we really only need it for tabs.
14800 */
14801
14802 int tablen;
14803 int ict = *icount;
14804 int oct = *ocount;
14805 int pos = tp->tty_position;
14806
14807 while (ict > 0) {
14808 switch (*bpos) {
14809 case ’\7’:
14810 break;
14811 case ’\b’:
14812 pos--;
14813 break;
14814 case ’\r’:
14815 pos = 0;
14816 break;
14817 case ’\n’:
14818 if ((tp->tty_termios.c_oflag & (OPOST|ONLCR))
14819 == (OPOST|ONLCR)) {
14820 /* Map LF to CR+LF if there is space. Note that the
14821 * next character in the buffer is overwritten, so
14822 * we stop at this point.
14823 */
14824 if (oct >= 2) {
14825 *bpos = ’\r’;
14826 if (++bpos == bend) bpos = bstart;
14827 *bpos = ’\n’;
14828 pos = 0;
14829 ict--;
832 File: drivers/tty/tty.c MINIX SOURCE CODE
14830 oct -= 2;
14831 }
14832 goto out_done; /* no space or buffer got changed */
14833 }
14834 break;
14835 case ’\t’:
14836 /* Best guess for the tab length. */
14837 tablen = TAB_SIZE - (pos & TAB_MASK);
14838
14839 if ((tp->tty_termios.c_oflag & (OPOST|XTABS))
14840 == (OPOST|XTABS)) {
14841 /* Tabs must be expanded. */
14842 if (oct >= tablen) {
14843 pos += tablen;
14844 ict--;
14845 oct -= tablen;
14846 do {
14847 *bpos = ’ ’;
14848 if (++bpos == bend) bpos = bstart;
14849 } while (--tablen != 0);
14850 }
14851 goto out_done;
14852 }
14853 /* Tabs are output directly. */
14854 pos += tablen;
14855 break;
14856 default:
14857 /* Assume any other character prints as one character. */
14858 pos++;
14859 }
14860 if (++bpos == bend) bpos = bstart;
14861 ict--;
14862 oct--;
14863 }
14864 out_done:
14865 tp->tty_position = pos & TAB_MASK;
14866
14867 *icount -= ict; /* [io]ct are the number of chars not used */
14868 *ocount -= oct; /* *[io]count are the number of chars that are used */
14869 }
14871 /*===========================================================================*
14872 * dev_ioctl *
14873 *===========================================================================*/
14874 PRIVATE void dev_ioctl(tp)
14875 tty_t *tp;
14876 {
14877 /* The ioctl’s TCSETSW, TCSETSF and TCDRAIN wait for output to finish to make
14878 * sure that an attribute change doesn’t affect the processing of current
14879 * output. Once output finishes the ioctl is executed as in do_ioctl().
14880 */
14881 int result;
14882
14883 if (tp->tty_outleft > 0) return; /* output not finished */
14884
14885 if (tp->tty_ioreq != TCDRAIN) {
14886 if (tp->tty_ioreq == TCSETSF) tty_icancel(tp);
14887 result = sys_vircopy(tp->tty_ioproc, D, tp->tty_iovir,
14888 SELF, D, (vir_bytes) &tp->tty_termios,
14889 (vir_bytes) sizeof(tp->tty_termios));
MINIX SOURCE CODE File: drivers/tty/tty.c 833
14890 setattr(tp);
14891 }
14892 tp->tty_ioreq = 0;
14893 tty_reply(REVIVE, tp->tty_iocaller, tp->tty_ioproc, result);
14894 }
14896 /*===========================================================================*
14897 * setattr *
14898 *===========================================================================*/
14899 PRIVATE void setattr(tp)
14900 tty_t *tp;
14901 {
14902 /* Apply the new line attributes (raw/canonical, line speed, etc.) */
14903 u16_t *inp;
14904 int count;
14905
14906 if (!(tp->tty_termios.c_lflag & ICANON)) {
14907 /* Raw mode; put a "line break" on all characters in the input queue.
14908 * It is undefined what happens to the input queue when ICANON is
14909 * switched off, a process should use TCSAFLUSH to flush the queue.
14910 * Keeping the queue to preserve typeahead is the Right Thing, however
14911 * when a process does use TCSANOW to switch to raw mode.
14912 */
14913 count = tp->tty_eotct = tp->tty_incount;
14914 inp = tp->tty_intail;
14915 while (count > 0) {
14916 *inp |= IN_EOT;
14917 if (++inp == bufend(tp->tty_inbuf)) inp = tp->tty_inbuf;
14918 --count;
14919 }
14920 }
14921
14922 /* Inspect MIN and TIME. */
14923 settimer(tp, FALSE);
14924 if (tp->tty_termios.c_lflag & ICANON) {
14925 /* No MIN & TIME in canonical mode. */
14926 tp->tty_min = 1;
14927 } else {
14928 /* In raw mode MIN is the number of chars wanted, and TIME how long
14929 * to wait for them. With interesting exceptions if either is zero.
14930 */
14931 tp->tty_min = tp->tty_termios.c_cc[VMIN];
14932 if (tp->tty_min == 0 && tp->tty_termios.c_cc[VTIME] > 0)
14933 tp->tty_min = 1;
14934 }
14935
14936 if (!(tp->tty_termios.c_iflag & IXON)) {
14937 /* No start/stop output control, so don’t leave output inhibited. */
14938 tp->tty_inhibited = RUNNING;
14939 tp->tty_events = 1;
14940 }
14941
14942 /* Setting the output speed to zero hangs up the phone. */
14943 if (tp->tty_termios.c_ospeed == B0) sigchar(tp, SIGHUP);
14944
14945 /* Set new line speed, character size, etc at the device level. */
14946 (*tp->tty_ioctl)(tp, 0);
14947 }
834 File: drivers/tty/tty.c MINIX SOURCE CODE
14949 /*===========================================================================*
14950 * tty_reply *
14951 *===========================================================================*/
14952 PUBLIC void tty_reply(code, replyee, proc_nr, status)
14953 int code; /* TASK_REPLY or REVIVE */
14954 int replyee; /* destination address for the reply */
14955 int proc_nr; /* to whom should the reply go? */
14956 int status; /* reply code */
14957 {
14958 /* Send a reply to a process that wanted to read or write data. */
14959 message tty_mess;
14960
14961 tty_mess.m_type = code;
14962 tty_mess.REP_PROC_NR = proc_nr;
14963 tty_mess.REP_STATUS = status;
14964
14965 if ((status = send(replyee, &tty_mess)) != OK) {
14966 panic("TTY","tty_reply failed, status\n", status);
14967 }
14968 }
14970 /*===========================================================================*
14971 * sigchar *
14972 *===========================================================================*/
14973 PUBLIC void sigchar(tp, sig)
14974 register tty_t *tp;
14975 int sig; /* SIGINT, SIGQUIT, SIGKILL or SIGHUP */
14976 {
14977 /* Process a SIGINT, SIGQUIT or SIGKILL char from the keyboard or SIGHUP from
14978 * a tty close, "stty 0", or a real RS-232 hangup. MM will send the signal to
14979 * the process group (INT, QUIT), all processes (KILL), or the session leader
14980 * (HUP).
14981 */
14982 int status;
14983
14984 if (tp->tty_pgrp != 0)
14985 if (OK != (status = sys_kill(tp->tty_pgrp, sig)))
14986 panic("TTY","Error, call to sys_kill failed", status);
14987
14988 if (!(tp->tty_termios.c_lflag & NOFLSH)) {
14989 tp->tty_incount = tp->tty_eotct = 0; /* kill earlier input */
14990 tp->tty_intail = tp->tty_inhead;
14991 (*tp->tty_ocancel)(tp, 0); /* kill all output */
14992 tp->tty_inhibited = RUNNING;
14993 tp->tty_events = 1;
14994 }
14995 }
14997 /*===========================================================================*
14998 * tty_icancel *
14999 *===========================================================================*/
15000 PRIVATE void tty_icancel(tp)
15001 register tty_t *tp;
15002 {
15003 /* Discard all pending input, tty buffer or device. */
15004
15005 tp->tty_incount = tp->tty_eotct = 0;
15006 tp->tty_intail = tp->tty_inhead;
15007 (*tp->tty_icancel)(tp, 0);
15008 }
MINIX SOURCE CODE File: drivers/tty/tty.c 835
15010 /*===========================================================================*
15011 * tty_init *
15012 *===========================================================================*/
15013 PRIVATE void tty_init()
15014 {
15015 /* Initialize tty structure and call device initialization routines. */
15016
15017 register tty_t *tp;
15018 int s;
15019 struct sigaction sigact;
15020
15021 /* Initialize the terminal lines. */
15022 for (tp = FIRST_TTY,s=0; tp < END_TTY; tp++,s++) {
15023
15024 tp->tty_index = s;
15025
15026 tmr_inittimer(&tp->tty_tmr);
15027
15028 tp->tty_intail = tp->tty_inhead = tp->tty_inbuf;
15029 tp->tty_min = 1;
15030 tp->tty_termios = termios_defaults;
15031 tp->tty_icancel = tp->tty_ocancel = tp->tty_ioctl = tp->tty_close =
15032 tty_devnop;
15033 if (tp < tty_addr(NR_CONS)) {
15034 scr_init(tp);
15035 tp->tty_minor = CONS_MINOR + s;
15036 } else
15037 if (tp < tty_addr(NR_CONS+NR_RS_LINES)) {
15038 rs_init(tp);
15039 tp->tty_minor = RS232_MINOR + s-NR_CONS;
15040 } else {
15041 pty_init(tp);
15042 tp->tty_minor = s - (NR_CONS+NR_RS_LINES) + TTYPX_MINOR;
15043 }
15044 }
15045 }
15047 /*===========================================================================*
15048 * tty_timed_out *
15049 *===========================================================================*/
15050 PRIVATE void tty_timed_out(timer_t *tp)
15051 {
15052 /* This timer has expired. Set the events flag, to force processing. */
15053 tty_t *tty_ptr;
15054 tty_ptr = &tty_table[tmr_arg(tp)->ta_int];
15055 tty_ptr->tty_min = 0; /* force read to succeed */
15056 tty_ptr->tty_events = 1;
15057 }
15059 /*===========================================================================*
15060 * expire_timers *
15061 *===========================================================================*/
15062 PRIVATE void expire_timers(void)
15063 {
15064 /* A synchronous alarm message was received. Check if there are any expired
15065 * timers. Possibly set the event flag and reschedule another alarm.
15066 */
15067 clock_t now; /* current time */
15068 int s;
836 File: drivers/tty/tty.c MINIX SOURCE CODE
15069
15070 /* Get the current time to compare the timers against. */
15071 if ((s=getuptime(&now)) != OK)
15072 panic("TTY","Couldn’t get uptime from clock.", s);
15073
15074 /* Scan the queue of timers for expired timers. This dispatch the watchdog
15075 * functions of expired timers. Possibly a new alarm call must be scheduled.
15076 */
15077 tmrs_exptimers(&tty_timers, now, NULL);
15078 if (tty_timers == NULL) tty_next_timeout = TMR_NEVER;
15079 else { /* set new sync alarm */
15080 tty_next_timeout = tty_timers->tmr_exp_time;
15081 if ((s=sys_setalarm(tty_next_timeout, 1)) != OK)
15082 panic("TTY","Couldn’t set synchronous alarm.", s);
15083 }
15084 }
15086 /*===========================================================================*
15087 * settimer *
15088 *===========================================================================*/
15089 PRIVATE void settimer(tty_ptr, enable)
15090 tty_t *tty_ptr; /* line to set or unset a timer on */
15091 int enable; /* set timer if true, otherwise unset */
15092 {
15093 clock_t now; /* current time */
15094 clock_t exp_time;
15095 int s;
15096
15097 /* Get the current time to calculate the timeout time. */
15098 if ((s=getuptime(&now)) != OK)
15099 panic("TTY","Couldn’t get uptime from clock.", s);
15100 if (enable) {
15101 exp_time = now + tty_ptr->tty_termios.c_cc[VTIME] * (HZ/10);
15102 /* Set a new timer for enabling the TTY events flags. */
15103 tmrs_settimer(&tty_timers, &tty_ptr->tty_tmr,
15104 exp_time, tty_timed_out, NULL);
15105 } else {
15106 /* Remove the timer from the active and expired lists. */
15107 tmrs_clrtimer(&tty_timers, &tty_ptr->tty_tmr, NULL);
15108 }
15109
15110 /* Now check if a new alarm must be scheduled. This happens when the front
15111 * of the timers queue was disabled or reinserted at another position, or
15112 * when a new timer was added to the front.
15113 */
15114 if (tty_timers == NULL) tty_next_timeout = TMR_NEVER;
15115 else if (tty_timers->tmr_exp_time != tty_next_timeout) {
15116 tty_next_timeout = tty_timers->tmr_exp_time;
15117 if ((s=sys_setalarm(tty_next_timeout, 1)) != OK)
15118 panic("TTY","Couldn’t set synchronous alarm.", s);
15119 }
15120 }
15122 /*===========================================================================*
15123 * tty_devnop *
15124 *===========================================================================*/
15125 PUBLIC int tty_devnop(tp, try)
15126 tty_t *tp;
15127 int try;
15128 {
MINIX SOURCE CODE File: drivers/tty/tty.c 837
15129 /* Some functions need not be implemented at the device level. */
15130 }
15132 /*===========================================================================*
15133 * do_select *
15134 *===========================================================================*/
15135 PRIVATE void do_select(tp, m_ptr)
15136 register tty_t *tp; /* pointer to tty struct */
15137 register message *m_ptr; /* pointer to message sent to the task */
15138 {
15139 int ops, ready_ops = 0, watch;
15140
15141 ops = m_ptr->PROC_NR & (SEL_RD|SEL_WR|SEL_ERR);
15142 watch = (m_ptr->PROC_NR & SEL_NOTIFY) ? 1 : 0;
15143
15144 ready_ops = select_try(tp, ops);
15145
15146 if (!ready_ops && ops && watch) {
15147 tp->tty_select_ops |= ops;
15148 tp->tty_select_proc = m_ptr->m_source;
15149 }
15150
15151 tty_reply(TASK_REPLY, m_ptr->m_source, m_ptr->PROC_NR, ready_ops);
15152
15153 return;
15154 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
drivers/tty/keyboard.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
15200 /* Keyboard driver for PC’s and AT’s.
15201 *
15202 * Changes:
15203 * Jul 13, 2004 processes can observe function keys (Jorrit N. Herder)
15204 * Jun 15, 2004 removed wreboot(), except panic dumps (Jorrit N. Herder)
15205 * Feb 04, 1994 loadable keymaps (Marcus Hampel)
15206 */
15207
15208 #include "../drivers.h"
15209 #include <sys/time.h>
15210 #include <sys/select.h>
15211 #include <termios.h>
15212 #include <signal.h>
15213 #include <unistd.h>
15214 #include <minix/callnr.h>
15215 #include <minix/com.h>
15216 #include <minix/keymap.h>
15217 #include "tty.h"
15218 #include "keymaps/us-std.src"
15219 #include "../../kernel/const.h"
15220 #include "../../kernel/config.h"
15221 #include "../../kernel/type.h"
15222 #include "../../kernel/proc.h"
15223
15224 int irq_hook_id = -1;
838 File: drivers/tty/keyboard.c MINIX SOURCE CODE
15225
15226 /* Standard and AT keyboard. (PS/2 MCA implies AT throughout.) */
15227 #define KEYBD 0x60 /* I/O port for keyboard data */
15228
15229 /* AT keyboard. */
15230 #define KB_COMMAND 0x64 /* I/O port for commands on AT */
15231 #define KB_STATUS 0x64 /* I/O port for status on AT */
15232 #define KB_ACK 0xFA /* keyboard ack response */
15233 #define KB_OUT_FULL 0x01 /* status bit set when keypress char pending */
15234 #define KB_IN_FULL 0x02 /* status bit set when not ready to receive */
15235 #define LED_CODE 0xED /* command to keyboard to set LEDs */
15236 #define MAX_KB_ACK_RETRIES 0x1000 /* max #times to wait for kb ack */
15237 #define MAX_KB_BUSY_RETRIES 0x1000 /* max #times to loop while kb busy */
15238 #define KBIT 0x80 /* bit used to ack characters to keyboard */
15239
15240 /* Miscellaneous. */
15241 #define ESC_SCAN 0x01 /* reboot key when panicking */
15242 #define SLASH_SCAN 0x35 /* to recognize numeric slash */
15243 #define RSHIFT_SCAN 0x36 /* to distinguish left and right shift */
15244 #define HOME_SCAN 0x47 /* first key on the numeric keypad */
15245 #define INS_SCAN 0x52 /* INS for use in CTRL-ALT-INS reboot */
15246 #define DEL_SCAN 0x53 /* DEL for use in CTRL-ALT-DEL reboot */
15247
15248 #define CONSOLE 0 /* line number for console */
15249 #define KB_IN_BYTES 32 /* size of keyboard input buffer */
15250 PRIVATE char ibuf[KB_IN_BYTES]; /* input buffer */
15251 PRIVATE char *ihead = ibuf; /* next free spot in input buffer */
15252 PRIVATE char *itail = ibuf; /* scan code to return to TTY */
15253 PRIVATE int icount; /* # codes in buffer */
15254
15255 PRIVATE int esc; /* escape scan code detected? */
15256 PRIVATE int alt_l; /* left alt key state */
15257 PRIVATE int alt_r; /* right alt key state */
15258 PRIVATE int alt; /* either alt key */
15259 PRIVATE int ctrl_l; /* left control key state */
15260 PRIVATE int ctrl_r; /* right control key state */
15261 PRIVATE int ctrl; /* either control key */
15262 PRIVATE int shift_l; /* left shift key state */
15263 PRIVATE int shift_r; /* right shift key state */
15264 PRIVATE int shift; /* either shift key */
15265 PRIVATE int num_down; /* num lock key depressed */
15266 PRIVATE int caps_down; /* caps lock key depressed */
15267 PRIVATE int scroll_down; /* scroll lock key depressed */
15268 PRIVATE int locks[NR_CONS]; /* per console lock keys state */
15269
15270 /* Lock key active bits. Chosen to be equal to the keyboard LED bits. */
15271 #define SCROLL_LOCK 0x01
15272 #define NUM_LOCK 0x02
15273 #define CAPS_LOCK 0x04
15274
15275 PRIVATE char numpad_map[] =
15276 {’H’, ’Y’, ’A’, ’B’, ’D’, ’C’, ’V’, ’U’, ’G’, ’S’, ’T’, ’@’};
15277
15278 /* Variables and definition for observed function keys. */
15279 typedef struct observer { int proc_nr; int events; } obs_t;
15280 PRIVATE obs_t fkey_obs[12]; /* observers for F1-F12 */
15281 PRIVATE obs_t sfkey_obs[12]; /* observers for SHIFT F1-F12 */
15282
15283 FORWARD _PROTOTYPE( int kb_ack, (void) );
15284 FORWARD _PROTOTYPE( int kb_wait, (void) );
MINIX SOURCE CODE File: drivers/tty/keyboard.c 839
15285 FORWARD _PROTOTYPE( int func_key, (int scode) );
15286 FORWARD _PROTOTYPE( int scan_keyboard, (void) );
15287 FORWARD _PROTOTYPE( unsigned make_break, (int scode) );
15288 FORWARD _PROTOTYPE( void set_leds, (void) );
15289 FORWARD _PROTOTYPE( void show_key_mappings, (void) );
15290 FORWARD _PROTOTYPE( int kb_read, (struct tty *tp, int try) );
15291 FORWARD _PROTOTYPE( unsigned map_key, (int scode) );
15292
15293 /*===========================================================================*
15294 * map_key0 *
15295 *===========================================================================*/
15296 /* Map a scan code to an ASCII code ignoring modifiers. */
15297 #define map_key0(scode) \
15298 ((unsigned) keymap[(scode) * MAP_COLS])
15299
15300 /*===========================================================================*
15301 * map_key *
15302 *===========================================================================*/
15303 PRIVATE unsigned map_key(scode)
15304 int scode;
15305 {
15306 /* Map a scan code to an ASCII code. */
15307
15308 int caps, column, lk;
15309 u16_t *keyrow;
15310
15311 if (scode == SLASH_SCAN && esc) return ’/’; /* don’t map numeric slash */
15312
15313 keyrow = &keymap[scode * MAP_COLS];
15314
15315 caps = shift;
15316 lk = locks[ccurrent];
15317 if ((lk & NUM_LOCK) && HOME_SCAN <= scode && scode <= DEL_SCAN) caps = !caps;
15318 if ((lk & CAPS_LOCK) && (keyrow[0] & HASCAPS)) caps = !caps;
15319
15320 if (alt) {
15321 column = 2;
15322 if (ctrl || alt_r) column = 3; /* Ctrl + Alt == AltGr */
15323 if (caps) column = 4;
15324 } else {
15325 column = 0;
15326 if (caps) column = 1;
15327 if (ctrl) column = 5;
15328 }
15329 return keyrow[column] & ˜HASCAPS;
15330 }
15332 /*===========================================================================*
15333 * kbd_interrupt *
15334 *===========================================================================*/
15335 PUBLIC void kbd_interrupt(m_ptr)
15336 message *m_ptr;
15337 {
15338 /* A keyboard interrupt has occurred. Process it. */
15339 int scode;
15340 static timer_t timer; /* timer must be static! */
15341
15342 /* Fetch the character from the keyboard hardware and acknowledge it. */
15343 scode = scan_keyboard();
15344
840 File: drivers/tty/keyboard.c MINIX SOURCE CODE
15345 /* Store the scancode in memory so the task can get at it later. */
15346 if (icount < KB_IN_BYTES) {
15347 *ihead++ = scode;
15348 if (ihead == ibuf + KB_IN_BYTES) ihead = ibuf;
15349 icount++;
15350 tty_table[ccurrent].tty_events = 1;
15351 if (tty_table[ccurrent].tty_select_ops & SEL_RD) {
15352 select_retry(&tty_table[ccurrent]);
15353 }
15354 }
15355 }
15357 /*===========================================================================*
15358 * kb_read *
15359 *===========================================================================*/
15360 PRIVATE int kb_read(tp, try)
15361 tty_t *tp;
15362 int try;
15363 {
15364 /* Process characters from the circular keyboard buffer. */
15365 char buf[3];
15366 int scode;
15367 unsigned ch;
15368
15369 tp = &tty_table[ccurrent]; /* always use the current console */
15370
15371 if (try) {
15372 if (icount > 0) return 1;
15373 return 0;
15374 }
15375
15376 while (icount > 0) {
15377 scode = *itail++; /* take one key scan code */
15378 if (itail == ibuf + KB_IN_BYTES) itail = ibuf;
15379 icount--;
15380
15381 /* Function keys are being used for debug dumps. */
15382 if (func_key(scode)) continue;
15383
15384 /* Perform make/break processing. */
15385 ch = make_break(scode);
15386
15387 if (ch <= 0xFF) {
15388 /* A normal character. */
15389 buf[0] = ch;
15390 (void) in_process(tp, buf, 1);
15391 } else
15392 if (HOME <= ch && ch <= INSRT) {
15393 /* An ASCII escape sequence generated by the numeric pad. */
15394 buf[0] = ESC;
15395 buf[1] = ’[’;
15396 buf[2] = numpad_map[ch - HOME];
15397 (void) in_process(tp, buf, 3);
15398 } else
15399 if (ch == ALEFT) {
15400 /* Choose lower numbered console as current console. */
15401 select_console(ccurrent - 1);
15402 set_leds();
15403 } else
15404 if (ch == ARIGHT) {
MINIX SOURCE CODE File: drivers/tty/keyboard.c 841
15405 /* Choose higher numbered console as current console. */
15406 select_console(ccurrent + 1);
15407 set_leds();
15408 } else
15409 if (AF1 <= ch && ch <= AF12) {
15410 /* Alt-F1 is console, Alt-F2 is ttyc1, etc. */
15411 select_console(ch - AF1);
15412 set_leds();
15413 } else
15414 if (CF1 <= ch && ch <= CF12) {
15415 switch(ch) {
15416 case CF1: show_key_mappings(); break;
15417 case CF3: toggle_scroll(); break; /* hardware <-> software */
15418 case CF7: sigchar(&tty_table[CONSOLE], SIGQUIT); break;
15419 case CF8: sigchar(&tty_table[CONSOLE], SIGINT); break;
15420 case CF9: sigchar(&tty_table[CONSOLE], SIGKILL); break;
15421 }
15422 }
15423 }
15424
15425 return 1;
15426 }
15428 /*===========================================================================*
15429 * make_break *
15430 *===========================================================================*/
15431 PRIVATE unsigned make_break(scode)
15432 int scode; /* scan code of key just struck or released */
15433 {
15434 /* This routine can handle keyboards that interrupt only on key depression,
15435 * as well as keyboards that interrupt on key depression and key release.
15436 * For efficiency, the interrupt routine filters out most key releases.
15437 */
15438 int ch, make, escape;
15439 static int CAD_count = 0;
15440
15441 /* Check for CTRL-ALT-DEL, and if found, halt the computer. This would
15442 * be better done in keyboard() in case TTY is hung, except control and
15443 * alt are set in the high level code.
15444 */
15445 if (ctrl && alt && (scode == DEL_SCAN || scode == INS_SCAN))
15446 {
15447 if (++CAD_count == 3) sys_abort(RBT_HALT);
15448 sys_kill(INIT_PROC_NR, SIGABRT);
15449 return -1;
15450 }
15451
15452 /* High-order bit set on key release. */
15453 make = (scode & KEY_RELEASE) == 0; /* true if pressed */
15454
15455 ch = map_key(scode &= ASCII_MASK); /* map to ASCII */
15456
15457 escape = esc; /* Key is escaped? (true if added since the XT) */
15458 esc = 0;
15459
15460 switch (ch) {
15461 case CTRL: /* Left or right control key */
15462 *(escape ? &ctrl_r : &ctrl_l) = make;
15463 ctrl = ctrl_l | ctrl_r;
15464 break;
842 File: drivers/tty/keyboard.c MINIX SOURCE CODE
15465 case SHIFT: /* Left or right shift key */
15466 *(scode == RSHIFT_SCAN ? &shift_r : &shift_l) = make;
15467 shift = shift_l | shift_r;
15468 break;
15469 case ALT: /* Left or right alt key */
15470 *(escape ? &alt_r : &alt_l) = make;
15471 alt = alt_l | alt_r;
15472 break;
15473 case CALOCK: /* Caps lock - toggle on 0 -> 1 transition */
15474 if (caps_down < make) {
15475 locks[ccurrent] ˆ= CAPS_LOCK;
15476 set_leds();
15477 }
15478 caps_down = make;
15479 break;
15480 case NLOCK: /* Num lock */
15481 if (num_down < make) {
15482 locks[ccurrent] ˆ= NUM_LOCK;
15483 set_leds();
15484 }
15485 num_down = make;
15486 break;
15487 case SLOCK: /* Scroll lock */
15488 if (scroll_down < make) {
15489 locks[ccurrent] ˆ= SCROLL_LOCK;
15490 set_leds();
15491 }
15492 scroll_down = make;
15493 break;
15494 case EXTKEY: /* Escape keycode */
15495 esc = 1; /* Next key is escaped */
15496 return(-1);
15497 default: /* A normal key */
15498 if (make) return(ch);
15499 }
15500
15501 /* Key release, or a shift type key. */
15502 return(-1);
15503 }
15505 /*===========================================================================*
15506 * set_leds *
15507 *===========================================================================*/
15508 PRIVATE void set_leds()
15509 {
15510 /* Set the LEDs on the caps, num, and scroll lock keys */
15511 int s;
15512 if (! machine.pc_at) return; /* PC/XT doesn’t have LEDs */
15513
15514 kb_wait(); /* wait for buffer empty */
15515 if ((s=sys_outb(KEYBD, LED_CODE)) != OK)
15516 printf("Warning, sys_outb couldn’t prepare for LED values: %d\n", s);
15517 /* prepare keyboard to accept LED values */
15518 kb_ack(); /* wait for ack response */
15519
15520 kb_wait(); /* wait for buffer empty */
15521 if ((s=sys_outb(KEYBD, locks[ccurrent])) != OK)
15522 printf("Warning, sys_outb couldn’t give LED values: %d\n", s);
15523 /* give keyboard LED values */
15524 kb_ack(); /* wait for ack response */
MINIX SOURCE CODE File: drivers/tty/keyboard.c 843
15525 }
15527 /*===========================================================================*
15528 * kb_wait *
15529 *===========================================================================*/
15530 PRIVATE int kb_wait()
15531 {
15532 /* Wait until the controller is ready; return zero if this times out. */
15533
15534 int retries, status, temp;
15535 int s;
15536
15537 retries = MAX_KB_BUSY_RETRIES + 1; /* wait until not busy */
15538 do {
15539 s = sys_inb(KB_STATUS, &status);
15540 if (status & KB_OUT_FULL) {
15541 s = sys_inb(KEYBD, &temp); /* discard value */
15542 }
15543 if (! (status & (KB_IN_FULL|KB_OUT_FULL)) )
15544 break; /* wait until ready */
15545 } while (--retries != 0); /* continue unless timeout */
15546 return(retries); /* zero on timeout, positive if ready */
15547 }
15549 /*===========================================================================*
15550 * kb_ack *
15551 *===========================================================================*/
15552 PRIVATE int kb_ack()
15553 {
15554 /* Wait until kbd acknowledges last command; return zero if this times out. */
15555
15556 int retries, s;
15557 u8_t u8val;
15558
15559 retries = MAX_KB_ACK_RETRIES + 1;
15560 do {
15561 s = sys_inb(KEYBD, &u8val);
15562 if (u8val == KB_ACK)
15563 break; /* wait for ack */
15564 } while(--retries != 0); /* continue unless timeout */
15565
15566 return(retries); /* nonzero if ack received */
15567 }
15569 /*===========================================================================*
15570 * kb_init *
15571 *===========================================================================*/
15572 PUBLIC void kb_init(tp)
15573 tty_t *tp;
15574 {
15575 /* Initialize the keyboard driver. */
15576
15577 tp->tty_devread = kb_read; /* input function */
15578 }
15580 /*===========================================================================*
15581 * kb_init_once *
15582 *===========================================================================*/
15583 PUBLIC void kb_init_once(void)
15584 {
844 File: drivers/tty/keyboard.c MINIX SOURCE CODE
15585 int i;
15586
15587 set_leds(); /* turn off numlock led */
15588 scan_keyboard(); /* discard leftover keystroke */
15589
15590 /* Clear the function key observers array. Also see func_key(). */
15591 for (i=0; i<12; i++) {
15592 fkey_obs[i].proc_nr = NONE; /* F1-F12 observers */
15593 fkey_obs[i].events = 0; /* F1-F12 observers */
15594 sfkey_obs[i].proc_nr = NONE; /* Shift F1-F12 observers */
15595 sfkey_obs[i].events = 0; /* Shift F1-F12 observers */
15596 }
15597
15598 /* Set interrupt handler and enable keyboard IRQ. */
15599 irq_hook_id = KEYBOARD_IRQ; /* id to be returned on interrupt */
15600 if ((i=sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE, &irq_hook_id)) != OK)
15601 panic("TTY", "Couldn’t set keyboard IRQ policy", i);
15602 if ((i=sys_irqenable(&irq_hook_id)) != OK)
15603 panic("TTY", "Couldn’t enable keyboard IRQs", i);
15604 kbd_irq_set |= (1 << KEYBOARD_IRQ);
15605 }
15607 /*===========================================================================*
15608 * kbd_loadmap *
15609 *===========================================================================*/
15610 PUBLIC int kbd_loadmap(m)
15611 message *m;
15612 {
15613 /* Load a new keymap. */
15614 int result;
15615 result = sys_vircopy(m->PROC_NR, D, (vir_bytes) m->ADDRESS,
15616 SELF, D, (vir_bytes) keymap,
15617 (vir_bytes) sizeof(keymap));
15618 return(result);
15619 }
15621 /*===========================================================================*
15622 * do_fkey_ctl *
15623 *===========================================================================*/
15624 PUBLIC void do_fkey_ctl(m_ptr)
15625 message *m_ptr; /* pointer to the request message */
15626 {
15627 /* This procedure allows processes to register a function key to receive
15628 * notifications if it is pressed. At most one binding per key can exist.
15629 */
15630 int i;
15631 int result;
15632
15633 switch (m_ptr->FKEY_REQUEST) { /* see what we must do */
15634 case FKEY_MAP: /* request for new mapping */
15635 result = OK; /* assume everything will be ok*/
15636 for (i=0; i < 12; i++) { /* check F1-F12 keys */
15637 if (bit_isset(m_ptr->FKEY_FKEYS, i+1) ) {
15638 if (fkey_obs[i].proc_nr == NONE) {
15639 fkey_obs[i].proc_nr = m_ptr->m_source;
15640 fkey_obs[i].events = 0;
15641 bit_unset(m_ptr->FKEY_FKEYS, i+1);
15642 } else {
15643 printf("WARNING, fkey_map failed F%d\n", i+1);
15644 result = EBUSY; /* report failure, but try rest */
MINIX SOURCE CODE File: drivers/tty/keyboard.c 845
15645 }
15646 }
15647 }
15648 for (i=0; i < 12; i++) { /* check Shift+F1-F12 keys */
15649 if (bit_isset(m_ptr->FKEY_SFKEYS, i+1) ) {
15650 if (sfkey_obs[i].proc_nr == NONE) {
15651 sfkey_obs[i].proc_nr = m_ptr->m_source;
15652 sfkey_obs[i].events = 0;
15653 bit_unset(m_ptr->FKEY_SFKEYS, i+1);
15654 } else {
15655 printf("WARNING, fkey_map failed Shift F%d\n", i+1);
15656 result = EBUSY; /* report failure but try rest */
15657 }
15658 }
15659 }
15660 break;
15661 case FKEY_UNMAP:
15662 result = OK; /* assume everything will be ok*/
15663 for (i=0; i < 12; i++) { /* check F1-F12 keys */
15664 if (bit_isset(m_ptr->FKEY_FKEYS, i+1) ) {
15665 if (fkey_obs[i].proc_nr == m_ptr->m_source) {
15666 fkey_obs[i].proc_nr = NONE;
15667 fkey_obs[i].events = 0;
15668 bit_unset(m_ptr->FKEY_FKEYS, i+1);
15669 } else {
15670 result = EPERM; /* report failure, but try rest */
15671 }
15672 }
15673 }
15674 for (i=0; i < 12; i++) { /* check Shift+F1-F12 keys */
15675 if (bit_isset(m_ptr->FKEY_SFKEYS, i+1) ) {
15676 if (sfkey_obs[i].proc_nr == m_ptr->m_source) {
15677 sfkey_obs[i].proc_nr = NONE;
15678 sfkey_obs[i].events = 0;
15679 bit_unset(m_ptr->FKEY_SFKEYS, i+1);
15680 } else {
15681 result = EPERM; /* report failure, but try rest */
15682 }
15683 }
15684 }
15685 break;
15686 case FKEY_EVENTS:
15687 m_ptr->FKEY_FKEYS = m_ptr->FKEY_SFKEYS = 0;
15688 for (i=0; i < 12; i++) { /* check (Shift+) F1-F12 keys */
15689 if (fkey_obs[i].proc_nr == m_ptr->m_source) {
15690 if (fkey_obs[i].events) {
15691 bit_set(m_ptr->FKEY_FKEYS, i+1);
15692 fkey_obs[i].events = 0;
15693 }
15694 }
15695 if (sfkey_obs[i].proc_nr == m_ptr->m_source) {
15696 if (sfkey_obs[i].events) {
15697 bit_set(m_ptr->FKEY_SFKEYS, i+1);
15698 sfkey_obs[i].events = 0;
15699 }
15700 }
15701 }
15702 break;
15703 default:
15704 result = EINVAL; /* key cannot be observed */
846 File: drivers/tty/keyboard.c MINIX SOURCE CODE
15705 }
15706
15707 /* Almost done, return result to caller. */
15708 m_ptr->m_type = result;
15709 send(m_ptr->m_source, m_ptr);
15710 }
15712 /*===========================================================================*
15713 * func_key *
15714 *===========================================================================*/
15715 PRIVATE int func_key(scode)
15716 int scode; /* scan code for a function key */
15717 {
15718 /* This procedure traps function keys for debugging purposes. Observers of
15719 * function keys are kept in a global array. If a subject (a key) is pressed
15720 * the observer is notified of the event. Initialization of the arrays is done
15721 * in kb_init, where NONE is set to indicate there is no interest in the key.
15722 * Returns FALSE on a key release or if the key is not observable.
15723 */
15724 message m;
15725 int key;
15726 int proc_nr;
15727 int i,s;
15728
15729 /* Ignore key releases. If this is a key press, get full key code. */
15730 if (scode & KEY_RELEASE) return(FALSE); /* key release */
15731 key = map_key(scode); /* include modifiers */
15732
15733 /* Key pressed, now see if there is an observer for the pressed key.
15734 * F1-F12 observers are in fkey_obs array.
15735 * SHIFT F1-F12 observers are in sfkey_req array.
15736 * CTRL F1-F12 reserved (see kb_read)
15737 * ALT F1-F12 reserved (see kb_read)
15738 * Other combinations are not in use. Note that Alt+Shift+F1-F12 is yet
15739 * defined in <minix/keymap.h>, and thus is easy for future extensions.
15740 */
15741 if (F1 <= key && key <= F12) { /* F1-F12 */
15742 proc_nr = fkey_obs[key - F1].proc_nr;
15743 fkey_obs[key - F1].events ++ ;
15744 } else if (SF1 <= key && key <= SF12) { /* Shift F2-F12 */
15745 proc_nr = sfkey_obs[key - SF1].proc_nr;
15746 sfkey_obs[key - SF1].events ++;
15747 }
15748 else {
15749 return(FALSE); /* not observable */
15750 }
15751
15752 /* See if an observer is registered and send it a message. */
15753 if (proc_nr != NONE) {
15754 m.NOTIFY_TYPE = FKEY_PRESSED;
15755 notify(proc_nr);
15756 }
15757 return(TRUE);
15758 }
15760 /*===========================================================================*
15761 * show_key_mappings *
15762 *===========================================================================*/
15763 PRIVATE void show_key_mappings()
15764 {
MINIX SOURCE CODE File: drivers/tty/keyboard.c 847
15765 int i,s;
15766 struct proc proc;
15767
15768 printf("\n");
15769 printf("System information. Known function key mappings to request debug dumps:\n");
15770 printf("-------------------------------------------------------------------------\n");
15771 for (i=0; i<12; i++) {
15772
15773 printf(" %sF%d: ", i+1<10? " ":"", i+1);
15774 if (fkey_obs[i].proc_nr != NONE) {
15775 if ((s=sys_getproc(&proc, fkey_obs[i].proc_nr))!=OK)
15776 printf("sys_getproc: %d\n", s);
15777 printf("%-14.14s", proc.p_name);
15778 } else {
15779 printf("%-14.14s", "<none>");
15780 }
15781
15782 printf(" %sShift-F%d: ", i+1<10? " ":"", i+1);
15783 if (sfkey_obs[i].proc_nr != NONE) {
15784 if ((s=sys_getproc(&proc, sfkey_obs[i].proc_nr))!=OK)
15785 printf("sys_getproc: %d\n", s);
15786 printf("%-14.14s", proc.p_name);
15787 } else {
15788 printf("%-14.14s", "<none>");
15789 }
15790 printf("\n");
15791 }
15792 printf("\n");
15793 printf("Press one of the registered function keys to trigger a debug dump.\n");
15794 printf("\n");
15795 }
15797 /*===========================================================================*
15798 * scan_keyboard *
15799 *===========================================================================*/
15800 PRIVATE int scan_keyboard()
15801 {
15802 /* Fetch the character from the keyboard hardware and acknowledge it. */
15803 pvb_pair_t byte_in[2], byte_out[2];
15804
15805 byte_in[0].port = KEYBD; /* get the scan code for the key struck */
15806 byte_in[1].port = PORT_B; /* strobe the keyboard to ack the char */
15807 sys_vinb(byte_in, 2); /* request actual input */
15808
15809 pv_set(byte_out[0], PORT_B, byte_in[1].value | KBIT); /* strobe bit high */
15810 pv_set(byte_out[1], PORT_B, byte_in[1].value); /* then strobe low */
15811 sys_voutb(byte_out, 2); /* request actual output */
15812
15813 return(byte_in[0].value); /* return scan code */
15814 }
15816 /*===========================================================================*
15817 * do_panic_dumps *
15818 *===========================================================================*/
15819 PUBLIC void do_panic_dumps(m)
15820 message *m; /* request message to TTY */
15821 {
15822 /* Wait for keystrokes for printing debugging info and reboot. */
15823 int quiet, code;
15824
848 File: drivers/tty/keyboard.c MINIX SOURCE CODE
15825 /* A panic! Allow debug dumps until user wants to shutdown. */
15826 printf("\nHit ESC to reboot, DEL to shutdown, F-keys for debug dumps\n");
15827
15828 (void) scan_keyboard(); /* ack any old input */
15829 quiet = scan_keyboard();/* quiescent value (0 on PC, last code on AT)*/
15830 for (;;) {
15831 tickdelay(10);
15832 /* See if there are pending request for output, but don’t block.
15833 * Diagnostics can span multiple printf()s, so do it in a loop.
15834 */
15835 while (nb_receive(ANY, m) == OK) {
15836 switch(m->m_type) {
15837 case FKEY_CONTROL: do_fkey_ctl(m); break;
15838 case SYS_SIG: do_new_kmess(m); break;
15839 case DIAGNOSTICS: do_diagnostics(m); break;
15840 default: ; /* do nothing */
15841 }
15842 tickdelay(1); /* allow more */
15843 }
15844 code = scan_keyboard();
15845 if (code != quiet) {
15846 /* A key has been pressed. */
15847 switch (code) { /* possibly abort MINIX */
15848 case ESC_SCAN: sys_abort(RBT_REBOOT); return;
15849 case DEL_SCAN: sys_abort(RBT_HALT); return;
15850 }
15851 (void) func_key(code); /* check for function key */
15852 quiet = scan_keyboard();
15853 }
15854 }
15855 }
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
drivers/tty/console.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
15900 /* Code and data for the IBM console driver.
15901 *
15902 * The 6845 video controller used by the IBM PC shares its video memory with
15903 * the CPU somewhere in the 0xB0000 memory bank. To the 6845 this memory
15904 * consists of 16-bit words. Each word has a character code in the low byte
15905 * and a so-called attribute byte in the high byte. The CPU directly modifies
15906 * video memory to display characters, and sets two registers on the 6845 that
15907 * specify the video origin and the cursor position. The video origin is the
15908 * place in video memory where the first character (upper left corner) can
15909 * be found. Moving the origin is a fast way to scroll the screen. Some
15910 * video adapters wrap around the top of video memory, so the origin can
15911 * move without bounds. For other adapters screen memory must sometimes be
15912 * moved to reset the origin. All computations on video memory use character
15913 * (word) addresses for simplicity and assume there is no wrapping. The
15914 * assembly support functions translate the word addresses to byte addresses
15915 * and the scrolling function worries about wrapping.
15916 */
15917
15918 #include "../drivers.h"
15919 #include <termios.h>
MINIX SOURCE CODE File: drivers/tty/console.c 849
15920 #include <minix/callnr.h>
15921 #include <minix/com.h>
15922 #include "tty.h"
15923
15924 #include "../../kernel/const.h"
15925 #include "../../kernel/config.h"
15926 #include "../../kernel/type.h"
15927
15928 /* Definitions used by the console driver. */
15929 #define MONO_BASE 0xB0000L /* base of mono video memory */
15930 #define COLOR_BASE 0xB8000L /* base of color video memory */
15931 #define MONO_SIZE 0x1000 /* 4K mono video memory */
15932 #define COLOR_SIZE 0x4000 /* 16K color video memory */
15933 #define EGA_SIZE 0x8000 /* EGA & VGA have at least 32K */
15934 #define BLANK_COLOR 0x0700 /* determines cursor color on blank screen */
15935 #define SCROLL_UP 0 /* scroll forward */
15936 #define SCROLL_DOWN 1 /* scroll backward */
15937 #define BLANK_MEM ((u16_t *) 0) /* tells mem_vid_copy() to blank the screen */
15938 #define CONS_RAM_WORDS 80 /* video ram buffer size */
15939 #define MAX_ESC_PARMS 4 /* number of escape sequence params allowed */
15940
15941 /* Constants relating to the controller chips. */
15942 #define M_6845 0x3B4 /* port for 6845 mono */
15943 #define C_6845 0x3D4 /* port for 6845 color */
15944 #define INDEX 0 /* 6845’s index register */
15945 #define DATA 1 /* 6845’s data register */
15946 #define STATUS 6 /* 6845’s status register */
15947 #define VID_ORG 12 /* 6845’s origin register */
15948 #define CURSOR 14 /* 6845’s cursor register */
15949
15950 /* Beeper. */
15951 #define BEEP_FREQ 0x0533 /* value to put into timer to set beep freq */
15952 #define B_TIME 3 /* length of CTRL-G beep is ticks */
15953
15954 /* definitions used for font management */
15955 #define GA_SEQUENCER_INDEX 0x3C4
15956 #define GA_SEQUENCER_DATA 0x3C5
15957 #define GA_GRAPHICS_INDEX 0x3CE
15958 #define GA_GRAPHICS_DATA 0x3CF
15959 #define GA_VIDEO_ADDRESS 0xA0000L
15960 #define GA_FONT_SIZE 8192
15961
15962 /* Global variables used by the console driver and assembly support. */
15963 PUBLIC int vid_index; /* index of video segment in remote mem map */
15964 PUBLIC u16_t vid_seg;
15965 PUBLIC vir_bytes vid_off; /* video ram is found at vid_seg:vid_off */
15966 PUBLIC unsigned vid_size; /* 0x2000 for color or 0x0800 for mono */
15967 PUBLIC unsigned vid_mask; /* 0x1FFF for color or 0x07FF for mono */
15968 PUBLIC unsigned blank_color = BLANK_COLOR; /* display code for blank */
15969
15970 /* Private variables used by the console driver. */
15971 PRIVATE int vid_port; /* I/O port for accessing 6845 */
15972 PRIVATE int wrap; /* hardware can wrap? */
15973 PRIVATE int softscroll; /* 1 = software scrolling, 0 = hardware */
15974 PRIVATE int beeping; /* speaker is beeping? */
15975 PRIVATE unsigned font_lines; /* font lines per character */
15976 PRIVATE unsigned scr_width; /* # characters on a line */
15977 PRIVATE unsigned scr_lines; /* # lines on the screen */
15978 PRIVATE unsigned scr_size; /* # characters on the screen */
15979
850 File: drivers/tty/console.c MINIX SOURCE CODE
15980 /* Per console data. */
15981 typedef struct console {
15982 tty_t *c_tty; /* associated TTY struct */
15983 int c_column; /* current column number (0-origin) */
15984 int c_row; /* current row (0 at top of screen) */
15985 int c_rwords; /* number of WORDS (not bytes) in outqueue */
15986 unsigned c_start; /* start of video memory of this console */
15987 unsigned c_limit; /* limit of this console’s video memory */
15988 unsigned c_org; /* location in RAM where 6845 base points */
15989 unsigned c_cur; /* current position of cursor in video RAM */
15990 unsigned c_attr; /* character attribute */
15991 unsigned c_blank; /* blank attribute */
15992 char c_reverse; /* reverse video */
15993 char c_esc_state; /* 0=normal, 1=ESC, 2=ESC[ */
15994 char c_esc_intro; /* Distinguishing character following ESC */
15995 int *c_esc_parmp; /* pointer to current escape parameter */
15996 int c_esc_parmv[MAX_ESC_PARMS]; /* list of escape parameters */
15997 u16_t c_ramqueue[CONS_RAM_WORDS]; /* buffer for video RAM */
15998 } console_t;
15999
16000 PRIVATE int nr_cons= 1; /* actual number of consoles */
16001 PRIVATE console_t cons_table[NR_CONS];
16002 PRIVATE console_t *curcons; /* currently visible */
16003
16004 /* Color if using a color controller. */
16005 #define color (vid_port == C_6845)
16006
16007 /* Map from ANSI colors to the attributes used by the PC */
16008 PRIVATE int ansi_colors[8] = {0, 4, 2, 6, 1, 5, 3, 7};
16009
16010 /* Structure used for font management */
16011 struct sequence {
16012 unsigned short index;
16013 unsigned char port;
16014 unsigned char value;
16015 };
16016
16017 FORWARD _PROTOTYPE( int cons_write, (struct tty *tp, int try) );
16018 FORWARD _PROTOTYPE( void cons_echo, (tty_t *tp, int c) );
16019 FORWARD _PROTOTYPE( void out_char, (console_t *cons, int c) );
16020 FORWARD _PROTOTYPE( void putk, (int c) );
16021 FORWARD _PROTOTYPE( void beep, (void) );
16022 FORWARD _PROTOTYPE( void do_escape, (console_t *cons, int c) );
16023 FORWARD _PROTOTYPE( void flush, (console_t *cons) );
16024 FORWARD _PROTOTYPE( void parse_escape, (console_t *cons, int c) );
16025 FORWARD _PROTOTYPE( void scroll_screen, (console_t *cons, int dir) );
16026 FORWARD _PROTOTYPE( void set_6845, (int reg, unsigned val) );
16027 FORWARD _PROTOTYPE( void get_6845, (int reg, unsigned *val) );
16028 FORWARD _PROTOTYPE( void stop_beep, (timer_t *tmrp) );
16029 FORWARD _PROTOTYPE( void cons_org0, (void) );
16030 FORWARD _PROTOTYPE( int ga_program, (struct sequence *seq) );
16031 FORWARD _PROTOTYPE( int cons_ioctl, (tty_t *tp, int) );
16032
16033 /*===========================================================================*
16034 * cons_write *
16035 *===========================================================================*/
16036 PRIVATE int cons_write(tp, try)
16037 register struct tty *tp; /* tells which terminal is to be used */
16038 int try;
16039 {
MINIX SOURCE CODE File: drivers/tty/console.c 851
16040 /* Copy as much data as possible to the output queue, then start I/O. On
16041 * memory-mapped terminals, such as the IBM console, the I/O will also be
16042 * finished, and the counts updated. Keep repeating until all I/O done.
16043 */
16044
16045 int count;
16046 int result;
16047 register char *tbuf;
16048 char buf[64];
16049 console_t *cons = tp->tty_priv;
16050
16051 if (try) return 1; /* we can always write to console */
16052
16053 /* Check quickly for nothing to do, so this can be called often without
16054 * unmodular tests elsewhere.
16055 */
16056 if ((count = tp->tty_outleft) == 0 || tp->tty_inhibited) return;
16057
16058 /* Copy the user bytes to buf[] for decent addressing. Loop over the
16059 * copies, since the user buffer may be much larger than buf[].
16060 */
16061 do {
16062 if (count > sizeof(buf)) count = sizeof(buf);
16063 if ((result = sys_vircopy(tp->tty_outproc, D, tp->tty_out_vir,
16064 SELF, D, (vir_bytes) buf, (vir_bytes) count)) != OK)
16065 break;
16066 tbuf = buf;
16067
16068 /* Update terminal data structure. */
16069 tp->tty_out_vir += count;
16070 tp->tty_outcum += count;
16071 tp->tty_outleft -= count;
16072
16073 /* Output each byte of the copy to the screen. Avoid calling
16074 * out_char() for the "easy" characters, put them into the buffer
16075 * directly.
16076 */
16077 do {
16078 if ((unsigned) *tbuf < ’ ’ || cons->c_esc_state > 0
16079 || cons->c_column >= scr_width
16080 || cons->c_rwords >= buflen(cons->c_ramqueue))
16081 {
16082 out_char(cons, *tbuf++);
16083 } else {
16084 cons->c_ramqueue[cons->c_rwords++] =
16085 cons->c_attr | (*tbuf++ & BYTE);
16086 cons->c_column++;
16087 }
16088 } while (--count != 0);
16089 } while ((count = tp->tty_outleft) != 0 && !tp->tty_inhibited);
16090
16091 flush(cons); /* transfer anything buffered to the screen */
16092
16093 /* Reply to the writer if all output is finished or if an error occured. */
16094 if (tp->tty_outleft == 0 || result != OK) {
16095 /* REVIVE is not possible. I/O on memory mapped consoles finishes. */
16096 tty_reply(tp->tty_outrepcode, tp->tty_outcaller, tp->tty_outproc,
16097 tp->tty_outcum);
16098 tp->tty_outcum = 0;
16099 }
852 File: drivers/tty/console.c MINIX SOURCE CODE
16100 }
16102 /*===========================================================================*
16103 * cons_echo *
16104 *===========================================================================*/
16105 PRIVATE void cons_echo(tp, c)
16106 register tty_t *tp; /* pointer to tty struct */
16107 int c; /* character to be echoed */
16108 {
16109 /* Echo keyboard input (print & flush). */
16110 console_t *cons = tp->tty_priv;
16111
16112 out_char(cons, c);
16113 flush(cons);
16114 }
16116 /*===========================================================================*
16117 * out_char *
16118 *===========================================================================*/
16119 PRIVATE void out_char(cons, c)
16120 register console_t *cons; /* pointer to console struct */
16121 int c; /* character to be output */
16122 {
16123 /* Output a character on the console. Check for escape sequences first. */
16124 if (cons->c_esc_state > 0) {
16125 parse_escape(cons, c);
16126 return;
16127 }
16128
16129 switch(c) {
16130 case 000: /* null is typically used for padding */
16131 return; /* better not do anything */
16132
16133 case 007: /* ring the bell */
16134 flush(cons); /* print any chars queued for output */
16135 beep();
16136 return;
16137
16138 case ’\b’: /* backspace */
16139 if (--cons->c_column < 0) {
16140 if (--cons->c_row >= 0) cons->c_column += scr_width;
16141 }
16142 flush(cons);
16143 return;
16144
16145 case ’\n’: /* line feed */
16146 if ((cons->c_tty->tty_termios.c_oflag & (OPOST|ONLCR))
16147 == (OPOST|ONLCR)) {
16148 cons->c_column = 0;
16149 }
16150 /*FALL THROUGH*/
16151 case 013: /* CTRL-K */
16152 case 014: /* CTRL-L */
16153 if (cons->c_row == scr_lines-1) {
16154 scroll_screen(cons, SCROLL_UP);
16155 } else {
16156 cons->c_row++;
16157 }
16158 flush(cons);
16159 return;
MINIX SOURCE CODE File: drivers/tty/console.c 853
16160
16161 case ’\r’: /* carriage return */
16162 cons->c_column = 0;
16163 flush(cons);
16164 return;
16165
16166 case ’\t’: /* tab */
16167 cons->c_column = (cons->c_column + TAB_SIZE) & ˜TAB_MASK;
16168 if (cons->c_column > scr_width) {
16169 cons->c_column -= scr_width;
16170 if (cons->c_row == scr_lines-1) {
16171 scroll_screen(cons, SCROLL_UP);
16172 } else {
16173 cons->c_row++;
16174 }
16175 }
16176 flush(cons);
16177 return;
16178
16179 case 033: /* ESC - start of an escape sequence */
16180 flush(cons); /* print any chars queued for output */
16181 cons->c_esc_state = 1; /* mark ESC as seen */
16182 return;
16183
16184 default: /* printable chars are stored in ramqueue */
16185 if (cons->c_column >= scr_width) {
16186 if (!LINEWRAP) return;
16187 if (cons->c_row == scr_lines-1) {
16188 scroll_screen(cons, SCROLL_UP);
16189 } else {
16190 cons->c_row++;
16191 }
16192 cons->c_column = 0;
16193 flush(cons);
16194 }
16195 if (cons->c_rwords == buflen(cons->c_ramqueue)) flush(cons);
16196 cons->c_ramqueue[cons->c_rwords++] = cons->c_attr | (c & BYTE);
16197 cons->c_column++; /* next column */
16198 return;
16199 }
16200 }
16202 /*===========================================================================*
16203 * scroll_screen *
16204 *===========================================================================*/
16205 PRIVATE void scroll_screen(cons, dir)
16206 register console_t *cons; /* pointer to console struct */
16207 int dir; /* SCROLL_UP or SCROLL_DOWN */
16208 {
16209 unsigned new_line, new_org, chars;
16210
16211 flush(cons);
16212 chars = scr_size - scr_width; /* one screen minus one line */
16213
16214 /* Scrolling the screen is a real nuisance due to the various incompatible
16215 * video cards. This driver supports software scrolling (Hercules?),
16216 * hardware scrolling (mono and CGA cards) and hardware scrolling without
16217 * wrapping (EGA cards). In the latter case we must make sure that
16218 * c_start <= c_org && c_org + scr_size <= c_limit
16219 * holds, because EGA doesn’t wrap around the end of video memory.
854 File: drivers/tty/console.c MINIX SOURCE CODE
16220 */
16221 if (dir == SCROLL_UP) {
16222 /* Scroll one line up in 3 ways: soft, avoid wrap, use origin. */
16223 if (softscroll) {
16224 vid_vid_copy(cons->c_start + scr_width, cons->c_start, chars);
16225 } else
16226 if (!wrap && cons->c_org + scr_size + scr_width >= cons->c_limit) {
16227 vid_vid_copy(cons->c_org + scr_width, cons->c_start, chars);
16228 cons->c_org = cons->c_start;
16229 } else {
16230 cons->c_org = (cons->c_org + scr_width) & vid_mask;
16231 }
16232 new_line = (cons->c_org + chars) & vid_mask;
16233 } else {
16234 /* Scroll one line down in 3 ways: soft, avoid wrap, use origin. */
16235 if (softscroll) {
16236 vid_vid_copy(cons->c_start, cons->c_start + scr_width, chars);
16237 } else
16238 if (!wrap && cons->c_org < cons->c_start + scr_width) {
16239 new_org = cons->c_limit - scr_size;
16240 vid_vid_copy(cons->c_org, new_org + scr_width, chars);
16241 cons->c_org = new_org;
16242 } else {
16243 cons->c_org = (cons->c_org - scr_width) & vid_mask;
16244 }
16245 new_line = cons->c_org;
16246 }
16247 /* Blank the new line at top or bottom. */
16248 blank_color = cons->c_blank;
16249 mem_vid_copy(BLANK_MEM, new_line, scr_width);
16250
16251 /* Set the new video origin. */
16252 if (cons == curcons) set_6845(VID_ORG, cons->c_org);
16253 flush(cons);
16254 }
16256 /*===========================================================================*
16257 * flush *
16258 *===========================================================================*/
16259 PRIVATE void flush(cons)
16260 register console_t *cons; /* pointer to console struct */
16261 {
16262 /* Send characters buffered in ’ramqueue’ to screen memory, check the new
16263 * cursor position, compute the new hardware cursor position and set it.
16264 */
16265 unsigned cur;
16266 tty_t *tp = cons->c_tty;
16267
16268 /* Have the characters in ’ramqueue’ transferred to the screen. */
16269 if (cons->c_rwords > 0) {
16270 mem_vid_copy(cons->c_ramqueue, cons->c_cur, cons->c_rwords);
16271 cons->c_rwords = 0;
16272
16273 /* TTY likes to know the current column and if echoing messed up. */
16274 tp->tty_position = cons->c_column;
16275 tp->tty_reprint = TRUE;
16276 }
16277
16278 /* Check and update the cursor position. */
16279 if (cons->c_column < 0) cons->c_column = 0;
MINIX SOURCE CODE File: drivers/tty/console.c 855
16280 if (cons->c_column > scr_width) cons->c_column = scr_width;
16281 if (cons->c_row < 0) cons->c_row = 0;
16282 if (cons->c_row >= scr_lines) cons->c_row = scr_lines - 1;
16283 cur = cons->c_org + cons->c_row * scr_width + cons->c_column;
16284 if (cur != cons->c_cur) {
16285 if (cons == curcons) set_6845(CURSOR, cur);
16286 cons->c_cur = cur;
16287 }
16288 }
16290 /*===========================================================================*
16291 * parse_escape *
16292 *===========================================================================*/
16293 PRIVATE void parse_escape(cons, c)
16294 register console_t *cons; /* pointer to console struct */
16295 char c; /* next character in escape sequence */
16296 {
16297 /* The following ANSI escape sequences are currently supported.
16298 * If n and/or m are omitted, they default to 1.
16299 * ESC [nA moves up n lines
16300 * ESC [nB moves down n lines
16301 * ESC [nC moves right n spaces
16302 * ESC [nD moves left n spaces
16303 * ESC [m;nH" moves cursor to (m,n)
16304 * ESC [J clears screen from cursor
16305 * ESC [K clears line from cursor
16306 * ESC [nL inserts n lines ar cursor
16307 * ESC [nM deletes n lines at cursor
16308 * ESC [nP deletes n chars at cursor
16309 * ESC [n@ inserts n chars at cursor
16310 * ESC [nm enables rendition n (0=normal, 4=bold, 5=blinking, 7=reverse)
16311 * ESC M scrolls the screen backwards if the cursor is on the top line
16312 */
16313
16314 switch (cons->c_esc_state) {
16315 case 1: /* ESC seen */
16316 cons->c_esc_intro = ’\0’;
16317 cons->c_esc_parmp = bufend(cons->c_esc_parmv);
16318 do {
16319 *--cons->c_esc_parmp = 0;
16320 } while (cons->c_esc_parmp > cons->c_esc_parmv);
16321 switch (c) {
16322 case ’[’: /* Control Sequence Introducer */
16323 cons->c_esc_intro = c;
16324 cons->c_esc_state = 2;
16325 break;
16326 case ’M’: /* Reverse Index */
16327 do_escape(cons, c);
16328 break;
16329 default:
16330 cons->c_esc_state = 0;
16331 }
16332 break;
16333
16334 case 2: /* ESC [ seen */
16335 if (c >= ’0’ && c <= ’9’) {
16336 if (cons->c_esc_parmp < bufend(cons->c_esc_parmv))
16337 *cons->c_esc_parmp = *cons->c_esc_parmp * 10 + (c-’0’);
16338 } else
16339 if (c == ’;’) {
856 File: drivers/tty/console.c MINIX SOURCE CODE
16340 if (cons->c_esc_parmp < bufend(cons->c_esc_parmv))
16341 cons->c_esc_parmp++;
16342 } else {
16343 do_escape(cons, c);
16344 }
16345 break;
16346 }
16347 }
16349 /*===========================================================================*
16350 * do_escape *
16351 *===========================================================================*/
16352 PRIVATE void do_escape(cons, c)
16353 register console_t *cons; /* pointer to console struct */
16354 char c; /* next character in escape sequence */
16355 {
16356 int value, n;
16357 unsigned src, dst, count;
16358 int *parmp;
16359
16360 /* Some of these things hack on screen RAM, so it had better be up to date */
16361 flush(cons);
16362
16363 if (cons->c_esc_intro == ’\0’) {
16364 /* Handle a sequence beginning with just ESC */
16365 switch (c) {
16366 case ’M’: /* Reverse Index */
16367 if (cons->c_row == 0) {
16368 scroll_screen(cons, SCROLL_DOWN);
16369 } else {
16370 cons->c_row--;
16371 }
16372 flush(cons);
16373 break;
16374
16375 default: break;
16376 }
16377 } else
16378 if (cons->c_esc_intro == ’[’) {
16379 /* Handle a sequence beginning with ESC [ and parameters */
16380 value = cons->c_esc_parmv[0];
16381 switch (c) {
16382 case ’A’: /* ESC [nA moves up n lines */
16383 n = (value == 0 ? 1 : value);
16384 cons->c_row -= n;
16385 flush(cons);
16386 break;
16387
16388 case ’B’: /* ESC [nB moves down n lines */
16389 n = (value == 0 ? 1 : value);
16390 cons->c_row += n;
16391 flush(cons);
16392 break;
16393
16394 case ’C’: /* ESC [nC moves right n spaces */
16395 n = (value == 0 ? 1 : value);
16396 cons->c_column += n;
16397 flush(cons);
16398 break;
16399
MINIX SOURCE CODE File: drivers/tty/console.c 857
16400 case ’D’: /* ESC [nD moves left n spaces */
16401 n = (value == 0 ? 1 : value);
16402 cons->c_column -= n;
16403 flush(cons);
16404 break;
16405
16406 case ’H’: /* ESC [m;nH" moves cursor to (m,n) */
16407 cons->c_row = cons->c_esc_parmv[0] - 1;
16408 cons->c_column = cons->c_esc_parmv[1] - 1;
16409 flush(cons);
16410 break;
16411
16412 case ’J’: /* ESC [sJ clears in display */
16413 switch (value) {
16414 case 0: /* Clear from cursor to end of screen */
16415 count = scr_size - (cons->c_cur - cons->c_org);
16416 dst = cons->c_cur;
16417 break;
16418 case 1: /* Clear from start of screen to cursor */
16419 count = cons->c_cur - cons->c_org;
16420 dst = cons->c_org;
16421 break;
16422 case 2: /* Clear entire screen */
16423 count = scr_size;
16424 dst = cons->c_org;
16425 break;
16426 default: /* Do nothing */
16427 count = 0;
16428 dst = cons->c_org;
16429 }
16430 blank_color = cons->c_blank;
16431 mem_vid_copy(BLANK_MEM, dst, count);
16432 break;
16433
16434 case ’K’: /* ESC [sK clears line from cursor */
16435 switch (value) {
16436 case 0: /* Clear from cursor to end of line */
16437 count = scr_width - cons->c_column;
16438 dst = cons->c_cur;
16439 break;
16440 case 1: /* Clear from beginning of line to cursor */
16441 count = cons->c_column;
16442 dst = cons->c_cur - cons->c_column;
16443 break;
16444 case 2: /* Clear entire line */
16445 count = scr_width;
16446 dst = cons->c_cur - cons->c_column;
16447 break;
16448 default: /* Do nothing */
16449 count = 0;
16450 dst = cons->c_cur;
16451 }
16452 blank_color = cons->c_blank;
16453 mem_vid_copy(BLANK_MEM, dst, count);
16454 break;
16455
16456 case ’L’: /* ESC [nL inserts n lines at cursor */
16457 n = value;
16458 if (n < 1) n = 1;
16459 if (n > (scr_lines - cons->c_row))
858 File: drivers/tty/console.c MINIX SOURCE CODE
16460 n = scr_lines - cons->c_row;
16461
16462 src = cons->c_org + cons->c_row * scr_width;
16463 dst = src + n * scr_width;
16464 count = (scr_lines - cons->c_row - n) * scr_width;
16465 vid_vid_copy(src, dst, count);
16466 blank_color = cons->c_blank;
16467 mem_vid_copy(BLANK_MEM, src, n * scr_width);
16468 break;
16469
16470 case ’M’: /* ESC [nM deletes n lines at cursor */
16471 n = value;
16472 if (n < 1) n = 1;
16473 if (n > (scr_lines - cons->c_row))
16474 n = scr_lines - cons->c_row;
16475
16476 dst = cons->c_org + cons->c_row * scr_width;
16477 src = dst + n * scr_width;
16478 count = (scr_lines - cons->c_row - n) * scr_width;
16479 vid_vid_copy(src, dst, count);
16480 blank_color = cons->c_blank;
16481 mem_vid_copy(BLANK_MEM, dst + count, n * scr_width);
16482 break;
16483
16484 case ’@’: /* ESC [n@ inserts n chars at cursor */
16485 n = value;
16486 if (n < 1) n = 1;
16487 if (n > (scr_width - cons->c_column))
16488 n = scr_width - cons->c_column;
16489
16490 src = cons->c_cur;
16491 dst = src + n;
16492 count = scr_width - cons->c_column - n;
16493 vid_vid_copy(src, dst, count);
16494 blank_color = cons->c_blank;
16495 mem_vid_copy(BLANK_MEM, src, n);
16496 break;
16497
16498 case ’P’: /* ESC [nP deletes n chars at cursor */
16499 n = value;
16500 if (n < 1) n = 1;
16501 if (n > (scr_width - cons->c_column))
16502 n = scr_width - cons->c_column;
16503
16504 dst = cons->c_cur;
16505 src = dst + n;
16506 count = scr_width - cons->c_column - n;
16507 vid_vid_copy(src, dst, count);
16508 blank_color = cons->c_blank;
16509 mem_vid_copy(BLANK_MEM, dst + count, n);
16510 break;
16511
16512 case ’m’: /* ESC [nm enables rendition n */
16513 for (parmp = cons->c_esc_parmv; parmp <= cons->c_esc_parmp
16514 && parmp < bufend(cons->c_esc_parmv); parmp++) {
16515 if (cons->c_reverse) {
16516 /* Unswap fg and bg colors */
16517 cons->c_attr = ((cons->c_attr & 0x7000) >> 4) |
16518 ((cons->c_attr & 0x0700) << 4) |
16519 ((cons->c_attr & 0x8800));
MINIX SOURCE CODE File: drivers/tty/console.c 859
16520 }
16521 switch (n = *parmp) {
16522 case 0: /* NORMAL */
16523 cons->c_attr = cons->c_blank = BLANK_COLOR;
16524 cons->c_reverse = FALSE;
16525 break;
16526
16527 case 1: /* BOLD */
16528 /* Set intensity bit */
16529 cons->c_attr |= 0x0800;
16530 break;
16531
16532 case 4: /* UNDERLINE */
16533 if (color) {
16534 /* Change white to cyan, i.e. lose red
16535 */
16536 cons->c_attr = (cons->c_attr & 0xBBFF);
16537 } else {
16538 /* Set underline attribute */
16539 cons->c_attr = (cons->c_attr & 0x99FF);
16540 }
16541 break;
16542
16543 case 5: /* BLINKING */
16544 /* Set the blink bit */
16545 cons->c_attr |= 0x8000;
16546 break;
16547
16548 case 7: /* REVERSE */
16549 cons->c_reverse = TRUE;
16550 break;
16551
16552 default: /* COLOR */
16553 if (n == 39) n = 37; /* set default color */
16554 if (n == 49) n = 40;
16555
16556 if (!color) {
16557 /* Don’t mess up a monochrome screen */
16558 } else
16559 if (30 <= n && n <= 37) {
16560 /* Foreground color */
16561 cons->c_attr =
16562 (cons->c_attr & 0xF8FF) |
16563 (ansi_colors[(n - 30)] << 8);
16564 cons->c_blank =
16565 (cons->c_blank & 0xF8FF) |
16566 (ansi_colors[(n - 30)] << 8);
16567 } else
16568 if (40 <= n && n <= 47) {
16569 /* Background color */
16570 cons->c_attr =
16571 (cons->c_attr & 0x8FFF) |
16572 (ansi_colors[(n - 40)] << 12);
16573 cons->c_blank =
16574 (cons->c_blank & 0x8FFF) |
16575 (ansi_colors[(n - 40)] << 12);
16576 }
16577 }
16578 if (cons->c_reverse) {
16579 /* Swap fg and bg colors */
860 File: drivers/tty/console.c MINIX SOURCE CODE
16580 cons->c_attr = ((cons->c_attr & 0x7000) >> 4) |
16581 ((cons->c_attr & 0x0700) << 4) |
16582 ((cons->c_attr & 0x8800));
16583 }
16584 }
16585 break;
16586 }
16587 }
16588 cons->c_esc_state = 0;
16589 }
16591 /*===========================================================================*
16592 * set_6845 *
16593 *===========================================================================*/
16594 PRIVATE void set_6845(reg, val)
16595 int reg; /* which register pair to set */
16596 unsigned val; /* 16-bit value to set it to */
16597 {
16598 /* Set a register pair inside the 6845.
16599 * Registers 12-13 tell the 6845 where in video ram to start
16600 * Registers 14-15 tell the 6845 where to put the cursor
16601 */
16602 pvb_pair_t char_out[4];
16603 pv_set(char_out[0], vid_port + INDEX, reg); /* set index register */
16604 pv_set(char_out[1], vid_port + DATA, (val>>8) & BYTE); /* high byte */
16605 pv_set(char_out[2], vid_port + INDEX, reg + 1); /* again */
16606 pv_set(char_out[3], vid_port + DATA, val&BYTE); /* low byte */
16607 sys_voutb(char_out, 4); /* do actual output */
16608 }
16610 /*===========================================================================*
16611 * get_6845 *
16612 *===========================================================================*/
16613 PRIVATE void get_6845(reg, val)
16614 int reg; /* which register pair to set */
16615 unsigned *val; /* 16-bit value to set it to */
16616 {
16617 char v1, v2;
16618 /* Get a register pair inside the 6845. */
16619 sys_outb(vid_port + INDEX, reg);
16620 sys_inb(vid_port + DATA, &v1);
16621 sys_outb(vid_port + INDEX, reg+1);
16622 sys_inb(vid_port + DATA, &v2);
16623 *val = (v1 << 8) | v2;
16624 }
16626 /*===========================================================================*
16627 * beep *
16628 *===========================================================================*/
16629 PRIVATE void beep()
16630 {
16631 /* Making a beeping sound on the speaker (output for CRTL-G).
16632 * This routine works by turning on the bits 0 and 1 in port B of the 8255
16633 * chip that drive the speaker.
16634 */
16635 static timer_t tmr_stop_beep;
16636 pvb_pair_t char_out[3];
16637 clock_t now;
16638 int port_b_val, s;
16639
MINIX SOURCE CODE File: drivers/tty/console.c 861
16640 /* Fetch current time in advance to prevent beeping delay. */
16641 if ((s=getuptime(&now)) != OK)
16642 panic("TTY","Console couldn’t get clock’s uptime.", s);
16643 if (!beeping) {
16644 /* Set timer channel 2, square wave, with given frequency. */
16645 pv_set(char_out[0], TIMER_MODE, 0xB6);
16646 pv_set(char_out[1], TIMER2, (BEEP_FREQ >> 0) & BYTE);
16647 pv_set(char_out[2], TIMER2, (BEEP_FREQ >> 8) & BYTE);
16648 if (sys_voutb(char_out, 3)==OK) {
16649 if (sys_inb(PORT_B, &port_b_val)==OK &&
16650 sys_outb(PORT_B, (port_b_val|3))==OK)
16651 beeping = TRUE;
16652 }
16653 }
16654 /* Add a timer to the timers list. Possibly reschedule the alarm. */
16655 tmrs_settimer(&tty_timers, &tmr_stop_beep, now+B_TIME, stop_beep, NULL);
16656 if (tty_timers->tmr_exp_time != tty_next_timeout) {
16657 tty_next_timeout = tty_timers->tmr_exp_time;
16658 if ((s=sys_setalarm(tty_next_timeout, 1)) != OK)
16659 panic("TTY","Console couldn’t set alarm.", s);
16660 }
16661 }
16663 /*===========================================================================*
16664 * stop_beep *
16665 *===========================================================================*/
16666 PRIVATE void stop_beep(tmrp)
16667 timer_t *tmrp;
16668 {
16669 /* Turn off the beeper by turning off bits 0 and 1 in PORT_B. */
16670 int port_b_val;
16671 if (sys_inb(PORT_B, &port_b_val)==OK &&
16672 sys_outb(PORT_B, (port_b_val & ˜3))==OK)
16673 beeping = FALSE;
16674 }
16676 /*===========================================================================*
16677 * scr_init *
16678 *===========================================================================*/
16679 PUBLIC void scr_init(tp)
16680 tty_t *tp;
16681 {
16682 /* Initialize the screen driver. */
16683 console_t *cons;
16684 phys_bytes vid_base;
16685 u16_t bios_columns, bios_crtbase, bios_fontlines;
16686 u8_t bios_rows;
16687 int line;
16688 int s;
16689 static int vdu_initialized = 0;
16690 unsigned page_size;
16691
16692 /* Associate console and TTY. */
16693 line = tp - &tty_table[0];
16694 if (line >= nr_cons) return;
16695 cons = &cons_table[line];
16696 cons->c_tty = tp;
16697 tp->tty_priv = cons;
16698
16699 /* Initialize the keyboard driver. */
862 File: drivers/tty/console.c MINIX SOURCE CODE
16700 kb_init(tp);
16701
16702 /* Fill in TTY function hooks. */
16703 tp->tty_devwrite = cons_write;
16704 tp->tty_echo = cons_echo;
16705 tp->tty_ioctl = cons_ioctl;
16706
16707 /* Get the BIOS parameters that describe the VDU. */
16708 if (! vdu_initialized++) {
16709
16710 /* How about error checking? What to do on failure??? */
16711 s=sys_vircopy(SELF, BIOS_SEG, (vir_bytes) VDU_SCREEN_COLS_ADDR,
16712 SELF, D, (vir_bytes) &bios_columns, VDU_SCREEN_COLS_SIZE);
16713 s=sys_vircopy(SELF, BIOS_SEG, (vir_bytes) VDU_CRT_BASE_ADDR,
16714 SELF, D, (vir_bytes) &bios_crtbase, VDU_CRT_BASE_SIZE);
16715 s=sys_vircopy(SELF, BIOS_SEG, (vir_bytes) VDU_SCREEN_ROWS_ADDR,
16716 SELF, D, (vir_bytes) &bios_rows, VDU_SCREEN_ROWS_SIZE);
16717 s=sys_vircopy(SELF, BIOS_SEG, (vir_bytes) VDU_FONTLINES_ADDR,
16718 SELF, D, (vir_bytes) &bios_fontlines, VDU_FONTLINES_SIZE);
16719
16720 vid_port = bios_crtbase;
16721 scr_width = bios_columns;
16722 font_lines = bios_fontlines;
16723 scr_lines = machine.vdu_ega ? bios_rows+1 : 25;
16724
16725 if (color) {
16726 vid_base = COLOR_BASE;
16727 vid_size = COLOR_SIZE;
16728 } else {
16729 vid_base = MONO_BASE;
16730 vid_size = MONO_SIZE;
16731 }
16732 if (machine.vdu_ega) vid_size = EGA_SIZE;
16733 wrap = ! machine.vdu_ega;
16734
16735 s = sys_segctl(&vid_index, &vid_seg, &vid_off, vid_base, vid_size);
16736
16737 vid_size >>= 1; /* word count */
16738 vid_mask = vid_size - 1;
16739
16740 /* Size of the screen (number of displayed characters.) */
16741 scr_size = scr_lines * scr_width;
16742
16743 /* There can be as many consoles as video memory allows. */
16744 nr_cons = vid_size / scr_size;
16745 if (nr_cons > NR_CONS) nr_cons = NR_CONS;
16746 if (nr_cons > 1) wrap = 0;
16747 page_size = vid_size / nr_cons;
16748 }
16749
16750 cons->c_start = line * page_size;
16751 cons->c_limit = cons->c_start + page_size;
16752 cons->c_cur = cons->c_org = cons->c_start;
16753 cons->c_attr = cons->c_blank = BLANK_COLOR;
16754
16755 if (line != 0) {
16756 /* Clear the non-console vtys. */
16757 blank_color = BLANK_COLOR;
16758 mem_vid_copy(BLANK_MEM, cons->c_start, scr_size);
16759 } else {
MINIX SOURCE CODE File: drivers/tty/console.c 863
16760 int i, n;
16761 /* Set the cursor of the console vty at the bottom. c_cur
16762 * is updated automatically later.
16763 */
16764 scroll_screen(cons, SCROLL_UP);
16765 cons->c_row = scr_lines - 1;
16766 cons->c_column = 0;
16767 }
16768 select_console(0);
16769 cons_ioctl(tp, 0);
16770 }
16772 /*===========================================================================*
16773 * kputc *
16774 *===========================================================================*/
16775 PUBLIC void kputc(c)
16776 int c;
16777 {
16778 putk(c);
16779 }
16781 /*===========================================================================*
16782 * do_new_kmess *
16783 *===========================================================================*/
16784 PUBLIC void do_new_kmess(m)
16785 message *m;
16786 {
16787 /* Notification for a new kernel message. */
16788 struct kmessages kmess; /* kmessages structure */
16789 static int prev_next = 0; /* previous next seen */
16790 int size, next;
16791 int bytes;
16792 int r;
16793
16794 /* Try to get a fresh copy of the buffer with kernel messages. */
16795 sys_getkmessages(&kmess);
16796
16797 /* Print only the new part. Determine how many new bytes there are with
16798 * help of the current and previous ’next’ index. Note that the kernel
16799 * buffer is circular. This works fine if less then KMESS_BUF_SIZE bytes
16800 * is new data; else we miss % KMESS_BUF_SIZE here.
16801 * Check for size being positive, the buffer might as well be emptied!
16802 */
16803 if (kmess.km_size > 0) {
16804 bytes = ((kmess.km_next + KMESS_BUF_SIZE) - prev_next) % KMESS_BUF_SIZE;
16805 r=prev_next; /* start at previous old */
16806 while (bytes > 0) {
16807 putk( kmess.km_buf[(r%KMESS_BUF_SIZE)] );
16808 bytes --;
16809 r ++;
16810 }
16811 putk(0); /* terminate to flush output */
16812 }
16813
16814 /* Almost done, store ’next’ so that we can determine what part of the
16815 * kernel messages buffer to print next time a notification arrives.
16816 */
16817 prev_next = kmess.km_next;
16818 }
864 File: drivers/tty/console.c MINIX SOURCE CODE
16820 /*===========================================================================*
16821 * do_diagnostics *
16822 *===========================================================================*/
16823 PUBLIC void do_diagnostics(m_ptr)
16824 message *m_ptr; /* pointer to request message */
16825 {
16826 /* Print a string for a server. */
16827 char c;
16828 vir_bytes src;
16829 int count;
16830 int result = OK;
16831 int proc_nr = m_ptr->DIAG_PROC_NR;
16832 if (proc_nr == SELF) proc_nr = m_ptr->m_source;
16833
16834 src = (vir_bytes) m_ptr->DIAG_PRINT_BUF;
16835 for (count = m_ptr->DIAG_BUF_COUNT; count > 0; count--) {
16836 if (sys_vircopy(proc_nr, D, src++, SELF, D, (vir_bytes) &c, 1) != OK) {
16837 result = EFAULT;
16838 break;
16839 }
16840 putk(c);
16841 }
16842 putk(0); /* always terminate, even with EFAULT */
16843 m_ptr->m_type = result;
16844 send(m_ptr->m_source, m_ptr);
16845 }
16847 /*===========================================================================*
16848 * putk *
16849 *===========================================================================*/
16850 PRIVATE void putk(c)
16851 int c; /* character to print */
16852 {
16853 /* This procedure is used by the version of printf() that is linked with
16854 * the TTY driver. The one in the library sends a message to FS, which is
16855 * not what is needed for printing within the TTY. This version just queues
16856 * the character and starts the output.
16857 */
16858 if (c != 0) {
16859 if (c == ’\n’) putk(’\r’);
16860 out_char(&cons_table[0], (int) c);
16861 } else {
16862 flush(&cons_table[0]);
16863 }
16864 }
16866 /*===========================================================================*
16867 * toggle_scroll *
16868 *===========================================================================*/
16869 PUBLIC void toggle_scroll()
16870 {
16871 /* Toggle between hardware and software scroll. */
16872
16873 cons_org0();
16874 softscroll = !softscroll;
16875 printf("%sware scrolling enabled.\n", softscroll ? "Soft" : "Hard");
16876 }
MINIX SOURCE CODE File: drivers/tty/console.c 865
16878 /*===========================================================================*
16879 * cons_stop *
16880 *===========================================================================*/
16881 PUBLIC void cons_stop()
16882 {
16883 /* Prepare for halt or reboot. */
16884 cons_org0();
16885 softscroll = 1;
16886 select_console(0);
16887 cons_table[0].c_attr = cons_table[0].c_blank = BLANK_COLOR;
16888 }
16890 /*===========================================================================*
16891 * cons_org0 *
16892 *===========================================================================*/
16893 PRIVATE void cons_org0()
16894 {
16895 /* Scroll video memory back to put the origin at 0. */
16896 int cons_line;
16897 console_t *cons;
16898 unsigned n;
16899
16900 for (cons_line = 0; cons_line < nr_cons; cons_line++) {
16901 cons = &cons_table[cons_line];
16902 while (cons->c_org > cons->c_start) {
16903 n = vid_size - scr_size; /* amount of unused memory */
16904 if (n > cons->c_org - cons->c_start)
16905 n = cons->c_org - cons->c_start;
16906 vid_vid_copy(cons->c_org, cons->c_org - n, scr_size);
16907 cons->c_org -= n;
16908 }
16909 flush(cons);
16910 }
16911 select_console(ccurrent);
16912 }
16914 /*===========================================================================*
16915 * select_console *
16916 *===========================================================================*/
16917 PUBLIC void select_console(int cons_line)
16918 {
16919 /* Set the current console to console number ’cons_line’. */
16920
16921 if (cons_line < 0 || cons_line >= nr_cons) return;
16922 ccurrent = cons_line;
16923 curcons = &cons_table[cons_line];
16924 set_6845(VID_ORG, curcons->c_org);
16925 set_6845(CURSOR, curcons->c_cur);
16926 }
16928 /*===========================================================================*
16929 * con_loadfont *
16930 *===========================================================================*/
16931 PUBLIC int con_loadfont(m)
16932 message *m;
16933 {
16934 /* Load a font into the EGA or VGA adapter. */
16935 int result;
16936 static struct sequence seq1[7] = {
16937 { GA_SEQUENCER_INDEX, 0x00, 0x01 },
866 File: drivers/tty/console.c MINIX SOURCE CODE
16938 { GA_SEQUENCER_INDEX, 0x02, 0x04 },
16939 { GA_SEQUENCER_INDEX, 0x04, 0x07 },
16940 { GA_SEQUENCER_INDEX, 0x00, 0x03 },
16941 { GA_GRAPHICS_INDEX, 0x04, 0x02 },
16942 { GA_GRAPHICS_INDEX, 0x05, 0x00 },
16943 { GA_GRAPHICS_INDEX, 0x06, 0x00 },
16944 };
16945 static struct sequence seq2[7] = {
16946 { GA_SEQUENCER_INDEX, 0x00, 0x01 },
16947 { GA_SEQUENCER_INDEX, 0x02, 0x03 },
16948 { GA_SEQUENCER_INDEX, 0x04, 0x03 },
16949 { GA_SEQUENCER_INDEX, 0x00, 0x03 },
16950 { GA_GRAPHICS_INDEX, 0x04, 0x00 },
16951 { GA_GRAPHICS_INDEX, 0x05, 0x10 },
16952 { GA_GRAPHICS_INDEX, 0x06, 0 },
16953 };
16954
16955 seq2[6].value= color ? 0x0E : 0x0A;
16956
16957 if (!machine.vdu_ega) return(ENOTTY);
16958 result = ga_program(seq1); /* bring font memory into view */
16959
16960 result = sys_physcopy(m->PROC_NR, D, (vir_bytes) m->ADDRESS,
16961 NONE, PHYS_SEG, (phys_bytes) GA_VIDEO_ADDRESS, (phys_bytes)GA_FONT_SIZE);
16962
16963 result = ga_program(seq2); /* restore */
16964
16965 return(result);
16966 }
16968 /*===========================================================================*
16969 * ga_program *
16970 *===========================================================================*/
16971 PRIVATE int ga_program(seq)
16972 struct sequence *seq;
16973 {
16974 pvb_pair_t char_out[14];
16975 int i;
16976 for (i=0; i<7; i++) {
16977 pv_set(char_out[2*i], seq->index, seq->port);
16978 pv_set(char_out[2*i+1], seq->index+1, seq->value);
16979 seq++;
16980 }
16981 return sys_voutb(char_out, 14);
16982 }
16984 /*===========================================================================*
16985 * cons_ioctl *
16986 *===========================================================================*/
16987 PRIVATE int cons_ioctl(tp, try)
16988 tty_t *tp;
16989 int try;
16990 {
16991 /* Set the screen dimensions. */
16992
16993 tp->tty_winsize.ws_row= scr_lines;
16994 tp->tty_winsize.ws_col= scr_width;
16995 tp->tty_winsize.ws_xpixel= scr_width * 8;
16996 tp->tty_winsize.ws_ypixel= scr_lines * font_lines;
16997 }