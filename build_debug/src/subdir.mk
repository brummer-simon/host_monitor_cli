################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/main.cpp \
../src/Args.cpp \
../src/Config.cpp \
../src/Util.cpp \
../src/GroupElement.cpp \
../src/ObserverElement.cpp \
../src/Window.cpp \
../src/UserInterface.cpp

OBJS += \
./src/main.o \
./src/Args.o \
./src/Config.o \
./src/Util.o \
./src/GroupElement.o \
./src/ObserverElement.o \
./src/Window.o \
./src/UserInterface.o

CPP_DEPS += \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -I$(GIT_REPOS)/host_monitor/include -O0 -g3 -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


