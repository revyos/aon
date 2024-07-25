##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_RSA01), y)
CSRC += $(VALDIR)/driver/rsa/rsa01/rsa01.c
endif

ifeq ($(CONFIG_BUILD_VAL_RSA02), y)
CSRC += $(VALDIR)/driver/rsa/rsa02/rsa02.c
endif
