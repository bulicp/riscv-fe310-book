
.equ MIE_MTIE_BIT_MASK,     0x00000080  # bit 7

#---------------------------------------
#    Enable machine timer interrupt in mie
#---------------------------------------

.balign 4
.global enable_mtimer_interrupt
.type enable_mtimer_interrupt, @function
enable_mtimer_interrupt:
    # prologue:
    addi sp, sp, -16    # Allocate the routine 
                        #   stack frame
    sw ra, 12(sp)       # Save the return address
    sw fp, 8(sp)        # Save the frame pointer
    sw s1, 4(sp)
    sw s2, 0(sp)
    addi fp, sp, 16     # Set the framepointer

    li t0, MIE_MTIE_BIT_MASK
    csrs mie, t0        # set MTIE in mie

    # epilogue :
    lw s2, 0(sp)
    lw s1, 4(sp)
    lw fp, 8(sp)        # restore the frame pointer
    lw ra, 12(sp)       # restore the return address 
    addi sp, sp, 16     # de-allocate the routine 
                        #   stack frame
    ret