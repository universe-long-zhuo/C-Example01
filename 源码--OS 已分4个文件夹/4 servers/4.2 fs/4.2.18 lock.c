++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/lock.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
23800 /* This file handles advisory file locking as required by POSIX.
23801 *
23802 * The entry points into this file are
23803 * lock_op: perform locking operations for FCNTL system call
23804 * lock_revive: revive processes when a lock is released
23805 */
23806
23807 #include "fs.h"
23808 #include <minix/com.h>
23809 #include <fcntl.h>
23810 #include <unistd.h>
23811 #include "file.h"
23812 #include "fproc.h"
23813 #include "inode.h"
23814 #include "lock.h"
23815 #include "param.h"
23816
23817 /*===========================================================================*
23818 * lock_op *
23819 *===========================================================================*/
23820 PUBLIC int lock_op(f, req)
23821 struct filp *f;
23822 int req; /* either F_SETLK or F_SETLKW */
23823 {
23824 /* Perform the advisory locking required by POSIX. */
23825
23826 int r, ltype, i, conflict = 0, unlocking = 0;
23827 mode_t mo;
23828 off_t first, last;
23829 struct flock flock;
23830 vir_bytes user_flock;
23831 struct file_lock *flp, *flp2, *empty;
23832
23833 /* Fetch the flock structure from user space. */
23834 user_flock = (vir_bytes) m_in.name1;
23835 r = sys_datacopy(who, (vir_bytes) user_flock,
23836 FS_PROC_NR, (vir_bytes) &flock, (phys_bytes) sizeof(flock));
23837 if (r != OK) return(EINVAL);
23838
23839 /* Make some error checks. */
23840 ltype = flock.l_type;
23841 mo = f->filp_mode;
23842 if (ltype != F_UNLCK && ltype != F_RDLCK && ltype != F_WRLCK) return(EINVAL);
23843 if (req == F_GETLK && ltype == F_UNLCK) return(EINVAL);
23844 if ( (f->filp_ino->i_mode & I_TYPE) != I_REGULAR) return(EINVAL);
MINIX SOURCE CODE File: servers/fs/lock.c 955
23845 if (req != F_GETLK && ltype == F_RDLCK && (mo & R_BIT) == 0) return(EBADF);
23846 if (req != F_GETLK && ltype == F_WRLCK && (mo & W_BIT) == 0) return(EBADF);
23847
23848 /* Compute the first and last bytes in the lock region. */
23849 switch (flock.l_whence) {
23850 case SEEK_SET: first = 0; break;
23851 case SEEK_CUR: first = f->filp_pos; break;
23852 case SEEK_END: first = f->filp_ino->i_size; break;
23853 default: return(EINVAL);
23854 }
23855 /* Check for overflow. */
23856 if (((long)flock.l_start > 0) && ((first + flock.l_start) < first))
23857 return(EINVAL);
23858 if (((long)flock.l_start < 0) && ((first + flock.l_start) > first))
23859 return(EINVAL);
23860 first = first + flock.l_start;
23861 last = first + flock.l_len - 1;
23862 if (flock.l_len == 0) last = MAX_FILE_POS;
23863 if (last < first) return(EINVAL);
23864
23865 /* Check if this region conflicts with any existing lock. */
23866 empty = (struct file_lock *) 0;
23867 for (flp = &file_lock[0]; flp < & file_lock[NR_LOCKS]; flp++) {
23868 if (flp->lock_type == 0) {
23869 if (empty == (struct file_lock *) 0) empty = flp;
23870 continue; /* 0 means unused slot */
23871 }
23872 if (flp->lock_inode != f->filp_ino) continue; /* different file */
23873 if (last < flp->lock_first) continue; /* new one is in front */
23874 if (first > flp->lock_last) continue; /* new one is afterwards */
23875 if (ltype == F_RDLCK && flp->lock_type == F_RDLCK) continue;
23876 if (ltype != F_UNLCK && flp->lock_pid == fp->fp_pid) continue;
23877
23878 /* There might be a conflict. Process it. */
23879 conflict = 1;
23880 if (req == F_GETLK) break;
23881
23882 /* If we are trying to set a lock, it just failed. */
23883 if (ltype == F_RDLCK || ltype == F_WRLCK) {
23884 if (req == F_SETLK) {
23885 /* For F_SETLK, just report back failure. */
23886 return(EAGAIN);
23887 } else {
23888 /* For F_SETLKW, suspend the process. */
23889 suspend(XLOCK);
23890 return(SUSPEND);
23891 }
23892 }
23893
23894 /* We are clearing a lock and we found something that overlaps. */
23895 unlocking = 1;
23896 if (first <= flp->lock_first && last >= flp->lock_last) {
23897 flp->lock_type = 0; /* mark slot as unused */
23898 nr_locks--; /* number of locks is now 1 less */
23899 continue;
23900 }
23901
23902 /* Part of a locked region has been unlocked. */
23903 if (first <= flp->lock_first) {
23904 flp->lock_first = last + 1;
956 File: servers/fs/lock.c MINIX SOURCE CODE
23905 continue;
23906 }
23907
23908 if (last >= flp->lock_last) {
23909 flp->lock_last = first - 1;
23910 continue;
23911 }
23912
23913 /* Bad luck. A lock has been split in two by unlocking the middle. */
23914 if (nr_locks == NR_LOCKS) return(ENOLCK);
23915 for (i = 0; i < NR_LOCKS; i++)
23916 if (file_lock[i].lock_type == 0) break;
23917 flp2 = &file_lock[i];
23918 flp2->lock_type = flp->lock_type;
23919 flp2->lock_pid = flp->lock_pid;
23920 flp2->lock_inode = flp->lock_inode;
23921 flp2->lock_first = last + 1;
23922 flp2->lock_last = flp->lock_last;
23923 flp->lock_last = first - 1;
23924 nr_locks++;
23925 }
23926 if (unlocking) lock_revive();
23927
23928 if (req == F_GETLK) {
23929 if (conflict) {
23930 /* GETLK and conflict. Report on the conflicting lock. */
23931 flock.l_type = flp->lock_type;
23932 flock.l_whence = SEEK_SET;
23933 flock.l_start = flp->lock_first;
23934 flock.l_len = flp->lock_last - flp->lock_first + 1;
23935 flock.l_pid = flp->lock_pid;
23936
23937 } else {
23938 /* It is GETLK and there is no conflict. */
23939 flock.l_type = F_UNLCK;
23940 }
23941
23942 /* Copy the flock structure back to the caller. */
23943 r = sys_datacopy(FS_PROC_NR, (vir_bytes) &flock,
23944 who, (vir_bytes) user_flock, (phys_bytes) sizeof(flock));
23945 return(r);
23946 }
23947
23948 if (ltype == F_UNLCK) return(OK); /* unlocked a region with no locks */
23949
23950 /* There is no conflict. If space exists, store new lock in the table. */
23951 if (empty == (struct file_lock *) 0) return(ENOLCK); /* table full */
23952 empty->lock_type = ltype;
23953 empty->lock_pid = fp->fp_pid;
23954 empty->lock_inode = f->filp_ino;
23955 empty->lock_first = first;
23956 empty->lock_last = last;
23957 nr_locks++;
23958 return(OK);
23959 }
MINIX SOURCE CODE File: servers/fs/lock.c 957
23961 /*===========================================================================*
23962 * lock_revive *
23963 *===========================================================================*/
23964 PUBLIC void lock_revive()
23965 {
23966 /* Go find all the processes that are waiting for any kind of lock and
23967 * revive them all. The ones that are still blocked will block again when
23968 * they run. The others will complete. This strategy is a space-time
23969 * tradeoff. Figuring out exactly which ones to unblock now would take
23970 * extra code, and the only thing it would win would be some performance in
23971 * extremely rare circumstances (namely, that somebody actually used
23972 * locking).
23973 */
23974
23975 int task;
23976 struct fproc *fptr;
23977
23978 for (fptr = &fproc[INIT_PROC_NR + 1]; fptr < &fproc[NR_PROCS]; fptr++){
23979 task = -fptr->fp_task;
23980 if (fptr->fp_suspended == SUSPENDED && task == XLOCK) {
23981 revive( (int) (fptr - fproc), 0);
23982 }
23983 }
23984 }