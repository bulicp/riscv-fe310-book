# ----------------------------------------------------------------------------
#
# GPIO Driver
# Pa3cio, UL FRI
# 2023
# ----------------------------------------------------------------------------

.section .text

.include "memory_map.inc"
.include "gpio.inc"

.option norvc       # disables the generation of compressed instructions.

/*---------------------------------------
    GPIO input enable

    # a0 - pin number
---------------------------------------*/
.balign 4
.global gpio_input_en 
.type gpio_input_en, @function 
gpio_input_en:
    # prologue (save three saved registers and the return address)
    addi sp, sp, -16                # Allocate the routine stack frame
    sw ra, 12(sp)                   # Save the return address
    sw fp, 8(sp)                    # Save the frame pointer
    sw s1, 4(sp)
    sw s2, 0(sp)
    addi fp, sp, 16                 # Set the framepointer

    
    li t0, GPIO0_BASE_ADDRESS          
    lw t1, GPIO_INPUT_EN(t0)        # Read present value
    li t2, 0x01
    sll t2, t2, a0                  # shift 1 to the pin position
    or t1, t1, t2                   #
    sw t1, GPIO_INPUT_EN(t0)        # Store back to the register

    # epilogue (restore three saved registers and the return address):
    lw s2, 0(sp)
    lw s1, 4(sp)
    lw fp, 8(sp)                    # restore the frame pointer
    lw ra, 12(sp)                   # restore the return address from the stack frame
    addi sp, sp, 16                 # de-allocate the routine stack frame
    ret



/*---------------------------------------
    GPIO output enable

    # a0 - pin number
---------------------------------------*/
.balign 4
.global gpio_output_en 
.type gpio_output_en, @function 
gpio_output_en:
    # prologue (save three saved registers and the return address)
    addi sp, sp, -16                # Allocate the routine stack frame
    sw ra, 12(sp)                   # Save the return address
    sw fp, 8(sp)                    # Save the frame pointer
    sw s1, 4(sp)
    sw s2, 0(sp)
    addi fp, sp, 16                 # Set the framepointer


    li t0, GPIO0_BASE_ADDRESS          
    lw t1, GPIO_OUTPUT_EN(t0)       # Read present value
    li t2, 0x01
    sll t2, t2, a0                  # shift 1 to pin position 
    or t1, t1, t2                   # set the bit
    sw t1, GPIO_OUTPUT_EN(t0)       # Store back 

    # epilogue (restore three saved registers and the return address):
    lw s2, 0(sp)
    lw s1, 4(sp)
    lw fp, 8(sp)                    # restore the frame pointer
    lw ra, 12(sp)                   # restore the return address from the stack frame
    addi sp, sp, 16                 # de-allocate the routine stack frame
    ret




/*---------------------------------------
    GPIO get pin state

    # a0 - pin number
    # return: a0 - pin state
---------------------------------------*/
.balign 4
.global gpio_get_pin_val
.type gpio_get_pin_val, @function 
gpio_get_pin_val:
    # prologue (save three saved registers and the return address)
    addi sp, sp, -16                # Allocate the routine stack frame
    sw ra, 12(sp)                   # Save the return address
    sw fp, 8(sp)                    # Save the frame pointer
    sw s1, 4(sp)
    sw s2, 0(sp)
    addi fp, sp, 16                 # Set the framepointer


    li t0, GPIO0_BASE_ADDRESS 
    lw t1, GPIO_INPUT_VAL(t0)
    li t2, 0x01
    sll t2, t2, a0                  # shift 1 to pin position 
    and t1, t1, t2                  # select pin
    srl t1, t1, a0                  # shift pin value to LSB
    add a0, x0, t1                  # return bit in a0

    # epilogue (restore three saved registers and the return address):
    lw s2, 0(sp)
    lw s1, 4(sp)
    lw fp, 8(sp)                    # restore the frame pointer
    lw ra, 12(sp)                   # restore the return address from the stack frame
    addi sp, sp, 16                 # de-allocate the routine stack frame
    ret




/*---------------------------------------
    GPIO set pin

    # a0 - pin number
---------------------------------------*/
.balign 4
.global gpio_set_pin
.type gpio_set_pin, @function 
gpio_set_pin:
    # prologue (save three saved registers and the return address)
    addi sp, sp, -16                # Allocate the routine stack frame
    sw ra, 12(sp)                   # Save the return address
    sw fp, 8(sp)                    # Save the frame pointer
    sw s1, 4(sp)
    sw s2, 0(sp)
    addi fp, sp, 16                 # Set the framepointer


    li t0, GPIO0_BASE_ADDRESS 
    lw t1, GPIO_OUTPUT_VAL(t0)
    li t2, 0x01
    sll t2, t2, a0                  # shift 1 to pin position 
    or  t1, t1, t2                  # set pin
    sw t1, GPIO_OUTPUT_VAL(t0)

    # epilogue (restore three saved registers and the return address):
    lw s2, 0(sp)
    lw s1, 4(sp)
    lw fp, 8(sp)                    # restore the frame pointer
    lw ra, 12(sp)                   # restore the return address from the stack frame
    addi sp, sp, 16                 # de-allocate the routine stack frame
    ret



/*---------------------------------------
    GPIO togle pin

    # a0 - pin number
---------------------------------------*/
.balign 4
.global gpio_toggle_pin
.type gpio_toggle_pin, @function 
gpio_toggle_pin:
    # prologue (save three saved registers and the return address)
    addi sp, sp, -16                # Allocate the routine stack frame
    sw ra, 12(sp)                   # Save the return address
    sw fp, 8(sp)                    # Save the frame pointer
    sw s1, 4(sp)
    sw s2, 0(sp)
    addi fp, sp, 16                 # Set the framepointer


    li t0, GPIO0_BASE_ADDRESS 
    lw t1, GPIO_OUTPUT_VAL(t0)
    li t2, 0x01
    sll t2, t2, a0                  # shift 1 to pin position 
    xor  t1, t1, t2                 # toggle pin
    sw t1, GPIO_OUTPUT_VAL(t0)

    # epilogue (restore three saved registers and the return address):
    lw s2, 0(sp)
    lw s1, 4(sp)
    lw fp, 8(sp)                    # restore the frame pointer
    lw ra, 12(sp)                   # restore the return address from the stack frame
    addi sp, sp, 16                 # de-allocate the routine stack frame
    ret


/*---------------------------------------
    GPIO clear pin

    # a0 - pin number
---------------------------------------*/
.balign 4
.global gpio_clear_pin
.type gpio_clear_pin, @function 
gpio_clear_pin:
    # prologue (save three saved registers and the return address)
    addi sp, sp, -16                # Allocate the routine stack frame
    sw ra, 12(sp)                   # Save the return address
    sw fp, 8(sp)                    # Save the frame pointer
    sw s1, 4(sp)
    sw s2, 0(sp)
    addi fp, sp, 16                 # Set the framepointer


    li t0, GPIO0_BASE_ADDRESS 
    lw t1, GPIO_OUTPUT_VAL(t0)
    li t2, 0x01
    sll t2, t2, a0                  # shift 1 to pin position 
    not t2, t2                      # 1' complement
    and  t1, t1, t2                 # clear pin
    sw t1, GPIO_OUTPUT_VAL(t0)

    # epilogue (restore three saved registers and the return address):
    lw s2, 0(sp)
    lw s1, 4(sp)
    lw fp, 8(sp)                    # restore the frame pointer
    lw ra, 12(sp)                   # restore the return address from the stack frame
    addi sp, sp, 16                 # de-allocate the routine stack frame
    ret



