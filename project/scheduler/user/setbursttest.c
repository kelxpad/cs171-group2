// PHASE 1 (NEW FILE)
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
test_invalid_burst(int burst)
{
  int pid = fork();

  if(pid < 0) {
    printf("ERROR: fork failed\n");
    exit(1);
  }

  if(pid == 0) {
    printf("Testing setburst(%d)...\n", burst);

    int result = setburst(burst);

    // Reaching this point means the invalid-burst process was not terminated as expected.
    printf("ERROR: setburst(%d) returned %d; process survived\n",
           burst, result);
    exit(1);
  }

  // The parent waits for the child to terminate.
  wait(0);
  printf("Invalid-burst child reaped (burst=%d).\n", burst);
}

// PHASE 1: Test termination when a valid burst is exhausted.
void
test_burst_exhaustion(void)
{
  int pid = fork();

  if(pid < 0) {
    printf("ERROR: fork failed\n");
    exit(1);
  }

  if(pid == 0) {
    printf("Child: setting burst to 1 tick\n");

    int result = setburst(1);
    if(result != 0) {
      printf("ERROR: setburst(1) returned %d\n", result);
    }
    printf("Child: entering infinite loop\n");
    printf("setburst(1) output: %d\n", result);

    // Prevent the compiler from optimizing away the loop.
    volatile uint64 counter = 0;
    while(1) {
      counter++;
    }
    // Should never be reached.
    printf("FAIL: child survived burst exhaustion\n");
    exit(1);
  }

  // The parent waits for the child to be terminated.
  wait(0);
  printf("Child reaped after burst-exhaustion test.\n");
}

int
main(void)
{
  printf("---SETBURST SYSCALL TESTS BEGIN---\n");

  // Test 1: Positive burst value.
  printf("\nTest 1: Valid burst\n");

  int result = setburst(10);
  if(result == 0) {
    printf("PASS: setburst(10) returned 0\n");
  } else {
    printf("FAIL: setburst(10) returned %d\n", result);
  }

  // Test 2: Update the burst value again.
  printf("\nTest 2: Update burst\n");

  result = setburst(20);
  if(result == 0) {
    printf("PASS: setburst(20) returned 0\n");
  } else {
    printf("FAIL: setburst(20) returned %d\n", result);
  }

  // Test 3: Invalid burst values.
  printf("\nTest 3: Invalid bursts\n");

  test_invalid_burst(0);
  test_invalid_burst(-5);

  #if defined(SCHED_POLICY_STCF) || defined(SCHED_POLICY_GROUP_STCF)
    printf("\nTest 4: Burst exhaustion\n");
    test_burst_exhaustion();
  #endif
  printf("\n---ALL TESTS FINISHED---\n"); // sanaol
  exit(0);
}
