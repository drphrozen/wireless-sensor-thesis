################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mros/Network/uStack/uStack.c 

OBJS += \
./mros/Network/uStack/uStack.o 

C_DEPS += \
./mros/Network/uStack/uStack.d 


# Each subdirectory must supply rules for building sources it contributes
mros/Network/uStack/%.o: ../mros/Network/uStack/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Coma\workspace_helios\RfSuitSweeper\mros" -I"C:\Users\Coma\workspace_helios\RfSuitSweeper\src" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128rfa1 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


