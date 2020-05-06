#include <unistd.h>
#include "py/mpconfig.h"
#include "uart.h"

/*
 * Core UART functions to implement for a port
 */

// Receive single character
int mp_hal_stdin_rx_chr(void) {

    unsigned char c = 0;

    c = uart_read_byte(UART1);

    return c;

    // unsigned char c = 0;
    // #if MICROPY_MIN_USE_STDOUT
    // int r = read(STDIN_FILENO, &c, 1);
    // (void)r;
    // #elif MICROPY_MIN_USE_STM32_MCU
    // // wait for RXNE
    // while ((USART1->SR & (1 << 5)) == 0) {
    // }
    // c = USART1->DR;
    // #endif
    // return c;
}

// Send string of given length -- replace this?!
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {

    while (len--) {
        uart_write_byte(UART1, *str++);
    }

    // printf_string(UART1, *str_ptr); //-Werror=discarded-qualifiers TODO
    // #if MICROPY_MIN_USE_STDOUT
    // int r = write(STDOUT_FILENO, str, len);
    // (void)r;
    // #elif MICROPY_MIN_USE_STM32_MCU
    // while (len--) {
    //     // wait for TXE
    //     while ((USART1->SR & (1 << 7)) == 0) {
    //     }
    //     USART1->DR = *str++;
    // }
    // #endif
}
