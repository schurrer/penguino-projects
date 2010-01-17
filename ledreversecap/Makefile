NAME       = ledreversecap_multi
DEVICE     = atmega32
CLOCK      = 16000000
LIB_PATH   = ../../../../../penguino-lib/
LIB_OBJS   = $(LIB_PATH)penguino/uart/RingBuff.o \
             $(LIB_PATH)penguino/uart/uart.o \
             $(LIB_PATH)penguino/uart/uart-stdio.o \
             $(LIB_PATH)penguino/spi/spi-master.o \
             $(LIB_PATH)penguino/io.o
OBJECTS    = $(NAME).o $(LIB_OBJS)
CFLAGS     = -I$(LIB_PATH)

COMPILE = avr-gcc -Wall -Os $(CFLAGS) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -std=gnu99

# symbolic targets:
all:	$(NAME).bin

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

clean:
	rm -f $(NAME).bin $(NAME).elf $(OBJECTS)

$(NAME).elf: $(OBJECTS)
	$(COMPILE) -o $(NAME).elf $(OBJECTS)

$(NAME).bin: $(NAME).elf
	rm -f $(NAME).bin
	avr-objcopy -j .text -j .data -O binary $(NAME).elf $(NAME).bin

disasm:	$(NAME).elf
	avr-objdump -d $(NAME).elf
