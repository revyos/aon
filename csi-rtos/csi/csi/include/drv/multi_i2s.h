/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     drv/multi_i2s.h
 * @brief    header file for multiple i2s driver
 * @version  V1.0
 * @date     4. Nov 2020
 * @model    multiple i2s
 ******************************************************************************/

#ifndef _DRV_MULTI_I2S_H_
#define _DRV_MULTI_I2S_H_

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
    MULTI_I2S_MODE_MASTER,                                     ///< I2s transmitter master mode
    MULTI_I2S_MODE_SLAVE,                                      ///< I2s transmitter slave mode
} drv_multi_i2s_mode_t;

typedef enum {
    MULTI_I2S_PROTOCOL_I2S,                                    ///< I2S protocol
    MULTI_I2S_PROTOCOL_MSB_JUSTIFIED,                          ///< MSB (left) justified protocol
    MULTI_I2S_PROTOCOL_LSB_JUSTIFIED,                          ///< LSB (right) justified protocol
    MULTI_I2S_PROTOCOL_PCM,                                    ///< PCM protocol
} drv_multi_i2s_protocol_t;

typedef enum {
    MULTI_I2S_LEFT_POLARITY_LOW,                               ///< Low level represents the left channel
    MULTI_I2S_LEFT_POLARITY_HIGH,                              ///< High level represents the left channel
} drv_multi_i2s_ws_left_polarity_t;

typedef enum {
    MULTI_I2S_SAMPLE_RATE_8000              = 8000U,           ///< I2S sample rate is 8000
    MULTI_I2S_SAMPLE_RATE_11025             = 11025U,
    MULTI_I2S_SAMPLE_RATE_12000             = 12000U,
    MULTI_I2S_SAMPLE_RATE_16000             = 16000U,
    MULTI_I2S_SAMPLE_RATE_22050             = 22050U,
    MULTI_I2S_SAMPLE_RATE_24000             = 24000U,
    MULTI_I2S_SAMPLE_RATE_32000             = 32000U,
    MULTI_I2S_SAMPLE_RATE_44100             = 44100U,
    MULTI_I2S_SAMPLE_RATE_48000             = 48000U,
    MULTI_I2S_SAMPLE_RATE_96000             = 96000U,
    MULTI_I2S_SAMPLE_RATE_192000            = 192000U,
    MULTI_I2S_SAMPLE_RATE_256000            = 256000U,
} drv_multi_i2s_sample_rate_t;

typedef enum {
    MULTI_I2S_SAMPLE_WIDTH_16BIT = 16U,                        ///< I2S sample width is 16bit
    MULTI_I2S_SAMPLE_WIDTH_24BIT = 24U,
    MULTI_I2S_SAMPLE_WIDTH_32BIT = 32U,
} drv_multi_i2s_sample_width_t;

typedef enum {
    MULTI_I2S_SCLK_16FS = 16U,                                ///< SCLK frequency is 16 times that of I2S sample rate
    MULTI_I2S_SCLK_32FS = 32U,
    MULTI_I2S_SCLK_48FS = 48U,
    MULTI_I2S_SCLK_64FS = 64U,
} drv_multi_i2s_sclk_freq_t;

typedef enum {
    MULTI_I2S_MCLK_256FS = 256U,                              ///< MCLK frequency is 256 times that of I2S sample rate
    MULTI_I2S_MCLK_384FS = 384U,
} drv_multi_i2s_mclk_freq_t;

typedef enum {
    MULTI_I2S_LEFT_CHANNEL,
    MULTI_I2S_RIGHT_CHANNEL,
    MULTI_I2S_LEFT_RIGHT_CHANNEL,
} drv_multi_i2s_sound_channel_t;

typedef struct {
    drv_multi_i2s_mode_t             mode;                    ///< I2S work mode
    drv_multi_i2s_protocol_t         protocol;                ///< Protocols used by I2S
    drv_multi_i2s_sound_channel_t    sound_channel;           ///< I2S sound channel
    drv_multi_i2s_ws_left_polarity_t polarity;                ///< left channel polarity
    drv_multi_i2s_sample_rate_t      rate;                    ///< I2S sample rate
    drv_multi_i2s_sample_width_t     width;                   ///< I2S sample width
    drv_multi_i2s_sclk_freq_t        sclk_nfs;                ///< SCLK frequency is N times that of I2S sample rate
    drv_multi_i2s_mclk_freq_t        mclk_nfs;                ///< MCLK frequency is N times that of I2S sample rate
} drv_multi_i2s_format_t;

typedef enum {
    MULTI_I2S_EVENT_SEND_COMPLETE,
    MULTI_I2S_EVENT_RECEIVE_COMPLETE,
    MULTI_I2S_EVENT_TX_BUFFER_EMPTY,
    MULTI_I2S_EVENT_RX_BUFFER_FULL,
    MULTI_I2S_EVENT_ERROR_OVERFLOW,
    MULTI_I2S_EVENT_ERROR_UNDERFLOW,
    MULTI_I2S_EVENT_ERROR,
} drv_multi_i2s_event_t;

typedef struct drv_multi_i2s drv_multi_i2s_t;
typedef struct drv_multi_i2s_ch drv_multi_i2s_ch_t;

struct drv_multi_i2s_ch {
    drv_multi_i2s_t         *multi_i2s;
    void                    *arg;               ///< user private param passed to user callback
    void (*callback)(drv_multi_i2s_ch_t *ch, drv_multi_i2s_event_t event, void *arg); ///< I2S hardware error callback for user
    uint32_t                ch_idx;
    ringbuffer_t            *tx_buf;            ///< I2S send buffer
    ringbuffer_t            *rx_buf;            ///< I2S receive buffer
    csi_dma_ch_t            *tx_dma;            ///< send dma channel handle
    csi_dma_ch_t            *rx_dma;            ///< receive dma channel handle
    uint32_t                tx_period;          ///< I2S send period num data will callback
    uint32_t                rx_period;          ///< I2S receive period num data will callback
    csi_state_t             state;              ///< I2S communication state
    void                    *priv;
    slist_t                 next;
};

struct drv_multi_i2s {
    csi_dev_t               dev;                ///< I2S hw-device info
    slist_t                 head;               ///< i2s ch handle
    void                    *priv;
};

/**
  \brief       Init mulitple i2s
  \param[in]   i2s    I2s handle to operate
  \param[in]   idx    I2s interface idx
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_init(drv_multi_i2s_t *i2s, uint32_t idx);

/**
  \brief       Uninit i2s
  \param[in]   i2s    I2s handle to operate
  \return      none
*/
void drv_mulit_i2s_uninit(drv_multi_i2s_t *i2s);

/**
  \brief       I2S enable
  \param[in]   i2s i2s handle to operate.
  \param[in]   en  1 enable, 0 disable
  \return      none
*/
void drv_mulit_i2s_enable(drv_multi_i2s_t *i2s, bool enable);

/**
  \brief       I2s config format
  \param[in]   i2s       I2s handle to operate
  \param[in]   format    I2s config param \ref drv_i2s_format_t
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_format(drv_multi_i2s_t *i2s, drv_multi_i2s_format_t *format);

/**
  \brief       Set i2s all ch pause asynchronous send
  \param[in]   i2s    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_all_ch_send_pause(drv_multi_i2s_t *i2s);

/**
  \brief       Set i2s single ch pause asynchronous send
  \param[in]   ch    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_send_pause(drv_multi_i2s_ch_t *ch);

/**
  \brief       Set i2s all ch resume asynchronous send
  \param[in]   i2s    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_all_ch_send_resume(drv_multi_i2s_t *i2s);

/**
  \brief       Set i2s single ch resume asynchronous send
  \param[in]   ch    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_send_resume(drv_multi_i2s_ch_t *ch);

/**
  \brief       Start i2s all ch asynchronous send
  \param[in]   i2s    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_all_ch_send_start(drv_multi_i2s_t *i2s);

/**
  \brief       Start i2s single ch asynchronous send
  \param[in]   ch    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_send_start(drv_multi_i2s_ch_t *ch);

/**
  \brief       Start i2s all ch asynchronous receive
  \param[in]   i2s    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_all_ch_receive_start(drv_multi_i2s_t *i2s);

/**
  \brief       Start i2s single ch asynchronous receive
  \param[in]   ch    Operate handle
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_receive_start(drv_multi_i2s_ch_t *ch);

/**
  \brief       Stop i2s all ch asynchronous send
  \param[in]   i2s    Operate handle
  \return      None
*/
void drv_mulit_i2s_all_ch_send_stop(drv_multi_i2s_t *i2s);

/**
  \brief       Stop i2s single ch asynchronous send
  \param[in]   ch    Operate handle
  \return      None
*/
void drv_mulit_i2s_ch_send_stop(drv_multi_i2s_ch_t *ch);

/**
  \brief       Stop i2s all ch asynchronous receive
  \param[in]   i2s    Operate handle
  \return      None
*/
void drv_mulit_i2s_all_ch_receive_stop(drv_multi_i2s_t *i2s);

/**
  \brief       Stop i2s single ch asynchronous receive
  \param[in]   ch    Operate handle
  \return      None
*/
void drv_mulit_i2s_ch_receive_stop(drv_multi_i2s_ch_t *ch);

/**
  \brief       Get i2s ch status
  \param[in]   i2s      I2s handle to operate
  \param[out]  state    I2s state
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_get_state(drv_multi_i2s_ch_t *i2s, csi_state_t *state);

/**
  \brief       Alloc mulitple i2s ch
  \param[in]   i2s    I2s handle to operate
  \param[in]   ch     I2s ch handle to operate
  \param[in]   ch_idx I2s ch idx
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_alloc(drv_multi_i2s_t *i2s, drv_multi_i2s_ch_t *ch, uint32_t ch_idx);

/**
  \brief       Attach the callback handler to i2s
  \param[in]   i2s    Operate handle
  \param[in]   cb     Callback function
  \param[in]   arg    User private param
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_attach_callback(drv_multi_i2s_ch_t *ch, void *callback, void *arg);

/**
  \brief       Detach the callback handler
  \param[in]   ch    Operate handle
  \return      None
*/
void drv_mulit_i2s_ch_detach_callback(drv_multi_i2s_ch_t *ch);

/**
  \brief       Free mulitple i2s ch
  \param[in]   i2s    I2s handle to operate
  \param[in]   ch     I2s ch handle to operate
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_free(drv_multi_i2s_t *i2s, drv_multi_i2s_ch_t *ch);

/**
  \brief       Link DMA channel to i2s device
  \param[in]   ch       I2s handle to operate
  \param[in]   rx_dma    The DMA channel  for receive, when it is NULL means to unused dma
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_rx_link_dma(drv_multi_i2s_ch_t *ch, csi_dma_ch_t *rx_dma);

/**
  \brief       Link DMA channel to i2s device
  \param[in]   ch       I2s handle to operate
  \param[in]   tx_dma    The DMA channel for send, when it is NULL means to unused dma
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_tx_link_dma(drv_multi_i2s_ch_t *ch, csi_dma_ch_t *tx_dma);

/**
  \brief       I2s rx buffer config
  \param[in]   ch       I2s handle to operate
  \param[in]   buffer    I2s rx buffer
  \return      None
*/
void drv_mulit_i2s_ch_rx_set_buffer(drv_multi_i2s_ch_t *ch, ringbuffer_t *buffer);

/**
  \brief       I2s tx buffer config
  \param[in]   ch       I2s handle to operate
  \param[in]   buffer    I2s tx buffer
  \return      None
*/
void drv_mulit_i2s_ch_tx_set_buffer(drv_multi_i2s_ch_t *ch, ringbuffer_t *buffer);

/**
  \brief       I2s rx set period.The value of period is to report a receive completion event
               after each period value data is received
  \param[in]   ch       I2s handle to operate
  \param[in]   period    I2s rx period
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_rx_set_period(drv_multi_i2s_ch_t *ch, uint32_t period);

/**
  \brief       I2s tx set period.The value of period is to report a receive completion event
               after each period value data is send
  \param[in]   ch       I2s handle to operate
  \param[in]   period    I2s tx period
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_tx_set_period(drv_multi_i2s_ch_t *ch, uint32_t period);

/**
  \brief       Get rx ringbuffer buffer free space
  \param[in]   ch    I2s handle to operate
  \return      Buffer free space (bytes)
*/
uint32_t drv_mulit_i2s_ch_rx_buffer_avail(drv_multi_i2s_ch_t *ch);

/**
  \brief       Get rx ringbuffer buffer used space
  \param[in]   ch    I2s handle to operate
  \return      Buffer used space (bytes)
*/
uint32_t drv_mulit_i2s_ch_rx_buffer_remain(drv_multi_i2s_ch_t *ch);

/**
  \brief       Reset the rx ringbuffer, discard all data in the buffer
  \param[in]   ch    I2s handle to operate
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_rx_buffer_reset(drv_multi_i2s_ch_t *ch);

/**
  \brief        Get tx ringbuffer buffer free space
  \param[in]    ch    I2s handle to operate
  \return       Buffer free space (bytes)
*/
uint32_t    drv_mulit_i2s_ch_tx_buffer_avail(drv_multi_i2s_ch_t *ch);

/**
  \brief        Get tx ringbuffer buffer used space
  \param[in]    ch    I2s handle to operate
  \return       Buffer used space (bytes)
*/
uint32_t    drv_mulit_i2s_ch_tx_buffer_remain(drv_multi_i2s_ch_t *ch);

/**
  \brief       Reset the tx ringbuffer, discard all data in the buffer
  \param[in]   ch    Handle to operate
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_ch_tx_buffer_reset(drv_multi_i2s_ch_t *ch);

/**
  \brief       Write data to the buffer
               With asynchronous sending
               The data is first written to the buffer and then output through the i2s interface
               Return value is the number of data that was successfully written to the buffer
  \param[in]   ch     Operate handle
  \param[in]   data    Pointer to send data buffer
  \param[in]   size    Send data size
  \return      The data size that write to buffer
*/
uint32_t    drv_mulit_i2s_ch_send_async(drv_multi_i2s_ch_t *ch, const void *data, uint32_t size);

/**
  \brief       Read data from the buffer
               Using asynchronous receive, i2s writes the received data to the buffer
               This function reads data from the buffer, returns the number of successful reads
               Returns 0 if there is no data in the buffer
  \param[in]   ch     Operate handle
  \param[out]  data    Pointer to receive data buffer
  \param[in]   size    Receive data size
  \return      The size of data read successfully
*/
uint32_t    drv_mulit_i2s_ch_receive_async(drv_multi_i2s_ch_t *ch, void *data, uint32_t size);

/**
  \brief       Enable i2s power manage
  \param[in]   i2s    I2s handle to operate
  \return      error code \ref csi_error_t
*/
csi_error_t drv_mulit_i2s_enable_pm(drv_multi_i2s_t *i2s);

/**
  \brief       Disable i2s power manage
  \param[in]   i2s    I2s handle to operate
  \return      None
*/
void drv_mulit_i2s_disable_pm(drv_multi_i2s_t *i2s);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_MULTI_I2S_H_ */
