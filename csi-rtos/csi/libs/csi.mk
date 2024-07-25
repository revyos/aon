##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##

INCDIR += -I$(LIBSDIR)/minilibc/include
INCDIR += -I$(LIBSDIR)/mm/include
INCDIR += -I$(LIBSDIR)/comm/include
INCDIR += -I$(LIBSDIR)/trace/include
INCDIR += -I$(LIBSDIR)/ringbuffer/
INCDIR += -I$(LIBSDIR)/console/
INCDIR += -I$(LIBSDIR)/syslog/include

ifndef CONFIG_NUTTXMM_NONE

ifeq ($(CONFIG_NEWLIB_WRAP), y)
#ifeq ($(CONFIG_SIMPLE_PRINTF), y)
#LIB_CSRC += $(LIBSDIR)/minilibc/src/simple_printf.c
#else
LIB_CSRC += $(LIBSDIR)/minilibc/src/printf.c
#endif
endif

LIB_CSRC += \
          $(LIBSDIR)/minilibc/src/malloc.c \
          $(LIBSDIR)/minilibc/src/minilibc_port.c \
          $(LIBSDIR)/minilibc/src/clock_gettime.c \
          $(LIBSDIR)/minilibc/src/_init.c \
          $(LIBSDIR)/minilibc/src/syslog.c \
          $(LIBSDIR)/mm/src/mm_malloc.c \
          $(LIBSDIR)/mm/src/mm_free.c \
          $(LIBSDIR)/mm/src/mm_leak.c \
          $(LIBSDIR)/mm/src/mm_size2ndx.c \
          $(LIBSDIR)/mm/src/mm_addfreechunk.c \
          $(LIBSDIR)/mm/src/mm_initialize.c \
          $(LIBSDIR)/mm/src/mm_mallinfo.c \
          $(LIBSDIR)/mm/src/lib_mallinfo.c \
          $(LIBSDIR)/mm/src/dq_addlast.c \
          $(LIBSDIR)/mm/src/dq_rem.c\
          $(LIBSDIR)/ringbuffer/src/ringbuffer.c\
          $(LIBSDIR)/console/src/sys_console.c
endif

ifdef CONFIG_OS_TRACE
CSRC += \
    $(LIBSDIR)/trace/trcTrigger.c \
    $(LIBSDIR)/trace/trcHardwarePort.c \
    $(LIBSDIR)/trace/trcUser.c
endif

ifeq ($(CONFIG_OS_COMM), y)
include $(LIBSDIR)/comm/csi.mk
endif
