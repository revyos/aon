
/*
 *    Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/*******************************************************
 * @file       da9063.c
 * @brief      source file for pmic-ic
 * @version    V1.0
 * @date       23. Feb 2021
 * ******************************************************/
#include <drv/common.h>
#include <drv/helper.h>
#include <drv/pmic.h>
#include "pmic_rsc_def.h"
#include "da9063.h"
#include "pmic_help.h"
#include "syslog.h"

#define DCxSR                             (10000)          /// 10mv/us
#define WDT_TMR_LEN_MIN                   (2)
#define WDT_TMR_LEN_MAX                   (128)
#define WDT_TMR_LEN_DEF                   (64)
#define VOLTAGE_DELAY_MARGIN              (50)
extern csi_wdt_ops_t da9063_wdt_ops;
csi_error_t da9063_init(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev);
csi_error_t da9063_wdt_set_timer_len(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev, uint32_t msecond);
csi_error_t da9063_wdt_start(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
csi_error_t da9063_wdt_feed(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
csi_error_t da9063_uninit(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev);
csi_error_t da9063_set_reg(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t reg, uint32_t val);
csi_error_t da9063_get_reg(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t reg, uint32_t *val);
csi_error_t da9063_get_powerflag(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t *flag);
csi_error_t da9063_clear_fault_log(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
csi_error_t da9063_get_bootmode(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
csi_error_t da9063_gpio_init(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
csi_error_t da9063_get_powerup_flag(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t *flag);

/* Macros for voltage DC\DC converters (BUCKs) */
#define DA_9063_REG_DVC(_id, min, max, step, en) \
{    														\
    	.name = "DA_9063_REG_ID_" # _id,							\
    	.id   = DA_9063_REG_ID_ ## _id,							\
    	.min_uv = min,										\
    	.max_uv = max,										\
    	.step_uv = step,									\
    	.initial_benable = en,								\
    	.emask = DA_9063_REG_BUCK_EN,							\
    	.ereg = DA_9063_REG_REG_## _id##_CONT,					\
    	.vreg =	DA_9063_REG_REG_V## _id##_A,						\
    	.vmask = DA_9063_REG_VBUCK_MASK,							\
	.vconf = DA_9063_REG_REG_## _id##_CFG,					\
    	.linear_min_sel = DA_9063_REG_VBUCK_BIAS,            \
}

/* Macros for LDO */
#define DA_9063_REG_LDO(_id, min, max, step, en) \
{    												\
        .name   = "DA_9063_REG_ID_" # _id,    			\
        .id     = DA_9063_REG_ID_ ## _id,    			\
        .min_uv = min,                              \
        .max_uv = max,                              \
        .initial_benable = en,                     \
    	.step_uv = step,							\
        .vreg = DA_9063_REG_REG_V## _id##_A,    			\
        .vmask = DA_9063_REG_V##_id##_MASK,    			\
        .ereg = DA_9063_REG_REG_## _id##_CONT,    		\
        .emask = DA_9063_REG_LDO_EN,                     \
    	.linear_min_sel = DA_9063_REG_V## _id##_BIAS,      \
}

/* Macros for LDO */
#define DA_9063_REG_GPIO(_id, en) \
{    												\
        .name   = "DA_9063_REG_ID_GPIO" # _id,    			\
        .id     = DA_9063_REG_ID_GPIO ## _id,    			\
        .initial_benable = en,                     \
        .ereg = DA_9063_REG_REG_GPIO_MODE0_7,    		\
        .emask = 1 << _id,                     \
}

static csi_regu_t da9063_regulator_info[] =
{
    	DA_9063_REG_DVC(BCORE1, 300000, 1570000, 10000, 1),
    	DA_9063_REG_DVC(BCORE2, 300000, 1570000, 10000, 1),
    	DA_9063_REG_DVC(BPRO, 530000, 1800000, 10000, 1),
    	DA_9063_REG_DVC(BMEM, 800000, 3340000, 20000, 1),
    	DA_9063_REG_DVC(BIO, 800000, 3340000, 20000, 1),
    	DA_9063_REG_DVC(BPERI, 800000, 3340000, 20000, 1),
    	DA_9063_REG_LDO(LDO1, 600000, 1860000, 20000, 1),
    	DA_9063_REG_LDO(LDO2, 600000, 1860000, 20000, 1),
    	DA_9063_REG_LDO(LDO3, 900000, 3440000, 20000, 1),
    	DA_9063_REG_LDO(LDO4, 600000, 1860000, 20000, 1),
    	DA_9063_REG_LDO(LDO5, 900000, 3600000, 50000, 1),
    	DA_9063_REG_LDO(LDO6, 900000, 3600000, 50000, 1),
    	DA_9063_REG_LDO(LDO7, 900000, 3600000, 50000, 1),
    	DA_9063_REG_LDO(LDO8, 900000, 3600000, 50000, 1),
    	DA_9063_REG_LDO(LDO9, 900000, 3600000, 50000, 1),
    	DA_9063_REG_LDO(LDO10, 900000, 3600000, 50000, 1),
    	DA_9063_REG_LDO(LDO11, 900000, 3600000, 50000, 1),
    	DA_9063_REG_GPIO(4, 0),
        DA_9063_REG_GPIO(7, 1),
};

static uint32_t powerupflag = 0;

csi_error_t da9063_get_reg(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t reg, uint32_t *val)
{
    int ret;
    uint16_t i2caddr;

    if (reg >= 0x100) {
    	i2caddr = pmic_dev->dev_info->addr2;
    	reg -= 0x100;
    } else {
    	i2caddr = pmic_dev->dev_info->addr1;
    }

    ret = pmic->pmic_read_reg(pmic, i2caddr, reg, val);
    if (ret)
    	return CSI_ERROR;

    return CSI_OK;
}

csi_error_t da9063_set_reg(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t reg, uint32_t val)
{
    int ret;
    uint16_t i2caddr;

    if (reg >= 0x100) {
    	i2caddr = pmic_dev->dev_info->addr2;
    	reg -= 0x100;
    } else {
    	i2caddr = pmic_dev->dev_info->addr1;
    }

    ret = pmic->pmic_write_reg(pmic, i2caddr, reg, val);
    if (ret)
    	return CSI_ERROR;

    return CSI_OK;
}

static csi_error_t da9063_regu_power_ctrl(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t regu_id, uint32_t bon)
{
    CSI_PARAM_CHK(pmic,CSI_ERROR);
    CSI_PARAM_CHK(pmic_dev,CSI_ERROR);

    csi_error_t ret = CSI_OK;
    uint32_t val;
    uint16_t dev_addr =  pmic_dev->dev_info->addr1;
    csi_regu_t *regu = NULL;

    /* Get regulator info from hw_id*/
    ret = pmic_get_regu_info(pmic_dev, regu_id, &regu);
    if(ret) {
        return ret;
    }

    if(!regu->ereg | !regu->emask)
    		return CSI_UNSUPPORTED;

    /* Enable\Disbale Bucks\LDOs */
    ret = pmic->pmic_read_reg(pmic, dev_addr, regu->ereg, &val);
    if(ret)
    		return ret;
    val = bon ? val | regu->emask : val & (~regu->emask);
    ret = pmic->pmic_write_reg(pmic, dev_addr, regu->ereg, val);
    if(ret)
    		return ret;

    return ret;
}

static csi_error_t da9063_get_voltage(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev,uint32_t regu_id,uint32_t *target_uv)
{
    CSI_PARAM_CHK(pmic,CSI_ERROR);
    CSI_PARAM_CHK(pmic_dev,CSI_ERROR);

    csi_error_t ret;
    csi_regu_t *regu = NULL;
    uint32_t val;
    uint16_t dev_addr =  pmic_dev->dev_info->addr1;

    ret = pmic_get_regu_info(pmic_dev, regu_id, &regu);
    if(ret)
            return ret;

    if(!regu->vreg | !regu->vmask)
    		return CSI_UNSUPPORTED;

    ret = pmic->pmic_read_reg(pmic, dev_addr, regu->vreg, &val);
    if(ret)
    		return ret;
    val = (val & regu->vmask) - (regu->linear_min_sel);
    *target_uv = regu->min_uv + val * regu->step_uv;

    //LOG_D("get da9063 vol:%d %d\n",id, *target_uv);
    return ret;
}

static csi_error_t da9063_set_voltage(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev,uint32_t regu_id,uint32_t target_uv)
{
    CSI_PARAM_CHK(pmic,CSI_ERROR);
    CSI_PARAM_CHK(pmic_dev,CSI_ERROR);

    csi_error_t ret;
    csi_regu_t *regu = NULL;
    uint32_t val, temp;
    uint16_t dev_addr =  pmic_dev->dev_info->addr1;

    ret = pmic_get_regu_info(pmic_dev, regu_id, &regu);
    if(ret) {
    	return ret;
    }

    if(!regu->vreg | !regu->vmask)
    		return CSI_UNSUPPORTED;

    ret = pmic->pmic_read_reg(pmic, dev_addr, regu->vreg, &temp);
    if(ret)
    		return ret;

    val = (temp & ~regu->vmask) | (regu->vmask & (target_uv - regu->min_uv)/regu->step_uv + regu->linear_min_sel);
    ret = pmic->pmic_write_reg(pmic, dev_addr, regu->vreg, val);
    if(ret)
    		return ret;

    LOG_D("da9063:set_voltage:regu_id %u target_uv %u \n",regu_id,target_uv);
    return ret;
}

csi_error_t da9063_gpio_ctrl(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t gpio_index, uint32_t val)
{
    CSI_PARAM_CHK(pmic,CSI_ERROR);
    csi_error_t ret;
    uint32_t reg_val;

    //LOG_D("gpio %d %d\n", gpio_index, val);
    uint16_t dev_addr = pmic_dev->dev_info->addr1;
    if(gpio_index <= 7){
        ret = pmic->pmic_read_reg(pmic,dev_addr,DA_9063_REG_REG_GPIO_MODE0_7, &reg_val);
        if(ret){
            return CSI_ERROR;
        }
        reg_val = (val) ? (reg_val | (1 << gpio_index)) : (reg_val & (~(1 << gpio_index)));
        ret = pmic->pmic_write_reg(pmic, dev_addr, DA_9063_REG_REG_GPIO_MODE0_7, reg_val);
    } else {
        ret = pmic->pmic_read_reg(pmic,dev_addr,DA_9063_REG_REG_GPIO_MODE8_15, &reg_val);
        if(ret){
            return CSI_ERROR;
        }
        reg_val = (val) ? (reg_val | (1 << (gpio_index - 8))) : (reg_val & (~(1 << (gpio_index - 8))));
        ret = pmic->pmic_write_reg(pmic, dev_addr, DA_9063_REG_REG_GPIO_MODE8_15, reg_val);
    }

    return ret;
}

uint32_t da9063_get_twdscale(uint32_t tick_scale)
{
    uint32_t tmp;

    if( tick_scale <= 2 ) {
    	tmp = 1;                        ///< 2-second
    }else if( tick_scale <= 4) {
    	tmp = 2;                        ///< 4-second
    }else if( tick_scale <= 8) {
    	tmp = 3;                        ///< 8-second
    }else if( tick_scale <= 16) {
    	tmp = 4;                        ///< 16-second
    }else if( tick_scale <= 32) {
    	tmp = 5;                        ///< 32-second
    }else if( tick_scale <= 64 ){
    	tmp = 6;                        ///< 64-second
    }
    else {
    	tmp = 7;                        ///< 128-second
    }

    return tmp;
}

csi_error_t da9063_wdt_update_timer(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t second)
{
    CSI_PARAM_CHK(pmic,CSI_ERROR);
    csi_error_t ret;
    uint32_t tmp;
    uint32_t val;
    uint16_t dev_addr =  pmic_dev->dev_info->addr1;

    /*reset wdt-timer with user-defined value*/
    ret = pmic->pmic_read_reg(pmic, dev_addr, DA_9063_REG_REG_CONTROL_D,&val);
    if (ret)
    	return ret;
    val &= ~0x7;
    tmp = (uint32_t)da9063_get_twdscale(second);
    val |= tmp;
    ret = pmic->pmic_write_reg(pmic,dev_addr,DA_9063_REG_REG_CONTROL_D,val);
    if(ret) {
        return ret;
    }

    return CSI_OK;
}

csi_error_t da9063_wdt_stop(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
    csi_error_t ret;
    uint32_t val;
    uint16_t dev_addr = pmic_dev->dev_info->addr1;
    ret = pmic->pmic_read_reg(pmic, dev_addr, DA_9063_REG_REG_CONTROL_D, &val);
    if(ret) {
        return ret;
    }
    val &= ~0x7;
    ret = pmic->pmic_write_reg(pmic, dev_addr, DA_9063_REG_REG_CONTROL_D, val);
    //da9063_wdt_ops.flag &= ~PMIC_WDT_ENABLE;
    mdelay(1);

    return ret;
}

csi_error_t da9063_wdt_feed(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
    CSI_PARAM_CHK(pmic,CSI_ERROR);
    csi_error_t ret;
    uint16_t addr = pmic_dev->dev_info->addr1;
    ret = pmic->pmic_write_reg(pmic, addr, DA_9063_REG_REG_CONTROL_F, 1);
    if(ret) {
    	return ret;
    }

    return CSI_OK;
}

csi_error_t da9063_wdt_set_timer_len(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t second)
{
    CSI_PARAM_CHK(pmic,CSI_ERROR);
    csi_error_t ret = CSI_OK;

    /*check whether wdt is enable*/

    	ret = da9063_wdt_update_timer(pmic,pmic_dev, second);
    	if(ret)
    		return ret;
    	/*feed wdt for reset wdt-timer-len*/
    	ret = da9063_wdt_feed(pmic,pmic_dev);


    return ret;
}

static csi_error_t _da9063_wdt_start(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t second)
{

    csi_error_t ret;

    /*disable wdt firstly*/
    ret = da9063_wdt_stop(pmic, pmic_dev);
    if (ret)
    	return ret;

    /*reset wdt-timer with user-defined value*/
    ret = da9063_wdt_update_timer(pmic, pmic_dev, second);
    if (ret)
    	return ret;

    /*set wdt enable flag*/
    //da9063_wdt_ops.flag |= PMIC_WDT_ENABLE;

    return CSI_OK;
}

csi_error_t da9063_wdt_start(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
    return _da9063_wdt_start(pmic, pmic_dev, pmic_dev->dev_info->wdt_len);
}

csi_error_t da9063_wdt_repower(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
    csi_error_t ret;

    /*set wdt-timer-lenght as min-val*/

    /*start wdt-timer wait for repower*/
    ret = _da9063_wdt_start(pmic, pmic_dev, WDT_TMR_LEN_MIN);
    if (ret)
    	return ret;
    else
    	while(1);
}

csi_error_t da9063_select_buck_mode(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t regu_id, pmic_buck_mode mode)
{
    CSI_PARAM_CHK(pmic, CSI_ERROR);
    CSI_PARAM_CHK(pmic_dev, CSI_ERROR);
    csi_error_t ret;
    csi_regu_t *regu = NULL;
    uint32_t val;
    uint32_t temp;
    uint16_t dev_addr = pmic_dev->dev_info->addr1;

    ret = pmic_get_regu_info(pmic_dev, regu_id, &regu);
    if(ret) {
    	return ret;
    }
    if(!regu->vreg | !regu->vmask | !regu->vconf)
    		return CSI_UNSUPPORTED;

   ret = pmic->pmic_read_reg(pmic, dev_addr, regu->vconf, &temp);
    if(ret)
    		return ret;

    switch(mode) {
	case SYNC:
		val = DA_9063_REG_BUCK_MODE_SYNC | (~DA_9063_REG_BUCK_MODE_MASK & temp);
		/*
		ret = pmic->pmic_read_reg(pmic, dev_addr, regu->vreg, &temp);
		if(ret)
	    	return ret;
		val = (~(1 << 7)) & temp;
		ret = pmic->pmic_write_reg(pmic, dev_addr, regu->vreg, val);
		if(ret)
			return ret;
			*/
	break;
	case SLEEP:
		val = DA_9063_REG_BUCK_MODE_SLEEP | (~DA_9063_REG_BUCK_MODE_MASK & temp);
		/*
		ret = pmic->pmic_read_reg(pmic, dev_addr, regu->vreg, &temp);
		if(ret)
			return ret;
		val = (1 << 7) | temp;
		ret = pmic->pmic_write_reg(pmic, dev_addr, regu->vreg, val);
		if(ret)
			return ret;
			*/
	break;
	case AUTO:
		val = DA_9063_REG_BUCK_MODE_AUTO | (~DA_9063_REG_BUCK_MODE_MASK & temp);
	break;
	default:
	{
		LOG_E("buck output mode is not supported\n");
		return CSI_UNSUPPORTED;
	}
	break;
    }
    ret = pmic->pmic_write_reg(pmic, dev_addr, regu->vconf, val);
    if(ret)
	return ret;
    LOG_D("[%s,%d]set_voltage:regu_id %d into %d mode"
		    "Register: 0x%x, Value: 0x%x\n",
		    __func__, __LINE__,
		    regu_id, mode,
		    regu->vconf, val);
    return ret;
}

csi_error_t da9063_regu_read_faults(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
    CSI_PARAM_CHK(pmic, CSI_ERROR);
    CSI_PARAM_CHK(pmic_dev, CSI_ERROR);
    csi_error_t ret;

    uint32_t event_data[2] = {0x00};
    uint16_t dev_addr = pmic_dev->dev_info->addr1;
    csi_pmic_dev_status_t *status = &pmic_dev->pmic_dev_status;

    ret = pmic->pmic_read_reg(
    		pmic, dev_addr, DA_9063_REG_REG_EVENT_B, &event_data[0]);
    if (ret)
    	return ret;
    ret = pmic->pmic_read_reg(
    		pmic, dev_addr, DA_9063_REG_REG_FAULT_LOG, &event_data[1]);
    if (ret)
    	return ret;
    if(event_data[0]) {
    	LOG_E("%s, %d da9063 error is detected!, the event data is: %08x\n", event_data[0]);
    switch(event_data[0]) {
    	case DA_9063_REG_E_TEMP:
    		{
    		status->dev_status = CSI_STATUS_PMIC_DEV_OT;
    		}
    	break;
    	case DA_9063_REG_E_VDD_MON:
    		{
    		status->dev_status = CSI_STATUS_PMIC_DEV_OV;
    		}
    	break;
    	case DA_9063_REG_E_VDD_WARN:
    		{
    		status->dev_status = CSI_STATUS_PMIC_DEV_UV;
    		}
    	break;
    	case DA_9063_REG_E_LDO_LIM:
    		{
    		status->dev_status = CSI_STATUS_PMIC_DEV_OC;
    		}
    	break;
    default:
    	{
    	status->dev_status = CSI_STATUS_PMIC_DEV_OK;
    	}
    break;
    }

    /* Clear event B reg to enable next IRQ */
    	ret = pmic->pmic_write_reg(
    			pmic, dev_addr, DA_9063_REG_REG_EVENT_B, event_data[0]);
    	if (ret)
    	{
    		LOG_D("%s, %d",__func__,__LINE__);
    		return ret;
    	}
    }

    if(event_data[1]) {
    /* Clear FAULT_LOG reg to enable next IRQ */
    	ret = pmic->pmic_write_reg(
    			pmic, dev_addr, DA_9063_REG_REG_FAULT_LOG, event_data[1]);
    	if (ret)
    	{
    		LOG_D("%s, %d",__func__,__LINE__);
    		return ret;
    	}
    }
    return CSI_OK;
}

csi_error_t da9063_regu_get_temperature(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t* temp)
{
    #warning "remove me"
    CSI_PARAM_CHK(pmic, CSI_ERROR);
    CSI_PARAM_CHK(pmic_dev, CSI_ERROR);
    csi_error_t ret;

    uint32_t val = 0;
    uint32_t val_l = 0;
    uint32_t val_h = 0;
    //csi_pmic_dev_status_t *status = &pmic_dev->pmic_dev_status;
    uint16_t dev_addr = pmic_dev->dev_info->addr1;

    /* Real-time junction temperature value */

    val = (( 1 << 2) | (3 << 4));
    ret = pmic->pmic_write_reg(pmic, dev_addr, DA_9063_REG_REG_ADC_MAN, val);
    if(ret) {
    	return ret;
    }
    mdelay(5);

    ret = pmic->pmic_read_reg(pmic, dev_addr, DA_9063_REG_REG_ADC_RES_L, &val_l);
    if(ret) {
    	return ret;
    }
    ret = pmic->pmic_read_reg(pmic, dev_addr, DA_9063_REG_REG_ADC_RES_H, &val_h);
    if(ret) {
    	return ret;
    }
    val = (((val_h & DA_9063_REG_ADC_RES_M_MASK) << DA_9063_REG_ADC_RES_L_BITS) | (((val_l & DA_9063_REG_ADC_RES_L_MASK)) >> DA_9063_REG_ADC_RES_M_BITS));
    *temp = 330000 - 398 * val;
    LOG_D(" current junction temperature value is: 0x%08u\n", val);

    return CSI_OK;
}

// /* Read PMIC ID */
// csi_error_t da9063_get_chip_id(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
// {
//     #warning "fix me"
//     #if 0
//     CSI_PARAM_CHK(pmic, CSI_ERROR);
//     CSI_PARAM_CHK(pmic_dev, CSI_ERROR);
//     csi_error_t ret;

//     uint32_t chip_id;
//     ret = pmic->pmic_read_reg(
//     		pmic, DEV_ADDR_PAGE23, DA_9063_REG_REG_CHIP_ID-0x100, &chip_id);
//     if (ret) {
//     	LOG_D("%s, %d, read chip id error!\n",__func__, __LINE__);
//     	return ret;
//     }
//     else {
//     	pmic_dev->chip_id = chip_id;
// #if ANT_DEBUG
//     	LOG_D("%s, the chip ID is: %08x\n", __func__,chip_id);
// #endif
//     }
//     #endif
//     	return CSI_OK;
// }

csi_error_t da9063_get_powerflag(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t *flag)
{
    uint32_t val;
    csi_error_t ret;

    ret = da9063_get_reg(pmic, pmic_dev, DA_9063_REG_REG_EVENT_A, &val);
        if (ret)
    	return CSI_ERROR;

        if (val & DA_9063_REG_E_NONKEY)
    	*flag = 1;
        else
    	*flag = 0;

    return CSI_OK;
}

csi_error_t da9063_clear_fault_log(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
    uint32_t val;
    csi_error_t ret;

    /* Clear all IRQ_MASK */
    val = 0xFF;
    for(uint32_t reg = DA_9063_REG_REG_IRQ_MASK_A; reg <= DA_9063_REG_REG_IRQ_MASK_D ; reg++)
    {
    	ret = da9063_set_reg(pmic, pmic_dev, reg, val);
    	if(ret)
    		return ret;
    }
    /* Clear all the EVENT_REG values */
    for(uint32_t reg = DA_9063_REG_REG_EVENT_A; reg <= DA_9063_REG_REG_EVENT_D; reg++)
    {
    	ret = da9063_get_reg(pmic, pmic_dev, reg, &val);
    	if(ret)
    		return ret;
    	ret = da9063_set_reg(pmic, pmic_dev, reg, val);
    	if(ret)
    		return ret;
    }
    return CSI_OK;
}

csi_error_t da9063_get_bootmode(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
    uint32_t val;
    csi_error_t ret;

    /* get auto_boot bit */
    ret = da9063_get_reg(pmic, pmic_dev, DA_9063_REG_REG_CONTROL_C, &val);
    if (ret)
    	return ret;
    if (val & DA_9063_REG_AUTO_BOOT)
    	pmic_dev->dev_info->flag |= PMIC_DEV_ENABLE_AUTO_REBOOT;

    return CSI_OK;
}

csi_error_t da9063_gpio_init(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
    uint32_t val;
    csi_error_t ret;

    /*set gpio4 as push-pull gpo mode*/
    ret = da9063_get_reg(pmic, pmic_dev, DA_9063_REG_REG_GPIO_4_5, &val);
    if(ret){
    	return CSI_ERROR;
    }
    val |=((3 << 0) | (1 <<3));
    ret = da9063_set_reg(pmic, pmic_dev, DA_9063_REG_REG_GPIO_4_5, val);
    if(ret){
    	return CSI_ERROR;
    }

    /*set gpio2 as push-pull gpo mode*/
    ret = da9063_get_reg(pmic, pmic_dev, DA_9063_REG_REG_GPIO_2_3, &val);
    if(ret){
    	return CSI_ERROR;
    }
    val |=((3 << 0) | (1 <<3));
    ret = da9063_set_reg(pmic, pmic_dev, DA_9063_REG_REG_GPIO_2_3, val);
    if(ret){
    	return CSI_ERROR;
    }

    /*set gpio7 as push-pull gpo mode*/
    ret = da9063_get_reg(pmic, pmic_dev, DA_9063_REG_REG_GPIO_6_7, &val);
    if(ret){
    	return CSI_ERROR;
    }
    val |=((3 << 4) | (1 <<7));
    ret = da9063_set_reg(pmic, pmic_dev, DA_9063_REG_REG_GPIO_6_7, val);
    if(ret){
    	return CSI_ERROR;
    }

    return CSI_OK;
}

csi_error_t da9063_get_powerup_flag(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t *flag)
{
    *flag = powerupflag;

    return CSI_OK;
}

static csi_regu_ops_t da9063_ops = {
    .set_voltage     = da9063_set_voltage,
    .get_voltage     = da9063_get_voltage,
#ifdef PMIC_DEBUG
    .set_reg         = da9063_set_reg,
    .get_reg         = da9063_get_reg,
#endif
    .get_powerup     = da9063_get_powerup_flag,
    .regu_power_ctrl = da9063_regu_power_ctrl,
    .read_faults	 = da9063_regu_read_faults,
    .mode_select     = da9063_select_buck_mode,
    .get_temperature = da9063_regu_get_temperature,
    .gpio_output     = da9063_gpio_ctrl,
    .init            = da9063_init,
    .uninit          = da9063_uninit,
};

csi_wdt_ops_t da9063_wdt_ops = {
    .set_timer_len = da9063_wdt_set_timer_len,
    .start         = da9063_wdt_start,
    .stop          = da9063_wdt_stop,
    .feed          = da9063_wdt_feed,
    .repower       = da9063_wdt_repower,
};

struct csi_pmic_drv_data  da9063_drv = {
    .features      = PMIC_DRV_SUPPORT_WDT | PMIC_DRV_SUPPORT_AUTO_REBOOT,
    .ops           = &da9063_ops,
    .wdt_ops       = &da9063_wdt_ops,
    .regu_num      = sizeof(da9063_regulator_info)/sizeof(da9063_regulator_info[0]),
    .regu_info     =(csi_regu_t*) &da9063_regulator_info,
};

csi_error_t da9063_uninit(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
    csi_error_t ret = CSI_OK;
    uint32_t val;
    uint16_t dev_addr = pmic_dev->dev_info->addr1;

      /*power off da9063 ,triger it enter rtc or delivery mode*/
    ret = pmic->pmic_read_reg(pmic, dev_addr, DA_9063_REG_REG_CONTROL_A, &val);
    if(ret){
    	return CSI_ERROR;
    }
    val &=~0x1;
        ret =  pmic->pmic_write_reg(pmic, dev_addr, DA_9063_REG_REG_CONTROL_A, val);
    if (ret)
    	return CSI_ERROR;
    while(1);

    return ret;
}

csi_error_t da9063_init(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{

    csi_error_t ret;
    uint32_t val;

    uint16_t dev_addr = pmic_dev->dev_info->addr1;
    uint16_t dev_addr2 = pmic_dev->dev_info->addr2;

    ret = da9063_get_powerflag(pmic, pmic_dev, &powerupflag);
    if(ret)
    		return ret;
    ret = da9063_clear_fault_log(pmic, pmic_dev);
    if(ret)
    		return ret;
    ret = da9063_get_bootmode(pmic, pmic_dev);
    if(ret)
    		return ret;
    ret = da9063_gpio_init(pmic, pmic_dev);
    if(ret)
    		return ret;

    /* set the voltag clamping slew rate */
    val = (DA_9063_REG_SLEW_RATE_1US << 5);
    ret = da9063_set_reg(pmic, pmic_dev, DA_9063_REG_REG_CONTROL_C, val);
    if(ret)
    		return ret;
    pmic_dev->dev_info->slew_rate = 10; //mv

    /* enable active discharge of buck rails */
    ret = da9063_get_reg(pmic, pmic_dev, DA_9063_REG_REG_CONFIG_C, &val);
    if (ret)
    	return ret;
    val |= (1<<2);
    ret = da9063_set_reg(pmic, pmic_dev, DA_9063_REG_REG_CONFIG_C, val);
    if	(ret)
    	return ret;

/*
 *    	PMIC error detector initialize
 */
    /* CONFIG_B: settings of VDD_FAULT_UPPER and VDD_FAULT_LOWER comparator */
    val = ~(1 << 7);
    ret = pmic-> pmic_write_reg(pmic, dev_addr2, DA_9063_REG_REG_CONFIG_B - 0x100, val);
    if(ret)
    	return ret;

/*
*    Pmic Vsys voltage monitor enable
*/
    /* IRQ_MASK_B: enable monitor IRQ
     * 1. BIT 6: M_VDD_WARN, Vsys: under-voltage
     * 2. BIT 5: M_VDD_MON, Vsys: over-voltage
     * 3. BIT 3: M_LDO_LIM, only LDO 3,4,7,8,11
     * 4. BIT 1: M_TEMP, junction temp over 120
     */
    ret = pmic->pmic_read_reg(pmic, dev_addr, DA_9063_REG_REG_IRQ_MASK_B, &val);
    if(ret)
    	return ret;
    val &= ~((3 << 6) | (1 << 1) | (1 << 3) );
    ret = pmic->pmic_write_reg(pmic, dev_addr, DA_9063_REG_REG_IRQ_MASK_B, val);
    if(ret) {

    	return ret;
    }

    return CSI_OK;
}
