/*
 * hal_uart.h
 *
 *  Created on: 12 Oct 2023
 *      Author: patriciobulic
 */

#ifndef HAL_UART_H_
#define HAL_UART_H_


#define UART0_BASEADDR     0x10013000
#define UART1_BASEADDR     0x10023000

#define UART0_RX_PIN           16
#define UART0_TX_PIN           17
#define UART0_PINS_SEL         0xfffcffff  // pins 16 and 17
#define UART0_PINS_EN          0x00030000  // pins 16 and 17
#define UART1_TX_PIN           18
#define UART1_RX_PIN           23
#define UART1_PINS_SEL         0xff7bffff  // pins 18 and 23
#define UART1_PINS_EN          0x00840000  // pins 18 and 23


#define UART_MODE_TX		   0x0
#define UART_MODE_RX		   0x1
#define UART_MODE_TX_RX        0x2


#define UART_STOPBITS_1        0xfffffffd
#define UART_STOPBITS_2        0x00000002

#define UART_BAUD_115200	   139	// this is clock divider for 115200


typedef struct
{
	volatile int UART_TXDATA;
	volatile int UART_RXDATA;
	volatile int UART_TXCTRL;
	volatile int UART_RXCTRL;
	volatile int UART_IE;
	volatile int UART_IP;
	volatile int UART_DIV;
}UART_TypeDef;

typedef struct
{
  unsigned int BaudRate;		// UART communication baud rate. Only 115200 is supported
  unsigned int StopBits;		// Specifies the number of stop bits transmitted.
  unsigned int Mode;			// Specifies whether the Receive or Transmit mode is enabled or disabled
} UART_InitTypeDef;



#define UART0 ((UART_TypeDef *)UART0_BASEADDR)
#define UART1 ((UART_TypeDef *)UART1_BASEADDR)




#endif /* HAL_UART_H_ */
