/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     sys_console.c
 * @brief    CSI Source File for console
 * @version  V1.0
 * @date     2019-12-19
 ******************************************************************************/

#ifndef __TRC_CONFIG_H__
#define __TRC_CONFIG_H__
#include "csi_config.h"

/****** Port Name ********************** Code ***** Official ** OS Platform *********/
#define PORT_CSKY_ABIV1                        1    /*	Yes			Any					*/
#define PORT_CSKY_ABIV2                        2    /*	Yes			Any					*/

/**
 * ENABLE_DEBUG_PRINT
 *
 * 1 means enable trace_printf
 * 0 means disable trace_printf
 * default is 0
 */
#define ENABLE_DEBUG_PRINT                     0

#ifdef CONFIG_KERNEL_UCOS
#include "trcConfig_ucos.h"
#elif CONFIG_KERNEL_FREERTOS
#include "trcConfig_freertos.h"
#elif CONFIG_KERNEL_RHINO
#include "trcConfig_rhino.h"
#endif

#endif
