# Programming the UART in C

The trick that makes a memory-mapped device pleasant to use in C is to describe
its register block with a `struct` whose members line up, in order, with the
device's registers. A pointer to that struct, set to the device's base address,
then lets us reach every register by name.

## Mirroring the registers with a struct

The UART register layout from the previous section maps directly onto this
structure (from [`hal_uart.h`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/drivers/hal_uart.h)):

```c
typedef struct {
    volatile int UART_TXDATA;   // 0x00
    volatile int UART_RXDATA;   // 0x04
    volatile int UART_TXCTRL;   // 0x08
    volatile int UART_RXCTRL;   // 0x0C
    volatile int UART_IE;       // 0x10
    volatile int UART_IP;       // 0x14
    volatile int UART_DIV;      // 0x18
} UART_TypeDef;
```

The members appear in the same order as the registers in memory, so the compiler
lays them out at the right offsets. The `volatile` qualifier is essential: it
tells the compiler that these locations can change outside the program's control
(the hardware updates the FIFOs and flags), so it must issue a real memory access
every time instead of caching the value in a register.

## A pointer to the base address

We then define a pointer that holds the UART0 base address. Anything reached
through it behaves as if the registers were ordinary struct fields:

```c
#define UART0_BASEADDR  0x10013000
#define UART1_BASEADDR  0x10023000

#define UART0 ((UART_TypeDef *)UART0_BASEADDR)
#define UART1 ((UART_TypeDef *)UART1_BASEADDR)
```

So `UART0->UART_DIV = 139;` writes `139` to the word at `0x10013018`.

## A configuration structure

For a friendlier hardware-abstraction layer (HAL) we hide the bit-twiddling
behind an init structure and a handful of constants:

```c
typedef struct {
    unsigned int BaudRate;   // only 115200 is supported here
    unsigned int StopBits;   // UART_STOPBITS_1 or UART_STOPBITS_2
    unsigned int Mode;       // UART_MODE_TX / _RX / _TX_RX
} UART_InitTypeDef;

#define UART_MODE_TX        0x0
#define UART_MODE_RX        0x1
#define UART_MODE_TX_RX     0x2

#define UART_STOPBITS_1     0xfffffffd
#define UART_STOPBITS_2     0x00000002

#define UART_BAUD_115200    139         // clock divider for 115200 bps
```

## The HAL functions

`HAL_UART_Init` configures the pins (covered in the next section), sets the baud
rate, the number of stop bits, and the direction. `HAL_UART_Transmit` sends a
buffer one byte at a time, polling the FULL flag of `txdata` before each write.
Both come from [`hal_uart.c`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/drivers/hal_uart.c):

```c
void HAL_UART_Init(UART_TypeDef *uart, UART_InitTypeDef *UARTInitStruct) {

    // select and enable the UART IO function on the right GPIO pins
    if (uart == (UART_TypeDef *) UART0_BASEADDR) {
        GPIO->GPIO_IOF_SEL &= UART0_PINS_SEL;
        GPIO->GPIO_IOF_EN  |= UART0_PINS_EN;
    }
    if (uart == (UART_TypeDef *) UART1_BASEADDR) {
        GPIO->GPIO_IOF_SEL &= UART1_PINS_SEL;
        GPIO->GPIO_IOF_EN  |= UART1_PINS_EN;
    }

    // set baud rate:
    uart->UART_DIV = UARTInitStruct->BaudRate;

    // set the number of stop bits (NSTOP in txctrl):
    if (UARTInitStruct->StopBits == UART_STOPBITS_1)
        uart->UART_TXCTRL &= 0xfffffffd;
    else if (UARTInitStruct->StopBits == UART_STOPBITS_2)
        uart->UART_TXCTRL |= 0x00000002;

    // set direction (TXEN / RXEN):
    if (UARTInitStruct->Mode == UART_MODE_TX) {
        uart->UART_TXCTRL |= 0x00000001;   // enable TX
        uart->UART_RXCTRL &= 0xfffffffe;   // disable RX
    } else if (UARTInitStruct->Mode == UART_MODE_RX) {
        uart->UART_RXCTRL |= 0x00000001;   // enable RX
        uart->UART_TXCTRL &= 0xfffffffe;   // disable TX
    } else if (UARTInitStruct->Mode == UART_MODE_TX_RX) {
        uart->UART_RXCTRL |= 0x00000001;   // enable RX
        uart->UART_TXCTRL |= 0x00000001;   // enable TX
    }
}
```

```c
void HAL_UART_Transmit(UART_TypeDef *uart, char *pData, unsigned int Size) {
    unsigned int txdata_reg;

    for (unsigned int i = 0; i < Size; i++) {
        // Reading txdata returns the FULL flag in bit 31.
        // Spin until the transmit FIFO can accept a new byte.
        do {
            txdata_reg = uart->UART_TXDATA;
        } while (txdata_reg & 0x80000000);

        // FIFO has room: enqueue the byte.
        uart->UART_TXDATA = pData[i];
    }
}
```

A minimal "hello world" then reduces to:

```c
UART_InitTypeDef cfg = {
    .BaudRate = UART_BAUD_115200,
    .StopBits = UART_STOPBITS_1,
    .Mode     = UART_MODE_TX,
};
HAL_UART_Init(UART0, &cfg);
HAL_UART_Transmit(UART0, "Hello, FE310!\r\n", 15);
```
