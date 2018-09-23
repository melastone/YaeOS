/*
 *	Yet Another Educational Operating System Phase 2
 *	------------------------------------------------
 *	EXCEPTION.H
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


#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <init.h>
#include <syscall.h>

/***************************************************************
*                           HANDLERS                           *
***************************************************************/

void sysHandler();

void pgmHandler();

void tlbHandler();


/***************************************************************
*                      FUNZIONI AUSILIARIE                     *
***************************************************************/


unsigned int checkSysBpHandler();

unsigned int checkTLBHandler();

unsigned int checkPGMHandler();




#endif