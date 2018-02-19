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
#include "../asht.h"


void initASL(){
	
	semd_t semd_table[MAXSEMD];
}

int insertBlocked(int *key, pcb_t *p){

// Searching active sempahore whose identifier is key

// The searched active semaphore doesn't exist (it may not be active)

// Setting the blocked ProcBlk to the semaphore and adding it to the tail of
// process queue of the semaphore



}



// Metodi ausiliari

semd_t* getSemByKey(int *key, semd_t* semdPointer)