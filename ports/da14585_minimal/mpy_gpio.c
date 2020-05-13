/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "gpio.h"

// pin.init(mode, pull=None, *, value)
STATIC mp_obj_t pyb_pin_obj_init_helper(pyb_pin_obj_t *self, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_mode, ARG_pull, ARG_value };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_mode, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_pull, MP_ARG_OBJ, {.u_obj = mp_const_none}},
        { MP_QSTR_value, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL}},
    };

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // get io mode
    uint mode = args[ARG_mode].u_int;

    // get pull mode
    uint pull = GPIO_PULL_NONE;
    if (args[ARG_pull].u_obj != mp_const_none) {
        pull = mp_obj_get_int(args[ARG_pull].u_obj);
    }

    // get initial value
    int value;
    if (args[ARG_value].u_obj == MP_OBJ_NULL) {
        value = -1;
    } else {
        value = mp_obj_is_true(args[ARG_value].u_obj);
    }

    // save the mode
    pin_mode[self->phys_port] = mode;

    // configure the GPIO as requested
    if (self->phys_port == 16) {
        // only pull-down seems to be supported by the hardware, and
        // we only expose pull-up behaviour in software
        if (pull != GPIO_PULL_NONE) {
            mp_raise_ValueError("Pin(16) doesn't support pull");
        }
    } else {
        PIN_FUNC_SELECT(self->periph, self->func);
        #if 0
        // Removed in SDK 1.1.0
        if ((pull & GPIO_PULL_DOWN) == 0) {
            PIN_PULLDWN_DIS(self->periph);
        }
        #endif
        if ((pull & GPIO_PULL_UP) == 0) {
            PIN_PULLUP_DIS(self->periph);
        }
        #if 0
        if ((pull & GPIO_PULL_DOWN) != 0) {
            PIN_PULLDWN_EN(self->periph);
        }
        #endif
        if ((pull & GPIO_PULL_UP) != 0) {
            PIN_PULLUP_EN(self->periph);
        }
    }

    pin_set(self->phys_port, value);

    return mp_const_none;
}

// pin.init(mode, pull)
STATIC mp_obj_t pyb_pin_obj_init(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    return pyb_pin_obj_init_helper(args[0], n_args - 1, args + 1, kw_args);
}
MP_DEFINE_CONST_FUN_OBJ_KW(pyb_pin_init_obj, 1, pyb_pin_obj_init);


STATIC mp_uint_t pin_ioctl(mp_obj_t self_in, mp_uint_t request, uintptr_t arg, int *errcode);
STATIC mp_uint_t pin_ioctl(mp_obj_t self_in, mp_uint_t request, uintptr_t arg, int *errcode) {
    (void)errcode;
    pyb_pin_obj_t *self = self_in;

    switch (request) {
        case MP_PIN_READ: {
            return pin_get(self->phys_port);
        }
        case MP_PIN_WRITE: {
            pin_set(self->phys_port, arg);
            return 0;
        }
    }
    return -1;
}

STATIC const mp_rom_map_elem_t pyb_pin_locals_dict_table[] = {
    // instance methods
    { MP_ROM_QSTR(MP_QSTR_init),    MP_ROM_PTR(&pyb_pin_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_value),   MP_ROM_PTR(&pyb_pin_value_obj) },
    { MP_ROM_QSTR(MP_QSTR_off),     MP_ROM_PTR(&pyb_pin_off_obj) },
    { MP_ROM_QSTR(MP_QSTR_on),      MP_ROM_PTR(&pyb_pin_on_obj) },
    { MP_ROM_QSTR(MP_QSTR_irq),     MP_ROM_PTR(&pyb_pin_irq_obj) },

    // class constants
    { MP_ROM_QSTR(MP_QSTR_IN),        MP_ROM_INT(GPIO_MODE_INPUT) },
    { MP_ROM_QSTR(MP_QSTR_OUT),       MP_ROM_INT(GPIO_MODE_OUTPUT) },
    { MP_ROM_QSTR(MP_QSTR_OPEN_DRAIN), MP_ROM_INT(GPIO_MODE_OPEN_DRAIN) },
    { MP_ROM_QSTR(MP_QSTR_PULL_UP),   MP_ROM_INT(GPIO_PULL_UP) },
    //{ MP_ROM_QSTR(MP_QSTR_PULL_DOWN), MP_ROM_INT(GPIO_PULL_DOWN) },

    // IRQ triggers, can be or'd together
    { MP_ROM_QSTR(MP_QSTR_IRQ_RISING), MP_ROM_INT(GPIO_PIN_INTR_POSEDGE) },
    { MP_ROM_QSTR(MP_QSTR_IRQ_FALLING), MP_ROM_INT(GPIO_PIN_INTR_NEGEDGE) },
};

STATIC MP_DEFINE_CONST_DICT(pyb_pin_locals_dict, pyb_pin_locals_dict_table);

STATIC const mp_pin_p_t pin_pin_p = {
    .ioctl = pin_ioctl,
};

const mp_obj_type_t pyb_pin_type = {
    { &mp_type_type },
    .name = MP_QSTR_Pin,
    .print = pyb_pin_print,
    .make_new = mp_pin_make_new,
    .call = pyb_pin_call,
    .protocol = &pin_pin_p,
    .locals_dict = (mp_obj_dict_t *)&pyb_pin_locals_dict,
};