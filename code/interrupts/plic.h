/*
 * plic.h
 *
 *  Created on: 24 Sep 2023
 *      Author: patriciobulic
 */

/*
# --------------------------------------------------------------------
# P L I C   M E M O R Y   M A P :
# Pa3cio, UL FRI, 2023
# --------------------------------------------------------------------
*/

#define PLIC_INT_PRIORITY_BASE     	0x0C000000
#define PLIC_INT_PENDING1          	0x0C001000
#define PLIC_INT_PENDING2          	0x0C001004
#define PLIC_INT_ENABLE1           	0x0C002000
#define PLIC_INT_ENABLE2           	0x0C002004
#define PLIC_INT_THRESHOLD          0x0C200000
#define PLIC_CLAIM        			0x0C200004


#define	PLIC_PRIORITY_0				0
#define	PLIC_PRIORITY_1				1
#define	PLIC_PRIORITY_2				2
#define	PLIC_PRIORITY_3				3
#define	PLIC_PRIORITY_4				4
#define	PLIC_PRIORITY_5				5
#define	PLIC_PRIORITY_6				6
#define	PLIC_PRIORITY_7				7
#define	PLIC_PRIORITY_LOWEST		PLIC_PRIORITY_1
#define	PLIC_PRIORITY_HIGHEST		PLIC_PRIORITY_7
#define	PLIC_PRIORITY_DISABLED		PLIC_PRIORITY_0



#define PLIC_SRC_UART0				3
#define PLIC_SRC_UART1				4

#define PLIC_SRC_GPIO0				8
#define PLIC_SRC_GPIO1				9
#define PLIC_SRC_GPIO2				10
#define PLIC_SRC_GPIO3				11
#define PLIC_SRC_GPIO4				12
#define PLIC_SRC_GPIO5				13
#define PLIC_SRC_GPIO6				14
#define PLIC_SRC_GPIO7				15

#define PLIC_SRC_GPIO8				16
#define PLIC_SRC_GPIO9				17
#define PLIC_SRC_GPIO10				18
#define PLIC_SRC_GPIO11				19
#define PLIC_SRC_GPIO12				20
#define PLIC_SRC_GPIO13				21
#define PLIC_SRC_GPIO14				22
#define PLIC_SRC_GPIO15				23

#define PLIC_SRC_GPIO16				24
#define PLIC_SRC_GPIO17				25
#define PLIC_SRC_GPIO18				26
#define PLIC_SRC_GPIO19				27
#define PLIC_SRC_GPIO20				28
#define PLIC_SRC_GPIO21				29
#define PLIC_SRC_GPIO22				30
#define PLIC_SRC_GPIO23				31

#define PLIC_SRC_GPIO24				32
#define PLIC_SRC_GPIO25				33
#define PLIC_SRC_GPIO26				34
#define PLIC_SRC_GPIO27				35
#define PLIC_SRC_GPIO28				36
#define PLIC_SRC_GPIO29				37
#define PLIC_SRC_GPIO30				38
#define PLIC_SRC_GPIO31				39





void plic_set_priority(unsigned int source, unsigned int priority);
void plic_set_threshold(unsigned int threshold);
void plic_enable_source(unsigned int source);
void plic_disable_all();
void plic_claim_amd_complete();


