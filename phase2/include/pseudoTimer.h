/*
 *	Yet Another Educational Operating System Phase 2
 *	------------------------------------------------
 *	PSEUDOTIMER.H
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

void setisCallInterruptTimer(bool value);
#endif