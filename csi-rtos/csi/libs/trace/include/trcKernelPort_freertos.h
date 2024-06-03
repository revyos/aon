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

#include "FreeRTOS.h"	/* Defines configUSE_TRACE_FACILITY */
#include "trcHardwarePort.h"
#include "trcKernelHooks.h"

#define USE_TRACEALYZER_RECORDER configUSE_TRACE_FACILITY

#if (USE_TRACEALYZER_RECORDER == 1)

#define traceTYPE_TASK_CREATE                                   0x01
#define traceTYPE_TASK_CREATE_FAILED                            0x02
#define traceTYPE_TASK_DELAY                                    0x03
#define traceTYPE_TASK_DELAY_UNTIL                              0x04
#define traceTYPE_TASK_PRIORITY_SET                             0x05
#define traceTYPE_TASK_SUSPEND                                  0x06
#define traceTYPE_TASK_RESUME                                   0x07
#define traceTYPE_TASK_RESUME_FROM_ISR                          0x08
#define traceTYPE_TASK_DELETE                                   0x09
#define traceTYPE_CREATE_COUNTING_SEMAPHORE                     0x0a
#define traceTYPE_CREATE_COUNTING_SEMAPHORE_FAILED              0x0b
#define traceTYPE_CREATE_MUTEX                                  0x13
#define traceTYPE_CREATE_MUTEX_FAILED                           0x14
#define traceTYPE_GIVE_MUTEX_RECURSIVE                          0x15
#define traceTYPE_GIVE_MUTEX_RECURSIVE_FAILED                   0x16
#define traceTYPE_TAKE_MUTEX_RECURSIVE                          0x17
#define traceTYPE_TAKE_MUTEX_RECURSIVE_FAILED                   0x18
#define traceTYPE_QUEUE_CREATE                                  0x21
#define traceTYPE_QUEUE_CREATE_FAILED                           0x22
#define traceTYPE_QUEUE_SEND                                    0x24
#define traceTYPE_QUEUE_SEND_FAILED                             0x25
#define traceTYPE_QUEUE_SEND_FROM_ISR                           0x26
#define traceTYPE_QUEUE_SEND_FROM_ISR_FAILED                    0x27
#define traceTYPE_BLOCKING_ON_QUEUE_RECEIVE                     0x28
#define traceTYPE_BLOCKING_ON_QUEUE_SEND                        0x29
#define traceTYPE_QUEUE_RECEIVE                                 0x2a
#define traceTYPE_QUEUE_RECEIVE_FAILED                          0x2b
#define traceTYPE_QUEUE_RECEIVE_FROM_ISR                        0x2c
#define traceTYPE_QUEUE_RECEIVE_FROM_ISR_FAILED                 0x2d
#define traceTYPE_QUEUE_PEEK                                    0x2e
#define traceTYPE_QUEUE_PEEK_FROM_ISR                           0x2f
#define traceTYPE_QUEUE_PEEK_FROM_ISR_FAILED                    0x30
#define traceTYPE_QUEUE_DELETE                                  0x32
#define traceTYPE_TIMER_CREATE                                  0x33
#define traceTYPE_TIMER_CREATE_FAILED                           0x34
#define traceTYPE_TIMER_EXPIRED                                 0x35

void _trace_store_data(uint8_t type, void *pxNewQueue);

#undef traceSTART
/* Used to perform any necessary initialisation - for example, open a file
into which trace is to be written. */
#define traceSTART()

#undef traceEND
/* Use to close a trace, for example close a file into which trace has been
written. */
#define traceEND()

#undef traceTASK_SWITCHED_IN
/* Called after a task has been selected to run.  pxCurrentTCB holds a pointer
to the task control block of the selected task. */
#define traceTASK_SWITCHED_IN()

#undef traceINCREASE_TICK_COUNT
/* Called before stepping the tick count after waking from tickless idle
sleep. */
#define traceINCREASE_TICK_COUNT(x)

#undef traceLOW_POWER_IDLE_BEGIN
/* Called immediately before entering tickless idle. */
#define traceLOW_POWER_IDLE_BEGIN()

#undef    traceLOW_POWER_IDLE_END
/* Called when returning to the Idle task after a tickless idle. */
#define traceLOW_POWER_IDLE_END()

#undef traceTASK_SWITCHED_OUT
/* Called before a task has been selected to run.  pxCurrentTCB holds a pointer
to the task control block of the task being switched out. */
#define traceTASK_SWITCHED_OUT()

#undef traceTASK_PRIORITY_INHERIT
/* Called when a task attempts to take a mutex that is already held by a
lower priority task.  pxTCBOfMutexHolder is a pointer to the TCB of the task
that holds the mutex.  uxInheritedPriority is the priority the mutex holder
will inherit (the priority of the task that is attempting to obtain the
muted. */
#define traceTASK_PRIORITY_INHERIT(pxTCBOfMutexHolder, uxInheritedPriority)

#undef traceTASK_PRIORITY_DISINHERIT
/* Called when a task releases a mutex, the holding of which had resulted in
the task inheriting the priority of a higher priority task.
pxTCBOfMutexHolder is a pointer to the TCB of the task that is releasing the
mutex.  uxOriginalPriority is the task's configured (base) priority. */
#define traceTASK_PRIORITY_DISINHERIT(pxTCBOfMutexHolder, uxOriginalPriority)

#undef traceBLOCKING_ON_QUEUE_RECEIVE
/* Task is about to block because it cannot read from a
queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
upon which the read was attempted.  pxCurrentTCB points to the TCB of the
task that attempted the read. */
#define traceBLOCKING_ON_QUEUE_RECEIVE(pxQueue) \
        _trace_store_data(traceTYPE_BLOCKING_ON_QUEUE_RECEIVE, pxQueue)

#undef traceBLOCKING_ON_QUEUE_SEND
/* Task is about to block because it cannot write to a
queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
upon which the write was attempted.  pxCurrentTCB points to the TCB of the
task that attempted the write. */
#define traceBLOCKING_ON_QUEUE_SEND(pxQueue)    \
        _trace_store_data(traceTYPE_BLOCKING_ON_QUEUE_SEND, pxQueue)

/* The following event macros are embedded in the kernel API calls. */

#undef traceMOVED_TASK_TO_READY_STATE
#define traceMOVED_TASK_TO_READY_STATE(pxTCB)

#undef  traceQUEUE_CREATE
#define traceQUEUE_CREATE(pxNewQueue)   \
        _trace_store_data(traceTYPE_QUEUE_CREATE, pxNewQueue)

#undef  traceQUEUE_CREATE_FAILED
#define traceQUEUE_CREATE_FAILED(ucQueueType)   \
        trace_recorder_event(traceTYPE_QUEUE_CREATE_FAILED, NULL)

#undef  traceCREATE_MUTEX
#define traceCREATE_MUTEX(pxNewQueue)   \
        _trace_store_data(traceTYPE_CREATE_MUTEX, pxNewQueue)

#undef  traceCREATE_MUTEX_FAILED
#define traceCREATE_MUTEX_FAILED()      \
        trace_recorder_event(traceTYPE_CREATE_MUTEX_FAILED, NULL)

#undef  traceGIVE_MUTEX_RECURSIVE
#define traceGIVE_MUTEX_RECURSIVE(pxMutex)      \
        _trace_store_data(traceTYPE_GIVE_MUTEX_RECURSIVE, pxMutex)

#undef  traceGIVE_MUTEX_RECURSIVE_FAILED
#define traceGIVE_MUTEX_RECURSIVE_FAILED(pxMutex)       \
        _trace_store_data(traceTYPE_GIVE_MUTEX_RECURSIVE_FAILED, pxMutex)

#undef  traceTAKE_MUTEX_RECURSIVE
#define traceTAKE_MUTEX_RECURSIVE(pxMutex)      \
        _trace_store_data(traceTYPE_TAKE_MUTEX_RECURSIVE, pxMutex)

#undef  traceTAKE_MUTEX_RECURSIVE_FAILED
#define traceTAKE_MUTEX_RECURSIVE_FAILED(pxMutex)       \
        _trace_store_data(traceTYPE_TAKE_MUTEX_RECURSIVE_FAILED, pxMutex)

#undef  traceCREATE_COUNTING_SEMAPHORE
#define traceCREATE_COUNTING_SEMAPHORE()        \
        trace_recorder_event(traceTYPE_CREATE_COUNTING_SEMAPHORE, NULL)

#undef  traceCREATE_COUNTING_SEMAPHORE_FAILED
#define traceCREATE_COUNTING_SEMAPHORE_FAILED() \
        trace_recorder_event(traceTYPE_CREATE_COUNTING_SEMAPHORE_FAILED, NULL)

#undef  traceQUEUE_SEND
#define traceQUEUE_SEND(pxQueue)        \
        _trace_store_data(traceTYPE_QUEUE_SEND, pxQueue)

#undef  traceQUEUE_SEND_FAILED
#define traceQUEUE_SEND_FAILED(pxQueue) \
        _trace_store_data(traceTYPE_QUEUE_SEND_FAILED, pxQueue)

#undef  traceQUEUE_RECEIVE
#define traceQUEUE_RECEIVE(pxQueue)     \
        _trace_store_data(traceTYPE_QUEUE_RECEIVE, pxQueue)

#undef  traceQUEUE_PEEK
#define traceQUEUE_PEEK(pxQueue)        \
        _trace_store_data(traceTYPE_QUEUE_PEEK, pxQueue)

#undef  traceQUEUE_PEEK_FROM_ISR
#define traceQUEUE_PEEK_FROM_ISR(pxQueue)       \
        _trace_store_data(traceTYPE_QUEUE_PEEK_FROM_ISR, pxQueue)

#undef  traceQUEUE_RECEIVE_FAILED
#define traceQUEUE_RECEIVE_FAILED(pxQueue)      \
        _trace_store_data(traceTYPE_QUEUE_RECEIVE_FAILED, pxQueue)

#undef  traceQUEUE_SEND_FROM_ISR
#define traceQUEUE_SEND_FROM_ISR(pxQueue)       \
        _trace_store_data(traceTYPE_QUEUE_SEND_FROM_ISR, pxQueue)

#undef  traceQUEUE_SEND_FROM_ISR_FAILED
#define traceQUEUE_SEND_FROM_ISR_FAILED(pxQueue)        \
        _trace_store_data(traceTYPE_QUEUE_SEND_FROM_ISR_FAILED, pxQueue)

#undef  traceQUEUE_RECEIVE_FROM_ISR
#define traceQUEUE_RECEIVE_FROM_ISR(pxQueue)    \
        _trace_store_data(traceTYPE_QUEUE_RECEIVE_FROM_ISR, pxQueue)

#undef  traceQUEUE_RECEIVE_FROM_ISR_FAILED
#define traceQUEUE_RECEIVE_FROM_ISR_FAILED(pxQueue)     \
        _trace_store_data(traceTYPE_QUEUE_RECEIVE_FROM_ISR_FAILED, pxQueue)

#undef  traceQUEUE_PEEK_FROM_ISR_FAILED
#define traceQUEUE_PEEK_FROM_ISR_FAILED(pxQueue)        \
        _trace_store_data(traceTYPE_QUEUE_PEEK_FROM_ISR_FAILED, pxQueue)

#undef  traceQUEUE_DELETE
#define traceQUEUE_DELETE(pxQueue)      \
        _trace_store_data(traceTYPE_QUEUE_DELETE, pxQueue)

#undef  traceTASK_CREATE
#define traceTASK_CREATE(pxNewTCB)    \
        trace_recorder_event(traceTYPE_TASK_CREATE, pxTCB->pcTaskName)

#undef  traceTASK_CREATE_FAILED
#define traceTASK_CREATE_FAILED()       \
        trace_recorder_event(traceTYPE_TASK_CREATE_FAILED, NULL)

#undef  traceTASK_DELETE
#define traceTASK_DELETE(pxTaskToDelete)        \
        trace_recorder_event(traceTYPE_TASK_DELETE, pxTaskToDelete->pcTaskName)

#undef  traceTASK_DELAY_UNTIL
#define traceTASK_DELAY_UNTIL() \
        trace_recorder_event(traceTYPE_TASK_DELAY_UNTIL, NULL)

#undef  traceTASK_DELAY
#define traceTASK_DELAY()       \
        trace_recorder_event(traceTYPE_TASK_DELAY, NULL)

#undef  traceTASK_PRIORITY_SET
#define traceTASK_PRIORITY_SET(pxTask, uxNewPriority)   \
        trace_recorder_event(traceTYPE_TASK_PRIORITY_SET, pxTask->pcTaskName)

#undef  traceTASK_SUSPEND
#define traceTASK_SUSPEND(pxTaskToSuspend)      \
        trace_recorder_event(traceTYPE_TASK_SUSPEND, pxTaskToSuspend->pcTaskName)

#undef  traceTASK_RESUME
#define traceTASK_RESUME(pxTaskToResume)        \
        trace_recorder_event(traceTYPE_TASK_RESUME, pxTaskToResume->pcTaskName)

#undef  traceTASK_RESUME_FROM_ISR
#define traceTASK_RESUME_FROM_ISR(pxTaskToResume)       \
        trace_recorder_event(traceTYPE_TASK_RESUME_FROM_ISR, pxTaskToResume->pcTaskName)

#undef  traceTASK_INCREMENT_TICK
#define traceTASK_INCREMENT_TICK(xTickCount)    \
          { trcKERNEL_HOOKS_INCREMENT_TICK(); }


#undef  traceTIMER_CREATE
#define traceTIMER_CREATE(pxNewTimer)   \
        trace_recorder_event(traceTYPE_TIMER_CREATE, pxNewTimer->pcTimerName)

#undef  traceTIMER_CREATE_FAILED
#define traceTIMER_CREATE_FAILED()      \
        trace_recorder_event(traceTYPE_TIMER_CREATE_FAILED, NULL)

#undef  traceTIMER_EXPIRED
#define traceTIMER_EXPIRED(pxTimer)     \
        trace_recorder_event(traceTYPE_TIMER_EXPIRED, pxTimer->pcTimerName)

#undef  traceMALLOC
#define traceMALLOC(pvAddress, uiSize)  \
        trace_recorder_malloc(pvAddress, uiSize)

#undef  traceFREE
#define traceFREE(pvAddress, uiSize)    \
        trace_recorder_mfree(pvAddress, uiSize)

#undef traceEVENT_GROUP_CREATE
#define traceEVENT_GROUP_CREATE(xEventGroup)

#undef traceEVENT_GROUP_CREATE_FAILED
#define traceEVENT_GROUP_CREATE_FAILED()

#undef traceEVENT_GROUP_SYNC_BLOCK
#define traceEVENT_GROUP_SYNC_BLOCK(xEventGroup, uxBitsToSet, uxBitsToWaitFor)

#undef traceEVENT_GROUP_SYNC_END
#define traceEVENT_GROUP_SYNC_END(xEventGroup, uxBitsToSet, uxBitsToWaitFor, xTimeoutOccurred) ( void ) xTimeoutOccurred

#undef traceEVENT_GROUP_WAIT_BITS_BLOCK
#define traceEVENT_GROUP_WAIT_BITS_BLOCK(xEventGroup, uxBitsToWaitFor)

#undef traceEVENT_GROUP_WAIT_BITS_END
#define traceEVENT_GROUP_WAIT_BITS_END(xEventGroup, uxBitsToWaitFor, xTimeoutOccurred) ( void ) xTimeoutOccurred

#undef traceEVENT_GROUP_CLEAR_BITS
#define traceEVENT_GROUP_CLEAR_BITS(xEventGroup, uxBitsToClear)

#undef traceEVENT_GROUP_CLEAR_BITS_FROM_ISR
#define traceEVENT_GROUP_CLEAR_BITS_FROM_ISR(xEventGroup, uxBitsToClear)

#undef traceEVENT_GROUP_SET_BITS
#define traceEVENT_GROUP_SET_BITS(xEventGroup, uxBitsToSet)

#undef traceEVENT_GROUP_SET_BITS_FROM_ISR
#define traceEVENT_GROUP_SET_BITS_FROM_ISR(xEventGroup, uxBitsToSet)

#undef traceEVENT_GROUP_DELETE
#define traceEVENT_GROUP_DELETE(xEventGroup)

#undef tracePEND_FUNC_CALL
#define tracePEND_FUNC_CALL(xFunctionToPend, pvParameter1, ulParameter2, ret)

#undef tracePEND_FUNC_CALL_FROM_ISR
#define tracePEND_FUNC_CALL_FROM_ISR(xFunctionToPend, pvParameter1, ulParameter2, ret)

#undef traceQUEUE_REGISTRY_ADD
#define traceQUEUE_REGISTRY_ADD(xQueue, pcQueueName)

#undef traceTASK_NOTIFY_TAKE_BLOCK
#define traceTASK_NOTIFY_TAKE_BLOCK()

#undef traceTASK_NOTIFY_TAKE
#define traceTASK_NOTIFY_TAKE()

#undef traceTASK_NOTIFY_WAIT_BLOCK
#define traceTASK_NOTIFY_WAIT_BLOCK()

#undef traceTASK_NOTIFY_WAIT
#define traceTASK_NOTIFY_WAIT()

#undef traceTASK_NOTIFY
#define traceTASK_NOTIFY()

#undef traceTASK_NOTIFY_FROM_ISR
#define traceTASK_NOTIFY_FROM_ISR()

#undef traceTASK_NOTIFY_GIVE_FROM_ISR
#define traceTASK_NOTIFY_GIVE_FROM_ISR()

#else

#endif
#endif //__TRCKERNELPORTFREERTOS_H__
