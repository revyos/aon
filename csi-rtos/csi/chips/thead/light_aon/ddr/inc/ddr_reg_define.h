/*
 * Copyright (C) 2023 Alibaba Group Holding Limited
 */

 /******************************************************************************
  * @file     ddr_common_func.h
  * @brief    ddr low power mode
  * @version  V0.1
  * @date     29. May 2023
  ******************************************************************************/

#ifndef __DDR_REG_DEFINE_H__
#define __DDR_REG_DEFINE_H__


#define AP_SUBSYS_BADDR 0xFF010000
#define VISYS_CLK_TEECFG (AP_SUBSYS_BADDR + 0x11d0)
#define VOSYS_ACLK_TEECFG (AP_SUBSYS_BADDR + 0x11dc)
#define NPU_CCLK_TEECFG (AP_SUBSYS_BADDR + 0x11c8)
#define VPSYS_CLK_TEECFG (AP_SUBSYS_BADDR + 0x11e0)

#define USE_LPDDR4

#ifdef USE_LPDDR4

  /* DDR REG BASE ADDRESS */
#define _DDR_PHY_BADDR   0xfd000000
#define DDR_SYSREG_BADDR 0xff005000
#define _DDR_PHY1_BADDR   _DDR_PHY_BADDR+0x1000000
#define _DDR_CTRL_BADDR  _DDR_PHY_BADDR+0x2000000

#define  DCH1_MRCTRL0              _DDR_CTRL_BADDR+0x00001b10 
#define  DCH1_MRCTRL1              _DDR_CTRL_BADDR+0x00001b14 
#define  DCH1_DERATECTL            _DDR_CTRL_BADDR+0x00001b2c 
#define  DCH1_PWRCTL               _DDR_CTRL_BADDR+0x00001b30 
#define  DCH1_HWLPCTL              _DDR_CTRL_BADDR+0x00001b38 
#define  DCH1_CRCPARCTL0           _DDR_CTRL_BADDR+0x00001bc0 
#define  DCH1_ZQCTL2               _DDR_CTRL_BADDR+0x00001c88 
#define  DCH1_ODTMAP               _DDR_CTRL_BADDR+0x00001d44 
#define  DCH1_DBG1                 _DDR_CTRL_BADDR+0x00001e04 
#define  DCH1_DBGCMD               _DDR_CTRL_BADDR+0x00001e0c 
#define  DCH1_DFISTAT              _DDR_CTRL_BADDR+0x00001cbc 
#define  DCH1_STAT                 _DDR_CTRL_BADDR+0x00001b04 
#define  DCH1_DBGCAM               _DDR_CTRL_BADDR+0x00001e08 
#define  DCH1_MRSTAT               _DDR_CTRL_BADDR+0x00001b18 
#define  FREQ1_DERATEEN            _DDR_CTRL_BADDR+0x00002020 
#define  FREQ1_DERATEINT           _DDR_CTRL_BADDR+0x00002024 
#define  FREQ1_PWRTMG              _DDR_CTRL_BADDR+0x00002034 
#define  FREQ1_RFSHCTL0            _DDR_CTRL_BADDR+0x00002050 
#define  FREQ1_RFSHTMG             _DDR_CTRL_BADDR+0x00002064 
#define  FREQ1_RFSHTMG1            _DDR_CTRL_BADDR+0x00002068 
#define  FREQ1_INIT3               _DDR_CTRL_BADDR+0x000020dc 
#define  FREQ1_INIT4               _DDR_CTRL_BADDR+0x000020e0 
#define  FREQ1_INIT6               _DDR_CTRL_BADDR+0x000020e8 
#define  FREQ1_INIT7               _DDR_CTRL_BADDR+0x000020ec 
#define  FREQ1_RANKCTL             _DDR_CTRL_BADDR+0x000020f4 
#define  FREQ1_RANKCTL1            _DDR_CTRL_BADDR+0x000020f8 
#define  FREQ1_DRAMTMG0            _DDR_CTRL_BADDR+0x00002100 
#define  FREQ1_DRAMTMG1            _DDR_CTRL_BADDR+0x00002104 
#define  FREQ1_DRAMTMG2            _DDR_CTRL_BADDR+0x00002108 
#define  FREQ1_DRAMTMG3            _DDR_CTRL_BADDR+0x0000210c 
#define  FREQ1_DRAMTMG4            _DDR_CTRL_BADDR+0x00002110 
#define  FREQ1_DRAMTMG5            _DDR_CTRL_BADDR+0x00002114 
#define  FREQ1_DRAMTMG6            _DDR_CTRL_BADDR+0x00002118 
#define  FREQ1_DRAMTMG7            _DDR_CTRL_BADDR+0x0000211c 
#define  FREQ1_DRAMTMG8            _DDR_CTRL_BADDR+0x00002120 
#define  FREQ1_DRAMTMG12           _DDR_CTRL_BADDR+0x00002130 
#define  FREQ1_DRAMTMG13           _DDR_CTRL_BADDR+0x00002134 
#define  FREQ1_DRAMTMG14           _DDR_CTRL_BADDR+0x00002138 
#define  FREQ1_ZQCTL0              _DDR_CTRL_BADDR+0x00002180 
#define  FREQ1_DFITMG0             _DDR_CTRL_BADDR+0x00002190 
#define  FREQ1_DFITMG1             _DDR_CTRL_BADDR+0x00002194 
#define  FREQ1_DFITMG2             _DDR_CTRL_BADDR+0x000021b4 
#define  FREQ1_ODTCFG              _DDR_CTRL_BADDR+0x00002240 
#define PCFGQOS_0_0                _DDR_CTRL_BADDR+0x494
#define PCFGQOS_0_1                _DDR_CTRL_BADDR+0x494+0xb0*1
#define PCFGQOS_0_2                _DDR_CTRL_BADDR+0x494+0xb0*2
#define PCFGQOS_0_3                _DDR_CTRL_BADDR+0x494+0xb0*3
#define PCFGQOS_0_4                _DDR_CTRL_BADDR+0x494+0xb0*4
#define PCFGQOS_0_5                _DDR_CTRL_BADDR+0x494+0xb0*5
#define PCFGQOS_1_0                _DDR_CTRL_BADDR+0x498
#define PCFGQOS_1_1                _DDR_CTRL_BADDR+0x498+0xb0*1
#define PCFGQOS_1_2                _DDR_CTRL_BADDR+0x498+0xb0*2
#define PCFGQOS_1_3                _DDR_CTRL_BADDR+0x498+0xb0*3
#define PCFGQOS_1_4                _DDR_CTRL_BADDR+0x498+0xb0*4

#include "ddr_sysreg_registers_struct.h"
#include "ddr_sysreg_registers.h"

#include "DWC_ddr_umctl2_c_struct.h"
#include "DWC_ddr_umctl2_header.h"

#include "ddr_retention.h"

#include "aonsys_rstget_reg_define.h"
#include "aonsys_reg_define.h"

#endif

#else

#error "Unsupport Board!!!!!!"

#endif //__DDR_REG_DEFINE_H__
