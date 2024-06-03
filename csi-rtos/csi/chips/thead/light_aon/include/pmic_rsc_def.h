

/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     pmic_rsc_def.h
 * @brief    header file for pmic_rsc_def.c
 * @version  V1.0
 * @date     2021-03-01
 ******************************************************************************/

#ifndef _PMIC_RSC_DEF_H_
#define _PMIC_RSC_DEF_H_

#define REGU_ID_DEF(x) \
{ \
    .name = #x, \
    .id   = x, \
}

/* struct of  regulator id resource */
#define HW_ID_CTRL_DEF(parent_id, id, ctrl_type) \
{ \
  	.pmic_id  = parent_id, \
    .io_hw_id = id, \
    .activate_status = ctrl_type, \
}

#define HW_ID_POWER_DEF(on_order_1,on_delay_ms_1, init_target_uv_1, off_order_1, off_delay_ms_1) \
{ \
   .on_info.on_order =  on_order_1, \
   .on_info.on_delay_ms = on_delay_ms_1, \
   .on_info.init_target_uv = init_target_uv_1, \
   .off_info.off_order = off_order_1, \
   .off_info.off_delay_ms = off_delay_ms_1, \
}

#define HW_ID_COUPLE_DEF(pmic_coupled, regu_coupled, maxspread, minspread)\
{ \
	.pmic_id = pmic_coupled, \
	.hw_id = regu_coupled, \
	.max_spread = maxspread, \
	.min_spread = minspread,\
}

#define REGU_ID_SINGLE_RAIL_DEF(ext_id, pmic_id1, hw_id1, id1_parent_id, id1_io_hw_id, id1_activate_status, id1_on_order, id1_on_delay_ms, id1_init_target_uv, id1_off_order, id1_off_delay_ms) \
{ \
	.regu_ext_id = ext_id, \
	.regu_ext_id_name = #ext_id,\
	.sub.id[0].pmic_id = pmic_id1, \
	.sub.id[0].hw_id   = hw_id1,  \
	.sub.id[0].coupling_desc = HW_ID_COUPLE_DEF(-1, PMIC_ID_INVALID, 0, 0),\
	.sub.id[0].parent_hw_info = HW_ID_CTRL_DEF(id1_parent_id, id1_io_hw_id, id1_activate_status), \
	.sub.id[1].pmic_id = PMIC_ID_INVALID, \
	.sub.id[2].pmic_id = PMIC_ID_INVALID, \
}

#define REGU_ID_DUAL_RAIL_DEF(ext_id, pmic_id1, hw_id1, cpmic_id1, chw_id1, max_spread1, min_spread1, id1_parent_id, id1_io_hw_id, id1_activate_status, id1_on_order, id1_on_delay_ms, id1_init_target_uv , id1_off_order, id1_off_delay_ms  \
                                    , pmic_id2, hw_id2, cpmic_id2, chw_id2, max_spread2, min_spread2, id2_parent_id, id2_io_hw_id, id2_activate_status, id2_on_order, id2_on_delay_ms, id2_init_target_uv , id2_off_order, id2_off_delay_ms) \
{ \
	.regu_ext_id = ext_id, \
	.regu_ext_id_name = #ext_id,\
	.sub.id[0].pmic_id = pmic_id1, \
	.sub.id[0].hw_id = hw_id1, \
	.sub.id[0].coupling_desc = HW_ID_COUPLE_DEF(cpmic_id1, chw_id1, max_spread1, min_spread1),\
	.sub.id[0].parent_hw_info = HW_ID_CTRL_DEF(id1_parent_id, id1_io_hw_id, id1_activate_status), \
	.sub.id[0].soft_power_ctrl_info = HW_ID_POWER_DEF(id1_on_order, id1_on_delay_ms, id1_init_target_uv, id1_off_order, id1_off_delay_ms),\
	.sub.id[1].pmic_id = pmic_id2, \
	.sub.id[1].hw_id = hw_id2, \
	.sub.id[1].coupling_desc = HW_ID_COUPLE_DEF(cpmic_id2, chw_id2, max_spread2, min_spread2),\
	.sub.id[1].parent_hw_info = HW_ID_CTRL_DEF(id2_parent_id, id2_io_hw_id, id2_activate_status), \
	.sub.id[1].soft_power_ctrl_info = HW_ID_POWER_DEF(id2_on_order, id2_on_delay_ms, id2_init_target_uv, id2_off_order, id2_off_delay_ms),\
	.sub.id[2].pmic_id = PMIC_ID_INVALID, \
}

#define REGU_ID_TRIPLE_RAIL_DEF(ext_id, pmic_id1, hw_id1, cpmic_id1, chw_id1, max_spread1, min_spread1, id1_parent_id, id1_io_hw_id, id1_activate_status, id1_on_order, id1_on_delay_ms, id1_init_target_uv  , id1_off_order, id1_off_delay_ms \
                                      , pmic_id2, hw_id2, cpmic_id2, chw_id2, max_spread2, min_spread2, id2_parent_id, id2_io_hw_id, id2_activate_status, id2_on_order, id2_on_delay_ms, id2_init_target_uv  , id2_off_order, id2_off_delay_ms \
									  , pmic_id3, hw_id3, cpmic_id3, chw_id3, max_spread3, min_spread3, id3_parent_id, id3_io_hw_id, id3_activate_status, id3_on_order, id3_on_delay_ms, id3_init_target_uv  , id3_off_order, id3_off_delay_ms) \
{ \
	.regu_ext_id = ext_id, \
	.regu_ext_id_name = #ext_id,\
	.sub.id[0].pmic_id = pmic_id1, \
	.sub.id[0].hw_id = hw_id1, \
	.sub.id[0].coupling_desc = HW_ID_COUPLE_DEF(cpmic_id1, chw_id1, max_spread1, min_spread1),\
	.sub.id[0].parent_hw_info = HW_ID_CTRL_DEF(id1_parent_id, id1_io_hw_id, id1_activate_status), \
	.sub.id[0].soft_power_ctrl_info = HW_ID_POWER_DEF(id1_on_order, id1_on_delay_ms, id1_init_target_uv, id1_off_order, id1_off_delay_ms),\
	.sub.id[1].pmic_id = pmic_id2, \
	.sub.id[1].hw_id = hw_id2, \
	.sub.id[1].coupling_desc = HW_ID_COUPLE_DEF(cpmic_id2, chw_id2, max_spread2, min_spread2),\
	.sub.id[1].parent_hw_info = HW_ID_CTRL_DEF(id2_parent_id, id2_io_hw_id, id2_activate_status), \
	.sub.id[1].soft_power_ctrl_info = HW_ID_POWER_DEF(id2_on_order, id2_on_delay_ms, id2_init_target_uv, id2_off_order, id2_off_delay_ms),\
	.sub.id[2].pmic_id = pmic_id3, \
	.sub.id[2].hw_id = hw_id3, \
	.sub.id[0].coupling_desc = HW_ID_COUPLE_DEF(cpmic_id3, chw_id3, max_spread3, min_spread3),\
	.sub.id[2].parent_hw_info = HW_ID_CTRL_DEF(id3_parent_id, id3_io_hw_id, id3_activate_status), \
	.sub.id[2].soft_power_ctrl_info = HW_ID_POWER_DEF(id3_on_order, id3_on_delay_ms, id3_init_target_uv, id3_off_order, id3_off_delay_ms),\
}

/* virtual global regulator id resource */

#endif
