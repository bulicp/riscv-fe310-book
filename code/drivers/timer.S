# ----------------------------------------------------------------------------
#
# CLINT Timer Driver
# Pa3cio, UL FRI
# 2023
#
# mtime is a 64-bit read-write register that contains the number of cycles
# counted from the rtcclk. The 48-bit RTC runs at 32.768 kHz.
# 48-bit counter width ensures there will no counter rollover for over 270 years
#
# assuming 32.768 kHz:
# 1s = 32768
# 100 ms = 3277
# 0.5s = 16384
#
# ----------------------------------------------------------------------------

.section .text

.include "memory_map.inc"

.option norvc       # disables the generation of compressed instructions.



/*---------------------------------------
    Set Timer Compare Register

    # a0 - delay
---------------------------------------*/
.balign 4
.global set_timer_compare
.type set_timer_compare, @function
set_timer_compare:
    # prologue (save three saved registers and the return address)
    addi sp, sp, -16                # Allocate the routine stack frame
    sw ra, 12(sp)                   # Save the return address
    sw fp, 8(sp)                    # Save the frame pointer
    sw s1, 4(sp)
    sw s2, 0(sp)
    addi fp, sp, 16                 # Set the framepointer

    li t0, CLINT_MTIME              # load the timer register address
    lw t1, 0(t0)                    # load the current timer value (LO)
    lw t2, 4(t0)                    # load the current timer value (HI)

    add t1,t1,a0                   # add delay
    li t0, CLINT_MTIME_CMP          # and store it to mtimecmp
    sw t2, 4(t0)
    sw t1, 0(t0)

    # epilogue (restore three saved registers and the return address):
    lw s2, 0(sp)
    lw s1, 4(sp)
    lw fp, 8(sp)                    # restore the frame pointer
    lw ra, 12(sp)                   # restore the return address from the stack frame
    addi sp, sp, 16                 # de-allocate the routine stack frame
    ret
