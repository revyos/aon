/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     wj_efuse_ll.h
 * @brief    header file for efuse ll driver
 * @version  V1.0
 * @date     27. March 2020
 ******************************************************************************/

#ifndef _WJ_EFUSE_LL_H_
#define _WJ_EFUSE_LL_H_

#include <drv/efuse.h>
#include <soc.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WJ_EFUSE_MEM_SIZE                       128U
#define WJ_EFUSE_COMMAND_NORMAL_WRITE       (1U << 1U)
#define WJ_EFUSE_COMMAND_NORMAL_READ        (1U << 0U)
#define WJ_EFUSE_STATUS_IDLE                (1U << 0U)

#define WJ_EFUSE_POWER_DOWN         0x1U
#define WJ_EFUSE_WRITE_PROTECT_EN   0xaa55aa55U
#define WJ_EFUSE_WRITE_PROTECT_DIS  0x55aa55aaU

typedef struct {
    __IOM uint32_t COMMAND;         /* Offset: 0x000 (R/W) efuse command register */
    __IOM uint32_t ADDR;            /* Offset: 0x004 (R/W) read and write efuse address */
    __IOM uint32_t READ_DATA;       /* Offset: 0x008 (R/W) data out from efuse register */
    __IM uint32_t STATUS;           /* Offset: 0x00C (R/ ) efuse operation status register */
    __IOM uint32_t TIMING_CONFIG;   /* Offset: 0x010 (R/W) timing register */
    uint32_t RESERVED1[3];
    __IOM uint32_t REGISTER_CONFIG; /* Offset: 0x020 (R/W) power down register */
    uint32_t RESERVED2[3];
    __OM uint32_t WRITE_PROTECT;    /* Offset: 0x030 ( /W) write protect register */
    uint32_t RESERVED3[50];
    __IM uint32_t IP_ID;            /* Offset: 0x0FC (R/ ) ip id register */
} wj_efuse_regs_t;

static inline void wj_efuse_config_command(wj_efuse_regs_t *efuse_base, uint32_t command)
{
    efuse_base->COMMAND = command;
}

static inline void wj_efuse_config_addr(wj_efuse_regs_t *efuse_base, uint32_t addr)
{
    efuse_base->ADDR = addr;
}

static inline uint32_t wj_efuse_read_word(wj_efuse_regs_t *efuse_base)
{
    return efuse_base->READ_DATA;
}

static inline uint32_t wj_efuse_get_ip_id(wj_efuse_regs_t *efuse_base)
{
    return efuse_base->IP_ID;
}

static inline uint32_t wj_efuse_get_status(wj_efuse_regs_t *efuse_base)
{
    return efuse_base->STATUS;
}

static inline void wj_efuse_power_down(wj_efuse_regs_t *efuse_base)
{
    (efuse_base->REGISTER_CONFIG |= (WJ_EFUSE_POWER_DOWN));
}

static inline void wj_efuse_release_power_down(wj_efuse_regs_t *efuse_base)
{
    (efuse_base->REGISTER_CONFIG &= ~(WJ_EFUSE_POWER_DOWN));
}

static inline void wj_efuse_en_wp(wj_efuse_regs_t *efuse_base)
{
    efuse_base->WRITE_PROTECT = WJ_EFUSE_WRITE_PROTECT_EN;
}

static inline void wj_efuse_dis_wp(wj_efuse_regs_t *efuse_base)
{
    efuse_base->WRITE_PROTECT = WJ_EFUSE_WRITE_PROTECT_DIS;
}

int32_t wj_efuse_program(wj_efuse_regs_t *efuse_base, uint32_t addr, uint8_t *data, uint32_t size);

int32_t wj_efuse_read(wj_efuse_regs_t *efuse_base, uint32_t addr, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
