################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/example_opengl.o \
../src/lodepng.o 

CPP_SRCS += \
../src/OpenGL_Primary.cpp \
../src/example_opengl.cpp \
../src/lodepng.cpp 

OBJS += \
./src/OpenGL_Primary.o \
./src/example_opengl.o \
./src/lodepng.o 

CPP_DEPS += \
./src/OpenGL_Primary.d \
./src/example_opengl.d \
./src/lodepng.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

