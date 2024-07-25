/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     pwm02.c
 * @brief    PWM driver test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "pwm_test.h"
#include "dtest.h"

void pwm_if_test()
{
    //initialize
    pwm_handle_t pwm = csi_pwm_initialize(100);
    ASSERT_TRUE(pwm == NULL);

    pwm = csi_pwm_initialize(0);
    ASSERT_TRUE(pwm != NULL);

    int32_t ret;
    //uninitialize
    csi_pwm_uninitialize(NULL);

#if 1
    //power_control
    ret = csi_pwm_power_control(NULL, DRV_POWER_FULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_PWM_BASE | DRV_ERROR_PARAMETER));

    ret = csi_pwm_power_control(pwm, 4);
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_PWM_BASE |DRV_ERROR_PARAMETER));
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_PWM_BASE | DRV_ERROR_UNSUPPORTED));
#endif

    //config
    ret = csi_pwm_config(NULL, 0, 300000, 100000);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_PWM_BASE | DRV_ERROR_PARAMETER));

    ret = csi_pwm_config(pwm, 0, 300000, 300001);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_PWM_BASE | DRV_ERROR_PARAMETER));

}

void pwm_func_test()
{
    uint32_t ret = 0;
    pwm_handle_t pwm = csi_pwm_initialize(PWM_INDEX);

    ret = csi_pwm_config(pwm, PWM_CH, 20000, 5000);
    ASSERT_TRUE(ret == 0);

    printf("LED on\n");
    csi_pwm_start(pwm, PWM_CH);

    mdelay(3000);

    csi_pwm_stop(pwm, PWM_CH);

    ret = csi_pwm_config(pwm, PWM_CH, 20000, 20000);
    ASSERT_TRUE(ret == 0);

    printf("LED on\n");
    csi_pwm_start(pwm, PWM_CH);
    mdelay(3000);

    csi_pwm_stop(pwm, PWM_CH);

    ret = csi_pwm_config(pwm, PWM_CH, 20000, 1);
    ASSERT_TRUE(ret == 0);

    printf("LED off\n");
    csi_pwm_start(pwm, PWM_CH);
    mdelay(3000);

    csi_pwm_stop(pwm, PWM_CH);
    csi_pwm_uninitialize(pwm);
}

void pwm_func1_test()
{
    uint32_t ret;
    uint16_t i, full = 50000;
    pwm_handle_t pwm = csi_pwm_initialize(PWM_INDEX);
    ASSERT_TRUE(pwm != NULL);

    csi_pwm_start(pwm, PWM_CH);

    for (i = 20000; i < 50000;) {
        ret = csi_pwm_config(pwm, PWM_CH, full, i);
        ASSERT_TRUE(ret == 0);
        mdelay(10);
        i += 25;
    }

    mdelay(2000);
    csi_pwm_stop(pwm, PWM_CH);
    csi_pwm_uninitialize(pwm);
}

int test_pwm(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_pwm", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"pwm_if_test", pwm_if_test, 1},
        {"pwm_func_test", pwm_func_test, 1},
        {"pwm_func1_test", pwm_func1_test, 1},
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_pwm);
    dtest();

    return 0;
}
