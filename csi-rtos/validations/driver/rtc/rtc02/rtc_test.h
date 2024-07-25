/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     task02.c
 * @brief    the main function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <drv/rtc.h>
#include <csi_kernel.h>

extern void mdelay(uint32_t ms);

#define RTC_COUNT   1
#define RTC_TEST_DURATION   200000
