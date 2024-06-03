/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     test_eth.c
 * @brief    the main function for the ETH driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#include <csi_config.h>
#include "drv/spi.h"
#include "stdio.h"
#include "soc.h"
#include <string.h>
#include "dtest.h"

#include "test_driver_config.h"

#ifndef CONFIG_KERNEL_NONE
#include "csi_kernel.h"
#endif
#include "pin.h"
#include "drv/spi.h"
#include "drv/gpio.h"
#include "drv/eth.h"
#include "drv/eth_phy.h"
#include "drv/eth_mac.h"

/* ******* ARP ******* */
#define ETH_ARP_OPCODE_REPLY_H_V 0x0
#define ETH_ARP_OPCODE_REPLY_L_V 0x02

#define ETHTYPE_ARP_L_V          0x06
/* arp.dst.ip */
#define ETH_ARP_DST_IP_P         0x26
/* arp.opcode */
#define ETH_ARP_OPCODE_H_P       0x14
#define ETH_ARP_OPCODE_L_P       0x15
/* arp.src.mac */
#define ETH_ARP_SRC_MAC_P        0x16
#define ETH_ARP_SRC_IP_P         0x1c
#define ETH_ARP_DST_MAC_P        0x20
#define ETH_ARP_DST_IP_P         0x26
#define BUFFER_SIZE 1518
static unsigned char bufs[BUFFER_SIZE];


eth_mac_handle_t    g_test_mac_handle;
eth_phy_handle_t    g_test_phy_handle;

static unsigned char wwwport = 80;
static unsigned char macaddr[6];
static unsigned char ipaddr[4];

static unsigned char mymac[6] = {0x00, 0x11, 0x22, 0x60, 0x94, 0x2d};
static unsigned char myip[4] = {192, 168, 103, 159};

static unsigned int mywwwport = 1879;
static int enc28j60_link_stat = 0;
static eth_frame_filter_t frame_filters = {0};
static eth_frame_filter_list_t frame_filter_lists = {0};
static uint16_t read_data;

struct nic_msg {
    struct netif *netif;
    uint32_t event;
};

static volatile int interrupt_count = 0;

eth_mac_time_t mac_time = {
    .ns = 0,
    .sec = 1,
};

int32_t test_fn_phy_read(uint8_t phy_addr, uint8_t reg_addr, uint16_t *data)
{

    return 0;
}

int32_t test_fn_phy_write(uint8_t phy_addr, uint8_t reg_addr, uint16_t  data)
{

    return 0;
}


static void eth_mac_signal_event(uint32_t idx, uint32_t event)
{
    if (event == CSI_ETH_MAC_EVENT_RX_FRAME) {
        interrupt_count = 1;
    } else {
        interrupt_count = 2;
    }
}

static void net_test_interfaces(void)
{
    int32_t ret;

    g_test_phy_handle = csi_eth_phy_initialize(test_fn_phy_read, test_fn_phy_write);
    ASSERT_TRUE(g_test_phy_handle != NULL);

    enc28j60_link_stat = csi_eth_phy_get_linkstate(g_test_phy_handle);
    ASSERT_TRUE(g_test_phy_handle != NULL);

    csi_drv_version_t versions = csi_eth_phy_get_version(g_test_phy_handle);
    ASSERT_TRUE(versions.drv != 0xff);

    ret = csi_eth_phy_uninitialize(g_test_phy_handle);
    ASSERT_TRUE(ret == 0);

    ret = csi_eth_phy_power_control(g_test_phy_handle, 0);
    ASSERT_TRUE(ret == 0);

    ret = csi_eth_phy_set_interface(g_test_phy_handle, 0);
    ASSERT_TRUE(ret == 0);

    ret = csi_eth_phy_set_mode(g_test_phy_handle, 0);
    ASSERT_TRUE(ret == 0);

    g_test_mac_handle = csi_eth_mac_initialize(0, (eth_event_cb_t)eth_mac_signal_event);
    ASSERT_TRUE(g_test_mac_handle != NULL);

    eth_link_info_t info_link = csi_eth_phy_get_linkinfo(g_test_phy_handle);
    ASSERT_TRUE(info_link.speed != 0);

    csi_drv_version_t eht_mac_version = csi_eth_mac_get_version(g_test_mac_handle);
    ASSERT_TRUE(eht_mac_version.drv != 0xff);

    eth_capabilities_t capab = csi_eth_mac_get_capabilities(0);
    ASSERT_TRUE(capab.mac_address != 0);

    ret =  csi_eth_mac_uninitialize(g_test_mac_handle);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_power_control(g_test_mac_handle, 0);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_get_macaddr(g_test_mac_handle, (eth_mac_addr_t *)mymac);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_set_macaddr(g_test_mac_handle, (eth_mac_addr_t *)mymac);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_set_addrfilter(g_test_mac_handle, (eth_mac_addr_t *)mymac, 0);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_send_frame(g_test_mac_handle, bufs, BUFFER_SIZE, 0);
    ASSERT_TRUE(ret != 0);

    ret =  csi_eth_mac_read_frame(g_test_mac_handle, bufs, BUFFER_SIZE);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_get_rx_framesize(g_test_mac_handle);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_get_rx_frametime(g_test_mac_handle, &mac_time);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_get_tx_frametime(g_test_mac_handle, &mac_time);
    ASSERT_TRUE(ret == 0);

    ret = csi_eth_mac_control(g_test_mac_handle, CSI_ETH_MAC_CONTROL_RX, 0);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_control_time(g_test_mac_handle, CSI_ETH_MAC_CONTROL_RX, &mac_time);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_phy_read(g_test_mac_handle, 0, 0, &read_data);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_phy_write(g_test_mac_handle, 0, 0, 0x55);
    ASSERT_TRUE(ret == 0);

    csi_eth_mac_signal_event(g_test_mac_handle, CSI_ETH_MAC_EVENT_RX_FRAME);
    ASSERT_TRUE(g_test_mac_handle != NULL);

    ret =  csi_eth_mac_add_framefilter(g_test_mac_handle, (const eth_frame_filter_t *)&frame_filters);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_remove_framefilter(g_test_mac_handle, 0);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_en_framefilter(g_test_mac_handle, 0, 0);
    ASSERT_TRUE(ret == 0);

    ret =  csi_eth_mac_get_framefilter(g_test_mac_handle, &frame_filter_lists, NULL, 0);
    ASSERT_TRUE(ret == 0);

    printf("net_test_interfaces OK\n");
}


/****************************************************************************
 * spi_net_test_main
 ****************************************************************************/

static void send_data(void)
{
    int i;
    unsigned char net_change[60] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x11, 0x22, 0x60, 0x94, 0x2d, 0x8, 0x6,
                                    0x0, 0x1, 0x8, 0x0, 0x6, 0x4, 0x0, 0x1, 0x00, 0x11, 0x22, 0x60, 0x94, 0x2d, 0xc0, 0xa8, 0x67, 0x9f, 0x3b, 0xc8,
                                    0x50, 0x10, 0xfa, 0xf0, 0xc0, 0xa8, 0x67, 0x73, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                    0x0, 0x0, 0x0, 0x0, 0x0, 0x0
                                   };

    csi_eth_mac_send_frame(g_test_mac_handle, net_change, 60, 0);

    printf("\n");
    printf("SEND DATA : \n");

    for (i = 0; i < 60; i++) {
        printf("0x%x ", net_change[i]);

        if ((i + 1) % 20 == 0) {
            printf("\n");
        }
    }

    printf("\n");
}

static void make_answer_from_request(unsigned char *buf, uint16_t length)
{
    unsigned char i = 0;
    unsigned char net_change[6];

    //MAC change
    for (i = 0; i < 6; i++) {
        net_change[i] = buf[6 + i];
        buf[6 + i]      = buf[i];
        buf[i]        = net_change[i];
    }

    for (i = 0; i < 6; i++) {
        buf[6 + i]      = mymac[i];
    }

    memset(net_change, 0, sizeof(net_change));

    //IP change
    for (i = 0; i < 4; i++) {
        net_change[i]   = buf[ETH_ARP_SRC_IP_P + i];
        buf[ETH_ARP_SRC_IP_P + i] = buf[ETH_ARP_DST_IP_P + i];
        buf[ETH_ARP_DST_IP_P + i] = net_change[i];
    }

    for (i = 0; i < 4; i++) {
        buf[ETH_ARP_DST_IP_P + i] = myip[i];
    }

    csi_eth_mac_send_frame(g_test_mac_handle, net_change, length, 0);

    printf("\n");
    printf("SEND DATA : \n");

    for (i = 0; i < length; i++) {
        printf("0x%x ", buf[i]);

        if ((i + 1) % 20 == 0) {
            printf("\n");
        }
    }

    printf("\n");
    printf("Network send OK \n");
}

static void init_ip_arp_udp_tcp(unsigned char *mac, unsigned char *ip, unsigned char wwwp)
{
    unsigned char i = 0;
    wwwport = wwwp;

    while (i < 4) {
        ipaddr[i] = ip[i];
        i++;
    }

    i = 0;

    while (i < 6) {
        macaddr[i] = mac[i];
        i++;
    }
}

int test_more = 0;
void net_test_fun(void)
{
    int link_status;
    uint16_t rlen, i;
    static int once = 0;

    init_ip_arp_udp_tcp(mymac, myip, mywwwport);
    g_test_phy_handle = csi_eth_phy_initialize(test_fn_phy_read, test_fn_phy_write);

    g_test_mac_handle = csi_eth_mac_initialize(0, (eth_event_cb_t)eth_mac_signal_event);

    csi_eth_mac_set_macaddr(g_test_mac_handle, (const eth_mac_addr_t *)mymac);

    enc28j60_link_stat = csi_eth_phy_get_linkstate(g_test_phy_handle);

    csi_eth_mac_control(g_test_mac_handle, CSI_ETH_MAC_CONFIGURE, CSI_ETH_MAC_LOOPBACK);
    csi_eth_mac_control(g_test_mac_handle, CSI_ETH_MAC_CONTROL_RX, 1);
    printf("enc28j60 init OK \n");

    while (1) {

        if (interrupt_count) {

            memset(bufs, 0, sizeof(bufs));

            link_status = csi_eth_phy_get_linkstate(g_test_phy_handle);

            if (link_status != -1) {
                enc28j60_link_stat  = link_status;
                printf("Ethernet is link %s\n", (enc28j60_link_stat == 1) ? "up" : "down");

                if (enc28j60_link_stat) {
                    if (once == 0) {
                        send_data();
                        once = 1;
                    }
                } else {
                    once = 0;
                }
            }

            if (interrupt_count == 1) {

                rlen = csi_eth_mac_read_frame(g_test_mac_handle, bufs, MAX_FRAMELEN);

                printf("\n");
                printf("RECEIVE DATA : \n");
                printf("len = %d \n", rlen);

                for (i = 0; i < rlen; i++) {
                    printf("0x%x ", bufs[i]);

                    if ((i + 1) % 20 == 0) {
                        printf("\n");
                    }
                }

                printf("\n");

                if (rlen > 0) {
                    printf("Network recv OK \n");
                    make_answer_from_request(bufs, rlen);
                    printf("Network answer_from_request OK\n");
                }
            }

            interrupt_count = 0;
            csi_eth_mac_control(g_test_mac_handle, CSI_ETH_MAC_CONTROL_RX, 1);
            test_more++;

            if (test_more > 10) {
                test_more = 0;
                break;
            }
        }

    }
}


int test_eth(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_network", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        { "net_test_interfaces", net_test_interfaces, NET_TEST_INTERFACE_EN },
        { "net_test_fun", net_test_fun, NET_TEST_FUN_EN },
        { NULL, NULL }
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_eth);
    dtest();

    return 0;
}
