/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic_test.h
 * @brief    the head file
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <drv/iic.h>
#include <stdio.h>
#include "pin.h"
#include "dtest.h"

extern void iic_capa_if_test(void);
extern void iic_init_if_test(void);
extern int8_t iic_uninit_if_test(void);
extern int8_t iic_master_send_if_test(void);
extern int8_t iic_master_receive_if_test(void);
extern int8_t iic_slave_send_if_test(void);
extern int8_t iic_slave_receive_if_test(void);
extern int8_t iic_other_if_test(void);
extern int8_t iic_config_if_test(void);
extern void iic_func1_test(void);
extern void iic_func2_test(void);
extern void iic_func3_test(void);
extern void iic_func4_test(void);

//SC5654
/*
#define TEST_IIC_INDEX 					1
#define TEST_IIC_SDA_PIN 				PC13
#define TEST_SDA_PIN_FUNCTION 			PC13_I2C2_SDA
#define TEST_IIC_SCL_PIN 				PC12
#define TEST_SCL_PIN_FUNCTION 			PC12_I2C2_SCL
*/

//CB2301
/*
#define TEST_IIC_INDEX                  0
#define TEST_IIC_SDA_PIN                PA30
#define TEST_SDA_PIN_FUNCTION           PA30_I2C0_SDA
#define TEST_IIC_SCL_PIN                PA31
#define TEST_SCL_PIN_FUNCTION           PA31_I2C0_SCL
*/

//CB2201
#define TEST_IIC_INDEX                  1
#define TEST_IIC_SDA_PIN                PC1
#define TEST_SDA_PIN_FUNCTION           PC1_I2C1_SDA
#define TEST_IIC_SCL_PIN                PC0
#define TEST_SCL_PIN_FUNCTION           PC0_I2C1_SCL

//Danica
/*
#define TEST_IIC_INDEX                  0
#define TEST_IIC_SDA_PIN                PA16
#define TEST_SDA_PIN_FUNCTION           PA16_I2C0_SDA
#define TEST_IIC_SCL_PIN                PA15
#define TEST_SCL_PIN_FUNCTION           PA15_I2C0_SCL
*/

/*
   typedef enum {
   		IIC_BUS_SPEED_STANDARD  = 0, ///< Standard Speed (100kHz)
   		IIC_BUS_SPEED_FAST      = 1, ///< Fast Speed     (400kHz)
   		IIC_BUS_SPEED_FAST_PLUS = 2, ///< Fast+ Speed    (  1MHz)
   		IIC_BUS_SPEED_HIGH      = 3  ///< High Speed     (3.4MHz)
   } iic_speed_e;
 */

#define TEST_IIC_SPEED IIC_BUS_SPEED_HIGH
//#define TEST_IIC_SPEED IIC_BUS_SPEED_FAST
//#define TEST_IIC_SPEED IIC_BUS_SPEED_STANDARD

#define LOW_POWER 0

extern void mdelay(uint32_t ms);
