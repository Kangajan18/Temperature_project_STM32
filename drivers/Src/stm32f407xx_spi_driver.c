/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: 4 Jun 2026
 *      Author: kangajan
 */


#include "stm32f407xx_spi_driver.h"

// Private helper function prototypes
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);


/**************************************************************
 * @fn				- SPI_PeriClockControl
 *
 * @brief			- This function enables or disables peripheral clock for SPI
 *
 * @Param[in]		- base address of the SPI
 * @Param[in]		- ENABLE OR DISABLE MACROS
 *
 * @return			- none
 *
 * @Note 			- none
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
		if (pSPIx == SPI1) {
			SPI1_PCLK_EN();
		} else if (pSPIx == SPI2) {
			SPI2_PCLK_EN();
		} else if (pSPIx == SPI3) {
			SPI3_PCLK_EN();
		}
	} else {
		if (pSPIx == SPI1) {
			SPI1_PCLK_DI();
		} else if (pSPIx == SPI2) {
			SPI2_PCLK_DI();
		} else if (pSPIx == SPI3) {
			SPI3_PCLK_DI();
		}
	}
}
/**************************************************************
 * @fn				- SPI_Init
 *
 * @brief			- This function do the Initialisation for SPI
 *
 * @Param[in]		- SPI_Handle (it's contain SPI Base_Address and configuration registers)
 *
 * @return			- none
 *
 * @Note 			- none
 */
void SPI_Init(SPI_Handle_t *pSPIHandle) {

	//peripheral clock enable
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	//first lets configure the SPI_CR1 register
	uint32_t tempreg = 0;
	//1. Configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;
	//2.Configure the bus Configuration
	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD) {
		//BIDI mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);//clear bit 15
	} else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD) {
		//BIDI mode should be set
		tempreg |= (1 << SPI_CR1_BIDIMODE);
	}else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY) {
		//BIDI mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
		//RXONLY bit must be set
		tempreg |= (1 << SPI_CR1_RXONLY);
	}
	//3. Configure clock speed
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;
	//4. Configure DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;
	//5.Configure CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;
	//6.Configure CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;
	//7.Configure SSM
	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	//Finally write to CR1
	pSPIHandle->pSPIx->CR1 = tempreg;
}
/**************************************************************
 * @fn				- SPI_DeInit
 *
 * @brief			- This function do the DE-Initialisation for SPI
 *
 * @Param[in]		- SPI_RegDef_t
 *
 * @return			- none
 *
 * @Note 			- none
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx) {
	if (pSPIx == SPI1) {
		SPI1_REG_RESET();
	} else if (pSPIx == SPI2) {
		SPI2_REG_RESET();
	} else if (pSPIx == SPI3) {
		SPI3_REG_RESET();
	}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName) {
	if (pSPIx->SR & FlagName) {
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/**************************************************************
 * @fn				- SPI_SendData
 *
 * @brief			- This function send data (blocking function)
 *
 * @Param[in]		- pSPIx			//what SPI (SPI1/SPI2/SPI3)
 * @Param[in]		- pTxBuffer		//box of data for transmit
 * @Param[in]		- len			//length of data (how many data you going to send)
 *
 * @return			- none
 *
 * @Note 			- it's a blocking function so if all the data move only CPU can move to next part.
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len) {
	while (len > 0) {
	    //1. wait until TXE is Set
	    while (SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG) == FLAG_RESET);
	    //2. check the DFF bit in CR1
	    if (pSPIx->CR1 & (1 << SPI_CR1_DFF)) {
	    	//(16bit) load 2 byte of data into DR register
	    	pSPIx->DR = *((uint16_t*)pTxBuffer);
	    	len--;
	    	len--;
	    	(uint16_t*)pTxBuffer++;
	    } else {
	    	//(8bit) load 1 byte of data into DR register
	    	pSPIx->DR = *pTxBuffer;
	    	len--;
	    	pTxBuffer++;
	    }
	}
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len) {
	while (len > 0) {
		    //1. wait until RXNE is Set
		    while (SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG) == FLAG_RESET);
		    //2. check the DFF bit in CR1
		    if (pSPIx->CR1 & (1 << SPI_CR1_DFF)) {
		    	//(16bit) load 2 byte of data into RXbuffer address
		    	*((uint16_t*)pRxBuffer) = pSPIx->DR;
		    	len--;
		    	len--;
		    	(uint16_t*)pRxBuffer++;
		    } else {
		    	//(8bit)
		    	*((uint16_t*)pRxBuffer) = pSPIx->DR;
		    	len--;
		    	pRxBuffer++;
		    }
		}
}
/**************************************************************
 * @fn				- SPI_PeripheralControl
 *
 * @brief			- This Function used to Enable the SPI peripheral
 *
 * @Param[in]		- pSPIx			//what SPI (SPI1/SPI2/SPI3)
 * @Param[in]		- EnOrDi		//Enable Or Disable
 *
 * @return			- none
 *
 * @Note 			-
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {
	if (EnOrDi == ENABLE) {
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	} else {
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {
	if (EnOrDi == ENABLE) {
			pSPIx->CR1 |= (1 << SPI_CR1_SSI);
		} else {
			pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
		}
}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {
	if (EnOrDi == ENABLE) {
			pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
		} else {
			pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
		}
}

/*
 * IRQ Configuration and ISR handling
 * */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) {
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
 * @fn				- SPI_IRQPriorityConfig
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
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority) {
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx ) |= (IRQPriority << shift_amount);
}

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len) {

	uint8_t state = pSPIHandle->TxState;
	if (state != SPI_BUSY_IN_TX) {
		//1. Save the Tx buffer address and Length information in some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = len;
		//2. Mark the SPI state as busy in transmission so that
		//		no other code can take over same SPI peripheral until transmission is over
		pSPIHandle->TxState = SPI_BUSY_IN_TX;
		//3 Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
	}
	return state;
}
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len) {
	uint8_t state = pSPIHandle->RxState;
		if (state != SPI_BUSY_IN_RX) {
			pSPIHandle->pRxBuffer = pRxBuffer;
			pSPIHandle->RxLen = len;
			pSPIHandle->RxState = SPI_BUSY_IN_RX;
			pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);
		}
		return state;
}



void SPI_IRQHandling(SPI_Handle_t *pSPIHandle) {
	uint8_t temp1, temp2;
	//first lets check for TXE
	temp1 = pSPIHandle->pSPIx->SR & ( 1 << SPI_SR_TXE);
	temp2 = pSPIHandle->pSPIx->CR2 & ( 1 << SPI_CR2_TXEIE);

	if (temp1 && temp2) {
		// handle TXE
		spi_txe_interrupt_handle(pSPIHandle);
	}

	//second lets check for RXNE
	temp1 = pSPIHandle->pSPIx->SR & ( 1 << SPI_SR_RXNE);
	temp2 = pSPIHandle->pSPIx->CR2 & ( 1 << SPI_CR2_RXNEIE);
	if (temp1 && temp2) {
		// handle TXE
		spi_rxne_interrupt_handle(pSPIHandle);
	}

	//third lets check for OVR flag
	temp1 = pSPIHandle->pSPIx->SR & ( 1 << SPI_SR_OVR);
	temp2 = pSPIHandle->pSPIx->CR2 & ( 1 << SPI_CR2_ERRIE);

	if ( temp1 && temp2 ) {
		//handle OVR error
		spi_ovr_err_interrupt_handle(pSPIHandle);
	}
}

//some helper functions implementation


static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle) {
	//check the DFF bit in CR1
	if (pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)) {
		//(16bit) load 2 byte of data into DR register
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
		(uint16_t*)pSPIHandle->pTxBuffer++;
	} else {
		//(8bit) load 1 byte of data into DR register
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}
	if (! pSPIHandle->TxLen ) {
		//TXLen is zero, so close the SPI transmission and inform the application that
		//TX is over.
		//This prevents interrupts from setting up of TXE flag
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
	}

}
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle) {
	//check the DFF bit in CR1
	if (pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)) {
		//(16Bit) receive 2 bytes of data from DR register
		*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t)pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer--;
		pSPIHandle->pRxBuffer--;
	}else {
		//(8bit) load 1 byte of data into DR register
		*pSPIHandle->pRxBuffer = pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer--;
	}
	if (! pSPIHandle->RxLen) {

		//reception is completed
		//lets off the RXNEIE interrupt
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
	}
}
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle) {
    uint8_t temp;
    // 1. Clear OVR flag — only if not busy transmitting
    if(pSPIHandle->TxState != SPI_BUSY_IN_TX) {
        temp = pSPIHandle->pSPIx->DR;  // read DR
        temp = pSPIHandle->pSPIx->SR;  // read SR
    }
    (void)temp;
    // 2. Notify application
    SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx) {
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle) {
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}
void SPI_CloseReception(SPI_Handle_t *pSPIHandle) {
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}

__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv) {

}



