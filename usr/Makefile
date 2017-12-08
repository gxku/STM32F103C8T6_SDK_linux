
TARGET = MAIN
SLIB = -lSTM32F1
SLIB_PATH = -L $(TOP)/bin

USR = $(shell find ./ -name '*.c')


OBJ += $(USR:%.c=%.o)

INC += -I$(TOP)/usr/inc

LINKER = stm32_flash.ld
LDFLAGS += -Wl,--gc-sections  --specs=nosys.specs 
#LDFLAGS += -Wl,--gc-sections --specs=nano.specs --specs=nosys.specs 
LDFLAGS += -Wl,--start-group -lgcc -lc -lm -Wl,--end-group 
LDFLAGS += -Wl,-Map=$(BIN)/$(TARGET).map -Wl,-cref,-u,Reset_Handler -Wl,--defsym=malloc_getpagesize_P=0x80 
LDFLAGS += $(MCU) -T$(LINKER) 
LDFLAGS += $(SLIB)
# Rules shared between targets and prerequisites
PHONY += all
all: $(OBJ)
	$(CC) $^ $(SLIB_PATH)  $(LDFLAGS)   -o $(BIN)/$(TARGET).elf
	$(OBJCOPY) -Obinary $(BIN)/$(TARGET).elf $(BIN)/$(TARGET).bin
	$(OBJCOPY) -Oihex $(BIN)/$(TARGET).elf $(BIN)/$(TARGET).hex
#rm $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $< -o $@

.PHONY: PHONY
