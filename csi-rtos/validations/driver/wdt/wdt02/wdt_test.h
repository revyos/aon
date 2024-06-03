/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     wdt_test.h
 * @brief    the head file
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <drv/wdt.h>
#include <csi_kernel.h>

extern void mdelay(uint32_t ms);

#define WDT_INDEX   0
#define WDT_TEST_DURATION   10000
