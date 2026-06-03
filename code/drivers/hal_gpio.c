/*
 * gpio-ors.c
 *
 *  Created on: 7 Oct 2023
 *      Author: patriciobulic
 */

#include "hal_gpio.h"


void HAL_GPIO_Init(GPIO_TypeDef *GPIO, GPIO_InitTypeDef *GPIO_Init){

	if (GPIO_Init->Mode == GPIO_MODE_INPUT) {
		GPIO->GPIO_INPUT_EN |= GPIO_Init->Pin;
		GPIO->GPIO_OUTPUT_EN &= ~(GPIO_Init->Pin);
	}

	else if (GPIO_Init->Mode == GPIO_MODE_OUTPUT) {
		GPIO->GPIO_OUTPUT_EN |= GPIO_Init->Pin;
		GPIO->GPIO_INPUT_EN &= ~(GPIO_Init->Pin);
	}
}

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIO, uint32t GPIO_Pin, GPIO_PinState PinState){

	if (PinState == GPIO_PIN_SET) {
		GPIO->GPIO_OUTPUT_VAL |= GPIO_Pin;
	}
	else {
		GPIO->GPIO_OUTPUT_VAL &= ~(GPIO_Pin);
	}
}


void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIO, uint32t GPIO_Pin){
	GPIO->GPIO_OUTPUT_VAL ^= GPIO_Pin;
}


GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIO, uint32t GPIO_Pin) {
	GPIO_PinState bitstatus;

	if (GPIO->GPIO_INPUT_VAL & GPIO_Pin){
		bitstatus = GPIO_PIN_SET;
	}
	else
	{
		bitstatus = GPIO_PIN_RESET;
	}
	return bitstatus;


}

