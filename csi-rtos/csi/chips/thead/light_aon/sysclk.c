#include <stdio.h>
#include <stdint.h>
#include <soc.h>
#include "power.h"
#include "sysclk.h"

clk_gate_t clk_gate [] = {
	CLK_GATE(MISCSYS_SDIO0, REG_MISCSYS_SDIO0_CLK_CTRL, SDIO_CLK_EN),
	CLK_GATE(MISCSYS_SDIO1, REG_MISCSYS_SDIO1_CLK_CTRL, SDIO_CLK_EN),
	CLK_GATE(APSYS_I2C0, REG_AP_PERI_CLK_CFG, I2C0_CLK_EN),
	CLK_GATE(APSYS_I2C1, REG_AP_PERI_CLK_CFG, I2C1_CLK_EN),
	CLK_GATE(APSYS_I2C2, REG_AP_PERI_CLK_CFG, I2C2_CLK_EN),
	CLK_GATE(APSYS_I2C3, REG_AP_PERI_CLK_CFG, I2C3_CLK_EN),
	CLK_GATE(APSYS_I2C4, REG_AP_PERI_CLK_CFG, I2C4_CLK_EN),
	CLK_GATE(APSYS_I2C5, REG_AP_PERI_CLK_CFG, I2C5_CLK_EN),
	CLK_GATE(APSYS_PWM, REG_AP_PERI_CLK_CFG, PWM_CLK_EN),
	CLK_GATE(VPSYS_G2D, REG_VP_VPSYS_CK_CFG, VPSYS_G2D_CLK_EN),
	CLK_GATE(VPSYS_FCE, REG_VP_VPSYS_CK_CFG, VPSYS_FCE_CLK_EN),
	CLK_GATE(VISYS_ISP0, REG_VI_VISYS_CLK_GATE_EN_1, VISYS_CLKCTRL_ISP0_CLK_EN),
	CLK_GATE(VISYS_ISP1, REG_VI_VISYS_CLK_GATE_EN_1, VISYS_CLKCTRL_ISP1_CLK_EN),
	CLK_GATE(VISYS_ISP_RY, REG_VI_VISYS_CLK_GATE_EN_0, VISYS_CLKCTRL_ISP_RY_HCLK_EN),
};

void audio_sys_clk_enable(bool en)
{
	uint32_t data = CHIP_REG_GET(REG_AON_AUDIO_CLK_GATE);

	if (en)
		data |= AUDIO_SUBSYS_ACLK_AP2CP_EN | AUDIO_SUBSYS_ACLK_CP2AP_EN;
	else
		data &= ~(AUDIO_SUBSYS_ACLK_AP2CP_EN | AUDIO_SUBSYS_ACLK_CP2AP_EN);

	CHIP_REG_SET(data, REG_AON_AUDIO_CLK_GATE);
}

void vdec_sys_clk_enable(bool en)
{
	uint32_t data = CHIP_REG_GET(REG_AON_VDEC_CLK_GATE);

	if (en)
		data |= VDEC_SUBSYS_VDEC_CLK_EN;
	else
		data &= ~VDEC_SUBSYS_VDEC_CLK_EN;

	CHIP_REG_SET(data, REG_AON_VDEC_CLK_GATE);
}

void venc_sys_clk_enable(bool en)
{
	uint32_t data = CHIP_REG_GET(REG_AON_VENC_CLK_GATE);

	if (en)
		data |= VDEC_SUBSYS_VENC_CLK_EN;
	else
		data &= ~VDEC_SUBSYS_VENC_CLK_EN;

	CHIP_REG_SET(data, REG_AON_VENC_CLK_GATE);
}

void gpu_sys_clk_enable(bool en)
{
	uint32_t data = CHIP_REG_GET(REG_AON_GPU_CLK_GATE);

	if (en)
		data |= VDEC_SUBSYS_GPU_CLK_EN;
	else
		data &= ~VDEC_SUBSYS_GPU_CLK_EN;

	CHIP_REG_SET(data, REG_AON_GPU_CLK_GATE);
}

void dsp0_sys_clk_enable(bool en)
{
	uint32_t data = CHIP_REG_GET(REG_AON_DSP0_CLK_GATE);

	if (en)
		data |= VDEC_SUBSYS_DSP0_CLK_EN;
	else
		data &= ~VDEC_SUBSYS_DSP0_CLK_EN;

	CHIP_REG_SET(data, REG_AON_DSP0_CLK_GATE);
}

void dsp1_sys_clk_enable(bool en)
{
	uint32_t data = CHIP_REG_GET(REG_AON_DSP1_CLK_GATE);

	if (en)
		data |= VDEC_SUBSYS_DSP1_CLK_EN;
	else
		data &= ~VDEC_SUBSYS_DSP1_CLK_EN;

	CHIP_REG_SET(data, REG_AON_DSP1_CLK_GATE);
}

void npu_sys_clk_enable(bool en)
{
	uint32_t data = CHIP_REG_GET(APSYS_CLKGEN_NPU_CCLK_TEECFG);

	if (en)
		data |= NPU_CORE_CLK_EN;
	else
		data &= ~NPU_CORE_CLK_EN;

	CHIP_REG_SET(data, APSYS_CLKGEN_NPU_CCLK_TEECFG);
}

void c910_sys_clk_enable(bool en, int cpuid)
{
	uint32_t data = CHIP_REG_GET(REG_AP_C910_CORE_CLK_TEECFG);

	switch (cpuid) {
	case C910_CORE0:
		if (en)
			data |= C910_CORE0_CLK_EN;
		else
			data &= ~C910_CORE0_CLK_EN;
		break;
	case C910_CORE1:
		if (en)
			data |= C910_CORE1_CLK_EN;
		else
			data &= ~C910_CORE1_CLK_EN;
		break;
	case C910_CORE2:
		if (en)
			data |= C910_CORE2_CLK_EN;
		else
			data &= ~C910_CORE2_CLK_EN;
		break;
	case C910_CORE3:
		if (en)
			data |= C910_CORE3_CLK_EN;
		else
			data &= ~C910_CORE3_CLK_EN;
		break;
	default:
		LOG_D("invalid c910 cpu id\n...");
		return;
	}
		LOG_D("reg = 0x%x, data = 0x%x\n",REG_AP_C910_CORE_CLK_TEECFG, data);
	CHIP_REG_SET(data, REG_AP_C910_CORE_CLK_TEECFG);
}


bool ip_clk_is_active(LIGHT_CLKGATE_NAME clk_name)
{
	int i = 0;
	uint32_t data;

	for(i; i<ARRAY_SIZE(clk_gate); i++)
	{
		if(clk_gate[i].clk_name == clk_name)
		{
		    data = CHIP_REG_GET(clk_gate[i].clken_addr);
		    return data & clk_gate[i].en_bit ? true : false;
		}
	}
	/* return false if ip clk is not found*/
	return false;
}
