/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     aes_test.h
 * @brief    the head file
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <csi_core.h>
#include <drv/aes.h>
#include <drv/errno.h>

struct aes_test {
    char *key;
    unsigned char *iv;
    char *pass;
    aes_mode_e aes_mode;
    aes_crypto_mode_e crypto_mode;
    aes_key_len_bits_e bits;
    aes_endian_mode_e endian;
    char *expect;
};

typedef struct aes_test aes_test_t;

extern void mdelay(uint32_t ms);
