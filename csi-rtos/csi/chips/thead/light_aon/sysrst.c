/*
 * Copyright (C) 2021 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     sysrst.c
 * @brief    mpw aon low power
 * @version  V1.0
 * @date     3. June 2021
 ******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <soc.h>
#include "power.h"

void c910_reset(int cpu)
{
	uint32_t data = CHIP_REG_GET(REG_AP_C910_SWRST);
	uint32_t msk = 1 << (cpu + 1);

	data &= ~msk;
	LOG_D("reg = 0x%x, data = 0x%x\n",REG_AP_C910_SWRST, data);
	CHIP_REG_SET(data, REG_AP_C910_SWRST);
}

void c910_reset_release(int cpu)
{
	uint32_t data = CHIP_REG_GET(REG_AP_C910_SWRST);
	uint32_t msk = 1 << (cpu + 1);

	data |= msk;

	CHIP_REG_SET(data, REG_AP_C910_SWRST);
}

void audio_reset()
{
	uint32_t data = CHIP_REG_GET(REG_AON_AUDIO_RST_CFG);

	data &= ~SW_AUDIO_RST_n;
	CHIP_REG_SET(data, REG_AON_AUDIO_RST_CFG);
}

void audio_reset_release()
{
	uint32_t data = CHIP_REG_GET(REG_AON_AUDIO_RST_CFG);

	data |= SW_AUDIO_RST_n;
	CHIP_REG_SET(data, REG_AON_AUDIO_RST_CFG);
}

void audio_reset_all()
{
	uint32_t data = CHIP_REG_GET(REG_AON_AUDIO_RST_CFG);
	data &= ~0x3F;
	CHIP_REG_SET(data, REG_AON_AUDIO_RST_CFG);
}

void audio_reset_release_0()
{
	uint32_t data = CHIP_REG_GET(REG_AON_AUDIO_RST_CFG);
	data |= 0x37;
	CHIP_REG_SET(data, REG_AON_AUDIO_RST_CFG);
}

void audio_reset_release_1()
{
	uint32_t data = CHIP_REG_GET(REG_AON_AUDIO_RST_CFG);
	data |= 0x08;
	CHIP_REG_SET(data, REG_AON_AUDIO_RST_CFG);
}

void vdec_reset()
{
	uint32_t data = CHIP_REG_GET(REG_AON_VDEC_RST_CFG);

	data &= ~SW_AON_VDEC_RST_n;
	CHIP_REG_SET(data, REG_AON_VDEC_RST_CFG);
}

void vdec_reset_release()
{
	uint32_t data = CHIP_REG_GET(REG_AON_VDEC_RST_CFG);

	data |= SW_AON_VDEC_RST_n;
	CHIP_REG_SET(data, REG_AON_VDEC_RST_CFG);
}

void npu_reset()
{
	uint32_t data = CHIP_REG_GET(REG_AON_NPUSYS_RST_CFG);

	data &= ~SW_AON_NPUSYS_RST_n;
	CHIP_REG_SET(data, REG_AON_NPUSYS_RST_CFG);

	data = CHIP_REG_GET(REG_AP_IMG_NNA_TEESWRST);
	data &= ~SW_AP_SW_IMG_NNA_RST_n;
	CHIP_REG_SET(data, REG_AP_IMG_NNA_TEESWRST);
}

void npu_reset_release()
{
	uint32_t data = CHIP_REG_GET(REG_AON_NPUSYS_RST_CFG);

	data |= SW_AON_NPUSYS_RST_n;
	CHIP_REG_SET(data, REG_AON_NPUSYS_RST_CFG);

	data = CHIP_REG_GET(REG_AP_IMG_NNA_TEESWRST);
	data |= SW_AP_SW_IMG_NNA_RST_n;

	CHIP_REG_SET(data, REG_AP_IMG_NNA_TEESWRST);
}

void venc_reset()
{
	uint32_t data = CHIP_REG_GET(REG_AON_VENC_RST_CFG);

	data &= ~SW_AON_VENC_RST_n;
	CHIP_REG_SET(data, REG_AON_VENC_RST_CFG);
}

void venc_reset_release()
{
	uint32_t data = CHIP_REG_GET(REG_AON_VENC_RST_CFG);

	data |= SW_AON_VENC_RST_n;
	CHIP_REG_SET(data, REG_AON_VENC_RST_CFG);
}

void gpu_reset()
{
	uint32_t data = CHIP_REG_GET(REG_AON_GPU_RST_CFG);

	data &= ~SW_AON_GPU_RST_n;
	CHIP_REG_SET(data, REG_AON_GPU_RST_CFG);
}

void gpu_reset_release()
{
	uint32_t data = CHIP_REG_GET(REG_AON_GPU_RST_CFG);

	data |= SW_AON_GPU_RST_n;
	CHIP_REG_SET(data, REG_AON_GPU_RST_CFG);
}

void dsp0_reset()
{
	uint32_t data = CHIP_REG_GET(REG_AON_DSP0_RST_CFG);

	data &= ~SW_AON_DSP0_RST_n;
	CHIP_REG_SET(data, REG_AON_DSP0_RST_CFG);
}

void dsp0_reset_release()
{
	uint32_t data = CHIP_REG_GET(REG_AON_DSP0_RST_CFG);

	data |= SW_AON_DSP0_RST_n;
	CHIP_REG_SET(data, REG_AON_DSP0_RST_CFG);
}

void dsp1_reset()
{
	uint32_t data = CHIP_REG_GET(REG_AON_DSP1_RST_CFG);

	data &= ~SW_AON_DSP1_RST_n;
	CHIP_REG_SET(data, REG_AON_DSP1_RST_CFG);
}

void dsp1_reset_release()
{
	uint32_t data = CHIP_REG_GET(REG_AON_DSP1_RST_CFG);

	data |= SW_AON_DSP1_RST_n;
	CHIP_REG_SET(data, REG_AON_DSP1_RST_CFG);
}

void apsys_reset()
{
	uint32_t data = CHIP_REG_GET(REG_AON_APSYS_RST_CFG);

	data &= ~SW_AON_APSYS_RST_n;
	CHIP_REG_SET(data, REG_AON_APSYS_RST_CFG);
}

void apsys_reset_release()
{
	uint32_t data = CHIP_REG_GET(REG_AON_APSYS_CLK_GATE);

	data &= ~AON_APSYS_CLK_EN;
	CHIP_REG_SET(data, REG_AON_APSYS_CLK_GATE);

	data |= SW_AON_APSYS_RST_n;
	CHIP_REG_SET(data, REG_AON_APSYS_RST_CFG);
}

void sram_axi_reset()
{
	uint32_t data = CHIP_REG_GET(REG_AON_SRAM_AXI_RST_CFG);

	data &= ~(AON_RSTGEN_SW_SRAM_AXI_CORE_RST_n |
		AON_RSTGEN_SW_SRAM_AXI_ARST_n_4 |
		AON_RSTGEN_SW_SRAM_AXI_ARST_n_3 |
		AON_RSTGEN_SW_SRAM_AXI_ARST_n_2 |
		AON_RSTGEN_SW_SRAM_AXI_ARST_n_1 |
		AON_RSTGEN_SW_SRAM_AXI_ARST_n_0 );
	CHIP_REG_SET(data, REG_AON_SRAM_AXI_RST_CFG);
}

void sram_axi_reset_release()
{
	uint32_t data = CHIP_REG_GET(REG_AON_SRAM_AXI_RST_CFG);

	data |= AON_RSTGEN_SW_SRAM_AXI_CORE_RST_n |
		AON_RSTGEN_SW_SRAM_AXI_ARST_n_4 |
		AON_RSTGEN_SW_SRAM_AXI_ARST_n_3 |
		AON_RSTGEN_SW_SRAM_AXI_ARST_n_2 |
		AON_RSTGEN_SW_SRAM_AXI_ARST_n_1 |
		AON_RSTGEN_SW_SRAM_AXI_ARST_n_0;
	CHIP_REG_SET(data, REG_AON_SRAM_AXI_RST_CFG);
}

void pvtc_reset()
{
	uint32_t data = CHIP_REG_GET(REG_AON_PVTC_RST_CFG);

	data &= ~AON_RSTGEN_SW_PVTC_PRST_n;
	CHIP_REG_SET(data, REG_AON_PVTC_RST_CFG);
}

void pvtc_reset_release()
{
	uint32_t data = CHIP_REG_GET(REG_AON_PVTC_RST_CFG);

	data |= AON_RSTGEN_SW_PVTC_PRST_n;
	CHIP_REG_SET(data, REG_AON_PVTC_RST_CFG);
}

