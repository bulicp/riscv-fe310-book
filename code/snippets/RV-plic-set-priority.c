#define PLIC_INT_PRIORITY_BASE     	0x0C000000

/* Set interrupt priority
 *
 * Interrupt source id: 1-52 
    * Interrupt priority levels 7
    * Bits 2:0
    * 0 - never interrupt/disables interrupt
    * 1 - lowest active priority
    * 7 - highest priority */

void plic_set_priority(unsigned int source, unsigned int priority){

	*((unsigned int *)PLIC_INT_PRIORITY_BASE + source) = priority;
}