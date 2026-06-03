

.macro  __macro_SAVE_CONTEXT
    addi sp, sp, -CONTEXT_SIZE
    sw x1,      1*WORD_SIZE(sp)
    sw x5,      2*WORD_SIZE(sp)
    sw x6,      3*WORD_SIZE(sp)
    sw x7,      4*WORD_SIZE(sp)
    sw x8,      5*WORD_SIZE(sp)
    sw x9,      6*WORD_SIZE(sp)
    sw x10,     7*WORD_SIZE(sp)
    sw x11,     8*WORD_SIZE(sp)
    sw x12,     9*WORD_SIZE(sp)
    sw x13,     10*WORD_SIZE(sp)
    sw x14,     11*WORD_SIZE(sp)
    sw x15,     12*WORD_SIZE(sp)
    sw x16,     13*WORD_SIZE(sp)
    sw x17,     14*WORD_SIZE(sp)
    sw x18,     15*WORD_SIZE(sp)
    sw x19,     16*WORD_SIZE(sp)
    sw x20,     17*WORD_SIZE(sp)
    sw x21,     18*WORD_SIZE(sp)
    sw x22,     19*WORD_SIZE(sp)
    sw x23,     20*WORD_SIZE(sp)
    sw x24,     21*WORD_SIZE(sp)
    sw x25,     22*WORD_SIZE(sp)
    sw x26,     23*WORD_SIZE(sp)
    sw x27,     24*WORD_SIZE(sp)
    sw x28,     25*WORD_SIZE(sp)
    sw x29,     26*WORD_SIZE(sp)
    sw x30,     27*WORD_SIZE(sp)
    sw x31,     28*WORD_SIZE(sp)

    csrr t0, mepc
    sw t0,      0*WORD_SIZE(sp)
    csrr t0, mstatus
    sw t0,      29*WORD_SIZE(sp)
.endm


.macro  __macro_RESTORE_CONTEXT
    lw t0,      0*WORD_SIZE(sp)
    csrw mepc, t0
    lw t0,      29*WORD_SIZE(sp)
    csrw mstatus, t0

    lw x1,      1*WORD_SIZE(sp)
    lw x5,      2*WORD_SIZE(sp)
    lw x6,      3*WORD_SIZE(sp)
    lw x7,      4*WORD_SIZE(sp)
    lw x8,      5*WORD_SIZE(sp)
    lw x9,      6*WORD_SIZE(sp)
    lw x10,     7*WORD_SIZE(sp)
    lw x11,     8*WORD_SIZE(sp)
    lw x12,     9*WORD_SIZE(sp)
    lw x13,     10*WORD_SIZE(sp)
    lw x14,     11*WORD_SIZE(sp)
    lw x15,     12*WORD_SIZE(sp)
    lw x16,     13*WORD_SIZE(sp)
    lw x17,     14*WORD_SIZE(sp)
    lw x18,     15*WORD_SIZE(sp)
    lw x19,     16*WORD_SIZE(sp)
    lw x20,     17*WORD_SIZE(sp)
    lw x21,     18*WORD_SIZE(sp)
    lw x22,     19*WORD_SIZE(sp)
    lw x23,     20*WORD_SIZE(sp)
    lw x24,     21*WORD_SIZE(sp)
    lw x25,     22*WORD_SIZE(sp)
    lw x26,     23*WORD_SIZE(sp)
    lw x27,     24*WORD_SIZE(sp)
    lw x28,     25*WORD_SIZE(sp)
    lw x29,     26*WORD_SIZE(sp)
    lw x30,     27*WORD_SIZE(sp)
    lw x31,     28*WORD_SIZE(sp)
    addi sp, sp, CONTEXT_SIZE
.endm


.macro __macro_INCREMENT_TICK
    # Increment timer compare by TIME_SLICE cycles
    la t0, CLINT_MTIME              # load the mtime address
    lw t1, 0(t0)                    # load mtime (LO)
    lw t2, 4(t0)                    # load mtime (HI)
    li t3, TIME_SLICE
    add t3, t1, t3                  # increment lower bits by TIME_SLICE cycles
    sltu t1, t3, t1                 # generate carry-out
    add t2, t2, t1                  # add carry to upper bits
    la t0, CLINT_MTIME_CMP
    sw t3, 0(t0)                    # update mtimecmp (LO)
    sw t2, 4(t0)                    # update mtimecmp (HI)
.endm



.macro __macro_SWITCH_CONTEXT
    # Switch context:
    la t0, current_task
    lw t1, 0(t0)                    # t1 holds current_task
    sll t4, t1, 3                   # t4 = t1 * 8
    la t5, TCB                      # t5 <- &TCB[0]
    add t5, t5, t4                  # t5 <- &TCB[current_task]
    sw sp, 0(t5)                    # save sp of the current task

    addi t1, t1, 1
    li t2, NTASKS
    bne t1, t2, 1f
    li t1, 0
1:  sw t1, 0(t0)

    sll t4, t1, 3                   # t4 = t1 * 8
    la t5, TCB                      # t5 <- &TCB[0]
    add t5, t5, t4                  # t5 <- &TCB[current_task]
    lw sp, 0(t5)                    # load sp of the current task

.endm

