################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ai.c \
../aiMinMax.c \
../main.c \
../play.c \
../setup.c 

OBJS += \
./ai.o \
./aiMinMax.o \
./main.o \
./play.o \
./setup.o 

C_DEPS += \
./ai.d \
./aiMinMax.d \
./main.d \
./play.d \
./setup.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


