/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     eflash02.c
 * @brief    the main function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "eflash_test.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dtest.h"

void eflash_cb_func()
{

}

void eflash_if_test()
{
    //get capabilities
    eflash_capabilities_t cap = csi_eflash_get_capabilities(2);
    ASSERT_TRUE(cap.event_ready == 0);

    //get status
    eflash_status_t status;
    status = csi_eflash_get_status(NULL);
    ASSERT_TRUE(status.busy == 0);
    ASSERT_TRUE(status.error == 0);

    //initialize
    eflash_handle_t eflash = csi_eflash_initialize(2, eflash_cb_func);
    ASSERT_TRUE(eflash == NULL);

    eflash = csi_eflash_initialize(0, eflash_cb_func);
    ASSERT_TRUE(eflash != NULL);
    csi_eflash_uninitialize(eflash);
    eflash = csi_eflash_initialize(0, NULL);
    ASSERT_TRUE(eflash != NULL);

    int32_t ret;
    //power control
    ret = csi_eflash_power_control(NULL, DRV_POWER_OFF);
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_EFLASH_BASE|DRV_ERROR_PARAMETER));
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_UNSUPPORTED));
    ret = csi_eflash_power_control(eflash, 10);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_UNSUPPORTED));

    //uninitialize
    ret = csi_eflash_uninitialize(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER));

    //get_info
    eflash_info_t *info = NULL;
    info = csi_eflash_get_info(NULL);
    ASSERT_TRUE(info == NULL);

    info = csi_eflash_get_info(eflash);
//  printf("info->start: %x",info->start);
//  printf("info->end: %x",info->end);

    //erase sector
    ret = csi_eflash_erase_sector(NULL, info->start);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER));

    ret = csi_eflash_erase_sector(eflash, info->start);
    ASSERT_TRUE(ret == 0);

    char data[info->sector_size];
    memset(data, 0x1, info->sector_size);

    //erase chip
    ret = csi_eflash_erase_chip(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER));

    //program
    ret = csi_eflash_program(NULL, info->start, data, info->sector_size);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER));

    ret = csi_eflash_program(eflash, info->start, NULL, info->sector_size);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER));

    ret = csi_eflash_program(eflash, info->start, data, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER));

    ret = csi_eflash_program(eflash, info->start, data, info->sector_size);

    //read
    ret = csi_eflash_read(NULL, info->start, data, info->sector_size);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER));

    ret = csi_eflash_read(eflash, info->start, NULL, info->sector_size);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER));

    ret = csi_eflash_read(eflash, info->start, data, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER));

    csi_eflash_uninitialize(eflash);

}

void eflash_func_test()
{
    int i = 0, ret;
    uint32_t operate_addr;
    eflash_handle_t eflash;

    eflash = csi_eflash_initialize(1, NULL);
    ASSERT_TRUE(eflash != NULL);

    eflash_info_t *info;

    info = csi_eflash_get_info(eflash);
    printf("start addr:0x%x\n", info->start);
    printf("end addr:0x%x\n", info->end);
    printf("Sector count:%u\n", info->sector_count);
    printf("Sector size:%u\n", info->sector_size);
    printf("Page size:%u\n", info->page_size);
    printf("Program unit:%u\n", info->program_unit);
    printf("Erased value:%u\n", info->erased_value);

    char *data, *out, erase_value[info->sector_size];

    memset(erase_value, info->erased_value, info->sector_size);

    /****************  Case-0 eflash erase   ************/
    operate_addr = info->start;
    out = (char *) malloc(info->sector_size);

    for (i = 0; i < info->sector_count; i++) {
        ret = csi_eflash_erase_sector(eflash, operate_addr);
        ASSERT_TRUE(ret == 0);
        ret = csi_eflash_read(eflash, operate_addr, out, info->sector_size);
        ASSERT_TRUE(ret == info->sector_size);
        /*because of in security,hobbit1_2 don't supported */
        ASSERT_TRUE(memcmp(erase_value, out, info->sector_size) == 0);

        operate_addr += info->sector_size;
    }

    /*************** Case-1 eflash write data  ************/
    data = (char *)malloc(info->sector_size);
    memset(data, 0x1, info->sector_size);
    operate_addr = info->start;
    ret = csi_eflash_program(eflash, operate_addr, data , info->sector_size);
    ASSERT_TRUE(ret == info->sector_size);

    /*****************  Case-2 Read data *************/
    ret = csi_eflash_read(eflash, operate_addr, out, info->sector_size);
    ASSERT_TRUE(ret == info->sector_size);
    ASSERT_TRUE(memcmp(data, out, info->sector_size) == 0);


    /***************** erase chip **********************/
    operate_addr = info->start + (60 * info->sector_size);
    ret = csi_eflash_program(eflash, operate_addr, data , info->sector_size);
    ASSERT_TRUE(ret == info->sector_size);
    ret = csi_eflash_read(eflash, operate_addr, out, info->sector_size);
    ASSERT_TRUE(ret == info->sector_size);
    ASSERT_TRUE(memcmp(data, out, info->sector_size) == 0);

    ret = csi_eflash_erase_chip(eflash);
    ASSERT_TRUE(ret == 0);

    ret = csi_eflash_read(eflash, operate_addr, out, info->sector_size);
    ASSERT_TRUE(ret == info->sector_size);
    ASSERT_TRUE(memcmp(data, out, info->sector_size) != 0);

    /************* Case-3  Write data without erasing  ***********/
    /*memset(data, 0x2, info->sector_size);
    ret = csi_eflash_program(eflash, operate_addr, data , info->sector_size);
    check_false_ret(ret, 3, casename);*/

    /************** Case-4  erase prgrame read for 1000 times  *********/
    for (i = 0; i < 1000; i++) {

        ret = csi_eflash_erase_sector(eflash, operate_addr);
        ASSERT_TRUE(ret == 0);
        ret = csi_eflash_program(eflash, operate_addr, data , info->sector_size);
        ASSERT_TRUE(ret == info->sector_size);
        ret = csi_eflash_read(eflash, operate_addr, out, info->sector_size);
        ASSERT_TRUE(ret == info->sector_size);

        ASSERT_TRUE(memcmp(data, out, info->sector_size) == 0);
    }

    /*********** Case-5  erase overflow  **************************/
    operate_addr = info->start + info->sector_count * info->sector_size * 4 ;
    ret = csi_eflash_erase_sector(eflash, operate_addr);
    ASSERT_TRUE(ret != 0);

    /********** Case-6 Write overflow  **************************/
    ret = csi_eflash_program(eflash, operate_addr, data , info->sector_size);
    ASSERT_TRUE(ret != 0);

    /************  Case-7 Read overflow  ***********************/
    ret = csi_eflash_read(eflash, operate_addr, out, info->sector_size);
    ASSERT_TRUE(ret != 0);

    /**************** Preparing data   *****************************/
    free(data);
    free(out);

    operate_addr = info->start + info->sector_size / 2;
    data = (char *)malloc(info->sector_size + 4);
    memset(data, 0x2, info->sector_size + 4);
    out = (char *)malloc(info->sector_size + 4);

    /*****************  Erase sector  ************************/
    csi_eflash_erase_sector(eflash, info->start);
    csi_eflash_erase_sector(eflash, info->start + info->sector_size);

    /************* Case-8 Write over sector  *************************/
    ret = csi_eflash_program(eflash, operate_addr, data , info->sector_size + 4);
    ASSERT_TRUE(ret == info->sector_size + 4);

    /************* Case-9 Read over sector  *************************/
    ret = csi_eflash_read(eflash, operate_addr, out, info->sector_size + 4);
    ASSERT_TRUE(ret == info->sector_size + 4);
    ASSERT_TRUE(memcmp(data, out, info->sector_size + 4) == 0);

    free(data);
    free(out);

    csi_eflash_uninitialize(eflash);
    printf("Test finished\n");
}


int test_eflash(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_eflash", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"eflash_if_test", eflash_if_test, 1},
        {"eflash_func_test", eflash_func_test, 1},
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_eflash);
    dtest();

    return 0;
}
