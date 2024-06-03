#include "inc/ddr_common_func.h"
#include "syslog.h"

/*
/// data structure to store register address, value pairs
typedef struct Reg_Addr_Val {

  uint32_t Address; ///< register address
  uint16_t Value;   ///< register value
} Reg_Addr_Val_t;

/// enumeration of instructions for PhyInit Register Interface
typedef enum {
  saveRegs,         ///< save(read) tracked register values
  restoreRegs,      ///< restore (write) saved register values
} regInstr;
*/
//int NumRegSaved = 934;    ///< Current Number of registers saved.

/*! \def MAX_NUM_RET_REGS
 *  \brief default Max number of retention registers
 *
 * This define is only used by the PhyInit Register interface to define the max
 * amount of registered that can be saved. The user may increase this variable
 * as desired if a larger number of registers need to be restored.
*/
#define MAX_NUM_RET_REGS 5000

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

static const uint32_t MiscRegList[] = {
    DFILPCFG0,
    ADDRMAP0,
    ADDRMAP1,
    ADDRMAP2,
    ADDRMAP3,
    ADDRMAP4,
    ADDRMAP5,
    ADDRMAP6,
    ADDRMAP7,
    ADDRMAP9,
    ADDRMAP10,
    ADDRMAP11,
};


extern uint32_t __ddr_phy_reg_start;
static Ddr_Reg_Config_t *ddr_Regu_Config = (Ddr_Reg_Config_t *) & __ddr_phy_reg_start;


int dwc_ddrphy_phyinit_regInterface(regInstr myRegInstr) {
  ddr_phy_reg_wr(0xd0000, 0x0);
  ddr_phy_reg_wr(0xc0080, 0x3);
  int NumReg = ddr_Regu_Config->phy_regu_num;

  Reg_Phy_Addr_Val_t* phy_addr_t = (Reg_Phy_Addr_Val_t*)((char*)&__ddr_phy_reg_start +  64 +  sizeof(Reg_Misc_Addr_Val_t) *  ddr_Regu_Config->misc_regu_num);

  if (myRegInstr == restoreRegs)
  {
    // write PHY registers based on Address, Data value pairs stores in
    // RetRegList
    int regIndx = 0;

    ddr_phy0_reg_wr(0x20089, 0x1);
    for (regIndx = 0; regIndx < NumReg; regIndx++)
    {
      ddr_phy0_reg_wr(phy_addr_t[regIndx].Address, phy_addr_t[regIndx].Value0);
    }

#ifndef CONFIG_DDR_H32_MODE
    ddr_phy1_reg_wr(0x20089, 0x1);
    for (regIndx = 0; regIndx < NumReg; regIndx++)
    {
      ddr_phy1_reg_wr(phy_addr_t[regIndx].Address, phy_addr_t[regIndx].Value1);
    }
#endif
  }
  ddr_phy_reg_wr(0xc0080, 0x2);
  ddr_phy_reg_wr(0xd0000, 0x1);
  return 1;
}

int dwc_get_ddr_rank_num()
{
    return ddr_Regu_Config->ddr_rank;
}

void addrmap_resume(int rank_num, enum DDR_BITWIDTH bits, Reg_Misc_Addr_Val_t*reg, uint32_t regu_num) {
    if (bits == DDR_BITWIDTH_16) {
#ifdef CONFIG_DDR_MSG
        printf("DDR 16bit mode\n");
#endif
        wr(MSTR, 0x01080020 | 0x1000);
    }
    for(int i=0 ; i< regu_num; i++) {
        wr(reg[i].Address, reg[i].Value);
    }
}

void dwc_umctl_init_skip_traing(enum DDR_TYPE type, int rank_num, int speed, enum DDR_BITWIDTH bits)
{
  ddr_sysreg_wr(DDR_CFG0, 0x50);  // release apb presetn
  ddr_sysreg_wr(DDR_CFG0, 0x50);
  ddr_sysreg_wr(DDR_CFG0, 0x50);
  if (bits == 32) {
    ddr_sysreg_wr(DDR_CFG0, 0x52);
  }
  int misc_regu_num  =  ddr_Regu_Config->misc_regu_num;
  Reg_Misc_Addr_Val_t* misc_addr_t = (Reg_Misc_Addr_Val_t*)((char*)&__ddr_phy_reg_start +  64);
  if(misc_addr_t[0].Address != DFILPCFG0 || misc_addr_t[1].Address != ADDRMAP0 || misc_regu_num!= ARRAY_SIZE(MiscRegList)) {
      printf("ddr err misc info\n");
      asm volatile("j .");
  }
  ctrl_init(rank_num, speed, misc_addr_t[0].Value);
  addrmap_resume(rank_num, bits, &misc_addr_t[1], misc_regu_num - 1);

  de_assert_other_reset_ddr(); //after this step, only PwrOk is staill low

  dq_pinmux(bits);
}

