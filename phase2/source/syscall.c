/*
 *	Yet Another Educational Operating System Phase 2
 *	------------------------------------------------
 *	SYSCALL.C
 *	System Call Implementation File
 *
 *
 *	Gruppo 13:
 *	Daniel Veroli
 *  Melania Ghelli
 *	Lorenzo Turrini
 *	Lorenzo Poluzzi
 *
 */


//  TODO: controllare il funzionamento dei doppi puntatori x il valore di ritorno
//		  di createProcess
//        fare il controllo sulla terminateRec x i processi bloccati sui semafori
//        aggiornare i tempi all'interno della semP
//        assicurarsi che le var cputime_t di ogni pcb vengano aggiornate correttamente
//        quando faccio la P sul sem del timer, chi è a fare poi la V?


#include <uARMtypes.h>
#include <uARMconst.h>
#include <libuarm.h>
#include "const.h"
#include "exception.h"
#include "syscall.h"
#include "pcb.h"
#include "asht.h"



/***************************************************************
*                      FUNZIONI AUSILIARIE                     *
***************************************************************/

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



/*
	Controlla che la differenza fra il command_device_register e l'indirizzo del sub-device rientri
	nella prima di metà di indirizzi del device_register associata ai registri di RICEZIONE, o nella
	seconda metà associata invece allaa TRASMISSIONE
*/

int checkIf_RCVMODE(unsigned int *comm_device_register, int interrupt_line, int device_number) {

	if ((comm_device_register - DEV_REG_ADDR(interrupt_line,device_number)) < 0x8)
		return TRUE;
	else
		return FALSE;

}





/***************************************************************
*                         SYSTEM CALL                      	   *
***************************************************************/

/* 
	SYSTEM CALL 1 - CREATE PROCESS
	-------------------------------------------------------------------------------
  	Crea un nuovo processo come figlio del chiamante. 
 	Se la system call ha successo il valore di ritorno è zero altrimenti è -1. 
  	Se la chiamata ha successo cpid contiene l’identificatore del processo figlio
  	(indirizzo del PCB).
*/
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

		newP->activation_time = getTODLO() ;

		cpid = &(newP) ;
		return 0 ;

	}

	return -1 ;
}


/*
	SYSTEM CALL 2 - TERMINATE PROCESS
	--------------------------------------------------------------------------------------
	Termina il processo indicato (o il processo chiamante se pid==NULL) e tutta la progenie 
	del processo indicato. Ritorna 0 se l’operazione ha avuto successo, -1 in caso contrario 
	(ovviamente se pid==NULL oppure è il pid proprio o di un proprio avo, la chiamata se ha 
	successo NON ritorna).
*/
int terminateProcess (void *pid) {

	if (pid == NULL) {

		if(curProc->p_parent != NULL) {
	  		semV((int) curProc->p_parent);
	  	}

	  	terminateRec(curProc) ;
	  	return ;
 	}

 	else {
		// Il pid è l'indirizzo del pcb
		pcb_t p = (*pid) ;

		if(p->p_parent != NULL) {
		   		
		   	semV((int) p->p_parent);
		}
		
		terminateRec(p) ;
		return 0 ;
 	}

 return -1 ;

}


/*
	SYSTEM CALL 3 - P
	-------------------------------------------------------------------------------
	Realizza l'operazione di P su un semaforo.
	Il valore del semaforo è memorizzato nella variabile di tipo intero passata per indirizzo.
	L’indirizzo della variabile agisce da identificatore del semaforo.
 */
void semP (int *semaddr) {

		(*semaddr)-- ;

	if ((*semAddr) < 0) {

		// TODO
		updateTime();

		// Inserisci il processo nella coda dei processi bloccati
		insertBlocked(semaddr, curProc);
		softBlockCounter++;
		curProc = NULL;

	}
	
}


/*
	SYSTEM CALL 4 - V
	-------------------------------------------------------------------------------
	Realizza l'operazione di V su un semaforo. 
	Il valore del semaforo è memorizzato nella variabile di tipo intero passata per 
	indirizzo. L’indirizzo della variabile agisce da indentificatore del semaforo.
*/
void semV (int *semaddr) {

	pcb_t *removedProc ;

	(*semaddr)++ ;

	if ((*semaddr) <= 0) {

		// Risveglia un processo bloccato
		removedProc = removeBlocked(semaddr);
		
		if (removedProc != NULL) {
			removedProc->p_semKey = NULL;		
			softBlockCounter--;
			insertProcQ(&readyQueue, removedProc);
		}


	}

}


/*
	SYSTEM CALL 5 - SPECIFY TRAP HANDLER
	-----------------------------------------------------------------------------------
	Registra quale handler di livello superiore debba essere attivato in caso di trap di 
	Syscall/breakpoint (type=0), TLB (type=1) o Program trap (type=2).
	Il significato dei parametri old e new è lo stesso delle aree old e new gestitre dal 
	codice della ROM: quando avviene una trap da passare al gestore lo stato del processo 
	che ha causato la trap viene posto nell’area old e viene caricato lo stato presente 
	nell’area new. La system call deve essere richiamata una sola volta per tipo.
	Se la system call ha successo restituisce 0 altrimenti -1.
*/
int specHdl (int type, state_t *old, state_t *new) {

	// Controllo che i valori in ingresso siano presenti
	if (type && old && new) {

		switch (type) {
			
			/* SYS/BP Trap */
			case 0:
				/* Controllo che i registri relativi agli handler del processo corrente non siano già stati settati */
				if(curProc->old_sysBp != NULL && curProc->new_sysBp != NULL) {
					curProc->old_sysBp = old;
					curProc->new_sysBp = new;
					break; 
				} 
				else
					return -1;

			/* TLB Trap */	
			case 1:
				/* Controllo che i registri relativi agli handler del processo corrente non siano già stati settati */
				if(curProc->old_tlb != NULL && curProc->new_tlb != NULL) {
					curProc->old_tlb = old;
					curProc->new_tlb = new;
					break;
				} 
				else 
					return -1;

			/* Program Trap */
			case 2:
				/* Controllo che i registri relativi agli handler del processo corrente non siano già stati settati */
				if(curProc->old_pgm != NULL && curProc->new_pgm != NULL) {
					curProc->old_pgm = old;
					curProc->new_pgm = new;
					break; 
				} 
				else
					return -1;
			
			/* ERRORE */	
			default:
				return -1;

		}

		return 0;
	}

	// I parametri in ingresso non sono correttamente settati
	else 
		return -1;

}


/*
	SYSTEM CALL 6 - GET TIMES
	-------------------------------------------------------------------------------------
	Restituisce il valore di tre “tempi” del processo:
		– Il tempo usato dal processo in modalità user
		– Il tempo usato dal processo in modalità kernel (gestione system call e interrupt 
	  		relativi al processo)
		– Il tempo trascorso dalla prima attivazione del processo.
*/
void getTime (cputime_t *user, cputime_t *kernel, cputime_t *wallclock) {

	(*user) = (curProc->global_time) - (curProc->kernel_time) ;

	(*kernel) = (curProc->kernel_time) ;

	(*wallclock) = getTODLO() - (curProc->activation_time) ;

}


/*
	SYSTEM CALL 7 - WAIT FOR CLOCK
	------------------------------------------------------------------------------------
	Sospende il processo fino al prossimo tick di 100ms. Lo pseudoclock produce un tick 
	ogni 100ms (esatti) e risveglia tutti i processi che hanno chiesto la wait for clock.
	Occorre fare in modo che non si accumulino gli errori di sincronizzazione dello pseudo 
	clock (la scadenza del prossimo tick deve sempre essere posta a 100ms esatti dal precedente).
*/
void waitClock () {

	semDevices[CLOCK_SEM]-- ;
	softBlocked++ ;

	semP(&(semDevices[CLOCK_SEM])) ;

	curProc = NULL ;
	scheduler() ;
}


/*
	SYSTEM CALL 8 - IO OPERATION
	-------------------------------------------------------------------------------------
	Attiva l’operazione di I/O copiando il comando (command) nel campo comando del device 
	register (*comm_device_register). Il chiamante verrà sospeso fino a completamento della
	operazione di input output; il valore di ritorno è il valore del registro di stato 
	status (che indica quindi il successo o meno dell’operazione). I terminali sono device 
	“doppi”: c’è un campo command per ricevere e uno per trasmettere (un solo processo alla
	volta accede ad uno specifico device, i processi si sincronizzano tramite sezioni critiche).
*/
unsigned int ioDevop (unsigned int command, unsigned int *comm_device_register) {

	// Abilito gli interrupt 
	setSTATUS(STATUS_ALL_INT_ENABLE(getSTATUS()));

	/* Calcolo l'indirizzo dello status_device_register */
	unsigned int base_device_register = comm_device_register - DEV_REG_START;

	/* 
		Puntatore ad una struttura registro device generica, 
		contenente due campi: dtpreg_t dtp e termreg_t term per una gestione
		più semplice dei campi del device_register
	*/
	devreg_t *deviceRegister;


	/*
		Calcolo l'Interrupt Line e l'indice del device corrispondente su quella linea
	*/
	int interrupt_line = base_device_register / (N_DEV_PER_IL * DEV_REG_SIZE);

	int device_number = (base_device_register - DEV_REG_ADDR(interrupt_line,0)) / DEV_REG_SIZE;



	/*
		Calcolo l'indice del semaforo associato al device, 
		tenendo in considerazione che possa trattarsi di un terminale
		(duplice modalità RCV/TRANSM)
	*/
	int index = EXT_IL_INDEX(interrupt_line) * N_DEV_PER_IL;
	index += device_number;

	if (interrupt_line == EXT_IL_INDEX(IL_TERMINAL) && checkIf_RCVMODE(comm_device_register,interrupt_line,device_number) ) { //(comm_device_register % DEV_REG_SIZE >= 0x08)

		index += N_DEV_PER_IL;

	}


	/*
		Essendo i semafori  dei device di sincronizzazione, se il valore del semaforo è < -1
		si presuppone che esista già un processo che ha richiesto un'operazione I/O ancora in fase di
		elaborazione.
	*/

	semP(&(semDevices[index]));

	// Inializzo una variabile per contenere lo status
	UI status = 0;

	
	if (semDevices[index] >= -1){

		// Casto l'indirizzo del device register a struttura devreg_t per semplicità di utilizzo
		deviceRegister = (devreg_t*)DEV_REG_ADDR(interrupt_line, device_number);

		/*
			Controllo l'interrupt line del device:
			-	Se dovesse trattarsi di un terminale distinguo il caso
				TRASMISSIONE da quello RICEZIONE , e copio il command nel campo
				RCV relativo (transm_command/recv_command)
				ritornando quindi lo stato del sub-device corretto (transm_status/recv_status)

			-	Se dovesse trattarsi di un qualsiasi altro device generico, mi limito a copiare
				il command nel registro (dtp.command) e ritornare lo status relativo (dtp.status)

		*/

		if (interrupt_line == IL_TERMINAL) {
			
			if (checkIf_RCVMODE(comm_device_register,interrupt_line,device_number))		
				deviceRegister->term.recv_command = command;
				status = deviceRegister->term.recv_status;
			
			else {				
				deviceRegisteer->term.transm_command = command;
				status = deviceRegister->term.transm_status;

			}


		} 
		else {		
			deviceRegister->dtp.command = command;
			status = deviceRegister->dtp.status;

		}
	
	}  

	return status;

}


/*
	SYSTEM CALL 9 - GET PIDS
	-------------------------------------------------------------------------------
	Restituisce il pid del processo stesso e del processo genitore. Se il campo pid o ppid 
	è NULL il valore corrispondente non viene restituito. Per il processo radice *ppid è NULL.
*/
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


/*
	SYSTEM CALL 10 - WAIT CHILD
	-------------------------------------------------------------------------------
	Aspetta la terminazione di un processo figlio.
*/
void waitChild() {

 	if(curProc->p_first_child != NULL) {

  		semP((int) *curProc);

 	} 

 }



