/*
 *	Yet Another Educational Operating System Phase 2
 *	------------------------------------------------
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
#include <arch.h>
#include <pcb.h>
#include <asht.h>
#include <types.h>
#include <libuarm.h>
#include <pseudoTimer.h>

#define memaddr unsigned int


/***************************************************************
*                        FUNZIONI ESTERNE                      *
***************************************************************/

EXTERN void test();
EXTERN void scheduler();

/***************************************************************
*                        VARIABILI KERNEL                      *
***************************************************************/


unsigned int processCounter;       // Numero dei processi totali nel sistema
unsigned int softBlockCounter;	   // Numero dei processi in attesa di operazioni di I/O
pcb_t *readyQueue;                 // Puntatore alla coda dei processi Ready
pcb_t *curProc;					   // Puntatore al processo corrente

int semDevices[MAX_DEVICES];		

/***************************************************************
*                        FUNZIONI AUSILIARIE                   *
***************************************************************/


void initArea(memaddr area, memaddr handler);











#endif