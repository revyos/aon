##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_CUNIT), y)
include $(MODULESDIR)/cunit/csi.mk
endif
ifeq ($(CONFIG_FS), y)
include $(MODULESDIR)/fs/csi.mk
endif
ifeq ($(CONFIG_TEST)$(CONFIG_VAL), y)
include $(MODULESDIR)/dtest/csi.mk
endif
ifeq ($(CONFIG_POSIX_DRIVER), y)
include $(MODULESDIR)/posix-driver/csi.mk
endif
ifeq ($(CONFIG_NET), y)
include $(MODULESDIR)/net/csi.mk
endif
ifeq ($(CONFIG_CMSIS_ADAPTER), y)
include $(MODULESDIR)/cmsis_adapter/csi.mk
endif
ifeq ($(CONFIG_MBED_ADAPTER), y)
include $(MODULESDIR)/mbed-os/csi.mk
endif

ifeq ($(CONFIG_FATFS), y)
include $(MODULESDIR)/fatfs/csi.mk
endif

ifeq ($(CONFIG_FAAD), y)
include $(MODULESDIR)/libfaad/csi.mk
endif

ifeq ($(CONFIG_OPUS), y)
include $(MODULESDIR)/libopus/csi.mk
endif
