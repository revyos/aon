##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_SEM01), y)
CSRC += $(VALDIR)/kernel/sem/sem01/sem_count.c
CSRC += $(VALDIR)/kernel/sem/sem01/sem_opr.c
CSRC += $(VALDIR)/kernel/sem/sem01/sem_param.c
CSRC += $(VALDIR)/kernel/sem/sem01/sem_reinit.c
CSRC += $(VALDIR)/kernel/sem/sem01/sem01.c
endif

ifeq ($(CONFIG_BUILD_VAL_SEM02), y)
CSRC += $(VALDIR)/kernel/sem/sem02/sem_del_interface.c
CSRC += $(VALDIR)/kernel/sem/sem02/sem_func2.c
#CSRC += $(VALDIR)/kernel/sem/sem02/sem_func3.c
CSRC += $(VALDIR)/kernel/sem/sem02/sem_func.c
CSRC += $(VALDIR)/kernel/sem/sem02/sem_new_interface.c
CSRC += $(VALDIR)/kernel/sem/sem02/sem_post_interface.c
CSRC += $(VALDIR)/kernel/sem/sem02/sem02.c
CSRC += $(VALDIR)/kernel/sem/sem02/sem_wait_interface.c
endif
