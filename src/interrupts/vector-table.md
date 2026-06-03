# Vector table and handlers

This chapter turns the theory into running code: a vector table, the CSR setup
that registers it, and interrupt handlers written first in assembly and then in
C. The teaching snippets here come from
[`code/snippets/`](https://github.com/bulicp/riscv-fe310-book/tree/main/code/snippets);
the production versions are in
[`vectored_interrupts.S`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/interrupts/vectored_interrupts.S)
and [`csr.S`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/drivers/csr.S).

## 1. Define the vector table

In vectored mode the CPU jumps to `BASE + 4 × code`. Each table slot is therefore
exactly **4 bytes** — room for a single instruction — so every entry is just a
jump to the real, full-size handler. The table must be **64-byte aligned**.

```asm
# ---------------------------------
#  V E C T O R   T A B L E
#    must be 64-byte aligned.
# ---------------------------------
.balign 64
.global _vector_table
_vector_table:                  # BASE
    j _default_handler
    j _default_handler
    j _default_handler
    j _msw_interrupt_handler    # 3  : machine software interrupt
    j _default_handler
    j _default_handler
    j _default_handler
    j _mtim_interrupt_handler   # 7  : machine timer interrupt
    j _default_handler
    j _default_handler
    j _default_handler
    j _mext_interrupt_handler   # 11 : machine external interrupt
```

The timer entry sits at offset `7 × 4 = 0x1C`, so a timer interrupt jumps to
`BASE + 0x1C`. The three real sources (software = 3, timer = 7, external = 11) get
their own handlers; every other slot points at `_default_handler`. That
catch-all matters because a **trap** (`INT = 0` in `mcause`) dispatches *directly*
to BASE — slot 0 — so something sensible has to live there too.

## 2. Register the table in `mtvec`

`register_handler` ORs the table base with the desired mode and writes the result
into `mtvec`:

```asm
# a0 = vector table base address
# a1 = mode (0 = direct, 1 = vectored)
.balign 4
.global register_handler
register_handler:
    addi sp, sp, -16
    sw   ra, 12(sp)
    sw   fp, 8(sp)
    sw   s1, 4(sp)
    sw   s2, 0(sp)
    addi fp, sp, 16

    or   a0, a0, a1        # fold mode bits into the base
    csrw mtvec, a0         # mtvec <- base | mode

    lw   s2, 0(sp)
    lw   s1, 4(sp)
    lw   fp, 8(sp)
    lw   ra, 12(sp)
    addi sp, sp, 16
    ret
```

## 3. Write a handler in assembly

A handler's **prologue** saves the caller-saved registers it might clobber; the
**body** does the work; the **epilogue** restores them and returns with `mret`.
Here is the machine timer handler, which simply pushes the next tick 1000 cycles
into the future:

```asm
#----------------------------------------
#   Machine Timer Interrupt Handler
#----------------------------------------
.balign 4
.global _mtim_interrupt_handler
_mtim_interrupt_handler:

# Prologue: save the 16 ABI caller-saved registers (ra, t0-t6, a0-a7)
addi sp, sp, -16*4
sw t0, 0*4(sp)
sw t1, 1*4(sp)
# ... t2-t6, a0-a7 ...
sw ra, 15*4(sp)

# Decode the cause
csrr t0, mcause       # read the cause
bgez t0, 1f           # if INT bit clear, it's a trap -> skip

# Increment mtimecmp by 1000 cycles (64-bit add with carry)
li   t0, 0x0200BFF8   # &mtime
lw   t1, 0(t0)        # mtime (lo)
lw   t2, 4(t0)        # mtime (hi)
li   t3, 1000
add  t3, t1, t3       # lo + 1000
sltu t1, t3, t1       # carry out
add  t2, t2, t1       # hi + carry
li   t0, 0x02004000   # &mtimecmp
sw   t3, 0(t0)        # mtimecmp (lo)
sw   t2, 4(t0)        # mtimecmp (hi)

1:
# Epilogue: restore caller-saved registers and return
lw t0, 0*4(sp)
# ... t1-t6, a0-a7 ...
lw ra, 15*4(sp)
addi sp, sp, 16*4
mret
```

Why bother saving 16 registers? Because the interrupted program may rely on any
of them. The general rule for a RISC-V handler is to preserve:

- **`ra`** — the return address, so control can get back correctly;
- the **caller-saved temporaries `t0`–`t6`** (and the argument registers `a0`–`a7`)
  if the handler touches them;
- the **stack pointer**, if the handler needs more stack;
- any other register the handler uses.

After the prologue the handler reads `mcause`: a negative value (INT bit set)
means a real interrupt; otherwise it is a trap and we skip the body. This handler
keeps interrupts disabled throughout, so it is **not** preemptible.

## 4. Write a handler in C

Handlers can also be written in C, with two caveats. First, mark the function so
the compiler emits an interrupt prologue/epilogue and ends it with `mret`:

```c
/* The "interrupt" attribute makes GCC generate an interrupt-style
   entry/exit sequence and a closing mret. */
__attribute__((interrupt)) void interrupt_handler(void) {
    // Interrupt handling code
}
```

Second, the CSRs are not reachable from plain C — you need **inline assembly**.
Reading `mcause`, for example:

```c
unsigned int mcause_value;

// Inline assembly to read mcause into a C variable
asm volatile (
    "csrr %0, mcause"        // read mcause into %0
    : "=r" (mcause_value)    // output: mcause_value
);
```

The `volatile` qualifier stops the optimiser from discarding the `asm` statement
when it thinks the output is unused.

Putting both together gives a C machine-timer handler:

```c
unsigned int *pMTime    = (unsigned int *)0x0200bff8;
unsigned int *pMTimeCmp = (unsigned int *)0x02004000;

__attribute__((interrupt)) void mtime_handler(void) {
    unsigned int mcause_value;

    // CSRs need inline assembly:
    __asm__ volatile ("csrr %0, mcause" : "=r" (mcause_value));

    if (mcause_value & 0x8000007) {     // machine timer interrupt
        *pMTimeCmp = *pMTime + 16384;   // next tick ~0.5 s later
    }
}
```

## 5. Enable interrupts

Finally, switch interrupts on. The global enable sets **MIE** in `mstatus`:

```asm
.equ MSTATUS_MIE_BIT_MASK, 0x00000008   # bit 3
.balign 4
.global enable_global_interrupts
enable_global_interrupts:
    # ... prologue ...
    li   t0, MSTATUS_MIE_BIT_MASK
    csrs mstatus, t0        # set MIE
    # ... epilogue ...
    ret
```

and the per-source enable sets the right bit in `mie` — here **MTIE** for the
timer:

```asm
.equ MIE_MTIE_BIT_MASK, 0x00000080      # bit 7
.balign 4
.global enable_mtimer_interrupt
enable_mtimer_interrupt:
    # ... prologue ...
    li   t0, MIE_MTIE_BIT_MASK
    csrs mie, t0            # set MTIE
    # ... epilogue ...
    ret
```

`csrs` (CSR *set bits*) ORs the mask into the register without disturbing the
other bits — exactly what you want for one-bit enables. The full set of CSR
helpers (`register_handler`, `enable_global_interrupts`,
`enable_msoftware_interrupt`, `enable_mtimer_interrupt`,
`enable_mexternal_interrupt`, `switch_user_mode`) is in
[`csr.S`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/drivers/csr.S).

> **External interrupts and the PLIC.** The external handler `_mext_interrupt_handler`
> goes one step further: it *claims* the highest-priority pending interrupt from
> the PLIC, uses that ID to index a second, PLIC-level vector table
> (`_plic_ext_vector_table`), and calls the matching peripheral handler. That is
> the subject of the [Global interrupts and the PLIC](./plic.md) chapter.
