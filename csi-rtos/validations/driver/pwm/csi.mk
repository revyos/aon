##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_PWM01), y)
CSRC += $(VALDIR)/driver/pwm/pwm01/pwm01.c
endif

ifeq ($(CONFIG_BUILD_VAL_PWM02), y)
CSRC += $(VALDIR)/driver/pwm/pwm02/pwm02.c
endif
