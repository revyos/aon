/*
 * Copyright (C) 2021 Alibaba Group Holding Limited
 */

/******************************************************************************
  * @file     power.c
  * @brief    mpw aon low power
  * @version  V1.0
  * @date     27. May 2021
  ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <soc.h>
#include <drv/tick.h>
#include <drv/uart.h>
#include <drv/mbox.h>

#include <drv/pin.h>
#include <drv/pmic.h>
#include <csi_core.h>
#include "syslog.h"
#include "board_config.h"
#include "board_init.h"
#include "power.h"
#include "pmic_rsc_def.h"

#include "aon_common.h"

extern csi_mbox_t mbox;

#ifdef CONFIG_PM

static void lpm_process()
{
        uint32_t    chip_lp_mode;
        csi_pmic_t *pmic = aon_pmic_get_handler();
        if (!pmic) {
                LOG_E("Get pmic faild");
                return;
        }
        chip_lp_mode = chip_lpm_suspend();
        if (chip_lp_mode != CHIP_LP_NONE) {
                chip_lpm_resume(pmic, chip_lp_mode);
                LOG_D(" lowpower resume sucessfully return ..\n");
        }
}

static void lpm_dispatch(aon_rpc_msg_t *msg, aon_rpc_msg_t *tx_msg)
{
        uint8_t    func     = RPC_GET_FUNC(msg);
        aon_err_en result   = AON_ERR_NONE;
        uint8_t    ack_size = 0;

        switch (func) {
                case LPM_FUNC_AON_REQUIRE_STR: {
                        chip_lp_mode_set_lp_mode(SYS_LP_STANDBY);
                        LOG_D("STR->suspend to ram instruction received\n");
                        lpm_process();
                } break;
                case LPM_FUNC_AON_RESUME_STR: {
                        chip_lpm_finish();
                        LOG_D("system successfully return ..\n");
                        ack_size = 1U;
                } break;
                case LPM_FUNC_AON_CPUHP: {
                        uint16_t cpu_id;
                        uint16_t status;
                        RPC_GET_BE16(msg->data, 0U, &cpu_id);
                        RPC_GET_BE16(msg->data, 2U, &status);
                        LOG_D("cpuid: %d, status : %d\n", cpu_id, status);
                        if (status == LIGHT_AON_PM_PW_MODE_OFF)
                                chip_cpu_power_ctrl(cpu_id, 0);
                        else
                                chip_cpu_power_ctrl(cpu_id, 1);
                        ack_size = 1U + 4;
                        RPC_SET_BE16(tx_msg->data, 1, cpu_id);
                        RPC_SET_BE16(tx_msg->data, 3, status);
                } break;
                        /* Handle default */
                default: {
                        result = AON_ERR_NOTFOUND;
                } break;
        }
        /* Fill in header */
        RPC_SET_HEAD(tx_msg, AON_RPC_VERSION, ack_size, RPC_SVC_MSG_TYPE_ACK,
                     RPC_SVC_MSG_NO_NEED_ACK, RPC_GET_SVC_ID(msg), func,
                     result);
        return;
}

int aon_lpm_init(uint32_t wakeup_flag)
{
        chip_lp_mode_set_lp_mode(SYS_LP_NONE);
        chip_set_wakeup_flag(wakeup_flag);
        return aon_rpc_svc_cb_register(AON_RPC_SVC_LPM, lpm_dispatch);
}

#endif