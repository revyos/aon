##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_USART01), y)
CSRC += $(VALDIR)/driver/usart/usart01/usart01.c
endif

ifeq ($(CONFIG_BUILD_VAL_USART02), y)
CSRC += $(VALDIR)/driver/usart/usart02/usart02.c
CSRC += $(VALDIR)/driver/usart/usart02/uart_func.c
endif

ifeq ($(CONFIG_BUILD_VAL_USART_SPEED), y)
CSRC += $(VALDIR)/driver/usart/usart_speed/uart_speed.c
CSRC += $(VALDIR)/driver/usart/usart_speed/timer_port.c
endif
