# Case study: using the UART interface on the FE310

> **In this part you will**
> - recall what *memory-mapped I/O* means and why every peripheral is just a set
>   of memory words,
> - learn the UART serial protocol: frames, start/stop bits, and baud rate,
> - meet the UART registers of the SiFive FE310 and what each bit does,
> - drive the UART entirely from **C** by mirroring its registers with a `struct`,
> - and route the UART signals onto physical pins using the GPIO **IO-function**
>   multiplexer.

The only way a RISC-V processor moves data is with **load** and **store**
instructions. A direct consequence is that if the CPU is to talk to an
input/output device, that device must look like a handful of memory words — it
must be **memory-mapped**. Each such word has a fixed meaning (status, control,
or data) and is called a **register**.

In this case study we program a particularly useful memory-mapped device, the
**Universal Asynchronous Receiver/Transmitter (UART)**, and this time we do it
purely in C. There is nothing special about C here: any memory-mapped device can
be driven from C once we describe its register block with a structure. A UART is
a common serial interface that lets a microcontroller exchange bytes
asynchronously with sensors, displays, other microcontrollers, or a desktop PC.
The FE310 contains two such interfaces, **UART0** and **UART1**.

The sections that follow build up the driver step by step. The finished driver
lives in [`code/drivers/hal_uart.c`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/drivers/hal_uart.c)
and [`code/drivers/hal_uart.h`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/drivers/hal_uart.h).
