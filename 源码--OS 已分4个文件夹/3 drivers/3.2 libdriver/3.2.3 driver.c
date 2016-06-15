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