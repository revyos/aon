##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_I2S01), y)
CSRC += $(VALDIR)/driver/i2s/i2s01/i2s01.c
endif

ifeq ($(CONFIG_BUILD_VAL_I2S02), y)
CSRC += $(VALDIR)/driver/i2s/i2s02/i2s02.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_Cfg.c 
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_DmaRecv.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_GetStat.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_CfgProt.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_DmaSend.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_Init.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_Uninit.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_CfgSampWid.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_fun2.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_Recv.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_CfgWorkMod.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_GetCpaa.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_Send.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_CfgSampRat.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_fun1.c
CSRC += $(VALDIR)/driver/i2s/i2s02/iis_PorConl.c
endif
