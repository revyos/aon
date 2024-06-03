#include <drv/common.h>
#include <drv/pmic.h>
#include "syslog.h"
#include "power.h"
#include "aon_common.h"

static csi_pmic_t g_pmic;

#ifdef CONFIG_AON_CONF_LOCAL
/*for LIGHT A debug*/

#include "da9063.h"
#include "da9121.h"
#include "slg51000.h"

__attribute__((section(".aon_conf"))) pmic_dev_info_t g_pmic_devices[] = {
    {"dialog,da9063",
     "v1",
     0,
     (0xB4 >> 1),
     (0xB6 >> 1),
     PMIC_DEV_ENABLE_WDT | PMIC_DEV_ENABLE_ERR_IO,
     0,
     {0, (1 << 14), GPIO_IRQ_MODE_LOW_LEVEL}},
    {"dialog,da9121", "v1", 1, 0x68, 0x68, 0}, /*2口高电平有效*/
    {"dialog,slg51000", "v1", 2, 0x75, 0x75, 0},
};

__attribute__((section(".aon_conf"))) csi_regu_id_t g_regu_id[] = {
    REGU_ID_SINGLE_RAIL_DEF(SOC_DVDD18_AON, 0, DA9063_ID_LDO3, PMIC_ID_INVALID,
                            HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            HW_ID_NO_SOFT_AUTO_ON, 0, 0, HW_ID_NO_SOFT_AUTO_OFF,
                            0),
    REGU_ID_SINGLE_RAIL_DEF(SOC_AVDD33_USB3, 0, DA9063_ID_LDO9, PMIC_ID_INVALID,
                            HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            HW_ID_NO_SOFT_AUTO_ON, 0, 0, HW_ID_NO_SOFT_AUTO_OFF,
                            0),
    REGU_ID_SINGLE_RAIL_DEF(SOC_DVDD08_AON, 0, DA9063_ID_LDO2, PMIC_ID_INVALID,
                            HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            HW_ID_NO_SOFT_AUTO_ON, 0, 0, HW_ID_NO_SOFT_AUTO_OFF,
                            0),
    REGU_ID_DUAL_RAIL_DEF(
        SOC_APCPU_DVDD_DVDDM, 0, DA9063_ID_BCORE1, 0, DA9063_ID_BCORE2, 5, -30,
        PMIC_ID_INVALID, HW_ID_INVALID, HW_ID_ACTIVATE_LOW, 0, 1, 800000,
        HW_ID_NO_SOFT_AUTO_OFF, 0, 0, DA9063_ID_BCORE2, 0, DA9063_ID_BCORE1, 30,
        -5, PMIC_ID_INVALID, HW_ID_INVALID, HW_ID_ACTIVATE_LOW, 1, 1, 800000,
        HW_ID_NO_SOFT_AUTO_OFF, 0),
    REGU_ID_SINGLE_RAIL_DEF(SOC_DVDD08_DDR, 0, DA9063_ID_BPERI, PMIC_ID_INVALID,
                            HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            HW_ID_NO_SOFT_AUTO_ON, 0, 0, HW_ID_NO_SOFT_AUTO_OFF,
                            0),
    REGU_ID_SINGLE_RAIL_DEF(SOC_VDD_DDR_1V8, 0, DA9063_ID_LDO4, PMIC_ID_INVALID,
                            HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            HW_ID_NO_SOFT_AUTO_ON, 0, 0, HW_ID_NO_SOFT_AUTO_OFF,
                            0),
    REGU_ID_DUAL_RAIL_DEF(
        SOC_VDD_DDR_1V1, 0, DA9063_ID_BMEM, PMIC_ID_INVALID, HW_ID_INVALID, 0,
        0, PMIC_ID_INVALID, HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
        HW_ID_NO_SOFT_AUTO_ON, 0, 0, HW_ID_NO_SOFT_AUTO_OFF, 0, 0,
        DA9063_ID_BIO, PMIC_ID_INVALID, HW_ID_INVALID, 0, 0, PMIC_ID_INVALID,
        HW_ID_INVALID, HW_ID_ACTIVATE_LOW, HW_ID_NO_SOFT_AUTO_ON, 0, 0,
        HW_ID_NO_SOFT_AUTO_OFF, 0),
    REGU_ID_SINGLE_RAIL_DEF(SOC_VDD_DDR_0V6, 0, DA9063_ID_BPRO, PMIC_ID_INVALID,
                            HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            HW_ID_NO_SOFT_AUTO_ON, 0, 0, HW_ID_NO_SOFT_AUTO_OFF,
                            0),
    REGU_ID_SINGLE_RAIL_DEF(SOC_DVDD18_AP, 0, DA9063_ID_LDO11, PMIC_ID_INVALID,
                            HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            HW_ID_NO_SOFT_AUTO_ON, 0, 0, HW_ID_NO_SOFT_AUTO_OFF,
                            0),
    REGU_ID_SINGLE_RAIL_DEF(SOC_AVDD08_MIPI_HDMI, 0, DA9063_ID_LDO1,
                            PMIC_ID_INVALID, HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            HW_ID_NO_SOFT_AUTO_ON, 0, 0, HW_ID_NO_SOFT_AUTO_OFF,
                            0),
    REGU_ID_SINGLE_RAIL_DEF(SOC_AVDD18_MIPI_HDMI, 0, DA9063_ID_LDO5,
                            PMIC_ID_INVALID, HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            HW_ID_NO_SOFT_AUTO_ON, 0, 0, HW_ID_NO_SOFT_AUTO_OFF,
                            0),
    REGU_ID_SINGLE_RAIL_DEF(SOC_DVDD33_EMMC, 0, DA9063_ID_LDO10,
                            PMIC_ID_INVALID, HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            HW_ID_NO_SOFT_AUTO_ON, 0, 0, HW_ID_NO_SOFT_AUTO_OFF,
                            0),
    REGU_ID_SINGLE_RAIL_DEF(SOC_DOVDD18_SCAN, 0, DA9063_ID_LDO6,
                            PMIC_ID_INVALID, HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            2, 1, 0, 8, 1),
    REGU_ID_SINGLE_RAIL_DEF(SOC_VEXT_2V8, 0, DA9063_ID_LDO7, PMIC_ID_INVALID,
                            HW_ID_INVALID, HW_ID_ACTIVATE_LOW, 3, 1, 2800000, 9,
                            1),
    REGU_ID_SINGLE_RAIL_DEF(SOC_DVDD12_SCAN, 0, DA9063_ID_LDO8, PMIC_ID_INVALID,
                            HW_ID_INVALID, HW_ID_ACTIVATE_LOW, 4, 1, 1200000,
                            10, 1),
    REGU_ID_SINGLE_RAIL_DEF(SOC_AVDD28_SCAN_EN, 0, DA9063_ID_GPIO4,
                            PMIC_ID_INVALID, HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            5, 1, 2800000, 6, 1),
    REGU_ID_SINGLE_RAIL_DEF(SOC_DVDD08_AP, 1, DA9121_ID_BUCK1, 0, 2,
                            HW_ID_ACTIVATE_HIGH, HW_ID_NO_SOFT_AUTO_ON, 0, 0,
                            HW_ID_NO_SOFT_AUTO_OFF, 0),
    REGU_ID_SINGLE_RAIL_DEF(SOC_AVDD28_RGB, 2, SLG51000_ID_LDO1,
                            PMIC_ID_INVALID, HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            6, 1, 0, 0, 1),
    REGU_ID_SINGLE_RAIL_DEF(SOC_AVDD25_IR, 2, SLG51000_ID_LDO2, PMIC_ID_INVALID,
                            HW_ID_INVALID, HW_ID_ACTIVATE_LOW, 7, 1, 0, 1, 1),
    REGU_ID_SINGLE_RAIL_DEF(SOC_DVDD18_EMMC, 2, SLG51000_ID_LDO3, 0, 7,
                            HW_ID_ACTIVATE_HIGH, HW_ID_NO_SOFT_AUTO_ON, 0, 0,
                            HW_ID_NO_SOFT_AUTO_OFF, 0),
    REGU_ID_SINGLE_RAIL_DEF(SOC_DOVDD18_RGB, 2, SLG51000_ID_LDO4,
                            PMIC_ID_INVALID, HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            8, 1, 0, 2, 1),
    REGU_ID_SINGLE_RAIL_DEF(SOC_DVDD12_RGB, 2, SLG51000_ID_LDO5,
                            PMIC_ID_INVALID, HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            9, 1, 0, 3, 1),
    REGU_ID_SINGLE_RAIL_DEF(SOC_DVDD12_IR, 2, SLG51000_ID_LDO6, PMIC_ID_INVALID,
                            HW_ID_INVALID, HW_ID_ACTIVATE_LOW, 10, 1, 0, 4, 1),
    REGU_ID_SINGLE_RAIL_DEF(SOC_DOVDD18_IR, 2, SLG51000_ID_LDO7,
                            PMIC_ID_INVALID, HW_ID_INVALID, HW_ID_ACTIVATE_LOW,
                            11, 1, 0, 5, 1),
};
#endif

static int set_each_hw_volatage(aon_rpc_msg_t *msg, int hw_num,
                                regu_voltage_param_t *regu_vol)
{
        int      index       = 0;
        uint16_t is_dualrail = 0;
        RPC_GET_BE16(msg->data, 2U, &is_dualrail);
        /*fix me: check the is_dualrail and hw_num*/
        for (int i = 0; i < hw_num; i++) {
                RPC_GET_BE32(msg->data, (4U + index * 4),
                             &regu_vol->target_uv[i]);
                if (is_dualrail & (0x1 << index)) {
                        regu_vol->target_uv[i + 1] = regu_vol->target_uv[i];
                        i++;
                }
                index++;
        }
        return 0;
}

static void pmic_dispatch(aon_rpc_msg_t *msg, aon_rpc_msg_t *tx_msg)
{
        uint8_t     func   = RPC_GET_FUNC(msg);
        aon_err_en  result = AON_ERR_NONE;
        csi_error_t ret;
        uint8_t     ack_size = 0;
        switch (func) {
                        /* Handle uknown function */
                case PM_FUNC_UNKNOWN: {
                        ack_size = 1;
                        result   = AON_ERR_NOTFOUND;
                } break;

                /* Dispatch set_resource_power_mode() */
                case PM_FUNC_SET_RESOURCE_REGULATOR: {
                        int hw_num = 0;
                        /* Declare return and parameters */
                        regu_voltage_param_t regu_vol;
                        RPC_GET_BE16(msg->data, 0U, &regu_vol.regu_ext_id);
                        hw_num = pmic_get_regu_hw_id_num(aon_pmic_get_handler(),
                                                         regu_vol.regu_ext_id);
                        if (hw_num < 0) {
                                LOG_E("Get hw num failed\n");
                                result = AON_ERR_FAIL;
                        } else {
                                ret = set_each_hw_volatage(msg, hw_num,
                                                           &regu_vol);
                                if (ret) {
                                        LOG_E("Set each hw volatage failed\n");
                                        result = AON_ERR_FAIL;
                                } else {
                                        /* Call function */
                                        ret = csi_pmic_regulator_set_voltage(
                                            &g_pmic, regu_vol);
                                        if (ret) {
                                                LOG_E("ERR->PM_FUNC_SET_"
                                                      "RESOURCE_REGULATOR\n");
                                                result = AON_ERR_FAIL;
                                        } else {
                                                result = AON_ERR_NONE;
                                        }
                                }
                        }
                        /*
		 *  resource: uint8_t type
		 *	- MPW: 0: AP cpu resource
		 *	- Fullmask: 0: AP cpu,  1: DDR ...
		 *  vol_dvdd/vol_dvddm: uint32_t type
		 *	- unit: uV
		 *
		 * return: 0:ok, other is error
		 */
                        /* Copy in return parameters */
                        ack_size = 1U;
                        LOG_D("regu_ext_id=%d, vol1=%d, vol2=%d, vol3=%d\n",
                              regu_vol.regu_ext_id, regu_vol.target_uv[0],
                              regu_vol.target_uv[1], regu_vol.target_uv[2]);
                } break;

                /* Dispatch get_resource_power_mode() */
                case PM_FUNC_GET_RESOURCE_REGULATOR: {
                        /* Declare return and parameters */
                        regu_voltage_param_t regu_vol;
                        RPC_GET_BE16(msg->data, 0U, &regu_vol.regu_ext_id);
                        /* Call function */
                        ret =
                            csi_pmic_regulator_get_voltage(&g_pmic, &regu_vol);
                        if (ret) {
                                LOG_E("ERR->PM_FUNC_GET-RESOURCE_REGULATOR :%d "
                                      "\n",
                                      regu_vol.regu_ext_id);
                                result = AON_ERR_FAIL;
                        } else {
                                result = AON_ERR_NONE;
                        }
                        /* Copy in return parameters */
                        ack_size = 1U + 4 + 8;
                        /*
                        regu id and dual rail param set by ap
                        RPC_SET_BE32(tx_msg->data, 1, 0, );
                        */
                        RPC_SET_BE32(tx_msg->data, 5,
                                     U32(regu_vol.target_uv[0]));
                        RPC_SET_BE32(tx_msg->data, 9,
                                     U32(regu_vol.target_uv[1]));

                        /* only for debug */
                        LOG_D("regu_ext_id=%d, vol1=%d, vol2=%d, vol3=%d\n",
                              regu_vol.regu_ext_id, regu_vol.target_uv[0],
                              regu_vol.target_uv[1], regu_vol.target_uv[2]);
                } break;

                case PM_FUNC_SET_RESOURCE_POWER_MODE: {
                        uint16_t resource;
                        uint16_t mode;

                        RPC_GET_BE16(msg->data, 0U, &resource);
                        RPC_GET_BE16(msg->data, 2U, &mode);
                        ret = chip_power_pd_ctrl(resource, mode);
                        if (ret < 0) {
                                result = AON_ERR_FAIL;
                        }
                        ack_size = 1U;
                } break;

                case PM_FUNC_PWR_SET: {
                        uint16_t regu_ext_id = 0;
                        uint32_t benable     = 0;
                        /* Declare return and parameters */
                        RPC_GET_BE16(msg->data, 0U, &regu_ext_id);
                        RPC_GET_BE16(msg->data, 2U, &benable);

                        /* Call function */
                        ret = csi_pmic_regulator_ctrl(&g_pmic, regu_ext_id,
                                                      benable);
                        if (ret) {
                                LOG_E("ERR->PM_FUNC_PWR_SET\n");
                                result = AON_ERR_FAIL;
                        } else {
                                result = AON_ERR_NONE;
                        }
                        ack_size = 1U + 4;

                        LOG_D("regu_ext_id = %d status %d \n", regu_ext_id,
                              benable);
                } break;

                case PM_FUNC_PWR_GET: {
                        /* Declare return and parameters */
                        int      regu_ext_id;
                        uint32_t benable;

                        RPC_GET_BE16(msg->data, 0U, &regu_ext_id);
                        ret = csi_pmic_regulator_is_enable(&g_pmic, regu_ext_id,
                                                           &benable);
                        if (ret) {
                                LOG_D("ERR->PM_FUNC_PWR_GET \n");
                                result = AON_ERR_FAIL;
                        } else {
                                result = AON_ERR_NONE;
                        }

                        /* Copy in return parameters */
                        ack_size = 1U + 4;
                        RPC_SET_BE16(tx_msg->data, 1, regu_ext_id);
                        RPC_SET_BE16(tx_msg->data, 3, benable);

                        /* only for debug */
                        LOG_D("regu_ext_id = %d status %d \n", regu_ext_id,
                              benable);
                } break;

                case PM_FUNC_CHECK_FAULT: {
                        /* Declare return parameters */
                        uint32_t error_code = CSI_STATUS_PMIC_OK;
                        if (g_pmic.pmic_status) {
                                error_code         = g_pmic.pmic_status;
                                g_pmic.pmic_status = CSI_STATUS_PMIC_OK;
                        }

                        /* Copy in return parameters */
                        ack_size = 1 + 4U;
                        RPC_SET_BE32(tx_msg->data, 1, U32(error_code));
                        LOG_D("error code %08x\n", error_code);
                } break;

                case PM_FUNC_GET_TEMPERATURE: /*get temperature*/
                {

                        uint32_t val     = 0;
                        uint8_t  pmic_id = 0;

                        RPC_GET_U8(msg->data, 0U, &pmic_id);
                        /* check pmic global temperature */
                        ret = csi_pmic_read_temperature(&g_pmic, &val);
                        if (ret) {
                                LOG_D("ERR->PM_FUNC_GET_TEMP \n");
                                result = AON_ERR_FAIL;
                        } else {
                                result = AON_ERR_NONE;
                        }
#warning "fixme, the pmic id is not we want"
                        /* Copy in return parameters */
                        ack_size = 1 + 8U;
                        RPC_SET_BE32(tx_msg->data, 1, U32(pmic_id));
                        RPC_SET_BE32(tx_msg->data, 5, U32(val));
                        LOG_D("chip id %08x, temp value %08x\n", pmic_id, val);
                } break;
                /* Handle default */
                default: {
                        ack_size = 1;
                        result   = AON_ERR_NOTFOUND;
                } break;
        }

        /* Fill in header */
        RPC_SET_HEAD(tx_msg, AON_RPC_VERSION, ack_size, RPC_SVC_MSG_TYPE_ACK,
                     RPC_SVC_MSG_NO_NEED_ACK, RPC_GET_SVC_ID(msg), func,
                     result);
#if AON_TRACE_ON
        /* Return parameters */
        LOG_D("ipc_debug:rx(%d)", RPC_GET_FUNC(msg));
        dumpBuf((uint8_t *)msg, 28);
        LOG_D("ipc_debug:tx(%d)", RPC_GET_FUNC(tx_msg));

        +dumpBuf((uint8_t *)tx_msg, 28);
#endif

        /* Handle error debug */
        if (result != AON_ERR_NONE)
                LOG_E("ipc_err: api=pm, func=%d, err=%d\n", func, result);
}

int aon_pmic_init(pmic_config_t *aon_conf)
{
        if (!aon_conf) {
                return -1;
        }
        int ret = 0;

        /* pmic init */
        ret = csi_pmic_init(&g_pmic, aon_conf);
        if (ret) {
                LOG_E("ERR->csi_pmic_init failed \n");
                return -1;
        }

        return aon_rpc_svc_cb_register(AON_RPC_SVC_PM, pmic_dispatch);
}

int aon_pmic_poweroff()
{
        return csi_pmic_soft_power_off(&g_pmic);
}

csi_pmic_t *aon_pmic_get_handler()
{
        return &g_pmic;
}
