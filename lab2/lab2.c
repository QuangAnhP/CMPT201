#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // fork()
#include <sys/wait.h>  // waitpid()
#include <unistd.h>    // execl()

int main() {
  // Read user input
  char *line = NULL;
  size_t len = 0;

  while (1) {
    printf("Enter program to run.\n");
    ssize_t numchar = getline(&line, &len, stdin);
    if (numchar == -1) {
      perror("getline failed");
      exit(EXIT_FAILURE);
    }

    // Remove "\n" (last index) + make a copy cuz strtok_r destroy line
    if (line[numchar - 1] == '\n') {
      line[numchar - 1] = '\0';
    }
    char *copy = strdup(line);

    // Tokenize and save last element (will need for execl() later)
    char *saveptr;
    char *last = NULL;
    char *ret = strtok_r(line, "/", &saveptr);
    while (ret != NULL) {
      last = ret;
      ret = strtok_r(NULL, "/", &saveptr);
    }

    pid_t pid = fork();
    if (pid == 0) { // Child
      if (execl(copy, last, NULL) == -1) {
        printf("Exec failure\n");
        exit(EXIT_FAILURE); // child must end whether fail or succeed
      }
    } else if (pid > 0) { // Parent
      int wstatus = 0;
      if (waitpid(pid, &wstatus, 0) == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
      }
    } else { // Fail
      perror("fork");
      exit(EXIT_FAILURE);
    }

    free(copy);
  }

  free(line);
  return 0;
}
