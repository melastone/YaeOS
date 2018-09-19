/*
 *	INIT.H
 *	Kernel Entry Point Header File
 *
 *
 *	Gruppo 13:
 *	Daniel Veroli
 *  Melania Ghelli
 *	Lorenzo Turrini
 *	Lorenzo Poluzzi
 *
 */

#ifndef INIT_H
#define INIT_H 

#include <uARMconst.h>
#include <uARMtypes.h>
#include <pcb.h>
#include <asht.h>
#include <types.n>
#include <libuarm.h>






/* Data Structures used for execution */

unsigned int processCount;         // Number of total processes
unsigned int softBlockCount;	   // Number of processes waiting for I/O operations
pcb_t *readyQueue;                 // Ready queue of processes
pcb_t *curProc;					   // Current process executing


/* Data Structures for Time Management */









#endif