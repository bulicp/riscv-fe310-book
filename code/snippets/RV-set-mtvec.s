#---------------------------------------
#    Register the base address for vector table 
#      in mtvec
#
#@arguments:
#    # a0 - interrupt vector table base address
#    # a1 - interrupt processing mode 
#            (0x0 - direct, 0x1 - vectored)
#---------------------------------------
.balign 4
.global register_handler
.type register_handler, @function
register_handler:
    # prologue:
    addi sp, sp, -16    # Allocate the routine 
                        #   stack frame
    sw ra, 12(sp)       # Save the return address
    sw fp, 8(sp)        # Save the frame pointer
    sw s1, 4(sp)
    sw s2, 0(sp)
    addi fp, sp, 16     # Set the framepointer

    or a0, a0, a1       # OR base address with mode
    csrw mtvec, a0      # and save into mtvec

    # epilogue:
    lw s2, 0(sp)
    lw s1, 4(sp)
    lw fp, 8(sp)        # restore the frame pointer
    lw ra, 12(sp)       # restore the return address
    addi sp, sp, 16     # de-allocate the routine 
                        #   stack frame
    ret