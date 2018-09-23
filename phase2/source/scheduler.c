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


#include "pseudoTimer.h"
#include "init.h"
#include "scheduler.h"
#include "const.h"



/***************************************************************
*                           SCHEDULER                          *
***************************************************************/


/*
	SCHEDULER
	Si occupa di regolare l'avvicendamento dei processi nella CPU: ogni processo ha un time-slice massimo di 3 ms 
	e questi vengono prelevati dalla readyQueue sulla base della loro priorità. 
	A tal proposito per far sì che tutti i processi arrivino poi ad aggiudicarsi un posto nel processore,
	ogni 10 ms viene lanciata una funzione di aging (readyQueueAging()) che si occupa di incrementare di 1 
	tutte le priorità dei processi nella readyQueue (purché questi non abbiano già priorità massima 10).
	Lo scheduler comprende inoltre, un meccanismo di deadlock detection per prevenire e gestire nella maniera 
	più opportuna possibili situazioni di stallo.
*/



void scheduler() {

	// Controllo l'eventuale presenza di processi in esecuzione 
	if (curProc == NULL) {

		// Controllo lo stato della readyQueue
		if (readyQueue == NULL) {
			/* Non ci sono più processi */
			if (processCounter == 0) HALT();
			/* Processi in atesa di I/O */
			if (processCounter > 0 && softBlockCounter > 0) {
				/* Riabilito tutti gli interrupt */ 
				setSTATUS(STATUS_ALL_INT_ENABLE(getSTATUS()));
				WAIT();
			} 
			/* Controllo possibile situazione di Deadlock (processCount > 0 && softBlockedCount == 0) */
			else {
				PANIC();
			}
		}

		/*
			Carico il primo processo dalla readyQueue (che coincide con quello a priorità più alta) 
			e risetto la sua priorità a quella con cui era stato creato
		*/

		curProc = removeProcQ(&readyQueue);
		curProc->p_priority = curProc->base_priority;
		//setto il tempo di inizo del processo 
		if(curProc->time == 0){
			curProc->time = getTODLO();
		}
		setPseudoClock(FALSE);
	}
	// Esiste un processo attualmente in esecuzione
	else {
		curProc->kernelTime += getTODLO() - getKernelStart();
		setPseudoClock(FALSE);
	}

	// setto il valore del interval timer 
	setTimer();
	setisCallInterruptTimer(FALSE);
	setUserStart();
	//CONTEXT SWITCH
	//Carico lo stato del processo corrente nei registri
	LDST(&(curProc->p_s));


}


/***************************************************************
*                           AGING                              *
***************************************************************/

/*
	Funzione di aging per la readyQueue.
*/

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

