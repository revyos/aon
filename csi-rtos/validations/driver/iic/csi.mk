##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_IIC01), y)
CSRC += $(VALDIR)/driver/iic/iic01/iic01.c
endif

ifeq ($(CONFIG_BUILD_VAL_IIC02), y)
CSRC += $(VALDIR)/driver/iic/iic02/iic02.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_init_if.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_slave_send_if.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_capa_if.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_speed.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_config_if.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_func1.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_uninit_if.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_func2.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_other_if.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_func3.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_func4.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_slave_receive_if.c
#CSRC += $(VALDIR)/driver/iic/iic02/iic_power_cont_if.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_master_receive_if.c
#CSRC += $(VALDIR)/driver/iic/iic02/iic_loop.c
CSRC += $(VALDIR)/driver/iic/iic02/iic_master_send_if.c
endif

ifeq ($(CONFIG_BUILD_VAL_IIC_SPEED), y)
CSRC += $(VALDIR)/driver/iic/iic_speed/iic_speed.c
CSRC += $(VALDIR)/driver/iic/iic_speed/timer_port.c
endif
