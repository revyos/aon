/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     sys_clk.h
 * @brief    header file for setting system frequency.
 * @version  V1.0
 * @date     9. April 2020
 ******************************************************************************/
#ifndef _SYS_CLK_H_
#define _SYS_CLK_H_

#include <stdint.h>
#include <soc.h>
#include <drv/clk.h>
#include <drv/porting.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CLKGEN_REG_BASE            (wj_apsys_clkgen_reg_t *)WJ_CLKGEN_BASE

typedef enum {
    IHS_CLK       = 0U,        /* internal high speed clock */
    EHS_CLK,                   /* external high speed clock */
    ILS_CLK,                   /* internal low  speed clock */
    ELS_CLK,                   /* external low  speed clock */
    PLL_CLK                    /* PLL clock */
} clk_src_t;

typedef enum {
    APB_62_5MHZ      = 62500000U,
    APB_50MHZ        = 50000000U,
    APB_41_7MHZ      = 41666667U,
    APB_35_7MHZ      = 35714286U,
    APB_33_33MHZ     = 33333333U,
    APB_31_25MHZ     = 31250000U,
    APB_27_78MHZ     = 27777778U,
    APB_25MHZ        = 25000000U,
    APB_23_81MHZ     = 23809524U,
    APB_20_83MHZ     = 20833333U,
    APB_20MHZ        = 20000000U,
    APB_17_86MHZ     = 17857143U,
    APB_16_67MHZ     = 16666667U,
    APB_15_625MHZ    = 15625000U,
    APB_14_29MHZ     = 14285714U,
    APB_13_89MHZ     = 13888889U,
    APB_12_5MHZ      = 12500000U,
    APB_11_90MHZ     = 11904762U,
    APB_10_42MHZ     = 10416667U,
    APB_10_20MHZ     = 10204082U,
    APB_8_93MHZ      = 8928571U,
    APB_7_8125MHZ    = 7812500U,
} apb_freq_t;

typedef enum {
    AHB_250MHZ       = 250000000U,
    AHB_166_67MHZ    = 166666667U,
    AHB_125MHZ       = 125000000U,
    AHB_100MHZ       = 100000000U,
    AHB_83_33MHZ     = 83333333U,
    AHB_71_43MHZ     = 71428574U,
    AHB_62_5MHZ      = 62500000U,
} ahb_freq_t;

/* pllclkout : ( pllclkin / 2)*( FN + Frac/4096 ) */
typedef struct {

    uint32_t pll_is_used;          /* pll is used */

    uint32_t pll_source;           /* select pll input source clock */

    uint32_t pll_src_clk_divider;  /* ratio between pll_srcclk clock and pll_clkin clock */

    uint32_t fn;                   /* integer value of frequency division */

    uint32_t frac;                 /* decimal value of frequency division */

} pll_config_t;


typedef struct {
    uint32_t system_clk;            /* system clock */

    // pll_config_t pll_config;        /* pll config struct */

    uint32_t sys_clk_source;        /* select sysclk source clock */

    uint32_t rtc_clk_source;        /* select rtcclk source clock */

    uint32_t cpu_clk_divider;       /* ratio between fs_mclk clock and mclk clock */

    uint32_t sys_clk_divider;       /* ratio between fs_mclk clock and mclk clock */

    uint32_t ahb_clk_divider;      /* ratio between mclk clock and ahb clock */

    uint32_t apb_clk_divider;      /* ratio between mclk clock and apb clock */

    uint32_t uart_clk_divider;      /* ratio between mclk clock and uart clock */

    uint32_t audio_clk_divider;      /* ratio between mclk clock and audio clock */

    uint32_t vad_clk_divider;      /* ratio between mclk clock and vad clock */

} system_clk_config_t;

typedef enum {
    CLK_DIV1 = 0U,
    CLK_DIV2,
    CLK_DIV3,
    CLK_DIV4,
    CLK_DIV5,
    CLK_DIV6,
    CLK_DIV7,
    CLK_DIV8,
    CLK_DIV9,
    CLK_DIV10,
    CLK_DIV11,
    CLK_DIV12,
    CLK_DIV13,
    CLK_DIV14,
    CLK_DIV15,
    CLK_DIV16
} apb_div_t;

typedef enum {
    PLL_FN_18 = 0U,
    PLL_FN_19,
    PLL_FN_20,
    PLL_FN_21,
    PLL_FN_22,
    PLL_FN_23,
    PLL_FN_24,
    PLL_FN_25,
    PLL_FN_26,
    PLL_FN_27,
    PLL_FN_28,
    PLL_FN_29,
    PLL_FN_30,
    PLL_FN_31,
    PLL_FN_32,
    PLL_FN_33,
    PLL_FN_34,
    PLL_FN_35,
    PLL_FN_36,
    PLL_FN_37,
    PLL_FN_38,
    PLL_FN_39,
    PLL_FN_40,
    PLL_FN_41,
    PLL_FN_42,
    PLL_FN_43,
    PLL_FN_44,
    PLL_FN_45,
    PLL_FN_46,
    PLL_FN_47,
    PLL_FN_48,
    PLL_FN_49
} pll_fn_t;

typedef enum {
    IIC5_CLK = 0U,
    IIC4_CLK,
    IIC3_CLK,
    IIC2_CLK,
    IIC1_CLK,
    IIC0_CLK,
    GPIO2_CLK,
    GPIO1_CLK,
    GPIO0_CLK,
    UART5_CLK,
    UART4_CLK,
    UART3_CLK,
    UART2_CLK,
    UART1_CLK,
    UART0_CLK,
    SPI_CLK,
    QSPI1_CLK,
    QSPI0_CLK,
    PWM_CLK,
    GMAC_CLK,
    EMMC1_CLK,
    EMMC0_CLK,
    EMMC0_X2X_ACLK,
    EMMC1_X2X_ACLK,
    USB3_DRD_CLK,
    USB3_DRD_SUSPEND_CLK,
    USB3_DRD_CTRL_REF_CLK,
    USB3_DRD_PHY_REF_CLK,
    PERISYS_APB2_HCLK,
    PERISYS_APB1_HCLK,
    PERISYS_AHB_HCLK,
    PERISYS_ACLK,
    TIMER1_CLK = 32U,
    TIMER0_CLK,
    WDT1_CLK,
    WDT0_CLK,
    MBOX3_PCLK,
    MBOX2_PCLK,
    MBOX1_PCLK,
    MBOX0_PCLK,
    DMAC_CLK,
    C910_BROM_HCLK,
    APB3_CPUSYS_HCLK,
    AHB2_CPUSYS_HCLK,
    CPU2PERI_X2H_CLK,
    CPU2CFG_X2H_CLK,
    CPU2AON_X2H_CLK,
    CPU2TEE_X2H_CLK,
    X2H_CPUSYS_CLK,
    AXI4_CFG_BUS_ACLK,
    CHIP_DBG_ACLK,
    X2X_CPUSYS_CLK,
    AXI4_CPUSYS2_CLK,
    CPU2RAM_X2X_CLK,
    CPU2CFG_X2X_CLK,
    AON2CPU_A2X_CLK,
    APB_CPU2CFG_HCLK,
    AXI_DUMMY_SLV1_ACLK,
    AXI_DUMMY_SLV2_ACLK,
    AXI_DUMMY_SLV3_ACLK,
    AXI_DUMMY_SLV4_ACLK,
    PERI2DDR_X2X_CLK,
    CHIP_DBG_CCLK = 62U,
    PADCTRL_APSYS_PCLK,
    DDR_APB_PCLK,
    CLK_OUT_1_CLK_EN,
    CLK_OUT_2_CLK_EN,
    CLK_OUT_3_CLK_EN,
    CLK_OUT_4_CLK_EN,
    EFUSE_MPW_CLK,
    AHB2_TEESYS_CLK,
    SRAM_AXI_PCLK,
    DMAC_3_CLK,
    DMAC_2_CLK,
    DMAC_1_CLK,
    AXI_ACLK_0_CLK,
    AXI_ACLK_1_CLK,
    AXI_ACLK_2_CLK,
    AXI_ACLK_3_CLK,
    AXI_ACLK_4_CLK,
    ADC_CLK,
    TRNG_RB_CLK,
} clk_module_t;

typedef struct {
    uint32_t apb_clk;                       /* APB clock */
    uint32_t ahb_clk;                       /* APB clock */
    uint8_t  perisys_ahb_hclk_radio;        /* perisys ahb clock radio */
    uint8_t  perisys_apb_pclk_radio;        /* perisys apb clock radio */
} perisys_clk_config_t;

typedef struct {
    __IOM uint32_t CPU_PLL0_CFG0;                            /* Offset: 0x000 (R/W) CPU PLL0's configuration register0 */
    __IOM uint32_t CPU_PLL0_CFG1;                            /* Offset: 0x004 (R/W) CPU PLL0's configuration register1 */
    __IOM uint32_t CPU_PLL0_CFG2;                            /* Offset: 0x008 (R/W) CPU PLL0's configuration register2 */
    uint32_t RESERVED0;
    __IOM uint32_t CPU_PLL1_CFG0;                            /* Offset: 0x010 (R/W) CPU PLL1's configuration register0 */
    __IOM uint32_t CPU_PLL1_CFG1;                            /* Offset: 0x014 (R/W) CPU PLL1's configuration register1 */
    __IOM uint32_t CPU_PLL1_CFG2;                            /* Offset: 0x018 (R/W) CPU PLL1's configuration register2 */
    uint32_t RESERVED1;
    __IOM uint32_t GMAC_PLL_CFG0;                            /* Offset: 0x020 (R/W) gmac PLL's configuration register0 */
    __IOM uint32_t GMAC_PLL_CFG1;                            /* Offset: 0x024 (R/W) gmac PLL's configuration register1 */
    __IOM uint32_t GMAC_PLL_CFG2;                            /* Offset: 0x028 (R/W) gmac PLL's configuration register2 */
    uint32_t RESERVED2;
    __IOM uint32_t VIDEO_PLL_CFG0;                           /* Offset: 0x030 (R/W) video PLL's configuration register0 */
    __IOM uint32_t VIDEO_PLL_CFG1;                           /* Offset: 0x034 (R/W) video PLL's configuration register1 */
    __IOM uint32_t VIDEO_PLL_CFG2;                           /* Offset: 0x038 (R/W) video PLL's configuration register2 */
    uint32_t RESERVED3[17];
    __IOM uint32_t PLL_STS;                                  /* Offset: 0x080 (R/W) ALL PLL'S status register */
    uint32_t RESERVED4[31];
    __IOM uint32_t C910_CCLK_CFG;                            /* Offset: 0x100 (R/W) C910_CCLK's configuration reigster */
    uint32_t RESERVED5[7];
    __IOM uint32_t CPUSYS_AHB_HCLK_CFG;                      /* Offset: 0x120 (R/W) C910_CCLK's configuration reigster */
    uint32_t RESERVED6[3];
    __IOM uint32_t APB3_CPUSYS_PCLK_CFG;                     /* Offset: 0x130 (R/W) APB3_CPUSYS_PCLK's configuration register */
    __IOM uint32_t CPUSYS_SUB_AXI_ACLK_CFG;                  /* Offset: 0x134 (R/W) CPUSYS_SUB_AXI_ACLK's configuration register */
    __IOM uint32_t CPUSYS_CFG_AXI_ACLK_CFG;                  /* Offset: 0x138 (R/W) CPUSYS_CFG_AXI_ACLK's configuration register */
    uint32_t RESERVED7;
    __IOM uint32_t PERISYS_AHB_HCLK_CFG;                     /* Offset: 0x140 (R/W) PERISYS_AHB_HCLK's configuration register */
    uint32_t RESERVED8[3];
    __IOM uint32_t PERISYS_APB_PCLK_CFG;                     /* Offset: 0x150 (R/W) PERISYS APB PCLK's configuration reigster */
    __IOM uint32_t TEESYS_HCLK_CFG;                          /* Offset: 0x154 (R/W) TEESYS HCLK's configuration reigster */
    __IOM uint32_t DMAC_1_CLK_CFG;                           /* Offset: 0x158 (R/W) DMAC_1 CLK's configuration reigster */
    __IOM uint32_t DMAC_2_CLK_CFG;                           /* Offset: 0x15c (R/W) DMAC_2 CLK's configuration reigster */
    __IOM uint32_t DMAC_3_CLK_CFG;                           /* Offset: 0x160 (R/W) DMAC_3 CLK's configuration reigster */
    __IOM uint32_t AXI_PORT4_CLK_CFG;                        /* Offset: 0x164 (R/W) AXI_PORT4 CLK's configuration reigster */
    uint32_t RESERVED9[19];
    __IOM uint32_t CLK_OUT_1_CFG;                            /* Offset: 0x1b4 (R/W) SENSOR CLK's configuration reigster */
    __IOM uint32_t CLK_OUT_2_CFG;                            /* Offset: 0x1b8 (R/W) SENSOR CLK's configuration reigster */
    __IOM uint32_t CLK_OUT_3_CFG;                            /* Offset: 0x1bc (R/W) SENSOR CLK's configuration reigster */
    __IOM uint32_t CLK_OUT_4_CFG;                            /* Offset: 0x1c0 (R/W) SENSOR CLK's configuration reigster */
    uint32_t RESERVED10[15];
    __IOM uint32_t SW_CLK_GATE_EN_0;                         /* Offset: 0x200 (R/W) SW CLK GATE's configuration reigster0 */
    __IOM uint32_t SW_CLK_GATE_EN_1;                         /* Offset: 0x204 (R/W) SW CLK GATE's configuration reigster1 */
    __IOM uint32_t SW_CLK_GATE_EN_2;                         /* Offset: 0x208 (R/W) SW CLK GATE's configuration reigster2 */
    uint32_t RESERVED11[29];
    __IOM uint32_t BOOT_CLK_SEL_CFG;                         /* Offset: 0x280 (R/W) BOOT_CLK_SEL configuration register */
    uint32_t RESERVED12[31];
    __IOM uint32_t TEST_CLK_CFG;                             /* Offset: 0x300 (R/W) TEST_CLK's configuration register */
    __IOM uint32_t TEST_CLK_STS;                             /* Offset: 0x304 (R/W) TEST_CLK's status register */
    uint32_t RESERVED13[2];
    __IOM uint32_t RESERVED_REG_0;                           /* Offset: 0x310 (R/W) Reseverd register */
    __IOM uint32_t RESERVED_REG_1;                           /* Offset: 0x314 (R/W) Reseverd register */
    __IOM uint32_t RESERVED_REG_2;                           /* Offset: 0x318 (R/W) Reseverd register */
    __IOM uint32_t RESERVED_REG_3;                           /* Offset: 0x31c (R/W) Reseverd register */
} wj_apsys_clkgen_reg_t;

/* CPU_PLL0_CFG0, offset: 0x00 */
#define WJ_CPU_PLL0_CFG0_REFDIV_Pos               (0U)
#define WJ_CPU_PLL0_CFG0_REFDIV_Msk               (0x3FU << WJ_CPU_PLL0_CFG0_REFDIV_Pos)

#define WJ_CPU_PLL0_CFG0_FBDIV_Pos                (8U)
#define WJ_CPU_PLL0_CFG0_FBDIV_Msk                (0xFFFU << WJ_CPU_PLL0_CFG0_FBDIV_Pos)

#define WJ_CPU_PLL0_CFG0_POSTDIV1_Pos             (20U)
#define WJ_CPU_PLL0_CFG0_POSTDIV1_Msk             (0x7U << WJ_CPU_PLL0_CFG0_POSTDIV1_Pos)

#define WJ_CPU_PLL0_CFG0_POSTDIV2_Pos             (24U)
#define WJ_CPU_PLL0_CFG0_POSTDIV2_Msk             (0x7U << WJ_CPU_PLL0_CFG0_POSTDIV2_Pos)

/* CPU_PLL0_CFG1, offset: 0x04 */
#define WJ_CPU_PLL0_CFG1_FRAC_Pos                 (0U)
#define WJ_CPU_PLL0_CFG1_FRAC_Msk                 (0xFFFFFFU << WJ_CPU_PLL0_CFG1_FRAC_Pos)

#define WJ_CPU_PLL0_CFG1_DSMPD_Pos                (24U)
#define WJ_CPU_PLL0_CFG1_DSMPD_Msk                (0x1U << WJ_CPU_PLL0_CFG1_DSMPD_Pos)

#define WJ_CPU_PLL0_CFG1_DACPD_Pos                (25U)
#define WJ_CPU_PLL0_CFG1_DACPD_Msk                (0x1U << WJ_CPU_PLL0_CFG1_DACPD_Pos)

#define WJ_CPU_PLL0_CFG1_PHASEPD_Pos              (27U)
#define WJ_CPU_PLL0_CFG1_PHASEPD_Msk              (0x1U << WJ_CPU_PLL0_CFG1_PHASEPD_Pos)

#define WJ_CPU_PLL0_CFG1_POSTDIVPD_Pos            (28U)
#define WJ_CPU_PLL0_CFG1_POSTDIVPD_Msk            (0x1U << WJ_CPU_PLL0_CFG1_POSTDIVPD_Pos)

#define WJ_CPU_PLL0_CFG1_RST_Pos                  (29U)
#define WJ_CPU_PLL0_CFG1_RST_Msk                  (0x1U << WJ_CPU_PLL0_CFG1_RST_Pos)

#define WJ_CPU_PLL0_CFG1_BYPASS_Pos               (30U)
#define WJ_CPU_PLL0_CFG1_BYPASS_Msk               (0x1U << WJ_CPU_PLL0_CFG1_BYPASS_Pos)

/* CPU_PLL0_CFG2, offset: 0x08 */
#define WJ_CPU_PLL0_CFG2_DSKEWCALOUT_Pos          (0U)
#define WJ_CPU_PLL0_CFG2_DSKEWCALOUT_Msk          (0xFFFU << WJ_CPU_PLL0_CFG2_DSKEWCALOUT_Pos)

#define WJ_CPU_PLL0_CFG2_DSKEWCALIN_Pos           (12U)
#define WJ_CPU_PLL0_CFG2_DSKEWCALIN_Msk           (0xFFFU << WJ_CPU_PLL0_CFG2_DSKEWCALIN_Pos)

#define WJ_CPU_PLL0_CFG2_DSKEWCALCNT_Pos          (24U)
#define WJ_CPU_PLL0_CFG2_DSKEWCALCNT_Msk          (0x7U << WJ_CPU_PLL0_CFG2_DSKEWCALCNT_Pos)

#define WJ_CPU_PLL0_CFG2_DSKEWFASTCAL_Pos         (27U)
#define WJ_CPU_PLL0_CFG2_DSKEWFASTCAL_Msk         (0x1U << WJ_CPU_PLL0_CFG2_DSKEWFASTCAL_Pos)

#define WJ_CPU_PLL0_CFG2_DSKEWCALEN_Pos           (28U)
#define WJ_CPU_PLL0_CFG2_DSKEWCALEN_Msk           (0x1U << WJ_CPU_PLL0_CFG2_DSKEWCALEN_Pos)

#define WJ_CPU_PLL0_CFG2_DSKEWCALBYP_Pos          (29U)
#define WJ_CPU_PLL0_CFG2_DSKEWCALBYP_Msk          (0x1U << WJ_CPU_PLL0_CFG2_DSKEWCALBYP_Pos)

#define WJ_CPU_PLL0_CFG2_DSKEWCALLOCK_Pos         (30U)
#define WJ_CPU_PLL0_CFG2_DSKEWCALLOCK_Msk         (0x1U << WJ_CPU_PLL0_CFG2_DSKEWCALLOCK_Pos)

/* CPU_PLL1_CFG0, offset: 0x10 */
#define WJ_CPU_PLL1_CFG0_REFDIV_Pos               (0U)
#define WJ_CPU_PLL1_CFG0_REFDIV_Msk               (0x3FU << WJ_CPU_PLL1_CFG0_REFDIV_Pos)

#define WJ_CPU_PLL1_CFG0_FBDIV_Pos                (8U)
#define WJ_CPU_PLL1_CFG0_FBDIV_Msk                (0xFFFU << WJ_CPU_PLL1_CFG0_FBDIV_Pos)

#define WJ_CPU_PLL1_CFG0_POSTDIV1_Pos             (20U)
#define WJ_CPU_PLL1_CFG0_POSTDIV1_Msk             (0x7U << WJ_CPU_PLL1_CFG0_POSTDIV1_Pos)

#define WJ_CPU_PLL1_CFG0_POSTDIV2_Pos             (24U)
#define WJ_CPU_PLL1_CFG0_POSTDIV2_Msk             (0x7U << WJ_CPU_PLL1_CFG0_POSTDIV2_Pos)

/* CPU_PLL1_CFG1, offset: 0x14 */
#define WJ_CPU_PLL1_CFG1_FRAC_Pos                 (0U)
#define WJ_CPU_PLL1_CFG1_FRAC_Msk                 (0xFFFFFFU << WJ_CPU_PLL1_CFG1_FRAC_Pos)

#define WJ_CPU_PLL1_CFG1_DSMPD_Pos                (24U)
#define WJ_CPU_PLL1_CFG1_DSMPD_Msk                (0x1U << WJ_CPU_PLL1_CFG1_DSMPD_Pos)

#define WJ_CPU_PLL1_CFG1_DACPD_Pos                (25U)
#define WJ_CPU_PLL1_CFG1_DACPD_Msk                (0x1U << WJ_CPU_PLL1_CFG1_DACPD_Pos)

#define WJ_CPU_PLL1_CFG1_PHASEPD_Pos              (27U)
#define WJ_CPU_PLL1_CFG1_PHASEPD_Msk              (0x1U << WJ_CPU_PLL1_CFG1_PHASEPD_Pos)

#define WJ_CPU_PLL1_CFG1_POSTDIVPD_Pos            (28U)
#define WJ_CPU_PLL1_CFG1_POSTDIVPD_Msk            (0x1U << WJ_CPU_PLL1_CFG1_POSTDIVPD_Pos)

#define WJ_CPU_PLL1_CFG1_RST_Pos                  (29U)
#define WJ_CPU_PLL1_CFG1_RST_Msk                  (0x1U << WJ_CPU_PLL1_CFG1_RST_Pos)

#define WJ_CPU_PLL1_CFG1_BYPASS_Pos               (30U)
#define WJ_CPU_PLL1_CFG1_BYPASS_Msk               (0x1U << WJ_CPU_PLL1_CFG1_BYPASS_Pos)

/* CPU_PLL1_CFG2, offset: 0x18 */
#define WJ_CPU_PLL1_CFG2_DSKEWCALOUT_Pos          (0U)
#define WJ_CPU_PLL1_CFG2_DSKEWCALOUT_Msk          (0xFFFU << WJ_CPU_PLL1_CFG2_DSKEWCALOUT_Pos)

#define WJ_CPU_PLL1_CFG2_DSKEWCALIN_Pos           (12U)
#define WJ_CPU_PLL1_CFG2_DSKEWCALIN_Msk           (0xFFFU << WJ_CPU_PLL1_CFG2_DSKEWCALIN_Pos)

#define WJ_CPU_PLL1_CFG2_DSKEWCALCNT_Pos          (24U)
#define WJ_CPU_PLL1_CFG2_DSKEWCALCNT_Msk          (0x7U << WJ_CPU_PLL1_CFG2_DSKEWCALCNT_Pos)

#define WJ_CPU_PLL1_CFG2_DSKEWFASTCAL_Pos         (27U)
#define WJ_CPU_PLL1_CFG2_DSKEWFASTCAL_Msk         (0x1U << WJ_CPU_PLL1_CFG2_DSKEWFASTCAL_Pos)

#define WJ_CPU_PLL1_CFG2_DSKEWCALEN_Pos           (28U)
#define WJ_CPU_PLL1_CFG2_DSKEWCALEN_Msk           (0x1U << WJ_CPU_PLL1_CFG2_DSKEWCALEN_Pos)

#define WJ_CPU_PLL1_CFG2_DSKEWCALBYP_Pos          (29U)
#define WJ_CPU_PLL1_CFG2_DSKEWCALBYP_Msk          (0x1U << WJ_CPU_PLL1_CFG2_DSKEWCALBYP_Pos)

#define WJ_CPU_PLL1_CFG2_DSKEWCALLOCK_Pos         (30U)
#define WJ_CPU_PLL1_CFG2_DSKEWCALLOCK_Msk         (0x1U << WJ_CPU_PLL1_CFG2_DSKEWCALLOCK_Pos)

/* GMAC_PLL_CFG0, offset: 0x20 */
#define WJ_GMAC_PLL_CFG0_REFDIV_Pos               (0U)
#define WJ_GMAC_PLL_CFG0_REFDIV_Msk               (0x3FU << WJ_GMAC_PLL_CFG0_REFDIV_Pos)

#define WJ_GMAC_PLL_CFG0_FBDIV_Pos                (8U)
#define WJ_GMAC_PLL_CFG0_FBDIV_Msk                (0xFFFU << WJ_GMAC_PLL_CFG0_FBDIV_Pos)

#define WJ_GMAC_PLL_CFG0_POSTDIV1_Pos             (20U)
#define WJ_GMAC_PLL_CFG0_POSTDIV1_Msk             (0x7U << WJ_GMAC_PLL_CFG0_POSTDIV1_Pos)

#define WJ_GMAC_PLL_CFG0_POSTDIV2_Pos             (24U)
#define WJ_GMAC_PLL_CFG0_POSTDIV2_Msk             (0x7U << WJ_GMAC_PLL_CFG0_POSTDIV2_Pos)

/* GMAC_PLL_CFG1, offset: 0x24 */
#define WJ_GMAC_PLL_CFG1_FRAC_Pos                 (0U)
#define WJ_GMAC_PLL_CFG1_FRAC_Msk                 (0xFFFFFFU << WJ_GMAC_PLL_CFG1_FRAC_Pos)

#define WJ_GMAC_PLL_CFG1_DSMPD_Pos                (24U)
#define WJ_GMAC_PLL_CFG1_DSMPD_Msk                (0x1U << WJ_GMAC_PLL_CFG1_DSMPD_Pos)

#define WJ_GMAC_PLL_CFG1_DACPD_Pos                (25U)
#define WJ_GMAC_PLL_CFG1_DACPD_Msk                (0x1U << WJ_GMAC_PLL_CFG1_DACPD_Pos)

#define WJ_GMAC_PLL_CFG1_PHASEPD_Pos              (27U)
#define WJ_GMAC_PLL_CFG1_PHASEPD_Msk              (0x1U << WJ_GMAC_PLL_CFG1_PHASEPD_Pos)

#define WJ_GMAC_PLL_CFG1_POSTDIVPD_Pos            (28U)
#define WJ_GMAC_PLL_CFG1_POSTDIVPD_Msk            (0x1U << WJ_GMAC_PLL_CFG1_POSTDIVPD_Pos)

#define WJ_GMAC_PLL_CFG1_RST_Pos                  (29U)
#define WJ_GMAC_PLL_CFG1_RST_Msk                  (0x1U << WJ_GMAC_PLL_CFG1_RST_Pos)

#define WJ_GMAC_PLL_CFG1_BYPASS_Pos               (30U)
#define WJ_GMAC_PLL_CFG1_BYPASS_Msk               (0x1U << WJ_GMAC_PLL_CFG1_BYPASS_Pos)

/* GMAC_PLL_CFG2, offset: 0x28 */
#define WJ_GMAC_PLL_CFG2_DSKEWCALOUT_Pos          (0U)
#define WJ_GMAC_PLL_CFG2_DSKEWCALOUT_Msk          (0xFFFU << WJ_GMAC_PLL_CFG2_DSKEWCALOUT_Pos)

#define WJ_GMAC_PLL_CFG2_DSKEWCALIN_Pos           (12U)
#define WJ_GMAC_PLL_CFG2_DSKEWCALIN_Msk           (0xFFFU << WJ_GMAC_PLL_CFG2_DSKEWCALIN_Pos)

#define WJ_GMAC_PLL_CFG2_DSKEWCALCNT_Pos          (24U)
#define WJ_GMAC_PLL_CFG2_DSKEWCALCNT_Msk          (0x7U << WJ_GMAC_PLL_CFG2_DSKEWCALCNT_Pos)

#define WJ_GMAC_PLL_CFG2_DSKEWFASTCAL_Pos         (27U)
#define WJ_GMAC_PLL_CFG2_DSKEWFASTCAL_Msk         (0x1U << WJ_GMAC_PLL_CFG2_DSKEWFASTCAL_Pos)

#define WJ_GMAC_PLL_CFG2_DSKEWCALEN_Pos           (28U)
#define WJ_GMAC_PLL_CFG2_DSKEWCALEN_Msk           (0x1U << WJ_GMAC_PLL_CFG2_DSKEWCALEN_Pos)

#define WJ_GMAC_PLL_CFG2_DSKEWCALBYP_Pos          (29U)
#define WJ_GMAC_PLL_CFG2_DSKEWCALBYP_Msk          (0x1U << WJ_GMAC_PLL_CFG2_DSKEWCALBYP_Pos)

#define WJ_GMAC_PLL_CFG2_DSKEWCALLOCK_Pos         (30U)
#define WJ_GMAC_PLL_CFG2_DSKEWCALLOCK_Msk         (0x1U << WJ_GMAC_PLL_CFG2_DSKEWCALLOCK_Pos)

/* VIDEO_PLL_CFG0, offset: 0x30 */
#define WJ_VIDEO_PLL_CFG0_REFDIV_Pos               (0U)
#define WJ_VIDEO_PLL_CFG0_REFDIV_Msk               (0x3FU << WJ_VIDEO_PLL_CFG0_REFDIV_Pos)

#define WJ_VIDEO_PLL_CFG0_FBDIV_Pos                (8U)
#define WJ_VIDEO_PLL_CFG0_FBDIV_Msk                (0xFFFU << WJ_VIDEO_PLL_CFG0_FBDIV_Pos)

#define WJ_VIDEO_PLL_CFG0_POSTDIV1_Pos             (20U)
#define WJ_VIDEO_PLL_CFG0_POSTDIV1_Msk             (0x7U << WJ_VIDEO_PLL_CFG0_POSTDIV1_Pos)

#define WJ_VIDEO_PLL_CFG0_POSTDIV2_Pos             (24U)
#define WJ_VIDEO_PLL_CFG0_POSTDIV2_Msk             (0x7U << WJ_VIDEO_PLL_CFG0_POSTDIV2_Pos)

/* VIDEO_PLL_CFG1, offset: 0x34 */
#define WJ_VIDEO_PLL_CFG1_FRAC_Pos                 (0U)
#define WJ_VIDEO_PLL_CFG1_FRAC_Msk                 (0xFFFFFFU << WJ_VIDEO_PLL_CFG1_FRAC_Pos)

#define WJ_VIDEO_PLL_CFG1_DSMPD_Pos                (24U)
#define WJ_VIDEO_PLL_CFG1_DSMPD_Msk                (0x1U << WJ_VIDEO_PLL_CFG1_DSMPD_Pos)

#define WJ_VIDEO_PLL_CFG1_DACPD_Pos                (25U)
#define WJ_VIDEO_PLL_CFG1_DACPD_Msk                (0x1U << WJ_VIDEO_PLL_CFG1_DACPD_Pos)

#define WJ_VIDEO_PLL_CFG1_PHASEPD_Pos              (27U)
#define WJ_VIDEO_PLL_CFG1_PHASEPD_Msk              (0x1U << WJ_VIDEO_PLL_CFG1_PHASEPD_Pos)

#define WJ_VIDEO_PLL_CFG1_POSTDIVPD_Pos            (28U)
#define WJ_VIDEO_PLL_CFG1_POSTDIVPD_Msk            (0x1U << WJ_VIDEO_PLL_CFG1_POSTDIVPD_Pos)

#define WJ_VIDEO_PLL_CFG1_RST_Pos                  (29U)
#define WJ_VIDEO_PLL_CFG1_RST_Msk                  (0x1U << WJ_VIDEO_PLL_CFG1_RST_Pos)

#define WJ_VIDEO_PLL_CFG1_BYPASS_Pos               (30U)
#define WJ_VIDEO_PLL_CFG1_BYPASS_Msk               (0x1U << WJ_VIDEO_PLL_CFG1_BYPASS_Pos)

/* VIDEO_PLL_CFG2, offset: 0x38 */
#define WJ_VIDEO_PLL_CFG2_DSKEWCALOUT_Pos          (0U)
#define WJ_VIDEO_PLL_CFG2_DSKEWCALOUT_Msk          (0xFFFU << WJ_VIDEO_PLL_CFG2_DSKEWCALOUT_Pos)

#define WJ_VIDEO_PLL_CFG2_DSKEWCALIN_Pos           (12U)
#define WJ_VIDEO_PLL_CFG2_DSKEWCALIN_Msk           (0xFFFU << WJ_VIDEO_PLL_CFG2_DSKEWCALIN_Pos)

#define WJ_VIDEO_PLL_CFG2_DSKEWCALCNT_Pos          (24U)
#define WJ_VIDEO_PLL_CFG2_DSKEWCALCNT_Msk          (0x7U << WJ_VIDEO_PLL_CFG2_DSKEWCALCNT_Pos)

#define WJ_VIDEO_PLL_CFG2_DSKEWFASTCAL_Pos         (27U)
#define WJ_VIDEO_PLL_CFG2_DSKEWFASTCAL_Msk         (0x1U << WJ_VIDEO_PLL_CFG2_DSKEWFASTCAL_Pos)

#define WJ_VIDEO_PLL_CFG2_DSKEWCALEN_Pos           (28U)
#define WJ_VIDEO_PLL_CFG2_DSKEWCALEN_Msk           (0x1U << WJ_VIDEO_PLL_CFG2_DSKEWCALEN_Pos)

#define WJ_VIDEO_PLL_CFG2_DSKEWCALBYP_Pos          (29U)
#define WJ_VIDEO_PLL_CFG2_DSKEWCALBYP_Msk          (0x1U << WJ_VIDEO_PLL_CFG2_DSKEWCALBYP_Pos)

#define WJ_VIDEO_PLL_CFG2_DSKEWCALLOCK_Pos         (30U)
#define WJ_VIDEO_PLL_CFG2_DSKEWCALLOCK_Msk         (0x1U << WJ_VIDEO_PLL_CFG2_DSKEWCALLOCK_Pos)

/* PLL_STS, offset: 0x80 */
#define WJ_PLL_STS_ALL_PLL_LOCK_Pos          (0U)
#define WJ_PLL_STS_ALL_PLL_LOCK_Msk          (0x1U << WJ_PLL_STS_ALL_PLL_LOCK_Pos)

#define WJ_PLL_STS_CPU_PLL0_LOCK_Pos         (1U)
#define WJ_PLL_STS_CPU_PLL0_LOCK_Msk         (0x1U << WJ_PLL_STS_CPU_PLL0_LOCK_Pos)

#define WJ_PLL_STS_DDR_PLL_LOCK_Pos          (2U)
#define WJ_PLL_STS_DDR_PLL_LOCK_Msk          (0x1U << WJ_PLL_STS_DDR_PLL_LOCK_Pos)

#define WJ_PLL_STS_GMAC_PLL_LOCK_Pos         (3U)
#define WJ_PLL_STS_GMAC_PLL_LOCK_Msk         (0x1U << WJ_PLL_STS_GMAC_PLL_LOCK_Pos)

#define WJ_PLL_STS_CPU_PLL1_LOCK_Pos         (4U)
#define WJ_PLL_STS_CPU_PLL1_LOCK_Msk         (0x1U << WJ_PLL_STS_CPU_PLL1_LOCK_Pos)

#define WJ_PLL_STS_AUDIO_PLL_LOCK_Pos        (5U)
#define WJ_PLL_STS_AUDIO_PLL_LOCK_Msk        (0x1U << WJ_PLL_STS_AUDIO_PLL_LOCK_Pos)

#define WJ_PLL_STS_SYS_PLL_LOCK_Pos          (6U)
#define WJ_PLL_STS_SYS_PLL_LOCK_Msk          (0x1U << WJ_PLL_STS_SYS_PLL_LOCK_Pos)

#define WJ_PLL_STS_VIDEO_PLL_LOCK_Pos        (7U)
#define WJ_PLL_STS_VIDEO_PLL_LOCK_Msk        (0x1U << WJ_PLL_STS_VIDEO_PLL_LOCK_Pos)

/* C910_CCLK_CFG, offset: 0x100 */
#define WJ_C910_CCLK_CFG_CCLK_SW_SEL_Pos       (0U)
#define WJ_C910_CCLK_CFG_CCLK_SW_SEL_Msk       (0x1U << WJ_C910_CCLK_CFG_CCLK_SW_SEL_Pos)

#define WJ_C910_CCLK_CFG_I0_SW_SEL_Pos         (1U)
#define WJ_C910_CCLK_CFG_I0_SW_SEL_Msk         (0x1U << WJ_C910_CCLK_CFG_I0_SW_SEL_Pos)

/* CPUSYS_AHB_HCLK_CFG, offset: 0x120 */
#define WJ_CPUSYS_AHB_HCLK_CFG_DIV_NUM_Pos          (0U)
#define WJ_CPUSYS_AHB_HCLK_CFG_DIV_NUM_Msk          (0xFU << WJ_CPUSYS_AHB_HCLK_CFG_DIV_NUM_Pos)

#define WJ_CPUSYS_AHB_HCLK_CFG_DIV_EN_Pos           (4U)
#define WJ_CPUSYS_AHB_HCLK_CFG_DIV_EN_Msk           (0x1U << WJ_CPUSYS_AHB_HCLK_CFG_DIV_EN_Pos)

#define WJ_CPUSYS_AHB_HCLK_CFG_SW_SEL_Pos           (5U)
#define WJ_CPUSYS_AHB_HCLK_CFG_SW_SEL_Msk           (0x1U << WJ_CPUSYS_AHB_HCLK_CFG_SW_SEL_Pos)

/* APB3_CPUSYS_PCLK_CFG, offset: 0x130 */
#define WJ_APB3_CPUSYS_PCLK_CFG_RADIO_Pos          (0U)
#define WJ_APB3_CPUSYS_PCLK_CFG_RADIO_Msk          (0x7U << WJ_APB3_CPUSYS_PCLK_CFG_RADIO_Pos)

#define WJ_APB3_CPUSYS_PCLK_CFG_SYNC_Pos           (3U)
#define WJ_APB3_CPUSYS_PCLK_CFG_SYNC_Msk           (0x1U << WJ_APB3_CPUSYS_PCLK_CFG_SYNC_Pos)

/* CPUSYS_SUB_AXI_ACLK_CFG, offset: 0x134 */
#define WJ_CPUSYS_SUB_AXI_ACLK_CFG_DIV_NUM_Pos          (0U)
#define WJ_CPUSYS_SUB_AXI_ACLK_CFG_DIV_NUM_Msk          (0xFU << WJ_CPUSYS_SUB_AXI_ACLK_CFG_DIV_NUM_Pos)

#define WJ_CPUSYS_SUB_AXI_ACLK_CFG_DIV_EN_Pos           (4U)
#define WJ_CPUSYS_SUB_AXI_ACLK_CFG_DIV_EN_Msk           (0x1U << WJ_CPUSYS_SUB_AXI_ACLK_CFG_DIV_EN_Pos)

/* CPUSYS_CFG_AXI_ACLK_CFG, offset: 0x138 */
#define WJ_CPUSYS_CFG_AXI_ACLK_CFG_DIV_NUM_Pos          (0U)
#define WJ_CPUSYS_CFG_AXI_ACLK_CFG_DIV_NUM_Msk          (0xFU << WJ_CPUSYS_CFG_AXI_ACLK_CFG_DIV_NUM_Pos)

#define WJ_CPUSYS_CFG_AXI_ACLK_CFG_DIV_EN_Pos           (4U)
#define WJ_CPUSYS_CFG_AXI_ACLK_CFG_DIV_EN_Msk           (0x1U << WJ_CPUSYS_CFG_AXI_ACLK_CFG_DIV_EN_Pos)

#define WJ_CPUSYS_CFG_AXI_ACLK_CFG_SW_SEL_Pos           (5U)
#define WJ_CPUSYS_CFG_AXI_ACLK_CFG_SW_SEL_Msk           (0x1U << WJ_CPUSYS_CFG_AXI_ACLK_CFG_SW_SEL_Pos)

/* PERISYS_AHB_HCLK_CFG, offset: 0x140 */
#define WJ_PERISYS_AHB_HCLK_CFG_DIV_NUM_Pos          (0U)
#define WJ_PERISYS_AHB_HCLK_CFG_DIV_NUM_Msk          (0xFU << WJ_PERISYS_AHB_HCLK_CFG_DIV_NUM_Pos)

#define WJ_PERISYS_AHB_HCLK_CFG_DIV_EN_Pos           (4U)
#define WJ_PERISYS_AHB_HCLK_CFG_DIV_EN_Msk           (0x1U << WJ_PERISYS_AHB_HCLK_CFG_DIV_EN_Pos)

#define WJ_PERISYS_AHB_HCLK_CFG_SW_SEL_Pos           (5U)
#define WJ_PERISYS_AHB_HCLK_CFG_SW_SEL_Msk           (0x1U << WJ_PERISYS_AHB_HCLK_CFG_SW_SEL_Pos)

/* PERISYS_APB_PCLK_CFG, offset: 0x150 */
#define WJ_PERISYS_APB_PCLK_CFG_DIV_NUM_Pos          (0U)
#define WJ_PERISYS_APB_PCLK_CFG_DIV_NUM_Msk          (0x7U << WJ_PERISYS_APB_PCLK_CFG_DIV_NUM_Pos)

#define WJ_PERISYS_APB_PCLK_CFG_DIV_EN_Pos           (3U)
#define WJ_PERISYS_APB_PCLK_CFG_DIV_EN_Msk           (0x1U << WJ_PERISYS_APB_PCLK_CFG_DIV_EN_Pos)

/* TEESYS_HCLK_CFG, offset: 0x154 */
#define WJ_TEESYS_HCLK_CFG_DIV_NUM_Pos               (0U)
#define WJ_TEESYS_HCLK_CFG_DIV_NUM_Msk               (0x3U << WJ_TEESYS_HCLK_CFG_DIV_NUM_Pos)

#define WJ_TEESYS_HCLK_CFG_DIV_EN_Pos                (2U)
#define WJ_TEESYS_HCLK_CFG_DIV_EN_Msk                (0x1U << WJ_TEESYS_HCLK_CFG_DIV_EN_Pos)

/* DMAC_1_CLK_CFG, offset: 0x158 */
#define WJ_DMAC_1_CLK_CFG_RATIO_Pos                  (0U)
#define WJ_DMAC_1_CLK_CFG_RATIO_Msk                  (0x7U << WJ_DMAC_1_CLK_CFG_RATIO_Pos)

#define WJ_DMAC_1_CLK_CFG_SYNC_Pos                   (3U)
#define WJ_DMAC_1_CLK_CFG_SYNC_Msk                   (0x1U << WJ_DMAC_1_CLK_CFG_SYNC_Pos)

/* DMAC_2_CLK_CFG, offset: 0x15c */
#define WJ_DMAC_2_CLK_CFG_RATIO_Pos                  (0U)
#define WJ_DMAC_2_CLK_CFG_RATIO_Msk                  (0x7U << WJ_DMAC_2_CLK_CFG_RATIO_Pos)

#define WJ_DMAC_2_CLK_CFG_SYNC_Pos                   (3U)
#define WJ_DMAC_2_CLK_CFG_SYNC_Msk                   (0x1U << WJ_DMAC_2_CLK_CFG_SYNC_Pos)

/* DMAC_3_CLK_CFG, offset: 0x160 */
#define WJ_DMAC_3_CLK_CFG_RATIO_Pos                  (0U)
#define WJ_DMAC_3_CLK_CFG_RATIO_Msk                  (0x7U << WJ_DMAC_3_CLK_CFG_RATIO_Pos)

#define WJ_DMAC_3_CLK_CFG_SYNC_Pos                   (3U)
#define WJ_DMAC_3_CLK_CFG_SYNC_Msk                   (0x1U << WJ_DMAC_3_CLK_CFG_SYNC_Pos)

/* AXI_PORT4_CLK_CFG, offset: 0x164 */
#define WJ_AXI_PORT4_CLK_CFG_RATIO_Pos                  (0U)
#define WJ_AXI_PORT4_CLK_CFG_RATIO_Msk                  (0x7U << WJ_AXI_PORT4_CLK_CFG_RATIO_Pos)

#define WJ_AXI_PORT4_CLK_CFG_SYNC_Pos                   (3U)
#define WJ_AXI_PORT4_CLK_CFG_SYNC_Msk                   (0x1U << WJ_AXI_PORT4_CLK_CFG_SYNC_Pos)

/* CLK_OUT_1_CFG, offset: 0x1b4 */
#define WJ_CLK_OUT_1_CFG_DIV_NUM_Pos                  (0U)
#define WJ_CLK_OUT_1_CFG_DIV_NUM_Msk                  (0x7U << WJ_CLK_OUT_1_CFG_DIV_NUM_Pos)

#define WJ_CLK_OUT_1_CFG_DIV_EN_Pos                   (3U)
#define WJ_CLK_OUT_1_CFG_DIV_EN_Msk                   (0x1U << WJ_CLK_OUT_1_CFG_DIV_EN_Pos)

#define WJ_CLK_OUT_1_CFG_MUX_SEL_Pos                  (4U)
#define WJ_CLK_OUT_1_CFG_MUX_SEL_Msk                  (0x1U << WJ_CLK_OUT_1_CFG_MUX_SEL_Pos)

/* CLK_OUT_2_CFG, offset: 0x1b8 */
#define WJ_CLK_OUT_2_CFG_DIV_NUM_Pos                  (0U)
#define WJ_CLK_OUT_2_CFG_DIV_NUM_Msk                  (0x7U << WJ_CLK_OUT_2_CFG_DIV_NUM_Pos)

#define WJ_CLK_OUT_2_CFG_DIV_EN_Pos                   (3U)
#define WJ_CLK_OUT_2_CFG_DIV_EN_Msk                   (0x1U << WJ_CLK_OUT_2_CFG_DIV_EN_Pos)

#define WJ_CLK_OUT_2_CFG_MUX_SEL_Pos                  (4U)
#define WJ_CLK_OUT_2_CFG_MUX_SEL_Msk                  (0x1U << WJ_CLK_OUT_2_CFG_MUX_SEL_Pos)

/* CLK_OUT_3_CFG, offset: 0x1bc */
#define WJ_CLK_OUT_3_CFG_DIV_NUM_Pos                  (0U)
#define WJ_CLK_OUT_3_CFG_DIV_NUM_Msk                  (0x7U << WJ_CLK_OUT_3_CFG_DIV_NUM_Pos)

#define WJ_CLK_OUT_3_CFG_DIV_EN_Pos                   (3U)
#define WJ_CLK_OUT_3_CFG_DIV_EN_Msk                   (0x1U << WJ_CLK_OUT_3_CFG_DIV_EN_Pos)

#define WJ_CLK_OUT_3_CFG_MUX_SEL_Pos                  (4U)
#define WJ_CLK_OUT_3_CFG_MUX_SEL_Msk                  (0x1U << WJ_CLK_OUT_3_CFG_MUX_SEL_Pos)

/* CLK_OUT_4_CFG, offset: 0x1c0 */
#define WJ_CLK_OUT_4_CFG_DIV_NUM_Pos                  (0U)
#define WJ_CLK_OUT_4_CFG_DIV_NUM_Msk                  (0x7U << WJ_CLK_OUT_4_CFG_DIV_NUM_Pos)

#define WJ_CLK_OUT_4_CFG_DIV_EN_Pos                   (3U)
#define WJ_CLK_OUT_4_CFG_DIV_EN_Msk                   (0x1U << WJ_CLK_OUT_4_CFG_DIV_EN_Pos)

#define WJ_CLK_OUT_4_CFG_MUX_SEL_Pos                  (4U)
#define WJ_CLK_OUT_4_CFG_MUX_SEL_Msk                  (0x1U << WJ_CLK_OUT_4_CFG_MUX_SEL_Pos)

/* SW_CLK_GATE_EN_0, offset: 0x200 */
#define WJ_SW_CLK_GATE_EN_0_I2C5_CLK_EN_Pos                  (0U)
#define WJ_SW_CLK_GATE_EN_0_I2C5_CLK_EN_Msk                  (0x1U << WJ_SW_CLK_GATE_EN_0_I2C5_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_I2C4_CLK_EN_Pos                  (1U)
#define WJ_SW_CLK_GATE_EN_0_I2C4_CLK_EN_Msk                  (0x1U << WJ_SW_CLK_GATE_EN_0_I2C4_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_I2C3_CLK_EN_Pos                  (2U)
#define WJ_SW_CLK_GATE_EN_0_I2C3_CLK_EN_Msk                  (0x1U << WJ_SW_CLK_GATE_EN_0_I2C3_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_I2C2_CLK_EN_Pos                  (3U)
#define WJ_SW_CLK_GATE_EN_0_I2C2_CLK_EN_Msk                  (0x1U << WJ_SW_CLK_GATE_EN_0_I2C2_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_I2C1_CLK_EN_Pos                  (4U)
#define WJ_SW_CLK_GATE_EN_0_I2C1_CLK_EN_Msk                  (0x1U << WJ_SW_CLK_GATE_EN_0_I2C1_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_I2C0_CLK_EN_Pos                  (5U)
#define WJ_SW_CLK_GATE_EN_0_I2C0_CLK_EN_Msk                  (0x1U << WJ_SW_CLK_GATE_EN_0_I2C0_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_GPIO2_CLK_EN_Pos                 (6U)
#define WJ_SW_CLK_GATE_EN_0_GPIO2_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_GPIO2_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_GPIO1_CLK_EN_Pos                 (7U)
#define WJ_SW_CLK_GATE_EN_0_GPIO1_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_GPIO1_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_GPIO0_CLK_EN_Pos                 (8U)
#define WJ_SW_CLK_GATE_EN_0_GPIO0_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_GPIO0_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_UART5_CLK_EN_Pos                 (9U)
#define WJ_SW_CLK_GATE_EN_0_UART5_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_UART5_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_UART4_CLK_EN_Pos                 (10U)
#define WJ_SW_CLK_GATE_EN_0_UART4_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_UART4_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_UART3_CLK_EN_Pos                 (11U)
#define WJ_SW_CLK_GATE_EN_0_UART3_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_UART3_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_UART2_CLK_EN_Pos                 (12U)
#define WJ_SW_CLK_GATE_EN_0_UART2_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_UART2_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_UART1_CLK_EN_Pos                 (13U)
#define WJ_SW_CLK_GATE_EN_0_UART1_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_UART1_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_UART0_CLK_EN_Pos                 (14U)
#define WJ_SW_CLK_GATE_EN_0_UART0_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_UART0_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_SPI_CLK_EN_Pos                   (15U)
#define WJ_SW_CLK_GATE_EN_0_SPI_CLK_EN_Msk                   (0x1U << WJ_SW_CLK_GATE_EN_0_SPI_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_QSPI1_CLK_EN_Pos                 (16U)
#define WJ_SW_CLK_GATE_EN_0_QSPI1_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_QSPI1_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_QSPI0_CLK_EN_Pos                 (17U)
#define WJ_SW_CLK_GATE_EN_0_QSPI0_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_QSPI0_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_PWM_CLK_EN_Pos                   (18U)
#define WJ_SW_CLK_GATE_EN_0_PWM_CLK_EN_Msk                   (0x1U << WJ_SW_CLK_GATE_EN_0_PWM_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_GMAC_CLK_EN_Pos                  (19U)
#define WJ_SW_CLK_GATE_EN_0_GMAC_CLK_EN_Msk                  (0x1U << WJ_SW_CLK_GATE_EN_0_GMAC_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_EMMC1_CLK_EN_Pos                 (20U)
#define WJ_SW_CLK_GATE_EN_0_EMMC1_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_EMMC1_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_EMMC0_CLK_EN_Pos                 (21U)
#define WJ_SW_CLK_GATE_EN_0_EMMC0_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_EMMC0_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_EMMC0_X2X_CLK_EN_Pos             (22U)
#define WJ_SW_CLK_GATE_EN_0_EMMC0_X2X_CLK_EN_Msk             (0x1U << WJ_SW_CLK_GATE_EN_0_EMMC0_X2X_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_EMMC1_X2X_CLK_EN_Pos             (23U)
#define WJ_SW_CLK_GATE_EN_0_EMMC1_X2X_CLK_EN_Msk             (0x1U << WJ_SW_CLK_GATE_EN_0_EMMC1_X2X_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_DRD_CLK_EN_Pos                   (24U)
#define WJ_SW_CLK_GATE_EN_0_DRD_CLK_EN_Msk                   (0x1U << WJ_SW_CLK_GATE_EN_0_DRD_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_DRD_SUSPEND_CLK_EN_Pos           (25U)
#define WJ_SW_CLK_GATE_EN_0_DRD_SUSPEND_CLK_EN_Msk           (0x1U << WJ_SW_CLK_GATE_EN_0_DRD_SUSPEND_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_DRD_CTRL_REF_CLK_EN_Pos          (26U)
#define WJ_SW_CLK_GATE_EN_0_DRD_CTRL_REF_CLK_EN_Msk          (0x1U << WJ_SW_CLK_GATE_EN_0_DRD_CTRL_REF_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_DRD_PHY_REF_CLK_EN_Pos           (27U)
#define WJ_SW_CLK_GATE_EN_0_DRD_PHY_REF_CLK_EN_Msk           (0x1U << WJ_SW_CLK_GATE_EN_0_DRD_PHY_REF_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_PERISYS_APB2_HCLK_EN_Pos         (28U)
#define WJ_SW_CLK_GATE_EN_0_PERISYS_APB2_HCLK_EN_Msk         (0x1U << WJ_SW_CLK_GATE_EN_0_PERISYS_APB2_HCLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_PERISYS_APB1_HCLK_EN_Pos         (29U)
#define WJ_SW_CLK_GATE_EN_0_PERISYS_APB1_HCLK_EN_Msk         (0x1U << WJ_SW_CLK_GATE_EN_0_PERISYS_APB1_HCLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_PERISYS_AHB_HCLK_EN_Pos          (30U)
#define WJ_SW_CLK_GATE_EN_0_PERISYS_AHB_HCLK_EN_Msk          (0x1U << WJ_SW_CLK_GATE_EN_0_PERISYS_AHB_HCLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_PERISYS_ACLK_EN_Pos              (31U)
#define WJ_SW_CLK_GATE_EN_0_PERISYS_ACLK_EN_Msk              (0x1U << WJ_SW_CLK_GATE_EN_0_PERISYS_ACLK_EN_Pos)

/* SW_CLK_GATE_EN_1, offset: 0x204 */
#define WJ_SW_CLK_GATE_EN_0_TIMER0_CLK_EN_Pos                (0U)
#define WJ_SW_CLK_GATE_EN_0_TIMER0_CLK_EN_Msk                (0x1U << WJ_SW_CLK_GATE_EN_0_TIMER0_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_TIMER1_CLK_EN_Pos                (1U)
#define WJ_SW_CLK_GATE_EN_0_TIMER1_CLK_EN_Msk                (0x1U << WJ_SW_CLK_GATE_EN_0_TIMER1_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_WDT1_CLK_EN_Pos                  (2U)
#define WJ_SW_CLK_GATE_EN_0_WDT1_CLK_EN_Msk                  (0x1U << WJ_SW_CLK_GATE_EN_0_WDT1_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_WDT0_CLK_EN_Pos                  (3U)
#define WJ_SW_CLK_GATE_EN_0_WDT0_CLK_EN_Msk                  (0x1U << WJ_SW_CLK_GATE_EN_0_WDT0_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_MBOX3_CLK_EN_Pos                 (4U)
#define WJ_SW_CLK_GATE_EN_0_MBOX3_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_MBOX3_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_MBOX2_CLK_EN_Pos                 (5U)
#define WJ_SW_CLK_GATE_EN_0_MBOX2_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_MBOX2_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_MBOX1_CLK_EN_Pos                 (6U)
#define WJ_SW_CLK_GATE_EN_0_MBOX1_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_MBOX1_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_MBOX0_CLK_EN_Pos                 (7U)
#define WJ_SW_CLK_GATE_EN_0_MBOX0_CLK_EN_Msk                 (0x1U << WJ_SW_CLK_GATE_EN_0_MBOX0_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_DMAC_CLK_EN_Pos                  (8U)
#define WJ_SW_CLK_GATE_EN_0_DMAC_CLK_EN_Msk                  (0x1U << WJ_SW_CLK_GATE_EN_0_DMAC_CLK_EN_Pos)

#define WJ_SW_CLK_GATE_EN_0_C910_BROM_HCLK_EN_Pos            (9U)
#define WJ_SW_CLK_GATE_EN_0_C910_BROM_HCLK_EN_Msk            (0x1U << WJ_SW_CLK_GATE_EN_0_C910_BROM_HCLK_EN_Pos)

/* BOOT_CLK_SEL_CFG, offset: 0x280 */
#define WJ_BOOT_CLK_SEL_CFG_BOOT_PAD_SEL_Pos                 (0U)
#define WJ_BOOT_CLK_SEL_CFG_BOOT_PAD_SEL_Msk                 (0x1U << WJ_BOOT_CLK_SEL_CFG_BOOT_PAD_SEL_Pos)

/* TEST_CLK_CFG, offset: 0x300 */
#define WJ_TEST_CLK_CFG_CLK_SEL_Pos                 (0U)
#define WJ_TEST_CLK_CFG_CLK_SEL_Msk                 (0xFU << WJ_TEST_CLK_CFG_CLK_SEL_Pos)

#define WJ_TEST_CLK_CFG_SAMPLE_EN_Pos               (4U)
#define WJ_TEST_CLK_CFG_SAMPLE_EN_Msk               (0x1U << WJ_TEST_CLK_CFG_SAMPLE_EN_Pos)

/* TEST_CLK_STS, offset: 0x304 */
#define WJ_TEST_CLK_STS_CLK_FREQ_STAT_Pos           (0U)
#define WJ_TEST_CLK_STS_CLK_FREQ_STAT_Msk           (0xFFFFFFFFU << WJ_TEST_CLK_STS_CLK_FREQ_STAT_Pos)

static inline void wj_perisys_ahb_hclk_cfg_clock_sel_pll(wj_apsys_clkgen_reg_t *clk_base)
{
    clk_base->PERISYS_AHB_HCLK_CFG &= ~(WJ_PERISYS_AHB_HCLK_CFG_SW_SEL_Msk);
}

static inline void wj_perisys_ahb_hclk_cfg_div_num(wj_apsys_clkgen_reg_t *clk_base, uint8_t div_num)
{
    clk_base->PERISYS_AHB_HCLK_CFG &= ~(WJ_PERISYS_AHB_HCLK_CFG_DIV_NUM_Msk);
    clk_base->PERISYS_AHB_HCLK_CFG |= (uint32_t)(div_num << WJ_PERISYS_AHB_HCLK_CFG_DIV_NUM_Pos);
}

static inline void wj_perisys_ahb_hclk_cfg_div_en(wj_apsys_clkgen_reg_t *clk_base)
{
    clk_base->PERISYS_AHB_HCLK_CFG &= ~(uint32_t)(WJ_PERISYS_AHB_HCLK_CFG_DIV_EN_Msk);
    clk_base->PERISYS_AHB_HCLK_CFG |= (uint32_t)(1 << WJ_PERISYS_AHB_HCLK_CFG_DIV_EN_Pos);
}

static inline void wj_perisys_apb_pclk_cde_radio(wj_apsys_clkgen_reg_t *clk_base, uint8_t radio)
{
    clk_base->PERISYS_APB_PCLK_CFG &= ~(WJ_PERISYS_APB_PCLK_CFG_DIV_NUM_Msk);
    clk_base->PERISYS_APB_PCLK_CFG |= (uint32_t)(radio << WJ_PERISYS_APB_PCLK_CFG_DIV_NUM_Pos);
}

static inline void wj_perisys_apb_pclk_cde_sync(wj_apsys_clkgen_reg_t *clk_base)
{
    clk_base->PERISYS_APB_PCLK_CFG &= ~(WJ_PERISYS_APB_PCLK_CFG_DIV_EN_Msk);
    clk_base->PERISYS_APB_PCLK_CFG |= (uint32_t)(1 << WJ_PERISYS_APB_PCLK_CFG_DIV_EN_Pos);
}

static inline void wj_sw_clk_gate_en_0_on(wj_apsys_clkgen_reg_t *clk_base, uint32_t idx)
{
    clk_base->SW_CLK_GATE_EN_0 |= (uint32_t)(1 << idx);
}

static inline void wj_sw_clk_gate_en_0_off(wj_apsys_clkgen_reg_t *clk_base, uint32_t idx)
{
    clk_base->SW_CLK_GATE_EN_0 &= ~(1 << idx);
}

static inline void wj_sw_clk_gate_en_1_on(wj_apsys_clkgen_reg_t *clk_base, uint32_t idx)
{
    clk_base->SW_CLK_GATE_EN_1 |= (uint32_t)(1 << idx);
}

static inline void wj_sw_clk_gate_en_1_off(wj_apsys_clkgen_reg_t *clk_base, uint32_t idx)
{
    clk_base->SW_CLK_GATE_EN_1 &= ~(1 << idx);
}

static inline void wj_sw_clk_gate_en_2_on(wj_apsys_clkgen_reg_t *clk_base, uint32_t idx)
{
    clk_base->SW_CLK_GATE_EN_2 |= (uint32_t)(1 << idx);
}

static inline void wj_sw_clk_gate_en_2_off(wj_apsys_clkgen_reg_t *clk_base, uint32_t idx)
{
    clk_base->SW_CLK_GATE_EN_2 &= ~(1 << idx);
}

/**
  \brief       Set the system clock according to the parameter
  \param[in]   config    system clock config.
  \return      error code
*/
csi_error_t soc_sysclk_config(system_clk_config_t *config);

/**
  \brief       Set iic reset
  \param[in]   idx    iic idx.
  \return      Null
*/
void soc_reset_iic(uint32_t idx);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_CLK_H_ */

