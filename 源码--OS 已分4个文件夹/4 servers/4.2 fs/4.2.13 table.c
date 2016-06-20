++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/table.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This file contains the table used to map system call numbers onto the
* routines that perform them.
*/

#define _TABLE

#include "fs.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include "buf.h"
#include "file.h"
#include "fproc.h"
#include "inode.h"
#include "lock.h"
#include "super.h"

do_stat,       /* 18 = stat */
no_sys,        /* 49 = unused */
no_sys,        /* 50 = unused */
no_sys,        /* 57 = unused */
no_sys,        /* 58 = unused */
no_sys,        /* 66 = unused */
no_sys,        /* 69 = unused */
no_sys,        /* 70 = unused */
no_sys,        /* 71 = si */
no_sys,        /* 78 = unused */
no_sys,        /* 80 = unused */
};
/* This should not fail with "array size is negative": */
extern int dummy[sizeof(call_vec) == NCALLS * sizeof(call_vec[0]) ? 1 : -1];
