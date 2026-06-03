/*
 * mtime.c
 *
 *  Created on: 14 Sep 2023
 *      Author: Pa3cio, UL FRI
 */
#include "mtime.h"
#include "pb-tasks.h"

unsigned int mtime_int_flag = 0;

extern volatile int current_task;
extern TCB_Type TCB[NTASKS];


/*
 * This is an example of the MTIM Interrupt Handler in C
 * Use "interrupt" attribute to indicate that the specified function
 * is an interrupt handler. The compiler generates function entry
 * and exit sequences suitable for use in an interrupt handler
 * when this attribute is present.
 *
 * Sep. 2023
 * Pa3cio, UL FRI
 */


__attribute__ ((weak, interrupt))
void mtime_handler (void) {

	volatile unsigned int mcause_value;
	// Decode interrupt cause:
	// Non memory-mapped CSR registers can only be accessed
	// using special CSR instructions. Hence, we should use
	// inline assembly:
	__asm__ volatile ("csrr %0, mcause"
	                   : "=r" (mcause_value) /* output */
	                   : /* input : none */
	                   : /* clobbers: none */);


	if (mcause_value & 0x8000007) { // mtime interrupt!

		// Save context:
		__SAVE_CONTEXT();
		TCB[current_task].sp = (unsigned int*) __get_SP();

		// Switch context:
		current_task = SelectNewTask(current_task);

		// Restore context:
		__set_SP((unsigned int)TCB[current_task].sp);
		__RESTORE_CONTEXT();

		// Increment timer compare by 1 ms:
		*(uint64_t*)MTIMECMP_ADDRESS = *(uint64_t*)MTIME_ADDRESS + (uint64_t)33;

		// set the flag:
		mtime_int_flag = 1;

	}
	else { // Store access fault
		__asm__ volatile("nop");
	}
}


void mtime_delay(uint32_t miliseconds){

	for (int i=0; i<miliseconds; i++) {
		// wait for the flag from mtime interrupt handler:
		while (!mtime_int_flag) {}
		mtime_int_flag = 0;
	}
}

void mtime_set_mtimecmp(uint64_t delay){
	*(uint64_t*)MTIMECMP_ADDRESS =
			*(uint64_t*)MTIME_ADDRESS + (uint64_t)delay;
}


