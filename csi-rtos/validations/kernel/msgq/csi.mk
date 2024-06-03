##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_MSGQ01), y)
CSRC += $(VALDIR)/kernel/msgq/msgq01/msg_queue_create.c
CSRC += $(VALDIR)/kernel/msgq/msgq01/msg_queue_del.c
CSRC += $(VALDIR)/kernel/msgq/msgq01/msg_queue_flush.c
CSRC += $(VALDIR)/kernel/msgq/msgq01/msg_queue_recv.c
CSRC += $(VALDIR)/kernel/msgq/msgq01/msgq01.c
endif


ifeq ($(CONFIG_BUILD_VAL_MSGQ02), y)
CSRC += $(VALDIR)/kernel/msgq/msgq02/msgq02.c
CSRC += $(VALDIR)/kernel/msgq/msgq02/msgq_new_interface.c
CSRC += $(VALDIR)/kernel/msgq/msgq02/msgq_del_interface.c
CSRC += $(VALDIR)/kernel/msgq/msgq02/msgq_flush_interface.c
CSRC += $(VALDIR)/kernel/msgq/msgq02/msgq_func.c
CSRC += $(VALDIR)/kernel/msgq/msgq02/msgq_func2.c
CSRC += $(VALDIR)/kernel/msgq/msgq02/msgq_func3.c
CSRC += $(VALDIR)/kernel/msgq/msgq02/msgq_func4.c
CSRC += $(VALDIR)/kernel/msgq/msgq02/msgq_get_count_interface.c
CSRC += $(VALDIR)/kernel/msgq/msgq02/msgq_get_interface.c
CSRC += $(VALDIR)/kernel/msgq/msgq02/msgq_put_interface.c
endif

