/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * SLG51000 High PSRR, Multi-Output Regulators
 * Copyright (C) 2019  Dialog Semiconductor
 *
 * Author: Eric Jeong <eric.jeong.opensource@diasemi.com>
 */

#ifndef __SLG51000_REGISTERS_H__
#define __SLG51000_REGISTERS_H__

/* Registers */

#define SLG51000_LDO1_VSEL                      0x2000
#define SLG51000_LDO2_VSEL                      0x2200
#define SLG51000_LDO3_VSEL                      0x2300
#define SLG51000_LDO4_VSEL                      0x2500
#define SLG51000_LDO5_VSEL                      0x2700
#define SLG51000_LDO6_VSEL                      0x2900
#define SLG51000_LDO7_VSEL                      0x3100
#define SLG51000_SYSCTL_MATRIX_CONF_A		0x110d

/* SLG51000_LDO1_VSEL ~ SLG51000_LDO7_VSEL =
 * 0x2000, 0x2200, 0x2300, 0x2500, 0x2700, 0x2900, 0x3100
 */
#define SLG51000_VSEL_MASK                      (0xff << 0)

typedef enum {
	SLG51000_ID_LDO1,
	SLG51000_ID_LDO2,
	SLG51000_ID_LDO3,
	SLG51000_ID_LDO4,
	SLG51000_ID_LDO5,
	SLG51000_ID_LDO6,
	SLG51000_ID_LDO7,
}slg51000_regu_hw_id_t;

#endif /* __SLG51000_REGISTERS_H__ */

