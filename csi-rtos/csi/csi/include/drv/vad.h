/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     drv/vad.h
 * @brief    header file for vad driver
 * @version  V1.0
 * @date     4. Nov 2020
 * @model    vad
 ******************************************************************************/

#ifndef _DRV_VAD_H_
#define _DRV_VAD_H_

#include <stdint.h>
#include <stdbool.h>
#include <drv/common.h>
#include <drv/dma.h>
#include "drv/ringbuffer.h"
#include <drv/list.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    VAD_DAI_MODE_PDM,                                    ///< VAD DAI is PDM interface
    VAD_DAI_MODE_I2S,                                    ///< VAD DAI is I2S interface
} drv_vad_dai_mode_t;

typedef enum {
    VAD_PDM_MODE_LOW_POWER,                              ///< PDM mode is low power mode
    VAD_PDM_MODE_STANDARD,                               ///< PDM mode is standard mode
} drv_vad_pdm_mode_t;

typedef enum {
    VAD_PDM_GAIN_0DB,                                    ///< PDM gain is  0 DB
    VAD_PDM_GAIN_6DB,                                    ///< PDM gain is  6 DB
    VAD_PDM_GAIN_12DB,                                   ///< PDM gain is 12 DB
    VAD_PDM_GAIN_18DB,                                   ///< PDM gain is 18 DB
    VAD_PDM_GAIN_24DB,                                   ///< PDM gain is 24 DB
    VAD_PDM_GAIN_30DB,                                   ///< PDM gain is 30 DB
    VAD_PDM_GAIN_36DB,                                   ///< PDM gain is 36 DB
    VAD_PDM_GAIN_42DB,                                   ///< PDM gain is 42 DB
} drv_vad_pdm_gain_t;

typedef enum {
    VAD_I2S_MODE_MASTER,                                 ///< I2S transmitter master mode
    VAD_I2S_MODE_SLAVE,                                  ///< I2S transmitter slave mode
} drv_vad_i2s_mode_t;

typedef enum {
    VAD_I2S_PROTOCOL_I2S,                                    ///< I2S protocol
    VAD_I2S_PROTOCOL_MSB_JUSTIFIED,                          ///< MSB (left) justified protocol
    VAD_I2S_PROTOCOL_LSB_JUSTIFIED,                          ///< LSB (right) justified protocol
    VAD_I2S_PROTOCOL_PCM,                                    ///< PCM protocol
} drv_vad_i2s_protocol_t;

typedef enum {
    VAD_I2S_SAMPLE_WIDTH_8BIT  = 8U,                        ///< VAD sample width is 8bit
    VAD_I2S_SAMPLE_WIDTH_16BIT = 16U,
    VAD_I2S_SAMPLE_WIDTH_24BIT = 24U,
    VAD_I2S_SAMPLE_WIDTH_32BIT = 32U,
} drv_vad_sample_width_t;

typedef struct {
    drv_vad_pdm_mode_t             pdm_mode;           ///< PDM mode
    drv_vad_pdm_gain_t             pdm_gain;           ///< PDM gain
} drv_vad_dai_pdm_format_t;

typedef struct {
    drv_vad_i2s_mode_t             i2s_mode;           ///< I2S mode
    drv_vad_i2s_protocol_t         i2s_protocol;       ///< I2S protocol
    drv_vad_sample_width_t         i2s_width;          ///< I2S sample width
} drv_vad_dai_i2s_format_t;

typedef struct {
    drv_vad_dai_mode_t             dai_mode;                ///< VAD DAI mode
    drv_vad_dai_pdm_format_t       vad_pdm_format;          ///< DAI PDM format
    drv_vad_dai_i2s_format_t       vad_i2s_format;          ///< DAI I2S format
} drv_vad_format_t;

typedef enum {
    VAD_EVENT_WAKE_UP,
    VAD_EVENT_RECEIVE_COMPLETE,
    VAD_EVENT_ERROR
} drv_vad_event_t;

typedef struct drv_vad drv_vad_t;
typedef struct drv_vad_ch drv_vad_ch_t;

struct drv_vad_ch {
    drv_vad_t               *vad;
    void                    *arg;               ///< user private param passed to user callback
    uint32_t                ch_idx;
    void                    *data;              ///< VAD cache data
    uint32_t                size;
    csi_dma_ch_t            *dma;               ///< VAD ch dma channel handle
    csi_state_t             state;              ///< VAD communication state
    void                    *priv;
    slist_t                 next;
};

struct drv_vad {
    csi_dev_t               dev;                ///< VAD hw-device info
    void                    *arg;               ///< user private param passed to user callback
    void (*callback)(drv_vad_t *dev, uint32_t ch_idx, drv_vad_event_t event, void *arg); ///< VAD callback for user
    slist_t                 head;               ///< VAD ch handle
    void                    *priv;
};

/**
  \brief       Init VAD dev
  \param[in]   VAD    VAD handle to operate
  \param[in]   idx    VAD interface idx
  \return      error code \ref csi_error_t
*/
csi_error_t drv_vad_init(drv_vad_t *vad, uint32_t idx);

/**
  \brief       Uninit VAD
  \param[in]   VAD    VAD handle to operate
  \return      none
*/
void drv_vad_uninit(drv_vad_t *vad);

/**
  \brief       VAD set algorithm parameters
  \param[in]   VAD VAD handle to operate.
  \param[in]   zcr  algorithm_parameter
  \param[in]   ste  algorithm_parameter
  \return      none
*/
void drv_vad_set_algorithm_parameters(drv_vad_t *vad, uint32_t zcr_l, uint32_t zcr_h, uint32_t ste_l, uint32_t ste_h);

/**
  \brief       VAD config format
  \param[in]   VAD       VAD handle to operate
  \param[in]   format    VAD config param \ref drv_vad_format_t
  \return      error code \ref csi_error_t
*/
csi_error_t drv_vad_format(drv_vad_t *vad, drv_vad_format_t *format);

/**
  \brief       VAD start trig mode
  \param[in]   VAD VAD handle to operate.
  \return      error code \ref csi_error_t
*/
csi_error_t drv_vad_start_trig_mode(drv_vad_t *vad);

/**
  \brief       VAD stop trig mode
  \param[in]   VAD VAD handle to operate.
  \return      none
*/
void drv_vad_stop_trig_mode(drv_vad_t *vad);

/**
  \brief       VAD start recorde mode
  \param[in]   VAD VAD handle to operate.
  \return      error code \ref csi_error_t
*/
csi_error_t drv_vad_start_recorde_mode(drv_vad_t *vad);

/**
  \brief       Get VAD ch status
  \param[in]   VAD      VAD handle to operate
  \param[out]  state    VAD state
  \return      error code \ref csi_error_t
*/
csi_error_t drv_vad_ch_get_state(drv_vad_ch_t *ch, csi_state_t *state);

/**
  \brief       Alloc mulitple VAD ch
  \param[in]   VAD    VAD handle to operate
  \param[in]   ch     VAD ch handle to operate
  \param[in]   ch_idx VAD ch idx
  \return      error code \ref csi_error_t
*/
csi_error_t drv_vad_ch_alloc(drv_vad_t *vad, drv_vad_ch_t *ch, uint32_t ch_idx);

/**
  \brief       Free mulitple VAD ch
  \param[in]   VAD    VAD handle to operate
  \param[in]   ch     VAD ch handle to operate
  \return      error code \ref csi_error_t
*/
void drv_vad_ch_free(drv_vad_t *vad, drv_vad_ch_t *ch);

/**
  \brief       Link DMA channel to VAD device
  \param[in]   ch           VAD handle to operate
  \param[in]   dma          The DMA channel for receive, when it is NULL means to unused dma
  \return      error code \ref csi_error_t
*/
csi_error_t drv_vad_ch_rx_link_dma(drv_vad_ch_t *ch, csi_dma_ch_t *dma);

/**
  \brief       Attach the callback handler to VAD
  \param[in]   VAD    Operate handle
  \param[in]   cb     Callback function
  \param[in]   arg    User private param
  \return      error code \ref csi_error_t
*/
csi_error_t drv_vad_attach_callback(drv_vad_t *vad, void *callback, void *arg);

/**
  \brief       Detach the callback handler
  \param[in]   ch    Operate handle
  \return      None
*/
void drv_vad_detach_callback(drv_vad_t *vad);

/**
  \brief       Read data from the buffer
               Using asynchronous receive, VAD writes the received data to the buffer
               This function reads data from the buffer, returns the number of successful reads
               Returns 0 if there is no data in the buffer
  \param[in]   ch     Operate handle
  \param[out]  data    Pointer to receive data buffer
  \param[in]   size    Receive data size
  \return      The size of data read successfully
*/
uint32_t    drv_vad_ch_receive_async(drv_vad_ch_t *ch, void *data, uint32_t size);

/**
  \brief       Read data from the buffer
               Using synchronous receive, VAD writes the received data to the buffer
               This function reads data from the buffer, returns the number of successful reads
               Returns 0 if there is no data in the buffer
  \param[in]   ch     Operate handle
  \param[out]  data    Pointer to receive data buffer
  \param[in]   size    Receive data size
  \param[in]   timeout Unit of time delay(ms)
  \return      The size of data read successfully
*/
uint32_t    drv_vad_ch_receive(drv_vad_ch_t *ch, void *data, uint32_t size, uint32_t timeout);

/**
  \brief       Enable VAD power manage
  \param[in]   VAD    VAD handle to operate
  \return      error code \ref csi_error_t
*/
csi_error_t drv_vad_enable_pm(drv_vad_t *vad);

/**
  \brief       Disable VAD power manage
  \param[in]   VAD    VAD handle to operate
  \return      None
*/
void drv_vad_disable_pm(drv_vad_t *vad);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_VAD_H_ */
