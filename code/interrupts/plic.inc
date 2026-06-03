/*
 * plic.inc
 *
 *  Created on: 24 Sep 2023
 *      Author: patriciobulic
 */

# --------------------------------------------------------------------
# P L I C   M E M O R Y   M A P :
# Pa3cio, UL FRI, 2023
# --------------------------------------------------------------------

.equ    PLIC_INT_PRIORITY_BASE,     0x0C000000
.equ    PLIC_INT_PENDING1,          0x0C001000
.equ    PLIC_INT_PENDING2,          0x0C001004
.equ    PLIC_INT_ENABLE1,           0x0C002000
.equ    PLIC_INT_ENABLE2,           0x0C002004
.equ    PLIC_INT_THR_REG,           0x0C200000
.equ    PLIC_CLAIM,        			0x0C200004

