/*
 * stm32f4xx.h
 *
 *  Created on: 24 Apr 2026
 *      Author: kangajan
 */

#ifndef INC_STM32F4XX_H_
#define INC_STM32F4XX_H_
#include <stddef.h>
#include <stdint.h>
#define __vo	volatile
#define __weak  __attribute__((weak))

/*****************************START:Processor Specific Details**************************************/





/*
 * ARM Cortex Mx Processor NVIC ISERx register Addresses
 * */
#define NVIC_ISER0				( (__vo uint32_t*) 0xE000E100 )
#define NVIC_ISER1				( (__vo uint32_t*) 0xE000E104 )
#define NVIC_ISER2				( (__vo uint32_t*) 0xE000E108 )
#define NVIC_ISER3				( (__vo uint32_t*) 0xE000E10c )
/*
 * ARM Cortex Mx Processor NVIC ICERx register Addresses
 * */
#define NVIC_ICER0				( (__vo uint32_t*) 0xE000E180 )
#define NVIC_ICER1				( (__vo uint32_t*) 0xE000E184 )
#define NVIC_ICER2				( (__vo uint32_t*) 0xE000E188 )
#define NVIC_ICER3				( (__vo uint32_t*) 0xE000E18c )
/*
 * ARM Cortex Mx Processor NVIC IPRx register Addresses
 * */
#define NVIC_PR_BASE_ADDR		( (__vo uint32_t*) 0xE000E400 )

#define NO_PR_BITS_IMPLEMENTED 		4



/*
 * Base addresses of Flash and SRAM memories
 */

#define FLASB_BASEADDR			0x08000000U 							//Flash_memory base Address
#define SRAM1_BASEADDR			0x20000000U								//SRAM1 base Address
#define SRAM2_BASEADDR			0x2001C000U								//SRAM2 base Address
#define ROM_BASEADDR      		0x1FFF0000U								//ROM base Address
#define SRAM 					SRAM1_BASEADDR							//SRAM base Address

/*
 * AHBX and APBX Bus Peripheral base address
 */

#define PERIPH_BASE				0x40000000U								//Peripheral base Address
#define APB1PERIPH_BASEADDR		PERIPH_BASE								//APB1 base Address
#define APB2PERIPH_BASEADDR		0x40010000U								//APB2 base Address
#define AHB1PERIPH_BASEADDR		0x40020000U								//AHB1 base Address
#define AHB2PERIPH_BASEADDR		0x50000000U								//AHB2 base Address

/*
 * AHB1 peripherals base Addresses
 */

#define GPIOA_BASEADDR			(AHB1PERIPH_BASEADDR + 0x0000) 			//GPIOA base Address
#define GPIOB_BASEADDR			(AHB1PERIPH_BASEADDR + 0x0400) 			//GPIOB base Address
#define GPIOC_BASEADDR			(AHB1PERIPH_BASEADDR + 0x0800)			//GPIOC base Address
#define GPIOD_BASEADDR			(AHB1PERIPH_BASEADDR + 0x0C00)			//GPIOD base Address
#define GPIOE_BASEADDR			(AHB1PERIPH_BASEADDR + 0x1000) 			//GPIOE base Address
#define GPIOF_BASEADDR			(AHB1PERIPH_BASEADDR + 0x1400) 			//GPIOF base Address
#define GPIOG_BASEADDR			(AHB1PERIPH_BASEADDR + 0x1800)			//GPIOG base Address
#define GPIOH_BASEADDR			(AHB1PERIPH_BASEADDR + 0x1C00)			//GPIOH base Address
#define GPIOI_BASEADDR			(AHB1PERIPH_BASEADDR + 0x2000)			//GPIOI base Address
#define RCC_BASEADDR			(AHB1PERIPH_BASEADDR + 0x3800)			//RCC base Address

/*
 * APB1 peripherals base Addresses
 */

#define I2C2_BASEADDR			(APB1PERIPH_BASEADDR + 0x5800)			//I2C2 Peripheral base Address
#define I2C3_BASEADDR			(APB1PERIPH_BASEADDR + 0x5C00)			//I2C3 Peripheral base Address
#define I2C1_BASEADDR			(APB1PERIPH_BASEADDR + 0x5400)			//I2C1 Peripheral base Address
#define SPI2_BASEADDR			(APB1PERIPH_BASEADDR + 0X3800)			//SPI2 Peripheral base Address
#define SPI3_BASEADDR			(APB1PERIPH_BASEADDR + 0x3C00)			//SPI3 Peripheral base Address
#define USART2_BASEADDR			(APB1PERIPH_BASEADDR + 0x4400)			//USART2 Peripheral base Address
#define USART3_BASEADDR			(APB1PERIPH_BASEADDR + 0x4800)			//USART3 Peripheral base Address
#define UART4_BASEADDR			(APB1PERIPH_BASEADDR + 0x4C00)			//UART4 Peripheral base Address
#define UART5_BASEADDR			(APB1PERIPH_BASEADDR + 0X5000)			//UART5 Peripheral base Address
#define TIM2_BASEADDR 			(APB1PERIPH_BASEADDR + 0x0000)			//TIMER2 Peripheral base Address
#define TIM3_BASEADDR 			(APB1PERIPH_BASEADDR + 0x0400)			//TIMER2 Peripheral base Address
#define TIM4_BASEADDR 			(APB1PERIPH_BASEADDR + 0x0800)			//TIMER2 Peripheral base Address
#define TIM5_BASEADDR 			(APB1PERIPH_BASEADDR + 0x0C00)			//TIMER2 Peripheral base Address


/*
 * APB2 peripherals base Addresses
 */

#define SPI1_BASEADDR			(APB2PERIPH_BASEADDR + 0x3000)			//SPI1 Peripheral base Address
#define USART1_BASEADDR			(APB2PERIPH_BASEADDR + 0x1000)			//USART1 Peripheral base Address
#define UART6_BASEADDR			(APB2PERIPH_BASEADDR + 0x1400)			//USART6 Peripheral base Address
#define EXTI_BASEADDR			(APB2PERIPH_BASEADDR + 0x3C00)			//EXTI Peripheral base Address
#define SYSCFG_BASEADDR			(APB2PERIPH_BASEADDR + 0x3800)			//SYSCFG Peripheral base Address

/*
 * SPI1 Peripheral's registers Address
 */

#define SPI1_CR1_REGADDR 		(SPI1_BASEADDR + 0x000);				//SPI1 control register Address
#define SPI1_CR2_REGADDR		(SPI1_BASEADDR + 0x004);				//SPI1 control register Address
#define SPI1_SR_REGADDR 		(SPI1_BASEADDR + 0X008);				//SPI1 status register Address
#define SPI1_DR_REGADDR			(SPI1_BASEADDR + 0X0C);					//SPI1 data register Address
#define SPI1_CRCPR_REGADDR 		(SPI1_BASEADDR + 0X10);					//SPI1 CRCPR register Address
#define SPI1_RXCRCR_REGADDR 	(SPI1_BASEADDR + 0X14);					//SPI1 RXCRCR register Address
#define SPI1_TXCRCR_REGADDR 	(SPI1_BASEADDR + 0X18);					//SPI1 TXCRCR register Address
#define SPI1_I2SCFGR_REGADDR 	(SPI1_BASEADDR + 0X1C);					//SPI1 I2SCFGR register Address
#define SPI1_I2SPR_REGADDR 		(SPI1_BASEADDR + 0X20);					//SPI1 I2SPR register Address


/******************Peripheral registration definition structure ********************************/

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    // ...more registers
} TIM_RegDef_t;

typedef struct {
    __vo uint32_t CR1;
    __vo uint32_t CR2;
    __vo uint32_t SR;
    __vo uint32_t DR;
    __vo uint32_t CRCPR;
    __vo uint32_t RXCRCR;
    __vo uint32_t TXCRCR;
    __vo uint32_t I2SCFGR;
    __vo uint32_t I2SPR;
} SPI_RegDef_t;


typedef struct {
	__vo uint32_t MODER;				//GPIO port mode register
	__vo uint32_t OTYPER;				//GPIO port output type register
	__vo uint32_t OSPEEDR;				//GPIO port output speed register
	__vo uint32_t PUPDR;				//GPIO port pull-up/pull-down register
	__vo uint32_t IDR;					//GPIO port input data register
	__vo uint32_t ODR;					//GPIO port output data register
	__vo uint32_t BSRR;					//GPIO port bit set/reset register
	__vo uint32_t LCKR;					//GPIO port configuration lock register
	__vo uint32_t AFR[2];				//GPIO alternate function low register (AFR[0] = Low_Reg & AFR[1] = High_Reg)
}GPIO_RegDef_t;

/*
 * Peripheral register definition structure for RCC
 */
typedef struct {
	__vo uint32_t CR;					//RCC clock control register
	__vo uint32_t PLLCFGR;				//RCC PLL configuration register
	__vo uint32_t CFGR;					//RCC clock configuration register
	__vo uint32_t CIR;					//RCC clock interrupt register
	__vo uint32_t AHB1RSTR;				//RCC AHB1 peripheral reset register
	__vo uint32_t AHB2RSTR;				//RCC AHB2 peripheral reset register
	__vo uint32_t AHB3RSTR;				//RCC AHB3 peripheral reset register
	uint32_t Reserved0;					// 0x1C (OFFSET)
	__vo uint32_t APB1RSTR;				//RCC APB1 peripheral reset register
	__vo uint32_t APB2RSTR;				//RCC APB2 peripheral reset register
	uint32_t Reserved1;					// 0x28
	uint32_t Reserved2;					// 0x2C
	__vo uint32_t AHB1ENR;				//RCC AHB1 peripheral clock enable register
	__vo uint32_t AHB2ENR;				//RCC AHB2 peripheral clock enable register
	__vo uint32_t AHB3ENR;				//RCC AHB3 peripheral clock enable register
	uint32_t Reserved3;					// 0x3C
	__vo uint32_t APB1ENR;				//RCC APB1 peripheral clock enable register
	__vo uint32_t APB2ENR;				//RCC APB2 peripheral clock enable register
	uint32_t Reserved4;					// 0x48
	uint32_t Reserved5;					// 0x4C
	__vo uint32_t AHB1LPENR;			//RCC AHB1 peripheral clock enable in low power mode register
	__vo uint32_t AHB2LPENR;			//RCC AHB2 peripheral clock enable in low power mode register
	__vo uint32_t AHB3LPENR;			//RCC AHB3 peripheral clock enable in low power mode register
	uint32_t Reserved6;					// 0x5C
	__vo uint32_t APB1LPENR;			//RCC APB1 peripheral clock enable in low power mode register
	__vo uint32_t APB2LPENR;			//RCC APB2 peripheral clock enabled in low power mode
	uint32_t Reserved7;					// 0x68
	uint32_t Reserved8;					// 0x6C
	__vo uint32_t BDCR;					//RCC Backup domain control register
	__vo uint32_t CSR;					//RCC clock control & status register
	uint32_t Reserved9;					// 0x78
	uint32_t Reserved10;				// 0x7C
	__vo uint32_t SSCGR;				//RCC spread spectrum clock generation register
	__vo uint32_t PLLI2SCFGR;			//RCC PLLI2S configuration register
}RCC_RegDef_t;

/*
 * Peripheral register definition structure for EXTI
 */
typedef struct {
	__vo uint32_t IMR;					//Enables/disables interrupt lines. Set a bit to unmask (allow) that line's interrupt.
	__vo uint32_t EMR;					//Same idea as IMR but for events rather than interrupts (used with low-power/WFE modes).
	__vo uint32_t RTSR;					//Configures a line to trigger on a rising edge (0→1 transition).
	__vo uint32_t FTSR;					//Configures a line to trigger on a falling edge (1→0 transition). Both RTSR and FTSR can be set simultaneously for any-edge triggering.
	__vo uint32_t SWIER;				//Lets you manually trigger an interrupt or event in software by setting a bit, without needing an actual pin signal.
	__vo uint32_t PR;					//A bit is set by hardware when a trigger condition is detected. You clear it by writing 1 to it (writing 0 has no effect).

}EXTI_RegDef_t;

/*
 * Peripheral register definition structure for SYSCFG
 */
typedef struct {
	__vo uint32_t MEMRMP;				//Controls which memory appears at address 0x00000000 at boot (Flash, SRAM, or system memory).
	__vo uint32_t PMC;					//Configures certain peripheral behaviours
	__vo uint32_t EXTICR[4];			//EXTI lines
	uint32_t RESERVED1[2];
	__vo uint32_t CMPCR;				//Controls the I/O compensation cell, which reduces noise on high-speed GPIO pins (≥50MHz).
	uint32_t RESERVED2[2];

}SYSCFG_RegDef_t;

/*
 * Peripheral definition (Peripheral base address typecasted to xxx_RegDef_t)
 */
#define GPIOA		( (GPIO_RegDef_t*) GPIOA_BASEADDR)
#define GPIOB		( (GPIO_RegDef_t*) GPIOB_BASEADDR)
#define GPIOC		( (GPIO_RegDef_t*) GPIOC_BASEADDR)
#define GPIOD		( (GPIO_RegDef_t*) GPIOD_BASEADDR)
#define GPIOE		( (GPIO_RegDef_t*) GPIOE_BASEADDR)
#define GPIOF		( (GPIO_RegDef_t*) GPIOF_BASEADDR)
#define GPIOG		( (GPIO_RegDef_t*) GPIOG_BASEADDR)
#define GPIOH		( (GPIO_RegDef_t*) GPIOH_BASEADDR)
#define GPIOI		( (GPIO_RegDef_t*) GPIOI_BASEADDR)

#define RCC			( (RCC_RegDef_t*)  RCC_BASEADDR)

#define EXTI		( (EXTI_RegDef_t*)  EXTI_BASEADDR)

#define SYSCFG		( (SYSCFG_RegDef_t*) SYSCFG_BASEADDR)

#define SPI1		((SPI_RegDef_t*) SPI1_BASEADDR)
#define SPI2		((SPI_RegDef_t*) SPI2_BASEADDR)
#define SPI3		((SPI_RegDef_t*) SPI3_BASEADDR)

#define TIM2		((TIM_RegDef_t*) TIM2_BASEADDR)
#define TIM3		((TIM_RegDef_t*) TIM3_BASEADDR)
#define TIM4		((TIM_RegDef_t*) TIM4_BASEADDR)
#define TIM5		((TIM_RegDef_t*) TIM5_BASEADDR)



/*
 * Clock Enable Macros for GPIOX Peripheral
 */
#define GPIOA_PCLK_EN()			(RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()			(RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()			(RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()			(RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()			(RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()			(RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()			(RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()			(RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN()			(RCC->AHB1ENR |= (1 << 8))
/*
 * Clock Enable Macros for I2CX Peripheral
 */
#define I2C1_PCLK_EN()			(RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()			(RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()			(RCC->APB1ENR |= (1 << 23))
/*
 * Clock Enable Macros for SPIX Peripheral
 */
#define SPI1_PCLK_EN()			(RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()			(RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()			(RCC->APB1ENR |= (1 << 15))
/*
 * Clock Enable Macros for USARTX Peripheral
 */
#define USART1_PCLK_EN()		(RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()		(RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()		(RCC->APB1ENR |= (1 << 18))
#define USART6_PCLK_EN()		(RCC->APB2ENR |= (1 << 5))
/*
 * Clock Enable Macros for SYSCFG Peripheral
 */
#define SYSCFG_PCLK_EN()		(RCC->APB2ENR |= (1 << 14))
/*
 * Clock Enable Macros for TIMERX Peripheral
 */
#define TIM2_PCLK_EN()			(RCC->APB1ENR |= (1 << 0))
#define TIM3_PCLK_EN()			(RCC->APB1ENR |= (1 << 1))
#define TIM4_PCLK_EN()			(RCC->APB1ENR |= (1 << 2))
#define TIM5_PCLK_EN()			(RCC->APB1ENR |= (1 << 3))



/*
 * Clock Disable Macros for GPIOX Peripheral
 */
#define GPIOA_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 8))
/*
 * Clock Enable Macros for I2CX Peripheral
 */
#define I2C1_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 23))
/*
 * Clock Enable Macros for SPIX Peripheral
 */
#define SPI1_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 15))
/*
 * Clock Enable Macros for USARTX Peripheral
 */
#define USART1_PCLK_DI()		(RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()		(RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()		(RCC->APB1ENR &= ~(1 << 18))
#define USART6_PCLK_DI()		(RCC->APB2ENR &= ~(1 << 5))
/*
 * Clock Enable Macros for SYSCFG Peripheral
 */
#define SYSCFG_PCLK_DI()		(RCC->APB2ENR &= ~(1 << 14))

/*
 * Clock Disable Macros for TIMERX Peripheral
 */
#define TIM2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 0))
#define TIM3_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 1))
#define TIM4_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 2))
#define TIM5_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 3))


/*
 * Macros to reset GPIOx Peripherals
 */
#define GPIOA_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); }while(0)
#define GPIOB_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); }while(0)
#define GPIOC_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); }while(0)
#define GPIOD_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); }while(0)
#define GPIOE_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); }while(0)
#define GPIOF_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); }while(0)
#define GPIOG_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); }while(0)
#define GPIOH_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); }while(0)
#define GPIOI_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 8)); (RCC->AHB1RSTR &= ~(1 << 8)); }while(0)


#define GPIO_BASEADDR_TO_CODE(x)   ((x == GPIOA) ? 0 : \
									(x == GPIOB) ? 1 : \
									(x == GPIOC) ? 2 : \
									(x == GPIOD) ? 3 : \
									(x == GPIOE) ? 4 : \
									(x == GPIOF) ? 5 : \
									(x == GPIOG) ? 6 : \
									(x == GPIOH) ? 7 : \
									(x == GPIOI) ? 8 : 0)

/*
 * IRQ(Interrupt Request) Number of STM32F407x MCU
 * */
//GPIO IRQ Number
#define IRQ_NO_EXTI0 				6
#define IRQ_NO_EXTI1 				7
#define IRQ_NO_EXTI2 				8
#define IRQ_NO_EXTI3 				9
#define IRQ_NO_EXTI4 				10
#define IRQ_NO_EXTI9_5 				23
#define IRQ_NO_EXTI15_10			40
//SPI IRQ Number
#define IRQ_NO_SPI1					35
#define IRQ_NO_SPI2					36
#define IRQ_NO_SPI3					51
#define IRQ_NO_SPI4					84
#define IRQ_NO_SPI5					85
#define IRQ_NO_SPI6					86

/*
 * IRQ(Interrupt Request) PRIORITHY Levels of STM32F407x MCU
 * */
#define NVIC_IRQ_PRI0				0
#define NVIC_IRQ_PRI1				1
#define NVIC_IRQ_PRI2				2
#define NVIC_IRQ_PRI3				3
#define NVIC_IRQ_PRI4				4
#define NVIC_IRQ_PRI5				5
#define NVIC_IRQ_PRI6				6
#define NVIC_IRQ_PRI7				7
#define NVIC_IRQ_PRI8				8
#define NVIC_IRQ_PRI9				9
#define NVIC_IRQ_PRI10				10
#define NVIC_IRQ_PRI11				11
#define NVIC_IRQ_PRI12				12
#define NVIC_IRQ_PRI13				13
#define NVIC_IRQ_PRI14				14
#define NVIC_IRQ_PRI15				15


//some generic macros
#define ENABLE 					1
#define DISABLE 				0
#define SET						ENABLE
#define RESET 					DISABLE
#define GPIO_PIN_SET			SET
#define GPIO_PIN_RESET			RESET
#define FLAG_RESET				RESET
#define FLAG_SET				SET
/*
 * ************************************************************************************************************************************************************
 * 														Bit position definitions of SPI peripheral
 * ************************************************************************************************************************************************************
 */

/*
 * Bit Position definitions SPI_CR1
 */
#define SPI_CR1_CPHA			0
#define SPI_CR1_CPOL			1
#define SPI_CR1_MSTR			2
#define SPI_CR1_BR				3
#define SPI_CR1_SPE				6
#define SPI_CR1_LSBFIRST		7
#define SPI_CR1_SSI				8
#define SPI_CR1_SSM				9
#define SPI_CR1_RXONLY			10
#define SPI_CR1_DFF				11
#define SPI_CR1_CRCNEXT			12
#define SPI_CR1_CRCEN			13
#define SPI_CR1_BIDIOE			14
#define SPI_CR1_BIDIMODE		15

/*
 * Bit Position definitions SPI_CR2
 */
#define SPI_CR2_RXDMAEN			0
#define SPI_CR2_TXDMAEN			1
#define SPI_CR2_SSOE 			2
#define SPI_CR2_FRF				4
#define SPI_CR2_ERRIE 			5
#define SPI_CR2_RXNEIE 			6
#define SPI_CR2_TXEIE 			7

/*
 * Bit Position definition SPI_SR
 */
#define SPI_SR_RXNE				0
#define SPI_SR_TXE				1
#define SPI_SR_CHSIDE			2
#define SPI_SR_UDR				3
#define SPI_SR_CRCERR			4
#define SPI_SR_MODF				5
#define SPI_SR_OVR				6
#define SPI_SR_BSY				7
#define SPI_SR_FRE				8

/*
 * Macros to reset SPIs Peripherals
 */
/*
 * @SPI1_reset
 */
#define SPI1_REG_RESET() do{ (RCC->APB2RSTR |= (1 << 12)); \
                             (RCC->APB2RSTR &= ~(1 << 12)); }while(0)
/*
 * @SPI2_reset
 */
#define SPI2_REG_RESET() do{ (RCC->APB1RSTR |= (1 << 14)); \
                             (RCC->APB1RSTR &= ~(1 << 14)); }while(0)
/*
 * @SPI3_reset
 */
#define SPI3_REG_RESET() do{ (RCC->APB1RSTR |= (1 << 15)); \
                             (RCC->APB1RSTR &= ~(1 << 15)); }while(0)

#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"
#include "stm32f407xx_timer_driver.h"

#endif /* INC_STM32F4XX_H_ */
