/*---------------------------------------
    Machine External Interrupt Handler
---------------------------------------*/
.balign 4
.global _mext_interrupt_handler
.type _mext_interrupt_handler, @function
_mext_interrupt_handler:
    # Prologue : save 16 ABI caller registers 
    ...

    # Decode interrupt cause:
    csrr t0, mcause     # read exception cause
    bgez t0, 1f         # exit if not an interrupt

    # Claim the interrupt - read CLAIM
    #   A non-zero read contains the ID of 
    #   the highest pending interrupt.
    la t0, PLIC_CLAIM   # load the address of CLAIM reg
    lw t1, 0(t0)        # read CLAIM
    slli t2, t1, 2      # id*4 to obtain the offset

    # load the address of the PLIC
    #   external interrupt vector table
    la t3, _plic_ext_vector_table   
    add t3, t3, t2      # ext_vector_table + 4*id
    jalr t3             # call interrupt handler

1:
    # epilogue: restore ABI caller regs
    ...

    mret