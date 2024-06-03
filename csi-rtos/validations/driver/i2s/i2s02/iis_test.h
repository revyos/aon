/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iis_test.h
 * @brief    the head file
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#ifndef IIS_HEADER
#define IIS_HEADER

#include <drv/i2s.h>
#include <stdio.h>
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

typedef struct init_testcase_t {
    int32_t idx;
    i2s_event_cb_t cb_event;
    uint8_t y_n;	//1- the testcase is true;0- the testcase is false
} init_testcase;

typedef struct cfgprot_testcase_t {
    i2s_handle_t handle;
    i2s_protocol_e protocol;
    uint8_t y_n;    //1- the testcase is true;0- the testcase is false
} cfgprot_testcase;

typedef struct cfgsampwid_testcase_t {
    i2s_handle_t handle;
    uint32_t samplewidth;
    uint8_t y_n;    //1- the testcase is true;0- the testcase is false
} cfgsampwid_testcase;

typedef struct cfgworkmod_testcase_t {
    i2s_handle_t handle;
    i2s_mode_e mode;
    uint8_t y_n;    //1- the testcase is true;0- the testcase is false
} cfgworkmod_testcase;

typedef struct cfgsamprat_testcase_t {
    i2s_handle_t handle;
    uint32_t rate;
    uint8_t y_n;    //1- the testcase is true;0- the testcase is false
} cfgsamprat_testcase;

typedef struct send_testcase_t {
    i2s_handle_t handle;
    const void *data;
    uint32_t data_size;
    uint8_t y_n;    //1- the testcase is true;0- the testcase is false
} send_testcase;

typedef struct recv_testcase_t {
    i2s_handle_t handle;
    void *data;
    uint32_t data_size;
    uint8_t y_n;    //1- the testcase is true;0- the testcase is false
} recv_testcase;

typedef struct porcont_testcase_t {
    i2s_handle_t handle;
    csi_power_stat_e state;
    uint8_t y_n;    //1- the testcase is true;0- the testcase is false
} porcont_testcase;

#endif
