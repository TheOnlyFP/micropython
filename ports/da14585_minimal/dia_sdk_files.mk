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
 	uart/uart_utils.c \
	)

# DIA_PERIPH_EX_UART += $(addprefix dialog_sdk/projects/target_apps/peripheral_examples/uart/,\
# 	src/user_periph_setup.c \
# 	)

DIA_C_FILES += $(DIA_DRIVERS)
# DIA_C_FILES += $(DIA_PERIPH_EX_UART)

DIA_OBJ = $(PY_CORE_O) $(addprefix $(BUILD)/, $(DIA_C_FILES:.c=.o))