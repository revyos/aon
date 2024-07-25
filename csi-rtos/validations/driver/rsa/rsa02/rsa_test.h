/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     rsa_test.h
 * @brief    the head file
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <csi_core.h>
#include <csi_kernel.h>
#include <drv/rsa.h>

struct rsa_test {
    rsa_hash_type_e    hash;
    rsa_padding_type_e padding;
};

typedef struct rsa_test rsa_test_t;

struct rsa_invalid {
    rsa_hash_type_e hash;
    rsa_endian_mode_e endian;
    rsa_data_bits_e bits;
    rsa_padding_type_e padding;
    int retval;
};

typedef struct rsa_invalid rsa_invalid_t;
