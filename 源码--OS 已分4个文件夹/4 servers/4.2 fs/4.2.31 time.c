++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/time.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
28800 /* This file takes care of those system calls that deal with time.
28801 *
28802 * The entry points into this file are
28803 * do_utime: perform the UTIME system call
28804 * do_stime: PM informs FS about STIME system call
28805 */
28806
28807 #include "fs.h"
28808 #include <minix/callnr.h>
28809 #include <minix/com.h>
1030 File: servers/fs/time.c MINIX SOURCE CODE
28810 #include "file.h"
28811 #include "fproc.h"
28812 #include "inode.h"
28813 #include "param.h"
28814
28815 /*===========================================================================*
28816 * do_utime *
28817 *===========================================================================*/
28818 PUBLIC int do_utime()
28819 {
28820 /* Perform the utime(name, timep) system call. */
28821
28822 register struct inode *rip;
28823 register int len, r;
28824
28825 /* Adjust for case of ’timep’ being NULL;
28826 * utime_strlen then holds the actual size: strlen(name)+1.
28827 */
28828 len = m_in.utime_length;
28829 if (len == 0) len = m_in.utime_strlen;
28830
28831 /* Temporarily open the file. */
28832 if (fetch_name(m_in.utime_file, len, M1) != OK) return(err_code);
28833 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
28834
28835 /* Only the owner of a file or the super_user can change its time. */
28836 r = OK;
28837 if (rip->i_uid != fp->fp_effuid && !super_user) r = EPERM;
28838 if (m_in.utime_length == 0 && r != OK) r = forbidden(rip, W_BIT);
28839 if (read_only(rip) != OK) r = EROFS; /* not even su can touch if R/O */
28840 if (r == OK) {
28841   if (m_in.utime_length == 0) {
28842     rip->i_atime = clock_time();
28843     rip->i_mtime = rip->i_atime;
28844   } else {
28845     rip->i_atime = m_in.utime_actime;
28846     rip->i_mtime = m_in.utime_modtime;
28847   }
28848 rip->i_update = CTIME; /* discard any stale ATIME and MTIME flags */
28849 rip->i_dirt = DIRTY;
28850 }
28851
28852 put_inode(rip);
28853 return(r);
28854 }
28856 /*===========================================================================*
28857 * do_stime *
28858 *===========================================================================*/
28859 PUBLIC int do_stime()
28860 {
28861 /* Perform the stime(tp) system call. */
28862   boottime = (long) m_in.pm_stime;
28863   return(OK);
28864 }