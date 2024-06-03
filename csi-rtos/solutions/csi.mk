##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_FM_AON_LOW_POWER), y)
INCDIR += -I$(SOLUTIONDIR)/fm_aon_low_power/src/inc
INCDIR += -I$(SOLUTIONDIR)/driver/rtc
CSRC += $(SOLUTIONDIR)/fm_aon_low_power/src/main.c
CSRC += $(SOLUTIONDIR)/fm_aon_low_power/src/aon_lowpower/*.c
CSRC += $(SOLUTIONDIR)/fm_aon_low_power/src/aon_misc/*.c
CSRC += $(SOLUTIONDIR)/fm_aon_low_power/src/aon_monitor/*.c
CSRC += $(SOLUTIONDIR)/fm_aon_low_power/src/aon_pin/*.c
CSRC += $(SOLUTIONDIR)/fm_aon_low_power/src/aon_pmic/*.c
CSRC += $(SOLUTIONDIR)/fm_aon_low_power/src/aon_pvt/*.c
CSRC += $(SOLUTIONDIR)/fm_aon_low_power/src/aon_rpc/*.c
CSRC += $(SOLUTIONDIR)/fm_aon_low_power/src/aon_wdt/*.c
CSRC += $(SOLUTIONDIR)/fm_aon_low_power/src/aon_sys/*.c
endif


