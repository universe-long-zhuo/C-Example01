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