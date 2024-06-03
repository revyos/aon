##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

INCDIR += -I$(CHIPDIR)/include
INCDIR += -I$(DRIVERDIR)/common/
INCDIR += -I$(DRIVERDIR)/uart/dw/v1.0
INCDIR += -I$(DRIVERDIR)/i2s/wj/v2.0
INCDIR += -I$(DRIVERDIR)/dma/dw/axi_dma
INCDIR += -I$(DRIVERDIR)/pinmux/wj/light_v1.0
INCDIR += -I$(DRIVERDIR)/timer/dw/v1.0/
INCDIR += -I$(DRIVERDIR)/iic/dw/v1.0/
INCDIR += -I$(DRIVERDIR)/gpio/dw/v1.0
INCDIR += -I$(DRIVERDIR)/wdt/dw/v1.0/
INCDIR += -I$(DRIVERDIR)/qspi/dw/v1.0/
INCDIR += -I$(DRIVERDIR)/spinand/wj/v1.0/
INCDIR += -I$(DRIVERDIR)/secure/rambus/inc/
INCDIR += -I$(DRIVERDIR)/rng/rb/v2.4
INCDIR += -I$(DRIVERDIR)/spi/dw/v2.0/
INCDIR += -I$(DRIVERDIR)/spiflash/v1.0/
INCDIR += -I$(DRIVERDIR)/rsa/rb/v3.3/inc
INCDIR += -I$(DRIVERDIR)/sm2/rb/v3.3/inc
INCDIR += -I$(DRIVERDIR)/mbox/wj/v4.0
INCDIR += -I$(DRIVERDIR)/sdhc/dw/v1.0

DRIVER_SSRC += $(CHIPDIR)/startup.S
DRIVER_CSRC += $(CHIPDIR)/system.c
DRIVER_CSRC += $(CHIPDIR)/cache.c
DRIVER_CSRC += $(CHIPDIR)/sys_clk.c
DRIVER_CSRC += $(CHIPDIR)/devices.c
DRIVER_CSRC += $(CHIPDIR)/novic_irq_tbl.c

include $(DRIVERDIR)/usb/dw/v3.0/csi.mk

DRIVER_CSRC += $(DRIVERDIR)/common/irq.c
DRIVER_CSRC += $(DRIVERDIR)/common/clk.c
DRIVER_CSRC += $(DRIVERDIR)/common/power_manage.c
DRIVER_CSRC += $(DRIVERDIR)/common/target_get.c
DRIVER_CSRC += $(DRIVERDIR)/common/reboot.c
DRIVER_CSRC += $(DRIVERDIR)/common/pre_main.c
DRIVER_CSRC += $(DRIVERDIR)/common/weak.c
DRIVER_CSRC += $(DRIVERDIR)/common/ringbuffer.c

DRIVER_CSRC += $(DRIVERDIR)/core/c910/trap_c.c
DRIVER_SSRC += $(DRIVERDIR)/core/c910/vectors.S
DRIVER_CSRC += $(DRIVERDIR)/irq/plic/v1.0/irq_port.c
DRIVER_CSRC += $(DRIVERDIR)/tick/csitimer/tick.c
DRIVER_CSRC += $(DRIVERDIR)/uart/dw/v1.0/uart.c
DRIVER_CSRC += $(DRIVERDIR)/uart/dw/v1.0/dw_uart_ll.c
DRIVER_CSRC += $(DRIVERDIR)/dma/dw/axi_dma/dma.c
DRIVER_CSRC += $(DRIVERDIR)/pinmux/wj/light_v1.0/pinmux.c
DRIVER_CSRC += $(DRIVERDIR)/timer/dw/v1.0/timer.c
DRIVER_CSRC += $(DRIVERDIR)/iic/dw/v1.0/iic.c
DRIVER_CSRC += $(DRIVERDIR)/iic/dw/v1.0/dw_iic_ll.c
DRIVER_CSRC += $(DRIVERDIR)/gpio/dw/v1.0/gpio.c
DRIVER_CSRC += $(DRIVERDIR)/gpio/dw/v1.0/dw_gpio_ll.c
DRIVER_CSRC += $(DRIVERDIR)/gpio_pin/v1.0/gpio_pin.c
DRIVER_CSRC += $(DRIVERDIR)/wdt/dw/v1.0/wdt.c
DRIVER_CSRC += $(DRIVERDIR)/wdt/dw/v1.0/dw_wdt_ll.c
DRIVER_CSRC += $(DRIVERDIR)/spi/dw/v2.0/dw_spi_ll.c
DRIVER_CSRC += $(DRIVERDIR)/spi/dw/v2.0/spi.c
DRIVER_CSRC += $(DRIVERDIR)/spiflash/v2.0/spiflash.c
DRIVER_CSRC += $(DRIVERDIR)/spiflash/v2.0/qspiflash_ops.c
DRIVER_CSRC += $(DRIVERDIR)/spiflash/v2.0/spiflash_ops.c
DRIVER_CSRC += $(DRIVERDIR)/spiflash/v2.0/spiflash_vendor.c
DRIVER_CSRC += $(DRIVERDIR)/qspi/dw/v1.0/dw_qspi_ll.c
DRIVER_CSRC += $(DRIVERDIR)/qspi/dw/v1.0/qspi.c
DRIVER_CSRC += $(DRIVERDIR)/spinand/v1.0/spinand.c
DRIVER_CSRC += $(DRIVERDIR)/spinand/v1.0/spinand_vendor.c
DRIVER_CSRC += $(DRIVERDIR)/spinand/v1.0/qspinand_ops.c
DRIVER_CSRC += $(DRIVERDIR)/mbox/wj/v4.0/mbox.c
DRIVER_CSRC += $(DRIVERDIR)/sdhc/dw/v1.0/*.c

DRIVER_CSRC += $(DRIVERDIR)/secure/rambus/src/*.c
DRIVER_CSRC += $(DRIVERDIR)/rng/rb/v2.4/*.c
DRIVER_CSRC += $(DRIVERDIR)/rsa/rb/v3.3/csi/*.c
DRIVER_CSRC += $(DRIVERDIR)/rsa/rb/v3.3/eip/*.c
DRIVER_CSRC += $(DRIVERDIR)/rsa/rb/v3.3/evp/*.c
DRIVER_CSRC += $(DRIVERDIR)/rsa/rb/v3.3/adapter/src/*.c
DRIVER_CSRC += $(DRIVERDIR)/sha/rb/v1.5/*.c
DRIVER_CSRC += $(DRIVERDIR)/sm3/rb/v1.5/*.c
DRIVER_CSRC += $(DRIVERDIR)/aes/rb/v1.5/*.c
DRIVER_CSRC += $(DRIVERDIR)/sm4/rb/v1.5/*.c
DRIVER_CSRC += $(DRIVERDIR)/sm2/rb/v3.3/*.c


