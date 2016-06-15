MINIX SOURCE CODE File: servers/fs/pipe.c 987
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/pipe.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
25900 /* This file deals with the suspension and revival of processes. A process can
25901 * be suspended because it wants to read or write from a pipe and can’t, or
25902 * because it wants to read or write from a special file and can’t. When a
25903 * process can’t continue it is suspended, and revived later when it is able
25904 * to continue.
25905 *
25906 * The entry points into this file are
25907 * do_pipe: perform the PIPE system call
25908 * pipe_check: check to see that a read or write on a pipe is feasible now
25909 * suspend: suspend a process that cannot do a requested read or write
25910 * release: check to see if a suspended process can be released and do
25911 * it
25912 * revive: mark a suspended process as able to run again
25913 * do_unpause: a signal has been sent to a process; see if it suspended
25914 */
25915
25916 #include "fs.h"
25917 #include <fcntl.h>
25918 #include <signal.h>
25919 #include <minix/callnr.h>
25920 #include <minix/com.h>
25921 #include <sys/select.h>
25922 #include <sys/time.h>
25923 #include "file.h"
25924 #include "fproc.h"
25925 #include "inode.h"
25926 #include "param.h"
25927 #include "super.h"
25928 #include "select.h"
25929
25930 /*===========================================================================*
25931 * do_pipe *
25932 *===========================================================================*/
25933 PUBLIC int do_pipe()
25934 {
25935 /* Perform the pipe(fil_des) system call. */
25936
25937 register struct fproc *rfp;
25938 register struct inode *rip;
25939 int r;
25940 struct filp *fil_ptr0, *fil_ptr1;
25941 int fil_des[2]; /* reply goes here */
25942
25943 /* Acquire two file descriptors. */
25944 rfp = fp;
25945 if ( (r = get_fd(0, R_BIT, &fil_des[0], &fil_ptr0)) != OK) return(r);
25946 rfp->fp_filp[fil_des[0]] = fil_ptr0;
25947 fil_ptr0->filp_count = 1;
25948 if ( (r = get_fd(0, W_BIT, &fil_des[1], &fil_ptr1)) != OK) {
25949 rfp->fp_filp[fil_des[0]] = NIL_FILP;
25950 fil_ptr0->filp_count = 0;
25951 return(r);
25952 }
25953 rfp->fp_filp[fil_des[1]] = fil_ptr1;
25954 fil_ptr1->filp_count = 1;
988 File: servers/fs/pipe.c MINIX SOURCE CODE
25955
25956 /* Make the inode on the pipe device. */
25957 if ( (rip = alloc_inode(root_dev, I_REGULAR) ) == NIL_INODE) {
25958 rfp->fp_filp[fil_des[0]] = NIL_FILP;
25959 fil_ptr0->filp_count = 0;
25960 rfp->fp_filp[fil_des[1]] = NIL_FILP;
25961 fil_ptr1->filp_count = 0;
25962 return(err_code);
25963 }
25964
25965 if (read_only(rip) != OK)
25966 panic(__FILE__,"pipe device is read only", NO_NUM);
25967
25968 rip->i_pipe = I_PIPE;
25969 rip->i_mode &= ˜I_REGULAR;
25970 rip->i_mode |= I_NAMED_PIPE; /* pipes and FIFOs have this bit set */
25971 fil_ptr0->filp_ino = rip;
25972 fil_ptr0->filp_flags = O_RDONLY;
25973 dup_inode(rip); /* for double usage */
25974 fil_ptr1->filp_ino = rip;
25975 fil_ptr1->filp_flags = O_WRONLY;
25976 rw_inode(rip, WRITING); /* mark inode as allocated */
25977 m_out.reply_i1 = fil_des[0];
25978 m_out.reply_i2 = fil_des[1];
25979 rip->i_update = ATIME | CTIME | MTIME;
25980 return(OK);
25981 }
25983 /*===========================================================================*
25984 * pipe_check *
25985 *===========================================================================*/
25986 PUBLIC int pipe_check(rip, rw_flag, oflags, bytes, position, canwrite, notouch)
25987 register struct inode *rip; /* the inode of the pipe */
25988 int rw_flag; /* READING or WRITING */
25989 int oflags; /* flags set by open or fcntl */
25990 register int bytes; /* bytes to be read or written (all chunks) */
25991 register off_t position; /* current file position */
25992 int *canwrite; /* return: number of bytes we can write */
25993 int notouch; /* check only */
25994 {
25995 /* Pipes are a little different. If a process reads from an empty pipe for
25996 * which a writer still exists, suspend the reader. If the pipe is empty
25997 * and there is no writer, return 0 bytes. If a process is writing to a
25998 * pipe and no one is reading from it, give a broken pipe error.
25999 */
26000
26001 /* If reading, check for empty pipe. */
26002 if (rw_flag == READING) {
26003 if (position >= rip->i_size) {
26004 /* Process is reading from an empty pipe. */
26005 int r = 0;
26006 if (find_filp(rip, W_BIT) != NIL_FILP) {
26007 /* Writer exists */
26008 if (oflags & O_NONBLOCK) {
26009 r = EAGAIN;
26010 } else {
26011 if (!notouch)
26012 suspend(XPIPE); /* block reader */
26013 r = SUSPEND;
26014 }
MINIX SOURCE CODE File: servers/fs/pipe.c 989
26015 /* If need be, activate sleeping writers. */
26016 if (susp_count > 0 && !notouch)
26017 release(rip, WRITE, susp_count);
26018 }
26019 return(r);
26020 }
26021 } else {
26022 /* Process is writing to a pipe. */
26023 if (find_filp(rip, R_BIT) == NIL_FILP) {
26024 /* Tell kernel to generate a SIGPIPE signal. */
26025 if (!notouch)
26026 sys_kill((int)(fp - fproc), SIGPIPE);
26027 return(EPIPE);
26028 }
26029
26030 if (position + bytes > PIPE_SIZE(rip->i_sp->s_block_size)) {
26031 if ((oflags & O_NONBLOCK)
26032 && bytes < PIPE_SIZE(rip->i_sp->s_block_size))
26033 return(EAGAIN);
26034 else if ((oflags & O_NONBLOCK)
26035 && bytes > PIPE_SIZE(rip->i_sp->s_block_size)) {
26036 if ( (*canwrite = (PIPE_SIZE(rip->i_sp->s_block_size)
26037 - position)) > 0) {
26038 /* Do a partial write. Need to wakeup reader */
26039 if (!notouch)
26040 release(rip, READ, susp_count);
26041 return(1);
26042 } else {
26043 return(EAGAIN);
26044 }
26045 }
26046 if (bytes > PIPE_SIZE(rip->i_sp->s_block_size)) {
26047 if ((*canwrite = PIPE_SIZE(rip->i_sp->s_block_size)
26048 - position) > 0) {
26049 /* Do a partial write. Need to wakeup reader
26050 * since we’ll suspend ourself in read_write()
26051 */
26052 release(rip, READ, susp_count);
26053 return(1);
26054 }
26055 }
26056 if (!notouch)
26057 suspend(XPIPE); /* stop writer -- pipe full */
26058 return(SUSPEND);
26059 }
26060
26061 /* Writing to an empty pipe. Search for suspended reader. */
26062 if (position == 0 && !notouch)
26063 release(rip, READ, susp_count);
26064 }
26065
26066 *canwrite = 0;
26067 return(1);
26068 }
26070 /*===========================================================================*
26071 * suspend *
26072 *===========================================================================*/
26073 PUBLIC void suspend(task)
26074 int task; /* who is proc waiting for? (PIPE = pipe) */
990 File: servers/fs/pipe.c MINIX SOURCE CODE
26075 {
26076 /* Take measures to suspend the processing of the present system call.
26077 * Store the parameters to be used upon resuming in the process table.
26078 * (Actually they are not used when a process is waiting for an I/O device,
26079 * but they are needed for pipes, and it is not worth making the distinction.)
26080 * The SUSPEND pseudo error should be returned after calling suspend().
26081 */
26082
26083 if (task == XPIPE || task == XPOPEN) susp_count++;/* #procs susp’ed on pipe*/
26084 fp->fp_suspended = SUSPENDED;
26085 fp->fp_fd = m_in.fd << 8 | call_nr;
26086 fp->fp_task = -task;
26087 if (task == XLOCK) {
26088 fp->fp_buffer = (char *) m_in.name1; /* third arg to fcntl() */
26089 fp->fp_nbytes = m_in.request; /* second arg to fcntl() */
26090 } else {
26091 fp->fp_buffer = m_in.buffer; /* for reads and writes */
26092 fp->fp_nbytes = m_in.nbytes;
26093 }
26094 }
26096 /*===========================================================================*
26097 * release *
26098 *===========================================================================*/
26099 PUBLIC void release(ip, call_nr, count)
26100 register struct inode *ip; /* inode of pipe */
26101 int call_nr; /* READ, WRITE, OPEN or CREAT */
26102 int count; /* max number of processes to release */
26103 {
26104 /* Check to see if any process is hanging on the pipe whose inode is in ’ip’.
26105 * If one is, and it was trying to perform the call indicated by ’call_nr’,
26106 * release it.
26107 */
26108
26109 register struct fproc *rp;
26110 struct filp *f;
26111
26112 /* Trying to perform the call also includes SELECTing on it with that
26113 * operation.
26114 */
26115 if (call_nr == READ || call_nr == WRITE) {
26116 int op;
26117 if (call_nr == READ)
26118 op = SEL_RD;
26119 else
26120 op = SEL_WR;
26121 for(f = &filp[0]; f < &filp[NR_FILPS]; f++) {
26122 if (f->filp_count < 1 || !(f->filp_pipe_select_ops & op) ||
26123 f->filp_ino != ip)
26124 continue;
26125 select_callback(f, op);
26126 f->filp_pipe_select_ops &= ˜op;
26127 }
26128 }
26129
26130 /* Search the proc table. */
26131 for (rp = &fproc[0]; rp < &fproc[NR_PROCS]; rp++) {
26132 if (rp->fp_suspended == SUSPENDED &&
26133 rp->fp_revived == NOT_REVIVING &&
26134 (rp->fp_fd & BYTE) == call_nr &&
MINIX SOURCE CODE File: servers/fs/pipe.c 991
26135 rp->fp_filp[rp->fp_fd>>8]->filp_ino == ip) {
26136 revive((int)(rp - fproc), 0);
26137 susp_count--; /* keep track of who is suspended */
26138 if (--count == 0) return;
26139 }
26140 }
26141 }
26143 /*===========================================================================*
26144 * revive *
26145 *===========================================================================*/
26146 PUBLIC void revive(proc_nr, returned)
26147 int proc_nr; /* process to revive */
26148 int returned; /* if hanging on task, how many bytes read */
26149 {
26150 /* Revive a previously blocked process. When a process hangs on tty, this
26151 * is the way it is eventually released.
26152 */
26153
26154 register struct fproc *rfp;
26155 register int task;
26156
26157 if (proc_nr < 0 || proc_nr >= NR_PROCS)
26158 panic(__FILE__,"revive err", proc_nr);
26159 rfp = &fproc[proc_nr];
26160 if (rfp->fp_suspended == NOT_SUSPENDED || rfp->fp_revived == REVIVING)return;
26161
26162 /* The ’reviving’ flag only applies to pipes. Processes waiting for TTY get
26163 * a message right away. The revival process is different for TTY and pipes.
26164 * For select and TTY revival, the work is already done, for pipes it is not:
26165 * the proc must be restarted so it can try again.
26166 */
26167 task = -rfp->fp_task;
26168 if (task == XPIPE || task == XLOCK) {
26169 /* Revive a process suspended on a pipe or lock. */
26170 rfp->fp_revived = REVIVING;
26171 reviving++; /* process was waiting on pipe or lock */
26172 } else {
26173 rfp->fp_suspended = NOT_SUSPENDED;
26174 if (task == XPOPEN) /* process blocked in open or create */
26175 reply(proc_nr, rfp->fp_fd>>8);
26176 else if (task == XSELECT) {
26177 reply(proc_nr, returned);
26178 } else {
26179 /* Revive a process suspended on TTY or other device. */
26180 rfp->fp_nbytes = returned; /*pretend it wants only what there is*/
26181 reply(proc_nr, returned); /* unblock the process */
26182 }
26183 }
26184 }
26186 /*===========================================================================*
26187 * do_unpause *
26188 *===========================================================================*/
26189 PUBLIC int do_unpause()
26190 {
26191 /* A signal has been sent to a user who is paused on the file system.
26192 * Abort the system call with the EINTR error message.
26193 */
26194
992 File: servers/fs/pipe.c MINIX SOURCE CODE
26195 register struct fproc *rfp;
26196 int proc_nr, task, fild;
26197 struct filp *f;
26198 dev_t dev;
26199 message mess;
26200
26201 if (who > PM_PROC_NR) return(EPERM);
26202 proc_nr = m_in.pro;
26203 if (proc_nr < 0 || proc_nr >= NR_PROCS)
26204 panic(__FILE__,"unpause err 1", proc_nr);
26205 rfp = &fproc[proc_nr];
26206 if (rfp->fp_suspended == NOT_SUSPENDED) return(OK);
26207 task = -rfp->fp_task;
26208
26209 switch (task) {
26210 case XPIPE: /* process trying to read or write a pipe */
26211 break;
26212
26213 case XLOCK: /* process trying to set a lock with FCNTL */
26214 break;
26215
26216 case XSELECT: /* process blocking on select() */
26217 select_forget(proc_nr);
26218 break;
26219
26220 case XPOPEN: /* process trying to open a fifo */
26221 break;
26222
26223 default: /* process trying to do device I/O (e.g. tty)*/
26224 fild = (rfp->fp_fd >> 8) & BYTE;/* extract file descriptor */
26225 if (fild < 0 || fild >= OPEN_MAX)
26226 panic(__FILE__,"unpause err 2",NO_NUM);
26227 f = rfp->fp_filp[fild];
26228 dev = (dev_t) f->filp_ino->i_zone[0]; /* device hung on */
26229 mess.TTY_LINE = (dev >> MINOR) & BYTE;
26230 mess.PROC_NR = proc_nr;
26231
26232 /* Tell kernel R or W. Mode is from current call, not open. */
26233 mess.COUNT = (rfp->fp_fd & BYTE) == READ ? R_BIT : W_BIT;
26234 mess.m_type = CANCEL;
26235 fp = rfp; /* hack - ctty_io uses fp */
26236 (*dmap[(dev >> MAJOR) & BYTE].dmap_io)(task, &mess);
26237 }
26238
26239 rfp->fp_suspended = NOT_SUSPENDED;
26240 reply(proc_nr, EINTR); /* signal interrupted call */
26241 return(OK);
26242 }
26244 /*===========================================================================*
26245 * select_request_pipe *
26246 *===========================================================================*/
26247 PUBLIC int select_request_pipe(struct filp *f, int *ops, int block)
26248 {
26249 int orig_ops, r = 0, err, canwrite;
26250 orig_ops = *ops;
26251 if ((*ops & SEL_RD)) {
26252 if ((err = pipe_check(f->filp_ino, READING, 0,
26253 1, f->filp_pos, &canwrite, 1)) != SUSPEND)
26254 r |= SEL_RD;
MINIX SOURCE CODE File: servers/fs/pipe.c 993
26255 if (err < 0 && err != SUSPEND && (*ops & SEL_ERR))
26256 r |= SEL_ERR;
26257 }
26258 if ((*ops & SEL_WR)) {
26259 if ((err = pipe_check(f->filp_ino, WRITING, 0,
26260 1, f->filp_pos, &canwrite, 1)) != SUSPEND)
26261 r |= SEL_WR;
26262 if (err < 0 && err != SUSPEND && (*ops & SEL_ERR))
26263 r |= SEL_ERR;
26264 }
26265
26266 *ops = r;
26267
26268 if (!r && block) {
26269 f->filp_pipe_select_ops |= orig_ops;
26270 }
26271
26272 return SEL_OK;
26273 }
26275 /*===========================================================================*
26276 * select_match_pipe *
26277 *===========================================================================*/
26278 PUBLIC int select_match_pipe(struct filp *f)
26279 {
26280 /* recognize either pipe or named pipe (FIFO) */
26281 if (f && f->filp_ino && (f->filp_ino->i_mode & I_NAMED_PIPE))
26282 return 1;
26283 return 0;
26284 }