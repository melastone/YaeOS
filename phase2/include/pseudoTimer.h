//
//  pseudoTimer.h
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

#ifndef PSEUDOTIMER_H
#define PSEUDOTIMER_H

#include "types.h"

void setTimer();

void setKernelStart();
uint getKernelStart();

void setUserStart();
uint getUserStart();

void initPseudoClock();
uint getPseudoClock();   
void setPseudoClock(bool reset);       

int incTickPriority();

bool getCompleteTimeSlice();

#endif