/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     crc_test.h
 * @brief    the head file
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <csi_core.h>
#include <csi_kernel.h>
#include <drv/crc.h>

struct crc_test {
    char *key;
    crc_mode_e crc_mode;
    crc_standard_crc_e crc_std;
    char *expect;
};

typedef struct crc_test crc_test_t;

struct crc_invalid {
    crc_mode_e crc_mode;
    crc_standard_crc_e crc_std;
    int retval;
};

typedef struct crc_invalid crc_invalid_t;
