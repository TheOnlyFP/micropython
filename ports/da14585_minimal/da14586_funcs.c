#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "da14586_funcs.h"
// SDK CODE V

void GPIO_SetPinFunction(GPIO_PORT port, GPIO_PIN pin, GPIO_PUPD mode, GPIO_FUNCTION function)
{
    const int data_reg = GPIO_BASE + (port << 5);
    const int mode_reg = data_reg + 0x6 + (pin << 1);

    SetWord16(mode_reg, mode | function);
}

void GPIO_SetActive(GPIO_PORT port, GPIO_PIN pin)
{
    const int data_reg = GPIO_BASE + (port << 5);
    const int set_data_reg = data_reg + 2;

    SetWord16(set_data_reg, 1 << pin);
}

void GPIO_SetInactive(GPIO_PORT port, GPIO_PIN pin)
{
    const int data_reg = GPIO_BASE + (port << 5);
    const int reset_data_reg = data_reg + 4;

    SetWord16(reset_data_reg, 1 << pin);
}

void GPIO_ConfigurePin(GPIO_PORT port, GPIO_PIN pin, GPIO_PUPD mode, GPIO_FUNCTION function,
                        const bool high)
{
    if (high)
        GPIO_SetActive( port, pin );
    else
        GPIO_SetInactive( port, pin );

    GPIO_SetPinFunction( port, pin, mode, function );
}

void wdg_freeze(void)
{
    // Freeze WDOG   
    SetWord16(SET_FREEZE_REG, FRZ_WDOG);  
}