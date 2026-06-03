/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include "main.h"
#include "mtime.h"
#include "pb-tasks.h"
#include "hal_gpio.h"


GPIO_TypeDef* GPIO = GPIO_BASE_ADDRESS;

extern unsigned int stackRegion[NTASKS * TASK_STACK_SIZE];
extern TCB_Type TCB[NTASKS];
void (*TaskFunctions[NTASKS])();
extern volatile int current_task;

void GPIO_Init();


int main() {

	GPIO_Init();

	/*
	 * Set timer compare for the first mtime interrupt
	 */
	//mtime_set_mtimecmp(16384); // first interrupt in 500 ms

	/*
	 *
	 */
	TaskFunctions[0] = Task0;
    TaskFunctions[1] = Task1;
    TaskFunctions[2] = Task2;
    TaskFunctions[3] = Task3;

    /*
     *
     */
    InitScheduler(stackRegion, TCB, TaskFunctions);
    current_task = 0;

	/*
	 * Set up vectored interrupts and enable CPU's interrupts
	 */
	_register_handler(_vector_table, INT_MODE_VECTORED);
	_enable_global_interrupts();
	//enable_mexternal_interrupt();


	//Environment call - start the scheduler:
    __asm__ volatile("ecall");

	// software interrupt - start the scheduler:
	//*((uint32_t *)CLINT_MSIP) = 0x00000001;

	while(1){}

	return 0;
}

void GPIO_Init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_GPIO_WritePin(GPIO, LED_GREEN_PIN | LED_BLUE_PIN | LED_RED_PIN, GPIO_PIN_SET);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Pin = LED_GREEN_PIN | LED_BLUE_PIN | LED_RED_PIN;
	HAL_GPIO_Init(GPIO, &GPIO_InitStruct);
}

