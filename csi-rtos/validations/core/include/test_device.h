/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#ifndef _TEST_DEVICE_H_
#define _TEST_DEVICE_H_

#include <stdint.h>
#include "soc.h"
#include "csi_core.h"

struct unary_calculation {
    uint32_t op1;
    uint32_t result;
};

struct binary_calculation {
    uint32_t op1;
    uint32_t op2;
    uint32_t result;
};

struct ternary_calculation {
    uint32_t op1;
    uint32_t op2;
    uint32_t op3;
    uint32_t result;
};

struct ternary64_calculation {
    uint32_t op1;
    uint32_t op2;
    uint64_t op3;
    uint64_t result;
};

#define TEST_SIZE 3
extern unsigned int *tst_addr;

#endif  /* TEST_DEVICE_H */
