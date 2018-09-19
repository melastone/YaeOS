state_t *tlb_old = (state_t*)TLB_OLDAREA;
state_t *pgmtrap_old = (state_t*)PGMTRAP_OLDAREA;
state_t *sysbp_old = (state_t*)SYSBK_OLDAREA;

void sysHandler() {

	//siamo nel caso in cui viene chiamata una SYSCALL, ora andiamo a vedere in che mode siamo
	if (CAUSE_EXCCODE_GET(sysbp_old->CP15_Cause) == EXC_SYSCALL && sysbp_old->a1 >= 1 && sysbp_old->a1 <= 10) {

		//controllo se siamo in USER MODE
		//ricordiamo & bit a bit
		if(curProc->p_s.cpsr == STATUS_USER_MODE){
			saveCurState(sysbp_old, pgmtrap_old);
			pgmtrap_old->CP15_Cause = CAUSE_EXCCODE_SET(pgmtrap_old->CP15_Cause, EXC_RESERVEDINSTR);
			pgmHandler();
		}
		//controllo se siamo in SYSTEM MODE (KERNEL) 
		//ricordiamo & bit a bit
		else if(curProc->p_s.cpsr == STATUS_SYS_MODE) {
			saveCurState(sysbp_old, curProc->p_s);
			//chiamare handler per SYSCALL
			uint a1 = sysbp_old->a1;
			uint a2 = sysbp_old->a2;
			uint a3 = sysbp_old->a3;
			uint a4 = sysbp_old->a4;

			// Invoke the appropriate SYSCALL by analyzing its value contiained in A1 register
			if (sysbp_old->CP15_Cause == EXC_SYSCALL) {
				switch (a1) {
					//fare casi SYSCALL
					case CREATEPROCESS:
						createProcess(a2, a3, a4);
						break;
					case TERMINATEPROCESS:
						terminateProcess(a2);
						break;
					case P:
						semP(a2);
						break;
					case V:
						semV(a2);
						break;
					case SPECHDL:
						specHdl(a2, a3, a4);
						break;
					case GETTIME:
						getTime(a2, a3, a4);
						break;
					case WAITCLOCK:
						waitClock();
						break;
					case IODEVOP:
						ioDevop(a2, a3);
						break;
					case GETPIDS:
						getPids(a2, a3);
						break;
					case WAITCHLD:
						waitChld();
						break;
					default:
						PANIC();
				}
			}
			
			scheduler();
		}

	} else {
		if(CAUSE_EXCCODE_GET(sysbp_old->CP15_Cause) == EXC_SYSCALL && sysbp_old->a1 >= 11){
			if(sysbp_old->a2 >= 0 && sysbp_old->a2 <= 2){
				specHdl(sysbp_old->a2, SYSBK_OLDAREA, (state_t*)SYSBK_NEWAREA);
			} else {
				terminateProcess(NULL);
			}
		}
	}

}

void pgmHandler(){
	if(sysbp_old->a2 == 2){
		spechdl(sysbp_old->a2, PGMTRAP_OLDAREA, (state_t*)PGMTRAP_NEWAREA);
	} else {
		terminateProcess(NULL);
	}
}

void tlbHandler() {
	if(sysbp_old->a2 == 1){
		spechdl(sysbp_old->a2, TLB_OLDAREA, (state_t*)TLB_NEWAREA);
	} else {
		terminateProcess(NULL);
	}
}

void saveCurState(state_t *state, state_t *newState) {
	newState->a1 = state->a1;
	newState->a2 = state->a2;
	newState->a3 = state->a3;
	newState->a4 = state->a4;
	newState->v1 = state->v1;
	newState->v2 = state->v2;
	newState->v3 = state->v3;
	newState->v4 = state->v4;
	newState->v5 = state->v5;
	newState->v6 = state->v6;
	newState->sl = state->sl;
	newState->fp = state->fp;
	newState->ip = state->ip;
	newState->sp = state->sp;
	newState->lr = state->lr;
	newState->pc = state->pc;
	newState->cpsr = state->cpsr;
	newState->CP15_Control = state->CP15_Control;
	newState->CP15_EntryHi = state->CP15_EntryHi;
	newState->CP15_Cause = state->CP15_Cause;
	newState->TOD_Hi = state->TOD_Hi;
	newState->TOD_Low = state->TOD_Low;
}