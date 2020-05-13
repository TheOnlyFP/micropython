#include "gpio.h"
#include "py/obj.h"

typedef struct {
    mp_obj_base_t base;
    qstr name;
    uint8_t idx;
    uint8_t fn;
    uint8_t unit;
    uint8_t type;
    void *reg; // The peripheral associated with this AF
} pin_af_obj_t;

typedef struct {
    mp_obj_base_t base;
    qstr name;
    uint32_t port   : 4;
    uint32_t pin    : 5;    // Some ARM processors use 32 bits/PORT
    uint32_t num_af : 4;
    uint32_t adc_channel : 5; // Some ARM processors use 32 bits/PORT
    uint32_t adc_num  : 3;  // 1 bit per ADC
    uint32_t pin_mask;
    pin_gpio_t *gpio;
    const pin_af_obj_t *af;
} pin_obj_t;

#define IS_GPIO_MODE(MODE) (((MODE) == INPUT) || \
    ((MODE) == INPUT_PULLUP) || \
    ((MODE) == INPUT_PULLDOWN) || \
    ((MODE) == OUTPUT)

#define IS_GPIO_PULL(PULL) (((PULL) == INPUT_PULLDOWN) || ((PULL) == GPIO_PULLUP) || \
    ((PULL) == GPIO_PULLDOWN))