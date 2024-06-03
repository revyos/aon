##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##


ifeq ($(CONFIG_BUILD_VAL_KERNEL), y)
INCDIR += -I$(VALDIR)/kernel/include
endif

ifeq ($(CONFIG_BUILD_VAL_KERNEL), y)
CSRC += $(VALDIR)/kernel/main.c
CSRC += $(VALDIR)/kernel/test_self_entry.c
CSRC += $(VALDIR)/kernel/test_util.c
endif

ifeq ($(CONFIG_BUILD_VAL_TASK), y)
include $(VALDIR)/kernel/task/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_SEM), y)
include $(VALDIR)/kernel/sem/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_MUTEX), y)
include $(VALDIR)/kernel/mutex/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_MSGQ), y)
include $(VALDIR)/kernel/msgq/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_SOFT_TIMER), y)
include $(VALDIR)/kernel/soft_timer/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_EVENT), y)
include $(VALDIR)/kernel/event/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_MEMPOOL), y)
ifneq ($(CONFIG_KERNEL_FREERTOS), y)
include $(VALDIR)/kernel/mempool/csi.mk
endif
endif
