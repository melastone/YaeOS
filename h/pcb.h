#ifndef CONST_H
#define CONST_H

#include "types.h"
#include "const.h"

/************************* Allocazione e deallocazione dei PCB ************************/

extern void initPcbsRic(pcb_t procArray[], int i,pcb_t *pcbfree_fun) ;

extern void initPcbs() ;

extern void freePcbRic(pcb_t *pcbfree_fun, pcb_t *p) ;

extern void freePcb (pcb_t *p) ;

extern pcb_t *allocPcbRic(pcb_t *pcbfree_fun) ; 

extern pcb_t *allocPcb() ;


/****************************** Gestione delle code dei PCB ******************************/

extern void insertProcQ(pcb_t **head, pcb_t *p) ;

extern pcb_t *headProcQ(pcb_t *head) ;

extern pcb_t *removeProcQ(pcb_t **head) ;

extern pcb_t *outProcQ(pcb_t **head, pcb_t *p) ;

extern void forallProcQ(pcb_t *head, void fun(pcb_t *pcb, void *), void *arg) ;


/****************************** Gestione dell'albero dei PCB *****************************/

extern void insertSibling(pcb_t *sib, pcb_t *p) ;

extern pcb_t *removeSibling(pcb_t *sib, pcb_t *p) ;

extern void insertChild(pcb_t *parent, pcb_t *p) ;

extern pcb_t *removeChild(pcb_t *p) ;

extern pcb_t *outChild(pcb_t *p) ;

#endif