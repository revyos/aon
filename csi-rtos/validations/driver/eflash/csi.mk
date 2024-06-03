##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_EFLASH01), y)
CSRC += $(VALDIR)/driver/eflash/eflash01/eflash01.c
endif

ifeq ($(CONFIG_BUILD_VAL_EFLASH02), y)
CSRC += $(VALDIR)/driver/eflash/eflash02/eflash02.c
endif
