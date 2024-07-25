/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     wj_mbox_ll.h
 * @brief    header file for wj mbox v3 ll driver
 * @version  V1.0
 * @date     15. Apr 2020
 ******************************************************************************/

#ifndef _WJ_mbox_LL_H_
#define _WJ_mbox_LL_H_

#include <soc.h>
#include <csi_core.h>

#ifdef __cplusplus
extern "C" {
#endif
enum thead_xt910_rpc_svc{
    XT910_RPC_SVC_UNKNOWN = 0,
    XT910_RPC_SVC_RETURN = 1,
    XT910_RPC_SVC_PM = 2,
    XT910_RPC_SVC_MISC = 3,
};

#define PM_FUNC_UNKONWN         0U
#define PM_FUNC_RESET           1U
#define PM_FUNC_SET_POWER       2U
#define PM_FUNC_GET_POWER       3U
#define PM_FUNC_SET_CLOCK_RATE  4U
#define PM_FUNC_GET_CLOCK_RATE  5U
#define PM_FUNC_APSYS_SUSPEND   6U

#define XT910_MAX_MSG 7U
struct thead_xt910_rpc_msg{
    uint8_t ver;                             //#define  SC_RPC_VERSION 1U
    uint8_t size;                            //the size of msg word, 4bytes align,the max size is XT910_MAX_MSG
    uint8_t svc;
    uint8_t func;

    union
    {
        uint8_t u8[(XT910_MAX_MSG - 1)*4];
        uint16_t u16[(XT910_MAX_MSG -1)*2];
        uint32_t u32[(XT910_MAX_MSG - 1)];
    }data;
};

/*Status Register*/
#define TH1520_MBOX_STA  0x0
#define TH1520_MBOX_CLR  0x4
#define TH1520_MBOX_MASK 0xc

/* Transmit/receive data register
 * INFO0 ~ INFO6
 */
#define TH1520_MBOX_INFO_NUM 8
#define TH1520_MBOX_DATA_INFO_NUM  7
#define TH1520_MBOX_INFO0  0x14

/*Transmit ack register:INFO7*/
#define TH1520_MBOX_INFO7 0x30

/*Generate remote icu IRQ Register*/
#define TH1520_MBOX_GEN 0x10
#define TH1520_MBOX_GEN_TX_DATA  1 << 6
#define TH1520_MBOX_GEN_TX_ACK   1 << 7

#define TH1520_MBOX_CHAN_RES_SIZE 0x1000
#define TH1520_MBOX_CHANS  4
#define TH1520_MBOX_CHAN_NAME_SIZE 20

#define TH1520_MBOX_ACK_MAGIC 0xdeadbeaf

#ifndef BIT
#define BIT(nr) ((1U) << (nr))
#endif


enum th1520_mbox_icu_cpu_id{
    TH1520_MBOX_ICU_CPU0, /*910T*/
    TH1520_MBOX_ICU_CPU1, /*902T*/
    TH1520_MBOX_ICU_CPU2, /*906R*/
    TH1520_MBOX_ICU_CPU3, /*910R*/
};

struct th1520_mbox_priv{
    uint32_t *cur_cpu_ch_base;
    enum  th1520_mbox_icu_cpu_id cur_icu_cpu_id;
    uint32_t *comm_local_base[TH1520_MBOX_CHANS];
    uint32_t *comm_remote_base[TH1520_MBOX_CHANS];
};

struct th1520_mbox_priv priv[] = {
     {
    .cur_cpu_ch_base = (uint32_t *)0xFFC41000,
    .cur_icu_cpu_id = TH1520_MBOX_ICU_CPU1,
    .comm_local_base =  {(uint32_t *)0xFFC40000,(uint32_t *)0xFFC41000,(uint32_t *)0xFFC42000,(uint32_t *)0xFFC43000},
    .comm_remote_base = {(uint32_t *)0xFFC3D000,(uint32_t *)0xFFC41000,(uint32_t *)0xFFC4D000,(uint32_t *)0xFFC55000}
    },
    {
    .cur_cpu_ch_base = (uint32_t *)0xFFEFC4A000,
    .cur_icu_cpu_id = TH1520_MBOX_ICU_CPU2,
    .comm_local_base =  {(uint32_t *)0xFFEFC48000,(uint32_t *)0xFFEFC49000,(uint32_t *)0xFFEFC4A000,(uint32_t *)0xFFEFC4B000},
    .comm_remote_base = {(uint32_t *)0xFFEFC3E000,(uint32_t *)0xFFEFC46000,(uint32_t *)0xFFEFC4A000,(uint32_t *)0xFFEFC56000}
    },
};

static struct th1520_mbox_priv* get_mbox_priv(int idx)
{
   for( int i = 0; i < sizeof(priv) / sizeof(priv[0]); i++) {
       if(priv[i].cur_icu_cpu_id == idx) {
           return &priv[i];
       }
   }
   return NULL;
}
/**
  \brief       wj_box_irq_handler
  \param[in]   mbox handle of mbox instance
*/
static void iowrite32(uint32_t val, uint32_t *addr)
{
    (*(volatile uint32_t*) addr) = val;
}

static uint32_t ioread32(uint32_t *addr)
{
    return (uint32_t)(* ((volatile uint32_t*)addr));
}

static void wj_mbox_write(struct th1520_mbox_priv *priv, uint32_t val, uint32_t offs)
{
   iowrite32(val, (uint32_t *)((uint32_t)priv->cur_cpu_ch_base + offs));
}

static uint32_t wj_mbox_read(struct th1520_mbox_priv *priv,uint32_t offs)
{
    return ioread32((uint32_t *)((uint32_t)priv->cur_cpu_ch_base + offs));
}

static uint32_t wj_mbox_rmw(struct th1520_mbox_priv *priv,uint32_t offs,uint32_t set,uint32_t clr)
{
    uint32_t val;
    val = wj_mbox_read(priv,offs);
    val &=~clr;
    val |= set;
    wj_mbox_write(priv,val,offs);
    return val;
}

static void wj_mbox_chan_write(struct th1520_mbox_priv *priv,uint32_t channel_id, uint32_t val,uint32_t offs,bool is_remote)
{
    if(is_remote)
        iowrite32(val, (uint32_t *)((uint32_t)(priv->comm_remote_base[channel_id]) + offs));
    else
        iowrite32(val, (uint32_t *)((uint32_t)(priv->comm_local_base[channel_id]) + offs));
}

static uint32_t wj_mbox_chan_read(struct th1520_mbox_priv *priv,uint32_t channel_id, uint32_t offs,bool is_remote)
{
    if(is_remote)
        return ioread32((uint32_t *)((uint32_t)(priv->comm_remote_base[channel_id]) + offs));
    else
        return ioread32((uint32_t *)((uint32_t)(priv->comm_local_base[channel_id]) + offs));
}

static void wj_mbox_chan_rmw(struct th1520_mbox_priv *priv,uint32_t channel_id,uint32_t off,uint32_t set,uint32_t clr,bool is_remote)
{
    uint32_t val;
    val = wj_mbox_chan_read(priv,channel_id,off,is_remote);
    val &= ~clr;
    val |= set;
    wj_mbox_chan_write(priv,channel_id,val,off,is_remote);
}

static void wj_mbox_chan_rd_data(struct th1520_mbox_priv *priv,uint32_t channel_id,void *data,bool is_remote)
{
    uint32_t off = TH1520_MBOX_INFO0;
    uint32_t *arg = data;
    uint32_t i;
    /*
     * read info0~info6，totally 28 bytes
     * requires data memory size is 28 bytes
     */
    for(i = 0;i < TH1520_MBOX_DATA_INFO_NUM;i++){
        *arg = wj_mbox_chan_read(priv,channel_id,off,is_remote);
        off += 4;
        arg++;
    }
}

static void wj_mbox_chan_wr_data(struct th1520_mbox_priv *priv,uint32_t channel_id, const void *data,bool is_remote)
{
    uint32_t off = TH1520_MBOX_INFO0;
    const uint32_t *arg = data;
    uint32_t i;
    /*write info0~info6,totaly 28 bytes
     * requires data memory is 28bytes valid data
     */
    for(i = 0;i < TH1520_MBOX_DATA_INFO_NUM;i++){
        wj_mbox_chan_write(priv,channel_id,*arg, off, is_remote);
        off += 4;
        arg++;
    }
}

static void wj_mbox_chan_wr_ack(struct th1520_mbox_priv *priv,uint32_t channel_id, void *data,bool is_remote)
{
    uint32_t off = TH1520_MBOX_INFO7;
    uint32_t *arg = data;
    wj_mbox_chan_write(priv,channel_id,*arg,off,is_remote);
}

#ifdef __cplusplus
}
#endif

#endif /* _WJ_MBOX_LL_H_ */

