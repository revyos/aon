/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


#include "drv/i2s.h"
#include "stdio.h"
#include "soc.h"
#include "dtest.h"
#include "pin.h"
#include "test_driver_config.h"
#include <csi_config.h>

typedef struct {
    i2s_config_t *config;
    int32_t expect_out;
} i2s_test_t;

#define TEST_I2S_SIZE  (480)
static i2s_handle_t i2s_handle;
static i2s_handle_t i2s_handle_master;
static i2s_handle_t i2s_handle_slave;
static uint8_t data_recv[TEST_I2S_SIZE * 4]  __attribute__ ((aligned(4)));
static uint8_t data_send[TEST_I2S_SIZE * 4]  __attribute__ ((aligned(4)));

static uint32_t i2s_rx_buf[1024 * 4];
static uint8_t i2s_tx_buf[1024 * 4];

static i2s_config_t i2s_config[] = {
    {.cfg.left_polarity = I2S_LEFT_POLARITY_LOW, .cfg.mclk_freq = I2S_MCLK_256FS,
     .cfg.mode = I2S_MODE_TX_MASTER, .cfg.protocol = I2S_PROTOCOL_I2S - 1,
     .cfg.rx_mono_enable = 0, .cfg.rx_mono_select_ch = I2S_RX_RIGHT_CHANNEL,
     .cfg.sclk_freq = I2S_SCLK_16FS, .cfg.tx_mono_enable = 0,
     .cfg.width = I2S_SAMPLE_16BIT, .rate = 16000,
     .tx_period = 1024, .tx_buf = (uint8_t *)i2s_tx_buf,
     .tx_buf_length = 2048,
    },
    {.cfg.left_polarity = I2S_LEFT_POLARITY_LOW, .cfg.mclk_freq = I2S_MCLK_256FS,
     .cfg.mode = I2S_MODE_TX_MASTER - 1, .cfg.protocol = I2S_PROTOCOL_I2S,
     .cfg.rx_mono_enable = 0, .cfg.rx_mono_select_ch = I2S_RX_RIGHT_CHANNEL,
     .cfg.sclk_freq = I2S_SCLK_16FS, .cfg.tx_mono_enable = 0,
     .cfg.width = I2S_SAMPLE_16BIT, .rate = 16000,
     .tx_period = 1024, .tx_buf = (uint8_t *)i2s_tx_buf,
     .tx_buf_length = 2048,
     },
    {.cfg.left_polarity = I2S_LEFT_POLARITY_LOW, .cfg.mclk_freq = I2S_MCLK_256FS,
     .cfg.mode = I2S_MODE_TX_MASTER, .cfg.protocol = I2S_PROTOCOL_I2S,
     .cfg.rx_mono_enable = 0, .cfg.rx_mono_select_ch = I2S_RX_RIGHT_CHANNEL,
     .cfg.sclk_freq = I2S_SCLK_16FS, .cfg.tx_mono_enable = 0,
     .cfg.width = I2S_SAMPLE_16BIT, .rate = 16000,
     .tx_period = 1024, .tx_buf = (uint8_t *)i2s_tx_buf,
     .tx_buf_length = 2048,
    },
};

static i2s_test_t i2s_config_cases[] = {
    {&i2s_config[0], CSI_DRV_ERRNO_I2S_BASE | I2S_ERROR_PROTOCOL},
    {&i2s_config[1], CSI_DRV_ERRNO_I2S_BASE | DRV_ERROR_PARAMETER},
    {&i2s_config[2], 0},
};

static void test_pin_i2s_init(void)
{
    csi_pin_set_mux(TEST_I2S_MCLK, TEST_I2S_MCLK_FUNC);
    csi_pin_set_mux(TEST_I2S_SCLK, TEST_I2S_SCLK_FUNC);
    csi_pin_set_mux(TEST_I2S_WSCLK, TEST_I2S_WSCLK_FUNC);
    csi_pin_set_mux(TEST_I2S_SDA, TEST_I2S_SDA_FUNC);

    csi_pin_set_mux(TEST_I2S_SLAVE_MCLK,  TEST_I2S_SLAVE_MCLK_FUNC);
    csi_pin_set_mux(TEST_I2S_SLAVE_SCLK,  TEST_I2S_SLAVE_SCLK_FUNC);
    csi_pin_set_mux(TEST_I2S_SLAVE_WSCLK, TEST_I2S_SLAVE_WSCLK_FUNC);
    csi_pin_set_mux(TEST_I2S_SLAVE_SDA,   TEST_I2S_SLAVE_SDA_FUNC);
}

static void test_csi_i2s_config(void)
{

    test_pin_i2s_init();

    i2s_handle = csi_i2s_initialize(TEST_I2S_IDX, NULL, NULL);
    ASSERT_TRUE(i2s_handle != NULL);

    int32_t ret;
    uint32_t i;

    for (i = 0; i < sizeof(i2s_config_cases) / (sizeof(i2s_test_t)); i++) {
        ret = csi_i2s_config(i2s_handle, i2s_config_cases[i].config);
        ASSERT_TRUE(ret == i2s_config_cases[i].expect_out);
    }
}

static uint8_t volatile i2s_recv_flag = 0;
static uint8_t volatile i2s_send_flag = 0;

static void i2s_event_cb_fun(int32_t idx, i2s_event_e event, void *arg)
{
    if (event == I2S_EVENT_RECEIVE_COMPLETE || event == I2S_EVENT_RX_BUFFER_FULL) {
        i2s_recv_flag = 1;
    }

    if (event == I2S_EVENT_SEND_COMPLETE || event == I2S_EVENT_TX_BUFFER_EMPYT) {
        i2s_send_flag = 1;
    }
}

static void test_i2s_loopback(void)
{
    test_pin_i2s_init();
    i2s_handle_master = csi_i2s_initialize(TEST_I2S_IDX, i2s_event_cb_fun, NULL);
    ASSERT_TRUE(i2s_handle_master != NULL);

    i2s_handle_slave = csi_i2s_initialize(TEST_I2S_SLAVE_IDX, i2s_event_cb_fun, NULL);
    ASSERT_TRUE(i2s_handle_slave != NULL);

    int32_t ret;
    i2s_config_t i2s_tx_config;
    memset(&i2s_tx_config, 0, sizeof(i2s_tx_config));
    i2s_tx_config.cfg.left_polarity = I2S_LEFT_POLARITY_LOW;
    i2s_tx_config.cfg.mclk_freq = I2S_MCLK_256FS;
    i2s_tx_config.cfg.mode = I2S_MODE_TX_MASTER;
    i2s_tx_config.cfg.protocol = I2S_PROTOCOL_I2S;
    i2s_tx_config.cfg.sclk_freq = I2S_SCLK_32FS;
    i2s_tx_config.cfg.tx_mono_enable = 0;
    i2s_tx_config.cfg.width = I2S_SAMPLE_16BIT;
    i2s_tx_config.rate = 16000;
    i2s_tx_config.tx_buf = (uint8_t *)i2s_tx_buf;
    i2s_tx_config.tx_buf_length = 1024 * 4;
    i2s_tx_config.tx_period = 480 * 2;

    ret = csi_i2s_config(i2s_handle_master, &i2s_tx_config);
    ASSERT_TRUE(ret == 0);

    i2s_config_t i2s_rx_config;

    memset(&i2s_rx_config, 0, sizeof(i2s_rx_config));

    i2s_rx_config.cfg.left_polarity = I2S_LEFT_POLARITY_LOW;
    i2s_rx_config.cfg.mclk_freq = I2S_MCLK_256FS;
    i2s_rx_config.cfg.mode = I2S_MODE_RX_SLAVE;
    i2s_rx_config.cfg.protocol = I2S_PROTOCOL_I2S;
    i2s_rx_config.cfg.sclk_freq = I2S_SCLK_32FS;
    i2s_rx_config.cfg.rx_mono_enable = 0;
    i2s_rx_config.cfg.width = I2S_SAMPLE_16BIT;
    i2s_rx_config.rate = 16000;
    i2s_rx_config.rx_buf = (uint8_t *)i2s_rx_buf;
    i2s_rx_config.rx_buf_length = 1024 * 4;
    i2s_rx_config.rx_period = 480 * 2;
    ret = csi_i2s_config(i2s_handle_slave, &i2s_rx_config);
    ASSERT_TRUE(ret == 0);

    uint32_t i = 0;
    uint8_t *tx = data_send;
    uint8_t *rx = data_recv;
    for (i = 0; i < TEST_I2S_SIZE; i++) {
        tx[i] = i + 1;
        rx[i] = 0;
    }

    csi_i2s_enable(i2s_handle_slave, 1);
    csi_i2s_enable(i2s_handle_master, 1);

    csi_i2s_send(i2s_handle_master, (uint8_t *)data_send, TEST_I2S_SIZE);
    csi_i2s_receive(i2s_handle_slave, (uint8_t *)data_recv, TEST_I2S_SIZE);

    while(!i2s_send_flag);
    while(!i2s_recv_flag);
    csi_i2s_receive(i2s_handle_slave, (uint8_t *)data_recv, TEST_I2S_SIZE);

    uint32_t j;
    i = 0;
    for (i = 0; i < TEST_I2S_SIZE; i++) {
        if (data_recv[i] && (data_recv[i] + 1 == data_recv[i + 1]) 
            && (data_recv[i] + 2 == data_recv[i + 2])) {
            break;
        }
    }

    for (j = 0; j < TEST_I2S_SIZE; j++) {
        if (data_recv[i] == data_send[j]) {
            break;
        }
    }

    while(j < TEST_I2S_SIZE && i < TEST_I2S_SIZE) {
        if (data_recv[i] != data_send[j]) {
            printf("i2s function test faild\n");
            return;
        }
        j++;
        i++;
    }

    printf("i2s function test OK\n");
}

int test_i2s()
{
    dtest_suite_info_t test_suite_info = {
        "test_i2s", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        { "test_csi_i2s_config", test_csi_i2s_config, I2S_TEST_INTERFACE_EN },
        { "test_csi_i2s_loopback", test_i2s_loopback, I2S_TEST_LOOPBACK_EN },
        { NULL, NULL }
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_i2s);
    dtest();

    return 0;
}

