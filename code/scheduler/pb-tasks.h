/*
 * pb-tasks.h
 *
 *  Created on: Oct 25, 2023
 *      Author: patriciobulic
 */

#ifndef SRC_PB_TASKS_H_
#define SRC_PB_TASKS_H_




#define NTASKS 4
#define TASK_STACK_SIZE 256

#define WORD_SIZE 4
#define CONTEXT_SIZE (32*WORD_SIZE)


typedef struct{
	unsigned int* sp;			// nazadnje videni SP opravila
	void (*pTaskFunction)();	// naslov finkcije, ki implementira opravilo
} TCB_Type;





/*
 *
 *
 * RISC-V maps registers to ABI names as follows (X1 to X31 integer registers
 * for the 'I' profile, X1 to X15 for the 'E' profile, currently I assumed).
 *
 * Register      ABI Name    Description                       Saver
 * x0            zero        Hard-wired zero                   -
 * x1            ra          Return address                    Caller
 * x2            sp          Stack pointer                     Callee
 * x3            gp          Global pointer                    -
 * x4            tp          Thread pointer                    -
 * x5-7          t0-2        Temporaries                       Caller
 * x8            s0/fp       Saved register/Frame pointer      Callee
 * x9            s1          Saved register                    Callee
 * x10-11        a0-1        Function Arguments/return values  Caller
 * x12-17        a2-7        Function arguments                Caller
 * x18-27        s2-11       Saved registers                   Callee
 * x28-31        t3-6        Temporaries                       Caller
 *
 * The RISC-V context is saved in the following stack frame,
 * where the global(tp) and thread (tp) pointers are currently assumed to be constant so
 * are not saved:
 *
 * mepc
 * x1 		(ra)
 * x5 		(t0)
 * x6		(t1)
 * x7		(t2)
 * x8		(s0/fp)
 * x9		(s1)
 * x10		(a0)
 * x11		(a1)
 * x12		(a2)
 * x13		(a3)
 * x14		(a4)
 * x15		(a5)
 * x16		(a6)
 * x17		(a7)
 * x18		(s2)
 * x19		(s3)
 * x20		(s4)
 * x21		(s5)
 * x22		(s6)
 * x23		(s7)
 * x24		(s8)
 * x25		(s9)
 * x26		(s10)
 * x27		(s11)
 * x28		(t3)
 * x29		(t4)
 * x30		(t5)
 * x31		(t6)
 * mstatus
 * -- (unused)
 * -- (unused)
 */
typedef struct{
	unsigned int mepc;		// (sp+0)
	unsigned int x1;		// (sp+1)
	unsigned int t5;		// (sp+2)
	unsigned int x6;		// (sp+3)
	unsigned int x7;		// (sp+4)
	unsigned int x8;		// (sp+5)
	unsigned int x9;		// (sp+6)
	unsigned int x10;		// (sp+7)
	unsigned int x11;		// (sp+8)
	unsigned int x12;		// (sp+9)
	unsigned int x13;		// (sp+10)
	unsigned int x14;		// (sp+11)
	unsigned int x15;		// (sp+12)
	unsigned int x16;		// (sp+13)
	unsigned int x17;		// (sp+14)
	unsigned int x18;		// (sp+15)
	unsigned int x19;		// (sp+16)
	unsigned int x20;		// (sp+17)
	unsigned int x21;		// (sp+18)
	unsigned int x22;		// (sp+19)
	unsigned int x23;		// (sp+20)
	unsigned int x24;		// (sp+21)
	unsigned int x25;		// (sp+22)
	unsigned int x26;		// (sp+23)
	unsigned int x27;		// (sp+24)
	unsigned int x28;		// (sp+25)
	unsigned int x29;		// (sp+26)
	unsigned int x30;		// (sp+27)
	unsigned int x31;		// (sp+28)
	unsigned int mstatus;	// (sp+29)
	unsigned int unused1;	// (sp+30)
	unsigned int unused2;	// (sp+31)
} Context_TypeDef;



void TaskCreate(TCB_Type* pTCB, unsigned int* pTaskStackBase, void (*TaskFunction)());
void TaskInit(TCB_Type* pTCB);
void InitScheduler(unsigned int* pStackRegion, TCB_Type pTCB[], void (*TaskFunctions[])());
void InitSchedulerSVC(unsigned int* pStackRegion, TCB_Type pTCB[], void (*TaskFunctions[])());
int  SelectNewTask(int current_task);

void primitivedelay0(unsigned int delay);
void primitivedelay2(unsigned int delay);


void Task0();
void Task1();
void Task2();
void Task3();



/*
	The inline function saves the context (r4-r11) into the software stack-frame on the process stack.
 	The hardware stack frame already contains the context saved by hardware (pc, lr, sp, r0-r3, r12, spr)

 	This function should be defined into the same translational module in which is use
 	as it is defined as static inline!

 	Pa3cio Bulic, 27.10.2023
*/


__attribute__((always_inline)) static inline void __SAVE_CONTEXT(void)
{
  __asm__ volatile
  (
  " addi sp, sp, -128       \n"
  " sw x1, 		1*4(sp)      \n"
  " sw x5, 		2*4(sp)      \n"
  " sw x6, 		3*4(sp)      \n"
  " sw x7, 		4*4(sp)      \n"
  " sw x8, 		5*4(sp)      \n"
  " sw x9, 		6*4(sp)      \n"
  " sw x10, 	7*4(sp)      \n"
  " sw x11, 	8*4(sp)      \n"
  " sw x12, 	9*4(sp)      \n"
  " sw x13, 	10*4(sp)     \n"
  " sw x14, 	11*4(sp)     \n"
  " sw x15, 	12*4(sp)     \n"
  " sw x16, 	13*4(sp)     \n"
  " sw x17, 	14*4(sp)     \n"
  " sw x18, 	15*4(sp)     \n"
  " sw x19, 	16*4(sp)     \n"
  " sw x20, 	17*4(sp)     \n"
  " sw x21, 	18*4(sp)     \n"
  " sw x22, 	19*4(sp)     \n"
  " sw x23, 	20*4(sp)     \n"
  " sw x24, 	21*4(sp)     \n"
  " sw x25, 	22*4(sp)     \n"
  " sw x26, 	23*4(sp)     \n"
  " sw x27, 	24*4(sp)     \n"
  " sw x28, 	25*4(sp)     \n"
  " sw x29, 	26*4(sp)     \n"
  " sw x30, 	27*4(sp)     \n"
  " sw x31, 	28*4(sp)     \n"
  " csrr t0, mepc            \n"
  " sw t0, 	    0*4(sp)      \n"
  " csrr t0, mstatus         \n"
  " sw t0, 	    29*4(sp)     \n"
  );
}



/*
	The function restores the context (r4-r11) from the software stack-frame on the process stack.

 	This function should be defined into the same translational module in which is use
 	as it is defined as static inline!

 	Pa3cio Bulic, 27.10.2023
*/


__attribute__((always_inline)) static inline void __RESTORE_CONTEXT(void)
{
  __asm__ volatile
  (
  " lw t0, 	    0*4(sp)      \n"
  " csrw mepc, t0            \n"
  " lw t0, 	    29*4(sp)     \n"
  " csrw mstatus, t0         \n"
  " lw x1, 		1*4(sp)      \n"
  " lw x5, 		2*4(sp)      \n"
  " lw x6, 		3*4(sp)      \n"
  " lw x7, 		4*4(sp)      \n"
  " lw x8, 		5*4(sp)      \n"
  " lw x9, 		6*4(sp)      \n"
  " lw x10, 	7*4(sp)      \n"
  " lw x11, 	8*4(sp)      \n"
  " lw x12, 	9*4(sp)      \n"
  " lw x13, 	10*4(sp)     \n"
  " lw x14, 	11*4(sp)     \n"
  " lw x15, 	12*4(sp)     \n"
  " lw x16, 	13*4(sp)     \n"
  " lw x17, 	14*4(sp)     \n"
  " lw x18, 	15*4(sp)     \n"
  " lw x19, 	16*4(sp)     \n"
  " lw x20, 	17*4(sp)     \n"
  " lw x21, 	18*4(sp)     \n"
  " lw x22, 	19*4(sp)     \n"
  " lw x23, 	20*4(sp)     \n"
  " lw x24, 	21*4(sp)     \n"
  " lw x25, 	22*4(sp)     \n"
  " lw x26, 	23*4(sp)     \n"
  " lw x27, 	24*4(sp)     \n"
  " lw x28, 	25*4(sp)     \n"
  " lw x29, 	26*4(sp)     \n"
  " lw x30, 	27*4(sp)     \n"
  " lw x31, 	28*4(sp)     \n"
  " addi sp, sp, 128         \n"
  );
}



/*
	The function sets SP

 	This function should be defined into the same translational module in which is use
 	as it is defined as static inline!

 	Pa3cio Bulic, 31.10.2023
*/
__attribute__((always_inline)) static inline void __set_SP(unsigned int stack_pointer)
{
	__asm__ volatile
	(
	  " add sp, x0, %0      \n"
	  :: "r" (stack_pointer)
	);
}



/*
	The function gets SP

 	This function should be defined into the same translational module in which is use
 	as it is defined as static inline!

 	Pa3cio Bulic, 31.10.2023
*/
__attribute__((always_inline)) static inline unsigned int __get_SP()
{
	unsigned int stack_pointer;
	__asm__ volatile
	(
	  " add %0, x0, sp      \n"
	  : "=r" (stack_pointer)
	);
	return stack_pointer;
}




#endif /* SRC_PB_TASKS_H_ */
