/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     sha_test.h
 * @brief    the head file
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <csi_core.h>
#include <csi_kernel.h>
#include <drv/sha.h>

struct sha_test {
    char *key;
    sha_mode_e sha_mode;
    sha_endian_mode_e endian;
    char *expect;
};

typedef struct sha_test sha_test_t;

struct sha_invalid {
    sha_mode_e sha_mode;
    sha_endian_mode_e endian;
    int retval;
};

typedef struct sha_invalid sha_invalid_t;
