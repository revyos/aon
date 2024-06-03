/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
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
#include <drv/common.h>
#include <drv/pin.h>
#include <drv/dma.h>

const csi_perip_info_t g_soc_info[] = {
    {DW_UART_AO_BASE,          DW_UART_AO_IRQn,          0,    DEV_DW_UART_TAG},
    {DW_UART1_BASE,            DW_UART1_IRQn,            1,    DEV_DW_UART_TAG},
    {DW_I2C0_BASE,             DW_I2C0_IRQn,             0,    DEV_DW_IIC_TAG},
    {DW_TIMER0_BASE,           DW_TIMER0_IRQn,           0,    DEV_DW_TIMER_TAG},
    {DW_TIMER1_BASE,           DW_TIMER1_IRQn,           1,    DEV_DW_TIMER_TAG},
    {DW_TIMER2_BASE,           DW_TIMER2_IRQn,           2,    DEV_DW_TIMER_TAG},
    {DW_TIMER3_BASE,           DW_TIMER3_IRQn,           3,    DEV_DW_TIMER_TAG},
    {DW_WDT1_BASE,             DW_WDT1_IRQn,             0,    DEV_DW_WDT_TAG},
    {DW_WDT2_BASE,             DW_WDT2_IRQn,             1,    DEV_DW_WDT_TAG},
    {DW_WDT3_BASE,             DW_WDT3_IRQn,             2,    DEV_DW_WDT_TAG},
    {DW_WDT0_BASE,             DW_WDT0_IRQn,             3,    DEV_DW_WDT_TAG},
    {DW_GPIO_BASE,             DW_GPIO_IRQn,             0,    DEV_DW_GPIO_TAG},
    {WJ_RTC_BASE,              WJ_RTC_IRQn,              0,    DEV_WJ_RTC_TAG},
    {WJ_MBOX1_BASE,            WJ_MBOX1_IRQn,             1,    DEV_WJ_MBOX_TAG},
    {MR_PVT_BASE,              MR_PVT_IRQn,              0,    DEV_MR_PVT_TAG},
    {0, 0, 0, 0}
};

const csi_perip_reg_t g_soc_reg[] = {
    {DW_EMMC_BASE,	DEV_WJ_EMMC_TAG,    0, 0, NULL},
    {DW_SDIO0_BASE,	DEV_WJ_SDIO_TAG,    0, 0, NULL},
    {DW_SDIO1_BASE,	DEV_WJ_SDIO_TAG,    0, 0, NULL},
    {DW_API2C0_BASE,	DEV_WJ_APIIC_TAG,   0, 0, NULL},
    {DW_API2C1_BASE,	DEV_WJ_APIIC_TAG,   1, 0, NULL},
    {DW_API2C2_BASE,	DEV_WJ_APIIC_TAG,   2, 0, NULL},
    {DW_API2C3_BASE,	DEV_WJ_APIIC_TAG,   3, 0, NULL},
    {DW_API2C4_BASE,	DEV_WJ_APIIC_TAG,   4, 0, NULL},
    {DW_API2C5_BASE,	DEV_WJ_APIIC_TAG,   5, 0, NULL},
    {DW_API2C0_BASE,	DEV_WJ_APIIC_TAG,   0, 0, NULL},
    {DW_API2C1_BASE,	DEV_WJ_APIIC_TAG,   1, 0, NULL},
    {DW_I2C0_BASE,	DEV_DW_IIC_TAG,	    2, 0, NULL},
    {DW_AUDI2C0_BASE,	DEV_WJ_AUDIIC_TAG,  0, 0, NULL},
    {DW_AUDI2C1_BASE,	DEV_WJ_AUDIIC_TAG,  1, 0, NULL},
    {DW_AUDI2C0_BASE,	DEV_WJ_AUDIIC_TAG,  0, 0, NULL},
    {DW_AUDI2C1_BASE,	DEV_WJ_AUDIIC_TAG,  1, 0, NULL},
    {DW_SPI0_BASE,	DEV_WJ_SPI_TAG,	    0, 0, NULL},
    {DW_QSPI1_BASE,	DEV_WJ_QSPI_TAG,    1, 0, NULL},
    {DW_VDEC_BASE,	DEV_WJ_VDEC_TAG,    0, 0, NULL},
    {DW_G2D_BASE,	DEV_WJ_G2D_TAG,	    0, 0, NULL},
    {DW_DSP0_APB_BASE,	DEV_WJ_DSP_TAG,	    0, 0, NULL},
    {DW_DSP1_APB_BASE,	DEV_WJ_DSP_TAG,	    1, 0, NULL},
    {DW_DDR_CTRL_BASE,	DEV_WJ_DDR_TAG,	    0, 0, NULL},
    {DW_GPU_BASE,	DEV_WJ_GPU_TAG,	    0, 0, NULL},
    {DW_DMAC_BASE,	DEV_WJ_DMAC_TAG,    0, 0, NULL},
    {DW_AUDDMAC_BASE,	DEV_WJ_DMAC_TAG,    1, 0, NULL},
    {0, 0, 0, 0, 0}
};

const csi_clkmap_t clk_map[] = {
    {RTC_PCLK,      DEV_WJ_RTC_TAG,       0},
    {AOGPIO_PCLK,   DEV_DW_GPIO_TAG,      0},
    {AOI2C_PCLK,    DEV_DW_IIC_TAG,       0},
    {AOTIMER_PCLK,  DEV_DW_TIMER_TAG,     0},
    {AOTIMER_PCLK,  DEV_DW_TIMER_TAG,     1},
    {AOTIMER_PCLK,  DEV_DW_TIMER_TAG,     2},
    {AOTIMER_PCLK,  DEV_DW_TIMER_TAG,     3},
    {0xFFFFFFFFU, 0xFFFFU,          0xFFU}
};
const uint8_t g_dma_chnum[] = {16, 0};

const uint16_t uart_tx_hs_num[4]  = {57};
const uint16_t uart_rx_hs_num[4]  = {56};
const uint16_t iic_tx_hs_num[4]   = {77, 79};
const uint16_t iic_rx_hs_num[4]   = {76, 78};
const uint16_t qspi_tx_hs_num[2]  = {65, 71};
const uint16_t qspi_rx_hs_num[2]  = {64, 70};
const csi_dma_ch_desc_t uart0_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2},  {0, 3},  {0, 4},  {0, 5},  {0, 6},  {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};

const csi_dma_ch_desc_t iic0_dma_ch_list[] = {
    {0, 0}, {0, 1}, {0, 2},  {0, 3},  {0, 4},  {0, 5},  {0, 6},  {0, 7},
    {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
    {0xff, 0xff}
};


const csi_dma_ch_spt_list_t dma_spt_list[] = {
    {DEV_DW_UART_TAG, 0, uart0_dma_ch_list},
    {DEV_DW_IIC_TAG,  0, iic0_dma_ch_list},
    {0xFFFFU,         0xFFU,         NULL},
};

const csi_pinmap_t gpio_pinmap[] = {
    {AOGPIO_12,   0x0, 12,  0x0U},
    {AOGPIO_11,   0x0, 11,  0x0U},
};

const csi_pinmap_t uart_pinmap[] = {
    {AOGPIO_9,            0,     PIN_UART_RX,    0  },
    {AOGPIO_8,            0,     PIN_UART_TX,    0  },
    {UART1_TXD,           1,     PIN_UART_TX,    0  },
    {UART1_RXD,           1,     PIN_UART_RX,    0  },
    {0xFFFFFFFFU, 0xFFU, 0xFFU,      0xFFFFFFFFU  },
};



const csi_pinmap_t iic_pinmap[] = {
    {I2C_AON_SCL,            0, PIN_IIC_SCL, 0 },
    {I2C_AON_SDA,            0, PIN_IIC_SDA, 0 },
    {0xFFFFFFFFU, 0xFFU,       0xFFU,   0xFFFFFFFFU},
};

csi_error_t add_reg_range(csi_dev_tag_t dev_tag, uint8_t idx, uint16_t start_addr, uint16_t end_addr)
{
	csi_error_t ret = CSI_OK;
	csi_perip_reg_t *info;
	reg_range_t *node;
	uint8_t i = 0;

	info = (csi_perip_reg_t *)&g_soc_reg;

	while(info->dev_tag)
	{
		if(info->dev_tag == (uint16_t)dev_tag && (info->idx == (uint8_t)idx))
			break;
		info++;
	}
	if(!info->reg_base)
			return CSI_UNSUPPORTED;

	info->range_list[info->reg_count].start_offset = start_addr;
	info->range_list[info->reg_count].end_offset = end_addr;
	info->reg_count++;

	return ret;
}

csi_error_t init_regmap()
{
	csi_error_t ret = CSI_OK;

	add_reg_range(DEV_WJ_EMMC_TAG, 0, 0x70, 0x70);
	add_reg_range(DEV_WJ_EMMC_TAG, 0, 0x80, 0x540);
	add_reg_range(DEV_WJ_SDIO_TAG, 0, 0x70, 0x70);
	add_reg_range(DEV_WJ_SDIO_TAG, 0, 0x74, 0x300);
	add_reg_range(DEV_WJ_SDIO_TAG, 0, 0x314, 0x314);
	add_reg_range(DEV_WJ_SDIO_TAG, 0, 0x334, 0x500);
	add_reg_range(DEV_WJ_SDIO_TAG, 0, 0x508, 0x540);
	add_reg_range(DEV_WJ_SDIO_TAG, 1, 0x70, 0x70);
	add_reg_range(DEV_WJ_SDIO_TAG, 1, 0x74, 0x300);
	add_reg_range(DEV_WJ_SDIO_TAG, 1, 0x314, 0x314);
	add_reg_range(DEV_WJ_SDIO_TAG, 1, 0x334, 0x500);
	add_reg_range(DEV_WJ_SDIO_TAG, 1, 0x508, 0x540);
	add_reg_range(DEV_WJ_SDIO_TAG, 1, 0x508, 0x540);
	add_reg_range(DEV_DW_IIC_TAG,  0, 0x10, 0x10);
	add_reg_range(DEV_DW_IIC_TAG,  1, 0x10, 0x10);
	add_reg_range(DEV_DW_IIC_TAG,  2, 0x10, 0x10);
	add_reg_range(DEV_DW_IIC_TAG,  3, 0x10, 0x10);
	add_reg_range(DEV_DW_IIC_TAG,  4, 0x10, 0x10);
	add_reg_range(DEV_DW_IIC_TAG,  5, 0x10, 0x10);
	add_reg_range(DEV_WJ_SPI_TAG,  0, 0x60, 0xf0);
	add_reg_range(DEV_WJ_QSPI_TAG,  1, 0x60, 0xf0);
	add_reg_range(DEV_WJ_VDEC_TAG,  0, 0x5000, 0x6000);
	add_reg_range(DEV_WJ_G2D_TAG,  0, 0x200, 0x400);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x8, 0xc);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x1c, 0x1c);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x28, 0x28);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x3c, 0x4c);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x58, 0x5c);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x6c, 0xcc);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0xfc, 0xfc);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x124, 0x12c);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x13c, 0x17c);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x19c, 0x19c);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x1ac, 0x1ac);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x1b8, 0x1b8);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x1c8, 0x1fc);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x220, 0x220);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x230, 0x23c);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x248, 0x24c);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x258, 0x258);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x260, 0x260);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x268, 0x268);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x278, 0x29c);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x314, 0x31c);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x32c, 0x368);
	add_reg_range(DEV_WJ_DDR_TAG,  0, 0x374, 0x3e8);
	add_reg_range(DEV_WJ_DMAC_TAG,  0, 0x38, 0x3c);
	add_reg_range(DEV_WJ_DMAC_TAG,  0, 0x64, 0xfc);
	add_reg_range(DEV_WJ_DMAC_TAG,  0, 0x144, 0x14c);
	add_reg_range(DEV_WJ_DMAC_TAG,  0, 0x194, 0x1fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  0, 0x244, 0x24c);
	add_reg_range(DEV_WJ_DMAC_TAG,  0, 0x294, 0x2fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  0, 0x344, 0x34c);
	add_reg_range(DEV_WJ_DMAC_TAG,  0, 0x394, 0x3fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  0, 0x444, 0x44c);
	add_reg_range(DEV_WJ_DMAC_TAG,  0, 0x494, 0x4fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x38, 0x3c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x64, 0xfc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x144, 0x14c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x194, 0x1fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x244, 0x24c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x294, 0x2fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x344, 0x34c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x394, 0x3fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x444, 0x44c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x494, 0x4fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x544, 0x54c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x594, 0x5fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x644, 0x64c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x694, 0x6fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x744, 0x74c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x794, 0x7fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x844, 0x84c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x894, 0x8fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x944, 0x94c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0x994, 0x9fc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0xa44, 0xa4c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0xa94, 0xafc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0xb44, 0xb4c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0xb94, 0xbfc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0xc44, 0xc4c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0xc94, 0xcfc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0xd44, 0xd4c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0xd94, 0xdfc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0xe44, 0xe4c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0xe94, 0xefc);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0xf44, 0xf4c);
	add_reg_range(DEV_WJ_DMAC_TAG,  1, 0xf94, 0xffc);
	return CSI_OK;
}
