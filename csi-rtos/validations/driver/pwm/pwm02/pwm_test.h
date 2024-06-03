/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     pwm_test.h
 * @brief    the main function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <drv/pwm.h>
#include <csi_kernel.h>
#include <stdio.h>
#include "pin.h"
#include "pin_name.h"

extern void mdelay(uint32_t ms);

#define  PWM_INDEX          0
#define  PWM_CH_PIN         PC4 //EXAMPLE_PWM_CH
#define  PWM_CH_PIN_FUNC    IO_FUNC_PWM  //EXAMPLE_PWM_CH_FUNC
#define  PWM_CH             5

