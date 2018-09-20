//
//  PCB.h
//  Process Control Blocks Header File
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

#ifndef PCB_H
#define PCB_H

#include "types.h"
#include "const.h"


/****************************** Gestione delle code dei PCB ******************************/

extern void insertProcQ(pcb_t **head, pcb_t *p) ;

extern pcb_t *headProcQ(pcb_t *head) ;

extern pcb_t *removeProcQ(pcb_t **head) ;

extern pcb_t *outProcQ(pcb_t **head, pcb_t *p) ;

extern void forallProcQ(pcb_t *head, void fun(pcb_t *pcb, void *), void *arg) ;

/************************* Allocazione e deallocazione dei PCB ************************/

extern void freePcb (pcb_t *p) ;

extern void initPcbsRic(pcb_t procArray[], int i) ;

extern void initPcbs() ;

extern pcb_t *allocPcb() ;

/****************************** Gestione dell'albero dei PCB *****************************/

extern void insertSibling(pcb_t *sib, pcb_t *p) ;

extern pcb_t *removeSibling(pcb_t *sib, pcb_t *p) ;

extern void insertChild(pcb_t *parent, pcb_t *p) ;

extern pcb_t *removeChild(pcb_t *p) ;

extern pcb_t *outChild(pcb_t *p) ;

#endif