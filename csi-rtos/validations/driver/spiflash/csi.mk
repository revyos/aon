##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_SPIFLASH01), y)
CSRC += $(VALDIR)/driver/spiflash/spiflash01/spiflash01.c
endif

ifeq ($(CONFIG_BUILD_VAL_SPIFLASH02), y)
CSRC += $(VALDIR)/driver/spiflash/spiflash02/spiflash02.c
endif
