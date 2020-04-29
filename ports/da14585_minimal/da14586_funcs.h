#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define SET_FREEZE_REG                       (0x50003300) /* Controls freezing of various timers/counters. */
#define FRZ_WDOG                             (0x0008)
#define GPIO_BASE                             0x50003000

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

void GPIO_SetPinFunction(GPIO_PORT port, GPIO_PIN pin, GPIO_PUPD mode, GPIO_FUNCTION function);

void GPIO_SetActive(GPIO_PORT port, GPIO_PIN pin);

void GPIO_SetInactive(GPIO_PORT port, GPIO_PIN pin);

void GPIO_ConfigurePin(GPIO_PORT port, GPIO_PIN pin, GPIO_PUPD mode, GPIO_FUNCTION function,
                        const bool high);

void wdg_freeze(void);