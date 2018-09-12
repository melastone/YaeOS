/*
 *	SCHEDULER.C
 *	Scheduling Policy and Deadlock Implementation Header File
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

void scheduler() {

	// There is no executing process 
	if (!curProc) {

		// Checking the readyQueue status
		if (readyQueue) {
			/* No More processes */
			if (processCount == 0) HALT();
			/* Processes Waiting for I/O */
			if (processCount > 0 && softBlockedCount > 0) {
				/* Enable all interrupts to be unmasked */
				setSTATUS(STATUS_ALL_INT_ENABLE(getSTATUS()));
				WAIT();
			}
			/* Deadlock Detection */
			if (processCount > 0 && softBlockedCount == 0) PANIC();

		}

		// Loading the first process in readyQueue 
		// (which is also the one with higher priority)
		if (!(curProc = removeProcQ(&readyQueue))) PANIC();

		//The PCB was taken successfully from the Queue

		// TIME MANAGMENT ( da implementare )


	}
	// There is a running process 
	else {


		// TIME MANAGMENT ( da implementare )

	}



	//Loading the state of currentProcess
	LDST(&(curProc->p_s));


}


void readyQueueAging() {


	// Checking the readyQueue status
	if (readyQueue) {
		// Init support variable
		pcb_t *parser = readyQueue;

		// Avoiding process which already have max_priority
		while (parser->p_priority == MAX_PRIORITY && parser->p_next != NULL) parser = parser->p_next;
		// Increasing priority on other processes
		while (parser->p_next != NULL) parser->p_priority++;

	}

}

