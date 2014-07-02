################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Radzio_T6963_LIB/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
LCD128x64_T6963.lss \

FLASH_IMAGE += \
LCD128x64_T6963.hex \

EEPROM_IMAGE += \
LCD128x64_T6963.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: LCD128x64_T6963.elf secondary-outputs

# Tool invocations
LCD128x64_T6963.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,LCD128x64_T6963.map -mmcu=atmega644pa -o "LCD128x64_T6963.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

LCD128x64_T6963.lss: LCD128x64_T6963.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S LCD128x64_T6963.elf  >"LCD128x64_T6963.lss"
	@echo 'Finished building: $@'
	@echo ' '

LCD128x64_T6963.hex: LCD128x64_T6963.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex LCD128x64_T6963.elf  "LCD128x64_T6963.hex"
	@echo 'Finished building: $@'
	@echo ' '

LCD128x64_T6963.eep: LCD128x64_T6963.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex LCD128x64_T6963.elf  "LCD128x64_T6963.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: LCD128x64_T6963.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega644pa LCD128x64_T6963.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) LCD128x64_T6963.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
