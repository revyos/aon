/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic_speed.c
 * @brief    the iic speed test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "../iic02/iic_test.h"
#include <string.h>
#include <soc.h>
#include <pin.h>
#include <drv/timer.h>
#include <stdlib.h>
#include "timer_port.h"
#include "dtest.h"

#define  CK_IIC_SLAVE_ADDR         0x50 //this address only adapt to hobbit1_2,and hobbit1_2 used IIC1
#define  EEPROM_PAGE_SIZE          32
#define  EEPROM_SIZE               8192
#define  EEPROM_SUB_ADDR_START     0x0000
#define  EEPROM_SUB_ADDR_END       0x1FFF

#define DATA_LENGTH 32

extern void mdelay(uint32_t ms);

static volatile uint8_t transfer_done_flag;

static unsigned long long time_num = 0;

static void iic_callback(int32_t idx, iic_event_e event)
{
    switch (event) {
        case IIC_EVENT_TRANSFER_DONE:
            time_num = clock_gettime();
            transfer_done_flag = 0;
            break;

        default :
            break;
    }
}

void iic_speed_test(void)
{
    uint8_t send_data[DATA_LENGTH + 2] = {0x0, 0x0, 0};
    uint8_t receive_data[DATA_LENGTH + 2] = {0x0, 0x0, 0};
    unsigned long long start_s = 0, end_s = 0, start_r = 0, end_r = 0;
    int i, r_value;

    iic_handle_t iic_handle;

    csi_pin_set_mux(TEST_IIC_SDA_PIN, TEST_SDA_PIN_FUNCTION);
    csi_pin_set_mux(TEST_IIC_SCL_PIN, TEST_SCL_PIN_FUNCTION);

    iic_handle = csi_iic_initialize(TEST_IIC_INDEX, iic_callback);
    //init device
    Timer_Open();

#if LOW_POWER
    r_value = csi_iic_power_control(iic_handle, DRV_POWER_FULL);
    ASSERT_TRUE(r_value == 0);
#endif

    r_value = csi_iic_config_mode(iic_handle, IIC_MODE_MASTER);
    ASSERT_TRUE(r_value == 0);

    r_value = csi_iic_config_speed(iic_handle, TEST_IIC_SPEED);
    ASSERT_TRUE(r_value == 0);

    r_value = csi_iic_config_addr_mode(iic_handle, IIC_ADDRESS_7BIT);
    ASSERT_TRUE(r_value == 0);

    r_value = csi_iic_config_slave_addr(iic_handle, CK_IIC_SLAVE_ADDR);
    ASSERT_TRUE(r_value == 0);

    //init data
    for (i = 2; i < DATA_LENGTH; i++) {
        send_data[i] = rand();
    }

    transfer_done_flag = 1;
    Timer_Start();
    start_s = clock_gettime();
    r_value = csi_iic_master_send(iic_handle, CK_IIC_SLAVE_ADDR, send_data, DATA_LENGTH, 0);
    ASSERT_TRUE(r_value == 0);

    while (transfer_done_flag) {
        ;
    }

    end_s = time_num;

    time_num = 0;
    mdelay(5);

    transfer_done_flag = 1;
    r_value = csi_iic_master_send(iic_handle, CK_IIC_SLAVE_ADDR, receive_data, 2, 0);
    ASSERT_TRUE(r_value == 0);

    while (transfer_done_flag) {
        ;
    }

    transfer_done_flag = 1;
    start_r = clock_gettime();
    r_value = csi_iic_master_receive(iic_handle, CK_IIC_SLAVE_ADDR, receive_data + 2 , DATA_LENGTH, 0);
    ASSERT_TRUE(r_value == 0);

    while (transfer_done_flag) {
        ;
    }

    end_r = time_num;

    if (memcmp(send_data, receive_data, DATA_LENGTH) != 0) {
        printf("transmission error\n");
    }

    printf("stansmission size:  %d Bytes\nsend time: start is %lld us, end is %lld us\nreceive time:	start is %lld us, end is %lld us\n", DATA_LENGTH, start_s, end_s, start_r, end_r);
    printf("\nsend speed:	%d Bytes/s\n", DATA_LENGTH * 1000000 / (end_s - start_s));
    printf("receive speed:   %d Bytes/s\n", DATA_LENGTH * 1000000 / (end_r - start_r));

#if LOW_POWER
    r_value = csi_iic_power_control(iic_handle, DRV_POWER_OFF);
    ASSERT_TRUE(r_value == 0);
#endif

    r_value = csi_iic_uninitialize(iic_handle);
    ASSERT_TRUE(r_value == 0);

    Timer_Stop();
    Timer_Close();

    printf("\niic speed test finished.\n");

}

int test_iic_speed(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_iic_speed", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"iic_speed_test", iic_speed_test, 1},
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;

}

int main(void)
{
    dtest_register_cb(test_iic_speed);
    dtest();

    return 0;
}
