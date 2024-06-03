
/*
 *	Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/*******************************************************
 * @file       pvt.c
 * @brief      source file for pvt-sensor controler
 * @version    V1.0
 * @date       23. Oct 2021
 * ******************************************************/
#include "stdio.h"
#include "string.h"
#include <soc.h>
#include <drv/common.h>
#include <drv/porting.h>
#include <drv/pvt.h>
#include <drv/irq.h>
#include <drv/helper.h>
#include <mr_pvt_ll.h>

static csi_pvt_regs_t g_pvt_reg_map;
typedef uint32_t (*sensor_done)(void *ipn, uint32_t idx);

csi_error_t pvt_sdif_wait_sample_done(void *ipn,sensor_type_t pvt_type, uint32_t sensor_idx, uint32_t tmr_us,uint32_t delay_us)
{
    uint32_t cnt = 0;
    sensor_done func;

    switch(pvt_type) {
        case PVT_PD:
            func = (sensor_done)mr_pvt_sdif_is_pd_sample_done;
            break;
        case PVT_TS:
            func = (sensor_done)mr_pvt_sdif_is_ts_sample_done;
            break;
        case PVT_VM:
            func =(sensor_done)mr_pvt_sdif_is_vm_sample_done;
            break;
        default:
            return CSI_ERROR;
    }

    while(cnt < tmr_us) {
        if(func(ipn,sensor_idx)) {
            break;
        }
        udelay(delay_us);
        cnt +=delay_us;
    }

    return (cnt > tmr_us) ? CSI_TIMEOUT : CSI_OK;
}

csi_error_t pvt_sdif_wait_idle(pvt_sdif_t *sdif, uint32_t tmr_us,uint32_t delay_us)
{
    uint32_t cnt = 0;

    while(cnt < tmr_us) {
        if(!mr_pvt_sdif_is_busy(sdif)) {
            break;
        }
        udelay(delay_us);
        cnt +=delay_us;
    }

    return (cnt > tmr_us) ? CSI_TIMEOUT : CSI_OK;
}

csi_error_t pvt_sdif_write_reg(pvt_sdif_t *sdif, uint32_t reg_addr,uint32_t reg_data)
{
   mr_pvt_sdif_write_ip_reg(sdif,reg_addr,reg_data);
   return pvt_sdif_wait_idle(sdif,PVT_POLL_TIMEOUT_US, PVT_POLL_DELAY_US);
}

csi_error_t pvt_sdif_read_reg(csi_pvt_t *pvt,
                              sensor_type_t type,
                              pvt_sdif_t *sdif,
                              uint32_t reg_addr,
                              uint32_t *reg_data)
{
   mr_pvt_sdif_read_ip_reg(sdif,reg_addr);
   if(pvt_sdif_wait_idle(sdif,PVT_POLL_TIMEOUT_US, PVT_POLL_DELAY_US)) {
       return CSI_TIMEOUT;
   }

   mr_pvt_sdif_read_regdata(pvt->reg_map,type,reg_data);

   return CSI_OK;
}

void pvt_clac_ip_sync(uint32_t pvt_clk,uint32_t *synth_hi, uint32_t *synth_lo, uint32_t *synth_stobe)
{
    uint32_t middle,key,high=8,low=4;

    /* sensor synth clk must range in [4,8]Mhz,get middle divider for synth_hi or synth_lo */
	while (high >= low) {
		middle = (low + high + 1) / 2;
		key = DIV_ROUND_CLOSEST(pvt_clk/1000000, middle);
		if (key > CLK_SYS_CYCLES_MAX) {
			low = middle + 1;
			continue;
		} else if (key < CLK_SYS_CYCLES_MIN) {
			high = middle - 1;
			continue;
		} else {
			break;
		}
	}

   if(key < CLK_SYS_CYCLES_MIN ) {
        key  = CLK_SYS_CYCLES_MIN;
   } else if ( key > CLK_SYS_CYCLES_MAX ){
        key = CLK_SYS_CYCLES_MAX;
   }
   key -=2;

   *synth_hi = key >> 1;
   *synth_lo = (key+1) >> 1;
   *synth_stobe = key >> 1;
}

csi_error_t pvt_ts_init_config(csi_pvt_t *pvt, uint32_t clk_synth)
{
    CSI_PARAM_CHK(pvt,CSI_ERROR);
    csi_error_t ret;
    uint32_t val;
    csi_pvt_regs_t *pvt_map =(csi_pvt_regs_t*)(pvt->reg_map);


    /* enable sdif sample-counter */
    mr_pvt_sdif_ena_counter(&pvt_map->ts_base->comm);

    /*disable sdif-halt */
    mr_pvt_sdif_halt_disable(&pvt_map->ts_base->comm);

    /*config clk_synth */
    mr_pvt_sdif_cfg_synth_clk(&pvt_map->ts_base->comm,clk_synth);

    /* enable all ip instance */
    mr_pvt_sdif_ipn_disable(&pvt_map->ts_base->comm,0);

    ret = pvt_sdif_wait_idle(&pvt_map->ts_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return ret;
    }

        /*config ip param ip_addr=1 ip_cfg*/
	val = TS_CFG0_MODE_1 | TS_CFG0_12_BIT;
    mr_pvt_sdif_write_ip_reg(&pvt_map->ts_base->comm,IP_CFG,val);
    ret = pvt_sdif_wait_idle(&pvt_map->ts_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return ret;
    }

    /*config ip param ip_addr=5 ip_timer*/
	val = POWER_DELAY_CYCLE_256;
    mr_pvt_sdif_write_ip_reg(&pvt_map->ts_base->comm,IP_TMR,val);
    ret = pvt_sdif_wait_idle(&pvt_map->ts_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return ret;
    }

    /*config ip param ip_addr=0 ip_ctrl*/
	val = IP_RST_REL | IP_RUN_CONT | IP_AUTO | IP_CLOAD;
    mr_pvt_sdif_write_ip_reg(&pvt_map->ts_base->comm,IP_CTRL,val);
    ret = pvt_sdif_wait_idle(&pvt_map->ts_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    return ret;
}

csi_error_t pvt_pd_init_config(csi_pvt_t *pvt, uint32_t clk_synth)
{
    CSI_PARAM_CHK(pvt,CSI_ERROR);
    csi_error_t ret;
    uint32_t val;
    csi_pvt_regs_t *pvt_map =(csi_pvt_regs_t*)(pvt->reg_map);

    /* enable sdif sample-counter */
    mr_pvt_sdif_ena_counter(&pvt_map->pd_base->comm);

    /*disable sdif-halt */
    mr_pvt_sdif_halt_disable(&pvt_map->pd_base->comm);

    /*config clk_synth */
    mr_pvt_sdif_cfg_synth_clk(&pvt_map->pd_base->comm,clk_synth);

    /* enable all ip instance */
    mr_pvt_sdif_ipn_disable(&pvt_map->pd_base->comm,0);

    ret = pvt_sdif_wait_idle(&pvt_map->pd_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return ret;
    }

    /*power up and release rst */
    val = IP_RST_REL;
    mr_pvt_sdif_write_ip_reg(&pvt_map->pd_base->comm,IP_CTRL,val);
    ret = pvt_sdif_wait_idle(&pvt_map->pd_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return ret;
    }

    /*config pd-sensor param */
    val = PD_CFG0_SER_MODE(1) |PD_CFG0_PRI_MODE(0) | PD_CFG0_SEC_MODE(0)|
		  PD_CFG1_PORT_SEL(1) | PD_CFG2_CWS(1) | PD_CFG2_PRE_SCALE_DIV(1);

    mr_pvt_sdif_write_ip_reg(&pvt_map->pd_base->comm,IP_CFG,val);
    ret = pvt_sdif_wait_idle(&pvt_map->pd_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return ret;
    }

    val = POWER_DELAY_CYCLE_64;
    mr_pvt_sdif_write_ip_reg(&pvt_map->pd_base->comm,IP_TMR,val);
    ret = pvt_sdif_wait_idle(&pvt_map->pd_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return ret;
    }

    val = IP_RST_REL | IP_RUN_ONCE | IP_AUTO;
    mr_pvt_sdif_write_ip_reg(&pvt_map->pd_base->comm,IP_CTRL,val);
    ret = pvt_sdif_wait_idle(&pvt_map->pd_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    return ret;
}

csi_error_t pvt_vm_init_config(csi_pvt_t *pvt, uint32_t clk_synth)
{
    CSI_PARAM_CHK(pvt,CSI_ERROR);
    csi_error_t ret;
    uint32_t val;
    csi_pvt_regs_t *pvt_map =(csi_pvt_regs_t*)(pvt->reg_map);

    /* enable sdif sample-counter */
    mr_pvt_sdif_ena_counter(&pvt_map->vm_base->comm);

    /*disable sdif-halt */
    mr_pvt_sdif_halt_disable(&pvt_map->vm_base->comm);

    /*config clk_synth */
    mr_pvt_sdif_cfg_synth_clk(&pvt_map->vm_base->comm,clk_synth);

    /* enable all ip instance */
    mr_pvt_sdif_ipn_disable(&pvt_map->vm_base->comm,0);

    ret = pvt_sdif_wait_idle(&pvt_map->vm_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return ret;
    }

    /*config vm ip */
    val =  VM_CFG0_PRI_MODE |VM_CFG0_RESLU |VM_CFG0_SER_MODE|VM_CFG0_VM_MEAS_MODE |
		   VM_CFG1_INPUT_SEL(0) | VM_CFG1_REF_SEL;
    mr_pvt_sdif_write_ip_reg(&pvt_map->vm_base->comm,IP_CFG,val);
    ret = pvt_sdif_wait_idle(&pvt_map->vm_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return ret;
    }

    /* config ip-poll */
    val =  0xFFFF | (1<<20);
    mr_pvt_sdif_write_ip_reg(&pvt_map->vm_base->comm,IP_POLL,val);
    ret = pvt_sdif_wait_idle(&pvt_map->vm_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return ret;
    }

    /* config ip timer */
    val = POWER_DELAY_CYCLE_64;
    mr_pvt_sdif_write_ip_reg(&pvt_map->vm_base->comm,IP_TMR,val);
    ret = pvt_sdif_wait_idle(&pvt_map->vm_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return ret;
    }

    /* config ip ctrl */
    val = IP_RST_REL | IP_RUN_CONT | IP_AUTO | IP_VM_MODE | IP_CLOAD;
    mr_pvt_sdif_write_ip_reg(&pvt_map->vm_base->comm,IP_CTRL,val);
    ret = pvt_sdif_wait_idle(&pvt_map->vm_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);

	return ret;
}

csi_error_t pvt_ip_config(csi_pvt_t *pvt,uint32_t pvt_idx)
{
    csi_error_t ret;
    uint32_t synth_hi,synth_lo,synth_stobe;
    uint32_t clk_synth;
    uint32_t pvtc_clk_ref = soc_get_pvtc_pclk(pvt_idx);
    /* calac ip synth for sensor */
    pvt_clac_ip_sync(pvtc_clk_ref,&synth_hi, &synth_lo, &synth_stobe);
    pvt->pvt_clk_factor = (pvtc_clk_ref/1000000)*100/((synth_hi << 1) +2);
	clk_synth = synth_lo << CLK_SYNTH_LO_SFT |
		        synth_hi << CLK_SYNTH_HI_SFT |
		        synth_stobe << CLK_SYNTH_HOLD_SFT | CLK_SYNTH_EN;

    /* if exit ts ,then init ts-sensor */
    if(pvt->sensor_info.ts_num) {
        ret = pvt_ts_init_config(pvt,clk_synth);
        if(ret) {
            return ret;
        }
    }

    /* if exit pd ,then init pd-sensor */
    if(pvt->sensor_info.pd_num) {
        ret = pvt_pd_init_config(pvt,clk_synth);
        if(ret) {
            return ret;
        }
    }

    /* if exit vm ,then init vm-sensor */
    if(pvt->sensor_info.vm_num) {
        ret = pvt_vm_init_config(pvt,clk_synth);
        if(ret) {
            return ret;
        }
    }

    return CSI_OK;
}

csi_error_t csi_pvt_init(csi_pvt_t *pvt, uint32_t pvt_idx)
{
    CSI_PARAM_CHK(pvt,CSI_ERROR);
    pvt_sensor_num_t sensor_num;
    csi_error_t ret;

    /*get ip dev info within specific soc*/
    if (target_get(DEV_MR_PVT_TAG,pvt_idx, &pvt->dev) != CSI_OK) {
        return CSI_ERROR;
    }

    /*init ip reg-map info*/
    g_pvt_reg_map.comm = (pvt_comm_t*)HANDLE_REG_BASE(pvt);
    g_pvt_reg_map.intr = (pvt_intr_t*)((uint8_t*)HANDLE_REG_BASE(pvt) + 0x40);
    g_pvt_reg_map.pd_base = (pd_map_t*)((uint8_t*)HANDLE_REG_BASE(pvt)+ MR_PVT_PD_OFFSET);
    g_pvt_reg_map.vm_base = (vm_map_t*)((uint8_t*)HANDLE_REG_BASE(pvt)+ MR_PVT_VM_OFFSET);
    g_pvt_reg_map.ts_base = (ts_map_t*)((uint8_t*)HANDLE_REG_BASE(pvt)+ MR_PVT_TS_OFFSET);
    /* get pvt sensor num */
    pvt->reg_map = (void*)&g_pvt_reg_map;
    mr_pvt_get_sensor_num((csi_pvt_regs_t*)&g_pvt_reg_map,&sensor_num);
    pvt->sensor_info.ch_num = sensor_num.ch_num;
    pvt->sensor_info.vm_num = sensor_num.vm_num;
    pvt->sensor_info.pd_num = sensor_num.pd_num;
    pvt->sensor_info.ts_num = sensor_num.ts_num;

    pvt->callback = NULL;

    /*check whether pvt is alvailable*/

    if(CSI_OK == mr_pvt_check_alive((csi_pvt_regs_t*)&g_pvt_reg_map)) {
        return CSI_ERROR;
    }
    ret = pvt_ip_config(pvt,pvt_idx);
    return ret;
}

void csi_pvt_uninit(csi_pvt_t *pvt)
{
    CSI_PARAM_CHK(pvt,CSI_ERROR);
    csi_pvt_detach_callback(pvt);
    pvt->arg = NULL;
    pvt->priv = NULL;
    pvt->reg_map = NULL;
    pvt->callback = NULL;
    memset(&pvt->sensor_info,0,sizeof(csi_pvt_sensor_info_t));
}

csi_error_t csi_pvt_read_ts(csi_pvt_t *pvt,uint32_t sensor_idx,uint32_t *raw_data)
{
    CSI_PARAM_CHK(pvt,CSI_ERROR);
    csi_error_t ret;
    csi_pvt_regs_t *pvt_map =(csi_pvt_regs_t*) pvt->reg_map;

    if(sensor_idx >= pvt->sensor_info.ts_num) {
        return CSI_ERROR;
    }

    ret = pvt_sdif_wait_sample_done(&pvt_map->ts_base->ipn,PVT_TS,sensor_idx,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return CSI_ERROR;
    }

    ret = mr_pvt_sdif_get_ts_sensor_data((ts_pd_ip_t*)&pvt_map->ts_base->ipn,sensor_idx,raw_data);
    return ret;
}

csi_error_t csi_pvt_pd_port_select(csi_pvt_t *pvt, uint32_t pd_port)
{
    CSI_PARAM_CHK(pvt,CSI_ERROR);
    csi_error_t ret;
    csi_pvt_regs_t *pvt_map =(csi_pvt_regs_t*) pvt->reg_map;
	uint32_t val;

    if(pd_port > MRS_PD_PORT_NUM)
        return CSI_ERROR;

    /*wait sdif bus idle*/
    ret = pvt_sdif_wait_idle(&pvt_map->pd_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
	if (ret)
		return ret;

	/*stop pd sensor */
	val = IP_RST_REL | IP_STOP |
	      SDIF_WRN_W | SDIF_PROG;
	mr_pvt_sdif_write_ip_reg(&pvt_map->pd_base->comm,IP_CTRL,val);
	ret = pvt_sdif_wait_idle(&pvt_map->pd_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
	if (ret)
		return ret;

	/* reconfig ip-config */
	val = PD_CFG0_PRI_MODE(0) |PD_CFG0_SEC_MODE(0) | PD_CFG0_SER_MODE(1)|
	      PD_CFG1_PORT_SEL(pd_port) | PD_CFG2_CWS(1) | PD_CFG2_PRE_SCALE_DIV(1);

	mr_pvt_sdif_write_ip_reg(&pvt_map->pd_base->comm,IP_CFG,val);
	ret = pvt_sdif_wait_idle(&pvt_map->pd_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
	if (ret)
		return ret;

	/*re-enable pd sensor*/
	val = IP_RST_REL | IP_RUN_ONCE | IP_AUTO  |
	      SDIF_WRN_W | SDIF_PROG;

	mr_pvt_sdif_write_ip_reg(&pvt_map->pd_base->comm,IP_CTRL,val);
	ret = pvt_sdif_wait_idle(&pvt_map->pd_base->comm,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);

	return ret;
}

csi_error_t csi_pvt_read_pd(csi_pvt_t *pvt,uint32_t sensor_idx,uint32_t *raw_data)
{
    CSI_PARAM_CHK(pvt,CSI_ERROR);
    csi_error_t ret;
    csi_pvt_regs_t *pvt_map =(csi_pvt_regs_t*) pvt->reg_map;

    if(sensor_idx >= pvt->sensor_info.pd_num) {
        return CSI_ERROR;
    }

    ret = pvt_sdif_wait_sample_done(&pvt_map->pd_base->ipn,PVT_PD,sensor_idx,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return CSI_ERROR;
    }

    ret = mr_pvt_sdif_get_pd_sensor_data((ts_pd_ip_t*)&pvt_map->pd_base->ipn,sensor_idx,raw_data);
    return ret;
}

csi_error_t pvt_get_vm_data(csi_pvt_t *pvt, uint32_t sensor_idx,uint32_t *raw_data)
{
    CSI_PARAM_CHK(pvt,CSI_ERROR);
    csi_error_t ret;
    csi_pvt_regs_t *pvt_map =(csi_pvt_regs_t*)pvt->reg_map;

    ret = pvt_sdif_wait_sample_done(&pvt_map->vm_base->ipn,PVT_VM,sensor_idx,PVT_POLL_TIMEOUT_US,PVT_POLL_DELAY_US);
    if(ret) {
        return CSI_ERROR;
    }

    ret = mr_pvt_sdif_get_vm_sensor_data((vm_ip_t*)&pvt_map->vm_base->ipn,sensor_idx,raw_data);
    return ret;
}

csi_error_t csi_pvt_read_vm_single_ch(csi_pvt_t *pvt,uint32_t sensor_idx,uint32_t vm_ch_idx,uint32_t *raw_data)
{
    CSI_PARAM_CHK(pvt,CSI_ERROR);
    csi_error_t ret;
    uint32_t vm_ch_data[MRS_VM_CH_NUM] = {0};

    if(sensor_idx >= pvt->sensor_info.vm_num || vm_ch_idx >= MRS_VM_CH_NUM) {
        return CSI_ERROR;
    }

    ret = pvt_get_vm_data(pvt,sensor_idx,(uint32_t*)&vm_ch_data);
    if(ret) {
        return ret;
    }

    *raw_data = vm_ch_data[vm_ch_idx];
    return CSI_OK;
}

csi_error_t csi_pvt_read_vm_muti_ch(csi_pvt_t *pvt,uint32_t sensor_idx,uint32_t vm_ch_sample_num,uint32_t *raw_data)
{
    CSI_PARAM_CHK(pvt,CSI_ERROR);
    csi_error_t ret;
    uint32_t vm_ch_data[MRS_VM_CH_NUM] = {0};
    uint32_t cnt = 0;

    if(sensor_idx >= pvt->sensor_info.vm_num || vm_ch_sample_num > MRS_VM_CH_NUM) {
        return CSI_ERROR;
    }

    ret = pvt_get_vm_data(pvt,sensor_idx,(uint32_t*)&vm_ch_data);
    if(ret) {
        return ret;
    }

    while(cnt < vm_ch_sample_num) {
        raw_data[cnt] = vm_ch_data[cnt];
        cnt++;
    }
    return CSI_OK;
}

csi_error_t csi_pvt_trig_ts_irq(csi_pvt_t *pvt, uint16_t sensor_idx, uint32_t tst_pattern)
{
	csi_pvt_regs_t *pvt_map = (csi_pvt_regs_t*)pvt->reg_map;
	if ( sensor_idx >= pvt->sensor_info.ts_num )
		return CSI_ERROR;
	tst_pattern &= TSPD_IRQ_MASK_ALL;
	mr_pvt_sdif_tst_irq(pvt_map->ts_base->ipn, sensor_idx, tst_pattern);
	return CSI_OK;
}

static void mr_pvt_ts_handler(void *args)
{
	csi_pvt_t *pvt = (csi_pvt_t*)args;
	csi_pvt_regs_t *pvt_map = (csi_pvt_regs_t*)pvt->reg_map;
	uint32_t idx = 0,temp;
	uint32_t ts_sensor_num = pvt->sensor_info.ts_num;
	csi_callback_info_t *param = (csi_callback_info_t*)pvt->arg;
	printf("%s\n",__func__);

	uint32_t global_irq_src;
	global_irq_src = mr_pvt_irq_global_ts_status(pvt_map);

	if (global_irq_src) {
		if (param) {
			memset((void*)param,0,sizeof(csi_callback_info_t));
			param->pvt_evt = PVT_TS_ALARM;
			param->irq_info.ts_irq_info.ts_ip_mask = global_irq_src;
		}

		while(idx < ts_sensor_num) {

			if (!(global_irq_src & (0x1 << idx))) {
				idx++;
				continue;
			}

			temp = mr_pvt_sdif_get_irq_status(pvt_map->ts_base->ipn,idx);

			/*record ts iqr info for user*/
			if (temp && param) {
				param->irq_info.ts_irq_info.ts_ip_high_alarm |= ((temp & TSPD_IRQ_ALARMA) ? 1 : 0) << idx;
				param->irq_info.ts_irq_info.ts_ip_low_alarm |= ((temp & TSPD_IRQ_ALARMB) ? 1 : 0) << idx;
				param->irq_info.ts_irq_info.ts_ip_fault |= ((temp & TSPD_IRQ_FAULT) ? 1 : 0) << idx;

			}

			/*clr int status*/
			mr_pvt_sdif_irq_clr(pvt_map->ts_base->ipn,idx,temp);
			idx++;
		}

		/*ts-event-callback*/
		if ( pvt->callback)
			pvt->callback(pvt,PVT_TS_ALARM,pvt->arg);
	}
}

static void mr_pvt_irq_handler(void *args)
{
	/*check whether triger ts irq event*/
	mr_pvt_ts_handler(args);

	/*check whether triger pd irq event*/

	/*check whether triger vm irq event*/

	return;
}

csi_error_t csi_pvt_create_ts_rising_alarm(csi_pvt_t *pvt, uint32_t sensor_idx, uint16_t hyst_thresh, uint16_t alarm_thresh)
{
    /*falling alarm: hyst_thresh must be less than alarm_thresh*/
    if (hyst_thresh >= alarm_thresh)
	    return CSI_ERROR;

    csi_pvt_regs_t *pvt_map =(csi_pvt_regs_t*)pvt->reg_map;
    mr_pvt_sdif_cfg_alarm_a(pvt_map->ts_base->ipn, sensor_idx, hyst_thresh, alarm_thresh);
    return CSI_OK;
}

csi_error_t csi_pvt_create_ts_falling_alarm(csi_pvt_t *pvt, uint32_t sensor_idx, uint16_t hyst_thresh, uint16_t alarm_thresh)
{
    /*falling alarm: hyst_thresh must be lager than alarm_thresh*/
    if (hyst_thresh <= alarm_thresh)
	    return CSI_ERROR;

    csi_pvt_regs_t *pvt_map =(csi_pvt_regs_t*)pvt->reg_map;
    mr_pvt_sdif_cfg_alarm_b(pvt_map->ts_base->ipn, sensor_idx, hyst_thresh, alarm_thresh);
    return CSI_OK;
}

csi_error_t csi_pvt_attach_callback(csi_pvt_t *pvt, void *callback, void *arg)
{
    pvt->callback = callback;
    pvt->arg = arg;
    csi_pvt_regs_t *pvt_map =(csi_pvt_regs_t*)pvt->reg_map;
    uint32_t ts_num = pvt->sensor_info.ts_num;
    uint32_t idx;
    uint32_t ts_ipn_mask;


    /*enable ts individual irq src*/
    for(idx = 0; idx < ts_num; idx++) {
	mr_pvt_sdif_irq_clr(pvt_map->ts_base->ipn, idx, TSPD_IRQ_MASK_ALL );
	mr_pvt_sdif_irq_ena(pvt_map->ts_base->ipn, idx, TSPD_IRQ_ALARMA | TSPD_IRQ_ALARMB);
    }

    /*enable global ts  irq src*/
    ts_ipn_mask = GENMASK(ts_num-1,0);
    mr_pvt_irq_global_ts_ena(pvt_map,ts_ipn_mask);
    mr_pvt_irq_global_ena(pvt_map,IRQ_EN_TS_IRQ_ENABLE);

    csi_irq_attach((uint32_t)pvt->dev.irq_num,mr_pvt_irq_handler,&pvt->dev);
    csi_irq_enable((uint32_t)pvt->dev.irq_num);

    return CSI_OK;
}

csi_error_t csi_pvt_detach_callback(csi_pvt_t *pvt)
{
    pvt->callback = NULL;
    pvt->arg = NULL;
    csi_pvt_regs_t *pvt_map =(csi_pvt_regs_t*)pvt->reg_map;

    mr_pvt_irq_global_ts_ena(pvt_map,0xFFFFFFFF);
    mr_pvt_irq_global_ena(pvt_map,0);
    csi_irq_disable((uint32_t)pvt->dev.irq_num);
    csi_irq_detach((uint32_t)pvt->dev.irq_num);

    return CSI_OK;
}

void csi_pvt_ts_convert_raw_data(csi_pvt_t *pvt, uint32_t raw_data, uint32_t *temp_mc)
{
    //uint32_t temp;
    uint64_t temp;
    /* use Mode1 output conversion(calibrated)
     * temp = A + B * Eqbs + C * Fclk
     * where:
     * Eqbs = (raw_data)/cal5 - 0.5
     * Fclk = sensor synth clk,unit in MHZ
     * A=42.74
     * cal5= 4094
     * B = 0.22
     * C = -0.16
     * *****************************************/


    temp = raw_data*1000;
    temp = temp/4094;
    *temp_mc = 42740 + 220 * (temp -500) -160*pvt->pvt_clk_factor/100;
}

void csi_pvt_vm_convert_raw_data(csi_pvt_t *pvt, uint32_t raw_data, uint32_t *vol_mv)
{
    /* use basic voltage conversion********************
     *
     * V = (Vref/5) * ( 6*raw_data/(2^14 -3/(2^R) -1)
     * where:
     * Vref = 1.2077
     * R = 12
     *
     * ************************************************/
    UNUSED(pvt);

	*vol_mv = (6*raw_data*1000/16384-3000/4096 -1000)*1207/5000;
}

void csi_pvt_pd_convert_raw_data(csi_pvt_t *pvt, uint32_t raw_data, uint32_t *speed_khz)
{

    /* digital output is converted to frequency by the followning equation
     *
     * Floop = raw_data * A * B * Fclk/W
     *
     * GateDelay = 1/(Floop * 2 *  Ngates)
     *
     * where:
     * A = 8
     * B = 4
     * Fclk = pvt->pvt_clk_factor/100 (uint in Mhz)
     * W = 127
     * Ngates =
     *
     * *******************************************************************/

    *speed_khz = raw_data * 8 * 4 * pvt->pvt_clk_factor*1000/(127 * 100);
}

