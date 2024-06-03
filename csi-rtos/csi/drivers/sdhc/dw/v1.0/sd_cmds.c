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
* \file		: sd_cmds.c
* \author	: ravibabu@synopsys.com
* \date		: 01-Dec-2019
* \brief	: sd_cmds.c source
* Revision history:
* Ver	Date		Author       	  	Change Id	Description
* 0.1	01-Dec-2019	ravibabui@synopsys.com  001		dev in progress
*/


#include <stdio.h>
#include <string.h>
#include "emmc_common.h"
#include "emmc_error.h"
#include "clog.h"
#include "mmc_dev.h"
#include "sd_cmd.h"
#include "mmc_core.h"
#include "mshc_regs.h"
#include "sd_cmds.h"
#include "sdhci.h"

#include "drv/porting.h"

/* globals */
extern uint8_t rx_phase;

void cmd_req_reinit(struct cmd_param_t *cmd_req)
{
    memset(cmd_req, 0, sizeof(struct cmd_param_t));
    cmd_req->state = XSTATE_CMD_INIT;
}

/** \brief: mmc_cmd_reinit
 *  re-init mmc-cmd to init state
 * \param mmc_cmd: pointer to mmc_cmd-param
 * \returns none
 */
void mmc_cmd_reinit(struct cmd_param_t *mmc_cmd)
{
    cmd_req_reinit(mmc_cmd);
}

/** \brief: mmc_cmd_submit_wait_for_completion
 *	submit mmc-cmd and wait for io-completion
 * \param mmc_dev: pointer to mmc_dev obj
 * \param mmc_cmds: pointer to mmc-cmd-param
 * \returns 0 cmd completion, other timeout
 */
extern int mmc_release_io_req(void *priv_data, struct io_req_t *io_req);
int mmc_cmd_submit_wait_for_completion(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds)
{
    int ret = mmc_prepare_io_req(mmc_dev, &mmc_cmds->io_req, &mmc_cmds->cur_cmd, sizeof(struct cmd_param_s));
    if (ret != SUCESS) {
		clog_print(CLOG_WARN, "%s, %d, cmd param error: %d\n", __func__, __LINE__, ret);
        return -1;
    }
    mmc_cmds->io_req.error = 0;
    sdhci_send_cmd(mmc_dev->host->drv_data, &mmc_cmds->io_req);
    while(mmc_cmds->io_req.complete == 0){};

	struct cmd_param_s *cur_cmd = &mmc_cmds->cur_cmd;
    struct mmc_req_t *mmc_req = mmc_cmds->io_req.req;

    memcpy(&cur_cmd->resp[0], &mmc_req->resp[0], 4 * sizeof(uint32_t));

    if (mmc_cmds->io_req.error != 0) {
        return -1;
    }

    mmc_release_io_req(NULL, &mmc_cmds->io_req);

    return 0;
}

/** \brief: mmc_cmds_go_idle
 *	The command GO_IDLE_STATE (CMD0) is the software reset command and sets each
 * card into Idle State regardless of the current card state. Cards in Inactive State
 * are not affected by this command. After power-on by the host, all cards are in Idle
 * State, including the cards that have been in Inactive State before.
 *	After power-on or CMD0, all cards’ CMD lines are in input mode, waiting for
 * start bit of the next command. The cards are initialized with a default relative card
 * address (RCA=0x0000) and with a default driver stage register setting (lowest speed,
 * highest driving current capability).
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param boot-mode: cmd0 boot-mode argument
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd0_card_set_idle(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds, uint32_t boot_mode)
{
	struct cmd0_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		param = (struct cmd0_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->boot_mode = boot_mode;

		mmc_cmds->cur_cmd.cmd = MMC_CMD0_GO_IDLE;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd0_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd1_emmc_send_op_cond
 *	prepare for cmd1 emmc send_operating condition
 *	CMD1 is special synchronization command used to negotiate the operation
 * volatage range and to poll the device until it is out of its power up sequence.
 * In addition to operation voltage profile of the emmc device, the response to CMD1
 * contains busy flag indicating that the device is still working on it spower up
 * procedure and is not ready for identification. This bit informs the host that the
 * device is not ready, and the host must wait until this bit is cleared.
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param args: cmd specific argument
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd1_emmc_send_op_cond(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds, uint32_t args)
{
	struct cmd1_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		param = (struct cmd1_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->args = args;

		mmc_cmds->cur_cmd.cmd = EMMC_CMD1_SEND_OP_COND;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd1_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd3_send_rel_adr
 *	prepare for cmd3 send relative address
 *	ask the card to publish the new relativ address
 *	device respond with response type r6 has following format
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param boot-mode: cmd0 boot-mode argument
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd3_send_rel_adr(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds, uint16_t rca)
{
	struct cmd3_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		param = (struct cmd3_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->rca = rca;

		mmc_cmds->cur_cmd.cmd = MMC_CMD3_SEND_REL_ADR;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd3_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd11_voltage_switch
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param args: cmd specific argument
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd11_voltage_switch(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds, uint32_t args)
{
	struct cmd11_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		param = (struct cmd11_param_t *)&mmc_cmds->cur_cmd.param[0];

		param->args = args;
		mmc_cmds->cur_cmd.cmd = MMC_CMD11_VOLT_SWITCH;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd11_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd2_get_cid
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param args: cmd specific argument
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd2_get_card_cid(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds, uint32_t args)
{
	struct cmd2_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		param = (struct cmd2_param_t *)&mmc_cmds->cur_cmd.param[0];

		param->args = args;
		mmc_cmds->cur_cmd.cmd = MMC_CMD2_ALL_SEND_CID;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd2_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd7_card_select
 *	prepare for cmd7 for card select/deselect
 *	host resend the CMD3 to change its RCA number
 *	other than 0 and then use CMD7 with RCA=0 for
 *	card de-selection
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param args: cmd specific argument
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd7_card_select(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds, uint16_t rca)
{
	struct cmd7_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		param = (struct cmd7_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->rca = rca;

		mmc_cmds->cur_cmd.cmd = MMC_CMD7_CARD_SELECT;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd7_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd9_get_csd
 *	prepare for cmd9 for send card's csd data
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param args: cmd specific argument
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd9_get_csd(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds, uint16_t rca)
{
	struct cmd9_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		param = (struct cmd9_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->rca = rca;

		mmc_cmds->cur_cmd.cmd = MMC_CMD9_SEND_CSD;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd9_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd13_send_status
 *	cmd13, request card to send 64-byte(512-bits) status
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param card_status_buf: status buffer
 * \param buflen : buffer length
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd13_send_status(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
		void *card_status_buf, uint32_t buflen)
{
	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */

		mmc_cmds->cur_cmd.databuf = card_status_buf;
		mmc_cmds->cur_cmd.datalen = buflen;

		mmc_cmds->cur_cmd.cmd = MMC_CMD13_SEND_STATUS;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd13_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd6_switch_function
 *		prepare for cmd6, switch function
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param speed_mode: mmc/sd speed_mode
 * \param databuf: data buffer
 * \param buflen : buffer length
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd6_switch_function(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
		uint8_t speed_mode, void *databuf, uint32_t buflen)
{
	struct cmd6_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		param = (struct cmd6_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->is_acmd = 0;
		param->speed_mode = speed_mode;

		mmc_cmds->cur_cmd.databuf = databuf;
		mmc_cmds->cur_cmd.datalen = buflen;

		mmc_cmds->cur_cmd.cmd = MMC_CMD6_SWITCH_FN;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd6_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_acmd6_switch_function
 *		prepare for cmd6, switch function
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param bus_width: mmc/sd bus-width
 * \returns succes/fail on cmd command completion
 */
int mmc_acmd6_switch_function(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
		uint8_t bus_width)
{
	struct acmd6_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		param = (struct acmd6_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->is_acmd = 1;
		param->bus_width = bus_width;

		mmc_cmds->cur_cmd.cmd = MMC_CMD6_SWITCH_FN;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd6_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd6_set_ext_csd
 *		prepare for cmd6, set_ext_csd
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param access_mode: cmd access-mode
 * \param index: index to csd data byte
 * \param value: value need to set
 * \param cmd_set: cmd operation
 * \returns succes/fail on cmd command completion
 */
int emmc_cmd6_set_ext_csd(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
		uint8_t access_mode, uint8_t index, uint8_t value, uint8_t cmd_set)
{
	struct cmd6_emmc_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		param = (struct cmd6_emmc_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->access_mode = access_mode;
		param->index = index;
		param->value = value;
		param->cmd_set = cmd_set;

		mmc_cmds->cur_cmd.cmd = EMMC_CMD6_SET_EXT_CSD;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd6_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_read_one_block
 *	cmd17 - read single block
 *	cmd18 - read multiple block
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param block_addr: device block address
 * \param databuf: data buffer
 * \param buflen : buffer length
 * \returns succes/fail on cmd command completion
 */
int mmc_generic_read_block(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
		uint32_t block_addr, uint32_t block_cnt, uint8_t  *databuf, uint32_t buflen)
{
	struct cmd17_param_t *cmd17_param;
	struct cmd18_param_t *cmd18_param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */

		if (buflen < SD_BLOCK_SZ)
			return ERROR_INVARG;

		if (block_cnt > 1) {
			cmd18_param = (struct cmd18_param_t *)&mmc_cmds->cur_cmd.param[0];
			cmd18_param->block_addr = block_addr;
			cmd18_param->nblocks = block_cnt;
			mmc_cmds->cur_cmd.cmd = MMC_CMD18_READ_MULTI_BLOCK;
		} else {
			cmd17_param = (struct cmd17_param_t *)&mmc_cmds->cur_cmd.param[0];
			cmd17_param->block_addr = block_addr;
			mmc_cmds->cur_cmd.cmd = MMC_CMD17_READ_ONE_BLOCK;
		}

		mmc_cmds->cur_cmd.databuf = databuf;
		mmc_cmds->cur_cmd.datalen = buflen;

		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd17_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_set_block_cnt
 *		cmd23, set bus block_cnt
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param block_cnt: number of blocks
 * \returns succes/fail on cmd command completion
 */
int mmc_set_block_cnt(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
		uint32_t block_cnt)
{
	struct cmd23_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		param = (struct cmd23_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->block_cnt = block_cnt;

		mmc_cmds->cur_cmd.cmd = MMC_CMD23_SET_BLOCKCNT;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd6_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_write_one_block
 *	cmd24 - write single block
 *	cmd25 - write multiple block
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param block_addr: device block address
 * \param block_cnt: number of blocks
 * \param databuf: data buffer
 * \param buflen : buffer length
 * \returns succes/fail on cmd command completion
 */
int mmc_generic_write_block(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
	uint32_t block_addr, uint32_t block_cnt, uint8_t  *databuf, uint32_t buflen)
{
	struct cmd24_param_t *cmd24_param;
	struct cmd25_param_t *cmd25_param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		if (buflen < SD_BLOCK_SZ)
			return ERROR_INVARG;

		if (block_cnt > 1) {
			cmd25_param = (struct cmd25_param_t *)&mmc_cmds->cur_cmd.param[0];
			cmd25_param->block_addr = block_addr;
			cmd25_param->nblocks = block_cnt;
			mmc_cmds->cur_cmd.cmd = MMC_CMD25_WRITE_MULTI_BLOCK;
		} else {
			cmd24_param = (struct cmd24_param_t *)&mmc_cmds->cur_cmd.param[0];
			cmd24_param->block_addr = block_addr;
			mmc_cmds->cur_cmd.cmd = MMC_CMD24_WRITE_ONE_BLOCK;
		}

		mmc_cmds->cur_cmd.databuf = databuf;
		mmc_cmds->cur_cmd.datalen = buflen;

		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd24_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd19_tuning_block
 *	cmd19 read tuning block
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param databuf: data buffer
 * \param buflen : buffer length
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd19_read_tuning_block(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
		uint8_t  *databuf, uint32_t buflen)
{
	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		if (buflen < SD_TUNING_BLOCK_SZ)
			return ERROR_INVARG;

		mmc_cmds->cur_cmd.cmd = MMC_CMD19_SEND_TUNING_BLOCK;

		mmc_cmds->cur_cmd.databuf = databuf;
		mmc_cmds->cur_cmd.datalen = buflen;

		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd21_read_tuning_block
 *	cmd21 read tuning block
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param bus_width: mmc/sd bus width
 * \param databuf: data buffer
 * \param buflen : buffer length
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd21_read_tuning_block(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
		uint8_t bus_width, uint8_t  *databuf, uint32_t buflen)
{
	struct cmd21_param_t *cmd21_param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */

		if (buflen < EMMC_TUNING_BLOCK_SZ)
			return ERROR_INVARG;

		cmd21_param = (struct cmd21_param_t *)&mmc_cmds->cur_cmd.param[0];
		cmd21_param->bus_width = bus_width;
		clog_print(CLOG_LEVEL5, "%s. bus-width = %d\n", __func__, cmd21_param->bus_width);

		mmc_cmds->cur_cmd.cmd = MMC_CMD21_SEND_TUNING_BLOCK;

		mmc_cmds->cur_cmd.databuf = databuf;
		mmc_cmds->cur_cmd.datalen = buflen;

		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd21_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd8_check_card_intf_condition
 * 	The command GO_send_if_cond (CMD8) to check whether card support current voltage
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param volt_supplied: volt supply
 * \param check_pattern: check pattern
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd8_check_card_intf_condition(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
		uint8_t volt_supplied, uint8_t check_pattern)
{
	struct cmd8_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		mmc_cmds->cur_cmd.cmd = MMC_CMD8_SEND_IF_COND;
		param = (struct cmd8_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->volt_supplied = volt_supplied;
		param->check_pattern = check_pattern;

		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd8_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: emmc_cmd8_get_ext_csd
 * 	The command SEND_EXT_CSD (CMD8) issued to emmc device to get ext-csd data
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param databuf: data buffer
 * \param buflen : buffer length
 * \returns succes/fail on cmd command completion
 */
int emmc_cmd8_get_ext_csd(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
			uint8_t  *databuf, uint32_t buflen, uint32_t args)
{
	struct cmd8_emmc_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		mmc_cmds->cur_cmd.cmd = EMMC_CMD8_SEND_EXT_CSD;
		param = (struct cmd8_emmc_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->args = args;

		mmc_cmds->cur_cmd.databuf = databuf;
		mmc_cmds->cur_cmd.datalen = buflen;
		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd8_emmc_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_cmd55_next is application cmd
 * 	The command send to inform card that next command is acmd
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param rca: relative card address
 * \returns succes/fail on cmd command completion
 */
int mmc_cmd55_next_is_acmd(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
		uint16_t rca)
{
	struct cmd55_param_t *param;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	if (mmc_cmds->state == XSTATE_CMD_INIT) {	/* initialize param */
		mmc_cmds->cur_cmd.cmd = MMC_CMD55_NXT_IS_ACMD;
		param = (struct cmd55_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->rca = rca;

		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd8_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;
	}

	return mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
}

/** \brief: mmc_acmd41_send_op_cond
 * 	The command GO_send_if_cond (CMD8) to check whether card support current voltage
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param is_sdxc_support: enable card extended capacity support
 * \param iuse_1p8v: use low power 1.8V
 * \param is_max_perf : configure for card max performance
 * \param is_ocr : ocr support
 * \returns succes/fail on cmd command completion
 */
int mmc_acmd41_check_card_op_condition(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
		uint8_t is_sdxc_support, uint8_t use_1p8v, uint8_t is_max_perf, uint8_t is_ocr)
{
	struct cmd41_param_t *param;
	int retval = 0;
	static uint8_t state = XSTATE0;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	switch(state) {
	case XSTATE0:	/* initialize param */
		retval = mmc_cmd55_next_is_acmd(mmc_dev, mmc_cmds, mmc_dev->card->rca);
        if (retval != 0) {
			state = XSTATE0;
        } else {
		    /* goto next command */
			mmc_cmd_reinit(mmc_cmds);
			state = XSTATE1;
		}
		break;
	case XSTATE1:
		mmc_cmds->cur_cmd.cmd = MMC_ACMD41_SD_SEND_OP_COND;
		param = (struct cmd41_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->is_hcs_sdxc_support = is_sdxc_support;
		param->s18r_use_18v = use_1p8v;
		param->is_max_perf = is_max_perf;
		param->is_ocr =	is_ocr;

		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd8_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;

		state = XSTATE2;
		//break; avoid break purposefully, fix for coverity

	case XSTATE2:
		retval = mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);
		if (retval == 0)
			state = XSTATE0;
		//break; avoid break purposefully, fix for coverity
	}
	return retval;
}

/** \brief: mmc_acmd51_get_card_scr
 * 	The application specific command ACMD51 to get card SCR
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param databuf: data buffer
 * \param buflen : buffer length
 * \returns succes/fail on cmd command completion
 */
int mmc_acmd51_get_card_scr(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
	void *databuf, uint32_t buflen)
{
	struct cmd51_param_t *param;
	int retval = 0;
	struct sd_card_t *card;
	static uint8_t state = XSTATE0;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	card = mmc_dev->card;
	switch(state) {
	case XSTATE0:	/* initialize param */
	    mmc_cmd_reinit(mmc_cmds);
		retval = mmc_cmd55_next_is_acmd(mmc_dev, mmc_cmds, card->rca);
		if (retval != 0) {
			state = XSTATE0;
		    break;
        } else {
		    /* goto next command */
			state = XSTATE1;
		}
	    mmc_cmd_reinit(mmc_cmds);
	case XSTATE1:
		mmc_cmds->cur_cmd.cmd = MMC_CMD51_SEND_SCR;
		param = (struct cmd51_param_t *)&mmc_cmds->cur_cmd.param[0];
		param->is_acmd = 1;

		mmc_cmds->cur_cmd.databuf = databuf;
		mmc_cmds->cur_cmd.datalen = buflen;

		mmc_cmds->cur_cmd.is_valid = 1;
		mmc_cmds->cur_cmd.param_len = sizeof(struct cmd8_param_t);
		mmc_cmds->state = XSTATE_CMD_SUBMIT;

		state = XSTATE2;
		//break; avoid break purposefully, fix for coverity

	case XSTATE2:
		retval = mmc_cmd_submit_wait_for_completion(mmc_dev, mmc_cmds);

		if (retval == 0)
			state = XSTATE0;
		//break; avoid break purposefully, fix for coverity
	}
	return retval;
}

/** \brief: mmc_acmd13_get_status
 * 	The application specific command ACMD13 to get card status
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param databuf: data buffer
 * \param buflen : buffer length
 * \returns succes/fail on cmd command completion
 */
int mmc_acmd13_get_status(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
	void *databuf, uint32_t buflen)
{
	int retval = 0;
	struct sd_card_t *card;
	static uint8_t state = XSTATE0;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	card = mmc_dev->card;
	switch(state) {
	case XSTATE0:	/* initialize param */
		retval = mmc_cmd55_next_is_acmd(mmc_dev, mmc_cmds, card->rca);
        if (retval != 0) {
			state = XSTATE0;
		    break;
        } else {
		    /* goto next command */
			mmc_cmd_reinit(mmc_cmds);
			state = XSTATE1;
		}
	case XSTATE1:
		retval = mmc_cmd13_send_status(mmc_dev, mmc_cmds, databuf, buflen);
        if (retval == 0)
			state = XSTATE0;
		break;
	}
	return retval;
}

/** \brief: mmc_acmd6_set_buswidth
 * 	The application specific command ACMD6 to set bus-width
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param bus_width: 4 or 8 bit bus-width
 * \returns succes/fail on cmd command completion
 */
int mmc_acmd6_set_buswidth(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
		uint8_t bus_width)
{
	int retval = 0;
	struct sd_card_t *card;
	static uint8_t state = XSTATE0;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	card = mmc_dev->card;

	switch(state) {
	case XSTATE0:	/* initialize param */
		retval = mmc_cmd55_next_is_acmd(mmc_dev, mmc_cmds, card->rca);
        if (retval != 0) {
			state = XSTATE0;
            break;
        } else {
		    /* goto next command */
			mmc_cmd_reinit(mmc_cmds);
			state = XSTATE1;
		}
	case XSTATE1:
		retval = mmc_acmd6_switch_function(mmc_dev, mmc_cmds, bus_width);
        if (retval == 0)
			state = XSTATE0;

		break;
	}
	return retval;
}

/** \brief: mmc_read_block
 * 	 API to read blocks from sd-card
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param block_addr: sd block address
 * \param block_cnt: nubmer of blocks to read
 * \param databuf: data buffer
 * \param buflen : buffer length
 * \returns succes/fail on cmd command completion
 */
int mmc_read_block(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
	uint32_t block_addr, uint32_t block_cnt, uint8_t  *databuf, uint32_t buflen)
{
	int retval = 0;
	static uint8_t state = XSTATE0;

	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	switch(state) {
	case XSTATE0:	/* initialize param */
            /*
		if (block_cnt > 1) {
			retval = mmc_set_block_cnt(mmc_dev, mmc_cmds, block_cnt);
			if (retval == 0) {
		        mmc_cmd_reinit(mmc_cmds);
			    state = XSTATE1;
			} else {
				break;
            }
		} else {
			state = XSTATE1;
        }
        */
		//break; avoid break purposefully, fix for coverity
        mmc_cmd_reinit(mmc_cmds);
	    state = XSTATE1;
	case XSTATE1:
		retval = mmc_generic_read_block(mmc_dev, mmc_cmds, block_addr, block_cnt,
				databuf, buflen);
		if (retval == 0)
			state = XSTATE0;
		break;
	}

	return retval;
}

/** \brief: mmc_write_block
 * 	 API to write blocks from sd-card
 *
 * \param mmc_dev: pointer to mmc_dev object
 * \param mmc_cmds: pointer to mmc-cmds
 * \param block_addr: sd block address
 * \param block_cnt: nubmer of blocks to write
 * \param databuf: data buffer
 * \param buflen : buffer length
 * \returns succes/fail on cmd command completion
 */
int mmc_write_block(struct mmc_dev_t *mmc_dev, struct cmd_param_t *mmc_cmds,
	uint32_t block_addr, uint32_t block_cnt, uint8_t  *databuf, uint32_t buflen)
{
	int retval = 0;
	static uint8_t state = XSTATE0;


	if (mmc_cmds == NULL || mmc_dev == NULL)
		return ERROR_INVARG;

	switch(state) {
	case XSTATE0:	/* initialize param */
	//	if (block_cnt > 1) {
	//		retval = mmc_set_block_cnt(mmc_dev, mmc_cmds, block_cnt);
	//		if (retval == 0) {
	//		    mmc_cmd_reinit(mmc_cmds);
	//		    state = XSTATE1;
	//		} else {
	//			break;
    //        }
	//	} else {
	//		state = XSTATE1;
    //    }
		//break; avoid break purposefully, fix for coverity
	    mmc_cmd_reinit(mmc_cmds);
		state = XSTATE1;
	case XSTATE1:
		retval = mmc_generic_write_block(mmc_dev, mmc_cmds, block_addr, block_cnt,
				databuf, buflen);
		if (retval == 0)
			state = XSTATE0;

		break;
	}

	return retval;
}
