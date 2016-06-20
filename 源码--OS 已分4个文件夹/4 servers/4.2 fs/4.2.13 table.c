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

no_sys,        /* 0 = unused   未使用；不用的；空着的 */
do_exit,       /* 1 = exit     退出 */
do_fork,       /* 2 = fork     叉 */
do_read,       /* 3 = read     读 */
do_write,      /* 4 = write    写 */
do_open,       /* 5 = open     打开 */
do_close,      /* 6 = close    关闭 */
no_sys,        /* 7 = wait     等待 */
do_creat,      /* 8 = creat    创建 */
do_link,       /* 9 = link     链接 */
do_unlink,     /* 10 = unlink  删除链接；取消链接；删除档案 */
no_sys,        /* 11 = waitpid 子进程；回收僵尸进程 */
do_chdir,      /* 12 = chdir   改变当前目录；改变目录；改变工作目录 */
no_sys,        /* 13 = time    时间 */
do_mknod,      /* 14 = mknod   创建索引节点；创建设备节点；创建任何类型文件 */
do_chmod,      /* 15 = chmod   权限；改变权限；改变文件的权限 */
do_chown,      /* 16 = chown   改变文件的所有者；改变文件的拥有者；改变文件所有权 */
no_sys,        /* 17 = break   中断；休息； */
do_stat,       /* 18 = stat */
do_lseek,      /* 19 = lseek   移动文件指针；移动文件指针函数；前后移动 */
no_sys,        /* 20 = getpid  获取进程标识号；读取进程号；打印程序编号 */
do_mount,      /* 21 = mount   挂载；安装；挂接 */
do_umount,     /* 22 = umount  卸载；卸载文件系统；拆卸文件系统 */
do_set,        /* 23 = setuid  设置用户标识符；设置用户标志号；用户提交过程 */
no_sys,        /* 24 = getuid  获取用户标识号；用户个人资料 */
do_stime,      /* 25 = stime   开始时间；生态美；系统时间 */
no_sys,        /* 26 = ptrace  进程跟踪；位；存储当前堆栈环境 */
no_sys,        /* 27 = alarm   报警；闹钟；报警器 */
do_fstat,      /* 28 = fstat   改变文件大小；f分布函数的统计量；文件状态访问 */
no_sys,        /* 29 = pause   暂停；停顿；暂停放像 */
do_utime,      /* 30 = utime   用户时间 */
no_sys,        /* 31 = (stty)  设置终端的选项；设置终端模式； */
no_sys,        /* 32 = (gtty)  获取终端tty */
do_access,     /* 33 = access  存取 */
no_sys,        /* 34 = (nice)  这是什么？ */
no_sys,        /* 35 = (ftime) 文件时间；文件日期； */
do_sync,       /* 36 = sync    同步；同步信号； */
no_sys,        /* 37 = kill    杀死 */
do_rename,     /* 38 = rename  重新命名；给…改名 */
do_mkdir,      /* 39 = mkdir   创建目录；建立目录；新建目录 */
do_unlink,     /* 40 = rmdir   删除目录 */
do_dup,        /* 41 = dup     数据用户部分(Data User Part)；是吗？ */
do_pipe,       /* 42 = pipe    管道 */
no_sys,        /* 43 = times   不是时间吗？“time”的第三人称单数 */
no_sys,        /* 44 = (prof)  这是什么？ */
no_sys,        /* 45 = unused  未使用；不用的；空着的 */
do_set,        /* 46 = setgid  设置组标记号 */
no_sys,        /* 47 = getgid  获取组标识号 */
no_sys,        /* 48 = (signal)信号 */
no_sys,        /* 49 = unused */
no_sys,        /* 50 = unused */
no_sys,        /* 51 = (acct)  这是什么？ */
no_sys,        /* 52 = (phys)  物理？ */
no_sys,        /* 53 = (lock)  锁 */
do_ioctl,      /* 54 = ioctl   不是I/O控制吗？ */
do_fcntl,      /* 55 = fcntl   文件控制 */
no_sys,        /* 56 = (mpx)   Multi-Pointer X; 多路传输 是吗？ */
no_sys,        /* 57 = unused */
no_sys,        /* 58 = unused */
do_exec,       /* 59 = execve  系统调用；执行文件；运行可执行文件 */
do_umask,      /* 60 = umask   掩码；权限；设置文件权限掩码 */
do_chroot,     /* 61 = chroot  改变根目录；根系统调用；关闭目录 */
do_setsid,     /* 62 = setsid  这是什么？ */
no_sys,        /* 63 = getpgrp 取得进程识别码 */
no_sys,        /* 64 = KSIG: signals originating in the kernel 原产于内核信号 */
do_unpause,    /* 65 = UNPAUSE     取消暂停 不是吗？ */
no_sys,        /* 66 = unused */
do_revive,     /* 67 = REVIVE      复活;重新使用 不是尸体复活吗？哈哈 */
no_sys,        /* 68 = TASK_REPLY  任务_答复 */
no_sys,        /* 69 = unused */
no_sys,        /* 70 = unused */
no_sys,        /* 71 = si */
no_sys,        /* 72 = sigsuspend  信号等待 */
no_sys,        /* 73 = sigpending  进程有信号要处理；未决信号；阻塞信号查询 */
no_sys,        /* 74 = sigprocmask 暂停直到信号到来；代替；信号阻塞控制 */
no_sys,        /* 75 = sigreturn   系统调用 */
do_reboot,     /* 76 = reboot      重新启动 */
do_svrctl,     /* 77 = svrctl      控制表 */
no_sys,        /* 78 = unused */
do_getsysinfo, /* 79 = getsysinfo  获取系统信息 */
no_sys,        /* 80 = unused */
do_devctl,     /* 81 = devctl      我觉得好像这是device（设备）控制 */
do_fstatfs,    /* 82 = fstatfs     这是什么？ */
no_sys,        /* 83 = memalloc    这是什么？ */
no_sys,        /* 84 = memfree     内存整理工具；可用内存；释放记忆体 */
do_select,     /* 85 = select      选择 */
do_fchdir,     /* 86 = fchdir      改变文件的权限；改变当前工作目录 */
do_fsync,      /* 87 = fsync       文件同步 */
no_sys,        /* 88 = getpriority 获取调度优先级；资源和限制；得到优先级 */
no_sys,        /* 89 = setpriority 优先级；优先级最低；设置优先级 */
no_sys,        /* 90 = gettimeofday 取得目前时间；取时间和时区；取得当前时间 */
};
/* This should not fail with "array size is negative": */
extern int dummy[sizeof(call_vec) == NCALLS * sizeof(call_vec[0]) ? 1 : -1];
