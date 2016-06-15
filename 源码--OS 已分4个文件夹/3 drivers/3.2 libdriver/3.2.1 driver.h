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