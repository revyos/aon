/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     aon_wdt.h
 * @brief    Header File for aon-wdt
 * @date     30. May 2020
 * @model    aon-wdt
 ******************************************************************************/

#ifndef __AON_WDT__
#define __AON_WDT__

#include "drv/pmic.h"
#include "drv/timer.h"

#ifdef __cplusplus
extern "C" {
#endif


#define AON_WDT_PMIC_TMR_LEN                              (32)
#define AON_WDT_DEF_MONITOR_TMR_LEN                       (AON_WDT_PMIC_TMR_LEN/8)


enum aon_rpc_wdg_svc_en {
    WDG_FUNC_UNKNOWN       = 0U,
    WDG_FUNC_WDG_START	   =  1U,
    WDG_FUNC_WDG_STOP	   =  2U,
    WDG_FUNC_WDG_PING	   =  3U,
    WDG_FUNC_WDG_TIMEOUTSET = 4U,
    WDG_FUNC_WDG_RESTART	= 5U,
    WDG_FUNC_WDG_GET_STATE	= 6U,
    WDG_FUNC_WDG_POWER_OFF	= 7U,
    WDG_FUNC_AON_WDT_ON		= 8U,
    WDG_FUNC_AON_WDT_OFF	= 9U,
};

typedef enum {
	AON_WDT_MONITOR_IDLE,
	AON_WDT_MONITOR_RUN,
	AON_WDT_MONITOR_REPOWER
}aont_fsm_t;

typedef enum {
   AON_HW_WDT_USE_PMIC   =  0U,
   AON_HW_WDT_USE_HW_WDT = 1U,
   AON_HW_WDT_SOFT_RST = 2U,
   AON_HW_WDT_CRASH_DUMP = 3U,
} aon_hw_wdt_en;

typedef struct {
   csi_pmic_t     *pmic_handle;
} hw_wdt_pmic_ctrl_t;

typedef struct {
    int type;
    union {
      hw_wdt_pmic_ctrl_t wdt_pmic;
    };
} hw_wdt_ctrl_info_t;

typedef struct {
	uint8_t        is_init:1;                          ///< init flag for this module
	uint8_t        hw_wdt_en:1;                      ///< hw_wdt_en flag for this module
	uint8_t        monitor_state;                      ///< fsm state for aon-wdt
	uint8_t        wdt_tmr_out_idx;                    ///< index of wdt under monitor
	uint8_t        monitor_tmr_len;                    ///< uinit in second
	csi_timer_t    timer_handle;                       ///< timer handle
	hw_wdt_ctrl_info_t  hw_wdt;                              ///< pmic-wdt handle
}aon_wdt_t;

extern aon_wdt_t g_aon_wdt;
/**
  \brief       init aon-wdt
  \param[in]   pmic_handle     handle for pmic
  \return      Error code \ref csi_error_t
*/
csi_error_t aon_wdt_init(aon_hw_wdt_en type);

/**
  \brief       start aon-wdt monitor
  \return      Error code \ref csi_error_t
*/
csi_error_t aon_wdt_start();
/**
  \brief       start pmic-wdt monitor
  \param[in]   aon_wdt    handle for aon_wdt
  \return      Error code \ref csi_error_t
*/
csi_error_t aon_wdt_start_pmic_wdt(aon_wdt_t *aon_wdt);

/**
  \brief       stop pmic-wdt monitor
  \param[in]   aon_wdt    handle for aon_wdt
  \return      Error code \ref csi_error_t
*/
csi_error_t aon_wdt_stop_pmic_wdt(aon_wdt_t *aon_wdt);

#ifdef __cplusplus
}
#endif

#endif
