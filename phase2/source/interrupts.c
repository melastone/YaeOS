/*
 *  Yet Another Educational Operating System Phase 2
 *  ------------------------------------------------
 *  INTERRUPTS.C
 *  
 *
 *
 *  Gruppo 13:
 *  Daniel Veroli
 *  Melania Ghelli
 *  Lorenzo Turrini
 *  Lorenzo Poluzzi
 *
 */

#include "pcb.h"
#include "interrupts.h"
#include "syscall.h"
#include "scheduler.h"

#include "pcb.h"

void intHandler() {
    state_t *oldState = (state_t *) INT_OLDAREA;
    if(curProc != NULL){
        curProc->userTime += (getTODLO() - getUserStart());  //memorizzo il tempo trascorso in user mode
        setKernelStart();
        (*oldState).pc -= WORD_SIZE;               /// Aggiorniamo il PC al istruzione che ha causato interrupt
        //salvataggio del nuovo stato del processo attivo 
        saveCurState(oldState, &(curProc->p_s));
    }

    uint cause = getCAUSE();

    /// Controllo Interrupt Line per capire quale device ha causato l'interrupt 
    if(CAUSE_IP_GET(cause, IL_TIMER)) timerHandler();
    else if(CAUSE_IP_GET(cause, IL_DISK)) deviceHandler(IL_DISK);
    else if(CAUSE_IP_GET(cause, IL_TAPE)) deviceHandler(IL_TAPE);
    else if(CAUSE_IP_GET(cause, IL_ETHERNET)) deviceHandler(IL_ETHERNET);
    else if(CAUSE_IP_GET(cause, IL_PRINTER)) deviceHandler(IL_PRINTER);
    else if(CAUSE_IP_GET(cause, IL_TERMINAL)) terminalHandler();

    /// chiamo lo scheduler 
    scheduler();

}

void timerHandler() {
    /***  aggiorno il clock  ***/
    setPseudoClock(FALSE);
    setisCallInterruptTimer(TRUE);
    if(getPseudoClock() >= TICK_PRIORITY){
        /***   chiamo lo scheduler e aumento la priorità dei processi in attesa ***/
        readyQueueAging();
        int numTickPriority = incTickPriority();
        if(numTickPriority == 10){
            /***  sblocco tutti i processi bloccati dalla SYS7   ***/
            while (semDevices[CLOCK_SEM] < 0) {
                ///SYS CALL V sul semaforo semDevices[CLOCK_SEM]
                semV (&(semDevices[CLOCK_SEM]));
            }
        }
        /***   resetto il clock considerando il ritardo ***/
        setPseudoClock(TRUE);
    }
    if(getCompleteTimeSlice()){
        if(curProc != NULL){
            insertProcQ(&readyQueue, curProc);
            curProc = NULL;
            //aggiorno il clock
            setPseudoClock(FALSE);
        }
    }
}

void deviceHandler(int type){
    uint *bitmapLine = (uint *) CDEV_BITMAP_ADDR(type);   //prendo la linea di interrupt della bitmpa per lo specifico device
    uint deviceNumber = getDeviceFromBitmap(bitmapLine);  //prendo il numero del device 
    devreg_t *deviceRegister = (devreg_t *) DEV_REG_ADDR(type ,deviceNumber); //prendo il registro del device

    uint indice = EXT_IL_INDEX(type) * N_DEV_PER_IL + deviceNumber;
    acknowledge(indice, deviceRegister, ACK_GEN_DEVICE);
}

void terminalHandler() {
    uint *bitmapLine = (uint *) CDEV_BITMAP_ADDR(INT_TERMINAL);
    uint terminalNumber = getDeviceFromBitmap(bitmapLine);
    devreg_t *terminalRegister = (devreg_t *) DEV_REG_ADDR(INT_TERMINAL,terminalNumber);

    uint indice = 0;

    if((terminalRegister->term.transm_status & 0x0000000F) == DEV_TTRS_S_CHARTRSM) {
        indice = EXT_IL_INDEX(INT_TERMINAL) * N_DEV_PER_IL + terminalNumber;
        acknowledge(indice, terminalRegister, ACK_TERM_TRANSMIT);
    }
    else if((terminalRegister->term.recv_status & 0x0000000F) == DEV_TRCV_S_CHARRECV) {
        indice = EXT_IL_INDEX(INT_TERMINAL) * N_DEV_PER_IL + N_DEV_PER_IL + terminalNumber;
        acknowledge(indice, terminalRegister, ACK_TERM_RECIVE);
    }
}

/***************************************************************
*                    FUNZIONI AUSILIARIE                       *
***************************************************************/
uint getDeviceFromBitmap(int * lineAddr) {
    int activeBit = 0x00000001;
    int i;

    for(int i = 0; i < 8; i++) {
        if((*lineAddr & activeBit) == activeBit) break;
        activeBit = activeBit << 1;
    }

    return i;
}

void acknowledge(uint semIndex, devreg_t *devRegister, ack_type typeAck) {
    // Quando un processo è bloccato su un device, ritorna un risultato dell'operazione e ack della fine
    if(semDevices[semIndex] < 0) {
        pcb_t *processo = headBlocked(&semDevices[semIndex]);
        switch(typeAck){
            case ACK_GEN_DEVICE:
                processo->p_s.a1 = devRegister->dtp.status;
                devRegister->dtp.command = DEV_C_ACK;
                break;
            case ACK_TERM_TRANSMIT:
                processo->p_s.a1 = devRegister->term.transm_status;
                devRegister->term.transm_command = DEV_C_ACK; 
            case ACK_TERM_RECIVE:
                processo->p_s.a1 = devRegister->term.recv_status;
                devRegister->term.recv_command = DEV_C_ACK; 
            default:
                PANIC();
                break;

        }
        //SYS CALL V sul semaforo semDevices[semIndex]
        semV (&(semDevices[semIndex]));
    }
}