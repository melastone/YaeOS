/*
 *	ASL.H
 *	Active Semaphore Hash Table Header File
 *
 *  Gruppo 13:
 *  Daniel Veroli
 *  Lorenzo Turrini
 *  Lorenzo Poluzzi
 *  Melania Ghelli
 *
 *
 */

#ifndef ASL_H
#define ASL_H

/* Sempahore Functions Prototypes */
void initASL();

int insertBlocked(int *key, pcb_t *p);
pcb_t* headBlocked(int *key);
pcb_t* removeBlocked(int *key);
void forallBlocked(int *key, void fun(pcb_t *pcb, void *), void *arg);
pcb_t* outChildBlocked(pcb_t *p);


#endif