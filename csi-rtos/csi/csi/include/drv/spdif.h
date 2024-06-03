/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     spdif.h
 * @brief    header file for spdif driver
 * @version  V1.0
 * @date     23. Oct 2020
 * @model    spdif
 ******************************************************************************/

#ifndef __DRV_SPDIF_H__
#define __DRV_SPDIF_H__

#include <stdint.h>
#include <stdbool.h>
#include <drv/common.h>
#include <drv/dma.h>
#include "drv/ringbuffer.h"

#ifdef __cplusplus
extern "C"  {
#endif

typedef enum {
    SPDIF_SAMPLE_RATE_22050         = 0U, ///<   SPDIF sample rate is 22.05kHz
    SPDIF_SAMPLE_RATE_24000,              ///<   SPDIF sample rate is 24kHz
    SPDIF_SAMPLE_RATE_32000,              ///<   SPDIF sample rate is 32kHz
    SPDIF_SAMPLE_RATE_44100,              ///<   SPDIF sample rate is 44.1kHz
    SPDIF_SAMPLE_RATE_48000,              ///<   SPDIF sample rate is 48Hz
    SPDIF_SAMPLE_RATE_88200,              ///<   SPDIF sample rate is 88.2kHz
    SPDIF_SAMPLE_RATE_96000,              ///<   SPDIF sample rate is 96kHz
    SPDIF_SAMPLE_RATE_176400,             ///<   SPDIF sample rate is 176.4kHz
    SPDIF_SAMPLE_RATE_192000,             ///<   SPDIF sample rate is 192kHz
} csi_spdif_sample_rate_t;

typedef enum {
    SPDIF_SAMPLE_WIDTH_16BIT        = 0U, ///< 16-bit tx data mode
    SPDIF_SAMPLE_WIDTH_20BIT,             ///< 20-bit tx data mode
    SPDIF_SAMPLE_WIDTH_24BIT,             ///< 24-bit tx data mode
} csi_spdif_sample_width_t;

typedef struct {
    csi_spdif_sample_rate_t     rate;                    ///< spdif sample rate
    csi_spdif_sample_width_t    width;                   ///< spdif sample width
} csi_spdif_format_t;

typedef enum {
    SPDIF_LEFT_CHANNEL,
    SPDIF_RIGHT_CHANNEL,
    SPDIF_LEFT_RIGHT_CHANNEL,
} csi_spdif_sound_channel_t;

typedef enum {
    SPDIF_EVENT_SEND_COMPLETE,
    SPDIF_EVENT_RECEIVE_COMPLETE,
    SPDIF_EVENT_TX_BUFFER_EMPTY,
    SPDIF_EVENT_RX_BUFFER_FULL,
    SPDIF_EVENT_ERROR_OVERFLOW,
    SPDIF_EVENT_ERROR_UNDERFLOW,
    SPDIF_EVENT_ERROR,
} csi_spdif_event_t;

typedef struct csi_spdif csi_spdif_t;

struct csi_spdif {
    csi_dev_t               dev;                ///< SPDIF hw-device info
    void (*callback)(csi_spdif_t *spdif, csi_spdif_event_t event, void *arg); ///< SPDIF event callback for user
    void                    *arg;               ///< user private param passed to user callback
    ringbuffer_t            *tx_buf;            ///< SPDIF send buffer
    ringbuffer_t            *rx_buf;            ///< SPDIF receive buffer
    csi_dma_ch_t            *tx_dma;            ///< send dma channel handle
    csi_dma_ch_t            *rx_dma;            ///< receive dma channel handle
    uint32_t                tx_period;          ///< SPDIF send period num data will callback
    uint32_t                rx_period;          ///< SPDIF receive period num data will callback
    csi_state_t             state;              ///< SPDIF communication state
    void                    *priv;
};

/**
  \brief       Init spdif
  \param[in]   spdif  SPDIF handle to operate
  \param[in]   idx    SPDIF interface idx
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_init(csi_spdif_t *spdif, uint32_t idx);

/**
  \brief       Uninit spdif
  \param[in]   spdif    spdif handle to operate
  \return      none
*/
void csi_spdif_uninit(csi_spdif_t *spdif);

/**
  \brief       SPDIF config format
  \param[in]   spdif     SPDIF handle to operate
  \param[in]   format    SPDIF config param \ref csi_spdif_format_t
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_format(csi_spdif_t *spdif, csi_spdif_format_t *format);

/**
  \brief       Link DMA channel to spdif device
  \param[in]   spdif     SPDIF handle to operate
  \param[in]   rx_dma    The DMA channel  for receive, when it is NULL means to unused dma
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_rx_link_dma(csi_spdif_t *spdif, csi_dma_ch_t *rx_dma);

/**
  \brief       Link DMA channel to spdif device
  \param[in]   spdif    SPDIF handle to operate
  \param[in]   tx_dma    The DMA channel for send, when it is NULL means to unused dma
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_tx_link_dma(csi_spdif_t *spdif, csi_dma_ch_t *tx_dma);

/**
  \brief       SPDIF rx buffer config
  \param[in]   spdif     SPDIF handle to operate
  \param[in]   buffer    SPDIF rx buffer
  \return      None
*/
void csi_spdif_rx_set_buffer(csi_spdif_t *spdif, ringbuffer_t *buffer);

/**
  \brief       SPDIF tx buffer config
  \param[in]   spdif     SPDIF handle to operate
  \param[in]   buffer    SPDIF tx buffer
  \return      None
*/
void csi_spdif_tx_set_buffer(csi_spdif_t *spdif, ringbuffer_t *buffer);

/**
  \brief       SPDIF rx buffer config
  \param[in]   spdif     SPDIF handle to operate
  \param[in]   buffer    SPDIF rx buffer
  \return      None
*/
void csi_spdif_rx_set_buffer(csi_spdif_t *spdif, ringbuffer_t *buffer);

/**
  \brief       SPDIF tx buffer config
  \param[in]   spdif     SPDIF handle to operate
  \param[in]   buffer    SPDIF tx buffer
  \return      None
*/
void csi_spdif_tx_set_buffer(csi_spdif_t *spdif, ringbuffer_t *buffer);

/**
  \brief       SPDIF rx set period.The value of period is to report a receive completion event
               after each period value data is received
  \param[in]   spdif     SPDIF handle to operate
  \param[in]   period    SPDIF rx period
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_rx_set_period(csi_spdif_t *spdif, uint32_t period);

/**
  \brief       SPDIF tx set period.The value of period is to report a receive completion event
               after each period value data is send
  \param[in]   spdif     SPDIF handle to operate
  \param[in]   period    SPDIF tx period
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_tx_set_period(csi_spdif_t *spdif, uint32_t period);

/**
  \brief       Get rx ringbuffer buffer free space
  \param[in]   spdif    SPDIF handle to operate
  \return      Buffer free space (bytes)
*/
uint32_t csi_spdif_rx_buffer_avail(csi_spdif_t *spdif);

/**
  \brief       Get rx ringbuffer buffer used space
  \param[in]   spdif   SPDIF handle to operate
  \return      Buffer used space (bytes)
*/
uint32_t csi_spdif_rx_buffer_remain(csi_spdif_t *spdif);

/**
  \brief       Reset the rx ringbuffer, discard all data in the buffer
  \param[in]   spdif    SPDIF handle to operate
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_rx_buffer_reset(csi_spdif_t *spdif);

/**
  \brief        Get tx ringbuffer buffer free space
  \param[in]    spdif    SPDIF handle to operate
  \return       Buffer free space (bytes)
*/
uint32_t    csi_spdif_tx_buffer_avail(csi_spdif_t *spdif);

/**
  \brief        Get tx ringbuffer buffer used space
  \param[in]    spdif    SPDIF handle to operate
  \return       Buffer used space (bytes)
*/
uint32_t    csi_spdif_tx_buffer_remain(csi_spdif_t *spdif);

/**
  \brief       Reset the tx ringbuffer, discard all data in the buffer
  \param[in]   spdif    Handle to operate
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_tx_buffer_reset(csi_spdif_t *spdif);

/**
  \brief       Write data to the buffer
               With asynchronous sending
               The data is first written to the buffer and then output through the spdif interface
               Return value is the number of data that was successfully written to the buffer
  \param[in]   spdif     Operate handle
  \param[in]   data    Pointer to send data buffer
  \param[in]   size    Send data size
  \return      The data size that write to buffer
*/
uint32_t    csi_spdif_send_async(csi_spdif_t *spdif, const void *data, uint32_t size);

/**
  \brief       Read data from the buffer
               Using asynchronous receive, spdif writes the received data to the buffer
               This function reads data from the buffer, returns the number of successful reads
               Returns 0 if there is no data in the buffer
  \param[in]   spdif     Operate handle
  \param[out]  data    Pointer to receive data buffer
  \param[in]   size    Receive data size
  \return      The size of data read successfully
*/
uint32_t    csi_spdif_receive_async(csi_spdif_t *spdif, const void *data, uint32_t size);

/**
  \brief       Start spdif pause asynchronous send
  \param[in]   spdif    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_send_pause(csi_spdif_t *spdif);

/**
  \brief       Start spdif resume asynchronous send
  \param[in]   spdif    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_send_resume(csi_spdif_t *spdif);

/**
  \brief       Start spdif asynchronous send
  \param[in]   spdif    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_send_start(csi_spdif_t *spdif);

/**
  \brief       Start spdif asynchronous receive
  \param[in]   spdif    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_receive_start(csi_spdif_t *spdif);

/**
  \brief       Stop spdif asynchronous send
  \param[in]   spdif    Operate handle
  \return      None
*/
void csi_spdif_send_stop(csi_spdif_t *spdif);

/**
  \brief       Stop spdif asynchronous receive
  \param[in]   spdif    Operate handle
  \return      None
*/
void csi_spdif_receive_stop(csi_spdif_t *spdif);

/**
  \brief       Attach the callback handler to spdif
  \param[in]   spdif  Operate handle
  \param[in]   cb     Callback function
  \param[in]   arg    User private param
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_attach_callback(csi_spdif_t *spdif, void *callback, void *arg);

/**
  \brief       Detach the callback handler
  \param[in]   spdif    Operate handle
  \return      None
*/
void csi_spdif_detach_callback(csi_spdif_t *spdif);

/**
  \brief       Get spdif status
  \param[in]   spdif    SPDIF handle to operate
  \param[out]  state    SPDIF state
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_get_state(csi_spdif_t *spdif, csi_state_t *state);

/**
  \brief       Enable spdif power manage
  \param[in]   spdif    SPDIF handle to operate
  \return      error code \ref csi_error_t
*/
csi_error_t csi_spdif_enable_pm(csi_spdif_t *spdif);

/**
  \brief       Disable spdif power manage
  \param[in]   spdif   SPDIF handle to operate
  \return      None
*/
void csi_spdif_disable_pm(csi_spdif_t *spdif);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_SPDIF_H_ */