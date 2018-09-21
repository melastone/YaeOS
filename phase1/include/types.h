//
//  TYPES.h
//  File contente le definizioni delle strutture di dati globali
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

#ifndef TYPES_H
#define TYPES_H

#include <uARMtypes.h>

unsigned int cputime_t ;
unsigned int memaddr ;

typedef struct pcb_t {

	struct pcb_t *p_next;
	
	struct pcb_t *p_parent;
	struct pcb_t *p_first_child;
	struct pcb_t *p_sib;
	
	state_t p_s;   /* processor state */
	int p_priority;
	int *p_semKey;

	cputime_t kernel_time ;
	cputime_t global_time ;
	cputime_t activation_time ;
    
} pcb_t;


typedef struct semd_t {
	struct semd_t *s_next;		  /*ASHL list link*/
	int *s_key;                   /*indirizzo variabile intera che contiene il valore del semaforo*/
	struct pcb_t *s_procQ;		  /*coda processi bloccati*/
	
} semd_t;

typedef unsigned int uint;

/* Struttura per gli Interrupt Acknowledgements */
typedef enum ack_type {
	ACK_GEN_DEVICE,
	ACK_TERM_TRANSMIT,
	ACK_TERM_RECIVE
} ack_type;

#endif