/*
 *	Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/*******************************************************
 * @file       slg51000.c
 * @brief      source file for pmic-ic
 * @version    V1.0
 * @date       10. may 2021
 * ******************************************************/
#include <drv/common.h>
#include <drv/pmic.h>
#include "pmic_rsc_def.h"
#include <pmic_help.h>
#include "slg51000.h"
#include "syslog.h"

#define CONFIG_SLG51000_WA

extern csi_pmic_dev_t slg51000_dev ;

/* Macros for LDO */
#define SLG51000_LDO(_id, min, max, step, en)	\
{						\
        .name   = "SLG51000_ID_LDO" # _id,	\
        .id     = SLG51000_ID_LDO ## _id,   	\
        .min_uv = min,                          \
        .max_uv = max,                          \
        .initial_benable = en,                  \
    	.step_uv = step,			\
        .vreg = SLG51000_LDO## _id##_VSEL,	\
        .vmask = SLG51000_VSEL_MASK,		\
        .ereg = SLG51000_SYSCTL_MATRIX_CONF_A,	\
        .emask = BIT(_id-1),		        \
}

static csi_regu_t slg51000_regulator_info[] =
{
    	SLG51000_LDO(1, 2200000, 3475000, 5000, 0),
    	SLG51000_LDO(2, 2200000, 3475000, 5000, 0),
    	SLG51000_LDO(3, 1200000, 3600000, 10000, 1),
    	SLG51000_LDO(4, 1200000, 3600000, 10000, 0),
    	SLG51000_LDO(5, 400000, 1675000, 5000, 0),
    	SLG51000_LDO(6, 400000, 1675000, 5000, 0),
    	SLG51000_LDO(7, 1200000, 3600000, 10000, 0),
};

static csi_error_t slg51000_get_voltage(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev,uint32_t regu_id,uint32_t *target_uv)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);
	csi_error_t ret = CSI_ERROR;
	csi_regu_t *regu = NULL;
	uint32_t val;

	ret = pmic_get_regu_info(pmic_dev, regu_id, &regu);
	if(ret)
	    return ret;

	if(!regu->vreg | !regu->vmask)
			return CSI_UNSUPPORTED;

	ret = pmic->pmic_read_reg(pmic, PMIC_DEV_ADDR(1,pmic_dev), regu->vreg, &val);
	if(ret)
			return ret;
	*target_uv = regu->min_uv + val * regu->step_uv;

#if PMIC_DEBUG
	LOG_D("slg51000:get_voltage:regu_id %u target_uv %u \n",regu_id,*target_uv);
#endif
	return ret;
}


static csi_error_t slg51000_set_voltage(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t regu_id, uint32_t target_uv)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);
	csi_error_t ret = CSI_ERROR;
	csi_regu_t *regu = NULL;
	uint32_t val;

	/* get regulator object */
	ret = pmic_get_regu_info(pmic_dev, regu_id, &regu);
	if(ret) {
		return ret;
	}

	if(!regu->vreg | !regu->vmask)
    		return CSI_UNSUPPORTED;

	val = (target_uv - regu->min_uv)/regu->step_uv;
	ret = pmic->pmic_write_reg(pmic, PMIC_DEV_ADDR(1,pmic_dev), regu->vreg, val);
	if(ret)
    		return ret;

	#if PMIC_DEBUG
	LOG_D("slg51000:set_voltage:regu_id %u target_uv %u \n",regu_id,target_uv);
	#endif

	return ret;
}

static csi_error_t slg51000_regu_power_ctrl(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t regu_id, uint32_t bon)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);

	csi_error_t ret;
	csi_regu_t *regu = NULL;
	uint32_t id = regu_id;
	uint32_t val;

	ret = pmic_get_regu_info(pmic_dev,id,&regu);
	if(ret) {
		return ret;
	}
	if(!regu->ereg | !regu->emask)
    		return CSI_UNSUPPORTED;

	/* Enable\Disbale Bucks\LDOs */
	ret = pmic->pmic_read_reg(pmic, PMIC_DEV_ADDR(1,pmic_dev), regu->ereg, &val);
	if(ret)
		return ret;
	val = bon ? val | regu->emask : val & (~regu->emask);
	ret = pmic->pmic_write_reg(pmic, PMIC_DEV_ADDR(1,pmic_dev), regu->ereg, val);
	if(ret)
    		return ret;
	#if PMIC_DEBUG
	LOG_D("slg51000: power control: regu_id %u bon%u \n",regu_id, bon);
	#endif
	return ret;
}

csi_error_t slg51000_uninit(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
	csi_error_t ret = CSI_OK;
	#warning "fix me"
	#if 0
	/* close all slg1/2/4/5/6/7 ldo*/
	ret = slg51000_regu_power_ctrl(pmic, &slg51000_dev, SLG51000_ID_LDO1, 0);
	if (ret )
		return ret;
	mdelay(1);
	ret = slg51000_regu_power_ctrl(pmic, &slg51000_dev, SLG51000_ID_LDO2, 0);
	if (ret )
		return ret;
	mdelay(1);
	ret = slg51000_regu_power_ctrl(pmic, &slg51000_dev, SLG51000_ID_LDO4, 0);
	if (ret )
		return ret;
	mdelay(1);
	ret = slg51000_regu_power_ctrl(pmic, &slg51000_dev, SLG51000_ID_LDO5, 0);
	if (ret )
		return ret;
	mdelay(1);
	ret = slg51000_regu_power_ctrl(pmic, &slg51000_dev, SLG51000_ID_LDO6, 0);
	if (ret )
		return ret;
	mdelay(1);
	ret = slg51000_regu_power_ctrl(pmic, &slg51000_dev, SLG51000_ID_LDO7, 0);
	mdelay(1);
    #endif
	return ret;
}

csi_error_t slg51000_init(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
	csi_error_t ret;

	/* set the voltag clamping slew rate */
	pmic_dev->dev_info->slew_rate = 10; //mv
#ifdef CONFIG_SLG51000_WA
#warning "fixme"
	/* unlock slg51000 protection */
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x111a,0x45);
	if (ret )
		return ret;

	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x111b,0x53);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x111c,0x54);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x111d,0x4d);
	if (ret )
		return ret;

       /* WA for slg51000 */
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x1106,0x04);
	if (ret )
		return ret;

	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x1107,0xb1);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x110d,0x00);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x1500,0x00);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x1503,0x00);
	if (ret )
		return ret;

	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x1715,0x02);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x1716,0x03);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x1717,0x0d);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x1718,0x05);
	if (ret )
		return ret;

	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x1719,0x06);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x171a,0x07);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x171b,0x08);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x2000,0x78);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x2200,0x3c);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x2365,0x30);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x2700,0xa0);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x2766,0x41);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x2900,0xa0);
	if (ret )
		return ret;
	ret = pmic->pmic_write_reg(pmic,PMIC_DEV_ADDR(1,pmic_dev),0x2966,0x41);
	if (ret )
		return ret;
#endif
    #warning "fix me"
	#if 0
	/* init open all slg1/2/4/5/6/7 ldo*/
	ret = slg51000_regu_power_ctrl(pmic, &slg51000_dev, SLG51000_ID_LDO1, 1);
	if (ret )
		return ret;
	ret = slg51000_regu_power_ctrl(pmic, &slg51000_dev, SLG51000_ID_LDO2, 1);
	if (ret )
		return ret;
	ret = slg51000_regu_power_ctrl(pmic, &slg51000_dev, SLG51000_ID_LDO4, 1);
	if (ret )
		return ret;
	ret = slg51000_regu_power_ctrl(pmic, &slg51000_dev, SLG51000_ID_LDO5, 1);
	if (ret )
		return ret;
	ret = slg51000_regu_power_ctrl(pmic, &slg51000_dev, SLG51000_ID_LDO6, 1);
	if (ret )
		return ret;
	ret = slg51000_regu_power_ctrl(pmic, &slg51000_dev, SLG51000_ID_LDO7, 1);
    #endif
	return CSI_OK;
}

static csi_regu_ops_t slg51000_ops = {
	.set_voltage      = slg51000_set_voltage,
	.get_voltage      = slg51000_get_voltage,
	//.is_exist         = slg51000_regu_exist,
	.regu_power_ctrl  = slg51000_regu_power_ctrl,
	.init             = slg51000_init,
	.uninit           = slg51000_uninit
};

struct csi_pmic_drv_data slg51000_drv = {
	.ops           = &slg51000_ops,
	.wdt_ops       = NULL,
	.regu_num      = sizeof(slg51000_regulator_info)/sizeof(slg51000_regulator_info[0]),
	.regu_info     =(csi_regu_t*) &slg51000_regulator_info,
};

