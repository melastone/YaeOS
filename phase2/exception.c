//
//  EXCEPTION.c
//  File contente l'interfaccia delle System Call e la routine di gestione delle eccezioni
//
//  Yet Another Educational Operating System Phase 2
//
//  Creato il 12/09/2018
//
//  Gruppo 13:
//  Daniel Veroli
//  Lorenzo Turrini
//  Lorenzo Poluzzi
//  Melania Ghelli
//	

#include <const.h>
#include "./h/exception.h"
#include "../h/pcb.h"

/*********************************** Funzioni ausiliarie *********************************/

// Realizza una copia di tutti i campi di una struttura state_t, a partire dall'attuale 
// state nella nuova newState
void copyState(state_t* state, state_t* newState) {
    
    newState->a1 = state->a1;
    newState->a2 = state->a2;
    newState->a3 = state->a3;
    newState->a4 = state->a4;
    newState->v1 = state->v1;
    newState->v2 = state->v2;
    newState->v3 = state->v3;
    newState->v4 = state->v4;
    newState->v5 = state->v5;
    newState->v6 = state->v6;
    newState->sl = state->sl;
    newState->fp = state->fp;
    newState->ip = state->ip;
    newState->sp = state->sp;
    newState->lr = state->lr;
    newState->pc = state->pc;
    newState->cpsr = state->cpsr;
    newState->CP15_Control = state->CP15_Control;
    newState->CP15_EntryHi = state->CP15_EntryHi;
    newState->CP15_Cause = state->CP15_Cause;
    newState->TOD_Hi = state->TOD_Hi;
    newState->TOD_Low = state->TOD_Low; 

}

/*************************************** System Call *************************************/
// SYSCALL(unsigned int sysNum, unsigned int arg1, unsigned int arg2, unsigned
// int arg3)
// Generates a software exception leading to kernel defined Syscall handler. a1 is loaded
// with sysNum, a2 is loaded with arg1, a3 is loaded with arg2 and a4 is loaded with arg3

/// SYS1
// Crea un nuovo processo come figlio del chiamante. Il program counter, lo stack
// pointer, e i flag di configurazione (modo, vm etc) sono indicati nello stato iniziale.
// Se la system call ha successo il valore di ritorno è zero altrimenti è -1. 
// Se la chiamata ha successo cpid contiene l’identificatore del processo figlio
// (indirizzo del PCB).
// Al momento della chiamata di questa operazione, il processo chiamante salva nei propri 
// registri (campi di statep, istanza della struttura dati state_t definita in 
// /usr/include/uarm/uARMtypes.h) le seguenti informazioni:
// -a1 contiene la costante 1; nello stesso registro verranno poi memorizzate le informazioni
//  di esito della chiamata di SYS1
// -a2 contiene l'indirizzo fisico di uno state_t da usare come stato iniziale del nuovo processo
// -a3 contiene il livello di priorità 
int SYSCALL (CREATEPROCESS, state_t *statep, int priority, void **cpid) {

	pcb_t newP = allocPcb() ;

	// Se non ci sono pcb liberi la funzione deve ritornare -1
	if (newP != NULL) {

		(newP->p_priority) = priority ;

		processCount++ ;

		// Setto i valori di state_t del nuovo processo come quelli del genitore
		copyState(statep, &(newP->p_s)) ;

		// Creo i legami di parentela e inserisco il nuovo processo nella coda dei ready
		insertChild(curProc, newP) ;
		insertProcQ(&readyQueue, newP) ;

		cpid = &(newP) ;
		return 0 ;

	}

	return -1 ;
}

/// SYS2
// Termina il processo indicato (o il processo chiamante se pid==NULL) e tutta la progenie 
// del processo indicato. Ritorna 0 se l’operazione ha avuto successo, -1 in caso contrario 
// (ovviamente se pid==NULL oppure è il pid proprio o di un proprio avo, la chiamata se ha 
// successo NON ritorna).
int SYSCALL (TERMINATEPROCESS, void * pid)

/// SYS3
// Realizza l'operazione di P su un semaforo. Il valore del semaforo è memorizzato nella
// variabile di tipo intero passata per indirizzo. L’indirizzo della variabile agisce da
// indentificatore del semaforo.
void SYSCALL (SEMP, int *semaddr)

/// SYS4
// Realizza l'operazione di V su un semaforo. Il valore del semaforo è memorizzato nella
// variabile di tipo intero passata per indirizzo. L’indirizzo della variabile agisce da
// indentificatore del semaforo.
void SYSCALL (SEMV, int *semaddr)

/// SYS5
// Registra quale handler di livello superiore debba essere attivato in caso di trap di 
// Syscall/breakpoint (type=0), TLB (type=1) o Program trap (type=2).
// Il significato dei parametri old e new è lo stesso delle aree old e new gestitre dal 
// codice della ROM: quando avviene una trap da passare al gestore lo stato del processo 
// che ha causato la trap viene posto nell’area old e viene caricato lo stato presente 
// nell’area new. La system call deve essere richiamata una sola volta per tipo.
// Se la system call ha successo restituisce 0 altrimenti -1.
int SYSCALL (SPECHDL, int type, state_t *old, state_t *new)

/// SYS6
// Restituisce il valore di tre “tempi” del processo:
// – Il tempo usato dal processo in modalità user
// – Il tempo usato dal processo in modalità kernel (gestione system call e interrupt 
//   relativi al processo)
// – Il tempo trascorso dalla prima attivazione del processo.
void SYSCALL (GETTIME, cputime_t *user, cputime_t *kernel, cputime_t *wallclock)

/// SYS7
// Sospende il processo fino al prossimo tick di 100ms. Lo pseudoclock produce un tick 
// ogni 100ms (esatti) e risveglia tutti i processi che hanno chiesto la wait for clock.
// Occorre fare in modo che non si accumulino gli errori di sincronizzazione dello pseudo 
// clock (la scadenza del prossimo tick deve sempre essere posta a 100ms esatti dal precedente).
void SYSCALL (WAITCLOCK)

/// SYS8
// Attiva l’operazione di I/O copiando il comando (command) nel campo comando del device 
// register (*comm_device_register). Il chiamante verrà sospeso fino a completamento della
// operazione di input output; il valore di ritorno è il valore del registro di stato 
// status (che indica quindi il successo o meno dell’operazione). I terminali sono device 
// “doppi”: c’è un campo command per ricevere e uno per trasmettere (un solo processo alla
// volta accede ad uno specifico device, i processi si sincronizzano tramite sezioni critiche).
unsigned int SYSCALL (IODEVOP, unsigned int command, unsigned int *comm_device_register)

/// SYS9
// Restituisce il pid del processo stesso e del processo genitore. Se il campo pid o ppid 
// è NULL il valore corrispondente non viene restituito. Per il processo radice *ppid è NULL.
void SYSCALL(GETPIDS, void **pid, void **ppid)

/// SYS10
// Aspetta la terminazione di un processo figlio.
void SYSCALL(WAITCHLD)
