/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     sem_test.c
 * @brief    the main function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <drv/gpio.h>
#include <csi_kernel.h>
#include <stdio.h>
#include "pin.h"
#include "pin_name.h"


#define GPIO_TEST_PIN	PA18 //PA5 //PD7  //sc5654a---R119
#define PIN_FUNC_GPIO	PIN_FUNC_GPIO

/* provide input/output for test pin*/
#define GPIO_PIN	PA17
#define GPIO_FUNC    PIN_FUNC_GPIO
