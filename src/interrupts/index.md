# RISC-V interrupts and exceptions

> **In this part you will**
> - understand RISC-V **privileged modes** and how the CPU switches between them,
> - learn the five machine-mode CSRs that govern exceptions: `mstatus`, `mie`,
>   `mip`, `mcause`, and `mtvec`,
> - see how the FE310 splits interrupts into **local** (CLINT) and **global**
>   (PLIC) sources,
> - follow exactly what the hardware does on interrupt **entry and exit**,
> - build a **vector table** and write handlers in both assembly and C,
> - and put it all together in a **round-robin task scheduler** driven by the
>   machine timer interrupt.

RISC-V defines **privilege modes** that decide how much of the machine a piece of
code may touch. Privilege separation keeps user programs away from
system-critical resources — special registers, protected memory, privileged
instructions — while letting system code reach everything. Each mode has its own
set of **control and status registers (CSRs)**, and a current-mode field decides
which CSRs and which instructions are legal right now.

How does a program leave user mode? It depends on the direction:

1. From **user** mode, the *only* way into a more privileged mode is through an
   **exception** — a trap or an interrupt. When one fires, the CPU automatically
   switches to a system mode, runs the handler there, and on return drops back to
   the mode it came from.
2. From a **system** mode, the CPU can drop to user mode deliberately, by
   executing a privileged instruction that rewrites the current-mode field.

The chapters that follow develop this machinery on the FE310's **E31** core,
which implements only Machine and User modes — so, conveniently, every trap and
interrupt is handled in **Machine mode**. All of the code shown here lives under
[`code/interrupts/`](https://github.com/bulicp/riscv-fe310-book/tree/main/code/interrupts)
and [`code/scheduler/`](https://github.com/bulicp/riscv-fe310-book/tree/main/code/scheduler).
