#ifndef _POWER_H
#define _POWER_H

#include "aonsys_reg_sw.h"
#include "aonsys_reg_define.h"
#include "aonsys_rstget_reg_sw.h"
#include "aonsys_rstget_reg_define.h"
#include "aonsys_clkgen_reg_define.h"
#include "aonsys_clkgen_reg_sw.h"
#include "apsys_rstget_reg_define.h"
#include "vpsys_sysreg_define.h"
#include "padctrl_aonsys_reg_define.h"
#include "aon_pmu_reg_define.h"
#include "pmic_rsc_def.h"
#include "power.h"
#include "drv/pmic.h"
#include "syslog.h"

/* user configuration */
#define CONFIG_LOWPOWER_EMU
#define LOG_SEVERITY_MAX	LOG_SEVERITY_INFO
#define LOG_SEVERITY_NO_OUTPUT  0
#define LOG_SEVERITY_CRIT       1
#define LOG_SEVERITY_WARN       2
#define LOG_SEVERITY_INFO       3
#define LOG_SEVERITY_DEBUG      4

#if LOG_SEVERITY_MAX >= LOG_SEVERITY_CRIT
#define LOG_CRIT_ENABLED
#endif

#if LOG_SEVERITY_MAX >= LOG_SEVERITY_WARN
#define LOG_WARN_ENABLED
#endif

#if LOG_SEVERITY_MAX >= LOG_SEVERITY_INFO
#define LOG_INFO_ENABLED
#endif

#if LOG_SEVERITY_MAX >= LOG_SEVERITY_DEBUG
#define LOG_DEBUG_ENABLED
#endif

// #ifdef LOG_CRIT_ENABLED
// #define LOG_CRIT LOG_D
// #else
// #define LOG_CRIT(...)
// #endif

// #ifdef LOG_WARN_ENABLED
// #define LOG_WARN LOG_D
// #else
// #define LOG_WARN(...)
// #endif

// #ifdef LOG_INFO_ENABLED
// #define LOG_INFO LOG_D
// #else
// #define LOG_INFO(...)
// #endif

// #ifdef LOG_DEBUG_ENABLED
// #define LOG_DBG LOG_D
// #else
// #define LOG_DBG(...)
// #endif

/* chip sys/rst registers define */
#define C910_CORE0			0
#define C910_CORE1			1
#define C910_CORE2			2
#define C910_CORE3			3
#define C906_CORE0			4
#define MAX_C910_CORE_NUM		4

#define APSYS_SOC_EXT_ID_DEF(ext_id)\
{\
	.regu_ext_id = ext_id,\
}

/* REG_AON_CHIP_LP_MODE bit define */
/*
|  31~16  |  2 ~ 15 |    1       |     0    |
| LP MODE | Reserve | IOPMP FLAG | DDR FLAG |
*/
#define CHIP_LP_MODE_OFFSET   (16)
#define CHIP_LP_MODE_MASK   (0xFFFFUL << CHIP_LP_MODE_OFFSET)

#define CHIP_LP_NONE		(1 << 0)
#define CHIP_LP_STANDBY		(1 << 1)
#define CHIP_LP_HW_VAD		(1 << 2)
/* ddr resume complete flag */
#define CHIP_LP_MODE_DDR_FLAG_OFFSET (0)
#define CHIP_LP_MODE_DDR_FLAG_MASK (1UL << CHIP_LP_MODE_DDR_FLAG_OFFSET)
/* iopmp resume complete flag */
#define CHIP_LP_MODE_IOPMP_FLAG_OFFSET (1)
#define CHIP_LP_MODE_IOPMP_FLAG_MASK (1UL << CHIP_LP_MODE_IOPMP_FLAG_OFFSET)


#define CPU_LP_MODE_MSK		0x3
#define CPU_LP_MODE_OFF(x)	(x << 1)

#define SYS_PD_SW_EN_MSK	0x3
#define SYS_PD_SW_EN_OFF(mod)	(mod << 1)

#define SYS_PD_ISO_EN_SET_OFF(mod)	(1 << mod)
#define SYS_PD_ISO_EN_CLR_OFF(mod)      (1 << mod)

#define SYS_PD_REQ_MSK		0x1
#define SYS_PD_REQ_OFF(mod)	(1 << mod)
#define SYS_PU_REQ_OFF(mod) (1 << (16 + mod))

#define AON_C910_CORE0_DBG_MASK	(1 << 0)
#define AON_C910_CORE1_DBG_MASK	(1 << 1)
#define AON_C910_CORE2_DBG_MASK	(1 << 2)
#define AON_C910_CORE3_DBG_MASK	(1 << 3)

#define SW_AUDIO_RST_n		(1 << 5)
#define SW_AUDIO_SUBSYS_IOPMP_CP_RST_n	(1 << 4)
#define SW_AUDIO_SUBSYS_CRST_n	(1 << 3)
#define SW_AUDIO_SUBSYS_ARST_n_CP2SRAM	(1 << 2)
#define SW_AUDIO_SUBSYS_ARST_n_CP2AP	(1 << 1)
#define SW_AUDIO_SUBSYS_ARST_n_AP2CP	(1 << 0)

#define SW_AON_VDEC_RST_n	(1 << 0)
#define SW_AON_NPUSYS_RST_n	(1 << 0)
#define SW_AON_VENC_RST_n	(1 << 0)
#define SW_AON_DDRSYS_RST_n	(1 << 0)
#define SW_AON_SYS_RST_REQ	(1 << 0)
#define SW_AON_APSYS_RST_n	(1 << 0)
#define SW_AON_GPU_RST_n	(1 << 0)
#define SW_AON_DSP0_RST_n        (1 << 0)
#define SW_AON_DSP1_RST_n        (1 << 0)

#define SW_VP_VDEC_CRST_n	(1 << 2)
#define SW_VP_VDEC_ARST_n	(1 << 1)
#define SW_VP_VDEC_PRST_n	(1 << 0)

#define SW_AP_SW_NPUSYS_AXI_ARST_n	(1 << 0)
#define SW_AP_SW_IMG_NNA_RST_n		(1 << 0)

#define AON_AON_ISOLATION_EN				(1 << 0)
#define AON_C910_ISOLATION_EN				(1 << 1)
#define AON_DDR_ISOLATION_EN				(1 << 2)
#define AON_USB3_ISOLATION_EN				(1 << 3)

#define AON_C910_CPU_L2CACHE_FLUSH_REQ		(1 << 0)
#define AON_C910_CPU_L2CACHE_FLUSH_DONE		(1 << 1)
#define AON_C910_CPU_NO_OP					(1 << 2)

#define AON_C910_CORE0_MEM_DSLP					(1 << 0)
#define AON_C910_CORE0_MEM_SLP					(1 << 1)
#define AON_C910_CORE0_MEM_SD					(1 << 2)
#define AON_C910_CORE1_MEM_DSLP					(1 << 4)
#define AON_C910_CORE1_MEM_SLP					(1 << 5)
#define AON_C910_CORE1_MEM_SD					(1 << 6)
#define AON_C910_CORE2_MEM_DSLP					(1 << 8)
#define AON_C910_CORE2_MEM_SLP					(1 << 9)
#define AON_C910_CORE2_MEM_SD					(1 << 10)
#define AON_C910_CORE3_MEM_DSLP					(1 << 12)
#define AON_C910_CORE3_MEM_SLP					(1 << 13)
#define AON_C910_CORE3_MEM_SD					(1 << 14)
#define AON_C910_CPU_MEM_DSLP					(1 << 16)
#define AON_C910_CPU_MEM_SLP					(1 << 17)
#define AON_C910_CPU_MEM_SD						(1 << 18)
#define AON_SRAM_AXI_AXI_SRAM_DSLP				(1 << 20)
#define AON_SRAM_AXI_AXI_SRAM_SLP				(1 << 21)
#define AON_SRAM_AXI_AXI_SRAM_SD				(1 << 22)

#define AON_AONSYS_CLK_SWITCH_0_SWITCH_SEL		(1 << 4)
#define AON_AONSYS_CLK_SWITCH_1_SWITCH_SEL		(1 << 5)
#define AON_AONSYS_CLK_CDE_SYNC				(1 << 3)
#define AON_APSYS_CLK_EN				(1 << 0)

#define AON_AONSYS_CLK_CDE_RATIO_OFF			(1 << 0)
#define AON_AONSYS_CLK_CDE_RATIO_MSK                    0x7

#define INDICATOR_0_MAGIC_NUM		0x5a5a5a5a
#define INDICATOR_1_MAGIC_NUM		0x12345678
#define INDICATOR_2_MAGIC_NUM		0x32fde438
#define INDICATOR_3_MAGIC_NUM		0x8ab4c52c

#define AON_RC_MODE	(1 << 0)
#define AON_RC_READY	(1 << 0)
#define AON_RC_EN	(1 << 0)

#define AON_AUDIO_PLL_RST	(1 << 29)
#define AON_SYS_PLL_RST		(1 << 29)

#define PMU_AOGPIO_4_INT_MASK		(1 << 4)		//KEY1 => JTG_TDI
#define PMU_AOGPIO_5_INT_MASK		(1 << 5)		//KEY2 => JTG_TDO
#define PAD_AONSYS_OSC_CLK_IO_EN	(1 << 8)
#define PMU_GPIO_INT_EN_OFF		(1 << 12)
#define PMU_RTC_INT_EN_OFF		(1 << 13)
#define PMU_INT_CLR			(1 << 0)
#define PMU_INTC_INT_MASK_L		0xffff
#define PMU_INTC_INT_MASK_H		0xffff
#define PMU_AOGPIO_4_INT_MASK		(1 << 4)
#define PMU_AOGPIO_5_INT_MASK		(1 << 5)

#define AON_IRQ_NUM_STAR		0
#define MAX_AON_IRQ_NUM			29
#define AON_INT_AUDIO_PMU_IRQn		37
#define INT_AONSYS_CPR_IRQn		30

#define AON_CLKGEN_SYS_PLL_LOCK		(1 << 1)
#define AON_CLKGEN_AUDIO_PLL_LOCK	(1 << 0)
#define AON_CLKGEN_C906_CCLK_DIV_0_DIV_EN	(1 << 4)
#define AON_CLKGEN_C906_CCLK_DIV_1_DIV_EN	(1 << 12)
#define AON_CLKGEN_AUDIO_SUBSYS_ACLK_AP2CP_EN	(1 << 1)
#define AON_CLKGEN_AUDIO_SUBSYS_ACLK_CP2AP_EN	(1 << 0)
#define AON_CLKGEN_AUDIO_ACLK_DIV_0_DIV_EN	(1 << 4)
#define AON_CLKGEN_AUDIO_ACLK_DIV_1_DIV_EN	(1 << 12)
#define AON_CLKGEN_AUDIO_SUBSYS_ACK_DIV_1_DIV_EN	(1 << 12)
#define AON_CLKGEN_AUDIO_C906_CCLK_SWITCH_SEL	(1 << 13)
#define AON_CLKGEN_AUDIO_SUBSYS_ACLK_SWITCH_SEL	(1 << 13)
#define AON_CLKGEN_SHARE_SRAM_CLK_SWITCH_SEL	(1 << 13)

#define AON_CLKGEN_SRAM_AXI_CORE_CLK_EN	(1 << 1)
#define AON_CLKGEN_SRAM_AXI_ACLK_4_EN	(1 << 2)
#define AON_CLKGEN_PVTC_PCLK_EN		(1 << 3)
#define AON_CLKGEN_AOPAD_PCLK_EN	(1 << 4)
#define AON_CLKGEN_AOTIMER_PCLK_EN	(1 << 10)
#define AON_CLKGEN_AOTIMER_CCLK_EN	(1 << 11)
//RESERVED				(1 << 12)
#define AON_CLKGEN_AOWDT_PCLK_EN	(1 << 13)
#define AON_CLKGEN_DSPSYS_ACLK_S_EN	(1 << 14)
#define AON_CLKGEN_ADC_PCLK_EN		(1 << 15)
#define AON_CLKGEN_AUDGPIO_PCLK_EN	(1 << 16)
#define AON_CLKGEN_AUDGPIO_DBCLK_EN	(1 << 17)
#define AON_CLKGEN_AOI2C_IC_CLK_EN	(1 << 18)
#define AON_CLKGEN_AOI2C_PCLK_EN	(1 << 19)
#define AON_CLKGEN_AOUART_SCLK_EN	(1 << 20)
#define AON_CLKGEN_AOUART_PCLK_EN	(1 << 21)

#define AON_RSTGEN_SW_SRAM_AXI_ARST_n_0	(1 << 0)
#define AON_RSTGEN_SW_SRAM_AXI_ARST_n_1	(1 << 1)
#define AON_RSTGEN_SW_SRAM_AXI_ARST_n_2	(1 << 2)
#define AON_RSTGEN_SW_SRAM_AXI_ARST_n_3	(1 << 3)
#define AON_RSTGEN_SW_SRAM_AXI_ARST_n_4	(1 << 4)
#define AON_RSTGEN_SW_SRAM_AXI_CORE_RST_n	(1 << 6)

#define AON_RSTGEN_SW_PVTC_PRST_n	(1 << 0)

#define AON_PMU_LP_MODE_MSK		0x7
#define AON_PMU_LP_MOD_OFF		3
#define AON_PMU_LP_MODE_SET(x)		((x & AON_PMU_LP_MODE_MSK) << AON_PMU_LP_MOD_OFF)
#define AON_PMU_LP_REQ			(1 << 1)

#define MAX_TRY_TIME_IDLE		10000
#define POWER_DOWN_DONE			0
#define POWER_ON_DONE			3

#define PD_SW_ACK_MSK			0x3

#define CHIP_REG_GET(reg)		(*(volatile uint32_t *)(reg))
#define CHIP_REG_SET(value, reg)	(*(volatile uint32_t *)(reg) = (value))

#define LIGHT_AON_PM_PW_MODE_OFF        0       /* Power off */
#define LIGHT_AON_PM_PW_MODE_STBY       1       /* Power in standby */
#define LIGHT_AON_PM_PW_MODE_LP         2       /* Power in low-power */
#define LIGHT_AON_PM_PW_MODE_ON         3       /* Power on */

#define LIGHT_AON_AUDIO_PD		0
#define LIGHT_AON_VDEC_PD		1
#define LIGHT_AON_NPU_PD		2
#define LIGHT_AON_VENC_PD		3
#define LIGHT_AON_GPU_PD		4
#define LIGHT_AON_DSP0_PD		5
#define LIGHT_AON_DSP1_PD		6

/*******************iopmp configure ***********************/
#define LIGHT_IOPMP_DEFAULT_ATTR        0xffffffff
#define LIGHT_IOPMP_DEFAULT_OFF         0xc0
#define IOPMP_VENC      0
#define IOPMP_VDEC      1
#define IOPMP_NPU       2
#define IOPMP_GPU       3
#define IOPMP_DSP0      4
#define IOPMP_DSP1      5
#define IOPMP_AUDIO     6
#define IOPMP_AUDIO0    7
#define IOPMP_AUDIO1    8

#define APSYS_CLKGEN_NPU_CCLK_TEECFG	(0xff0111c8)
#define NPU_AXI_ACLK_EN			(1 << 5)
#define NPU_CORE_CLK_EN			(1 << 4)

typedef enum _POWER_STATE {
	POWER_ON,
	POWER_DOWN,
}POWER_STATE;

typedef enum  _LIGHT_SUBSYS_NAME {
	SUBSYS_AUDIO,
	SUBSYS_VDEC,
	SUBSYS_NPU,
	SUBSYS_VENC,
	SUBSYS_GPU,
	SUBSYS_DSP0,
	SUBSYS_DSP1,
	SUBSYS_C910_CORE0,
	SUBSYS_C910_CORE1,
	SUBSYS_C910_CORE2,
	SUBSYS_C910_CORE3,
}LIGHT_SUBSYS_NAME;


typedef enum _MEM_RETENTION_MODE {
	MEM_RET_SLP,
	MEM_RET_DSLP,
	MEM_RET_SD,
}MEM_RET_MODE;

typedef enum _PLL_SLEEP_MODE {
	PLL_SLEEP,
	PLL_NORMAL,
}PLL_SLEEP_MODE;

typedef enum _SYS_LP_MODE {
	SYS_LP_NONE,
	SYS_LP_STANDBY,
	SYS_LP_HW_VAD,
}SYS_LP_MODE;

typedef enum _PMU_LP_MODE {
	PMU_LP_CLOCK_SWITCH,
	PMU_LP_CLOCK_GATE,
	PMU_LP_CPU_PLL_FREQUENCY_UPDATE,
	PMU_LP_CPU_PLL_POWERDOWN,
	PMU_LP_CPU_POWER_DOWN,
	PMU_CPU_AND_PLL_POWER_DOWN,
}PMU_LP_MODE;

typedef enum _SYS_POWER_PD_INT_NUM {
	SYS_AUDIO_PD_INTR = 16,
	SYS_VDEC_PD_INTR,
	SYS_NPU_PD_INTR,
	SYS_VENC_PD_INTR,
	SYS_GPU_PD_INTR,
	SYS_DSP0_PD_INTR,
	SYS_DSP1_PD_INTR,
	SYS_C910_CORE0_PD_PD_INTR,
	SYS_C910_CORE1_PD_PD_INTR,
	SYS_C910_CORE2_PD_PD_INTR,
	SYS_C910_CORE3_PD_PD_INTR,
}POWER_PD_INT;

typedef struct  {
	char *ip_name;
	LIGHT_SUBSYS_NAME pd_name;
} pd_match_list_t;

#define AON_WAKEUP_BY_GPIO (1 << 0)
#define AON_WAKEUP_BY_RTC  (1 << 1)

int chip_lp_mode_set_lp_mode(SYS_LP_MODE mode);
uint32_t chip_lp_mode_get_lp_mode();
uint32_t chip_lpm_suspend();
void     chip_lpm_resume(csi_pmic_t* pmic, uint32_t chip_lp_mode);
void chip_lpm_finish();
void chip_cpu_power_ctrl(int cpu_id, bool on);
int chip_power_pd_ctrl(int pd, int mode);
void chip_set_wakeup_flag(uint32_t flag);

#endif
