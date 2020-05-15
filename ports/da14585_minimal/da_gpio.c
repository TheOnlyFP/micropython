#include "stdio.h"

#include "py/mpconfig.h"
#include "py/nlr.h"
#include "py/misc.h"
#include "py/qstr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/mpprint.h"

#include "uart.h"
#include "da_gpio.h"
#include "uart_utils.h"

#define BUFSIZE_UART1 20

//########################################################
// Add a type
const mp_obj_type_t mymodule_hello_type;

//########################################################
// Adding a class:
typedef struct _mymodule_hello_obj_t
{
    mp_obj_base_t base;         // base represents some basic information, like type
    uint8_t hello_number;       // a member created by us
} mymodule_hello_obj_t;


// creating the table of local members
STATIC const mp_rom_map_elem_t mymodule_hello_locals_dict_table[] = { };
STATIC MP_DEFINE_CONST_DICT(mymodule_hello_locals_dict, mymodule_hello_locals_dict_table);


mp_obj_t mymodule_hello_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args)
{
    // this checks the number of arguments (min 1, max 1);
    // on error -> raise python exception
    printf_string(UART1, "INIT\n\r");
    mp_arg_check_num(n_args, n_kw, 1, 1, true);
    // create a new object of our C-struct type
    mymodule_hello_obj_t *self = m_new_obj(mymodule_hello_obj_t);
    // give it a type
    self->base.type = &mymodule_hello_type;
    // set the member number with the first argument of the constructor
    self->hello_number = mp_obj_get_int(args[0]);
    char string[BUFSIZE_UART1];
    const char * start_string = "Hello(%u)";
    snprintf(string, BUFSIZE_UART1, start_string, self->hello_number);
    printf_string(UART1, string);
    return MP_OBJ_FROM_PTR(self);
}

void mymodule_hello_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind)
{
    printf_string(UART1, "\n\rPRINTING\n\r");
    // get a ptr to the C-struct of the object
    mymodule_hello_obj_t *self = MP_OBJ_TO_PTR(self_in);
    // print the number
    mp_printf(print, "Hello(%u)", self->hello_number);
    
    // char string[BUFSIZE_UART1];
    // const char * start_string = "Hello(%u)";
    // snprintf(string, BUFSIZE_UART1, start_string, self->hello_number);
    printf_string(UART1, "\n\rPRINTING\n\r");
    // printf_string(UART1, string);
}


// create the class-object itself
const mp_obj_type_t mymodule_helloObj =
{
    { &mp_type_type },                                          // "inherit" the type "type"
    .name = MP_QSTR_helloObj,                                   // give it a name
    .print = mymodule_hello_print,                              // give it a print-function
    .make_new = mymodule_hello_make_new,                        // give it a constructor
    .locals_dict = (mp_obj_dict_t*)&mymodule_hello_locals_dict, // and the local members
};

//########################################################
// Adding a function to module

STATIC mp_obj_t machine_hello(void)
{
    printf_string(UART1, "BEEP BOOP I'm machine BEEP BOOP!\n\r");
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_hello_obj, machine_hello);

// create a table of global strings, here the only one is name


//########################################################
//Create module

STATIC const mp_map_elem_t global_table[] =
{
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_machine) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_hello), (mp_obj_t)&machine_hello_obj},
    { MP_OBJ_NEW_QSTR(MP_QSTR_helloObj), (mp_obj_t)&mymodule_helloObj}
};

// create a dictionary of strings from the table
STATIC MP_DEFINE_CONST_DICT(mymodule_globals, global_table);

// fill the mp_obj_module_t struct, which defines the module itself
const mp_obj_module_t machine =
{
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mymodule_globals,
};

// mp_obj_t mp_pin_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
//     mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

//     if (n_args > 1 || n_kw > 0) {
//         // pin mode given, so configure this GPIO
//         mp_map_t kw_args;
//         mp_map_init_fixed_table(&kw_args, n_kw, args + n_args);
//         pin_obj_init_helper(pin, n_args - 1, args + 1, &kw_args);
//     }

//     return MP_OBJ_FROM_PTR(pin);
// }


// const mp_obj_type_t da_pin = {
//     { &mp_type_type },
//     .name = MP_QSTR_Pin,
//     .print = pin_print,
//     .make_new = mp_pin_make_new,
//     .call = pin_call,
//     .protocol = &pin_pin_p,
//     .locals_dict = (mp_obj_dict_t *)&pin_locals_dict,
// };