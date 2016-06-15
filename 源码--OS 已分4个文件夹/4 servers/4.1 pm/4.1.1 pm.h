MINIX SOURCE CODE File: servers/pm/pm.h 867
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/pm/pm.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
17000 /* This is the master header for PM. It includes some other files
17001 * and defines the principal constants.
17002 */
17003 #define _POSIX_SOURCE 1 /* tell headers to include POSIX stuff */
17004 #define _MINIX 1 /* tell headers to include MINIX stuff */
17005 #define _SYSTEM 1 /* tell headers that this is the kernel */
17006
17007 /* The following are so basic, all the *.c files get them automatically. */
17008 #include <minix/config.h> /* MUST be first */
17009 #include <ansi.h> /* MUST be second */
17010 #include <sys/types.h>
17011 #include <minix/const.h>
17012 #include <minix/type.h>
17013
17014 #include <fcntl.h>
17015 #include <unistd.h>
17016 #include <minix/syslib.h>
17017 #include <minix/sysutil.h>
17018
17019 #include <limits.h>
17020 #include <errno.h>
17021
17022 #include "const.h"
17023 #include "type.h"
17024 #include "proto.h"
17025 #include "glo.h"