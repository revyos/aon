##
 # Copyright (C) 2017-2019 Alibaba Group Holding Limited
##


#LIB_CSRC += $(MODULESDIR)/newlib_wrap/sys/write.c

LIB_CSRC += $(MODULESDIR)/newlib_wrap/__dtostr.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/fprintf.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/getc.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/getchar.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/__isinf.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/__isnan.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/__lltostr.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/__ltostr.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/printf.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/putc.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/putchar.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/puts.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/snprintf.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/sprintf.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/vfprintf.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/__v_printf.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/vprintf.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/vsnprintf.c
LIB_CSRC += $(MODULESDIR)/newlib_wrap/vsprintf.c
