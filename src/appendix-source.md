# Source code map

Every listing in this book is an excerpt from the companion source tree under
[`code/`](https://github.com/bulicp/riscv-fe310-book/tree/main/code). The tree
is organised by role:

```
code/
├── drivers/      HAL + low-level peripheral drivers
├── interrupts/   interrupt infrastructure (vector table, CLINT, PLIC)
├── scheduler/    the round-robin scheduler and the application entry point
└── snippets/     short teaching listings used inline in the text
```

## `drivers/`

| File | What it is | Discussed in |
|:-----|:-----------|:-------------|
| `hal_uart.h` / `hal_uart.c` | UART register struct, init/transmit HAL | [Programming the UART in C](./mmio/uart-in-c.md), [UART pins](./mmio/uart-pins.md) |
| `hal_gpio.h` / `hal_gpio.c` | GPIO register struct and pin HAL | [Programming GPIO in C](./mmio/gpio-c.md), [UART pins](./mmio/uart-pins.md) |
| `gpio.S` / `gpio.inc` | GPIO routines in assembly + register offsets | [Programming GPIO in assembly](./mmio/gpio-asm.md) |
| `timer.S` | `set_timer_compare` for the CLINT timer | [FE310 interrupts and the CLINT](./interrupts/fe310-interrupts.md) |
| `csr.S` | CSR helpers: register `mtvec`, enable/disable interrupts, switch mode | [Vector table and handlers](./interrupts/vector-table.md) |

## `interrupts/`

| File | What it is | Discussed in |
|:-----|:-----------|:-------------|
| `vectored_interrupts.S` | Production vector table, exception handler, CLINT/PLIC handlers | [Vector table and handlers](./interrupts/vector-table.md), [Scheduler](./interrupts/scheduler.md), [PLIC](./interrupts/plic.md) |
| `macros.S` | `__macro_SAVE_CONTEXT` / `RESTORE` / `INCREMENT_TICK` / `SWITCH_CONTEXT` | [Scheduler](./interrupts/scheduler.md) |
| `mtime.h` / `mtime.c` | CLINT addresses, C timer handler, `mtime_delay` | [FE310 interrupts and the CLINT](./interrupts/fe310-interrupts.md) |
| `plic.h` / `plic.c` | PLIC memory map, priority/enable/claim helpers | [Global interrupts and the PLIC](./interrupts/plic.md) |
| `plic.S` / `plic.inc` | PLIC external-interrupt vector table and weak handlers | [Global interrupts and the PLIC](./interrupts/plic.md) |

## `scheduler/`

| File | What it is | Discussed in |
|:-----|:-----------|:-------------|
| `pb-tasks.h` / `pb-tasks.c` | TCB and stack-frame types, `TaskCreate`/`TaskInit`/`InitScheduler`/`SelectNewTask`, the four demo tasks | [Scheduler](./interrupts/scheduler.md) |
| `main.h` / `main.c` | Application entry point, sets up vectored interrupts and starts the scheduler with `ecall` | [Scheduler](./interrupts/scheduler.md) |

## `snippets/`

Short, self-contained listings used to illustrate one idea at a time:

| File | Illustrates |
|:-----|:------------|
| `RV-vectable.s` | A minimal vector table |
| `RV-set-mtvec.s` | Registering the table base in `mtvec` |
| `RV-mtime-hand.s` | A bare machine-timer handler in assembly |
| `RV-Chandler.c` | The `interrupt` function attribute |
| `RV-inlineasm.c` | Reading `mcause` via inline assembly |
| `RV-mtime-handler-C.c` | A machine-timer handler in C |
| `RV-en-global-int.s` | Enabling global interrupts (`mstatus.MIE`) |
| `RV-en-mtime-int.s` | Enabling the timer interrupt (`mie.MTIE`) |
| `RV-mexti-hand.s` | The external-interrupt handler / PLIC claim flow |
| `RV-gpio13-hand.s` | A weak per-pin GPIO handler stub |
| `RV-plic-set-priority.c`, `RV-plic-enable-source.c`, `RV-plic-vectable.s` | PLIC priority, enable, and vector table |

> **A note on the listings.** The chapters sometimes trim long register
> save/restore sequences (shown as `/* ... */`) so the logic stays readable. The
> files in `code/` are always complete and authoritative.
