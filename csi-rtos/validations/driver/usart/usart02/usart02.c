/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     usart02.c
 * @brief    the main function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "uart_test.h"
#include <stdio.h>
#include "soc.h"
#include <string.h>
#include <drv/timer.h>
#include "pin.h"

#define DATA_LENGTH   1024

static const char *msg = " !\"#$%&'()*+,-.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~i0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\0";

volatile uint8_t usart_send_cb_flag, usart_recv_cb_flag;
static uint8_t msg_func1[4096] = {0x0};
static void usart_event_cb_fun(int32_t idx, usart_event_e event)
{
    //printf("USART%d CB event %d entered!\n", idx, event);
    if (event == USART_EVENT_SEND_COMPLETE) {
        usart_send_cb_flag = 0;
    } else if (event == USART_EVENT_RECEIVE_COMPLETE) {
        usart_recv_cb_flag = 0;
    }
}

void usart_if_test()
{
    usart_handle_t usart;

    csi_pin_set_mux(UART_TX, UART_TX_FUNC);
    csi_pin_set_mux(UART_RX, UART_RX_FUNC);

    //get_capabilities
    usart_capabilities_t cap = csi_usart_get_capabilities(4);
    ASSERT_TRUE(cap.asynchronous == 0);

    //initialize

    usart = csi_usart_initialize(10, usart_event_cb_fun);
    ASSERT_TRUE(usart == NULL);

    usart = csi_usart_initialize(2, NULL);
    ASSERT_TRUE(usart != NULL);

    //uninitialize
    int32_t ret;
    ret = csi_usart_uninitialize(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    //power_control
    ret = csi_usart_power_control(NULL, DRV_POWER_FULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_power_control(usart, 5);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_UNSUPPORTED));

    //config_baudrate
    ret = csi_usart_config_baudrate(NULL, 115200);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    //config_mode
    ret = csi_usart_config_mode(NULL, USART_MODE_ASYNCHRONOUS);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_config_mode(usart, 6);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | USART_ERROR_MODE));

    //config_parity
    ret = csi_usart_config_parity(NULL, USART_PARITY_NONE);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_config_parity(usart, 5);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | USART_ERROR_PARITY));

    //config_stopbits
    ret = csi_usart_config_stopbits(NULL, USART_STOP_BITS_1);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_config_stopbits(usart, 4);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | USART_ERROR_STOP_BITS));

    //config_databits
    ret = csi_usart_config_databits(NULL, USART_DATA_BITS_8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_config_databits(usart, 5);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | USART_ERROR_DATA_BITS));

    //config_flowctrl //CM6501 UNSUPPORTED

    ret = csi_usart_config_flowctrl(NULL, USART_FLOWCTRL_CTS_RTS);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_config_flowctrl(usart, 4);
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE |USART_ERROR_FLOW_CONTROL));
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_UNSUPPORTED));

    char data[8] = {1, 1, 1, 1, 1, 1, 1, 1};

    //send
    ret = csi_usart_send(NULL, data, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_send(usart, NULL, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_send(usart, data, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    //send_dma //	csi中没有该接口
    /*
    ret = csi_usart_send_dma(NULL, data, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE |DRV_ERROR_PARAMETER));

    ret = csi_usart_send_dma(usart, NULL, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE |DRV_ERROR_PARAMETER));

    ret = csi_usart_send_dma(usart, data, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE |DRV_ERROR_PARAMETER));
    */
    //abort_send
    ret = csi_usart_abort_send(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_send(usart, data, 8);

    char out[8];
    //receive
    ret = csi_usart_receive(NULL, out, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_receive(usart, NULL, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    //receive_dma	//	csi中没有该接口
    /*
    ret = csi_usart_receive_dma(NULL, out, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE |DRV_ERROR_PARAMETER));

    ret = csi_usart_receive_dma(usart, NULL, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE |DRV_ERROR_PARAMETER));
    */

    //receive_query
    ret = csi_usart_receive_query(NULL, out, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_receive_query(usart, NULL, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    //abort_receive
    ret = csi_usart_abort_receive(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    //get_status
    usart_status_t stat = csi_usart_get_status(NULL);
    ASSERT_TRUE(stat.rx_enable == 0);

    //flush	//CM6501 UNSUPPORTED
    ret = csi_usart_flush(NULL, USART_FLUSH_WRITE);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_flush(usart, 2);
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE |DRV_ERROR_PARAMETER));
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_UNSUPPORTED));

    //set_interrupt	//CM6501 UNSUPPORTED

    ret = csi_usart_set_interrupt(NULL, USART_INTR_WRITE, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    ret = csi_usart_set_interrupt(usart, 2, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE |DRV_ERROR_UNSUPPORTED));

    ret = csi_usart_set_interrupt(usart, USART_INTR_WRITE, -1);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE |DRV_ERROR_UNSUPPORTED));

    //putchar
    ret = csi_usart_putchar(NULL, 0x6e);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    uint8_t value;
    //getchar
    printf("getchar test\n");
    ret = csi_usart_getchar(NULL, &value);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    printf("getchar test1\n");
    ret = csi_usart_getchar(usart, NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    //get_tx_count
    ret = csi_usart_get_tx_count(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    //get_rx_count
    ret = csi_usart_get_rx_count(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    //control_tx
    printf("control test\n");
    ret = csi_usart_control_tx(NULL, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));

    //control_rx
    ret = csi_usart_control_rx(NULL, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_USART_BASE | DRV_ERROR_PARAMETER));
}

/*
 *	test send and abort send function
 */
void usart_func_test()
{
    int32_t ret;
    usart_handle_t usart;
    uint8_t reply;

    /*
    	uint8_t msg[1024100] = {0};

    	for (i=0;i<1024100;i++){
    		msg[i] = (uint8_t)rand();
    	}
    */

    csi_pin_set_mux(UART_TX, UART_TX_FUNC);
    csi_pin_set_mux(UART_RX, UART_RX_FUNC);

    usart = csi_usart_initialize(UART_IDX, usart_event_cb_fun);
    /* config the UART */
    ret = csi_usart_config(usart, 2400, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);
    ASSERT_TRUE(ret == 0);

    printf("Check the receiving terminal:\n Baudrate --- 2400,\n  Parity --- NONE,\n  Wordsize --- 8.\n   Stopbit --- 1.\n");
    printf("Ready? --- please input [y/n]\n");

    while (1) {
        ret = csi_usart_getchar(usart, &reply);
        ASSERT_TRUE(ret == 0);

        if (reply != 'y') {
            printf("Ready? --- please input [y/n]\n");
        } else {
            ret = csi_usart_putchar(usart, reply);
            ASSERT_TRUE(ret == 0);
            reply = '\n';
            csi_usart_putchar(usart, reply);
            break;
        }
    }

    usart_send_cb_flag = 1;
    ret = csi_usart_send(usart, msg, strlen(msg));
    ASSERT_TRUE(ret == 0);

    /*
        usart_status_t status;
        while (1) {
            status = csi_usart_get_status(usart);

            if (!status.tx_busy) {
                break;
            }
        }
    */
    while (usart_send_cb_flag) {
        ;
    }

    ASSERT_TRUE(csi_usart_get_tx_count(usart) == strlen(msg));

    printf("testing usart abort sender,when test is over that the tranmission will not complete.\n");

    csi_usart_send(usart, msg, strlen(msg));

//    for(i=0;i<1;i++) ;
    ret = csi_usart_abort_send(usart);
    ASSERT_TRUE(ret == 0);
    printf("end send\n");

    printf("\n");
    ret = csi_usart_uninitialize(usart);
    ASSERT_TRUE(ret == 0);

    csi_usart_uninitialize(usart);
    printf("usart function test over.\n");
}

/*
 *	test receive function
 */
void usart_func1_test()
{
    int32_t ret;
    usart_handle_t usart;
    uint8_t reply;
    uint32_t num;

    csi_pin_set_mux(UART_TX, UART_TX_FUNC);
    csi_pin_set_mux(UART_RX, UART_RX_FUNC);

    usart = csi_usart_initialize(UART_IDX, usart_event_cb_fun);
    /* config the UART */
    ret = csi_usart_config(usart, 2400, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);
    ASSERT_TRUE(ret == 0);

    printf("Check the receiving terminal:\n Baudrate --- 2400,\n  Parity --- NONE,\n  Wordsize --- 8.\n   Stopbit --- 1.\n");
    printf("Ready? --- please input [y/n]\n");

    while (1) {
        ret = csi_usart_getchar(usart, &reply);
        ASSERT_TRUE(ret == 0);

        if (reply != 'y') {
            printf("Ready? --- please input [y/n]\n");
        } else {
            ret = csi_usart_putchar(usart, reply);
            ASSERT_TRUE(ret == 0);
            reply = '\n';
            csi_usart_putchar(usart, reply);
            break;
        }
    }

    printf("begin receive data,please input 50 byte data on test usart\n");
    usart_recv_cb_flag = 1;
    ret = csi_usart_receive(usart, &msg_func1[0], 50);
    ASSERT_TRUE(ret == 0);

    while (usart_recv_cb_flag);

    num = csi_usart_get_rx_count(usart);
    ASSERT_TRUE(num == 50);

    usart_send_cb_flag = 1;
    csi_usart_send(usart, (const char *)&msg_func1[0], 50);

    while (usart_send_cb_flag);

    num = csi_usart_get_tx_count(usart);
    ASSERT_TRUE(num == 50);
    reply = '\n';
    csi_usart_putchar(usart, reply);


    ret = csi_usart_uninitialize(usart);
    ASSERT_TRUE(ret == 0);
}

/*
 *	test receive_query
 */
void usart_func2_test()
{
    int32_t ret;
    usart_handle_t usart;
    uint8_t reply;
    uint32_t num;

    csi_pin_set_mux(UART_TX, UART_TX_FUNC);
    csi_pin_set_mux(UART_RX, UART_RX_FUNC);

    usart = csi_usart_initialize(UART_IDX, usart_event_cb_fun);
    /* config the UART */
    ret = csi_usart_config(usart, 2400, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);
    ASSERT_TRUE(ret == 0);

    printf("Check the receiving terminal:\n Baudrate --- 2400,\n  Parity --- NONE,\n  Wordsize --- 8.\n   Stopbit --- 1.\n");
    printf("Ready? --- please input [y/n]\n");

    while (1) {
        ret = csi_usart_getchar(usart, &reply);
        ASSERT_TRUE(ret == 0);

        if (reply != 'y') {
            printf("Ready? --- please input [y/n]\n");
        } else {
            ret = csi_usart_putchar(usart, reply);
            ASSERT_TRUE(ret == 0);
            reply = '\n';
            csi_usart_putchar(usart, reply);
            break;
        }
    }

    printf("begin receive data again,please input 50 byte data on test usart\n");
    mdelay(1000);	//这里延时5s的目的是为了向串口的fifo中发数据，注意发送的数据分小于fifo，大于fifo ，等于fifo
    ret = csi_usart_receive_query(usart, &msg_func1[50], FIFO_SIZE);
    //printf("*****%d\n",ret);
    ASSERT_TRUE(ret == FIFO_SIZE);
    usart_send_cb_flag = 1;
    msg_func1[99] = 0x41;
    csi_usart_send(usart, (const char *)&msg_func1[50], FIFO_SIZE);

    while (usart_send_cb_flag);

    num = csi_usart_get_tx_count(usart);
    printf("*****%d\n", num);

    ret = csi_usart_uninitialize(usart);
    ASSERT_TRUE(ret == 0);
}

/*
 *	test receive abort function
 */
#if 0
void usart_func3_test()
{
    int32_t ret;
    usart_handle_t usart, usart1;
    uint8_t reply;
    uint32_t num;

    csi_pin_set_mux(UART_TX, UART_TX_FUNC);
    csi_pin_set_mux(UART_RX, UART_RX_FUNC);
    csi_pin_set_mux(UART1_TX, UART1_TX_FUNC);
    csi_pin_set_mux(UART1_RX, UART1_RX_FUNC);

    usart = csi_usart_initialize(UART_IDX, usart_event_cb_fun);
    usart1 = csi_usart_initialize(UART1_IDX, usart_event_cb_fun);
    /* config the UART */
    ret = csi_usart_config(usart, 2400, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);
    ASSERT_TRUE(ret == 0);
    ret = csi_usart_config(usart1, 2400, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);
    ASSERT_TRUE(ret == 0);

    printf("usart0 send data,usart1 receive data\n");
    printf("usart0 send 2K byte data\n");
    printf("usart1 begin receive data,then abort it\n");
    //mdelay(5000);
    msg_func1[0] = 0x41;
    msg_func1[2047] = 0x41;
    msg_func1[4096] = '\0';
    ret = csi_usart_send(usart, (const char *)&msg_func1[0], 2048);
    ASSERT_TRUE(ret == 0);
    ret = csi_usart_receive(usart1, &msg_func1[2048], 2048);
    ASSERT_TRUE(ret == 0);
    ret = csi_usart_abort_receive(usart);
    ASSERT_TRUE(ret == 0);
    mdelay(10000);
    num = csi_usart_get_rx_count(usart);
    printf("receive %d byte\n", num);

    printf("%s", &msg_func1[2048]);
    ret = csi_usart_uninitialize(usart);
    ASSERT_TRUE(ret == 0);
}

/*void usart_dma_test() {
    usart_handle_t usart;

    char data[DATA_LENGTH], out[DATA_LENGTH];
    usart = csi_usart_initialize(0,usart_event_cb_fun);
    ret = csi_usart_config(usart, 115200, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);

    memset(data, 0x3f, DATA_LENGTH);
    memset(out, 0x0, DATA_LENGTH);

    //dma send , then abort
    ret = csi_usart_send_dma(usart, data, DATA_LENGTH);
    ASSERT_TRUE(ret == 0);
    csi_usart_abort_send(usart);

    ret = csi_usart_receive_dma(usart, out, DATA_LENGTH);
    ASSERT_TRUE(ret == 0);
    while(usart_recv_cb_flag);
    ASSERT_TRUE(memcmp(data,out, DATA_LENGTH) != 0);

    //dma receive , then abort
    ret = csi_usart_send_dma(usart, data, DATA_LENGTH);
    ASSERT_TRUE(ret == 0);
    while(usart_send_cb_flag);

    ret = csi_usart_receive_dma(usart, out, DATA_LENGTH);
    ASSERT_TRUE(ret == 0);
    csi_usart_abort_send(usart);
    ASSERT_TRUE(memcmp(data,out,DATA_LENGTH) != 0);

    //dma transfer, then abort
    ret = csi_usart_transfer_dma(usart, out, data, DATA_LENGTH);
    csi_usart_abort_transfer(usart);
    ASSERT_TRUE(memcmp(data,out,DATA_LENGTH) != 0);

    usart_send_cb_flag = 1;
    usart_recv_cb_flag = 1;
    //dma send , dma receive
    ret = csi_usart_send_dma(usart, data , DATA_LENGTH);
    ASSERT_TRUE(ret == 0);
    while(usart_send_cb_flag);

    ret = csi_usart_receive_dma(usart, out, DATA_LENGTH);
    ASSERT_TRUE(ret == 0);
    while(usart_recv_cb_flag);

    ASSERT_TRUE(memcmp(data, out, DATA_LENGTH) == 0);

    usart_send_cb_flag = 1;
    usart_recv_cb_flag = 1;

    //dma send, no dma receive
    ret = csi_usart_send_dma(usart, data , DATA_LENGTH);
    ASSERT_TRUE(ret == 0);
    while(usart_send_cb_flag);

    ret = csi_usart_receive(usart, out, DATA_LENGTH);
    ASSERT_TRUE(ret == 0);
    while(usart_recv_cb_flag);

    ASSERT_TRUE(memcmp(data, out, DATA_LENGTH) != 0);

    usart_send_cb_flag = 1;
    usart_recv_cb_flag = 1;

    //no dma send, dma receive
    ret = csi_usart_send(usart, data , DATA_LENGTH);
    ASSERT_TRUE(ret == 0);
    while(usart_send_cb_flag);

    ret = csi_usart_receive_dma(usart, out, DATA_LENGTH);
    ASSERT_TRUE(ret == 0);
    while(usart_recv_cb_flag);

    ASSERT_TRUE(memcmp(data, out, DATA_LENGTH) == 0);

    usart_send_cb_flag = 1;
    usart_recv_cb_flag = 1;

    //Mixing
    int i;

    for (i=0;i<1000;i++) {
        if(i%2 == 0) {
            ret = csi_usart_send(usart, data , DATA_LENGTH);
            ASSERT_TRUE(ret == 0);
            while(usart_send_cb_flag);

            ret = csi_usart_receive_dma(usart, out, DATA_LENGTH);
            ASSERT_TRUE(ret == 0);
            while(usart_recv_cb_flag);

        }
        else {
            ret = csi_usart_send_dma(usart, data , DATA_LENGTH);
            ASSERT_TRUE(ret == 0);
            while(usart_send_cb_flag);

            ret = csi_usart_receive(usart, out, DATA_LENGTH);
            ASSERT_TRUE(ret == 0);
            while(usart_recv_cb_flag);
        }

        usart_send_cb_flag = 1;
        usart_recv_cb_flag = 1;
    }
}*/
#endif
void usart_perf_test()
{
    usart_send_cb_flag = 1;
    usart_recv_cb_flag = 1;

    usart_handle_t usart = csi_usart_initialize(UART_IDX, usart_event_cb_fun);
    timer_handle_t timer = csi_timer_initialize(0, NULL);

    uint32_t start, end;
    csi_timer_start(timer);
    int i, length;
    volatile int ret;

    /*length=16;

    char data[length];
    char out[length];

    memset(out, 0 , length);
    memset(data, 0x7f, length);

    csi_usart_config(usart, 115200, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);
    csi_timer_get_current_value(timer, &start);
    for(i=0;i<1024;i++) {
        if(i%2==0)
            length=16;
        else
            length=15;

        csi_usart_receive(usart, out, length);
        csi_usart_send(usart, data, length);
        while(usart_send_cb_flag);
        while(usart_recv_cb_flag);

        ret = memcmp(data, out, length);
        if(ret !=0) {
            printf("Test failed.\n");
            return ;
        }
        usart_send_cb_flag = 1;
        usart_recv_cb_flag = 1;
        memset(out, 0 , length);
    }
    */

    char data[DATA_LENGTH];
    char out[DATA_LENGTH];

    memset(out, 0 , DATA_LENGTH);
    memset(data, 0x7f, DATA_LENGTH);

    csi_usart_config(usart, 57600, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);
    csi_timer_get_current_value(timer, &start);

    for (i = 0; i < 1024; i++) {
        csi_usart_receive(usart, out, DATA_LENGTH);
        csi_usart_send(usart, data, DATA_LENGTH);

        while (usart_send_cb_flag);

        while (usart_recv_cb_flag);

        ret = memcmp(data, out, DATA_LENGTH);

        if (ret != 0) {
            printf("Test failed.\n");
            return ;
        }

        usart_send_cb_flag = 1;
        usart_recv_cb_flag = 1;
        memset(out, 0 , DATA_LENGTH);
        //csi_usart_abort_receive(usart);
    }

    csi_timer_get_current_value(timer, &end);

    printf("start is %u, end is %u\n", start, end);
}

/*
 * 串口接回环，测试csi_usart_set_interrupt()的功能
 */
#if 0
void usart_func4_test()
{
    int32_t ret;
    usart_handle_t usart;
    usart_status_t status;
    uint8_t lock;

    csi_pin_set_mux(UART_TX, UART_TX_FUNC);
    csi_pin_set_mux(UART_RX, UART_RX_FUNC);

    usart = csi_usart_initialize(UART_IDX, usart_event_cb_fun);

    ret = csi_usart_config(usart, 38400, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);
    ASSERT_TRUE(ret == 0);

    printf("turn off uart read and write irq\n");

    usart_send_cb_flag = 1;
    usart_recv_cb_flag = 1;

    ret = csi_usart_receive(usart, &msg_func1[0], strlen(msg));
    ASSERT_TRUE(ret == 0);
    ret = csi_usart_send(usart, msg, strlen(msg));
    ASSERT_TRUE(ret == 0);

    lock = 1;

    while (lock) {
        status = csi_usart_get_status(usart);
    }
}
#endif

int test_usart(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_usart", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"usart_if_test", usart_if_test, 1},
#if 0
        {"usart_func_test", usart_func_test, 1},
        {"usart_func1_test", usart_func1_test, 1 },
        {"usart_func2_test", usart_func2_test, 1},
        {"usart_func3_test", usart_func3_test, 1},
        {"usart_perf_test", usart_perf_test, 1},
        {"usart_func_test2", usart_func_test2, 1},
#endif
        {NULL, NULL, NULL}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;

}

int main(void)
{
    dtest_register_cb(test_usart);
    dtest();

    return 0;
}
