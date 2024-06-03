/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file       drv/pvt.h
 * @brief      Header File for PVT Driver
 * @version    V1.0
 * @date       9. Oct 2021
 * @model      pvt
 ******************************************************************************/

#ifndef __DRV_PVT_H_
#define __DRV_PVT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t ch_num:8;
    uint32_t vm_num:8;
    uint32_t pd_num:8;
    uint32_t ts_num:8;
}csi_pvt_sensor_info_t;

typedef enum {
	PVT_TS_ALARM,
	PVT_PD_ALARM,
	PVT_VM_ALARM
}csi_pvt_event_t;

typedef struct {
	uint32_t ts_ip_mask;                                     ///< ipn mask with event
	uint32_t ts_ip_high_alarm;                               ///< alarm a
	uint32_t ts_ip_low_alarm;                                ///< alarm b
	uint32_t ts_ip_fault;                                    ///< fault
}csi_ts_irq_prv_info_t;

typedef struct {
	csi_pvt_event_t pvt_evt;
	union _irq_info{
	csi_ts_irq_prv_info_t ts_irq_info;
	}irq_info;
}csi_callback_info_t;

typedef struct csi_pvt csi_pvt_t;

struct csi_pvt {
    csi_dev_t             dev;
    csi_pvt_sensor_info_t sensor_info;
    uint32_t              pvt_clk_factor;
    void*                 reg_map;
    void (*callback)(csi_pvt_t *pvt, uint32_t event, csi_callback_info_t *arg); ///< Call-back of pvt controler
    void                  *arg;                                  ///< User param passed to callback
    void                  *priv;                                 ///< User private param
};

/**
  \brief       Initialize pvt handle,and init config sensor befor sample
  \param[in]   pvt         pvt handle
  \param[in]   pvt_idx     pvt instance index
  \return      Error code
*/
csi_error_t csi_pvt_init(csi_pvt_t *pvt, uint32_t pvt_idx);

/**
  \brief       De-initialize pvt handle
               releases the software resources used by the pvt
  \param[in]   pvt    pvt  handle
  \return      None
*/
void csi_pvt_uninit(csi_pvt_t *pvt);

/**
  \brief       read ts sensor data
  \param[in]   pvt           pvt handle
  \param[in]   sensor_idx    ts-sensor instance index
  \param[out]  raw_data      raw-data after sample
  \return      Error code
*/
csi_error_t csi_pvt_read_ts(csi_pvt_t *pvt,uint32_t sensor_idx,uint32_t *raw_data);

/**
  \brief       read pd sensor data
  \param[in]   pvt           pvt handle
  \param[in]   sensor_idx    pd-sensor instance index
  \param[out]  raw_data      raw-data after sample
  \return      Error code
*/
csi_error_t csi_pvt_read_pd(csi_pvt_t *pvt,uint32_t sensor_idx,uint32_t *raw_data);

/**
  \brief       read single-ch vm-sensor data
               each vm-sensor has 16 vm channel
  \param[in]   pvt           pvt handle
  \param[in]   sensor_idx    vm-sensor instance index
  \param[in]   vm_ch_idx     vm-sensor ch within current vm-sensor instance
  \param[out]  raw_data      raw-data after sample
  \return      Error code
*/
csi_error_t csi_pvt_read_vm_single_ch(csi_pvt_t *pvt,uint32_t sensor_idx,uint32_t vm_ch_idx,uint32_t *raw_data);

/**
  \brief       read multi-ch vm-sensor data
               each vm-sensor has 16 vm channel
  \param[in]   pvt           pvt handle
  \param[in]   sensor_idx    vm-sensor instance index
  \param[in]   vm_ch_num     read vm_ch_num channel data from zero-index
  \param[out]  raw_data      raw-data after sample
  \return      Error code
*/
csi_error_t csi_pvt_read_vm_muti_ch(csi_pvt_t *pvt,uint32_t sensor_idx,uint32_t vm_ch_num,uint32_t *raw_data);

/**
  \brief       register pvt interrupt callback
  \param[in]   pvt           pvt handle
  \param[in]   callback      user callback
  \param[in]   arg           user param
  \return      Error code
*/
csi_error_t csi_pvt_attach_callback(csi_pvt_t *pvt, void *callback, void *arg);

/**
  \brief       un-register pvt interrupt callback
  \param[in]   pvt           pvt handle
  \return      Error code
*/
csi_error_t csi_pvt_detach_callback(csi_pvt_t *pvt);
/**
  \brief       convert ts-rawdata to mini degrees centigrage
  \param[in]   pvt           pvt handle
  \param[in]   raw_data      digital output value
  \param[out]  temp_mc       value after converted ,unit in mini degrees centigrades
  \return      void
*/
void csi_pvt_ts_convert_raw_data(csi_pvt_t *pvt, uint32_t raw_data, uint32_t *temp_mc);
/**
  \brief       convert vm-rawdata to mini degrees voltage
  \param[in]   pvt           pvt handle
  \param[in]   raw_data      digital output value
  \param[out]  temp_mv       value after converted ,unit in mv
  \return      void
*/
void csi_pvt_vm_convert_raw_data(csi_pvt_t *pvt, uint32_t raw_data, uint32_t *vol_mv);
/**
  \brief       convert pd-rawdata to mini degrees voltage
  \param[in]   pvt           pvt handle
  \param[in]   raw_data      digital output value
  \param[out]  speed_khz     frequecy, uint in khz
  \return      void
*/
void csi_pvt_pd_convert_raw_data(csi_pvt_t *pvt, uint32_t raw_data, uint32_t *speed_khz);

/**
  \brief       select pd delay-chain ,range in [1..6]
  \param[in]   pvt           pvt handle
  \param[in]   pd_port       delay chain port
  \return      Error code
*/
csi_error_t csi_pvt_pd_port_select(csi_pvt_t *pvt, uint32_t pd_port);

/**
  \brief       create a thermal-high alarm for ts-ip
  \param[in]   pvt           pvt handle
  \param[in]   sensor_idx    ts-ip-index
  \param[in]   hyst_thresh   sample value for hysteresis
  \param[in]   alarm_thresh  sample value for alarm
  \return      Error code
*/
csi_error_t csi_pvt_create_ts_rising_alarm(csi_pvt_t *pvt, uint32_t sensor_idx, uint16_t hyst_thresh, uint16_t alarm_thresh);


/**
  \brief       create a thermal-low alarm for ts-ip
  \param[in]   pvt           pvt handle
  \param[in]   sensor_idx    ts-ip-index
  \param[in]   hyst_thresh   sample value for hysteresis
  \param[in]   alarm_thresh  sample value for alarm
  \return      Error code
*/
csi_error_t csi_pvt_create_ts_falling_alarm(csi_pvt_t *pvt, uint32_t sensor_idx, uint16_t hyst_thresh, uint16_t alarm_thresh);
csi_error_t csi_pvt_trig_ts_irq(csi_pvt_t *pvt, uint16_t sensor_idx, uint32_t tst_pattern);
#ifdef __cplusplus
}
#endif

#endif
