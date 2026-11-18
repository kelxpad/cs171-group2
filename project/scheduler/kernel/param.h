#define NPROC        64  // maximum number of processes
#define NCPU          8  // maximum number of CPUs
#define NOFILE       16  // open files per process
#define NFILE       100  // open files per system
#define NINODE       50  // maximum number of active i-nodes
#define NDEV         10  // maximum major device number
#define ROOTDEV       1  // device number of file system root disk
#define MAXARG       32  // max exec arguments
#define MAXOPBLOCKS  10  // max # of blocks any FS op writes
#define LOGSIZE      (MAXOPBLOCKS*3)  // max data blocks in on-disk log
#define NBUF         (MAXOPBLOCKS*3)  // size of disk block cache
#define FSSIZE       2000  // size of file system in blocks
#define MAXPATH      128   // maximum file path name
#define USERSTACK    1     // user stack pages


// PHASE 1
#include "stcf.h"

#if defined(SCHED_POLICY_RR)
#define SCHEDULING_POLICY "RR"
#elif defined(SCHED_POLICY_STCF)
#define SCHEDULING_POLICY "STCF"
#elif defined(SCHED_POLICY_GROUP_STCF)
#define SCHEDULING_POLICY "GROUP_STCF"
#else
#define SCHEDULING_POLICY "RR"
#endif