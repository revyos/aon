/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     devices.c
 * @brief    source file for the devices
 * @version  V1.0
 * @date     2019-12-18
******************************************************************************/

#include <stdio.h>
#include <csi_config.h>
#include <soc.h>
#include <drv/uart.h>
#include <drv/timer.h>
#include <drv/dma.h>
#include <drv/iic.h>
#include <drv/gpio.h>
#include <drv/irq.h>
#include <drv/pin.h>

const csi_perip_info_t g_soc_info[] = {
    {DW_UART0_BASE,            DW_UART0_IRQn,            0,    DEV_DW_UART_TAG},
    {DW_UART1_BASE,            DW_UART1_IRQn,            1,    DEV_DW_UART_TAG},
    {DW_UART2_BASE,            DW_UART2_IRQn,            2,    DEV_DW_UART_TAG},
    {DW_UART3_BASE,            DW_UART3_IRQn,            3,    DEV_DW_UART_TAG},
    {DW_UART4_BASE,            DW_UART4_IRQn,            4,    DEV_DW_UART_TAG},
    {DW_UART5_BASE,            DW_UART5_IRQn,            5,    DEV_DW_UART_TAG},
    {DW_DMA_BASE,              DW_DMA0_IRQn,             0,    DEV_DW_DMA_TAG},
    {DW_TIMER0_BASE,           DW_TIMER0_IRQn,           0,    DEV_DW_TIMER_TAG},
    {DW_TIMER1_BASE,           DW_TIMER1_IRQn,           1,    DEV_DW_TIMER_TAG},
    {DW_TIMER2_BASE,           DW_TIMER2_IRQn,           2,    DEV_DW_TIMER_TAG},
    {DW_TIMER3_BASE,           DW_TIMER3_IRQn,           3,    DEV_DW_TIMER_TAG},
    {DW_GPIO0_BASE,            DW_GPIO0_IRQn,            0,    DEV_DW_GPIO_TAG},
    {DW_GPIO1_BASE,            DW_GPIO1_IRQn,            0,    DEV_DW_GPIO_TAG},
    {DW_GPIO2_BASE,            DW_GPIO2_IRQn,            0,    DEV_DW_GPIO_TAG},
    {DW_WDT_BASE,              DW_WDT0_IRQn,             0,    DEV_DW_WDT_TAG},
    {DW_SPI0_BASE,             DW_SPI0_IRQn,             0,    DEV_DW_SPI_TAG},
    {DW_QSPI0_BASE,            DW_QSPI0_IRQn,            0,    DEV_DW_QSPI_TAG},
    {DW_QSPI1_BASE,            DW_QSPI1_IRQn,            1,    DEV_DW_QSPI_TAG},
    {DW_I2C0_BASE,             DW_I2C0_IRQn,             0,    DEV_DW_IIC_TAG},
    {WJ_MBOX_BASE,             WJ_MBOX_IRQn,             0,    DEV_WJ_MBOX_TAG},
    {WJ_MBOX1_BASE,            WJ_MBOX_IRQn,             1,    DEV_WJ_MBOX_TAG},
    {DW_EMMC_BASE,             DW_EMMC_IRQn,             0,    DEV_DW_SDMMC_TAG},
    {DW_SD_BASE,               DW_SD_IRQn,               1,    DEV_DW_SDMMC_TAG},
    {RB_EIP28_BASE,            RB_150B_PKA1_IRQn,        0,    DEV_RAMBUS_150B_PKA_TAG},
    {RB_EIP150B_BASE,          RB_150B_TRNG_IRQn,        0,    DEV_RAMBUS_150B_TRNG_TAG},
    {RB_EIP120SI_BASE,         RB_120SI_AV_IRQn,         0,    DEV_RAMBUS_120SI_TAG},
    {RB_EIP120SII_BASE,        RB_120SII_AV_IRQn,        0,    DEV_RAMBUS_120SII_TAG},
    {RB_EIP120SIII_BASE,       RB_120SIII_AV_IRQn,       0,    DEV_RAMBUS_120SIII_TAG},
    {0, 0, 0, 0}
};

const uint8_t g_dma_chnum[] = {4};

/* DMA handshake number */
/* The member of uart_tx_hs_num is the handshake number for ETB */
const uint16_t uart_tx_hs_num[] = {19};
const uint16_t uart_rx_hs_num[] = {18};
const uint16_t iic_tx_hs_num[4]  = {21, 23};
const uint16_t iic_rx_hs_num[4]  = {20, 22};
const uint16_t i2s_tx_hs_num[]  = {9, 11, 13};
const uint16_t i2s_rx_hs_num[]  = {8, 10, 12};
const uint16_t qspi_tx_hs_num[2]  = {24,26};
const uint16_t qspi_rx_hs_num[2]  = {25,27};

const csi_dma_ch_desc_t uart0_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2},  {0, 3},  
    {0xff, 0xff}
};

const csi_dma_ch_desc_t iic0_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2},  {0, 3},  
    {0xff, 0xff}
};

const csi_dma_ch_desc_t iic1_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2},  {0, 3},  
    {0xff, 0xff}
};

const csi_dma_ch_desc_t i2s0_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2},  {0, 3},  
    {0xff, 0xff}
};

const csi_dma_ch_desc_t i2s1_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2},  {0, 3},  
    {0xff, 0xff}
};

const csi_dma_ch_desc_t i2s2_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2},  {0, 3},  
    {0xff, 0xff}
};

const csi_dma_ch_desc_t qspi0_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2},  {0, 3}, 
    {0xff, 0xff}
};

const csi_dma_ch_desc_t qspi1_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2},  {0, 3}, 
    {0xff, 0xff}
};
const csi_dma_ch_spt_list_t dma_spt_list[] = {
    {DEV_DW_UART_TAG, 0, uart0_dma_ch_list},
    {DEV_DW_IIC_TAG,  0, iic0_dma_ch_list},
    {DEV_DW_IIC_TAG,  1, iic1_dma_ch_list},
    {DEV_WJ_I2S_TAG,  0, i2s0_dma_ch_list},
    {DEV_WJ_I2S_TAG,  1, i2s1_dma_ch_list},
    {DEV_WJ_I2S_TAG,  2, i2s2_dma_ch_list},
    {DEV_DW_QSPI_TAG, 0, qspi0_dma_ch_list},
    {DEV_DW_QSPI_TAG, 1, qspi1_dma_ch_list},
    {0xFFFFU,         0xFFU,         NULL},
};

const csi_pinmap_t gpio_pinmap[] = {
    {PA0,           0,     0,           PIN_FUNC_GPIO },
    {PA1,           0,     1,           PIN_FUNC_GPIO },
    {PA2,           0,     2,           PIN_FUNC_GPIO },
    {PA3,           0,     3,           PIN_FUNC_GPIO },
    {PA4,           0,     4,           PIN_FUNC_GPIO },
    {PA5,           0,     5,           PIN_FUNC_GPIO },
    {PA6,           0,     6,           PIN_FUNC_GPIO },
    {PA7,           0,     7,           PIN_FUNC_GPIO },
    {PA8,           0,     8,           PIN_FUNC_GPIO },
    {PA9,           0,     9,           PIN_FUNC_GPIO },
    {PA10,          0,    10,           PIN_FUNC_GPIO },
    {PA11,          0,    11,           PIN_FUNC_GPIO },
    {PA12,          0,    12,           PIN_FUNC_GPIO },
    {PA13,          0,    13,           PIN_FUNC_GPIO },
    {PA14,          0,    14,           PIN_FUNC_GPIO },
    {PA15,          0,    15,           PIN_FUNC_GPIO },
    {PA16,          0,    16,           PIN_FUNC_GPIO },
    {PA17,          0,    17,           PIN_FUNC_GPIO },
    {PA18,          0,    18,           PIN_FUNC_GPIO },
    {PA19,          0,    19,           PIN_FUNC_GPIO },
    {PA20,          0,    20,           PIN_FUNC_GPIO },
    {PA21,          0,    21,           PIN_FUNC_GPIO },
    {PA22,          0,    22,           PIN_FUNC_GPIO },
    {PA23,          0,    23,           PIN_FUNC_GPIO },
    {PA24,          0,    24,           PIN_FUNC_GPIO },
    {PA25,          0,    25,           PIN_FUNC_GPIO },
    {PA26,          0,    26,           PIN_FUNC_GPIO },
    {PA27,          0,    27,           PIN_FUNC_GPIO },
    {PA28,          0,    28,           PIN_FUNC_GPIO },
    {PA29,          0,    29,           PIN_FUNC_GPIO },
    {PA30,          0,    30,           PIN_FUNC_GPIO },
    {PA31,          0,    31,           PIN_FUNC_GPIO },
    {0xFFFFFFFFU,   0xFFU, 0xFFU,       0xFFFFFFFFU   },
};

const csi_pinmap_t uart_pinmap[] = {
    {PA23,            0,     PIN_UART_RX,    PA29_UART0_RXD  },
    {PA24,            0,     PIN_UART_TX,    PA30_UART0_TXD  },
    {0xFFFFFFFFU, 0xFFU, 0xFFU,      0xFFFFFFFFU  },
};

const csi_pinmap_t iic_pinmap[] = {

    {0xFFFFFFFFU, 0xFFU,       0xFFU,   0xFFFFFFFFU },
};

const csi_pinmap_t i2s_pinmap[] = {

    {0xFFFFFFFFU, 0xFFU, 0xFFU, 0xFFFFFFFFU  },
};
