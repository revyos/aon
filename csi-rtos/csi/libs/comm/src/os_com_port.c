/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     sys_console.c
 * @brief    CSI Source File for console
 * @version  V1.0
 * @date     2019-12-19
 ******************************************************************************/
#include "os_com_service.h"
#include "os_com_port.h"
#include <stdio.h>
#include <drv/uart.h>
#ifdef csi2
#include <drv/usi_usart.h>
#endif
#include "board_config.h"
#include <soc.h>
//#include <drv/drv_usart.h>
//#include <drv/drv_common.h>
//#include <drv/drv_errno.h>
//#include <drv/errno.h>


static os_com_service_tx_func_t  g_os_com_ser_txs[] =
{
#if OS_COM_RS232
  os_com_rs232_transmit,
#else
  NULL,
#endif
#if OS_COM_JTAG
  os_com_jtag_transmit,
#else
  NULL,
#endif
#if OS_COM_USB
  os_com_usb_transmit,
#else
  NULL,
#endif
#if OS_COM_TCPIP
  os_com_tcpip_transmit,
#else
  NULL,
#endif
};


os_com_service_tx_func_t
os_com_get_tx_func (enum os_com_type_t type)
{
  return g_os_com_ser_txs[type];
}

#if OS_COM_RS232
#ifdef csi1
static usart_handle_t phandle;
#endif
static csi_uart_t phandle;
static char rdata = 0;

#ifdef csi1
void os_com_event_cb(int32_t idx, usart_event_e event)
{
    if (event == USART_EVENT_RECEIVE_COMPLETE)
        {
            os_com_service_rx_data (OS_COM_TYPE_RS232, (uint8_t *)&rdata, 1);
            csi_usart_receive(phandle, &rdata, 1);
        }
}
#endif

void os_com_event_cb(csi_uart_t *uart,csi_uart_event_t event,void *arg)
{
    if (event == UART_EVENT_RECEIVE_COMPLETE)
        {
            os_com_service_rx_data (OS_COM_TYPE_RS232, (uint8_t *)&rdata, 1);
            csi_uart_receive_async(&phandle, &rdata, 1);
        }
}
#ifdef csi1
int32_t os_com_rs232_init (void)
{
    int32_t ret;
    phandle = csi_usart_initialize(COMM_IDX, os_com_event_cb);
    if (phandle == NULL) {
        printf("csi_usart_initialize error\n");
        return 1;
    }

    ret = csi_usart_config(phandle, OS_COM_RS232_BAUDRATE,
                           USART_MODE_ASYNCHRONOUS, OS_COM_RS232_PARITY,
                           OS_COM_RS232_STOP, OS_COM_RS232_BITS);

    if (ret < 0) {
        printf("csi_usart_config error %x\n", ret);
        return -1;
    }
    csi_usart_receive(phandle, &rdata, 1);

    return ret;
}
#endif

csi_error_t os_com_rs232_init(void)
{
	csi_error_t ret;
	char testdata[6] = "hello";
	ret = csi_uart_init(&phandle,TEST_USART_IDX);
	if(ret != CSI_OK)
	{
		printf("csi_usart_initialize error\n");
		return CSI_ERROR;
	}
	ret = csi_uart_attach_callback(&phandle,os_com_event_cb,NULL);
	if(ret != CSI_OK)
	{
		printf("csi_usart_initialize error\n");
		return CSI_ERROR;
	}
	ret = csi_uart_baud(&phandle,OS_COM_RS232_BAUDRATE);
	if(ret != CSI_OK)
	{
		printf("csi_usart_initialize error\n");
		return CSI_ERROR;
	}
	
	ret = csi_uart_format(&phandle,OS_COM_RS232_BITS,OS_COM_RS232_PARITY,OS_COM_RS232_STOP);
	if(ret != CSI_OK)
	{
		printf("csi_usart_initialize error\n");
		return CSI_ERROR;
	}
	ret = csi_uart_receive_async(&phandle,&rdata,1);
	if(ret != CSI_OK)
	{
		printf("csi_usart_initialize error\n");
		return CSI_ERROR;
	}
	ret = csi_uart_send_async(&phandle,&testdata,6);
	if(ret != CSI_OK)
	{
		printf("csi_uart_send_async error\n");
		return CSI_ERROR;	
	}
	return ret;
}

#ifdef csi1
int32_t os_com_rs232_transmit(uint8_t*pdata, uint32_t nbytes)
{
    int time_out = 0x7ffff;
    usart_status_t status;

    csi_usart_send(phandle, pdata, nbytes);

    while (time_out) {
        time_out--;
        status = csi_usart_get_status(phandle);

        if (!status.tx_busy) {
            break;
        }
    }
    if (0 == time_out) {
        return -1;
    }

    return 0;
}
#endif

int32_t os_com_rs232_transmit(uint8_t*pdata,uint32_t nbytes)
{
	int time_out = 0x7ffff;	
	csi_state_t status;
	csi_uart_send_async(&phandle,pdata,nbytes);
	
	while(time_out)
	{
		time_out--;
		csi_uart_get_state(&phandle,&status);
		if(!status.writeable)
		{
			break;
		}
	}
	if(0 == time_out)
	{
		return -1;
	}
	return 0;
}
#endif
