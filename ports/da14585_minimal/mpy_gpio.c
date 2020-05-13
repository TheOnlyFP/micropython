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

// Required defs
#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"

#include "gpio.h"
#include "mpy_gpio.h"


// init(mode, pull=None, af=-1, *, value, alt)
STATIC mp_obj_t pin_obj_init_helper(const pin_obj_t *self, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_mode, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_pull, MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE}},
        { MP_QSTR_value, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL}},
        { MP_QSTR_alt, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1}},
    };

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // get io mode
    uint mode = args[0].u_int;
    if (!IS_GPIO_MODE(mode)) {
        mp_raise_msg_varg(&mp_type_ValueError, "invalid pin mode: %d", mode);
    }

    // get pull mode
    uint pull = GPIO_NOPULL;
    if (args[1].u_obj != mp_const_none) {
        pull = mp_obj_get_int(args[1].u_obj);
    }
    if (!IS_GPIO_PULL(pull)) {
        mp_raise_msg_varg(&mp_type_ValueError, "invalid pin pull: %d", pull);
    }

    // get af (alternate function); alt-arg overrides af-arg
    mp_int_t af = args[4].u_int;
    if (af == -1) {
        af = args[2].u_int;
    }
    if ((mode == GPIO_MODE_AF_PP || mode == GPIO_MODE_AF_OD) && !IS_GPIO_AF(af)) {
        mp_raise_msg_varg(&mp_type_ValueError, "invalid pin af: %d", af);
    }

    // enable the peripheral clock for the port of this pin
    mp_hal_gpio_clock_enable(self->gpio);

    GPIO_ConfigurePin(GPIO_PORT port, GPIO_PIN pin, GPIO_PUPD mode, GPIO_FUNCTION function,
                        const bool high)

    // if given, set the pin value before initialising to prevent glitches
    if (args[3].u_obj != MP_OBJ_NULL) {
        mp_hal_pin_write(self, mp_obj_is_true(args[3].u_obj));
    }

    // configure the GPIO as requested
    // GPIO_InitTypeDef GPIO_InitStructure;
    // GPIO_InitStructure.Pin = self->pin_mask;
    // GPIO_InitStructure.Mode = mode;
    // GPIO_InitStructure.Pull = pull;
    // GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    // GPIO_InitStructure.Alternate = af;
    // HAL_GPIO_Init(self->gpio, &GPIO_InitStructure);

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
    { MP_ROM_QSTR(MP_QSTR_IN),        MP_ROM_INT(INPUT) },
    { MP_ROM_QSTR(MP_QSTR_OUT),       MP_ROM_INT(OUTPUT) },
    { MP_ROM_QSTR(MP_QSTR_PULL_UP),   MP_ROM_INT(INPUT_PULLUP) },
    { MP_ROM_QSTR(MP_QSTR_PULL_DOWN), MP_ROM_INT(INPUT_PULLDOWN) },

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