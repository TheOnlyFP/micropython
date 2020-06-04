##### INCLUDE PATHS #####

SDK_TOP := ./dialog_sdk/sdk
PROJ_TOP :=./dialog_sdk/projects

SDK_PLATFORM :=$(SDK_TOP)/platform
SDK_INCLUDE :=$(SDK_PLATFORM)/include

## COMMON PROJECT FILES
SDK_COMMON_PROJ_FILES :=$(SDK_TOP)/common_project_files


## DRIVERS

SDK_DRIVER :=$(SDK_PLATFORM)/driver

SDK_GPIO :=$(SDK_DRIVER)/gpio
SDK_UART :=$(SDK_DRIVER)/uart
SDK_DMA :=$(SDK_DRIVER)/dma
# SDK_SYSCNTL :=$(SDK_DRIVER)/syscntl # Might be needed in later SDK

SDK_SPI :=$(SDK_DRIVER)/spi
SDK_SPI_FLASH :=$(SDK_DRIVER)/spi_flash

SDK_I2C :=$(SDK_DRIVER)/i2c
SDK_I2C_EEPROM :=$(SDK_DRIVER)/i2c_eeprom

## CORE_MODULES

SDK_CORE_MODULES :=$(SDK_PLATFORM)/core_modules

SDK_RWIP :=$(SDK_CORE_MODULES)/rwip
SDK_RWIP_API :=$(SDK_RWIP)/api

SDK_KE :=$(SDK_CORE_MODULES)/ke
SDK_KE_API :=$(SDK_KE)/api

SDK_CORE_COMMON :=$(SDK_CORE_MODULES)/common
SDK_COMMON_API :=$(SDK_CORE_COMMON)/api


## BLE_STACK

BLE_STACK :=$(SDK_TOP)/ble_stack

BLE_HOST :=$(BLE_STACK)/host

BLE_GAP :=$(BLE_HOST)/gap

BLE_GAPC :=$(BLE_GAP)/gapc

BLE_RWBLE :=$(BLE_STACK)/rwble

BLE_RWBLE_HL :=$(BLE_STACK)/rwble_hl

BLE_SMP :=$(BLE_HOST)/smp

BLE_SMPM :=$(BLE_SMP)/smpm


BLE_PROFILES :=$(BLE_STACK)/profiles
BLE_SOUTA :=$(BLE_PROFILES)/suota
BLE_SOUTAR :=$(BLE_SOUTA)/suotar

BLE_SOUTAR_API :=$(BLE_SOUTAR)/api

## ARCH DEPENDENT

SDK_ARCH :=$(SDK_PLATFORM)/arch

SDK_ARCH_MAIN :=$(SDK_ARCH)/main
SDK_COMPILER :=$(SDK_ARCH)/compiler
SDK_LL :=$(SDK_ARCH)/ll

## CMSIS
SDK_CMSIS :=$(SDK_INCLUDE)/CMSIS

SDK_CMSIS_INCLUDE :=$(SDK_CMSIS)/5.6.0/include

## PROJECT CONFIGURATIONS
SDK_TARGET_APPS :=$(PROJ_TOP)/target_apps

SDK_BLE_EX :=$(SDK_TARGET_APPS)/ble_examples

## USER_ALL_IN_ONE
# SDK_USER_ALL_IN_ONE :=$(SDK_BLE_EX)/ble_app_all_in_one
# SDK_USER_ALL_IN_ONE_SRC :=$(SDK_USER_ALL_IN_ONE)/src

## PROX REPORTER

SDK_PROX_REP :=$(SDK_BLE_EX)/prox_reporter
SDK_PROX_REP_SRC :=$(SDK_PROX_REP)/src
SDK_PROX_REP_CONFIG :=$(SDK_PROX_REP_SRC)/config
SDK_PROX_REP_PLATFORM :=$(SDK_PROX_REP_SRC)/platform

## UART PERIPHERAL_EXAMPLE

SDK_PER_EX_UART :=$(SDK_TARGET_APPS)/peripheral_examples/uart
SDK_PER_EX_UART_INCLUDE :=$(SDK_PER_EX_UART)/include
SDK_PER_EX_UART_SRC := $(SDK_PER_EX_UART)/src
SDK_PER_EX_UART_CONFIG := $(SDK_PER_EX_UART_SRC)/config

##### COMPILER INCUDES #####

SDK_INC = $(SDK_INCLUDE) # No -I here as the main Makefile has it

## COMMON PROJECT FILES

SDK_INC += -I$(SDK_COMMON_PROJ_FILES)


## DRIVERS

SDK_INC += -I$(SDK_GPIO)
SDK_INC += -I$(SDK_UART)
SDK_INC += -I$(SDK_DMA)
# SDK_INC += -I$(SDK_SYSCNTL)
SDK_INC += -I$(SDK_SPI)
SDK_INC += -I$(SDK_SPI_FLASH)
SDK_INC += -I$(SDK_I2C)
SDK_INC += -I$(SDK_I2C_EEPROM)

## CORE_MODULES

SDK_INC += -I$(SDK_RWIP_API)
SDK_INC += -I$(SDK_KE_API)
SDK_INC += -I$(SDK_COMMON_API)

## BLE_STACK

SDK_INC += -I$(BLE_GAP)
SDK_INC += -I$(BLE_GAPC)
SDK_INC += -I$(BLE_RWBLE)
SDK_INC += -I$(BLE_RWBLE_HL)
SDK_INC += -I$(BLE_SMP)
SDK_INC += -I$(BLE_SMPM)
SDK_INC += -I$(BLE_SOUTAR_API)

## ARCH

SDK_INC += -I$(SDK_ARCH)
SDK_INC += -I$(SDK_ARCH_MAIN)
SDK_INC += -I$(SDK_COMPILER)
SDK_INC += -I$(SDK_LL)

## CMSIS

SDK_INC += -I$(SDK_CMSIS_INCLUDE)


### EXAMPLE PROJECT
## USER_ALL_IN_ONE
# SDK_INC += -I$(SDK_USER_ALL_IN_OsNE_SRC)


## UART PERIPHERAL_EXAMPLE
SDK_INC += -I$(SDK_PER_EX_UART_INCLUDE)
SDK_INC += -I$(SDK_PER_EX_UART_SRC)

# PROX_REPORTER
SDK_INC += -I$(SDK_PROX_REP_SRC)
SDK_INC += -I$(SDK_PROX_REP_CONFIG)
SDK_INC += -I$(SDK_PROX_REP_PLATFORM)