/*
 * Copyright (C) 2017-2021 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file       pmic_help.h
 * @brief      Header File for pmic Driver
 * @version    V1.0
 * @date       9. Oct 2021
 * @model      pmic
 ******************************************************************************/

#ifndef _DRV_PMIC_HELP_H_
#define _DRV_PMIC_HELP_H_

#include <drv/pmic.h>

/**
  \brief       get regulator object
  \param[in]   pmic_dev    pmic device handle.
  \param[in]   regu_id     hw-regulator id defined in pmic_rsc_def.h
  \param[out]  regu        point to regulator object
  \return      Error code \ref csi_error_t.
*/
csi_error_t pmic_get_regu_info(csi_pmic_dev_t *pmic_dev, uint32_t regu_id,csi_regu_t **regu);

/**
  \brief       get regulator id object
  \param[in]   pmic_dev       pmic device handle.
  \param[in]   regu_ext_id    virtual regulator id defined in pmic_rsc_def.h
  \param[out]  id_info        point to regulator id object
  \return      Error code \ref csi_error_t.
*/
//csi_error_t pmic_get_id_info(csi_pmic_dev_t *pmic_dev, uint32_t regu_ext_id, csi_regu_id_t* id_info);

/**
  \brief       get dev-addr for ricoh
  \param[in]   regu_id        hw regu id
  \param[out]  dev_addr       pmic dev-addr (init set by vendor's OTP)
  \return      Error code \ref csi_error_t.
*/
csi_error_t soc_pmic_get_ricoh567_dev_addr_by_reguid(uint32_t regu_id, uint16_t *dev_addr);

uint16_t     soc_pmic_get_ricoh567_major_dev(void);
uint16_t     soc_pmic_get_ricoh567_minor_dev(void);
csi_error_t slg_51000_ldo3_onoff(csi_pmic_t *pmic, uint32_t bon);
csi_error_t da9121_bulk_onoff(csi_pmic_t *pmic, uint32_t bon);
#endif
