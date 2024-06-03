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
* \file		: sd_card.h
* \author	: ravibabu@synopsys.com
* \date		: 10-Dec-2019
* \brief	: sd_card.h header
* Revision history:
* Ver	Date		Author       	  	Change Id	Description
* 0.1	10-Dec-2019	ravibabui@synopsys.com  001		dev in progress
*/

#ifndef _MMC_CARD_H_
#define _MMC_CARD_H_

#include "emmc_ctype.h"
#include "emmc_error.h"

/**
 * \brief card_ocr_t
 *	card ocr structure definition
 */
union card_ocr_t {
	uint32_t word;
	struct {
		uint32_t res1:7;
		uint32_t high_dual_volt:7;
		uint32_t volt20_to_26:7;
		uint32_t volt27_to_36:9;
		uint32_t res2:5;
		uint32_t access_mode:2;
		uint32_t busy:1;
	}bit;
};

/**
 * \brief card_cid_t
 *	card cid structure definition
 */
union card_cid_t {
	uint32_t word[4];
	struct {
		uint32_t w0;
		uint32_t w1;
		uint32_t w2;
		uint32_t w3;
	}bit;
};

/**
 * \brief card_csd_t
 *	sd device structure definition
 */
union card_csd_t {
	uint32_t word[4];
	struct {
		uint32_t w0;
		uint32_t w1;
		uint32_t w2;
		uint32_t w3;
	}bit;
};

/**
 * \brief card_scr_t
 *	 card scr structure definition
 */
union card_scr_t {
	uint32_t word[2];
	struct {
		uint32_t w0;
		uint32_t w1;
	}bit;
};

/**
 * \brief card_ext_csd_t
 *	card extended csd structure definition
 */
union card_ext_csd_t {
	uint32_t word[128];
	struct {
		uint32_t w0[32];	/* byte index 0 to 127 */

#define EXTCSD_IDX177_BUS_CONDITIONS 177
#define EXTCSD_IDX179_BOOT_PARTTION	179
	#define	EMMC_BOOT_ACK_EN_6BIT       (1 << 6)
	#define	EMMC_BOOT_PARTITION_EN_ADDR 3
	    #define	EMMC_BOOT_PARTITION_1       1
	    #define	EMMC_BOOT_PARTITION_2       2
	    #define	EMMC_BOOT_USER_AREA_EN      7
	#define	EMMC_BOOT_PARTITION_ACCESS  0
	    #define	EMMC_BOOT_RW_PARTITION1     1
	    #define	EMMC_BOOT_RW_PARTITION2     2
	    #define	EMMC_BOOT_RW_RPMB           3
	    #define	EMMC_BOOT_ACCESS_GENERAL_PURPOSE_PARTITION1 4
	    #define	EMMC_BOOT_ACCESS_GENERAL_PURPOSE_PARTITION2 5
	    #define	EMMC_BOOT_ACCESS_GENERAL_PURPOSE_PARTITION3 6
	    #define	EMMC_BOOT_ACCESS_GENERAL_PURPOSE_PARTITION4 7
#define EXTCSD_IDX183_BUSWIDTH		183
	#define	EMMC_SET_BUSWIDTH_1BIT		0
	#define	EMMC_SET_BUSWIDTH_4BIT		1
	#define	EMMC_SET_BUSWIDTH_8BIT		2
	#define	EMMC_SET_BUSWIDTH_4BIT_DDR	5
	#define	EMMC_SET_BUSWIDTH_8BIT_DDR	6
#define EXTCSD_IDX184			184
#define EXTCSD_IDX184_HS_TIMING		185
	#define EMMC_SET_DEFAULT	0
	#define	EMMC_SET_HIGH_SPEED	1
	#define	EMMC_SET_HS200		2
	#define	EMMC_SET_HS400		3
		uint32_t w1[32];	/* byte idnex 128 to 255 */
		uint32_t w2[32];	/* byte index 256 to 384 */
		uint32_t w3[32];	/* byte index 384 to 511 */
#define EXTCSD_IDX226_BOOT_BLOCK_CNT_ADDR  226
#define EXTCSD_IDX504_SUPPORTED_CMDSET	504
	#define	S_CMD_SET0	0	/* Standard MMC cmd */
	#define	S_CMD_SET1	1	/* Allocated by MMCA */
	#define	S_CMD_SET2	2	/* Allocated by MMCA */
	#define	S_CMD_SET3	3	/* Allocated by MMCA */
	#define	S_CMD_SET4	4	/* Allocated by MMCA */
#define EXTCSD_IDX505			505
	}bit;
};

/**
 * \brief card_status_t
 *	sd card status
 */
union card_status_t {
	uint32_t word[16];
	struct {
		uint32_t w0;
		uint32_t w1;
		uint32_t w2;
		uint32_t w3;
		uint32_t w4;
		uint32_t w5;
		uint32_t w6;
		uint32_t w7;
		uint32_t w8;
		uint32_t w9;
		uint32_t w10;
		uint32_t w11;
		uint32_t w12;
		uint32_t w13;
		uint32_t w14;
		uint32_t w15;
	}bit;
};

/**
 * \brief acmd41_resp_t
 *	acmd41 resonse structure
 */
struct acmd41_resp_t {
	uint32_t status;
};

/**
 * \brief sd_card_t
 *	mmc card data structure
 */
struct sd_card_t {
	uint8_t			in_use; /**< card is allocated or not */
	uint8_t 		state; /**< card state, idle, trans, etc */
	uint16_t		rca; /**< 16bit relative device address RCA */
	struct acmd41_resp_t	acmd41_resp; /**< acmd41 response */
	uint8_t			init_complete; /**< card init complete */
	uint8_t			is_sdxc_card; /**< sd card support extended capacity */
	uint8_t			is_ush2_card; /**< whether sdcard is uhs2 card */
	uint8_t			s18a_volt_switch_ready; /**< s18a volt ready to switch */

	union card_ocr_t 	ocr; /**< ocr: */
	union card_cid_t 	cid; /**< Device Idnetification register 128 bit wide */
	union card_csd_t 	csd; /**< Device specific data 128 bit wide */
	union card_ext_csd_t 	ext_csd; /**< Extended Device specific data 512i bytes */
	union card_scr_t	scr; /**< scr value */
	uint16_t		dsr; /**< 16-bit driver stage register */
	uint32_t		qsr; /**< 32 bit queue status register */
	union card_status_t	status; /**< card status */
	union card_status_t	cmd6_query_status; /**< card cmd6 querey status */

	uint8_t 		fn_state; /**< internal fn-state */
	void *mmc_dev;		/**< ponter ot mmc-dev */
};

/** \brief: mmc_dev_enumerate
 * 	enumerate the device and initialize to known state
 * \param mmc_dev: pointer to mmc_dev
 * \param mmc_cmd: pointer to mmc_cmd
 * \param speed_mode: speed-mode
 * \param bus_width : sd bus width (4/8 bit)
 * \param xfer_mode : pio/sdma/adma mode (only pio supported)
 * \param emmc_vdd : vdd 1.8V
 * \param mmcm_clk : mmcm clock in Hz
 * \param usr_intf : wait for user interface cmd for read/write blocks & tuning
 * \returns
 */
int mmc_dev_enumerate(struct sd_card_t *sd_card, struct cmd_param_t *mmc_cmd,
	uint8_t speed_mode, uint8_t bus_width, uint8_t xfer_mode,
	uint8_t emmc_vdd, uint32_t mmcm_clock, uint8_t usr_intf);

/** \brief: emmc_dev_enumerate
 * 	enumerate the emmc device and initialize to known state
 * \param mmc_dev: pointer to mmc_dev
 * \param mmc_cmd: pointer to mmc_cmd
 * \param speed_mode: speed-mode
 * \param bus_width : sd bus width (4/8 bit)
 * \param xfer_mode : pio/sdma/adma mode (only pio supported)
 * \param emmc_vdd : vdd 1.8V
 * \param mmcm_clk : mmcm clock in Hz
 * \param usr_intf : wait for user interface cmd for read/write blocks & tuning
 * \returns
 */
int emmc_dev_enumerate(struct sd_card_t *sd_card, struct cmd_param_t *mmc_cmd,
	uint8_t speed_mode, uint8_t bus_width, uint8_t xfer_mode,
	uint8_t emmc_vdd, uint32_t mmcm_clock, uint8_t usr_intf);

/** \brief: sd_card_module_init
 * 	initialize the mmc-card module
 * \param none
 * \returns none
 */
void sd_card_module_init(void);

/** \brief: buffer_fill
 * 	fill buffer with required pattern type
 * \param buffer: buffer pointer
 * \param len : buffer length
 * \param pattern : pattern to be filled
 * \param type : 0-user defined pattern, 1-random pattern
 * \returns none
 */
void buffer_fill(void *databuf, uint32_t len, uint32_t pattern, uint8_t type);

/** \brief: sd_card_alloc
 * 	allocate mmc cards from free pool
 * \param flags : memory allocation flags
 * \returns none
 */
struct sd_card_t *sd_card_alloc(uint8_t flags);
#endif /* _MMC_CARD_H_ */

