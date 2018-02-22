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
FLAG_CC = -mcpu=arm7tdmi -I $(INCL_UARM) -I $(INCL) -c

# ARM linker
UL = arm-none-eabi-ld
# Linker flags
FLAG_UL = -T $(INCL_UARM)/ldscripts/elf32ltsarm.h.uarmcore.x $(INCL_UARM)/crtso.o $(INCL_UARM)/libuarm.o

# Files paths
P1 = /phase1
INCL = /h
INCL_UARM = /usr/include/uarm
INCL_TST = $(INCL)/pcb.h $(INCL)/types.h
INCL_PCB = $(INCL)/const.h $(INCL_TST)

all: test

test: pcb.o test-pcb.o
	$(UL) $(FLAG_UL) -o $@ pcb.o test-pcb.o


pcb.o: $(P1)/pcb.c $(INCL_PCB)
	$(CC) $(FLAG_CC) $(P1)/pcb.c

test-pcb.o: $(P1)/test-pcb.c $(INCL_TST)
	$(CC) $(FLAG_CC) $(P1)/test-pcb.c


