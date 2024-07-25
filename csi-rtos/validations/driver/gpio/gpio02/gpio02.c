/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     gpio02.c
 * @brief    the main function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "gpio_test.h"
#include "dtest.h"

volatile static bool int_flag;

void gpio_event_cb(int idx)
{
    //printf("GPIO %d interrupt entered\n", idx);
    int_flag = 0;
}

void gpio_if_test()
{
    //initialize
    gpio_pin_handle_t pin = csi_gpio_pin_initialize(100, gpio_event_cb);
    ASSERT_TRUE(pin == NULL);

    pin = csi_gpio_pin_initialize(0, NULL);
    ASSERT_TRUE(pin != NULL);

    int32_t ret;
    //uninitialize
    ret = csi_gpio_pin_uninitialize(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_GPIO_BASE | DRV_ERROR_PARAMETER));

    //config_mode
    ret = csi_gpio_pin_config_mode(NULL, GPIO_MODE_PULLNONE);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_GPIO_BASE | DRV_ERROR_PARAMETER));

    ret = csi_gpio_pin_config_mode(pin, 5);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_GPIO_BASE | GPIO_ERROR_MODE));

    ret = csi_gpio_pin_config_mode(pin, GPIO_MODE_PULLNONE);

    //config_direction
    ret = csi_gpio_pin_config_direction(NULL, GPIO_DIRECTION_INPUT);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_GPIO_BASE | DRV_ERROR_PARAMETER));

    ret = csi_gpio_pin_config_direction(pin, 2);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_GPIO_BASE | GPIO_ERROR_DIRECTION));

    ret = csi_gpio_pin_config_direction(pin, GPIO_DIRECTION_INPUT);

    //set_irq
    ret = csi_gpio_pin_set_irq(NULL, GPIO_IRQ_MODE_RISING_EDGE, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_GPIO_BASE | DRV_ERROR_PARAMETER));

    ret = csi_gpio_pin_set_irq(pin, 5, 1);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_GPIO_BASE | GPIO_ERROR_IRQ_MODE));

    ret = csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_RISING_EDGE, 0);
    ASSERT_TRUE(ret == 0);

    ret = csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_RISING_EDGE, 1);
    ASSERT_TRUE(ret == 0);

    //write
    ret = csi_gpio_pin_write(NULL, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_GPIO_BASE | DRV_ERROR_PARAMETER));

    //read
    ret = csi_gpio_pin_read(NULL, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_GPIO_BASE | DRV_ERROR_PARAMETER));
}

void gpio_func_test()
{
    uint32_t i, ret = 0;
    gpio_pin_handle_t pin = NULL;
    int_flag = 1;
    bool value = false;

    typedef struct testcase {
        gpio_mode_e mode;
        gpio_direction_e dir;
        char *doc;
    } tc;

    tc cases[] = {
        {GPIO_MODE_PULLNONE, GPIO_DIRECTION_INPUT, "pull none for input"},
        {GPIO_MODE_PULLUP, GPIO_DIRECTION_INPUT, "pull up for input"},
        {GPIO_MODE_PULLDOWN, GPIO_DIRECTION_INPUT, "pull down for input"},
        {GPIO_MODE_OPEN_DRAIN, GPIO_DIRECTION_OUTPUT, "open drain mode for output"},
        {GPIO_MODE_PUSH_PULL, GPIO_DIRECTION_OUTPUT, "push-pull mode for output"},
        {GPIO_MODE_PULLNONE, GPIO_DIRECTION_OUTPUT, "pull none for output"},
    };

    csi_pin_set_mux(GPIO_TEST_PIN, PIN_FUNC_GPIO);

    pin = csi_gpio_pin_initialize(GPIO_TEST_PIN, gpio_event_cb);

// 测试GPIO作为外部中断
    i = 2;
    printf("%s\n", cases[i].doc);
    ret = csi_gpio_pin_config_mode(pin, cases[i].mode);
    ret = csi_gpio_pin_config_direction(pin, cases[i].dir);
#if 1
    ret = csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_FALLING_EDGE, 1);
    printf("GPIO falling edge\n");

    while (int_flag);

    printf("GPIO falling edge test passed.\n");
#endif

#if 1
    int_flag = 1;
    ret = csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_RISING_EDGE, 1);
    printf("GPIO rising edge\n");

    while (int_flag);

    printf("GPIO rising edge test passed.\n");
#endif

#if 1
    int_flag = 1;
    csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_DOUBLE_EDGE, 1);
    printf("GPIO double edge\n");

    while (int_flag);

    printf("GPIO double edge test passed.\n");
#endif

#if 1
    int_flag = 1;
    csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_LOW_LEVEL, 1);
    printf("GPIO low level\n");

    while (int_flag);

    ret = csi_gpio_pin_read(pin, &value);
    ASSERT_TRUE(ret == 0);
    printf("read value is %d\n", value);
    printf("GPIO low level test passed.\n");
#endif

#if 1
    int_flag = 1;
    csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_HIGH_LEVEL, 1);
    printf("GPIO high level\n");

    while (int_flag);

    ret = csi_gpio_pin_read(pin, &value);
    ASSERT_TRUE(ret == 0);
    printf("read value is %d\n", value);
    printf("GPIO high level test passed.\n");
#endif

//测试GPIO作为输入
#if 1
    i = 2;	//i should be 0,1,2
    printf("%s\n", cases[i].doc);
    ret = csi_gpio_pin_config_mode(pin, cases[i].mode);
    ret = csi_gpio_pin_config_direction(pin, cases[i].dir);
    csi_gpio_pin_read(pin, &value);
    printf("input %d\n", value);
    mdelay(5000);
    csi_gpio_pin_read(pin, &value);
    printf("input %d\n", value);
#endif

//测试GPIO作为输出
#if 0
    i = 5;	//	i shoule be 3,4,5
    printf("%s\n", cases[i].doc);
    ret = csi_gpio_pin_config_mode(pin, cases[i].mode);
    ret = csi_gpio_pin_config_direction(pin, cases[i].dir);
    printf("output high level\n");
    value = true;
    csi_gpio_pin_write(pin, value);
    mdelay(5000);
    printf("output low level\n");
    value = false;
    csi_gpio_pin_write(pin, value);
    mdelay(5000);
#endif
}
/*
 *	one gpio output, one gpio input
 */
void gpio_func1_test(void)
{
    uint32_t i, ret = 0;
    int32_t ret1;
    gpio_pin_handle_t pin = NULL, pin1 = NULL;
    int_flag = 1;
    bool value = false;

    typedef struct testcase {
        gpio_mode_e mode;
        gpio_direction_e dir;
        char *doc;
    } tc;

    tc cases[] = {
        {GPIO_MODE_PULLNONE, GPIO_DIRECTION_INPUT, "pull none for input"},
        {GPIO_MODE_PULLUP, GPIO_DIRECTION_INPUT, "pull up for input"},
        {GPIO_MODE_PULLDOWN, GPIO_DIRECTION_INPUT, "pull down for input"},
        {GPIO_MODE_OPEN_DRAIN, GPIO_DIRECTION_OUTPUT, "open drain mode for output"},
        {GPIO_MODE_PUSH_PULL, GPIO_DIRECTION_OUTPUT, "push-pull mode for output"},
        {GPIO_MODE_PULLNONE, GPIO_DIRECTION_OUTPUT, "pull none for output"},
    };

//测试输入中断时打开
#if 1
    ret1 = csi_pin_set_mux(GPIO_TEST_PIN, PIN_FUNC_GPIO);
    ASSERT_TRUE(ret1 == 0);
    pin = csi_gpio_pin_initialize(GPIO_TEST_PIN, gpio_event_cb);
    ASSERT_TRUE(pin != NULL);
    ret1 = csi_pin_set_mux(GPIO_PIN, GPIO_FUNC);
    ASSERT_TRUE(ret1 == 0);
    pin1 = csi_gpio_pin_initialize(GPIO_PIN, NULL);
    ASSERT_TRUE(pin1 != NULL);

    ret = csi_gpio_pin_config_mode(pin1, cases[5].mode);
    ASSERT_TRUE(ret == 0);
    ret = csi_gpio_pin_config_direction(pin1, cases[5].dir);
    ASSERT_TRUE(ret == 0);

    for (i = 0; i < 3; i++) {
        ret = csi_gpio_pin_write(pin1, true);
        ASSERT_TRUE(ret == 0);
        printf("%s\n", cases[i].doc);

        ret = csi_gpio_pin_config_mode(pin, cases[i].mode);
        ASSERT_TRUE(ret == 0);
        ret = csi_gpio_pin_config_direction(pin, cases[i].dir);
        ASSERT_TRUE(ret == 0);

        ret = csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_FALLING_EDGE, 1);
        int_flag = 1;
        printf("GPIO accept falling edge interrupt\n");
        csi_gpio_pin_write(pin1, false);

        while (int_flag);

        ret = csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_FALLING_EDGE, 0);
        ASSERT_TRUE(ret == 0);
        mdelay(1000);

        ret = csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_RISING_EDGE, 1);
        int_flag = 1;
        printf("GPIO accept rising edge interrupt\n");
        csi_gpio_pin_write(pin1, true);

        while (int_flag);

        ret = csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_RISING_EDGE, 0);
        ASSERT_TRUE(ret == 0);
        mdelay(1000);

        csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_DOUBLE_EDGE, 1);
        int_flag = 1;
        printf("GPIO accept double edge interrupt\n");
        csi_gpio_pin_write(pin1, false);

        while (int_flag);	//falling edge

        int_flag = 1;
        mdelay(10);
        csi_gpio_pin_write(pin1, true);

        while (int_flag);	//rising edge

        ret = csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_DOUBLE_EDGE, 0);
        ASSERT_TRUE(ret == 0);
        mdelay(1000);

        csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_LOW_LEVEL, 1);
        int_flag = 1;
        printf("GPIO accept low level interrupt\n");
        csi_gpio_pin_write(pin1, false);
        mdelay(10);

        while (int_flag);

        ret = csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_LOW_LEVEL, 0);
        ASSERT_TRUE(ret == 0);
        udelay(1000);

        csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_HIGH_LEVEL, 1);
        int_flag = 1;
        printf("GPIO accept high level interrupt\n");
        csi_gpio_pin_write(pin1, true);
        mdelay(10);

        while (int_flag);

        ret = csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_HIGH_LEVEL, 0);
        ASSERT_TRUE(ret == 0);

        printf("*******************************************************\n\n");
    }
#endif

//测试GPIO作为输入输出
#if 0
    ret1 = csi_pin_set_mux(GPIO_TEST_PIN, PIN_FUNC_GPIO);
    ASSERT_TRUE(ret1 == 0);
    pin = csi_gpio_pin_initialize(GPIO_TEST_PIN, NULL);
    ASSERT_TRUE(pin != NULL);
    ret1 = csi_pin_set_mux(GPIO_PIN, GPIO_FUNC);
    ASSERT_TRUE(ret1 == 0);
    pin1 = csi_gpio_pin_initialize(GPIO_PIN, NULL);
    ASSERT_TRUE(pin1 != NULL);

    ASSERT_TRUE(ret == 0);
    ret = csi_gpio_pin_config_direction(pin1, cases[5].dir);
    ASSERT_TRUE(ret == 0);

    for (i = 0; i < 3; i++) {
        ret = csi_gpio_pin_write(pin1, true);
        ASSERT_TRUE(ret == 0);
        printf("%s\n", cases[i].doc);

        ret = csi_gpio_pin_config_mode(pin, cases[i].mode);
        ASSERT_TRUE(ret == 0);
        ret = csi_gpio_pin_config_direction(pin, cases[i].dir);
        ASSERT_TRUE(ret == 0);

        printf("read high level\n");
        csi_gpio_pin_write(pin1, true);
        mdelay(10);
        ret = csi_gpio_pin_read(pin, &value);
        ASSERT_TRUE(ret == 0);
        ASSERT_TRUE(value == true);
        mdelay(1000);
        csi_gpio_pin_read(pin, &value);
        ASSERT_TRUE(value == true);

        printf("read low level\n");
        csi_gpio_pin_write(pin1, false);
        mdelay(10);
        ret = csi_gpio_pin_read(pin, &value);
        ASSERT_TRUE(ret == 0);
        ASSERT_TRUE(value == false);
        mdelay(1000);
        csi_gpio_pin_read(pin, &value);
        ASSERT_TRUE(value == false);

        printf("*******************************************************\n\n");
    }

//测试GPIO作为输出

    ret = csi_gpio_pin_config_mode(pin1, cases[0].mode);
    ASSERT_TRUE(ret == 0);
    ret = csi_gpio_pin_config_direction(pin1, cases[0].dir);
    ASSERT_TRUE(ret == 0);

    for (i = 3; i < 6; i++) {
        printf("%s\n", cases[i].doc);
        ret = csi_gpio_pin_config_mode(pin, cases[i].mode);
        ASSERT_TRUE(ret == 0);
        ret = csi_gpio_pin_config_direction(pin, cases[i].dir);
        ASSERT_TRUE(ret == 0);

        printf("output high level\n");
        ret = csi_gpio_pin_write(pin, true);
        ASSERT_TRUE(ret == 0);
        mdelay(10);
        ret = csi_gpio_pin_read(pin1, &value);
        ASSERT_TRUE(ret == 0);
        ASSERT_TRUE(value == true);
        mdelay(1000);
        uint32_t i = 0;

        for (i = 0; i < 20000; i++) {
            csi_gpio_pin_read(pin1, &value);

            if (!value) {
                printf("++++error:%d\n", i);
            }
        }

        csi_gpio_pin_read(pin1, &value);
        ASSERT_TRUE(value == true);

        mdelay(1000);
        printf("output low level\n");
        ret = csi_gpio_pin_write(pin, false);
        ASSERT_TRUE(ret == 0);
        mdelay(10);
        /*
        uint32_t i=0;
        while (1) {
        ret = csi_gpio_pin_read(pin1,&value);
        ASSERT_TRUE(ret == 0);
        if (value)
        	printf("++++error:%d\n",i);
        i++;
        }
        */
        ret = csi_gpio_pin_read(pin1, &value);
        ASSERT_TRUE(ret == 0);
        ASSERT_TRUE(value == false);
        mdelay(1000);

        for (i = 0; i < 20000; i++) {
            csi_gpio_pin_read(pin1, &value);

            if (value) {
                printf("++++error:%d\n", i);
            }
        }

        csi_gpio_pin_read(pin1, &value);
        ASSERT_TRUE(value == false);
    }
#endif

    ret = csi_gpio_pin_uninitialize(pin);
    ASSERT_TRUE(ret == 0);
    ret = csi_gpio_pin_uninitialize(pin1);
    ASSERT_TRUE(ret == 0);
}

int test_gpio(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_gpio", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
#if 0
        {"gpio_if_test", gpio_if_test, 1},
        {"gpio_func_test", gpio_func_test, 1},
#endif
        {"gpio_func1_test", gpio_func1_test, 1},
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_gpio);
    dtest();

    return 0;
}
