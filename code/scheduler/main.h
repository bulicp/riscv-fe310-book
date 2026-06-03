/*
 * main.h
 *
 *  Created on: 14 Sep 2023
 *      Author: Pa3cio, UL FRI
 */


#ifndef MAIN_H
#define MAIN_H


#define MCAUSE_INT      0x80000000UL
#define MCAUSE_CAUSE    0x000003FFUL
#define MSTATUS_MIE_BIT     3  // global interrupt enable bit mask.
#define MIE_MTIE_BIT        7  // machine mode timer enable bit mask.

#define INT_MODE_DIRECT     0x00000000UL
#define INT_MODE_VECTORED   0x00000001UL



/* Timer */


/* Interrupts : */
void _vector_table();

/* CSR Functions */
void _register_handler(void *vec_table_base, unsigned int int_mode);
void _enable_global_interrupts();
void _enable_msoftware_interrupt();
void _enable_mtimer_interrupt();
void _enable_mexternal_interrupt();
void _switch_user_mode();

#endif /* MAIN_H */
