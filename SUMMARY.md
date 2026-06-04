# Summary

[Welcome](./welcome.md)
[Introduction](./introduction.md)
[Setting up Freedom Studio](./freedom-studio.md)

# Part I — Memory-mapped I/O: GPIO and the UART

- [Case study: the GPIO interface on the FE310](./mmio/gpio-index.md)
  - [Programming GPIO in assembly](./mmio/gpio-asm.md)
  - [Programming GPIO in C](./mmio/gpio-c.md)
- [Case study: the UART on the FE310](./mmio/index.md)
  - [The UART and serial communication](./mmio/uart-protocol.md)
  - [The UART interface in the SiFive FE310](./mmio/fe310-uart.md)
  - [Programming the UART in C](./mmio/uart-in-c.md)
  - [UART pins and IO functions](./mmio/uart-pins.md)

# Part II — RISC-V interrupts and exceptions

- [RISC-V interrupts and exceptions](./interrupts/index.md)
  - [RISC-V privileged modes](./interrupts/privileged-modes.md)
  - [Machine-mode exceptions and CSRs](./interrupts/machine-csrs.md)
  - [FE310 interrupts and the CLINT](./interrupts/fe310-interrupts.md)
  - [Interrupt entry and exit](./interrupts/entry-exit.md)
  - [Vector table and handlers](./interrupts/vector-table.md)
  - [Case study: a round-robin scheduler](./interrupts/scheduler.md)
  - [Global interrupts and the PLIC](./interrupts/plic.md)

# Appendix

- [Source code map](./appendix-source.md)
