//
//  types.h
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

#include <uARMconst.h>
#include <uARMtypes.h>

#define cpu_t unsigned int
#define memaddr unsigned int

typedef unsigned int uint;


typedef struct pcb_t {

	struct pcb_t *p_next;
	
	struct pcb_t *p_parent;
	struct pcb_t *p_first_child;
	struct pcb_t *p_sib;
	
	state_t tlb_old;
	state_t tlb_new;
	state_t pgm_old;
	state_t pgm_new;
	state_t sys_old;
	state_t sys_new;
	state_t p_s;   /* processor state */
	int p_priority;
	int *p_semKey;


	// Indirizzi per le aree NEW e OLD relative alle eccezioni lanciate dal processo
	state_t* old_sysBp;
	state_t* new_sysBp;
	state_t* old_tlb;
	state_t* new_tlb;
	state_t* old_pgm;
	state_t* new_pgm;

	// Variabili per la gestione del timing

	cpu_t time;
	cpu_t kernelTime;
	cpu_t userTime;

} pcb_t;


typedef struct semd_t {
	struct semd_t *s_next;		  /*ASHL list link*/
	int *s_key;                   /*indirizzo variabile intera che contiene il valore del semaforo*/
	struct pcb_t *s_procQ;		  /*coda processi bloccati*/
	
} semd_t;


/* Struttura per gli Interrupt Acknowledgements */
typedef enum ack_type {
	ACK_GEN_DEVICE,
	ACK_TERM_TRANSMIT,
	ACK_TERM_RECIVE
} ack_type;


#endif