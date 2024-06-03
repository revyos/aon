
/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     soc.h
 * @brief    CSI Core Peripheral Access Layer Header File for
 *           CSKYSOC Device Series
 * @version  V1.0
 * @date     2019-12-18
 ******************************************************************************/

#ifndef _SOC_H_
#define _SOC_H_


#include "csi_core.h"                     /* Processor and core peripherals */
#include "stdint.h"
#include "sys_clk.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_BOOT_FR_UBOOT

#ifndef EHS_VALUE
#define EHS_VALUE    20000000U
#endif

#ifndef ELS_VALUE
#define ELS_VALUE    32768U
#endif

#ifndef IHS_VALUE
#define IHS_VALUE    50000000U
#endif

#ifndef ILS_VALUE
#define ILS_VALUE    32768U
#endif

typedef enum {
    CORET_IRQn                     =   7,      /*core-timer         */
    DW_GPIO_IRQn                   =   28,     /* ao-gpio interrupt */
    WJ_RTC_IRQn                    =   29,     /* rtc interrupt */
    WJ_CPR_IRQn                    =   30,     /* cpr interrupt */
    MR_PVT_IRQn                    =   31,     /* pvct interrupt */
    DW_I2C0_IRQn                   =   32,     /* iic interrupt */
    DW_TIMER0_IRQn                 =   33,     /* timerc0 Interrupt */
    DW_TIMER1_IRQn                 =   34,     /* timerc0 Interrupt */
    DW_TIMER2_IRQn                 =   35,     /* timerc0 Interrupt */
    DW_TIMER3_IRQn                 =   36,     /* timerc0 Interrupt */
    DW_AUDIO_IRQn                  =   37,     /* wdt Interrupt */
    WJ_MBOX1_IRQn                   =   38,     /* MBOX Interrupt */
    DW_WDT0_IRQn                   =   40,     /* Aon-WDT Interrupt */
    DW_UART1_IRQn                  =   39,     /* UART0 Interrupt*/
    DW_UART_AO_IRQn                =   44,     /* Aon uart interrupt */
    DW_WDT1_IRQn                   =   45,     /* AP-WDT0 Interrupt */
    DW_WDT2_IRQn                   =   46,     /* AP-WDT1 Interrupt */
    DW_WDT3_IRQn                   =   47,     /* AUD-WDT0 Interrupt */
}irqn_type_t;

/*
typedef enum {
}wakeupn_type_t;
*/

/*
typedef enum {
} clk_manager_type_t;
*/

#define SOC_PIN_AP_RIGHT_TOP          (0x0)
#define SOC_PIN_AP_LEFT_TOP           (0x1)
#define SOC_PIN_AON                   (0x2)

#define PAD_GRP_BASE_SET(x)           (x << 12)
#define PAD_GRP_IDX_GET(x)            ( (x >> 12) & 0xF)
#define PAD_INDEX(x)                  (x & 0xFFF)

typedef enum {
	UART0_TXD = PAD_GRP_BASE_SET(SOC_PIN_AP_RIGHT_TOP),
	UART0_RXD,
	QSPI0_SCLK,
	QSPI0_CSN0,
	QSPI0_CSN1,
	QSPI0_D0_MOSI,
	QSPI0_D1_MISO,
	QSPI0_D2_WP,
	QSPI0_D3_HOLD,
	I2C2_SCL,
	I2C2_SDA,
	I2C3_SCL,
	I2C3_SDA,
	GPIO2_13,
	SPI_SCLK,
	SPI_CSN,
	SPI_MOSI,
	SPI_MISO,
	GPIO2_18,
	GPIO2_19,
	GPIO2_20,
	GPIO2_21,
	GPIO2_22,
	GPIO2_23,
	GPIO2_24,
	GPIO2_25,
	SDIO0_WPRTN,
	SDIO0_DETN,
	SDIO1_WPRTN,
	SDIO1_DETN,
	GPIO2_30,
	GPIO2_31,
	GPIO3_0,
	GPIO3_1,
	GPIO3_2,
	GPIO3_3,
	HDMI_SCL,
	HDMI_SDA,
	HDMI_CEC,
	GMAC0_TX_CLK,
	GMAC0_RX_CLK,
	GMAC0_TXEN,
	GMAC0_TXD0,
	GMAC0_TXD1,
	GMAC0_TXD2,
	GMAC0_TXD3,
	GMAC0_RXDV,
	GMAC0_RXD0,
	GMAC0_RXD1,
	GMAC0_RXD2,
	GMAC0_RXD3,
	GMAC0_MDC,
	GMAC0_MDIO,
	GMAC0_COL,
	GMAC0_CRS,

	QSPI1_SCLK = PAD_GRP_BASE_SET(SOC_PIN_AP_LEFT_TOP),
	QSPI1_CSN0,
	QSPI1_D0_MOSI,
	QSPI1_D1_MISO,
	QSPI1_D2_WP,
	QSPI1_D3_HOLD,
	I2C0_SCL,
	I2C0_SDA,
	I2C1_SCL,
	I2C1_SDA,
	UART1_TXD,
	UART1_RXD,
	UART4_TXD,
	UART4_RXD,
	UART4_CTSN,
	UART4_RTSN,
	UART3_TXD,
	UART3_RXD,
	GPIO0_18,
	GPIO0_19,
	GPIO0_20,
	GPIO0_21,
	GPIO0_22,
	GPIO0_23,
	GPIO0_24,
	GPIO0_25,
	GPIO0_26,
	GPIO0_27,
	GPIO0_28,
	GPIO0_29,
	GPIO0_30,
	GPIO0_31,
	GPIO1_0,
	GPIO1_1,
	GPIO1_2,
	GPIO1_3,
	GPIO1_4,
	GPIO1_5,
	GPIO1_6,
	GPIO1_7,
	GPIO1_8,
	GPIO1_9,
	GPIO1_10,
	GPIO1_11,
	GPIO1_12,
	GPIO1_13,
	GPIO1_14,
	GPIO1_15,
	GPIO1_16,
	CLK_OUT_0,
	CLK_OUT_1,
	CLK_OUT_2,
	CLK_OUT_3,
	GPIO1_21,
	GPIO1_22,
	GPIO1_23,
	GPIO1_24,
	GPIO1_25,
	GPIO1_26,
	GPIO1_27,
	GPIO1_28,
	GPIO1_29,
	GPIO1_30,

	OSC_CLK_IN= PAD_GRP_BASE_SET(SOC_PIN_AON),
	OSC_CLK_OUT,
	SYS_RST_N,
	RTC_CLK_IN,
	RTC_CLK_OUT,
	TEST_MODE,
	DFT_PROT_DIS_0,
	DEBUG_MODE,
	POR_SEL,
	I2C_AON_SCL,
	I2C_AON_SDA,
	CPU_JTG_TCLK,
	CPU_JTG_TMS,
	CPU_JTG_TDI,
	CPU_JTG_TDO,
	CPU_JTG_TRST,
	AOGPIO_7,
	AOGPIO_8,
	AOGPIO_9,
	AOGPIO_10,
	AOGPIO_11,
	AOGPIO_12,
	AOGPIO_13,
	AOGPIO_14,
	AOGPIO_15,
	AUDIO_PA0,
	AUDIO_PA1,
	AUDIO_PA2,
	AUDIO_PA3,
	AUDIO_PA4,
	AUDIO_PA5,
	AUDIO_PA6,
	AUDIO_PA7,
	AUDIO_PA8,
	AUDIO_PA9,
	AUDIO_PA10,
	AUDIO_PA11,
	AUDIO_PA12,
	AUDIO_PA13,
	AUDIO_PA14,
	AUDIO_PA15,
	AUDIO_PA16,
	AUDIO_PA17,
	AUDIO_PA27,
	AUDIO_PA28,
	AUDIO_PA29,
	AUDIO_PA30,
} pin_name_t;

typedef enum {
	PIN_FUNC_GPIO =3,
}pin_func_t;

enum {
	PIN_PD,
	PIN_PU,
	PIN_PN,
};

enum {
	PIN_SPEED_NORMAL,
	PIN_SPEED_FAST
};

typedef enum {
	SOC_DVDD18_AON,      /*da9063:  ldo-3 */
	SOC_AVDD33_USB3,     /*da9063:  ldo-9 */
	SOC_DVDD08_AON,      /*da9063:  ldo-2 */
	SOC_APCPU_DVDD_DVDDM,/*da9063:  vbcore1 & vbcore2*/
	SOC_DVDD08_DDR,      /*da9063:  buckperi */
	SOC_VDD_DDR_1V8,     /*da9063:  ldo-4  */
	SOC_VDD_DDR_1V1,     /*da9063:  buckmem & buckio  */
	SOC_VDD_DDR_0V6,     /*da9063:  buckpro */
	SOC_DVDD18_AP,       /*da9063:  ldo-11 */
	SOC_DVDD08_AP,       /*da9121:  da9121_ex */
	SOC_AVDD08_MIPI_HDMI,/*da9063:  ldo-1  */
	SOC_AVDD18_MIPI_HDMI,/*da9063:  ldo-5  */
	SOC_DVDD33_EMMC,     /*da9063:  ldo-10 */
	SOC_DVDD18_EMMC,     /*slg51000:ldo-3 */
	SOC_DOVDD18_SCAN,    /*da9063:  ldo-6 */
	SOC_VEXT_2V8,        /*da9063:  ldo-7 */
	SOC_DVDD12_SCAN,     /*da9063:  ldo-8 */
	SOC_AVDD28_SCAN_EN,  /*da9063:  gpio-4,SGM2019-ADJ */
	SOC_AVDD28_RGB,      /*slg51000:ldo-1 */
	SOC_DOVDD18_RGB,     /*slg51000:ldo-4 */
	SOC_DVDD12_RGB,      /*slg51000:ldo-5 */
	SOC_AVDD25_IR,       /*slg51000:ldo-2 */
	SOC_DOVDD18_IR,      /*slg51000:ldo-7 */
	SOC_DVDD12_IR,       /*slg51000:ldo-6 */
	SOC_ADC_VREF,
	SOC_LCD0_EN,
	SOC_VEXT_1V8,

	SOC_REGU_INVALID = 0xFF
}soc_virtual_id_t;


#define CONFIG_IRQ_NUM  64

#define WJ_IOC_BASE                0xFFCCD00000UL
#define WJ_IOC_SIZE                0x100000U
#define SRAM_BASE                  0xFFF54000
#define SRAM_SIZE                  0x8000

#define WJ_RTC_BASE                0xFFF40000UL
#define WJ_RTC_BASE_SIZE           0x1000U

#define DW_GPIO_BASE               0xFFF41000UL
#define DW_GPIO_BASE_SIZE          0x1000U

#define WJ_CPR_BASE                0xFFF42000UL
#define WJ_CPR_BASE_SIZE           0x8000U

#define WJ_PAD_BASE                0xFFF4A000UL
#define WJ_PAD_BASE_SIZE           0x2000U

#define DW_I2C0_BASE               0xFFF4C000UL
#define DW_I2C0_BASE_SIZE          0x1000U

#define DW_TIMER0_BASE             0xFFF4D000UL
#define DW_TIMER0_SIZE             0x14U

#define DW_TIMER1_BASE             (DW_TIMER0_BASE+DW_TIMER0_SIZE)
#define DW_TIMER1_SIZE             DW_TIMER0_SIZE

#define DW_TIMER2_BASE             (DW_TIMER1_BASE+DW_TIMER1_SIZE)
#define DW_TIMER2_SIZE             DW_TIMER1_SIZE

#define DW_TIMER3_BASE             (DW_TIMER2_BASE+DW_TIMER2_SIZE)
#define DW_TIMER3_SIZE             DW_TIMER2_SIZE

#define DW_PVT_CTRL_BASE           0xFFF4E000UL
#define DW_PVT_CTRL_BASE_SIZE      0x2000U

/*ap-i2c*/
#define DW_API2C0_BASE		   0xB7F20000U
#define DW_API2C1_BASE		   0xB7F24000U
#define DW_API2C2_BASE		   0xBC00C000U
#define DW_API2C3_BASE		   0xBC014000U
#define DW_API2C4_BASE		   0xB7F28000U
#define DW_API2C5_BASE		   0xB7F2C000U

/*ap-i2c*/
#define DW_AUDI2C0_BASE		   0xCB01A000U
#define DW_AUDI2C1_BASE		   0xCB01B000U

/*aon-wdt*/
#define DW_WDT0_BASE		   0xFFF50000U
#define DW_WDT0_SIZE		   0x1000U

/*ap-wdt0*/
#define DW_WDT1_BASE		   0xBFC30000U
#define DW_WDT1_SIZE		   0x1000U

/*ap-wdt1*/
#define DW_WDT2_BASE		   0xBFC31000U
#define DW_WDT2_SIZE		   0x1000U

/*audio-wdt*/
#define DW_WDT3_BASE		   0xCB010000U
#define DW_WDT3_SIZE		   0x1000U

#define DW_UART_AO_BASE		   0xFFF53000UL
#define DW_UART_AO_BASE_SIZE	   0x1000U

#define DW_UART1_BASE              0xB7F00000UL
#define DW_UART1_BASE_SIZE         0x4000U

#define DW_UART0_BASE              0xB7014000UL
#define DW_UART0_SIZE              0x4000U

#define DW_UART2_BASE              0xBC010000UL
#define DW_UART2_SIZE              0x4000U

#define DW_UART3_BASE              0xB7F04000UL
#define DW_UART3_SIZE              0x4000U

#define DW_UART4_BASE              0xF7F08000UL
#define DW_UART4_SIZE              0x4000U

#define DW_UART5_BASE              0xF7F0C000UL
#define DW_UART5_SIZE              0x4000U

#define WJ_MBOX1_BASE               0xFFC38000UL
#define WJ_MBOX1_SIZE               0x1000U

#define DW_EMMC_BASE               0xB7080000L

#define DW_SDIO0_BASE               0xB7090000L
#define DW_SDIO1_BASE               0xB70A0000L
#define DW_SPI0_BASE		    0xB700C000L
#define DW_QSPI1_BASE		    0xF8000000L
#define DW_VDEC_BASE		    0xBCC00000L
#define DW_G2D_BASE		    0xBCC80000L
#define DW_DSP0_APB_BASE    	    0xBF048000L
#define DW_DSP1_APB_BASE    	    0xBF050000L
#define DW_DDR_CTRL_BASE	    0xFF000000L
#define DW_GPU_BASE		    0xBF400000L
#define DW_DMAC_BASE		    0xBFC00000L
#define DW_AUDDMAC_BASE		    0xC8000000L

#define MR_PVT_BASE                0xFFF4E000UL
#define MR_PVT_SIZE                0x2000U
#define MR_PVT_TS_OFFSET           (0x0080)
#define MR_PVT_PD_OFFSET           (0x0180)
#define MR_PVT_VM_OFFSET           (0x0800)

#define AONSYS_RSTGEN_BASE		0xFFF44000UL
#define AONSYS_CLKGEN_BASE		0xFFF46000UL
#define AONSYS_SYSREG_BASE		0xFFF48000UL


#define CONFIG_GPIO_NUM             1U
#define CONFIG_MAILBOX_CHANNEL_NUM  4U

#define CONFIG_RTC_FAMILY_E

#define AONSYS_WDT_RESET_EN                               (AONSYS_RSTGEN_BASE+0x140)
#define AONSYS_WDT_RESET_CLR                              (AONSYS_RSTGEN_BASE+0x104)
#define AONSYS_WDT_RESET_ST                               (AONSYS_RSTGEN_BASE+0x124)

#define APWDT0_INDEX 0
#define APWDT1_INDEX 1
#define AUDWDT_INDEX 2


#ifdef __cplusplus
}
#endif

#endif  /* _SOC_H_ */
