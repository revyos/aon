/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     i2s02.c
 * @brief    the main function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iis_test.h"
#include "dtest.h"

extern int8_t iis_Init_test(void);
extern int8_t iis_Uninit_test(void);
extern int8_t iis_GetCapa_test(void);
extern int8_t iis_CfgProt_test(void);
extern int8_t iis_CfgSampWid_test(void);
extern int8_t iis_CfgWorkMod_test(void);
extern int8_t iis_CfgSampRat_test(void);
extern int8_t iis_Cfg_test(void);
extern int8_t iis_Send_test(void);
extern int8_t iis_Recv_test(void);
extern int8_t iis_GetStat_test(void);
extern int8_t iis_PorCont_test(void);
extern int8_t iis_fun1_test(void);
extern int8_t iis_fun2_test(void);

int test_iis(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_iis", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"iis_Init_test", iis_Init_test, 1},
        {"iis_Uninit_test", iis_Uninit_test, 1},
        {"iis_GetCapa_test", iis_GetCapa_test, 1},
        {"iis_CfgProt_test", iis_CfgProt_test, 1},
        {"iis_CfgSampWid_test", iis_CfgSampWid_test, 1},
        {"iis_CfgWorkMod_test", iis_CfgWorkMod_test, 1},
        {"iis_CfgSampRat_test", iis_CfgSampRat_test, 1},
        {"iis_Cfg_test", iis_Cfg_test, 1},
        {"iis_Send_test", iis_Send_test, 1},
        {"iis_Recv_test", iis_Recv_test, 1},
        {"iis_GetStat_test", iis_GetStat_test, 1},
        {"iis_PorCont_test", iis_PorCont_test, 1},
#if 0
        {"iis_fun1_test", iis_fun1_test, 1},
        {"iis_fun2_test", iis_fun2_test, 1},
#endif
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_iis);
    dtest();

    return 0;
}
