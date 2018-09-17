//
//  EXCEPTION.h
//  exception.c Header File
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

/*************************************** System Call *************************************/

/* SYS1 */
int SYSCALL (CREATEPROCESS, state t *statep, int priority, void **cpid)

/* SYS2 */
int SYSCALL (TERMINATEPROCESS, void * pid)

/* SYS3 */
void SYSCALL (SEMP, int *semaddr)

/* SYSC4 */
void SYSCALL (SEMV, int *semaddr)

/* SYS5 */
int SYSCALL (SPECHDL, int type, state_t *old, state_t *new)

/* SYS6 */
void SYSCALL (GETTIME, cputime_t *user, cputime_t *kernel, cputime_t *wallclock)

/* SYS7 */
void SYSCALL (WAITCLOCK)

/* SYS8 */
unsigned int SYSCALL (IODEVOP, unsigned int command, unsigned int *comm_device_register)

/* SYS9 */
void SYSCALL(GETPIDS, void **pid, void **ppid)

/* SYS10 */
void SYSCALL(WAITCHLD)
