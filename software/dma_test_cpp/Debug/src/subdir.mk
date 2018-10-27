################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/csr_map.cpp \
../src/dma_test.cpp \
../src/sgdma_dispatcher.cpp 

OBJS += \
./src/csr_map.o \
./src/dma_test.o \
./src/sgdma_dispatcher.o 

CPP_DEPS += \
./src/csr_map.d \
./src/dma_test.d \
./src/sgdma_dispatcher.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler 4 [arm-linux-gnueabihf]'
	arm-linux-gnueabihf-g++ -Dsoc_cv_av -I/wsfat/altera_2018.1/embedded/ip/altera/hps/altera_hps/hwlib/include -I/wsfat/altera_2018.1/embedded/ip/altera/hps/altera_hps/hwlib/include/soc_cv_av -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


