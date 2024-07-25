/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     drv/avfs.h
 * @brief    Header File for RNG Driver
 * @version  V1.0
 * @date     22. Dec 2020
 * @model    tng
 ******************************************************************************/
#ifndef _DRV_AVFS_H_
#define _DRV_AVFS_H_

#include "drv/common.h"
#include <stdint.h>
#include "drv/iic.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	AVFS_EVENT_TMP_LOW,                                                          ///< temperature low 
	AVFS_EVENT_TMP_HIGH,                                                         ///< temperature high
	AVFS_EVENT_PMIC_DONE,                                                        ///< volate change done by pmic 
	AVFS_EVENT_FREQ_DONE,                                                        ///< freq change done 
	AVFS_EVENT_PVT_DONE,                                                         ///< access pvt sensor done
	AVFS_EVENT_SW_AVFS_DONE,                                                     ///< sw avfs done 
	AVFS_EVENT_ERROR_FREQ_INVALID,                                               ///< freq not match
	AVFS_EVENT_ERROR_ITER_EXCEED,                                                ///< iteration times exceed 
	AVFS_EVENT_ERROR_ITER_BACK,                                                  ///< psensor thd value abnomal
	AVFS_EVENT_ERROR_COMM                                                        ///< common err 
}csi_avfs_event_t ;

typedef enum {
	AVFS_HW_PARAM_CPU_FREQ_PATTERN,
	AVFS_HW_PARAM_VM_ARRAY_LVL,
	AVFS_HW_PARAM_VM_LOGIC_LVL,
	AVFS_HW_PARAM_CGM_PATTERN,
	AVFS_HW_PARAM_CMU_GEAR_CFG,
	AVFS_HW_PARAM_PLL_SWITCH_CFG,
	AVFS_HW_PARAM_ACLK_TLD_CFG,
	AVFS_HW_PARAM_FPV_CFG,
	AVFS_HW_PARAM_PS_WEIGHT,
	AVFS_HW_PARAM_MAX
}csi_avfs_param_id_t;

typedef struct {
	csi_avfs_param_id_t        tag;
	uint8_t                    len;
	uint8_t                    pad[3];
	uint16_t                   *freq_pattern;
}csi_avfs_param_cpu_freq_pattern_t;

typedef struct {
	csi_avfs_param_id_t        tag;
	uint8_t                    len;
	uint8_t                    pad[3];
	uint8_t                    *mem_array_vltg;	
}csi_avfs_param_vm_array_t;

typedef struct {
	csi_avfs_param_id_t        tag;
	uint8_t                    len;
	uint8_t                    pad[3];
	uint8_t                    *mem_logic_vltg;	
}csi_avfs_param_vm_logic_t;

typedef struct {
	csi_avfs_param_id_t        tag;
	uint16_t                   fix_pattern;
	uint8_t                    len;
	uint8_t                    pad[1];
	uint16_t                    *inc_pattern;	
}csi_avfs_param_cgm_pattern_t;
typedef struct 
{
	uint32_t cfg0;
	uint32_t cfg1;
}pll_param_t;

typedef struct {
	csi_avfs_param_id_t        tag;
	uint8_t                    len;
	uint8_t                    pad[3];
	pll_param_t                *pll_param;
}csi_avfs_param_cmu_gear_t;


typedef struct {
	csi_avfs_param_id_t        tag;
	uint8_t                    len;
	uint8_t                    pad[3];
	uint8_t                    *pll_switch;
}csi_avfs_param_pll_switch_t;

typedef struct {
	csi_avfs_param_id_t        tag;
	uint8_t                    len;
	uint8_t                    pad[3];
	uint16_t                   *aclk_tld_cfg;
}csi_avfs_param_aclk_tld_t;

typedef struct {
	uint16_t                   pth;
	uint16_t                   v_tar;
}fpv_t;

typedef struct {
	csi_avfs_param_id_t        tag;
	uint8_t                    len;
	uint8_t                    pad[3];
	fpv_t                      *fpv;
}csi_avfs_param_fpv_t;

typedef struct {
	csi_avfs_param_id_t        tag;
	uint8_t                    len;
	uint8_t                    pad[3];
	uint8_t                    *pwt_low;
	uint8_t                    *pwt_nor;
	uint8_t                    *pwt_high;
}csi_avfs_param_psensor_weight_t;

typedef struct {
	uint32_t                   max_iterate_num:8;
	uint32_t                   bp_vsensor:     1;
	uint32_t                   bp_voltage:     1;
	uint32_t                   bp_psensor:     1;
	uint32_t                   bp_freq_down:   1;
	uint32_t                   bp_aclk:        1;  
}csi_avfs_mainctrl_cfg_t;

typedef  struct {
	uint8_t                    pmic_dvs_rate;
	uint8_t                    vol_margin;
	uint8_t                    vol_step;
	uint8_t                    dua_rail_mem_vltg_ena;
	uint16_t                   pmic_cnt_base;
}csi_avfs_pmic_cfg_t;

typedef struct {
	uint16_t                   ps_margin;
	uint16_t                   ts_warn_low;
	uint16_t                   ts_warn_high;
	uint16_t                   ts_thd_low;
	uint16_t                   ts_thd_high;
	uint16_t                   vs_thd_low;
	uint16_t                   vs_thd_high;
	uint8_t                    ps_sample_mode;
	uint8_t                    pad;
}csi_avfs_pvt_cfg_t;

typedef struct {
	uint32_t                   cgm_freq_down:1;
	uint32_t                   cgm_freq_dirc_up_cgm_en:1;
	uint32_t                   cgm_direct_up:1;    
	uint32_t                   pad:5;
	uint32_t                   ramp_rate:16;      
}csi_avfs_cgm_cfg_t;

typedef struct {
	uint16_t                   pll_lock_hd_time;                                 ///< pll lock hold time 
	uint16_t                   pll_disable_time;                                 ///< pll disable time
	uint16_t                   freq_stable_time;                                 ///< freq stable time
	uint16_t                   pll_lock_time;                                    ///< pll lock time 
	uint8_t                    clk_sw_time;                                      ///< clk switch time
	uint8_t                    lock_timer_enable;                                ///< flag for whether enable lock timer
}csi_avfs_cmu_cfg_t;

typedef struct 
{
	csi_iic_t             handle;                                                ///< external iic handle
	uint32_t              dev_addr;                                              ///< addr of iic dev
}pmic_if_iic_t;

typedef  enum {
	PMIC_IF_SPI_LITE,
	PMIC_IF_IIC,
	PMIC_IF_MAX
}pmic_if_t;

typedef struct csi_avfs csi_avfs_t;
struct csi_avfs {
	csi_dev_t             dev;                                                   ///< general hw-dev 
	uint8_t               type;                                                  ///< pmic interface type
	uint8_t               is_hw_mode;                                            ///< 0: sw mode, 1: hw mode
	uint8_t               pad[2];                                                ///< dummy byte
	pmic_if_iic_t         iic;                                                   ///< iic dev info
	csi_error_t (*pmic_write_reg)(uint32_t offset,uint32_t val);                 /// ops for write reg to pmic ic,only available when avfs in software mode
	csi_error_t (*pmic_read_reg )(uint32_t offset,uint32_t *val);                /// ops for read reg from pmic ic,only available when avfs in software mode
	void (*call_back)(csi_avfs_t *avfs, csi_avfs_event_t event, void *arg);      ///< event callback
	void *arg;                                                                   ///< User private param ,passed to user callback
	void *priv;
};

csi_error_t csi_avfs_spilite_init(csi_avfs_t *avfs);

csi_error_t csi_avfs_i2c_init(csi_avfs_t *avfs,uint32_t i2c_dev_addr);

void csi_avfs_uninit(csi_avfs_t *avfs);

csi_error_t csi_avfs_attach_callback(csi_avfs_t *avfs,void *callback, void *arg);

csi_error_t csi_avfs_detach_callback(csi_avfs_t *avfs);

csi_error_t csi_avfs_get_hw_constant_param(csi_avfs_param_id_t param_tag,void *pdata);

csi_error_t csi_avfs_set_hw_constant_param(csi_avfs_param_id_t param_tag,void *pdata);

csi_error_t csi_avfs_hw_enable(csi_avfs_t *avfs);

csi_error_t csi_avfs_hw_disable(csi_avfs_t *avfs);

csi_error_t csi_avfs_freq_change_request(csi_avfs_t *avfs,uint16_t freq_in, uint16_t *freq_out, uint32_t time_out);

csi_error_t csi_avfs_config_mainctrl(csi_avfs_t *avfs, csi_avfs_mainctrl_cfg_t *pdata);

csi_error_t csi_avfs_config_pmic(csi_avfs_t *avfs, csi_avfs_pmic_cfg_t *pdata);

csi_error_t csi_avfs_config_pvt(csi_avfs_t *avfs, csi_avfs_pvt_cfg_t *pdata);

csi_error_t csi_avfs_config_cgm(csi_avfs_t *avfs, csi_avfs_cgm_cfg_t *pdata);

csi_error_t csi_avfs_config_cmu(csi_avfs_t *avfs, csi_avfs_cmu_cfg_t *pdata);

csi_error_t csi_avfs_config_aclk(csi_avfs_t *avfs);

csi_error_t csi_avfs_start_avfs_monitor_timer(csi_avfs_t *avfs);

csi_error_t csi_avfs_stop_avfs_monitor_timer(csi_avfs_t *avfs);

csi_error_t csi_avfs_if_config_spilite(csi_avfs_t *avfs, uint32_t hz);

csi_error_t csi_avfs_if_config_iic(csi_avfs_t *avfs, csi_iic_addr_mode_t addr_mode,csi_iic_speed_t hz);














#ifdef __cplusplus
}
#endif

#endif /* _DRV_AVFS_H_ */
