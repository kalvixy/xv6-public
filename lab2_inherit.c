#include "types.h"
#include "user.h"


int main() {
  int pid = fork();
  int i = 0;
  //Child pid of 0 means it is the child printing"
  printf(1, "Priority: %d(Child pid = %d)\n", setpriority(-100), pid);
  if (!pid) {
    printf(1, "Child priority before: 10\n", setpriority(-100));
    for (i = 0; i < 5000000; ++i)
      asm("nop");
    printf(1, "Child priority after: %d\n", setpriority(-100));
    exit(0);
  } else if (pid < 0) {
    printf(1, "Error forking\n");
  } else {
    setpriority(0);
    printf(1, "Priority: %d(Child pid = %d)\n", setpriority(-100), pid);
    printf(1, "Waiting for child\n");
    waitpid(pid, 0, 0);
    printf(1, "Child exited\n");
    exit(0);
  }
  return 0;
}
