##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_TIMER01), y)
CSRC += $(VALDIR)/driver/timer/timer01/timer01.c
endif

ifeq ($(CONFIG_BUILD_VAL_TIMER02), y)
CSRC += $(VALDIR)/driver/timer/timer02/timer02.c
endif
