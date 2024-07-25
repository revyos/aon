// *****************************************************************************
// FILE NAME       : emmc_api.c
// AUTHOR          : Yongchen Bai
// FUNCTION        :
// VERSION         : 2.0
// DESCRIPTION     : This is a new api when the version 1.80a of DWC_mshc is uesd.
// DATE            : 2020/06/17
// DESCRIPTION     :
// *****************************************************************************

#include "common_lib.h"
#include "MSH8_regmodel_offset.h"
#include "MSH8_driver.h"

void emmc_host_clk_config(u64 emmc_clkgen_baddr, u64 emmc_clkgen_offset, int emmc_clkgen_value){
    u64 addr;
    addr = emmc_clkgen_baddr + emmc_clkgen_offset;
    wr(addr, emmc_clkgen_value);
}

void emmc_host_rst_config(u64 emmc_rstgen_baddr, u64 emmc_rstgen_offset, int emmc_rstgen_value){
    u64 addr;
    addr = emmc_rstgen_baddr + emmc_rstgen_offset;
    wr(addr, emmc_rstgen_value);
}

void emmc_cmd_send(u64 emmc_msh8_baddr, int arg, int cmd){
    u64 addr;
    int rdata = 0x0;

    printf("*****Cmd sending....CMD%d \n",((cmd>>8) & 0x003f));

    //Set Argument
    addr = emmc_msh8_baddr + ARGUMENT_R_mshc_OFFSET ;
    wr(addr, arg);
    //Set Command
    addr = emmc_msh8_baddr + CMD_R_mshc_OFFSET ;
    wr16(addr, cmd);
    //Wait for command complete
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x1 != (rdata & 0x1)){
        rdata = rd16(addr)    ;
    }
    wr16(addr, 0x1);
    //Check CMD_TOUT_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x1 == (rdata & 0x1)){
        printf("*****CMD_TOUT_ERR!!!\n");
        wr16(addr, 0x1);
        finish(FAIL);
    }

    //Check CMD_CRC_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x2 == (rdata & 0x2)){
        printf("*****CMD_CRC_ERR!!!\n");
        wr16(addr, 0x2);
        finish(FAIL);
    }

    //Check CMD_END_BIT_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x4 == (rdata & 0x4)){
        printf("*****CMD_END_BIT_ERR!!!\n");
        wr16(addr, 0x4);
        finish(FAIL);
    }

    //Check CMD_IDX_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x8 == (rdata & 0x8)){
        printf("*****CMD_IDX_ERR!!!\n");
        wr16(addr, 0x8);
        finish(FAIL);
    }
}


void emmc_cmd_send_tuning_adma3(u64 emmc_msh8_baddr, int arg, int cmd){
    u64 addr;
    int rdata = 0x0;

    printf("*****Cmd sending....CMD%d \n",((cmd>>8) & 0x003f));

    //Set Argument
    addr = emmc_msh8_baddr + ARGUMENT_R_mshc_OFFSET ;
    wr(addr, arg);
    //Set Command
    addr = emmc_msh8_baddr + CMD_R_mshc_OFFSET ;
    wr16(addr, cmd);
    ////Wait for command complete
    //addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    //rdata = rd16(addr)     ;
    //while(0x1 != (rdata & 0x1)){
    //    rdata = rd16(addr)    ;
    //}
    //wr16(addr, 0x1);

    //Check CMD_TOUT_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x1 == (rdata & 0x1)){
        printf("*****CMD_TOUT_ERR!!!\n");
        wr16(addr, 0x1);
        finish(FAIL);
    }

    //Check CMD_CRC_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x2 == (rdata & 0x2)){
        printf("*****CMD_CRC_ERR!!!\n");
        wr16(addr, 0x2);
        finish(FAIL);
    }

    //Check CMD_END_BIT_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x4 == (rdata & 0x4)){
        printf("*****CMD_END_BIT_ERR!!!\n");
        wr16(addr, 0x4);
        finish(FAIL);
    }

    //Check CMD_IDX_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x8 == (rdata & 0x8)){
        printf("*****CMD_IDX_ERR!!!\n");
        wr16(addr, 0x8);
        finish(FAIL);
    }
}


uint32_t emmc_get_rsp(u64 emmc_msh8_baddr, int resp){
    u64 addr;
    int rdata = 0x0;

    switch(resp){
        case 0: addr =  emmc_msh8_baddr + RESP01_R_mshc_OFFSET ;
                rdata = rd(addr);
                return rdata;
                break;
        case 1: addr =  emmc_msh8_baddr + RESP23_R_mshc_OFFSET ;
                rdata = rd(addr);
                return rdata;
                break;
        case 2: addr =  emmc_msh8_baddr + RESP45_R_mshc_OFFSET ;
                rdata = rd(addr);
                return rdata;
                break;
        case 3: addr =  emmc_msh8_baddr + RESP67_R_mshc_OFFSET ;
                rdata = rd(addr);
                return rdata;
                break;
    }

    return 0;
}


void emmc_clk_change(u64 emmc_msh8_baddr, int clk_divisor){
    u64 addr ;
    int rdata = 0x0;

    //Stop SD/eMMC clock
    addr =  emmc_msh8_baddr + CLK_CTRL_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.u32 = CLK_CTRL_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.SD_CLK_EN = 0x0;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.INTERNAL_CLK_EN = 0x0;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.PLL_ENABLE = 0x0;
    wr16(addr , EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.u32);
    //Clock change
    addr =  emmc_msh8_baddr + CLK_CTRL_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.u32 = rd16(addr);
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.CLK_GEN_SELECT = 0x0;
    if(clk_divisor <= 510){
        EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.FREQ_SEL = clk_divisor/2;
    }
    else{
        EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.FREQ_SEL = ((clk_divisor/2)&0xff);
        EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.UPPER_FREQ_SEL = (((clk_divisor/2)&0x300)>>8);
        printf("FREQ_SEL=%0x\n", EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.FREQ_SEL);
        printf("UPPER_FREQ_SEL=%0x\n", EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.UPPER_FREQ_SEL);
    }
    wr16(addr , EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.u32);
    //Enable SD/eMMC clock
    addr =  emmc_msh8_baddr + CLK_CTRL_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.u32 = rd16(addr);
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.SD_CLK_EN = 0x1;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.INTERNAL_CLK_EN = 0x1;
    wr16(addr , EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.u32);
    //Check Clock Stability
    addr =  emmc_msh8_baddr + CLK_CTRL_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x2 != (rdata & 0x2)){
        rdata = rd16(addr)    ;
    }
    //Enable SD/eMMC clock
    addr =  emmc_msh8_baddr + CLK_CTRL_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.u32 = rd16(addr);
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.PLL_ENABLE = 0x1;

    printf("%s, %d\n", __func__, __LINE__);
    wr16(addr , EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.u32);
    printf("%s, %d\n", __func__, __LINE__);

    //Check Clock Stability
    addr =  emmc_msh8_baddr + CLK_CTRL_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x2 != (rdata & 0x2)){
        rdata = rd16(addr)    ;
    }
}


void emmc_msh8_base_config(u64 emmc_msh8_baddr){
    u64 addr ;

    //eMMC Bus Voltage Select
    addr = emmc_msh8_baddr + PWR_CTRL_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_pwr_ctrl_r.u32 = PWR_CTRL_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_pwr_ctrl_r.SD_BUS_VOL_VDD1 = 0x6;
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_pwr_ctrl_r.u32);
    //eMMC Bus Voltage Enable
    addr = emmc_msh8_baddr + PWR_CTRL_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_pwr_ctrl_r.u32 = rd8(addr);
    EMMC_REG.dwc_msh8_regmodel_pwr_ctrl_r.SD_BUS_PWR_VDD1 = 0x1;
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_pwr_ctrl_r.u32);
    //Set Data Timeout Counter Value
    addr = emmc_msh8_baddr + TOUT_CTRL_R_mshc_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_tout_ctrl_r.u32 = TOUT_CTRL_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_tout_ctrl_r.TOUT_CNT = 0x0;
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_tout_ctrl_r.u32);
    //Enable eMMC interface
    addr = emmc_msh8_baddr + HOST_CTRL2_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.u32 = HOST_CTRL2_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.PRESET_VAL_ENABLE = 0x0;
    EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.UHS2_IF_ENABLE = 0x0;
    EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.HOST_VER4_ENABLE = 0x1;
    EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.ASYNC_INT_ENABLE = 0x1;
    EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.CMD23_ENABLE = 0x1;
    EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.ADMA2_LEN_MODE = 0x1;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.u32);
    //Set eMMC Card Present Flag
    addr = emmc_msh8_baddr + EMMC_CTRL_R_mshc_vendor1_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_emmc_ctrl_r_vendor1.u32 = EMMC_CTRL_R_mshc_vendor1_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_emmc_ctrl_r_vendor1.CARD_IS_EMMC = 0x1;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_emmc_ctrl_r_vendor1.u32);
    //Set PHY General Configuration
    addr = emmc_msh8_baddr +  PHY_CNFG_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_phy_cnfg_phy.u32= PHY_CNFG_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_phy_cnfg_phy.PHY_RSTN= 0x1;
    wr(addr, EMMC_REG.dwc_msh8_regmodel_phy_cnfg_phy.u32);
    //Stop SD/eMMC clock
    addr =  emmc_msh8_baddr + CLK_CTRL_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.u32 = CLK_CTRL_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.SD_CLK_EN = 0x0;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.INTERNAL_CLK_EN = 0x0;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.PLL_ENABLE = 0x0;
    wr16(addr , EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.u32);
    //Set DL settings, SDCLK
    addr = emmc_msh8_baddr +  SDCLKDL_CNFG_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_sdclkdl_cnfg_phy.u32= SDCLKDL_CNFG_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_sdclkdl_cnfg_phy.UPDATE_DC= 0x1;
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_sdclkdl_cnfg_phy.u32);
    //Write DL code, SDCLK
    addr = emmc_msh8_baddr +  SDCLKDL_DC_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_sdclkdl_dc_phy.u32= SDCLKDL_DC_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_sdclkdl_dc_phy.CCKDL_DC= 0x40;
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_sdclkdl_dc_phy.u32);
    //Set DL settings, SDCLK
    addr = emmc_msh8_baddr +  SDCLKDL_CNFG_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_sdclkdl_cnfg_phy.u32= rd8(addr);
    EMMC_REG.dwc_msh8_regmodel_sdclkdl_cnfg_phy.UPDATE_DC= 0x0;
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_sdclkdl_cnfg_phy.u32);
    //Set PHY CMD Settings
    addr = emmc_msh8_baddr +  CMDPAD_CNFG_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_cmdpad_cnfg_phy.u32= CMDPAD_CNFG_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmdpad_cnfg_phy.RXSEL= 0x1;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_cmdpad_cnfg_phy.u32);
    //Set PHY DAT Settings
    addr = emmc_msh8_baddr +  DATPAD_CNFG_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_datpad_cnfg_phy.u32= DATPAD_CNFG_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_datpad_cnfg_phy.RXSEL= 0x1;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_datpad_cnfg_phy.u32);
    //Set PHY RST Settings
    addr = emmc_msh8_baddr +  RSTNPAD_CNFG_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_rstnpad_cnfg_phy.u32= RSTNPAD_CNFG_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_rstnpad_cnfg_phy.RXSEL= 0x1;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_rstnpad_cnfg_phy.u32);
    //Set PHY CLK Settings
    addr = emmc_msh8_baddr +  CLKPAD_CNFG_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_clkpad_cnfg_phy.u32= CLKPAD_CNFG_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_clkpad_cnfg_phy.RXSEL= 0x0;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_clkpad_cnfg_phy.u32);
    //Set PHY STROBE Settings
    addr = emmc_msh8_baddr +  STBPAD_CNFG_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_stbpad_cnfg_phy.u32= STBPAD_CNFG_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_stbpad_cnfg_phy.RXSEL= 0x1;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_stbpad_cnfg_phy.u32);
    //Enable DLL
    addr = emmc_msh8_baddr + DLL_CTRL_mshc_phy_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_dll_ctrl_phy.u32= DLL_CTRL_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_dll_ctrl_phy.DLL_EN= 0x1;
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_dll_ctrl_phy.u32);
    //Enable Normal Interrupt State
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_EN_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.u32 = NORMAL_INT_STAT_EN_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.CMD_COMPLETE_STAT_EN   = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.XFER_COMPLETE_STAT_EN  = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.BGAP_EVENT_STAT_EN     = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.DMA_INTERRUPT_STAT_EN  = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.BUF_WR_READY_STAT_EN   = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.BUF_RD_READY_STAT_EN   = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.CARD_INSERTION_STAT_EN = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.CARD_REMOVAL_STAT_EN   = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.CARD_INTERRUPT_STAT_EN = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.INT_A_STAT_EN          = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.INT_B_STAT_EN          = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.INT_C_STAT_EN          = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.RE_TUNE_EVENT_STAT_EN  = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.FX_EVENT_STAT_EN       = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.CQE_EVENT_STAT_EN      = 0x1;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_normal_int_stat_en_r.u32);
    //Enable Error Interrupt State
    addr = emmc_msh8_baddr + ERROR_INT_STAT_EN_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.u32 = ERROR_INT_STAT_EN_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.CMD_TOUT_ERR_STAT_EN      = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.CMD_CRC_ERR_STAT_EN       = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.CMD_END_BIT_ERR_STAT_EN   = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.CMD_IDX_ERR_STAT_EN       = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.DATA_TOUT_ERR_STAT_EN     = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.DATA_CRC_ERR_STAT_EN      = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.DATA_END_BIT_ERR_STAT_EN  = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.CUR_LMT_ERR_STAT_EN       = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.AUTO_CMD_ERR_STAT_EN      = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.ADMA_ERR_STAT_EN          = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.TUNING_ERR_STAT_EN        = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.RESP_ERR_STAT_EN          = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.BOOT_ACK_ERR_STAT_EN      = 0x1;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_error_int_stat_en_r.u32);
    //Enable Normal Interrupt Signal
    addr = emmc_msh8_baddr + NORMAL_INT_SIGNAL_EN_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.u32 = NORMAL_INT_SIGNAL_EN_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.CMD_COMPLETE_SIGNAL_EN    = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.XFER_COMPLETE_SIGNAL_EN   = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.BGAP_EVENT_SIGNAL_EN      = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.DMA_INTERRUPT_SIGNAL_EN   = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.BUF_WR_READY_SIGNAL_EN    = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.BUF_RD_READY_SIGNAL_EN    = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.CARD_INSERTION_SIGNAL_EN  = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.CARD_REMOVAL_SIGNAL_EN    = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.CARD_INTERRUPT_SIGNAL_EN  = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.INT_A_SIGNAL_EN           = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.INT_B_SIGNAL_EN           = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.INT_C_SIGNAL_EN           = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.RE_TUNE_EVENT_SIGNAL_EN   = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.FX_EVENT_SIGNAL_EN        = 0x1;
    EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.CQE_EVENT_SIGNAL_EN       = 0x1;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_normal_int_signal_en_r.u32);
    //Enable Error Interrupt Signal
    addr = emmc_msh8_baddr + ERROR_INT_SIGNAL_EN_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.u32 = ERROR_INT_SIGNAL_EN_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.CMD_TOUT_ERR_SIGNAL_EN      = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.CMD_CRC_ERR_SIGNAL_EN       = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.CMD_END_BIT_ERR_SIGNAL_EN   = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.CMD_IDX_ERR_SIGNAL_EN       = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.DATA_TOUT_ERR_SIGNAL_EN     = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.DATA_CRC_ERR_SIGNAL_EN      = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.DATA_END_BIT_ERR_SIGNAL_EN  = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.CUR_LMT_ERR_SIGNAL_EN       = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.AUTO_CMD_ERR_SIGNAL_EN      = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.ADMA_ERR_SIGNAL_EN          = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.TUNING_ERR_SIGNAL_EN        = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.RESP_ERR_SIGNAL_EN          = 0x1;
    EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.BOOT_ACK_ERR_SIGNAL_EN      = 0x1;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_error_int_signal_en_r.u32);
}


void emmc_bus_test(u64 emmc_msh8_baddr, int bus_width){

    int w_data;
    int w_data_8;
    int r_data;
    int r_data_8;
    int result;
    int result_8;
    int wdata = 0x0;
    int rdata = 0x0;
    u64 addr;

    //Disable Data CRC Check during Bus Test
    addr = emmc_msh8_baddr + EMMC_CTRL_R_mshc_vendor1_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_emmc_ctrl_r_vendor1.u32 = rd16(addr);
    EMMC_REG.dwc_msh8_regmodel_emmc_ctrl_r_vendor1.DISABLE_DATA_CRC_CHK = 0x1;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_emmc_ctrl_r_vendor1.u32);

    //Set BLKSIZ
    addr = emmc_msh8_baddr + BLOCKSIZE_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_blocksize_r.u32 = BLOCKSIZE_R_mshc_DFLT_VAL;
    switch(bus_width){
        case 1 : EMMC_REG.dwc_msh8_regmodel_blocksize_r.XFER_BLOCK_SIZE = 0x1;  break;
        case 4 :
        case 44: EMMC_REG.dwc_msh8_regmodel_blocksize_r.XFER_BLOCK_SIZE = 0x4;  break;
        case 8 :
        case 88: EMMC_REG.dwc_msh8_regmodel_blocksize_r.XFER_BLOCK_SIZE = 0x8;  break;
    }
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_blocksize_r.u32);

    //CMD19
    addr = emmc_msh8_baddr + XFER_MODE_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32 = XFER_MODE_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DMA_ENABLE           = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.BLOCK_COUNT_ENABLE   = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.AUTO_CMD_ENABLE      = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DATA_XFER_DIR        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.MULTI_BLK_SEL        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_ERR_CHK_ENABLE  = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_INT_DISABLE     = 0x0 ;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32);
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x13 ;
    emmc_cmd_send(emmc_msh8_baddr, 0x0,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    emmc_get_rsp(emmc_msh8_baddr, 0);

    //Wait for BUF_WR_READY
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x10 != (rdata & 0x10)){
        rdata = rd16(addr)    ;
    }
    wr16(addr, 0x10);

    addr = emmc_msh8_baddr+BUF_DATA_R_mshc_OFFSET;
    switch(bus_width){
        case 1 : wdata = 0x80;
                 wr(addr, wdata);
                 break;
        case 4 :
        case 44: wdata = 0x0000005a;
                 wr(addr, wdata);
                 break;
        case 8 :
        case 88: wdata = 0x0000aa55;
                 wr(addr, wdata);
                 wdata = 0x00000000;
                 wr(addr, wdata);
                 break;
    }

    //Wait for transfer complete
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x2 != (rdata & 0x2)){
        rdata = rd16(addr)    ;
    }
    wr16(addr, 0x2);

    //CMD14: Bus testing
    addr = emmc_msh8_baddr + XFER_MODE_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32 = XFER_MODE_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DMA_ENABLE           = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.BLOCK_COUNT_ENABLE   = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.AUTO_CMD_ENABLE      = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DATA_XFER_DIR        = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.MULTI_BLK_SEL        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_ERR_CHK_ENABLE  = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_INT_DISABLE     = 0x0 ;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32);
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0xe ;
    emmc_cmd_send(emmc_msh8_baddr, 0x0,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    emmc_get_rsp(emmc_msh8_baddr, 0);

    //Wait for BUF_RD_READY
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x20 != (rdata & 0x20)){
        rdata = rd16(addr)    ;
    }
    wr16(addr, 0x20);

    switch(bus_width){
        case 1 : w_data = 0x80;
                 r_data = rd(emmc_msh8_baddr+BUF_DATA_R_mshc_OFFSET);
                 result = ~ (w_data ^ r_data);
                 if((result & 0xc0) != 0x0){
                     printf("*****Bus Test FAIL!!!\n");
                     finish(FAIL);
                 }
                 break;
        case 4 :
        case 44: w_data = 0x0000005a;
                 r_data = rd(emmc_msh8_baddr+BUF_DATA_R_mshc_OFFSET);
                 result = ~ (w_data ^ r_data);
                 if((result & 0x000000ff) != 0x0){
                     printf("*****Bus Test FAIL!!!\n");
                     finish(FAIL);
                 }
                 break;
        case 8 :
        case 88: w_data = 0x0000aa55;
                 r_data = rd(emmc_msh8_baddr+BUF_DATA_R_mshc_OFFSET);
                 result = ~ (w_data ^ r_data);
                 w_data_8 = 0x00000000;
                 r_data_8 = rd(emmc_msh8_baddr+BUF_DATA_R_mshc_OFFSET);
                 result_8 = ~ (w_data_8 ^ r_data_8);
                 if(((result & 0x0000ffff) != 0x0) || ((result_8 & 0x00000000) != 0x0)){
                     printf("*****Bus Test FAIL!!!\n");
                     finish(FAIL);
                 }
                 break;
    }

    //Wait for transfer complete
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x2 != (rdata & 0x2)){
        rdata = rd16(addr)    ;
    }
    wr16(addr, 0x2);

    //Enable Data CRC Check during Bus Test
    addr = emmc_msh8_baddr + EMMC_CTRL_R_mshc_vendor1_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_emmc_ctrl_r_vendor1.u32 = rd16(addr);
    EMMC_REG.dwc_msh8_regmodel_emmc_ctrl_r_vendor1.DISABLE_DATA_CRC_CHK = 0x0;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_emmc_ctrl_r_vendor1.u32);

    //Clear All Interrupts
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    wr16(addr, 0x40ff);
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    wr16(addr, 0x1fff);
}


void emmc_auto_tuning(u64 emmc_msh8_baddr, int bus_width){
    int rdata = 0x0;
    u64 addr;

    //Set ATDL_CNFG
    addr = emmc_msh8_baddr +  ATDL_CNFG_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_atdl_cnfg_phy.u32= ATDL_CNFG_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_atdl_cnfg_phy.INPSEL_CNFG= 0x3;
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_atdl_cnfg_phy.u32);

    //Change AT Default Settings
    addr = emmc_msh8_baddr + AT_CTRL_R_mshc_vendor1_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_at_ctrl_r_vendor1.u32= AT_CTRL_R_mshc_vendor1_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_at_ctrl_r_vendor1.CI_SEL=0x0 ;
    EMMC_REG.dwc_msh8_regmodel_at_ctrl_r_vendor1.SWIN_TH_EN=0x1 ;
    EMMC_REG.dwc_msh8_regmodel_at_ctrl_r_vendor1.RPT_TUNE_ERR=0x0 ;
    EMMC_REG.dwc_msh8_regmodel_at_ctrl_r_vendor1.SW_TUNE_EN=0x0 ;
    EMMC_REG.dwc_msh8_regmodel_at_ctrl_r_vendor1.WIN_EDGE_SEL=0x0 ;
    EMMC_REG.dwc_msh8_regmodel_at_ctrl_r_vendor1.TUNE_CLK_STOP_EN=0x1 ;
    EMMC_REG.dwc_msh8_regmodel_at_ctrl_r_vendor1.PRE_CHANGE_DLY=0x1 ;
    EMMC_REG.dwc_msh8_regmodel_at_ctrl_r_vendor1.POST_CHANGE_DLY=0x3 ;
    EMMC_REG.dwc_msh8_regmodel_at_ctrl_r_vendor1.SWIN_TH_VAL=0x9 ;
    wr(addr, EMMC_REG.dwc_msh8_regmodel_at_ctrl_r_vendor1.u32);

    //Check AT_EN
    addr = emmc_msh8_baddr + AT_CTRL_R_mshc_vendor1_OFFSET ;
    rdata = rd(addr)     ;
    if(0x1 != (rdata & 0x1)){
        printf("*****Auto Tuning is NOT Enable!!!\n");
        finish(FAIL);
    }

    //Start Tuning
    addr = emmc_msh8_baddr + HOST_CTRL2_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.u32= rd16(addr);
    EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.EXEC_TUNING= 0x1;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.u32);

    //Set BLKSIZ
    addr = emmc_msh8_baddr + BLOCKSIZE_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_blocksize_r.u32 = BLOCKSIZE_R_mshc_DFLT_VAL;
    switch(bus_width){
        case 4 : EMMC_REG.dwc_msh8_regmodel_blocksize_r.XFER_BLOCK_SIZE = 0x40;  break;
        case 8 :
        case 88: EMMC_REG.dwc_msh8_regmodel_blocksize_r.XFER_BLOCK_SIZE = 0x80;  break;
    }
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_blocksize_r.u32);

    //CMD21: eMMC Card Tuning
    addr = emmc_msh8_baddr + HOST_CTRL2_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x40 == (rdata & 0x40)){
        addr = emmc_msh8_baddr + XFER_MODE_R_mshc_OFFSET ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32 = XFER_MODE_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DMA_ENABLE           = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.BLOCK_COUNT_ENABLE   = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.AUTO_CMD_ENABLE      = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DATA_XFER_DIR        = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.MULTI_BLK_SEL        = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_TYPE            = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_ERR_CHK_ENABLE  = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_INT_DISABLE     = 0x0 ;
        wr16(addr, EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32);
        EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x15;
        emmc_cmd_send_tuning_adma3(emmc_msh8_baddr, 0x0,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
        emmc_get_rsp(emmc_msh8_baddr, 0);

        //Wait for BUF_RD_READY
        addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
        rdata = rd16(addr)     ;
        while(0x20 != (rdata & 0x20)){
            rdata = rd16(addr)    ;
        }
        wr16(addr, 0x20);

        addr = emmc_msh8_baddr + HOST_CTRL2_R_mshc_OFFSET ;
        rdata = rd16(addr)     ;
    }

    //Check SAMPLE_CLK_SEL
    addr = emmc_msh8_baddr + HOST_CTRL2_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x80 != (rdata & 0x80)){
        printf("*****Tuning FAIL!!!\n");
        finish(FAIL);
    }

    //Clear All Interrupts
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    wr16(addr, 0x40ff);
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    wr16(addr, 0x1fff);
}


void emmc_data_bus_width(u64 emmc_msh8_baddr, int bus_width, int enhanced_strobe_en){
    int resp01;
    u64 addr;

    //Set Bus Width for DWC_mshc
    addr = emmc_msh8_baddr + HOST_CTRL1_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.u32= HOST_CTRL1_R_mshc_DFLT_VAL;
    switch(bus_width){
        case 1:  EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.EXT_DAT_XFER= 0x0;
                 EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.DAT_XFER_WIDTH= 0x0;
                 break;  //1bit
        case 4:
        case 44: EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.EXT_DAT_XFER= 0x0;
                 EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.DAT_XFER_WIDTH= 0x1;
                 break;  //4bit
        case 8:
        case 88: EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.EXT_DAT_XFER= 0x1;
                 EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.DAT_XFER_WIDTH= 0x0;
                 break;  //8bit
    }
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.u32);

    //CMD6:Set Bus Width for eMMC Card
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x3 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x6 ;
    if(enhanced_strobe_en == 0x1){
        switch(bus_width){
            case 88: emmc_cmd_send(emmc_msh8_baddr, 0x03b78600,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;
            default: printf("Bus width must be '88' when enhanced_strobe_en=1\n"); finish(FAIL);      break;
        }
    } else{
        switch(bus_width){
            case 1:  emmc_cmd_send(emmc_msh8_baddr, 0x03b70000,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;
            case 4:  emmc_cmd_send(emmc_msh8_baddr, 0x03b70100,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;
            case 8:  emmc_cmd_send(emmc_msh8_baddr, 0x03b70200,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;
            case 44: emmc_cmd_send(emmc_msh8_baddr, 0x03b70500,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;
            case 88: emmc_cmd_send(emmc_msh8_baddr, 0x03b70600,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;
        }
    }
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    if(resp01 != 0x900){
        printf("*****Resp for CMD6 is %x.\n", resp01);
       // finish(FAIL);
    }

    //CMD13:Check Switch Error
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0xd ;
    emmc_cmd_send(emmc_msh8_baddr, EMMC_RCA_ARG,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    if(0x80 == (resp01 & 0x80)){
        printf("*****Switch Error!!!\n");
        finish(FAIL);
    }
}


void emmc_data_speed_mode(u64 emmc_msh8_baddr, int speed_mode, int enhanced_strobe_en){
    int resp01;
    u64 addr;

    //Set Speed Mode for DWC_mshc
    addr = emmc_msh8_baddr + HOST_CTRL2_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.u32= rd16(addr);
    switch(speed_mode){
        case 0: EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.UHS_MODE_SEL= 0x0; break;  //Legacy
        case 1: EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.UHS_MODE_SEL= 0x1; break;  //HS SDR
        case 2: EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.UHS_MODE_SEL= 0x4; break;  //HS DDR
        case 3: EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.UHS_MODE_SEL= 0x3; break;  //HS200
        case 4: EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.UHS_MODE_SEL= 0x7; break;  //HS400
    }
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_host_ctrl2_r.u32);

    //CMD6:Set Speed Mode for eMMC Card
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x3 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x6 ;
    switch(speed_mode){
        case 0: emmc_cmd_send(emmc_msh8_baddr, 0x03b90000,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;  //Legacy
        case 1:                                                                                          //HS
        case 2: emmc_cmd_send(emmc_msh8_baddr, 0x03b90100,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;  //HS
        case 3: emmc_cmd_send(emmc_msh8_baddr, 0x03b90200,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;  //HS200
        case 4: emmc_cmd_send(emmc_msh8_baddr, 0x03b90300,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;  //HS400
    }
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    if(resp01 != 0x900){
        printf("*****Resp for CMD6 is %x.\n", resp01);
    //    finish(FAIL);
    }

    //Set Enhanced Data Strobe for DWC_mshc
    if(enhanced_strobe_en == 0x1){
        addr = emmc_msh8_baddr +  EMMC_CTRL_R_mshc_vendor1_OFFSET;
        EMMC_REG.dwc_msh8_regmodel_emmc_ctrl_r_vendor1.u32= rd16(addr);
        EMMC_REG.dwc_msh8_regmodel_emmc_ctrl_r_vendor1.ENH_STROBE_ENABLE= 0x1;
        wr16(addr, EMMC_REG.dwc_msh8_regmodel_emmc_ctrl_r_vendor1.u32);
    }

    //CMD13:Check Switch Error
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0xd ;
    emmc_cmd_send(emmc_msh8_baddr, EMMC_RCA_ARG,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    if(0x80 == (resp01 & 0x80)){
        printf("*****Switch Error!!!\n");
        finish(FAIL);
    }
}


void emmc_partition_select(u64 emmc_msh8_baddr, int partition_sel){
    int resp01;

    //CMD6:Select partition of eMMC Card
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x3 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x6 ;
    switch(partition_sel){
        case 0: emmc_cmd_send(emmc_msh8_baddr, 0x03b30000,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;  //User Area
        case 1: emmc_cmd_send(emmc_msh8_baddr, 0x03b30100,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;  //Boot Partition 1
        case 2: emmc_cmd_send(emmc_msh8_baddr, 0x03b30200,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32); break;  //Boot Partition 2
    }
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    if(resp01 != 0x900){
        printf("error *****Resp for CMD6 is %x.\n", resp01);
        finish(FAIL);
    }

    //CMD13:Check Switch Error
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0xd ;
    emmc_cmd_send(emmc_msh8_baddr, EMMC_RCA_ARG,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    if(0x80 == (resp01 & 0x80)){
        printf("*****Switch Error!!!\n");
        finish(FAIL);
    }
}


void emmc_hs400(u64 emmc_msh8_baddr, int enhanced_strobe_en){
    u64 addr;

    //Switch to High Speed Mode
    emmc_data_speed_mode(emmc_msh8_baddr, 1, 0x0);

    //Frequence < 52MHz
    emmc_clk_change(emmc_msh8_baddr, 4);

    //Set DL settings, Data Strobe
    addr = emmc_msh8_baddr + DLLDL_CNFG_mshc_phy_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_dlldl_cnfg_phy.u32= DLLDL_CNFG_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_dlldl_cnfg_phy.SLV_INPSEL= 0x3;
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_dlldl_cnfg_phy.u32);

    //Switch to DDR 8bit Mode and Enable/Disable Enhanced Data Strobe
    emmc_data_bus_width(emmc_msh8_baddr, 88, enhanced_strobe_en);

    //Switch to HS400 Mode
    emmc_data_speed_mode(emmc_msh8_baddr, 4, enhanced_strobe_en);

    //Stop SD/eMMC clock
    addr =  emmc_msh8_baddr + CLK_CTRL_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.u32 = CLK_CTRL_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.SD_CLK_EN = 0x0;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.INTERNAL_CLK_EN = 0x0;
    EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.PLL_ENABLE = 0x0;
    wr16(addr , EMMC_REG.dwc_msh8_regmodel_clk_ctrl_r.u32);
    //Set DL settings, SDCLK
    addr = emmc_msh8_baddr +  SDCLKDL_CNFG_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_sdclkdl_cnfg_phy.u32= SDCLKDL_CNFG_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_sdclkdl_cnfg_phy.UPDATE_DC= 0x1;
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_sdclkdl_cnfg_phy.u32);
    //Write DL code, SDCLK
    addr = emmc_msh8_baddr +  SDCLKDL_DC_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_sdclkdl_dc_phy.u32= SDCLKDL_DC_mshc_phy_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_sdclkdl_dc_phy.CCKDL_DC= 0x20;
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_sdclkdl_dc_phy.u32);
    //Set DL settings, SDCLK
    addr = emmc_msh8_baddr +  SDCLKDL_CNFG_mshc_phy_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_sdclkdl_cnfg_phy.u32= rd8(addr);
    EMMC_REG.dwc_msh8_regmodel_sdclkdl_cnfg_phy.UPDATE_DC= 0x0;
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_sdclkdl_cnfg_phy.u32);

    //Frequence <= 200MHz
    emmc_clk_change(emmc_msh8_baddr, 0);
}


void emmc_initialization(u64 emmc_msh8_baddr){
    int resp01;

    //Frequence < 400KHz
    emmc_clk_change(emmc_msh8_baddr, 125);

    //CMD0: Reset Card
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x0 ;
    emmc_cmd_send(emmc_msh8_baddr, 0x0,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);

    //CMD1: Get OCR
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x1 ;
    emmc_cmd_send(emmc_msh8_baddr, 0xc0ff8080,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    while(0x80000000 != (resp01 & 0x80000000)){
        emmc_cmd_send(emmc_msh8_baddr, 0xc0ff8080,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
        resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    }
    if(resp01 != 0xc0ff8080){
        printf("*****Resp for CMD1 is %x.\n", resp01);
        finish(FAIL);
    }

    //CMD2: Get CID
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x2 ;
    emmc_cmd_send(emmc_msh8_baddr, 0x0,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    emmc_get_rsp(emmc_msh8_baddr, 1);
    emmc_get_rsp(emmc_msh8_baddr, 2);
    emmc_get_rsp(emmc_msh8_baddr, 3);

    //CMD3:RCA
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x3 ;
    emmc_cmd_send(emmc_msh8_baddr, EMMC_RCA_ARG,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    if(resp01 != 0x500){
        printf("*****Resp for CMD3 is %x.\n", resp01);
        finish(FAIL);
    }

    //CMD7:Select Card
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x7 ;
    emmc_cmd_send(emmc_msh8_baddr, EMMC_RCA_ARG,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    if(resp01 != 0x700){
        printf("*****Resp for CMD7 is %x.\n", resp01);
        finish(FAIL);
    }
}


void emmc_write_data(u64 emmc_msh8_baddr, int blksize, int blkcount, int waddr, int daddr, int dma_type, int is_ddr, int write_cmd){
    int rdata = 0x0;
    int resp01;
    u64 addr;

    //Clear All Interrupts
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    wr16(addr, 0x40ff);
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    wr16(addr, 0x1fff);

    //Set Block Size for DWC_mshc
    addr = emmc_msh8_baddr + BLOCKSIZE_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_blocksize_r.u32= BLOCKSIZE_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_blocksize_r.XFER_BLOCK_SIZE= blksize;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_blocksize_r.u32);

    //Set Block Count for DWC_mshc
    if(write_cmd == 0x19){
        addr = emmc_msh8_baddr + BLOCKCOUNT_R_mshc_OFFSET ;
        EMMC_REG.dwc_msh8_regmodel_blockcount_r.u32= BLOCKCOUNT_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_blockcount_r.BLOCK_CNT= blkcount;
        wr16(addr, EMMC_REG.dwc_msh8_regmodel_blockcount_r.u32);
    }

    //Select DMA Mode
    addr = emmc_msh8_baddr + HOST_CTRL1_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.u32= rd8(addr);
    EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.DMA_SEL= 0x3;  //ADMA2 or ADMA3 is selected
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.u32);

    //Set Descriptor Address
    addr = emmc_msh8_baddr +  ADMA_SA_LOW_R_mshc_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_adma_sa_low_r.u32= ADMA_SA_LOW_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_adma_sa_low_r.ADMA_SA_LOW= daddr;
    wr(addr, EMMC_REG.dwc_msh8_regmodel_adma_sa_low_r.u32);

    //CMD16:Set Block Size for eMMC Card
    if(is_ddr == 0x0){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x10;
        emmc_cmd_send(emmc_msh8_baddr, (blksize & 0xfff),EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
        resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
        if(resp01 != 0x900){
            printf("*****Resp for CMD16 is %x.\n", resp01);
            finish(FAIL);
        }
    }

    //CMD23:Set Block Count for eMMC Card
    if(write_cmd == 0x19){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x17;
        emmc_cmd_send(emmc_msh8_baddr, blkcount,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
        resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
        if(resp01 != 0x900){
            printf("*****Resp for CMD23 is %x.\n", resp01);
            finish(FAIL);
        }
    }

    //CMD24: Single Block Write
    //CMD25: Multiple Block Write
    addr = emmc_msh8_baddr + XFER_MODE_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32 = XFER_MODE_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DMA_ENABLE                = 0x1 ;
    if(write_cmd == 0x18){
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.BLOCK_COUNT_ENABLE    = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.MULTI_BLK_SEL         = 0x0 ;
    }
    else if(write_cmd == 0x19){
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.BLOCK_COUNT_ENABLE    = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.MULTI_BLK_SEL         = 0x1 ;
    }
    else{
        printf("*****Invalid Write Command!!!\n");
        finish(FAIL);
    }
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.AUTO_CMD_ENABLE           = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DATA_XFER_DIR             = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_TYPE                 = 0x0 ;
    if(dma_type == 0x2){
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_ERR_CHK_ENABLE   = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_INT_DISABLE      = 0x0 ;
    }
    else if(dma_type == 0x3){
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_ERR_CHK_ENABLE   = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_INT_DISABLE      = 0x1 ;
    }
    else{
        printf("*****Invalid DMA Type!!!\n");
        finish(FAIL);
    }
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32);
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT        = 0x2 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE      = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE      = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL        = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE                = 0x0 ;
    if(write_cmd == 0x18){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x18;
    }
    else if(write_cmd == 0x19){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x19;
    }
    else{
        printf("*****Invalid Write Command!!!\n");
        finish(FAIL);
    }
    if(dma_type == 0x2){
        emmc_cmd_send(emmc_msh8_baddr, waddr,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    }
    else if(dma_type == 0x3){
        emmc_cmd_send_tuning_adma3(emmc_msh8_baddr, waddr,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    }
    else{
        printf("*****Invalid DMA Type!!!\n");
        finish(FAIL);
    }
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    if(resp01 != 0x900){
        printf("*****Resp for CMD24/CMD25 is %x.\n", resp01);
        finish(FAIL);
    }

    //Wait for transfer complete
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x2 != (rdata & 0x2)){
        rdata = rd16(addr)    ;
    }
    wr16(addr, 0x2);

    //Check ADMA_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x200 == (rdata & 0x200)){
        printf("*****ADMA_ERR!!!\n");
        wr16(addr, 0x200);
        finish(FAIL);
    }

    //Check RESP_ERR
    if(dma_type == 0x3){
        addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
        rdata = rd16(addr)     ;
        if(0x800 == (rdata & 0x800)){
            printf("*****RESP_ERR!!!\n");
            wr16(addr, 0x800);
            finish(FAIL);
        }
    }

    //Check DATA_TOUT_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x10 == (rdata & 0x10)){
        printf("*****DATA_TOUT_ERR!!!\n");
        wr16(addr, 0x10);
        finish(FAIL);
    }

    //Check DATA_CRC_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x20 == (rdata & 0x20)){
        printf("*****DATA_CRC_ERR!!!\n");
        wr16(addr, 0x20);
        finish(FAIL);
    }

    //Check DATA_END_BIT_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x40 == (rdata & 0x40)){
        printf("*****DATA_END_BIT_ERR!!!\n");
        wr16(addr, 0x40);
        finish(FAIL);
    }
}


void emmc_read_data(u64 emmc_msh8_baddr, int blksize, int blkcount, int raddr, int daddr, int dma_type, int is_ddr, int read_cmd){
    int rdata = 0x0;
    int resp01;
    u64 addr;

    //Clear All Interrupts
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    wr16(addr, 0x40ff);
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    wr16(addr, 0x1fff);

    //Set Block Size for DWC_mshc
    addr = emmc_msh8_baddr + BLOCKSIZE_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_blocksize_r.u32= BLOCKSIZE_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_blocksize_r.XFER_BLOCK_SIZE= blksize;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_blocksize_r.u32);

    //Set Block Count for DWC_mshc
    if(read_cmd == 0x12){
        addr = emmc_msh8_baddr + BLOCKCOUNT_R_mshc_OFFSET ;
        EMMC_REG.dwc_msh8_regmodel_blockcount_r.u32= BLOCKCOUNT_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_blockcount_r.BLOCK_CNT= blkcount;
        wr16(addr, EMMC_REG.dwc_msh8_regmodel_blockcount_r.u32);
    }

    //Select DMA Mode
    addr = emmc_msh8_baddr + HOST_CTRL1_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.u32= rd8(addr);
    EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.DMA_SEL= 0x3;  //ADMA2 or ADMA3 is selected
    wr8(addr, EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.u32);

    //Set Descriptor Address
    addr = emmc_msh8_baddr +  ADMA_SA_LOW_R_mshc_OFFSET;
    EMMC_REG.dwc_msh8_regmodel_adma_sa_low_r.u32= ADMA_SA_LOW_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_adma_sa_low_r.ADMA_SA_LOW= daddr;
    wr(addr, EMMC_REG.dwc_msh8_regmodel_adma_sa_low_r.u32);

    //CMD16:Set Block Size for eMMC Card
    if(is_ddr == 0x0){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x10;
        emmc_cmd_send(emmc_msh8_baddr, (blksize & 0xfff),EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
        resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
        if(resp01 != 0x900){
            printf("*****Resp for CMD16 is %x.\n", resp01);
            finish(FAIL);
        }
    }

    //CMD23:Set Block Count for eMMC Card
    if(read_cmd == 0x12){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x17;
        emmc_cmd_send(emmc_msh8_baddr, blkcount,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
        resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
        if(resp01 != 0x900){
            printf("*****Resp for CMD23 is %x.\n", resp01);
            finish(FAIL);
        }
    }

    //CMD17: Single Block Read
    //CMD18: Multiple Block Read
    addr = emmc_msh8_baddr + XFER_MODE_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32 = XFER_MODE_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DMA_ENABLE                = 0x1 ;
    if(read_cmd == 0x11){
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.BLOCK_COUNT_ENABLE    = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.MULTI_BLK_SEL         = 0x0 ;
    }
    else if(read_cmd == 0x12){
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.BLOCK_COUNT_ENABLE    = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.MULTI_BLK_SEL         = 0x1 ;
    }
    else{
        printf("*****Invalid Write Command!!!\n");
        finish(FAIL);
    }
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.AUTO_CMD_ENABLE           = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DATA_XFER_DIR             = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_TYPE                 = 0x0 ;
    if(dma_type == 0x2){
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_ERR_CHK_ENABLE   = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_INT_DISABLE      = 0x0 ;
    }
    else if(dma_type == 0x3){
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_ERR_CHK_ENABLE   = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_INT_DISABLE      = 0x1 ;
    }
    else{
        printf("*****Invalid DMA Type!!!\n");
        finish(FAIL);
    }
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32);
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT        = 0x2 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE      = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE      = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL        = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE                = 0x0 ;
    if(read_cmd == 0x11){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x11;
    }
    else if(read_cmd == 0x12){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x12;
    }
    else{
        printf("*****Invalid Write Command!!!\n");
        finish(FAIL);
    }
    if(dma_type == 0x2){
        emmc_cmd_send(emmc_msh8_baddr, raddr,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    }
    else if(dma_type == 0x3){
        emmc_cmd_send_tuning_adma3(emmc_msh8_baddr, raddr,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    }
    else{
        printf("*****Invalid DMA Type!!!\n");
        finish(FAIL);
    }
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    if(resp01 != 0x900){
        printf("*****Resp for CMD24/CMD25 is %x.\n", resp01);
        finish(FAIL);
    }

    //Wait for transfer complete
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x2 != (rdata & 0x2)){
        rdata = rd16(addr)    ;
    }
    wr16(addr, 0x2);

    //Check ADMA_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x200 == (rdata & 0x200)){
        printf("*****ADMA_ERR!!!\n");
        wr16(addr, 0x200);
        finish(FAIL);
    }

    //Check RESP_ERR
    if(dma_type == 0x3){
        addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
        rdata = rd16(addr)     ;
        if(0x800 == (rdata & 0x800)){
            printf("*****RESP_ERR!!!\n");
            wr16(addr, 0x800);
            finish(FAIL);
        }
    }

    //Check DATA_TOUT_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x10 == (rdata & 0x10)){
        printf("*****DATA_TOUT_ERR!!!\n");
        wr16(addr, 0x10);
        finish(FAIL);
    }

    //Check DATA_CRC_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x20 == (rdata & 0x20)){
        printf("*****DATA_CRC_ERR!!!\n");
        wr16(addr, 0x20);
        finish(FAIL);
    }

    //Check DATA_END_BIT_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x40 == (rdata & 0x40)){
        printf("*****DATA_END_BIT_ERR!!!\n");
        wr16(addr, 0x40);
        finish(FAIL);
    }
}


void emmc_write_data_nonDMA(u64 emmc_msh8_baddr, int blksize, int blkcount, int waddr, int is_ddr, int source_addr, int write_cmd){
    int rdata = 0x0;
    int resp01;
    u64 addr;
    u64 addr1;
    u64 pkt_buf_addr;
    int i;
    int j;

    //Clear All Interrupts
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    wr16(addr, 0x40ff);
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    wr16(addr, 0x1fff);

    //Set Block Size for DWC_mshc
    addr = emmc_msh8_baddr + BLOCKSIZE_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_blocksize_r.u32= BLOCKSIZE_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_blocksize_r.XFER_BLOCK_SIZE= blksize;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_blocksize_r.u32);

    //Set Block Count for DWC_mshc
    if(write_cmd == 0x19){
        addr = emmc_msh8_baddr + BLOCKCOUNT_R_mshc_OFFSET ;
        EMMC_REG.dwc_msh8_regmodel_blockcount_r.u32= BLOCKCOUNT_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_blockcount_r.BLOCK_CNT= blkcount;
        wr16(addr, EMMC_REG.dwc_msh8_regmodel_blockcount_r.u32);
    }

    ////Select DMA Mode
    //addr = emmc_msh8_baddr + HOST_CTRL1_R_mshc_OFFSET ;
    //EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.u32= rd8(addr);
    //EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.DMA_SEL= 0x3;  //ADMA2 or ADMA3 is selected
    //wr8(addr, EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.u32);

    ////Set Descriptor Address
    //addr = emmc_msh8_baddr +  ADMA_SA_LOW_R_mshc_OFFSET;
    //EMMC_REG.dwc_msh8_regmodel_adma_sa_low_r.u32= ADMA_SA_LOW_R_mshc_DFLT_VAL;
    //EMMC_REG.dwc_msh8_regmodel_adma_sa_low_r.ADMA_SA_LOW= daddr;
    //wr(addr, EMMC_REG.dwc_msh8_regmodel_adma_sa_low_r.u32);

    //CMD16:Set Block Size for eMMC Card
    if(is_ddr == 0x0){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x10;
        emmc_cmd_send(emmc_msh8_baddr, (blksize & 0xfff),EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
        resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
        if(resp01 != 0x900){
            printf("*****Resp for CMD16 is %x.\n", resp01);
            finish(FAIL);
        }
    }

    //CMD23:Set Block Count for eMMC Card
    if(write_cmd == 0x19){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x17;
        emmc_cmd_send(emmc_msh8_baddr, blkcount,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
        resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
        if(resp01 != 0x900){
            printf("*****Resp for CMD23 is %x.\n", resp01);
            finish(FAIL);
        }
    }

    //CMD24: Single Block Write
    //CMD25: Multiple Block Write
    addr = emmc_msh8_baddr + XFER_MODE_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32 = XFER_MODE_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DMA_ENABLE                = 0x0 ;
    if(write_cmd == 0x18){
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.BLOCK_COUNT_ENABLE    = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.MULTI_BLK_SEL         = 0x0 ;
    }
    else if(write_cmd == 0x19){
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.BLOCK_COUNT_ENABLE    = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.MULTI_BLK_SEL         = 0x1 ;
    }
    else{
        printf("*****Invalid Write Command!!!\n");
        finish(FAIL);
    }
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.AUTO_CMD_ENABLE           = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DATA_XFER_DIR             = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_TYPE                 = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_ERR_CHK_ENABLE       = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_INT_DISABLE          = 0x0 ;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32);
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT        = 0x2 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE      = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE      = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL        = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE                = 0x0 ;
    if(write_cmd == 0x18){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x18;
    }
    else if(write_cmd == 0x19){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x19;
    }
    else{
        printf("*****Invalid Write Command!!!\n");
        finish(FAIL);
    }
    emmc_cmd_send(emmc_msh8_baddr, waddr,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    if(resp01 != 0x900){
        printf("*****Resp for CMD24/CMD25 is %x.\n", resp01);
        finish(FAIL);
    }

    //Wait for BUF_WR_READY
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x10 != (rdata & 0x10)){
        rdata = rd16(addr)    ;
    }
    wr16(addr, 0x10);

    pkt_buf_addr = emmc_msh8_baddr+BUF_DATA_R_mshc_OFFSET;
    addr = source_addr;
    for(j = 0; j < blkcount; j++){
        //Wait for BUF_WR_ENABLE
        addr1 = emmc_msh8_baddr + PSTATE_REG_mshc_OFFSET ;
        rdata = rd(addr1)     ;
        rdata = rd(addr1)     ;
        while(0x400 != (rdata & 0x400)){
            rdata = rd(addr1)     ;
        }
        for(i = 0; i < 128; i++){
            rdata = rd(addr);
            wr(pkt_buf_addr, rdata);
            addr = addr + 4;
        }
    }

    //Wait for transfer complete
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x2 != (rdata & 0x2)){
        rdata = rd16(addr)    ;
    }
    wr16(addr, 0x2);

    //Check DATA_TOUT_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x10 == (rdata & 0x10)){
        printf("*****DATA_TOUT_ERR!!!\n");
        wr16(addr, 0x10);
        finish(FAIL);
    }

    //Check DATA_CRC_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x20 == (rdata & 0x20)){
        printf("*****DATA_CRC_ERR!!!\n");
        wr16(addr, 0x20);
        finish(FAIL);
    }

    //Check DATA_END_BIT_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x40 == (rdata & 0x40)){
        printf("*****DATA_END_BIT_ERR!!!\n");
        wr16(addr, 0x40);
        finish(FAIL);
    }
}


void emmc_read_data_nonDMA(u64 emmc_msh8_baddr, int blksize, int blkcount, int raddr, int is_ddr, int destination_addr, int read_cmd){
    int rdata = 0x0;
    int resp01;
    u64 addr;
    u64 addr1;
    u64 pkt_buf_addr;
    int i;
    int j;

    //Clear All Interrupts
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    wr16(addr, 0x40ff);
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    wr16(addr, 0x1fff);

    //Set Block Size for DWC_mshc
    addr = emmc_msh8_baddr + BLOCKSIZE_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_blocksize_r.u32= BLOCKSIZE_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_blocksize_r.XFER_BLOCK_SIZE= blksize;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_blocksize_r.u32);

    //Set Block Count for DWC_mshc
    if(read_cmd == 0x12){
        addr = emmc_msh8_baddr + BLOCKCOUNT_R_mshc_OFFSET ;
        EMMC_REG.dwc_msh8_regmodel_blockcount_r.u32= BLOCKCOUNT_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_blockcount_r.BLOCK_CNT= blkcount;
        wr16(addr, EMMC_REG.dwc_msh8_regmodel_blockcount_r.u32);
    }

    ////Select DMA Mode
    //addr = emmc_msh8_baddr + HOST_CTRL1_R_mshc_OFFSET ;
    //EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.u32= rd8(addr);
    //EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.DMA_SEL= 0x3;  //ADMA2 or ADMA3 is selected
    //wr8(addr, EMMC_REG.dwc_msh8_regmodel_host_ctrl1_r.u32);

    ////Set Descriptor Address
    //addr = emmc_msh8_baddr +  ADMA_SA_LOW_R_mshc_OFFSET;
    //EMMC_REG.dwc_msh8_regmodel_adma_sa_low_r.u32= ADMA_SA_LOW_R_mshc_DFLT_VAL;
    //EMMC_REG.dwc_msh8_regmodel_adma_sa_low_r.ADMA_SA_LOW= daddr;
    //wr(addr, EMMC_REG.dwc_msh8_regmodel_adma_sa_low_r.u32);

    //CMD16:Set Block Size for eMMC Card
    if(is_ddr == 0x0){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x10;
        emmc_cmd_send(emmc_msh8_baddr, (blksize & 0xfff),EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
        resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
        if(resp01 != 0x900){
            printf("*****Resp for CMD16 is %x.\n", resp01);
            finish(FAIL);
        }
    }

    //CMD23:Set Block Count for eMMC Card
    if(read_cmd == 0x12){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT    = 0x2 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG        = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE  = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL    = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE            = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x17;
        emmc_cmd_send(emmc_msh8_baddr, blkcount,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
        resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
        if(resp01 != 0x900){
            printf("*****Resp for CMD23 is %x.\n", resp01);
            finish(FAIL);
        }
    }

    //CMD17: Single Block Read
    //CMD18: Multiple Block Read
    addr = emmc_msh8_baddr + XFER_MODE_R_mshc_OFFSET ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32 = XFER_MODE_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DMA_ENABLE                = 0x0 ;
    if(read_cmd == 0x11){
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.BLOCK_COUNT_ENABLE    = 0x0 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.MULTI_BLK_SEL         = 0x0 ;
    }
    else if(read_cmd == 0x12){
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.BLOCK_COUNT_ENABLE    = 0x1 ;
        EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.MULTI_BLK_SEL         = 0x1 ;
    }
    else{
        printf("*****Invalid Write Command!!!\n");
        finish(FAIL);
    }
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.AUTO_CMD_ENABLE           = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.DATA_XFER_DIR             = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_TYPE                 = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_ERR_CHK_ENABLE       = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.RESP_INT_DISABLE          = 0x0 ;
    wr16(addr, EMMC_REG.dwc_msh8_regmodel_xfer_mode_r.u32);
    EMMC_REG.dwc_msh8_regmodel_cmd_r.u32 = CMD_R_mshc_DFLT_VAL;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.RESP_TYPE_SELECT        = 0x2 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.SUB_CMD_FLAG            = 0x0 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_CRC_CHK_ENABLE      = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_IDX_CHK_ENABLE      = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.DATA_PRESENT_SEL        = 0x1 ;
    EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_TYPE                = 0x0 ;
    if(read_cmd == 0x11){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x11;
    }
    else if(read_cmd == 0x12){
        EMMC_REG.dwc_msh8_regmodel_cmd_r.CMD_INDEX           = 0x12;
    }
    else{
        printf("*****Invalid Write Command!!!\n");
        finish(FAIL);
    }
    emmc_cmd_send(emmc_msh8_baddr, raddr,EMMC_REG.dwc_msh8_regmodel_cmd_r.u32);
    resp01 = emmc_get_rsp(emmc_msh8_baddr, 0);
    if(resp01 != 0x900){
        printf("*****Resp for CMD24/CMD25 is %x.\n", resp01);
        finish(FAIL);
    }

    //Wait for BUF_RD_READY
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x20 != (rdata & 0x20)){
        rdata = rd16(addr)    ;
    }
    wr16(addr, 0x20);

    pkt_buf_addr = emmc_msh8_baddr+BUF_DATA_R_mshc_OFFSET;
    addr = destination_addr;
    for(j = 0; j < blkcount; j++){
        //Wait for BUF_RD_ENABLE
        addr1 = emmc_msh8_baddr + PSTATE_REG_mshc_OFFSET ;
        rdata = rd(addr1)     ;
        rdata = rd(addr1)     ;
        while(0x800 != (rdata & 0x800)){
            rdata = rd(addr1)     ;
        }
        for(i = 0; i < 128; i++){
            rdata = rd(pkt_buf_addr);
            wr(addr, rdata);
            addr = addr + 4;
        }
    }

    //Wait for transfer complete
    addr = emmc_msh8_baddr + NORMAL_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    while(0x2 != (rdata & 0x2)){
        rdata = rd16(addr)    ;
    }
    wr16(addr, 0x2);

    //Check DATA_TOUT_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x10 == (rdata & 0x10)){
        printf("*****DATA_TOUT_ERR!!!\n");
        wr16(addr, 0x10);
        finish(FAIL);
    }

    //Check DATA_CRC_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x20 == (rdata & 0x20)){
        printf("*****DATA_CRC_ERR!!!\n");
        wr16(addr, 0x20);
        finish(FAIL);
    }

    //Check DATA_END_BIT_ERR
    addr = emmc_msh8_baddr + ERROR_INT_STAT_R_mshc_OFFSET ;
    rdata = rd16(addr)     ;
    if(0x40 == (rdata & 0x40)){
        printf("*****DATA_END_BIT_ERR!!!\n");
        wr16(addr, 0x40);
        finish(FAIL);
    }
}


void emmc_for_bootrom(u64 emmc_msh8_baddr, int blkcount, int source_addr, int destination_addr, int partition_sel){


    tb_wr(FLAG_FOR_BOOT_BACKDOOR, 0x0);

//Configure Clkgen*********************************************************
	//osc_clk_freq_cfg(24);
 	//osc_clk_gate_en(0);
 	//ref_clk_freq_cfg(200);
 	//ref_clk_gate_en(0);
 	//axi_aclk_freq_cfg(300);
 	//axi_aclk_gate_en(0);
 	//ahb_hclk_freq_cfg(250);
 	//ahb_hclk_gate_en(0);
 	//msh8_rst_n_en(0);

    //wait_ns(1000);
    emmc_host_clk_config(EMMC_CLKGEN_BADDR, EMMC_CLKGEN_OFFSET, EMMC_CLKGEN_VALUE);


//Configure DWC_mshc*********************************************************
    emmc_msh8_base_config(emmc_msh8_baddr);


//Configure eMMC_Card*******************************************************
    //Card Initialization and Identification
    emmc_initialization(emmc_msh8_baddr);

    //Switch Data Bus Width
    //1  - 1bit SDR
    //4  - 4bit SDR
    //8  - 8bit SDR
    //44 - 4bit DDR
    //88 - 8bit DDR
    emmc_data_bus_width(emmc_msh8_baddr, 8, 0x0);

    //Bus Test
    //1  - 1bit SDR
    //4  - 4bit SDR
    //8  - 8bit SDR
    //44 - 4bit DDR
    //88 - 8bit DDR
    emmc_bus_test(emmc_msh8_baddr, 8);

    //Switch Speed Mode
    //0 - Legacy
    //1 - HS SDR
    //2 - HS DDR
    //3 - HS200
    //4 - HS400
    emmc_data_speed_mode(emmc_msh8_baddr, 0x1, 0x0);

    //Change Clock Frequence to 200MHz/EMMC_CLK_DIVISOR
    //EMMC_CLK_DIVISOR=0,2,4,...,2046
    emmc_clk_change(emmc_msh8_baddr, 4);

    //Select Partitions to Access
    //0 - User Area
    //1 - Boot Partition 1
    //2 - Boot Partition 2
    emmc_partition_select(emmc_msh8_baddr, partition_sel);


//Read data*************************************************************

    //Read command
    //0x11 - CMD17: READ_SINGLE_BLOCK
    //0x12 - CMD18: READ_MULTIPLE_BLOCK
    emmc_read_data_nonDMA(emmc_msh8_baddr, 0x200, blkcount, source_addr, 0x0, destination_addr, 0x12);

    tb_wr(FLAG_FOR_BOOT_BACKDOOR, blkcount);
}






//    //
//    addr = emmc_msh8_baddr +  ;
//    EMMC_REG..u32= rd(addr);
//    wr(addr, );
