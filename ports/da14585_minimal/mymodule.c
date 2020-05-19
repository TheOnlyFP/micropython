// Is there any documentation regarding the addition of classes and their properties in micropython?
// The closest I was able to find was this: https://github.com/deshipu/micropython-dev-docs/blob/master/adding-module.rst 
// But I'm unable to make the print method function correctly with it only returning / printing "<>".


#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/binary.h"
#include "py/mpconfig.h"

#include "stdio.h"
#include "uart.h"
#include "uart_utils.h"

#define BUFSIZE_UART1 20
const mp_obj_type_t mymodule_helloObj_type;

//##############################################################
//CLASS TYPES
// this is the actual C-structure for our new object
typedef struct _mymodule_hello_obj_t {
    // base represents some basic information, like type
    mp_obj_base_t base;
    // a member created by us
    uint8_t hello_number;
} mymodule_hello_obj_t;

//CLASS INIT METHODS
mp_obj_t mymodule_hello_make_new( const mp_obj_type_t *type,
                                  size_t n_args,
                                  size_t n_kw,
                                  const mp_obj_t *args ) {
    // this checks the number of arguments (min 1, max 1);
    // on error -> raise python exception
    mp_arg_check_num(n_args, n_kw, 1, 1, true);
    // create a new object of our C-struct type
    mymodule_hello_obj_t *self = m_new_obj(mymodule_hello_obj_t);
    // give it a type
    self->base.type = &mymodule_helloObj_type;
    // set the member number with the first argument of the constructor
    self->hello_number = mp_obj_get_int(args[0]);
    return MP_OBJ_FROM_PTR(self);
}


STATIC void mymodule_hello_print( const mp_print_t *print,
                                  mp_obj_t self_in,
                                  mp_print_kind_t kind ) {
    // get a ptr to the C-struct of the object
    mymodule_hello_obj_t *self = MP_OBJ_TO_PTR(self_in);
    // print the number
    char string[BUFSIZE_UART1];
    const char * start_string = "Hello(%u)";
    snprintf(string, BUFSIZE_UART1, start_string, self->hello_number);
    printf_string(UART1, string);
}



//##############################################################
//CLASS
// creating the table of global members
STATIC const mp_rom_map_elem_t mymodule_hello_locals_dict_table[] = { };
STATIC MP_DEFINE_CONST_DICT(mymodule_hello_locals_dict,
                            mymodule_hello_locals_dict_table);

// create the class-object itself
const mp_obj_type_t mymodule_helloObj_type = {
    // "inherit" the type "type"
    { &mp_type_type },
     // give it a name
    .name = MP_QSTR_helloObj,
     // give it a print-function
    .print = mymodule_hello_print,
     // give it a constructor
    .make_new = mymodule_hello_make_new,
     // and the global members
    .locals_dict = (mp_obj_dict_t*)&mymodule_hello_locals_dict,
};


// #############################################################
// module
STATIC mp_obj_t mymodule_hello(void) {
    printf_string(UART1, "BEEP BOOP I'm machine BEEP BOOP!\n\r");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mymodule_hello_obj, mymodule_hello);

STATIC const mp_map_elem_t mymodule_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_mymodule) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_hello), (mp_obj_t)&mymodule_hello_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_helloObj), (mp_obj_t)&mymodule_helloObj_type },
};

STATIC MP_DEFINE_CONST_DICT (
    mp_module_mymodule_globals,
    mymodule_globals_table
);

const mp_obj_module_t mp_module_mymodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_mymodule_globals,
};


