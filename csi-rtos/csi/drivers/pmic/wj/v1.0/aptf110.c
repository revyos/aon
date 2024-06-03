
/*
 *	Copyright (C) 2022 Alibaba Group Holding Limited
 */

/*******************************************************
 * @file       aptf110.c
 * @brief      source file for soft pmic-ic
 * @version    V1.0
 * @date       23. Feb 2021
 * ******************************************************/
#include <drv/common.h>
#include <drv/helper.h>
#include <drv/pmic.h>
#include "pmic_rsc_def.h"
#include "aptf110.h"
#include "pmic_help.h"
#include "syslog.h"

#warning "fix me"
#define DCxSR               (10000)            ///< 10mv/us

#define IS_REGU_DUAL_RAIL(regu)                                                \
        ((regu.sub.id1 != 0xff && regu.sub.id2 != 0xff) ? 1 : 0)
#define WDT_TMR_TICK_LEN (2000)
#define WDT_TMR_LEN (WDT_TMR_TICK_LEN * 32)
#define VOLTAGE_DELAY_MARGIN (50)

extern csi_wdt_ops_t          aptf110_wdt_ops;

static csi_regu_t aptf110_regulator_info[] = {
    {
        .name   = "APTF110_ID_I2C_DC1",
        .id     = APTF110_ID_I2C_DC1,
        .min_uv = 720000,
        .max_uv           = 1500000,
        .step_uv          = 12500,
        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_I2C_DC2",
        .id               = APTF110_ID_I2C_DC2,
        .min_uv           = 720000,
        .max_uv           = 1500000,
        .step_uv          = 12500,
        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_PGOOD_DC1",
        .id               = APTF110_ID_PGOOD_DC1,
        .min_uv           = 800000,
        .max_uv           = 800000,
        .step_uv          = 10000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_PGOOD_DC2",
        .id               = APTF110_ID_PGOOD_DC2,
        .min_uv           = 1100000,
        .max_uv           = 1100000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_PGOOD_DC3",
        .id               = APTF110_ID_PGOOD_DC3,
        .min_uv           = 600000,
        .max_uv           = 600000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO1",
        .id               = APTF110_ID_LDO1,
        .min_uv           = 1800000,
        .max_uv           = 1800000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO2",
        .id               = APTF110_ID_LDO2,
        .min_uv           = 1800000,
        .max_uv           = 1800000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO3",
        .id               = APTF110_ID_LDO3,
        .min_uv           = 3300000,
        .max_uv           = 3300000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO4",
        .id               = APTF110_ID_LDO4,
        .min_uv           = 800000,
        .max_uv           = 800000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO5",
        .id               = APTF110_ID_LDO5,
        .min_uv           = 1800000,
        .max_uv           = 1800000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO6",
        .id               = APTF110_ID_LDO6,
        .min_uv           = 800000,
        .max_uv           = 800000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO7",
        .id               = APTF110_ID_LDO7,
        .min_uv           = 800000,
        .max_uv           = 800000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO8",
        .id               = APTF110_ID_LDO8,
        .min_uv           = 1800000,
        .max_uv           = 1800000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO9",
        .id               = APTF110_ID_LDO9,
        .min_uv           = 3300000,
        .max_uv           = 3300000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO10",
        .id               = APTF110_ID_LDO10,
        .min_uv           = 1800000,
        .max_uv           = 1800000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO11",
        .id               = APTF110_ID_LDO11,
        .min_uv           = 1800000,
        .max_uv           = 1800000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO12",
        .id               = APTF110_ID_LDO12,
        .min_uv           = 1800000,
        .max_uv           = 1800000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO13",
        .id               = APTF110_ID_LDO13,
        .min_uv           = 1200000,
        .max_uv           = 1200000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
    {
        .name             = "APTF110_ID_LDO14",
        .id               = APTF110_ID_LDO14,
        .min_uv           = 2800000,
        .max_uv           = 2800000,
        .step_uv          = 20000,

        .initial_benable          = 1,
    },
};



static inline int _aptf110_get_ctrl_id(uint32_t regu_id)
{
        if (regu_id < APTF110_ID_BASE + 1) {
                return CSI_ERROR;
        } else {
                return regu_id - APTF110_ID_BASE - 1;
        }
}

static csi_error_t aptf110_regu_power_ctrl(csi_pmic_t *    pmic,
                                           csi_pmic_dev_t *pmic_dev,
                                           uint32_t regu_id, uint32_t bon)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        CSI_PARAM_CHK(pmic_dev, CSI_ERROR);
        csi_error_t ret;
        csi_regu_t *regu         = NULL;
        uint32_t    id           = regu_id;
        uint8_t     ctrl_data[2] = {0x0};

        int actual_id = _aptf110_get_ctrl_id(regu_id);
        if (actual_id < 0) {
                return CSI_ERROR;
        }

        ret = pmic_get_regu_info(pmic_dev, id, &regu);
        if (ret) {
                return ret;
        }
#if PMIC_DEBUG
        LOG_D("aptf110:pwr_ctrl:%s regu_id %u actual id %u bon %u \n",
               regu->name, regu_id, actual_id, bon);
        //regu->benable = bon;
#endif

        ctrl_data[0] = actual_id;
        ctrl_data[1] = bon;
        ret          = pmic->pmic_write_reg_multi_bytes(pmic, PMIC_DEV_ADDR(1,pmic_dev),
                                               APTF110_REG_REGU_POWER,
                                               ctrl_data, sizeof(ctrl_data));
        if (ret) {
                return ret;
        }
        //regu->benable = bon;

        return ret;
}

static csi_error_t aptf110_get_voltage(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev,
                                  uint32_t regu_id, uint32_t *target_uv)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        CSI_PARAM_CHK(pmic_dev, CSI_ERROR);
        csi_error_t ret  = CSI_ERROR;
        csi_regu_t *regu = NULL;
        uint32_t    id   = regu_id;

        ret = pmic_get_regu_info(pmic_dev, id, &regu);
        if (ret) {
                return ret;
        }

        switch (id) {
                case APTF110_ID_I2C_DC1:
                case APTF110_ID_I2C_DC2: {
                        uint8_t  actual_id = _aptf110_get_ctrl_id(id);
                        uint8_t  reg_id    = actual_id + PMIC_CTRL_REGU0_VOL;
                        uint8_t  vol[4]    = {0x0};
                        uint8_t  len       = sizeof(vol);
                        uint32_t uv        = 0;
                        ret = pmic->pmic_read_reg_multi_bytes(
                            pmic, PMIC_DEV_ADDR(1,pmic_dev), reg_id, vol, &len);
                        if (ret) {
                              return CSI_ERROR;
                        } else {
                                uv = vol[0] << 24 | vol[1] << 16 | vol[2] << 8 |
                                     vol[3];
                                if (uv == 0xFFFFFFFF) {
                                        LOG_E("get uv faided\r\n");
                                      return CSI_ERROR;
                                } else {
                                        *target_uv = uv;
                                }
                        }
                } break;
                default:
                        *target_uv = regu->min_uv;
                        break;
        }

        return ret;
}

static inline csi_error_t set_single_step(csi_pmic_t *pmic, uint16_t dev_addr, \
                                                  uint32_t tar_vol, uint8_t id)
{
        int err = CSI_OK;

        uint8_t set_val[5] = {0x0};
        set_val[0]         = id;
        set_val[1]         = tar_vol >> 24 & 0xFF;
        set_val[2]         = tar_vol >> 16 & 0xFF;
        set_val[3]         = tar_vol >> 8 & 0xFF;
        set_val[4]         = tar_vol & 0xFF;

        err = pmic->pmic_write_reg_multi_bytes(
            pmic, dev_addr, APTF110_REG_REGU_VOL_SET, set_val, sizeof(set_val));
        if (err) {
                return CSI_ERROR;
        }

        return CSI_OK;
}

static csi_error_t aptf110_set_voltage(csi_pmic_t *          pmic,
                                       csi_pmic_dev_t *      pmic_dev,
                                       uint32_t regu_id, uint32_t target_uv)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        CSI_PARAM_CHK(pmic_dev, CSI_ERROR);

        csi_error_t   ret = CSI_OK;
        csi_regu_t *regu = NULL;
      
        ret = pmic_get_regu_info(pmic_dev,regu_id,&regu);
	if(ret) {
		return ret;
	}
        uint16_t dev_addr  =  PMIC_DEV_ADDR(1, pmic_dev);
        switch (regu_id) {
                case APTF110_ID_I2C_DC1: {
                       return set_single_step(pmic, dev_addr, target_uv, regu_id);
                } break;
                default:
                        return CSI_UNSUPPORTED;
        }
        return ret;
}

csi_error_t aptf110_gpio_output(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t gpio_index,
                                uint32_t val)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);

        return CSI_UNSUPPORTED;
}


csi_error_t aptf110_init(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
        /*do nothing for aptf110*/
        return CSI_OK;
}

csi_error_t aptf110_uninit(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
        csi_error_t ret          = CSI_OK;
        uint8_t     ctrl_data[1] = {0x0};
        ctrl_data[0]             = 0;
        ret = pmic->pmic_write_reg_multi_bytes(pmic, PMIC_DEV_ADDR(1,pmic_dev),
                                               APTF110_REG_ALL_REGU_POWER,
                                               ctrl_data, sizeof(ctrl_data));
        if (ret) {
                return ret;
        }
        //mdelay(30);
        /*power off aptf110 ,triger it enter rtc or delivery mode*/
        while (1)
                ;
        return ret;
}

csi_error_t aptf110_wdt_set_timer_len(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t msecond)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);

        pmic_dev->dev_info->wdt_len = msecond;

        return CSI_OK;
}

uint32_t aptf110_get_twdscale(uint32_t tick_scale)
{
        uint32_t tmp;
        if (tick_scale < 1) {
                tmp = 0;
        } else if (tick_scale < 2) {
                tmp = 1;
        } else if (tick_scale < 4) {
                tmp = 2;
        } else if (tick_scale < 8) {
                tmp = 3;
        } else if (tick_scale < 16) {
                tmp = 4;
        } else if (tick_scale < 32) {
                tmp = 5;
        } else if (tick_scale < 64) {
                tmp = 6;
        } else {
                tmp = 7;
        }

        return tmp;
}

csi_error_t aptf110_wdt_start(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);

        csi_error_t ret;
        uint32_t    tmp;
        uint8_t     val[2] = {0x0};

        /*disable wdt firstly*/
        val[0] = WDOG_START;
        val[1] &= ~0x7;
        tmp =
            (pmic_dev->dev_info->wdt_len + WDT_TMR_TICK_LEN - 1) / WDT_TMR_TICK_LEN;
        tmp = (uint32_t)aptf110_get_twdscale(tmp);

        val[1] |= tmp;
        ret = pmic->pmic_write_reg_multi_bytes(
            pmic, PMIC_DEV_ADDR(1,pmic_dev), APTF110_REG_WDOG_CTRL, val, sizeof(val));
        if (ret) {
                return ret;
        }

        /*set wdt enable flag*/
        //aptf110_wdt_ops.flag |= PMIC_WDT_ENABLE;

        return CSI_OK;
}

csi_error_t aptf110_wdt_stop(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev)
{
        csi_error_t ret;
        uint8_t     val[1] = {0x0};

        val[0] = WDOG_STOP;
        ret    = pmic->pmic_write_reg_multi_bytes(
            pmic, PMIC_DEV_ADDR(1,pmic_dev), APTF110_REG_WDOG_CTRL, val, sizeof(val));
        //aptf110_wdt_ops.flag &= (~PMIC_WDT_ENABLE);
        mdelay(1);

        return ret;
}

csi_error_t aptf110_wdt_feed(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);

        csi_error_t ret;
        uint8_t     val[1] = {0x00};

        val[0] = WDOG_FEED;

        ret = pmic->pmic_write_reg_multi_bytes(
            pmic, PMIC_DEV_ADDR(1,pmic_dev), APTF110_REG_WDOG_CTRL, val, sizeof(val));
        if (ret) {
                return ret;
        }

        return CSI_OK;
}

csi_error_t aptf110_wdt_repower(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
        csi_error_t ret;
        uint8_t     val[1] = {0x0};

        val[0] = PMIC_REPOWER_DELAY;
        ret    = pmic->pmic_write_reg_multi_bytes(
            pmic, PMIC_DEV_ADDR(1,pmic_dev), APTF110_REG_REBOOT, val, sizeof(val));

        return ret;
}

csi_error_t aptf110_log_level_set(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint8_t level)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        csi_error_t ret;

        ret = pmic->pmic_write_reg_multi_bytes(
            pmic, PMIC_DEV_ADDR(1,pmic_dev), APTF110_REG_LOG_LEVEL, &level, 1);
        if (ret) {
                return ret;
        }
        return CSI_OK;
}

csi_error_t aptf110_get_faults(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev)
{
        #if 0
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        CSI_PARAM_CHK(pmic_dev, CSI_ERROR);
        csi_error_t ret;
        uint8_t     len = 2;

        uint8_t faults_data[2] = {0x00};

        ret = pmic->pmic_read_reg_multi_bytes(
            pmic, PMIC_DEV_ADDR(1,pmic_dev), APTF110_REG_FAULTS_GET, faults_data, &len);
        if (ret) {
                return ret;
        }

        *code    = faults_data[0];
        *subcode = faults_data[1];
        #endif
        return CSI_OK;
}

static csi_regu_ops_t aptf110_ops = {
    .set_voltage     = aptf110_set_voltage,
    .get_voltage     = aptf110_get_voltage,
    .regu_power_ctrl = aptf110_regu_power_ctrl,
    //.is_exist        = aptf110_regu_exist,
    .gpio_output     = aptf110_gpio_output,
    .read_faults      = aptf110_get_faults,
    .set_log_level   = aptf110_log_level_set,
    .init            = aptf110_init,
    .uninit          = aptf110_uninit,
};

csi_wdt_ops_t aptf110_wdt_ops = {
    // .wdt_len       = WDT_TMR_LEN, ///< defaut wdt timer len = 64S
    .set_timer_len = aptf110_wdt_set_timer_len,
    .start         = aptf110_wdt_start,
    .stop          = aptf110_wdt_stop,
    .feed          = aptf110_wdt_feed,
    .repower       = aptf110_wdt_repower,
};

struct csi_pmic_drv_data aptf110_drv = {
    .features      = PMIC_DRV_SUPPORT_WDT | PMIC_DRV_SUPPORT_AUTO_REBOOT,
    .DCSR          = DCxSR,
    .ops           = &aptf110_ops,
    .wdt_ops       = &aptf110_wdt_ops,
    .regu_num =
        sizeof(aptf110_regulator_info) / sizeof(aptf110_regulator_info[0]),
    .regu_info     = (csi_regu_t *)&aptf110_regulator_info,
};
