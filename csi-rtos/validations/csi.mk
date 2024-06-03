##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##


INCDIR += -I$(MODULESDIR)/dtest/include

ifeq ($(CONFIG_BUILD_VAL_DRIVER), y)
include $(VALDIR)/driver/csi.mk
INCDIR += -I$(VALDIR)/driver/include
endif

ifeq ($(CONFIG_BUILD_VAL_CORE), y)
include $(VALDIR)/core/csi.mk
INCDIR += -I$(VALDIR)/core/include
endif

ifeq ($(CONFIG_BUILD_VAL_KERNEL), y)
include $(VALDIR)/kernel/csi.mk
endif

