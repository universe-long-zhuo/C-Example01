++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             drivers/libdriver/driver.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* This file contains device independent device driver interface.
*
* Changes:
* Jul 25, 2005 added SYS_SIG type for signals (Jorrit N. Herder)
* Sep 15, 2004 added SYN_ALARM type for timeouts (Jorrit N. Herder)  alarm 报警；alert 警报。
* Jul 23, 2004 removed kernel dependencies (Jorrit N. Herder)
* Apr 02, 1992 constructed from AT wini and floppy driver (Kees J. Bot)  软盘floppy
*
*
* The drivers support the following operations (using message format m2):
*
* m_type DEVICE PROC_NR COUNT POSITION ADRRESS
* ----------------------------------------------------------------
* | DEV_OPEN   | device  | proc nr |         |         |         |
* |------------+---------+---------+---------+---------+---------|
* | DEV_CLOSE  | device  | proc nr |         |         |         |
* |------------+---------+---------+---------+---------+---------|
* | DEV_READ   | device  | proc nr | bytes   | offset  | buf ptr |
* |------------+---------+---------+---------+---------+---------|
* | DEV_WRITE  | device  | proc nr | bytes   | offset  | buf ptr |
* |------------+---------+---------+---------+---------+---------|
* | DEV_GATHER | device  | proc nr | iov len | offset  | iov ptr |
* |------------+---------+---------+---------+---------+---------|
* | DEV_SCATTER| device  | proc nr | iov len | offset  | iov ptr |
* |------------+---------+---------+---------+---------+---------|
* | DEV_IOCTL  | device  | proc nr |func code|         | buf ptr |
* |------------+---------+---------+---------+---------+---------|
* | CANCEL     | device  | proc nr | r/w     |         |         |
* |------------+---------+---------+---------+---------+---------|
* | HARD_STOP  |         |         |         |         |         |
* ----------------------------------------------------------------
*
* The file contains one entry point:
*
* driver_task: called by the device dependent task entry
*/

#include "../drivers.h"
#include <sys/ioc_disk.h>
#include "driver.h"

#define BUF_EXTRA 0

/* Claim space for variables. */
PRIVATE u8_t buffer[(unsigned) 2 * DMA_BUF_SIZE + BUF_EXTRA];
u8_t *tmp_buf;       /* the DMA buffer eventually */
phys_bytes tmp_phys; /* phys address of DMA buffer */

FORWARD _PROTOTYPE( void init_buffer, (void) );
FORWARD _PROTOTYPE( int do_rdwt, (struct driver *dr, message *mp) );
FORWARD _PROTOTYPE( int do_vrdwt, (struct driver *dr, message *mp) );

int device_caller;

// Minix驱动可以响应的消息有有六大类： 
// 1. OPEN         打开设备； 
// 2. CLOSE        关闭设备； 
// 3. READ         对设备读操作； 
// 4. WRITE        对设备写操作； 
// 5. IOCTL        用于I/O控制； 
// 6. SCATERED_IO  预读消息。

// driver_task 驱动程序_任务
// init_buffer   初始化_缓冲区
// do_rdwt           做_读驱动程序等待
// do_vrdwt          做_向量驱动程序等待
// no_name         没有_姓名
// do_nop            做_没有指针
// nop_signal  没有指针_信号
// nop_alarm   没有指针_警告
// nop_prepare 没有指针_准备
// nop_cleanup 没有指针_清理
// nop_cancel  没有指针_取消
// nop_select  没有指针_选择
// do_diocntl        做_驱动程序输入输出控制

// 参数& 取地址符号
// & ：取地址符
// &a：变量a的地址 

/*===========================================================================*
*                       driver_task 驱动程序_任务 *
*===========================================================================*/
// 参数dp是不是device points的缩写？不是，终于明白了，如果有*，就写后缀p
PUBLIC void driver_task(dp)
struct driver *dp; /* Device dependent entry points. */
{
/* Main program of any device driver task. */

    int r, proc_nr;
    message mess;

    /* Get a DMA buffer. */
    init_buffer();

    /* Here is the main loop of the disk task. It waits for a message, carries
    * it out, and sends a reply.
    */
    while (TRUE) {

        /* Wait for a request to read or write a disk block. */
        if(receive(ANY, &mess) != OK) continue;

        device_caller = mess.m_source;
        proc_nr = mess.PROC_NR;

        /* Now carry out the work. */
        switch(mess.m_type) {
            case DEV_OPEN: r = (*dp->dr_open)(dp, &mess); break;
            case DEV_CLOSE: r = (*dp->dr_close)(dp, &mess); break;
            case DEV_IOCTL: r = (*dp->dr_ioctl)(dp, &mess); break;
            case CANCEL: r = (*dp->dr_cancel)(dp, &mess);break;
            case DEV_SELECT: r = (*dp->dr_select)(dp, &mess);break;

            case DEV_READ:
            case DEV_WRITE: r = do_rdwt(dp, &mess); break;
            case DEV_GATHER:
            case DEV_SCATTER: r = do_vrdwt(dp, &mess); break;

            case HARD_INT: /* leftover interrupt or expired timer. */
                if(dp->dr_hw_int) {
                    (*dp->dr_hw_int)(dp, &mess);
                }
                continue;
            case SYS_SIG: (*dp->dr_signal)(dp, &mess);
                continue; /* don’t reply */
            case SYN_ALARM: (*dp->dr_alarm)(dp, &mess);
                continue; /* don’t reply */
            default:
                if(dp->dr_other)
                    r = (*dp->dr_other)(dp, &mess);
                else
                    r = EINVAL;
                break;
        }

        /* Clean up leftover state. */
        (*dp->dr_cleanup)();

        /* Finally, prepare and send the reply message. */
        if (r != EDONTREPLY) {
            mess.m_type = TASK_REPLY;
            mess.REP_PROC_NR = proc_nr;
            /* Status is # of bytes transferred or error code. */
            mess.REP_STATUS = r;
            send(device_caller, &mess);
        }
    }
}
/*===========================================================================*
*                       init_buffer 初始化_缓冲区 *
*===========================================================================*/
PRIVATE void init_buffer()
{
/* Select a buffer that can safely be used for DMA transfers. It may also
* be used to read partition tables and such. Its absolute address is
* ’tmp_phys’, the normal address is ’tmp_buf’.
*/

    unsigned left;  /* 无符号 */

    tmp_buf = buffer;
    sys_umap(SELF, D, (vir_bytes)buffer, (phys_bytes)sizeof(buffer), &tmp_phys);

    if ((left = dma_bytes_left(tmp_phys)) < DMA_BUF_SIZE) {
        /* First half of buffer crosses a 64K boundary, can’t DMA into that */
        tmp_buf += left;
        tmp_phys += left;
    }
}
/*===========================================================================*
*                       do_rdwt 做_读驱动程序等待 *
*===========================================================================*/
PRIVATE int do_rdwt(dp, mp)
struct driver *dp; /* device dependent entry points */
message *mp;       /* pointer to read or write message */
{
/* Carry out a single read or write request. */
    iovec_t iovec1;
    int r, opcode;
    phys_bytes phys_addr;

    /* Disk address? Address and length of the user buffer? */
    if (mp->COUNT < 0) return(EINVAL);

    /* Check the user buffer. */
    sys_umap(mp->PROC_NR, D, (vir_bytes) mp->ADDRESS, mp->COUNT, &phys_addr);
    if (phys_addr == 0) return(EFAULT);

    /* Prepare for I/O. */
    if ((*dp->dr_prepare)(mp->DEVICE) == NIL_DEV) return(ENXIO);

    /* Create a one element scatter/gather vector for the buffer. */
    opcode = mp->m_type == DEV_READ ? DEV_GATHER : DEV_SCATTER;
    iovec1.iov_addr = (vir_bytes) mp->ADDRESS;
    iovec1.iov_size = mp->COUNT;

    /* Transfer bytes from/to the device. */
    r = (*dp->dr_transfer)(mp->PROC_NR, opcode, mp->POSITION, &iovec1, 1);

    /* Return the number of bytes transferred or an error code. */
    return(r == OK ? (mp->COUNT - iovec1.iov_size) : r);
}
/*==========================================================================*
*                       do_vrdwt 做_向量驱动程序等待 *
*==========================================================================*/
PRIVATE int do_vrdwt(dp, mp)
struct driver *dp; /* device dependent entry points */
message *mp; /* pointer to read or write message */
{
/* Carry out an device read or write to/from a vector of user addresses.
* The "user addresses" are assumed to be safe, i.e. FS transferring to/from
* its own buffers, so they are not checked.
*/
    static iovec_t iovec[NR_IOREQS];
    iovec_t *iov;
    phys_bytes iovec_size;
    unsigned nr_req;
    int r;

    nr_req = mp->COUNT; /* Length of I/O vector */

    if (mp->m_source < 0) { /* 用法: 指针->成员名字 */
        /* Called by a task, no need to copy vector. */
        iov = (iovec_t *) mp->ADDRESS;
    } else {
        /* Copy the vector from the caller to kernel space. */
        if (nr_req > NR_IOREQS) nr_req = NR_IOREQS;
        iovec_size = (phys_bytes) (nr_req * sizeof(iovec[0]));

        if (OK != sys_datacopy(mp->m_source, (vir_bytes) mp->ADDRESS,
        SELF, (vir_bytes) iovec, iovec_size))
            panic((*dp->dr_name)(),"bad I/O vector by", mp->m_source);
        iov = iovec;
    }

    /* Prepare for I/O. */
    if ((*dp->dr_prepare)(mp->DEVICE) == NIL_DEV) return(ENXIO);

    /* Transfer bytes from/to the device. */
    r = (*dp->dr_transfer)(mp->PROC_NR, mp->m_type, mp->POSITION, iov, nr_req);

    /* Copy the I/O vector向量 back to the caller. */
    if (mp->m_source >= 0) {
        sys_datacopy(SELF, (vir_bytes) iovec,
        mp->m_source, (vir_bytes) mp->ADDRESS, iovec_size);
    }
    return(r);
}
/*===========================================================================*
*                       no_name 没有_姓名 *
*===========================================================================*/
PUBLIC char *no_name()
{
/* Use this default name if there is no specific name for the device. This was
* originally done by fetching the name from the task table for this process:
* "return(tasktab[proc_number(proc_ptr) + NR_TASKS].name);", but currently a
* real "noname" is returned. Perhaps, some system information service can be
* queried for a name at a later time.
*/
    static char name[] = "noname";
    return name;
}
/*============================================================================*
*                       do_nop 做_没有指针 *
*============================================================================*/
PUBLIC int do_nop(dp, mp)
struct driver *dp;
message *mp;
{
/* Nothing there, or nothing to do. */

    switch (mp->m_type) {
        case DEV_OPEN: return(ENODEV);
        case DEV_CLOSE: return(OK);
        case DEV_IOCTL: return(ENOTTY);
        default: return(EIO);
    }
}
/*============================================================================*
*                       nop_signal 没有指针_信号 *
*============================================================================*/
PUBLIC void nop_signal(dp, mp)
struct driver *dp;
message *mp;
{
    /* Default action for signal is to ignore. */
}
/*============================================================================*
*                       nop_alarm 没有指针_警告 *
*============================================================================*/
PUBLIC void nop_alarm(dp, mp)
struct driver *dp;
message *mp;
{
    /* Ignore忽视 the leftover剩余 alarm. */
}
/*===========================================================================*
*                       nop_prepare 没有指针_准备 *
*===========================================================================*/
PUBLIC struct device *nop_prepare(device)
{
/* Nothing to prepare for. */
    return(NIL_DEV);
}
/*===========================================================================*
*                       nop_cleanup 没有指针_清理 *
*===========================================================================*/
PUBLIC void nop_cleanup()
{
    /* Nothing to clean up. */
}
/*===========================================================================*
*                       nop_cancel 没有指针_取消 *
*===========================================================================*/
PUBLIC int nop_cancel(struct driver *dr, message *m)
{
/* Nothing to do for cancel. */
    return(OK);
}
/*===========================================================================*
*                       nop_select 没有指针_选择 *
*===========================================================================*/
PUBLIC int nop_select(struct driver *dr, message *m)
{
/* Nothing to do for select. */
    return(OK);
}
/*============================================================================*
*                       do_diocntl 做_驱动程序输入输出控制 *
*============================================================================*/
PUBLIC int do_diocntl(dp, mp)
struct driver *dp;
message *mp; /* pointer to ioctl request */
{
/* Carry out a partition setting/getting request. 携带，carry-out完成*/
    struct device *dv;
    struct partition entry;
    int s;

    if (mp->REQUEST != DIOCSETP && mp->REQUEST != DIOCGETP) {
        if(dp->dr_other) {
            return dp->dr_other(dp, mp);
        } else return(ENOTTY);
    }

    /* Decode the message parameters. */
    if ((dv = (*dp->dr_prepare)(mp->DEVICE)) == NIL_DEV) return(ENXIO);

    if (mp->REQUEST == DIOCSETP) {
        /* Copy just this one partition table entry. */
        if (OK != (s=sys_datacopy(mp->PROC_NR, (vir_bytes) mp->ADDRESS,
        SELF, (vir_bytes) &entry, sizeof(entry))))
            return s;
        dv->dv_base = entry.base;
        dv->dv_size = entry.size;
    } else {
        /* Return a partition table entry and the geometry of the drive. geometry几何*/
        entry.base = dv->dv_base;
        entry.size = dv->dv_size;
        (*dp->dr_geometry)(&entry);
        if (OK != (s=sys_datacopy(SELF, (vir_bytes) &entry,
        mp->PROC_NR, (vir_bytes) mp->ADDRESS, sizeof(entry))))
            return s;
    }
    return(OK);
}