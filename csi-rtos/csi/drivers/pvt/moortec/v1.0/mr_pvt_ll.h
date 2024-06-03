/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file       mr_pvt_ll.h
 * @brief      Header File for PVT Driver
 * @version    V1.0
 * @date       9. Oct 2021
 * @model      pvt
 ******************************************************************************/

#ifndef __MR_PVT_LL_H_
#define __MR_PVT_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MRS_TS_NUM_MAX                         (8)
#define MRS_VM_NUM_MAX                         (8)
#define MRS_PD_NUM_MAX                         (24)
#define MRS_PD_PORT_NUM                        (6)
#define MRS_VM_CH_NUM                          (16)
#define MR_IP_ID                               (0x26D2)

/* PVT Common register */
//#define PVT_IP_CONFIG	                       0x04
#define TS_NUM_MSK	                       GENMASK(4, 0)
#define TS_NUM_SFT	                       0
#define PD_NUM_MSK	                       GENMASK(12, 8)
#define PD_NUM_SFT	                       8
#define VM_NUM_MSK	                       GENMASK(20, 16)
#define VM_NUM_SFT	                       16
#define CH_NUM_MSK	                       GENMASK(31, 24)
#define CH_NUM_SFT	                       24

/* Macro Common Register */
//#define CLK_SYNTH		               0x00
#define CLK_SYNTH_LO_SFT	               0
#define CLK_SYNTH_HI_SFT	               8
#define CLK_SYNTH_HOLD_SFT	               16
#define CLK_SYNTH_EN		               BIT(24)
#define CLK_SYS_CYCLES_MAX	               514
#define CLK_SYS_CYCLES_MIN	               2
#define HZ_PER_MHZ		               1000000L

//#define SDIF_DISABLE	                       0x04

#define SDIF_STAT	                       0x08
#define SDIF_BUSY	                       BIT(0)
#define SDIF_LOCK	                       BIT(1)

#define SDIF_W		                       0x0c
#define SDIF_PROG	                       BIT(31)
#define SDIF_WRN_W	                       BIT(27)
#define SDIF_WRN_R	                       0x00
#define SDIF_ADDR_SFT	                       24

//#define SDIF_HALT	                       0x10
//#define SDIF_CTRL	                       0x14
#define SDIF_SMPL_CTRL	                       0x20

/* globa irq register*/
#define IRQ_EN_PD_IRQ_ENABLE                   BIT(3)
#define IRQ_EN_VM_IRQ_ENABLE                   BIT(2)
#define IRQ_EN_TS_IRQ_ENABLE                   BIT(1)
#define IRQ_EN_TMR_IRQ_ENABLE                  BIT(0)


/* TS & PD Individual Macro Register */
#define COM_REG_SIZE	                       0x40

#define SDIF_DONE(n)	                       (COM_REG_SIZE + 0x14 + 0x40 * (n))
#define SDIF_SMPL_DONE	                       BIT(0)

#define SDIF_DATA(n)	                       (COM_REG_SIZE + 0x18 + 0x40 * (n))
#define SDIF_RDATA(n)	                       (COM_REG_SIZE + 0x10 + 0x40 * (n))
#define SAMPLE_DATA_MSK	                       GENMASK(15, 0)

#define HILO_RESET(n)	                       (COM_REG_SIZE + 0x2c + 0x40 * (n))


#define TSPD_IRQ_ALARMA                        BIT(3)
#define TSPD_IRQ_ALARMB                        BIT(4)
#define TSPD_IRQ_DONE                          BIT(1)
#define TSPD_IRQ_FAULT                         BIT(0)

#define TSPD_IRQ_MASK_ALL                      (TSPD_IRQ_ALARMA |\
						TSPD_IRQ_ALARMB |\
						TSPD_IRQ_DONE |\
						TSPD_IRQ_FAULT)

/* VM Individual Macro Register */
#define VM_COM_REG_SIZE	                       0x200
#define VM_SDIF_DONE(n)	                       (VM_COM_REG_SIZE + 0x34 + 0x200 * (n))
#define VM_SDIF_DATA(n,m)	               (VM_COM_REG_SIZE + 0x40 + 0x4*m + 0x200 * (n))

/* SDA Slave Register */
#define IP_CTRL			               (0x00)

#define IP_RST_REL		               BIT(1)
#define IP_RUN_CONT		               BIT(3)
#define IP_RUN_ONCE		               BIT(2)
#define IP_CLOAD                               BIT(4)
#define IP_AUTO			               BIT(8)
#define IP_STOP                                BIT(9)
#define IP_VM_MODE		               BIT(10)

#define IP_CFG			               (0x01)

#define   TS_CFG0_MODE_2                       BIT(0)
#define   TS_CFG0_MODE_1                       0
#define   TS_CFG0_12_BIT                       0

#define   PD_CFG0_SEC_MODE(x)                 (x)
#define   PD_CFG0_PRI_MODE(x)                 (x << 2)
#define   PD_CFG0_SER_MODE(x)                 (x << 4)
#define   PD_CFG1_PORT_SEL(x)                 (x << 13)
#define   PD_CFG2_PRE_SCALE_DIV(x)            (x << 16)
#define   PD_CFG2_CWS(x)                      (x << 20)

#define   VM_CFG0_VM_MEAS_MODE                (0 << 0)
#define   VM_CFG0_PRI_MODE                    (0<<2)
#define   VM_CFG0_SER_MODE                    BIT(4)
#define   VM_CFG0_RESLU                       (1<<5)
#define   VM_CFG1_INPUT_SEL(x)                (x<<8)
#define   VM_CFG1_REF_SEL                     (0)


#define IP_DATA		                      (0x03)

#define IP_POLL		                      (0x04)

#define VM_CH_INIT	                      BIT(20)
#define VM_CH_REQ	                      BIT(21)

#define IP_TMR			              (0x05)

#define POWER_DELAY_CYCLE_256	              0x100
#define POWER_DELAY_CYCLE_64	              0x40

#define PVT_POLL_DELAY_US	              20
#define PVT_POLL_TIMEOUT_US	              20000
#define PVT_H_CONST		              100000
#define PVT_CAL5_CONST		              2047
#define PVT_G_CONST		              40000
#define PVT_CONV_BITS		              10
#define PVT_N_CONST		              90
#define PVT_R_CONST		              245805

typedef enum {
    PVT_TS,
    PVT_PD,
    PVT_VM,
}sensor_type_t;

typedef enum {
	IRQ_ALARMA,
	IRQ_ALARMB,
	IRQ_SAMPLE_DONE,
	IRQ_FAULT
}irq_test_t;

typedef struct  {
    __IOM uint32_t PVT_COMP_ID;
    __IOM uint32_t PVT_IP_CONFIG;
    __IOM uint32_t PVT_ID_NUM;
    __IOM uint32_t PVT_TM_SCRATCH;
    __IOM uint32_t PVT_REG_LOCK;
    __IOM uint32_t PVT_REG_LOCK_STATUS;
    __IOM uint32_t PVT_TAM_STATUS;
    __IOM uint32_t PVT_TAM_CLEAR;
    __IOM uint32_t PVT_TMR_CTRL;
    __IOM uint32_t PVT_TMR_STATUS;
    __IOM uint32_t PVT_TMR_IRQ_CLEAR;
    __IOM uint32_t PVT_TMR_IRQ_TEST;
    uint32_t       REV[4];
}pvt_comm_t;

typedef struct  {
    __IOM uint32_t IRQ_EN;
    uint32_t       REV[3];
    __IOM uint32_t IRQ_TR_MASK;
    __IOM uint32_t IRQ_TS_MASK;
    __IOM uint32_t IRQ_VM_MASK;
    __IOM uint32_t IRQ_PD_MASK;
    __IOM uint32_t IRQ_TR_STATUS;
    __IOM uint32_t IRQ_TS_STATUS;
    __IOM uint32_t IRQ_VM_STATUS;
    __IOM uint32_t IRQ_PD_STATUS;
    __IOM uint32_t IRQ_TR_RAW_STATUS;
    __IOM uint32_t IRQ_TS_RAW_STATUS;
    __IOM uint32_t IRQ_VM_RAW_STATUS;
    __IOM uint32_t IRQ_PD_RAW_STATUS;

}pvt_intr_t;

typedef struct  {
    __IOM uint32_t CLK_SYNTH;
    __IOM uint32_t SDIF_DISABLE;
    __IOM uint32_t SDIF_STATUS;
    __IOM uint32_t SDIF;
    __IOM uint32_t SDIF_HALT;
    __IOM uint32_t SDIF_CTRL;
    __IOM uint32_t SMPL_CTRL;
    __IOM uint32_t SMPL_CLR;
    __IOM uint32_t SMPL_CNT;
    uint32_t       REV[7];
}pvt_sdif_t;

typedef struct  {
    __IOM uint32_t IRQ_ENABLE;
    __IOM uint32_t IRQ_STATUS;
    __IOM uint32_t IRQ_CLR;
    __IOM uint32_t IRQ_TEST;
    __IOM uint32_t SDIF_RDATA;
    __IOM uint32_t SDIF_DONE;
    __IOM uint32_t SDIF_DATA;
    uint32_t       REV1[1];
    __IOM uint32_t ALARMA_CFG;
    __IOM uint32_t ALARMB_CFG;
    __IOM uint32_t SMPL_HILO;
    __IOM uint32_t HILO_RESET;
    uint32_t       REV2[4];
}ts_pd_ip_t;

typedef struct  {
    __IOM uint32_t ALARMA_CFG;
    __IOM uint32_t ALARMB_CFG;
    __IOM uint32_t SMPL_HILO;
    __IOM uint32_t HILO_RESET;
}vm_ch_cfg_t;

typedef struct  {
    __IOM uint32_t IRQ_ENABLE;
    __IOM uint32_t IRQ_STATUS;
    __IOM uint32_t IRQ_CLR;
    __IOM uint32_t IRQ_TEST;
    __IOM uint32_t IRQ_ALARMA_ENABLE;
    __IOM uint32_t IRQ_ALARMA_STATUS;
    __IOM uint32_t IRQ_ALARMA_CLR;
    __IOM uint32_t IRQ_ALARMA_TEST;
    __IOM uint32_t IRQ_ALARMB_ENABLE;
    __IOM uint32_t IRQ_ALARMB_STATUS;
    __IOM uint32_t IRQ_ALARMB_CLR;
    __IOM uint32_t IRQ_ALARMB_TEST;
    __IOM uint32_t SDIF_RDATA;
    __IOM uint32_t SDIF_DONE;
    uint32_t       REV1[2];
    __IOM uint32_t CH_M_DATA[MRS_VM_CH_NUM];
    vm_ch_cfg_t  CH_M_CFG[MRS_VM_CH_NUM];
    uint32_t      REV2[8];
}vm_ip_t;

typedef struct {
    pvt_sdif_t    comm;
    ts_pd_ip_t    ipn[0];
}pd_map_t;

typedef struct {
    pvt_sdif_t    comm;
    ts_pd_ip_t    ipn[0];
}ts_map_t;

typedef struct {
    pvt_sdif_t     comm;
    uint32_t       RV1[112];
    vm_ip_t        ipn[0];

}vm_map_t;

typedef struct {
    pvt_comm_t     *comm;
    pvt_intr_t     *intr;
    pd_map_t       *pd_base;
    vm_map_t       *vm_base;
    ts_map_t       *ts_base;
}csi_pvt_regs_t;

typedef struct {
    uint32_t ch_num:8;
    uint32_t vm_num:8;
    uint32_t pd_num:8;
    uint32_t ts_num:8;
}pvt_sensor_num_t;

static inline void mr_pvt_get_sensor_num(csi_pvt_regs_t *pvt, pvt_sensor_num_t *sensor_info )
{
    sensor_info->ch_num = pvt->comm->PVT_IP_CONFIG >> 24;
    sensor_info->vm_num = (pvt->comm->PVT_IP_CONFIG >> 16) & 0xFF;
    sensor_info->pd_num = (pvt->comm->PVT_IP_CONFIG >> 8) & 0xFF;
    sensor_info->ts_num = (pvt->comm->PVT_IP_CONFIG ) & 0xFF;
}

static inline csi_error_t mr_pvt_check_alive(csi_pvt_regs_t *pvt)
{
    if( (pvt->comm->PVT_COMP_ID >>16) != MR_IP_ID) {
        return CSI_ERROR;
    }

    pvt->comm->PVT_TM_SCRATCH = 0xdeadbeef;

    return (pvt->comm->PVT_TM_SCRATCH == 0xdeadbeef) ? CSI_OK : CSI_ERROR;
}

static inline void mr_pvt_sdif_ena_counter(pvt_sdif_t *sdif)
{
    sdif->SMPL_CTRL = 0;
}

static inline void mr_pvt_sdif_halt_disable(pvt_sdif_t *sdif)
{
    sdif->SDIF_HALT = 0;
}

static inline void mr_pvt_sdif_cfg_synth_clk(pvt_sdif_t *sdif,uint32_t clk_synth)
{
    sdif->CLK_SYNTH = clk_synth;
}

static inline void mr_pvt_sdif_ipn_disable(pvt_sdif_t *sdif, uint32_t msk)
{
    sdif->SDIF_DISABLE = msk;
}

static inline uint32_t mr_pvt_sdif_is_busy(pvt_sdif_t *sdif)
{
    return (sdif->SDIF_STATUS & SDIF_BUSY) ? 1 : 0;
}

static inline void mr_pvt_irq_global_ena(csi_pvt_regs_t *pvt, uint32_t mask)
{
	pvt->intr->IRQ_EN = mask;
}

static inline void mr_pvt_irq_global_ts_ena(csi_pvt_regs_t *pvt, uint32_t mask)
{
	pvt->intr->IRQ_TS_MASK &= ~mask;
}

static inline uint32_t mr_pvt_irq_global_ts_status(csi_pvt_regs_t *pvt)
{
	return pvt->intr->IRQ_TS_STATUS;
}

static inline uint32_t mr_pvt_irq_global_ts_raw_status(csi_pvt_regs_t *pvt)
{
	return pvt->intr->IRQ_TS_RAW_STATUS;
}

static inline void mr_pvt_sdif_cfg_alarm_a(ts_pd_ip_t *ipn, uint32_t sensor_idx,uint16_t hyst_thresh, uint16_t alarm_thresh)
{
	ipn[sensor_idx].ALARMA_CFG = (alarm_thresh << 16) | hyst_thresh;
}

static inline void mr_pvt_sdif_cfg_alarm_b(ts_pd_ip_t *ipn, uint32_t sensor_idx,uint16_t hyst_thresh, uint16_t alarm_thresh)
{
	ipn[sensor_idx].ALARMB_CFG = (alarm_thresh << 16) | hyst_thresh;
}

static inline void mr_pvt_sdif_tst_irq(ts_pd_ip_t *ipn, uint32_t sensor_idx,uint32_t tst_pattern)
{
	ipn[sensor_idx].IRQ_TEST = 0;
	mdelay(50);
	ipn[sensor_idx].IRQ_TEST = tst_pattern;
}

static inline void mr_pvt_sdif_irq_ena(ts_pd_ip_t *ipn, uint32_t sensor_idx,uint32_t mask)
{
	ipn[sensor_idx].IRQ_ENABLE = mask;
}

static inline void mr_pvt_sdif_irq_clr(ts_pd_ip_t *ipn, uint32_t sensor_idx,uint32_t mask)
{
	ipn[sensor_idx].IRQ_CLR = mask;
}

static inline uint32_t mr_pvt_sdif_get_irq_status(ts_pd_ip_t *ipn, uint32_t sensor_idx)
{
	return ipn[sensor_idx].IRQ_STATUS;
}

static inline uint32_t mr_pvt_sdif_is_ts_sample_done(ts_pd_ip_t *ipn, uint32_t sensor_idx)
{
    return ipn[sensor_idx].SDIF_DONE & 0x1;
}

static inline uint32_t mr_pvt_sdif_is_pd_sample_done(ts_pd_ip_t *ipn, uint32_t sensor_idx)
{
    return ipn[sensor_idx].SDIF_DONE & 0x1;
}

static inline uint32_t mr_pvt_sdif_is_vm_sample_done(vm_ip_t *ipn, uint32_t sensor_idx)
{
    return ipn[sensor_idx].SDIF_DONE & 0x1;
}

static inline csi_error_t mr_pvt_sdif_get_ts_sensor_data(ts_pd_ip_t *ipn, uint32_t sensor_idx,uint32_t *data)
{
    if ((ipn[sensor_idx].SDIF_DATA >> 16) & 0x3) {
        return CSI_ERROR;
    }

    *data = ipn[sensor_idx].SDIF_DATA & 0xFFFFF;
    return CSI_OK;
}

static inline csi_error_t mr_pvt_sdif_get_pd_sensor_data(ts_pd_ip_t *ipn,uint32_t sensor_idx,uint32_t *data)
{
    if ((ipn[sensor_idx].SDIF_DATA >> 16) & 0x3) {
        return CSI_ERROR;
    }

    *data = ipn[sensor_idx].SDIF_DATA & 0xFFFFF;
    return CSI_OK;
}

static inline csi_error_t mr_pvt_sdif_get_vm_sensor_data(vm_ip_t *ipn,uint32_t sensor_idx,uint32_t *vm_ch_16_data)
{
    uint32_t nm = 0;

    do {
        vm_ch_16_data[nm] = ipn[sensor_idx].CH_M_DATA[nm] & 0xFFFFFF;
	nm++;
    }while(nm < MRS_VM_CH_NUM);

    return CSI_OK;
}

static inline void mr_pvt_sdif_write_ip_reg(pvt_sdif_t *sdif,uint32_t reg_addr,uint32_t reg_data)
{
    uint32_t val;
    val = SDIF_PROG|SDIF_WRN_W|((reg_addr & 0x7) << 24) | (reg_data & 0xFFFFFF);
    sdif->SDIF = val;
}


static inline void mr_pvt_sdif_read_ip_reg(pvt_sdif_t *sdif,uint32_t reg_addr)
{
    uint32_t val;
    val = SDIF_PROG|SDIF_WRN_R|((reg_addr & 0x7) << 24);
    sdif->SDIF = val;
}

static inline void mr_pvt_sdif_read_regdata(csi_pvt_regs_t *pvt, sensor_type_t type,uint32_t *data)
{
    if(type == PVT_TS ) {
        *data = pvt->ts_base->ipn[0].SDIF_RDATA;
    }else if(type == PVT_PD) {
        *data = pvt->pd_base->ipn[0].SDIF_RDATA;
    } else {
        *data = pvt->vm_base->ipn[0].SDIF_RDATA;
    }
}

#ifdef __cplusplus
}
#endif

#endif
