#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *line = NULL;
  size_t len = 0;
  ssize_t numchar = getline(&line, &len, stdin);
  if (numchar == -1) {
    perror("getline failed");
    exit(EXIT_FAILURE);
  }

  char *saveptr;
  char *ret = strtok_r(line, " ", &saveptr);
  while (ret != NULL) {
    printf("%s\n", ret);
    ret = strtok_r(NULL, " ", &saveptr);
  }

  free(line);
  return 0;
}
