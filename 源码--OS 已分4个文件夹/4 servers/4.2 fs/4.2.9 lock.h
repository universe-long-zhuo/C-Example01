++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/lock.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
21800 /* This is the file locking table. Like the filp table, it points to the
21801 * inode table, however, in this case to achieve advisory locking.
21802 */
21803 EXTERN struct file_lock {
21804 short lock_type; /* F_RDLOCK or F_WRLOCK; 0 means unused slot */
21805 pid_t lock_pid; /* pid of the process holding the lock */
21806 struct inode *lock_inode; /* pointer to the inode locked */
21807 off_t lock_first; /* offset of first byte locked */
21808 off_t lock_last; /* offset of last byte locked */
21809 } file_lock[NR_LOCKS];