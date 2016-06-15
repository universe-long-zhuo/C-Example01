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