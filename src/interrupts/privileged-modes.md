# RISC-V privileged modes

Privileged modes are what let one instruction set carry operating systems,
hypervisors, and security models. RISC-V defines three:

1. **User mode (U)** — the lowest privilege. Application code runs here. It can
   execute the base instruction set, access memory, and compute, but it **cannot**
   touch the CSRs that govern exception handling or interrupt control.

2. **Supervisor mode (S)** — for an OS kernel. It can do context switches, manage
   devices, and reach most CSRs, including the interrupt- and exception-related
   ones.

3. **Machine mode (M)** — the highest privilege and the only mode guaranteed to
   exist. It has complete control: every instruction, every CSR, and full command
   over exceptions and interrupts at all levels. **A RISC-V core resets into
   Machine mode.**

## On the FE310's E31 core

The E31 core inside the FE310 implements only **Machine** and **User** modes.
Transitions between them are driven by CSR bits, managed from Machine mode, and
triggered by exceptions and interrupts.

Because **all** traps and interrupts on this core are taken in Machine mode, the
rest of this part restricts itself to the Machine-mode view of exceptions. That
keeps things concrete: there is exactly one set of CSRs to learn.
