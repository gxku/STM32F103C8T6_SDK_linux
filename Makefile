# Make variables (CC, GDB, etc...)
TC      = arm-none-eabi-
CC      = $(TC)gcc           
AS      = $(TC)as
LD      = $(TC)ld
AR      = $(TC)ar
GDB     = $(TC)gdb
OBJCOPY = $(TC)objcopy

# Source files, Object files, etc...
TOP = $(shell pwd)
BIN = $(TOP)/bin
STMLIB = $(TOP)/libs/STM32Cube_FW_F1_V1.4.0/Drivers


INC += -I$(STMLIB)/CMSIS/Include
INC += -I$(STMLIB)/CMSIS/Device/ST/STM32F1xx/Include
INC += -I$(STMLIB)/STM32F1xx_HAL_Driver/Inc
INC += -I$(STMLIB)/BSP/STM32F1xx_Nucleo
INC += -I$(TOP)/usr/inc


# Compiler parameters 
MCU  = -mcpu=cortex-m3 -mthumb -mlittle-endian 

# Adjust TypeOfMCU in use, see CMSIS file "stm32f1xx.h"
# STM32F103RBT (128KB FLASH, 20KB RAM) --> TypeOfMCU=STM32F10X_MD
# STM32F103ZET (512KB FLASH, 64KB RAM) --> TypeOfMCU=STM32F10X_HD
TypeOfMCU = STM32F103xB 

MACRO  = -D$(TypeOfMCU) -DUSE_STDPERIPH_DRIVER -DARM_MATH_CM3

CFLAGS = -c -Wall $(MCU) $(MACRO) $(INC) -std=c99 -Os -fno-exceptions
CFLAGS += -ffunction-sections


# Rules shared between targets and prerequisites
PHONY += all
all: 
	$(if $(wildcard $(BIN)/libSTM32F1.a),,make -C libs)
	make -C usr

PHONY += clean
clean:
	rm -f $(shell find ./ -name '*.o')
	rm -f $(shell find ./ -name '*.d')
	rm -f $(shell find ./ -name '*.map')
	rm -f $(shell find ./ -name '*.elf')
	rm -f $(shell find ./ -name '*.bin')
	rm -f $(shell find ./ -name '*.hex')

PHONY += distclean
distclean:clean
	rm -f $(shell find ./bin -name '*.a')
	

flash: all 
	stm32flash -w $(BIN)/MAIN.bin -v -g 0 /dev/ttyUSB0

.PHONY: PHONY

export
