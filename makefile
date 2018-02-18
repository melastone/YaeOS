//
//  makefile
//  Automatically compile and build phase1 by running command "make" 
//
//  Creato il 18/02/2018
//
//  Gruppo 13
//  Daniel Veroli
//  Lorenzo Turrini
//  Lorenzo Poluzzi
//  Melania Ghelli
//

# ARM compiler
CC = arm-none-eabi-gcc
# Compiler flags
FLAG_CC = -mcpu=arm7tdmi

# ARM linker
UL = arm-none-eabi-ld

# Files paths
P1 = phase1
INCL_P1 = h/const.h h/pcb.h h/types.h 

all: test-pcb

test-pcb: arm-none-eabi-ld \
	-T /usr/include/uarm/ldscripts/elf32ltsarm.h.uarmcore.x \
	-o test pcb.o test-pcb.o \
	/usr/include/uarm/crtso.o /usr/include/uarm/libuarm.o


pcb.o: $(P1)/pcb.c $(INCL_P1)
	arm-none-eabi-gcc -mcpu=arm7tdmi -I /usr/include/uarm -I ../h -c pcb.c

test-pcb.o: 
	arm-none-eabi-gcc -mcpu=arm7tdmi -I /usr/include/uarm -I ../h -c test-pcb.c


