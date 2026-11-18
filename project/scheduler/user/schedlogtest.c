#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  printf("schedlog returned %d\n", schedlog(5));
  for(volatile int i = 0; i < 100000000; i++)
    ;
  exit(0);
}