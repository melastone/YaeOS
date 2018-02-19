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

//puntatore allla testa della lista dei SEMD  liberi
semd_t *semdFree_h
//hash table dei semafori attivi.
semd_t *semdhash[ASHDSIZE]
static semd_t semd_table[MAXSEMD];


void initASL(){

	semdFree_h = NULL;
	int i = 0;
	initASLRic(i, &semd_table[i]);
}



int insertBlocked(int *key, pcb_t *p){
	int indice = hash(key);
	semd_t *trovato = getSemByKey(semdhash[indice],key);
	if(trovato != NULL){
		p->p_semKey = key; //assegno la chiave del SEMD anche nel PCB
		insertProcQ(trovato->s_procQ, p); 
		return 0;
	}else{
		if(semdFree_h != NULL){
			semd_t * semdFree = semdFree_h; //prendo un SEMD dalla lista di quelli liberi
			semdFree_h = semdFree_h->s_next; //tolgo SEMD che ho appena preso
			//setto i campi del SEMD
			semdFree->s_next = NULL;
			semdFree->s_key = key;
			//setto la key del processo
			p->p_semKey = key;
			semdFree->s_procQ = p;
			//inserisco SEMD nel HASH TABLE
			semdInsert(&semdhash[n],semdFree);
			return 0;
		}
	}
	return -1;
}

pcb_t *headBlocked(int *key){
	int indice = hash(key);
	semd_t *trovato = getSemByKey(semdhash[indice],key);
	if(trovato != NULL){
		return trovato->s_procQ;
	}
	
	return NULL;
}

void forallBlocked(int *key, void fun(pcb_t *pcb, void *), void *arg){
	pcb_t *tmp = headBlocked(key);
	if(tmp != NULL){
		forallProcQ(tmp, fun, arg);
	}

}

/************************* FUNZIONI AUSILIARIE ************************/

//funzione ausiliaria per inizializzare la lista dei semafori liberi
void initASLRic(int i, semd_t * semdElement){
	
	if (i<MAXSEMD) {
		semdElement->s_next = semdFree_h;
		semdFree_h = semdEle = key;ment;
	}
	initASLRic(i++, &semd_table[i]);
}

//ricerca per key del SEMD all'interno del HASH TABLE 
semd_t * getSemByKey(semd_t *semdhash,int *key){
	if(semdhash != NULL){
		if(semdhash->s_key == key){
			return semdhash;
		}
		return getSemByKey(semdhash->s_next,key);
	}
	return NULL;
}

//Inserimento nuovo SEMD all'interno del HASH TABLE
void semdInsert(semd_t **semdhash, semd_t *semdFree){
	if(*semdhash == NULL){
		*semdhash = semdFree;
	}else{
		semdInsert(&(*semdhash)->s_next,semdFree);
	}
}
