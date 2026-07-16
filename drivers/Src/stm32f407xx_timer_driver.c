/*
 *  stm32f407xx_timer_driver.c
 *
 *  Created on: 8 Jul 2026
 *      Author: kangajan
 */

#include "stm32f4xx.h"

#include "stm32f407xx_timer_driver.h"



/**************************************************************
 * @fn				- TIMER_PeriClockControl
 *
 * @brief			- This function enables or disables peripheral clock for Timer
 *
 * @Param[in]		- base address of the Timer
 * @Param[in]		- ENABLE OR DISABLE MACROS
 *
 * @return			- none
 *
 * @Note 			- none
 */
void TIMER_PeriClockControl(TIM_RegDef_t *pTIMx, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
		if (pTIMx == TIM2) {
			TIM2_PCLK_EN();
		} else if (pTIMx == TIM3) {
			TIM3_PCLK_EN();
		} else if (pTIMx == TIM4) {
			TIM4_PCLK_EN();
		} else if (pTIMx == TIM5) {
			TIM5_PCLK_EN();
		}
	} else {
		if (pTIMx == TIM2) {
			TIM2_PCLK_DI();
		} else if (pTIMx == TIM3) {
			TIM3_PCLK_DI();
		} else if (pTIMx == TIM4) {
			TIM4_PCLK_DI();
		} else if (pTIMx == TIM5) {
			TIM5_PCLK_DI();
		}
	}
}
/**************************************************************
 * @fn				- TIMER_Init
 *
 * @brief			- This function Initiate the Timer
 *
 * @Param[in]		- TIM_Handle_t ( this contain timer registers and TIMER_Prescaler value)
 *
 * @return			- none
 *
 * @Note 			- none
 */
/*
 * 	So the correct formula is:
	PSC = (TIM2_input_clock_Hz / 1,000,000) - 1
	If TIM2's input clock is 16MHz:
	PSC = (16,000,000 / 1,000,000) - 1 = 16 - 1 = 15
 */
void TIMER_Init(TIM_Handle_t *pTIMHandle) {
	pTIMHandle->pTIMx->PSC = pTIMHandle->TIMER_Prescaler;
    pTIMHandle->pTIMx->EGR |= (1 << 0);   // UG bit - force update event, load PSC immediately

}
/**************************************************************
 * @fn				- TIMER_Start
 *
 * @brief			- This function start the Timer
 *
 * @Param[in]		- TIM_RegDef_t
 *
 * @return			- none
 *
 * @Note 			- none
 */
void TIMER_Start(TIM_RegDef_t *pTIMx) {
	pTIMx->CNT = 0;
	pTIMx->CR1 |= (1 << 0); // enable bit, CEN, typically bit 0 of CR1

}

uint32_t TIMER_GetCounter(TIM_RegDef_t *pTIMx) {
	return pTIMx->CNT;
}
