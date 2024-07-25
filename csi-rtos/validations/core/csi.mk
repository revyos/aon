##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##


ifeq ($(CONFIG_BUILD_VAL_CORE), y)
INCDIR += -I$(VALDIR)/core/include
endif

ifeq ($(CONFIG_BUILD_VAL_CACHE), y)
include $(VALDIR)/core/cache/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_CALCULATION), y)
include $(VALDIR)/core/calculation/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_MMU), y)
include $(VALDIR)/core/mmu/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_MPU), y)
include $(VALDIR)/core/mpu/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_VIC), y)
include $(VALDIR)/core/vic/csi.mk
endif
