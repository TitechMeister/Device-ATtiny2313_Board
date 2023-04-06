DEVICE	= atmega88
WRITER	= arduino
PORT		= COM3
OBJECTS	= main.o

FUSES     = -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0x07:m -F

AVRDUDE = avrdude -c arduino -P $(PORT) -p $(DEVICE) -b 19200
CXX     = avr-g++
COMPILE = $(CXX) -std=c++11 -Wall -mmcu=$(DEVICE) -Wl,-u,vfprintf -lprintf_flt

all: main.hex

flash: all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

.PHONY: clean
clean:
	rm -f main.hex main.elf $(OBJECTS)
.cpp.o:
	$(COMPILE) -c $< -o $@

main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf
