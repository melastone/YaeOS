//
//  SYSCALL.c
//  File contente l'interfaccia delle System Call
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
//  TODO: controllare il funzionamento dei doppi puntatori x il valore di ritorno
//		  di createProcess
//        fare il controllo sulla terminateRec x i processi bloccati sui semafori
//        aggiornare i tempi all'interno della semP
//        assicurarsi che le var cputime_t di ogni pcb vengano aggiornate correttamente
//        qual è la key del semaforo del timer???


#include <uARMtypes.h>
#include <uARMconst.h>
#include <libuarm.h>
#include "const.h"
#include "exception.h"
#include "syscall.h"
#include "pcb.h"
#include "asht.h"



/*********************************** Funzioni ausiliarie *********************************/

// Funzione ricorsiva ausiliaria per la terminateProcess
void terminateRec(pcb_t *proc) {

	while ((proc->p_first_child) != NULL) {
		terminateRec(removeChild(proc)) ;
	}

	// Se il processo è in esecuzione
	if (proc == curProc) {
		curProc = NULL ;
	}

	// Se il processo è bloccato su un semaforo
	if (proc->p_semKey != NULL) {
		// Se è un semaforo di un device diminuisco il numero di proc softBlocked
		// Altrimenti devo cambiare il valore del semaforo
	}

	// Se il processo è nella readyQueue
	else {
		outProcQ(&readyQueue, proc) ;
	}

	outChild(proc) ;
	freePcb(proc) ;
}

/*************************************** System Call *************************************/

/// SYS1
// Crea un nuovo processo come figlio del chiamante. 
// Se la system call ha successo il valore di ritorno è zero altrimenti è -1. 
// Se la chiamata ha successo cpid contiene l’identificatore del processo figlio
// (indirizzo del PCB).
int createProcess (state_t *statep, int priority, void **cpid) {

	pcb_t newP = allocPcb() ;

	// Se non ci sono pcb liberi la funzione deve ritornare -1
	if (newP != NULL) {

		(newP->p_priority) = priority ;

		// Setto i valori di state_t del nuovo processo come quelli del genitore
		saveCurState(statep, &(newP->p_s)) ;

		processCount++ ;

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
int terminateProcess (void *pid) {

	if (pid == NULL) {
		terminateRec(curProc) ;
		return ;
	}

	else {
		// Il pid è l'indirizzo del pcb
		pcb_t *p = (*pid) ;
		terminateRec(p) ;
		return 0 ;
	}

	return -1 ;

}


/// SYS3
// Realizza l'operazione di P su un semaforo.
// Il valore del semaforo è memorizzato nella variabile di tipo intero passata per indirizzo.
// L’indirizzo della variabile agisce da identificatore del semaforo.
void semP (int *semaddr) {

	(*semaddr)-- ;

	if ((*semAddr) < 0) {

		// TODO
		updateTime();

		// Inserisci il processo nella coda dei processi bloccati
		insertBlocked(semaddr, curProc) ;
		curProc = NULL ;

		scheduler() ;

	}
	
}

/// SYS4
// Realizza l'operazione di V su un semaforo. 
// Il valore del semaforo è memorizzato nella variabile di tipo intero passata per 
// indirizzo. L’indirizzo della variabile agisce da indentificatore del semaforo.
void semV (int *semaddr) {

	pcb_t *removedProc ;

	(*semaddr)++ ;

	if ((*semaddr) <= 0) {

		// Risveglia un processo bloccato
		removedProc = removeBlocked(semaddr) ;
		insertProcQ(&readyQueue, removedProc) ;
	}

	LDST(&(curProc->p_s)) ;
}

/// SYS5
// Registra quale handler di livello superiore debba essere attivato in caso di trap di 
// Syscall/breakpoint (type=0), TLB (type=1) o Program trap (type=2).
// Il significato dei parametri old e new è lo stesso delle aree old e new gestitre dal 
// codice della ROM: quando avviene una trap da passare al gestore lo stato del processo 
// che ha causato la trap viene posto nell’area old e viene caricato lo stato presente 
// nell’area new. La system call deve essere richiamata una sola volta per tipo.
// Se la system call ha successo restituisce 0 altrimenti -1.
int specHdl (int type, state_t *old, state_t *new)

/// SYS6
// Restituisce il valore di tre “tempi” del processo:
// – Il tempo usato dal processo in modalità user
// – Il tempo usato dal processo in modalità kernel (gestione system call e interrupt 
//   relativi al processo)
// – Il tempo trascorso dalla prima attivazione del processo.
void getTime (cputime_t *user, cputime_t *kernel, cputime_t *wallclock) {

	(*user) = (curProc->global_time) - (curProc->kernel_time) ;

	(*kernel) = (curProc->kernel_time) ;

	(*wallclock) = getTODLO() - (curProc->activation_time) ;

}

/// SYS7
// Sospende il processo fino al prossimo tick di 100ms. Lo pseudoclock produce un tick 
// ogni 100ms (esatti) e risveglia tutti i processi che hanno chiesto la wait for clock.
// Occorre fare in modo che non si accumulino gli errori di sincronizzazione dello pseudo 
// clock (la scadenza del prossimo tick deve sempre essere posta a 100ms esatti dal precedente).
void waitClock () {

	semDevices[CLOCK_SEM]-- ;
	softBlocked++ ;

	// Inserisco il processo nella coda dei bloccati sul semaforo del timer
	insertBlocked(?, curProc) ;

	curProc = NULL ;
	scheduler() ;
}

/// SYS8
// Attiva l’operazione di I/O copiando il comando (command) nel campo comando del device 
// register (*comm_device_register). Il chiamante verrà sospeso fino a completamento della
// operazione di input output; il valore di ritorno è il valore del registro di stato 
// status (che indica quindi il successo o meno dell’operazione). I terminali sono device 
// “doppi”: c’è un campo command per ricevere e uno per trasmettere (un solo processo alla
// volta accede ad uno specifico device, i processi si sincronizzano tramite sezioni critiche).
unsigned int IODevOp (unsigned int command, unsigned int *comm_device_register)

/// SYS9
// Restituisce il pid del processo stesso e del processo genitore. Se il campo pid o ppid 
// è NULL il valore corrispondente non viene restituito. Per il processo radice *ppid è NULL.
void getPids(void **pid, void **ppid) {

	if (pid) {
		
		(pcb_t *) tmp_1 = &curProc ;	
		pid = &tmp_1 ;

		// Se il processo non è radice
		if (ppid) {

			(pcb_t *) tmp_2 = &(curProc->p_parent) ;
			ppid = &tmp_2 ;
			return ;
		
		}

		return ;
	}
}

/// SYS10
// Aspetta la terminazione di un processo figlio.
void waitChild() {

}
