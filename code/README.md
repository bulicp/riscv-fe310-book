# Companion source code

This tree holds the C and RISC-V assembly that backs the book. It targets the
SiFive **FE310-G002** (E31, RV32IMAC) — e.g. the HiFive1 board.

## Layout

| Directory | Contents |
|:----------|:---------|
| `drivers/` | UART and GPIO HAL, assembly GPIO/timer drivers, CSR helpers |
| `interrupts/` | Vector table, CLINT timer, PLIC, context-switch macros |
| `scheduler/` | Round-robin scheduler (TCB, create/init/switch) and `main` |
| `snippets/` | Short standalone listings used inline in the book |

## How the pieces fit

```
main.c
  ├── GPIO_Init()                         (hal_gpio.*)
  ├── InitScheduler()                     (pb-tasks.*)
  ├── _register_handler(_vector_table, …) (csr.S + vectored_interrupts.S)
  ├── _enable_global_interrupts()         (csr.S)
  └── ecall ─────────► _exception_handler (vectored_interrupts.S)
                          starts Task0; arms the machine timer
                              │
              every TIME_SLICE│ machine timer interrupt
                              ▼
                    _mtim_interrupt_handler (vectored_interrupts.S)
                       __macro_SAVE_CONTEXT / INCREMENT_TICK /
                       SWITCH_CONTEXT / RESTORE_CONTEXT   (macros.S)
```

## Notes on building

These files come from an FE310 bare-metal project and expect:

- a RISC-V GCC toolchain (`riscv64-unknown-elf-gcc` / `riscv32-...`),
- a startup file and a linker script for the FE310 memory map (not included here),
- the include files `memory_map.inc` (referenced by several `.S` files) defining
  the SoC base addresses, alongside the provided `gpio.inc` and `plic.inc`.

The code is provided as study material to accompany the book rather than as a
turnkey, flashable project; drop these files into your FE310/HiFive1 project
(Freedom E SDK, SiFive Freedom Studio, or a PlatformIO `sifive` setup) to build
and run them.
