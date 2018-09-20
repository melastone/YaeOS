//
//	ASHT.h
//	Active Semaphore Hash Table Header File
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

#ifndef ASL_H
#define ASL_H

#include "types.h"

/* Sempahore Functions Prototypes */
void initASL();

/* Insert PCB pointed to by p to the semaphore */
int insertBlocked(int *key, pcb_t *p);   

/* Remove first blocked ProcBlk on semAdd, if present, otherwise return NULL  */
pcb_t* headBlocked(int *key);

/* Remove ProcBlk associated to key*/
pcb_t* removeBlocked(int *key); 

/* Recalls fun function for every blocked process on the sem associated to key*/
void forallBlocked(int *key, void fun(pcb_t *pcb, void *), void *arg);

/* Removes pointed PCB from the queue wich is blocked in*/
pcb_t* outChildBlocked(pcb_t *p); 


#endif