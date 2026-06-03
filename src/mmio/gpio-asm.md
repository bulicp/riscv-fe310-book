# Programming GPIO in assembly

Controlling a pin from assembly comes down to a **read-modify-write** on the right
register: load it, set/clear/toggle the one bit for the pin, store it back. The
pin number arrives in `a0` (the first argument register), and the register
offsets come from
[`gpio.inc`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/drivers/gpio.inc):

```asm
.equ GPIO_INPUT_VAL,   0x00
.equ GPIO_INPUT_EN,    0x04
.equ GPIO_OUTPUT_EN,   0x08
.equ GPIO_OUTPUT_VAL,  0x0C
# ...
.equ GREEN_LED_PIN,    19
.equ BLUE_LED_PIN,     21
.equ RED_LED_PIN,      22
```

The common idiom in every routine below is "shift a 1 to the pin position":

```asm
li  t2, 0x01
sll t2, t2, a0      # t2 = 1 << pin   -> a single-bit mask
```

The routines all share the same prologue/epilogue (saving `ra`, `fp`, `s1`, `s2`);
those lines are elided as `# ...` so the logic stands out. The complete functions
are in [`gpio.S`](https://github.com/bulicp/riscv-fe310-book/blob/main/code/drivers/gpio.S).

## Enable a pin as output

Set the pin's bit in `GPIO_OUTPUT_EN`:

```asm
.global gpio_output_en          # a0 = pin number
gpio_output_en:
    # ... prologue ...
    li  t0, GPIO0_BASE_ADDRESS
    lw  t1, GPIO_OUTPUT_EN(t0)  # read current value
    li  t2, 0x01
    sll t2, t2, a0              # mask = 1 << pin
    or  t1, t1, t2              # set the bit
    sw  t1, GPIO_OUTPUT_EN(t0)  # write back
    # ... epilogue ...
    ret
```

## Enable a pin as input

The same, on `GPIO_INPUT_EN`:

```asm
.global gpio_input_en           # a0 = pin number
gpio_input_en:
    # ... prologue ...
    li  t0, GPIO0_BASE_ADDRESS
    lw  t1, GPIO_INPUT_EN(t0)
    li  t2, 0x01
    sll t2, t2, a0
    or  t1, t1, t2
    sw  t1, GPIO_INPUT_EN(t0)
    # ... epilogue ...
    ret
```

## Set, clear, and toggle a pin

All three operate on `GPIO_OUTPUT_VAL`; only the bit operation differs.

**Set** — OR in the mask:

```asm
.global gpio_set_pin            # a0 = pin number
gpio_set_pin:
    # ... prologue ...
    li  t0, GPIO0_BASE_ADDRESS
    lw  t1, GPIO_OUTPUT_VAL(t0)
    li  t2, 0x01
    sll t2, t2, a0
    or  t1, t1, t2              # set pin
    sw  t1, GPIO_OUTPUT_VAL(t0)
    # ... epilogue ...
    ret
```

**Clear** — AND with the inverted mask:

```asm
.global gpio_clear_pin          # a0 = pin number
gpio_clear_pin:
    # ... prologue ...
    li  t0, GPIO0_BASE_ADDRESS
    lw  t1, GPIO_OUTPUT_VAL(t0)
    li  t2, 0x01
    sll t2, t2, a0
    not t2, t2                  # one's complement of the mask
    and t1, t1, t2              # clear pin
    sw  t1, GPIO_OUTPUT_VAL(t0)
    # ... epilogue ...
    ret
```

**Toggle** — XOR with the mask:

```asm
.global gpio_toggle_pin         # a0 = pin number
gpio_toggle_pin:
    # ... prologue ...
    li  t0, GPIO0_BASE_ADDRESS
    lw  t1, GPIO_OUTPUT_VAL(t0)
    li  t2, 0x01
    sll t2, t2, a0
    xor t1, t1, t2              # toggle pin
    sw  t1, GPIO_OUTPUT_VAL(t0)
    # ... epilogue ...
    ret
```

## Read a pin

Reading shifts the wanted bit of `GPIO_INPUT_VAL` down to bit 0 and returns it in
`a0`:

```asm
.global gpio_get_pin_val        # a0 = pin number, returns a0 = 0/1
gpio_get_pin_val:
    # ... prologue ...
    li  t0, GPIO0_BASE_ADDRESS
    lw  t1, GPIO_INPUT_VAL(t0)
    li  t2, 0x01
    sll t2, t2, a0
    and t1, t1, t2              # isolate the pin
    srl t1, t1, a0              # move it to the LSB
    add a0, x0, t1             # return in a0
    # ... epilogue ...
    ret
```

The pattern never changes — only the target register and the bit operation
(`or` / `and+not` / `xor`) do. That is the whole of bare-metal GPIO.
