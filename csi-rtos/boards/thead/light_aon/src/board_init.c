/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     main.c
 * @brief    hello world
 * @version  V1.0
 * @date     17. Jan 2018
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <drv/uart.h>
#include <drv/mbox.h>
#include <drv/rtc.h>

#include "syslog.h"
#include "board_config.h"
#include "console/sys_console.h"

sys_console_t console;
csi_mbox_t mbox;
csi_rtc_t rtc;

void board_init(void)
{
    int ret = 0;

    console.uart_id = (uint32_t)CONSOLE_IDX;
    console.baudrate = 115200U;
    console.tx.pin = CONSOLE_TXD;
    console.tx.func = CONSOLE_TXD_FUNC;
    console.rx.pin = CONSOLE_RXD;
    console.rx.func = CONSOLE_RXD_FUNC;
    console.uart = NULL;

    console_init(&console);

	ret = csi_mbox_init(&mbox, EXAMPLE_MBOX_IDX);
	if (ret != CSI_OK) {
		LOG_E("mbox init failed\n");
	}
	ret = csi_rtc_init(&rtc, 0);
	if (ret != CSI_OK) {
		LOG_E("rtc init failed\n");
	}
}

void __attribute__ ((weak)) board_resume()
{
    int ret = 0;
    ret = csi_mbox_init(&mbox, EXAMPLE_MBOX_IDX);
	if (ret != CSI_OK) {
		LOG_E("mbox resume failed\n");
	}
}
