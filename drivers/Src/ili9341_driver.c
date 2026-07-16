/*
 * ili9341_driver.c
 *
 *  Created on: 9 Jul 2026
 *      Author: kangajan
 */
#include<stdint.h>
#include "ili9341_driver.h"

static const uint8_t font5x7_digits[10][5] = {
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
};

static const uint8_t font5x7_letters[11][5] = {
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x7F, 0x41, 0x41, 0x41, 0x3E}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
};

static const uint8_t font5x7_extra[4][5] = {
    {0x00, 0x00, 0x60, 0x00, 0x00}, // . (period)
    {0x00, 0x00, 0x00, 0x00, 0x00}, // (space)
    {0x23, 0x13, 0x08, 0x64, 0x62}, // % (percent)
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
};


static void ILI9341_Delay(void) {
    for (volatile uint32_t i = 0; i < 50000; i++);   // rough placeholder, not yet calibrated
}

static void ILI9341_WriteCommand(ILI9341_Handle_t *pILI9341, uint8_t command) {
    GPIO_WriteToOutputPin(pILI9341->pDC_Port, pILI9341->DC_Pin, LOW);
    GPIO_WriteToOutputPin(pILI9341->pCS_Port, pILI9341->CS_Pin, LOW);
    SPI_SendData(pILI9341->pSPIx, &command, 1);
    GPIO_WriteToOutputPin(pILI9341->pCS_Port, pILI9341->CS_Pin, HIGH);
}

static void ILI9341_WriteData(ILI9341_Handle_t *pILI9341, uint8_t command) {
    GPIO_WriteToOutputPin(pILI9341->pDC_Port, pILI9341->DC_Pin, HIGH);
    GPIO_WriteToOutputPin(pILI9341->pCS_Port, pILI9341->CS_Pin, LOW);
    SPI_SendData(pILI9341->pSPIx, &command, 1);
    GPIO_WriteToOutputPin(pILI9341->pCS_Port, pILI9341->CS_Pin, HIGH);
}

void ILI9341_Init(ILI9341_Handle_t *pILI9341) {
	GPIO_Handle_t pinConfig;

	// SCK and MOSI pin setup (SPI1 alternate function)
	pinConfig.pGPIOx = GPIOA;
	pinConfig.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;   // SCK
	pinConfig.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	pinConfig.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	pinConfig.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	pinConfig.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	pinConfig.GPIO_PinConfig.GPIO_PinAltFunMode = 5;   // AF5 = SPI1 on F407
	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&pinConfig);

	pinConfig.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;   // MOSI
	GPIO_Init(&pinConfig);

	// CS pin setup
	pinConfig.pGPIOx = pILI9341->pCS_Port;
	pinConfig.GPIO_PinConfig.GPIO_PinNumber = pILI9341->CS_Pin;
	pinConfig.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	pinConfig.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	pinConfig.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	pinConfig.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_PeriClockControl(pILI9341->pCS_Port, ENABLE);
	GPIO_Init(&pinConfig);
	// DC pin setup
	pinConfig.pGPIOx = pILI9341->pDC_Port;
	pinConfig.GPIO_PinConfig.GPIO_PinNumber = pILI9341->DC_Pin;
	pinConfig.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	pinConfig.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	pinConfig.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	pinConfig.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_PeriClockControl(pILI9341->pDC_Port, ENABLE);
	GPIO_Init(&pinConfig);
	// RESET setup
	pinConfig.pGPIOx = pILI9341->pRST_Port;
	pinConfig.GPIO_PinConfig.GPIO_PinNumber = pILI9341->RST_Pin;
	pinConfig.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	pinConfig.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	pinConfig.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	pinConfig.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_PeriClockControl(pILI9341->pRST_Port, ENABLE);
	GPIO_Init(&pinConfig);

	//SPI setup
	SPI_Handle_t SPIHandle;
	SPIHandle.pSPIx = SPI1;
	SPIHandle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPIHandle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPIHandle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPIHandle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPIHandle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPIHandle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPIHandle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	SPI_PeriClockControl(SPI1, ENABLE);
	SPI_Init(&SPIHandle);
	SPI_SSIConfig(SPI1, ENABLE);           // <-- add this: prevents MODF fault
	SPI_PeripheralControl(SPI1, ENABLE);   // <-- add this line, actually turns SPI on


	// RESET Configuration
	GPIO_WriteToOutputPin(pILI9341->pRST_Port, pILI9341->RST_Pin, LOW);
	ILI9341_Delay();
	GPIO_WriteToOutputPin(pILI9341->pRST_Port, pILI9341->RST_Pin, HIGH);
	ILI9341_Delay();

	// Real ILI9341 init sequence, from MSP2807 reference lcd.c
	ILI9341_WriteCommand(pILI9341, 0xCF);
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0xC9);
	ILI9341_WriteData(pILI9341, 0x30);

	ILI9341_WriteCommand(pILI9341, 0xED);
	ILI9341_WriteData(pILI9341, 0x64);
	ILI9341_WriteData(pILI9341, 0x03);
	ILI9341_WriteData(pILI9341, 0x12);
	ILI9341_WriteData(pILI9341, 0x81);

	ILI9341_WriteCommand(pILI9341, 0xE8);
	ILI9341_WriteData(pILI9341, 0x85);
	ILI9341_WriteData(pILI9341, 0x10);
	ILI9341_WriteData(pILI9341, 0x7A);

	ILI9341_WriteCommand(pILI9341, 0xCB);
	ILI9341_WriteData(pILI9341, 0x39);
	ILI9341_WriteData(pILI9341, 0x2C);
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0x34);
	ILI9341_WriteData(pILI9341, 0x02);

	ILI9341_WriteCommand(pILI9341, 0xF7);
	ILI9341_WriteData(pILI9341, 0x20);

	ILI9341_WriteCommand(pILI9341, 0xEA);
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0x00);

	ILI9341_WriteCommand(pILI9341, 0xC0);   // Power control
	ILI9341_WriteData(pILI9341, 0x1B);

	ILI9341_WriteCommand(pILI9341, 0xC1);   // Power control
	ILI9341_WriteData(pILI9341, 0x00);

	ILI9341_WriteCommand(pILI9341, 0xC5);   // VCM control
	ILI9341_WriteData(pILI9341, 0x30);
	ILI9341_WriteData(pILI9341, 0x30);

	ILI9341_WriteCommand(pILI9341, 0xC7);   // VCM control2
	ILI9341_WriteData(pILI9341, 0xB7);

	ILI9341_WriteCommand(pILI9341, 0x36);   // Memory Access Control
	ILI9341_WriteData(pILI9341, 0x08);

	ILI9341_WriteCommand(pILI9341, 0x3A);   // Pixel Format
	ILI9341_WriteData(pILI9341, 0x55);

	ILI9341_WriteCommand(pILI9341, 0xB1);
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0x1A);

	ILI9341_WriteCommand(pILI9341, 0xB6);   // Display Function Control
	ILI9341_WriteData(pILI9341, 0x0A);
	ILI9341_WriteData(pILI9341, 0xA2);

	ILI9341_WriteCommand(pILI9341, 0xF2);   // 3Gamma disable
	ILI9341_WriteData(pILI9341, 0x00);

	ILI9341_WriteCommand(pILI9341, 0x26);   // Gamma curve select
	ILI9341_WriteData(pILI9341, 0x01);

	ILI9341_WriteCommand(pILI9341, 0xE0);   // Positive Gamma
	ILI9341_WriteData(pILI9341, 0x0F);
	ILI9341_WriteData(pILI9341, 0x2A);
	ILI9341_WriteData(pILI9341, 0x28);
	ILI9341_WriteData(pILI9341, 0x08);
	ILI9341_WriteData(pILI9341, 0x0E);
	ILI9341_WriteData(pILI9341, 0x08);
	ILI9341_WriteData(pILI9341, 0x54);
	ILI9341_WriteData(pILI9341, 0xA9);
	ILI9341_WriteData(pILI9341, 0x43);
	ILI9341_WriteData(pILI9341, 0x0A);
	ILI9341_WriteData(pILI9341, 0x0F);
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0x00);

	ILI9341_WriteCommand(pILI9341, 0xE1);   // Negative Gamma
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0x15);
	ILI9341_WriteData(pILI9341, 0x17);
	ILI9341_WriteData(pILI9341, 0x07);
	ILI9341_WriteData(pILI9341, 0x11);
	ILI9341_WriteData(pILI9341, 0x06);
	ILI9341_WriteData(pILI9341, 0x2B);
	ILI9341_WriteData(pILI9341, 0x56);
	ILI9341_WriteData(pILI9341, 0x3C);
	ILI9341_WriteData(pILI9341, 0x05);
	ILI9341_WriteData(pILI9341, 0x10);
	ILI9341_WriteData(pILI9341, 0x0F);
	ILI9341_WriteData(pILI9341, 0x3F);
	ILI9341_WriteData(pILI9341, 0x3F);
	ILI9341_WriteData(pILI9341, 0x0F);

	ILI9341_WriteCommand(pILI9341, 0x2B);   // Row Address Set
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0x01);
	ILI9341_WriteData(pILI9341, 0x3F);

	ILI9341_WriteCommand(pILI9341, 0x2A);   // Column Address Set
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0x00);
	ILI9341_WriteData(pILI9341, 0xEF);

	ILI9341_WriteCommand(pILI9341, 0x11);   // Exit Sleep
	ILI9341_Delay();
	ILI9341_Delay();
	ILI9341_Delay();

	ILI9341_WriteCommand(pILI9341, 0x29);   // Display ON

}
void ILI9341_SetWindow(ILI9341_Handle_t *pILI9341, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    ILI9341_WriteCommand(pILI9341, 0x2A);        // Column Address Set
    ILI9341_WriteData(pILI9341, x1 >> 8);
    ILI9341_WriteData(pILI9341, x1 & 0xFF);
    ILI9341_WriteData(pILI9341, x2 >> 8);
    ILI9341_WriteData(pILI9341, x2 & 0xFF);

    ILI9341_WriteCommand(pILI9341, 0x2B);        // Row Address Set
    ILI9341_WriteData(pILI9341, y1 >> 8);
    ILI9341_WriteData(pILI9341, y1 & 0xFF);
    ILI9341_WriteData(pILI9341, y2 >> 8);
    ILI9341_WriteData(pILI9341, y2 & 0xFF);

    ILI9341_WriteCommand(pILI9341, 0x2C);        // Memory Write
}

void ILI9341_DrawPixel(ILI9341_Handle_t *pILI9341, uint16_t x, uint16_t y, uint16_t color) {
	// step 1: set window to just this one pixel
	ILI9341_SetWindow(pILI9341, x, y, x, y);
	// step 2: send the colour, high byte then low byte
	ILI9341_WriteData(pILI9341, color >> 8);     // high byte
	ILI9341_WriteData(pILI9341, color & 0xFF);   // low byte

}
void ILI9341_FillScreen(ILI9341_Handle_t *pILI9341, uint16_t color) {
	ILI9341_SetWindow(pILI9341, 0, 0, 239, 319);

	for (int i = 0; i < 76800; i++) {
		ILI9341_WriteData(pILI9341, color >> 8);     // high byte
		ILI9341_WriteData(pILI9341, color & 0xFF);   // low byte
	}
}
void ILI9341_ClearArea(ILI9341_Handle_t *pILI9341, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    ILI9341_SetWindow(pILI9341, x1, y1, x2, y2);

    uint32_t pixelCount = (uint32_t)(x2 - x1 + 1) * (y2 - y1 + 1);
    for (uint32_t i = 0; i < pixelCount; i++) {
        ILI9341_WriteData(pILI9341, color >> 8);
        ILI9341_WriteData(pILI9341, color & 0xFF);
    }
}

//void ILI9341_DrawChar(ILI9341_Handle_t *pILI9341, uint16_t x, uint16_t y, char digit, uint16_t color) {
//    const uint8_t *charData;
//
//    if (digit >= '0' && digit <= '9') {
//        charData = font5x7_digits[digit - '0'];
//    } else if (digit == '.') {
//        charData = font5x7_extra[0];
//    } else if (digit == ' ') {
//        charData = font5x7_extra[1];
//    } else if (digit == '%') {
//        charData = font5x7_extra[2];
//    } else if (digit == 'C') {
//        charData = font5x7_extra[3];
//    } else {
//        return;   // unsupported character, skip drawing anything
//    }
//
//    for (int col = 0; col < 5; col++) {
//        uint8_t columnData = charData[col];
//
//        for (int row = 0; row < 7; row++) {
//            if (columnData & (1 << row)) {
//                ILI9341_DrawPixel(pILI9341, x + col, y + row, color);
//            }
//        }
//    }
//}

void ILI9341_DrawChar(ILI9341_Handle_t *pILI9341, uint16_t x, uint16_t y, char digit, uint16_t color, uint8_t size) {
    const uint8_t *charData;

    if (digit >= 'a' && digit <= 'z') {
        digit -= 32;   // convert lowercase to uppercase
    }

    if (digit >= '0' && digit <= '9') {
        charData = font5x7_digits[digit - '0'];
    } else if (digit == '.') {
        charData = font5x7_extra[0];
    } else if (digit == ' ') {
        charData = font5x7_extra[1];
    } else if (digit == '%') {
        charData = font5x7_extra[2];
    } else if (digit == 'C') {
        charData = font5x7_extra[3];
    } else if (digit == ':') {
        static const uint8_t colon[5] = {0x00, 0x00, 0x36, 0x00, 0x00};
        charData = colon;
    } else {
        switch (digit) {
            case 'A': charData = font5x7_letters[0]; break;
            case 'D': charData = font5x7_letters[1]; break;
            case 'E': charData = font5x7_letters[2]; break;
            case 'H': charData = font5x7_letters[3]; break;
            case 'I': charData = font5x7_letters[4]; break;
            case 'M': charData = font5x7_letters[5]; break;
            case 'P': charData = font5x7_letters[6]; break;
            case 'R': charData = font5x7_letters[7]; break;
            case 'T': charData = font5x7_letters[8]; break;
            case 'U': charData = font5x7_letters[9]; break;
            case 'Y': charData = font5x7_letters[10]; break;
            default: return;
        }
    }

    for (int col = 0; col < 5; col++) {
        uint8_t columnData = charData[col];
        for (int row = 0; row < 7; row++) {
            if (columnData & (1 << row)) {
                for (int dx = 0; dx < size; dx++) {
                    for (int dy = 0; dy < size; dy++) {
                        ILI9341_DrawPixel(pILI9341, x + (col * size) + dx, y + (row * size) + dy, color);
                    }
                }
            }
        }
    }
}

void ILI9341_DrawString(ILI9341_Handle_t *pILI9341, uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t size) {
    uint16_t currentX = x;
    int i = 0;

    while (str[i] != '\0') {
        ILI9341_DrawChar(pILI9341, currentX, y, str[i], color, size);
        currentX += 6 * size;   // spacing scales with size too
        i++;
    }
}

void ILI9341_DrawRect(ILI9341_Handle_t *pILI9341, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t thickness) {
    ILI9341_ClearArea(pILI9341, x1, y1, x2, y1 + thickness - 1, color);           // top edge
    ILI9341_ClearArea(pILI9341, x1, y2 - thickness + 1, x2, y2, color);           // bottom edge
    ILI9341_ClearArea(pILI9341, x1, y1, x1 + thickness - 1, y2, color);           // left edge
    ILI9341_ClearArea(pILI9341, x2 - thickness + 1, y1, x2, y2, color);           // right edge
}
