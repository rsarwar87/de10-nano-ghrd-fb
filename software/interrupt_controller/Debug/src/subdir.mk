################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/csr_map.cpp \
../src/interrupt_controller.cpp 

OBJS += \
./src/csr_map.o \
./src/interrupt_controller.o 

CPP_DEPS += \
./src/csr_map.d \
./src/interrupt_controller.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler 4 [arm-linux-gnueabihf]'
	arm-linux-gnueabihf-g++ -std=c++0x -Dsoc_cv_av -Dm -I/wsfat/altera_2018.1/embedded/ip/altera/hps/altera_hps/hwlib/include -I/wsfat/altera_2018.1/embedded/ip/altera/hps/altera_hps/hwlib/include/soc_cv_av -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


