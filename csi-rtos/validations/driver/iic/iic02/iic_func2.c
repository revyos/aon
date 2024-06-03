/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic_func2.c
 * @brief    iic func2 test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

/*  function of this testcase:(use inquire mode)
 *  1.testing the iic's reading and writing with EEPROM.
 *  2.testing the iic's status.
*/

#include "iic_test.h"
#include <string.h>
#include <soc.h>
#include <pin.h>

#define  CK_IIC_SLAVE_ADDR         0x50 //this address only adapt to hobbit1_2,and hobbit1_2 used IIC1
#define  EEPROM_PAGE_SIZE          32
#define  EEPROM_SIZE               8192
#define  EEPROM_SUB_ADDR_START     0x0000
#define  EEPROM_SUB_ADDR_END       0x1FFF

//extern void mdelay(uint32_t ms);

void iic_func2_test(void)
{
    iic_handle_t iic_handle;
    int32_t r_value;
    iic_status_t iic_status;

//    const char *send_data1 = " !\"#$%&'()*+,-./0123456789:;<=>\0";
    const char *send_data2 = "?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]\0";
    const char *send_data3 = "^_`abcdefghijklmnopqrstuvwxyz{|\0";
    uint8_t send_addr[34] = {0x0, 0x0, 0};
    uint8_t receive_data[EEPROM_PAGE_SIZE + 2] = {0x0, 0x0, 0};
    uint8_t i;

//    csi_coret_config(soc_get_sys_freq() / 100, CORET_IRQn);    //10ms
//    csi_vic_enable_irq(CORET_IRQn);

    csi_pin_set_mux(TEST_IIC_SDA_PIN, TEST_SDA_PIN_FUNCTION);
    csi_pin_set_mux(TEST_IIC_SCL_PIN, TEST_SCL_PIN_FUNCTION);

    iic_handle = csi_iic_initialize(TEST_IIC_INDEX, NULL);
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

//	send_addr[1] += 32;

    /* get iic's status */
    iic_status = csi_iic_get_status(iic_handle);
    ASSERT_TRUE(iic_status.busy == 0);
    //ASSERT_TRUE((iic_status.busy == 0) && (iic_status.mode == 1));

    /* iic writing as master,continuous writing 2 pages*/
    for (i = 0; i < 32; i++) {
        send_addr[i + 2] = *(send_data2 + i);
    }

    r_value = csi_iic_master_send(iic_handle, CK_IIC_SLAVE_ADDR, send_addr, 34, 0);
    ASSERT_TRUE(r_value == 0);

    do {
        ASSERT_TRUE(iic_status.direction == 0); //testing the iic's status
        iic_status = csi_iic_get_status(iic_handle);
    } while (iic_status.busy == 1);

    mdelay(5);

    send_addr[1] += 32;

    for (i = 0; i < 32; i++) {
        send_addr[i + 2] = *(send_data3 + i);
    }

    r_value = csi_iic_master_send(iic_handle, CK_IIC_SLAVE_ADDR, send_addr, 34, 0);
    ASSERT_TRUE(r_value == 0);

    do {
        ASSERT_TRUE(iic_status.direction == 0);
        iic_status = csi_iic_get_status(iic_handle);
    } while (iic_status.busy == 1);

    mdelay(5);
    /* iic reading as master,reading the first page */
    //receive_data[1] += 32;

    r_value = csi_iic_master_send(iic_handle, CK_IIC_SLAVE_ADDR, receive_data, 2, 0);
    ASSERT_TRUE(r_value == 0);

    do {
        ASSERT_TRUE(iic_status.direction == 0);  //testing the iic's status
        iic_status = csi_iic_get_status(iic_handle);
    } while (iic_status.busy == 1);

    mdelay(5);
    r_value = csi_iic_master_receive(iic_handle, CK_IIC_SLAVE_ADDR, receive_data + 2 , 32, 0);
    ASSERT_TRUE(r_value == 0);

    /* testing the iic's status. */
    uint8_t	lock = 1;

    while (lock) {
        iic_status = csi_iic_get_status(iic_handle);

        if (iic_status.busy == 1) {
            ASSERT_TRUE((iic_status.direction == 1) && (iic_status.mode == 1));
        } else {
            lock = 0;
        }
    }

//	mdelay(5);

    ASSERT_TRUE(strcmp(send_data2, (const char *)(receive_data + 2)) == 0);

#if LOW_POWER
    r_value = csi_iic_power_control(iic_handle, DRV_POWER_OFF);
    ASSERT_TRUE(r_value == 0);
#endif
    r_value = csi_iic_uninitialize(iic_handle);
    ASSERT_TRUE(r_value == 0);

    printf("iic func2 test finished.\n");
}
