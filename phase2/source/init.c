/*
 *	Yet Another Educational Operating System Phase 2
 *	------------------------------------------------
 *	INIT.C
 *	Kernel Entry Point Header File
 *
 *
 *	Gruppo 13:
 *	Daniel Veroli
 *  Melania Ghelli
 *	Lorenzo Turrini
 *	Lorenzo Poluzzi
 *
 */


#include <init.h>


/***************************************************************
*                        KERNEL MAIN ENTRY                 	   *
***************************************************************/


int main() {
	
	//-------------------------------------------------------

	/*  
		Inizializzazione delle aree NEW con gli indirizzi che puntano
		alle routine di gestione delle Trap 
	*/
	
	initArea(INT_NEWAREA,(memaddr)intHandler);
	initArea(TLB_NEWAREA,(memaddr)tlbHandler);
	initArea(PGMTRAP_NEWAREA,(memaddr)pgmHandler);
	initArea(SYSBK_NEWAREA,(memaddr)sysHandler);

	// Inizializzazione strutture dati Phase 1
	initPcbs();
	initASL();

	// Inizializzazione variabili del Kernel	
	processCounter = 0;
	softBlockCounter = 0;
	curProc = NULL;

	// Inizializzazione dei Semafori (?)
	for (i = 0; i < MAX_DEVICES; i++) semDevices[i] = 0;

	//-------------------------------------------------------

	// Creazione PCB del primo processo 
	if ((firstProcess = allocPcb()) == NULL) PANIC();
	
	// Abilitazione degli Interrupt
	(firstProcess->p_s).cpsr = STATUS_ALL_INT_ENABLE((firstProcess->p_s).cpsr);	
	// Set priorità del processo a 0
	firstProcess->p_priority = 0;
	// Memoria virtuale disabilitata
	(firstProcess->p_s).CP15_Control = CP15_DISABLE_VM((firstProcess->p_s).CP15_Control);
	// Kernel Mode Abilitato
	(firstProcess->p_s).cpsr &= STATUS_CLEAR_MODE;
	(firstProcess->p_s).cpsr |= STATUS_SYS_MODE;
	// Set Stack Pointer
	(firstProcess->p_s).sp = RAM_TOP - FRAMESIZE;
	// Inizializzazione del Program Counter all'indirizzo del test
	(firstProcess->p_s).pc = (memaddr)test;

	//-------------------------------------------------------

	//Inserisco il primo processo nella readyQueue e chiamo lo Scheduler
	insertProcQ(&readyQueue, firstProcess);
	processCounter++;

	//TEMPO PROCESSO
	initPseudoClock();
	setisCallInterruptTimer(true);
	setUserStart();
	scheduler();

	//-------------------------------------------------------

	//Non dovrebbe arrivare quì
	PANIC();

	return 0;
}



/***************************************************************
*                        FUNZIONI AUSILIARIE                   *
***************************************************************/


void initArea(memaddr area, memaddr handler) {

	state_t* newArea = (state_t*)area;
	// Salva lo stato corrente della CPU in "newArea"
	STST(newArea);
	// Set del Program Counter all'indirizzo della routine di gestione
	newArea->pc = handler;
	// Set dello Stack Pointer a RAMTOP
	newArea->sp = RAM_TOP;
	// Set del Current Program State Register affinchè siano mascherati tutti gli Interrupt e venga eseguito in Kernel Mode
	newArea->cpsr = STATUS_NULL | STATUS_SYS_MODE;
	newArea->cpsr = STATUS_ALL_INT_DISABLE(newArea->cpsr);

}