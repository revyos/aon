/*
 *	Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/*******************************************************
 * @file       wj_pmic.c
 * @brief      source file for pmic csi driver
 * @version    V1.0
 * @date       23. Feb 2021
 * ******************************************************/
#include <drv/common.h>
#include "pmic_rsc_def.h"
#include <drv/pmic.h>
#include <drv/irq.h>
#include <pmic_help.h>
#include "syslog.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#ifndef CONFIG_MAX_PMIC_NUM
#define CONFIG_MAX_PMIC_NUM 10
#endif

#define CONFIG_MAX_REGU_EXT_NUM 30

#define I2C_MAX_SEND_DATA_SZIE 30 //it should large than 4

typedef enum {
        PMIC_INTERRUPT_IO_ERR    = 0U,
        PMIC_INTERRUPT_IO_LPM    = 1U,
        PMIC_INTERRUPT_IO_UNKNOW = 0xff,
} pmic_interrupt_io_type_en;

csi_pmic_dev_t pmic_devs[CONFIG_MAX_PMIC_NUM];

static csi_gpio_t pmic_csi_gpio;

#ifdef CONFIG_PMIC_DA9063_V1
extern struct csi_pmic_drv_data da9063_drv;
#endif
#ifdef CONFIG_PMIC_DA9121_V1
extern struct csi_pmic_drv_data da9121_drv;
#endif

#ifdef CONFIG_PMIC_SLG51000_V1
extern struct csi_pmic_drv_data slg51000_drv;
#endif

#ifdef CONFIG_PMIC_RICOH567_V0
extern struct csi_pmic_drv_data ricoh567_drv_v0;
#endif

#ifdef CONFIG_PMIC_RICOH567_V1
extern struct csi_pmic_drv_data ricoh567_drv_v1;
#endif

#ifdef CONFIG_PMIC_APTF110_V1
extern struct csi_pmic_drv_data aptf110_drv;
#endif

const pmic_drv_info_t support_drv[] = {
#ifdef CONFIG_PMIC_DA9063_V1
    {"dialog,da9063", "v1", &da9063_drv},
#endif
#ifdef CONFIG_PMIC_DA9121_V1
    {"dialog,da9121", "v1", &da9121_drv},
#endif
#ifdef CONFIG_PMIC_SLG51000_V1
    {"dialog,slg51000", "v1", &slg51000_drv},
#endif
#ifdef CONFIG_PMIC_RICOH567_V0
    {"ricoh,rn5t567", "v0", &ricoh567_drv_v0},
#endif
#ifdef CONFIG_PMIC_RICOH567_V1
    {"ricoh,rn5t567", "v1", &ricoh567_drv_v1},
#endif
#ifdef CONFIG_PMIC_APTF110_V1
    {"apt,aptf110", "v1", &aptf110_drv},
#endif
};

csi_error_t pmic_get_id_info(csi_pmic_t *pmic, uint32_t regu_ext_id,
                             csi_regu_id_t **id_info)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);

        for (int i = 0; i < pmic->regu_num; i++) {
                if (pmic->regu_list[i].regu_ext_id == regu_ext_id) {
                        *id_info = &pmic->regu_list[i];
                        return CSI_OK;
                }
        }
        LOG_E("get pmic id %d info faild\n", regu_ext_id);
        return CSI_ERROR;
}

csi_error_t pmic_get_regu_info(csi_pmic_dev_t *pmic_dev, uint32_t regu_id,
                               csi_regu_t **regu)
{
        CSI_PARAM_CHK(pmic_dev, CSI_ERROR);
        CSI_PARAM_CHK(regu, CSI_ERROR);

        uint32_t regu_num = pmic_dev->drv_data->regu_num;

        for (int i = 0; i < regu_num; i++) {
                if (regu_id == pmic_dev->drv_data->regu_info[i].id) {
                        *regu = &pmic_dev->drv_data->regu_info[i];
                        return CSI_OK;
                }
        }
        return CSI_UNSUPPORTED;
}

int pmic_get_regu_hw_id_num(csi_pmic_t *pmic, uint32_t regu_ext_id)
{
        int            ret;
        csi_regu_id_t *regu_id = NULL;
        int            num     = 0;

        ret = pmic_get_id_info(pmic, regu_ext_id, &regu_id);
        if (ret) {
                return -1;
        }
        for (int i = 0; i < ARRAY_SIZE(regu_id->sub.id); i++) {
                if (regu_id->sub.id[i].pmic_id != PMIC_ID_INVALID) {
                        num++;
                }
        }
        return num;
}

csi_pmic_dev_t *csi_pmic_get_dev_by_pmic_id(csi_pmic_t *pmic, uint8_t pmic_id)
{
        if (pmic_id > pmic->dev_num - 1) {
                LOG_E("err pmic_id %d\n", pmic_id);
                return NULL;
        }
        return &pmic->dev_list[pmic_id];
}

static csi_error_t iic_write_reg(csi_pmic_t *pmic, uint16_t dev_addr,
                                 uint32_t offset, uint32_t val)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        int32_t  num;
        uint8_t  temp[6] = {0};
        uint32_t len     = 0;

        if ((offset >> 8) & 0xff) {
                /*16 bit addr*/
                temp[len++] = (offset >> 8) & 0xff;
                temp[len++] = offset & 0xff;
                temp[len++] = (uint8_t)val;
        } else {
                /*8bit addr*/
                temp[len++] = offset & 0xff;
                temp[len++] = (uint8_t)val;
        }

        num = csi_iic_master_send((csi_iic_t *)(&pmic->if_dev), dev_addr, temp,
                                  len, 10);
        if (num != len) {
                return CSI_ERROR;
        }

        return CSI_OK;
}

static csi_error_t iic_write_reg_multi_bytes(csi_pmic_t *pmic,
                                             uint16_t dev_addr, uint32_t offset,
                                             uint8_t *data, uint8_t size)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        int32_t  num;
        uint8_t  temp[I2C_MAX_SEND_DATA_SZIE] = {0};
        uint32_t len                          = 0;

        if (size > I2C_MAX_SEND_DATA_SZIE - 4) {
                return CSI_ERROR;
        }

        if ((offset >> 8) & 0xff) {
                /*16 bit addr*/
                temp[len++] = (offset >> 8) & 0xff;
                temp[len++] = offset & 0xff;
        } else {
                /*8bit addr*/
                temp[len++] = offset & 0xff;
        }

        if (data && size) {
                memcpy(temp + len, data, size);
                len += size;
        }
        //LOG_D("iic send:len:%d %02x%02x%02x%02x%02x%02x\r\n",len, temp[0],temp[1],temp[2],temp[3],temp[4],temp[5]);
        num = csi_iic_master_send((csi_iic_t *)(&pmic->if_dev), dev_addr, temp,
                                  len, 100);
        if (num != len) {
                return CSI_ERROR;
        }

        return CSI_OK;
}

static csi_error_t iic_read_reg_sr(csi_pmic_t *pmic, uint16_t dev_addr,
                                   uint32_t offset, uint32_t *val)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        int32_t num;
        uint8_t temp[2] = {0};
        num             = csi_iic_mem_receive(
            (csi_iic_t *)(&pmic->if_dev), dev_addr, offset,
            (offset >> 8) ? IIC_MEM_ADDR_SIZE_16BIT : IIC_MEM_ADDR_SIZE_8BIT,
            &temp, 1, 10);
        if (num != 1) {
                return CSI_ERROR;
        }
        *val = temp[0];
        return CSI_OK;
}

static csi_error_t iic_read_reg(csi_pmic_t *pmic, uint16_t dev_addr,
                                uint32_t offset, uint32_t *val)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        int32_t  num;
        uint8_t  temp[2] = {0};
        uint32_t len     = 0;

        if ((offset >> 8) & 0xff) {
                temp[len++] = (offset >> 8) & 0xff;
                temp[len++] = offset & 0xff;
        } else {
                temp[len++] = offset & 0xff;
        }

        num = csi_iic_master_send((csi_iic_t *)(&pmic->if_dev), dev_addr, &temp,
                                  len, 10);
        if (num != len) {
                return CSI_ERROR;
        }
        num = csi_iic_master_receive((csi_iic_t *)(&pmic->if_dev), dev_addr,
                                     &temp[0], 1, 10);
        if (num != 1) {
                return CSI_ERROR;
        }

        *val = temp[0];
        return CSI_OK;
}

static csi_error_t iic_read_reg_multi_bytes(csi_pmic_t *pmic, uint16_t dev_addr,
                                            uint32_t offset, uint8_t *val,
                                            uint8_t *get_len)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        int32_t  num;
        uint8_t  temp[2] = {0};
        uint32_t len     = 0;

        if ((offset >> 8) & 0xff) {
                temp[len++] = (offset >> 8) & 0xff;
                temp[len++] = offset & 0xff;
        } else {
                temp[len++] = offset & 0xff;
        }

        num = csi_iic_master_send((csi_iic_t *)(&pmic->if_dev), dev_addr, &temp,
                                  len, 100);
        if (num != len) {
                return CSI_ERROR;
        }
        num = csi_iic_master_receive((csi_iic_t *)(&pmic->if_dev), dev_addr,
                                     val, *get_len, 100);
        if (num != *get_len) {
                return CSI_ERROR;
        }

        return CSI_OK;
}

csi_error_t csi_pmic_if_config(csi_pmic_t           *pmic,
                               csi_pmic_if_config_t *if_config)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        CSI_PARAM_CHK(if_config, CSI_ERROR);
        csi_error_t ret;

        ret = csi_iic_mode((csi_iic_t *)(&pmic->if_dev), IIC_MODE_MASTER);
        if (ret) {
                return ret;
        }

        ret = csi_iic_addr_mode((csi_iic_t *)(&pmic->if_dev),
                                if_config->addr_mode);
        if (ret) {
                return ret;
        }

        ret = csi_iic_speed((csi_iic_t *)(&pmic->if_dev), if_config->speed);
        if (ret) {
                return ret;
        }

        return CSI_OK;
}

#define PMIC_GET_COUPLING_INFO_SUCCESS 0
#define PMIC_GET_COUPLING_INFO_FAILD -1

bool is_id_coupling(int id, coupling_desc_t *coupling_info)
{
        if ((coupling_info->id0 == id || coupling_info->id1 == id) &&
            (coupling_info->id0 != coupling_info->id1) &&
            (!(coupling_info->max_spread == 0 &&
               coupling_info->min_spread == 0))) {
                return true;
        }

        return false;
}

int get_min_and_max_vol_by_coupled_info(int id, uint32_t *curent_uv,
                                        int              curent_uv_num,
                                        coupling_desc_t *coupling_list,
                                        int coupling_num, uint32_t *min_uv,
                                        uint32_t *max_uv)
{
        int32_t min_uv_temp = 0;
        int32_t max_uv_temp = 0;
        int     coupling_id = 0;
        *min_uv             = 0;
        *max_uv             = 0xffffffff;
        int not_get_flag    = 1;
        for (int j = 0; j < coupling_num; j++) {
                if (is_id_coupling(id, &coupling_list[j]) == true) {
                        if (coupling_list[j].id0 == id) {
                                coupling_id = coupling_list[j].id1;
                                min_uv_temp =
                                    coupling_list[j].min_spread * 10000 +
                                    curent_uv[coupling_id];
                                max_uv_temp =
                                    coupling_list[j].max_spread * 10000 +
                                    curent_uv[coupling_id];
                        } else {
                                coupling_id = coupling_list[j].id0;
                                min_uv_temp =
                                    curent_uv[coupling_id] -
                                    coupling_list[j].max_spread * 10000;
                                max_uv_temp =
                                    curent_uv[coupling_id] -
                                    coupling_list[j].min_spread * 10000;
                        }
                        if (min_uv_temp > *min_uv) {
                                *min_uv = min_uv_temp;
                        }
                        if (max_uv_temp < *max_uv) {
                                *max_uv = max_uv_temp;
                        }
                        not_get_flag = 0;
                }
        }

        if (*min_uv > *max_uv || not_get_flag) {
                return PMIC_GET_COUPLING_INFO_FAILD;
        }

        return PMIC_GET_COUPLING_INFO_SUCCESS;
}

static void get_hw_need_set_vol_by_coupled_info(csi_regu_id_t *regu_id,
                                                uint8_t       *flag_list)
{
        coupling_desc_t *coupling_list     = regu_id->sub.coupling_list;
        int              coupling_list_num = PMIC_MAX_COUPLING_LIST_NUM;
        for (int i = 0; i < ARRAY_SIZE(regu_id->sub.id); i++) {
                if (regu_id->sub.id[i].pmic_id != PMIC_ID_INVALID) {
                        for (int j = 0; j < coupling_list_num; j++) {
                                if (is_id_coupling(i, &coupling_list[j]) ==
                                    true) {
                                        flag_list[i] = true;
                                        // LOG_D("%d:%d need set couple\n",
                                        //        regu_id->regu_ext_id, i);
                                }
                        }
                }
        }
}

csi_error_t csi_pmic_get_voltage_balanced(csi_regu_id_t *regu_id, int8_t id,
                                          int32_t target_uv, int32_t step_uv,
                                          uint32_t *current_uv_list,
                                          int current_uv_num, uint32_t *temp_uv)
{

        csi_error_t     ret         = CSI_OK;
        int32_t         ccurrent_uv = 0;
        int32_t         best_margin = 0;
        uint32_t        min_uv = 0, max_uv = 0;
        int             coupling_list_num = PMIC_MAX_COUPLING_LIST_NUM;

        ret = get_min_and_max_vol_by_coupled_info(
            id, current_uv_list, current_uv_num, regu_id->sub.coupling_list,
            coupling_list_num, &min_uv, &max_uv);
        if (ret == PMIC_GET_COUPLING_INFO_FAILD) {
                LOG_E("calculate coupling info  faild");
                return CSI_ERROR;
        }
        int32_t max_step_uv = target_uv - current_uv_list[id];
        /* calculate the best margin */
        best_margin = target_uv > current_uv_list[id]
                          ? min((max_uv - current_uv_list[id]), max_step_uv)
                          : max((min_uv - current_uv_list[id]), max_step_uv);
        if (step_uv) {
                *temp_uv =
                    current_uv_list[id] + best_margin / step_uv * step_uv;
        } else {
                *temp_uv = current_uv_list[id] + best_margin;
        }

        LOG_D("%d <= id:%d <= %d, curentuv:%d, targetuv:%d, best margin: %d, "
               "nextuv: %d\n",
               min_uv, id, max_uv, current_uv_list[id], target_uv, best_margin,
               *temp_uv);
        return CSI_OK;
}

csi_error_t csi_pmic_regulator_set_voltage(csi_pmic_t          *pmic,
                                           regu_voltage_param_t regu_param)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);

        csi_error_t     ret                                   = CSI_ERROR;
        csi_regu_id_t  *regu_id                               = NULL;
        csi_pmic_dev_t *pmic_dev[PMIC_MAX_HW_ID_NUM]          = {0};
        csi_regu_t     *regu[PMIC_MAX_HW_ID_NUM]              = {0};
        uint8_t         coupling_flag[PMIC_MAX_HW_ID_NUM]     = {false};
        uint32_t        current_target_uv[PMIC_MAX_HW_ID_NUM] = {0x0};
        int             current_uv_num                        = 0;
        uint32_t        current_uv[PMIC_MAX_HW_ID_NUM]        = {0};
        uint8_t         need_set                              = 0;
        uint8_t         need_set_flag                         = 0;
        uint8_t         vote                                  = 0;
        uint32_t        wait_done                             = 0;
        uint32_t        step                                  = 0;

        ret = pmic_get_id_info(pmic, regu_param.regu_ext_id, &regu_id);
        if (ret) {
                return CSI_ERROR;
        }

        get_hw_need_set_vol_by_coupled_info(regu_id, coupling_flag);

        for (int i = 0; i < ARRAY_SIZE(regu_id->sub.id); i++) {
                if (regu_id->sub.id[i].pmic_id != PMIC_ID_INVALID) {
                        current_uv_num++;
                }
        }

        for (int i = 0; i < ARRAY_SIZE(regu_id->sub.id); i++) {
                if (regu_id->sub.id[i].pmic_id != PMIC_ID_INVALID) {
                        pmic_dev[i] = csi_pmic_get_dev_by_pmic_id(
                            pmic, regu_id->sub.id[i].pmic_id);
                        if (!pmic_dev[i] ||
                            !pmic_dev[i]->drv_data->ops->get_voltage ||
                            !pmic_dev[i]->drv_data->ops->set_voltage) {
                                return CSI_ERROR;
                        }

                        ret = pmic_get_regu_info(
                            pmic_dev[i], regu_id->sub.id[i].hw_id, &regu[i]);
                        if (ret) {
                                return ret;
                        }

                        if (regu_param.target_uv[i] > regu[i]->max_uv ||
                            regu_param.target_uv[i] < regu[i]->min_uv) {
                                return CSI_ERROR;
                        }

                        /* get the current voltage */
                        ret = pmic_dev[i]->drv_data->ops->get_voltage(
                            pmic, pmic_dev[i], regu_id->sub.id[i].hw_id,
                            &current_uv[i]);
                        if (ret) {
                                return ret;
                        }
                        if (current_uv[i] != regu_param.target_uv[i]) {
                                need_set_flag |= 1 << i;
                                need_set++;
                        }
                }
        }
        LOG_D("cur_uv0:%d  cur_uv1:%d cur_uv2:%d tar_uv0:%d tar_uv1:%d "
               "tar_uv2:%d, %d need set\n",
               current_uv[0], current_uv[1], current_uv[2],
               regu_param.target_uv[0], regu_param.target_uv[1],
               regu_param.target_uv[2], need_set);

        while (need_set) {

                for (int i = 0; i < current_uv_num; i++) {
                        if (need_set_flag & (1 << i)) {

                                if (regu_param.target_uv[i] == current_uv[i]) {
                                        need_set--;
                                        need_set_flag &= ~(1 << i);
                                        LOG_D("set success: regu: %d, "
                                               "current_uv:%d %d need_set\n",
                                               i, current_uv[i], need_set);
                                } else {
                                        current_target_uv[i] =
                                            regu_param.target_uv[i];

                                        /* check if regu has coupled regus, calculate the target voltage */
                                        if (coupling_flag[i] == true) {
                                                ret =
                                                    csi_pmic_get_voltage_balanced(
                                                        regu_id, i,
                                                        regu_param.target_uv[i],
                                                        regu[i]->step_uv,
                                                        current_uv,
                                                        current_uv_num,
                                                        &current_target_uv[i]);
                                                if (ret) {
                                                        return ret;
                                                }
                                        }
                                        /* set the target voltage */
                                        ret = pmic_dev[i]
                                                  ->drv_data->ops->set_voltage(
                                                      pmic, pmic_dev[i],
                                                      regu_id->sub.id[i].hw_id,
                                                      current_target_uv[i]);
                                        if (ret) {
                                                return ret;
                                        }
                                        step =
                                            current_target_uv[i] > current_uv[i]
                                                ? (current_target_uv[i] -
                                                   current_uv[i])
                                                : (current_uv[i] -
                                                   current_target_uv[i]);
                                        wait_done =
                                            step /
                                            (pmic_dev[i]->dev_info->slew_rate *
                                             1000);
                                        udelay(wait_done);
                                        LOG_D("%d wait:%d\n", i, wait_done);
                                        /* update the current voltage */
                                        ret = pmic_dev[i]
                                                  ->drv_data->ops->get_voltage(
                                                      pmic, pmic_dev[i],
                                                      regu_id->sub.id[i].hw_id,
                                                      &current_uv[i]);
                                        if (ret) {
                                                return ret;
                                        }
                                }
                        }
                }
        }
        return CSI_OK;
}

csi_error_t csi_pmic_regulator_get_voltage(csi_pmic_t           *pmic,
                                           regu_voltage_param_t *regu_param)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        csi_error_t     ret;
        csi_pmic_dev_t *pmic_dev = NULL;
        csi_regu_id_t  *regu_id  = NULL;
        /* check whether exist regu resource in  pmic-dev list which registered by user*/
        ret = pmic_get_id_info(pmic, regu_param->regu_ext_id, &regu_id);
        if (ret) {
                return CSI_ERROR;
        }

        for (int i = 0; i < ARRAY_SIZE(regu_id->sub.id); i++) {
                if (regu_id->sub.id[i].pmic_id != PMIC_ID_INVALID) {
                        pmic_dev = csi_pmic_get_dev_by_pmic_id(
                            pmic, regu_id->sub.id[i].pmic_id);
                        if (pmic_dev && pmic_dev->drv_data->ops->get_voltage) {
                                ret = pmic_dev->drv_data->ops->get_voltage(
                                    pmic, pmic_dev, regu_id->sub.id[i].hw_id,
                                    &regu_param->target_uv[i]);
                                    if (ret) {
                                        /*fix me*/
                                        return ret;
                                    }
                        } else {
                                return CSI_ERROR;
                        }
                }
        }
        LOG_D("get volatage :%d %d  %d\n", regu_param->regu_ext_id,regu_param->target_uv[0], regu_param->target_uv[1]);
        return ret;
}

csi_error_t csi_pmic_regulator_ctrl(csi_pmic_t *pmic, uint8_t regu_ext_id,
                                    bool enable)
{
        csi_error_t     ret;
        csi_regu_id_t  *regu_id        = NULL;
        csi_pmic_dev_t *pmic_dev       = NULL;
        csi_pmic_dev_t *pmic_dev_hw_pa = NULL;
        bool            io_set_val;

        /* check whether exist regu resource in  pmic-dev list which registered by user*/
        ret = pmic_get_id_info(pmic, regu_ext_id, &regu_id);
        if (ret) {
                return CSI_ERROR;
        }

        for (int i = 0; i < ARRAY_SIZE(regu_id->sub.id); i++) {
#if defined(CONFIG_BOARD_LIGHT_AON_ANT_DISCRETE)
                if (enable && i == 1) {
                        udelay(20000);
                }
#endif
                if (regu_id->sub.id[i].pmic_id != PMIC_ID_INVALID) {
                        pmic_dev = csi_pmic_get_dev_by_pmic_id(
                            pmic, regu_id->sub.id[i].pmic_id);
                        if (pmic_dev &&
                            pmic_dev->drv_data->ops->regu_power_ctrl) {
                                ret = pmic_dev->drv_data->ops->regu_power_ctrl(
                                    pmic, pmic_dev, regu_id->sub.id[i].hw_id,
                                    enable);
                                if (ret) {
                                        return ret;
                                }
                        } else {
                                return CSI_ERROR;
                        }
                        if (regu_id->sub.id[i].parent_hw_info.pmic_id !=
                            PMIC_ID_INVALID) {
                                pmic_dev_hw_pa = csi_pmic_get_dev_by_pmic_id(
                                    pmic,
                                    regu_id->sub.id[i].parent_hw_info.pmic_id);
                                if (pmic_dev_hw_pa && pmic_dev_hw_pa->drv_data
                                                          ->ops->gpio_output) {
                                        io_set_val = (regu_id->sub.id[i]
                                                          .parent_hw_info
                                                          .activate_status ==
                                                      HW_ID_ACTIVATE_HIGH)
                                                         ? enable
                                                         : !enable;
                                        ret = pmic_dev_hw_pa->drv_data->ops
                                                  ->gpio_output(
                                                      pmic, pmic_dev_hw_pa,
                                                      regu_id->sub.id[i]
                                                          .parent_hw_info
                                                          .io_hw_id,
                                                      io_set_val);
                                        if (ret) {
                                                LOG_E("Gpio ctrl faild:%d\n",   regu_id->sub.id[i].parent_hw_info.io_hw_id);
                                                return ret;
                                        }
                                } else {
                                        return ret;
                                }
                        }
                        regu_id->sub.id[i].benable = enable;
                }
        }

        return CSI_OK;
}

csi_error_t csi_pmic_read_faults(csi_pmic_t *pmic, uint8_t pmic_id)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        csi_error_t     ret;
        csi_pmic_dev_t *pmic_dev = NULL;

        pmic_dev = csi_pmic_get_dev_by_pmic_id(pmic, pmic_id);
        if (!pmic_dev) {
                return CSI_ERROR;
        }

        if (!pmic_dev->drv_data->ops->read_faults) {
                LOG_E("pmic_id %d not support faults get\n", pmic_id);
                return CSI_UNSUPPORTED;
        }

        ret = pmic_dev->drv_data->ops->read_faults(pmic, pmic_dev);
        if (ret) {
                LOG_E("pmic_id %d faults get faild\n", pmic_id);
                return CSI_ERROR;
        }

        return CSI_OK;
}

csi_error_t csi_pmic_read_temperature(csi_pmic_t *pmic, uint32_t *temp)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        csi_pmic_dev_t *pmic_dev = NULL;
        csi_error_t     ret      = CSI_OK;
        uint32_t        max_temp = 0;

        for (int i = 0; i < pmic->dev_num; i++) {
                csi_pmic_dev_t *pmic_dev = &pmic->dev_list[i];
                if (pmic_dev->drv_data->ops->get_temperature) {
                        ret = pmic_dev->drv_data->ops->get_temperature(
                            pmic, pmic_dev, temp);
                        if (ret) {
                                LOG_E("pmic dev:%d get temperature faild %d", i,
                                      ret);
                                return ret;
                        }
                        if (*temp > max_temp)
                                max_temp = *temp;
                }
        }
        *temp = max_temp;
        return ret;
}

csi_error_t csi_pmic_regulator_is_enable(csi_pmic_t *pmic, uint8_t regu_ext_id,
                                         uint32_t *benable)
{
        csi_regu_id_t *regu_id;
        csi_error_t    ret;
        int            no_regu_flag = 1;
        *benable                    = 1;

        ret = pmic_get_id_info(pmic, regu_ext_id, &regu_id);
        if (ret) {
                return CSI_ERROR;
        }

        for (int i = 0; i < ARRAY_SIZE(regu_id->sub.id); i++) {
                if (regu_id->sub.id[i].pmic_id != PMIC_ID_INVALID) {
                        no_regu_flag = 0;
                        *benable     = *benable && regu_id->sub.id[i].benable;
                }
        }

        if (no_regu_flag) {
                *benable = 0;
        }
        LOG_D("regu:%d %d \n", regu_ext_id, *benable);

        return CSI_OK;
}

csi_error_t csi_pmic_wdt_start(csi_pmic_t *pmic)
{
        csi_error_t ret;

        if (!pmic->wdt_ops) {
                return CSI_ERROR;
        }
        csi_pmic_dev_t *dev =
            csi_pmic_get_dev_by_pmic_id(pmic, pmic->hw_wdt_pmic_id);
        if (!dev) {
                return CSI_ERROR;
        }

        ret = pmic->wdt_ops->start(pmic, dev);
        if (ret) {
                return CSI_ERROR;
        }
        dev->dev_info->flag |= PMIC_DEV_ENABLE_WDT;
        return ret;
}

csi_error_t csi_pmic_wdt_stop(csi_pmic_t *pmic)
{
        csi_error_t ret;
        if (!pmic->wdt_ops) {
                return CSI_UNSUPPORTED;
        }
        csi_pmic_dev_t *dev =
            csi_pmic_get_dev_by_pmic_id(pmic, pmic->hw_wdt_pmic_id);
        if (!dev) {
                return CSI_ERROR;
        }
        ret = pmic->wdt_ops->stop(pmic, dev);
        if (ret) {
                return CSI_ERROR;
        }

        dev->dev_info->flag &= ~PMIC_DEV_ENABLE_WDT;

        return ret;
}

csi_error_t csi_pmic_wdt_set_timer(csi_pmic_t *pmic, uint32_t tmr_len)
{
        csi_error_t ret = CSI_OK;
        if (!pmic->wdt_ops) {
                return CSI_ERROR;
        }

        csi_pmic_dev_t *dev =
            csi_pmic_get_dev_by_pmic_id(pmic, pmic->hw_wdt_pmic_id);
        if (!dev) {
                return CSI_ERROR;
        }

        dev->dev_info->wdt_len = tmr_len;

        if (dev->dev_info->flag & PMIC_DEV_ENABLE_WDT) {
                ret = pmic->wdt_ops->set_timer_len(pmic, dev, tmr_len);
        }

        return ret;
}

csi_error_t csi_pmic_wdt_is_enable(csi_pmic_t *pmic, uint32_t *benable)
{
        if (!pmic->wdt_ops) {
                return CSI_UNSUPPORTED;
        }
        csi_pmic_dev_t *dev =
            csi_pmic_get_dev_by_pmic_id(pmic, pmic->hw_wdt_pmic_id);
        if (!dev) {
                return CSI_ERROR;
        }

        *benable = (dev->dev_info->flag & PMIC_DEV_ENABLE_WDT) ? 1 : 0;
        return CSI_OK;
}

csi_error_t csi_pmic_wdt_feed(csi_pmic_t *pmic)
{

        if (!pmic->wdt_ops) {
                return CSI_ERROR;
        }
        csi_pmic_dev_t *dev =
            csi_pmic_get_dev_by_pmic_id(pmic, pmic->hw_wdt_pmic_id);
        if (!dev) {
                return CSI_ERROR;
        }

        return pmic->wdt_ops->feed(pmic, dev);
}

csi_error_t csi_pmic_get_autoboot_flag(csi_pmic_t *pmic, uint32_t *bsupport)
{
        if (!pmic->wdt_ops) {
                LOG_I("not support wdt-ops \n");
                *bsupport = 0;
                return CSI_OK;
        }
        csi_pmic_dev_t *dev =
            csi_pmic_get_dev_by_pmic_id(pmic, pmic->hw_wdt_pmic_id);
        if (!dev) {
                return CSI_ERROR;
        }

        *bsupport = dev->dev_info->flag & PMIC_DEV_ENABLE_AUTO_REBOOT;

        return CSI_OK;
}

csi_error_t csi_pmic_repower(csi_pmic_t *pmic)
{

        csi_error_t ret;

        if (!pmic->wdt_ops) {
                return CSI_UNSUPPORTED;
        }

        csi_pmic_dev_t *dev =
            csi_pmic_get_dev_by_pmic_id(pmic, pmic->hw_wdt_pmic_id);
        if (!dev) {
                return CSI_ERROR;
        }

        ret = pmic->wdt_ops->repower(pmic, dev);
        if (ret == CSI_OK)
                while (1)
                        ;
        return ret;
}

csi_error_t csi_pmic_set_log_level(csi_pmic_t *pmic, uint8_t pmic_id,
                                   uint8_t level)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        csi_pmic_dev_t *pmic_dev = NULL;
        csi_error_t     ret;

        pmic_dev = csi_pmic_get_dev_by_pmic_id(pmic, pmic_id);
        if (!pmic_dev) {
                return CSI_ERROR;
        }

        if (!pmic_dev->drv_data->ops->set_log_level) {
                LOG_D("pmic_id %d not support temperature get\n", pmic_id);
                return CSI_UNSUPPORTED;
        }

        ret = pmic_dev->drv_data->ops->set_log_level(pmic, pmic_dev, level);
        if (ret) {
                LOG_D("pmic_id %d temp get faild\n", pmic_id);
                return CSI_ERROR;
        }

        return CSI_OK;
}

csi_error_t csi_pmic_get_reg(csi_pmic_t *pmic, uint8_t pmic_id, uint32_t reg,
                             uint32_t *val)
{

        csi_error_t     ret      = CSI_OK;
        csi_pmic_dev_t *pmic_dev = NULL;

        pmic_dev = csi_pmic_get_dev_by_pmic_id(pmic, pmic_id);
        if (!pmic_dev) {
                return CSI_ERROR;
        }

        if (!pmic_dev->drv_data->ops->get_reg) {
                LOG_D("pmic_id %d not support  get reg \n", pmic_id);
                return CSI_UNSUPPORTED;
        }

        ret = pmic_dev->drv_data->ops->get_reg(pmic, pmic_dev, reg, val);
        if (ret) {
                LOG_D("pmic_id %d reg get faild\n", pmic_id);
                return CSI_ERROR;
        }

        return ret;
}

csi_error_t csi_pmic_set_reg(csi_pmic_t *pmic, uint8_t pmic_id, uint32_t reg,
                             uint32_t val)
{

        csi_error_t     ret      = CSI_OK;
        csi_pmic_dev_t *pmic_dev = NULL;

        pmic_dev = csi_pmic_get_dev_by_pmic_id(pmic, pmic_id);
        if (!pmic_dev) {
                return CSI_ERROR;
        }

        if (!pmic_dev->drv_data->ops->set_reg) {
                LOG_D("pmic_id %d not support  set reg \n", pmic_id);
                return CSI_UNSUPPORTED;
        }

        ret = pmic_dev->drv_data->ops->set_reg(pmic, pmic_dev, reg, val);
        if (ret) {
                LOG_D("pmic_id %d reg set faild\n", pmic_id);
                return CSI_ERROR;
        }

        return ret;
}

csi_error_t csi_pmic_get_powerup_flag(csi_pmic_t *pmic, uint32_t *flag)
{

        csi_error_t ret = CSI_ERROR;

        for (int i = 0; i < pmic->dev_num; i++) {
                csi_pmic_dev_t *pmic_dev = &pmic->dev_list[i];
                if (pmic_dev->drv_data->ops->get_powerup) {
                        ret = pmic_dev->drv_data->ops->get_powerup(
                            pmic, pmic_dev, flag);
                        if (ret) {
                                LOG_E("pmic dev:%d flag faild %d", i, ret);
                        } else {
                                return CSI_OK;
                        }
                }
        }
        LOG_I("ops not support\n");
        return CSI_UNSUPPORTED;
}

static int get_interrupt_pmic_id(csi_pmic_t *pmic, uint32_t pins, int *type)
{
        for (int i = 0; i < pmic->dev_num; i++) {
                if (pmic->dev_list[i].dev_info->flag & PMIC_DEV_ENABLE_ERR_IO) {
                        if ((1 << pmic->dev_list[i].dev_info->err_io_info.pin) & pins != 0) {
                                *type = PMIC_INTERRUPT_IO_ERR;
                                return pmic->dev_list[i].dev_info->pmic_id;
                        }
                }
                if (pmic->dev_list[i].dev_info->flag & PMIC_DEV_ENABLE_LPM_IO) {
                        if ((1 << pmic->dev_list[i].dev_info->lpm_io_info.pin) & pins != 0) {
                                *type = PMIC_INTERRUPT_IO_LPM;
                                return pmic->dev_list[i].dev_info->pmic_id;
                        }
                }
        }

        *type = PMIC_INTERRUPT_IO_UNKNOW;
        return -1;
}

static void gpio_interrupt_handler(csi_gpio_t *gpio, uint32_t pins, void *arg)
{
        /*fix me: for different gpio port*/
        csi_error_t ret;
        int         type    = 0;
        csi_pmic_t *pmic    = (csi_pmic_t *)arg;
        int         pmic_id = 0;

        pmic_id = get_interrupt_pmic_id(pmic, pins, &type);
        if (type == PMIC_INTERRUPT_IO_UNKNOW || pmic_id < 0) {
                goto UNKNOW_PINS;
        }

        /*handle pmic-err-irq event*/
        if (type == PMIC_INTERRUPT_IO_ERR) {
                ret = csi_pmic_read_faults(pmic, pmic_id);
                if (ret) {
                        LOG_E("read pmic-operation err %d\n", ret);
                        return;
                }
        }

        /*handle pmic-lpm-irq event*/
        if (type == PMIC_INTERRUPT_IO_LPM) {
                /*fix me: for other mode*/
                if (csi_gpio_read(gpio, pins)) {
                        LOG_D("Pmic exist sleep\r\n");
                        ret = csi_gpio_irq_mode(gpio, pins,
                                                GPIO_IRQ_MODE_LOW_LEVEL);
                        if (ret) {
                                LOG_D("Change to low level trigger "
                                      "mode failed %d\r\n",
                                      ret);
                        }
                } else {
                        LOG_D("Pmic enter sleep\r\n");
                        ret = csi_gpio_irq_mode(gpio, pins,
                                                GPIO_IRQ_MODE_HIGH_LEVEL);
                        if (ret) {
                                LOG_D("Change to high level trigger "
                                      "mode failed %d\r\n",
                                      ret);
                        }
                }
        }
        return;

UNKNOW_PINS:

        LOG_E("unexpected gpio-interrupt-pins pins:0x%x\n", pins);
        return;
}

int light_pin_irq_cfg(csi_gpio_t *csi_gpio, uint32_t gpio_port, uint32_t pin,
                      csi_gpio_irq_mode_t mode)
{
        CSI_PARAM_CHK(csi_gpio, CSI_ERROR);
        int err = CSI_OK;
        LOG_D("gpio port:%d pin:%d mode:%d\n", gpio_port, pin, mode);
        err = csi_gpio_init(csi_gpio, gpio_port);
        if (err) {
                return err;
        }

        err = csi_gpio_dir(csi_gpio, pin, GPIO_DIRECTION_INPUT);
        if (err) {
                return err;
        }

        err = csi_gpio_irq_mode(csi_gpio, pin, mode);
        if (err) {
                return err;
        }

        err = csi_gpio_irq_enable(csi_gpio, pin, true);
        if (err) {
                return err;
        }

        return CSI_OK;
}

static const pmic_drv_info_t *pmic_dev_drv_found(char *device_name,
                                                 char *version_name)
{
        for (int i = 0; i < ARRAY_SIZE(support_drv); i++) {
                if (!strncmp(support_drv[i].device_name, device_name,
                             strlen(support_drv[i].device_name)) &&
                    !strncmp(support_drv[i].version_name, version_name,
                             strlen(support_drv[i].version_name))) {
                        return &support_drv[i];
                }
        }
        return NULL;
}

static int pmic_parent_ctrl_init(csi_pmic_t *pmic, csi_gpio_t *csi_gpio, pmic_dev_info_t *dev)
{
    if(dev->ctrl_info.pmic_ctrl_type == PMIC_CTRL_BY_NOTHINTG) {
         return 0;
    }
     int err = 0;
     if(dev->ctrl_info.pmic_ctrl_type == PMIC_CTRL_BY_AON_GPIO) {
        pmic_ctrl_by_aon_info_t  *aon_io_info = &dev->ctrl_info.info.aon_io;
        LOG_D("parent aon gpio port:%d pin:%d mode:%d\n", aon_io_info->gpio_port,   \
        aon_io_info->pin, aon_io_info->activate_status);
        err = csi_gpio_init(csi_gpio, aon_io_info->gpio_port);
        if (err) {
                return err;
        }
        err = csi_gpio_dir(csi_gpio, aon_io_info->pin, GPIO_DIRECTION_OUTPUT);
        if (err) {
                return err;
        }
        } else if(dev->ctrl_info.pmic_ctrl_type == PMIC_CTRL_BY_PMIC_GPIO){
            pmic_ctrl_by_pmic_info_t *pmic_io_info = &dev->ctrl_info.info.pmic_io;
            csi_pmic_dev_t* parent_io_pmic = NULL;
            if (pmic_io_info->pmic_id !=  PMIC_ID_INVALID) {
             parent_io_pmic = csi_pmic_get_dev_by_pmic_id(
                                    pmic,
                                    pmic_io_info->pmic_id);
             if (!parent_io_pmic) {
                  LOG_E("Parent pmic not found %d\n", pmic_io_info->pmic_id);
                  return CSI_ERROR;
             }
             if(!parent_io_pmic->drv_data->ops->gpio_output) {
                  LOG_E("Parent pmic %d not support gpio output\n", pmic_io_info->pmic_id);
                  return CSI_ERROR;
             }
            } else {
                LOG_E("Parent pmic id not set");
                return CSI_ERROR;
            }

        } else {
            LOG_E("Unsupport parent pmic ctrl mode %d\n", dev->ctrl_info.pmic_ctrl_type);
     }
     return 0;
}

static int pmic_parent_ctrl(csi_pmic_t *pmic, pmic_dev_info_t *dev, bool enable)
{
    int ret = 0;
    if(dev->ctrl_info.pmic_ctrl_type == PMIC_CTRL_BY_NOTHINTG) {
         return 0;
    }
    if(dev->ctrl_info.pmic_ctrl_type == PMIC_CTRL_BY_AON_GPIO) {
         pmic_ctrl_by_aon_info_t  *aon_io_info = &dev->ctrl_info.info.aon_io;
         csi_gpio_pin_state_t state = aon_io_info->activate_status == HW_ID_ACTIVATE_HIGH? enable : !enable;
         csi_gpio_write(&pmic_csi_gpio, aon_io_info->pin,  state);
    } else if(dev->ctrl_info.pmic_ctrl_type == PMIC_CTRL_BY_PMIC_GPIO) {
        pmic_ctrl_by_pmic_info_t *pmic_io_info = &dev->ctrl_info.info.pmic_io;
        csi_pmic_dev_t* parent_io_pmic = NULL;
        /*fix me: consider the case: the parent pmic is ctrl by another one*/
                                parent_io_pmic = csi_pmic_get_dev_by_pmic_id(
                                    pmic,
                                    pmic_io_info->pmic_id);
                                        uint8_t io_set_val = (pmic_io_info->activate_status ==
                                                      HW_ID_ACTIVATE_HIGH)
                                                         ? enable
                                                         : !enable;
                                        ret = parent_io_pmic->drv_data->ops
                                                  ->gpio_output(
                                                      pmic, parent_io_pmic,
                                                      pmic_io_info->io_hw_id,
                                                      io_set_val);
                                        if (ret) {
                                                LOG_E("Pmic hw ctrl faild %d\n", ret);
                                                return ret;
                                        }
                              
        
        
    } else {
       LOG_E("Unsupport parent pmic ctrl mode %d\n", dev->ctrl_info.pmic_ctrl_type);
       return CSI_ERROR;
    }
    return 0;
}

csi_error_t pmic_register_device(csi_pmic_t *pmic, int dev_num,
                                 pmic_dev_info_t *dev_list)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        CSI_PARAM_CHK(dev_num, CSI_ERROR);
        CSI_PARAM_CHK(dev_list, CSI_ERROR);

        if (dev_num > CONFIG_MAX_PMIC_NUM) {
                LOG_E("Pmic dev num should no more than %d,actual set %d\n",
                      CONFIG_MAX_PMIC_NUM, dev_num);
                return CSI_ERROR;
        }

        int                    ret               = 0;
        const pmic_drv_info_t *drv_info          = NULL;
        int                    has_interrupt_pin = 0;

        for (int i = 0; i < dev_num; i++) {
                drv_info = pmic_dev_drv_found(dev_list[i].device_name,
                                              dev_list[i].version_name);
                if (!drv_info) {
                        LOG_E("No pmic drv for device:%s version:%s\n",
                              dev_list[i].device_name,
                              dev_list[i].version_name);
                        return CSI_ERROR;
                }

                if (dev_list[i].flag & PMIC_DEV_ENABLE_WDT) {
                        if (drv_info->drv_data->wdt_ops &&
                            (drv_info->drv_data->features &
                             PMIC_DRV_SUPPORT_WDT)) {
                                pmic->wdt_ops = drv_info->drv_data->wdt_ops;
                                pmic->hw_wdt_pmic_id = dev_list[i].pmic_id;
                                /*disable again, it will be enabled when call csi_pmic_wdt_start*/
                                dev_list[i].flag &= ~PMIC_DEV_ENABLE_WDT;
                        } else {
                                LOG_E("device:%s version:%s set wdt enable,but "
                                      "drv not support\n",
                                      dev_list[i].device_name,
                                      dev_list[i].version_name);
                                return CSI_ERROR;
                        }
                }

                if (dev_list[i].flag & PMIC_DEV_ENABLE_ERR_IO) {
                        ret = light_pin_irq_cfg(
                            &pmic_csi_gpio, dev_list[i].err_io_info.gpio_port,
                            1 << dev_list[i].err_io_info.pin,
                            dev_list[i].err_io_info.trigger_mode);
                        if (ret) {
                                LOG_E("pmic set err io faild %d\n", ret);
                                return CSI_ERROR;
                        }
                        has_interrupt_pin = 1;
                }

                if (dev_list[i].flag & PMIC_DEV_ENABLE_LPM_IO) {
                        ret = light_pin_irq_cfg(
                            &pmic_csi_gpio, dev_list[i].lpm_io_info.gpio_port,
                            1 << dev_list[i].lpm_io_info.pin,
                            dev_list[i].lpm_io_info.trigger_mode);
                        if (ret) {
                                LOG_E("pmic set lpm io faild %d\n", ret);
                                return CSI_ERROR;
                        }
                        has_interrupt_pin = 1;
                }

                      ret = pmic_parent_ctrl_init(pmic, &pmic_csi_gpio, &dev_list[i]);
                      if(ret) {
                             LOG_E("pmic parent ctrl init faild %d\n", ret);
                             return CSI_ERROR;
                      }

                pmic_devs[i].drv_data = drv_info->drv_data;
                pmic_devs[i].dev_info = &dev_list[i];
        }

        for (int i = 0; i < dev_num; i++) {
                ret = pmic_parent_ctrl(pmic, &pmic_devs[i], 1);
                if(ret) {
                      LOG_E("pmic parent ctrl on faild %d\n", ret);
                      return CSI_ERROR;     
                }
                if (pmic_devs[i].drv_data->ops->init) {
                        ret = pmic_devs[i].drv_data->ops->init(pmic,
                                                               &pmic_devs[i]);
                        if (ret) {
                                pmic_devs[i].pmic_dev_status.dev_status =
                                    CSI_STATUS_PMIC_DEV_INIT_FAILD;
                                return CSI_ERROR;
                        }
                }
                /* read chip ID info */
                if (pmic_devs[i].drv_data->ops->read_id != NULL) {
                        ret = pmic_devs[i].drv_data->ops->read_id(
                            pmic, &pmic_devs[i]);
                        if (ret) {
                                pmic_devs[i].pmic_dev_status.dev_status =
                                    CSI_STATUS_PMIC_DEV_INIT_FAILD;
                                return CSI_ERROR;
                        }
                }
                pmic_devs[i].pmic_dev_status.dev_status =
                    CSI_STATUS_PMIC_DEV_OK;
        }

        pmic->dev_num  = dev_num;
        pmic->dev_list = pmic_devs;

        if (has_interrupt_pin) {
                ret = csi_gpio_attach_callback(&pmic_csi_gpio,
                                               gpio_interrupt_handler, pmic);
                if (ret) {
                        LOG_E("pmic set io interrupt handler faild %d\n", ret);
                        return CSI_ERROR;
                }
        }

        return CSI_OK;
}

static csi_error_t pmic_register_regu(csi_pmic_t *pmic, int regu_num,
                                      csi_regu_id_t *regu_list)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        CSI_PARAM_CHK(regu_num, CSI_ERROR);
        CSI_PARAM_CHK(regu_list, CSI_ERROR);
        int ret = 0;

        if (regu_num > CONFIG_MAX_REGU_EXT_NUM) {
                LOG_E("regu num max support%d, actual set %d\n",
                      CONFIG_MAX_REGU_EXT_NUM, regu_num);
                return CSI_ERROR;
        }
        pmic->regu_num  = regu_num;
        pmic->regu_list = regu_list;
        for (int i = 0; i < regu_num; i++) {
                for (int j = 0; j < ARRAY_SIZE(regu_list[i].sub.id);
                     j++) {
                        int pmic_id = pmic->regu_list[i].sub.id[j].pmic_id;
                        if (pmic_id != PMIC_ID_INVALID) {
                                uint8_t hw_id = pmic->regu_list[i].sub.id[j].hw_id;
                                csi_regu_t *regu = NULL;
                                csi_pmic_dev_t *dev =
                                    csi_pmic_get_dev_by_pmic_id(pmic, pmic_id);
                                if (!dev) {
                                        LOG_E("%d not found", pmic_id);
                                        return CSI_ERROR;
                                }
                                ret = pmic_get_regu_info(dev, hw_id, &regu);
                                if(ret) {
                                     LOG_E("hw_id:%d not found for pmic:%s:%s\n", hw_id, dev->dev_info->device_name, dev->dev_info->version_name);
                                     return CSI_ERROR;
                                }
                                pmic->regu_list[i].sub.id[j].benable =  regu->initial_benable;
                        }
                }
        }



        return CSI_OK;
}

static csi_error_t pmic_soft_power_ctrl(csi_pmic_t *pmic, bool on)
{
        uint32_t    hw_seq[CONFIG_MAX_REGU_EXT_NUM * PMIC_MAX_HW_ID_NUM] = {0};
        int         soft_on_off_num                                      = 0;
        uint32_t    temp                                                 = 0;
        csi_error_t ret                                                  = 0;
        /*
     low 8_bit for  regu_ext_id
     high 8_bit for hw_id
     high 16_t  for seq
    */
        for (int i = 0; i < pmic->regu_num; i++) {
                for (int j = 0; j < ARRAY_SIZE(pmic->regu_list[i].sub.id);
                     j++) {
                        if (pmic->regu_list[i].sub.id[j].pmic_id ==
                            PMIC_ID_INVALID) {
                                continue;
                        }
                        if (on &&
                            pmic->regu_list[i]
                                    .sub.id[j]
                                    .soft_power_ctrl_info.on_info.on_order !=
                                HW_ID_NO_SOFT_AUTO_ON) {

                                hw_seq[soft_on_off_num++] =
                                    pmic->regu_list[i].regu_ext_id | j << 8 |
                                    pmic->regu_list[i]
                                            .sub.id[j]
                                            .soft_power_ctrl_info.on_info
                                            .on_order
                                        << 16;
                        } else if (!on && pmic->regu_list[i]
                                                  .sub.id[j]
                                                  .soft_power_ctrl_info.off_info
                                                  .off_order !=
                                              HW_ID_NO_SOFT_AUTO_OFF) {
                                hw_seq[soft_on_off_num++] =
                                    pmic->regu_list[i].regu_ext_id | j << 8 |
                                    pmic->regu_list[i]
                                            .sub.id[j]
                                            .soft_power_ctrl_info.off_info
                                            .off_order
                                        << 16;
                        }
                }
        }

        if (!soft_on_off_num) {
                LOG_D("No soft %s\n", on ? "ON" : "OFF");
                return CSI_OK;
        }

        for (int k = 0; k < soft_on_off_num - 1; k++) {
                for (int l = 0; l < soft_on_off_num - k - 1; l++) {
                        if ((hw_seq[l] & 0xFFFF0000) >
                            (hw_seq[l + 1] & 0xFFFF0000)) {
                                temp          = hw_seq[l];
                                hw_seq[l]     = hw_seq[l + 1];
                                hw_seq[l + 1] = temp;
                        }
                }
        }

        for (int h = 0; h < soft_on_off_num;) {
                int base_hw_id  = (hw_seq[h] >> 8) & 0xFF;
                int regu_ext_id = hw_seq[h] & 0xFF;
                int hw_id_num   = pmic_get_regu_hw_id_num(pmic, regu_ext_id);
                if (hw_id_num < 0) {
                        return CSI_ERROR;
                }
                csi_regu_id_t *regu_id = NULL;
                csi_regu_t    *regu    = NULL;
                ret = pmic_get_id_info(pmic, regu_ext_id, &regu_id);
                if (ret) {
                        return ret;
                }

                LOG_D("%s:regu_ext_id:%d hw_num:%d\n", on ? "ON" : "OFF",
                      regu_ext_id, hw_id_num);
                /*set volatage if on*/
                if (on) {
                        regu_voltage_param_t regu_param = {0x0};

                        for (int j = 0; j < hw_id_num; j++) {
                                int      hw_id  = (hw_seq[h + j] >> 8) & 0xFF;
                                int      ext_id = hw_seq[h + j] & 0xFF;
                                uint32_t on_uv  = regu_id->sub.id[hw_id]
                                                     .soft_power_ctrl_info
                                                     .on_info.init_target_uv;

                                if (ext_id != regu_ext_id) {
                                        LOG_E("Multi regu hw sould set "
                                              "together\n");
                                        return -1;
                                }
                                /*if on_uv not equal 0, check if the hw id support vol set*/
                                if (on_uv) {
                                        int pmic_id =
                                            regu_id->sub.id[hw_id].pmic_id;
                                        int regu_hw_id =
                                            regu_id->sub.id[hw_id].hw_id;
                                        csi_pmic_dev_t *dev =
                                            csi_pmic_get_dev_by_pmic_id(
                                                pmic, pmic_id);
                                        if (!dev) {
                                                LOG_E("%d not found", pmic_id);
                                                return CSI_ERROR;
                                        }
                                        ret = pmic_get_regu_info(
                                            dev, regu_hw_id, &regu);
                                        if ((regu->max_uv == regu->min_uv) &&
                                            !regu->step_uv) {
                                                LOG_D("pmic:%d hw_id:%d not "
                                                      "support volatage set\n",
                                                      pmic_id, regu_hw_id);
                                                on_uv = 0;
                                        }
                                }
                                regu_param.target_uv[hw_id] = on_uv;
                        }

                        regu_param.regu_ext_id = regu_ext_id;
                        if (regu_param.target_uv[0]) {
                                LOG_D("regu_ext_id:%d uv0:%duv uv1:%duv "
                                      "uv2:%duv\n",
                                      regu_ext_id, regu_param.target_uv[0],
                                      regu_param.target_uv[1],
                                      regu_param.target_uv[2]);
                                ret = csi_pmic_regulator_set_voltage(
                                    pmic, regu_param);
                                if (ret) {
                                        return ret;
                                }
                        }
                }

                uint32_t delay_ms = 0;

                if (on) {
                        delay_ms =
                            regu_id->sub.id[base_hw_id]
                                .soft_power_ctrl_info.on_info.on_delay_ms;
                } else {
                        delay_ms =
                            regu_id->sub.id[base_hw_id]
                                .soft_power_ctrl_info.off_info.off_delay_ms;
                }

                ret = csi_pmic_regulator_ctrl(pmic, regu_ext_id, on);
                if (ret) {
                        return ret;
                }
                if (delay_ms) {
                        mdelay(delay_ms);
                }

                h += hw_id_num;
        }
        return CSI_OK;
}

csi_error_t csi_pmic_init(csi_pmic_t *pmic, pmic_config_t *config)
{
        csi_error_t ret;
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        CSI_PARAM_CHK(config, CSI_ERROR);

        ret = csi_iic_init(&pmic->if_dev, config->iic_config.iic_id);
        if (ret) {
                pmic->pmic_status = CSI_STATUS_PMIC_IIC_ERR;
                return ret;
        }

        pmic->arg                        = NULL;
        pmic->call_back                  = NULL;
        pmic->pmic_read_reg              = iic_read_reg;
        pmic->pmic_read_reg_sr           = iic_read_reg_sr;
        pmic->pmic_write_reg             = iic_write_reg;
        pmic->pmic_read_reg_multi_bytes  = iic_read_reg_multi_bytes;
        pmic->pmic_write_reg_multi_bytes = iic_write_reg_multi_bytes;

	/*pmic iic initialization */
	ret = csi_pmic_if_config(pmic, &config->iic_config);
        if (ret) {
                pmic->pmic_status = CSI_STATUS_PMIC_IIC_CONFIG_ERR;
                return ret;
        }

        /*regist all pmic-dev*/
        ret = pmic_register_device(pmic, config->pmic_dev_num,
                                   config->pmic_dev_list);
        if (ret) {
                pmic->pmic_status = CSI_STATUS_PMIC_DEV_REG_ERR;
                return ret;
        }

        /*regist all regu list*/
        ret = pmic_register_regu(pmic, config->regu_num, config->regu_id_list);
        if (ret) {
                pmic->pmic_status = CSI_STATUS_PMIC_REGU_REG_ERR;
                return ret;
        }

        /*soft init powerup */
        ret = pmic_soft_power_ctrl(pmic, 1);
        if (ret) {
                pmic->pmic_status = CSI_STATUS_PMIC_SOFT_POWER_CTRL_ERR;
                return ret;
        }
        pmic->pmic_status = CSI_STATUS_PMIC_OK;

        return CSI_OK;
}

void csi_pmic_uninit(csi_pmic_t *pmic)
{
        CSI_PARAM_CHK(pmic, CSI_ERROR);
        csi_iic_uninit(&pmic->if_dev);

        pmic->arg            = NULL;
        pmic->call_back      = NULL;
        pmic->pmic_read_reg  = NULL;
        pmic->pmic_write_reg = NULL;
}

csi_error_t csi_pmic_soft_power_off(csi_pmic_t *pmic)
{
        csi_error_t ret = 0;
        /*power off soft pw id*/
        ret = pmic_soft_power_ctrl(pmic, 0);
        if (ret) {
                pmic->pmic_status = CSI_STATUS_PMIC_SOFT_POWER_CTRL_ERR;
                LOG_E("soft power down faild %d\n", ret);
                return ret;
        }
        /*call each pmic uninit*/
        for (int i = 0; i < pmic->dev_num; i++) {
                csi_pmic_dev_t *dev = &pmic->dev_list[i];
                if (dev->drv_data->ops->uninit) {
                        ret = dev->drv_data->ops->uninit(pmic, dev);
                        if (ret) {
                                dev->pmic_dev_status.dev_status =
                                    CSI_STATUS_PMIC_DEV_UNINIT_FAILD;
                                return CSI_ERROR;
                        }
                }
                ret = pmic_parent_ctrl(pmic, dev, 0);
                if(ret) {
                      LOG_E("pmic parent ctrl off faild %d\n", ret);
                      return CSI_ERROR;     
                }
                dev->pmic_dev_status.dev_status = CSI_STATUS_PMIC_DEV_NOT_INIT;
        }

        pmic->pmic_status = CSI_STATUS_PMIC_NOT_INIT;

        return CSI_OK;
}
