##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_WDT01), y)
CSRC += $(VALDIR)/driver/wdt/wdt01/wdt01.c
endif

ifeq ($(CONFIG_BUILD_VAL_WDT02), y)
CSRC += $(VALDIR)/driver/wdt/wdt02/wdt02.c
endif
