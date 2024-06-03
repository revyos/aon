#include "drv/pmic.h"
#include "drv/irq.h"
#include "soc.h"
#include "power.h"
#include "pmic_rsc_def.h"
#include "board_config.h"
#include "sysclk.h"
#include "drv/timer.h"
#include "aon_wdt.h"

extern int start_timer(uint32_t timeout_s);
extern int rtc_alarm();
extern uint32_t soc_irq_get_irq_num(void);
extern void soc_irq_disable(uint32_t irq_num);
extern void soc_irq_enable(uint32_t irq_num);
extern void board_resume();
extern aon_wdt_t g_aon_wdt;
int32_t enabled_irq_num[32];

extern void audio_sys_clk_enable(bool en);
extern void vdec_sys_clk_enable(bool en);
extern void gpu_sys_clk_enable(bool en);
extern void venc_sys_clk_enable(bool en);
extern void dsp0_sys_clk_enable(bool en);
extern void dsp1_sys_clk_enable(bool en);
extern void lpddr4_phy_lp3_exit(int speed);
extern void lpddr4_phy_lp3_enter(int speed);
extern void c910_sys_clk_enable(bool en, int cpuid);
extern void c910_reset();
extern void c910_reset_release();
extern void audio_reset();
extern void audio_reset_release();
extern void audio_reset_all();
extern void audio_reset_release_0();
extern void audio_reset_release_1();
extern void vdec_reset();
extern void vdec_reset_release();
extern void npu_reset();
extern void npu_reset_release();
extern void gpu_reset();
extern void gpu_reset_release();
extern void dsp0_reset();
extern void dsp0_reset_release();
extern void dsp1_reset();
extern void dsp1_reset_release();
extern void venc_reset();
extern void venc_reset_release();
extern void apsys_reset();
extern void apsys_reset_release();
extern void sram_axi_reset();
extern void sram_axi_reset_release();
extern void pvtc_reset();
extern void pvtc_reset_release();
extern void sram_axi_reset();

extern void npu_sys_clk_enable(bool en);

extern void audio_powerdown();
extern void audio_powerup();
extern void vdec_powerdown();
extern void vdec_powerup();
extern void npu_powerdown();
extern void npu_powerup();
extern void venc_powerdown();
extern void venc_powerup();
extern void gpu_powerdown();
extern void gpu_powerup();
extern void dsp0_powerdown();
extern void dsp0_powerup();
extern void dsp1_powerdown();
extern void dsp1_powerup();

static struct light_iopmp_list {
	int iopmp_type;
	uint32_t reg;
}light_iopmp_lists[] = {
	{IOPMP_VENC, 0xFCC600C0},
	{IOPMP_VDEC, 0xFCC610C0},
	{IOPMP_NPU,  0xFF01C0C0},
	{IOPMP_GPU,  0xFF5220C0},
	{IOPMP_DSP0, 0xFF0580C0},
	{IOPMP_DSP1, 0xFF0590C0},
	{IOPMP_AUDIO, 0xFFC220C0},
	{IOPMP_AUDIO0, 0xCB02E0C0},
	{IOPMP_AUDIO1, 0xCB02F0C0},
};

pd_match_list_t pd_list [] = {
    {"VDEC", SUBSYS_VDEC},
    {"VENC", SUBSYS_VENC},
    {"NPU", SUBSYS_NPU},
    {"GPU", SUBSYS_GPU},
    {"DSP0", SUBSYS_DSP0},
    {"DSP1", SUBSYS_DSP1},
};

clk_match_list_t clk_list [] = {
    {"SDIO0", MISCSYS_SDIO0},
    {"SDIO1", MISCSYS_SDIO1},
    {"I2C0", APSYS_I2C0},
    {"I2C1", APSYS_I2C1},
    {"I2C2", APSYS_I2C2},
    {"I2C3", APSYS_I2C3},
    {"I2C4", APSYS_I2C4},
    {"PWM", APSYS_PWM},
    {"G2D", VPSYS_G2D},
    {"FCE", VPSYS_FCE},
    {"ISP0", VISYS_ISP0},
    {"ISP1", VISYS_ISP1},
    {"ISP_RY", VISYS_ISP_RY},
};

const regu_voltage_param_t apsys_soc_ext_id[] = {
	APSYS_SOC_EXT_ID_DEF(SOC_DVDD18_EMMC),
	APSYS_SOC_EXT_ID_DEF(SOC_DVDD33_EMMC),
	APSYS_SOC_EXT_ID_DEF(SOC_AVDD18_MIPI_HDMI),
	APSYS_SOC_EXT_ID_DEF(SOC_AVDD08_MIPI_HDMI),
	APSYS_SOC_EXT_ID_DEF(SOC_DVDD08_AP),
	APSYS_SOC_EXT_ID_DEF(SOC_DVDD18_AP),
	APSYS_SOC_EXT_ID_DEF(SOC_VDD_DDR_0V6),
	APSYS_SOC_EXT_ID_DEF(SOC_APCPU_DVDD_DVDDM),
};

uint32_t g_chip_wakeup_flag = 0;

void light_iopmp_config(int iopmp_id)
{
	LOG_D("iopmp value = 0x%x, cfg reg: 0x%x\n", LIGHT_IOPMP_DEFAULT_ATTR, light_iopmp_lists[iopmp_id].reg);
	CHIP_REG_SET(LIGHT_IOPMP_DEFAULT_ATTR, light_iopmp_lists[iopmp_id].reg);
}

uint32_t light_iopmp_get(int iopmp_id)
{
	LOG_D("iopmp_id = %d, cfg reg: 0x%x\n", iopmp_id, light_iopmp_lists[iopmp_id].reg);
	return CHIP_REG_GET(light_iopmp_lists[iopmp_id].reg);
}

static bool cpu_is_inactive(int cpu)
{
	return (CHIP_REG_GET(REG_AON_CPU_LP_MODE) >> CPU_LP_MODE_OFF(cpu)) &
		CPU_LP_MODE_MSK ? false : true;
}

static inline int chip_lp_mode_get_iopmp_flag()
{
	return ((CHIP_REG_GET(REG_AON_CHIP_LP_MODE) & CHIP_LP_MODE_IOPMP_FLAG_MASK) >> CHIP_LP_MODE_IOPMP_FLAG_OFFSET);
}

static inline int chip_lp_mode_set_iopmp_flag(int enable)
{
	uint32_t data = CHIP_REG_GET(REG_AON_CHIP_LP_MODE);
	if (enable)
	{
		data |= CHIP_LP_MODE_IOPMP_FLAG_MASK;
	}
	else
	{
		data &= ~CHIP_LP_MODE_IOPMP_FLAG_MASK;
	}
	return CHIP_REG_SET(data, REG_AON_CHIP_LP_MODE);
}

static inline int chip_lp_mode_get_ddr_flag()
{
	return ((CHIP_REG_GET(REG_AON_CHIP_LP_MODE) & CHIP_LP_MODE_DDR_FLAG_MASK) >> CHIP_LP_MODE_DDR_FLAG_OFFSET);
}

static inline int chip_lp_mode_set_ddr_flag(int enable)
{
	uint32_t data = CHIP_REG_GET(REG_AON_CHIP_LP_MODE);
	if (enable)
	{
		data |= CHIP_LP_MODE_DDR_FLAG_MASK;
	}
	else
	{
		data &= ~CHIP_LP_MODE_DDR_FLAG_MASK;
	}
	return CHIP_REG_SET(data, REG_AON_CHIP_LP_MODE);
}

uint32_t chip_lp_mode_get_lp_mode()
{
	return ((CHIP_REG_GET(REG_AON_CHIP_LP_MODE) & CHIP_LP_MODE_MASK) >> CHIP_LP_MODE_OFFSET);
}

int chip_lp_mode_set_lp_mode(SYS_LP_MODE mode)
{
	uint32_t data = CHIP_REG_GET(REG_AON_CHIP_LP_MODE) & CHIP_LP_MODE_MASK;
	data &= ~CHIP_LP_MODE_MASK;
	switch(mode) {
		case SYS_LP_NONE:
			data |= CHIP_LP_NONE << CHIP_LP_MODE_OFFSET;
			break;
		case SYS_LP_STANDBY:
			data |= CHIP_LP_STANDBY << CHIP_LP_MODE_OFFSET;
			break;
		case SYS_LP_HW_VAD:
			data |= CHIP_LP_HW_VAD << CHIP_LP_MODE_OFFSET;
			break;
		default:
			LOG_I("invalid chip lp mode\n");
	}
	return CHIP_REG_SET(data, REG_AON_CHIP_LP_MODE);
}

void subsys_pd_cnt_enable(LIGHT_SUBSYS_NAME subsys, bool ena)
{
	uint32_t data = CHIP_REG_GET(REG_AON_PD_SW_CNT_EN);

	if (ena)
		data |= SYS_PD_SW_EN_MSK << SYS_PD_SW_EN_OFF(subsys);	// 2bit
	else
		data &= ~(SYS_PD_SW_EN_MSK << SYS_PD_SW_EN_OFF(subsys));

	CHIP_REG_SET(data, REG_AON_PD_SW_CNT_EN);
}

#ifdef CONFIG_HW_PD_MODE
static void subsys_powerdown_req(LIGHT_SUBSYS_NAME subsys)
{
	CHIP_REG_SET((CHIP_REG_GET(REG_AON_PD_REQ) | SYS_PD_REQ_OFF(subsys)), REG_AON_PD_REQ);		//1 bit
}
#endif

#ifdef CONFIG_HW_PD_MODE
static void subsys_powerup_req(LIGHT_SUBSYS_NAME subsys)
{
	CHIP_REG_SET((CHIP_REG_GET(REG_AON_PD_REQ) | SYS_PU_REQ_OFF(subsys)), REG_AON_PD_REQ);	//1 bit
}
#endif

static void subsys_powerdown_iso_en_set(LIGHT_SUBSYS_NAME subsys)
{
	CHIP_REG_SET((CHIP_REG_GET(REG_AON_PD_ISO_EN_SET) | SYS_PD_ISO_EN_SET_OFF(subsys)), REG_AON_PD_ISO_EN_SET);

	uint32_t data = CHIP_REG_GET(REG_AON_PD_ISO_EN_SET);

	LOG_D("reg = 0x%x, data = 0x%x\n",   REG_AON_PD_ISO_EN_SET, data);
	(void)data;
}

static void subsys_powerdown_iso_en_clr(LIGHT_SUBSYS_NAME subsys)
{
	CHIP_REG_SET((CHIP_REG_GET(REG_AON_PD_ISO_EN_CLR) | SYS_PD_ISO_EN_CLR_OFF(subsys)), REG_AON_PD_ISO_EN_CLR);

	uint32_t data = CHIP_REG_GET(REG_AON_PD_ISO_EN_CLR);

	LOG_D("reg = 0x%x, data = 0x%x\n",   REG_AON_PD_ISO_EN_CLR, data);
	(void)data;
}

static void subsys_powerdown_sw_en_set(LIGHT_SUBSYS_NAME subsys)
{
	uint32_t data = CHIP_REG_GET(REG_AON_PD_SW_EN_SET);

	data |= SYS_PD_SW_EN_MSK << SYS_PD_SW_EN_OFF(subsys);	// 2bit

	LOG_D("reg = 0x%x, data = 0x%x\n",   REG_AON_PD_SW_EN_SET, data);

	CHIP_REG_SET(data, REG_AON_PD_SW_EN_SET);
	(void)data;
}

static void subsys_powerdown_sw_en_clr(LIGHT_SUBSYS_NAME subsys)
{
	uint32_t data = CHIP_REG_GET(REG_AON_PD_SW_EN_CLR);

	data |= SYS_PD_SW_EN_MSK << SYS_PD_SW_EN_OFF(subsys);	// 2bit

	LOG_D("reg = 0x%x, data = 0x%x\n",   REG_AON_PD_SW_EN_CLR, data);

	CHIP_REG_SET(data, REG_AON_PD_SW_EN_CLR);
}

static int subsys_pd_verify(LIGHT_SUBSYS_NAME subsys, POWER_STATE state)
{
	int try_cnt = MAX_TRY_TIME_IDLE;
	int done_state;

	if (state == POWER_ON)
		done_state = POWER_ON_DONE;
	else if (state == POWER_DOWN)
		done_state = POWER_DOWN_DONE;
	else {
		LOG_I("invalid target power state\n");
		return CSI_ERROR;
	}

	while (try_cnt--) {
		if (((CHIP_REG_GET(REG_AON_PD_SW_ACK) >> SYS_PD_SW_EN_OFF(subsys)) & PD_SW_ACK_MSK) == done_state) {
			LOG_D("reg = 0x%x], data = 0x%x\n",   REG_AON_PD_SW_ACK, CHIP_REG_GET(REG_AON_PD_SW_ACK));
			return CSI_OK;
		}
	}

	if (try_cnt <= 0) {
		LOG_I("failed to change power state\n");
		return CSI_ERROR;
	}

	return CSI_OK;
}

void pmu_lowpower_mode_set(uint32_t mode)
{
	uint32_t data = CHIP_REG_GET(AON_PMU_PMU_CFG);
	data &= ~AON_PMU_LP_MODE_SET(AON_PMU_LP_MODE_MSK);
	data |= AON_PMU_LP_MODE_SET(mode) | AON_PMU_LP_REQ;
	CHIP_REG_SET(data, AON_PMU_PMU_CFG);
}

static inline void c910_dbg_mask(uint32_t cpu)
{
	uint32_t data = CHIP_REG_GET(REG_AON_C910_DBG_MASK);

	switch (cpu) {
	case 0:
		data |= AON_C910_CORE0_DBG_MASK;
		break;
	case 1:
		data |= AON_C910_CORE1_DBG_MASK;
		break;
	case 2:
		data |= AON_C910_CORE2_DBG_MASK;
		break;
	case 3:
		data |= AON_C910_CORE3_DBG_MASK;
		break;
	default:
		LOG_I("invalid c910 cpu id\n");
		return;
	}

	CHIP_REG_SET(data, REG_AON_C910_DBG_MASK);
}

static inline void c910_dbg_unmask(uint32_t cpu)
{
	uint32_t data = CHIP_REG_GET(REG_AON_C910_DBG_MASK);

	switch (cpu) {
		case 0:
			data &= ~AON_C910_CORE0_DBG_MASK;
			break;
		case 1:
			data &= ~AON_C910_CORE1_DBG_MASK;
			break;
		case 2:
			data &= ~AON_C910_CORE2_DBG_MASK;
			break;
		case 3:
			data &= ~AON_C910_CORE3_DBG_MASK;
			break;
		default:
			LOG_I("invalid c910 cpu id\n");
			return;
	}
	CHIP_REG_SET(data, REG_AON_C910_DBG_MASK);
}

#if 0
static void ddr_power_up()
{
	uint32_t data = CHIP_REG_GET(REG_AON_ISO_CFG);

	data &= ~AON_DDR_ISOLATION_EN;
	CHIP_REG_SET(data, REG_AON_ISO_CFG);

	data = CHIP_REG_GET(REG_AON_DDRSYS_RST_CFG);
	data |= SW_AON_DDRSYS_RST_n;
	CHIP_REG_SET(data, REG_AON_DDRSYS_RST_CFG);
}
#endif

static inline void c910_l2cache_flush()
{
	uint32_t data = CHIP_REG_GET(REG_AON_C910_L2CACHE);

	data |= AON_C910_CPU_L2CACHE_FLUSH_REQ;
	CHIP_REG_SET(data, REG_AON_C910_L2CACHE);

	data = CHIP_REG_GET(REG_AON_C910_L2CACHE);
	while (!(data & AON_C910_CPU_L2CACHE_FLUSH_DONE))
		data = CHIP_REG_GET(REG_AON_C910_L2CACHE);

	LOG_D("flush c910 l2cache done!\n");

	data &= ~AON_C910_CPU_L2CACHE_FLUSH_REQ;
	CHIP_REG_SET(data, REG_AON_C910_L2CACHE);

	data = CHIP_REG_GET(REG_AON_C910_L2CACHE);
	while (!(data & AON_C910_CPU_NO_OP))
		data = CHIP_REG_GET(REG_AON_C910_L2CACHE);

	LOG_D("Get c910 no_op signal\n");
}

static void c910_mem_rentention_enter(uint32_t cpu, MEM_RET_MODE mode)
{
	uint32_t data = CHIP_REG_GET(REG_AON_MEM_LP_MODE);

	if (mode != MEM_RET_SLP && mode != MEM_RET_DSLP && mode != MEM_RET_SD) {
		LOG_I("invalid mem retention mode\n");
		return;
	}

	switch (cpu) {
	case 0:
		if (mode == MEM_RET_DSLP)
			data |= AON_C910_CORE0_MEM_DSLP;
		else if (mode == MEM_RET_SLP)
			data |= AON_C910_CORE0_MEM_SLP;
		else /* mode == MEM_RET_SD */
			data |= AON_C910_CORE0_MEM_SD;
		break;
	case 1:
		if (mode == MEM_RET_DSLP)
			data |= AON_C910_CORE1_MEM_DSLP;
		else if (mode == MEM_RET_SLP)
			data |= AON_C910_CORE1_MEM_SLP;
		else
			data |= AON_C910_CORE1_MEM_SD;
		break;
	case 2:
		if (mode == MEM_RET_DSLP)
			data |= AON_C910_CORE2_MEM_DSLP;
		else if (mode == MEM_RET_SLP)
			data |= AON_C910_CORE2_MEM_SLP;
		else
			data |= AON_C910_CORE2_MEM_SD;

		break;
	case 3:
		if (mode == MEM_RET_DSLP)
			data |= AON_C910_CORE3_MEM_DSLP;
		else if (mode == MEM_RET_SLP)
			data |= AON_C910_CORE3_MEM_SLP;
		else
			data |= AON_C910_CORE3_MEM_SD;
		break;
	default:
		LOG_I("invalid c910 cpu id\n");
		return;
	}

	CHIP_REG_SET(data, REG_AON_MEM_LP_MODE);
}

static void c910_mem_rentention_exit(uint32_t cpu, MEM_RET_MODE mode)
{
	uint32_t data = CHIP_REG_GET(REG_AON_MEM_LP_MODE);

	if (mode != MEM_RET_SLP && mode != MEM_RET_DSLP && mode != MEM_RET_SD) {
		LOG_I("invalid cpu mem retention mode\n");
		return;
	}

	switch (cpu) {
	case 0:
		if (mode == MEM_RET_DSLP)
			data &= ~AON_C910_CORE0_MEM_DSLP;
		else if (mode == MEM_RET_SLP)
			data &= ~AON_C910_CORE0_MEM_SLP;
		else /* mode == MEM_RET_SD */
			data &= ~AON_C910_CORE0_MEM_SD;
		break;
	case 1:
		if (mode == MEM_RET_DSLP)
			data &= ~AON_C910_CORE1_MEM_DSLP;
		else if (mode == MEM_RET_SLP)
			data &= ~AON_C910_CORE1_MEM_SLP;
		else
			data &= ~AON_C910_CORE1_MEM_SD;
		break;
	case 2:
		if (mode == MEM_RET_DSLP)
			data &= ~AON_C910_CORE2_MEM_DSLP;
		else if (mode == MEM_RET_SLP)
			data &= ~AON_C910_CORE2_MEM_SLP;
		else
			data &= ~AON_C910_CORE2_MEM_SD;

		break;
	case 3:
		if (mode == MEM_RET_DSLP)
			data &= ~AON_C910_CORE3_MEM_DSLP;
		else if (mode == MEM_RET_SLP)
			data &= ~AON_C910_CORE3_MEM_SLP;
		else
			data &= ~AON_C910_CORE3_MEM_SD;
		break;
	default:
		LOG_I("invalid c910 cpu id\n");
		return;
	}

	CHIP_REG_SET(data, REG_AON_MEM_LP_MODE);
}

static void aon_mem_retention_enter(MEM_RET_MODE mode)
{
	uint32_t data = CHIP_REG_GET(REG_AON_MEM_LP_MODE);

	switch (mode) {
	case MEM_RET_SLP:
		data |= AON_SRAM_AXI_AXI_SRAM_SLP;
		break;
	case MEM_RET_DSLP:
		data |= AON_SRAM_AXI_AXI_SRAM_DSLP;
		break;
	case MEM_RET_SD:
		data |= AON_SRAM_AXI_AXI_SRAM_SD;
		break;
	default:
		LOG_I("invalid ap sram retention mode\n");
		return;
	}
	CHIP_REG_SET(data, REG_AON_MEM_LP_MODE);
}

static void aon_mem_retention_exit(MEM_RET_MODE mode)
{
	uint32_t data = CHIP_REG_GET(REG_AON_MEM_LP_MODE);

	switch (mode) {
	case MEM_RET_SLP:
		data &= ~AON_SRAM_AXI_AXI_SRAM_SLP;
		break;
	case MEM_RET_DSLP:
		data &= ~AON_SRAM_AXI_AXI_SRAM_DSLP;
		break;
	case MEM_RET_SD:
		data &= ~AON_SRAM_AXI_AXI_SRAM_SD;
		break;
	default:
		LOG_I("invalid ap sram retention mode\n");
		return;
	}
	CHIP_REG_SET(data, REG_AON_MEM_LP_MODE);
}

static void cpu_top_mem_retention_enter(MEM_RET_MODE mode)
{
	uint32_t data = CHIP_REG_GET(REG_AON_MEM_LP_MODE);
	switch (mode) {
	case MEM_RET_SLP:
		data |= AON_C910_CPU_MEM_SLP;
		break;
	case MEM_RET_DSLP:
		data |= AON_C910_CPU_MEM_DSLP;
		break;
	case MEM_RET_SD:
		data |= AON_C910_CPU_MEM_SD;
		break;
	default:
		LOG_I("invalid cpu top retention mode\n");
		return;
	}
	CHIP_REG_SET(data, REG_AON_MEM_LP_MODE);
}

static void cpu_top_mem_retention_exit(MEM_RET_MODE mode)
{
	uint32_t data = CHIP_REG_GET(REG_AON_MEM_LP_MODE);
	switch (mode) {
	case MEM_RET_SLP:
		data &= ~AON_C910_CPU_MEM_SLP;
		break;
	case MEM_RET_DSLP:
		data &= ~AON_C910_CPU_MEM_DSLP;
		break;
	case MEM_RET_SD:
		data &= ~AON_C910_CPU_MEM_SD;
		break;
	default:
		LOG_I("invalid cpu top retention mode\n");
		return;
	}
	CHIP_REG_SET(data, REG_AON_MEM_LP_MODE);
}

bool ip_pd_is_active(LIGHT_SUBSYS_NAME pd_name)
{
     uint32_t data;
     data = (CHIP_REG_GET(REG_AON_PD_SW_ACK) >> SYS_PD_SW_EN_OFF(pd_name)) & PD_SW_ACK_MSK;

     return data & POWER_ON_DONE ? true : false;
}

bool ip_is_active(char *name)
{
    int i;
    csi_error_t ret = CSI_UNSUPPORTED;

    for(i=0; i< ARRAY_SIZE(clk_list); i++)
    {
	if(strcmp(name, clk_list[i].ip_name) == 0)
	{
		return ip_clk_is_active(clk_list[i].clk_name);
	}
    }

    for(i=0; i< ARRAY_SIZE(pd_list); i++)
    {
	if(strcmp(name, pd_list[i].ip_name) == 0)
	{
		return ip_pd_is_active(pd_list[i].pd_name);
	}
    }
    /* assume ip is active if it's not in the list */
    return true;
}

void c910_powerup(uint16_t cpuid)
{
	LIGHT_SUBSYS_NAME subsys;

		if (cpuid == C910_CORE0)
			subsys = SUBSYS_C910_CORE0;
		else if (cpuid == C910_CORE1)
			subsys = SUBSYS_C910_CORE1;
		else if (cpuid == C910_CORE2)
			subsys = SUBSYS_C910_CORE2;
		else if (cpuid == C910_CORE3)
			subsys = SUBSYS_C910_CORE3;
		else {
			LOG_D("invalid c910 cpu id\n");
			return;
		}
		LOG_D("bringup core %d\n", cpuid);
		c910_reset(cpuid);
		subsys_powerdown_sw_en_clr(subsys);
		subsys_pd_verify(subsys, POWER_ON);
	#ifdef CONFIG_HW_PD_MODE
		subsys_powerup_req(subsys);
	#endif
		mdelay(1);	//TBD
		subsys_powerdown_iso_en_clr(subsys);
		c910_sys_clk_enable(true, cpuid);
		/* leave this pull-up in opensbi */
	//	c910_reset_release(cpuid);
}

void c910_powerdown(uint16_t cpuid)
{
	LIGHT_SUBSYS_NAME subsys;

	while(!cpu_is_inactive(cpuid));

	if (cpuid == C910_CORE0)
		subsys = SUBSYS_C910_CORE0;
	else if (cpuid == C910_CORE1)
		subsys = SUBSYS_C910_CORE1;
	else if (cpuid == C910_CORE2)
		subsys = SUBSYS_C910_CORE2;
	else if (cpuid == C910_CORE3)
		subsys = SUBSYS_C910_CORE3;
	else {
		LOG_I("invalid c910 cpu id\n");
		return;
	}
	LOG_I("reset core %d\n", cpuid);
	c910_reset(cpuid);
	c910_sys_clk_enable(false, cpuid);
	subsys_powerdown_iso_en_set(subsys);
#ifdef CONFIG_HW_PD_MODE
	subsys_powerdown_req(subsys);
#else
	subsys_powerdown_sw_en_set(subsys);
	subsys_pd_verify(subsys, POWER_DOWN);
#endif
}

void audio_powerdown()
{
	int ret;
	audio_reset();
	audio_sys_clk_enable(false);
	subsys_powerdown_iso_en_set(SUBSYS_AUDIO);
#ifdef CONFIG_HW_PD_MODE
	subsys_powerdown_req(SUBSYS_AUDIO);
#else
	subsys_powerdown_sw_en_set(SUBSYS_AUDIO);
	ret = subsys_pd_verify(SUBSYS_AUDIO, POWER_DOWN);
	if (ret)
		LOG_I("failed to power down audio\n");
#endif
}

void audio_powerup()
{
	int ret;
	light_iopmp_config(IOPMP_AUDIO);
#ifdef CONFIG_HW_PD_MODE
	subsys_powerup_req(SUBSYS_AUDIO);
	mdelay(1);
	audio_reset_release();
#else
	audio_reset_all();
	subsys_powerdown_sw_en_clr(SUBSYS_AUDIO);
	ret = subsys_pd_verify(SUBSYS_AUDIO, POWER_ON);
	if (ret)
		LOG_I("failed to power on audio\n");
	udelay(10);
	subsys_powerdown_iso_en_clr(SUBSYS_AUDIO);
	audio_sys_clk_enable(true);
	audio_reset_release_0();
	udelay(10);
	light_iopmp_config(IOPMP_AUDIO0);
	light_iopmp_config(IOPMP_AUDIO1);
	audio_reset_release_1();
#endif
}

void vdec_powerdown()
{
	vdec_reset();
	vdec_sys_clk_enable(false);
	subsys_powerdown_iso_en_set(SUBSYS_VDEC);
#ifdef CONFIG_HW_PD_MODE
	subsys_powerdown_req(SUBSYS_VDEC);
#else
	subsys_powerdown_sw_en_set(SUBSYS_VDEC);
	subsys_pd_verify(SUBSYS_VDEC, POWER_DOWN);
#endif
}

void vdec_powerup()
{
	int ret;
#ifdef CONFIG_HW_PD_MODE
	subsys_powerup_req(SUBSYS_VDEC);
	mdelay(1);
	vdec_reset_release();
#else
	vdec_reset();
	subsys_powerdown_sw_en_clr(SUBSYS_VDEC);
	ret = subsys_pd_verify(SUBSYS_VDEC, POWER_ON);
	if (ret)
		LOG_I("failed to power on audio\n");
	udelay(10);
	subsys_powerdown_iso_en_clr(SUBSYS_VDEC);
	vdec_sys_clk_enable(true);
	vdec_reset_release();
#endif
}

void npu_powerdown()
{
	npu_reset();
	npu_sys_clk_enable(false);
	subsys_powerdown_iso_en_set(SUBSYS_NPU);
#ifdef CONFIG_HW_PD_MODE
	subsys_powerdown_req(SUBSYS_NPU);
#else
	subsys_powerdown_sw_en_set(SUBSYS_NPU);
	subsys_pd_verify(SUBSYS_NPU, POWER_DOWN);
#endif
}

void npu_powerup()
{
	int ret;
#ifdef CONFIG_HW_PD_MODE
	subsys_powerup_req(SUBSYS_NPU);
	mdelay(1);
	npu_reset_release();
#else
	npu_reset();
	subsys_powerdown_sw_en_clr(SUBSYS_NPU);
	ret = subsys_pd_verify(SUBSYS_NPU, POWER_ON);
	if (ret)
		LOG_I("failed to power on audio\n");
	udelay(10);
	subsys_powerdown_iso_en_clr(SUBSYS_NPU);
	npu_sys_clk_enable(true);
	npu_reset_release();
#endif
}

void venc_powerdown()
{
	venc_reset();
	venc_sys_clk_enable(false);
	subsys_powerdown_iso_en_set(SUBSYS_VENC);
#ifdef CONFIG_HW_PD_MODE
	subsys_powerdown_req(SUBSYS_VENC);
#else
	subsys_powerdown_sw_en_set(SUBSYS_VENC);
	subsys_pd_verify(SUBSYS_VENC, POWER_DOWN);
#endif
}

void venc_powerup()
{
	int ret;
#ifdef CONFIG_HW_PD_MODE

#else
	venc_reset();
	subsys_powerdown_sw_en_clr(SUBSYS_VENC);
	ret = subsys_pd_verify(SUBSYS_VENC, POWER_ON);
	if (ret)
		LOG_I("failed to power on audio\n");
	udelay(10);
	subsys_powerdown_iso_en_clr(SUBSYS_VENC);
	venc_sys_clk_enable(true);
	venc_reset_release();
#endif
}

void gpu_powerdown()
{
	subsys_powerdown_iso_en_set(SUBSYS_GPU);
#ifdef CONFIG_HW_PD_MODE
	subsys_powerdown_req(SUBSYS_GPU);
#else
	subsys_powerdown_sw_en_set(SUBSYS_GPU);
	subsys_pd_verify(SUBSYS_GPU, POWER_DOWN);
#endif
}

void gpu_powerup()
{
	int ret;
#ifdef CONFIG_HW_PD_MODE

#else
	subsys_powerdown_sw_en_clr(SUBSYS_GPU);
	ret = subsys_pd_verify(SUBSYS_GPU, POWER_ON);
	if (ret)
		LOG_I("failed to power on audio\n");
	udelay(10);
	subsys_powerdown_iso_en_clr(SUBSYS_GPU);
#endif
}

void dsp0_powerdown()
{
	dsp0_reset();
	dsp0_sys_clk_enable(false);
	subsys_powerdown_iso_en_set(SUBSYS_DSP0);
#ifdef CONFIG_HW_PD_MODE
	subsys_powerdown_req(SUBSYS_DSP0);
#else
	subsys_powerdown_sw_en_set(SUBSYS_DSP0);
	subsys_pd_verify(SUBSYS_DSP0, POWER_DOWN);
#endif
}

void dsp0_powerup()
{
	int ret;
#ifdef CONFIG_HW_PD_MODE

#else
	dsp0_reset();
	subsys_powerdown_sw_en_clr(SUBSYS_DSP0);
	ret = subsys_pd_verify(SUBSYS_DSP0, POWER_ON);
	if (ret)
		LOG_I("failed to power on audio\n");
	udelay(10);
	subsys_powerdown_iso_en_clr(SUBSYS_DSP0);
	dsp0_sys_clk_enable(true);
	dsp0_reset_release();
#endif
}

void dsp1_powerdown()
{
	dsp1_reset();
	dsp1_sys_clk_enable(false);
	subsys_powerdown_iso_en_set(SUBSYS_DSP1);
#ifdef CONFIG_HW_PD_MODE
	subsys_powerdown_req(SUBSYS_DSP1);
#else
	subsys_powerdown_sw_en_set(SUBSYS_DSP1);
	subsys_pd_verify(SUBSYS_DSP1, POWER_DOWN);
#endif
}

void dsp1_powerup()
{
	int ret;
#ifdef CONFIG_HW_PD_MODE

#else
	dsp1_reset();
	subsys_powerdown_sw_en_clr(SUBSYS_DSP1);
	ret = subsys_pd_verify(SUBSYS_DSP1, POWER_ON);
	if (ret)
		LOG_I("failed to power on audio\n");
	udelay(10);
	subsys_powerdown_iso_en_clr(SUBSYS_DSP1);
	dsp1_sys_clk_enable(true);
	dsp1_reset_release();
#endif
}

static inline void ap_power_down()
{
	uint32_t data = CHIP_REG_GET(REG_AON_APSYS_RST_CFG);

	data &= ~SW_AON_APSYS_RST_n;
	CHIP_REG_SET(data, REG_AON_APSYS_RST_CFG);

	data = CHIP_REG_GET(REG_AON_APSYS_CLK_GATE);
	data &= ~AON_APSYS_CLK_EN;
	CHIP_REG_SET(data, REG_AON_APSYS_CLK_GATE);
	LOG_I("ap reset&clk disable finished..\n");
}

csi_error_t apsys_power_ctrl(csi_pmic_t* pmic, bool ena)
{
	csi_error_t ret = CSI_OK;
	uint32_t soc_id_num = sizeof(apsys_soc_ext_id) / sizeof(apsys_soc_ext_id[0]);
	for (uint32_t soc_id = 0; soc_id < soc_id_num; soc_id++) {
		if (ena)
		{
			/* AP sys power enable/disable sequence should be reversed */
			ret = csi_pmic_regulator_ctrl(pmic, apsys_soc_ext_id[soc_id_num - soc_id - 1].regu_ext_id, 1);
			mdelay(2);
			LOG_I("enable %d, ret %d\n", soc_id, ret);
			if (ret)
				return ret;
		}
		else
		{
			ret = csi_pmic_regulator_ctrl(pmic, apsys_soc_ext_id[soc_id].regu_ext_id, 0);
			mdelay(2);
			LOG_I("disable %d, ret %d\n", soc_id, ret);
			if (ret)
				return ret;
		}
	}
	return CSI_OK;
}
static void ap_power_up()
{
	uint32_t data = CHIP_REG_GET(REG_AON_APSYS_CLK_GATE);

	data |= AON_APSYS_CLK_EN;
	CHIP_REG_SET(data, REG_AON_APSYS_CLK_GATE);

	data = CHIP_REG_GET(REG_AON_APSYS_RST_CFG);
	data |= SW_AON_APSYS_RST_n;
	CHIP_REG_SET(data, REG_AON_APSYS_RST_CFG);
}

static void power_isolator_en()
{
	uint32_t data = CHIP_REG_GET(REG_AON_ISO_CFG);

	data |= AON_AON_ISOLATION_EN | AON_USB3_ISOLATION_EN | AON_C910_ISOLATION_EN;
	CHIP_REG_SET(data, REG_AON_ISO_CFG);
}

#if 0
static void indicator_magic_num_set()
{
	//CHIP_REG_SET(0x0, REG_AON_STR_INDICATOR_0);
	CHIP_REG_SET(INDICATOR_0_MAGIC_NUM, REG_AON_STR_INDICATOR_0);
	CHIP_REG_SET(INDICATOR_1_MAGIC_NUM, REG_AON_STR_INDICATOR_1);
	CHIP_REG_SET(INDICATOR_2_MAGIC_NUM, REG_AON_STR_INDICATOR_2);
	CHIP_REG_SET(INDICATOR_3_MAGIC_NUM, REG_AON_STR_INDICATOR_3);

	/* set boot addr */
	CHIP_REG_SET(0, REG_AON_RESERVED_REG_0);
	CHIP_REG_SET(0, REG_AON_RESERVED_REG_1);
}
#endif

static bool indicator_magic_num_check()
{
	uint32_t magic_num0 = CHIP_REG_GET(REG_AON_STR_INDICATOR_0);
	uint32_t magic_num1 = CHIP_REG_GET(REG_AON_STR_INDICATOR_1);
	uint32_t magic_num2 = CHIP_REG_GET(REG_AON_STR_INDICATOR_2);
	uint32_t magic_num3 = CHIP_REG_GET(REG_AON_STR_INDICATOR_3);

	return ((magic_num0 == INDICATOR_0_MAGIC_NUM)&&(magic_num1 == INDICATOR_1_MAGIC_NUM)&&(magic_num2 == INDICATOR_2_MAGIC_NUM)&&(magic_num3 == INDICATOR_3_MAGIC_NUM)) ? true : false;
}

static void indicator_magic_num_clear()
{
	CHIP_REG_SET(0x0, REG_AON_STR_INDICATOR_0);
	CHIP_REG_SET(0x0, REG_AON_STR_INDICATOR_1);
	CHIP_REG_SET(0x0, REG_AON_STR_INDICATOR_2);
	CHIP_REG_SET(0x0, REG_AON_STR_INDICATOR_3);
}

static void power_isolator_release()
{
	uint32_t data = CHIP_REG_GET(REG_AON_ISO_CFG);

	data &= ~(AON_AON_ISOLATION_EN | AON_USB3_ISOLATION_EN | AON_C910_ISOLATION_EN);

	CHIP_REG_SET(data, REG_AON_ISO_CFG);
}

static void  audio_slp_clk_sel(bool ena)
{
	uint32_t data;

	if (ena) {
		/* close clkgen_audio_subsys_sys_pll clock */
		data = CHIP_REG_GET(REG_AON_AUDIO_C906_CCLK_CFG);
		data &= ~(AON_CLKGEN_C906_CCLK_DIV_0_DIV_EN | AON_CLKGEN_C906_CCLK_DIV_1_DIV_EN);
		CHIP_REG_SET(data, REG_AON_AUDIO_C906_CCLK_CFG);

		/* close clkgen_audio_subsys_aclk_ap2cp clock */
		data = CHIP_REG_GET(REG_AON_AUDIO_CLK_GATE);
		data &= ~AON_CLKGEN_AUDIO_SUBSYS_ACLK_AP2CP_EN;
		CHIP_REG_SET(data, REG_AON_AUDIO_CLK_GATE);

		/* close clkgen_audio_subsys_aclk_cp2sram clock */
		data = CHIP_REG_GET(REG_AON_AUDIO_SUBSYS_ACLK_CFG);
		data &= ~(AON_CLKGEN_AUDIO_ACLK_DIV_0_DIV_EN | AON_CLKGEN_AUDIO_ACLK_DIV_1_DIV_EN);
		CHIP_REG_SET(data, REG_AON_AUDIO_SUBSYS_ACLK_CFG);

		/* close clkgen_audio_subsys_aclk_cp2ap clock */
		data = CHIP_REG_GET(REG_AON_AUDIO_CLK_GATE);
		data &= ~AON_CLKGEN_AUDIO_SUBSYS_ACLK_CP2AP_EN;
		CHIP_REG_SET(data, REG_AON_AUDIO_CLK_GATE);
	}
	else {
		/* open clkgen_audio_subsys_sys_pll */
		data = CHIP_REG_GET(REG_AON_AONSYS_CLK_CFG);
		data |= AON_CLKGEN_C906_CCLK_DIV_1_DIV_EN;
		CHIP_REG_SET(data, REG_AON_AONSYS_CLK_CFG);

		/* open clkgen_audio_subsys_aclk_cp2sram */
		data = CHIP_REG_GET(REG_AON_AUDIO_SUBSYS_ACLK_CFG);
		data |= AON_CLKGEN_AUDIO_SUBSYS_ACK_DIV_1_DIV_EN;
		CHIP_REG_SET(data, REG_AON_AUDIO_SUBSYS_ACLK_CFG);

		/* switch clkgen_audio_subsys_sys_pll to audio_pll_foutvco */
		data = CHIP_REG_GET(REG_AON_AUDIO_C906_CCLK_CFG);
		data |= AON_CLKGEN_AUDIO_C906_CCLK_SWITCH_SEL;
		CHIP_REG_SET(data, REG_AON_AUDIO_C906_CCLK_CFG);

		/* switch clkgen_audio_subsys_aclk_cp2sram to audio_pll_foutvco */
		data = CHIP_REG_GET(REG_AON_AUDIO_SUBSYS_ACLK_CFG);
		data |= AON_CLKGEN_AUDIO_SUBSYS_ACLK_SWITCH_SEL;
		CHIP_REG_SET(data, REG_AON_AUDIO_SUBSYS_ACLK_CFG);

		/* switch share_sram_clk to audio_pll_foutvco */
		data = CHIP_REG_GET(REG_AON_SHARE_SRAM_CLK_CFG);
		data |= AON_CLKGEN_SHARE_SRAM_CLK_SWITCH_SEL;
		CHIP_REG_SET(data, REG_AON_SHARE_SRAM_CLK_CFG);
	}
}

static void aonsys_slp_clk_sel(bool ena)
{
	uint32_t data;

	if (ena) { /* sleep */
		/* D.a
		 * switch clk source to osc_24M */
		data = CHIP_REG_GET(REG_AON_AONSYS_CLK_CFG);
		data |= AON_AONSYS_CLK_SWITCH_0_SWITCH_SEL;
		CHIP_REG_SET(data, REG_AON_AONSYS_CLK_CFG);
		udelay(1);

		/* D.e
		 * set divide by 8 */
		data = CHIP_REG_GET(REG_AON_AONSYS_CLK_CFG);
		data &= ~(AON_AONSYS_CLK_CDE_RATIO_MSK << AON_AONSYS_CLK_CDE_RATIO_OFF);
		data |= (7 & AON_AONSYS_CLK_CDE_RATIO_MSK) << AON_AONSYS_CLK_CDE_RATIO_OFF;
		data &= ~AON_AONSYS_CLK_CDE_SYNC;
		CHIP_REG_SET(data, REG_AON_AONSYS_CLK_CFG);
		udelay(1);
		data = CHIP_REG_GET(REG_AON_AONSYS_CLK_CFG);
		data |= AON_AONSYS_CLK_CDE_SYNC;
		CHIP_REG_SET(data, REG_AON_AONSYS_CLK_CFG);

		/* D.d
		 * power off RC_OUT */
		data = CHIP_REG_GET(REG_AON_RC_EN);
		data &= ~AON_RC_EN;
		CHIP_REG_SET(data, REG_AON_RC_EN);
		LOG_D("reg = 0x%x, data = 0x%x\n",   REG_AON_RC_EN, data);
	}
	else { /* wakeup */
		/* E.c
		 * switch AONSYS clock to audio_pll */
		 /* keep osc as clk source for stability */
		/*
		data = CHIP_REG_GET(REG_AON_AONSYS_CLK_CFG);
		data &= ~AON_AONSYS_CLK_SWITCH_1_SWITCH_SEL;
		udelay(1);
		*/
		data = CHIP_REG_GET(REG_AON_AONSYS_CLK_CFG);
		data &= ~AON_AONSYS_CLK_SWITCH_0_SWITCH_SEL;
		CHIP_REG_SET(data, REG_AON_AONSYS_CLK_CFG);
		udelay(1);

		/* E.d
		 * power on RC_OUT */
		data = CHIP_REG_GET(REG_AON_RC_EN);
		data |= AON_RC_EN;
		CHIP_REG_SET(data, REG_AON_RC_EN);

		data = CHIP_REG_GET(REG_AON_RC_READY);
		while (!(data & AON_RC_READY))
			CHIP_REG_GET(REG_AON_RC_READY);

		/* E.e
		 * set divider by 1 */
		data = CHIP_REG_GET(REG_AON_AONSYS_CLK_CFG);
		data &= ~(AON_AONSYS_CLK_CDE_RATIO_MSK << AON_AONSYS_CLK_CDE_RATIO_OFF);
		data &= ~AON_AONSYS_CLK_CDE_SYNC;
		CHIP_REG_SET(data, REG_AON_AONSYS_CLK_CFG);
		udelay(1);
		data |= AON_AONSYS_CLK_CDE_SYNC;
		CHIP_REG_SET(data, REG_AON_AONSYS_CLK_CFG);
	}
}

static void power_sys_pll(POWER_STATE state)
{
	uint32_t data = CHIP_REG_GET(REG_AON_SYS_PLL_CFG1);

	if (state == POWER_DOWN) {
		data |= AON_SYS_PLL_RST;
		CHIP_REG_SET(data, REG_AON_SYS_PLL_CFG1);
		LOG_D("reg = 0x%x, data = 0x%x\n",    REG_AON_SYS_PLL_CFG1, data);
	}
	else {
		data &= ~AON_SYS_PLL_RST;
		CHIP_REG_SET(data, REG_AON_SYS_PLL_CFG1);

		data = CHIP_REG_GET(REG_AON_AONSYS_PLL_STS);
		while (!(data & AON_CLKGEN_SYS_PLL_LOCK))
			data = CHIP_REG_GET(REG_AON_AONSYS_PLL_STS);
	}
}

static void power_audio_pll(POWER_STATE state)
{
	uint32_t data = CHIP_REG_GET(REG_AON_AUDIO_PLL_CFG1);

	if (state == POWER_DOWN) {
		data |= AON_AUDIO_PLL_RST;
		CHIP_REG_SET(data, REG_AON_AUDIO_PLL_CFG1);
		LOG_D("reg = 0x%x, data = 0x%x\n",   REG_AON_AUDIO_PLL_CFG1, data);
	}
	else {
		data &= ~AON_AUDIO_PLL_RST;
		CHIP_REG_SET(data, REG_AON_AUDIO_PLL_CFG1);

		data = CHIP_REG_GET(REG_AON_AONSYS_PLL_STS);
		while (!(data & AON_CLKGEN_AUDIO_PLL_LOCK))
			data = CHIP_REG_GET(REG_AON_AONSYS_PLL_STS);
	}
}

void power_osc_clk(POWER_STATE state)
{
	uint32_t data = CHIP_REG_GET(REG_AONSYS_PAD_CFG_001);

	if (state == POWER_DOWN)
		data &= ~PAD_AONSYS_OSC_CLK_IO_EN;
	else
		data |= PAD_AONSYS_OSC_CLK_IO_EN;

	CHIP_REG_SET(data, REG_AONSYS_PAD_CFG_001);
}

static void aonsys_periph_clk_disable(bool ena)
{
	uint32_t data = CHIP_REG_GET(REG_AON_AONSYS_CLK_GATE);

	if (ena)
		data &= ~(AON_CLKGEN_AOUART_PCLK_EN |
			AON_CLKGEN_AOUART_SCLK_EN |
			AON_CLKGEN_AOI2C_PCLK_EN |
			AON_CLKGEN_AOI2C_IC_CLK_EN |
			AON_CLKGEN_AUDGPIO_DBCLK_EN |
			AON_CLKGEN_AUDGPIO_PCLK_EN |
			AON_CLKGEN_ADC_PCLK_EN |
			AON_CLKGEN_DSPSYS_ACLK_S_EN |
			AON_CLKGEN_AOWDT_PCLK_EN |
			AON_CLKGEN_AOTIMER_CCLK_EN |
			AON_CLKGEN_AOTIMER_PCLK_EN |
			AON_CLKGEN_AOPAD_PCLK_EN |
			AON_CLKGEN_PVTC_PCLK_EN |
			AON_CLKGEN_SRAM_AXI_ACLK_4_EN |
			AON_CLKGEN_SRAM_AXI_CORE_CLK_EN
			);
	else
		data |= AON_CLKGEN_AOUART_PCLK_EN |
		AON_CLKGEN_AOUART_SCLK_EN |
		AON_CLKGEN_AOI2C_PCLK_EN |
		AON_CLKGEN_AOI2C_IC_CLK_EN |
		AON_CLKGEN_AUDGPIO_DBCLK_EN |
		AON_CLKGEN_AUDGPIO_PCLK_EN |
		AON_CLKGEN_ADC_PCLK_EN |
		AON_CLKGEN_DSPSYS_ACLK_S_EN |
		AON_CLKGEN_AOWDT_PCLK_EN |
		AON_CLKGEN_AOTIMER_CCLK_EN |
		AON_CLKGEN_AOTIMER_PCLK_EN |
		AON_CLKGEN_AOPAD_PCLK_EN |
		AON_CLKGEN_PVTC_PCLK_EN |
		AON_CLKGEN_SRAM_AXI_ACLK_4_EN |
		AON_CLKGEN_SRAM_AXI_CORE_CLK_EN
		;
	CHIP_REG_SET(data, REG_AON_AONSYS_CLK_GATE);
	LOG_D("reg = 0x%x, data = 0x%x\n",   REG_AON_AONSYS_CLK_GATE, data);
}

void system_clk_switch(PLL_SLEEP_MODE pll_mode, SYS_LP_MODE sys_lp_mode)
{
	if (pll_mode != PLL_SLEEP && pll_mode != PLL_NORMAL) {
		LOG_I("invalid system clock mode\n");
		return;
	}

	if (sys_lp_mode != SYS_LP_STANDBY && sys_lp_mode != SYS_LP_HW_VAD) {
		LOG_I("invalid system low power mode\n");
		return;
	}

	switch (pll_mode) {
	case PLL_SLEEP:
		/* A.
		 * Disable UART, I2C, ADC, WDT, TIMER, PAD, PVTC, SHARE SRAM clock source*/
		aonsys_periph_clk_disable(true);
		/* D. switch wake up clock source in plan 2-2
		 * */
		aonsys_slp_clk_sel(true);
		/* D. b,c
		 * stop audio_pll, sys_pll
		 * */
		power_sys_pll(POWER_DOWN);

		if (sys_lp_mode == SYS_LP_HW_VAD)
			audio_slp_clk_sel(true);
		else {
			power_audio_pll(POWER_DOWN);
			/* keep osc as aon clock source for stability */
		//	power_osc_clk(POWER_DOWN);
		}

		break;
	case PLL_NORMAL:
		if (sys_lp_mode == SYS_LP_HW_VAD)
			audio_slp_clk_sel(false);
		else {
			/* keep osc as aon clock source for stability */
		//	power_osc_clk(POWER_ON);
		/* D. a,b in plan 2-2
		 * Enable audio_pll, sys_pll */
			power_audio_pll(POWER_ON);
		}

		power_sys_pll(POWER_ON);
		udelay(22);	/* delay according to asic suggestion */
		/* D. c.d.e in plan 2-2
		 * switch clk source to audio_pll*/
		aonsys_slp_clk_sel(false);
		/* E.
		 * Enable UART, I2C, ADC, WDT, TIMER, PAD, PVTC, SHARE SRAM clock source*/
		aonsys_periph_clk_disable(false);
		break;
	}
}

static bool vic_enabled_irq_detect(int32_t IRQn)
{
	return CLIC->CLICINT[IRQn].IE == CLIC_INTIE_IE_Msk ? true : false;
}

static void aonsys_wakeup_source_set(uint32_t wakeup_flag)
{
	int32_t irq, j;

	//CHIP_REG_SET(~(PMU_AOGPIO_4_INT_MASK | PMU_AOGPIO_5_INT_MASK), AON_PMU_GPIO_INT_MASK_CFG);
	//CHIP_REG_SET(~0xFFCF, AON_PMU_GPIO_INT_POLARITY_CFG);
	//	CHIP_REG_SET(PMU_GPIO_INT_EN_OFF | PMU_RTC_INT_EN_OFF, AON_PMU_INTC_INT_MASK_CFG_L);
	//CHIP_REG_SET((1 << 14), AON_PMU_INTC_INT_MASK_CFG_L);
	//CHIP_REG_SET(0, AON_PMU_INTC_INT_MASK_CFG_H);

	for (irq = AON_IRQ_NUM_STAR + 16, j = 0; irq < MAX_AON_IRQ_NUM + 16; irq++) { //External CLIC Interrupt Num: 16 ~ 255
		if (vic_enabled_irq_detect(irq)) {
			csi_irq_disable(irq);
			LOG_I("Disabled irq: %d\n", irq);
			enabled_irq_num[j++] = irq;	/* store the enabled irq before suspend */
		}
	}
    if(wakeup_flag & AON_WAKEUP_BY_RTC) {
        csi_irq_enable(WJ_RTC_IRQn);
	}
	if(wakeup_flag & AON_WAKEUP_BY_GPIO) {
        csi_irq_enable(DW_GPIO_IRQn);
	}
}

static void aonsys_wakeup_source_clr()
{
	//uint32_t data = CHIP_REG_GET(AON_PMU_PMU_INT_CLR_CFG);
	int32_t irq, j;

	/* clear pmu interrupt */
	//data |= PMU_INT_CLR;
	//CHIP_REG_SET(data, AON_PMU_PMU_INT_CLR_CFG);

	//CHIP_REG_SET(0, AON_PMU_GPIO_INT_MASK_CFG);
	//CHIP_REG_SET(0, AON_PMU_INTC_INT_MASK_CFG_L);
	//CHIP_REG_SET(0, AON_PMU_INTC_INT_MASK_CFG_H);
	csi_irq_disable(WJ_CPR_IRQn);
	csi_irq_disable(WJ_RTC_IRQn);
	csi_irq_disable(DW_GPIO_IRQn);

	for (j = 0; j < 32; j++) {
		irq = enabled_irq_num[j];
		if (irq) {
			csi_irq_enable(irq);
			LOG_I("Enabled irq: %d\n", irq);
		}
		else
			break;
	}
	csi_irq_disable(DW_TIMER0_IRQn);

}

void subsys_power_down_init()
{
	/* core0 ~ core3/AUDO/VDEC/NPU/VENC/GPU/DSP0/DSP1 */
}

void subsys_power_up_init()
{
	/* core0 ~ core3/AUDO/VDEC/NPU/VENC/GPU/DSP0/DSP1 */
}

void board_device_resume()
{
	board_resume();
}

void chip_lpm_resume(csi_pmic_t* pmic, uint32_t chip_lp_mode)
{
	int wake_source = CHIP_REG_GET(AON_PMU_GPIO_INT_STS);
	if (chip_lp_mode == CHIP_LP_STANDBY) {

		//7.a, 7b, 7.c
		aonsys_wakeup_source_clr();

		// powerdown secondary core
 		for(int cpu = 1; cpu < MAX_C910_CORE_NUM; cpu++)
 			c910_powerup(cpu);
 		//7.d , 7.e
 		system_clk_switch(PLL_NORMAL, SYS_LP_STANDBY);

 		// 8.a
 		aon_mem_retention_exit(MEM_RET_DSLP);

		// 8.b
		apsys_power_ctrl(pmic, true);
		mdelay(2);

		//TODO: 8.c
		//nothing to do

		//8.d
		power_isolator_release();

		//8.e
		cpu_top_mem_retention_exit(MEM_RET_SD);
		for (int cpu = 0; cpu < MAX_C910_CORE_NUM; cpu++)
			c910_mem_rentention_exit(cpu, MEM_RET_SD);

		//8.f ddr isolator release, ddr rst release
		CHIP_REG_SET(0x0, REG_AON_ISO_CFG);
		CHIP_REG_SET(0x1, REG_AON_DDRSYS_RST_CFG);
		CHIP_REG_SET(0x1, REG_AON_DDRSYS_RST_CFG);
		CHIP_REG_SET(0x1, REG_AON_DDRSYS_RST_CFG);

		//8.h
		pvtc_reset_release();
		sram_axi_reset_release();
		//8.g, move ap rst release after pvct reset & sram axi reset
		ap_power_up();

		mdelay(2); // add new delay 2ms

		/* Mask the Debug Control Bit */
		for (int cpu = 0; cpu < MAX_C910_CORE_NUM; cpu++)
			c910_dbg_unmask(cpu);

         /*wdt reset enabled in brom,disable it hear to avoid system reset when hang*/
         CHIP_REG_SET(0x0, AONSYS_WDT_RESET_EN);

		// waiting for aon iopmp config restore by spl
		while (!chip_lp_mode_get_iopmp_flag());
		chip_lp_mode_set_iopmp_flag(0);

		//TODO: other IOPMP restor here ?
		//iopmp_restore();

		//10, exit ddr lp3
		mdelay(2);

		lpddr4_phy_lp3_exit(3733);
		LOG_I("After DDR LP3\n");


		//TODO: open PMP
		// Set PMP dummy start addr
		CHIP_REG_SET(0x0, 0xFFC21008);
		CHIP_REG_SET(0x0, 0xFFC21284);

		//TODO: 8.c board device resume here?
		board_device_resume();

		chip_lp_mode_set_ddr_flag(1);

		//8.i
#ifdef  CONFIG_AUDIO_ENABLE
		audio_powerup();
#endif
	}
	else if (chip_lp_mode == CHIP_LP_HW_VAD) {
		LOG_I("enter system suspend to disk resume..\n");
	}
	LOG_I("enter, %x, %x, %x\n",wake_source, CHIP_REG_GET(AON_PMU_INTC_INT_STS_L), CHIP_REG_GET(AON_PMU_INTC_INT_STS_H));
	(void)wake_source;
}

uint32_t chip_lpm_suspend(csi_pmic_t* pmic)
{
	//regu_voltage_param_t regu_apsys;
	uint32_t chip_lp_mode;

	chip_lp_mode = chip_lp_mode_get_lp_mode();
	if (chip_lp_mode == CHIP_LP_STANDBY)
	{
		//4.a
		do {
			if (cpu_is_inactive(C910_CORE0) && cpu_is_inactive(C910_CORE1) &&cpu_is_inactive(C910_CORE2) &&
					cpu_is_inactive(C910_CORE3) && indicator_magic_num_check()
#ifdef  CONFIG_AUDIO_ENABLE
				&& cpu_is_inactive(C906_CORE0)
#endif
				)
				break;
		} while (1);
		// powerdown secondary core
		for(int cpu = 1; cpu < MAX_C910_CORE_NUM; cpu++)
			c910_powerdown(cpu);

		//FIXME: DISABLE C910 Debug Mode？
		//4.b
		/* Mask the Debug Control Bit */
		for (int cpu = 0; cpu < MAX_C910_CORE_NUM; cpu++)
			c910_dbg_mask(cpu);
		//4.c ~ 4.g
		c910_l2cache_flush();

		//4.h
		for (int cpu = 0; cpu < MAX_C910_CORE_NUM; cpu++)
			c910_mem_rentention_enter(cpu, MEM_RET_SD);
		cpu_top_mem_retention_enter(MEM_RET_SD);

		//5.aa phy lp

		// Set PMP dummy start addr
		CHIP_REG_SET(0xFFFFF08, 0xFFC21008);
		// Set PMP start addr, start from 0x0
		CHIP_REG_SET(0x0 ,0xFFC21280);
		// Set PMP end addr, end at 3G
		CHIP_REG_SET(0xC0000, 0xFFC21284);
		// Set PMP_CFG, disable WR
		CHIP_REG_SET(0x0, 0xFFC21080);

		//5.a, 5.b
		lpddr4_phy_lp3_enter(3733);

		//5.c
		ap_power_down();
		mdelay(1);

		//5.d
		power_isolator_en();

		//5.e
		// already set in opensbi

		//TODO: 5.f
		//set board device to suspend
		//nothing to do

		//TODO: Save iopmp reg here ?
		//iopmp_save();

		//5.g
		aon_mem_retention_enter(MEM_RET_DSLP);

		//5.h
		sram_axi_reset();
		pvtc_reset();

		//5.i
		apsys_power_ctrl(pmic, false);

		//5.j
#ifdef  CONFIG_AUDIO_ENABLE
		audio_powerdown();
#endif

		//6.a ~ 6.b
		system_clk_switch(PLL_SLEEP, SYS_LP_STANDBY);

		//6.c, 6.d,6.e
		aonsys_wakeup_source_set(g_chip_wakeup_flag);
		LOG_I("Aon enter wfi\n");

		//6.f
		__WFI();

		LOG_I("Aon exit wfi\n");
	}
	else if (chip_lp_mode == CHIP_LP_HW_VAD) {
		LOG_I("enter system suspend to disk..\n");
	}
	return chip_lp_mode;
}

void chip_lpm_finish()
{
	// Check lowpower mode before went into finish process
	uint32_t chip_lp_mode;
	chip_lp_mode = chip_lp_mode_get_lp_mode();
	if(chip_lp_mode == CHIP_LP_STANDBY)
	{
		// clear magic number
		indicator_magic_num_clear();
		csi_timer_start((csi_timer_t *)&g_aon_wdt.timer_handle,
                              g_aon_wdt.monitor_tmr_len * 1000000);
		csi_irq_enable(DW_TIMER0_IRQn);
	}
	//clear lp mode
	chip_lp_mode_set_lp_mode(SYS_LP_NONE);
}


int chip_power_pd_ctrl(int pd, int mode)
{
	if (mode != LIGHT_AON_PM_PW_MODE_OFF && mode != LIGHT_AON_PM_PW_MODE_ON) {
			LOG_D("ERR->PM_FUNC_SET_RESOURCE_POWER_MODE\n");
			return -1;
	}

			switch (pd) {
			case LIGHT_AON_AUDIO_PD:
			    if(mode == LIGHT_AON_PM_PW_MODE_OFF) {
					audio_powerdown();
					LOG_D("audio power up\n");
				}
				else {
                    audio_powerup();
					LOG_D("audio power down\n");
				}
				break;
			case LIGHT_AON_VDEC_PD:
				if (mode == LIGHT_AON_PM_PW_MODE_OFF)
					vdec_powerdown();
				else {
					vdec_powerup();
					LOG_D("iopmp value before cfg: 0x%x\n", light_iopmp_get(IOPMP_VDEC));
					light_iopmp_config(IOPMP_VDEC);
					LOG_D("iopmp value after cfg: 0x%x\n", light_iopmp_get(IOPMP_VDEC));
				}
				break;
			case LIGHT_AON_NPU_PD:
				if (mode == LIGHT_AON_PM_PW_MODE_OFF)
					npu_powerdown();
				else {
					npu_powerup();
					LOG_D("iopmp value before cfg: 0x%x\n", light_iopmp_get(IOPMP_NPU));
					light_iopmp_config(IOPMP_NPU);
					LOG_D("iopmp value after cfg: 0x%x\n", light_iopmp_get(IOPMP_NPU));
				}
				break;
			case LIGHT_AON_VENC_PD:
				if (mode == LIGHT_AON_PM_PW_MODE_OFF)
					venc_powerdown();
				else {
					venc_powerup();
					LOG_D("iopmp value before cfg: 0x%x\n", light_iopmp_get(IOPMP_NPU));
					light_iopmp_config(IOPMP_VENC);
					LOG_D("iopmp value after cfg: 0x%x\n", light_iopmp_get(IOPMP_NPU));
				}
				break;
			case LIGHT_AON_GPU_PD:
				if (mode == LIGHT_AON_PM_PW_MODE_OFF)
					gpu_powerdown();
				else {
					gpu_powerup();
					light_iopmp_config(IOPMP_GPU);
				}
				break;
			case LIGHT_AON_DSP0_PD:
				if (mode == LIGHT_AON_PM_PW_MODE_OFF)
					dsp0_powerdown();
				else {
					dsp0_powerup();
					light_iopmp_config(IOPMP_DSP0);
				}
				break;
			case LIGHT_AON_DSP1_PD:
				if (mode == LIGHT_AON_PM_PW_MODE_OFF)
					dsp1_powerdown();
				else {
					dsp1_powerup();
					light_iopmp_config(IOPMP_DSP1);
				}
				break;
			default: {
				LOG_D("invalid power domain resource\n");
				return -1;
			}
}
return 0;
}

void chip_cpu_power_ctrl(int cpu_id, bool on)
{
   if(on){
       c910_powerup(cpu_id);
   } else{
	   c910_powerdown(cpu_id);
   }
}

void chip_set_wakeup_flag(uint32_t flag)
{
  g_chip_wakeup_flag = flag;
}


