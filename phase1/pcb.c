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

#include <uARMtypes.h>

#include "pcb.h"
#include "const.h"

pcb_t *pcbfree_h ; /* testa della lista pcbFree */



/****************************** Gestione delle code dei PCB ******************************/

/// Inserisce l'elemento puntato da p nella coda dei processi puntata da head.
/// L'inserimento deve avvenire tenendo conto della priorità di ciascun PCB.
/// La coda dei processi deve essere ordinata in base alla priorità dei PCB, in ordine 
/// decrescente (i.e. l'elemento di testa è l'elemento con la priorità più alta).
void insertProcQ(pcb_t **head, pcb_t *p) {
	
	// Se procQ è vuota
	if((*head) == NULL) (*head) = p;
	
	// Se procQ ha un solo elemento
	else if((*head)->p_next == NULL) {
		
		if((*head)->p_priority < p->p_priority) {
			p->p_next = (*head);
			(*head) = p;
		} 
		else {
			(*head)->p_next = p;
		}

	} 
	else {
		
		insertProcQ(&(*head)->p_next, p);
	
	}
}


/// Restituisce l'elemento di testa della coda dei processi puntata da head, senza 
/// rimuoverlo. Ritorna NULL se la coda non ha elementi.
pcb_t *headProcQ(pcb_t *head) {

	if(head == NULL) return NULL;
	
	else return head;
}


/// Rimuove il primo elemento della coda dei processi puntata da head. 
/// Ritorna NULL se la coda è vuota. Altrimenti ritorna il puntatore all'elemento rimosso.
pcb_t *removeProcQ(pcb_t **head) {
	
	// tmp punta all'elemento da rimuovere
    pcb_t *tmp = headProcQ(*head) ;
   
    // Se la coda non è vuota
	if (tmp != NULL) {

		(*head) = (*head)->p_next;
		tmp->p_next = NULL ;
	}

	return tmp ;
}

/// Rimuove il PCB puntato da p dalla coda dei processi puntata da head. Se p non è 
/// presente nella coda restituisce NULL. Nota: p può trovarsi in una posizione arbitraria.
pcb_t *outProcQ(pcb_t **head, pcb_t *p) {
	
	// Se p si trova in testa
	if((*head) == p) {
		return removeProcQ(&(*head)) ;	
	}

	// Se p non è presente nella coda 
	else if((*head) != p && (*head)->p_next == NULL){
		return NULL;
	} 

	else {
		return outProcQ(&(*head)->p_next, p);
	}
}


/// Richiama la funzione fun per ogni elemento della lista puntata da head.
void forallProcQ(pcb_t *head, void fun(pcb_t *pcb, void *), void *arg) {

}



/************************* Allocazione e deallocazione dei PCB ************************/

/// Inserisce il PCB puntato da p nella lista dei PCB liberi.
void freePcb (pcb_t *p) {
	
	insertProcQ(&pcbfree_h, p) ;

}


/// Funzione ausiliaria per la chiamata ricorsiva di initPcb().
void initPcbsRic(pcb_t pcbArray[], int i){
	
	if(i<MAXPROC){
		freePcb(&pcbArray[i]);
		i++;
		initPcbsRic(pcbArray, i);
	}

}


/// Inizializza la pcbFree in modo da contenere tutti gli elementi della pcbFree_table. 
/// Questo metodo deve essere chiamato una volta sola in fase di inizializzazione della 
/// struttura dati.
void initPcbs() {

	static struct pcb_t pcbFree_table[MAXPROC];
	
	int i = 0;
	initPcbsRic(pcbFree_table, i);
}


/// Restituisce NULL se la pcbFree è vuota - CIOÈ SE NON CI SONO PCB LIBERI - . 
/// Altrimenti rimuove un elemento dalla pcbFree, inizializza tutti i campi (NULL/0) 
/// e restituisce l'elemento rimosso.
pcb_t *allocPcb() {

	// Rimuove l'elemento in testa alla pcbFree
	pcb_t *pcb_rimosso = removeProcQ(&pcbfree_h) ;

	if (pcb_rimosso != NULL) {

		// Inizializza i campi a NULL e restituisce l'elemento
	    pcb_rimosso->p_next = NULL ;

	    pcb_rimosso->p_parent = NULL ;
	    pcb_rimosso->p_first_child = NULL ;
	    pcb_rimosso->p_sib = NULL ;

	    //Inizializzo tutti i campi della struttura p_s a null
	    pcb_rimosso->p_s.a1 = 0;
		pcb_rimosso->p_s.a2 = 0 ;
		pcb_rimosso->p_s.a3 = 0 ;
		pcb_rimosso->p_s.a4 = 0 ;
		pcb_rimosso->p_s.v1 = 0 ;
		pcb_rimosso->p_s.v2 = 0 ;
		pcb_rimosso->p_s.v3 = 0 ;
		pcb_rimosso->p_s.v4 = 0 ;
		pcb_rimosso->p_s.v5 = 0 ;
		pcb_rimosso->p_s.v6 = 0 ;
		pcb_rimosso->p_s.sl = 0 ;
		pcb_rimosso->p_s.fp = 0 ;
		pcb_rimosso->p_s.ip = 0 ;
		pcb_rimosso->p_s.sp = 0 ;
		pcb_rimosso->p_s.lr = 0 ;
		pcb_rimosso->p_s.pc = 0 ;
		pcb_rimosso->p_s.cpsr = 0 ;
		pcb_rimosso->p_s.CP15_Control = 0 ;
		pcb_rimosso->p_s.CP15_EntryHi = 0;
		pcb_rimosso->p_s.CP15_Cause = 0 ;
		pcb_rimosso->p_s.TOD_Hi = 0 ;
		pcb_rimosso->p_s.TOD_Low = 0 ;

	    pcb_rimosso->p_priority = 0 ;
	    pcb_rimosso->p_semKey = NULL ;
	}

    return pcb_rimosso ;
}



/****************************** Gestione dell'albero dei PCB *****************************/


///Inserisce il PCB puntato da p come fratello del PCB puntato da sib.
void insertSibling(pcb_t *sib, pcb_t *p) {

	if (sib->p_sib == NULL) {
		
		sib->p_sib = p ;
		return ;

	}

	return insertSibling(sib->p_sib, p) ;
}


///Rimuove il PCB puntato da p se si trova nella posizione successiva del PCB puntato da sib.
pcb_t *removeSibling(pcb_t *sib, pcb_t *p) {

	if (sib->p_sib == p) {

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
	if (parent->p_first_child == NULL) {

    	parent->p_first_child = p ;
    	return ;
	
	}

	//Se invece ci sono già altri figli
	else {

		return insertSibling(parent->p_first_child, p) ;

	}
}


/// Rimuove il primo figlio del PCB puntato da p. Se p non ha figli restituisce NULL.
pcb_t *removeChild(pcb_t *p) {

	//Se p non ha figli
	if (p->p_first_child == NULL) {

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

	if (p->p_parent == NULL) {

		return NULL;

	}

	//Se p è il primo figlio
	if (p->p_parent->p_first_child == p) {

		return removeChild(p->p_parent) ;
	}

	//Se invece p si trova in un'altra posizione
	return removeSibling(p->p_parent->p_first_child, p) ;

}