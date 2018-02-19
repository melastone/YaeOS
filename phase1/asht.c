//  
//	ASHT.c
//  Yet Another Educational Operating System Phase 1
//
//  Active Semaphore Hash Table Implementation File
//
//  Creato il 15/02/2018
//
//  Gruppo 13:
//  Daniel Veroli
//  Lorenzo Turrini
//  Lorenzo Poluzzi
//  Melania Ghelli
//

#include "../const.h"
#include "../types.h"

//puntatore allla testa della lista  dei SEMD  liberi
semd_t *semdFree_h

//hash table dei semafori attivi.
semd_t *semdhash[ASHDSIZE]

void initASL(){
	
	static semd_t semd_table[MAXSEMD];

	int i=0;
	initASLRic(i, semd_t semd_table[MAXSEMD], semdFree_h, NULL);
}

void initASLRic(int i, semd_t semdArray[], semd_t * semdFree_head, semd_t * semdFree_succ){
	if(i == 0){
		semdFree_head = &semdArray[i];
		semdFree_head->s_next = semdFree_prec;
		i++;
	}
	else if(i < MAXSEMD-1){
		semdFree_succ = &semdArray[i];
		semdFree_succ->s_next = NULL;
		semdFree_head->s_next = semdFree_succ;
		i++;
	}
	initASLRic(i,semdArray[],semdFree_head,semdFree_head->s_next);
}

int insertBlocked(int *key, pcb_t *p){

// Searching active sempahore whose identifier is key

// The searched active semaphore doesn't exist (it may not be active)

// Setting the blocked ProcBlk to the semaphore and adding it to the tail of
// process queue of the semaphore



}



// Metodi ausiliari

semd_t* getSemByKey(int *key, semd_t* semdPointer)