/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     uart_func.c
 * @brief    the head file
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "uart_test.h"
#include "stdio.h"
#include "soc.h"
#include <string.h>
#include <drv/timer.h>
#include "pin.h"

static usart_handle_t g_usart_handle;
volatile uint8_t usart_send_cb_flag, usart_recv_cb_flag, rece_query_flag;
static uint8_t msg_func1[4096] = {0x0};
static const char *msg = "a!\"#$%&'()*+,-.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~i0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\0";

static void usart_event_cb_fun(int32_t idx, usart_event_e event)
{
    //printf("USART%d CB event %d entered!\n", idx, event);
    //uint8_t g_data[32];
    if (event == USART_EVENT_SEND_COMPLETE) {
        usart_send_cb_flag = 0;
    } else if (event == USART_EVENT_RECEIVE_COMPLETE) {
        usart_recv_cb_flag = 0;
    } else if (event == USART_EVENT_RECEIVED) {
        //	csi_usart_receive_query(g_usart_handle, g_data, 32);
        rece_query_flag = 0;
    }
}

static void clear_buffer(uint8_t *buffer, uint32_t begin, uint32_t end)
{
    uint32_t i;

    for (i = begin; i < end; i++) {
        *(buffer + i) = 0x0;
    }

}

/*
 *	测试串口接回环时在中断模式下的发送与接收
 */

void usart_func_test2()
{
    int32_t ret;
    uint32_t num;
    usart_handle_t usart, usart1;
    int cmp;

    csi_pin_set_mux(UART_TX, UART_TX_FUNC);
    csi_pin_set_mux(UART_RX, UART_RX_FUNC);
//	csi_pin_set_mux(UART1_TX, UART1_TX_FUNC);
//	csi_pin_set_mux(UART1_RX, UART1_RX_FUNC);

//	test items
    /*
    		uint32_t baud;
    		usart_stop_bits_e stopbits;
    		usart_data_bits_e bits;
    */

    uint32_t baud_t[] = {2400, 115200, 9600, 38400};
    usart_stop_bits_e stopbits_t[] = {USART_STOP_BITS_1, USART_STOP_BITS_2};
    usart_data_bits_e bits_t[] = {USART_DATA_BITS_7, USART_DATA_BITS_8};

    usart = csi_usart_initialize(UART_IDX, usart_event_cb_fun);
//	usart1 = csi_usart_initialize(UART1_IDX,usart_event_cb_fun);
    g_usart_handle = usart;
    uint8_t i, j, k;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 2; j++)
            for (k = 0; k < 2; k++) {
                /* config the UART */
                /*	ret = csi_usart_config(usart, 115200, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_2, USART_DATA_BITS_7);
                	ASSERT_TRUE(ret == 0);
                	ret = csi_usart_config(usart1, 115200, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_2, USART_DATA_BITS_7);
                	ASSERT_TRUE(ret == 0);
                	*/
                printf("**********************\nbauds=%d,stop bits=%d,data bits=%d\n**********************\n", baud_t[i], stopbits_t[j], bits_t[k]);
                ret = csi_usart_config(usart, baud_t[i], USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, stopbits_t[j], bits_t[k]);
                ASSERT_TRUE(ret == 0);
//				ret = csi_usart_config(usart1, baud_t[i], USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, stopbits_t[j], bits_t[k]);
//				ASSERT_TRUE(ret == 0);

#if 0
                printf("test send and receive\n");
                clear_buffer(&msg_func1[0], 0, 4095);
                usart_send_cb_flag = 1;
                usart_recv_cb_flag = 1;
                ret = csi_usart_receive(usart, &msg_func1[0], strlen(msg));
                ASSERT_TRUE(ret == 0);
                ret = csi_usart_send(usart, msg, strlen(msg));
                ASSERT_TRUE(ret == 0);

                while (usart_send_cb_flag || usart_recv_cb_flag);

                num = csi_usart_get_tx_count(usart);
                ASSERT_TRUE(num == strlen(msg));
                num = 0;
                num = csi_usart_get_rx_count(usart);
                ASSERT_TRUE(num == strlen(msg));
                cmp = strncmp(msg, (const char *)&msg_func1[0], strlen(msg));
                ASSERT_TRUE(cmp == 0);
#endif

#if 0
                printf("test abort send\n");
                clear_buffer(&msg_func1[0], 0, 4095);
//				mdelay(1000);
//				csi_usart_receive_query(usart,&msg_func1[0], FIFO_SIZE);//清空上次没有接收的fifo

                ret = csi_usart_receive(usart, &msg_func1[0], strlen(msg));
                ASSERT_TRUE(ret == 0);
                ret = csi_usart_send(usart, msg, strlen(msg));
                ASSERT_TRUE(ret == 0);
                mdelay(50);
                ret = csi_usart_abort_send(usart);
                ASSERT_TRUE(ret == 0);
                num = csi_usart_get_tx_count(usart);
                ASSERT_TRUE(num != strlen(msg));
                mdelay(2);
                usart_status_t status;
                status = csi_usart_get_status(usart);
                ASSERT_TRUE(status.tx_busy == 0);
                status = csi_usart_get_status(usart);
                ASSERT_TRUE(status.rx_busy == 1);
                cmp = strncmp(msg, (const char *)&msg_func1[0], strlen(msg));
                ASSERT_TRUE(cmp != 0);
                ASSERT_TRUE(msg_func1[0] == 'a');
                ret = csi_usart_abort_receive(usart);
                ASSERT_TRUE(ret == 0);
                mdelay(2);
                status = csi_usart_get_status(usart);
                ASSERT_TRUE(status.rx_busy == 0);
                mdelay(1000);
                csi_usart_receive_query(usart, &msg_func1[0], FIFO_SIZE);
#endif

#if 0
                printf("test abort receive\n");
//				mdelay(1000);
//				csi_usart_receive_query(usart,&msg_func1[0], FIFO_SIZE);
                clear_buffer(&msg_func1[0], 0, 4095);

                ret = csi_usart_receive(usart, &msg_func1[0], strlen(msg));
                ASSERT_TRUE(ret == 0);
                usart_send_cb_flag = 1;
                ret = csi_usart_send(usart, msg, strlen(msg));
                ASSERT_TRUE(ret == 0);
                mdelay(50);
                ret = csi_usart_abort_receive(usart);
                ASSERT_TRUE(ret == 0);
                num = csi_usart_get_rx_count(usart);
                //printf("num:	%d\n",num);
                //printf("msg:	%c-%c-%c\n",msg_func1[0],msg_func1[1],msg_func1[2]);
                ASSERT_TRUE(num != strlen(msg));
                mdelay(2);
//				usart_status_t status;
                status = csi_usart_get_status(usart);
                ASSERT_TRUE(status.tx_busy == 1);
                status = csi_usart_get_status(usart);
                ASSERT_TRUE(status.rx_busy == 0);
                cmp = strncmp(msg, (const char *)&msg_func1[0], strlen(msg));
                ASSERT_TRUE(cmp != 0);
                ASSERT_TRUE(msg_func1[0] == 'a');
                ret = csi_usart_abort_send(usart);
                ASSERT_TRUE(ret == 0);
                mdelay(2);
                status = csi_usart_get_status(usart);
                ASSERT_TRUE(status.tx_busy == 0);
                mdelay(1000);
                csi_usart_receive_query(usart, &msg_func1[0], FIFO_SIZE);
#endif

#if 1
                printf("test receive_query\n");
                clear_buffer(&msg_func1[0], 0, 4095);
                usart_send_cb_flag = 1;
                rece_query_flag = 1;
                ret = csi_usart_send(usart, msg, FIFO_SIZE);
                ASSERT_TRUE(ret == 0);

                while (usart_send_cb_flag || rece_query_flag);

                mdelay(500);
                ret = csi_usart_receive_query(usart, &msg_func1[0], FIFO_SIZE);
                ASSERT_TRUE(ret == FIFO_SIZE);
                cmp = strncmp(msg, (const char *)&msg_func1[0], FIFO_SIZE);
                ASSERT_TRUE(cmp == 0);

                usart_send_cb_flag = 1;
                rece_query_flag = 1;
                ret = csi_usart_send(usart, msg, FIFO_SIZE - 2);
                ASSERT_TRUE(ret == 0);

                while (usart_send_cb_flag || rece_query_flag);

                mdelay(500);
                ret = csi_usart_receive_query(usart, &msg_func1[50], FIFO_SIZE - 2);
                ASSERT_TRUE(ret == (FIFO_SIZE - 2));
                cmp = strncmp(msg, (const char *)&msg_func1[50], FIFO_SIZE - 2);
                ASSERT_TRUE(cmp == 0);
#endif

#if 1
                printf("test putchar and getchar\n");
                uint8_t reply;

                usart_send_cb_flag = 1;
                ret = csi_usart_send(usart, msg, 1);
                ASSERT_TRUE(ret == 0);

                while (usart_send_cb_flag);

                ret = csi_usart_getchar(usart, &reply);
                ASSERT_TRUE(ret == 0);
                ASSERT_TRUE(reply == 'a');
                usart_recv_cb_flag = 1;
                ret = csi_usart_receive(usart, &msg_func1[100], 1);
                ASSERT_TRUE(ret == 0);
                ret = csi_usart_putchar(usart, 'y');
                ASSERT_TRUE(ret == 0);

                while (usart_recv_cb_flag);

                mdelay(10);
                ASSERT_TRUE(msg_func1[100] == 'y');
#endif
            }

    ret = csi_usart_uninitialize(usart);
    ASSERT_TRUE(ret == 0);
//	ret = csi_usart_uninitialize(usart1);
//	ASSERT_TRUE(ret == 0);
}
