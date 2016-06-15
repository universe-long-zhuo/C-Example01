++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/dmap.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
28100 /* This file contains the table with device <-> driver mappings. It also
28101 * contains some routines to dynamically add and/ or remove device drivers
28102 * or change mappings.
28103 */
28104
28105 #include "fs.h"
28106 #include "fproc.h"
28107 #include <string.h>
28108 #include <stdlib.h>
28109 #include <ctype.h>
28110 #include <unistd.h>
28111 #include <minix/com.h>
28112 #include "param.h"
28113
28114 /* Some devices may or may not be there in the next table. */
28115 #define DT(enable, opcl, io, driver, flags) \
28116 { (enable?(opcl):no_dev), (enable?(io):0), \
28117 (enable?(driver):0), (flags) },
28118 #define NC(x) (NR_CTRLRS >= (x))
28119
28120 /* The order of the entries here determines the mapping between major device
28121 * numbers and tasks. The first entry (major device 0) is not used. The
28122 * next entry is major device 1, etc. Character and block devices can be
28123 * intermixed at random. The ordering determines the device numbers in /dev/.
28124 * Note that FS knows the device number of /dev/ram/ to load the RAM disk.
28125 * Also note that the major device numbers used in /dev/ are NOT the same as
28126 * the process numbers of the device drivers.
28127 */
28128 /*
28129 Driver enabled Open/Cls I/O Driver # Flags Device File
28130 -------------- -------- ------ ----------- ----- ------ ----
28131 */
28132 struct dmap dmap[NR_DEVICES]; /* actual map */
28133 PRIVATE struct dmap init_dmap[] = {
28134 DT(1, no_dev, 0, 0, 0) /* 0 = not used */
28135 DT(1, gen_opcl, gen_io, MEM_PROC_NR, 0) /* 1 = /dev/mem */
28136 DT(0, no_dev, 0, 0, DMAP_MUTABLE) /* 2 = /dev/fd0 */
28137 DT(0, no_dev, 0, 0, DMAP_MUTABLE) /* 3 = /dev/c0 */
28138 DT(1, tty_opcl, gen_io, TTY_PROC_NR, 0) /* 4 = /dev/tty00 */
28139 DT(1, ctty_opcl,ctty_io, TTY_PROC_NR, 0) /* 5 = /dev/tty */
28140 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /* 6 = /dev/lp */
28141 DT(1, no_dev, 0, 0, DMAP_MUTABLE) /* 7 = /dev/ip */
28142 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /* 8 = /dev/c1 */
28143 DT(0, 0, 0, 0, DMAP_MUTABLE) /* 9 = not used */
28144 DT(0, no_dev, 0, 0, DMAP_MUTABLE) /*10 = /dev/c2 */
1020 File: servers/fs/dmap.c MINIX SOURCE CODE
28145 DT(0, 0, 0, 0, DMAP_MUTABLE) /*11 = not used */
28146 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /*12 = /dev/c3 */
28147 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /*13 = /dev/audio */
28148 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /*14 = /dev/mixer */
28149 DT(1, gen_opcl, gen_io, LOG_PROC_NR, 0) /*15 = /dev/klog */
28150 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /*16 = /dev/random*/
28151 DT(0, no_dev, 0, NONE, DMAP_MUTABLE) /*17 = /dev/cmos */
28152 };
28153
28154 /*===========================================================================*
28155 * do_devctl *
28156 *===========================================================================*/
28157 PUBLIC int do_devctl()
28158 {
28159 int result;
28160
28161 switch(m_in.ctl_req) {
28162 case DEV_MAP:
28163 /* Try to update device mapping. */
28164 result = map_driver(m_in.dev_nr, m_in.driver_nr, m_in.dev_style);
28165 break;
28166 case DEV_UNMAP:
28167 result = ENOSYS;
28168 break;
28169 default:
28170 result = EINVAL;
28171 }
28172 return(result);
28173 }
28175 /*===========================================================================*
28176 * map_driver *
28177 *===========================================================================*/
28178 PUBLIC int map_driver(major, proc_nr, style)
28179 int major; /* major number of the device */
28180 int proc_nr; /* process number of the driver */
28181 int style; /* style of the device */
28182 {
28183 /* Set a new device driver mapping in the dmap table. Given that correct
28184 * arguments are given, this only works if the entry is mutable and the
28185 * current driver is not busy.
28186 * Normal error codes are returned so that this function can be used from
28187 * a system call that tries to dynamically install a new driver.
28188 */
28189 struct dmap *dp;
28190
28191 /* Get pointer to device entry in the dmap table. */
28192 if (major >= NR_DEVICES) return(ENODEV);
28193 dp = &dmap[major];
28194
28195 /* See if updating the entry is allowed. */
28196 if (! (dp->dmap_flags & DMAP_MUTABLE)) return(EPERM);
28197 if (dp->dmap_flags & DMAP_BUSY) return(EBUSY);
28198
28199 /* Check process number of new driver. */
28200 if (! isokprocnr(proc_nr)) return(EINVAL);
28201
28202 /* Try to update the entry. */
28203 switch (style) {
28204 case STYLE_DEV: dp->dmap_opcl = gen_opcl; break;
MINIX SOURCE CODE File: servers/fs/dmap.c 1021
28205 case STYLE_TTY: dp->dmap_opcl = tty_opcl; break;
28206 case STYLE_CLONE: dp->dmap_opcl = clone_opcl; break;
28207 default: return(EINVAL);
28208 }
28209 dp->dmap_io = gen_io;
28210 dp->dmap_driver = proc_nr;
28211 return(OK);
28212 }
28214 /*===========================================================================*
28215 * build_dmap *
28216 *===========================================================================*/
28217 PUBLIC void build_dmap()
28218 {
28219 /* Initialize the table with all device <-> driver mappings. Then, map
28220 * the boot driver to a controller and update the dmap table to that
28221 * selection. The boot driver and the controller it handles are set at
28222 * the boot monitor.
28223 */
28224 char driver[16];
28225 char *controller = "c##";
28226 int nr, major = -1;
28227 int i,s;
28228 struct dmap *dp;
28229
28230 /* Build table with device <-> driver mappings. */
28231 for (i=0; i<NR_DEVICES; i++) {
28232 dp = &dmap[i];
28233 if (i < sizeof(init_dmap)/sizeof(struct dmap) &&
28234 init_dmap[i].dmap_opcl != no_dev) { /* a preset driver */
28235 dp->dmap_opcl = init_dmap[i].dmap_opcl;
28236 dp->dmap_io = init_dmap[i].dmap_io;
28237 dp->dmap_driver = init_dmap[i].dmap_driver;
28238 dp->dmap_flags = init_dmap[i].dmap_flags;
28239 } else { /* no default */
28240 dp->dmap_opcl = no_dev;
28241 dp->dmap_io = 0;
28242 dp->dmap_driver = 0;
28243 dp->dmap_flags = DMAP_MUTABLE;
28244 }
28245 }
28246
28247 /* Get settings of ’controller’ and ’driver’ at the boot monitor. */
28248 if ((s = env_get_param("label", driver, sizeof(driver))) != OK)
28249 panic(__FILE__,"couldn’t get boot monitor parameter ’driver’", s);
28250 if ((s = env_get_param("controller", controller, sizeof(controller))) != OK)
28251 panic(__FILE__,"couldn’t get boot monitor parameter ’controller’", s);
28252
28253 /* Determine major number to map driver onto. */
28254 if (controller[0] == ’f’ && controller[1] == ’d’) {
28255 major = FLOPPY_MAJOR;
28256 }
28257 else if (controller[0] == ’c’ && isdigit(controller[1])) {
28258 if ((nr = (unsigned) atoi(&controller[1])) > NR_CTRLRS)
28259 panic(__FILE__,"monitor ’controller’ maximum ’c#’ is", NR_CTRLRS);
28260 major = CTRLR(nr);
28261 }
28262 else {
28263 panic(__FILE__,"monitor ’controller’ syntax is ’c#’ of ’fd’", NO_NUM);
28264 }
1022 File: servers/fs/dmap.c MINIX SOURCE CODE
28265
28266 /* Now try to set the actual mapping and report to the user. */
28267 if ((s=map_driver(major, DRVR_PROC_NR, STYLE_DEV)) != OK)
28268 panic(__FILE__,"map_driver failed",s);
28269 printf("Boot medium driver: %s driver mapped onto controller %s.\n",
28270 driver, controller);
28271 }