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

TARGET = Demo

#STARTUP = $(STMLIB)/CMSIS/Device/ST/STM32F10x/Source/Templates/gcc_ride7/startup_stm32f10x_md.S
STARTUP = $(STMLIB)/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s

LIB = $(shell find ./libs -name '*.c')  
USR = $(shell find ./usr -name '*.c')
#include src/Makefile

INC += -I$(STMLIB)/CMSIS/Include
INC += -I$(STMLIB)/CMSIS/Device/ST/STM32F1xx/Include
INC += -I$(STMLIB)/STM32F1xx_HAL_Driver/Inc
INC += -I$(STMLIB)/BSP/STM32F1xx_Nucleo
INC += -I$(TOP)/usr/inc

OBJ += $(LIB:%.c=%.o)
OBJ += $(USR:%.c=%.o)
OBJ += $(STARTUP:%.S=%.o)

# Compiler parameters 
MCU  = -mcpu=cortex-m3 -mthumb -mlittle-endian 

# Adjust TypeOfMCU in use, see CMSIS file "stm32f1xx.h"
# STM32F103RBT (128KB FLASH, 20KB RAM) --> TypeOfMCU=STM32F10X_MD
# STM32F103ZET (512KB FLASH, 64KB RAM) --> TypeOfMCU=STM32F10X_HD
TypeOfMCU = STM32F103xB 

MACRO  = -D$(TypeOfMCU) -DUSE_STDPERIPH_DRIVER -DARM_MATH_CM3

CFLAGS = -c -Wall $(MCU) $(MACRO) $(INC) -std=c99

LINKER = stm32_flash.ld
LDFLAGS += -Wl,--gc-sections --specs=nano.specs --specs=nosys.specs 
LDFLAGS += -Wl,--start-group -lgcc -lc -lm -Wl,--end-group 
LDFLAGS += -Wl,-Map=$(BIN)/$(TARGET).map -Wl,-cref,-u,Reset_Handler -Wl,--defsym=malloc_getpagesize_P=0x80 
LDFLAGS += $(MCU) -T$(LINKER) 

# Rules shared between targets and prerequisites
PHONY += all
all: $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $(BIN)/$(TARGET).elf
	$(OBJCOPY) -Obinary $(BIN)/$(TARGET).elf $(BIN)/$(TARGET).bin
	$(OBJCOPY) -Oihex $(BIN)/$(TARGET).elf $(BIN)/$(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $< -o $@

PHONY += clean
clean:
	rm -f $(shell find ./ -name '*.o')
	rm -f $(shell find ./ -name '*.d')
	rm -f $(shell find ./ -name '*.map')
	rm -f $(shell find ./ -name '*.elf')
	rm -f $(shell find ./ -name '*.bin')
	rm -f $(shell find ./ -name '*.hex')

flash: all 
	stm32flash -w $(BIN)/$(TARGET).hex -v -g 0 /dev/ttyUSB0

.PHONY: PHONY
