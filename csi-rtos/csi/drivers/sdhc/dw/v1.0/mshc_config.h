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
* \file		: mshc_config.h
* \author	: ravibabu@synopsys.com
* \date		: 12-Aug-2019
* \brief	: mshc driver config file
* Revision history:
* Ver	Date		Author       	  	Change Id	Description
* 0.1	2019-08-12	ravibabui@synopsys.com  001		dev in progress
*/

#ifndef __MSHC_CORE_CFG_H_
#define _MSHC_CORE_CFG_H_
#include "core_cfg.h"

#define MAX_SDHCI_INSTANCES 	4

/* \brief: supported platform 
 *	chose/define one supported platforms as shown below
 *	CONFIG_PLATFORM_CSIM_SDLINK 	:platform/csim_sdlink
 *	CONFIG_PLATFORM_RTLSIM		:platform/rtlsim
 *	CONFIG_PLATFORM_LINUX_UIO	:platform/uiolinux
 *	CONFIG_PLATFORM_LINUX_VFIO	:platform/vfiolinux
 * Note: currently LINUX-UIO mode is supported for
 *	device (HAPS80 FPGA) platform through pci interface
*/
#undef CONFIG_PLATFORM_CSIM_SDLINK
#undef CONFIG_MSHC_CSIM
#undef CONFIG_PLATFORM_RTLSIM
#undef CONFIG_PLATFORM_LINUX_VFIO

/* support pci uio support for device acess */
#define CONFIG_PLATFORM_LINUX_UIO
/* support pci map */
#define CONFIG_PCI_MAP

/* \brief: SDHCI driver configuration
	sdhci driver definitions are included here
 */
#ifdef CONFIG_PLATFORM_CSIM_SDLINK
#define CONFIG_MSHC_MSDC_CLINK
#define CONFIG_MSHC_CLINK
#else
#undef CONFIG_MSHC_CLINK
#define CONFIG_MSDC_CLINK
#endif

/* support for terminal */
#define CONFIG_TERMINAL

/* support for stdio based terminal */
#undef CONFIG_TERMINAL_STDIO

/* support for VIO based terminal */
#define CONFIG_TERMINAL_VIO
#endif /* _MSHC_CORE_CFG_H_ */
