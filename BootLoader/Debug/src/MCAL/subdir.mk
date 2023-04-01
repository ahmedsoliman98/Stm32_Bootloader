################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MCAL/Dma.c \
../src/MCAL/FlashDriver.c \
../src/MCAL/GPIO.c \
../src/MCAL/NVIC.c \
../src/MCAL/Rcc.c \
../src/MCAL/Systick.c \
../src/MCAL/Uart_cfg.c \
../src/MCAL/Usart.c 

OBJS += \
./src/MCAL/Dma.o \
./src/MCAL/FlashDriver.o \
./src/MCAL/GPIO.o \
./src/MCAL/NVIC.o \
./src/MCAL/Rcc.o \
./src/MCAL/Systick.o \
./src/MCAL/Uart_cfg.o \
./src/MCAL/Usart.o 

C_DEPS += \
./src/MCAL/Dma.d \
./src/MCAL/FlashDriver.d \
./src/MCAL/GPIO.d \
./src/MCAL/NVIC.d \
./src/MCAL/Rcc.d \
./src/MCAL/Systick.d \
./src/MCAL/Uart_cfg.d \
./src/MCAL/Usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/MCAL/%.o: ../src/MCAL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


