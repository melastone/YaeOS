/*
 *	Yet Another Educational Operating System Phase 2
 *	------------------------------------------------
 *	SCHEDULER.H
 *	Scheduling Policy and Deadlock Detection Header File
 *
 *
 *	Gruppo 13:
 *	Daniel Veroli
 *  Melania Ghelli
 *	Lorenzo Turrini
 *	Lorenzo Poluzzi
 *
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <init.h>

/***************************************************************
*                           SCHEDULER                          *
***************************************************************/

void scheduler();

/***************************************************************
*                           AGING                              *
***************************************************************/

void readyQueueAging();


#endif