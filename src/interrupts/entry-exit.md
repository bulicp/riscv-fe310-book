# Interrupt entry and exit

Taking an interrupt and returning from it are partly hardware, partly software.
Knowing exactly which is which is the key to writing a correct handler — and to
the scheduler later on.

## Entry — what the hardware does

When a machine interrupt is taken, the core automatically:

1. copies **MIE → MPIE** in `mstatus`, then **clears MIE** — so interrupts are
   disabled inside the handler;
2. saves the previous privilege mode in **MPP**;
3. writes the cause into **`mcause`** (INT bit set, code in the low bits);
4. copies the current PC into **`mepc`**, then sets PC to the target derived from
   **`mtvec`** (BASE in direct mode, or `BASE + 4 × code` in vectored mode).

At that point software runs **with interrupts disabled**. The handler can
re-enable them deliberately by setting MIE, or leave them off until it returns.

Crucially, RISC-V does **not** stack the general registers for you — it saves only
`mepc` and the relevant `mstatus` bits. Preserving `ra`, the temporaries, and any
other registers the handler clobbers is the handler's own responsibility.

## Exit — the `mret` instruction

A handler ends with **`mret`**. Executing it makes the hardware:

1. restore the privilege mode from **MPP**;
2. copy **MPIE → MIE** in `mstatus`, re-enabling interrupts as they were;
3. set PC to **`mepc`**, resuming the interrupted instruction stream.

So the symmetry is: entry parks the return address in `mepc` and the old enable
flag in MPIE; `mret` puts them back. Everything *between* — saving and restoring
the general-purpose registers — is up to you, and it is where the next two
chapters spend their effort.
