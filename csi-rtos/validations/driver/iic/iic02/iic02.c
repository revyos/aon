/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic02.c
 * @brief    the main function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iic_test.h"
#include "dtest.h"

int test_iic(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_iic", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"iic_capa_if_test", iic_capa_if_test, 1},
        {"iic_init_if_test", iic_init_if_test, 1},
        {"iic_uninit_if_test", iic_uninit_if_test, 1},
        {"iic_master_send_if_test", iic_master_send_if_test, 1},
        {"iic_master_receive_if_test", iic_master_receive_if_test, 1},
        {"iic_slave_send_if_test", iic_slave_send_if_test, 1},
        {"iic_slave_receive_if_test", iic_slave_receive_if_test, 1},
        {"iic_other_if_test", iic_other_if_test, 1},
        {"iic_config_if_test", iic_config_if_test, 1},
        {"iic_func1_test", iic_func1_test, 1},
        {"iic_func2_test", iic_func2_test, 1},
        {"iic_func3_test", iic_func3_test, 1},
#if 0
        {"iic_func4_test", iic_func4_test, 1},
#endif
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;

}

int main(void)
{
    dtest_register_cb(test_iic);
    dtest();

    return 0;
}
