/*
 * gpio-ors.h
 *
 *  Created on: 7 Oct 2023
 *      Author: Patricio Bulic
 */

#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_


typedef unsigned int uint32t;

#define GPIO_MODE_INPUT 0x00U
#define GPIO_MODE_OUTPUT 0x01U


/* GPIO pins define
  *
  */
#define GPIO_PIN_0                 ((uint32t)0x00000001)
#define GPIO_PIN_1                 ((uint32t)0x00000002)
#define GPIO_PIN_2                 ((uint32t)0x00000004)
#define GPIO_PIN_3                 ((uint32t)0x00000008)
#define GPIO_PIN_4                 ((uint32t)0x00000010)
#define GPIO_PIN_5                 ((uint32t)0x00000020)
#define GPIO_PIN_6                 ((uint32t)0x00000040)
#define GPIO_PIN_7                 ((uint32t)0x00000080)
#define GPIO_PIN_8                 ((uint32t)0x00000100)
#define GPIO_PIN_9                 ((uint32t)0x00000200)
#define GPIO_PIN_10                ((uint32t)0x00000400)
#define GPIO_PIN_11                ((uint32t)0x00000800)
#define GPIO_PIN_12                ((uint32t)0x00001000)
#define GPIO_PIN_13                ((uint32t)0x00002000)
#define GPIO_PIN_14                ((uint32t)0x00004000)
#define GPIO_PIN_15                ((uint32t)0x00008000)
#define GPIO_PIN_16                ((uint32t)0x00010000)
#define GPIO_PIN_17                ((uint32t)0x00020000)
#define GPIO_PIN_18                ((uint32t)0x00040000)
#define GPIO_PIN_19                ((uint32t)0x00080000)
#define GPIO_PIN_20                ((uint32t)0x00100000)
#define GPIO_PIN_21                ((uint32t)0x00200000)
#define GPIO_PIN_22                ((uint32t)0x00400000)
#define GPIO_PIN_23                ((uint32t)0x00800000)
#define GPIO_PIN_24                ((uint32t)0x01000000)
#define GPIO_PIN_25                ((uint32t)0x02000000)
#define GPIO_PIN_26                ((uint32t)0x04000000)
#define GPIO_PIN_27                ((uint32t)0x08000000)
#define GPIO_PIN_28                ((uint32t)0x10000000)
#define GPIO_PIN_29                ((uint32t)0x20000000)
#define GPIO_PIN_30                ((uint32t)0x40000000)
#define GPIO_PIN_31                ((uint32t)0x80000000)

#define LED_GREEN_PIN              GPIO_PIN_19
#define LED_BLUE_PIN               GPIO_PIN_21
#define LED_RED_PIN                GPIO_PIN_22



/**
  * @brief General Purpose I/O
  */
typedef struct
{
	volatile uint32t GPIO_INPUT_VAL;	/* GPIO input value data register		Address offset 0x00 */
	volatile uint32t GPIO_INPUT_EN;		/* GPIO input enable register			Address offset 0x04 */
	volatile uint32t GPIO_OUTPUT_EN;	/* GPIO output enable register			Address offset 0x08 */
	volatile uint32t GPIO_OUTPUT_VAL;	/* GPIO output value data register		Address offset 0x0C */
	volatile uint32t GPIO_PUE;
	volatile uint32t GPIO_DS;
	volatile uint32t GPIO_RISE_IE;
	volatile uint32t GPIO_RISE_IP;
	volatile uint32t GPIO_FALL_IE;
	volatile uint32t GPIO_FALL_IP;
	volatile uint32t GPIO_HIGH_IE;
	volatile uint32t GPIO_HIGH_IP;
	volatile uint32t GPIO_LOW_IE;
	volatile uint32t GPIO_LOW_IP;
	volatile uint32t GPIO_IOF_EN;
	volatile uint32t GPIO_IOF_SEL;
	volatile uint32t GPIO_OUT_XOR;

} GPIO_TypeDef;

#define GPIO_BASE_ADDRESS        (GPIO_TypeDef*)0x10012000


/**
  * @brief   GPIO Init structure definition
  */
typedef struct
{
  uint32t Pin;       /* Specifies the GPIO pins to be configured. */
  uint32t Mode;      /* Specifies the operating mode for the selected pins */
} GPIO_InitTypeDef;


/**
  * @brief  GPIO Bit SET and Bit RESET enumeration
  */
typedef enum
{
  GPIO_PIN_RESET = 0U,
  GPIO_PIN_SET
} GPIO_PinState;




void HAL_GPIO_Init(GPIO_TypeDef *GPIO, GPIO_InitTypeDef *GPIO_Init);
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIO, uint32t GPIO_Pin, GPIO_PinState PinState);
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIO, uint32t GPIO_Pin);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIO, uint32t GPIO_Pin);


#endif /* HAL_GPIO_H_ */
