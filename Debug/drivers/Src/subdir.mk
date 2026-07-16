################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/Src/dht11_driver.c \
../drivers/Src/ili9341_driver.c \
../drivers/Src/stm32f407xx_gpio_driver.c \
../drivers/Src/stm32f407xx_spi_driver.c \
../drivers/Src/stm32f407xx_timer_driver.c 

OBJS += \
./drivers/Src/dht11_driver.o \
./drivers/Src/ili9341_driver.o \
./drivers/Src/stm32f407xx_gpio_driver.o \
./drivers/Src/stm32f407xx_spi_driver.o \
./drivers/Src/stm32f407xx_timer_driver.o 

C_DEPS += \
./drivers/Src/dht11_driver.d \
./drivers/Src/ili9341_driver.d \
./drivers/Src/stm32f407xx_gpio_driver.d \
./drivers/Src/stm32f407xx_spi_driver.d \
./drivers/Src/stm32f407xx_timer_driver.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/Src/%.o drivers/Src/%.su drivers/Src/%.cyclo: ../drivers/Src/%.c drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I../drivers/Inc -I../drivers/Src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drivers-2f-Src

clean-drivers-2f-Src:
	-$(RM) ./drivers/Src/dht11_driver.cyclo ./drivers/Src/dht11_driver.d ./drivers/Src/dht11_driver.o ./drivers/Src/dht11_driver.su ./drivers/Src/ili9341_driver.cyclo ./drivers/Src/ili9341_driver.d ./drivers/Src/ili9341_driver.o ./drivers/Src/ili9341_driver.su ./drivers/Src/stm32f407xx_gpio_driver.cyclo ./drivers/Src/stm32f407xx_gpio_driver.d ./drivers/Src/stm32f407xx_gpio_driver.o ./drivers/Src/stm32f407xx_gpio_driver.su ./drivers/Src/stm32f407xx_spi_driver.cyclo ./drivers/Src/stm32f407xx_spi_driver.d ./drivers/Src/stm32f407xx_spi_driver.o ./drivers/Src/stm32f407xx_spi_driver.su ./drivers/Src/stm32f407xx_timer_driver.cyclo ./drivers/Src/stm32f407xx_timer_driver.d ./drivers/Src/stm32f407xx_timer_driver.o ./drivers/Src/stm32f407xx_timer_driver.su

.PHONY: clean-drivers-2f-Src

