################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Radzio_T6963_LIB/graphic.c \
../Radzio_T6963_LIB/t6963c.c 

OBJS += \
./Radzio_T6963_LIB/graphic.o \
./Radzio_T6963_LIB/t6963c.o 

C_DEPS += \
./Radzio_T6963_LIB/graphic.d \
./Radzio_T6963_LIB/t6963c.d 


# Each subdirectory must supply rules for building sources it contributes
Radzio_T6963_LIB/%.o: ../Radzio_T6963_LIB/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644pa -DF_CPU=14745600UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


