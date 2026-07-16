/*
 * dht11_driver.h
 *
 *  Created on: 9 Jul 2026
 *      Author: kangajan
 */

#ifndef INC_DHT11_DRIVER_H_
#define INC_DHT11_DRIVER_H_

#include <stdint.h>
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_timer_driver.h"

void DHT11_Init(void);
uint8_t DHT11_Read(uint8_t *data);   // fills a 5-byte array, returns 1 if checksum OK, 0 if failed


#endif /* INC_DHT11_DRIVER_H_ */
