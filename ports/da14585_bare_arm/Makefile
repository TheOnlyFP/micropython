include ../../py/mkenv.mk

PYTHON=python
#DEBUG=1
V=1

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include $(TOP)/py/py.mk

CROSS_COMPILE ?= arm-none-eabi-

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)


CFLAGS_CORTEX_M = -mthumb -mabi=aapcs -fsingle-precision-constant -Wdouble-promotion
CFLAGS_CORTEX_M0 = $(CFLAGS_CORTEX_M) -fshort-enums -mtune=cortex-m0 -mcpu=cortex-m0 -mfloat-abi=soft -fno-builtin

# CFLAGS_CORTEX_M = -mthumb -fsingle-precision-constant -Wdouble-promotion
# CFLAGS_MCU_M0 = $(CFLAGS_CORTEX_M) -fshort-enums -mtune=cortex-m0 -mcpu=cortex-m0 -mfloat-abi=soft -fno-builtin -mabi=aapcs-linux
# CFLAGS_CORTEX_M4 = -mthumb -mtune=cortex-m0 -mabi=aapcs-linux -mcpu=cortex-m0 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant -Wdouble-promotion
CFLAGS = $(INC) -Wall -Werror -std=c99 -nostdlib $(CFLAGS_CORTEX_M0) $(COPT)
CSUPEROPT = -Os # save some code space

#Debugging/Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -O0 -ggdb
else
CFLAGS += -Os -DNDEBUG
endif

LDFLAGS = -nostdlib -T stm32f405_modified.ld -Map=$@.map --cref
# LDFLAGS = -nostdlib -T stm32f405_original.ld -Map=$@.map --cref
# LDFLAGS = -nostdlib -T da14585.ld -Map=$@.map --cref
LGCC_LOC = arm-none-eabi-gcc -print-libgcc-file-name
LIBS = "$(shell $(CC) $(CFLAGS) -print-libgcc-file-name)"

SRC_C = \
	main.c \
#	printf.c \
	string0.c \
	malloc0.c \
	gccollect.c \

SRC_S = \
#	startup_stm32f40xx.s \
	gchelper.s \

OBJ = $(PY_CORE_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o) $(SRC_S:.s=.o))

all: $(BUILD)/micropython.elf

$(BUILD)/micropython.elf: $(OBJ)
	$(ECHO) "LGCC_LOC $(LGCC_LOC)"
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

include $(TOP)/py/mkrules.mk
