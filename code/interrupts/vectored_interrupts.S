# ----------------------------------------------------------------------------
#
# Vector table and machine exception handlers
# (c) Pa3cio, UL FRI
# 2023
#
# ----------------------------------------------------------------------------

.section .text

.include "memory_map.inc"
.include "plic.inc"
.include "macros.S"

//.extern mtime__handler
.extern pTCB
.extern TCB
.extern current_task

.equ NTASKS,            4
.equ DELAY,             16384
.equ TIME_SLICE,        33          // 1ms
.equ REGSIZEBYTES,      4
.equ WORD_SIZE,         4
.equ CONTEXT_SIZE,      (32*WORD_SIZE)

.option norvc       # disables the generation of compressed instructions.


# ------------------------------------------------------------------------------
#
#  V E C T O R   T A B L E
#
# While operating in vectored mode, interrupts set the pc to mtvec.BASE + 4 × exception code.
# For example, if a machine timer interrupt is taken, the pc is set to mtvec.BASE + 0x1C. Typically,
# the vector table is populated with jump instructions to transfer control to interrupt-specific
# handlers. In vectored interrupt mode, BASE must be 64-byte aligned. This is to avoid an adder
# for BASE + 4 × exception code computation.
#
# ------------------------------------------------------------------------------

.balign 128
.global _vector_table
.type _vector_table, @function          # Mark the symbol as being a function name
_vector_table:
    j _exception_handler
    j _default_handler
    j _default_handler
    # ------------------------------------------------------------------------------
    j _msw_interrupt_handler            # 3: machine software interrupt
    # ------------------------------------------------------------------------------
    j _default_handler
    j _default_handler
    j _default_handler
    # ------------------------------------------------------------------------------
    j _mtim_interrupt_handler           # 7: machine timer interrupt handler
    # ------------------------------------------------------------------------------
    j _default_handler
    j _default_handler
    j _default_handler
    # ------------------------------------------------------------------------------
    j _mext_interrupt_handler           # 11: machine external interrupt
    # ------------------------------------------------------------------------------
    .size  _vector_table, .-_vector_table




/*---------------------------------------
    CLINT Exception Handler
    - Exceptions are not vectored
    - All exceptions are traped here

    - ECALL - starts the sceduler
---------------------------------------*/
.balign 4, 0
.global _exception_handler
.type _exception_handler, @function          # Mark the symbol as being a function name
_exception_handler:

    # Decode exception cause:
    csrr t0, mcause                 # read exception cause
    bltz t0, 2f                     # exit if not an exception

    # Check if ECALL:
1:  li t1, 0xB                      # ecall from M-mode
    bne t1, t0, 2f

    /*
      ECALL:
      1. load the SP of the first task (SP <- TCB[0].sp)
      2. increment tick to set the time of the first timer interrupt
      3. enable MTIME interrupt
      4. restore kontext of the first task
      5. Upon return, the fiirsat task is executed and the scheduler is running
    */
    /* 1. Load SP of the first task: */
    la t1, TCB      // load the address of TCB[0]
    lw sp, 0(t1)    // load sp from TCB[0].sp

    # 2. Increment time slice (tick)
    __macro_INCREMENT_TICK

    /* 3. Enable MTIME interrupt */
    li t0, 0x00000080
    csrs mie, t0

    # 4. Restore the context of Task0
    __macro_RESTORE_CONTEXT

2:
    mret
    .size  _exception_handler, .-_exception_handler


# ------------------------------------------------------------------------------
#
#  C L I N T  I N T E R R U P T   H A N D L E R S
#
# ------------------------------------------------------------------------------


/*---------------------------------------
    Machine Software Interrupt Handler
---------------------------------------*/
.balign 4, 0
.global _msw_interrupt_handler
.type _msw_interrupt_handler, @function          # Mark the symbol as being a function name
_msw_interrupt_handler:

    /* Clear SWI : */
    la t0, CLINT_MSIP
    sw x0, 0(t0)

    /* Load SP of the first task: */
    la t1, TCB      // load the address of TCB[0]
    lw sp, 0(t1)    // load sp from TCB[0].sp

    # Restore the context of Task0
    __macro_RESTORE_CONTEXT

    mret
    .size   _msw_interrupt_handler, .- _msw_interrupt_handler


/*---------------------------------------
    Machine Timer Interrupt Handler
---------------------------------------*/
.weak _mtim_interrupt_handler
.balign 4, 0
.global _mtim_interrupt_handler
.type _mtim_interrupt_handler, @function     # Mark the symbol as being a function name
_mtim_interrupt_handler:

    # Save context:
    __macro_SAVE_CONTEXT

    # Decode interrupt cause:
    csrr t0, mcause                 # read exception cause
    bgez t0, 2f                     # exit if not an interrupt

    # Increment time slice (tick)
    __macro_INCREMENT_TICK

    # switch context
    __macro_SWITCH_CONTEXT

2:
    # Restore context
    __macro_RESTORE_CONTEXT

    mret
    .size  _mtim_interrupt_handler, .-_mtim_interrupt_handler




/*---------------------------------------
    Machine External Interrupt Handler
---------------------------------------*/
.balign 4
.global _mext_interrupt_handler
.type _mext_interrupt_handler, @function          # Mark the symbol as being a function name
_mext_interrupt_handler:
    # Prologue : save 16 ABI caller registers (ra, t0-t6, a0-a7)
    addi sp, sp, -16*REGSIZEBYTES   # Allocate the routine stack frame
    sw t0, 0*REGSIZEBYTES(sp)
    sw t1, 1*REGSIZEBYTES(sp)
    sw t2, 2*REGSIZEBYTES(sp)
    sw t3, 3*REGSIZEBYTES(sp)
    sw t4, 4*REGSIZEBYTES(sp)
    sw t5, 5*REGSIZEBYTES(sp)
    sw t6, 6*REGSIZEBYTES(sp)
    sw a0, 7*REGSIZEBYTES(sp)
    sw a1, 8*REGSIZEBYTES(sp)
    sw a2, 9*REGSIZEBYTES(sp)
    sw a3, 10*REGSIZEBYTES(sp)
    sw a4, 11*REGSIZEBYTES(sp)
    sw a5, 12*REGSIZEBYTES(sp)
    sw a6, 13*REGSIZEBYTES(sp)
    sw a7, 14*REGSIZEBYTES(sp)
    sw ra, 15*REGSIZEBYTES(sp)

    # Decode interrupt cause:
    csrr t0, mcause                 # read exception cause
    bgez t0, 1f                     # exit if not an interrupt

    # Claim the interrupt - read CLAIM/COMPLETE - A non-zero read ontains the id of the highest pending interrupt.
    la t0, PLIC_CLAIM               # load the address of CLAIM reg
    lw t1, 0(t0)                    # read the CLAIM reg. t1 holds the id of the highest pending interrupt
                                    # reading CLAIM delets the pending bit
    slli t2, t1, 2                  # id * 4 to obtain the offset

    la t3, _plic_ext_vector_table   # load the address of the external interrupt vector table
    add t3, t3, t2                  # ext_vector_table + 4*id
    jalr t3                         # call interrupt handler that procees the id interrupt

    # Complete the interrupt - A write to the CLAIM/COMPLETE register signals completion of the interrupt id written
    ;sw t1, 0(t0)

1:
    # epilogue: restore ABI caller regs
    lw t0, 0*REGSIZEBYTES(sp)
    lw t1, 1*REGSIZEBYTES(sp)
    lw t2, 2*REGSIZEBYTES(sp)
    lw t3, 3*REGSIZEBYTES(sp)
    lw t4, 4*REGSIZEBYTES(sp)
    lw t5, 5*REGSIZEBYTES(sp)
    lw t6, 6*REGSIZEBYTES(sp)
    lw a0, 7*REGSIZEBYTES(sp)
    lw a1, 8*REGSIZEBYTES(sp)
    lw a2, 9*REGSIZEBYTES(sp)
    lw a3, 10*REGSIZEBYTES(sp)
    lw a4, 11*REGSIZEBYTES(sp)
    lw a5, 12*REGSIZEBYTES(sp)
    lw a6, 13*REGSIZEBYTES(sp)
    lw a7, 14*REGSIZEBYTES(sp)
    lw ra, 15*REGSIZEBYTES(sp)
    addi sp, sp, 16*REGSIZEBYTES    # de-allocate the routine stack frame
    mret
    .size   _mext_interrupt_handler, .- _mext_interrupt_handler




# ------------------------------------------------------------------------------
#
#  D E F / P A N I C  I N T E R R U P T   H A N D L E R S
#
# ------------------------------------------------------------------------------


/*---------------------------------------
    Default Handler:
        do nothing, just retur form interrupt
---------------------------------------*/
.balign 4
.global _default_handler
.type _default_handler, @function
_default_handler:
    mret
    .size   _default_handler, .- _default_handler


/*---------------------------------------
    Panic Handler:
        stay forever in the handler with interrupts disabled
---------------------------------------*/
.balign 4
.global _panic_handler
.type _panic_handler, @function 
_panic_handler:
    j _panic_handler
    .size   _panic_handler, .- _panic_handler











