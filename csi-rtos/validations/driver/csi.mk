##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##


ifeq ($(CONFIG_BUILD_VAL_SPI), y)
include $(VALDIR)/driver/spi/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_SPU_SPI), y)
include $(VALDIR)/driver/spu_spi/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_USI_SPI), y)
include $(VALDIR)/driver/usi_spi/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_USART), y)
include $(VALDIR)/driver/usart/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_SPU_USART), y)
include $(VALDIR)/driver/spu_usart/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_USI_USART), y)
include $(VALDIR)/driver/usi_usart/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_TRNG), y)
include $(VALDIR)/driver/trng/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_CRC), y)
include $(VALDIR)/driver/crc/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_EFLASH), y)
include $(VALDIR)/driver/eflash/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_SPIFLASH), y)
include $(VALDIR)/driver/spiflash/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_TIMER), y)
include $(VALDIR)/driver/timer/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_GPIO), y)
include $(VALDIR)/driver/gpio/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_AES), y)
include $(VALDIR)/driver/aes/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_IIC), y)
include $(VALDIR)/driver/iic/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_SPU_IIC), y)
include $(VALDIR)/driver/spu_iic/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_USI_IIC), y)
include $(VALDIR)/driver/usi_iic/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_RSA), y)
include $(VALDIR)/driver/rsa/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_RTC), y)
include $(VALDIR)/driver/rtc/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_WDT), y)
include $(VALDIR)/driver/wdt/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_SHA), y)
include $(VALDIR)/driver/sha/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_PWM), y)
include $(VALDIR)/driver/pwm/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_DMAC), y)
include $(VALDIR)/driver/dmac/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_ETH), y)
include $(VALDIR)/driver/eth/csi.mk
endif

ifeq ($(CONFIG_BUILD_VAL_I2S), y)
include $(VALDIR)/driver/i2s/csi.mk
endif
