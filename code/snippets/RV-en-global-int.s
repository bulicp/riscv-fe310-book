
.equ MSTATUS_MIE_BIT_MASK,  0x00000008  # bit 3

#---------------------------------------
#    Enable global interrupts in mstatus
#---------------------------------------
.balign 4
.global enable_global_interrupts
.type enable_global_interrupts, @function
enable_global_interrupts:
    # prologue:
    addi sp, sp, -16    # Allocate the routine 
                        #     stack frame
    sw ra, 12(sp)       # Save the return address
    sw fp, 8(sp)        # Save the frame pointer
    sw s1, 4(sp)
    sw s2, 0(sp)
    addi fp, sp, 16     # Set the framepointer

    li t0, MSTATUS_MIE_BIT_MASK     
    csrs mstatus, t0    #  set the MIE bit in mstatus

    # epilogue:
    lw s2, 0(sp)
    lw s1, 4(sp)
    lw fp, 8(sp)        # restore the frame pointer
    lw ra, 12(sp)       # restore the return address 
    addi sp, sp, 16     # de-allocate the routine 
                        #     stack frame
    ret