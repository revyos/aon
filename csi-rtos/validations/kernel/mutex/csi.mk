##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_MUTEX01), y)
CSRC += $(VALDIR)/kernel/mutex/mutex01/mutex_opr.c
CSRC += $(VALDIR)/kernel/mutex/mutex01/mutex_param.c
CSRC += $(VALDIR)/kernel/mutex/mutex01/mutex01.c
endif
