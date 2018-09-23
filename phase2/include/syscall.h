//
//  SYSCALL.h
//  syscall.c Header File
//
//  Yet Another Educational Operating System Phase 2
//
//  Creato il 17/09/2018
//
//  Gruppo 13:
//  Daniel Veroli
//  Lorenzo Turrini
//  Lorenzo Poluzzi
//  Melania Ghelli
//

#include <uARMtypes.h>
#include "types.h"

/*********************************** Funzioni ausiliarie *********************************/

void terminateRec(pcb_t *proc)

/*************************************** System Call *************************************/

/* SYS1 */
int createProcess (state_t *statep, int priority, void **cpid) ;

/* SYS2 */
int terminateProcess (void *pid) ;

/* SYS3 */
void semP (int *semaddr) ;

/* SYSC4 */
void semV (int *semaddr) ;

/* SYS5 */
int specHdl (int type, state_t *old, state_t *new) ;

/* SYS6 */
void getTime (cputime_t *user, cputime_t *kernel, cputime_t *wallclock) ;

/* SYS7 */
void waitClock () ;

/* SYS8 */
unsigned int ioDevop (unsigned int command, unsigned int *comm_device_register)

/* SYS9 */
void getPids(void **pid, void **ppid) ;

/* SYS10 */
void waitChild() ;