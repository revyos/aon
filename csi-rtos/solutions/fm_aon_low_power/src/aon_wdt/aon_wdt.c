/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
  * @file     aon_wdt.c
  * @brief    the main function for the timer driver
  * @version  V1.0
  * @date     31 May 2022
  ******************************************************************************/
#include <stdio.h>
#include <soc.h>
#include <drv/timer.h>
#include <board_init.h>
#include <board_config.h>
#include <drv/common.h>
#include <drv/pmic.h>
#include <drv/wdt.h>

#include "syslog.h"
#include "wdt.h"
#include "power.h"
#include "aon_wdt.h"
#include "aon_rpc.h"
#include "aon_sys.h"
#include "aon_pmic.h"
#include "aon_common.h"

aon_wdt_t g_aon_wdt;
uint32_t last_wdt_stat = 0;

static int aon_hw_wdt_set_time_len(aon_wdt_t *aon_wdt, uint32_t time_len)
{
        int ret = 0;
        /*start pmic-wdt*/

        if (g_aon_wdt.hw_wdt.type == AON_HW_WDT_USE_PMIC) {
                ret = csi_pmic_wdt_set_timer(
                    g_aon_wdt.hw_wdt.wdt_pmic.pmic_handle, time_len);
                if (ret) {
                        LOG_E("set pmic-wdt timer len err %d \n", ret);
                }
        } else {
              ret = 0;
        }

        return ret;
}

int aon_hw_wdt_repower(aon_wdt_t *aon_wdt)
{

        int ret          = 0;

        /*start pmic-wdt*/
        if (g_aon_wdt.hw_wdt.type == AON_HW_WDT_USE_PMIC) {
		if (!g_aon_wdt.hw_wdt_en) {
			g_aon_wdt.monitor_state = AON_WDT_MONITOR_RUN;
			return ret;
		}
		LOG_D(" Reboot type: PMIC RST"
                         "restarting......\n");
                ret = csi_pmic_repower(
                g_aon_wdt.hw_wdt.wdt_pmic.pmic_handle);
                if (ret) {
			LOG_E("pmic-wdt repower err %d", ret);}
	} else if(g_aon_wdt.hw_wdt.type == AON_HW_WDT_SOFT_RST) {
		if (!g_aon_wdt.hw_wdt_en) {
			g_aon_wdt.monitor_state = AON_WDT_MONITOR_RUN;
			return ret;
		}
		LOG_D(" Reboot type: SYSTEM SOFT RST"
                              "restarting......\n");
		system_reset();

		//never reaches here.
		__WFI();
	} else if(g_aon_wdt.hw_wdt.type == AON_HW_WDT_CRASH_DUMP) {
	        aon_regdump_handler();
		if (!g_aon_wdt.hw_wdt_en) {
			g_aon_wdt.monitor_state = AON_WDT_MONITOR_RUN;
			return ret;
		}
                LOG_D(" Reboot type: SYSTEM SOFT RST"
                              "restarting......\n");
	        system_reset();
	        //never reaches here.
	        __WFI();
	}else
    	{
		return -1;
	}

        return ret;
}

static int aon_hw_wdt_feed(aon_wdt_t *aon_wdt)
{
        int ret = 0;

        ret = csi_pmic_wdt_feed(g_aon_wdt.hw_wdt.wdt_pmic.pmic_handle);
        if (ret) {
				LOG_E("feed pmic-wdt timer err %d \n", ret);
        }

        return ret;
}

static void aon_wdt_monitor_tmr_callback(csi_timer_t *timer_handle, void *arg)
{
        aon_wdt_t *aon_wdt = (aon_wdt_t *)arg;
        static int index   = -1;
        uint32_t   wdt_tmr_out;

        switch (g_aon_wdt.monitor_state) {
                case AON_WDT_MONITOR_RUN: {
                        index = chip_wdt_check_timeout(aon_wdt, &wdt_tmr_out);

                        if (index > 0) {
                                g_aon_wdt.monitor_state =
                                    AON_WDT_MONITOR_REPOWER;
                                /*reset monitor-timer for repower*/
                        }
				if((g_aon_wdt.hw_wdt.type == AON_HW_WDT_USE_PMIC)
								&& (g_aon_wdt.hw_wdt_en))
					/*kick-off wdt */
					aon_hw_wdt_feed(aon_wdt);
                } break;
                case AON_WDT_MONITOR_REPOWER: {
                        LOG_E("###chip-wdt-device timeout: %d  ###\n", index);
                        mdelay(50);
			set_aon_rebootmode(CHIP_EVENT_SW_HANG);
                        aon_hw_wdt_repower(aon_wdt);
                } break;
                default: {
                        LOG_D("unexpected fsm-state:%d \n",
                              g_aon_wdt.monitor_state);
                } break;
        }

        return;
}

static int aon_wdt_init_monitor_timer(aon_wdt_t *aon_wdt, uint32_t tmr_sec)
{
        int ret = 0;

        ret = csi_timer_init((csi_timer_t *)&g_aon_wdt.timer_handle, 0);
        if (ret < 0) {
                return ret;
        }

        g_aon_wdt.monitor_tmr_len = tmr_sec;

        ret = csi_timer_attach_callback((csi_timer_t *)&g_aon_wdt.timer_handle,
                                        aon_wdt_monitor_tmr_callback, aon_wdt);
        if (ret < 0) {
                return ret;
        }

        return ret;
}

int aon_hw_wdt_start(aon_wdt_t *aon_wdt)
{
        int ret = 0;
        /*start pmic-wdt*/

        if (g_aon_wdt.hw_wdt_en == 1)
                return 0;

        if (g_aon_wdt.hw_wdt.type == AON_HW_WDT_USE_PMIC) {
                ret = csi_pmic_wdt_start(g_aon_wdt.hw_wdt.wdt_pmic.pmic_handle);
                if (ret) {
                        LOG_E("start pmic-wdt timer err %d \n", ret);
                }
        } else {
               ret = 0;
        }

        g_aon_wdt.hw_wdt_en = 1;

        return ret;
}

int aon_hw_wdt_stop(aon_wdt_t *aon_wdt)
{
        int ret = 0;

	if(!g_aon_wdt.hw_wdt_en)
		return ret;

        if (g_aon_wdt.hw_wdt.type == AON_HW_WDT_USE_PMIC) {
                /*stop backup wdt-timer */
                ret = csi_pmic_wdt_stop(g_aon_wdt.hw_wdt.wdt_pmic.pmic_handle);
                if (ret) {
                        LOG_E("stop pmic-wdt err\n");
                }
        }

        g_aon_wdt.hw_wdt_en = 0;
        return ret;
}

int aon_wdt_start()
{

        int ret;

        if (g_aon_wdt.monitor_state != AON_WDT_MONITOR_IDLE)
                return 0;

        /*start monitor-timer for wdt-time-out within ap-subsys and aud-subsys*/
        ret = csi_timer_start((csi_timer_t *)&g_aon_wdt.timer_handle,
				g_aon_wdt.monitor_tmr_len * 1000000);
        if (ret) {
                LOG_E("start monitor timer err %d \n", ret);
                return ret;
        }

        /*start backup wdt-timer to prevent system-lockup */
        ret = aon_hw_wdt_set_time_len(&g_aon_wdt, AON_WDT_PMIC_TMR_LEN);
        if (ret) {
                csi_timer_stop((csi_timer_t *)&g_aon_wdt.timer_handle);
                return ret;
        }

        /*start pmic-wdt*/
        g_aon_wdt.monitor_state = AON_WDT_MONITOR_RUN;

        last_wdt_stat  = chip_wdt_get_sta();
        printf("Last wdt sta 0x%x\n", last_wdt_stat);

        /*clear wdt-reset state*/
        chip_wdt_clear_reset();
        /*clear wdt-interrupt flag*/
        chip_wdt_clear_timeout();
        /*disable generating reset event of wdt within ap-subsys and aud-subsys*/
        chip_wdt_disable_reset();

        return 0;
}

int aon_wdt_stop()
{

	/* Return if watchdog is already detected timeout */
        if (g_aon_wdt.monitor_state == AON_WDT_MONITOR_REPOWER)
                return 0;

	/* Stop hw watchdog before disable sw watchdog */
	aon_hw_wdt_stop(&g_aon_wdt);
        /* stop watchdog timer*/
        csi_timer_stop((csi_timer_t *)&g_aon_wdt.timer_handle);

	g_aon_wdt.monitor_state = AON_WDT_MONITOR_IDLE;

	return 0;
}

static void wdg_dispatch(aon_rpc_msg_t *msg, aon_rpc_msg_t *tx_msg)
{
        uint8_t    func     = RPC_GET_FUNC(msg);
        aon_err_en result   = AON_ERR_NONE;
        uint8_t    ack_size = 0;
        int        ret      = 0;

        switch (func) {
                case WDG_FUNC_WDG_GET_STATE: {
                        uint32_t state = 0;
                        ack_size       = 1 + 4 + 4;
                        /*fix me timeout set to 0*/
                        RPC_SET_BE32(tx_msg->data, 1, 0);
                        RPC_SET_BE32(tx_msg->data, 5, U32(state));
                } break;
                case WDG_FUNC_WDG_START: {
                        ret = aon_wdt_start();
                        if (ret < 0) {
                                result = AON_ERR_FAIL;
                        }
                        ack_size = 1U;
                } break;
                case WDG_FUNC_WDG_STOP: {
                        ret = aon_wdt_stop();
                        if (ret < 0) {
                                result = AON_ERR_FAIL;
                        }
                        ack_size = 1U;
                } break;
                case WDG_FUNC_WDG_PING: {

                        ret = aon_hw_wdt_feed(&g_aon_wdt);
                        if (ret < 0) {
                                result = AON_ERR_FAIL;
                        }
                        ack_size = 1U;
                } break;
                case WDG_FUNC_WDG_TIMEOUTSET: {
                        uint32_t wdt_len;

                        RPC_GET_BE32(msg->data, 0U, &wdt_len);
                        ret = aon_hw_wdt_set_time_len(&g_aon_wdt, wdt_len);
                        if (ret) {
                                result = AON_ERR_FAIL;
                        }

                        /* Copy in return parameters */
                        LOG_D("pmic wdt set timer len=%d \n", wdt_len);
                        ack_size = 1U;

                } break;
                case WDG_FUNC_WDG_RESTART: {
                        /* c902 reboot soc has two modes: pmic watchdogtimout and soc reset
		 * pmic watchdog timout which is used in soc HANG.
		 * soc reset which is used in normal reboot mode, only reset soc
		 * and pmic output ldo/bulk retain unchanged.
		 */
                        LOG_I("starting system reset now......\n");
                        mdelay(1000);
                        system_reset();
                } break;
                case WDG_FUNC_WDG_POWER_OFF: {
                        LOG_I("Power off system......\n");
                        aon_pmic_poweroff();
                } break;
                case WDG_FUNC_AON_WDT_ON: {
                        ret = aon_hw_wdt_start(&g_aon_wdt);
                        if (ret != 0) {
                                result = AON_ERR_FAIL;
                        }
                        ack_size = 1U;
                } break;
                case WDG_FUNC_AON_WDT_OFF: {
                        ret = aon_hw_wdt_stop((aon_wdt_t *)&g_aon_wdt);
                        if (ret != 0) {
                                LOG_D("ERR->aon_wdt_stop_pmic_wdt failed\n");
                                result = AON_ERR_FAIL;
                        }
                        ack_size = 1U;
                } break;
                        /* Handle default */
                default: {
                        ack_size = 1;
                        result   = AON_ERR_NOTFOUND;
                } break;
        }
                                /* Fill in header */
        RPC_SET_HEAD(tx_msg, AON_RPC_VERSION, ack_size,
                                     RPC_SVC_MSG_TYPE_ACK,
                                     RPC_SVC_MSG_NO_NEED_ACK,
                                     RPC_GET_SVC_ID(msg), func, result);
        return;
}

int aon_wdt_init(aon_hw_wdt_en type)
{
        int ret;
        uint32_t autoboot_ena = 0;

        if (g_aon_wdt.is_init)
                return 0;

        ret = aon_wdt_init_monitor_timer(&g_aon_wdt, AON_WDT_DEF_MONITOR_TMR_LEN);
        if (ret) {
                return ret;
        }

        ret = chip_wdt_init();
        if (ret) {
                return ret;
        }

        if (type == AON_HW_WDT_USE_PMIC) {
                g_aon_wdt.hw_wdt.wdt_pmic.pmic_handle = aon_pmic_get_handler();
                if (!g_aon_wdt.hw_wdt.wdt_pmic.pmic_handle) {
                        LOG_E("get pmic handler faild\n");
                        return -1;
                }
				ret = csi_pmic_get_autoboot_flag(
						g_aon_wdt.hw_wdt.wdt_pmic.pmic_handle, &autoboot_ena);
				if (ret < 0) {
					LOG_E("pmic wdt is not supported\n");
					return -1;
				}

				/* with autoboot function, the system is capable of reboot automotically
				 * without any
				 * */
                if (!autoboot_ena) {
					type = AON_HW_WDT_SOFT_RST;
				}
        }

        g_aon_wdt.monitor_state = AON_WDT_MONITOR_IDLE;
        g_aon_wdt.is_init       = 1;
        g_aon_wdt.hw_wdt_en     = 0;
        g_aon_wdt.hw_wdt.type   = type;
        return aon_rpc_svc_cb_register(AON_RPC_SVC_WDG, wdg_dispatch);
}


