/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic_func4.c
 * @brief    test transfer size
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iic_test.h"
#include <string.h>
#include <soc.h>
#include <pin.h>

#define  CK_IIC_SLAVE_ADDR         0x50	//this address only adapt to hobbit1_2,and hobbit1_2 used IIC1
#define  EEPROM_PAGE_SIZE          32
#define  EEPROM_SIZE               8192
#define  EEPROM_SUB_ADDR_START     0x0000
#define  EEPROM_SUB_ADDR_END       0x1FFF

#define TEST_DATA_SIZE 8000
extern void mdelay(uint32_t ms);
static volatile uint8_t transfer_done_flag;

static uint8_t send_addr[TEST_DATA_SIZE + 2] = {0x0, 0x0, 0};

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

void iic_func4_test(void)
{
    iic_handle_t iic_handle;
    int32_t r_value;
    uint32_t num, i, j;

    const char *send_data1 = "abcdefghijklmnopqrstuvwxyz01234\0";
//    uint8_t send_addr[TEST_DATA_SIZE + 2] = {0x0,0x0,0};
    uint8_t receive_addr[EEPROM_PAGE_SIZE + 2] = {0x20, 0x0, 0};

    csi_coret_config(soc_get_sys_freq() / 100, CORET_IRQn);    //10ms
    csi_vic_enable_irq(CORET_IRQn);

    csi_pin_set_mux(TEST_IIC_SDA_PIN, TEST_SDA_PIN_FUNCTION);
    csi_pin_set_mux(TEST_IIC_SCL_PIN, TEST_SCL_PIN_FUNCTION);

    iic_handle = csi_iic_initialize(TEST_IIC_INDEX, iic_callback);
    ASSERT_TRUE(iic_handle != NULL);

#if LOW_POWER
    r_value = csi_iic_power_control(iic_handle, DRV_POWER_FULL);
    ASSERT_TRUE(r_value == 0);
#endif

    /* config the iic */
    r_value = csi_iic_config_mode(iic_handle, IIC_MODE_MASTER);
    ASSERT_TRUE(r_value == 0);

    r_value = csi_iic_config_speed(iic_handle, TEST_IIC_SPEED);
    ASSERT_TRUE(r_value == 0);

    r_value = csi_iic_config_addr_mode(iic_handle, IIC_ADDRESS_7BIT);
    ASSERT_TRUE(r_value == 0);

    r_value = csi_iic_config_slave_addr(iic_handle, CK_IIC_SLAVE_ADDR);
    ASSERT_TRUE(r_value == 0);

    /* iic writing as master*/
    j = 0;

    for (i = 0; i < EEPROM_PAGE_SIZE; i++) {
        send_addr[i + 2] = *(send_data1 + j);
        j++;

        if (j % 32 == 0) {
            j = 0;
        }
    }

    //EEPROM_SIZE/EEPROM_PAGE_SIZE
    transfer_done_flag = 1;
    r_value = csi_iic_master_send(iic_handle, CK_IIC_SLAVE_ADDR, send_addr, TEST_DATA_SIZE + 2, 0);
    ASSERT_TRUE(r_value == 0);

    while (transfer_done_flag) ;

    /* get IIC transferred data count */
    num = csi_iic_get_data_count(iic_handle);
    //printf("---%d\n",num);
    ASSERT_TRUE(num == (TEST_DATA_SIZE + 2));

    mdelay(5);

    /* iic reading as master */
    transfer_done_flag = 1;
    r_value = csi_iic_master_send(iic_handle, CK_IIC_SLAVE_ADDR, receive_addr, 2, 1);
    ASSERT_TRUE(r_value == 0);

    while (transfer_done_flag) ;

    transfer_done_flag = 1;
    r_value = csi_iic_master_receive(iic_handle, CK_IIC_SLAVE_ADDR, receive_addr + 2, 32, 0);
    ASSERT_TRUE(r_value == 0);

    while (transfer_done_flag) ;

    /* abort transmission */
    /* iic writing as master*/
    r_value = csi_iic_master_send(iic_handle, CK_IIC_SLAVE_ADDR, send_addr, EEPROM_PAGE_SIZE + 2, 1);
    ASSERT_TRUE(r_value == 0);

#if LOW_POWER
    r_value = csi_iic_power_control(iic_handle, DRV_POWER_OFF);
    ASSERT_TRUE(r_value == 0);
#endif

    r_value = csi_iic_uninitialize(iic_handle);
    ASSERT_TRUE(r_value == 0);

    printf("iic func4 test finished.\n");
}
