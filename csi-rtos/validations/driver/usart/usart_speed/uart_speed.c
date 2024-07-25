/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     uart_speed.c
 * @brief    uart speed test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "../usart02/uart_test.h"
#include "stdio.h"
#include "soc.h"
#include <string.h>
#include <stdlib.h>
#include <drv/timer.h>
#include "pin.h"
#include "dtest.h"

static volatile uint8_t usart_send_cb_flag;

static const char *msg = "a!\"#$%&'()*+,-.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~i0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\0";

static char receive_buffer[2048] = {0};
static unsigned long long time_num = 0;

static void usart_event_cb_fun(int32_t idx, usart_event_e event)
{
    //printf("USART%d CB event %d entered!\n", idx, event);
    //uint8_t g_data[32];
    if (event == USART_EVENT_SEND_COMPLETE) {
        usart_send_cb_flag = 0;
        time_num = clock_gettime();
    }

    /*    else if (event == USART_EVENT_RECEIVE_COMPLETE)
            usart_recv_cb_flag = 0;
        else if (event == USART_EVENT_RECEIVED) {
        //  csi_usart_receive_query(g_usart_handle, g_data, 32);
            rece_query_flag = 0;
        }
    */
}

void usart_speed_test()
{
    int32_t ret;
    uint32_t num;
    usart_handle_t usart;
    unsigned long long time_start = 0, time_end = 0;

    csi_pin_set_mux(UART_TX, UART_TX_FUNC);
    csi_pin_set_mux(UART_RX, UART_RX_FUNC);

    usart = csi_usart_initialize(UART_IDX, usart_event_cb_fun);
    //usart = csi_usart_initialize(UART_IDX,NULL);
    ASSERT_TRUE(usart != NULL);

    ret = csi_usart_config(usart, 400, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);
    ASSERT_TRUE(ret == 0);

    Timer_Open();
    Timer_Start();

    ret = csi_usart_receive(usart, &receive_buffer[0], strlen(msg));
    //ret = csi_usart_receive(usart,&receive_buffer[0],16);
    ASSERT_TRUE(ret == 0);

    usart_send_cb_flag = 1;
    time_start = clock_gettime();

    ret = csi_usart_send(usart, msg, strlen(msg));
    ASSERT_TRUE(ret == 0);
    /*
    	usart_status_t sta;
    	sta = csi_usart_get_status(usart);
    	while (sta.tx_busy) {
    		sta = csi_usart_get_status(usart);
    	}
    */

    while (usart_send_cb_flag);

    time_end = time_num;

    num = csi_usart_get_tx_count(usart);
    ASSERT_TRUE(num == strlen(msg));

    if (strncmp((const char *)&receive_buffer[0], msg, (strlen(msg) - 1)) != 0) {
        printf("uart transfer error!!!\n");
    }

    printf("transfer size:  %d Bytes\ntransfer time: start is %lld us, end is %lld us\n", strlen(msg) + 1, time_start, time_end);
    printf("\ntransfer speed:   %lld Bytes/s\n", (unsigned long long)(strlen(msg) + 1) * 1000000 / (time_end - time_start));

    ret = csi_usart_uninitialize(usart);
    ASSERT_TRUE(ret == 0);

    Timer_Stop();
    Timer_Close();
}

int test_usart_speed(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_usart_speed", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"usart_speed_test", usart_speed_test, 1},
        {NULL, NULL, NULL}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;

}

int main(void)
{
    dtest_register_cb(test_usart_speed);
    dtest();

    return 0;
}
