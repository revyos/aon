##
 # Copyright (C) 2017-2020 Alibaba Group Holding Limited
##

###############################################################################
# @file     makefile
# @brief    the makefile for the whole project
# @version  V1.0
# @date     02. June 2017
###############################################################################
ABSROOTDIR = $(shell pwd)
ROOTDIR = .

-include $(ROOTDIR)/.config

TARGETS_ROOT_PATH ?=

CC      = $(TOOL_PATH)csky-abiv2-elf-gcc
CPP     = $(TOOL_PATH)csky-abiv2-elf-g++
AS      = $(TOOL_PATH)csky-abiv2-elf-as
AR      = $(TOOL_PATH)csky-abiv2-elf-ar
LD      = $(TOOL_PATH)csky-abiv2-elf-ld
DUMP    = $(TOOL_PATH)csky-abiv2-elf-objdump
OBJCOPY = $(TOOL_PATH)csky-abiv2-elf-objcopy

ifeq ($(CONFIG_CPU_CK610E)$(CONFIG_CPU_CK610EF), y)
CC      = $(TOOL_PATH)csky-elf-gcc
CPP     = $(TOOL_PATH)csky-elf-g++
AS      = $(TOOL_PATH)csky-elf-as
AR      = $(TOOL_PATH)csky-elf-ar
LD      = $(TOOL_PATH)csky-elf-ld
DUMP    = $(TOOL_PATH)csky-elf-objdump
OBJCOPY = $(TOOL_PATH)csky-elf-objcopy
endif

ifeq ($(CONFIG_ARCH_RV32)$(CONFIG_ARCH_RV64), y)
CC      = $(TOOL_PATH)riscv64-unknown-elf-gcc
CPP     = $(TOOL_PATH)riscv64-unknown-elf-g++
AS      = $(TOOL_PATH)riscv64-unknown-elf-as
AR      = $(TOOL_PATH)riscv64-unknown-elf-ar
LD      = $(TOOL_PATH)riscv64-unknown-elf-ld
DUMP    = $(TOOL_PATH)riscv64-unknown-elf-objdump
OBJCOPY = $(TOOL_PATH)riscv64-unknown-elf-objcopy
endif

ifeq ($(CONFIG_ARCH_ARM), y)
CC      = $(TOOL_PATH)arm-none-eabi-gcc
CPP     = $(TOOL_PATH)arm-none-eabi-g++
AS      = $(TOOL_PATH)arm-none-eabi-as
AR      = $(TOOL_PATH)arm-none-eabi-ar
LD      = $(TOOL_PATH)arm-none-eabi-ld
DUMP    = $(TOOL_PATH)arm-none-eabi-objdump
OBJCOPY = $(TOOL_PATH)arm-none-eabi-objcopy
endif

CONFIG_CHIP_VENDOR_NAME := $(patsubst "%",%,$(strip $(CONFIG_CHIP_VENDOR_STR)))
CONFIG_CHIP_NAME  := $(patsubst "%",%,$(strip $(CONFIG_CHIP_NAME_STR)))
CONFIG_BOARD_NAME := $(patsubst "%",%,$(strip $(CONFIG_BOARD_NAME_STR)))
CONFIG_USER_DEFINED_LD_DIR := $(patsubst "%",%,$(strip $(CONFIG_USER_DEFINED_LD_DIR_STR)))

CONFIG_DEBUG_LEVEL := $(patsubst "%", %, $(strip $(CONFIG_DEBUG_LEVEL)))
CONFIG_OPTIMIZE_LEVEL := $(patsubst "%", %, $(strip $(CONFIG_OPTIMIZE_LEVEL)))

ifeq ($(CONFIG_BIG_ENDIAN),y)
ENDIAN_MODE = -mbig-endian
else
ENDIAN_MODE =
endif

OUTDIR = out
OBJDIR = $(OUTDIR)/obj
TESTDIR = $(ROOTDIR)/examples
SOLUTIONDIR = $(ROOTDIR)/solutions
DTESTDIR = $(ROOTDIR)/tests
AUTOTEST_DIR = $(ROOTDIR)/csi-test
VALDIR = $(ROOTDIR)/validations

ifeq ($(CONFIG_ARCH_ARM), y)
COREDIR = $(ROOTDIR)/csi/csi/include/cmsis
else
COREDIR = $(ROOTDIR)/csi/csi
endif

OFFCHIPDRIVERDIR = $(ROOTDIR)/csi/drivers
DRIVERDIR = $(ROOTDIR)/csi/drivers
LIBSDIR = $(ROOTDIR)/csi/libs
MODULESDIR = $(ROOTDIR)/modules
CHIPDIR = $(ROOTDIR)/csi/chips/$(CONFIG_CHIP_VENDOR_NAME)/$(CONFIG_CHIP_NAME)
BOARDDIR = $(ROOTDIR)/boards/$(CONFIG_CHIP_VENDOR_NAME)/$(CONFIG_BOARD_NAME)
KERNELDIR = $(ROOTDIR)/csi/csi_kernel
INCDIR =
CSRC =
LDDIR = $(BOARDDIR)/$(CONFIG_USER_DEFINED_LD_DIR)
NAME = $(CONFIG_BOARD_NAME)
PACKNAME = lib$(CONFIG_CHIP_NAME).a

export CC AS AR LD DUMP OBJCOPY CFLAGS LDFLAGS ASFLAGS INCLUDEDIRS ROOTDIR TARGERDIR DRIVERDIR COREDIR TESTDIR KERNELDIR SOLUTIONDIR

$(shell [ -d ${OUTDIR} ] || mkdir -p ${OUTDIR} && mkdir -p ${OBJDIR})

-include $(CHIPDIR)/csi.mk
-include $(BOARDDIR)/csi.mk
-include $(TESTDIR)/csi.mk
-include $(SOLUTIONDIR)/csi.mk
-include $(DTESTDIR)/csi.mk
-include $(VALDIR)/csi.mk
-include $(OFFCHIPDRIVERDIR)/csi.mk
-include $(MODULESDIR)/csi.mk
-include $(LIBSDIR)/csi.mk
ifneq ($(CONFIG_KERNEL_NONE), y)
-include $(KERNELDIR)/csi.mk
endif

INCLUDEDIRS = -I$(ROOTDIR)/include -I$(COREDIR)/include -I$(TESTDIR)/include -I$(KERNELDIR)/include -I$(LIBSDIR)
INCLUDEDIRS += $(INCDIR)

CKMAP=-Wl,-ckmap="$(OUTDIR)/$(NAME).ckmap"

ifeq ($(CONFIG_CPU_E801), y)
CPU += -mcpu=e801
endif

ifeq ($(CONFIG_CPU_E802), y)
CPU += -mcpu=e802
endif

ifeq ($(CONFIG_CPU_E802T), y)
CPU += -mcpu=e802t
endif

ifeq ($(CONFIG_CPU_S802), y)
CPU += -mcpu=s802
endif

ifeq ($(CONFIG_CPU_S802T), y)
CPU += -mcpu=s802t
endif

ifeq ($(CONFIG_CPU_E803), y)
CPU += -mcpu=e803
endif

ifeq ($(CONFIG_CPU_E803T), y)
CPU += -mcpu=e803t
endif

ifeq ($(CONFIG_CPU_S803), y)
CPU += -mcpu=s803
endif

ifeq ($(CONFIG_CPU_S803T), y)
CPU += -mcpu=s803t
endif

ifeq ($(CONFIG_CPU_E804D), y)
CPU += -mcpu=e804d
endif

ifeq ($(CONFIG_CPU_E804DT), y)
CPU += -mcpu=e804dt
endif

ifeq ($(CONFIG_CPU_E804F), y)
CPU += -mcpu=e804f
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_E804FT), y)
CPU += -mcpu=e804ft
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_E804DF), y)
CPU += -mcpu=e804df
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_E804DFT), y)
CPU += -mcpu=e804dft
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_I805), y)
CPU += -mcpu=i805
endif

ifeq ($(CONFIG_CPU_I805F), y)
CPU += -mcpu=i805f
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_CK801), y)
CPU += -mcpu=ck801
endif

ifeq ($(CONFIG_CPU_CK802), y)
CPU += -mcpu=ck802
endif

ifeq ($(CONFIG_CPU_CK803), y)
CPU += -mcpu=ck803
endif

ifeq ($(CONFIG_CPU_CK803F), y)
CPU += -mcpu=ck803f
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_CK803EF), y)
CPU += -mcpu=ck803ef -mdsp
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_CK803ER1), y)
CPU += -mcpu=ck803er1
endif

ifeq ($(CONFIG_CPU_CK803ER2), y)
CPU += -mcpu=ck803er2
endif

ifeq ($(CONFIG_CPU_CK803ER3), y)
CPU += -mcpu=ck803er3
endif

ifeq ($(CONFIG_CPU_CK803EFR1), y)
CPU += -mcpu=ck803efr1
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_CK803EFR2), y)
CPU += -mcpu=ck803efr2
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_CK803EFR3), y)
CPU += -mcpu=ck803efr3
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_CK804EF), y)
CPU += -mcpu=ck804ef
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_CK805), y)
CPU += -mcpu=ck805
endif

ifeq ($(CONFIG_CPU_CK805F), y)
CPU += -mcpu=ck805f
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_CK805EF), y)
CPU += -mcpu=ck805ef
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_CK807), y)
CPU += -mcpu=ck807
endif

ifeq ($(CONFIG_CPU_CK807F)$(CONFIG_CPU_R807F), y)
CPU += -mcpu=ck807f
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_R807), y)
CPU += -mcpu=ck807
endif

ifeq ($(CONFIG_CPU_CK810), y)
CPU += -mcpu=ck810
endif

ifeq ($(CONFIG_CPU_CK810F), y)
CPU += -mcpu=ck810f
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_CK610E), y)
CPU += -mcpu=ck610e
CFLAGS += -mdsp
endif

ifeq ($(CONFIG_CPU_CK610EF), y)
CPU += -mcpu=ck610ef
CFLAGS += -mdsp
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_C810), y)
CPU += -mcpu=c810
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_C810T), y)
CPU += -mcpu=c810t
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_C810V), y)
CPU += -mcpu=c810v
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_C810TV), y)
CPU += -mcpu=c810tv
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_C807), y)
CPU += -mcpu=c807
endif

ifeq ($(CONFIG_CPU_C807F), y)
CPU += -mcpu=c807f
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_C807FV), y)
CPU += -mcpu=c807fv
CFLAGS += -mhard-float
LDFLAGS += -mhard-float
endif

ifeq ($(CONFIG_CPU_E902), y)
CPU += -march=rv32ecxthead -mabi=ilp32e
CKMAP=
LDFLAGS += -Map
endif

ifeq ($(CONFIG_CPU_E902M), y)
CPU += -march=rv32emcxthead -mabi=ilp32e
CKMAP=
endif

ifeq ($(CONFIG_CPU_RV32EC), y)
CPU += -march=rv32ec -mabi=ilp32e
CKMAP=
endif

ifeq ($(CONFIG_CPU_RV32I), y)
CPU += -march=rv32i -mabi=ilp32 -mcmodel=medlow
CKMAP=
endif

ifeq ($(CONFIG_CPU_RV32IAC), y)
CPU += -march=rv32iac -mabi=ilp32 -mcmodel=medlow
CKMAP=
endif

ifeq ($(CONFIG_CPU_RV32IM), y)
CPU += -march=rv32im -mabi=ilp32 -mcmodel=medlow
CKMAP=
endif

ifeq ($(CONFIG_CPU_RV32IMAC), y)
CPU += -march=rv32imac -mabi=ilp32 -mcmodel=medlow
CKMAP=
endif

ifeq ($(CONFIG_CPU_RV32IMAFC), y)
CPU += -march=rv32imafc -mabi=ilp32f -mcmodel=medlow
CKMAP=
endif

ifeq ($(CONFIG_CPU_E906), y)
CPU +=  -Wa,-march=rv32imacxtheade -mabi=ilp32 -mcmodel=medlow -march=rv32imac
CKMAP=
endif

ifeq ($(CONFIG_CPU_E906F), y)
CPU += -march=rv32imafcxthead -mabi=ilp32f -mcmodel=medlow
CKMAP=
endif

ifeq ($(CONFIG_CPU_E906FD), y)
CPU += -march=rv32imafdcxthead -mabi=ilp32d -mcmodel=medlow
CKMAP=
endif

ifeq ($(CONFIG_CPU_C906), y)
CPU += -march=rv64imafdcvxthead -mabi=lp64d -mcmodel=medany -mtune=c906
CKMAP=
endif

ifeq ($(CONFIG_CPU_C910), y)
CPU += -march=rv64imafdcvxthead -mabi=lp64d -mcmodel=medany -mtune=c910
CKMAP=-Xlinker -Map=output.map
endif

ifeq ($(CONFIG_CPU_RV64IMAC), y)
CPU += -march=rv64imac -mabi=lp64 -mcmodel=medany
CKMAP=
endif

ifeq ($(CONFIG_CPU_RV64IMACXCKI), y)
CPU += -march=rv64imacxcki -mabi=lp64 -mcmodel=medany
CKMAP=
endif

ifeq ($(CONFIG_CPU_RV64IMAFDC), y)
CPU += -march=rv64imafdc -mabi=lp64d -mcmodel=medany
CKMAP=
endif

ifeq ($(CONFIG_CPU_RV64IMAFDCXCKI), y)
CPU += -march=rv64imafdcxcki -mabi=lp64d -mcmodel=medany
CKMAP=
endif

ifeq ($(CONFIG_CPU_CM0), y)
CPU += -mcpu=cortex-m0 -mthumb
ASFLAGS = -Wa,-mthumb
CKMAP=
endif

CFLAGS += $(CPU) -c

CFLAGS += $(CONFIG_DEBUG_LEVEL) $(CONFIG_OPTIMIZE_LEVEL) $(ENDIAN_MODE)
CFLAGS += -Wall
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -Wpointer-arith

CFLAGS += $(INCLUDEDIRS)

ifeq ($(CONFIG_CPU_E902), y)
LDFLAGS += -march=rv32ecxthead -mabi=ilp32e
endif

ifeq ($(CONFIG_CPU_E902M), y)
LDFLAGS += -march=rv32emcxthead -mabi=ilp32e -mccrt
endif

ifeq ($(CONFIG_CPU_E906), y)
LDFLAGS += -march=rv32imacxthead -mabi=ilp32
endif

ifeq ($(CONFIG_CPU_E906F), y)
LDFLAGS += -march=rv32imafcxthead -mabi=ilp32f
endif

ifeq ($(CONFIG_CPU_E906FD), y)
LDFLAGS += -march=rv32imafdcxthead -mabi=ilp32d
endif

LDFLAGS += -Wl,-zmax-page-size=1024

ifeq ($(CONFIG_ARCH_CSKY), y)
LDFLAGS += $(CPU)
endif

ASFLAGS += $(CFLAGS)
ARFLAGS = r
CPPLDFLAG_END += -lstdc++ -lsupc++


LDLIBS	=  -lm -lc -lgcc_tf -lcc-rt -Wl,--gc-section
ifeq ($(CONFIG_EXAMPLES_DSP), y)
ifeq ($(CONFIG_CPU_CK804EF), y)
LDLIBS += $(ROOTDIR)/csi/csi/dsp/lib/libcsky_CK804ef_dsp2_math.a
else
LDLIBS += $(ROOTDIR)/csi/csi/dsp/lib/libcsky_CK803ef_math.a
endif
endif

ifeq ($(CONFIG_EXAMPLES_VDSP), y)
LDLIBS += $(ROOTDIR)/csi/csi/dsp/lib/libcsky_CK805_vdsp2_math.a
endif

ifeq ($(CONFIG_EXAMPLES_DSP2_NN), y)
ifeq ($(CONFIG_CPU_E804D)$(CONFIG_CPU_E804DF)$(CONFIG_CPU_E804DT)$(CONFIG_CPU_E804DFT), y)
LDLIBS += $(ROOTDIR)/csi/csi/nn/lib/libcsky_E804d_dsp2_nn.a
endif
endif

ifeq ($(CONFIG_EXAMPLES_VDSP2_NN), y)
ifeq ($(CONFIG_CPU_CK805)$(CONFIG_CPU_CK805F), y)
LDLIBS += $(ROOTDIR)/csi/csi/nn/lib/libcsky_CK805_vdsp2_nn.a
else
  ifeq ($(CONFIG_CPU_I805)$(CONFIG_CPU_I805F), y)
  LDLIBS += $(ROOTDIR)/csi/csi/nn/lib/libcsky_I805_vdsp2_nn.a
  endif
endif
endif

ifeq ($(CONFIG_EXAMPLES_KERNEL_USB_DEVICE), y)
ifeq ($(CONFIG_CPU_CK804EF), y)
LDLIBS += $(ROOTDIR)/modules/libusb/ck804ef/libXUSBD.a
endif
ifeq ($(CONFIG_CPU_E804F), y)
LDLIBS += $(ROOTDIR)/modules/libusb/e804f/libXUSBD.a
endif
endif

ifeq ($(CONFIG_EXAMPLES_KERNEL_USB_HOST)$(CONFIG_BUILD_VAL_USB_HOST), y)
ifeq ($(CONFIG_CPU_CK804EF), y)
LDLIBS += $(ROOTDIR)/modules/libusb/ck804ef/libXUSBH.a
LDLIBS += $(ROOTDIR)/modules/libusb/ck804ef/libXFS.a
endif
ifeq ($(CONFIG_CPU_E804F), y)
LDLIBS += $(ROOTDIR)/modules/libusb/e804f/libXUSBH.a
LDLIBS += $(ROOTDIR)/modules/libusb/e804f/libXFS.a
endif
endif
ifeq ($(V),1)
Q =
else
Q = @
endif

export Q

.PHONY: all

all: mkconfig
	make $(NAME).elf

lib: lib_sub

S_SRC = $(wildcard $(SSRC))
C_SRC = $(wildcard $(CSRC))
CPP_SRC      = $(wildcard $(CPPSRC))
CORE_C_SRC   = $(wildcard $(CORE_CSRC))
DRIVER_S_SRC = $(wildcard $(DRIVER_SSRC))
DRIVER_C_SRC = $(wildcard $(DRIVER_CSRC))
KERNEL_S_SRC = $(wildcard $(KERNEL_SSRC))
KERNEL_C_SRC = $(wildcard $(KERNEL_CSRC))
LIB_S_SRC    = $(wildcard $(LIB_SSRC))
LIB_C_SRC    = $(wildcard $(LIB_CSRC))
CMSIS_S_SRC  = $(wildcard $(CMSIS_SSRC))
CMSIS_C_SRC  = $(wildcard $(CMSIS_CSRC))
MBEDOS_S_SRC = $(wildcard $(MBEDOS_SSRC))
MBEDOS_C_SRC = $(wildcard $(MBEDOS_CSRC))
MBEDOS_CPP_SRC = $(wildcard $(MBEDOS_CPPSRC))

DOBJECTS        = $(C_SRC:%.c=%.o) $(S_SRC:%.S=%.o) $(CPP_SRC:%.cpp=%.o)
CORE_DOBJECTS   = $(CORE_C_SRC:%.c=%.o)
DRIVER_DOBJECTS = $(DRIVER_C_SRC:%.c=%.o) $(DRIVER_S_SRC:%.S=%.o)
KERNEL_DOBJECTS = $(KERNEL_C_SRC:%.c=%.o) $(KERNEL_S_SRC:%.S=%.o)
LIB_DOBJECTS    = $(LIB_C_SRC:%.c=%.o) $(LIB_S_SRC:%.S=%.o)
CMSIS_DOBJECTS  = $(CMSIS_C_SRC:%.c=%.o) $(CMSIS_S_SRC:%.S=%.o)
MBEDOS_DOBJECTS = $(MBEDOS_C_SRC:%.c=%.o) $(MBEDOS_S_SRC:%.S=%.o) $(MBEDOS_CPP_SRC:%.cpp=%.o)

L_DEPS := $(DOBJECTS:%o=%d) $(CORE_DOBJECTS:%o=%d) $(DRIVER_DOBJECTS:%o=%d) $(KERNEL_DOBJECTS:%o=%d) $(LIB_DOBJECTS:%o=%d) \
          $(CMSIS_DOBJECTS:%o=%d) $(MBEDOS_DOBJECTS:%o=%d)

lib_sub: $(CORE_DOBJECTS) $(DRIVER_DOBJECTS) $(KERNEL_DOBJECTS) $(LIB_DOBJECTS) $(CMSIS_DOBJECTS) $(MBEDOS_DOBJECTS)
	$(Q)$(AR) $(ARFLAGS) $(OUTDIR)/$(PACKNAME) $(DRIVER_DOBJECTS)
	$(Q)$(AR) $(ARFLAGS) $(OUTDIR)/libcsi.a $(CORE_DOBJECTS) $(DRIVER_DOBJECTS) $(KERNEL_DOBJECTS) $(LIB_DOBJECTS)
	$(Q)$(AR) $(ARFLAGS) $(OUTDIR)/libcmsis.a $(CMSIS_DOBJECTS)
	$(Q)$(AR) $(ARFLAGS) $(OUTDIR)/libmbedos.a $(MBEDOS_DOBJECTS)

main_objs: $(DOBJECTS)

%.o:%.c
	@echo CC $<
	$(Q)$(CC) -MP -MMD $(CFLAGS)  -o $@ $<

%.o:%.S
	@echo AS $<
	$(Q)$(CC) -MP -MMD $(ASFLAGS) -o $@ $<

%.o:%.cpp
	@echo CPP $<
	$(Q)$(CPP) -MP -MMD $(CFLAGS)  -o $@ $<

sinclude $(L_DEPS)

ifeq ($(CONFIG_XIP), y)
LD_FILE = gcc_flash.ld
else
LD_FILE = gcc_mem.ld
endif

$(NAME).elf: lib_sub main_objs $(LDDIR)/$(LD_FILE)
	@echo LINK $@
	$(Q)$(CC) -nostdlib -T $(LDDIR)/$(LD_FILE) -o $(OUTDIR)/$(NAME).elf -Wl,-Map,$(OUTDIR)/$(NAME).map $(CKMAP) $(CPPLDFLAG_BEGIN) $(LDFLAGS) \
		$(DOBJECTS) $(EOBJECTS) $(CORE_DOBJECTS) $(DRIVER_DOBJECTS) $(KERNEL_DOBJECTS) $(LIB_DOBJECTS) \
		$(CMSIS_DOBJECTS) $(MBEDOS_DOBJECTS) \
		$(SUBDIR_FILES) $(LDLIBS) $(CPPLDFLAG_END) -nostartfiles
	@-cp $(DOBJECTS) $(OBJDIR)
	@echo DUMP $@ to $(NAME).asm
	$(Q)$(DUMP) -S $(OUTDIR)/$(NAME).elf > $(OUTDIR)/$(NAME).asm
	@echo OBJCOPY $@ to $(NAME).bin
	$(Q)$(OBJCOPY) -O binary $(OUTDIR)/$(NAME).elf $(OUTDIR)/$(NAME).bin
	$(Q)$(OBJCOPY) -O srec $(OUTDIR)/$(NAME).elf $(OUTDIR)/$(NAME).hex

.PHONY:hexfiles
hexfiles:
	@cp $(OUTDIR)/$(NAME).elf tools/hex/
	@sh tools/hex/imgmk.sh

.PHONY:clean
clean:
	rm -rf $(OUTDIR)
	find . -name "*.d" | xargs rm -f
	find . -name "*.o" | xargs rm -f
	rm -f include/csi_config.h

.PHONY:distclean
distclean: clean
	rm -f .config

.PHONY : debug-make

DEBUG_VARS = 	SUB_DIRS \
		SUBDIR_FILES\
		INCLUDEDIRS \
		CSRC \
		SSRC \
		EOBJECTS \
		DOBJECTS \
		ASFLAGS \
		CFLAGS
#:
#: debug-make -- Print a list of Makefile variables

debug-make:
	@$(foreach var, $(DEBUG_VARS), echo $(var)=$($(var)) ; )

mkconfig:.config
	sh ./tools/gendiffconfig.sh

./tools/kconfig-frontends/frontends/mconf/mconf:
	@./tools/mkmconf.sh

menuconfig: ./tools/kconfig-frontends/frontends/mconf/mconf
	@./tools/kconfig-frontends/frontends/mconf/mconf Kconfig

sdk:
	@gcc tools/sdk/list2cdkvd.c -o tools/sdk/list2cdkvd
	#@./tools/sdk/mksdk.sh $m $z $h $os
	@./tools/create_sdk.sh $m $z $h $os

template:
	@gcc tools/sdk/list2cdkvd.c -o tools/sdk/list2cdkvd
	@./tools/sdk/py_mksdk/mksdk.py m=$m h=$h

###############
tee_os:
	@make -C $(ROOTDIR)/csi/chips/$(CONFIG_CHIP_VENDOR_STR)/common/tee \
	TEEOS_LIB_PATH=$(ABSROOTDIR)/$(CHIPDIR)/tee/ \
	TEEOS_LIB=libtee_os_$(CONFIG_CHIP_NAME_STR).a \
	TARGETS_ROOT_PATH=$(TARGETS_ROOT_PATH)

help:
	@echo "==================== Make SDK ===================="
	@make sdk h=1 short_help=1 | grep -v "Entering directory\|Leaving directory"
