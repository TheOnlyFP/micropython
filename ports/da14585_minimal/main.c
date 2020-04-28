#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "lib/utils/pyexec.h"

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
static char heap[2048];
#endif

int main(int argc, char **argv) {
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

#if MICROPY_MIN_USE_CORTEX_CPU

// this is a minimal IRQ and reset framework for any Cortex-M CPU

extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;

void Reset_Handler(void) __attribute__((naked));
void Reset_Handler(void) {
    // set stack pointer
    //__asm volatile ("ldr sp, =_estack");
    // copy .data section from flash to RAM
    for (uint32_t *src = &_sidata, *dest = &_sdata; dest < &_edata;) {
        *dest++ = *src++;
    }
    // zero out .bss section
    for (uint32_t *dest = &_sbss; dest < &_ebss;) {
        *dest++ = 0;
    }
    // jump to board initialisation
    void _start(void);
    _start();
}

void Default_Handler(void) {
    for (;;) {
    }
}

const uint32_t isr_vector[] __attribute__((section(".isr_vector"))) = {
    (uint32_t)&_estack,
    (uint32_t)&Reset_Handler,
    (uint32_t)&Default_Handler, // NMI_Handler
    (uint32_t)&Default_Handler, // HardFault_Handler
    (uint32_t)&Default_Handler, // MemManage_Handler
    (uint32_t)&Default_Handler, // BusFault_Handler
    (uint32_t)&Default_Handler, // UsageFault_Handler
    0,
    0,
    0,
    0,
    (uint32_t)&Default_Handler, // SVC_Handler
    (uint32_t)&Default_Handler, // DebugMon_Handler
    0,
    (uint32_t)&Default_Handler, // PendSV_Handler
    (uint32_t)&Default_Handler, // SysTick_Handler
};

void _start(void) {
    // when we get here: stack is initialised, bss is clear, data is copied

    // SCB->CCR: enable 8-byte stack alignment for IRQ handlers, in accord with EABI
    *((volatile uint32_t *)0xe000ed14) |= 1 << 9;

    // initialise the cpu and peripherals
    #if MICROPY_MIN_USE_STM32_MCU
    void stm32_init(void);
    stm32_init();
    #endif

    // now that we have a basic system up and running we can call main
    main(0, NULL);

    // we must not return
    for (;;) {
    }
}

#endif

#if MICROPY_MIN_USE_STM32_MCU

// this is minimal set-up code for an STM32 MCU

// typedef struct {
//     volatile uint32_t CR;
//     volatile uint32_t PLLCFGR;
//     volatile uint32_t CFGR;
//     volatile uint32_t CIR;
//     uint32_t _1[8];
//     volatile uint32_t AHB1ENR;
//     volatile uint32_t AHB2ENR;
//     volatile uint32_t AHB3ENR;
//     uint32_t _2;
//     volatile uint32_t APB1ENR;
//     volatile uint32_t APB2ENR;
// } periph_rcc_t;

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint16_t BSRRL;
    volatile uint16_t BSRRH;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} periph_gpio_t;

typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
} periph_uart_t;

#define USART1 ((periph_uart_t *)0x50001000)



#define GPIO_BASE  0x50003000
#define GPIOA  ((periph_gpio_t *)0x50003028)
// #define GPIOB  ((periph_gpio_t *)0x40020400)
// #define RCC    ((periph_rcc_t *)0x40023800)

// simple GPIO interface
#define GPIO_MODE_IN (0)
#define GPIO_MODE_OUT (1)
#define GPIO_MODE_ALT (2)
#define GPIO_PULL_NONE (0)
#define GPIO_PULL_UP (0)
#define GPIO_PULL_DOWN (1)

#define FUNC_GPIO          ( 0)
#define FUNC_UART_RX       ( 1)
#define FUNC_UART_TX       ( 2)
#define FUNC_UART2_RX      ( 3)
#define FUNC_UART2_TX      ( 4)
#define FUNC_SPI_DI        ( 5)
#define FUNC_SPI_DO        ( 6)
#define FUNC_SPI_CLK       ( 7)
#define FUNC_SPI_EN        ( 8)
#define FUNC_I2C_SCL       ( 9)
#define FUNC_I2C_SDA       (10)
#define FUNC_UART_IRDA_RX  (11)
#define FUNC_UART_IRDA_TX  (12)
#define FUNC_UART2_IRDA_RX (13)
#define FUNC_UART2_IRDA_TX (14)
#define FUNC_ADC           (15)
#define FUNC_PWM0          (16)
#define FUNC_PWM1          (17)
#define FUNC_BLE_DIAG      (18)
#define FUNC_UART_CTSN     (19)
#define FUNC_UART_RTSN     (20)
#define FUNC_UART2_CTSN    (21)
#define FUNC_UART2_RTSN    (22)
#define FUNC_PWM2          (23)
#define FUNC_PWM3          (24)
#define FUNC_PWM4          (25)
#define FUNC_PCM_DI        (26)
#define FUNC_PCM_DO        (27)
#define FUNC_PCM_FSC       (28)
#define FUNC_PCM_CLK       (29)
#define FUNC_PDM_DATA      (30)
#define FUNC_PDM_CLK       (31)

typedef enum {
    GPIO_PIN_0 = 0,
    GPIO_PIN_1 = 1,
    GPIO_PIN_2 = 2,
    GPIO_PIN_3 = 3,
    GPIO_PIN_4 = 4,
    GPIO_PIN_5 = 5,
    GPIO_PIN_6 = 6,
    GPIO_PIN_7 = 7,
    GPIO_PIN_8 = 8,
    GPIO_PIN_9 = 9,
} GPIO_PIN;

typedef enum {
    GPIO_PORT_0 = 0,
    GPIO_PORT_1 = 1,
    GPIO_PORT_2 = 2,
    GPIO_PORT_3 = 3,
    GPIO_PORT_3_REMAP = 4,
} GPIO_PORT;

typedef enum {
    INPUT = 0,
    INPUT_PULLUP = 0x100,
    INPUT_PULLDOWN = 0x200,
    OUTPUT = 0x300,
} GPIO_PUPD;

typedef enum {
    PID_GPIO = FUNC_GPIO,
    PID_UART1_RX = FUNC_UART_RX,
    PID_UART1_TX = FUNC_UART_TX,
    PID_UART2_RX = FUNC_UART2_RX,
    PID_UART2_TX = FUNC_UART2_TX,
    PID_SPI_DI = FUNC_SPI_DI,
    PID_SPI_DO = FUNC_SPI_DO,
    PID_SPI_CLK = FUNC_SPI_CLK,
    PID_SPI_EN = FUNC_SPI_EN,
    PID_I2C_SCL = FUNC_I2C_SCL,
    PID_I2C_SDA = FUNC_I2C_SDA,
    PID_UART1_IRDA_RX = FUNC_UART_IRDA_RX,
    PID_UART1_IRDA_TX = FUNC_UART_IRDA_TX,
    PID_UART2_IRDA_RX = FUNC_UART2_IRDA_RX,
    PID_UART2_IRDA_TX = FUNC_UART2_IRDA_TX,
    PID_ADC = FUNC_ADC,
    PID_PWM0 = FUNC_PWM0,
    PID_PWM1 = FUNC_PWM1,
    PID_BLE_DIAG = FUNC_BLE_DIAG,
    PID_UART1_CTSN = FUNC_UART_CTSN,
    PID_UART1_RTSN = FUNC_UART_RTSN,
    PID_UART2_CTSN = FUNC_UART2_CTSN,
    PID_UART2_RTSN = FUNC_UART2_RTSN,
    PID_PWM2 = FUNC_PWM2,
    PID_PWM3 =FUNC_PWM3,
    PID_PWM4 = FUNC_PWM4,
    PID_PCM_DI=FUNC_PCM_DI,
    PID_PCM_DO = FUNC_PCM_DO,
    PID_PCM_FSC = FUNC_PCM_FSC,
    PID_PCM_CLK = FUNC_PCM_CLK,
    PID_PDM_DATA = FUNC_PDM_DATA,
    PID_PDM_CLK = FUNC_PDM_CLK,
} GPIO_FUNCTION;

#define SetWord16(a,d) (* ( volatile uint16_t*)(a)=(d) )

void GPIO_SetPinFunction(GPIO_PORT port, GPIO_PIN pin, GPIO_PUPD mode, GPIO_FUNCTION function)
{
    const int data_reg = GPIO_BASE + (port << 5);
    const int mode_reg = data_reg + 0x6 + (pin << 1);

    SetWord16(mode_reg, mode | function);
}

void GPIO_SetActive(GPIO_PORT port, GPIO_PIN pin)
{
    const int data_reg = GPIO_BASE + (port << 5);
    const int set_data_reg = data_reg + 2;

    SetWord16(set_data_reg, 1 << pin);
}

void GPIO_SetInactive(GPIO_PORT port, GPIO_PIN pin)
{
    const int data_reg = GPIO_BASE + (port << 5);
    const int reset_data_reg = data_reg + 4;

    SetWord16(reset_data_reg, 1 << pin);
}

void GPIO_ConfigurePin(GPIO_PORT port, GPIO_PIN pin, GPIO_PUPD mode, GPIO_FUNCTION function,
                        const bool high)
{
    if (high)
        GPIO_SetActive( port, pin );
    else
        GPIO_SetInactive( port, pin );

    GPIO_SetPinFunction( port, pin, mode, function );
}

// void gpio_init(periph_gpio_t *gpio, int pin, int mode, int pull, int alt) {
//     gpio->MODER = (gpio->MODER & ~(3 << (2 * pin))) | (mode << (2 * pin));
//     // OTYPER is left as default push-pull
//     // OSPEEDR is left as default low speed
//     gpio->PUPDR = (gpio->PUPDR & ~(3 << (2 * pin))) | (pull << (2 * pin));
//     gpio->AFR[pin >> 3] = (gpio->AFR[pin >> 3] & ~(15 << (4 * (pin & 7)))) | (alt << (4 * (pin & 7)));
// }
// #define gpio_get(gpio, pin) ((gpio->IDR >> (pin)) & 1)
// #define gpio_set(gpio, pin, value) do { gpio->ODR = (gpio->ODR & ~(1 << (pin))) | (value << pin); } while (0)
// #define gpio_low(gpio, pin) do { gpio->BSRRH = (1 << (pin)); } while (0)
// #define gpio_high(gpio, pin) do { gpio->BSRRL = (1 << (pin)); } while (0)

void stm32_init(void) {
    // basic MCU config
    // RCC->CR |= (uint32_t)0x00000001; // set HSION
    // RCC->CFGR = 0x00000000; // reset all
    // RCC->CR &= (uint32_t)0xfef6ffff; // reset HSEON, CSSON, PLLON
    // RCC->PLLCFGR = 0x24003010; // reset PLLCFGR
    // RCC->CR &= (uint32_t)0xfffbffff; // reset HSEBYP
    // RCC->CIR = 0x00000000; // disable IRQs

    // // leave the clock as-is (internal 16MHz)

    // // enable GPIO clocks
    // RCC->AHB1ENR |= 0x00000003; // GPIOAEN, GPIOBEN

    // turn on an LED! (on pyboard it's the red one)
    GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_1, OUTPUT, PID_GPIO, true);
    // gpio_init(GPIOA, 13, GPIO_MODE_OUT, GPIO_PULL_NONE, 0);
    // gpio_high(GPIOA, 13);

    // enable UART1 at 9600 baud (TX=B6, RX=B7)
    // gpio_init(GPIOB, 6, GPIO_MODE_ALT, GPIO_PULL_NONE, 7);
    // gpio_init(GPIOB, 7, GPIO_MODE_ALT, GPIO_PULL_NONE, 7);
    // RCC->APB2ENR |= 0x00000010; // USART1EN
    // USART1->BRR = (104 << 4) | 3; // 16MHz/(16*104.1875) = 9598 baud
    // USART1->CR1 = 0x0000200c; // USART enable, tx enable, rx enable
}

#endif
