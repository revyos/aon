/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic_loop.c
 * @brief    iic loop test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iic_test.h"
#include <string.h>
#include <soc.h>
#include <pin.h>

static volatile uint8_t transfer_done_flag;
static send_data[2048] = {0x0};
static receive_data[2048] = {0x0};

static void iic_callback(int32_t idx, iic_event_e event)
{
    switch (event) {
        case IIC_EVENT_TRANSFER_DONE:
            transfer_done_flag = 0;
            break;

        default :
            break;
    }
}

void iic_loop_test(void)
{

//TODO

}
