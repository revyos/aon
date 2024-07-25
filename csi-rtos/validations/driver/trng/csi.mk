##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_TRNG01), y)
CSRC += $(VALDIR)/driver/trng/trng01/trng01.c
endif

ifeq ($(CONFIG_BUILD_VAL_TRNG02), y)
CSRC += $(VALDIR)/driver/trng/trng02/trng02.c
endif
