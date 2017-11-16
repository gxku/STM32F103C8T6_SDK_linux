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
STMLIB = $(TOP)/libs/STM32_USB-FS-Device_Lib_V4.0.0/Libraries

TARGET = Demo

STARTUP = $(STMLIB)/CMSIS/Device/ST/STM32F10x/Source/Templates/gcc_ride7/startup_stm32f10x_md.S

SRC = $(shell find ./ -name '*.c')  

INC += -I$(STMLIB)/CMSIS/Include
INC += -I$(STMLIB)/CMSIS/Device/ST/STM32F10x/Include
INC += -I$(STMLIB)/STM32F10x_StdPeriph_Driver/inc
INC += -I$(TOP)/inc
INC += -I$(TOP)/led

OBJ += $(SRC:%.c=%.o)
OBJ += $(STARTUP:%.S=%.o)

# Compiler parameters 
MCU  = -mcpu=cortex-m3 -mthumb -mlittle-endian 

# Adjust TypeOfMCU in use, see CMSIS file "stm32f10x.h"
# STM32F103RBT (128KB FLASH, 20KB RAM) --> TypeOfMCU=STM32F10X_MD
# STM32F103ZET (512KB FLASH, 64KB RAM) --> TypeOfMCU=STM32F10X_HD
TypeOfMCU = STM32F10X_MD

MACRO  = -D$(TypeOfMCU) -DUSE_STDPERIPH_DRIVER

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

PHONY += st-flash
stlink: all 
	st-flash write $(TARGET).bin 0x8000000 

.PHONY: PHONY
