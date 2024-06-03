/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     uart_test.h
 * @brief    the head file
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <drv/uart.h>
#include "pin.h"
#include "pin_name.h"
#include "dtest.h"

void usart_func_test2(void);

extern void mdelay(uint32_t ms);

//SC5654a
/*
#define UART_IDX    2
#define UART_TX     PD7
#define UART_RX     PD8
#define UART_TX_FUNC    PD7_UART3_TX
#define UART_RX_FUNC    PD8_UART3_RX
*/

//CB2301
/*
#define UART_IDX    0
#define UART_TX     PA8
#define UART_RX     PA9
#define UART_TX_FUNC    PA8_UART0_TX
#define UART_RX_FUNC    PA9_UART0_RX
*/

//CB2201
#define UART_IDX    1
#define UART_TX     PA17
#define UART_RX     PA16
#define UART_TX_FUNC    PA17_UART1_TX
#define UART_RX_FUNC    PA16_UART1_RX

//Danica
/*
#define UART_IDX    1
#define UART_TX     PA28
#define UART_RX     PA27
#define UART_TX_FUNC    PA28_UART1_TX
#define UART_RX_FUNC    PA27_UART1_RX
*/

/*
#define UART_IDX    2
#define UART_TX     GPIO18
#define UART_RX     GPIO19
#define UART_TX_FUNC    GPIO18_UART2_TXD
#define UART_RX_FUNC    GPIO19_UART2_RXD
*/

#define FIFO_SIZE	16	//UART1_IDX's fifo size
