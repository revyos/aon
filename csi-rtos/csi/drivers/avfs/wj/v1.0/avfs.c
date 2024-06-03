/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file    avfs.c
 * @brief
 * @version
 * @date     01. Dec 2020
 ******************************************************************************/

#include <drv/irq.h>
#include "wj_avfs_ll.h"
#include <drv/tick.h>
#include "drv/iic.h"
#include "drv/common.h"
#include <drv/avfs.h>

static void wj_avfs_irqhandler(void *args)
{
	return;
}
csi_error_t csi_avfs_spilite_init(csi_avfs_t *avfs)
{
	csi_error_t ret;
	wj_avfs_ctrl_regs_t *avfs_base;
	CSI_PARAM_CHK(avfs,CSI_ERROR);

	/* get dev info */
	ret = target_get(DEV_WJ_AVFS_TAG,0,&avfs->dev);
	if(ret){
		return ret;
	}

	/* init handle */
	avfs_base            = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);
	avfs->pmic_read_reg  = NULL;
	avfs->pmic_write_reg = NULL;
	avfs->call_back      = NULL;
	avfs->arg            = NULL;
	avfs->type           = PMIC_IF_SPI_LITE;
	avfs->priv           = NULL;

	/* disable avfs engine */
	wj_avfs_misc_hw_auto_disable(avfs_base);
	/* set pmic interface as spi-lite */
	wj_avfs_vol_set_if_dev(avfs_base,AVFS_PMIC_IF_SPI);

	return CSI_OK;
}

csi_error_t csi_avfs_i2c_init(csi_avfs_t *avfs, uint32_t i2c_idx)
{
	csi_error_t ret;
	wj_avfs_ctrl_regs_t *avfs_base;
	CSI_PARAM_CHK(avfs,CSI_ERROR);

	/* get dev info */
	ret = target_get(DEV_WJ_AVFS_TAG,0,&avfs->dev);
	if(ret){
		return ret;
	}

	/* init handle */
	avfs_base            = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);
	avfs->pmic_read_reg  = NULL;
	avfs->pmic_write_reg = NULL;
	avfs->call_back      = NULL;
	avfs->arg            = NULL;
	avfs->type           = PMIC_IF_SPI_LITE;
	avfs->priv           = NULL;

	/* disable avfs engine */
	wj_avfs_misc_hw_auto_disable(avfs_base);
	/* set pmic interface as spi-lite */
	wj_avfs_vol_set_if_dev(avfs_base,AVFS_PMIC_IF_I2C);

	ret = csi_iic_init(&(avfs->iic.handle),i2c_idx);
	if(ret) {
		return ret;
	}

	return CSI_OK;
}
static void avfs_disable_all_irq(wj_avfs_ctrl_regs_t *avfs_base)
{
	wj_avfs_intr_clear(avfs_base,WJ_AVFS_INTR_MASK);
	wj_avfs_intr_disable(avfs_base,WJ_AVFS_INTR_MASK);
	wj_avfs_intr_tmp_clear(avfs_base,WJ_AVFS_TMP_INTR_MASK);
	wj_avfs_intr_tmp_disable(avfs_base,WJ_AVFS_TMP_INTR_MASK);
}

void csi_avfs_uninit(csi_avfs_t *avfs)
{
	wj_avfs_ctrl_regs_t *avfs_base;
	CSI_PARAM_CHK_NORETVAL(avfs);

	if(avfs->type == AVFS_PMIC_IF_I2C) {
		csi_iic_uninit(&(avfs->iic.handle));
	}

	avfs_base            = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);
	avfs_disable_all_irq(avfs_base);
	csi_irq_disable(avfs->dev.irq_num);
	avfs->pmic_read_reg  = NULL;
	avfs->pmic_write_reg = NULL;
	avfs->call_back      = NULL;
	avfs->arg            = NULL;
	avfs->type           = PMIC_IF_MAX;
	avfs->priv           = NULL;

}

csi_error_t csi_avfs_attach_callback(csi_avfs_t *avfs, void *callback, void *arg)
{
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	CSI_PARAM_CHK(callback,CSI_ERROR);
	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);
	avfs->arg = arg;
	avfs->call_back = callback;
	uint32_t msk;

	wj_avfs_intr_clear(avfs_base,WJ_AVFS_INTR_MASK);
	wj_avfs_intr_tmp_clear(avfs_base,WJ_AVFS_TMP_INTR_MASK);

	/* enable all temperature irq */
	msk = WJ_AVFS_TMP_INTR_MASK;
	wj_avfs_intr_tmp_enable(avfs_base,msk);
	
	/* enable important intr irq */
	msk = WJ_AVFS_INTR_MASK;
	msk &=~(WJ_AVFS_INTR_VS_DONE_MASK | \
	        WJ_AVFS_INTR_TS_DONE_MASK | \
			WJ_AVFS_INTR_PS_DONE_MASK | \
			WJ_AVFS_INTR_PMIC_DONE_MASK);
	wj_avfs_intr_enable(avfs_base,msk);
	csi_irq_attach(avfs->dev.irq_num, wj_avfs_irqhandler, &avfs->dev);

	csi_irq_enable(avfs->dev.irq_num);

	return CSI_OK;
}

csi_error_t csi_avfs_detach_callback(csi_avfs_t *avfs)
{
	unsigned long status;
	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	csi_irq_disable(avfs->dev.irq_num);
	
	status = csi_irq_save();
	avfs->arg = NULL;
	avfs->call_back = NULL;
	csi_irq_restore(status);
	wj_avfs_intr_clear(avfs_base,WJ_AVFS_INTR_MASK);
	wj_avfs_intr_tmp_clear(avfs_base,WJ_AVFS_TMP_INTR_MASK);

	return CSI_OK;
}

csi_error_t csi_avfs_set_avfs_hw_constant_param(csi_avfs_t *avfs, csi_avfs_param_id_t param_tag, void *pdata)
{
	CSI_PARAM_CHK(pdata,CSI_ERROR);
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);	
	uint32_t item_num = 0;
	csi_error_t ret = CSI_ERROR;

	if(param_tag != *((csi_avfs_param_id_t*)pdata) ) {
		return CSI_ERROR;
	}

	switch(param_tag) {
		case AVFS_HW_PARAM_CPU_FREQ_PATTERN:
		{
			csi_avfs_param_cpu_freq_pattern_t *param = (csi_avfs_param_cpu_freq_pattern_t*)pdata;
			while (item_num < param->len && item_num < TAR_FREQ_LVL_MAX)
			{
				ret = wj_avfs_misc_set_freq_lvl(avfs_base,item_num,param->freq_pattern[item_num]);
				item_num++;
			}
		}	
		break;

		case AVFS_HW_PARAM_VM_ARRAY_LVL:
		{
			csi_avfs_param_vm_array_t *param = (csi_avfs_param_vm_array_t*)pdata;
			wj_avfs_vol_set_memarray_vltg(avfs_base,param->mem_array_vltg[0],\
			                                              param->mem_array_vltg[1],\
													      param->mem_array_vltg[2]);
		}
		break;

		case AVFS_HW_PARAM_VM_LOGIC_LVL:
		{
			csi_avfs_param_vm_logic_t *param = (csi_avfs_param_vm_logic_t*)pdata;
			wj_avfs_vol_set_memarray_logic_vltg(avfs_base,param->mem_logic_vltg[0], \
			                                                    param->mem_logic_vltg[1]);
		}
		break;

		case AVFS_HW_PARAM_CGM_PATTERN:
		{
			csi_avfs_param_cgm_pattern_t *param = (csi_avfs_param_cgm_pattern_t*)pdata;
			wj_avfs_cgm_set_fix_patern(avfs_base,param->fix_pattern);
			while (item_num < param->len && item_num < CGM_INC_PATTERN_NUM)
			{
				wj_avfs_cgm_set_inc_pattern(avfs_base,item_num,param->inc_pattern[item_num]);
				item_num++;
			}
		}
		break;

		case AVFS_HW_PARAM_CMU_GEAR_CFG:
		{
			csi_avfs_param_cmu_gear_t *param = (csi_avfs_param_cmu_gear_t*)pdata;
			if(param->len >= CGM_PLL_GEAR_CONFIG_NUM){
				ret = CSI_ERROR;
				break;
			}
			wj_avfs_cmu_set_pll_gear_params(avfs_base,(pll_cfg_t*)param->pll_param,param->len);
		}
		break;

		case AVFS_HW_PARAM_PLL_SWITCH_CFG:
		{
			csi_avfs_param_pll_switch_t* param = (csi_avfs_param_pll_switch_t*)pdata;
			wj_avfs_cmu_pll_set_switch_param(avfs_base,param->pll_switch,param->len);
		}
		break;

		case AVFS_HW_PARAM_ACLK_TLD_CFG:
		{
			csi_avfs_param_aclk_tld_t *param = (csi_avfs_param_aclk_tld_t*)pdata;
			while(item_num < param->len && item_num < FREQ_TLD_NUM)
			{
				wj_avfs_aclk_tld_config(avfs_base,item_num,param->aclk_tld_cfg[item_num]);
				item_num++;
			}
		}
		break;

		case AVFS_HW_PARAM_FPV_CFG:
		{
			csi_avfs_param_fpv_t *param = (csi_avfs_param_fpv_t*)pdata;
			while (item_num < param->len && item_num < FPV_TABLE_LEN)
			{
				wj_avfs_misc_config_fpv(avfs_base,item_num,param->fpv[item_num].pth,param->fpv[item_num].v_tar);	
				item_num++;
			}
		}
		break;

		case AVFS_HW_PARAM_PS_WEIGHT:
		{
			csi_avfs_param_psensor_weight_t *param = (csi_avfs_param_psensor_weight_t*)pdata;
			while (item_num < param->len && item_num < PS_WEIGHT_LEN)
			{
				wj_avfs_misc_config_psensor_low_weight(avfs_base,item_num,param->pwt_low[item_num]);
				wj_avfs_misc_config_psensor_normal_weight(avfs_base,item_num,param->pwt_nor[item_num]);
				wj_avfs_misc_config_psensor_high_weight(avfs_base,item_num,param->pwt_high[item_num]);
				item_num++;
			}
		}
		break;

		default:
			return CSI_UNSUPPORTED;
			
	}

	return ret;
}

csi_error_t csi_avfs_hw_disable(csi_avfs_t *avfs)
{
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);
	wj_avfs_misc_hw_auto_disable(avfs_base);
	return CSI_OK;
}

csi_error_t csi_avfs_hw_enable(csi_avfs_t *avfs)
{
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);
	wj_avfs_misc_hw_auto_enable(avfs_base);

	return CSI_OK;
}

csi_error_t csi_avfs_freq_change_request(csi_avfs_t *avfs,uint16_t freq_in, uint16_t *freq_out, uint32_t time_out)
{
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);
	uint32_t time_start = csi_tick_get_ms();

	/*wait avfs engine idle */
	while (wj_avfs_misc_is_state_busy(avfs_base))
	{
		if(csi_tick_get_ms() > time_start + time_out) {
			return CSI_TIMEOUT;
		}
	}
	
	wj_avfs_misc_set_target_freq(avfs_base,freq_in);
	wj_avfs_misc_freq_change_enable(avfs_base);
	
	/* wait avfs freq done */
	while (wj_avfs_misc_is_state_busy(avfs_base))
	{
		if(csi_tick_get_ms() > time_start + time_out) {
			return CSI_TIMEOUT;
		}
	}

	/* check freq change result */
	*freq_out = wj_avfs_cmu_get_cur_freq_lvl(avfs_base);
	if(*freq_out != freq_in) {
		return CSI_ERROR;
	}

	return CSI_OK;
}

csi_error_t csi_avfs_config_mainctrl(csi_avfs_t *avfs,csi_avfs_mainctrl_cfg_t *pdata)
{
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	CSI_PARAM_CHK(pdata,CSI_ERROR);

	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);
	wj_avfs_misc_wkmode_set_iteration_count(avfs_base,pdata->max_iterate_num);

	/* whether bypass aclk */
	if(pdata->bp_aclk) {
		wj_avfs_misc_wkmode_bypass_aclk(avfs_base,1);
	}
	else {
		wj_avfs_misc_wkmode_bypass_aclk(avfs_base,0);
	}

	/* whether bypass vsensor */
	if(pdata->bp_vsensor) {
		wj_avfs_misc_wkmode_bypass_vsensor(avfs_base,1);
	}
	else {
		wj_avfs_misc_wkmode_bypass_vsensor(avfs_base,0);
	}

	/* whether bypas vol-adjust */
	if(pdata->bp_voltage) {
		wj_avfs_misc_wkmode_bypass_voltage(avfs_base,1);
	}
	else {
		wj_avfs_misc_wkmode_bypass_voltage(avfs_base,0);
	}

	/* whether bypass psensor */
	if(pdata->bp_psensor) {
		wj_avfs_misc_wkmode_bypass_psensor(avfs_base,1);
	}
	else {
		wj_avfs_misc_wkmode_bypass_psensor(avfs_base,0);
	}

	/* whether bypass freq-down */
	if(pdata->bp_freq_down) {
		wj_avfs_misc_wkmode_bypass_freq_down(avfs_base,1);
	}
	else {
		wj_avfs_misc_wkmode_bypass_freq_down(avfs_base,0);
	}

	return CSI_OK;
}

csi_error_t csi_avfs_config_pmic(csi_avfs_t *avfs, csi_avfs_pmic_cfg_t *pdata)
{
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	CSI_PARAM_CHK(pdata,CSI_ERROR);

	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);

	wj_avfs_vol_cfg_margin(avfs_base,pdata->vol_margin);
	wj_avfs_vol_cfg_pmic_dvs_rate(avfs_base,pdata->pmic_dvs_rate);
	wj_avfs_vol_cfg_step_value(avfs_base,pdata->vol_step);
	wj_avfs_vol_set_counter(avfs_base,pdata->pmic_cnt_base);
	if(pdata->dua_rail_mem_vltg_ena) {
		wj_avfs_vol_dual_rail_mem_vltg_enable(avfs_base);
	}
	else {
		wj_avfs_vol_dual_rail_mem_vltg_disable(avfs_base);
	}

	return CSI_OK;
}

csi_error_t csi_avfs_config_pvt(csi_avfs_t *avfs, csi_avfs_pvt_cfg_t *pdata)
{
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	CSI_PARAM_CHK(pdata,CSI_ERROR);

	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);

	wj_avfs_sensor_set_pmargin(avfs_base,pdata->ps_margin);
	wj_avfs_sensor_set_tthd(avfs_base,pdata->ts_thd_low,pdata->ts_thd_high);
	wj_avfs_sensor_set_twarn(avfs_base,pdata->ts_warn_low,pdata->ts_warn_high);
	wj_avfs_sensor_set_vthd(avfs_base,pdata->vs_thd_low,pdata->vs_thd_high);
	wj_avfs_sensor_cfg_mgr_en(avfs_base,pdata->ps_sample_mode);

	return CSI_OK;
}

csi_error_t csi_avfs_config_cgm(csi_avfs_t *avfs, csi_avfs_cgm_cfg_t *pdata)
{
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	CSI_PARAM_CHK(pdata,CSI_ERROR);
	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);

	wj_avfs_cgm_set_ramp_rate(avfs_base,pdata->ramp_rate);
	if(pdata->cgm_direct_up) {
		wj_avfs_cgm_freq_direct_up_enable(avfs_base);
	}
	else {
		wj_avfs_cgm_freq_direct_up_disable(avfs_base);
	}

	if(pdata->cgm_freq_dirc_up_cgm_en) {
		wj_avfs_cgm_freq_cgm_up_enable(avfs_base);
	}
	else {
		wj_avfs_cgm_freq_cgm_up_disable(avfs_base);
	}
	
	if(pdata->cgm_freq_down) {
		wj_avfs_cgm_freq_down_bf_volup_enable(avfs_base);
	}
	else {
		wj_avfs_cgm_freq_down_bf_volup_disable(avfs_base);
	}

	return CSI_OK;
}

csi_error_t csi_avfs_config_cmu(csi_avfs_t *avfs, csi_avfs_cmu_cfg_t *pdata)
{
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	CSI_PARAM_CHK(pdata,CSI_ERROR);
	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);

	wj_avfs_cmu_set_clk_switch_wait_time(avfs_base,pdata->clk_sw_time);
	wj_avfs_cmu_set_pll_disable_time(avfs_base,pdata->pll_disable_time);
	wj_avfs_cmu_set_pll_lock_time(avfs_base,pdata->pll_lock_time);
	wj_avfs_cmu_set_pll_lock_hold_time(avfs_base,pdata->pll_lock_hd_time);
	wj_avfs_cmu_set_freq_stable_time(avfs_base,pdata->freq_stable_time);

	if(pdata->lock_timer_enable) {
		wj_avfs_cmu_pll_lock_timer_enable(avfs_base);
	}
	else {
		wj_avfs_cmu_pll_lock_timer_disable(avfs_base);
	}

	return CSI_OK;
}

csi_error_t csi_avfs_config_aclk(csi_avfs_t *avfs)
{
	return CSI_OK;
}

csi_error_t csi_avfs_start_avfs_monitor_timer(csi_avfs_t *avfs)
{
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);
	wj_avfs_misc_wkmode_periodic_sensor_detect(avfs_base,1);
	return CSI_OK;
}

csi_error_t csi_avfs_stop_avfs_monitor_timer(csi_avfs_t *avfs)
{
	CSI_PARAM_CHK(avfs,CSI_ERROR);
	wj_avfs_ctrl_regs_t *avfs_base = (wj_avfs_ctrl_regs_t*)HANDLE_REG_BASE(avfs);
	wj_avfs_misc_wkmode_periodic_sensor_detect(avfs_base,0);
	return CSI_OK;
}

csi_error_t csi_avfs_if_config_spilite(csi_avfs_t *avfs, uint32_t hz)
{

	return CSI_OK; 
}

csi_error_t csi_avfs_if_config_iic(csi_avfs_t *avfs, csi_iic_addr_mode_t addr_mode,csi_iic_speed_t hz)
{

	return CSI_OK;
}
