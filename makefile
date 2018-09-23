#
#  makefile
#  Automatically compile and build phase1 by running command "make" 
#
#  Creato il 18/02/2018
#
#  Gruppo 13
#  Daniel Veroli
#  Lorenzo Turrini
#  Lorenzo Poluzzi
#  Melania Ghelli
#


# ARM compiler
CC = arm-none-eabi-gcc
# Compiler flags
FLAG_CC = -mcpu=arm7tdmi -I $(INCL_UARM) -I ./$(INCL_P1)
FLAG_CC_P1 = $(FLAG_CC) -c
FLAG_CC_P2 = $(FLAG_CC) -I ./$(INCL_P2) -c

# ARM linker
UL = arm-none-eabi-ld
# Linker flags
FLAG_UL = -T $(INCL_UARM)/ldscripts/elf32ltsarm.h.uarmcore.x $(INCL_UARM)/crtso.o $(INCL_UARM)/libuarm.o

P1 = phase1
P2 = phase2

# Header files paths
INCL_P1 = $(P1)/include
INCL_P2 = $(P2)/include

INCL_UARM = /usr/include/uarm

INCL_DEFAULT = $(INCL_P1)/types.h $(INCL_P1)/const.h

INCL_PCB = $(INCL_P1)/pcb.h $(INCL_DEFAULT)
INCL_ASHT = $(INCL_P1)/pcb.h $(INCL_P1)/asht.h $(INCL_DEFAULT)

INCL_EXC =
INCL_INIT =
INCL_INTR =
INCL_PST =
INCL_SCH =
INCL_SYSC = $(INCL_DEFAULT) $(INCL_ASHT) $(INCL_P2)/syscall.h

# Source files paths
SRC_P1 = $(P1)/source
SRC_P2 = $(P2)/source

# Object files paths
OBJECTS_P2 = $(SRC_P2)/exception.o $(SRC_P2)/init.o $(SRC_P2)/interrupts.o $(SRC_P2)/pseudoTimer.o $(SRC_P2)/scheduler.o $(SRC_P2)/syscall.o $(SRC_P2)/p2test.o

all: yaeOS

yaeOS: p1 kernel

p1: pcb.o asht.o test-pcb.o
	$(UL) $(FLAG_UL) -o $@ pcb.o asht.o test-pcb.o

kernel: $(OBJECTS_P2)
	$(UL) $(FLAG_UL) -o $@ exception.o init.o interrupts.o pseudoTimer.o scheduler.o syscall.o p2test.o

pcb.o: $(SRC_P1)/pcb.c $(INCL_PCB)
	$(CC) $(FLAG_CC_P1) $(SRC_P1)/pcb.c

asht.o: $(SRC_P1)/asht.c $(INCL_ASHT)
	$(CC) $(FLAG_CC_P1) $(SRC_P1)/asht.c

test-pcb.o: $(P1)/test-pcb.c $(INCL_ASHT)
	$(CC) $(FLAG_CC_P1) $(P1)/test-pcb.c

exception.o: $(SRC_P2)/exception.c $(INCL_EXC)
	$(CC) $(FLAG_CC_P2) $(SRC_P2)/exception.c

init.o: $(SRC_P2)/init.c $(INCL_INIT)
	$(CC) $(FLAG_CC_P2) $(SRC_P2)/init.c

interrupts.o: $(SRC_P2)/interrupts.c $(INCL_INTR)
	$(CC) $(FLAG_CC_P2) $(SRC_P2)/interrupts.c

pseudoTimer.o: $(SRC_P2)/pseudoTimer.c $(INCL_PST)
	$(CC) $(FLAG_CC_P2) $(SRC_P2)/pseudoTimer.c

scheduler.o: $(SRC_P2)/scheduler.c $(INCL_SCH)
	$(CC) $(FLAG_CC_P2) $(SRC_P2)/scheduler.c

syscall.o: $(SRC_P2)/syscall.c $(INCL_SYSC)
	$(CC) $(FLAG_CC_P2) $(SRC_P2)/syscall.c

p2test.o: $(P2)/p2test.c $(INCL_ASHT) $(INCL_DEFAULT)
	$(CC) $(FLAG_CC_P2) $(P2)/p2test.c

clean:
	rm -f *.o 