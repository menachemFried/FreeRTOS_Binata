################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../UserApp/Root/Src/UserCallBack.c \
../UserApp/Root/Src/app_main.c \
../UserApp/Root/Src/producerTask1.c \
../UserApp/Root/Src/producerTask2.c 

OBJS += \
./UserApp/Root/Src/UserCallBack.o \
./UserApp/Root/Src/app_main.o \
./UserApp/Root/Src/producerTask1.o \
./UserApp/Root/Src/producerTask2.o 

C_DEPS += \
./UserApp/Root/Src/UserCallBack.d \
./UserApp/Root/Src/app_main.d \
./UserApp/Root/Src/producerTask1.d \
./UserApp/Root/Src/producerTask2.d 


# Each subdirectory must supply rules for building sources it contributes
UserApp/Root/Src/%.o UserApp/Root/Src/%.su UserApp/Root/Src/%.cyclo: ../UserApp/Root/Src/%.c UserApp/Root/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DDEBUG_ON -DSTM32F411xE -DUSE_STM32F4XX_NUCLEO -DUSE_HAL_DRIVER -c -I"C:/Work_H/VSCode_Projects/ST_Exampels/stm32F4/F411RE/FreeRTOS_Binata/Project/FreeRTOS_Binata/Core/Inc" -I"C:/Work_H/VSCode_Projects/ST_Exampels/stm32F4/F411RE/FreeRTOS_Binata/Project/FreeRTOS_Binata/UserApp/PeriphDrivers/USART/Inc" -I"C:/Work_H/VSCode_Projects/ST_Exampels/stm32F4/F411RE/FreeRTOS_Binata/Project/FreeRTOS_Binata/UserApp/Root/Inc" -I"C:/Work_H/VSCode_Projects/ST_Exampels/stm32F4/F411RE/FreeRTOS_Binata/Project/FreeRTOS_Binata/..//Cube_F4_V1.28.2/Drivers/BSP/Components/Common" -I"C:/Work_H/VSCode_Projects/ST_Exampels/stm32F4/F411RE/FreeRTOS_Binata/Project/FreeRTOS_Binata/..//Cube_F4_V1.28.2/Drivers/BSP/STM32F4xx-Nucleo" -I"C:/Work_H/VSCode_Projects/ST_Exampels/stm32F4/F411RE/FreeRTOS_Binata/Project/FreeRTOS_Binata/..//Cube_F4_V1.28.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Work_H/VSCode_Projects/ST_Exampels/stm32F4/F411RE/FreeRTOS_Binata/Project/FreeRTOS_Binata/..//Cube_F4_V1.28.2/Drivers/CMSIS/Include" -I"C:/Work_H/VSCode_Projects/ST_Exampels/stm32F4/F411RE/FreeRTOS_Binata/Project/FreeRTOS_Binata/..//Cube_F4_V1.28.2/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Work_H/VSCode_Projects/ST_Exampels/stm32F4/F411RE/FreeRTOS_Binata/Project/FreeRTOS_Binata/..//Cube_F4_V1.28.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Work_H/VSCode_Projects/ST_Exampels/stm32F4/F411RE/FreeRTOS_Binata/Project/FreeRTOS_Binata/..//Cube_F4_V1.28.2/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2" -I"C:/Work_H/VSCode_Projects/ST_Exampels/stm32F4/F411RE/FreeRTOS_Binata/Project/FreeRTOS_Binata/..//Cube_F4_V1.28.2/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Work_H/VSCode_Projects/ST_Exampels/stm32F4/F411RE/FreeRTOS_Binata/Project/FreeRTOS_Binata/..//Cube_F4_V1.28.2/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-UserApp-2f-Root-2f-Src

clean-UserApp-2f-Root-2f-Src:
	-$(RM) ./UserApp/Root/Src/UserCallBack.cyclo ./UserApp/Root/Src/UserCallBack.d ./UserApp/Root/Src/UserCallBack.o ./UserApp/Root/Src/UserCallBack.su ./UserApp/Root/Src/app_main.cyclo ./UserApp/Root/Src/app_main.d ./UserApp/Root/Src/app_main.o ./UserApp/Root/Src/app_main.su ./UserApp/Root/Src/producerTask1.cyclo ./UserApp/Root/Src/producerTask1.d ./UserApp/Root/Src/producerTask1.o ./UserApp/Root/Src/producerTask1.su ./UserApp/Root/Src/producerTask2.cyclo ./UserApp/Root/Src/producerTask2.d ./UserApp/Root/Src/producerTask2.o ./UserApp/Root/Src/producerTask2.su

.PHONY: clean-UserApp-2f-Root-2f-Src

