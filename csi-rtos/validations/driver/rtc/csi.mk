##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_RTC01), y)
CSRC += $(VALDIR)/driver/rtc/rtc01/rtc01.c
endif

ifeq ($(CONFIG_BUILD_VAL_RTC02), y)
CSRC += $(VALDIR)/driver/rtc/rtc02/rtc02.c
endif
