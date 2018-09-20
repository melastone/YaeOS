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

/***************************************************************
*                           HANDLERS                           *
***************************************************************/

void sysHandler();

void pgmHandler();

void tlbHandler();


/***************************************************************
*                      AUXILIARY FUNCTION                      *
***************************************************************/


void saveCurState(state_t *state, state_t *newState);



#endif
