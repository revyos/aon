##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_SOFT_TIMER01), y)
CSRC += $(VALDIR)/kernel/soft_timer/soft_timer01/timer_create_del.c
CSRC += $(VALDIR)/kernel/soft_timer/soft_timer01/timer_start_stop.c
CSRC += $(VALDIR)/kernel/soft_timer/soft_timer01/soft_timer01.c
endif
