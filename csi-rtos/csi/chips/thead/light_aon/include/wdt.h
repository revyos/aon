#ifndef __WDT_H_
#define __WDT_H_


int chip_wdt_init();

void chip_wdt_clear_reset();

uint32_t chip_wdt_get_sta();

void chip_wdt_disable_reset();

void chip_wdt_clear_timeout();

int chip_wdt_check_timeout();

void system_reset();


#endif