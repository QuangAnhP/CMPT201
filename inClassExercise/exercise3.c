#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();
  switch (pid) {
  case 0:
    printf("Child");
  default:
    printf("Parent, %d\n", pid);
  }
  while (1) {
    sleep(3);
  }
}
