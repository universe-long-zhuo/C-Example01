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