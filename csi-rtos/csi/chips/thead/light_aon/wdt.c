
#include <stdint.h>
#include "drv/wdt.h"
#include "soc.h"
#include "syslog.h"

#ifndef readl
#define readl(addr) \
    ({ unsigned int __v = (*(volatile uint32_t *) (addr)); __v; })
#endif

#ifndef writel
#define writel(addr,b) ((*(volatile uint32_t *) (addr)) = (b))
#endif

#define REG_SYS_RST_CFG		(AONSYS_RSTGEN_BASE + 0x10)
#define REG_RST_REQ_EN_0	(AONSYS_RSTGEN_BASE + 0x140)
#define SW_SYS_RST_REQ		(1 << 0)
#define SW_GLB_RST_EN		(1 << 0)

#define AON_WDT_RST_EN                                    (1 << 1)
#define AUD_WDT_RST_EN                                    (1 << 5)
#define AP_WDT0_RST_EN                                    (1 << 8)
#define AP_WDT1_RST_EN                                    (1 << 9)

#define AON_WDT_RST_MSK_ALL                               (AON_WDT_RST_EN | \
							   AUD_WDT_RST_EN | \
							   AP_WDT0_RST_EN | \
							   AP_WDT1_RST_EN )

#if 0
typedef enum {
	AON_WDT_MONITOR_APWDT0,                            ///< ap-wdt0
	AON_WDT_MONITOR_APWDT1,                            ///< ap-wdt1
	AON_WDT_MONITOR_AUDWDT,                            ///< aud-wdt
	AON_WDT_MONITOR_MAX,
}aon_wdt_idx_t;
#endif

typedef struct {
    uint8_t   id;
    const char *name;
    csi_wdt_t      wdt_handle;
}chip_wdt_rsc_t;

#define AON_WDT_ID_DEF(x) \
{ \
    .name = #x, \
    .id   = x, \
}

static chip_wdt_rsc_t g_light_wdt_rsc[] = {
	AON_WDT_ID_DEF(APWDT0_INDEX),
	AON_WDT_ID_DEF(APWDT1_INDEX),
	AON_WDT_ID_DEF(AUDWDT_INDEX)
};



void chip_wdt_clear_reset()
{
	uint32_t val;

	val = 0xffffffff; //readl(AONSYS_WDT_RESET_CLR);
	writel(AONSYS_WDT_RESET_CLR, val);
}

uint32_t chip_wdt_get_sta()
{
    return readl(AONSYS_WDT_RESET_ST);
}

void chip_wdt_disable_reset(void)
{
	uint32_t val;

	val = readl(AONSYS_WDT_RESET_EN);
	val &= ~AON_WDT_RST_MSK_ALL;
	writel(AONSYS_WDT_RESET_EN, val);
}



void chip_wdt_clear_timeout()
{
    for(int i = 0; i< ARRAY_SIZE(g_light_wdt_rsc); i++) {
	/*init AP-WDT0 handle*/
	   csi_wdt_intr_clear((csi_wdt_t*)&(g_light_wdt_rsc[i].wdt_handle));

    }

}




int chip_wdt_init()
{
	csi_error_t ret;
    for(int i = 0; i< ARRAY_SIZE(g_light_wdt_rsc); i++) {
	/*init AP-WDT0 handle*/
	ret = csi_wdt_init((csi_wdt_t*)&(g_light_wdt_rsc[i].wdt_handle), g_light_wdt_rsc[i].id);
	if (ret) {
		LOG_E("init-wdt-handle err %d\n", ret);
		return ret;
    }
    }
	return 0;
}


int chip_wdt_check_timeout()
{
	int        idx = 0;

  for(int i = 0; i< ARRAY_SIZE(g_light_wdt_rsc); i++) {
	if (csi_wdt_is_timeout((csi_wdt_t*)&(g_light_wdt_rsc[i].wdt_handle))) {
		idx |= 1 << g_light_wdt_rsc[i].id;
	}

    }
	return idx;
}


void system_reset()
{
	uint32_t data = *(uint32_t*)REG_RST_REQ_EN_0;

	/* global reset enable */
	data |= SW_GLB_RST_EN;
	*(uint32_t*)REG_RST_REQ_EN_0 = data;

	/* global reset request */
	*(uint32_t*)REG_SYS_RST_CFG = SW_SYS_RST_REQ;
	mdelay(1000);
}
