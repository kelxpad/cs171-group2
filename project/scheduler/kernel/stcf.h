// PHASE 1 (NEW FILE)
#ifndef _STCF_H_
#define _STCF_H_

// Project document defined constants.
#define DEFAULT_STCF_BURST 100 // default total burst time assigned to processes
#define STCF_AT 20 // constant denoting how many ticks must be spent waiting befoe a process can earn a time discount; used to calculate ERBT
#define STCF_AD 20 // constant denoting the time discount given to a process for waiting; used to calculate ERBT
#define N_GROUPS 3 // note: this will need to be set t 4 for Phase 4, 8 for Phase 5
#define DEFAULT_SHARES 100

#endif