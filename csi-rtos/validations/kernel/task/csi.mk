##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_TASK01), y)
CSRC += $(VALDIR)/kernel/task/task01/task_create.c
CSRC += $(VALDIR)/kernel/task/task01/task_param.c
CSRC += $(VALDIR)/kernel/task/task01/task_sleep.c
CSRC += $(VALDIR)/kernel/task/task01/task_suspend_test.c
CSRC += $(VALDIR)/kernel/task/task01/task_yield_test.c
CSRC += $(VALDIR)/kernel/task/task01/task01.c
endif

ifeq ($(CONFIG_BUILD_VAL_TASK02), y)
CSRC += $(VALDIR)/kernel/task/task02/create_task.c
CSRC += $(VALDIR)/kernel/task/task02/delete_task.c
CSRC += $(VALDIR)/kernel/task/task02/exit_task.c
CSRC += $(VALDIR)/kernel/task/task02/get_task_count.c
CSRC += $(VALDIR)/kernel/task/task02/get_task_handle.c
CSRC += $(VALDIR)/kernel/task/task02/get_task_stat.c
#CSRC += $(VALDIR)/kernel/task/task02/list_task.c
CSRC += $(VALDIR)/kernel/task/task02/resume_task.c
CSRC += $(VALDIR)/kernel/task/task02/set_get_task_prio.c
CSRC += $(VALDIR)/kernel/task/task02/suspend_task.c
CSRC += $(VALDIR)/kernel/task/task02/task_del_interface.c
CSRC += $(VALDIR)/kernel/task/task02/task_get_prio_interface.c
CSRC += $(VALDIR)/kernel/task/task02/task_get_stat_interface.c
CSRC += $(VALDIR)/kernel/task/task02/task_list_interface.c
CSRC += $(VALDIR)/kernel/task/task02/task_new_interface.c
#CSRC += $(VALDIR)/kernel/task/task02/task_quanta.c
CSRC += $(VALDIR)/kernel/task/task02/task_resume_interface.c
CSRC += $(VALDIR)/kernel/task/task02/task_set_prio_interface.c
CSRC += $(VALDIR)/kernel/task/task02/task_suspend_interface.c
CSRC += $(VALDIR)/kernel/task/task02/task_terminate_interface.c
CSRC += $(VALDIR)/kernel/task/task02/task02.c
CSRC += $(VALDIR)/kernel/task/task02/terminate_task.c
CSRC += $(VALDIR)/kernel/task/task02/yield_tasks.c
endif
