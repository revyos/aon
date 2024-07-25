##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_AES01), y)
CSRC += $(VALDIR)/driver/aes/aes01/aes01.c
endif

ifeq ($(CONFIG_BUILD_VAL_AES02), y)
CSRC += $(VALDIR)/driver/aes/aes02/aes02.c
endif
