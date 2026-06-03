# --------------------------------------------------
#
#  P L I C   V E C T O R    T A B L E
#
# --------------------------------------------------
.balign 4
.global _plic_ext_vector_table
_plic_ext_vector_table:
    j _panic_handler                # PLIC src 0
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


    