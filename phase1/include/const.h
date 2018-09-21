//
//  CONST.h
//  File contenente le definizioni delle costanti globali
//
//  Yet Another Educational Operating System Phase 1
//
//  Creato il 28/01/2018
//
//  Gruppo 13:
//  Daniel Veroli
//  Lorenzo Turrini
//  Lorenzo Poluzzi
//  Melania Ghelli
//

#ifndef CONST_H
#define CONST_H

#define MAXPROC 20
#define MAXSEMD MAXPROC
#define ASHDSIZE 8

#define memaddr unsigned int

#define CREATEPROCESS    1
#define TERMINATEPROCESS 2
#define SEMP             3
#define SEMV             4
#define SPECHDL          5
#define GETTIME          6
#define WAITCLOCK        7
#define IODEVOP          8
#define GETPIDS          9
#define WAITCHLD        10

/// Tempi di Scheduling costanti 
#define TIME_SLICE 3000  
#define TICK_SYS7 100000
#define TICK_PRIORITY 10000


#endif