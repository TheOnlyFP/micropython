#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mpy_main.h"

#include "user_periph_setup.h"

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

#if MICROPY_ENABLE_COMPILER
void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, true);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}
#endif

static char *stack_top;
#if MICROPY_ENABLE_GC
static char heap[1024]; // 2048
#endif


int mpymain(int argc, char **argv) {
    int stack_dummy;
    stack_top = (char *)&stack_dummy;

    #if MICROPY_ENABLE_GC
    gc_init(heap, heap + sizeof(heap));
    #endif
    mp_init();
    #if MICROPY_ENABLE_COMPILER
    #if MICROPY_REPL_EVENT_DRIVEN
    pyexec_event_repl_init();
    for (;;) {
        int c = mp_hal_stdin_rx_chr();
        if (pyexec_event_repl_process_char(c)) {
            break;
        }
    }
    #else
        pyexec_friendly_repl();
    #endif
        do_str("print('hello world!', list(x+1 for x in range(10)), end='eol\\n')", MP_PARSE_SINGLE_INPUT);
        do_str("for i in range(10):\r\n  print(i)", MP_PARSE_FILE_INPUT);
    #else
        pyexec_frozen_module("frozentest.py");
    #endif
    mp_deinit();
    return 0;
}

void gc_collect(void) {
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    void *dummy;
    gc_collect_start();
    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info();
}

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    mp_raise_OSError(MP_ENOENT);
}

mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}

mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

void nlr_jump_fail(void *val) {
    while (1) {
        ;
    }
}

void NORETURN __fatal_error(const char *msg) {
    while (1) {
        ;
    }
}

#ifndef NDEBUG
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}
#endif

// Eye candy
void uart_send_blocking_example(uart_t* uart)
{
    printf_string(uart, "\n\r\n\r****************************************\n\r");
    printf_string(uart, "     Micropython Booting on da1458x  \n\r");
    printf_string(uart, "****************************************\n\r\n\r");
};

void da_system_init(void) {
    wdg_freeze();
}

void mpy_start(void) {
    da_system_init();
    periph_init();
    uart_periph_init(UART1);
    uart_enable(UART1);
    uart_send_blocking_example(UART1);
    mpymain(0, NULL);
}



// Calls the frozen python function based on the name passed 
// TODO Make it take variable amount of args (variable type) and parse to mpy 
void mpy_exec(char* string) {
    qstr qst = qstr_from_str(string); // finds Qstr in mpy environment from string
    mp_obj_t found = mp_load_global(qst); // loads that Qstr's object (value) into a mpy object
    mp_call_function_n_kw(found, 0, 0, NULL); //calls a the function matching the supplied:
    //| Qstr | X number of args | X number of keywords | List of all args / pointer to singular arg |
}

// void mpy_exec_pass_struct(char* string, struct gapc_connection_req_ind struct_arg) {
//     qstr qst = qstr_from_str(string);
//     mp_obj_t found = mp_load_global(qst);
//     mp_call_function_n_kw(found, 1, 0, struct_arg);
// }