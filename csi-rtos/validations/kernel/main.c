/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     main.c
 * @brief    CSI Source File for main
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#include <csi_config.h>
#include <stdint.h>
#include <stdio.h>
#include <csi_kernel.h>
#include "test_util.h"
#include <board_init.h>

#define K_API_PARENT_PRIO    5
#define APP_MAIN_TASK_STK_SIZE 1024

extern void app_main(void);

k_task_handle_t app_main_handle;

int main(void)
{
    uint32_t ret = 0;
    board_init();
    csi_kernel_init();

    test_case_init();
    csi_kernel_task_new((k_task_entry_t)app_main, "app_main",
                        0, K_API_PARENT_PRIO, 0, 0, APP_MAIN_TASK_STK_SIZE, &app_main_handle);
    csi_kernel_start();

    return ret;
}
