/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     sys_console.c
 * @brief    CSI Source File for console
 * @version  V1.0
 * @date     2019-12-19
 ******************************************************************************/
#ifndef __TRCKERNELPORTRHINO_H__
#define __TRCKERNELPORTRHINO_H__
#include <k_api.h>
#include "trcHardwarePort.h"
#include "csi_kernel.h"
#include "trcKernelHooks.h"

#define USE_TRACEALYZER_RECORDER   1

#if (SELECTED_PORT == PORT_CSKY_ABIV1)
#define TRACE_SR_ALLOC_CRITICAL_SECTION() CPSR_ALLOC()
#define TRACE_ENTER_CRITICAL_SECTION()    RHINO_CPU_INTRPT_DISABLE();
#define TRACE_EXIT_CRITICAL_SECTION()     RHINO_CPU_INTRPT_ENABLE();
#elif (SELECTED_PORT == PORT_CSKY_ABIV2)
#define TRACE_SR_ALLOC_CRITICAL_SECTION() CPSR_ALLOC();
	#define TRACE_ENTER_CRITICAL_SECTION()    RHINO_CPU_INTRPT_DISABLE();
	#define TRACE_EXIT_CRITICAL_SECTION()     RHINO_CPU_INTRPT_ENABLE();
#endif

#define TRACE_TICK_RATE_HZ           RHINO_CONFIG_TICKS_PER_SECOND

#ifndef TRACE_ENTER_CRITICAL_SECTION
#error "This port has no valid definition for critical sections! See http://percepio.com/2014/10/27/how-to-define-critical-sections-for-the-recorder/"
#endif

#if (USE_TRACEALYZER_RECORDER == 1)

#define TRACE_TYPE_TASK_CREATE                          0x01
#define TRACE_TYPE_TASK_SLEEP                           0x03
#define TRACE_TYPE_TASK_PRI_CHANGE                      0x05
#define TRACE_TYPE_TASK_SUSPEND                         0x06
#define TRACE_TYPE_TASK_RESUME                          0x07
#define TRACE_TYPE_INTRPT_TASK_SWITCH                   0x08
#define TRACE_TYPE_TASK_DEL                             0x09
#define TRACE_TYPE_SEM_CREATE                           0x0a
#define TRACE_TYPE_SEM_OVERFLOW                         0x0c
#define TRACE_TYPE_SEM_CNT_INCREASE                     0x0d
#define TRACE_TYPE_SEM_GET_SUCCESS                      0x0f
#define TRACE_TYPE_SEM_GET_BLK                          0x11
#define TRACE_TYPE_SEM_DEL                              0x12
#define TRACE_TYPE_MUTEX_CREATE                         0x13
#define TRACE_TYPE_MUTEX_RELEASE                        0x19
#define TRACE_TYPE_MUTEX_RELEASE_SUCCESS                0x1b
#define TRACE_TYPE_MUTEX_GET                            0x1c
#define TRACE_TYPE_MUTEX_GET_BLK                        0x1e
#define TRACE_TYPE_MUTEX_TASK_WAKE                      0x1f
#define TRACE_TYPE_MUTEX_DEL                            0x20
#define TRACE_TYPE_BUF_QUEUE_CREATE                     0x21
#define TRACE_TYPE_BUF_QUEUE_MAX                        0x23
#define TRACE_TYPE_BUF_QUEUE_POST                       0x24
#define TRACE_TYPE_BUF_QUEUE_GET_BLK                    0x28
#define TRACE_TYPE_BUF_QUEUE_TASK_WAKE                  0x31
#define TRACE_TYPE_TIMER_CREATE                         0x33
#define TRACE_TYPE_TIMER_DEL                            0x35
#define TRACE_TYPE_EVENT_CREATE                         0x36
#define TRACE_TYPE_EVENT_GET                            0x37
#define TRACE_TYPE_EVENT_GET_BLK                        0x38
#define TRACE_TYPE_EVENT_TASK_WAKE                      0x39
#define TRACE_TYPE_EVENT_DEL                            0x3a
#define TRACE_TYPE_EVT_DIV                              0x3f




/* task trace */
#undef  TRACE_INIT
#define TRACE_INIT()

#undef TRACE_TASK_INCREMENT_TICK
#define TRACE_TASK_INCREMENT_TICK() \
          { trcKERNEL_HOOKS_INCREMENT_TICK(); }

#undef  TRACE_TASK_SWITCH
#define TRACE_TASK_SWITCH(from, to)     \
        trace_recorder_event(TRACE_TYPE_INTRPT_TASK_SWITCH, to->task_name)

#undef  TRACE_TASK_CREATE
#define TRACE_TASK_CREATE(task)         \
        trace_recorder_event(TRACE_TYPE_TASK_CREATE, task->task_name)

#undef  TRACE_TASK_SLEEP
#define TRACE_TASK_SLEEP(task, ticks)   \
        trace_recorder_event(TRACE_TYPE_TASK_SLEEP, task->task_name)

#undef  TRACE_INTRPT_TASK_SWITCH
#define TRACE_INTRPT_TASK_SWITCH(from, to)      \
        trace_recorder_event(TRACE_TYPE_INTRPT_TASK_SWITCH, to->task_name)

#undef  TRACE_TASK_PRI_CHANGE
#define TRACE_TASK_PRI_CHANGE(task, task_pri_chg, pri)  \
        trace_recorder_event(TRACE_TYPE_TASK_PRI_CHANGE, task->task_name)

#undef  TRACE_TASK_SUSPEND
#define TRACE_TASK_SUSPEND(task, task_suspended)        \
        trace_recorder_event(TRACE_TYPE_TASK_SUSPEND, task->task_name)

#undef  TRACE_TASK_RESUME
#define TRACE_TASK_RESUME(task, task_resumed)   \
        trace_recorder_event(TRACE_TYPE_TASK_RESUME, task->task_name)

#undef  TRACE_TASK_DEL
#define TRACE_TASK_DEL(task, task_del)          \
        trace_recorder_event(TRACE_TYPE_TASK_DEL, task->task_name)

#undef  TRACE_TASK_WAIT_ABORT
#define TRACE_TASK_WAIT_ABORT(task, task_abort)

/* semaphore trace */
#undef  TRACE_SEM_CREATE
#define TRACE_SEM_CREATE(task, sem)     \
        trace_recorder_event(TRACE_TYPE_SEM_CREATE, sem->blk_obj.name)

#undef  TRACE_SEM_OVERFLOW
#define TRACE_SEM_OVERFLOW(task, sem)   \
        trace_recorder_event(TRACE_TYPE_SEM_OVERFLOW, sem->blk_obj.name)

#undef  TRACE_SEM_CNT_INCREASE
#define TRACE_SEM_CNT_INCREASE(task, sem)       \
        trace_recorder_event(TRACE_TYPE_SEM_CNT_INCREASE, sem->blk_obj.name)

#undef  TRACE_SEM_GET_SUCCESS
#define TRACE_SEM_GET_SUCCESS(task, sem)        \
        trace_recorder_event(TRACE_TYPE_SEM_GET_SUCCESS, sem->blk_obj.name)

#undef  TRACE_SEM_GET_BLK
#define TRACE_SEM_GET_BLK(task, sem, wait_option)       \
        trace_recorder_event(TRACE_TYPE_SEM_GET_BLK, sem->blk_obj.name)

#undef  TRACE_SEM_TASK_WAKE
#define TRACE_SEM_TASK_WAKE(task, task_waked_up, sem, opt_wake_all)

#undef  TRACE_SEM_DEL
#define TRACE_SEM_DEL(task, sem)        \
        trace_recorder_event(TRACE_TYPE_SEM_DEL, sem->blk_obj.name)

/* mutex trace */
#undef  TRACE_MUTEX_CREATE
#define TRACE_MUTEX_CREATE(task, mutex, name)       \
        trace_recorder_event(TRACE_TYPE_MUTEX_CREATE, name)

#undef  TRACE_MUTEX_RELEASE
#define TRACE_MUTEX_RELEASE(task, task_release, new_pri)        \
        trace_recorder_event(TRACE_TYPE_MUTEX_RELEASE, task_release->task_name)

#undef  TRACE_MUTEX_GET
#define TRACE_MUTEX_GET(task, mutex, wait_option)       \
        trace_recorder_event(TRACE_TYPE_MUTEX_GET, mutex->blk_obj.name)

#undef  TRACE_TASK_PRI_INV
#define TRACE_TASK_PRI_INV(task, mtxtsk)
#undef  TRACE_MUTEX_GET_BLK
#define TRACE_MUTEX_GET_BLK(task, mutex, wait_option)   \
        trace_recorder_event(TRACE_TYPE_MUTEX_GET_BLK, mutex->blk_obj.name)

#undef  TRACE_MUTEX_RELEASE_SUCCESS
#define TRACE_MUTEX_RELEASE_SUCCESS(task, mutex)        \
        trace_recorder_event(TRACE_TYPE_MUTEX_RELEASE_SUCCESS, mutex->blk_obj.name)

#undef  TRACE_MUTEX_TASK_WAKE
#define TRACE_MUTEX_TASK_WAKE(task, task_waked_up, mutex)       \
        trace_recorder_event(TRACE_TYPE_MUTEX_TASK_WAKE, mutex->blk_obj.name)

#undef  TRACE_MUTEX_DEL
#define TRACE_MUTEX_DEL(task, mutex)    \
        trace_recorder_event(TRACE_TYPE_MUTEX_DEL, mutex->blk_obj.name)

/* event trace */
#undef TRACE_EVENT_CREATE
#define TRACE_EVENT_CREATE(task, event, name, flags_init)       \
        trace_recorder_event(TRACE_TYPE_EVENT_CREATE, name)

#undef  TRACE_EVENT_GET
#define TRACE_EVENT_GET(task, event)    \
        trace_recorder_event(TRACE_TYPE_EVENT_GET, event->blk_obj.name)

#undef  TRACE_EVENT_GET_BLK
#define TRACE_EVENT_GET_BLK(task, event, wait_option)   \
        trace_recorder_event(TRACE_TYPE_EVENT_GET_BLK, event->blk_obj.name)

#undef  TRACE_EVENT_TASK_WAKE
#define TRACE_EVENT_TASK_WAKE(task, task_waked_up, event)       \
        trace_recorder_event(TRACE_TYPE_EVENT_TASK_WAKE, event->blk_obj.name)

#undef  TRACE_EVENT_DEL
#define TRACE_EVENT_DEL(task, event)    \
        trace_recorder_event(TRACE_TYPE_EVENT_DEL, event->blk_obj.name)

/* buf_queue trace */
#undef  TRACE_BUF_QUEUE_CREATE
#define TRACE_BUF_QUEUE_CREATE(task, buf_queue)     \
        trace_recorder_event(TRACE_TYPE_BUF_QUEUE_CREATE, buf_queue->blk_obj.name)

#undef  TRACE_BUF_QUEUE_MAX
#define TRACE_BUF_QUEUE_MAX(task, buf_queue, msg, msg_size)     \
        trace_recorder_event(TRACE_TYPE_BUF_QUEUE_MAX, buf_queue->blk_obj.name)

#undef  TRACE_BUF_QUEUE_POST
#define TRACE_BUF_QUEUE_POST(task, buf_queue, msg, msg_size)    \
        trace_recorder_event(TRACE_TYPE_BUF_QUEUE_POST, buf_queue->blk_obj.name)

#undef  TRACE_BUF_QUEUE_TASK_WAKE
#define TRACE_BUF_QUEUE_TASK_WAKE(task, task_waked_up, buf_queue)   \
        trace_recorder_event(TRACE_TYPE_BUF_QUEUE_TASK_WAKE, buf_queue->blk_obj.name)

#undef  TRACE_BUF_QUEUE_GET_BLK
#define TRACE_BUF_QUEUE_GET_BLK(task, buf_queue, wait_option)   \
        trace_recorder_event(TRACE_TYPE_BUF_QUEUE_GET_BLK, buf_queue->blk_obj.name)


/* timer trace */
#undef  TRACE_TIMER_CREATE
#define TRACE_TIMER_CREATE(task, timer)             \
        trace_recorder_event(TRACE_TYPE_TIMER_CREATE, timer->name)

#undef  TRACE_TIMER_DEL
#define TRACE_TIMER_DEL(task, timer)    \
        trace_recorder_event(TRACE_TYPE_TIMER_DEL, timer->name)

/* memory trace */
#undef  TRACE_MEM_MALLOC
#define TRACE_MEM_MALLOC(addr, size)    \
        trace_recorder_malloc(addr, size)

#undef  TRACE_MEM_MALLOC_FAILED
#define TRACE_MEM_MALLOC_FAILED(size)    \
        trace_recorder_malloc_failed(size)

#undef  TRACE_MEM_FREE
#define TRACE_MEM_FREE(addr, size)    \
        trace_recorder_mfree(addr, size)

#undef  TRACE_MEM_FREE_FAILED
#define TRACE_MEM_FREE_FAILED(addr)    \
        trace_recorder_mfree_failed(addr)

/* MBLK trace */
#undef  TRACE_MBLK_POOL_CREATE
#define TRACE_MBLK_POOL_CREATE(task, pool)    //traceMEM_CREATE(MEM, poll, "mem")

/* MM trace */
#undef  TRACE_MM_POOL_CREATE
#define TRACE_MM_POOL_CREATE(task, pool)

/* MM region trace*/
#undef  TRACE_MM_REGION_CREATE
#define TRACE_MM_REGION_CREATE(task, regions)

/* work queue trace */
#undef  TRACE_WORK_INIT
#define TRACE_WORK_INIT(task, work)
#undef  TRACE_WORKQUEUE_CREATE
#define TRACE_WORKQUEUE_CREATE(task, workqueue)
#undef  TRACE_WORKQUEUE_DEL
#define TRACE_WORKQUEUE_DEL(task, workqueue)

#endif  // (USE_TRACEALYZER_RECORDER == 1)

#endif //__TRCKERNELPORTRHINO_H__
