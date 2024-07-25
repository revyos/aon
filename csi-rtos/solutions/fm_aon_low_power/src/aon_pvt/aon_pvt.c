#include <stdint.h>
#include <drv/common.h>
#include "syslog.h"
#include <drv/pvt.h>

#include "aon_common.h"

csi_pvt_t  pvt;

csi_callback_info_t g_pvt_param;

void pvt_evt_callback(csi_pvt_t* pvt, uint32_t event, csi_callback_info_t* arg)
{
	uint32_t i, raw_data, cvt_data;
	for (i = 0; i < pvt->sensor_info.ts_num; i++) {
		csi_pvt_read_ts(pvt, i, &raw_data);
		csi_pvt_ts_convert_raw_data(pvt, raw_data, &cvt_data);
		LOG_D("pvt_ts[%d]:%d %d\n", i, raw_data, cvt_data);
	}
	LOG_D("event:%d \n", event);
	LOG_D("ts_ip_mask:%08x \n", arg->irq_info.ts_irq_info.ts_ip_mask);
	LOG_D("ts_ip_alarm_high:%08x \n", arg->irq_info.ts_irq_info.ts_ip_high_alarm);
	LOG_D("ts_ip_alarm_low:%08x \n", arg->irq_info.ts_irq_info.ts_ip_low_alarm);
	mdelay(10);
	aon_pmic_poweroff();
}

int aon_pvt_init(int pvt_idx)
{
    int ret = 0;
	/*pvt init */
	ret = csi_pvt_init(&pvt, pvt_idx);
	if (ret)
	{
		LOG_D("ERR->csi_pvt_init failed\n");
		return CSI_ERROR;
	}

	/*pvt create ts alarm  hyst: 115 alarm: 125 */

	ret = csi_pvt_create_ts_rising_alarm(&pvt, 0, 3415, 3600);
	if (ret) {
		LOG_D("ERR->csi_pvt_create_ts_rising_alarm 0 failed \n");
		return CSI_ERROR;
	}

	/*pvt create ts alarm  hyst: -20 alarm: -30 */
	ret = csi_pvt_create_ts_falling_alarm(&pvt, 0, 890, 700);
	if (ret) {
		LOG_D("ERR->csi_pvt_create_ts_faling_alarm 0 failed \n");
		return CSI_ERROR;
	}

	/*register pvt-callback*/
	ret = csi_pvt_attach_callback(&pvt, pvt_evt_callback, &g_pvt_param);
	if (ret) {
		LOG_D("ERR->csi_pvt_attach_callback failed \n");
		return CSI_ERROR;
	}
   return ret;
}