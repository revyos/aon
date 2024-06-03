
/*
 *	Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/*******************************************************
 * @file       ricoh567.c
 * @brief      source file for pmic-ic
 * @version    V1.0
 * @date       23. Feb 2021
 * ******************************************************/
#include <drv/common.h>
#include <drv/pmic.h>
#include "pmic_rsc_def.h"
#include "pmic_help.h"
#include "ricoh567.h"
#include "syslog.h"


#define DCxSR               (14000)            ///< 1400uv/us
#define IS_REGU_DUAL_RAIL(regu) ((regu.sub.id1.hw_id!= 0xff && regu.sub.id2.hw_id != 0xff)? 1:0)
#define WDT_TMR_LEN         (128)

#define RICOH567_DC(_id, min, max, step, en)	    \
{						    \
        .name   = "RICOH567_ID_DC" # _id,	    \
        .id     = RICOH567_ID_DC ## _id,	    \
        .min_uv = min,				    \
        .max_uv = max,                              \
        .initial_benable = en,			    \
    	.step_uv = step,			    \
        .vreg = RICOH_REG_DC## _id##_OUT,	    \
        .vmask = RICOH_DC_OUT_MASK,		    \
        .ereg = RICOH_REG_DC## _id##_CTRL,	    \
        .emask = RICOH_DC_EN,                    \
}

#define RICOH567_LDO(_id, min, max, step, en)	    \
{						    \
        .name   = "RICOH567_ID_LDO" # _id,	    \
        .id     = RICOH567_ID_LDO ## _id,    	    \
        .min_uv = min,                              \
        .max_uv = max,                              \
        .initial_benable = en,			    \
    	.step_uv = step,			    \
        .vreg = RICOH_REG_LDO## _id##_OUT,       \
        .vmask = RICOH_LDO_OUT_MASK,		    \
        .ereg = RICOH_REG_LDO_CTRL,      \
        .emask = RICOH_LDO_EN_##_id,            \
}

static csi_error_t ricoh567_regu_pso3_slp(csi_pmic_t *pmic,uint16_t dev_addr, uint32_t benable);

extern csi_wdt_ops_t ricoh567_wdt_ops ;
static csi_regu_t ricoh567_regulator_info_v0[] =
{
	RICOH567_DC(1, 600000, 3500000, 12500, 1),
	RICOH567_DC(2, 600000, 3500000, 12500, 1),
	RICOH567_DC(3, 600000, 3500000, 12500, 1),
	RICOH567_DC(4, 600000, 3500000, 12500, 1),
	RICOH567_LDO(1, 900000, 3500000, 50000, 1),
	RICOH567_LDO(2, 900000, 3500000, 50000, 1),
	RICOH567_LDO(3, 900000, 3500000, 50000, 1),
	RICOH567_LDO(4, 900000, 3500000, 50000, 1),
	RICOH567_LDO(5, 900000, 3500000, 50000, 0), //0
	{
		.name   = "RICOH567_ID_GPIO3",
		.id     = RICOH567_ID_GPIO3,
		.min_uv = 800000,
		.max_uv = 800000,
		.step_uv = 0,
		.initial_benable  = 0, //0
	},
};

static csi_regu_t ricoh567_regulator_info_v1[] =
{
	RICOH567_DC(1, 600000, 3500000, 12500, 1),
	RICOH567_DC(2, 600000, 3500000, 12500, 1),
	RICOH567_DC(3, 600000, 3500000, 12500, 1),
	RICOH567_DC(4, 600000, 3500000, 12500, 0), //0
	RICOH567_LDO(1, 900000, 3500000, 50000, 1),
	RICOH567_LDO(2, 900000, 3500000, 50000, 1),
	RICOH567_LDO(3, 900000, 3500000, 50000, 1),
	RICOH567_LDO(4, 900000, 3500000, 50000, 1),
	RICOH567_LDO(5, 900000, 3500000, 50000, 1),
	{
		.name   = "RICOH567_ID_GPIO3",
		.id     = RICOH567_ID_GPIO3,
		.min_uv = 800000,
		.max_uv = 800000,
		.step_uv = 0,

		.initial_benable  = 1,
	},
};

static csi_error_t ricoh567_get_voltage(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev,uint32_t regu_id,uint32_t *target_uv)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);
	csi_error_t ret = CSI_ERROR;
	csi_regu_t *regu = NULL;
	uint32_t id = regu_id;
	uint32_t val;
	uint16_t dev_addr;

	ret = pmic_get_regu_info(pmic_dev,id,&regu);
	if(ret) {
		return ret;
	}

	dev_addr =  PMIC_DEV_ADDR(1, pmic_dev);

	#if PMIC_DEBUG
	LOG_D("ricoh567:get_voltage:regu_id %u target_uv %u \n",regu_id,*target_uv);
	#endif

	switch (id)
	{
	case RICOH567_ID_GPIO3:
		*target_uv = regu->max_uv;
		break;
	case RICOH567_ID_DC1:
		{
			ret =  pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_DC1_OUT,&val);
			if(ret){
				return CSI_ERROR;
			}
			val &=RICOH_DC_OUT_MASK;
			*target_uv = regu->min_uv + val*regu->step_uv;

		}
		break;
	case RICOH567_ID_DC2:
		{
			ret =  pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_DC2_OUT,&val);
			if(ret){
				return CSI_ERROR;
			}
			val &=RICOH_DC_OUT_MASK;

			*target_uv = regu->min_uv + val*regu->step_uv;

		}
		break;
	case RICOH567_ID_DC3:
		{
			ret =  pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_DC3_OUT,&val);
			if(ret){
				return CSI_ERROR;
			}
			val &=RICOH_DC_OUT_MASK;

			*target_uv = regu->min_uv + val*regu->step_uv;

		}
		break;
	case RICOH567_ID_DC4:
		{
			ret =  pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_DC4_OUT,&val);
			if(ret){
				return CSI_ERROR;
			}
			val &=RICOH_DC_OUT_MASK;

			*target_uv = regu->min_uv + val*regu->step_uv;

		}
		break;
	case RICOH567_ID_LDO1:
		{
			ret =  pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_LDO1_OUT,&val);
			if(ret){
				return CSI_ERROR;
			}
			val &=RICOH_LDO_OUT_MASK;
			val >>=1;

			*target_uv = regu->min_uv + val*regu->step_uv;

		}
		break;
	case RICOH567_ID_LDO2:
		{
			ret =  pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_LDO2_OUT,&val);
			if(ret){
				return CSI_ERROR;
			}
			val &=RICOH_LDO_OUT_MASK;
			val >>=1;

			*target_uv = regu->min_uv + val*regu->step_uv;

		}
		break;
	case RICOH567_ID_LDO3:
		{
			ret =  pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_LDO3_OUT,&val);
			if(ret){
				return CSI_ERROR;
			}
			val &=RICOH_LDO_OUT_MASK;
			val >>=1;

			*target_uv = regu->min_uv + val*regu->step_uv;

		}
		break;
	case RICOH567_ID_LDO4:
		{
			ret =  pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_LDO4_OUT,&val);
			if(ret){
				return CSI_ERROR;
			}
			val &=RICOH_LDO_OUT_MASK;
			val >>=1;

			*target_uv = regu->min_uv + val*regu->step_uv;

		}
		break;
	case RICOH567_ID_LDO5:
		{
			ret =  pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_LDO5_OUT,&val);
			if(ret){
				return CSI_ERROR;
			}
			val &=RICOH_LDO_OUT_MASK;
			val >>=1;

			*target_uv = regu->min_uv + val*regu->step_uv;

		}
		break;
	default:
		break;
	}

	return ret;
}

static csi_error_t ricoh567_set_voltage(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev,uint32_t regu_id,uint32_t target_uv)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);
	csi_error_t ret = CSI_ERROR;
	csi_regu_t *regu = NULL;
	uint32_t id = regu_id;
	uint16_t dev_addr;
	uint32_t val;

	ret = pmic_get_regu_info(pmic_dev,id,&regu);
	if(ret) {
		return ret;
	}

	dev_addr = PMIC_DEV_ADDR(1, pmic_dev);

	if (target_uv < regu->min_uv || target_uv > regu->max_uv )
		return CSI_ERROR;

	#if PMIC_DEBUG
	LOG_D("ricoh567:set_voltage:regu_id %d target_uv %d \n",regu_id,target_uv);
	#endif

	switch (id)
	{
	case RICOH567_ID_GPIO3:
		ret = CSI_UNSUPPORTED;
		break;
	case RICOH567_ID_DC1:
		val = (target_uv-regu->min_uv)/regu->step_uv;
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_DC1_OUT,val);
		if(ret){
			return CSI_ERROR;
		}
		//regu->target_uv = target_uv;
		break;

	case RICOH567_ID_DC2:
		val = (target_uv-regu->min_uv)/regu->step_uv;
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_DC2_OUT,val);
		if(ret){
			return CSI_ERROR;
		}
		//regu->target_uv = target_uv;
		break;

	case RICOH567_ID_DC3:
		val = (target_uv-regu->min_uv)/regu->step_uv;
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_DC3_OUT,val);
		if(ret){
			return CSI_ERROR;
		}
		//regu->target_uv = target_uv;
		break;

	case RICOH567_ID_DC4:
		val = (target_uv-regu->min_uv)/regu->step_uv;
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_DC4_OUT,val);
		if(ret){
			return CSI_ERROR;
		}
		//regu->target_uv = target_uv;
		break;

	case RICOH567_ID_LDO1:
		val = (target_uv-regu->min_uv)/regu->step_uv;
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_LDO1_OUT,val<<1);
		if(ret){
			return CSI_ERROR;
		}
		//regu->target_uv = target_uv;

		break;

	case RICOH567_ID_LDO2:
		val = (target_uv-regu->min_uv)/regu->step_uv;
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_LDO2_OUT,val<<1);
		if(ret){
			return CSI_ERROR;
		}
		//regu->target_uv = target_uv;

		break;

	case RICOH567_ID_LDO3:
		val = (target_uv-regu->min_uv)/regu->step_uv;
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_LDO3_OUT,val<<1);
		if(ret){
			return CSI_ERROR;
		}
		//regu->target_uv = target_uv;

		break;

	case RICOH567_ID_LDO4:
		val = (target_uv-regu->min_uv)/regu->step_uv;
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_LDO4_OUT,val<<1);
		if(ret){
			return CSI_ERROR;
		}
		//regu->target_uv = target_uv;

		break;

	case RICOH567_ID_LDO5:
		val = (target_uv-regu->min_uv)/regu->step_uv;
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_LDO5_OUT,val<<1);
		if(ret){
			return CSI_ERROR;
		}
		//regu->target_uv = target_uv;

		break;

	default:
		break;
	}

	return ret;

}

static csi_error_t ricoh567_regu_power_ctrl(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev,uint32_t regu_id,uint32_t bon)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);
	csi_error_t ret = CSI_ERROR;
	csi_regu_t *regu = NULL;
	uint32_t id = regu_id;
	uint32_t val;
	uint16_t dev_addr;

	ret = pmic_get_regu_info(pmic_dev,id,&regu);
	if(ret) {
		return ret;
	}

	dev_addr = PMIC_DEV_ADDR(1, pmic_dev);

	#if PMIC_DEBUG
	LOG_D("ricoh567:pwr_ctrl:%s regu_id %d bon %d \n",regu->name, regu_id,bon);
	#endif

	switch (id)
	{
	case RICOH567_ID_GPIO3:
#if 0
		ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_IOOUT,&val);
		if(ret){
			return CSI_ERROR;
		}
		val = (bon) ? (val | RICOH_GPIO(3) ):(val & (~RICOH_GPIO(3)));
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_IOOUT,val);
		if(ret) {
			return ret;
		}
#else
		ret  = ricoh567_regu_pso3_slp(pmic,dev_addr, !bon);
		if(ret) {
			return ret;
		}
#endif
		break;

	case RICOH567_ID_DC1:
		ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_DC1_CTRL,&val);
		if(ret){
			return CSI_ERROR;
		}
		val = (bon) ? (val | RICOH_DC_EN ):(val & (~RICOH_DC_EN));
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_DC1_CTRL,val);
		if(ret) {
			return ret;
		}

		break;

	case RICOH567_ID_DC2:
		ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_DC2_CTRL,&val);
		if(ret){
			return CSI_ERROR;
		}
		val = (bon) ? (val | RICOH_DC_EN ):(val & (~RICOH_DC_EN));
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_DC2_CTRL,val);
		if(ret) {
			return ret;
		}

		break;

	case RICOH567_ID_DC3:
		ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_DC3_CTRL,&val);
		if(ret){
			return CSI_ERROR;
		}
		val = (bon) ? (val | RICOH_DC_EN ):(val & (~RICOH_DC_EN));
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_DC3_CTRL,val);
		if(ret) {
			return ret;
		}

		break;

	case RICOH567_ID_DC4:
		ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_DC4_CTRL,&val);
		if(ret){
			return CSI_ERROR;
		}
		val = (bon) ? (val | RICOH_DC_EN ):(val & (~RICOH_DC_EN));
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_DC4_CTRL,val);
		if(ret) {
			return ret;
		}

		break;

	case RICOH567_ID_LDO1:
		ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_LDO_CTRL,&val);
		if(ret){
			return CSI_ERROR;
		}
		val = (bon) ? (val | RICOH_LDO_EN_1 ):(val & (~RICOH_LDO_EN_1));
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_LDO_CTRL,val);
		if(ret) {
			return ret;
		}

		break;

	case RICOH567_ID_LDO2:
		ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_LDO_CTRL,&val);
		if(ret){
			return CSI_ERROR;
		}
		val = (bon) ? (val | RICOH_LDO_EN_2 ):(val & (~RICOH_LDO_EN_2));
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_LDO_CTRL,val);
		if(ret) {
			return ret;
		}

		break;

	case RICOH567_ID_LDO3:
		ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_LDO_CTRL,&val);
		if(ret){
			return CSI_ERROR;
		}
		val = (bon) ? (val | RICOH_LDO_EN_3 ):(val & (~RICOH_LDO_EN_3));
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_LDO_CTRL,val);
		if(ret) {
			return ret;
		}

		break;

	case RICOH567_ID_LDO4:
		ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_LDO_CTRL,&val);
		if(ret){
			return CSI_ERROR;
		}
		val = (bon) ? (val | RICOH_LDO_EN_4 ):(val & (~RICOH_LDO_EN_4));
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_LDO_CTRL,val);
		if(ret) {
			return ret;
		}

		break;

	case RICOH567_ID_LDO5:
		ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_LDO_CTRL,&val);
		if(ret){
			return CSI_ERROR;
		}
		val = (bon) ? (val | RICOH_LDO_EN_5 ):(val & (~RICOH_LDO_EN_5));
		ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_LDO_CTRL,val);
		if(ret) {
			return ret;
		}

		break;

	default:
		break;
	}

	return ret;
}

uint32_t ricoh567_get_twdscale(uint32_t tick_scale)
{
	uint32_t tmp;

	if( tick_scale <= 8 ) {
		tmp = 1;                      ///<8-second
	}else if( tick_scale <= 32 ) {
		tmp = 2;                      ///<32-second
	}else if( tick_scale <= 128 ) {
		tmp = 3;                      ///<128-second
	}else {
		tmp =3;                       ///<128-second
	}

	return tmp;
}

csi_error_t ricoh567_wdt_update_timer_len(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t second)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);
	csi_error_t ret;
	uint32_t tmp;
	uint32_t val;

	uint16_t dev_addr = pmic_dev->dev_info->addr1;

	/*reset wdt-timer with user-defined value*/
	ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_WDT_CTRL,&val);
	if (ret)
		return ret;
	val &= ~RICOH_WDT_TIM_MSK;
	tmp = (uint32_t)ricoh567_get_twdscale(second);
	val |= tmp;
	val |= RICOH_WDT_EN;
	ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_WDT_CTRL,val);
	if(ret) {
	    return ret;
	}

	return CSI_OK;
}

csi_error_t ricoh567_wdt_feed(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);
	uint32_t val;
	uint16_t dev_addr = pmic_dev->dev_info->addr1;

	/*read RICOH_REG_WDT_CTRL to kick off wdt*/
	pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_WDT_CTRL,&val);
	pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_WDT_CTRL,&val);
	return CSI_OK;
}

static csi_error_t ricoh567_wdt_stop(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
	csi_error_t ret;
	uint32_t val;
	uint16_t dev_addr = pmic_dev->dev_info->addr1;

	ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_WDT_CTRL,&val);
	if (ret)
		return ret;
	val &=~RICOH_WDT_EN;

	ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_WDT_CTRL,val);
	if(ret)
		return ret;

	//ricoh567_wdt_ops.flag &= ~PMIC_WDT_ENABLE;

	return ret;
}

csi_error_t ricoh567_wdt_set_timer_len(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t second)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	csi_error_t ret = CSI_OK;

	pmic_dev->dev_info->wdt_len = (second <= 128) ? second:128;

	//if(ricoh567_wdt_ops.flag & PMIC_WDT_ENABLE) {
		ret = ricoh567_wdt_update_timer_len(pmic, pmic_dev, pmic_dev->dev_info->wdt_len);
		if(ret)
			return ret;

		/*kick off wdt-timer*/
		ret = ricoh567_wdt_feed(pmic, pmic_dev);
	//}

	return ret;
}

csi_error_t ricoh567_wdt_start(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);
	csi_error_t ret;
	uint32_t val;

	uint16_t dev_addr = pmic_dev->dev_info->addr1;
	/*disable wdt firstly*/
	ret = ricoh567_wdt_stop(pmic, pmic_dev);
	if (ret)
		return ret;
	/*enable out wdt timer-out signal*/
	ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_PWRIREN,&val);
	if (ret)
		return ret;
	val |= BIT(6);
	ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_PWRIREN,val);
	if(ret)
		return ret;
	/*reset wdt-timer with user-defined value*/
	ret = ricoh567_wdt_update_timer_len(pmic, pmic_dev, pmic_dev->dev_info->wdt_len);
	/*set wdt enable flag*/
	//ricoh567_wdt_ops.flag |= PMIC_WDT_ENABLE;

	return CSI_OK;
}

csi_error_t ricoh567_wdt_repower(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);
	csi_error_t ret;
	uint32_t val;
	uint16_t dev_addr = pmic_dev->dev_info->addr1;

	/*start repower*/
	val = RICOH_PWR_OFF;
	ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_PWR_SLP,val);
	return ret;
}

csi_error_t ricoh567_uninit(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
	csi_error_t ret;
	uint32_t val;
	uint16_t dev_addr = pmic_dev->dev_info->addr1;
    #warning "only for pmic 0??"
	/*disable wdt firstly*/
	ret = ricoh567_wdt_stop(pmic, pmic_dev);
	if (ret)
		return ret;

	/*disable repower on*/
	ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_PWR_REPCT,&val);
	if(ret)
		return ret;
	val &=~0x1;
	ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_PWR_REPCT,val);
	if(ret)
		return ret;

	/*start repower*/
	val = RICOH_PWR_OFF;
	ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_PWR_SLP,val);
	#warning "only for pmic 0??"
	while(1);
	return ret;
}

static csi_error_t ricoh567_regu_pso3_slp(csi_pmic_t *pmic,uint16_t dev_addr, uint32_t benable)
{
	csi_error_t ret;
	uint32_t val;

	val = benable? RICOH_PWR_SLEEP:RICOH_PWR_EXIT_SLEEP;
	ret =pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_PWR_SLP,val);
	return ret;
}

csi_error_t ricoh567_v0_init(csi_pmic_t *pmic , csi_pmic_dev_t *pmic_dev)
{
	csi_error_t ret;
	uint32_t val;

	uint16_t dev_addr = pmic_dev->dev_info->addr1;
	pmic_dev->dev_info->slew_rate = 14; //mv

	/*reset-out-signal hold 16ms,repower on delay 500ms after power-off seq finish,enable repower on*/
	val = (3<<4) | (2<<1) | (1<<0);
	ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_PWR_REPCT,val);
	if(ret)
		return ret;

	/*set power-off key press time :1 second*/
	ret = pmic->pmic_read_reg_sr(pmic,dev_addr,RICOH_REG_PWRONTIMSET,&val);
	if(ret)
		return ret;
	val &=~OFF_PRESS_TIME_MSK;
	val |= 1 << OFF_PRESS_TIM_POS;
	ret = pmic->pmic_write_reg(pmic,dev_addr,RICOH_REG_PWRONTIMSET,val);
	if(ret)
		return ret;
	if(pmic_dev->drv_data->features & PMIC_DRV_SUPPORT_AUTO_REBOOT) {
	     pmic_dev->dev_info->flag |= PMIC_DEV_ENABLE_AUTO_REBOOT;
	}
	return 0;
}

#warning "enable PMIC_DEV_ENABLE_AUTO_REBOOT"

csi_error_t ricoh567_v1_init(csi_pmic_t *pmic , csi_pmic_dev_t *pmic_dev)
{
	csi_error_t ret;
	uint32_t val;

	uint16_t dev_addr1 = pmic_dev->dev_info->addr1;
	pmic_dev->dev_info->slew_rate = 14; //mv

	/*disable gpio3 as pso */
	ret = pmic->pmic_read_reg_sr(pmic,dev_addr1,0x28,&val);
	if(ret){
		return CSI_ERROR;
	}
	val |=0xF0;
	ret = pmic->pmic_write_reg(pmic,dev_addr1,0x28,val);
	if(ret){
		return CSI_ERROR;
	}

	ret = pmic->pmic_read_reg_sr(pmic,dev_addr1,0x28,&val);
	if(ret){
		return CSI_ERROR;
	}

	/*init gpio3 as output */
	ret = pmic->pmic_read_reg_sr(pmic,dev_addr1,RICOH_REG_IOSEL,&val);
	if(ret){
		return CSI_ERROR;
	}

	val |=BIT(3);
	ret = pmic->pmic_write_reg(pmic,dev_addr1,RICOH_REG_IOSEL,val);


	/*enable PSO3 sleep slot  */
	ret = pmic->pmic_read_reg_sr(pmic,dev_addr1,RICOH_REG_PSO3_SLOT,&val);
	if(ret)
		return ret;
	val &=~RICOH_SLP_SLOT_MSK;
	ret = pmic->pmic_write_reg(pmic,dev_addr1,RICOH_REG_PSO3_SLOT,val);

	if(pmic_dev->drv_data->features & PMIC_DRV_SUPPORT_AUTO_REBOOT) {
	    pmic_dev->dev_info->flag |= PMIC_DEV_ENABLE_AUTO_REBOOT;
	}
	return ret;
}


static csi_regu_ops_t ricoh567_ops_v0 = {
	.set_voltage          = ricoh567_set_voltage,
	.get_voltage          = ricoh567_get_voltage,
	//.is_exist             = ricoh567_regu_exist,
	.regu_power_ctrl      = ricoh567_regu_power_ctrl,
	.init                 = ricoh567_v0_init,
	.uninit               = ricoh567_uninit
};

static csi_regu_ops_t ricoh567_ops_v1 = {
	.set_voltage          = ricoh567_set_voltage,
	.get_voltage          = ricoh567_get_voltage,
	//.is_exist             = ricoh567_regu_exist,
	.regu_power_ctrl      = ricoh567_regu_power_ctrl,
	.init                 = ricoh567_v1_init,
	.uninit               = ricoh567_uninit
};




csi_wdt_ops_t ricoh567_wdt_ops = {
    //.wdt_len       = WDT_TMR_LEN,               ///< defaut wdt timer len = 64S
    .set_timer_len = ricoh567_wdt_set_timer_len,
    .start         = ricoh567_wdt_start,
    .stop          = ricoh567_wdt_stop,
    .feed          = ricoh567_wdt_feed,
    .repower       = ricoh567_wdt_repower
};

struct csi_pmic_drv_data ricoh567_drv_v0 = {
	.features      = PMIC_DRV_SUPPORT_WDT | PMIC_DRV_SUPPORT_AUTO_REBOOT,
	.ops           = &ricoh567_ops_v0,
	.wdt_ops       = &ricoh567_wdt_ops,
	.regu_num      = ARRAY_SIZE(ricoh567_regulator_info_v0),
	.regu_info     =(csi_regu_t*) &ricoh567_regulator_info_v0,
};

struct csi_pmic_drv_data ricoh567_drv_v1 = {
	.ops           = &ricoh567_ops_v1,
	.regu_num      = ARRAY_SIZE(ricoh567_regulator_info_v1),
	.regu_info     =(csi_regu_t*) &ricoh567_regulator_info_v1,
};

