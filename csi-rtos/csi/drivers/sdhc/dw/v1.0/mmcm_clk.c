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
* \file		: mmcm_clk.c
* \author	: ravibabu@synopsys.com
* \date		: 11-Jan-2020
* \brief	: mmcm_clk.c source
* Revision history:
* Ver	Date		Author       	  	Change Id	Description
* 0.1	2019-08-12	ravibabui@synopsys.com  001		dev in progress
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "emmc_ctype.h"
#include "clog.h"
#include "clkcore.h"
#include "mmcm_clk.h"

/**
 * \brief mmcm_clk_init
 * 	initialize mmcm clokc init
 * \param mmcm: pointer to mmcm clock module
 * \param data: driver private data
 * \returns 0
 */
int mmcm_clk_init(struct clk_mod_t *mmcm, void *data)
{
	/* add mmcm module initialization */
	return 0;
}

/**
 * \brief mmcm_clk_deinit
 * 	deinitialize mmcm clokc init
 * \param mmcm: pointer to mmcm clock module
 * \returns 0
 */
int mmcm_clk_deinit(struct clk_mod_t *mmcm)
{
	/* add mmcm module de-initialization */
	return 0;
}

/**
 * \brief mmcm_clk_enable
 * 	enable the mmcm clock
 * \param mmcm: pointer to mmcm clock module
 * \param clk_hz: mmcm-clock in hz 
 * returns 0 on success, error -ve on fail
 */
int mmcm_clk_enable(struct clk_mod_t *mmcm, uint32_t clk_hz)
{
	uint32_t clk_reg_val, frac_reg_val;
	uint32_t *io_mem = mmcm->io_mem;

	/* make sure mmcm is put into reset mode before configure mmcm */

	clog_print(CLOG_LEVEL5, "%s: clk_hz: %d\n", __func__, clk_hz);

	frac_reg_val = 0;
	switch (clk_hz) {
	case MMCM_CLK_200MHZ: /* 200 Mhz */
		clk_reg_val = CLK_HIGH_TIME_200MHZ | CLK_LOW_TIME_200MHZ;
		frac_reg_val = MMCM_FRAC_ENABLE;
		break;
	case MMCM_CLK_100MHZ: /* 100 Mhz */
		clk_reg_val = CLK_HIGH_TIME_100MHZ | CLK_LOW_TIME_100MHZ;
		break;
	case MMCM_CLK_125MHZ: /* 125 Mhz */
		clk_reg_val = CLK_HIGH_TIME_125MHZ | CLK_LOW_TIME_125MHZ;
		break;
	case MMCM_CLK_143MHZ: /* 143 Mhz */
		clk_reg_val = CLK_HIGH_TIME_143MHZ | CLK_LOW_TIME_143MHZ;
		frac_reg_val = MMCM_FRAC_ENABLE;
		break;
	case MMCM_CLK_166MHZ: /* 166 Mhz */
		clk_reg_val = CLK_HIGH_TIME_166MHZ | CLK_LOW_TIME_166MHZ;
		break;
	default:
		return ERR_INVARG;
	}

	/* configure clock high and low vco cycle time */
	hal_write(mmcm->hal, (void *)(io_mem + MMCM_CLK_LEVEL_REG_OFFS),
		clk_reg_val | MMCM_CLKOUT0_BIT12_RESVD);

	/* enable fraction */
	hal_write(mmcm->hal, (void *)(io_mem + MMCM_FRAC_REG_OFFS), frac_reg_val);

	/* make sure mmcm is bring out of reset to enable mmcm */
	return 0;
}

/**
 * \brief mmcm_clk_disable
 * 	disable the mmcm clock
 * \param name: clink_mod name
 * \param clk_hz: mmcm-clock in hz 
 * returns 0
 */
int mmcm_clk_disable(struct clk_mod_t *mmcm, uint32_t clk_hz)
{
	/* add mmcm clk disable code */
	return 0;
}

/**
 * \brief mmcm_clk_module_init
 * 	initialize mmcm_clk_module_init
 * \param hal: pointer to hal object
 * \param io_mem: io-address of mmcm module
 * \param name: name of mmcm clock module
 * returns: pointer to new clink_mod structure, NULL on fail to create.
 */
struct clk_mod_t *mmcm_clk_module_init(struct hal_obj_t *hal, void *io_mem, char *name)
{
	struct clk_ops_t ops;

	/* initialize clock ops */
	ops.init = mmcm_clk_init;
	ops.enable = mmcm_clk_enable;
	ops.disable = mmcm_clk_disable;
	ops.deinit = mmcm_clk_deinit;
	ops.priv_data = NULL;

	return register_clk_module(name, hal, io_mem, &ops);
}
