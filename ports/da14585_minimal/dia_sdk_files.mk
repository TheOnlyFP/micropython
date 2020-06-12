SKD_HEAD ?= ../../../6.0.12.1020.2/SDK/6.0.12.1020.2

DIA_DRIVERS += $(addprefix $(SKD_HEAD)/sdk/platform/driver/,\
	uart/uart.c \
 	dma/dma.c \
 	gpio/gpio.c \
 	uart/uart_utils.c \
	)


DIA_C_FILES += $(DIA_DRIVERS)
# DIA_C_FILES += $(DIA_PERIPH_EX_UART)

DIA_OBJ = $(PY_CORE_O) $(addprefix $(BUILD)/, $(DIA_C_FILES:.c=.o))