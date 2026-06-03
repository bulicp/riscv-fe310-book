# Programming GPIO in C

The same device is far more pleasant from C, using the now-familiar trick: a
`struct` that mirrors the register block, reached through a pointer at the base
address.

## Mirroring the registers

The GPIO block has 19 registers; the C structure lists them in order so each
lands at the right offset (from
[`hal_gpio.h`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/drivers/hal_gpio.h)):

```c
typedef unsigned int uint32t;

typedef struct {
    volatile uint32t GPIO_INPUT_VAL;   // 0x00
    volatile uint32t GPIO_INPUT_EN;    // 0x04
    volatile uint32t GPIO_OUTPUT_EN;   // 0x08
    volatile uint32t GPIO_OUTPUT_VAL;  // 0x0C
    volatile uint32t GPIO_PUE;
    volatile uint32t GPIO_DS;
    volatile uint32t GPIO_RISE_IE;
    volatile uint32t GPIO_RISE_IP;
    volatile uint32t GPIO_FALL_IE;
    volatile uint32t GPIO_FALL_IP;
    volatile uint32t GPIO_HIGH_IE;
    volatile uint32t GPIO_HIGH_IP;
    volatile uint32t GPIO_LOW_IE;
    volatile uint32t GPIO_LOW_IP;
    volatile uint32t GPIO_IOF_EN;
    volatile uint32t GPIO_IOF_SEL;
    volatile uint32t GPIO_OUT_XOR;
} GPIO_TypeDef;

#define GPIO_BASE_ADDRESS  (GPIO_TypeDef *)0x10012000
```

Every member is `volatile`: the hardware can change `GPIO_INPUT_VAL` at any time,
so the compiler must always issue a real memory access instead of caching a stale
value.

## Direct access through a pointer

With a pointer to the base address, the registers read like ordinary fields. For
example, make pin 19 an output and toggle it:

```c
GPIO_TypeDef *GPIO = GPIO_BASE_ADDRESS;

GPIO->GPIO_OUTPUT_EN  |= (1 << 19);   // pin 19 = output
GPIO->GPIO_OUTPUT_VAL ^= (1 << 19);   // toggle pin 19
```

## A small hardware-abstraction layer

Bit-twiddling is fine once, but a HAL lets callers configure pins without knowing
the register layout. The pins are exposed as bit masks, plus a config struct and
a pin-state enum:

```c
#define GPIO_MODE_INPUT   0x00U
#define GPIO_MODE_OUTPUT  0x01U

#define GPIO_PIN_19  ((uint32t)0x00080000)   // i-th pin = i-th bit
#define GPIO_PIN_21  ((uint32t)0x00200000)
#define GPIO_PIN_22  ((uint32t)0x00400000)

#define LED_GREEN_PIN  GPIO_PIN_19
#define LED_BLUE_PIN   GPIO_PIN_21
#define LED_RED_PIN    GPIO_PIN_22

typedef struct {
    uint32t Pin;    // one or more GPIO_PIN_x, OR-ed together
    uint32t Mode;   // GPIO_MODE_INPUT or GPIO_MODE_OUTPUT
} GPIO_InitTypeDef;

typedef enum { GPIO_PIN_RESET = 0U, GPIO_PIN_SET } GPIO_PinState;
```

`Pin` is a **bit mask**, so several pins can be configured at once by OR-ing their
masks (`GPIO_PIN_19 | GPIO_PIN_21 | GPIO_PIN_22`).

The HAL functions themselves (from
[`hal_gpio.c`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/drivers/hal_gpio.c))
are the C counterparts of the assembly routines:

```c
void HAL_GPIO_Init(GPIO_TypeDef *GPIO, GPIO_InitTypeDef *GPIO_Init) {
    if (GPIO_Init->Mode == GPIO_MODE_INPUT) {
        GPIO->GPIO_INPUT_EN  |=  GPIO_Init->Pin;
        GPIO->GPIO_OUTPUT_EN &= ~(GPIO_Init->Pin);
    } else if (GPIO_Init->Mode == GPIO_MODE_OUTPUT) {
        GPIO->GPIO_OUTPUT_EN |=  GPIO_Init->Pin;
        GPIO->GPIO_INPUT_EN  &= ~(GPIO_Init->Pin);
    }
}

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIO, uint32t GPIO_Pin, GPIO_PinState PinState) {
    if (PinState == GPIO_PIN_SET)
        GPIO->GPIO_OUTPUT_VAL |=  GPIO_Pin;
    else
        GPIO->GPIO_OUTPUT_VAL &= ~(GPIO_Pin);
}

void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIO, uint32t GPIO_Pin) {
    GPIO->GPIO_OUTPUT_VAL ^= GPIO_Pin;
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIO, uint32t GPIO_Pin) {
    return (GPIO->GPIO_INPUT_VAL & GPIO_Pin) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}
```

## Putting it together

Initialising the on-board RGB LED (pins 19, 21, 22) as outputs is then two lines:

```c
GPIO_InitTypeDef cfg = {0};
cfg.Mode = GPIO_MODE_OUTPUT;
cfg.Pin  = LED_GREEN_PIN | LED_BLUE_PIN | LED_RED_PIN;
HAL_GPIO_Init(GPIO, &cfg);

HAL_GPIO_WritePin(GPIO, LED_BLUE_PIN, GPIO_PIN_RESET);  // turn the blue LED on
```

This is exactly the `GPIO_Init()` routine used by the scheduler case study to set
up its blinking-LED tasks — and the same `GPIO_TypeDef` is reused in the next
chapter to route the UART onto its pins via the IO-function registers.
