/*
 * plic.c
 *
 *  Created on: 24 Sep 2023
 *      Author: patriciobulic
 */

#include "plic.h"


/* Set interrupt priority
 *
 * Interrupt source id: 1-52 */
    /* Interrupt priority levels 7
    * Bits 2:0
    * 0 - never interrupt/disables interrupt
    * 1 - lowest active priority
    * 7 - highest priority
    * Tie between sources of same priority - resolved
    * by Interrupt ID. lowest ID highest effective priority */
    /* Write priority into (PLIC_INT_PRIORITY + 4*source id ) */

void plic_set_priority(unsigned int source, unsigned int priority){

	*((unsigned int *)PLIC_INT_PRIORITY_BASE + source) = priority;
}


void plic_set_threshold(unsigned int threshold){

	*((unsigned int *)PLIC_INT_THRESHOLD) = threshold;
}


/*
 * Enable interrupt source in enable registers
 */
void plic_enable_source(unsigned int source){
	unsigned int bit_position = source | 32;
	unsigned int enable_reg = source / 32;

	*((unsigned int *)PLIC_INT_ENABLE1 + enable_reg) |= (1 << bit_position);
}


/*
 * Disable all PLIC interrupts
 */
void plic_disable_all(){
	*((unsigned int *)PLIC_INT_ENABLE1 + 0) = 0x00000000;
	*((unsigned int *)PLIC_INT_ENABLE1 + 1) = 0x00000000;
}



/*
 * Claim and complete
 */
void plic_claim_amd_complete(){

	// 1. claim:
	volatile unsigned int claim = *((unsigned int *)PLIC_CLAIM);

	// 2. complete:
	*((unsigned int *)PLIC_CLAIM) = claim;
}


