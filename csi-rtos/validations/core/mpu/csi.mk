##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_MPU01), y)
CSRC += $(VALDIR)/core/mmu/mmu01/mmu01.c
endif

ifeq ($(CONFIG_BUILD_VAL_MPU01_CK610), y)
CSRC += $(VALDIR)/core/mpu/mpu01_ck610/mpu01_ck610.c
endif
