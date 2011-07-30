###################################
# Makefile, for AVR Programming


# Target name:
TARGET = output

# Target type
MCU=atxmega128a1
MCUSHORT=x128a1
F_CPU=32000000L


SRC = main.c draw.c hw.c pong.c
OBJ = $(SRC:.c=.o)


# Compiler / Linker flags:
CFLAGS = -g -mmcu=$(MCU) -Wall -Os -std=gnu99 -D F_CPU=$(F_CPU) -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
LDFLAGS = -g -mmcu=$(MCU) -Wl,-Map=$(TARGET).map



###################################
# Makerules:


.PHONY: compile flash clean

compile: $(TARGET).hex $(TARGET).eep $(TARGET).lss

flash: compile
	avrdude -c jtagmkII -P usb -p $(MCUSHORT) -U flash:w:$(TARGET).hex
#avrdude -c jtagmkII -P usb -p $(MCUSHORT) -U eeprom:w:$(TARGET).eep
#	avrdude -c dragon_jtag -P usb -p $(MCUSHORT) -U flash:w:$(TARGET).hex
# sleep 2

clean:
	rm -f $(OBJ) $(TARGET).*



###################################
# Psudorules:

%.eep: %.hex
	avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex $< $@

%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

%.lss: %.elf
	avr-objdump -h -S $< > $@

%.elf: $(OBJ)
	avr-gcc $(LDFLAGS) $^ -o $@

%.o : %.c
	avr-gcc $(CFLAGS) -c $< -o $@
