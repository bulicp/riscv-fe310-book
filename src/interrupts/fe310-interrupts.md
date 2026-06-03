# FE310 interrupts and the CLINT

The FE310 sorts RISC-V interrupts into two families:

- **Local interrupts** are wired straight to the E31 core, one dedicated line
  each. The core has three: **software**, **timer**, and **external**. Software
  and timer interrupts are produced by the **Core-Local Interruptor (CLINT)**.
- **Global interrupts** come from peripherals (UART, GPIO, …). They are collected
  by the **Platform-Level Interrupt Controller (PLIC)** and delivered to the core
  on its single **external** interrupt line.

So the external line is shared by every peripheral, multiplexed through the PLIC,
while the timer and software lines are private to the CLINT.

## The CLINT

The CLINT is a mandatory part of a RISC-V system. It generates two of the three
local interrupts:

- **Machine timer interrupts**, for timekeeping, scheduling, and periodic work.
- **Machine software interrupts**, which software raises itself — for instance to
  request a kernel service from user code.

Its registers are memory-mapped:

| Address | Width | Register |
|:--------|:-----:|:---------|
| `0x02000000` | 4 B | `msip` |
| `0x02004000` | 8 B | `mtimecmp` |
| `0x0200BFF8` | 8 B | `mtime` |

These addresses are captured as constants in
[`code/interrupts/mtime.h`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/interrupts/mtime.h):

```c
#define CLINT_BASE       0x02000000
#define CLINT_MSIP       0x02000000
#define CLINT_MTIME      0x0200bff8
#define CLINT_MTIME_CMP  0x02004000
```

### Machine software interrupts

A machine software interrupt is raised by writing `1` to **`msip`**. The register
is 32 bits wide with only the least-significant bit meaningful — that bit is
reflected in `mip.MSIP`. It resets to zero.

### Machine timer interrupts

The CLINT provides a free-running 64-bit counter, **`mtime`**, that increments at
the real-time-clock rate. A timer interrupt is programmed with two registers:

- **`mtime`** — the current 64-bit time. Software reads it to know "now".
- **`mtimecmp`** — a comparand. **When `mtime` reaches `mtimecmp`, a timer
  interrupt fires.**

To schedule the next tick, a handler simply writes a future value into
`mtimecmp`. On the FE310 the RTC runs at 32.768 kHz, so:

- 1 s  ≈ 32768 counts
- 0.5 s ≈ 16384 counts
- 100 ms ≈ 3277 counts

This `mtime`/`mtimecmp` mechanism is the engine behind preemptive multitasking:
arm a timer interrupt every *N* counts and the CPU is forced to re-enter the
kernel at fixed intervals — which is exactly what the scheduler case study uses.
