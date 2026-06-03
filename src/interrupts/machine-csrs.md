# Machine-mode exceptions and CSRs

Five Machine-mode CSRs govern exception handling on the E31 core. Together they
decide whether interrupts are on, which ones are armed, which are pending, what
caused the current trap, and where the CPU jumps when one occurs.

These CSRs are **not** memory-mapped — you cannot reach them with a load or store.
They are read and written only with the privileged CSR instructions
`csrr`/`csrw` (and the bit-set/clear variants `csrs`/`csrc`), so any code that
touches them must drop into assembly.

> In the bit-field tables below, the most-significant bit is on the left. Only
> the interrupt-related fields are shown; each CSR has more bits than this.

## `mstatus` — machine status

Holds the global interrupt-enable state and the saved state used on trap entry.

| Bits | 12:11 | … | 7 | … | 3 | … |
|------|:-----:|:-:|:-:|:-:|:-:|:-:|
| Field | **MPP** | | **MPIE** | | **MIE** | |

- **MIE** (bit 3) — *Machine Interrupt Enable*: the global on/off switch. When
  set, the CPU may take machine interrupts; when clear, all are masked.
- **MPIE** (bit 7) — *Machine Previous Interrupt Enable*: snapshot of MIE from
  before the trap, used to restore MIE on return (this is how nesting is managed).
- **MPP** (bits 12:11) — *Machine Previous Privilege*: the privilege mode the CPU
  was in before the trap, restored on return.

## `mie` — machine interrupt enable

Arms individual interrupt sources. A source can only fire if both its bit here
**and** the global MIE in `mstatus` are set.

| Bits | … | 11 | … | 7 | … | 3 | … |
|------|:-:|:--:|:-:|:-:|:-:|:-:|:-:|
| Field | | **MEIE** | | **MTIE** | | **MSIE** | |

- **MSIE** (bit 3) — machine **software** interrupt enable.
- **MTIE** (bit 7) — machine **timer** interrupt enable.
- **MEIE** (bit 11) — machine **external** interrupt enable.

## `mip` — machine interrupt pending

Read-only view of which interrupts are waiting. When a source asserts, its bit is
set; when the CPU takes the interrupt, the bit clears.

| Bits | … | 11 | … | 7 | … | 3 | … |
|------|:-:|:--:|:-:|:-:|:-:|:-:|:-:|
| Field | | **MEIP** | | **MTIP** | | **MSIP** | |

If several are pending at once, the core prioritises them, highest first:
**external → software → timer**.

## `mcause` — machine cause

Tells the handler what happened.

| Bit 31 | Bits 9:0 |
|:------:|:--------:|
| **INT** | **EXCEPTION CODE** |

- If **INT = 0**, a **trap** (synchronous exception) occurred and the code field
  identifies it.
- If **INT = 1**, an **interrupt** occurred and the code field is the interrupt
  number, using the same bit positions as `mip`.

For example, a machine timer interrupt sets `mcause` to `0x80000007` (INT = 1,
code = 7).

| INT | Code | Meaning |
|:---:|:----:|:--------|
| 0 | 0  | Instruction address misaligned |
| 0 | 1  | Instruction access fault |
| 0 | 2  | Illegal instruction |
| 0 | 3  | Breakpoint |
| 0 | 4  | Load address misaligned |
| 0 | 5  | Load access fault |
| 0 | 6  | Store address misaligned |
| 0 | 7  | Store access fault |
| 0 | 8  | Environment call from U-mode |
| 0 | 11 | Environment call from M-mode |
| 1 | 3  | Machine software interrupt |
| 1 | 7  | Machine timer interrupt |
| 1 | 11 | Machine external interrupt |

## `mtvec` — machine trap vector

Sets *where* the CPU goes on a trap, and *how* it dispatches.

| Bits 31:2 | Bits 1:0 |
|:---------:|:--------:|
| **BASE** (64-byte aligned) | **MODE** |

- **MODE = 0 — direct.** Every trap and interrupt jumps to the single address in
  BASE. One handler must inspect `mcause` to figure out what happened.
- **MODE = 1 — vectored.** Interrupts jump to `BASE + 4 × code`, i.e. through a
  small table of jump instructions. This is faster and is the usual choice.

| MODE | Dispatch | Handler address |
|:----:|:---------|:----------------|
| 0 | Direct (all traps share one handler) | `PC = BASE` |
| 1 | Vectored (per-interrupt entry) | `PC = BASE + 4 × mcause[EXCEPTION CODE]` |

**Exceptions always use direct dispatch**, regardless of MODE — they all trap to
BASE and the handler sorts them out. Only *interrupts* are vectored. BASE must be
64-byte aligned so the `BASE + 4 × code` calculation needs no adder.

> *Worked example.* With vectored mode and `BASE = 0x20011500`, a machine timer
> interrupt has code `7`, so the CPU jumps to
> `0x20011500 + 4 × 7 = 0x2001151C`. The first instruction of the timer handler
> entry must sit at that address.

Getting these five registers right is what makes interrupts work: they decide
whether the CPU listens, what it listens for, and where it goes when something
happens.
