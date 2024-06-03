##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_MEMPOOL01), y)
CSRC += $(VALDIR)/kernel/mempool/mempool01/mm_blk_fragment.c
CSRC += $(VALDIR)/kernel/mempool/mempool01/mm_blk_opr.c
CSRC += $(VALDIR)/kernel/mempool/mempool01/mm_blk_param.c
CSRC += $(VALDIR)/kernel/mempool/mempool01/mm_blk_reinit.c
CSRC += $(VALDIR)/kernel/mempool/mempool01/mempool01.c
endif


ifeq ($(CONFIG_BUILD_VAL_MEMPOOL02), y)
CSRC += $(VALDIR)/kernel/mempool/mempool02/mempool02.c
CSRC += $(VALDIR)/kernel/mempool/mempool02/mpool_new_interface.c
CSRC += $(VALDIR)/kernel/mempool/mempool02/mpool_del_interface.c
CSRC += $(VALDIR)/kernel/mempool/mempool02/mpool_alloc_interface.c
CSRC += $(VALDIR)/kernel/mempool/mempool02/mpool_free_interface.c
CSRC += $(VALDIR)/kernel/mempool/mempool02/mpool_func.c
endif

