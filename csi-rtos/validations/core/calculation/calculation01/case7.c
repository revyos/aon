/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include "dtest.h"
#include "test_device.h"

int test_case7(void)
{
    int i = 0;
    unsigned int test_mem[20];
    tst_addr = (unsigned int *)&test_mem;

    printf("Testing functions case7: byte operation interface!\n");

    struct unary_calculation rev_test[TEST_SIZE] = {
        {0x12345678, 0x78563412},
        {0x2a3a4a5a, 0x5a4a3a2a},
        {0x12233445, 0x45342312}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        ASSERT_TRUE(__REV(rev_test[i].op1) == rev_test[i].result);
    }

    struct unary_calculation rev16_test[TEST_SIZE] = {
        {0x12345678, 0x34127856},
        {0x2a3a4a5a, 0x3a2a5a4a},
        {0x12233445, 0x23124534}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        ASSERT_TRUE(__REV16(rev16_test[i].op1) == rev16_test[i].result);
    }

    struct unary_calculation revsh_test[TEST_SIZE] = {
        {0x5688, 0xFFFF8856},
        {0x4a5a, 0x00005a4a},
        {0x3445, 0x00004534}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        ASSERT_TRUE(__REVSH(revsh_test[i].op1) == revsh_test[i].result);
    }

    struct unary_calculation rbit_test[TEST_SIZE] = {
        {0x88888888, 0x11111111},
        {0x5a5a5a5a, 0x5a5a5a5a},
        {0x22222222, 0x44444444}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        ASSERT_TRUE(__RBIT(rbit_test[i].op1) == rbit_test[i].result);
    }

    struct binary_calculation ror_test[TEST_SIZE] = {
        {0x12345678,  1, 0x91A2B3C },
        {0x12345678, 16, 0x56781234},
        {0x12345678, 31, 0x2468ACF0}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        ASSERT_TRUE(__ROR(ror_test[i].op1, ror_test[i].op2) == ror_test[i].result);
    }

    struct binary_calculation ssat_test[TEST_SIZE] = {
        {0x12345678, 28,  0x7FFFFFF},
        {    0x1234, 28,     0x1234},
        {0xFFFFFFFF,  1, 0xFFFFFFFF}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        ASSERT_TRUE(__SSAT(ssat_test[i].op1, ssat_test[i].op2) == ssat_test[i].result);
    }

    struct binary_calculation ssat16_test[TEST_SIZE] = {
        {0x12345678, 8, 0x007F007F},
        {0x12341234, 4, 0x00070007},
        {0x12345678, 1, 0x00000000}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        ASSERT_TRUE(__SSAT16(ssat16_test[i].op1, ssat16_test[i].op2) == ssat16_test[i].result);
    }


    struct binary_calculation usat_test[TEST_SIZE] = {
        {0x12345678, 28,  0xFFFFFFF},
        {    0x1234, 28,     0x1234},
        {0x56781234,  1,        0x1}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        ASSERT_TRUE(__USAT(usat_test[i].op1, usat_test[i].op2) == usat_test[i].result);
    }

    struct binary_calculation usat16_test[TEST_SIZE] = {
        {0x12345678,  8, 0x00FF00FF},
        {0x12341234, 15, 0x12341234},
        {0x12345678,  1, 0x00010001}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        ASSERT_TRUE(__USAT16(usat16_test[i].op1, usat16_test[i].op2) == usat16_test[i].result);
    }

    struct unary_calculation clz_test[TEST_SIZE] = {
        {0x92345678,  0},
        {0x00008234, 16},
        {       0x0, 32}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        ASSERT_TRUE(__CLZ(clz_test[i].op1) == clz_test[i].result);
    }

    struct binary_calculation ldrbt_test[TEST_SIZE] = {
        {0, 0x12345678, 0x78},
        {2, 0x12345678, 0x34},
        {3, 0x12345678, 0x12}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        *(uint32_t *)tst_addr = ldrbt_test[i].op2;
        ASSERT_TRUE(__LDRBT((uint8_t *)tst_addr + ldrbt_test[i].op1) == ldrbt_test[i].result);
    }

    struct binary_calculation ldrht_test[TEST_SIZE] = {
        {0, 0x5678, 0x5678},
        {2, 0xFEDC, 0xFEDC},
        {0, 0x4321, 0x4321}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        *(uint16_t *)(tst_addr + ldrht_test[i].op1) = ldrht_test[i].op2;
        ASSERT_TRUE(__LDRHT((uint16_t *)(tst_addr + ldrht_test[i].op1)) == ldrht_test[i].result);
    }

    struct binary_calculation ldrt_test[TEST_SIZE] = {
        {0, 0x12345678, 0x12345678},
        {0, 0xFEDCBA09, 0xFEDCBA09},
        {0, 0x87654321, 0x87654321}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        *(uint32_t *)tst_addr = ldrt_test[i].op2;
        ASSERT_TRUE(__LDRT((uint32_t *)(tst_addr + ldrt_test[i].op1)) == ldrt_test[i].result);
    }

    struct binary_calculation strbt_test[TEST_SIZE] = {
        {0, 0x12, 0x12},
        {1, 0x34, 0x34},
        {3, 0x78, 0x78}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        __STRBT(strbt_test[i].op2, (uint8_t *)(tst_addr + strbt_test[i].op1));
        ASSERT_TRUE(*(uint8_t *)(tst_addr + strbt_test[i].op1) == strbt_test[i].result);
    }

    struct binary_calculation strht_test[TEST_SIZE] = {
        {0, 0x1234, 0x1234},
        {2, 0xFEDC, 0xFEDC},
        {0, 0x5678, 0x5678}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        __STRHT(strht_test[i].op2, (uint16_t *)(tst_addr + strht_test[i].op1));
        ASSERT_TRUE(*(uint16_t *)(tst_addr + strht_test[i].op1) == strht_test[i].result);
    }

    struct binary_calculation strt_test[TEST_SIZE] = {
        {0, 0x12345678, 0x12345678},
        {4, 0xFEDCBA09, 0xFEDCBA09},
        {0, 0x56781234, 0x56781234}
    };

    for (i = 0; i < TEST_SIZE; i++) {
        __STRT(strt_test[i].op2, (uint32_t *)(tst_addr + strt_test[i].op1));
        ASSERT_TRUE(*(uint32_t *)(tst_addr + strt_test[i].op1) == strt_test[i].result);
    }

    return 0;
}
