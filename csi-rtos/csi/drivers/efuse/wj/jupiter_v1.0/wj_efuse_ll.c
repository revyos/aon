/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     wj_efuse_ll.c
 * @brief    wj efuse ll driver
 * @version  V1.0
 * @date     17. July 2020
 ******************************************************************************/
#include <stdint.h>
#include <drv/tick.h>
#include "wj_efuse_ll.h"

#define EFUSE_PROGRAM_TIMEOUT           (100U)
#define EFUSE_READ_TIMEOUT              (100U)

static int32_t wj_efuse_program_bytes(wj_efuse_regs_t *efuse_base, uint32_t addr, uint32_t data, uint32_t num)
{
    csi_error_t ret = CSI_OK;
    uint32_t program_start = csi_tick_get_ms();

    for (uint8_t k = 0U; k < (num << 3U); k++) {
        if (data & 0x1U) {
            wj_efuse_config_addr(efuse_base, addr + ((uint32_t)k << 5U));
            wj_efuse_config_command(efuse_base, WJ_EFUSE_COMMAND_NORMAL_WRITE);

            while (!(wj_efuse_get_status(efuse_base) & WJ_EFUSE_STATUS_IDLE)) {
                if ((csi_tick_get_ms() - program_start) >= EFUSE_PROGRAM_TIMEOUT) {
                    ret = CSI_TIMEOUT;
                    break;
                }
            }
        }

        data >>= 1U;
    }

    return (int32_t)ret;
}

static int32_t wj_efuse_read_words(wj_efuse_regs_t *efuse_base, uint32_t addr, uint32_t *data, uint32_t num)
{
    csi_error_t ret = CSI_OK;
    uint32_t read_start = csi_tick_get_ms();

    for (uint8_t k = 0U; k < num; k++) {
        wj_efuse_config_addr(efuse_base, addr + k);
        wj_efuse_config_command(efuse_base, WJ_EFUSE_COMMAND_NORMAL_READ);

        while (!(wj_efuse_get_status(efuse_base) & WJ_EFUSE_STATUS_IDLE)) {
            if ((csi_tick_get_ms() - read_start) >= EFUSE_READ_TIMEOUT) {
                ret = CSI_TIMEOUT;
                break;
            }
        }

        *data++ = wj_efuse_read_word(efuse_base);
    }

    return (int32_t)ret;
}

int32_t wj_efuse_program(wj_efuse_regs_t *efuse_base, uint32_t addr, uint8_t *data, uint32_t size)
{
    uint8_t *src = data;
    uint8_t redundant = (uint8_t)(addr % 4U);
    uint32_t i, word;
    uint32_t word_addr = addr >> 2U;
    uint32_t cnt = 0U;

    wj_efuse_dis_wp(efuse_base);
    uint32_t byte_base = word_addr + (0x100U * (uint32_t)redundant);
    cnt = (size > (4U - (uint32_t)redundant)) ? (4U - (uint32_t)redundant) : size;

    memcpy(&word, src, cnt);

    wj_efuse_program_bytes(efuse_base, byte_base, word, cnt);

    src += cnt;
    word_addr++;

    uint32_t times = (size - cnt) >> 2U;

    for (i = 0U; i < times; i++) {
        word = ((uint32_t)src[0] | ((uint32_t)src[1] << 8U) | ((uint32_t)src[2] << 16U) | ((uint32_t)src[3] << 24U));
        wj_efuse_program_bytes(efuse_base, word_addr, word, 4U);
        word_addr++;
        src += 4U;
    }

    cnt += times << 2U;

    if (size > cnt) {
        memcpy(&word, src, size - cnt);
        wj_efuse_program_bytes(efuse_base, word_addr, word, size - cnt);
    }

    wj_efuse_en_wp(efuse_base);

    return (int32_t)size;
}

int32_t wj_efuse_read(wj_efuse_regs_t *efuse_base, uint32_t addr, uint8_t *data, uint32_t size)
{
    uint8_t *src = data;
    uint8_t redundant = (uint8_t)(addr % 4U);
    uint32_t word_addr = addr >> 2U;
    uint32_t word;
    uint32_t cnt = 0U;
    uint32_t times = 0U;
    int32_t  ret = 0;

    do {
        wj_efuse_read_words(efuse_base, word_addr, &word, 1U);
        word_addr++;

        cnt = (size > (4U - (uint32_t)redundant)) ? (4U - (uint32_t)redundant) : size;
        memcpy(src, ((uint8_t *)&word + redundant), cnt);
        src += cnt;

        if (size == cnt) {
            ret = (int32_t)cnt;
            break;
        }

        times = (size - cnt) >> 2U;

        for (uint32_t i = 0U; i < times; i++) {
            wj_efuse_read_words(efuse_base, word_addr, &word, 1U);
            memcpy(src, &word, 4U);
            src += 4U;
            word_addr++;
        }

        cnt += times << 2U;

        if (size > cnt) {
            wj_efuse_read_words(efuse_base, word_addr, &word, 1U);
            memcpy(src, &word, size - cnt);
        }

        ret = (int32_t)size;
    } while (0);


    return ret;
}

