/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     sys_console.c
 * @brief    CSI Source File for console
 * @version  V1.0
 * @date     2019-12-19
 ******************************************************************************/
#ifndef __TRCKERNELPORT_H__
#define __TRCKERNELPORT_H__

#include <csi_config.h>
#if (CONFIG_OS_TRACE == 1)
    #define USE_TRACEALYZER_RECORDER   1
#else
    #define USE_TRACEALYZER_RECORDER   0
#endif

#ifdef CONFIG_KERNEL_UCOS
#include "trcRecorder_ucos.h"
#elif CONFIG_KERNEL_FREERTOS
#include "trcRecorder_freertos.h"
#elif CONFIG_KERNEL_RHINO
#include "trcKernelPort_rhino.h"
#endif

#endif //__TRCKERNELPORT_H__
