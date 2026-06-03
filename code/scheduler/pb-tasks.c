/*
 * pb-tasks.c
 *
 *  Created on: Oct 25, 2023
 *      Author: patriciobulic
 */

#include "pb-tasks.h"
#include "hal_gpio.h"

extern GPIO_TypeDef* GPIO;

unsigned int stackRegion[NTASKS * TASK_STACK_SIZE];
TCB_Type TCB[NTASKS];
TCB_Type* pTCB = (TCB_Type*)&TCB[0];
volatile int current_task = -1;
/*
	The function creates a new task. Creating a new task involves:

	1. making space for the task's stack. Each task gets 1kB for the stack
	2. sets the tasks stack pointer in the task table to point at the top of the SW stack frame

	After this steps a new task is ready to be initialized.

 	Pa3cio Bulic, 25.10.2023
*/

void TaskCreate(TCB_Type* pTCB, unsigned int* pTaskStackBase, void (*TaskFunction)()){

	pTCB->sp 			= (unsigned int*) pTaskStackBase;
	pTCB->pTaskFunction = TaskFunction;
}



/*
	The function initializes a task. Initializing a task involves:

	1. populating the initial HW stack frame for the task. HW stack frame initially resides
	   at the bottom of the task's stack. The HW stack frame is populated as follows:
	   PSR = 0x01000000 - this is the default reset value in the state register
	   PC = address of the task
	   LR = the address of the delete_task() function (if needed). In our case tasks never finish so LR=0xFFFFFFFF (reset value)
	   r12, r3-r0 = 0x00000000 - we may also pass the arguments into the tas via r0-r3, but this is not
	   	the case in our simple RTOS
	3. making room for the initial uninitialized SW stack frame.
	4. setting the task's stack pointer in the TCB to point at the top of the task's SW stack frame

	After initialization a new task is ready to be executed for the first time when task switch occurs and
	the task is selected for execution.

 	Pa3cio Bulic, 25.10.2023
*/

void TaskInit(TCB_Type* pTCB){
	Context_TypeDef* pStackFrame;



	// Make the room for the stack frame and set pointer to the top of stack frame:
	pStackFrame = (Context_TypeDef*)((void*)pTCB->sp - sizeof(Context_TypeDef));


	// populate Stack Frame
	pStackFrame->mepc 	  = (unsigned int) (pTCB->pTaskFunction);
	pStackFrame->x1		  = 0xFFFFFFFF; // (ra: task never exits)
	pStackFrame->mstatus  = (0x03 << 11) | (0x01 << 7); // value 0x1880: set MPP=11 and MPIE = 1

	// Set task's stack pointer in the TCB to point at the top of the task's SW stack frame
	pTCB->sp 			= (unsigned int*) pStackFrame;
}


/*
	The function initializes the scheduler.
	Our scheduler is very simple and has fixed number of tasks that are created
	at the start of scheduler.
	The tasks never stop and the number of the running task never changes.

 	Pa3cio Bulic, 25.10.2023
*/

void InitScheduler(unsigned int* pStackRegion, TCB_Type pTCB[], void (*TaskFunctions[])()){
	unsigned int* pTaskStackBase;

	// 1. create all tasks:
	for(int i=0; i<NTASKS; i++){
		pTaskStackBase = pStackRegion + (i+1)*TASK_STACK_SIZE;
		TaskCreate(&pTCB[i], pTaskStackBase, TaskFunctions[i]);
	}

	// 2. initialize all tasks:
	for(int i=0; i<NTASKS; i++){
		TaskInit(&pTCB[i]);
	}

	// set PSP to Task0.SP:
	//__set_SP((unsigned int*)pTCB[0].sp);
}




int SelectNewTask(int current_task){

	volatile int new_task;

	// select next task in round-robin fashion
	new_task = current_task + 1;
	if (new_task == NTASKS) new_task = 0;

	return new_task;
}


// This function should be reentrat as it is called by several threads
void primitivedelay(unsigned int delay){
	while(delay>0){
		__asm__ volatile ( "nop ;"
				           "nop");
		delay--;
	}
}


/*
 * TASKS:
 */

void Task0(){
	while(1) {
		//HAL_GPIO_TogglePin(GPIO, LED_BLUE_PIN);
		//primitivedelay0(100000);

		HAL_GPIO_WritePin(GPIO, LED_BLUE_PIN, GPIO_PIN_RESET);
		primitivedelay(50000);
		HAL_GPIO_WritePin(GPIO, LED_BLUE_PIN, GPIO_PIN_SET);
		primitivedelay(500000);

	}
}

void Task1(){
	while(1) {
		__asm__ volatile ( "nop ;"
						   "nop");
	}
}

void Task2(){
	primitivedelay(150000);
	while(1) {
		//HAL_GPIO_TogglePin(GPIO, LED_RED_PIN);
		HAL_GPIO_WritePin(GPIO, LED_RED_PIN, GPIO_PIN_RESET);
		primitivedelay(50000);
		HAL_GPIO_WritePin(GPIO, LED_RED_PIN, GPIO_PIN_SET);
		primitivedelay(500000);
	}
}

void Task3(){
	primitivedelay(300000);
	while(1) {
		__asm__ volatile ( "nop ;"
						   "nop");
		HAL_GPIO_WritePin(GPIO, LED_GREEN_PIN, GPIO_PIN_RESET);
		primitivedelay(50000);
		HAL_GPIO_WritePin(GPIO, LED_GREEN_PIN, GPIO_PIN_SET);
		primitivedelay(500000);



	}
}

