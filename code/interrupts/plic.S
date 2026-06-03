/*
 * plic.S
 *
 *  Created on: 24 Sep 2023
 *      Author: patriciobulic
 */

.include "memory_map.inc"
.option norvc       # disables the generation of compressed instructions.



.section .text


# ------------------------------------------------------------------------------
#
#  P L I C   V E C T O R    T A B L E
#
# ------------------------------------------------------------------------------
.balign 4
.global _plic_ext_vector_table
_plic_ext_vector_table:
    j _panic_handler                # PLIC src 0 - invalid
    j _aon_wdt_handler              # PLIC src 1
    j _aon_rtc_handler              # PLIC src 2
    j _uart0_handler                # PLIC src 3
    j _uart1_handler                # PLIC src 4
    j _qspi0_handler                # PLIC src 5
    j _spi1_handler                 # PLIC src 6
    j _spi2_handler                 # PLIC src 7
    j _gpio0_handler                # PLIC src 8
    j _gpio1_handler                # PLIC src 9
    j _gpio2_handler                # PLIC src 10
    j _gpio3_handler                # PLIC src 11
    j _gpio4_handler                # PLIC src 12
    j _gpio5_handler                # PLIC src 13
    j _gpio6_handler                # PLIC src 14
    j _gpio7_handler                # PLIC src 15
    j _gpio8_handler                # PLIC src 16
    j _gpio9_handler                # PLIC src 17
    j _gpio10_handler               # PLIC src 18
    j _gpio11_handler               # PLIC src 19
    j _gpio12_handler               # PLIC src 20
    j _gpio13_handler               # PLIC src 21
    j _gpio14_handler               # PLIC src 22
    j _gpio15_handler               # PLIC src 23
    j _gpio16_handler               # PLIC src 24
    j _gpio17_handler               # PLIC src 25
    j _gpio18_handler               # PLIC src 26
    j _gpio19_handler               # PLIC src 27
    j _gpio20_handler               # PLIC src 28
    j _gpio21_handler               # PLIC src 29
    j _gpio22_handler               # PLIC src 30
    j _gpio23_handler               # PLIC src 31
    j _gpio24_handler               # PLIC src 32
    j _gpio25_handler               # PLIC src 33
    j _gpio26_handler               # PLIC src 34
    j _gpio27_handler               # PLIC src 35
    j _gpio28_handler               # PLIC src 36
    j _gpio29_handler               # PLIC src 37
    j _gpio30_handler               # PLIC src 38
    j _gpio31_handler               # PLIC src 39
    j _pwm0_handler                 # PLIC src 40
    j _pwm0_handler                 # PLIC src 41
    j _pwm0_handler                 # PLIC src 42
    j _pwm0_handler                 # PLIC src 43
    j _pwm1_handler                 # PLIC src 44
    j _pwm1_handler                 # PLIC src 45
    j _pwm1_handler                 # PLIC src 46
    j _pwm1_handler                 # PLIC src 47
    j _pwm2_handler                 # PLIC src 48
    j _pwm2_handler                 # PLIC src 49
    j _pwm2_handler                 # PLIC src 50
    j _pwm2_handler                 # PLIC src 51
    j _i2c_handler                  # PLIC src 52

# ------------------------------------------------------------------------------
#
#  P L I C   I N T E R R U P T   H A N D L E R S
#
# ------------------------------------------------------------------------------



.balign 4
.weak _aon_wdt_handler
_aon_wdt_handler:
    ret

.balign 4
.weak _aon_rtc_handler
_aon_rtc_handler:
    ret

.balign 4
.weak _uart0_handler
_uart0_handler:
    ret

.balign 4
.weak _uart1_handler
_uart1_handler:
    ret

.balign 4
.weak _qspi0_handler
_qspi0_handler:
    ret

.balign 4
.weak _spi1_handler
_spi1_handler:
    ret

.balign 4
.weak _spi2_handler
_spi2_handler:
    ret

.balign 4
.weak _gpio0_handler
_gpio0_handler:
    ret

.balign 4
.weak _gpio1_handler
_gpio1_handler:
    ret

.balign 4
.weak _gpio2_handler
_gpio2_handler:
    ret

.balign 4
.weak _gpio3_handler
_gpio3_handler:
    ret

.balign 4
.weak _gpio4_handler
_gpio4_handler:
    ret

.balign 4
.weak _gpio5_handler
_gpio5_handler:
    ret

.balign 4
.weak _gpio6_handler
_gpio6_handler:
    ret

.balign 4
.weak _gpio7_handler
_gpio7_handler:
    ret

.balign 4
.weak _gpio8_handler
_gpio8_handler:
    ret

.balign 4
.weak _gpio9_handler
_gpio9_handler:
    ret

.balign 4
.weak _gpio10_handler
_gpio10_handler:
    ret

.balign 4
.weak _gpio11_handler
_gpio11_handler:
    ret

.balign 4
.weak _gpio12_handler
_gpio12_handler:
    ret

.balign 4
.weak _gpio13_handler
_gpio13_handler:
    ret

.balign 4
.weak _gpio14_handler
_gpio14_handler:
    ret

.balign 4
.weak _gpio15_handler
_gpio15_handler:
    ret

.balign 4
.weak _gpio16_handler
_gpio16_handler:
    ret

.balign 4
.weak _gpio17_handler
_gpio17_handler:
    ret

.balign 4
.weak _gpio118_handler
_gpio18_handler:
    ret

.balign 4
.weak _gpio19_handler
_gpio19_handler:
    ret

.balign 4
.weak _gpio20_handler
_gpio20_handler:
    ret

.balign 4
.weak _gpio21_handler
_gpio21_handler:
    ret

.balign 4
.weak _gpio22_handler
_gpio22_handler:
    ret

.balign 4
.weak _gpio23_handler
_gpio23_handler:
    ret

.balign 4
.weak _gpio24_handler
_gpio24_handler:
    ret

.balign 4
.weak _gpio25_handler
_gpio25_handler:
    ret

.balign 4
.weak _gpio26_handler
_gpio26_handler:
    ret

.balign 4
.weak _gpio27_handler
_gpio27_handler:
    ret

.balign 4
.weak _gpio28_handler
_gpio28_handler:
    ret

.balign 4
.weak _gpio29_handler
_gpio29_handler:
    ret

.balign 4
.weak _gpio30_handler
_gpio30_handler:
    ret

.balign 4
.weak _gpio31_handler
_gpio31_handler:
    ret

.balign 4
.weak _pwm0_handler
_pwm0_handler:
    ret

.balign 4
.weak _pwm1_handler
_pwm1_handler:
    ret

.balign 4
.weak _pwm2_handler
_pwm2_handler:
    ret

.balign 4
.weak _i2c_handler
_i2c_handler:
    ret




