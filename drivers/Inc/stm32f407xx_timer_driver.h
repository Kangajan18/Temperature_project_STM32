/*
 *  stm32f407xx_timer_driver.h
 *
 *  Created on: 8 Jul 2026
 *      Author: kangajan
 */

#ifndef STM32F407XX_TIMER_DRIVER_H_
#define STM32F407XX_TIMER_DRIVER_H_

#include "stm32f4xx.h"




typedef struct {
	TIM_RegDef_t *pTIMx;
    uint32_t TIMER_Prescaler;
} TIM_Handle_t;


void TIMER_PeriClockControl(TIM_RegDef_t *pTIMx, uint8_t EnorDi);
void TIMER_Init(TIM_Handle_t *pTIMHandle);
void TIMER_Start(TIM_RegDef_t *pTIMx);
uint32_t TIMER_GetCounter(TIM_RegDef_t *pTIMx);

#endif /* STM32F407XX_TIMER_DRIVER_H_ */
