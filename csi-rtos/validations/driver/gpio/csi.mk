##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_GPIO01), y)
CSRC += $(VALDIR)/driver/gpio/gpio01/gpio01.c
endif

ifeq ($(CONFIG_BUILD_VAL_GPIO02), y)
CSRC += $(VALDIR)/driver/gpio/gpio02/gpio02.c
endif
