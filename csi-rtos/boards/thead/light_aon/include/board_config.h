/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     board_config.h
 * @brief    header File for pin definition
 * @version  V1.0
 * @date     02. June 2018
 ******************************************************************************/

#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <stdint.h>
#include "soc.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define CONFIG_BMU_ENABLE
#define CONFIG_AUDIO_ENABLE 1

#define CLOCK_GETTIME_USE_TIMER_ID

#define CONFIG_AON_UART              1
#define CONFIG_AP_UART1              2

#define CONFIG_UART_PORT             CONFIG_AON_UART

#ifdef CONFIG_UART_PORT

#if (CONFIG_UART_PORT == CONFIG_AON_UART )
#define CONSOLE_IDX                     0
#define CONSOLE_TXD                     AOGPIO_8
#define CONSOLE_RXD                     AOGPIO_9
#define CONSOLE_TXD_FUNC                0
#define CONSOLE_RXD_FUNC                0
#elif (CONFIG_UART_PORT  == CONFIG_AP_UART1 )
#define CONSOLE_IDX                     1
#define CONSOLE_TXD                     UART1_TXD
#define CONSOLE_RXD                     UART1_RXD
#define CONSOLE_TXD_FUNC                0
#define CONSOLE_RXD_FUNC                0
#endif

#else

#error "please check CONFIG_UART_PORT define"

#endif

#define EXAMPLE_UART_IDX                0
#define EXAMPLE_PIN_UART_TX             AOGPIO_8
#define EXAMPLE_PIN_UART_RX             AOGPIO_9
#define EXAMPLE_PIN_UART_TX_FUNC        0
#define EXAMPLE_PIN_UART_RX_FUNC        0

#define EXAMPLE_IIC_IDX                 0
#define EXAMPLE_PIN_IIC_SDA             I2C_AON_SDA
#define EXAMPLE_PIN_IIC_SCL             I2C_AON_SCL
#define EXAMPLE_PIN_IIC_SDA_FUNC        0
#define EXAMPLE_PIN_IIC_SCL_FUNC        0

#define EXAMPLE_TOGGLE_GPIO_PIN         AOGPIO_12
#define EXAMPLE_TOGGLE_GPIO_PIN1        AOGPIO_13
#define EXAMPLE_TOGGLE_GPIO_PIN_FUNC    0
#define EXAMPLE_TOGGLE_GPIO_PIN1_FUNC   0
#define EXAMPLE_TOGGLE_GPIO_IDX         0
#define EXAMPLE_TOGGLE_GPIO_PIN_MASK    ( ( 1 << 12  ) | ( 1 << 13   )  )

#define EXAMPLE_INTR_GPIO_PIN           AOGPIO_12
#define EXAMPLE_INTR_GPIO_PIN1          AOGPIO_13
#define EXAMPLE_INTR_GPIO_PIN_FUNC      0
#define EXAMPLE_INTR_GPIO_PIN1_FUNC     0
#define EXAMPLE_INTR_GPIO_IDX           0
#define EXAMPLE_INTR_GPIO_PIN_NAME      "AOGPIO_12"
#define EXAMPLE_INTR_GPIO_PIN1_NAME     "AOGPIO_13"
#define EXAMPLE_INTR_GPIO_PIN_MASK      ( ( 1 << 12  ) | ( 1 << 13   )  )

#define EXAMPLE_MBOX_IDX                 1
#define EXAMPLE_MBOX_TARGET_CPU          0



#ifdef __cplusplus
}
#endif

#endif /* _BOARD_CONFIG_H_ */

