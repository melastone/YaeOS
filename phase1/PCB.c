//
//  PCB.c
//  Yet Another Educational Operating System Phase 1
//
//  Creato il 24/01/2018
//
//  Gruppo 13:
//  Daniel Veroli
//  Lorenzo Turrini
//  Lorenzo Poluzzi
//  Melania Ghelli
//

#include <stdio.h>
#include "../h/const.h"
#include "../h/types.h"

pcb_t *pcbfree_h  /* testa della lista pcbFree */



/************************* Allocazione e deallocazione dei PCB ************************/

/// Inizializza la pcbFree in modo da contenere tutti gli elementi della pcbFree_table. 
/// Questo metodo deve essere chiamato una volta sola in fase di inizializzazione della 
/// struttura dati.
void initPcbs() {

	static pcb_t pcbFree_table[MAXPROC] ;

}


/// Inserisce il PCB puntato da p nella lista dei PCB liberi
void freePcb (pcb_t *p) {

}


/// Restituisce NULL se la pcbFree è vuota - CIOÈ SE NON CI SONO PCB LIBERI - . 
/// Altrimenti rimuove un elemento dalla pcbFree, inizializza tutti i campi (NULL/0) 
/// e restituisce l'elemento rimosso.
pcb_t *allocPcb() {

}
