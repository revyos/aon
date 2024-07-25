##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_CRC01), y)
CSRC += $(VALDIR)/driver/crc/crc01/crc01.c
endif

ifeq ($(CONFIG_BUILD_VAL_CRC02), y)
CSRC += $(VALDIR)/driver/crc/crc02/crc02.c
endif
