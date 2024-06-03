##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_EVENT01), y)
CSRC += $(VALDIR)/kernel/event/event01/event_opr.c
CSRC += $(VALDIR)/kernel/event/event01/event_param.c
CSRC += $(VALDIR)/kernel/event/event01/event01.c
endif
