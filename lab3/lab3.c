#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_LEN 5

// Utilize array ("history") as a FIFO stack (queue)
// New inputs "push" old inputs out

char *get_input(char **input, size_t *len, ssize_t *numchar) {
  printf("Enter input: ");
  *numchar = getline(input, len, stdin);
  if (*numchar == -1) {
    perror("getline failed");
    exit(EXIT_FAILURE);
  }
  return *input;
}

void add_to_history(char *history[], char *input, int *count, int *index) {
  // Free old line if overwriting
  if (history[*index]) {
    free(history[*index]);
  }

  // Save input to history, update index and count
  history[*index] = strdup(input);

  *index = (*index + 1) % HISTORY_LEN;
  if (*count < HISTORY_LEN) {
    (*count)++;
  }
}

void print_history(char *history[], int count, int index) {
  int start = (count == HISTORY_LEN) ? index : 0;
  for (int i = 0; i < count; i++) {
    int pos = (start + i) % HISTORY_LEN;
    printf("%s", history[pos]);
  }
}

int main() {
  // Can make history, count and index global to reduce pointers usage
  char *history[HISTORY_LEN] = {0};
  int count = 0;
  int index = 0;

  char *input = NULL;
  size_t len = 0;
  ssize_t numchar = 0;

  const char *print = "print\n";

  while (1) {
    input = get_input(&input, &len, &numchar);

    add_to_history(history, input, &count, &index);

    // Print history if "print" is inputted
    if (strcmp(input, print) == 0) {
      print_history(history, count, index);
    }
  }
  // free(input);  // This line do nothing since it's never reached (infinite
  // loop). No need to free history either, OS will deal with that when program
  // terminate

  return 0;
}
