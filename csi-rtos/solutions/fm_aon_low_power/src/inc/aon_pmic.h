
#ifndef __AON_PMIC_H_
#define __AON_PMIC_H_

#include "drv/pmic.h"

enum aon_rpc_pm_svc_en {
    PM_FUNC_UNKNOWN                 = 0U,
    PM_FUNC_SET_RESOURCE_REGULATOR  = 1U, /*设置regu 电压*/
    PM_FUNC_GET_RESOURCE_REGULATOR  = 2U, /*获取regu 电压*/
    PM_FUNC_SET_RESOURCE_POWER_MODE = 3U, /*设置 regu 工作模式*/
    PM_FUNC_PWR_SET                 = 4U, /*设置regu 开关状态*/
    PM_FUNC_PWR_GET                 = 5U, /*获取regu 开关状态*/
    PM_FUNC_CHECK_FAULT				= 6U, /*查看pmic 错误码*/
    PM_FUNC_GET_TEMPERATURE				= 7U, /*查看pmic 状态*/
    //PM_FUNC_GET_PMIC_INFO           = 8U,  /*获取pmic信息*/
    //PM_FUNC_GET_PMIC_HWREGULATOR    = 10U,
};

#define   AON_CONFIG_MAGIC   "AON_CONFIG"
#define   UBOOT_CONFIG_MAGIC  "UBOOT_SET"
#define   AON_CONFIG_VERSION "1.0.0"
#define   AON_CONFIG_PARTITION_SIZE 3096

typedef  struct __packed{
	csi_pmic_if_config_t iic_config;
    uint8_t pmic_dev_num;
    uint8_t regu_num;
	uint32_t pmic_dev_list_offset;
	uint32_t regu_id_list_offset;
}aon_pmic_config_t;

typedef  struct  __packed{
   const char  magic[11];
   const char  version[11];
   const char  uboot_set_magic[11];
   uint8_t   max_hw_id_num;
   uint64_t  aon_config_partition_size;
   uint32_t  wakeup_flag;
   aon_pmic_config_t aon_pmic;
} aon_config_t;

int aon_pmic_init(pmic_config_t* pmic_conf);
int aon_pmic_poweroff();
csi_pmic_t* aon_pmic_get_handler();

#endif
