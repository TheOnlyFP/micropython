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
}

// Send string of given length -- replace this?!
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    while (len--) {
        uart_write_byte(UART1, *str++);
    }
}
