unsigned int *pMTime 	= (unsigned int *)0x0200bff8;
unsigned int *pMTimeCmp = (unsigned int *)0x02004000;

__attribute__ ((interrupt)) void mtime__handler (void) {

	unsigneg int mcause_value;
	// Decode interrupt cause:
	// Non memory-mapped CSR registers can only be accessed
	// using special CSR instructions. Hence, we should use
	// inline assembly:
	__asm__ volatile ("csrr %0, mcause"
	                   : "=r" (mcause_value) /* output */
	                   : /* input : none */
	                   : /* clobbers: none */
    );

	if (mcause_value & 0x8000007) { // mtime interrupt!
		// Increment timer compare by 500 ms:
		*pMTimeCmp = *pMTime + 16384;
	}
}