/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#include "pin.h"
#define csi_usart_initialize       drv_spu_usart_initialize
#define csi_usart_uninitialize     drv_spu_usart_uninitialize
#define csi_usart_get_capabilities drv_spu_usart_get_capabilities
#define csi_usart_config           drv_spu_usart_config
#define csi_usart_send             drv_spu_usart_send
#define csi_usart_abort_send       drv_spu_usart_abort_send
#define csi_usart_receive          drv_spu_usart_receive
#define csi_usart_receive_query    drv_spu_usart_receive_query
#define csi_usart_abort_receive    drv_spu_usart_abort_receive
#define csi_usart_transfer         drv_spu_usart_transfer
#define csi_usart_abort_transfer   drv_spu_usart_abort_transfer
#define csi_usart_get_status       drv_spu_usart_get_status
#define csi_usart_flush            drv_spu_usart_flush
#define csi_usart_set_interrupt    drv_spu_usart_set_interrupt
#define csi_usart_config_baudrate  drv_spu_usart_config_baudrate
#define csi_usart_config_mode      drv_spu_usart_config_mode
#define csi_usart_config_parity    drv_spu_usart_config_parity
#define csi_usart_config_stopbits  drv_spu_usart_config_stopbits
#define csi_usart_config_databits  drv_spu_usart_config_databits
#define csi_usart_getchar          drv_spu_usart_getchar
#define csi_usart_putchar          drv_spu_usart_putchar
#define csi_usart_get_tx_count     drv_spu_usart_get_tx_count
#define csi_usart_get_rx_count     drv_spu_usart_get_rx_count
#define csi_usart_power_control    drv_spu_usart_power_control
#define csi_usart_config_flowctrl  drv_spu_usart_config_flowctrl
#define csi_usart_config_clock     drv_spu_usart_config_clock
#define csi_usart_control_tx       drv_spu_usart_control_tx
#define csi_usart_control_rx       drv_spu_usart_control_rx
#define csi_usart_control_break    drv_spu_usart_control_break
#define test_usart                 test_spu_usart

#undef TEST_PIN_USART_TX
#undef TEST_PIN_USART_RX
#undef TEST_PIN_USART_TX_FUNC
#undef TEST_PIN_USART_RX_FUNC
#undef TEST_USART_IDX

#define TEST_USART_IDX        TEST_SPU_USART_IDX
#define TEST_PIN_USART_TX      TEST_PIN_SPU_USART_TX
#define TEST_PIN_USART_RX      TEST_PIN_SPU_USART_RX
#define TEST_PIN_USART_TX_FUNC TEST_PIN_SPU_USART_TX_FUNC
#define TEST_PIN_USART_RX_FUNC TEST_PIN_SPU_USART_RX_FUNC

#include "../../usart/usart01.c"
