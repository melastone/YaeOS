/*
 *	Yet Another Educational Operating System Phase 2
 *	------------------------------------------------
 *	INIT.H
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

#ifndef INIT_H
#define INIT_H 

#include <uARMconst.h>
#include <uARMtypes.h>
#include <libuarm.h>
#include "pcb.h"
#include "asht.h"
#include "types.n"




/***************************************************************
*                        FUNZIONI ESTERNE                      *
***************************************************************/

EXTERN void test();

/***************************************************************
*                        FUNZIONI AUSILIARIE                   *
***************************************************************/

void initArea(memaddr area, memaddr handler);

#endif