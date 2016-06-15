MINIX SOURCE CODE File: servers/fs/fs.h 919
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/fs.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
20900 /* This is the master header for fs. It includes some other files
20901 * and defines the principal constants.
20902 */
20903 #define _POSIX_SOURCE 1 /* tell headers to include POSIX stuff */
20904 #define _MINIX 1 /* tell headers to include MINIX stuff */
20905 #define _SYSTEM 1 /* tell headers that this is the kernel */
20906
20907 #define VERBOSE 0 /* show messages during initialization? */
20908
20909 /* The following are so basic, all the *.c files get them automatically. */
20910 #include <minix/config.h> /* MUST be first */
20911 #include <ansi.h> /* MUST be second */
20912 #include <sys/types.h>
20913 #include <minix/const.h>
20914 #include <minix/type.h>
20915 #include <minix/dmap.h>
20916
20917 #include <limits.h>
20918 #include <errno.h>
20919
20920 #include <minix/syslib.h>
20921 #include <minix/sysutil.h>
20922
20923 #include "const.h"
20924 #include "type.h"
20925 #include "proto.h"
20926 #include "glo.h"