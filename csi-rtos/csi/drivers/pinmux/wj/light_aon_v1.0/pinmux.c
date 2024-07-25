/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     pinmux.c
 * @brief    source file for the pinmux
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <drv/pin.h>
#include <drv/gpio.h>
#include "soc.h"

#define PIN_DRV_MAX             (15)

#ifdef CONFIG_XIP
#define ATTRIBUTE_DATA __attribute__((section(".ram.code")))
#else
#define ATTRIBUTE_DATA
#endif

#define readl(addr) \
    ({ unsigned int __v = (*(volatile uint32_t *) (addr)); __v; })

#define writel(b,addr) ((*(volatile uint32_t *) (addr)) = (b))

#define PADMUX_REG_ADDR(base,index)  ((uint8_t*)(base) + ((index >> 3) << 2))
#define PADMUX_REG_BIT_POS(index)    ((index & 0x7) << 2)
#define PADMUX_REG_MASK(index)       (0xF << PADMUX_REG_BIT_POS(index))

#define PADCFG_REG_ADDR(base,index)  ((uint8_t*)(base) + ((index >> 1) << 2))
#define PADCFG_REG_BIT_POS(index)    ((index & 0x1) << 4)
#define PADCFG_REG_PULL_MASK(index)  (0x70 << PADCFG_REG_BIT_POS(index))
#define PADCFG_REG_DRV_MASK(index)   (0xF << PADCFG_REG_BIT_POS(index))
#define PADCFG_REG_SLEW_MASK(index) (0x100 << PADCFG_REG_BIT_POS(index))


#define PADMUX_CFG(grp_addr_base,index,pin_func)  \
        { \
		uint32_t val; \
		val = readl(PADMUX_REG_ADDR(grp_addr_base,index)); \
		val &=~PADMUX_REG_MASK(index); \
		val |=	pin_func << PADMUX_REG_BIT_POS(index); \
		writel(val,PADMUX_REG_ADDR(grp_addr_base,index)); \
	}

#define PADCFG_PULL(grp_addr_base,index,bpull,bpullup)  \
        { \
		uint32_t val; \
		val = readl(PADCFG_REG_ADDR(grp_addr_base,index)); \
		val &=~PADCFG_REG_PULL_MASK(index); \
		val |=	((bpull<<4)|(bpullup<<5)) << PADCFG_REG_BIT_POS(index); \
		writel(val,PADCFG_REG_ADDR(grp_addr_base,index)); \
	}

#define PADCFG_SLEW(grp_addr_base,index,bfast)  \
        { \
		uint32_t val; \
		val = readl(PADCFG_REG_ADDR(grp_addr_base,index)); \
		val &=~PADCFG_REG_SLEW_MASK(index); \
		val |=	(bfast<<8) << PADCFG_REG_BIT_POS(index); \
		writel(val,PADCFG_REG_ADDR(grp_addr_base,index)); \
	}

#define PADCFG_DRV(grp_addr_base,index,strength)  \
        { \
		uint32_t val; \
		val = readl(PADCFG_REG_ADDR(grp_addr_base,index)); \
		val &=~PADCFG_REG_DRV_MASK(index); \
		val |=	(strength) << PADCFG_REG_BIT_POS(index); \
		writel(val,PADCFG_REG_ADDR(grp_addr_base,index)); \
	}

struct pinmux_addr_t {
	uint32_t pin_grp_id;
	uint32_t *pin_grp_cfg_base;
	uint32_t *pin_grp_mux_base;
};

#define PIN_GRP_ADDR_DEF(grp_idx,cfg_base,mux_base) \
{ \
	.pin_grp_id = grp_idx, \
	.pin_grp_cfg_base =(uint32_t*) cfg_base, \
	.pin_grp_mux_base =(uint32_t*) mux_base, \
}

struct pinmux_addr_t g_soc_pin_grp_addr[] = {
	PIN_GRP_ADDR_DEF(SOC_PIN_AP_RIGHT_TOP,      0xBC00C000,0xBC00C400),
	PIN_GRP_ADDR_DEF(SOC_PIN_AP_LEFT_TOP,       0xB7F3C000,0xB7F3C400),
	PIN_GRP_ADDR_DEF(SOC_PIN_AON,               0xFFF4A000,0xFFF4A400),
};

static int  light_pinmx_get_cfg_base(pin_name_t pin_name, uint32_t** cfg_base)
{
	uint32_t grp_num = ARRAY_SIZE(g_soc_pin_grp_addr);
	uint32_t grp_idx = PAD_GRP_IDX_GET(pin_name);
	uint32_t i;

	for(i = 0; i < grp_num; i++) {
		if (grp_idx == g_soc_pin_grp_addr[i].pin_grp_id)
			break;
	}

	if (i == grp_num)
		return -1;

	*cfg_base = g_soc_pin_grp_addr[i].pin_grp_cfg_base;
	return 0;
}

static int  light_pinmx_get_mux_base(pin_name_t pin_name, uint32_t** cfg_base)
{
	uint32_t grp_num = ARRAY_SIZE(g_soc_pin_grp_addr);
	uint32_t grp_idx = PAD_GRP_IDX_GET(pin_name);
	uint32_t i;

	for(i = 0; i < grp_num; i++) {
		if (grp_idx == g_soc_pin_grp_addr[i].pin_grp_id)
			break;
	}

	if (i == grp_num)
		return -1;

	*cfg_base = g_soc_pin_grp_addr[i].pin_grp_mux_base;
	return 0;
}
/*******************************************************************************
 * function: danica_ioreuse_inital
 *
 * description:
 *   initial danica_pinmux
 *******************************************************************************/
ATTRIBUTE_DATA csi_error_t csi_pin_set_mux(pin_name_t pin_name, pin_func_t pin_func)
{
	uint32_t *reg_addr;
	int      ret;

	ret = light_pinmx_get_mux_base(pin_name,&reg_addr);
	if (ret)
		return ret;

	PADMUX_CFG(reg_addr,PAD_INDEX(pin_name),pin_func);
	return 0;
}


csi_error_t csi_pin_mode(pin_name_t pin_name, csi_gpio_mode_t mode)
{

	uint32_t bpull,bpullup;
	uint32_t *reg_addr;
	int      ret;

	ret = light_pinmx_get_cfg_base(pin_name,&reg_addr);
	if (ret)
		return ret;

	switch (mode)
	{
	case PIN_PD:
		bpull =1;
		bpullup = 0;
		/* code */
		break;
	case PIN_PU:
		bpull =1;
		bpullup = 1;
		/* code */
		break;
	case PIN_PN:
		bpull =0;
		bpullup = 0;
		/* code */
		break;
	default:
		return -1;
		break;
	}
	PADCFG_PULL(reg_addr,PAD_INDEX(pin_name),bpull,bpullup);

    return CSI_OK;
}

csi_error_t csi_pin_wakeup(pin_name_t pin_name, bool enable)
{
    return CSI_UNSUPPORTED;
}

/**
  \brief       get the pin function.
  \param[in]   pin       refs to pin_name_e.
  \return      pin function count
*/
pin_func_t csi_pin_get_mux(pin_name_t pin_name)
{
    return CSI_UNSUPPORTED;
}

/**
  \brief       set pin speed
  \param[in]   pin_name pin name, defined in soc.h.
  \param[in]   speed    io speed
  \return      error code
*/
csi_error_t csi_pin_speed(pin_name_t pin_name, csi_pin_speed_t speed)
{
	uint32_t bfast;
	uint32_t *reg_addr;
	int      ret;

	ret = light_pinmx_get_cfg_base(pin_name,&reg_addr);
	if (ret)
		return ret;

	switch (speed)
	{
	case PIN_SPEED_NORMAL:
		bfast = 0;
		break;
	case PIN_SPEED_FAST:
		bfast = 1;
		break;
	default:
		return -1;
	}

	PADCFG_SLEW(reg_addr,PAD_INDEX(pin_name),bfast);
    return ret;
}


/**
  \brief       set pin drive
  \param[in]   pin_name pin name, defined in soc.h.
  \param[in]   drive    io drive
  \return      error code
*/
csi_error_t csi_pin_drive(pin_name_t pin_name, csi_pin_drive_t drive)
{
	uint32_t *reg_addr;
	int      ret;

	ret = light_pinmx_get_cfg_base(pin_name,&reg_addr);
	if (ret)
		return ret;

	if(drive > PIN_DRV_MAX) {
		return -1;
	}

	PADCFG_DRV(reg_addr,PAD_INDEX(pin_name),drive);
	return 0;
}
