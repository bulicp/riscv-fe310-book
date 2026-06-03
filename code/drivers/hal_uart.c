/*
 * hal_uart.c
 *
 *  Created on: 12 Oct 2023
 *      Author: patriciobulic
 */

#include "hal_uart.h"
#include "hal_gpio.h"

extern GPIO_TypeDef *GPIO;


/*
 * Init uart
 * @arguments:
 * 	uart: UART0 or UART1
 *
 */
void HAL_UART_Init(UART_TypeDef *uart, UART_InitTypeDef* UARTInitStruct){

	// select and enable IO FUNC
	if (uart == (UART_TypeDef*) UART0_BASEADDR){
		GPIO->GPIO_IOF_SEL &= UART0_PINS_SEL;
		GPIO->GPIO_IOF_EN |= UART0_PINS_EN;
	}
	if (uart == (UART_TypeDef*) UART1_BASEADDR){
		GPIO->GPIO_IOF_SEL &= UART1_PINS_SEL;
		GPIO->GPIO_IOF_EN |= UART1_PINS_EN;
	}

	// set baud rate:
	uart->UART_DIV = UARTInitStruct->BaudRate;

	// set the number of stop bits:
	if (UARTInitStruct->StopBits == UART_STOPBITS_1) {
		uart->UART_TXCTRL &= 0xfffffffd;
	}
	else if (UARTInitStruct->StopBits == UART_STOPBITS_2) {
		uart->UART_TXCTRL |= 0x00000002;
	}

	// set UART mode:
	if (UARTInitStruct->Mode == UART_MODE_TX){
		uart->UART_TXCTRL |= 0x00000001; // enable TX
		uart->UART_RXCTRL &= 0xfffffffe; // disable RX
	}
	else if (UARTInitStruct->Mode == UART_MODE_RX){
		uart->UART_RXCTRL |= 0x00000001; // enable RX
		uart->UART_TXCTRL &= 0xfffffffe; // disable TX
	}
	else if (UARTInitStruct->Mode == UART_MODE_TX_RX){
		uart->UART_RXCTRL |= 0x00000001; // enable RX
		uart->UART_TXCTRL |= 0x00000001; // enable TX
	}
}


/*
 * Init uart
 * @arguments:
 * 	uart: UART0 or UART1
 *
 */
void HAL_UART_DeInit(UART_TypeDef *uart){
	uart->UART_RXCTRL &= 0xfffffffe; // disable RX
	uart->UART_TXCTRL &= 0xfffffffe; // disable TX
}



/**
  * Send an amount of data.
  *
  * @param huart   UART handle.
  * @param pData   Pointer to data buffer (u8 data elements).
  * @param Size    Amount of data elements to be sent.
  */
void HAL_UART_Transmit(UART_TypeDef *uart, char *pData,  unsigned int Size){
	unsigned int txdata_reg;

	for (unsigned int i = 0; i<Size; i++){
	// Reading from txdata returns the current value of
		// the full flag and zero in the data field
		// wait until ready to accept new data.
		do {
			txdata_reg = uart->UART_TXDATA;
		}
		while (txdata_reg & 0x80000000);

		// transmit FIFO able to accept new entry
		uart->UART_TXDATA = pData[i];
	}
}

