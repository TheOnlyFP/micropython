#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "lib/utils/pyexec.h"

// DIALOG SDK INCLUDES
#include "uart.h"
#include "uart_utils.h"
#include "gpio.h"
#include "arch_wdg.h"
#include "spi.h"
#include "spi_flash.h"

void mpy_start(void);

void do_str(const char *src, mp_parse_input_kind_t input_kind);

int be_main(int argc, char **argv);

void gc_collect(void);

mp_lexer_t *mp_lexer_new_from_file(const char *filename);
mp_import_stat_t mp_import_stat(const char *path);
mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs);
void nlr_jump_fail(void *val);
void NORETURN __fatal_error(const char *msg);
#ifndef NDEBUG
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr);
#endif
void Reset_Handler(void) __attribute__((naked));
void Default_Handler(void);
void uart_send_blocking_example(uart_t* uart);
