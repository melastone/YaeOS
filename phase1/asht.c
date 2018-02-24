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

#include "asht.h"
#include "pcb.h"
#include "const.h"
#include "types.h"


semd_t *semdFree_h ; //puntatore allla testa della lista dei SEMD  liberi

semd_t *semdhash[ASHDSIZE] ; // hash table dei semafori attivi.

static semd_t semd_table[MAXSEMD];


/***************************************************************
*                    FUNZIONI AUSILIARIE                       *
***************************************************************/



/// Ricerca per key del SEMD all'interno del HASH TABLE 
semd_t *getSemByKey(semd_t *semdhash,int *key){
	if(semdhash != NULL){
		if(semdhash->s_key == key){
			return semdhash;
		}
		return getSemByKey(semdhash->s_next,key);
	}
	return NULL;
}


/// Inserimento nuovo SEMD ELEMENT all'interno del HASH TABLE dei SEMD attivi
void semdInsert(semd_t **semdHead, semd_t *semdElement){
	if(*semdHead == NULL){
		*semdHead = semdElement;
	}else{
		semdInsert(&(*semdHead)->s_next,semdElement);
	}
}


/// Rimozione SEMD attraverso la key all'interno della lista puntata dalla HASH TABLE dei SEMD attivi
void semdRemove(semd_t **semdHead,int *key){
	
	if(*semdHead != NULL){
		
		if((*semdHead)->s_key == key){
			(*semdHead) = (*semdHead) ->s_next;
		}
		
		else{
			semdRemove(&((*semdHead)->s_next),key);
		}
	}
}



/************************* HASH FUNCTION ************************/

unsigned int hash(unsigned int x) {
    //calcolo il valore di hash
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
	
	//riporto l'hash calcolata in un indice fra 0 e ASHDZISE-1
    x = x % ASHDSIZE;
   
    return x;
}



/***************************************************************
*                    ACTIVE SEMAPHORE HASH TABLE               *
***************************************************************/

/// Funzione ausiliaria per inizializzare la lista dei semafori liberi
void initASLRic(int i, semd_t *semdElement){
	
	if (i<MAXSEMD) {
		semdElement->s_next = semdFree_h;
		semdFree_h = semdElement;
		i++;
		initASLRic(i, &semd_table[i]);
	}
}


/// Inizializza la lista dei semdFree in modo da contenere tutti gli elementi della semdTable.
/// Questo metodo viene invocato una volta sola durante l’inizializzazione della struttura dati.
void initASL(){

	semdFree_h = NULL;
	int i = 0;
	initASLRic(i, &semd_table[i]);
}


/// Viene inserito il PCB puntato da p nella coda dei processi bloccati associata al semaforo con chiave key.
/// Se il semaforo corrispondente non è presente nella ASHT, alloca un nuovo SEMD dalla lista di quelli liberi 
/// e lo inserisce nella ASHT, settando i campi in maniera opportuna. Se non è possibile allocare un nuovo SEMD 
/// perché la lista di quelli liberi e’ vuota, restituisce -1. In tutti gli altri casi, restituisce 0.
int insertBlocked(int *key, pcb_t *p){
	
	if (p != NULL){

		// Cerco il descrittore associato alla chiave Key (potrebbe non essere attivo)
		int indice = hash(*key);
		semd_t *trovato = getSemByKey(semdhash[indice],key);

		// Ritorno NULL se il semaforo non è attivo
		if (trovato != NULL) {
			p->p_semKey = key; //assegno la chiave del SEMD anche nel PCB
			insertProcQ(&trovato->s_procQ, p); 
			return 0;
		}

		else {
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
				semdInsert(&semdhash[indice],semdFree);
				return 0;
			}
		}
	}
	
	return -1;
}


/// Ritorna il primo PCB dalla coda dei processi bloccati (s_ProcQ) associata al SEMD della ASHT con chiave key.
/// Se tale descrittore non esiste nella ASHT, restituisce NULL. Altrimenti, restituisce l’elemento rimosso. 
/// Se la coda dei processi bloccati per il semaforo diventa vuota, rimuove il descrittore corrispondente dalla 
/// ASHT e lo inserisce nella coda dei descrittori liberi (semdFree).
pcb_t* removeBlocked(int *key){
	
	// Cerco il descrittore associato alla chiave Key (potrebbe non essere attivo)
	int index = hash(*key);
	semd_t *trovato = getSemByKey(semdhash[index],key);
	
	// Ritorno NULL se il semaforo non è attivo
	if (trovato == NULL ) {
		return NULL;
	}

	// Se il semaforo è attivo, controllo i processi bloccati
	else {
		
		pcb_t* removedPCB = trovato->s_procQ;
		
		// Se non è l'unico processo nella coda dei processi bloccati
		// lo rimuovo e restituisco il pcb rimosso
		if (trovato->s_procQ->p_next != NULL) {
			trovato->s_procQ = trovato->s_procQ->p_next;
		}

		// Se è l'unico rimasto in coda, lo rimuovo e riporto il SEMD 
		// nella lista dei descrittori liberi
		else {
				semdRemove(&semdhash[index],key);
				trovato->s_procQ = NULL;
				trovato->s_next = NULL;
				semdInsert(&semdFree_h,trovato);
				
		}

		return removedPCB;
	}
}


/// Restituisce il puntatore al pcb del primo processo bloccato sul semaforo, senza deaccordarlo. 
/// Se il semaforo non esiste restituisce NULL.
pcb_t *headBlocked(int *key){
	
	int indice = hash(*key);
	semd_t *trovato = getSemByKey(semdhash[indice],key);
	
	if(trovato != NULL){
		return trovato->s_procQ;
	}
	
	return NULL;
}


/// Rimuove il PCB puntato da p dalla coda del semaforo su cui è bloccato. (La hash table deve essere aggiornata 
/// in modo coerente).
pcb_t* outChildBlocked(pcb_t *p){
	
	// Controllo che il pcb non sia nullo
	if (p != NULL){
		
		int *key = p->p_semKey;
		int index = hash(*key);
		semd_t *trovato = getSemByKey(semdhash[index], key);
		pcb_t *semdHead = headBlocked(key);

		if(semdHead != NULL){
			
			pcb_t *temp = outProcQ(&semdHead, p);
			
			// Si modifica la testa della lista dei processi modificati nel caso sia p l'unico processo
			if(semdHead == NULL){
				
				semdRemove(&semdhash[index],key);
				trovato->s_procQ = NULL;
				trovato->s_next = NULL;
				semdInsert(&semdFree_h,trovato);
				
			}

			return temp;
		}
	}
	
	return NULL;

}


/// Richiama la funzione fun per ogni processo bloccato sul semaforo identificato da key.
void forallBlocked(int *key, void fun(pcb_t *pcb, void *), void *arg){
	
	pcb_t *tmp = headBlocked(key);
	
	if(tmp != NULL){
		forallProcQ(tmp, fun, arg);
	}

}




