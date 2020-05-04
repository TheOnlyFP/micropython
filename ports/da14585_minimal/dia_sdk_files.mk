# SDK_PATH := ports/da14585_minimal/dialog_sdk
# SDK_PLATFORM := $(SDK_PATH)/platform
# SDK_DRIVER := $(SDK_PLATFORM)/driver

# SRC_C += \
# 	$(SDK_DRIVER)/uart/uart.c \
# 	$(SDK_DRIVER)/dma/dma.c \
# 	$(SDK_DRIVER)/gpio/gpio.c \

DIA_DRIVERS += $(addprefix dialog_sdk/sdk/platform/driver/,\
	uart/uart.c \
 	dma/dma.c \
 	gpio/gpio.c \
	)

DIA_OBJ = $(PY_CORE_O) $(addprefix $(BUILD)/, $(DIA_DRIVERS:.c=.o))