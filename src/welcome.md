# Bare-metal RISC-V on the SiFive FE310

A short, hands-on guide to two pillars of bare-metal programming —
**memory-mapped I/O** and **interrupts** — on the **SiFive FE310-G002**, the
RISC-V microcontroller on the **HiFive1 Rev B** board. Everything in these pages
is *working* code that you build, flash, and watch run on the real device.

> **Workshop edition.** This book accompanies the *Bare-metal RISC-V on the
> SiFive FE310* workshop run within **CC Chip.si**, the Slovenian Competence
> Center on Chips and Semiconductor Technologies
> ([cc-chip.si](https://cc-chip.si/about/)).

## What you'll build

- Drive GPIO pins — and blink the on-board RGB LED — from both **assembly** and
  **C**.
- Talk to a PC over the **UART**: a real `Hello, FE310!` arriving on a serial
  terminal.
- Take **interrupts and exceptions** on RISC-V, starting from the control and
  status registers and working up to a full vector table.
- Tie it together into a small **preemptive round-robin scheduler** driven by the
  machine timer — a tiny multitasking kernel in a few hundred lines of C and
  assembly.

## How the book is organised

The book is built around two **case studies**, each taught the same way: a short
tour of the hardware, the registers involved, and then code that runs.

- **Part I — Memory-mapped I/O** introduces GPIO and the UART, and shows how
  every peripheral is just a block of memory words.
- **Part II — Interrupts and exceptions** covers RISC-V privilege modes, the
  machine-mode CSRs, the CLINT and the PLIC, and the round-robin scheduler.
- The **appendix** maps every listing to its file in the companion source tree.

## Before you start

You will need a **HiFive1 Rev B** board (FE310-G002) and a USB cable, and
**SiFive Freedom Studio** installed on your machine. The
[Setting up Freedom Studio](./freedom-studio.md) page walks through creating the
project, building, flashing, and opening a serial console — start there if you
want to run the code as you read.

## Where to go next

- **Setting up the board and tools** → [Setting up Freedom Studio](./freedom-studio.md)
- **Background, target hardware, and conventions** → [Introduction](./introduction.md)
- **Straight into code** → [GPIO on the FE310](./mmio/gpio-index.md)

## About

The material is written by **Prof. Dr. Patricio Bulić** (University of Ljubljana,
Faculty of Computer and Information Science) and adapted from his textbook
*Understanding Computer Organization*. The accompanying source code is the
author's FE310 driver and context-switch code. Produced for the CC Chip.si skills
and education program.
