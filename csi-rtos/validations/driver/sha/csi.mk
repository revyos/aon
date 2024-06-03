##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_SHA01), y)
CSRC += $(VALDIR)/driver/sha/sha01/sha01.c
endif

ifeq ($(CONFIG_BUILD_VAL_SHA02), y)
CSRC += $(VALDIR)/driver/sha/sha02/sha02.c
endif
