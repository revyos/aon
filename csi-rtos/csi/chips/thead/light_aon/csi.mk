##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##
INCDIR += -I$(CHIPDIR)/include
INCDIR += -I$(DRIVERDIR)/common/
INCDIR += -I$(DRIVERDIR)/uart/dw/v1.0
INCDIR += -I$(DRIVERDIR)/iic/dw/v1.0
INCDIR += -I$(DRIVERDIR)/wdt/dw/v1.0
INCDIR += -I$(DRIVERDIR)/gpio/dw/v1.0
INCDIR += -I$(DRIVERDIR)/timer/dw/v1.0
INCDIR += -I$(DRIVERDIR)/pinmux/wj/light_aon_v1.0
INCDIR += -I$(DRIVERDIR)/mbox/wj/v5.0
INCDIR += -I$(DRIVERDIR)/rtc/wj/v2.0
INCDIR += -I$(DRIVERDIR)/rtc/wj/alg/
INCDIR += -I$(DRIVERDIR)/pmic/wj/v1.0/
INCDIR += -I$(DRIVERDIR)/pvt/moortec/v1.0/

DRIVER_SSRC += $(CHIPDIR)/startup.S
DRIVER_CSRC += $(CHIPDIR)/system.c
DRIVER_CSRC += $(CHIPDIR)/sys_clk.c
DRIVER_CSRC += $(CHIPDIR)/devices.c
DRIVER_CSRC += $(CHIPDIR)/system_power.c
DRIVER_CSRC += $(CHIPDIR)/power.c
DRIVER_CSRC += $(CHIPDIR)/sysclk.c
DRIVER_CSRC += $(CHIPDIR)/sysrst.c
DRIVER_CSRC += $(CHIPDIR)/wdt.c
DRIVER_CSRC += $(CHIPDIR)/ddr/*.c

DRIVER_CSRC += $(DRIVERDIR)/common/irq.c
DRIVER_CSRC += $(DRIVERDIR)/common/target_get.c
DRIVER_CSRC += $(DRIVERDIR)/common/clk.c
DRIVER_CSRC += $(DRIVERDIR)/common/reboot.c
DRIVER_CSRC += $(DRIVERDIR)/common/pre_main.c
DRIVER_CSRC += $(DRIVERDIR)/common/weak.c
DRIVER_CSRC += $(DRIVERDIR)/core/e902/trap_c.c
DRIVER_SSRC += $(DRIVERDIR)/core/e902/vectors.S
DRIVER_CSRC += $(DRIVERDIR)/irq/vic/v1.0/irq_port.c
DRIVER_CSRC += $(DRIVERDIR)/tick/mtime/tick.c
DRIVER_CSRC += $(DRIVERDIR)/iic/dw/v1.0/iic.c
DRIVER_CSRC += $(DRIVERDIR)/iic/dw/v1.0/dw_iic_ll.c
DRIVER_CSRC += $(DRIVERDIR)/gpio/dw/v1.0/gpio.c
DRIVER_CSRC += $(DRIVERDIR)/gpio/dw/v1.0/dw_gpio_ll.c
DRIVER_CSRC += $(DRIVERDIR)/gpio_pin/v1.0/gpio_pin.c
DRIVER_CSRC += $(DRIVERDIR)/uart/dw/v1.0/uart.c
DRIVER_CSRC += $(DRIVERDIR)/uart/dw/v1.0/dw_uart_ll.c
DRIVER_CSRC += $(DRIVERDIR)/timer/dw/v1.0/timer.c
DRIVER_CSRC += $(DRIVERDIR)/wdt/dw/v1.0/wdt.c
DRIVER_CSRC += $(DRIVERDIR)/pinmux/wj/light_aon_v1.0/pinmux.c
DRIVER_CSRC += $(DRIVERDIR)/mbox/wj/v5.0/mbox.c
DRIVER_CSRC += $(DRIVERDIR)/rtc/wj/v2.0/rtc.c
DRIVER_CSRC += $(DRIVERDIR)/rtc/wj/v2.0/wj_rtc_ll.c
DRIVER_CSRC += $(DRIVERDIR)/rtc/wj/alg/rtc_alg.c
DRIVER_CSRC += $(DRIVERDIR)/pmic/wj/v1.0/pmic.c
DRIVER_CSRC += $(DRIVERDIR)/pvt/moortec/v1.0/pvt.c


ifeq ($(CONFIG_PMIC_DA9063_V1), y)
DRIVER_CSRC += $(DRIVERDIR)/pmic/wj/v1.0/da9063.c
endif
ifeq ($(CONFIG_PMIC_DA9121_V1), y)
DRIVER_CSRC += $(DRIVERDIR)/pmic/wj/v1.0/da9121.c
endif
ifeq ($(CONFIG_PMIC_SLG51000_V1), y)
DRIVER_CSRC += $(DRIVERDIR)/pmic/wj/v1.0/slg51000.c
endif
ifeq ($(CONFIG_PMIC_RICOH567_V0), y) || ($(CONFIG_PMIC_RICOH567_V1), y)
DRIVER_CSRC += $(DRIVERDIR)/pmic/wj/v1.0/ricoh567.c
endif
ifeq ($(CONFIG_PMIC_APTF110_V1), y)
DRIVER_CSRC += $(DRIVERDIR)/pmic/wj/v1.0/aptf110.c
endif
