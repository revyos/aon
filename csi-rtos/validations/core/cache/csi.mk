##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_CACHE_PROFILE), y)
CSRC += $(VALDIR)/core/cache/cache_profile/cache_profile.c
endif

ifeq ($(CONFIG_BUILD_VAL_DCACHE_BASIC), y)
CSRC += $(VALDIR)/core/cache/dcache_basic/dcache_basic.c
endif
