/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: 29 Apr 2026
 *      Author: kangajan
 */


#include "stm32f407xx_gpio_driver.h"


/*
 * Peripheral Clock setup
 * */
/**************************************************************
 * @fn				- GPIO_PeriClockControl
 *
 * @brief			- This function enables or disables peripheral clock for given GPIO port
 *
 * @Param[in]		- base address of the GPIO peripheral
 * @Param[in]		- ENABLE OR DISABLE MACROS
 *
 * @return			- none
 *
 * @Note 			- none
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_EN();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_EN();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_EN();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_EN();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_EN();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_EN();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_EN();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_EN();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLK_EN();
		}
	} else {
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_DI();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_DI();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_DI();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_DI();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_DI();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_DI();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_DI();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_DI();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLK_DI();
		}
	}
}
/*
 * Init and De-Init
 * */
/**************************************************************
 * @fn				- GPIO_Init
 *
 * @brief			- This function do the GPIO port initialisation
 *
 * @Param[in]		- GPIO_Handle_t used to define GPIO and Pin config
 *
 * @return			- none
 *
 * @Note 			- none
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle) {

	uint32_t temp = 0; // temp. register

	//enable the peripheral clock
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	//1. configure the mode of GPIO Pin
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG) {
		//the non interrupt mode
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER |= temp;
	} else {
		//this part will code latter . (interrupt mode)
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT ) {
			//1. Configure the FTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT ) {
			//1. Configure the RTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding FTSR bit
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT ) {
			//1. Configure the both FTSR and RTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}
		//2. Configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1]  = portcode << (temp2 * 4);
		//3. Enable the EXTI interrupt delivery using IMR
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
	temp = 0;
	//2. configure the speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;
	temp = 0;
	//3. configure the pupd settings
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR |= temp;
	temp = 0;
	//4. configure the optype
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	temp = 0;
	//5. configure the alt functionality
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN) {
		//configure the alt function registers.
		uint8_t temp1, temp2;
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] |=	(pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2));
	}

}
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx) {
	if (pGPIOx == GPIOA) {
		GPIOA_REG_RESET();
	} else if (pGPIOx == GPIOB) {
		GPIOB_REG_RESET();
	} else if (pGPIOx == GPIOC) {
		GPIOC_REG_RESET();
	} else if (pGPIOx == GPIOD) {
		GPIOD_REG_RESET();
	} else if (pGPIOx == GPIOE) {
		GPIOE_REG_RESET();
	} else if (pGPIOx == GPIOF) {
		GPIOF_REG_RESET();
	} else if (pGPIOx == GPIOG) {
		GPIOG_REG_RESET();
	} else if (pGPIOx == GPIOH) {
		GPIOH_REG_RESET();
	} else if (pGPIOx == GPIOI) {
		GPIOI_REG_RESET();
	}
}
/*
 * Data read and write
 * */
/**************************************************************
 * @fn				- GPIO_ReadFromInputPin
 *
 * @brief			- This function Read from input pin
 *
 * @Param[in]		- GPIO_RegDef_t GPIO
 * @Param[in]		- GPIOPort PinNumber
 *
 * @return			- uint8_t ( 0 or 1 )
 *
 * @Note 			- none
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001); //move to LSB and masked to read the pin value
	return value;
}
/**************************************************************
 * @fn				- GPIO_ReadFromInputPort
 *
 * @brief			- This function Read from input port
 *
 * @Param[in]		- GPIO_RegDef_t GPIO
 *
 * @return			- uint8_t
 *
 * @Note 			- none
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx) {
	uint16_t value;
	value = (uint16_t)pGPIOx->IDR;
	return value;
}
/**************************************************************
 * @fn				- GPIO_WriteToOutputPin
 *
 * @brief			- This function use to write output Pin
 *
 * @Param[in]		- GPIO_RegDef_t GPIO
 * @Param[in]		- GPIOPort PinNumber
 * @Param[in]		- write value
 *
 * @return			- none
 *
 * @Note 			- none
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value) {
	if (Value == GPIO_PIN_SET) {
		//write 1 to the output data register at the bit field corresponding PinNumber
		pGPIOx->ODR |= (1 << PinNumber);
	} else {
		//write 0 to the output data register at the bit field corresponding PinNumber
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}
/**************************************************************
 * @fn				- GPIO_WriteToOutputPort
 *
 * @brief			- This function use to write output Port
 *
 * @Param[in]		- GPIO_RegDef_t GPIO
 * @Param[in]		- write value
 *
 * @return			- none
 *
 * @Note 			- none
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value) {
	pGPIOx->ODR = Value;
}
/**************************************************************
 * @fn				- GPIO_ToggleOutputPin
 *
 * @brief			- This function use to toggle the output pin
 *
 * @Param[in]		- GPIO_RegDef_t GPIO
 * @Param[in]		- GPIOPort PinNumber
 *
 * @return			- none
 *
 * @Note 			- none
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {
	pGPIOx->ODR ^= (1 << PinNumber);
}
/*
 * IRQ Configuration and ISR handling
 * */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) {
	//Enable or Disable the Interrupt Set Register
	if (EnorDi == ENABLE) {
		if (IRQNumber <= 31) {
			//program ISER0 register
			*NVIC_ISER0 |= (1 << IRQNumber);
		}else if (IRQNumber > 31 && IRQNumber < 64) { // 32 to 63
			//program ISER1 register
			*NVIC_ISER1 |= (1 << IRQNumber % 32);// % 32 for target the first register in ISER1
		}else if (IRQNumber >= 64 && IRQNumber < 96) { // 64 to 95
			//program ISER2 register
			*NVIC_ISER2 |= (1 << IRQNumber % 64);// % 64 for target the first register in ISER2
		}
	} else {
		if (IRQNumber <= 31) {
			//program ICER0 register
			*NVIC_ICER0 |= (1 << IRQNumber);
		}else if (IRQNumber > 31 && IRQNumber < 64) { // 32 to 63
			//program ICER1 register
		}else if (IRQNumber >= 64 && IRQNumber < 96) { // 64 to 95
			//program ICER2 register
			*NVIC_ICER2 |= (1 << IRQNumber % 64);
		}
	}
}

/**************************************************************
 * @fn				- GPIO_IRQPriorityConfig
 *
 * @brief			- This function use to set priority of the interrupt
 *
 * @Param[in]		- IRQNumber
 * @Param[in]		- IRQPriority
 *
 * @return			- none
 *
 * @Note 			- none
 */

/*Example
Step 1: iprx         = 6 / 4      = 1        → IPR1
Step 2: iprx_section = 6 % 4      = 2        → slot 2
Step 3: shift_amount = (8*2)+(8-4) = 20       → bit 20
Step 4: write 5 << 20 into IPR1

IPR1 register:
bit31      bit24  bit23      bit16  bit15       bit8  bit7        bit0
┌──────────────┬──────────────┬──────────────┬──────────────┐
│   IRQ7_PRI   │   IRQ6_PRI   │   IRQ5_PRI   │   IRQ4_PRI   │
│              │  0101 xxxx   │              │              │
│              │  ↑ bit20-23  │              │              │
└──────────────┴──────────────┴──────────────┴──────────────┘
                    5 written here ✅
 * */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority) {
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx ) |= (IRQPriority << shift_amount);
}
/**************************************************************
 * @fn				- GPIO_IRQHandling
 *
 * @brief			-
 *
 * @Param[in]		- PinNumber
 *
 * @return			- none
 *
 * @Note 			- none
 */
void GPIO_IRQHandling(uint8_t PinNumber) {
	//clear the exti pr register corresponding to the pin number
	if(EXTI->PR & (1 << PinNumber)) {
		//clear
		EXTI->PR |= (1 << PinNumber);
	}
}
