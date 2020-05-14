#include "py/mpconfig.h"
#include "py/nlr.h"
#include "py/misc.h"
#include "py/qstr.h"
#include "py/obj.h"
#include "py/runtime.h"

#include "da_gpio.h"
#include "uart_utils.h"

// create a table of global strings, here the only one is name

STATIC const mp_map_elem_t global_table[] =
{
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_machine) },
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