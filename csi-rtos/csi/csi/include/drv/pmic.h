/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file       drv/pmic.h
 * @brief      Header File for pmic Driver
 * @version    V1.0
 * @date       9. Feb 2021
 * @model      pmic
 ******************************************************************************/

#ifndef _DRV_PMIC_H_
#define _DRV_PMIC_H_

#include <drv/common.h>
#include <drv/iic.h>
#include <drv/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CONFIG_PMIC_MAX_HW_ID_NUM
#define PMIC_MAX_HW_ID_NUM 3
#else
#define PMIC_MAX_HW_ID_NUM CONFIG_PMIC_MAX_HW_ID_NUM
#endif

#define PMIC_MAX_COUPLING_LIST_NUM 3


#define PMIC_DRV_SUPPORT_WDT                       (1U << 0)
#define PMIC_DRV_SUPPORT_ERR_IO                    (1U << 1)
#define PMIC_DRV_SUPPORT_LPM_IO                    (1U << 2)
#define PMIC_DRV_SUPPORT_AUTO_REBOOT               (1U << 3)

#define PMIC_DEV_ENABLE_WDT                (1U << 0)
#define PMIC_DEV_ENABLE_ERR_IO             (1U << 1)
#define PMIC_DEV_ENABLE_LPM_IO             (1U << 2)
#define PMIC_DEV_ENABLE_AUTO_REBOOT        (1U << 3)

#define  PMIC_DEBUG    0
#define	 ANT_DEBUG	   0

#define HW_ID_NO_SOFT_AUTO_ON   (0xff)
#define HW_ID_NO_SOFT_AUTO_OFF  (0xff)
#define HW_ID_INVALID           (0xff)
#define PMIC_ID_INVALID         (0xff)

#define REGU_EXT_ID_NAME_LEN 30
#define PMIC_DEV_NAME_LEN 20
#define PMIC_DEV_VERSION_LEN 20

#define PMIC_DEV_ADDR(id, pmic_dev)   (pmic_dev->dev_info->addr##id)

typedef enum {
   HW_ID_ACTIVATE_HIGH  = 0U,
   HW_ID_ACTIVATE_LOW =   1U,
} hw_activate_status_en;

typedef struct __packed {
	uint8_t pmic_id;
    uint8_t io_hw_id;
    uint8_t activate_status;
} pmic_parent_hw_io_ctrl_info_t;

typedef struct __packed  {
    uint8_t   on_order;
	uint8_t   on_delay_ms;
    uint32_t  init_target_uv;
} regu_soft_power_ctrl_on_t;

typedef struct __packed {
    uint8_t off_order;
	uint8_t off_delay_ms;
} regu_soft_power_ctrl_off_t;

typedef struct __packed {
   regu_soft_power_ctrl_on_t  on_info;
   regu_soft_power_ctrl_off_t off_info;
} regu_soft_power_ctrl_t;

/* Describes coupling of regulators */
typedef struct __packed {
	uint8_t id0;
	uint8_t id1;
	int8_t max_spread; // mv/10
	int8_t min_spread; // mv/10
}coupling_desc_t;

typedef struct __packed {
   uint8_t pmic_id;
   uint8_t hw_id;
   uint8_t benable;
   pmic_parent_hw_io_ctrl_info_t parent_hw_info;
   regu_soft_power_ctrl_t soft_power_ctrl_info;
} pmic_hw_info_t;
typedef	struct __packed{
	coupling_desc_t coupling_list[PMIC_MAX_COUPLING_LIST_NUM];
	pmic_hw_info_t  id[PMIC_MAX_HW_ID_NUM];                             ///<  sub id1 for single-rail or first src of dual-rail
}pmic_hw_id_t;

typedef struct __packed {
	uint8_t     regu_ext_id;										 ///< virtual global regulator id
	char        regu_ext_id_name[REGU_EXT_ID_NAME_LEN];              ///< vitual regu-id name
	pmic_hw_id_t sub;						                         ///<  sub id set for dual-rail/single-rail regulator
}csi_regu_id_t;

typedef enum {
   PMIC_CTRL_BY_AON_GPIO =  0U,
   PMIC_CTRL_BY_PMIC_GPIO = 1U,
   PMIC_CTRL_BY_NOTHINTG  = 0xFF,
} pmic_ctrl_info_en;

typedef enum _PMIC_BUCK_STATE {
	SYNC = 0U,
	SLEEP,
	AUTO,
} pmic_buck_mode;

typedef struct __packed {
    uint8_t gpio_port;
	uint8_t pin;
    uint8_t activate_status;
} pmic_ctrl_by_aon_info_t;

typedef struct __packed {
	uint8_t pmic_id;
    uint8_t io_hw_id;
    uint8_t activate_status;
} pmic_ctrl_by_pmic_info_t;

typedef struct __packed {
    uint8_t pmic_ctrl_type;
	union {
       pmic_ctrl_by_aon_info_t  aon_io;
	   pmic_ctrl_by_pmic_info_t pmic_io;
	} info;
} pmic_parent_ctrl_info_t;

typedef struct  __packed{
	 uint8_t gpio_port;
     uint8_t pin;
	 uint8_t  trigger_mode; /*ecqual to csi_gpio_irq_mode_t*/
} pmic_interrupt_io_info_t;

typedef struct __packed  {
    char device_name[PMIC_DEV_NAME_LEN];
    char version_name[PMIC_DEV_VERSION_LEN];
    uint8_t pmic_id;
    uint8_t addr1;
    uint8_t addr2;
	uint8_t flag;          /*support wdt|errio| lpm io*/
	uint8_t slew_rate;
	uint32_t    wdt_len;
	pmic_interrupt_io_info_t err_io_info;
	pmic_interrupt_io_info_t lpm_io_info;
    pmic_parent_ctrl_info_t ctrl_info;
} pmic_dev_info_t;


typedef struct  csi_pmic csi_pmic_t;
typedef struct csi_pmic_dev csi_pmic_dev_t;

typedef struct {
	/*gpio funciton*/
	csi_error_t (*gpio_output)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t gpio_index, uint32_t val);
	/* modify voltage for regulator*/
	csi_error_t (*set_voltage)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t regu_id,uint32_t target_uv);
	/* get voltage from regulator*/
	csi_error_t (*get_voltage)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev,uint32_t regu_id,uint32_t *target_uv);
#ifdef PMIC_DEBUG
	csi_error_t (*set_reg)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t reg, uint32_t val);
	csi_error_t (*get_reg)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t reg, uint32_t *val);
#endif
	csi_error_t (*get_powerup)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev,  uint32_t *flag);
	/* find regu obj from curent pmic-dev*/
	//csi_error_t (*is_exist)(csi_pmic_dev_t *pmic_dev,uint32_t regu_ext_id,uint32_t *is_dual_rail);
	/* enable regulator */
	csi_error_t (*regu_power_ctrl)(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev,uint32_t regu_id,uint32_t bon);
	/* get pmic temperature */
	csi_error_t (*get_temperature)(csi_pmic_t *pmic,csi_pmic_dev_t *pmic_dev, uint32_t* val);
	/* read pmic faults */
	csi_error_t (*read_faults)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
	/* select pmic output mode */
	csi_error_t (*mode_select)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t regu_id, pmic_buck_mode mode);
	/* get pmic chip id */
	csi_error_t (*read_id)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
	/* set pmic log level */
	csi_error_t (*set_log_level)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev,  uint8_t level);
	/*init pmic device */
	csi_error_t (*init)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
	/*uninit pmic device */
	csi_error_t (*uninit)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
}csi_regu_ops_t;

typedef enum {
	CSI_STATUS_PMIC_DEV_NOT_INIT   = 0U,
	CSI_STATUS_PMIC_DEV_INIT_FAILD,
	CSI_STATUS_PMIC_DEV_UNINIT_FAILD,
	CSI_STATUS_PMIC_DEV_OK,
	CSI_STATUS_PMIC_DEV_OV,
	CSI_STATUS_PMIC_DEV_UV,
	CSI_STATUS_PMIC_DEV_OC,
	CSI_STATUS_PMIC_DEV_OT,
} csi_pmic_dev_status_en;

typedef struct {
	uint8_t                 is_fault;			///< fault condition
	csi_pmic_dev_status_en	dev_status;			///<  OT, OV, UV, OC, IIC error
	//uint32_t			    temperature;				///< measurable value, includes cur temp, voltage.
}csi_pmic_dev_status_t;

typedef struct {
    csi_error_t (*set_timer_len)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev, uint32_t msecond);
    csi_error_t (*start)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
    csi_error_t (*stop)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
    csi_error_t (*feed)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
    csi_error_t (*repower)(csi_pmic_t *pmic, csi_pmic_dev_t *pmic_dev);
}csi_wdt_ops_t;

typedef struct __packed {
	const char *name;                       ///<name of regulator
	uint8_t    id;                          ///<id of regulator
	uint8_t   initial_benable;				 ///< whether regulator is enabled
	uint32_t    vreg;                        ///< voltage setting register of regulator
	uint32_t   vmask;                       ///< voltage setting mask of regulator
	uint32_t   vconf;						///< voltage configuration
	uint32_t   n_voltages;                  ///< number of selectors avaliable for list voltage
    const unsigned int *volt_table;         ///< voltage mapping table(if table based mapping)
	uint32_t   ereg;                        ///< regiater for control
	uint32_t   emask;                       ///< regulator control register mask
	uint32_t   linear_min_sel;				///< minimal selector for starting linear mapping
	uint32_t   min_uv;                      ///< min uv supported by regulator
	uint32_t   max_uv;                      ///< max uv supported by regulator
	//uint32_t   target_uv;                   ///< target uv of regulator
	uint32_t   step_uv;                     ///< step uv per-unit
}csi_regu_t;

typedef struct csi_pmic_drv_data {
	uint16_t          features;
	uint16_t          regu_num;             ///< number of ext-regu resource belong to pmic-dev
   	csi_regu_ops_t*   ops;                  ///< regu ops
	csi_wdt_ops_t*    wdt_ops;              ///< wdt  ops
	csi_regu_t*       regu_info;            ///< params of individual regulator
}csi_pmic_drv_data_t;

typedef struct csi_pmic_dev {
	pmic_dev_info_t*         dev_info;
    csi_pmic_drv_data_t*  drv_data;
	csi_pmic_dev_status_t      pmic_dev_status;  ///< status of chip or specific regulator
} csi_pmic_dev_t;

typedef enum {
	CSI_STATUS_PMIC_NOT_INIT   = 0U,
	CSI_STATUS_PMIC_OK,
	CSI_STATUS_PMIC_DEV_REG_ERR,
	CSI_STATUS_PMIC_REGU_REG_ERR,
	CSI_STATUS_PMIC_SOFT_POWER_CTRL_ERR,
	CSI_STATUS_PMIC_IIC_CONFIG_ERR,
	CSI_STATUS_PMIC_IIC_ERR,
} csi_pmic_status_en;

typedef enum  {
	/* C902 event rebootmode */
	CHIP_EVENT_PMIC_RESET = 0x0,
	CHIP_EVENT_PMIC_ONKEY,
	CHIP_EVENT_PMIC_POWERUP,

	/* C910 event rebootmode */
	CHIP_EVENT_SW_REBOOT = 0x20,
	CHIP_EVENT_SW_WATCHDOG,
	CHIP_EVENT_SW_PANIC,
	CHIP_EVENT_SW_HANG,
	CHIP_EVENT_MAX,
}chip_rebootmode_index_en;


typedef struct {
   const char* device_name;
   const char* version_name;
   csi_pmic_drv_data_t *drv_data;
} pmic_drv_info_t;


typedef struct
{
	uint8_t     regu_ext_id;                ///< virtual regulator ID
	uint32_t    target_uv[PMIC_MAX_HW_ID_NUM];                 ///< voltage for first rail,unit in uv
}regu_voltage_param_t;

typedef struct __packed{
	uint8_t  iic_id;                         ///< iic id
	uint8_t  addr_mode;              ///< iic addr_mode  ---> csi_iic_addr_mode_t
	uint8_t      speed;             ///< iic speed type  ---> csi_iic_speed_t
	uint8_t      reserved[1];
} csi_pmic_if_config_t;


typedef struct {
	csi_pmic_if_config_t iic_config;
    uint8_t pmic_dev_num;
    uint8_t regu_num;
    pmic_dev_info_t* pmic_dev_list;
    csi_regu_id_t* regu_id_list;
} pmic_config_t;


struct csi_pmic {
	/* pmic status handler */
	csi_pmic_status_en pmic_status;
	/* handle of pmic-interface-dev*/
	csi_iic_t  if_dev;
	uint8_t       dev_num;
	/* pmic device list*/
	csi_pmic_dev_t* dev_list;
	uint8_t       regu_num;
	/*pmic regu list*/
	csi_regu_id_t*  regu_list;

	uint8_t    hw_wdt_pmic_id;
	/* wdt ops, note: only one device can support wdt func whthin dev_list_head */
	csi_wdt_ops_t*  wdt_ops;
	/* interrupt call-back for pmic */
	void (*call_back)(csi_pmic_t *pmic, void *arg);
	/* method for writing reg to pmic-ic */
	csi_error_t (*pmic_write_reg)(csi_pmic_t *pmic, uint16_t dev_addr,uint32_t offset, uint32_t val);
	/* method for reading reg to pmic-ic,reading format A: issue a siginle write op first ,then a single read op */
	csi_error_t (*pmic_read_reg)(csi_pmic_t *pmic, uint16_t dev_addr,uint32_t offset, uint32_t *val);
	/* method for reading reg to pmic-ic,reading format B: generate Sr timing between write and read op */
	csi_error_t (*pmic_read_reg_sr)(csi_pmic_t *pmic, uint16_t dev_addr,uint32_t offset, uint32_t *val);
	/* method for writing reg multi bytes to pmic-ic */
	csi_error_t (*pmic_write_reg_multi_bytes)(csi_pmic_t *pmic, uint16_t dev_addr,uint32_t offset, uint8_t *data, uint8_t size);
	/* method for reading reg multi bytes to pmic-ic,reading format A: issue a siginle write op first ,then a single read op */
	csi_error_t (*pmic_read_reg_multi_bytes)(csi_pmic_t *pmic, uint16_t dev_addr,uint32_t offset, uint8_t *val, uint8_t *get_len);
	void  *arg;
	void  *priv;
};





/**
  \brief       Initialize pmic handle
               Initializes pmic driver and return pmic handle
  \param[in]   pmic  pmic handle.
  \param[in]   iic_id pmic use iic as hw interface to access ext-pmic-ic, iic_id indicate that
               pmic use which iic instance to access external device
  \return      Error code \ref csi_error_t.
*/
csi_error_t csi_pmic_init(csi_pmic_t *pmic, pmic_config_t* config);

/**
  \brief       un-initialize pmic handle
               un-initializes pmic driver ,release global resource referenced by pmic
  \param[in]   pmic  pmic handle.
  \return      void.
*/
void        csi_pmic_uninit(csi_pmic_t *pmic);

/**
  \brief       config pmic-interface before access external devices
  \param[in]   pmic          pmic handle.
  \param[in]   if_config     detail attributes of pmic-interface ,pls refer to "if_config"
  \return      Error code \ref csi_error_t.
*/
csi_error_t csi_pmic_if_config(csi_pmic_t *pmic, csi_pmic_if_config_t *if_config);

/**
  \brief       modify regulator's voltate
  \param[in]   pmic          pmic handle.
  \param[in]   regu_param    detail info pls refer to regu_voltage_param_t
  \return      Error code \ref csi_error_t.
*/
csi_error_t csi_pmic_regulator_set_voltage(csi_pmic_t *pmic, regu_voltage_param_t regu_param);

/**
  \brief       get regulator's voltate
  \param[in]   pmic          pmic handle.
  \param[in]   regu_param    detail info pls refer to regu_voltage_param_t
  \return      Error code \ref csi_error_t.
*/
csi_error_t csi_pmic_regulator_get_voltage(csi_pmic_t *pmic, regu_voltage_param_t *regu_param);


/**
  \brief       power on regulator
  \param[in]   pmic          pmic handle.
  \param[in]   regu_ext_id    regu ext id
  \param[in]   enable         1:enable 0:disable
  \return      Error code \ref csi_error_t.
*/
csi_error_t csi_pmic_regulator_ctrl(csi_pmic_t *pmic, uint8_t  regu_ext_id, bool enable);

/**
  \brief       check whether specific regulator is enable
  \param[in]   pmic          pmic handle.
  \param[in]   regu_param    detail info pls refer to regu_voltage_param_t
  \param[out]  benable       enable state of regulator
  \return      Error code \ref csi_error_t.
*/
csi_error_t csi_pmic_regulator_is_enable(csi_pmic_t *pmic, uint8_t  regu_ext_id, uint32_t *benable);

/**
  \brief       set wdt timer length which support by external pmic-ic
  \param[in]   pmic          pmic handle.
  \param[in]   tmr_len       timer length, uinit in minisecond
  \return      Error code \ref csi_error_t.
*/
csi_error_t csi_pmic_wdt_set_timer(csi_pmic_t *pmic,uint32_t tmr_len);

/**
  \brief       start pmic-wdt function
  \param[in]   pmic          pmic handle.
  \return      Error code \ref csi_error_t.
*/
csi_error_t csi_pmic_wdt_start(csi_pmic_t *pmic);

/**
  \brief       stop pmic-wdt function
  \param[in]   pmic          pmic handle.
  \return      Error code \ref csi_error_t.
*/
csi_error_t csi_pmic_wdt_stop(csi_pmic_t *pmic);
/**
  \brief       check whether wdt is enable which is whithin pmic-ic
  \param[in]   pmic          pmic handle.
  \param[out]  benable       enable status of wdt-function
  \return      Error code \ref csi_error_t.
*/
csi_error_t csi_pmic_wdt_is_enable(csi_pmic_t *pmic,uint32_t *benable);

/**
  \brief       feed wdt
  \param[in]   pmic          pmic handle.
  \return      Error code \ref csi_error_t.
*/
csi_error_t csi_pmic_wdt_feed(csi_pmic_t *pmic);

/**
  \brief       get regu hw num
  \param[in]   pmic          pmic handle.
  \param[in]   regu_ext_id   regu_id.
  \return      Error code \ref csi_error_t.
*/
int pmic_get_regu_hw_id_num(csi_pmic_t *pmic, uint32_t regu_ext_id);

/**
  \brief       trige pmic excecute power off all regulator with default regulator
               power off sequence ,then generate reset signal into soc,and power on all
               regulator by default order
  \param[in]   pmic          pmic handle.
  \return      Error code \ref csi_error_t.
*/
csi_error_t csi_pmic_repower(csi_pmic_t *pmic);

csi_error_t csi_pmic_get_reg(csi_pmic_t *pmic, uint8_t pmic_id, uint32_t reg, uint32_t *val);
csi_error_t csi_pmic_set_reg(csi_pmic_t *pmic, uint8_t pmic_id, uint32_t reg, uint32_t val);
csi_error_t csi_pmic_get_powerup_flag(csi_pmic_t *pmic, uint32_t *flag);
/**
  \brief       get pmic-virtual name list for debug utility
  \param[out]  pmic_rsc_list        pmic id list pointer.
  \return      NULL.
*/

/**
  \brief       get autoboot flag
  \param[in]   pmic      pmic handle
  \param[out]  bsupport: 0: unsupport autoboot 1: support autoboot
  \return      err code.
*/
csi_error_t csi_pmic_get_autoboot_flag(csi_pmic_t *pmic, uint32_t *bsupport);

csi_error_t csi_pmic_read_faults(csi_pmic_t *pmic, uint8_t pmic_id);

csi_error_t csi_pmic_read_temperature(csi_pmic_t *pmic, uint32_t *val);

/**
  \brief       select pmic output mode
  \param[in]   pmic      pmic handle
  \param[in]   regu_ext_id
  \param[in]   mode			pmic lowpower mode
  \return      err code.
*/
csi_error_t csi_pmic_mode_select(csi_pmic_t *pmic, uint8_t regu_ext_id, pmic_buck_mode mode);
/**
  \brief       set log level
  \param[in]   pmic      pmic handle
  \param[in]   level     log level
  \return      err code.
*/
csi_error_t csi_pmic_set_log_level(csi_pmic_t *pmic, uint8_t pmic_id, uint8_t level);

/**
  \brief       pmic soft power off
  \param[in]   pmic      pmic handle
  \return      err code.
*/
csi_error_t csi_pmic_soft_power_off(csi_pmic_t *pmic);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_PMIC_H_ */
