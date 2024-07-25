
/*
 *	Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/*******************************************************
 * @file       da9121.c
 * @brief      source file for pmic-ic
 * @version    V1.0
 * @date       23. Feb 2021
 * ******************************************************/
#include <drv/common.h>
#include <drv/pmic.h>
#include "pmic_rsc_def.h"
#include "pmic_help.h"
#include "da9121.h"
#include "syslog.h"

static csi_regu_t da9121_regulator_info[] =
{
	{
		.name   = "DA9121_ID_BUCK1",
		.id     = DA9121_ID_BUCK1,
		.min_uv = 300000,
		.max_uv = 1900000,
		.step_uv = 10000,
		.vreg = DA9121_BUCK1_A,
		.vmask = DA9121_VBUCK1_MASK,
		.ereg = DA9121_BUCK1_0,
		.emask = DA9121_BUCK1_0_CONT,
		.linear_min_sel = 0x1e,
        .initial_benable  = 1,
	},
};

static csi_error_t da9121_get_voltage(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t regu_id, uint32_t *target_uv)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);
	csi_error_t ret = CSI_ERROR;
	csi_regu_t *regu = NULL;
	uint32_t val;

    ret = pmic_get_regu_info(pmic_dev, regu_id, &regu);
    if(ret) {
        return ret;
    }

	if(!regu->vreg | !regu->vmask)
		   return CSI_UNSUPPORTED;

	ret = pmic->pmic_read_reg(pmic, PMIC_DEV_ADDR(1, pmic_dev), regu->vreg, &val);
	if(ret)
			return ret;
	val = (val & regu->vmask) - (regu->linear_min_sel);
	*target_uv = regu->min_uv + val * regu->step_uv;

	return ret;
}

static csi_error_t da9121_set_voltage(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev,uint32_t regu_id,uint32_t target_uv)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);

	csi_error_t ret = CSI_ERROR;
	csi_regu_t *regu = NULL;
	uint32_t val;

	ret = pmic_get_regu_info(pmic_dev, regu_id, &regu);
	if(ret) {
		return ret;
	}

	if(!regu->vreg | !regu->vmask)
			return CSI_UNSUPPORTED;

	val = (target_uv - regu->min_uv)/regu->step_uv + regu->linear_min_sel;
	ret = pmic->pmic_write_reg(pmic, PMIC_DEV_ADDR(1, pmic_dev), regu->vreg, val);
	if(ret)
			return ret;

	return ret;

}

static csi_error_t da9121_regu_power_ctrl(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev,uint32_t regu_id,uint32_t bon)
{
	CSI_PARAM_CHK(pmic,CSI_ERROR);
	CSI_PARAM_CHK(pmic_dev,CSI_ERROR);
	csi_error_t ret;
	csi_regu_t *regu = NULL;
	uint32_t val;

	ret = pmic_get_regu_info(pmic_dev, regu_id, &regu);
	if(ret) {
		return ret;
	}

	if(!regu->ereg | !regu->emask)
			return CSI_UNSUPPORTED;

	/* Enable\Disbale Bucks\LDOs */
	ret = pmic->pmic_read_reg(pmic, PMIC_DEV_ADDR(1, pmic_dev), regu->ereg, &val);
	if(ret)
			return ret;
	val = bon ? val | regu->emask : val & (~regu->emask);
	ret = pmic->pmic_write_reg(pmic, PMIC_DEV_ADDR(1, pmic_dev), regu->ereg, val);
	if(ret)
			return ret;


	return ret;
}

csi_error_t da9121_uninit(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
	UNUSED(pmic);
	return CSI_OK;
}

csi_error_t da9121_init(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
	/* set the voltage clamping slew rate */
	pmic_dev->dev_info->slew_rate = 20; //mv
	return CSI_OK;
}

static csi_regu_ops_t da9121_ops = {
	.set_voltage          = da9121_set_voltage,
	.get_voltage          = da9121_get_voltage,
	.regu_power_ctrl      = da9121_regu_power_ctrl,
	.init                 = da9121_init,
	.uninit               = da9121_uninit
};

struct csi_pmic_drv_data da9121_drv = {
	.ops           = &da9121_ops,
	.wdt_ops       = NULL,
	.regu_num      = ARRAY_SIZE(da9121_regulator_info),
	.regu_info     =(csi_regu_t*) &da9121_regulator_info,
};
