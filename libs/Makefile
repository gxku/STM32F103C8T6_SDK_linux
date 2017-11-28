
TARGET = libSTM32F1.a

LIB = $(shell find ./ -name '*.c')  
STARTUP = $(STMLIB)/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.S

OBJ += $(LIB:%.c=%.o)
SOBJ += $(STARTUP:%.S=%.o)


# Rules shared between targets and prerequisites
PHONY += all
all: $(SOBJ) $(OBJ)
	$(AR) cr $(BIN)/$(TARGET) $(SOBJ) $(OBJ)
	rm $(OBJ) 

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $< -o $@

.PHONY: PHONY
