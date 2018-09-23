/*
 *	Yet Another Educational Operating System Phase 2
 *	------------------------------------------------
 *	SYSCALL.H
 *  System Call Header File
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

#include <init.h>


/***************************************************************
*                         SYSTEM CALL                      	   *
***************************************************************/

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
unsigned int IODevOp (unsigned int command, unsigned int *comm_device_register)

/* SYS9 */
void getPids(void **pid, void **ppid) ;

/* SYS10 */
void waitChild() ;


/***************************************************************
*                      FUNZIONI AUSILIARIE                     *
***************************************************************/

int checkIf_RCVMODE(unsigned int *comm_device_register, int interrupt_line, int device_number);

void terminateRec(pcb_t *proc);