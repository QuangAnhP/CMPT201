#include <stdio.h>
#include <stdlib.h>

struct Node {
  int data;
  struct Node *next;
};

// Function to create a new node with the given data
struct Node *createNode(int data) {
  struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

// Function to insert a new node at the front of the linked list
void append(struct Node **head, int data) {
  // Create new node
  struct Node *newNode = createNode(data);

  newNode->next = *head;
  *head = newNode;
}

// Function to traverse and print the linked list
void traverse(struct Node *head) {
  struct Node *current = head;
  while (current != NULL) {
    printf("%d -> ", current->data);
    current = current->next;
  }
  printf("NULL\n");
}

void reverse(struct Node *head) {
  struct Node *current = head;
  struct Node *prev = NULL;
  struct Node *next = head->next;
  while (next != NULL) {
    current->next = prev;
    next->next = current;
    prev = current;
    // TODO
  }
}

int main() {
  struct Node *head = NULL;
  // Append elements to the linked list
  for (int i = 0; i <= 10; i++) {
    append(&head, i);
  }

  //  Traverse and print
  printf("Linked List: ");
  traverse(head);

  // Free memory
  while (head != NULL) {
    struct Node *remove = head;
    head = head->next;
    free(remove);
  }
  return 0;
}
