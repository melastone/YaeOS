/*
 *	Yet Another Educational Operating System Phase 2
 *	------------------------------------------------
 *	EXCEPTION.C
 *	Scheduling Policy and Deadlock Detection Implementation File
 *
 *
 *	Gruppo 13:
 *	Daniel Veroli
 *  Melania Ghelli
 *	Lorenzo Turrini
 *	Lorenzo Poluzzi
 *
 */


#include <scheduler.h>



/***************************************************************
*                           SCHEDULER                          *
***************************************************************/

void scheduler() {

	// Controllo l'eventuale presenza di processi in esecuzione 
	if (curProc == NULL) {

		// Controllo lo stato della readyQueue
		if (readyQueue == NULL) {
			/* Non ci sono più processi */
			if (processCounter == 0) HALT();
			/* Processi in atesa di I/O */
			if (processCounter > 0 && softBlockedCounter > 0) {
				/* Riabilito tutti gli interrupt */ 
				setSTATUS(STATUS_ALL_INT_ENABLE(getSTATUS()));
				WAIT();
			} 
			/* Controllo possibile situazione di Deadlock (processCount > 0 && softBlockedCount == 0) */
			else {
				PANIC();
			}
		}
		// Carico il primo processo dalla readyQueue 
		// (che coincide con quello a priorità più alta)
		curProc = removeProcQ(&readyQueue);
		//setto il tempo di inizo del processo 
		if(curProc->time == 0){
			curProc->time = getTODLO();
		}
		setPseudoClock(FALSE);
	}
	// Esiste un processo attualmente in esecuzione
	else {
		curProc->kernelTime += getTODLO() - kernelMode_Start;
		setPseudoClock(FALSE);
	}

	// setto il valore del interval timer 
	setTimer();
	isCallInterruptTimer(FALSE);
	setUserStart();
	//CONTEXT SWITCH
	//Carico lo stato del processo corrente nei registri
	LDST(&(curProc->p_s));


}


/***************************************************************
*                           AGING                              *
***************************************************************/


void readyQueueAging() {


	// Controllo lo stato della readyQueue
	if (readyQueue) {
		// Variabile di supporto
		pcb_t *parser = readyQueue;

		// Scorro i processi che hanno già priorità massima
		while (parser->p_priority == MAXPRIO && parser->p_next != NULL) parser = parser->p_next;
		// Aumento la priorità degli altri processi
		while (parser->p_next != NULL) {
			parser->p_priority++;
			parser = parser->p_next;
		}
	}

}

