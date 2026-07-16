/*
 * dht11_driver.c
 *
 *  Created on: 9 Jul 2026
 *      Author: kangajan
 */


/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Kangajan Kuganathan
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "dht11_driver.h"

#define HIGH 1
#define LOW  0
#define DHT11_PORT  GPIOD
#define DHT11_PIN   GPIO_PIN_NO_0

void delay(void) {
	for (volatile uint32_t i = 0; i < 22000 ;i++);
}

static GPIO_Handle_t GpioTem;
static TIM_Handle_t Timer;

void DHT11_Init(void) {
	//GPIO SETUP
	GpioTem.pGPIOx = DHT11_PORT;
	GpioTem.GPIO_PinConfig.GPIO_PinNumber = DHT11_PIN;
	GpioTem.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioTem.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioTem.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GpioTem.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(DHT11_PORT, ENABLE);
	GPIO_Init(&GpioTem);

	//TIMER SETUP
	Timer.pTIMx = TIM2;
	Timer.TIMER_Prescaler = 15;

	TIMER_PeriClockControl(TIM2, ENABLE);
	TIMER_Init(&Timer);
	TIMER_Start(Timer.pTIMx);
}

uint8_t DHT11_Read(uint8_t *data) {
    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    //make the pin Low to WAKEUP the DHT11 sensor
    GpioTem.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GPIO_Init(&GpioTem);

    GPIO_WriteToOutputPin(DHT11_PORT, DHT11_PIN, LOW);
    delay();
    GPIO_WriteToOutputPin(DHT11_PORT, DHT11_PIN, HIGH);

    uint32_t start = TIMER_GetCounter(TIM2);
    while ((TIMER_GetCounter(TIM2) - start) < 30);  // wait ~30us

    //make the pin as input because DHT11 is ready to send the data.
    GpioTem.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GPIO_Init(&GpioTem);

    while (GPIO_ReadFromInputPin(DHT11_PORT, DHT11_PIN) == HIGH);
    while (GPIO_ReadFromInputPin(DHT11_PORT, DHT11_PIN) == LOW);
    while (GPIO_ReadFromInputPin(DHT11_PORT, DHT11_PIN) == HIGH);
    while (GPIO_ReadFromInputPin(DHT11_PORT, DHT11_PIN) == LOW);

    for (int i = 0; i < 40; i++) {
        while (GPIO_ReadFromInputPin(DHT11_PORT, DHT11_PIN) == LOW);

        uint32_t bitStart = TIMER_GetCounter(TIM2);

        while (GPIO_ReadFromInputPin(DHT11_PORT, DHT11_PIN) == HIGH);

        uint32_t bitEnd = TIMER_GetCounter(TIM2);
        uint32_t highDuration = bitEnd - bitStart;

        uint8_t bit = (highDuration > 40) ? 1 : 0;
        int byteIndex = i / 8;
        data[byteIndex] = (data[byteIndex] << 1) | bit;
    }

    uint8_t checksumCalc = data[0] + data[1] + data[2] + data[3];
    return (checksumCalc == data[4]) ? 1 : 0;
}
