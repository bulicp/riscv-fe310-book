# Global interrupts and the PLIC

The timer and software interrupts we have used so far are **local** — each has its
own dedicated line into the core. Peripherals are different: a real SoC has *tens*
of interrupt sources, and giving the CPU one input pin per device would be
unworkable. The answer is an **interrupt controller**, and on the FE310 that
controller is the **Platform-Level Interrupt Controller (PLIC)**.

This chapter develops external interrupts end to end: how the PLIC aggregates and
prioritises sources, the registers that configure it, the claim/complete
handshake, and the assembly + C that wires it all up. The code is in
[`plic.S`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/interrupts/plic.S),
[`plic.c`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/interrupts/plic.c),
[`plic.h`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/interrupts/plic.h),
and the external handler in
[`vectored_interrupts.S`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/interrupts/vectored_interrupts.S).

## Why an interrupt controller?

Suppose many devices share a single CPU interrupt line. When it asserts, the CPU
would have to **poll** every peripheral's status register to discover which one
needs attention — correct, but slow.

An interrupt controller does better. It:

- **combines** all external interrupt requests onto one CPU line,
- **prioritises** them when several are pending at once,
- **routes** the highest-priority request to the CPU, and
- **identifies** the source for the CPU, so no polling is needed.

All of this happens in hardware, so the response is fast. The controller is still
**programmable**: a set of memory-mapped registers lets us assign priorities and
enable individual sources.

## The PLIC in the FE310

Recall that the FE310 has two interrupt controllers. The **CLINT** provides the
local timer and software interrupts; the **PLIC** manages **global** interrupts
from I/O devices and delivers them to the core on the single **Machine External
Interrupt** line.

The FE310's peripherals drive **52 interrupt sources**, which the PLIC aggregates:

| Device | Interrupt source IDs |
|:-------|:--------------------:|
| WDT    | 1 |
| RTC    | 2 |
| UART0  | 3 |
| UART1  | 4 |
| QSPI0  | 5 |
| SPI1   | 6 |
| SPI2   | 7 |
| GPIO   | 8–39 |
| PWM0   | 40–43 |
| PWM1   | 44–47 |
| PWM2   | 48–51 |
| I²C    | 52 |

Each GPIO *pin* is its own source, so the 32 pins occupy IDs 8–39. Source ID 0
does not exist and is hardwired off.

The PLIC register block (from
[`plic.h`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/interrupts/plic.h)):

```c
#define PLIC_INT_PRIORITY_BASE  0x0C000000   // priority[source] at BASE + 4*source
#define PLIC_INT_PENDING1       0x0C001000
#define PLIC_INT_PENDING2       0x0C001004
#define PLIC_INT_ENABLE1        0x0C002000
#define PLIC_INT_ENABLE2        0x0C002004
#define PLIC_INT_THRESHOLD      0x0C200000
#define PLIC_CLAIM              0x0C200004
```

## Priority

Every source has a 32-bit **priority** register at `0x0C000000 + 4 × SourceID`.
For example, UART0 (ID 3) is at `0x0C00000C`. Only the three least-significant
bits matter — the FE310 supports **7 priority levels**:

| Bits 31:3 | Bits 2:0 |
|:---------:|:--------:|
| (reserved, 0) | **PRIORITY** |

- `0` — *never interrupt*: disables the source.
- `1` — lowest active priority.
- `7` — highest priority.

When two sources share a priority level, the one with the **lower source ID**
wins. If several sources are pending, the PLIC forwards the highest-priority one.

## Per-source enable

Each source is individually enabled by setting its bit in **`enable1`**
(`0x0C002000`) or **`enable2`** (`0x0C002004`), treated as a contiguous bit array.
The enable bit for source *ID* lives in **bit `ID mod 32`** of **word `ID / 32`**:

- source 3 (UART0) → bit 3 of word 0 → `enable1`;
- source 39 (GPIO pin 31) → bit 7 of word 1 → `enable2`.

Bit 0 of `enable1` represents the non-existent source 0 and is hardwired to 0.

## The claim/complete handshake

This is the heart of external-interrupt handling:

1. One or more sources assert. The PLIC selects the highest-priority one, writes
   its **ID into the `claim` register**, and raises the **Machine External
   Interrupt** line.
2. The core runs the machine external interrupt handler, which **reads `claim`**.
   The read returns the winning source ID (or 0 if none) *and* tells the PLIC the
   interrupt is being serviced, so it will not reassert the same source meanwhile.
3. After servicing the device, the handler **writes the same ID back to `claim`**
   to signal **completion**.

A minimal claim/complete in C looks like this (from `plic.c`):

```c
void plic_claim_amd_complete(void) {
    // 1. claim: read the ID of the highest-priority pending interrupt
    volatile unsigned int claim = *((unsigned int *)PLIC_CLAIM);
    // ... service the source identified by `claim` ...
    // 2. complete: write the ID back
    *((unsigned int *)PLIC_CLAIM) = claim;
}
```

## Implementation

### 1. The PLIC vector table

Because the PLIC reports a *source ID*, the external handler dispatches through a
second, PLIC-level vector table indexed by that ID. It is an array of jump
instructions, one per source (from
[`plic.S`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/interrupts/plic.S)):

```asm
.balign 4
.global _plic_ext_vector_table
_plic_ext_vector_table:
    j _panic_handler        # PLIC src 0 - invalid
    j _aon_wdt_handler      # PLIC src 1
    j _aon_rtc_handler      # PLIC src 2
    j _uart0_handler        # PLIC src 3
    j _uart1_handler        # PLIC src 4
    # ... QSPI/SPI ...
    j _gpio0_handler        # PLIC src 8
    # ...
    j _gpio13_handler       # PLIC src 21
    # ... GPIO/PWM/I2C up to src 52 ...
    j _i2c_handler          # PLIC src 52
```

Each peripheral handler is declared **weak** so an application can override just
the ones it cares about, leaving the rest as harmless `ret` stubs:

```asm
.balign 4
.weak _gpio13_handler
_gpio13_handler:
    # Your code goes here:
    ...
    ret
```

### 2. The machine external interrupt handler

This handler sits in slot 11 of the *main* vector table. It claims the source
from the PLIC, multiplies the ID by 4 to get the table offset, and jumps into the
PLIC vector table — calling the right per-source handler (from
`vectored_interrupts.S`):

```asm
.global _mext_interrupt_handler
_mext_interrupt_handler:
    # Prologue: save the 16 ABI caller-saved registers (ra, t0-t6, a0-a7)
    addi sp, sp, -16*REGSIZEBYTES
    sw t0, 0*REGSIZEBYTES(sp)
    # ... t1-t6, a0-a7 ...
    sw ra, 15*REGSIZEBYTES(sp)

    # Decode the cause:
    csrr t0, mcause
    bgez t0, 1f                     # not an interrupt -> skip

    # Claim: read the highest-priority pending source ID
    la t0, PLIC_CLAIM
    lw t1, 0(t0)                    # t1 = source ID (reading also clears pending)
    slli t2, t1, 2                  # offset = ID * 4

    la t3, _plic_ext_vector_table
    add t3, t3, t2                  # &table + 4*ID
    jalr t3                         # call the per-source handler

    # Complete: (write the ID back to claim once servicing is done)
    # sw t1, 0(t0)

1:
    # Epilogue: restore caller-saved registers
    lw t0, 0*REGSIZEBYTES(sp)
    # ... t1-t6, a0-a7 ...
    lw ra, 15*REGSIZEBYTES(sp)
    addi sp, sp, 16*REGSIZEBYTES
    mret
```

In short, the handler: decodes the cause exactly as the timer handler does; reads
the source ID from `claim`; computes `base + 4 × ID`; and calls that handler.

### 3. Setting priority and enabling a source (C)

The two configuration helpers are short. Setting a source's priority writes to
`priority[source]`:

```c
#define PLIC_INT_PRIORITY_BASE  0x0C000000

void plic_set_priority(unsigned int source, unsigned int priority) {
    *((unsigned int *)PLIC_INT_PRIORITY_BASE + source) = priority;
}
```

Enabling a source sets its bit in the right enable word:

```c
#define PLIC_INT_ENABLE1  0x0C002000

void plic_enable_source(unsigned int source) {
    unsigned int bit_position = source | 32;
    unsigned int enable_reg   = source / 32;
    *((unsigned int *)PLIC_INT_ENABLE1 + enable_reg) |= (1 << bit_position);
}
```

> `plic.c` also provides `plic_set_threshold` (mask all sources below a level) and
> `plic_disable_all`. Together with `plic_claim_amd_complete`, these are enough to
> bring a peripheral interrupt — say, a GPIO pin or UART RX — all the way from the
> device, through the PLIC, into a handler of your own.

## Putting it together

To make GPIO pin 13 (source 21) interrupt the core, an application would:

1. give source 21 a non-zero **priority** with `plic_set_priority(PLIC_SRC_GPIO13, …)`;
2. **enable** it with `plic_enable_source(PLIC_SRC_GPIO13)`;
3. set the PLIC **threshold** below that priority so it is not masked;
4. configure the GPIO pin to raise an interrupt (e.g. rising-edge enable);
5. enable the **machine external interrupt** (`mie.MEIE`) and global interrupts;
6. override the weak **`_gpio13_handler`** with the real servicing code.

When the pin fires, the chain is: GPIO → PLIC (selects + sets `claim`) → core
external line → `_mext_interrupt_handler` → claim → `_plic_ext_vector_table[21]`
→ `_gpio13_handler` → complete → `mret`.
