//
//  pseudoTimer.c
//  Yet Another Educational Operating System Phase 2
//
//  Creato il 29/08/2018
//
//  Gruppo 13:
//  Daniel Veroli
//  Lorenzo Turrini
//  Lorenzo Poluzzi
//  Melania Ghelli
//

#include "pseudoTimer.h"
#include "const.h"
#include "types.h"


/// Variabili per gestire il tempo 
uint kernelMode_Start;              /// Valore di inizio di una operazione in kernel mode
uint userMode_Start;                /// Valore di inizio di una operazione in user mode
uint clock;                         /// Valore totale dello pseudo timer
uint clockTick;                     /// Intervallo di tempo per il prossimo evento 
int numTickPriority = 0;            /// Numero di Tick per l'aumento della priorità
uint quantomPart2 = 0;              /// Valore del TIME SLICE rimanente
bool completeTimeSlice = true;      /// Indica se il TIME SLICE è stato completato

/// setta il valore del Invertal Timer
void setTimer(){
    if(!completeTimeSlice){
        completeTimeSlice = true;
        setTIMER(quantomPart2);
    }else{
        if((clock + TIME_SLICE) > TICK_PRIORITY){
            quantomPart2 = (clock + TIME_SLICE) - TICK_PRIORITY;
            completeTimeSlice = false;
            setTIMER(TIME_SLICE - quantomPart2);
        }else{
            quantomPart2 = 0;
            completeTimeSlice = true;
            setTIMER(TIME_SLICE);
        }
    } 
}

/// Aggiorna(reset = false) e resetta(reset = true) lo psuedo clock e valorizza il tempo 
/// di inizio del prossimo intervallo 
void setPseudoClock(bool reset){
    if(reset){
        clock = clock - TICK_PRIORITY;
    }else{
        clock += getTODLO() - clockTick;
    }
    clockTick = getTODLO();
}

uint getPseudoClock(){
    return clock;
}

int incTickPriority(){
    if(numTickPriority < 10){
        numTickPriority++;
    }else{
        numTickPriority = 0;
    }
    return numTickPriority;
}

bool getCompleteTimeSlice(){
    return completeTimeSlice;
}

void initPseudoClock(){
    clock = 0;
    clockTick = getTODLO();
}

void setKernelStart(){
    kernelMode_Start = getTODLO();
}

void setUserStart(){
    userMode_Start = getTODLO()
}

uint getKernelStart(){
    return kernelMode_Start;
}

uint getUserStart(){
    return userMode_Start;
}