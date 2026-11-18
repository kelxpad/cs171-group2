#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(n < 0) {
    if(shrinkproc(-n) < 0)
      return -1;
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_shutdown(void)
{
  (*(volatile uint32 *) 0x100000) = 0x5555;
  return 0;
}

// PHASE 1: Add schedlog and setburst calls.
uint64
sys_schedlog(void)
{
  int duration;

  argint(0, &duration);

  if(duration <= 0) {
    return -1;
  }
  
  schedlog_start(duration);
  return 0;
}

uint64
sys_setburst(void)
{
  int burst;
  struct proc *p = myproc();

  argint(0, &burst);

  // Invalid burst: mark the process as killed.
  if(burst <= 0) {
    setkilled(p);
    return -1;
  }

  // Phase 1: Valid setburst has no effect under vanilla RR (CL-03/CL-19).
#if defined(SCHED_POLICY_STCF) || defined(SCHED_POLICY_GROUP_STCF)
  // Phase 1: Update the process's burst-time budget.
  p->total_burst = burst;
  p->remaining_burst = burst;

  // Phase 1: Reset the number of ticks consumed.
  p->ticks_consumed = 0;

  // Phase 1: Always yield after a valid STCF burst update (CL-13).
  yield();
#endif

  return 0;
}

