/*
 * Copyright (C) 2021 Alibaba Group Holding Limited
 */

/******************************************************************************
  * @file     main.c
  * @brief    fm aon low power
  * @version  V1.0
  * @date     07. Feb 2021
  ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <soc.h>
#include "board_config.h"
#include "board_init.h"
#include "syslog.h"

#include "aon_common.h"

#define PVT_IDX (0)

#ifdef CONFIG_AON_CONF_LOCAL
__attribute__((section(".aon_conf"))) aon_config_t g_conf = {
    .magic                     = AON_CONFIG_MAGIC,
    .version                   = AON_CONFIG_VERSION,
    .uboot_set_magic           = UBOOT_CONFIG_MAGIC,
    .max_hw_id_num             = PMIC_MAX_HW_ID_NUM,
    .aon_config_partition_size = AON_CONFIG_PARTITION_SIZE,
    .wakeup_flag               = AON_WAKEUP_BY_GPIO | AON_WAKEUP_BY_RTC,
    .aon_pmic =
        {
            .iic_config               = {0, IIC_ADDRESS_7BIT, IIC_BUS_SPEED_FAST_PLUS},
            .regu_num             = ARRAY_SIZE(g_regu_id),
            .pmic_dev_num         = ARRAY_SIZE(g_pmic_devices),
            .iic_config_offset    = 0,
            .pmic_dev_list_offset = 0,
            .regu_id_list_offset  = 0,
        },
};

#else

__attribute__((section(".aon_conf"))) aon_config_t g_conf = {
    .magic                     = AON_CONFIG_MAGIC,
    .version                   = AON_CONFIG_VERSION,
    .max_hw_id_num             = PMIC_MAX_HW_ID_NUM,
    .aon_config_partition_size = AON_CONFIG_PARTITION_SIZE,
    .wakeup_flag               = 0,
    .aon_pmic =
        {
            .iic_config           = {0},
            .regu_num             = 0,
            .pmic_dev_num         = 0,
            .pmic_dev_list_offset = 0,
            .regu_id_list_offset  = 0,
        },
};
#endif

static int aon_submodule_init()
{
        csi_error_t ret;
        /*aon rpc init*/
        ret = aon_rpc_init();
        if (ret) {
                LOG_E("aon rpc init failed\n");
                goto FAIL;
        }

        /*aon pmic init*/
        printf("wake_flag:%d pmic_dev_list_offset:%d regu_id_list_offset:%d iic_id:%d addr_mode:%d speed:%d\n",
              g_conf.wakeup_flag, g_conf.aon_pmic.pmic_dev_list_offset,    \
              g_conf.aon_pmic.regu_id_list_offset, g_conf.aon_pmic.iic_config.iic_id,  \
			  g_conf.aon_pmic.iic_config.addr_mode, g_conf.aon_pmic.iic_config.speed);
        pmic_config_t aon_conf = {
            .iic_config.iic_id       = g_conf.aon_pmic.iic_config.iic_id,
			.iic_config.addr_mode    = g_conf.aon_pmic.iic_config.addr_mode,
			.iic_config.speed        = g_conf.aon_pmic.iic_config.speed,
            .pmic_dev_num = g_conf.aon_pmic.pmic_dev_num,
            .regu_num     = g_conf.aon_pmic.regu_num,
#ifdef CONFIG_AON_CONF_LOCAL
            .pmic_dev_list = g_pmic_devices,
            .regu_id_list  = g_regu_id,
#else
            .pmic_dev_list =
                (pmic_dev_info_t *)((char *)&g_conf +
                                    g_conf.aon_pmic.pmic_dev_list_offset),
            .regu_id_list =
                (csi_regu_id_t *)((char *)&g_conf +
                                  g_conf.aon_pmic.regu_id_list_offset),
#endif
        };

        if (!g_conf.aon_pmic.pmic_dev_num) {
                LOG_E("aon pmic dev num is zero\n");
                goto FAIL;
        }

        if (!g_conf.aon_pmic.regu_num) {
                LOG_E("aon regu num is zero\n");
                goto FAIL;
        }

        if (strncmp(g_conf.uboot_set_magic, UBOOT_CONFIG_MAGIC,
                    strlen(UBOOT_CONFIG_MAGIC))) {
                LOG_E("No uboot magic flag found, please update u-boot\n");
                goto FAIL;
        }

        ret = aon_pmic_init(&aon_conf);
        if (ret) {
                LOG_E("aon rpc init failed\n");
                goto FAIL;
        }

        /*aon pvt init*/
        ret = aon_pvt_init(PVT_IDX);
        if (ret != CSI_OK) {
                LOG_E("aon pvt init failed\n");
                goto FAIL;
        }

        /* start aon-wdt monitor */
        ret = aon_wdt_init(AON_HW_WDT_USE_PMIC);
        if (ret != CSI_OK) {
                LOG_E("ERR->aon_wdt_init failed\n");
                goto FAIL;
        }

        ret = aon_wdt_start();
        if (ret != CSI_OK) {
                LOG_E("ERR->aon_wdt_start failed\n");
                goto FAIL;
        }

        ret = aon_misc_init();
        if (ret != CSI_OK) {
                LOG_E("aon misc init failed\n");
                goto FAIL;
        }

        ret = aon_sys_init();
        if (ret != CSI_OK) {
                LOG_E("aon sys init failed\n");
                goto FAIL;
        }
#ifdef CONFIG_PM
        ret = aon_lpm_init(g_conf.wakeup_flag);
        if (ret != CSI_OK) {
                LOG_E("aon lpm init failed\n");
                goto FAIL;
        }
#endif

#ifdef CONFIG_AON_MONITOR
        aon_dbg_monitor();
#endif
        //LOG_D("\nenter rpc message process\n");

        aon_rpc_process_loop();
FAIL:
        asm volatile("j .");
        return 0;
}

int main(void)
{
        /*board base init: uart*/
        board_init();

        printf("$$$$Light FM aon low power solution - build %s %s!\n", __DATE__,
               __TIME__);

        aon_submodule_init();
        return 0;
}

void pre_main(void)
{
        main();
}
