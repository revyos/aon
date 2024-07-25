/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file       drv/pwmr.h
 * @brief      Header File for PWMR Driver
 * @version    V1.0
 * @date       9. Oct 2020
 * @model      pwmr
 ******************************************************************************/

#ifndef _DRV_PWMR_H_
#define _DRV_PWMR_H_

#include <drv/common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PWMR_EVENT_COUNT_ZERO = 0U,                  ///< Capture Posedge Event
    PWMR_EVENT_COUNT_EQU_TOP,                    ///< Capture Negedge Event
    PWMR_EVENT_COUNT_EQU_COMP_UP,                ///< Capture Bothedge Event
    PWMR_EVENT_COUNT_EQU_COMP_DOWN,              ///< Capture Bothedge Event
    PWMR_EVENT_OVERTIME,                         ///< Error
} csi_pwmr_event_t;

typedef struct csi_pwmr csi_pwmr_t;

struct csi_pwmr {
    csi_dev_t      dev;
    void (*callback)(csi_pwmr_t *pwmr, csi_pwmr_event_t event, uint32_t ch, void *arg);
    void           *arg;
    void           *priv;
};

/**
  \brief       Initialize PWMR interface. Initializes the resources needed for the PWMR interface
  \param[in]   pwmr    Handle to operate
  \param[in]   idx    PWMR idx
  \return      Error code \ref csi_error_t
*/
csi_error_t csi_pwmr_init(csi_pwmr_t *pwmr, uint32_t idx);

/**
  \brief       De-initialize PWMR interface. Stops operation and releases the software resources used by the interface
  \param[in]   pwmr    Handle to operate
  \return      None
*/
void csi_pwmr_uninit(csi_pwmr_t *pwmr);

/**
  \brief       Config PWMR out mode
  \param[in]   pwmr              Handle to operate
  \param[in]   channel           Channel num
  \param[in]   duty              The PWMR high duty cycle.0~1000 means 0~100%.
  \param[in]   frequency         The PWMR frequency.Min value is apb0/4,max vlaue is apb0/8000000
  \return      Error code \ref csi_error_t
*/
csi_error_t csi_pwmr_out_config(csi_pwmr_t *pwmr,
                                uint32_t channel,
                                uint32_t duty,
                                uint32_t frequency);

/**
  \brief       Start generate PWMR signal
  \param[in]   pwmr       Handle to operate
  \param[in]   channel    Channel num
  \return      Error code \ref csi_error_t
*/
csi_error_t csi_pwmr_out_start(csi_pwmr_t *pwmr, uint32_t channel);

/**
  \brief       Stop generate PWMR signal
  \param[in]   pwmr       Handle to operate
  \param[in]   channel    Channel num
  \return      None
*/
void csi_pwmr_out_stop(csi_pwmr_t *pwmr, uint32_t channel);

/**
  \brief       Attach PWMR callback
  \param[in]   pwmr         Handle to operate
  \param[in]   callback    Callback func
  \param[in]   arg         Callback's param
  \return      Error code \ref csi_error_t
*/
csi_error_t csi_pwmr_attach_callback(csi_pwmr_t *pwmr, void *callback, void *arg);

/**
  \brief       Detach PWMR callback
  \param[in]   pwmr    Handle to operate
  \return      None
*/
void csi_pwmr_detach_callback(csi_pwmr_t *pwmr);

/**
  \brief       Enable PWMR power manage
  \param[in]   pwmr    Handle to operate
  \return      Error code \ref csi_error_t
*/
csi_error_t csi_pwmr_enable_pm(csi_pwmr_t *pwmr);

/**
  \brief       Disable PWMR power manage
  \param[in]   pwmr    Handle to operate
  \return      None
*/
void csi_pwmr_disable_pm(csi_pwmr_t *pwmr);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_PWMR_H_ */
