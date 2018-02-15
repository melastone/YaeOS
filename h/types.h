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

typedef struct pcb_t {

	struct pcb_t *p_next;
	
	struct pcb_t *p_parent;
	struct pcb_t *p_first_child;
	struct pcb_t *p_sib;
	
	state_t p_s;   /* processor state */
	int priority;
	int *p_semKey;
} pcb_t;

/************************* Allocazione e deallocazione dei PCB ************************/

//inizializzazione pcb
void initPcbs();

//Inserisce il pcb puntato da p nella lista pcbFree
void freePcb (pcb_t *p);

//Rimuove elemento in testa dalla lista, se la lista è vuota restituisce null
pcb_t *allocPcb();

/****************************** Gestione delle code dei PCB ******************************/

//Inserisce l'elemento puntato da p nella coda dei processi puntata da head
// ordinati in base alla priorità (decrescente)
void insertProcQ(pcb_t **head, pcb_t *p);

//Restituisce l'elemento della coda puntato da head senza rimuoverlo, null se la coda è vuota
pcb_t *headProcQ(pcb_t *head);

//Rimuove il primo elemento della coda puntato da head
// ritorna il puntatore all'elemento rimosso, null se la coda è vuota
pcb_t *removeProcQ(pcb_t **head);

//Rimuove il PCB puntato da p dalla coda dei processi puntata da head
//restituisce il pcb puntato da p, se non è presente restituisce null
pcb_t *outProcQ(pcb_t **head, pcb_t *p);

//Richiama la funzione fun per ogni elemento della lista puntata da head
void forallProcQ(pcb_t *head, void fun(pcb_t *pcb, void *), void *arg);

