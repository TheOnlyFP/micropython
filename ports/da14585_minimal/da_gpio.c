#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "py/mpconfig.h"
#include "py/nlr.h"
#include "py/misc.h"
#include "py/qstr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/mpprint.h"

#include "uart.h"
#include "gpio.h"
#include "da_gpio.h"
#include "uart_utils.h"
#include "user_periph_setup.h"

#define BUFSIZE_UART1 25
#define BUFFSIZE_SMALL 1

const mp_obj_type_t machine_type;

//##############################################################
//CLASS TYPES
// this is the actual C-structure for our new object
typedef struct _mymodule_hello_obj_t {
    // base represents some basic information, like type
    mp_obj_base_t base;
    // a member created by us
    uint8_t hello_number;
    uint8_t pin;
    uint8_t port;
} mymodule_hello_obj_t;

//CLASS INIT METHODS
static void configure_pin(mymodule_hello_obj_t *self, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_mode, ARG_pull, ARG_value };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_mode, MP_ARG_OBJ, {.u_obj = MP_OBJ_NEW_SMALL_INT(INPUT_PULLDOWN)}},
        { MP_QSTR_pull, MP_ARG_OBJ, {.u_obj = MP_OBJ_NEW_SMALL_INT(-1)}},
        { MP_QSTR_value, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NEW_SMALL_INT(0)}},
    };

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    GPIO_ConfigurePin(self->port, self->pin, (GPIO_PUPD)mp_obj_get_int(args[ARG_mode].u_obj), PID_GPIO, (bool)mp_obj_get_int(args[ARG_value].u_obj));
}


STATIC mp_obj_t machine_hello_make_new( const mp_obj_type_t *type,
                                  size_t n_args,
                                  size_t n_kw,
                                  const mp_obj_t *args ) {
    // this checks the number of arguments (min 1, max 1);
    // on error -> raise python exception
    mp_arg_check_num(n_args, n_kw, 1, 1, true);
    // create a new object of our C-struct type
    mymodule_hello_obj_t *self = m_new_obj(mymodule_hello_obj_t);
    // give it a type
    self->base.type = &machine_type;
    // set the member number with the first argument of the constructor
    const char * pin_port;
    pin_port = mp_obj_str_get_str(args[0]);
    int ascii_zeroval = 48;
    self->port = (int)pin_port[0]- ascii_zeroval;
    self->pin = (int)pin_port[1] - ascii_zeroval;


    if (n_args > 1 || n_kw > 0) {
        // pin mode given, so configure this GPIO
        mp_map_t kw_args;
        mp_map_init_fixed_table(&kw_args, n_kw, args + n_args);
        configure_pin(self, n_args - 1, args + 1, &kw_args);
    }

    return MP_OBJ_FROM_PTR(self);
}


STATIC void machine_hello_print( const mp_print_t *print,
                                  mp_obj_t self_in,
                                  mp_print_kind_t kind ) {
    // get a ptr to the C-struct of the object
    mymodule_hello_obj_t *self = MP_OBJ_TO_PTR(self_in);
    // print the number
    char string[BUFSIZE_UART1];
    const char * start_string = "Port(%u) | Pin(%u)";
    snprintf(string, BUFSIZE_UART1, start_string, self->port, self->pin);
    printf_string(UART1, string);
}



//##############################################################
//CLASS
// creating the table of global members
STATIC const mp_rom_map_elem_t mymodule_hello_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_Pin) },

    //Constants
    { MP_OBJ_NEW_QSTR(MP_QSTR_OUT), MP_ROM_INT(OUTPUT) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PULL_UP), MP_ROM_INT(INPUT_PULLUP) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PULL_DOWN), MP_ROM_INT(INPUT_PULLDOWN) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_OUT), MP_ROM_INT(OUTPUT) },
};
STATIC MP_DEFINE_CONST_DICT(mymodule_hello_locals_dict,
                            mymodule_hello_locals_dict_table);

// create the class-object itself
const mp_obj_type_t machine_type = {
    // "inherit" the type "type"
    { &mp_type_type },
     // give it a name
    .name = MP_QSTR_helloObj,
     // give it a print-function
    .print = machine_hello_print,
     // give it a constructor
    .make_new = machine_hello_make_new,
     // and the global members
    .locals_dict = (mp_obj_dict_t*)&mymodule_hello_locals_dict,
};


// #############################################################
// module
STATIC mp_obj_t machine_hello(void) {
    printf_string(UART1, "BEEP BOOP I'm machine BEEP BOOP!\n\r");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_hello_obj, machine_hello);

STATIC const mp_map_elem_t machine_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_machine) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_machine), (mp_obj_t)&machine_hello_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Pin), (mp_obj_t)&machine_type },
};

STATIC MP_DEFINE_CONST_DICT ( mp_module_machine_globals, machine_globals_table );

const mp_obj_module_t mp_module_machine = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_machine_globals,
};