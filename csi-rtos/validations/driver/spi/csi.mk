##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

ifeq ($(CONFIG_BUILD_VAL_SPI01), y)
CSRC += $(VALDIR)/driver/spi/spi01/spi01.c
CSRC += $(VALDIR)/driver/spi/spi01/w25q64fv.c
endif

ifeq ($(CONFIG_BUILD_VAL_SPI02), y)
CSRC += $(VALDIR)/driver/spi/spi02/spi02.c
CSRC += $(VALDIR)/driver/spi/spi02/w25q64fv.c
CSRC += $(VALDIR)/driver/spi/spi02/spi_func.c
endif

ifeq ($(CONFIG_BUILD_VAL_SPI_SPEED), y)
CSRC += $(VALDIR)/driver/spi/spi_speed/spi_speed.c
CSRC += $(VALDIR)/driver/spi/spi_speed/timer_port.c
endif
