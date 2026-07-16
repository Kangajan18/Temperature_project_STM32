/*
 * ili9341_driver.h
 *
 *  Created on: 9 Jul 2026
 *      Author: kangajan
 */

#ifndef INC_ILI9341_DRIVER_H_
#define INC_ILI9341_DRIVER_H_

#include <stdint.h>
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"

#define HIGH 1
#define LOW  0



typedef struct {
    SPI_RegDef_t  *pSPIx;

    GPIO_RegDef_t *pCS_Port;
    uint8_t        CS_Pin;

    GPIO_RegDef_t *pDC_Port;
    uint8_t        DC_Pin;

    GPIO_RegDef_t *pRST_Port;
    uint8_t        RST_Pin;
} ILI9341_Handle_t;

void ILI9341_Init(ILI9341_Handle_t *pILI9341); //sets up the display, sends the INIT command sequence
void ILI9341_SetWindow(ILI9341_Handle_t *pILI9341, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2); // defines a rectangular region for the next pixel data
void ILI9341_DrawPixel(ILI9341_Handle_t *pILI9341, uint16_t x, uint16_t y, uint16_t color); //writes a single pixel's colour at a specific coordinate
void ILI9341_FillScreen(ILI9341_Handle_t *pILI9341, uint16_t color); // efficiently fills the whole display with one colour (which also serves as your "clear")
void ILI9341_DrawChar(ILI9341_Handle_t *pILI9341, uint16_t x, uint16_t y, char digit, uint16_t color, uint8_t size);
void ILI9341_DrawString(ILI9341_Handle_t *pILI9341, uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t size);
void ILI9341_ClearArea(ILI9341_Handle_t *pILI9341, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ILI9341_DrawRect(ILI9341_Handle_t *pILI9341, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t thickness);
#endif /* INC_ILI9341_DRIVER_H_ */
