/*
 *	Yet Another Educational Operating System Phase 2
 *	------------------------------------------------
 *	INTERRUPTS.H
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

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <init.h>

/***************************************************************
*                         HANDLER                       	   *
***************************************************************/

void intHandler();

void timerHandler();

void deviceHandler(int type);

void terminalHandler();

/***************************************************************
*                    FUNZIONI AUSILIARIE                       *
***************************************************************/

uint getDeviceFromBitmap(int * lineAddr);

void acknowledge(uint semIndex, devreg_t *devRegister, ack_type typeAck);


#endif