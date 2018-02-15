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

}

/// Inserisce il PCB puntato da p nella lista dei PCB liberi
void freePcb (pcb_t *p) {

}


/// Restituisce NULL se la pcbFree è vuota - CIOÈ SE NON CI SONO PCB LIBERI - . 
/// Altrimenti rimuove un elemento dalla pcbFree, inizializza tutti i campi (NULL/0) 
/// e restituisce l'elemento rimosso.
pcb_t *allocPcb() {

}



/****************************** Gestione delle code dei PCB ******************************/

/// Inserisce l'elemento puntato da p nella coda dei processi puntata da head.
/// L'inserimento deve avvenire tenendo conto della priorità di ciascun PCB.
/// La coda dei processi deve essere ordinata in base alla priorità dei PCB, in ordine 
/// decrescente (i.e. l'elemento di testa è l'elemento con la priorità più alta).
void insertProcQ(pcb_t **head, pcb_t *p) {

}


/// Restituisce l'elemento di testa della coda dei processi puntata da head, senza 
/// rimuoverlo. Ritorna NULL se la coda non ha elementi.
pcb_t *headProcQ(pcb_t *head) {

}


/// Rimuove il primo elemento della coda dei processi puntata da head. 
/// Ritorna NULL se la coda è vuota. Altrimenti ritorna il puntatore all'elemento rimosso.
pcb_t *removeProcQ(pcb_t **head) {

}


/// Rimuove il PCB puntato da p dalla coda dei processi puntata da head. Se p non è p
/// resente nella coda restituisce NULL. Nota: p può trovarsi in una posizione arbitraria.
pcb_t *outProcQ(pcb_t **head, pcb_t *p) {

}


/// Richiama la funzione fun per ogni elemento della lista puntata da head.
void forallProcQ(pcb_t *head, void fun(pcb_t *pcb, void *), void *arg) {

}



/****************************** Gestione dell'albero dei PCB *****************************/

/// Inserisce il PCB puntato da p come figlio del PCB puntato da parent.
void insertChild(pcb_t *parent, pcb_t *p) {

}


/// Rimuove il primo figlio del PCB puntato da p. Se p non ha figli restituisce NULL.
pcb_t *removeChild(pcb_t *p) {

}


/// Rimuove il PCB puntato da p dalla lista dei figli del padre. Se il PCB puntato da p 
/// non ha un padre, restituisce NULL. Altrimenti restituisce l'elemento rimosso (cioè p).
/// A differenza della removeChild, p può trovarsi in una posizione arbitraria (ossia non è
/// necessariamente il primo figlio del padre).
pcb_t *outChild(pcb_t *p) {

}
