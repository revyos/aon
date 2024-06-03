/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     main.c
 * @brief    hello world
 * @version  V1.0
 * @date     03. April 2020
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "drv/sdhc.h"

int dev_mmc_init(dev_mmc_t *handler, int idx)
{
    handler->sdhci = sdhci_module_init(idx);
    handler->emmc_dev= mmc_dev_new(idx);
    mmc_core_init(handler->emmc_dev, &handler->sdhci->host);
    handler->sd_card.mmc_dev = handler->emmc_dev;
    handler->emmc_dev->card = &handler->sd_card;
    return 0;
}

int dev_mmc_config(dev_mmc_t *handler, dev_mmc_config_t *config)
{
    mmc_clock_enable(handler->emmc_dev, config->speed_mode, config->vdd, config->clk_freq);
    mmc_set_bus_width(handler->emmc_dev, config->bus_width);
    mmc_set_voltage(handler->emmc_dev, XSD_BUS_PWR_VDD1, config->vdd);
    mmc_set_speed_mode(handler->emmc_dev, config->speed_mode);
    return 0;
}

int dev_emmc_card_init(dev_mmc_t *handler, dev_mmc_config_t *config)
{
    struct mmc_dev_t *mmc_dev = handler->emmc_dev;
    struct sd_card_t *sd_card = &handler->sd_card;
    struct cmd_param_t cur_emmc_cmd;

    memset(&cur_emmc_cmd, 0, sizeof(cur_emmc_cmd));

    /*sendcmd*/
    //XSTATE_SET_CARD_IDLE
    int retval = mmc_cmd0_card_set_idle(mmc_dev, &cur_emmc_cmd, 0);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return 0;
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 0);
    //XSTATE_CHK_SD_OP_COND
#define EMMC_CMD1_ARG           0xc0ff8080
#define EMMC_DEV_INIT_COMPLETE  (1 << 31)
    int cmd1_retry = 1000;
    while(cmd1_retry--)  {
         retval = mmc_cmd1_emmc_send_op_cond(mmc_dev, &cur_emmc_cmd, EMMC_CMD1_ARG);
         if (retval != 0) {
             clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
         } else if ((cur_emmc_cmd.cur_cmd.resp[0] & EMMC_DEV_INIT_COMPLETE) != EMMC_DEV_INIT_COMPLETE) {
             clog_print(CLOG_INFO, "%s, %d, cmd1 retry %d\n", __func__, __LINE__, 20 - cmd1_retry);
             clog_print(CLOG_INFO, "%s, %d, cmd1 cur_cmd.resp[0]%x\n", __func__, __LINE__, cur_emmc_cmd.cur_cmd.resp[0]);
             mmc_cmd_reinit(&cur_emmc_cmd);
         } else {
             mmc_cmd_reinit(&cur_emmc_cmd);
             break;
         }
    }
    if (cmd1_retry <= 0) {
        clog_print(CLOG_ERR, "%s, %d,cmd1 timer out\n", __func__, __LINE__);
        return -1;
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 1);

    //XSTATE_GET_CARD_CID
    retval = mmc_cmd2_get_card_cid(mmc_dev, &cur_emmc_cmd, 0);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        for (int i = 0; i < 4; ++i) {
            sd_card->cid.word[i] = cur_emmc_cmd.cur_cmd.resp[i];
        }
        mmc_cmd_reinit(&cur_emmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 2);

    //XSTATE_GET_CARD_REL_ADR
    retval = mmc_cmd3_send_rel_adr(mmc_dev, &cur_emmc_cmd, config->rca);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        sd_card->rca = config->rca;
        clog_print(CLOG_INFO, "sd-card rel-addr is %d\n", sd_card->rca);
        mmc_cmd_reinit(&cur_emmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 3);

    //XSTATE_GET_CARD_CSD
    retval = mmc_cmd9_get_csd(mmc_dev, &cur_emmc_cmd, sd_card->rca);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        for (int i = 0; i < 4; ++i)
            sd_card->csd.word[i] = cur_emmc_cmd.cur_cmd.resp[i];
        mmc_cmd_reinit(&cur_emmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 9);

    //XSTATE_SELECT_CARD
    retval = mmc_cmd7_card_select(mmc_dev, &cur_emmc_cmd, sd_card->rca);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    }
    mmc_cmd_reinit(&cur_emmc_cmd);
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 7);

    //XSTATE_GET_EXT_CSD
    retval = emmc_cmd8_get_ext_csd(mmc_dev, &cur_emmc_cmd, (uint8_t *)&sd_card->ext_csd,
                                   sizeof(union card_ext_csd_t), 0);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        clog_print(CLOG_INFO, "card-ext-csd data\n");
        mmc_cmd_reinit(&cur_emmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 8);

    //XSTATE_SET_BUS_WIDTH
    retval = emmc_cmd6_set_ext_csd(mmc_dev, &cur_emmc_cmd, ACCESS_MODE_WRITE_BYTE,
                EXTCSD_IDX183_BUSWIDTH, config->bus_width, S_CMD_SET1);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        mmc_set_bus_width(sd_card->mmc_dev, config->bus_width);
        mmc_set_voltage(sd_card->mmc_dev, XSD_BUS_PWR_VDD1, config->vdd);
        clog_print(CLOG_INFO, "emmc buswidth set to 8 bit\n");
        mmc_cmd_reinit(&cur_emmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 6);

    //XSTATE_SET_SPEED_MODE
    retval = emmc_cmd6_set_ext_csd(mmc_dev, &cur_emmc_cmd, ACCESS_MODE_WRITE_BYTE,
                                   EXTCSD_IDX184_HS_TIMING, config->speed_mode, S_CMD_SET1);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        mmc_set_speed_mode(sd_card->mmc_dev, config->speed_mode);
        clog_print(CLOG_INFO, "emmc set hs200 speed mode\n");
        mmc_clock_enable(sd_card->mmc_dev, config->speed_mode, config->vdd, config->clk_freq);
        mmc_cmd_reinit(&cur_emmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 6);
    return 0;
}

int dev_emmc_boot_partition_enable(dev_mmc_t *handler, emmc_partition_en partition)
{
    struct mmc_dev_t *mmc_dev = handler->emmc_dev;
    struct cmd_param_t cur_emmc_cmd;
    uint8_t *ext_csd = (uint8_t *)handler->sd_card.ext_csd.word;
    uint8_t reg_val = ext_csd[179];

    memset(&cur_emmc_cmd, 0, sizeof(cur_emmc_cmd));

    reg_val &= ~(7 << EMMC_BOOT_PARTITION_EN_ADDR);
    reg_val |= (partition << EMMC_BOOT_PARTITION_EN_ADDR);

    int retval = emmc_cmd6_set_ext_csd(mmc_dev, &cur_emmc_cmd, ACCESS_MODE_WRITE_BYTE,
                EXTCSD_IDX179_BOOT_PARTTION, reg_val, S_CMD_SET1);

    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    }

    memset(&cur_emmc_cmd, 0, sizeof(cur_emmc_cmd));
    uint8_t retry = 10;
    while(retry--) {
        retval = emmc_cmd8_get_ext_csd(handler->emmc_dev, &cur_emmc_cmd, (uint8_t *)&handler->sd_card.ext_csd,
                                       sizeof(union card_ext_csd_t), 0);
        if (retval != 0) {
            clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
            return -1;
        }

        if(ext_csd[179] == reg_val)
            break;
        mdelay(5);
    }

    if(ext_csd[179] != reg_val) {
        return -1;
    }

    return 0;
}

int dev_emmc_boot_partition_access(dev_mmc_t *handler, emmc_partition_access partition)
{
    struct mmc_dev_t *mmc_dev = handler->emmc_dev;
    uint8_t *ext_csd = (uint8_t *)handler->sd_card.ext_csd.word;
    struct cmd_param_t cur_emmc_cmd;
    uint8_t reg_val = ext_csd[179];

    memset(&cur_emmc_cmd, 0, sizeof(cur_emmc_cmd));

    reg_val &= ~(7 << EMMC_BOOT_PARTITION_ACCESS);
    reg_val |= (partition << EMMC_BOOT_PARTITION_ACCESS);

    int retval = emmc_cmd6_set_ext_csd(mmc_dev, &cur_emmc_cmd, ACCESS_MODE_WRITE_BYTE,
                EXTCSD_IDX179_BOOT_PARTTION, reg_val, S_CMD_SET1);

    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    }

    memset(&cur_emmc_cmd, 0, sizeof(cur_emmc_cmd));

    uint8_t retry = 10;
    while(retry--) {
        retval = emmc_cmd8_get_ext_csd(handler->emmc_dev, &cur_emmc_cmd, (uint8_t *)&handler->sd_card.ext_csd,
                                       sizeof(union card_ext_csd_t), 0);
        if (retval != 0) {
            clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
            return -1;
        }

        if(ext_csd[179] == reg_val)
            break;
        mdelay(5);
    }

    if(ext_csd[179] != reg_val) {
        return -1;
    }

    return 0;
}

int dev_emmc_boot_set_bus_width(dev_mmc_t *handler, emmc_boot_bus_width width)
{
    struct mmc_dev_t *mmc_dev = handler->emmc_dev;
    struct cmd_param_t cur_emmc_cmd;
    uint8_t *ext_csd = (uint8_t *)handler->sd_card.ext_csd.word;
    uint8_t reg_val = ext_csd[EXTCSD_IDX177_BUS_CONDITIONS];

    memset(&cur_emmc_cmd, 0, sizeof(cur_emmc_cmd));

    reg_val &= ~0x3;
    reg_val |= width;

    int retval = emmc_cmd6_set_ext_csd(mmc_dev, &cur_emmc_cmd, ACCESS_MODE_WRITE_BYTE,
                EXTCSD_IDX177_BUS_CONDITIONS, reg_val, S_CMD_SET1);

    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    }

    memset(&cur_emmc_cmd, 0, sizeof(cur_emmc_cmd));
    retval = emmc_cmd8_get_ext_csd(handler->emmc_dev, &cur_emmc_cmd, (uint8_t *)&handler->sd_card.ext_csd,
                                   sizeof(union card_ext_csd_t), 0);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    }

    if(ext_csd[EXTCSD_IDX177_BUS_CONDITIONS] != reg_val) {
        return -1;
    }

    return 0;
}

int dev_emmc_boot_info(dev_mmc_t *handler, emmc_boot_info_t *info)
{
    struct cmd_param_t cur_emmc_cmd;
    uint8_t *ext_csd = (uint8_t *)handler->sd_card.ext_csd.word;

    memset(&cur_emmc_cmd, 0, sizeof(cur_emmc_cmd));
    int retval = emmc_cmd8_get_ext_csd(handler->emmc_dev, &cur_emmc_cmd, (uint8_t *)&handler->sd_card.ext_csd,
                                   sizeof(union card_ext_csd_t), 0);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    }

    if (ext_csd[EXTCSD_IDX177_BUS_CONDITIONS] == 0) {
        info->bus_width = 1;
    } else if (ext_csd[EXTCSD_IDX177_BUS_CONDITIONS] == 1) {
        info->bus_width = 4;
    } else if (ext_csd[EXTCSD_IDX177_BUS_CONDITIONS] == 2) {
        info->bus_width = 8;
    }

    info->block_cnt = ext_csd[EXTCSD_IDX226_BOOT_BLOCK_CNT_ADDR];

    return 0;
}

int dev_sd_card_inserted(dev_mmc_t *handler)
{
    extern int is_sd_card_inserted(struct sdhci_t *sdhci);
    return is_sd_card_inserted(handler->sdhci);
}

int dev_sd_card_init(dev_mmc_t *handler, dev_mmc_config_t *config)
{
    struct mmc_dev_t *sd_handler = handler->emmc_dev;
    struct sd_card_t *sd_card = &handler->sd_card;
    struct cmd_param_t cur_mmc_cmd;

    memset(&cur_mmc_cmd, 0, sizeof(cur_mmc_cmd));

    /*sendcmd*/
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 0);
    //XSTATE_SET_CARD_IDLE
    int retval = mmc_cmd0_card_set_idle(sd_handler, &cur_mmc_cmd, 0);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    }
    mmc_cmd_reinit(&cur_mmc_cmd);

    retval = mmc_cmd8_check_card_intf_condition(sd_handler, &cur_mmc_cmd, 1, 0xaa);
     if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    }
    mmc_cmd_reinit(&cur_mmc_cmd);
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 8);

    //XSTATE_CHK_SD_OP_COND
    uint32_t status;
    uint32_t retury = 0;
    while(retury < 500) {
        retury++;
        mmc_cmd_reinit(&cur_mmc_cmd);
        retval = mmc_acmd41_check_card_op_condition(sd_handler, &cur_mmc_cmd, 1, SD_VDD1_1P8_VOLT, 0, 1);
        if (retval != 0) {
            clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
            return -1;
        } else {
           sd_card->acmd41_resp.status = cur_mmc_cmd.cur_cmd.resp[0];
           status = sd_card->acmd41_resp.status;
           sd_card->init_complete = !((status & ACMD41_RESP_VALID) == 0);
           if (sd_card->init_complete) {
                sd_card->is_sdxc_card = !!(status & ACMD41_R2_CARD_CAPACITY_STS);
                sd_card->is_ush2_card = !!(status & ACMD41_R2_UHS2_CARD);
                sd_card->s18a_volt_switch_ready = !!(status & ACMD41_R2_S18A_READY);
                clog_print(CLOG_INFO, "sd-card supports %s, %s %s\n",
                    sd_card->is_sdxc_card ? "sdxc" : "sdhc",
                    sd_card->is_ush2_card ? "uhs2," : "",
                    sd_card->s18a_volt_switch_ready ? "ready to switch 1.8v":"");
                if (!sd_card->s18a_volt_switch_ready) {
                    goto XSTATE_GET_CARD_CID;
                }
                break;
           }
        }
        clog_print(CLOG_INFO, "%s, %d,######################### cmd[41] retury %d\n", __func__, __LINE__, retury);
    }
    if (sd_card->init_complete == 0) {
        clog_print(CLOG_ERR, "%s, %d, cmd41 error\n", __func__, __LINE__);
        return -1;
    }

    mmc_cmd_reinit(&cur_mmc_cmd);
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 41);

    //XSTATE_SWITCH_TO_1P8V
//    if(config->)
//    retval = mmc_cmd11_voltage_switch(sd_handler, &cur_mmc_cmd, 0);
//    if (retval != 0) {
//        clog_print(CLOG_INFO, "%s, %d, timer out\n", __func__, __LINE__);
//        return -1;
//    } else {
//        /* disable and enable the mmc clocks */
//        mmc_clock_enable(sd_card->mmc_dev, SDR12_DS_SPEED_MODE, SD_VDD1_1P8_VOLT, MMCM_CLK_400KHZ);
//        mmc_set_voltage(sd_card->mmc_dev, XSD_BUS_PWR_VDD1, SD_VDD1_1P8_VOLT);
//        mmc_cmd_reinit(&cur_mmc_cmd);
//    }
//    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 11);

XSTATE_GET_CARD_CID:
    mmc_cmd_reinit(&cur_mmc_cmd);
    retval = mmc_cmd2_get_card_cid(sd_handler, &cur_mmc_cmd, 0);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        for (int i = 0; i < 4; ++i)
            sd_card->cid.word[i] = cur_mmc_cmd.cur_cmd.resp[i];
        mmc_cmd_reinit(&cur_mmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 2);

    //XSTATE_GET_CARD_REL_ADR
    retval = mmc_cmd3_send_rel_adr(sd_handler, &cur_mmc_cmd, 0);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        sd_card->rca = (cur_mmc_cmd.cur_cmd.resp[0] >> 16) & 0xFFFF;
        clog_print(CLOG_INFO, "sd-card rel-addr is %d\n", sd_card->rca);
        mmc_cmd_reinit(&cur_mmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 3);

    //XSTATE_GET_CARD_CSD
    retval = mmc_cmd9_get_csd(sd_handler, &cur_mmc_cmd, sd_card->rca);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        for (int i = 0; i < 4; ++i)
            sd_card->csd.word[i] = cur_mmc_cmd.cur_cmd.resp[i];
        mmc_cmd_reinit(&cur_mmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 9);

    //XSTATE_SELECT_CARD
    retval = mmc_cmd7_card_select(sd_handler, &cur_mmc_cmd, sd_card->rca);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    }
    mmc_cmd_reinit(&cur_mmc_cmd);
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 7);


    //XSTATE_GET_CARD_SCR
    retval = mmc_acmd51_get_card_scr(sd_handler, &cur_mmc_cmd, &sd_card->scr.word[0], 8);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        clog_print(CLOG_INFO, "card scr %08x %08x\n", sd_card->scr.word[0], sd_card->scr.word[1]);
        mmc_cmd_reinit(&cur_mmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 51);

    //XSTATE_GET_CARD_STATUS
    retval = mmc_acmd13_get_status(sd_handler, &cur_mmc_cmd, &sd_card->status, 64);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        for (int i = 0; i < 16; ++i)
            clog_print(CLOG_INFO, "acmd13-status.w%d %08x\n", i, sd_card->status.word[i]);
        mmc_cmd_reinit(&cur_mmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 13);

    //XSTATE_SWITCH_CARD_FN
    retval = mmc_cmd6_switch_function(sd_handler, &cur_mmc_cmd, 0, &sd_card->cmd6_query_status, 64);
    if (retval != 0) {
        clog_print(CLOG_INFO, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        for (int i = 0; i < 16; ++i)
            clog_print(CLOG_INFO, "cmd6-query-status.w%d %08x\n", i, sd_card->cmd6_query_status.word[i]);
        mmc_cmd_reinit(&cur_mmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 6);

    //XSTATE_SET_BUS_WIDTH
    retval = mmc_acmd6_set_buswidth(sd_handler, &cur_mmc_cmd, config->bus_width);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        mmc_set_bus_width(sd_card->mmc_dev, config->bus_width);
        clog_print(CLOG_INFO, "sd buswidth set to 4 bit\n");
        mmc_cmd_reinit(&cur_mmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 6);

    //XSTATE_SET_SPEED_MODE
    retval = mmc_cmd6_switch_function(sd_handler, &cur_mmc_cmd, config->speed_mode, &sd_card->cmd6_query_status, 64);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    } else {
        mmc_set_speed_mode(sd_card->mmc_dev, config->speed_mode);
        mmc_clock_enable(sd_card->mmc_dev, config->speed_mode, XVDD1_1P8_VOLT, config->clk_freq);
        mmc_cmd_reinit(&cur_mmc_cmd);
    }
    clog_print(CLOG_INFO, "%s, %d,######################### cmd[%d] complete\n", __func__, __LINE__, 6);

    return 0;
}

static uint32_t get_csd_bit_val(uint8_t *data, uint32_t start_bit, uint32_t end_bit)
{
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t k = 0;
    uint32_t val = 0;

    for(i = 0; i <= 127; i++) {
        j = i / 8;
        uint8_t bit = data[j] & (1 << (i % 8)) ? 1 : 0;
        if (i >= start_bit) {
            val |= bit << k;
            k++;
        }

        if (i >= end_bit) {
            break;
        }
    }


    return val;
}

uint64_t dev_sd_card_size_kbytes(dev_mmc_t *handler)
{
    uint8_t csd[16] = {0};
    uint8_t *p = (uint8_t *)&handler->sd_card.csd.word;

    csd[0] = 0;
    for (int i = 0 ; i < 15; i++) {
        csd[i + 1] = p[i];
    }

    uint8_t version = get_csd_bit_val(csd, 126, 127);
    uint32_t csize = 0;
    if (version == 0) {
        csize = get_csd_bit_val(csd, 62, 73);
        uint32_t read_bl_len = get_csd_bit_val(csd, 80, 83);
        uint32_t csize_mult = get_csd_bit_val(csd, 47, 49);
        uint32_t mult = 1 << (csize_mult + 2);
        uint32_t blknr = (csize + 1) * mult;
        uint32_t bl_len = 1 << read_bl_len;
        return bl_len * blknr / 1024;
    } else if(version == 1) {
        csize = get_csd_bit_val(csd, 48, 69);
        return (csize + 1) * 512;
    } else if(version == 2) {
        csize = get_csd_bit_val(csd, 48, 75);
        return (csize + 1) * 512;
    }

    return -1;
}

uint32_t dev_mmc_write(dev_mmc_t *handler, uint8_t *buf, uint32_t buflen, uint32_t block_addr, uint32_t block_cnt)
{
    struct cmd_param_t cur_emmc_cmd;
    memset(&cur_emmc_cmd, 0, sizeof(cur_emmc_cmd));
    mmc_cmd_reinit(&cur_emmc_cmd);

    int retval = mmc_write_block(handler->emmc_dev, &cur_emmc_cmd, block_addr, block_cnt,
                                 buf, buflen);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    }
    mmc_cmd_reinit(&cur_emmc_cmd);
    return 0;
}

uint32_t dev_mmc_read(dev_mmc_t *handler, uint8_t *buf, uint32_t buflen, uint32_t block_addr, uint32_t block_cnt)
{
    struct cmd_param_t cur_emmc_cmd;
    memset(&cur_emmc_cmd, 0, sizeof(cur_emmc_cmd));
    mmc_cmd_reinit(&cur_emmc_cmd);

    int retval = mmc_read_block(handler->emmc_dev, &cur_emmc_cmd, block_addr, block_cnt, buf, buflen);
    if (retval != 0) {
        clog_print(CLOG_ERR, "%s, %d, timer out\n", __func__, __LINE__);
        return -1;
    }

    mmc_cmd_reinit(&cur_emmc_cmd);

    return 0;
}

void dev_mmc_uninit(dev_mmc_t *handler)
{
    sdhci_module_uninit(handler->sdhci);
    mmc_dev_free(handler->emmc_dev);
    memset(handler, 0, sizeof(dev_mmc_t));
}
