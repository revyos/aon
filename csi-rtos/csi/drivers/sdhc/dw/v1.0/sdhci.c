/******************************************************************************
Software Sample Terms and Conditions These Software Sample Terms and Conditions
("Terms") cover the Software you license from Synopsys, unless and until we
enter into new terms that expressly replace these Terms. If you use the Software
as an employee of or for the benefit of your company, your company will be the
licensee under these Terms. Accepting it you consent to these Terms on behalf of
yourself and the company on whose behalf you will use the Software. The
effective date of these Terms is the date that you click accepted them. If you
do not agree to these Terms or if you do not have the power and authority to
accept these Terms on behalf of your company, you may not use the Software and
Synopsys is unwilling to provide you with them.

1. The Software is not an item of Licensed Software or Licensed Product under
any end-user software license agreement with Synopsys or any supplement thereto.
Synopsys hereby grants to you a limited, personal, non-exclusive,
non-transferable, non-assignable, fully paid, royalty free, worldwide, perpetual
license to use the Software, and create modifications of the components of the
Software provided to you in source code format, solely for use with a Synopsys
DesignWare IP. All modifications of the Software are owned by Synopsys, and you
hereby irrevocably assign ownership of those modifications (and all intellectual
property rights therein) to Synopsys. However, you are under no obligation to
disclose any such modifications to Synopsys and the modifications are
automatically licensed to you as Software. The Software and all modifications
are the Confidential Information of Synopsys, and you agree not to distribute or
disclose them.

2. THE SOFTWARE IS PROVIDED BY SYNOPSYS SOLELY ON AN "AS IS" BASIS AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE HEREBY
DISCLAIMED. IN NO EVENT SHALL SYNOPSYS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THE SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.

3. These Terms, which can be modified only by Synopsys in writing, shall be
governed by and construed under the laws of the State of California, USA,
without regard for its conflict of laws principles. You may not transfer or
assign your license rights to any other person in any manner (by assignment,
operation of law or otherwise) unless you have obtained written consent from
Synopsys. If you attempt to transfer or assign any of your license rights
without Synopsys's consent, the transfer or assignment will be ineffective,
null, and void. For purposes of this Section, a transfer or assignment of your
license rights will be deemed to have occurred (a) if a third party (or group of
third parties acting in concert) acquires beneficial ownership of fifty percent
(50%) or more of either your assets or of the stock or other equity interests
entitled to vote for your directors or equivalent managing authority, or (b) in
the event of a merger, consolidation or other business combination between you
and one or more third parties where your stockholders immediately before that
transaction own (directly or indirectly), after that transaction, less than
fifty percent (50%) of the stock or other equity interests entitled to vote for
the directors or equivalent managing authority of the surviving entity. These
Terms constitute the entire understanding and agreement between you and Synopsys
with respect to the subject matter hereof and supersedes and replaces all prior
and contemporaneous understandings and agreements, oral or written, express or
implied, regarding the same subject matter.
******************************************************************************/
/**
* \file		: sdhci.c
* \author	: ravibabu@synopsys.com
* \date		: 01-June-2019
* \brief	: sdhci.c source
* Revision history:
* Ver	Date		Author       	  	Change Id	Description
* 0.1	01-June-2019	ravibabui@synopsys.com  001		dev in progress
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "emmc_common.h"
#include "clog.h"
#include "emmc_error.h"
#include "mshc_regs.h"

#include "mmc_dev.h"
#include "mmc_core.h"
#include "sd_card.h"
#include "sd_cmd.h"
#include "hal.h"
#include "dwc_msdc_cfg.h"
#include "clkcore.h"
#include "sdhci.h"
#include "mmcm_clk.h"
#include "drv/common.h"
#include "drv/irq.h"
#include "drv/porting.h"

#define SDHCI_RESET_TIMEOUT	0xF
#define SDHCI_CMD_TIMEROUT 0xA   //Host cmd timerout 10s

/* module global datas */
struct sdhci_t g_sdhci[MAX_SDHCI_INSTANCES];

#ifdef CONFIG_MSHC_CSIM
uint8_t mem[1024 * MAX_SDHCI_INSTANCES];
#endif

/* external definitions */
extern struct clink_t *mshc_link, *msdc_link;
extern struct msg_desc_t mshc_msg, msdc_msg;
extern int sdhci_hal_register(struct sdhci_t *sdhci);

/* module specific parameters */
uint8_t tx_phase = 0x3F;
uint8_t rx_phase = 0;
/* below params are passed from API, below module param are not used,TBD */
uint8_t speed_mode = SDR12_DS_SPEED_MODE;
uint8_t is_emmc_dev = 1;

int sdhci_set_voltage(struct sdhci_t *sdhci, uint8_t mmc_vdd_sel, uint8_t mmc_vdd_volt);

/**
 * \brief is_sd_card_inserted
 *	check whether card is inserted or not
 * \param none
 * \returns true if card is inserted, 0 otherwise
 */
int is_sd_card_inserted(struct sdhci_t *sdhci)
{
	addr_t io_mem = sdhci->io_base;
	uint32_t pstate_reg;

	/* PState Register always shows (bit16) card is inserted
	 * irrespective of card is removed or inserted */
	pstate_reg = hal_read(sdhci->hal, (void *)(io_mem + PSTATE_REG_R));
	sdhci->card_present = ((pstate_reg & 0x10000) == 0x10000);
	clog_print(CLOG_INFO, "%s: pstate-reg=%x, card_present=%d\n", __func__, pstate_reg, sdhci->card_present);
	if (sdhci)
		return sdhci->card_present;
	else
		return 0;
}

int mmc_is_sd_card_inserted(struct mmc_dev_t *mmc_dev)
{
    struct sdhci_t *sdhci = mmc_dev->host->drv_data;
    return is_sd_card_inserted(sdhci);
}
/**
 * \brief sdhci_reset
 *	do sdhci controller reset
 * \param sdhci: pointer to sdhci object
 * \param mask: reset mask value
 * \returns none
 */
int sdhci_reset(struct sdhci_t *sdhci, uint8_t mask)
{
	addr_t io_mem = sdhci->io_base;
	uint32_t timeout = SDHCI_RESET_TIMEOUT, val;

	/* reset the sdhci controller */
	clog_print(CLOG_INFO, "%s: mask = %x\n", __func__, mask);
	hal_write8(sdhci->hal, (void *)(io_mem + SW_RST_R), mask);

	/* wait for completion */
	do {
		val = hal_read8(sdhci->hal, (void *)(io_mem + SW_RST_R));
		clog_print(CLOG_LEVEL6, "%s: SW_RST_R = %x\n", __func__, val);
		if (val & mask) {
			timeout--;
		} else
			break;
        mdelay(1);
	} while (timeout);

	if ((val & mask) && timeout == 0) {
		clog_print(CLOG_INFO, "sdhci reset failed, regval = %x\n", val);
        return -1;
    } else {
		clog_print(CLOG_INFO, "sdhci reset success, regval = %x\n", val);
    }

    return 0;
}

/**
 * \brief sdhci_enable_intr
 *	enable the interrupts
 * \param sdhci: pointer to sdhci object
 * \returns none
 */
void sdhci_enable_intr(struct sdhci_t *sdhci)
{
	addr_t io_mem = sdhci->io_base;

	/* enable the interrupt flags */
	sdhci->intr_mask =  CMD_TOUT_ERR_SIGNAL_EN |
			CUR_LMT_ERR_SIGNAL_EN | DATA_END_BIT_ERR_SIGNAL_EN |
			DATA_CRC_ERR_SIGNAL_EN | DATA_TOUT_ERR_SIGNAL_EN |
			CMD_END_BIT_ERR_SIGNAL_EN | CMD_IDX_ERR_SIGNAL_EN |
			CMD_CRC_ERR_SIGNAL_EN | RE_TUNE_EVENT_SIGNAL_EN |
			CARD_REMOVAL_SIGNAL_EN | XFER_COMPLETE_SIGNAL_EN |
			CMD_COMPLETE_SIGNAL_EN | CARD_INSERTION_SIGNAL_EN | BUF_RD_READY_SIGNAL_EN;

    hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_EN_R), sdhci->intr_mask);
	hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_SIGNAL_EN_R), sdhci->intr_mask);
}

/**
 * \brief sdhci_disable_intr
 *	disable the interrupts
 * \param sdhci: pointer to sdhci object
 * \returns none
 */
void sdhci_disable_intr(struct sdhci_t *sdhci)
{
	addr_t io_mem = sdhci->io_base;

	/* enable the interrupt flags */
	sdhci->intr_mask = 0;

	hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_EN_R), sdhci->intr_mask);
	hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_SIGNAL_EN_R), sdhci->intr_mask);
}

/**
 * \brief sdhci_io_ctrl
 *	perform various io-ctrl operation based on cmd/args
 * \param sdhci: pointer to sdhci object
 * \param cmd: cmd parameter
 * \param args : parameter
 * \returns none
 */
int sdhci_io_ctrl(struct sdhci_t *sdhci, uint32_t cmd, uint32_t args)
{
	addr_t io_mem = sdhci->io_base;
	uint16_t v1_offs = sdhci->vendor1_offs;

	switch (cmd) {
	case SDHCI_CLOCK_OFF:
		hal_write16(sdhci->hal, (void *)(io_mem + CLK_CTRL_R), 0);
		break;
	case SDHCI_CLOCK_ON:
		hal_write16(sdhci->hal, (void *)(io_mem + CLK_CTRL_R),
			FREQ_SEL_VAL | INTERNAL_CLK_EN |
			INTERNAL_CLK_STABLE | SD_CLK_EN | PLL_ENABLE);
		//val = hal_read16(sdhci->hal, (void *)(io_mem + CLK_CTRL_R));
		//clog_print(CLOG_LEVEL7, "%s: clk_ctrl_reg = %x\n", __func__, val);
		break;
	case SDHCI_POWER_OFF:
		hal_write16(sdhci->hal, (void *)(io_mem + HOST_CTRL2_R), 0);
		hal_write(sdhci->hal, (void *)(io_mem + HOST_CTRL1_R), 0);
		break;
	case SDHCI_POWER_ON:
		sdhci_set_voltage(sdhci, SD_BUS_PWR_VDD1, args);
		break;
	case SDHCI_SET_TXPHASE:
		hal_reg32_set_field(sdhci->hal, (void *)(io_mem + 0x31c),
			16, 8, args & 0x7F);
		break;
	case SDHCI_SET_RXPHASE:
		/* enable software tuning on */
		hal_reg8_set_field(sdhci->hal, (void *)(io_mem + v1_offs + AT_CTRL_R),
					SW_TUNE_EN_STBIT, 1, 1);

		/* set rx phase value */
		hal_write(sdhci->hal, (void *)(io_mem + v1_offs + AT_STAT_R), args);
		break;
	case SDHCI_SW_TUNE_CTRL:
		/* enable/disable software tuning on */
		if (args == DISABLE)
                ;
		else if (args == ENABLE)
                ;
		else
			return 1;

		if (cmd == SDHCI_SW_TUNE_CTRL) {
			hal_write(sdhci->hal, (void *)(io_mem + v1_offs + AT_CTRL_R),
					0x1f000015);
		}
		else if (cmd == SDHCI_CMDCFLT_CHK)
			hal_write(sdhci->hal, (void *)(io_mem + v1_offs + MHSC_CTRL_R),
					0xf0000);
		break;
	}
	return 0;
}

/**
 * \brief sdhci_setup_clock
 *	setup sdhci clock control
 * \param sdhci: pointer to sdhci object
 * \param mask: reset mask value
 * \returns none
 */
int sdhci_setup_clock(struct sdhci_t *sdhci, uint32_t clock)
{
	addr_t io_mem = sdhci->io_base;
	uint16_t vendor1_ptr;

	/* set the tx_phase value */
	hal_write8(sdhci->hal, (void *)(io_mem + PHY_SDCLKDL_DC_R), tx_phase);

	/* disable the software rx-tuning */
	vendor1_ptr = hal_read16(sdhci->hal, (void *)(io_mem + P_VENDOR_1_SPECIFIC_AREA));
	hal_reg32_set_field(sdhci->hal, (void *)(io_mem + vendor1_ptr + SNPS_AT_CTRL_R),
			SWIN_TH_EN_BITP, SWIN_TH_EN_BITW, BIT_OFF);

	/* set the rx_phase value */
	hal_write(sdhci->hal, (void *)(io_mem + vendor1_ptr + SNPS_AT_STAT_R), rx_phase);
	return 0;
}

/**
 * \brief sdhci_disable_clock
 *	disable the controller clock
 * \param sdhci: pointer to sdhci object
 * \returns none
 */
int sdhci_disable_clock(struct sdhci_t *sdhci)
{
	addr_t io_mem = sdhci->io_base;
	clog_print(CLOG_INFO, "disable sdhci clock\n");

	/* disable clock */
	hal_write(sdhci->hal, (void *)(io_mem + CLK_CTRL_R), 2);
	return 0;
}

/**
 * \brief sdhci_enable_clock
 *	configure the sdhci speed-mode, mmc-clocks
 * \param sdhci: pointer to sdhci object
 * \param speed_mode: sdhci speed-mode
 * \param mmc_vdd : mmc vdd volatage (1.8V or 3.3V)
 * \param mmcm_clock: setup mmcmc clocks
 * \returns 0 always
 */
int sdhci_enable_clock(struct sdhci_t *sdhci, uint8_t speed_mode,
	uint8_t mmc_vdd, uint32_t mmcm_clock)
{
	addr_t io_mem = sdhci->io_base;
	uint32_t freq_sel, val;
	uint8_t enable_1p8v = 0;
//	uint8_t enable_1p8v, mmcm_clk_enable = 0;
//	uint16_t vptr;
//	uint8_t is_emmc = sdhci->is_emmc_dev;

	clog_print(CLOG_INFO, "enable sdhci clock\n");
	/* switch to 1.8V */
	enable_1p8v = mmc_vdd;
	if (enable_1p8v)
		hal_reg16_set_field(sdhci->hal, (void *)(io_mem + HOST_CTRL2_R), 3, 1, 1);
	else
		hal_reg16_set_field(sdhci->hal, (void *)(io_mem + HOST_CTRL2_R), 3, 1, 0);
    uint32_t sdhci_freq = soc_get_sdio_freq(sdhci->id);
#if 0
	/* set default 400Khz */
	freq_sel = FREQSEL_CLK_400KHZ;
	if (is_emmc == 0) {
		switch (speed_mode) {
		case SDR12_DS_SPEED_MODE:
			freq_sel = FREQSEL_SDR12;
			break;
		case SDR25_HS_SPEED_MODE:
			freq_sel = FREQSEL_SDR25;
			break;
		case SDR50_SPEED_MODE:
			freq_sel = FREQSEL_SDR50;
			mmcm_clk_enable = 1;
			break;
		case SDR104_SPEED_MODE:
			freq_sel = FREQSEL_SDR104;
			mmcm_clk_enable = 1;
			break;
		case DDR50_SPEED_MODE:
			freq_sel = FREQSEL_DDR50;
			break;
		default:
			break;
		}
	} else {
		/* set default 400Khz */
		freq_sel = FREQSEL_CLK_400KHZ;
		switch (speed_mode) {
		case XEMMC_DS_SPEED_MODE:
			freq_sel = FREQSEL_SDR12;
			break;
		case XEMMC_HS_SPEED_MODE:
			freq_sel = FREQSEL_SDR25;
			break;
		case XEMMC_HSDDR_SPEED_MODE:
			freq_sel = FREQSEL_DDR50;
			break; case XEMMC_HS200_SPEED_MODE: case XEMMC_HS400_SPEED_MODE:
			mmcm_clk_enable = 1;
			break;
		default:
			break;
		}
	}

	vptr = hal_read(sdhci->hal, (void *)(io_mem + P_VENDOR_1_SPECIFIC_AREA));

	/* enable mmcm clock to generate higher frequency */
	if (mmcm_clk_enable && mmcm_clock != MMCM_CLK_400KHZ) {

		/* disable software tuning on */
		hal_reg8_set_field(sdhci->hal, (void *)(io_mem + vptr + AT_CTRL_R),
					4, 1, 0);
		/* disable command conflict check on */
		hal_reg8_set_field(sdhci->hal, (void *)(io_mem + vptr + MHSC_CTRL_R),
				0, 1, 0);


		/* put mmcm module into reset mode */
		hal_write(sdhci->hal, (void *)(io_mem + MMCM_RESET_REG_OFFS), 1);

		/* configure and enable mmcm clock */
		clock_enable(sdhci->mmcm_clk, mmcm_clock);

		/* bring mmcm module out of reset mode */
		hal_write(sdhci->hal, (void *)(io_mem + MMCM_RESET_REG_OFFS), 0);
	}

	if (rx_phase > 0) {
		/* enable software tuning on */
		hal_reg8_set_field(sdhci->hal, (void *)(io_mem + vptr + AT_CTRL_R), 4, 1, 1);

		/* set rx phase value */
		hal_write(sdhci->hal, (void *)(io_mem + vptr + AT_STAT_R), rx_phase);
	}

	if (mmcm_clock == MMCM_CLK_400KHZ) {
		freq_sel = FREQSEL_CLK_400KHZ;
    }

	if (mmcm_clk_enable && mmcm_clock != MMCM_CLK_400KHZ)
		val |= CLK_GEN_SELECT;

#endif

    freq_sel = (sdhci_freq + mmcm_clock % sdhci_freq) / mmcm_clock / 2;
    if (sdhci_freq <= mmcm_clock) {
        freq_sel = 0;
    }

	/* configure clkctrl to select the mmc clock */
	val = (freq_sel << 8) | INTERNAL_CLK_EN;
    /*disable clock*/
	hal_write16(sdhci->hal, (void *)(io_mem + CLK_CTRL_R), 0);
	hal_write16(sdhci->hal, (void *)(io_mem + CLK_CTRL_R), val);
	hal_write16(sdhci->hal, (void *)(io_mem + CLK_CTRL_R), val | SD_CLK_EN | PLL_ENABLE);
	hal_write8(sdhci->hal, (void *)(io_mem + SW_RST_R), 0x6);

	return 0;
}

/**
 * \brief sdhci_set_voltage
 *	configure sdhci VDD1 or VDD2 voltage
 * \param sdhci: pointer to sdhci object
 * \param mmc_vdd_sel: mmc vdd selection
 * \param mmc_vdd_vol: mmc vdd volatge
 * \returns 0
 */
int sdhci_set_voltage(struct sdhci_t *sdhci, uint8_t mmc_vdd_sel, uint8_t mmc_vdd_volt)
{
	addr_t io_mem = sdhci->io_base;
	uint8_t val = 0, vdd = EMMC_VDD1_3P3_VOLT;
	uint8_t is_emmc = sdhci->is_emmc_dev;

	switch (mmc_vdd_sel) {
	case XSD_BUS_PWR_VDD1:

		/* select vdd voltage based on sd or eMMC device */
		switch (mmc_vdd_volt) {
		case XVDD1_1P8_VOLT:
			if (is_emmc)
				vdd = EMMC_VDD1_1P8_VOLT;
			else
				vdd = SD_VDD1_1P8_VOLT;
			break;
		case XVDD1_3P0_VOLT:
			if (is_emmc)
				return ERR_INVARG;
			vdd = SD_VDD1_3P0_VOLT;
			break;
		case XVDD1_1P2_VOLT:
			if (is_emmc)
				vdd = EMMC_VDD1_1P2_VOLT;
			else
				return ERR_INVARG;
			break;
		case XVDD1_3P3_VOLT:
			vdd = SD_VDD1_3P3_VOLT;
			break;
		case XVDD1_POWER_OFF:
			vdd = 0;
			break;
		default:
			return ERR_INVARG;
		}

		if (mmc_vdd_volt != XVDD1_POWER_OFF)
			val = (vdd << 1) | SD_BUS_PWR_VDD1;
		break;
	case XSD_BUS_PWR_VDD2:
		val = (mmc_vdd_volt << 5) | SD_BUS_PWR_VDD2;
		break;
	default:
		return ERR_INVARG;
	}

	/* set bus power */
	hal_write8(sdhci->hal, (void *)(io_mem + PWR_CTRL_R), val);
	return 0;
}

/**
 * \brief sdhci_set_speed_mode
 *	set the speed mode
 * \param sdhci: pointer to sdhci object
 * \param speed_mode: sdhci speed-mode
 * \returns none
 */
int sdhci_set_speed_mode(struct sdhci_t *sdhci, uint8_t speed_mode)
{
	addr_t io_mem = sdhci->io_base;
	uint8_t uhs_mode = 0;
	uint8_t is_emmc = sdhci->is_emmc_dev;

	clog_print(CLOG_INFO, "set speed mode %d\n", speed_mode);

	if (is_emmc == 0) {
		switch (speed_mode) {
		case SDR12_DS_SPEED_MODE:
			break;
		case SDR25_HS_SPEED_MODE:
			uhs_mode = UHS_MODE_SDR25;
			break;
		case SDR50_SPEED_MODE:
			uhs_mode = UHS_MODE_SDR50;
			break;
		case SDR104_SPEED_MODE:
			uhs_mode = UHS_MODE_SDR104;
			break;
		case DDR50_SPEED_MODE:
			uhs_mode = UHS_MODE_DDR50;
			break;
		default:
			uhs_mode = 0;
			break;
		}
	} else {

		switch (speed_mode) {
		case XEMMC_DS_SPEED_MODE:
			uhs_mode = UHS_MODE_EMMC_DS;
			break;
		case XEMMC_HS_SPEED_MODE:
			uhs_mode = UHS_MODE_EMMC_HS;
			break;
		case XEMMC_HSDDR_SPEED_MODE:
			uhs_mode = UHS_MODE_EMMC_HSDDR;
			break;
		case XEMMC_HS200_SPEED_MODE:
			uhs_mode = UHS_MODE_EMMC_HS200;
			break;
		case XEMMC_HS400_SPEED_MODE:
			uhs_mode = USH_MODE_EMMC_HS400;
			break;
		default:
			uhs_mode = 0;
			break;
		}
	}

	/* set speed mode */
    hal_reg16_set_field(sdhci->hal, (void *)(io_mem + HOST_CTRL2_R), 0, 3, uhs_mode);
	return 0;
}

/**
 * \brief sdhci_set_data_width
 *	set sd bus data width
 * \param sdhci: pointer to sdhci object
 * \param width: bus width 4 or 8bit
 * \returns none
 */
int sdhci_set_data_width(struct sdhci_t *sdhci, uint8_t width)
{
	addr_t io_mem = sdhci->io_base;
	uint8_t val;

	/* set data width */
	val = 0x00;
	if (width == SD_BUSWIDTH_8)
		val |= (HIGH_SPEED_EN | EXTDAT_XFER_WIDTH8);
	else if (width == SD_BUSWIDTH_4)
		val |= DAT_XFER_WIDTH_4BIT;

	hal_write8(sdhci->hal, (void *)(io_mem + HOST_CTRL1_R), val);
	return 0;
}

/**
 * \brief sdhci_alloc
 *	allocate sdhci instance
 * \param none:
 * \returns return pointer to sdhci obj
 */
struct sdhci_t *sdhci_alloc(void)
{
	int i;

	for (i = 0; i < MAX_SDHCI_INSTANCES; ++i)
		if (g_sdhci[i].assigned == 0)
			break;

	if (i >= MAX_SDHCI_INSTANCES)
		return NULL;

	g_sdhci[i].id = i;
	g_sdhci[i].assigned = 1;
	return &g_sdhci[i];
}

void sdhci_free(struct sdhci_t *sdhci)
{
    if (sdhci == NULL)
        return;
    sdhci->id = 0;
    sdhci->assigned = 0;
}

/**
 * \brief sdhci_register
 *	register sdhci controller
 * \param base_addr: base address of sdhci controller
 * \param irq: irq number
 * \returns pointer to sdhci instance
 */
struct sdhci_t *sdhci_register(addr_t base_addr, uint8_t irq)
{
	struct sdhci_t *sdhci;
	int retval;

	sdhci = sdhci_alloc();
	if (sdhci == NULL)
		return NULL;

	sdhci->io_base = base_addr;
	sdhci->reg = (struct sdhci_reg_t *)base_addr;
	sdhci->irq = irq;

	retval = sdhci_hal_register(sdhci);
	if (retval < 0) {
		clog_print(CLOG_LEVEL5, "failed to register hal\n");
		return NULL;
	}

	return sdhci;
}


/**
 * \brief sdhci_send_cmd
 *	configure sdhci to send sd command
 * \param sdhci: pointer to sdhci object
 * \param io_req: pointer to io_req_t
 * \returns none
 */

#define SDHCI_CMD_MAX_TIMEOUT           3200
#define SDHCI_CMD_DEFAULT_TIMEOUT       100

int sdhci_send_cmd(struct sdhci_t *sdhci, struct io_req_t *io_req)
{
	struct mmc_req_t *req = io_req->req;
	struct mmc_dev_t *mmc_dev = req->mmc_dev;
	addr_t io_mem = sdhci->io_base;
	uint16_t xfer_mode;
	uint32_t time = 0;
	uint32_t cmd_timeout = SDHCI_CMD_DEFAULT_TIMEOUT;

	if (io_req->issued) {
		clog_print(CLOG_LEVEL5, "Bug: wrong state, cmd already issued\n");
		return io_req->status;
	}

#define  SDHCI_CMD_INHIBIT	BIT(0)
#define  SDHCI_DATA_INHIBIT	BIT(1)
#define MMC_CMD_STOP_TRANSMISSION	12
#define MMC_CMD_SEND_TUNING_BLOCK		19
#define MMC_CMD_SEND_TUNING_BLOCK_HS200	21

    uint32_t mask = SDHCI_CMD_INHIBIT | SDHCI_DATA_INHIBIT;
    uint32_t cmd_idx = (req->cmdreg & (0x3f << 8)) >> 8;

    if(cmd_idx == MMC_CMD_STOP_TRANSMISSION ||
       ((cmd_idx == MMC_CMD_SEND_TUNING_BLOCK ||
         cmd_idx == MMC_CMD_SEND_TUNING_BLOCK_HS200) && req->databuf == NULL)) {
        mask &= ~SDHCI_DATA_INHIBIT;
    }

    while (hal_read(sdhci->hal, (void *)(io_mem + PSTATE_REG_R)) & mask) {
        if (time >= cmd_timeout) {
		    clog_print(CLOG_WARN, "%s: MMC: %d busy ", __func__, mmc_dev->id);
            if (2 * cmd_timeout <= SDHCI_CMD_MAX_TIMEOUT) {
                cmd_timeout += cmd_timeout;
		        clog_print(CLOG_WARN, "timeout increasing to: %u ms.\n", cmd_timeout);
            } else {
                return -1;
            }
        }
        time++;
        udelay(1000);
    }

	/* if use_dat = 1 check if host driver issues abort command */
	mmc_dev->cmd_inprogress = 1;
	io_req->status = MMC_CMD_STATUS_IN_PROGRESS;
	io_req->complete = 0;
	io_req->issued = 1;
	sdhci->act_req = io_req;

	/* clear all interrupts */
	hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R), 0xFFFFFFFF);
	//hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R), 0);

	xfer_mode = 0;
	clog_print(CLOG_LEVEL5, "%s: req->flags=%x\n", __func__, req->flags);
	/* configure block size */
	if (req->flags & XFR_MODE_DATA_READ_F)
		xfer_mode |= XFR_MODE_DATA_READ;
	if (req->flags & XFR_MODE_MULTBLK_SEL_F)
		xfer_mode |= XFR_MODE_MULTBLK_SEL;
	if (req->flags & XFR_MODE_DMA_EN_F)
		xfer_mode |= XFR_MODE_DMA_EN;
	if (req->flags & XFR_MODE_AUTOCMD12_EN_F)
		xfer_mode |= XFR_MODE_AUTOCMD12_EN;
	if (req->flags & XFR_MODE_AUTOCMD23_EN_F)
		xfer_mode |= XFR_MODE_AUTOCMD23_EN;
    if (req->flags & XFR_MODE_AUTOCMD_AUTO_SEL_F)
		xfer_mode |= XFR_MODE_AUTOCMD_AUTOSEL;
	if (req->flags & XFR_MODE_RESP_ERRCHK_EN_F)
		xfer_mode |= XFR_MODE_RESP_ERRCHK_EN;
	if (req->flags & XFR_MODE_RESP_INT_EN_F)
		xfer_mode |= XFR_MODE_RESP_INT_EN;
	if (req->flags & XFR_MODE_BLKCNT_EN_F) {
		xfer_mode |= XFR_MODE_BLKCNT_EN;
		hal_write16(sdhci->hal, (void *)(io_mem + BLOCKSIZE_R),
			req->blocksize & 0xFFF /*| SDMA_BUF_BDARY_512K*/);
		hal_write16(sdhci->hal, (void *)(io_mem + BLOCKCOUNT_R),
			req->nblocks & 0xFFF);
	}
	if (req->flags & SDHCI_INTR_ENABLE) {
		hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_EN_R), req->int_mask);
	    hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_SIGNAL_EN_R), req->int_mask);
    }

	//hal_write(sdhci->hal, &sdhci->reg->arg, req->args);
	hal_write(sdhci->hal, (void *)(io_mem + ARGUMENT_R), req->args);

	hal_write(sdhci->hal, (void *)(io_mem + XFER_MODE_R),
		(xfer_mode | (req->cmdreg << 16)));

#ifndef SDHIC_IRQ_MODE
void sdhci_irq_handler_for_csi(void *arg);
#include "drv/tick.h"
    while(!io_req->complete) {
        sdhci_irq_handler_for_csi(sdhci);
    }
#endif

	return 0;
}

static void sdhci_read_one_block(struct sdhci_t *sdhci, uint8_t *databuf, uint32_t blocksize)
{
	addr_t io_mem = sdhci->io_base;
    blocksize = blocksize / 4;
    uint16_t psta = hal_read16(sdhci->hal, (void *)(io_mem + PSTATE_REG_R));
    while(!(psta & BUF_RD_ENABLE)) {
        psta = hal_read16(sdhci->hal, (void *)(io_mem + PSTATE_REG_R));
    }

    int idx = 0;
    for(int i = 0; i < blocksize; i++) {
        uint32_t data = hal_read(sdhci->hal, (void *)(io_mem + BUF_DATA_R));
	    *(uint32_t *)&databuf[idx] = data;
	    idx += 4;
    }
}

static int sdhci_cmd0_preidle(struct sdhci_t *sdhci)
{
	addr_t io_mem = sdhci->io_base;
    hal_write(sdhci->hal, (void *)(io_mem + ARGUMENT_R), 0xf0f0f0f0);
    hal_write(sdhci->hal, (void *)(io_mem + XFER_MODE_R), (0 | (0 << 16)));

    uint32_t intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));
    while(!(intr_status & CMD_COMPLETE_SIGNAL_EN)) {
        intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));
        if (intr_status & SD_ERROR_MASK) {
            clog_print(CLOG_ERR, "%s, %d error int sta, 0x%x\n", __func__, __LINE__ ,intr_status);
            return -1;
        }
    }

	hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R), 0xFFFFFFFF);
    return 0;
}

/*
static int sdhci_cmd0_boot(struct sdhci_t *sdhci)
{
	addr_t io_mem = sdhci->io_base;
    hal_write(sdhci->hal, (void *)(io_mem + ARGUMENT_R), 0xfffffffa);
    hal_write16(sdhci->hal, (void *)(io_mem + CMD_R), 1 << 5);
    uint32_t intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));
    while(!(intr_status & CMD_COMPLETE_SIGNAL_EN)) {
        intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));
        if (intr_status & SD_ERROR_MASK) {
            clog_print(CLOG_ERR, "%s, %d error int sta, 0x%x\n", __func__, __LINE__ ,intr_status);
            return -1;
        }
    }

	//hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R), 0xFFFFFFFF);
    return 0;
}*/

int sdhci_boot_read(struct sdhci_t *sdhci, uint8_t *databuf, uint32_t buflen, uint32_t blocksize)
{
	addr_t io_mem = sdhci->io_base;
	uint32_t pstate_reg;
	uint16_t xfer_mode = 0;
    uint32_t nblocks = 0;
    int ret = 0;

    if(databuf == NULL || (blocksize % 4) != 0) {
        return -1;
    }

	uint32_t int_signal_en = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_SIGNAL_EN_R));
	/* check if PSTATE_REG.CMD_INHIBIT = 0 ? */
	pstate_reg = hal_read(sdhci->hal, (void *)(io_mem + PSTATE_REG_R));
	if (pstate_reg & CMD_INHIBIT) {
		clog_print(CLOG_ERR, "cannot issue cmd, cmd line busy, pst_reg=%x\n", pstate_reg);
		return -1;
	}

    /* clear all interrupts */
	hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R), 0xFFFFFFFF);
	hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_SIGNAL_EN_R), 0);

	/* configure block size */
	xfer_mode |= XFR_MODE_DATA_READ;
    nblocks = buflen / blocksize;
    if (nblocks > 1) {
		xfer_mode |= XFR_MODE_MULTBLK_SEL;
    }

    xfer_mode |= XFR_MODE_BLKCNT_EN;

    /*into pre idle*/
    if (sdhci_cmd0_preidle(sdhci) != 0) {
        return -1;
    }

	hal_write8(sdhci->hal, (void *)(io_mem + TOUT_CTRL_R), SDHCI_CMD_TIMEROUT);
	hal_write16(sdhci->hal, (void *)(io_mem + BLOCKSIZE_R), blocksize & 0xFFF);
	hal_write16(sdhci->hal, (void *)(io_mem + BLOCKCOUNT_R), nblocks & 0xFFFF);
    hal_write16(sdhci->hal, (void *)(io_mem + XFER_MODE_R), xfer_mode);
	hal_write16(sdhci->hal, (void *)(io_mem + 0x52e), (0xE << 12 | 1 << 7 | 1));

    uint32_t intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));
    while(!(intr_status & SD_DAT_INTR_MASK)) {
        intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));
        if ((intr_status & SD_ERROR_MASK)) {
            clog_print(CLOG_ERR, "%s, %d error int sta, 0x%x\n", __func__, __LINE__ ,intr_status);
	        hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R), SD_ERROR_MASK);
            return -1;
        }
    }

    for (int i = 0; i < nblocks; i++) {
        sdhci_read_one_block(sdhci, databuf, blocksize);
        databuf += blocksize;
    }

    /*wait transfer complete*/
    intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));
    while(!(intr_status & XFER_COMPLETE_SIGNAL_EN)) {
        intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));
        if (intr_status & SD_ERROR_MASK) {
            clog_print(CLOG_ERR, "%s, %d error int sta, 0x%x\n", __func__, __LINE__ ,intr_status);
            ret = -1;
            break;
        }
    }

    intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));
    while(!(intr_status & CMD_COMPLETE_SIGNAL_EN)) {
        intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));
        if (intr_status & SD_ERROR_MASK) {
            clog_print(CLOG_ERR, "%s, %d error int sta, 0x%x\n", __func__, __LINE__ ,intr_status);
            ret = -1;
            break;
        }
    }

	hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R), 0xFFFFFFFF);
	hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_EN_R), int_signal_en);
	hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_SIGNAL_EN_R), int_signal_en);

	return ret;
}

/**
 * \brief sdhci_complete_request
 *	complete the io-req and giveback
 * \param sdhci: pointer to sdhci object
 * \returns none
 */
void sdhci_complete_request(struct sdhci_t *sdhci)
{
	struct io_req_t *io_req = sdhci->act_req;

	io_req->complete = 1;
}

/**
 * \brief sdhci_handle_cmd_intr
 *	handle the command interrupt events
 * \param sdhci: pointer to sdhci object
 * \param mask: interrupt mask
 * \returns none
 */
int sdhci_handle_cmd_intr(struct sdhci_t *sdhci, uint32_t intr_mask)
{
	struct io_req_t *io_req = sdhci->act_req;
	addr_t io_mem = sdhci->io_base;
	struct mmc_req_t *mmc_req;
	uint8_t req_complete = 0, i;

	mmc_req = io_req->req;

	/* handle command with/without data transfer */
	/*check cmd complete interrupt rcvd */
	if (intr_mask & CMD_COMPLETE_SIGNAL_EN) {
		/* read response register */
		sdhci->intr_mask &= ~CMD_COMPLETE_SIGNAL_EN;

		for (i = 0; i < 4; ++i) {
			mmc_req->resp[i] = hal_read(sdhci->hal, (void *)(io_mem	+ RESP01_R + i * 4));
			clog_print(CLOG_LEVEL4, "%s: reg(%p) resp[%d]=%x reqflag=%x\n",__func__,
				(io_mem + RESP01_R + i * 4), i, mmc_req->resp[i], mmc_req->flags);
		}

		if (mmc_req->flags & CMD_WITHOUT_DATA)
			req_complete = 1;
	}

	/* clear specific interrupt */
	hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R), intr_mask);

	if (sdhci->intr_mask & SD_CMD_INTR_MASK) {
		clog_print(CLOG_LEVEL3, "unhandled interrupts %x\n", sdhci->intr_mask & SD_CMD_INTR_MASK);
	}

	io_req->error = intr_mask >> 16;
	/* give back and complete the request */

	return req_complete;
}

/**
 * \brief sdhci_handle_data_intr
 *	handle the data interrupt events
 * \param sdhci: pointer to sdhci object
 * \param mask: interrupt mask
 * \returns none
 */
int sdhci_handle_data_intr(struct sdhci_t *sdhci, int32_t intr_mask)
{
	struct io_req_t *io_req = sdhci->act_req;
	addr_t io_mem = sdhci->io_base;
	struct mmc_req_t *mmc_req;
	uint8_t req_complete = 0;
	uint32_t data;
	int idx, i;

	mmc_req = io_req->req;
	if (mmc_req == NULL)
		return req_complete;

	/* handle command with/without data transfer */
	/*check cmd complete interrupt rcvd */
	if (intr_mask & BUF_RD_READY_SIGNAL_EN) {
		clog_print(CLOG_LEVEL3, "%s: buffer data Ready signal \n",__func__);
		/* read response register */
		sdhci->intr_mask &= ~BUF_RD_READY_SIGNAL_EN;

		idx = mmc_req->rx_data_len;
		clog_print(CLOG_LEVEL4, "%s:mmc_req->blocksize(%d), idx(%d) mmc_req->buflen(%d)\n", __func__,
			mmc_req->blocksize, idx, mmc_req->buflen);
		clog_print(CLOG_LEVEL4, "%s: reading args/block-adr(%x)\n", __func__, mmc_req->args);
		for (i = 0; (i < mmc_req->blocksize/4) && (idx < mmc_req->buflen); ++i) {
			data = hal_read(sdhci->hal, (void *)(io_mem + BUF_DATA_R));
			if (mmc_req->databuf && idx < mmc_req->buflen)
				*(uint32_t *)&mmc_req->databuf[idx] = data;
			else {
				clog_print(CLOG_ERR, "Error: DataLoss: insufficient read buffer\n");
				clog_print(CLOG_ERR, "databuf(%x), buf_offs(%d), buflen(%d)\n",
						mmc_req->databuf, idx, mmc_req->buflen);
			}
			idx += 4;
		}

		mmc_req->rx_data_len = idx;
		clog_print(CLOG_LEVEL4, "\n");
	}

	if (intr_mask & BUF_WR_READY_SIGNAL_EN) {
		clog_print(CLOG_LEVEL3, "%s: buffer data Write ready signal len(%d)\n",__func__,
			mmc_req->buflen);

		sdhci->intr_mask &= ~BUF_WR_READY_SIGNAL_EN;
		idx = mmc_req->tx_data_len;

		clog_print(CLOG_LEVEL4, "%s:mmc_req->blocksize(%d), idx(%d) mmc_req->buflen(%d)\n",
			__func__, mmc_req->blocksize, idx, mmc_req->buflen);
		if (mmc_req->databuf && (idx < mmc_req->buflen)) {
			/* write data to write buffer */
			for (i = 0; (i < mmc_req->blocksize/4) && (idx < mmc_req->buflen); ++i) {
				hal_write(sdhci->hal, (void *)(io_mem+ BUF_DATA_R),
					*(uint32_t *)&mmc_req->databuf[idx]);
				idx += 4;
			}
			mmc_req->tx_data_len = idx;
		}
	}

	return req_complete;
}


static int sdhic_wait_cmd_end(struct sdhci_t *sdhci)
{
	addr_t io_mem = sdhci->io_base;
    uint32_t intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));
    uint32_t timeout_us = 1000;
    while(!(intr_status & CMD_COMPLETE_SIGNAL_EN)) {
        intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));
        udelay(10);
        timeout_us -=10;
        if (timeout_us <= 0) {
            break;
        }
    }

    if(timeout_us == 0 && (intr_status & CMD_COMPLETE_SIGNAL_EN)) {
        clog_print(CLOG_ERR, "%s, %d, wait cmd complete timeout!!!\n", __func__, __LINE__);
        return -1;
    }

    return 0;
}

int sdhci_handle_cmd_err_intr(struct sdhci_t *sdhci, uint32_t intr_status)
{
	addr_t io_mem = sdhci->io_base;
    int ret = 0, wait_cmd_flag = false;
    uint32_t xfer_mode = hal_read16(sdhci->hal, (void *)(io_mem + XFER_MODE_R));
    uint32_t ctrl_2 = hal_read16(sdhci->hal, (void *)(io_mem + HOST_CTRL2_R));

    if (intr_status & SD_CMD_ERROR_MASK) {
        if (intr_status == AUTO_CMD_ERR_SIGNAL_EN) {
            wait_cmd_flag = false;
        } else if (!(xfer_mode & XFR_MODE_RESP_INT_EN) && !(ctrl_2 & HOST_CTRL2_EXEC_TUNNING)) {
            if (!(intr_status & CMD_TOUT_ERR_SIGNAL_EN)) {
                wait_cmd_flag = true;
            }
        }

        if (wait_cmd_flag) {
            if (sdhic_wait_cmd_end(sdhci) != 0) {
                return -1;
            }
        }

        ret = sdhci_reset(sdhci, SW_RST_CMD);
        if (ret != 0) {
            clog_print(CLOG_ERR, "%s, %d, reset timeout!!\n", __func__, __LINE__);
            return -1;
        }
    }
    return 0;
}

int sdhci_handle_dat_err_intr(struct sdhci_t *sdhci, uint32_t intr_status)
{
    if (intr_status & SD_DAT_ERROR_MASK) {
        int ret = sdhci_reset(sdhci, SW_RST_DAT);
        if (ret != 0) {
            clog_print(CLOG_ERR, "%s, %d, reset timeout!!\n", __func__, __LINE__);
            return -1;
        }
    }
    return 0;
}

int sdhci_handle_err_intr(struct sdhci_t *sdhci, uint32_t intr_status)
{
	addr_t io_mem = sdhci->io_base;
	uint32_t pstate_reg, timeout_us = 10000;
    int ret = 0;

    if (intr_status & SD_ERROR_MASK) {
        if (intr_status & SD_CMD_ERROR_MASK) {
            ret += sdhci_handle_cmd_err_intr(sdhci, intr_status);
        }
        if (intr_status & SD_DAT_ERROR_MASK) {
            ret +=sdhci_handle_dat_err_intr(sdhci, intr_status);
        }
    }

	pstate_reg = hal_read(sdhci->hal, (void *)(io_mem + PSTATE_REG_R));
    while(pstate_reg & (CMD_INHIBIT | CMD_INHIBIT_DAT)) {
	    pstate_reg = hal_read(sdhci->hal, (void *)(io_mem + PSTATE_REG_R));
        timeout_us--;
	    if (!timeout_us){
            break;
        }
        udelay(1);
	}

    if(pstate_reg & (CMD_INHIBIT | CMD_INHIBIT_DAT)) {
        clog_print(CLOG_ERR, "%s, %d, cmd line and data line busy, timeout!!!\n", __func__, __LINE__);
        return -1;
    }

    if (ret != 0) {
        return -1;
    }

    return 0;
}
/**
 * \brief sdhci_irq_handler
 *	interrupt service routine
 * \param task_id: task-id
 * \param data: pointer to sdhci object
 * \returns none
 */
int sdhci_irq_handler(uint8_t task_id, void *data)
{
	struct sdhci_t *sdhci = (struct sdhci_t *)data;
	addr_t io_mem = sdhci->io_base;
	uint32_t pstate_reg, intr_status;
	struct io_req_t *io_req;
	uint8_t req_complete = 0;

	clog_print(CLOG_LEVEL9, "=========== %s ==========\n", __func__);

	io_req = sdhci->act_req;
	intr_status = hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R));

	if (intr_status == 0 || intr_status == 0xffffffff) {
		clog_print(CLOG_LEVEL9, "no interrupt, intr_status = %x\n", intr_status);
	}

	sdhci->intr_status = intr_status;

	if (intr_status & SD_CARD_INTR_CHANGE) {
		pstate_reg = hal_read(sdhci->hal, (void *)(io_mem + PSTATE_REG_R));
		sdhci->card_present = !!(pstate_reg & CARD_INSERTED);
		clog_print(CLOG_INFO, "Isr: Card status changed, card %s pstate=%x\n", sdhci->card_present ?
				"inserted" : "removed", pstate_reg);
		/* TODO: whether to disable the card inserted/removal intr ?? */
		if (intr_status & CARD_INSERTION_SIGNAL_EN)
			hal_reg32_set_field(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_EN_R),
						CARD_INSERTION_SIGNAL_EN, 1, 0);
	}

	/* clear the interrupt status */
	if (intr_status) {
		clog_print(CLOG_LEVEL3, "Isr: %s, intr_status = %x\n", __func__, intr_status);
		clog_print(CLOG_LEVEL5, "Intr_status before clearing = %0x\n",
				hal_read(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R)));
		hal_write(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_R), intr_status);
	}

	if (intr_status & CARD_INTERRUPT_SIGNAL_EN) {
		clog_print(CLOG_INFO, "Disabling card interrupt\n");
		hal_reg32_set_field(sdhci->hal, (void *)(io_mem + NORMAL_INT_STAT_EN_R),
			8, 1, 0);
	}
	if (sdhci->act_req == NULL && intr_status) {
		clog_print(CLOG_LEVEL3, "%s: No Request queued\n", __func__);
		return 1;
	}

	if (intr_status)
		clog_print(CLOG_LEVEL3, "=========== %s intr_stat(%x)==========\n", __func__, intr_status);
	if (intr_status & SD_CMD_INTR_MASK)
		req_complete |= sdhci_handle_cmd_intr(sdhci, intr_status);

	if (intr_status & SD_DAT_INTR_MASK)
		req_complete |= sdhci_handle_data_intr(sdhci, intr_status);

	if (intr_status & SD_BUS_PWR_INTR_MASK) {
		clog_print(CLOG_ERR, "Isr: SD buspower, Current Limit interrupt, intr_status(%x)\n",
				intr_status);
	}

	if (intr_status & XFER_COMPLETE_SIGNAL_EN) {
		req_complete = 1;
		clog_print(CLOG_LEVEL5, "1.%s: Line %d:\n", __func__, __LINE__);
	} else
	if (!req_complete && (intr_status & SD_ERROR_MASK)) {
		req_complete = 1;
		io_req->error = intr_status & SD_ERROR_MASK;
		clog_print(CLOG_LEVEL5, "2.%s: Line %d:\n", __func__, __LINE__);
        clog_print(CLOG_ERR,"int error: 0x%x\n", intr_status);
        sdhci_handle_err_intr(sdhci, intr_status);
	}

	if (req_complete)
		sdhci_complete_request(sdhci);

	return 1;
}


void sdhci_irq_handler_for_csi(void *arg)
{
    sdhci_irq_handler(0, arg);
}

static void sdhic_phy_init(struct sdhci_t *sdhci)
{
    addr_t io_mem = sdhci->io_base;
    hal_write(sdhci->hal, (void *)(io_mem + DWC_MSHC_PTR_PHY_R), 0x1);
    hal_write8(sdhci->hal, (void *)(io_mem + PHY_SDCLKDL_CNFG_R), 0x1 << 4);
    hal_write8(sdhci->hal, (void *)(io_mem + PHY_SDCLKDL_DC_R), 0x40);

    uint32_t val = hal_read8(sdhci->hal, (void *)(io_mem + PHY_SDCLKDL_CNFG_R));
    val &= ~(1 << 4);
    hal_write8(sdhci->hal, (void *)(io_mem + PHY_SDCLKDL_CNFG_R), val);

    hal_write16(sdhci->hal, (void *)(io_mem + PHY_CMDPAD_CNFG_R), 0x1);
    hal_write16(sdhci->hal, (void *)(io_mem + PHY_DATAPAD_CNFG_R), 0x1);
    hal_write16(sdhci->hal, (void *)(io_mem + PHY_RSTNPAD_CNFG_R), 0x1);
    hal_write16(sdhci->hal, (void *)(io_mem + PHY_CLKPAD_CNFG_R), 0x1);
    hal_write16(sdhci->hal, (void *)(io_mem + PHY_STBPAD_CNFG_R), 0x1);
}

static void sdhci_auto_cmd_en(struct sdhci_t *sdhci)
{
    addr_t io_mem = sdhci->io_base;

    //hal_write16(sdhci->hal, (void *)(io_mem + HOST_CTRL2_R), 0x5c00);// (0x5 << 12) | (1 << 11));
    uint16_t val = hal_read16(sdhci->hal, (void *)(io_mem + HOST_CTRL2_R));
    val |= (0x5 << 12) | (1 << 11);
    hal_write16(sdhci->hal, (void *)(io_mem + HOST_CTRL2_R), val);
}

/**
 * \brief sdhci_init
 *	initialize the sdhci host controller
 * \param sdhci: pointer to sdhci object
 * \returns 0 on success
 */
int sdhci_init(struct sdhci_t *sdhci)
{
	struct sdhci_reg_t *reg;
	addr_t io_mem;

	if (sdhci == NULL)
		return ERROR_NOMEM;

	io_mem = sdhci->io_base;
	sdhci->reg = (struct sdhci_reg_t *)sdhci->io_base;
	sdhci->irq_handler = sdhci_irq_handler;
	sdhci->host.drv_data = sdhci;
	sdhci->host.ops.send_cmd = sdhci_send_cmd;
	sdhci->host.ops.enable_clock = sdhci_enable_clock;
	sdhci->host.ops.disable_clock = sdhci_disable_clock;
	sdhci->host.ops.set_data_width = sdhci_set_data_width;
	sdhci->host.ops.set_speed = sdhci_set_speed_mode;
	sdhci->host.ops.set_voltage = sdhci_set_voltage;
	sdhci->host.ops.io_ctrl = sdhci_io_ctrl;
	sdhci->is_emmc_dev = is_emmc_dev;

	sdhci->vendor1_offs = hal_read16(sdhci->hal, (void *)(io_mem + P_VENDOR_1_SPECIFIC_AREA));
	sdhci->vendor2_offs = hal_read16(sdhci->hal, (void *)(io_mem + P_VENDOR_2_SPECIFIC_AREA));
	sdhci->mmcm_clk = mmcm_clk_module_init(sdhci->hal,
		(void *)(sdhci->io_base + MMCM_REG_OFFS), "mmcm");
	if (sdhci->mmcm_clk == NULL)
		clog_print(CLOG_ERR, "Failed to initialize mmcm clk module\n");
	else
		clock_init(sdhci->mmcm_clk, NULL);

	sdhci->act_req = 0;

	/* set power */
	sdhci_io_ctrl(sdhci, SDHCI_CLOCK_OFF, 0);
	sdhci_io_ctrl(sdhci, SDHCI_POWER_OFF, 0);

	/* reset sdhci controller */
	sdhci_reset(sdhci, SW_RST_ALL | SW_RST_CMD | SW_RST_DAT);
	is_sd_card_inserted(sdhci);

	/* enable the power */
	sdhci_io_ctrl(sdhci, SDHCI_POWER_ON, XVDD1_3P3_VOLT);

    /* enable auto cmd*/
    sdhci_auto_cmd_en(sdhci);

    /* enable the phy*/
    sdhic_phy_init(sdhci);

	/* enable the clock 400Khz */
	sdhci_io_ctrl(sdhci, SDHCI_CLOCK_ON, FREQ_SEL_VAL);
	sdhci_io_ctrl(sdhci, SDHCI_SET_TXPHASE, tx_phase);

	/* set data time out for max val*/
	hal_write8(sdhci->hal, (void *)(io_mem + TOUT_CTRL_R), SDHCI_CMD_TIMEROUT);

	/* enable the interrupts */
	sdhci_disable_intr(sdhci);

	/* enable the interrupts */
	sdhci_enable_intr(sdhci);

	clog_print(CLOG_LEVEL5, "%s: sdhci(%x)\n",__func__, sdhci);

	reg = sdhci->reg;

	mem_dump(sdhci->hal, reg, sizeof(struct sdhci_reg_t), 4);
	return 0;
}

/**
 * \brief sdhci_get_base
 *	get base address of sdhci controller
 * \param instance: sdhci instance number
 * \returns returns the base address of sdhci
 */
addr_t sdhci_get_base(uint8_t instance)
{
    csi_dev_t dev;
    target_get(DEV_DW_SDMMC_TAG, instance, &dev);
    return dev.reg_base;
}

/**
 * \brief sdhci_module_init
 *	initialize sdhci module
 * \param none:
 * \returns 0 on success
 */
struct sdhci_t *sdhci_module_init(int idx)
{
	int ret;

    csi_dev_t dev;
    ret = target_get(DEV_DW_SDMMC_TAG, idx, &dev);
    if (ret != 0) {
        return NULL;
    }
    struct sdhci_t *sdhci = NULL;
	clog_print(CLOG_LEVEL5, "sdhci simulation\n");
	sdhci = sdhci_register(sdhci_get_base(idx), dev.irq_num);
	if (sdhci == NULL) {
		clog_print(CLOG_LEVEL5, "sdhci: unable to allocate sdhci instance-0\n");
		return NULL;
	}
    sdhci->dev = dev;
    sdhci->id = idx;

	clog_print(CLOG_LEVEL5, "%s: sdhci(%x)\n",__func__, sdhci);
	ret = sdhci_init(sdhci);
	if (ret < 0) {
		clog_print(CLOG_LEVEL5, "sdhci: failed to init-0\n");
		return NULL;
	}

#ifdef SDHIC_IRQ_MODE
    csi_irq_attach((uint32_t)(dev.irq_num), &sdhci_irq_handler_for_csi, &sdhci->dev);
    csi_irq_enable((uint32_t)(dev.irq_num));
#endif

	return sdhci;
}

void sdhci_module_uninit(struct sdhci_t *sdhci)
{
    #define wr(addr, val)  (*(volatile uint32_t *)(addr) = (val))
    sdhci_reset(sdhci, SW_RST_ALL | SW_RST_CMD | SW_RST_DAT);
    uint32_t offset = sdhci->id * 4;
    //soc reset sdhc, fix ip reset error
    wr(0xFFEF014060 + offset, 0);
    wr(0xFFEF014060 + offset, 1);
#ifdef SDHIC_IRQ_MODE
    csi_dev_t *dev = &sdhci->dev;
    csi_irq_detach((uint32_t)(dev->irq_num));
    csi_irq_disable((uint32_t)(dev->irq_num));
#endif
    hal_unregister(sdhci->hal);
    hal_unregister(sdhci->pal);
    sdhci_free(sdhci);
}
