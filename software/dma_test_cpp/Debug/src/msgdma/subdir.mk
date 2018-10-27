################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/msgdma/sgdma_dispatcher.c 

OBJS += \
./src/msgdma/sgdma_dispatcher.o 

C_DEPS += \
./src/msgdma/sgdma_dispatcher.d 


# Each subdirectory must supply rules for building sources it contributes
src/msgdma/%.o: ../src/msgdma/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler 4 [arm-linux-gnueabihf]'
	arm-linux-gnueabihf-gcc -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


