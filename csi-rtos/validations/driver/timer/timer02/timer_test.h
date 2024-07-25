/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     timer_test.h
 * @brief    the head file
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <drv/timer.h>
#include <csi_kernel.h>

extern void mdelay(uint32_t ms);

/*
 *	CM6501 has 5 timer
 */
#define TIMER_TOTLE_COUNT   5
#define TIMER_TEST_DURATION   20000000
#define TEST_TIMER	0
