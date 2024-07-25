##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_MMU01), y)
CSRC += $(VALDIR)/core/mmu/mmu01/mmu01.c
endif

ifeq ($(CONFIG_BUILD_VAL_MMU01_CK610M), y)
CSRC += $(VALDIR)/core/mmu/mmu01_ck610m/mmu01_ck610m.c
endif
