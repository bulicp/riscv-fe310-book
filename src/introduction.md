# Introduction

This book collects two closely related topics in bare-metal programming of the
**SiFive FE310-G002** RISC-V microcontroller (the chip on the HiFive1 board):

1. **Memory-mapped I/O**, illustrated by driving the **UART** serial interface.
2. **Interrupts and exceptions** on RISC-V, culminating in a small
   **round-robin task scheduler** built on the machine timer interrupt.

Both topics are taught the same way: a short tour of the hardware, the registers
involved, and then *working* C and assembly code that runs on the real device.
Every listing in these pages is taken from the companion source tree under
[`code/`](https://github.com/bulicp/riscv-fe310-book/tree/main/code), so what
you read is exactly what you can build and flash. The
[Source code map](./appendix-source.md) tells you which file backs which section.

## About this workshop and the competence center

This book accompanies the *Bare-metal RISC-V on the SiFive FE310* workshop run
within **CC Chip.si**, the Slovenian Competence Center on Chips and Semiconductor
Technologies. CC Chip.si is a consortium of six Slovenian institutions,
coordinated by the Faculty of Electrical Engineering, University of Ljubljana. It
is one of around 30 Chips Competence Centers established across the EU under the
Chips for Europe initiative of the European Chips Act, with skills development and
education among its core activities. You can read more at
[cc-chip.si](https://cc-chip.si/about/).

## About the author

The material is written by **Prof. Dr. Patricio Bulić**, Full Professor at the
Faculty of Computer and Information Science, University of Ljubljana, where he
teaches Computer Architecture, Computer Systems Organization, and Embedded
Systems. He is the author of the Springer textbook *Understanding Computer
Organization* and co-author of *Introduction to Parallel Computing*; this book's
chapters are adapted from the former.

## The target hardware

The FE310-G002 is built around SiFive's **E31** core, a 32-bit RV32IMAC
implementation. For the purposes of this book the relevant facts are:

- The core has a flat 32-bit address space; **every** peripheral is reached
  through ordinary load and store instructions (memory-mapped I/O).
- It supports only two privilege modes: **Machine (M)** and **User (U)**.
- Local interrupts (timer and software) come from the **CLINT**; external
  interrupts from peripherals are funnelled through the **PLIC**.

## How to read the code listings

C listings are shown like this:

```c
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIO, uint32t GPIO_Pin) {
    GPIO->GPIO_OUTPUT_VAL ^= GPIO_Pin;
}
```

RISC-V assembly listings like this:

```asm
.balign 4
.global enable_global_interrupts
enable_global_interrupts:
    li   t0, 0x00000008    # MIE bit (bit 3) of mstatus
    csrs mstatus, t0       # set it
    ret
```

Register diagrams are drawn either as figures or as **bit-field tables**, where
the most-significant bit is on the left.

<!---## Building this book

The book is written for [mdBook](https://rust-lang.github.io/mdBook/). To preview
it locally:

```bash
cargo install mdbook        # or download a release binary
mdbook serve --open         # live-reloading preview on http://localhost:3000
mdbook build                # static site in ./book
```

Pushing to the `main` branch publishes the site to GitHub Pages automatically
through the workflow in `.github/workflows/deploy.yml`. --->

> **Attribution.** The text is adapted from the *Memory-mapped Input/Output* and
> *Interrupts and interrupt handling* chapters of Patricio Bulić's
> *Understanding Computer Organization*. The accompanying source code is the
> author's FE310 context-switch / driver code.
