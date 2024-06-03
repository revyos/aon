/**
 ***************************************************************************************
 * @file     ct_sdhci.c
 * @author   ravibabu@synopsys.com
 * @version  00.00.01
 * @date     21 June 2019
 * @brief    sdhci driver
 *
 ***************************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright(C) 2017 Synopsys, Inc. All rights reserved</center></h2>
 *
 * This Synopsys software and all associated documentation are proprietary to
 * Synopsys, Inc. And may only be used pursuant to the terms and conditions of
 * a written license agreement with Synopsys, Inc. All other use, reproduction,
 * modification, or distribution of the Synopsys software or the associated
 * documentation is strictly prohibited.
 ***************************************************************************************
 @verbatim
   REVISION HISTORY:
   Version    Date         Author       	  Change Id     Description
   00.00.00   2019-06-21   ravibabu@synopsys.com    -           dev in progress
 @endverbatim
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emmc_common.h"
#include "clog.h"
#include "mmc_dev.h"
#include "hal.h"
#include "sd_cmd.h"
#include "dwc_msdc_cfg.h"
#include "mmc_dev.h"
#include "sdhci.h"
#include "drv/io.h"

/**
 * \brief sdhci_read
 *	wrapper API to read 8/16/32/bit data from sdhci register
 * \param pdata: pointer to sdhci
 * \param addr: register address
 * \param grain: granularity 8/16/32 bit access
 */
uint32_t sdhci_read(void *pdata, void *addr, uint8_t grain)
{
	uint32_t val = 0;
	struct sdhci_t *sdhci = (struct sdhci_t *)pdata;

	if (sdhci->pal)
		val = sdhci->pal->read(sdhci, (void *)addr, grain);

	return val;
}

/**
 * \brief sdhci_write
 *	Wrapper API to write 8/16/32/bit data to sdhci register
 * \param pdata: pointer to sdhci
 * \param addr: register address
 * \param grain: granularity 8/16/32 bit access
 */
uint32_t sdhci_write(void *pdata, void *addr, uint32_t val, uint8_t grain)
{
	struct sdhci_t *sdhci = (struct sdhci_t *)pdata;

	if (sdhci->pal && sdhci->pal->write)
		return sdhci->pal->write(sdhci, addr, val, grain);

	return ERROR_INVARG;
}

/**
 * @brief hal_sdhci_read
 *	read 32bit from sdhci register
 * @param addr: register address
 */
uint32_t hal_sdhci_read(void *pdata, void *addr, uint8_t grain)
{
	uint32_t val = 0;

    switch (grain) {
	case 1:
        val = getreg8(addr);
		break;
	case 2:
        val = getreg16(addr);
		break;
	case 4:
        val = getreg32(addr);
		break;
	default:
		break;
	}

	clog_print(CLOG_LEVEL9, "%s: read value (%p) from addr[%x]\n", __func__,
			val, (addr_t)addr);

	return val;
}

/**
 * @brief sdhci_sim_write
 *	write 32bit value to sdhci register
 * @param addr: register address
 */
uint32_t hal_sdhci_write(void *pdata, void *addr, uint32_t val, uint8_t grain)
{
//	struct sdhci_t *sdhci = (struct sdhci_t *)pdata;

	switch (grain) {
	case 1:
        putreg8(val, addr);
		break;
	case 2:
        putreg16(val, addr);
		break;
	case 4:
        putreg32(val, addr);
		break;
	default:
		break;
	}

	clog_print(CLOG_LEVEL9, "%s: writing value (%p) to addr[%x]\n", __func__,
			val, (addr_t)addr);

	return 0;
}


/**
 * \brief sdhci_hal_register
 *	register to sdhci driver to hal layer
 * \param sdhci: pointer to sdhci object
 * \returns 0 on success else fail -ve
 */
int sdhci_hal_register(struct sdhci_t *sdhci)
{
	sdhci->hal = hal_register(sdhci_read, sdhci_write, sdhci);
	if (sdhci->hal == NULL)
		return ERROR_OPER_FAIL;

	sdhci->pal = hal_register(hal_sdhci_read, hal_sdhci_write, sdhci);
	if (sdhci->hal == NULL)
		return ERROR_OPER_FAIL;

	return 0;
}
