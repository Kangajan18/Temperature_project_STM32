#include <stdio.h>
#include <stdint.h>
#include "dht11_driver.h"
#include "ili9341_driver.h"
#include "stm32f407xx_gpio_driver.h"

#define COLOR_BG      0x0000   // black
#define COLOR_ACCENT  0x051D   // dark teal-blue
#define COLOR_BORDER  0x2965   // subtle gray-blue
#define COLOR_LABEL   0x07FF   // cyan
#define COLOR_VALUE   0xFFFF   // white

int main(void)
{
    DHT11_Init();

    uint8_t data[5];
    DHT11_Read(data);

    uint8_t lastTemp = 255, lastTempDec = 255;
    uint8_t lastHum = 255, lastHumDec = 255;

    ILI9341_Handle_t lcd;
    lcd.pSPIx = SPI1;
    lcd.pCS_Port = GPIOB;   lcd.CS_Pin = GPIO_PIN_NO_6;
    lcd.pDC_Port = GPIOB;   lcd.DC_Pin = GPIO_PIN_NO_8;
    lcd.pRST_Port = GPIOB;  lcd.RST_Pin = GPIO_PIN_NO_7;

    ILI9341_Init(&lcd);
    ILI9341_FillScreen(&lcd, COLOR_BG);

    // Top accent bar
    ILI9341_ClearArea(&lcd, 0, 0, 239, 6, COLOR_ACCENT);

    // Temperature card
    ILI9341_DrawRect(&lcd, 10, 30, 229, 110, COLOR_BORDER, 2);
    ILI9341_DrawString(&lcd, 22, 42, "TEMPERATURE", COLOR_LABEL, 2);

    // Humidity card
    ILI9341_DrawRect(&lcd, 10, 130, 229, 210, COLOR_BORDER, 2);
    ILI9341_DrawString(&lcd, 22, 142, "HUMIDITY", COLOR_LABEL, 2);

    char buffer[20];

    while(1) {
        if (DHT11_Read(data)) {
            if (data[2] != lastTemp || data[3] != lastTempDec) {
                ILI9341_ClearArea(&lcd, 22, 65, 210, 95, COLOR_BG);
                sprintf(buffer, "%2d.%d C", data[2], data[3]);
                ILI9341_DrawString(&lcd, 22, 65, buffer, COLOR_VALUE, 4);

                lastTemp = data[2];
                lastTempDec = data[3];
            }

            if (data[0] != lastHum || data[1] != lastHumDec) {
                ILI9341_ClearArea(&lcd, 22, 165, 210, 195, COLOR_BG);
                sprintf(buffer, "%2d.%d %%", data[0], data[1]);
                ILI9341_DrawString(&lcd, 22, 165, buffer, COLOR_VALUE, 4);

                lastHum = data[0];
                lastHumDec = data[1];
            }
        }

        for (volatile uint32_t i = 0; i < 2000000; i++);
    }
}
