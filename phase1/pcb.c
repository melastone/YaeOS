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

//#include <stdio.h>
#include <uARMtypes.h>
#include "const.h"
#include "pcb.h"
#include "types.h"

pcb_t *pcbfree_h ; /* testa della lista pcbFree */



/************************* Allocazione e deallocazione dei PCB ************************/


/// Funzione ausiliaria per la chiamata ricorsiva di initPcb().

void initPcbsRic(pcb_t pcbArray[], int i, pcb_t *pcbfree_fun){
	
	if(i<MAXPROC){
		pcbfree_fun = &pcbArray[i];
		i++;
		initPcbsRic(pcbArray, i, pcbfree_fun->p_next);
	}
}


/// Inizializza la pcbFree in modo da contenere tutti gli elementi della pcbFree_table. 
/// Questo metodo deve essere chiamato una volta sola in fase di inizializzazione della 
/// struttura dati.
void initPcbs() {

	static struct pcb_t pcbFree_table[MAXPROC];
	
	int i = 0;
	initPcbsRic(pcbFree_table, i, pcbfree_h);
}


/// Funzione ausiliaria per la chiamata ricorsiva di freePcbs().
void freePcbRic(pcb_t *pcbfree_fun, pcb_t *p){
    
    if(pcbfree_h == NULL) pcbfree_h = p;

	else if (pcbfree_fun->p_next == NULL) pcbfree_fun->p_next = p;
	
	else freePcbRic(pcbfree_fun->p_next, p);
}


/// Inserisce il PCB puntato da p nella lista dei PCB liberi.
void freePcb (pcb_t *p) {
	
	// Se la pcbFree è vuota
	if(pcbfree_h == NULL) pcbfree_h = p;

	else freePcbRic(pcbfree_h->p_next, p);
}

/// Funzione ausiliaria per la chiamata ricorsiva di allocPcb().
pcb_t *allocPcbRic(pcb_t *pcbfree_fun) {
	
	pcb_t *pcb_rimosso;
	
	if(pcbfree_fun->p_next == NULL) {
		pcb_rimosso = pcbfree_fun;
		pcbfree_fun = pcbfree_fun->p_next;
		return pcb_rimosso;
	} else{
		return allocPcbRic(pcbfree_fun->p_next);
	}
}


/// Restituisce NULL se la pcbFree è vuota - CIOÈ SE NON CI SONO PCB LIBERI - . 
/// Altrimenti rimuove un elemento dalla pcbFree, inizializza tutti i campi (NULL/0) 
/// e restituisce l'elemento rimosso.
pcb_t *allocPcb() {

	pcb_t *pcb_rimosso;
	
	// Se la pcbFree è vuota
	if(pcbfree_h == NULL) return NULL;
	
	// Se la pcbFree contiene un solo elemento
	else if(pcbfree_h->p_next == NULL) {
		pcb_rimosso = pcbfree_h;
		pcbfree_h = pcbfree_h->p_next;
		return pcb_rimosso;
	} else {
		return allocPcbRic(pcbfree_h->p_next);
	}
}



/****************************** Gestione delle code dei PCB ******************************/
/*
/// Inserisce l'elemento puntato da p nella coda dei processi puntata da head.
/// L'inserimento deve avvenire tenendo conto della priorità di ciascun PCB.
/// La coda dei processi deve essere ordinata in base alla priorità dei PCB, in ordine 
/// decrescente (i.e. l'elemento di testa è l'elemento con la priorità più alta).
void insertProcQ(pcb_t **head, pcb_t *p) {
	if(**head->*head == NULL) **head->*head = *p;
	else if(**head->*head->next == NULL) {
		if(**head->*head->priority < *p->priority){
			pcb_t *tmp;
			*tmp = **head->*head;
			**head->*head = *p;
			**head->*head->next = *tmp;
		} else {
			**head->*head->next = *p;
		}
	} else {
		insertProcQ(**head->next,*p);
	}
}

/// Restituisce l'elemento di testa della coda dei processi puntata da head, senza 
/// rimuoverlo. Ritorna NULL se la coda non ha elementi.
pcb_t *headProcQ(pcb_t *head) {
	if(*head == NULL) return NULL;
	else return *head;
}

/// Rimuove il primo elemento della coda dei processi puntata da head. 
/// Ritorna NULL se la coda è vuota. Altrimenti ritorna il puntatore all'elemento rimosso.
pcb_t *removeProcQ(pcb_t **head) {
	if(**head->*head == NULL) return NULL;
	else {
		pcb_t *tmp = **head->*head;
		**head = **head->*head->next;
		return *tmp;
	}
}

/// Rimuove il PCB puntato da p dalla coda dei processi puntata da head. Se p non è 
/// presente nella coda restituisce NULL. Nota: p può trovarsi in una posizione arbitraria.
pcb_t *outProcQ(pcb_t **head, pcb_t *p) {
	if(**head->*head == *p) {
		pcb_t *tmp = **head->*head;
		**head = **head->*head->next;
		return *tmp;	
	} else if(**head->*head != *p && *head->next == NULL){
		return null;
	} else {
		return *outProcQ(**head->next, *p);
	}
}


/// Richiama la funzione fun per ogni elemento della lista puntata da head.
void forallProcQ(pcb_t *head, void fun(pcb_t *pcb, void *), void *arg) {

}*/



/****************************** Gestione dell'albero dei PCB *****************************/


///Inserisce il PCB puntato da p come fratello del PCB puntato da sib.
void insertSibling(pcb_t *sib, pcb_t *p) {

	if (sib->p_sib = NULL) {
		
		sib->p_sib = p ;
		return ;

	}

	return insertSibling(sib->p_sib, p) ;
}


///Rimuove il PCB puntato da p se si trova nella posizione successiva del PCB puntato da sib.
pcb_t *removeSibling(pcb_t *sib, pcb_t *p) {

	if (sib->p_sib = p) {

		pcb_t *tmp = p ;
		sib->p_sib = p->p_sib ;
		tmp->p_parent, tmp->p_sib = NULL ;
		return tmp ;

	}

	//Se p non è il fratello successivo di sib 
	return removeSibling(sib->p_sib, p) ;

}


/// Inserisce il PCB puntato da p come figlio del PCB puntato da parent.
void insertChild(pcb_t *parent, pcb_t *p) {

	p->p_parent = parent ;

	//Se parent non  ha nessun figlio
	if (parent->p_first_child = NULL) {

    	parent->p_first_child = p ;
    	return;
	
	}

	//Se invece ci sono già altri figli
	else {

		return insertSibling(parent->p_first_child, p) ;

	}
}


/// Rimuove il primo figlio del PCB puntato da p. Se p non ha figli restituisce NULL.
pcb_t *removeChild(pcb_t *p) {

	//Se p non ha figli
	if (p->p_first_child = NULL) {

		return NULL ;

	}

	pcb_t *child = p->p_first_child ;
	p->p_first_child = child->p_sib ;
	child->p_parent, child->p_sib = NULL ;
	return child ;

}


/// Rimuove il PCB puntato da p dalla lista dei figli del padre. Se il PCB puntato da p 
/// non ha un padre, restituisce NULL. Altrimenti restituisce l'elemento rimosso (cioè p).
/// A differenza della removeChild, p può trovarsi in una posizione arbitraria (ossia non è
/// necessariamente il primo figlio del padre).
pcb_t *outChild(pcb_t *p) {

	if (p->p_parent = NULL) {

		return NULL;

	}

	//Se p è il primo figlio
	if (p->p_parent->p_first_child = p) {

		return removeChild(p->p_parent) ;
	}

	//Se invece p si trova in un'altra posizione
	return removeSibling(p->p_parent->p_first_child, p) ;

}