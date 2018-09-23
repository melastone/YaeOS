/*
 *	Yet Another Educational Operating System Phase 2
 *	------------------------------------------------
 *	EXCEPTION.C
 *	
 *
 *
 *	Gruppo 13:
 *	Daniel Veroli
 *  Melania Ghelli
 *	Lorenzo Turrini
 *	Lorenzo Poluzzi
 *
 */


#include <exception.h>


/*
	Creazione variabili di appoggio definite nella libreria uArm per comodità di accesso ai registri
	a1, a2, a3, a4 in cui saranno contenuti i parametri che poi verranno passati alle SYSCALL.
*/

state_t *tlb_old = (state_t*)TLB_OLDAREA;
state_t *pgmtrap_old = (state_t*)PGMTRAP_OLDAREA;
state_t *sysbp_old = (state_t*)SYSBK_OLDAREA;


/***************************************************************
*                           HANDLERS                           *
***************************************************************/

/* 
	Handler per le SYSCALL. 
	Intercetta le eccezioni generate dalle SYSCALL e verifica che il processo corrente sia in KERNEL MODE. 
	Successivamente, esegue la System Call relativa al valore salvato nel registro di stato a1.
	In caso contrario richiama il Program Trap Handler che si occuperà di inviare la richiesta
	a un gestione superiore se presente o eventualmente terminare l'esecuzione del processo.
*/

void sysHandler() {

	//controllo che sia stata richiesta l'esecuzione di una SYSCALL (SYS 1 - SYS 10)
	if (CAUSE_EXCCODE_GET(sysbp_old->CP15_Cause) == EXC_SYSCALL && sysbp_old->a1 >= 1 && sysbp_old->a1 <= 10) {

		//controllo se la modaltà di esecuzione è USER MODE
		if((curProc->p_s.cpsr & STATUS_USER_MODE) == STATUS_USER_MODE) {
			copyState(sysbp_old, pgmtrap_old);
			pgmtrap_old->CP15_Cause = CAUSE_EXCCODE_SET(pgmtrap_old->CP15_Cause, EXC_RESERVEDINSTR);
			pgmHandler();
		}
		
		//controllo che la modalità di esecuzione sia SYSTEM MODE (KERNEL) 
		else if((curProc->p_s.cpsr & STATUS_SYS_MODE) == STATUS_SYS_MODE) {
			copyState(sysbp_old, curProc->p_s);
			curProc->userTime += getTODLO() - getUserStart();
			setKernelStart();
			//creo variabili relative ai registri di stato a1 ... a4
			uint a1 = sysbp_old->a1;
			uint a2 = sysbp_old->a2;
			uint a3 = sysbp_old->a3;
			uint a4 = sysbp_old->a4;

			// Eseguo la SYSCALL correlativa al valore salvato nel registro di stato a1. 
			if (sysbp_old->CP15_Cause == EXC_SYSCALL) {
				switch (a1) {
					//casi SYSCALL
					case CREATEPROCESS:
						createProcess->p_s.a1 = createProcess(a2, a3, a4);
						break;
					case TERMINATEPROCESS:
						createProcess->p_s.a1 = terminateProcess(a2);
						break;
					case P:
						semP(a2);
						break;
					case V:
						semV(a2);
						break;
					case SPECHDL:
						createProcess->p_s.a1 = specHdl(a2, a3, a4);
						break;
					case GETTIME:
						getTime(a2, a3, a4);
						break;
					case WAITCLOCK:
						waitClock();
						break;
					case IODEVOP:
						createProcess->p_s.a1 = ioDevop(a2, a3);
						break;
					case GETPIDS:
						getPids(a2, a3);
						break;
					case WAITCHLD:
						waitChld();
						break;
					default:
						PANIC();
				}

				scheduler();

			} else 
				PANIC();
		}

	} 
	//Controllo che sia stata richiesta l'esecuzione di una SYSCALL e che questa sia maggiore di 11 (SYS > 11)
	else if(CAUSE_EXCCODE_GET(sysbp_old->CP15_Cause) == EXC_SYSCALL && sysbp_old->a1 >= 11) {
		/* 
		Controllo se è presente un gestore di livello superiore, se presente carico l'indirizzo 
		relativo alla routine di livello superiore, altrimenti termino il processo
		che ha causato l'eccezione.
		*/

			if(checkSysBpHandler()) {
				curProc->userTime += getTODLO() - getUserStart();
				setKernelStart();
				copyState(sysbp_old, curProc->old_sysBp);
				copyState(curProc->new_sysBp, &(currentProcess->p_s));
			} else {
				terminateProcess (NULL);
				scheduler();
			}
			 
	}
	// Controllo se la l'eccezione è di tipo BREAKPOINT 
	else {
			
		if(CAUSE_EXCCODE_GET(sysbp_old->CP15_Cause) == EXC_BREAKPOINT) {

			if(checkSysBpHandler()) {
				curProc->userTime += getTODLO() - getUserStart();
				setKernelStart();
				copyState(sysbp_old, curProc->old_sysBp);
				copyState(curProc->new_sysBp, &(currentProcess->p_s));
			} 
			else {
				terminateProcess (NULL);
				scheduler(); 
			}	
			
		}
		else
			PANIC();

	}

}

/*
	Handler per Program Trap Exception. 
	Controlla all'interno delle aree di memoria "NEW" relative al processo, 
	per verificare se è stato specificato o meno un gestore di livello superiore.
	In caso negativo termina il processo tramite SYSCALL 2 e richiama lo scheduler.
*/

void pgmHandler() {

	if(checkPGMHandler()) {
		curProc->userTime += getTODLO() - getUserStart();
		setKernelStart();
		copyState(pgmtrap_old, curProc->old_pgm);
		copyState(curProc->new_pgm, &(currentProcess->p_s));
	}
	else {
		terminateProcess(NULL);
		scheduler();
	}

}


/*
	Handler per TLB Exception. 
	Controlla all'interno delle aree di memoria "NEW" relative al processo, 
	per verificare se è stato specificato o meno un gestore di livello superiore.
	In caso negativo termina il processo tramite SYSCALL 2 e richiama lo scheduler.
*/
void tlbHandler() {

		if(checkTLBHandler()) {	
			curProc->userTime += getTODLO() - getUserStart();
			setKernelStart();
			copyState(tlb_old, curProc->old_tlb); 
			copyState(curProc->new_tlb,&(currentProcess->p_s));
		}
		else {
			terminateProcess(NULL);
			scheduler();
		}


}



/***************************************************************
*                      FUNZIONI AUSILIARIE                     *
***************************************************************/

/*
	Controllo routine per passup.
	Funzione di controllo per routine di gestione di livello superiore. Se è stata specificata 
	una routine di gestione di livello superiore per il tipo di eccezione sollevata
	ritornano 0 (TRUE), altrimenti 1 (FALSE)
*/

unsigned int checkSysBpHandler() {
	if(curProc->old_sysBp != NULL && curProc->new_sysBp != NULL)
		return 1;
	else 
		return 0;
}


unsigned int checkTLBHandler() {
		if(curProc->old_tlb != NULL && curProc->new_tlb != NULL)
		return 1;
	else 
		return 0;
}

unsigned int checkPGMHandler() {
		if(curProc->old_pgm != NULL && curProc->new_pgm != NULL)
		return 1;
	else 
		return 0;
}