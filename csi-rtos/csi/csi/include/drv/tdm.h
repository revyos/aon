/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     drv/tdm.h
 * @brief    header file for tdm driver
 * @version  V1.0
 * @date     20. Oct 2020
 * @model    tdm
 ******************************************************************************/

#ifndef _DRV_TDM_H_
#define _DRV_TDM_H_

#include <stdint.h>
#include <stdbool.h>
#include <drv/common.h>
#include <drv/dma.h>
#include "drv/ringbuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TDM_MODE_MASTER,                                     ///< TDM transmitter master mode
    TDM_MODE_SLAVE,                                      ///< TDM transmitter slave mode
} drv_tdm_mode_t;

typedef enum {
    TDM_SAMPLE_RISING_EDGE,                               ///< The rising edge of BCLK to transmit next bit
    TDM_SAMPLE_FALL_EDGE,                                 ///< The fall edge of BCLK to transmit next bit
} drv_tdm_sample_edge_t;

typedef enum {
    TDM_LEFT_DATA_FIRST,                                 ///< Left channel first 
    TDM_RIGHT_DATA_FIRST,                                ///< Right channel first
} drv_tdm_polarity_t;

typedef enum {
    TDM_SAMPLE_RATE_8000              = 8000U,           ///< TDM sample rate is 8000
    TDM_SAMPLE_RATE_11025             = 11025U,
    TDM_SAMPLE_RATE_12000             = 12000U,
    TDM_SAMPLE_RATE_16000             = 16000U,
    TDM_SAMPLE_RATE_22050             = 22050U,
    TDM_SAMPLE_RATE_24000             = 24000U,
    TDM_SAMPLE_RATE_32000             = 32000U,
    TDM_SAMPLE_RATE_44100             = 44100U,
    TDM_SAMPLE_RATE_48000             = 48000U,
    TDM_SAMPLE_RATE_96000             = 96000U,
    TDM_SAMPLE_RATE_192000            = 192000U,
    TDM_SAMPLE_RATE_256000            = 256000U,
} drv_tdm_sample_rate_t;

typedef enum {
    TDM_SAMPLE_WIDTH_16BIT = 16U,                      ///< TDM sample width is 16bit
    TDM_SAMPLE_WIDTH_24BIT = 24U,
    TDM_SAMPLE_WIDTH_32BIT = 32U,
} drv_tdm_sample_width_t;

typedef enum {
    TDM_SCLK_32FS  = 32U,                              ///< SCLK frequency is 32 times that of tdm sample rate
    TDM_SCLK_48FS  = 48U,
    TDM_SCLK_64FS  = 64U,
    TDM_SCLK_96FS  = 96U,
    TDM_SCLK_128FS = 128U,
    TDM_SCLK_192FS = 192U,
    TDM_SCLK_256FS = 256U,
} drv_tdm_sclk_freq_t;

typedef struct {
    drv_tdm_mode_t             mode;                    ///< TDM work mode
    drv_tdm_sample_edge_t      sample_edge;             ///< The edge of BCLK to transmit next bit
    drv_tdm_polarity_t         polarity;                ///< Left/Right channel transmit first
    drv_tdm_sample_rate_t      rate;                    ///< TDM sample rate
    drv_tdm_sample_width_t     width;                   ///< TDM sample width
    drv_tdm_sclk_freq_t        sclk_nfs;                ///< SCLK frequency is N times that of I2S sample rate
} drv_tdm_format_t;

typedef enum {
    TDM_EVENT_SEND_COMPLETE,
    TDM_EVENT_RECEIVE_COMPLETE,
    TDM_EVENT_TX_BUFFER_EMPTY,
    TDM_EVENT_RX_BUFFER_FULL,
    TDM_EVENT_ERROR_OVERFLOW,
    TDM_EVENT_ERROR_UNDERFLOW,
    TDM_EVENT_ERROR,
} drv_tdm_event_t;

struct drv_tdm {
    csi_dev_t               dev;                        ///< TDM hw-device info
    slist_t                 head;                       ///< TDM ch handle
    void                    *priv;
};

typedef struct drv_tdm drv_tdm_t;
typedef struct drv_tdm_ch drv_tdm_ch_t;

struct drv_tdm_ch {
    drv_tdm_t               *tdm;
    void (*callback)(drv_tdm_ch_t *tdm, drv_tdm_event_t event, void *arg); ///< TDM event callback for user
    void                    *arg;                                          ///< user private param passed to user callback
    uint32_t                ch_idx;
    ringbuffer_t            *tx_buf;                                       ///< TDM send buffer
    ringbuffer_t            *rx_buf;                                       ///< TDM receive buffer 
    csi_dma_ch_t            *tx_dma;                                       ///< send dma channel handle
    csi_dma_ch_t            *rx_dma;                                       ///< receive dma channel handle  
    uint32_t                tx_period;                                     ///< TDM send period num data will callback
    uint32_t                rx_period;                                     ///< TDM receive period num data will callback
    csi_state_t             state;                                         ///< TDM communication state
    slist_t                 next;
    void                    *priv;
};

/**
  \brief       Init tdm
  \param[in]   tdm    tdm handle to operate
  \param[in]   idx    tdm interface idx
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_init(drv_tdm_t *tdm, uint32_t idx);

/**
  \brief       Uninit tdm
  \param[in]   tdm    tdm handle to operate
  \return      none
*/
void drv_tdm_uninit(drv_tdm_t *tdm);

/**
  \brief       Enable tdm
  \param[in]   tdm      tdm handle to operate
  \param[in]   enable   True enable, False disable
  \return      None
*/
void drv_tdm_enable(drv_tdm_t *tdm, bool enable);

/**
  \brief       tdm config format
  \param[in]   tdm       tdm handle to operate
  \param[in]   format    tdm config param \ref drv_tdm_format_t
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_format(drv_tdm_t *tdm, drv_tdm_format_t *format);

/**
  \brief       Link DMA channel to tdm device
  \param[in]   ch        tdm handle to operate
  \param[in]   rx_dma    The DMA channel  for receive, when it is NULL means to unused dma
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_ch_rx_link_dma(drv_tdm_ch_t *ch, csi_dma_ch_t *rx_dma);

/**
  \brief       Link DMA channel to tdm device
  \param[in]   ch        tdm handle to operate
  \param[in]   tx_dma    The DMA channel  for send, when it is NULL means to unused dma
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_ch_tx_link_dma(drv_tdm_ch_t *ch, csi_dma_ch_t *tx_dma);

/**
  \brief       tdm rx buffer config
  \param[in]   ch        tdm handle to operate
  \param[in]   buffer    tdm rx buffer
  \return      None
*/
void drv_tdm_ch_rx_set_buffer(drv_tdm_ch_t *ch, ringbuffer_t *buffer);

/**
  \brief       tdm tx buffer config
  \param[in]   ch        tdm handle to operate
  \param[in]   buffer    tdm tx buffer
  \return      None
*/
void drv_tdm_ch_tx_set_buffer(drv_tdm_ch_t *ch, ringbuffer_t *buffer);

/**
  \brief       tdm rx set period.The value of period is to report a receive completion event
               after each period value data is received
  \param[in]   ch        tdm handle to operate
  \param[in]   period    tdm rx period
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_ch_rx_set_period(drv_tdm_ch_t *ch, uint32_t period);

/**
  \brief       tdm tx set period.The value of period is to report a send completion event
               after each period value data is sended
  \param[in]   ch        tdm handle to operate
  \param[in]   period    tdm tx period
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_ch_tx_set_period(drv_tdm_ch_t *ch, uint32_t period);

/**
  \brief       Get rx ringbuffer buffer free space
  \param[in]   ch     tdm handle to operate
  \return      Buffer free space (bytes)
*/
uint32_t drv_tdm_ch_rx_buffer_avail(drv_tdm_ch_t *ch);

/**
  \brief       Get tx ringbuffer buffer free space
  \param[in]   ch     tdm handle to operate
  \return      Buffer free space (bytes)
*/
uint32_t drv_tdm_ch_tx_buffer_avail(drv_tdm_ch_t *ch);

/**
  \brief       Get rx ringbuffer buffer used space
  \param[in]   ch     tdm handle to operate
  \return      Buffer used space (bytes)
*/
uint32_t drv_tdm_ch_rx_buffer_remain(drv_tdm_ch_t *ch);

/**
  \brief       Get tx ringbuffer buffer used space
  \param[in]   ch     tdm handle to operate
  \return      Buffer used space (bytes)
*/
uint32_t drv_tdm_ch_tx_buffer_remain(drv_tdm_ch_t *ch);

/**
  \brief       Reset the rx ringbuffer, discard all data in the buffer
  \param[in]   ch     tdm handle to operate
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_ch_rx_buffer_reset(drv_tdm_ch_t *ch);

/**
  \brief       Reset the tx ringbuffer, discard all data in the buffer
  \param[in]   ch     tdm handle to operate
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_ch_tx_buffer_reset(drv_tdm_ch_t *ch);

/**
  \brief       Read data from the buffer
               Using asynchronous receive, tdm writes the received data to the buffer
               This function reads data from the buffer, returns the number of successful reads
               Returns 0 if there is no data in the buffer
  \param[in]   ch      Operate handle
  \param[out]  data    Pointer to receive data buffer
  \param[in]   size    Receive data size
  \return      The size of data read successfully
*/
uint32_t drv_tdm_ch_receive_async(drv_tdm_ch_t *ch, const void *data, uint32_t size);

/**
  \brief       Write data to the buffer
               With asynchronous sending
               The data is first written to the buffer and then output through the tdm interface
               Return value is the number of data that was successfully written to the buffer
  \param[in]   ch      Operate handle
  \param[out]  data    Pointer to send data buffer
  \param[in]   size    send data size
  \return      The data size that write to buffer
*/
uint32_t drv_tdm_ch_send_async(drv_tdm_ch_t *ch, const void *data, uint32_t size);

/**
  \brief       Start tdm single ch asynchronous receive
  \param[in]   ch     Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_ch_receive_start(drv_tdm_ch_t *ch);

/**
  \brief       Start tdm single ch asynchronous send
  \param[in]   ch     Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_ch_send_start(drv_tdm_ch_t *ch);

/**
  \brief       Start tdm all ch asynchronous receive
  \param[in]   tdm    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_all_ch_receive_start(drv_tdm_t *tdm);

/**
  \brief       Start tdm all ch asynchronous send
  \param[in]   tdm    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_all_ch_send_start(drv_tdm_t *tdm);

/**
  \brief       Stop tdm single ch asynchronous receive
  \param[in]   ch     Operate handle
  \return      None
*/
void drv_tdm_ch_receive_stop(drv_tdm_ch_t *ch);

/**
  \brief       Stop tdm single ch asynchronous send
  \param[in]   ch     Operate handle
  \return      None
*/
void drv_tdm_ch_send_stop(drv_tdm_ch_t *ch);

/**
  \brief       Stop tdm all ch asynchronous receive
  \param[in]   tdm    Operate handle
  \return      None
*/
void drv_tdm_all_ch_receive_stop(drv_tdm_t *tdm);

/**
  \brief       Stop tdm all ch asynchronous send
  \param[in]   tdm    Operate handle
  \return      None
*/
void drv_tdm_all_ch_send_stop(drv_tdm_t *tdm);

/**
  \brief       Attach the callback handler to tdm
  \param[in]   ch     Operate handle
  \param[in]   cb     Callback function
  \param[in]   arg    User private param
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_ch_attach_callback(drv_tdm_ch_t *ch, void *callback, void *arg);

/**
  \brief       Detach the callback handler
  \param[in]   ch     Operate handle
  \return      None
*/
void drv_tdm_ch_detach_callback(drv_tdm_ch_t *ch);

/**
  \brief       Alloc mulitple tdm ch
  \param[in]   tdm    tdm handle to operate
  \param[in]   ch     tdm ch handle to operate
  \param[in]   ch_idx tdm ch idx
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_ch_alloc(drv_tdm_t *tdm, drv_tdm_ch_t *ch, uint32_t ch_idx);

/**
  \brief       Free mulitple tdm ch
  \param[in]   tdm    tdm handle to operate
  \param[in]   ch     tdm ch handle to operate
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_ch_free(drv_tdm_t *tdm, drv_tdm_ch_t *ch);

/**
  \brief       Get tdm status
  \param[in]   ch       tdm handle to operate
  \param[out]  state    tdm state
  \return      error code \ref csi_error_t
*/
csi_error_t drv_tdm_ch_get_state(drv_tdm_ch_t *ch, csi_state_t *state);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_TDM_H_ */
