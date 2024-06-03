/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     sys_console.c
 * @brief    CSI Source File for console
 * @version  V1.0
 * @date     2019-12-19
 ******************************************************************************/
#ifndef __TRCKERNELPORTFREERTOS_H__
#define __TRCKERNELPORTFREERTOS_H__

#include "os.h"
#include "trcHardwarePort.h"
#include "csi_kernel.h"
#include "trcKernelHooks.h"

#if (USE_TRACEALYZER_RECORDER == 1 && TRACE_CFG_EN)

#define TRACE_OS_TYPE_TASK_CREATE						        0x01
#define TRACE_OS_TYPE_TASK_CREATE_FAILED						0x02
#define TRACE_OS_TYPE_TASK_DLY						            0x03
#define TRACE_OS_TYPE_TASK_SUSPEND						        0x06
#define TRACE_OS_TYPE_TASK_RESUME						        0x07
#define TRACE_OS_TYPE_TASK_DEL						            0x09
#define TRACE_OS_TYPE_SEM_CREATE						        0x0a
#define TRACE_OS_TYPE_SEM_POST						            0x0d
#define TRACE_OS_TYPE_SEM_POST_FAILED						    0x0e
#define TRACE_OS_TYPE_SEM_PEND						            0x0f
#define TRACE_OS_TYPE_SEM_PEND_FAILED						    0x10
#define TRACE_OS_TYPE_SEM_PEND_BLOCK						    0x11
#define TRACE_OS_TYPE_SEM_DEL						            0x12
#define TRACE_OS_TYPE_MUTEX_CREATE						        0x13
#define TRACE_OS_TYPE_MUTEX_POST						        0x19
#define TRACE_OS_TYPE_MUTEX_POST_FAILED						    0x1a
#define TRACE_OS_TYPE_MUTEX_PEND						        0x1c
#define TRACE_OS_TYPE_MUTEX_PEND_FAILED						    0x1d
#define TRACE_OS_TYPE_MUTEX_PEND_BLOCK						    0x1e
#define TRACE_OS_TYPE_MUTEX_DEL						            0x20
#define TRACE_OS_TYPE_Q_CREATE						            0x21
#define TRACE_OS_TYPE_Q_POST						            0x24
#define TRACE_OS_TYPE_Q_POST_FAILED						        0x25
#define TRACE_OS_TYPE_Q_PEND_BLOCK			        			0x28
#define TRACE_OS_TYPE_Q_PEND					            	0x2a
#define TRACE_OS_TYPE_Q_PEND_FAILED						        0x2b
#define TRACE_OS_TYPE_Q_DEL						                0x32



#define  TRACE_OS_TICK_INCREMENT(OSTickCtr) \
          { trcKERNEL_HOOKS_INCREMENT_TICK(); }

#define  TRACE_OS_TASK_CREATE(p_tcb)                        \
        trace_recorder_event(TRACE_OS_TYPE_TASK_CREATE, p_tcb->NamePtr)

#define  TRACE_OS_TASK_CREATE_FAILED(p_tcb)                 \
        trace_recorder_event(TRACE_OS_TYPE_TASK_CREATE_FAILED, p_tcb->NamePtr)

#define  TRACE_OS_TASK_DEL(p_tcb)                           \
        trace_recorder_event(TRACE_OS_TYPE_TASK_DEL, p_tcb->NamePtr)

#define  TRACE_OS_TASK_READY(p_tcb)
#define  TRACE_OS_TASK_SWITCHED_IN(p_tcb)

#define  TRACE_OS_TASK_DLY(dly_ticks)                      \
        trace_recorder_event(TRACE_OS_TYPE_TASK_DLY, NULL)

#define  TRACE_OS_TASK_SUSPEND(p_tcb)                       \
        trace_recorder_event(TRACE_OS_TYPE_TASK_SUSPEND, p_tcb->NamePtr)

#define  TRACE_OS_TASK_RESUME(p_tcb)                        \
        trace_recorder_event(TRACE_OS_TYPE_TASK_RESUME, p_tcb->NamePtr)

#define  TRACE_OS_ISR_REGISTER(isr_id, isr_name, isr_prio)
#define  TRACE_OS_ISR_BEGIN(isr_id)
#define  TRACE_OS_ISR_END()

#define  TRACE_OS_TASK_MSG_Q_CREATE(p_msg_q, p_name)
#define  TRACE_OS_TASK_MSG_Q_POST(p_msg_q)
#define  TRACE_OS_TASK_MSG_Q_POST_FAILED(p_msg_q)
#define  TRACE_OS_TASK_MSG_Q_PEND(p_msg_q)
#define  TRACE_OS_TASK_MSG_Q_PEND_FAILED(p_msg_q)
#define  TRACE_OS_TASK_MSG_Q_PEND_BLOCK(p_msg_q)

#define  TRACE_OS_TASK_SEM_CREATE(p_tcb, p_name)
#define  TRACE_OS_TASK_SEM_POST(p_tcb)
#define  TRACE_OS_TASK_SEM_POST_FAILED(p_tcb)
#define  TRACE_OS_TASK_SEM_PEND(p_tcb)
#define  TRACE_OS_TASK_SEM_PEND_FAILED(p_tcb)
#define  TRACE_OS_TASK_SEM_PEND_BLOCK(p_tcb)

#define  TRACE_OS_MUTEX_CREATE(p_mutex, p_name)         \
        trace_recorder_event(TRACE_OS_TYPE_MUTEX_CREATE, p_name)

#define  TRACE_OS_MUTEX_DEL(p_mutex)    \
        trace_recorder_event(TRACE_OS_TYPE_MUTEX_DEL, p_mutex->NamePtr)

#define  TRACE_OS_MUTEX_POST(p_mutex)   \
        trace_recorder_event(TRACE_OS_TYPE_MUTEX_POST, p_mutex->NamePtr)

#define  TRACE_OS_MUTEX_POST_FAILED(p_mutex)    \
        trace_recorder_event(TRACE_OS_TYPE_MUTEX_POST_FAILED, p_mutex->NamePtr)

#define  TRACE_OS_MUTEX_PEND(p_mutex)   \
        trace_recorder_event(TRACE_OS_TYPE_MUTEX_PEND, p_mutex->NamePtr)

#define  TRACE_OS_MUTEX_PEND_FAILED(p_mutex)    \
        trace_recorder_event(TRACE_OS_TYPE_MUTEX_PEND_FAILED, p_mutex->NamePtr)

#define  TRACE_OS_MUTEX_PEND_BLOCK(p_mutex)     \
        trace_recorder_event(TRACE_OS_TYPE_MUTEX_PEND_BLOCK, p_mutex->NamePtr)

#define  TRACE_OS_MUTEX_TASK_PRIO_INHERIT(p_tcb, prio)
#define  TRACE_OS_MUTEX_TASK_PRIO_DISINHERIT(p_tcb, prio)

#define  TRACE_OS_SEM_CREATE(p_sem, p_name)             \
        trace_recorder_event(TRACE_OS_TYPE_SEM_CREATE, p_name)

#define  TRACE_OS_SEM_DEL(p_sem)                         \
        trace_recorder_event(TRACE_OS_TYPE_SEM_DEL, p_sem->NamePtr)
        
#define  TRACE_OS_SEM_POST(p_sem)       \
        trace_recorder_event(TRACE_OS_TYPE_SEM_POST, p_sem->NamePtr)

#define  TRACE_OS_SEM_POST_FAILED(p_sem)        \
        trace_recorder_event(TRACE_OS_TYPE_SEM_POST_FAILED, p_sem->NamePtr)

#define  TRACE_OS_SEM_PEND(p_sem)       \
        trace_recorder_event(TRACE_OS_TYPE_SEM_PEND, p_sem->NamePtr)

#define  TRACE_OS_SEM_PEND_FAILED(p_sem)        \
        trace_recorder_event(TRACE_OS_TYPE_SEM_PEND_FAILED, p_sem->NamePtr)

#define  TRACE_OS_SEM_PEND_BLOCK(p_sem)         \
        trace_recorder_event(TRACE_OS_TYPE_SEM_PEND_BLOCK, p_sem->NamePtr)

#define  TRACE_OS_Q_CREATE(p_q, p_name)         \
        trace_recorder_event(TRACE_OS_TYPE_Q_CREATE, p_name)

#define  TRACE_OS_Q_DEL(p_q)    \
        trace_recorder_event(TRACE_OS_TYPE_Q_DEL, p_q->NamePtr)

#define  TRACE_OS_Q_POST(p_q)   \
        trace_recorder_event(TRACE_OS_TYPE_Q_POST, p_q->NamePtr)

#define  TRACE_OS_Q_POST_FAILED(p_q)    \
        trace_recorder_event(TRACE_OS_TYPE_Q_POST_FAILED, p_q->NamePtr)

#define  TRACE_OS_Q_PEND(p_q)   \
        trace_recorder_event(TRACE_OS_TYPE_Q_PEND, p_q->NamePtr)

#define  TRACE_OS_Q_PEND_FAILED(p_q)    \
        trace_recorder_event(TRACE_OS_TYPE_Q_PEND_FAILED, p_q->NamePtr)

#define  TRACE_OS_Q_PEND_BLOCK(p_q)     \
        trace_recorder_event(TRACE_OS_TYPE_Q_PEND_BLOCK, p_q->NamePtr)

#define  TRACE_OS_MEM_CREATE(p_mem, p_name)

#define  TRACE_OS_MEM_PUT(p_mem)        \
        trace_recorder_mfree(p_mem->AddrPtr, p_mem->BlkSize)

#define  TRACE_OS_MEM_PUT_FAILED(p_mem) \
        trace_recorder_mfree_failed(p_mem->AddrPtr)

#define  TRACE_OS_MEM_GET(p_mem)        \
        trace_recorder_malloc(p_mem->AddrPtr, p_mem->BlkSize)

#define  TRACE_OS_MEM_GET_FAILED(p_mem) \
        trace_recorder_malloc_failed(p_mem->BlkSize)

#define  TRACE_OS_FLAG_CREATE(p_grp, p_name)
#define  TRACE_OS_FLAG_DEL(p_grp)
#define  TRACE_OS_FLAG_POST(p_grp)
#define  TRACE_OS_FLAG_POST_FAILED(p_grp)
#define  TRACE_OS_FLAG_PEND(p_grp)
#define  TRACE_OS_FLAG_PEND_FAILED(p_grp)
#define  TRACE_OS_FLAG_PEND_BLOCK(p_grp)

#else

#define  TRACE_OS_TICK_INCREMENT(OSTickCtr)

#define  TRACE_OS_TASK_CREATE(p_tcb)
#define  TRACE_OS_TASK_CREATE_FAILED(p_tcb)
#define  TRACE_OS_TASK_DEL(p_tcb)
#define  TRACE_OS_TASK_READY(p_tcb)
#define  TRACE_OS_TASK_SWITCHED_IN(p_tcb)
#define  TRACE_OS_TASK_DLY(dly_ticks)
#define  TRACE_OS_TASK_SUSPEND(p_tcb)
#define  TRACE_OS_TASK_RESUME(p_tcb)


#define  TRACE_OS_ISR_REGISTER(isr_id, isr_name, isr_prio)
#define  TRACE_OS_ISR_BEGIN(isr_id)
#define  TRACE_OS_ISR_END()

#define  TRACE_OS_TASK_MSG_Q_CREATE(p_msg_q, p_name)
#define  TRACE_OS_TASK_MSG_Q_POST(p_msg_q)
#define  TRACE_OS_TASK_MSG_Q_POST_FAILED(p_msg_q)
#define  TRACE_OS_TASK_MSG_Q_PEND(p_msg_q)
#define  TRACE_OS_TASK_MSG_Q_PEND_FAILED(p_msg_q)
#define  TRACE_OS_TASK_MSG_Q_PEND_BLOCK(p_msg_q)

#define  TRACE_OS_TASK_SEM_CREATE(p_tcb, p_name)
#define  TRACE_OS_TASK_SEM_POST(p_tcb)
#define  TRACE_OS_TASK_SEM_POST_FAILED(p_tcb)
#define  TRACE_OS_TASK_SEM_PEND(p_tcb)
#define  TRACE_OS_TASK_SEM_PEND_FAILED(p_tcb)
#define  TRACE_OS_TASK_SEM_PEND_BLOCK(p_tcb)
#define  TRACE_OS_MUTEX_CREATE(p_mutex, p_name)
#define  TRACE_OS_MUTEX_DEL(p_mutex)
#define  TRACE_OS_MUTEX_POST(p_mutex)
#define  TRACE_OS_MUTEX_POST_FAILED(p_mutex)
#define  TRACE_OS_MUTEX_PEND(p_mutex)
#define  TRACE_OS_MUTEX_PEND_FAILED(p_mutex)
#define  TRACE_OS_MUTEX_PEND_BLOCK(p_mutex)

#define  TRACE_OS_MUTEX_TASK_PRIO_INHERIT(p_tcb, prio)
#define  TRACE_OS_MUTEX_TASK_PRIO_DISINHERIT(p_tcb, prio)

#define  TRACE_OS_SEM_CREATE(p_sem, p_name)
#define  TRACE_OS_SEM_DEL(p_sem)
#define  TRACE_OS_SEM_POST(p_sem)
#define  TRACE_OS_SEM_POST_FAILED(p_sem)
#define  TRACE_OS_SEM_PEND(p_sem)
#define  TRACE_OS_SEM_PEND_FAILED(p_sem)
#define  TRACE_OS_SEM_PEND_BLOCK(p_sem)

#define  TRACE_OS_Q_CREATE(p_q, p_name)
#define  TRACE_OS_Q_DEL(p_q)
#define  TRACE_OS_Q_POST(p_q)
#define  TRACE_OS_Q_POST_FAILED(p_q)
#define  TRACE_OS_Q_PEND(p_q)
#define  TRACE_OS_Q_PEND_FAILED(p_q)
#define  TRACE_OS_Q_PEND_BLOCK(p_q)

#define  TRACE_OS_FLAG_CREATE(p_grp, p_name)
#define  TRACE_OS_FLAG_DEL(p_grp)
#define  TRACE_OS_FLAG_POST(p_grp)
#define  TRACE_OS_FLAG_POST_FAILED(p_grp)
#define  TRACE_OS_FLAG_PEND(p_grp)
#define  TRACE_OS_FLAG_PEND_FAILED(p_grp)
#define  TRACE_OS_FLAG_PEND_BLOCK(p_grp)

#define  TRACE_OS_MEM_CREATE(p_mem, p_name)
#define  TRACE_OS_MEM_PUT(p_mem)
#define  TRACE_OS_MEM_PUT_FAILED(p_mem)
#define  TRACE_OS_MEM_GET(p_mem)
#define  TRACE_OS_MEM_GET_FAILED(p_mem)

#endif

#endif //__TRCKERNELPORTFREERTOS_H__
