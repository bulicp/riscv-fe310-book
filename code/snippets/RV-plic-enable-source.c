#define PLIC_INT_ENABLE1           	0x0C002000

/*
 * Enable interrupt source in enable registers
 */
void plic_enable_source(unsigned int source){
	unsigned int bit_position = source | 32;
	unsigned int enable_reg = source / 32;

	*((unsigned int *)PLIC_INT_ENABLE1 + enable_reg) |= (1 << bit_position);
}