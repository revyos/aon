/*
 * Copyright (C) 2023 Alibaba Group Holding Limited
 */

 /******************************************************************************
  * @file     ddr_common_func.h
  * @brief    ddr low power mode
  * @version  V0.1
  * @date     29. May 2023
  ******************************************************************************/

#ifndef __DDR_COMMON_FUNC_H__
#define __DDR_COMMON_FUNC_H__

#include <stdint.h>
#include <csi_config.h>
#include "ddr_reg_define.h"

enum DDR_TYPE {
    DDR_TYPE_LPDDR4X = 0,
    DDR_TYPE_LPDDR4,
    DDR_TYPE_MAX,
};

enum DDR_BITWIDTH {
    DDR_BITWIDTH_16 = 16,
    DDR_BITWIDTH_32 = 32,
    DDR_BITWIDTH_64 = 64,
    DDR_BITWIDTH_MAX = DDR_BITWIDTH_64,
};

enum DDR_RANK_NUM {
    DDR_SINGLE_RANK = 1,
    DDR_DUAL_RANK = 2,
};

#define __INLINE inline

static __INLINE void wr(unsigned int addr, unsigned int data)
{
    *(volatile unsigned int*)(addr) = data;
}

static __INLINE unsigned int rd(unsigned int addr)
{
    return *(volatile unsigned int*)(addr);
}

static __INLINE void wr16(unsigned int addr, unsigned short data)
{
    *(volatile unsigned short*)(addr) = data;
}
static __INLINE unsigned short rd16(unsigned int addr)
{
    return *(volatile unsigned short*)(addr);
}

static __INLINE void ddr_sysreg_wr(unsigned int addr, unsigned int wr_data) {
    wr(addr + DDR_SYSREG_BADDR, wr_data);
}

static __INLINE unsigned int ddr_sysreg_rd(int addr) {
    int rdata;
    rdata = rd(addr + DDR_SYSREG_BADDR);
    return rdata;
}

static __INLINE void ddr_phy_reg_wr(unsigned int addr,unsigned int wr_data) {
    //unsigned int ddr_phy_sel,addr_low;
    addr<<=1;
    //ddr_phy_sel=(addr)&0x00ffffff;
    //ddr_phy_sel>>=21;
    //addr_low=(addr)&0x001fffff;
    //wr(_SYS_REG_DDR_PHY_PS_SEL,ddr_phy_sel);
    //ddr_sysreg_rd(DDR_CFG0);
    wr16(_DDR_PHY_BADDR+addr, wr_data);
#ifndef CONFIG_DDR_H32_MODE
    wr16(_DDR_PHY1_BADDR+addr, wr_data);
#endif

}

static __INLINE void ddr_phy0_reg_wr(unsigned int addr,unsigned int wr_data) {
    addr<<=1;
    wr16(_DDR_PHY_BADDR+addr, wr_data);

}

static __INLINE void ddr_phy1_reg_wr(unsigned int addr,unsigned int wr_data) {
    addr<<=1;
    wr16(_DDR_PHY1_BADDR+addr, wr_data);

}

static __INLINE void ddr_phy_broadcast_en(int enable) {
    //ddr_sysreg.ddr_sysreg_registers_struct_ddr_cfg0.u32 = ddr_sysreg_rd(DDR_CFG0);
    //ddr_sysreg.ddr_sysreg_registers_struct_ddr_cfg0.rg_broadcast_mode = enable;
    //ddr_sysreg_wr(DDR_CFG0,ddr_sysreg.ddr_sysreg_registers_struct_ddr_cfg0.u32);
    ddr_sysreg_rd(DDR_CFG0);
    ddr_sysreg_rd(DDR_CFG0);
}

static __INLINE unsigned int ddr_phy1_reg_rd(unsigned int addr) {
    //unsigned int ddr_phy_sel,addr_low,rd_data;
    unsigned int rd_data;
    addr<<=1;
    rd_data=rd16(_DDR_PHY1_BADDR+addr);
    return rd_data;
}
static __INLINE unsigned int ddr_phy0_reg_rd(unsigned int addr) {
    //unsigned int ddr_phy_sel,addr_low,rd_data;
    unsigned int rd_data;
    addr<<=1;
    rd_data=rd16(_DDR_PHY_BADDR+addr);
    return rd_data;
}
static __INLINE unsigned int ddr_phy_reg_rd(unsigned int addr) {
    //unsigned int ddr_phy_sel,addr_low,rd_data;
    unsigned int rd_data;
    addr<<=1;
    rd_data=rd16(_DDR_PHY_BADDR+addr);
    return rd_data;
}

void ctrl_init(int rank_num, int speed, uint32_t dfilpcfg0_value);
void addrmap (int rank_num, enum DDR_BITWIDTH bits);
void de_assert_other_reset_ddr();
void dq_pinmux (enum DDR_BITWIDTH bits);
#endif //__DDR_COMMON_FUNC_H__
