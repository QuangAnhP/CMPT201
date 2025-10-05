#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 128

struct header {
  uint64_t size;
  struct header *next;
};

void *increase_heap_size(size_t heapSize) {
  void *newAddr = sbrk(heapSize);
  return newAddr;
}

struct header *init_block(void *startLocation, size_t blockSize,
                          uint8_t value) {
  // Create block and initialize header
  struct header *block = (struct header *)startLocation;
  size_t headerSize = sizeof(struct header);
  size_t dataSize = blockSize - headerSize;

  block->size = blockSize;
  block->next = NULL;

  // Fill data region
  memset((char *)block + headerSize, value, dataSize);

  return block;
}

void handle_error(const char *error) {
  perror(error);
  exit(1);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data
                                                       : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

void print_data(struct header *block) {
  // data starts after header
  unsigned char *data = (unsigned char *)block + sizeof(struct header);
  size_t dataSize = block->size - sizeof(struct header);
  for (size_t i = 0; i < dataSize; i++) {
    printf("%u ", data[i]);
  }
  printf("\n");
}

int main() {
  size_t heapSize = 256;
  size_t blockSize = heapSize / 2;
  void *heapStartLocation = increase_heap_size(heapSize);

  struct header *block1 = init_block(heapStartLocation, blockSize, 0);
  struct header *block2 =
      init_block((char *)heapStartLocation + blockSize, blockSize, 1);
  block2->next = block1;

  print_out("first block:       %p\n", &block1, sizeof(void *));
  print_out("second block:      %p\n", &block2, sizeof(void *));
  print_out("first block size:  %llu\n", &block1->size, sizeof(uint64_t));
  print_out("first block next:  %p\n", &block1->next, sizeof(void *));
  print_out("second block size: %llu\n", &block2->size, sizeof(uint64_t));
  print_out("second block next: %p\n", &block2->next, sizeof(void *));

  print_data(block1);
  print_data(block2);
  return 0;
}
