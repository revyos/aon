/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     rtc_test.c
 * @brief    the main function for the RTC driver
 * @version  V1.0
 * @date     3. Jue 2021
 ******************************************************************************/
#include <stdio.h>
#include <soc.h>
#include <drv/rtc.h>
#include <csi_config.h>
#include <board_config.h>
#include <board_init.h>

#define RTC_TIME_OSC_RATIO                 (2U)

#define RTC_TIMEOUT0_SECS                  (5U)
#define RTC_TIMEOUT1_SECS                  (10U)
#define RTC_TIMEOUT2_SECS                  (15U)

#define RTC_TIME_CURRENT_YEAR              (120)
#define RTC_TIME_CURRENT_MON               (0)
#define RTC_TIME_CURRENT_DAY               (5)
#define RTC_TIME_CURRENT_HOUR              (23)
#define RTC_TIME_CURRENT_MIN               (59)
#define RTC_TIME_CURRENT_SEC               (40)

#define RTC_TIME_ALARM_YEAR                (120)
#define RTC_TIME_ALARM_MON                 (0)
#define RTC_TIME_ALARM_DAY                 (5)
#define RTC_TIME_ALARM_HOUR                (23)
#define RTC_TIME_ALARM_MIN                 (59)
#define RTC_TIME_ALARM_SEC                 (50)

static csi_rtc_t g_rtc_handle;

extern int system_resume();

void rtc_event_cb_fun(csi_rtc_t *rtc_handle, void *arg)
{
	LOG_D("test rtc alarm time %ds\n", RTC_TIMEOUT1_SECS);

	csi_rtc_cancel_alarm(rtc_handle);
	csi_rtc_uninit(&g_rtc_handle);

	LOG_D("Stop rtc\n");
	system_resume();
}

static int32_t test_rtc_alarm_time(csi_rtc_t *rtc_handle)
{
	int32_t ret = 0, state;
	csi_rtc_time_t initime, alarm_time;

	initime.tm_year	= RTC_TIME_CURRENT_YEAR;
	initime.tm_mon	= RTC_TIME_CURRENT_MON ;
	initime.tm_mday	= RTC_TIME_CURRENT_DAY ;
	initime.tm_hour	= RTC_TIME_CURRENT_HOUR;
	initime.tm_min	= RTC_TIME_CURRENT_MIN ;
	initime.tm_sec	= RTC_TIME_CURRENT_SEC ;

	state = csi_rtc_set_time(&g_rtc_handle, &initime);
	if (state < 0) {
		LOG_D("csi_rtc_set_time error\n");
		ret = -1;
	}

	alarm_time.tm_year = RTC_TIME_ALARM_YEAR;
	alarm_time.tm_mon = RTC_TIME_ALARM_MON ;
	alarm_time.tm_mday = RTC_TIME_ALARM_DAY ;
	alarm_time.tm_hour = RTC_TIME_ALARM_HOUR;
	alarm_time.tm_min  = RTC_TIME_ALARM_MIN ;
	alarm_time.tm_sec  = RTC_TIME_ALARM_SEC ;

	state = csi_rtc_set_alarm(&g_rtc_handle, &alarm_time, rtc_event_cb_fun, NULL);
	if (state < 0) {
		LOG_D("csi_rtc_set_alarm error\n");
		ret = -1;
	}

	return ret;
}


int rtc_alarm()
{
	int32_t ret = 0, state;

	LOG_D("start test_rtc\n");

	state = csi_rtc_init(&g_rtc_handle, 0U);
	if (state < 0) {
		LOG_D("csi_rtc_init error\n");
		ret = -1;
	}

	state = test_rtc_alarm_time(&g_rtc_handle);
	if (state < 0) {
		LOG_D("test_rtc_alarm_time error\n");
		ret = -1;
	}

	return ret;
}
