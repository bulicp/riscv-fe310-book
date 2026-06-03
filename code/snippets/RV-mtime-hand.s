#----------------------------------------
#   Machine Timer Interrupt Handler
#----------------------------------------
.balign 4
.global _mtim_interrupt_handler
_mtim_interrupt_handler:

# Prologue : 
#    save 16 ABI caller registers 
#    (ra, t0-t6, a0-a7)
addi sp, sp, -16*4   # Allocate the routine stack frame
sw t0, 0*4(sp)
sw t1, 1*4(sp)
sw t2, 2*4(sp)
sw t3, 3*4(sp)  
sw t4, 4*4(sp)
sw t5, 5*4(sp)
sw t6, 6*4(sp)  
sw a0, 7*4(sp)
sw a1, 8*4(sp)
sw a2, 9*4(sp)
sw a3, 10*4(sp)  
sw a4, 11*4(sp)
sw a5, 12*4(sp)
sw a6, 13*4(sp) 
sw a7, 14*4(sp)  
sw ra, 15*4(sp)             

# Decode interrupt cause
csrr t0, mcause     # read exception cause
bgez t0, 1f         # exit if not an interrupt

# Increment timer compare by 1000 cycles
li t0, 0x0200BFF8   # load the mtime address
lw t1, 0(t0)        # load mtime (LO)
lw t2, 4(t0)        # load mtime (HI)
li t3, 1000         # load 1000 cycles
add t3, t1, t3      # increment lower bits by 1000
sltu t1, t3, t1     # generate carry-out
add t2, t2, t1      # increment upper bits with carry

li t0, 0x02004000   # load the mtimecmp address
sw t3, 0(t0)        # update mtimecmp (LO)
sw t2, 4(t0)        # update mtimecmp (HI)

1:
# Epilogue: restore ABI caller regs
lw t0, 0*4(sp)
lw t1, 1*4(sp)
lw t2, 2*4(sp)
lw t3, 3*4(sp)  
lw t4, 4*4(sp)
lw t5, 5*4(sp)
lw t6, 6*4(sp)  
lw a0, 7*4(sp)
lw a1, 8*4(sp)
lw a2, 9*4(sp)
lw a3, 10*4(sp)  
lw a4, 11*4(sp)
lw a5, 12*4(sp)
lw a6, 13*4(sp) 
lw a7, 14*4(sp)  
lw ra, 15*4(sp)                 
addi sp, sp, 16*4   # de-allocate the routine stack frame
mret