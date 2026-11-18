/ Phase 1

## Group 11 - `SYS_getpid`

- Aaron Xander H. Antazo   | 2024-02208 | THY/FVW1
- Wigbert Amadeus N. Madla | 2024-06911 | THX/FRU2
- Renz Sergio T. Pactolin  | 2024-13690 | THY/FVW3
- Klex Felouise G. Padilla | 2024-00561 | THY/FVW1
- Hanz Josef O. Vicencio   | 2024-02172 | THY/FVW1

## Changes

### Summary

- set `CPUS` parameter default value to 1
- set up `SCHEDULING_POLICY` environment variable logic (`Makefile`)
- set up `SCHEDULING_POLICY` compilation flag logic (`Makefile`)
- set up `SCHEDULING_POLICY` parameter logic (`kernel/param.h`)
- add "Using Policy:" message to `xv6` startup (`kernel/main.c`)
- register `schedlog` and `setburst` syscalls (`kernel/syscall.{c,h}`)
- add `schedlog_start_tick` and `schedlog_duration` global variables (`kernel/proc.c`)
- add `schedlog_lock` for `schedlog`-related global variables (`kernel/proc.c`)
- modify `procinit` to initialize `schedlog_lock` (`kernel/proc.c`)
- implement `schedlog_start` and `schedlog_print` helper functions (`kernel/proc.c`)
- implement `schedlog` (using helper functions) and `setburst` syscalls (`kernel/sysproc.c`)
- keep valid `setburst` calls without effect under vanilla RR, while retaining invalid-input handling across policies and unconditional yielding after valid STCF updates (forum clarifications CL-03, CL-19, CL-13)
- add user-space wrapper for `setburst` and `schedlog` syscalls (`user/{user.h, usys.pl}`)
- define STCF policy constants as per project specs (`kernel/stcf.h`)
- add STCF policy tracking values to PCB `struct proc` (`kernel/proc.h`)
- modify `allocproc` to initialize new tracking values in PCB (`kernel/proc.c`)
- implement `account_cpu_tick` for tracking burst consumption (`kernel/proc.c`)
- skip CPU tick accounting for processes already marked killed (forum clarification CL-38)
- register `account_cpu_tick` and `schedlog_start` signatures (`kernel/defs.h`)
- set up policy-dependent logic in `scheduler` related to selected scheduling policy (`kernel/proc.c`)
- implment STCF scheduler with `schedlog_print` functionality (`kernel/proc.c`)
- set up policy-dependent logic in `usertrap` and `kerneltrap` related to `yield`ing (`kernel/trap.c`)
- on burst exhaustion in `kerneltrap`, yield immediately and defer termination to `usertrap` after the interrupted syscall resumes (forum clarification CL-20)
- implement `schedlogtest` program (`user/schedlogtest.c`)
- implement `setbursttest` program (`user/setbursttest.c`)
- register `schedlogtest` and `setbursttest` user programs (`Makefile`)

### Exhaustive Change List

#### `Makefile`

```diff
diff --git a/Makefile b/Makefile
index 75b96cd..8037368 100644
--- a/Makefile
+++ b/Makefile
@@ -1,6 +1,19 @@
 K=kernel
 U=user
 
+# PHASE 1: Select Scheduling policy: RR, STCF, or GROUP_STCF
+SCHEDULING_POLICY ?= RR
+
+ifeq ($(SCHEDULING_POLICY),RR)
+POLICY_CFLAGS = -DSCHED_POLICY_RR
+else ifeq ($(SCHEDULING_POLICY),STCF)
+POLICY_CFLAGS = -DSCHED_POLICY_STCF
+else ifeq ($(SCHEDULING_POLICY),GROUP_STCF)
+POLICY_CFLAGS = -DSCHED_POLICY_GROUP_STCF
+else
+$(error Unknown SCHEDULING_POLICY '$(SCHEDULING_POLICY)'. Use RR, STCF, or GROUP_STCF)
+endif
+
 OBJS = \
   $K/entry.o \
   $K/start.o \
@@ -73,6 +86,7 @@ CFLAGS += -fno-builtin-printf -fno-builtin-fprintf -fno-builtin-vprintf
 CFLAGS += -I.
 CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)
 CFLAGS += -Wno-unused-but-set-variable
+CFLAGS += $(POLICY_CFLAGS)
 
 # Disable PIE when possible (for Ubuntu 16.10 toolchain)
 ifneq ($(shell $(CC) -dumpspecs 2>/dev/null | grep -e '[^f]no-pie'),)
@@ -141,6 +155,8 @@ UPROGS=\
 	$U/_wc\
 	$U/_zombie\
 	$U/_shutdown\
+	$U/_setbursttest\
+	$U/_schedlogtest\
 
 fs.img: mkfs/mkfs README $(UPROGS)
 	mkfs/mkfs fs.img README $(UPROGS)
@@ -162,7 +178,7 @@ QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
 	then echo "-gdb tcp::$(GDBPORT)"; \
 	else echo "-s -p $(GDBPORT)"; fi)
 ifndef CPUS
-CPUS := 3
+CPUS := 1
 endif
 
 QEMUOPTS = -machine virt -bios none -kernel $K/kernel -m 128M -smp $(CPUS) -nographic
```

#### `kernel/defs.h`

```diff
diff --git a/kernel/defs.h b/kernel/defs.h
index aef7718..7b33522 100644
--- a/kernel/defs.h
+++ b/kernel/defs.h
@@ -101,6 +101,8 @@ void            yield(void);
 int             either_copyout(int user_dst, uint64 dst, void *src, uint64 len);
 int             either_copyin(void *dst, int user_src, uint64 src, uint64 len);
 void            procdump(void);
+int             account_cpu_tick(struct proc *); // PHASE 1 MODIFICATION
+void            schedlog_start(int duration); // PHASE 1 MODIFICATION
 
 // swtch.S
 void            swtch(struct context*, struct context*);
```

#### `kernel/main.c`

```diff
diff --git a/kernel/main.c b/kernel/main.c
index f0d3171..5a956ca 100644
--- a/kernel/main.c
+++ b/kernel/main.c
@@ -13,6 +13,7 @@ main()
   if(cpuid() == 0){
     consoleinit();
     printfinit();
+    printf("Using Policy: %s\n", SCHEDULING_POLICY);
     printf("\n");
     printf("xv6 kernel is booting\n");
     printf("\n");
```

#### `kernel/param.h`

```diff
diff --git a/kernel/param.h b/kernel/param.h
index 80ec6d3..70560d3 100644
--- a/kernel/param.h
+++ b/kernel/param.h
@@ -13,3 +13,16 @@
 #define MAXPATH      128   // maximum file path name
 #define USERSTACK    1     // user stack pages
 
+
+// PHASE 1
+#include "stcf.h"
+
+#if defined(SCHED_POLICY_RR)
+#define SCHEDULING_POLICY "RR"
+#elif defined(SCHED_POLICY_STCF)
+#define SCHEDULING_POLICY "STCF"
+#elif defined(SCHED_POLICY_GROUP_STCF)
+#define SCHEDULING_POLICY "GROUP_STCF"
+#else
+#define SCHEDULING_POLICY "RR"
+#endif
\ No newline at end of file
```

#### `kernel/proc.c`

```diff
diff --git a/kernel/proc.c b/kernel/proc.c
index b6ecf6d..a5e8908 100644
--- a/kernel/proc.c
+++ b/kernel/proc.c
@@ -20,6 +20,26 @@ static void freeproc(struct proc *p);
 
 extern char trampoline[]; // trampoline.S
 
+// PHASE 1: let me use these for schedlogging in scheduler()
+static uint schedlog_start_tick;
+static int schedlog_duration;
+struct spinlock schedlog_lock; // secures operations on the schedlog_start_tick and schedlog_duration global variables
+
+// PHASE 1: retrieve start tick as global tick value at that tick
+void
+schedlog_start(int duration)
+{
+  acquire(&tickslock);
+  uint start_tick = ticks;
+  release(&tickslock);
+  
+  acquire(&schedlog_lock);
+  // Publish the start and duration together so readers see one activation.
+  schedlog_start_tick = start_tick;
+  schedlog_duration = duration;
+  release(&schedlog_lock);
+}
+
 // helps ensure that wakeups of wait()ing
 // parents are not lost. helps obey the
 // memory model when using p->parent.
@@ -51,6 +71,7 @@ procinit(void)
   
   initlock(&pid_lock, "nextpid");
   initlock(&wait_lock, "wait_lock");
+  initlock(&schedlog_lock, "schedlog_lock");
   for(p = proc; p < &proc[NPROC]; p++) {
       initlock(&p->lock, "proc");
       p->state = UNUSED;
@@ -111,6 +132,14 @@ allocproc(void)
 {
   struct proc *p;
 
+  // get the creation tick here, before the lock to prevent
+  // possible deadlock with p->lock
+  uint creation_tick;
+
+  acquire(&tickslock);
+  creation_tick = ticks;
+  release(&tickslock);
+
   for(p = proc; p < &proc[NPROC]; p++) {
     acquire(&p->lock);
     if(p->state == UNUSED) {
@@ -125,6 +154,14 @@ found:
   p->pid = allocpid();
   p->state = USED;
 
+  // PHASE 1
+  // Initialize STCF scheduling information.
+  p->creation_tick = creation_tick; // Record when process was created; important for breaking scheduling ties
+  p->total_burst = DEFAULT_STCF_BURST; // Stores the process's configured CPU burst budget
+  p->remaining_burst = DEFAULT_STCF_BURST; // Tracks how much of that budget remains
+  p->time_in_queue = 0;
+  p->ticks_consumed = 0;
+
   // Allocate a trapframe page.
   if((p->trapframe = (struct trapframe *)kalloc()) == 0){
     freeproc(p);
@@ -408,6 +445,74 @@ wait(uint64 addr)
   }
 }
 
+// PHASE 1 MODIFICATION: Charge one CPU tick to the process interrupted by the timer.
+// returns 1 if a process is killed, 0 otherwise
+#if defined(SCHED_POLICY_STCF) || defined(SCHED_POLICY_GROUP_STCF)
+int
+account_cpu_tick(struct proc *p)
+{
+  // Phase 1: Stop burst accounting once the process is marked killed (CL-38).
+  if(p == 0 || killed(p)) {
+    return 0;
+  }
+
+  p->ticks_consumed++;
+
+  if(p->remaining_burst > 0) {
+    p->remaining_burst--;
+  }
+
+  // The burst has not been exhausted yet.
+  if(p->remaining_burst > 0) {
+    return 0;
+  }
+  
+  // Keep init and sh alive by refreshing their burst budgets.
+  if(strncmp(p->name, "init", sizeof("init")) == 0 ||
+     strncmp(p->name, "sh", sizeof("sh")) == 0) {
+    p->total_burst = DEFAULT_STCF_BURST;
+    p->remaining_burst = DEFAULT_STCF_BURST;
+    p->ticks_consumed = 0;
+    return 0;
+  } else {
+    // Ordinary processes must not run beyond their burst.
+    setkilled(p);
+    return 1;
+  }
+}
+#endif
+
+// PHASE 1: Print scheduling information when schedlog is active.
+static void
+schedlog_print(void)
+{
+  acquire(&tickslock);
+  uint current_tick = ticks;
+  release(&tickslock);
+
+  // Verify schedlog_duration has not lapsed.
+  if(schedlog_duration <= 0 || current_tick - schedlog_start_tick >= schedlog_duration) {
+    return;
+  }
+
+  printf("%u|STCF|0(0,0)", current_tick);
+
+  for(struct proc *p = proc; p < &proc[NPROC]; p++) {
+    if(p->state == UNUSED) {
+      continue;
+    }
+
+    printf(",[%d]%s:%d(%d,%d)",
+      p->pid,
+      p->name,
+      p->state,
+      p->remaining_burst,
+      p->ticks_consumed);
+  }
+
+  printf("\n");
+}
+
 // Per-CPU process scheduler.
 // Each CPU calls scheduler() after setting itself up.
 // Scheduler never returns.  It loops, doing:
@@ -432,23 +537,82 @@ scheduler(void)
     intr_off();
 
     int found = 0;
-    for(p = proc; p < &proc[NPROC]; p++) {
-      acquire(&p->lock);
-      if(p->state == RUNNABLE) {
-        // Switch to chosen process.  It is the process's job
-        // to release its lock and then reacquire it
-        // before jumping back to us.
-        p->state = RUNNING;
-        c->proc = p;
-        swtch(&c->context, &p->context);
-
-        // Process is done running for now.
-        // It should have changed its p->state before coming back.
-        c->proc = 0;
-        found = 1;
+    #if defined(SCHED_POLICY_STCF)
+      // PHASE 1: Find the RUNNABLE process with the shortest
+      // remaining burst, breaking ties by creation tick and PID.
+      struct proc *best = 0;
+      int best_remaining = 0;
+      uint best_creation_tick = 0;
+      int best_pid = 0;
+
+      for(p = proc; p < &proc[NPROC]; p++) {
+        acquire(&p->lock);
+
+        if(p->state == RUNNABLE) {
+          if(best == 0 ||
+            p->remaining_burst < best_remaining ||
+            (p->remaining_burst == best_remaining &&
+              p->creation_tick < best_creation_tick) ||
+            (p->remaining_burst == best_remaining &&
+              p->creation_tick == best_creation_tick &&
+              p->pid < best_pid)) {
+            best = p;
+            best_remaining = p->remaining_burst;
+            best_creation_tick = p->creation_tick;
+            best_pid = p->pid;
+          }
+        }
+
+        release(&p->lock);
       }
-      release(&p->lock);
-    }
+
+      if(best != 0) {
+        // In case some part of the project is not run with CPUS=1: (verify)
+        // A different CPU may have changed this process since
+        // the scan, so reacquire its lock and validate our snapshot.
+        acquire(&best->lock);
+
+        if(best->state == RUNNABLE &&
+          best->remaining_burst == best_remaining &&
+          best->creation_tick == best_creation_tick &&
+          best->pid == best_pid) {
+
+          found = 1;
+          best->state = RUNNING;
+          c->proc = best;
+
+          schedlog_print();
+          swtch(&c->context, &best->context);
+
+          // The process has yielded, slept, or exited.
+          c->proc = 0;
+        }
+
+        release(&best->lock);
+      }
+    #else
+      // RR: preserve the original round-robin-style scan.
+      for(p = proc; p < &proc[NPROC]; p++) {
+        acquire(&p->lock);
+
+        if(p->state == RUNNABLE) {
+          // Switch to chosen process.  It is the process's job
+          // to release its lock and then reacquire it
+          // before jumping back to us.
+          p->state = RUNNING;
+          c->proc = p;
+
+          schedlog_print();
+          swtch(&c->context, &p->context);
+
+          // Process is done running for now.
+          // It should have changed its p->state before coming back.
+          c->proc = 0;
+          found = 1;
+        }
+        release(&p->lock);
+      }
+    #endif
     if(found == 0) {
       // nothing to run; stop running on this core until an interrupt.
       asm volatile("wfi");
```

#### `kernel/proc.h`

```diff
diff --git a/kernel/proc.h b/kernel/proc.h
index d021857..de29003 100644
--- a/kernel/proc.h
+++ b/kernel/proc.h
@@ -104,4 +104,11 @@ struct proc {
   struct file *ofile[NOFILE];  // Open files
   struct inode *cwd;           // Current directory
   char name[16];               // Process name (debugging)
+
+  // PHASE 1 MODS
+  uint creation_tick; // Tick at which the process is considered created; used for tie-breaking.
+  int total_burst; // Configured total CPU burst allowance.
+  int remaining_burst; // Remaining CPU ticks before the burst expires.
+  uint time_in_queue; // Time spend waiting in the RUNNABLE state; needed for later starvation mitigation.
+  uint ticks_consumed;
 };
```

#### `kernel/stcf.h` *(new)*

```diff
diff --git a/kernel/stcf.h b/kernel/stcf.h
new file mode 100644
index 0000000..b664973
--- /dev/null
+++ b/kernel/stcf.h
@@ -0,0 +1,12 @@
+// PHASE 1 (NEW FILE)
+#ifndef _STCF_H_
+#define _STCF_H_
+
+// Project document defined constants.
+#define DEFAULT_STCF_BURST 100 // default total burst time assigned to processes
+#define STCF_AT 20 // constant denoting how many ticks must be spent waiting befoe a process can earn a time discount; used to calculate ERBT
+#define STCF_AD 20 // constant denoting the time discount given to a process for waiting; used to calculate ERBT
+#define N_GROUPS 3 // note: this will need to be set t 4 for Phase 4, 8 for Phase 5
+#define DEFAULT_SHARES 100
+
+#endif
\ No newline at end of file
```

#### `kernel/syscall.c`

```diff
diff --git a/kernel/syscall.c b/kernel/syscall.c
index bd30737..444075b 100644
--- a/kernel/syscall.c
+++ b/kernel/syscall.c
@@ -102,6 +102,8 @@ extern uint64 sys_link(void);
 extern uint64 sys_mkdir(void);
 extern uint64 sys_close(void);
 extern uint64 sys_shutdown(void);
+extern uint64 sys_schedlog(void); // PHASE 1 MODIFICATIONS
+extern uint64 sys_setburst(void);
 
 // An array mapping syscall numbers from syscall.h
 // to the function that handles the system call.
@@ -128,6 +130,8 @@ static uint64 (*syscalls[])(void) = {
 [SYS_mkdir]   sys_mkdir,
 [SYS_close]   sys_close,
 [SYS_shutdown]   sys_shutdown,
+[SYS_schedlog]   sys_schedlog, // PHASE 1 MODIFICATIONS
+[SYS_setburst]   sys_setburst,
 };
 
 void
```

#### `kernel/syscall.h`

```diff
diff --git a/kernel/syscall.h b/kernel/syscall.h
index 1913514..8b18d81 100644
--- a/kernel/syscall.h
+++ b/kernel/syscall.h
@@ -21,3 +21,5 @@
 #define SYS_mkdir  20
 #define SYS_close  21
 #define SYS_shutdown  22
+#define SYS_schedlog  23    // PHASE 1 MODIFICATION
+#define SYS_setburst  24    // PHASE 1 MODIFICATION
\ No newline at end of file
```

#### `kernel/sysproc.c`

```diff
diff --git a/kernel/sysproc.c b/kernel/sysproc.c
index 182d924..9b81b4b 100644
--- a/kernel/sysproc.c
+++ b/kernel/sysproc.c
@@ -105,3 +105,50 @@ sys_shutdown(void)
   (*(volatile uint32 *) 0x100000) = 0x5555;
   return 0;
 }
+
+// PHASE 1: Add schedlog and setburst calls.
+uint64
+sys_schedlog(void)
+{
+  int duration;
+
+  argint(0, &duration);
+
+  if(duration <= 0) {
+    return -1;
+  }
+  
+  schedlog_start(duration);
+  return 0;
+}
+
+uint64
+sys_setburst(void)
+{
+  int burst;
+  struct proc *p = myproc();
+
+  argint(0, &burst);
+
+  // Invalid burst: mark the process as killed.
+  if(burst <= 0) {
+    setkilled(p);
+    return -1;
+  }
+
+  // Phase 1: Valid setburst has no effect under vanilla RR (CL-03/CL-19).
+#if defined(SCHED_POLICY_STCF) || defined(SCHED_POLICY_GROUP_STCF)
+  // Phase 1: Update the process's burst-time budget.
+  p->total_burst = burst;
+  p->remaining_burst = burst;
+
+  // Phase 1: Reset the number of ticks consumed.
+  p->ticks_consumed = 0;
+
+  // Phase 1: Always yield after a valid STCF burst update (CL-13).
+  yield();
+#endif
+
+  return 0;
+}
+
```

#### `kernel/trap.c`

```diff
diff --git a/kernel/trap.c b/kernel/trap.c
index 8a93454..5af4084 100644
--- a/kernel/trap.c
+++ b/kernel/trap.c
@@ -77,13 +77,32 @@ usertrap(void)
     setkilled(p);
   }
 
+  // PHASE 1: Account CPU time for STCF policies.
+  #if defined(SCHED_POLICY_STCF) || defined(SCHED_POLICY_GROUP_STCF)
+    if(which_dev == 2)
+      account_cpu_tick(p);
+  #endif
+
   if(killed(p))
     exit(-1);
 
   // give up the CPU if this is a timer interrupt.
-  if(which_dev == 2)
-    yield();
-
+  // RR and GROUP_STCF retain their timer-based yielding behavior.
+  // PHASE 1 STCF is non-preemptive (technically SJF). 
+  #if !defined(SCHED_POLICY_STCF)
+    if(which_dev == 2)
+      yield();
+  #endif
+  
+  // A process killed after the earlier check may resume here in kernel mode,
+  // but must exit before executing more user code. 
+  // Keep interrupts off between this check and the return
+  // to user mode so a timer cannot mark us killed after the final check.
+  intr_off();
+  if(killed(p)) {
+    exit(-1);
+  }
+  
   prepare_return();
 
   // the user page table to switch to, for trampoline.S
@@ -152,8 +171,27 @@ kerneltrap()
   }
 
   // give up the CPU if this is a timer interrupt.
-  if(which_dev == 2 && myproc() != 0)
-    yield();
+  // PHASE 1 CHANGE
+  #if defined(SCHED_POLICY_STCF)
+    if(which_dev == 2 && myproc() != 0) {
+      struct proc *p = myproc();
+
+      if(account_cpu_tick(p)) {
+        // Phase 1: Switch immediately on exhaustion; let the interrupted
+        // syscall finish before usertrap() handles normal killed termination.
+        yield();
+      }
+    }
+  #elif defined(SCHED_POLICY_GROUP_STCF)
+    if(which_dev == 2 && myproc() != 0) {
+      account_cpu_tick(myproc());
+      yield();
+    }
+  #else // RR
+    if(which_dev == 2 && myproc() != 0) {
+      yield();
+    }
+  #endif
 
   // the yield() may have caused some traps to occur,
   // so restore trap registers for use by kernelvec.S's sepc instruction.
```

#### `user/schedlogtest.c` *(new)*

```diff
diff --git a/user/schedlogtest.c b/user/schedlogtest.c
new file mode 100644
index 0000000..2da5067
--- /dev/null
+++ b/user/schedlogtest.c
@@ -0,0 +1,11 @@
+#include "kernel/types.h"
+#include "user/user.h"
+
+int
+main(void)
+{
+  printf("schedlog returned %d\n", schedlog(5));
+  for(volatile int i = 0; i < 100000000; i++)
+    ;
+  exit(0);
+}
\ No newline at end of file
```

#### `user/setbursttest.c` *(new)*

```diff
diff --git a/user/setbursttest.c b/user/setbursttest.c
new file mode 100644
index 0000000..1fcb40a
--- /dev/null
+++ b/user/setbursttest.c
@@ -0,0 +1,105 @@
+// PHASE 1 (NEW FILE)
+#include "kernel/types.h"
+#include "kernel/stat.h"
+#include "user/user.h"
+
+void
+test_invalid_burst(int burst)
+{
+  int pid = fork();
+
+  if(pid < 0) {
+    printf("ERROR: fork failed\n");
+    exit(1);
+  }
+
+  if(pid == 0) {
+    printf("Testing setburst(%d)...\n", burst);
+
+    int result = setburst(burst);
+
+    // Reaching this point means the invalid-burst process was not terminated as expected.
+    printf("ERROR: setburst(%d) returned %d; process survived\n",
+           burst, result);
+    exit(1);
+  }
+
+  // The parent waits for the child to terminate.
+  wait(0);
+  printf("Invalid-burst child reaped (burst=%d).\n", burst);
+}
+
+// PHASE 1: Test termination when a valid burst is exhausted.
+void
+test_burst_exhaustion(void)
+{
+  int pid = fork();
+
+  if(pid < 0) {
+    printf("ERROR: fork failed\n");
+    exit(1);
+  }
+
+  if(pid == 0) {
+    printf("Child: setting burst to 1 tick\n");
+
+    int result = setburst(1);
+    if(result != 0) {
+      printf("ERROR: setburst(1) returned %d\n", result);
+    }
+    printf("Child: entering infinite loop\n");
+    printf("setburst(1) output: %d\n", result);
+
+    // Prevent the compiler from optimizing away the loop.
+    volatile uint64 counter = 0;
+    while(1) {
+      counter++;
+    }
+    // Should never be reached.
+    printf("FAIL: child survived burst exhaustion\n");
+    exit(1);
+  }
+
+  // The parent waits for the child to be terminated.
+  wait(0);
+  printf("Child reaped after burst-exhaustion test.\n");
+}
+
+int
+main(void)
+{
+  printf("---SETBURST SYSCALL TESTS BEGIN---\n");
+
+  // Test 1: Positive burst value.
+  printf("\nTest 1: Valid burst\n");
+
+  int result = setburst(10);
+  if(result == 0) {
+    printf("PASS: setburst(10) returned 0\n");
+  } else {
+    printf("FAIL: setburst(10) returned %d\n", result);
+  }
+
+  // Test 2: Update the burst value again.
+  printf("\nTest 2: Update burst\n");
+
+  result = setburst(20);
+  if(result == 0) {
+    printf("PASS: setburst(20) returned 0\n");
+  } else {
+    printf("FAIL: setburst(20) returned %d\n", result);
+  }
+
+  // Test 3: Invalid burst values.
+  printf("\nTest 3: Invalid bursts\n");
+
+  test_invalid_burst(0);
+  test_invalid_burst(-5);
+
+  #if defined(SCHED_POLICY_STCF) || defined(SCHED_POLICY_GROUP_STCF)
+    printf("\nTest 4: Burst exhaustion\n");
+    test_burst_exhaustion();
+  #endif
+  printf("\n---ALL TESTS FINISHED---\n"); // sanaol
+  exit(0);
+}
```

#### `user/shutdown.c`

```diff
diff --git a/user/shutdown.c b/user/shutdown.c
index 1558ad2..a695995 100644
--- a/user/shutdown.c
+++ b/user/shutdown.c
@@ -3,4 +3,5 @@
 
 int main() {
   shutdown();
+  return 0;
 }
```

#### `user/user.h`

```diff
diff --git a/user/user.h b/user/user.h
index 628ff19..929c3fe 100644
--- a/user/user.h
+++ b/user/user.h
@@ -23,6 +23,10 @@ char* sbrk(int);
 int sleep(int);
 int uptime(void);
 int shutdown(void);
+int schedlog(int); // PHASE 1: Expose schedlog to user programs.
+int setburst(int); // Expose setburst to user programs.
+
+
 
 // ulib.c
 int stat(const char*, struct stat*);
```

#### `user/usys.pl`

```diff
diff --git a/user/usys.pl b/user/usys.pl
index 461806c..2af0c36 100644
--- a/user/usys.pl
+++ b/user/usys.pl
@@ -37,3 +37,5 @@ entry("sbrk");
 entry("sleep");
 entry("uptime");
 entry("shutdown");
+entry("setburst"); # PHASE 1 MODIFICATION: Generate user-space wrapper for setburst.
+entry("schedlog"); # PHASE 1 MODIFICATION: Generate user-space wrapper for schedlog.
\ No newline at end of file
```
