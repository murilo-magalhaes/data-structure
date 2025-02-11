#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

struct Person {
  char *name;
  char sex;
  float wage;
};

struct Node {
  struct Person person;
  struct Node *next;
  struct Node *prev;
};

struct List {
  struct Node *head;
  struct Node *tail;
};

struct List* createList() {

  struct List* list = (struct List*) malloc(sizeof(struct List));

  if (list == NULL) {
    printf("Not possible to create a list");
  }
  list->head = NULL;
  list->tail = NULL;

  return list;

}

struct Node* createNode(const struct Person person) {

  struct Node* node = (struct Node*) malloc(sizeof(struct Node));
  if (node == NULL) {
    printf("Not possible to create a node");
  }

  node->person = person;
  node->next = NULL;
  node->prev = NULL;

  return node;

}

char* substr(const char *str, const int start, const int len) {

  char *dest = malloc(len * sizeof(char));
  if (!dest) return NULL;

  int currentChar = 0;
  for (int i = start; i < start + len; i++) {
    dest[currentChar] = str[i];
    currentChar++;
  }
  dest[currentChar] = '\0';

  return dest;
}

char *strToLower(const char *str) {
  int length = strlen(str);
  char *lowerStr = malloc((length + 1) * sizeof(char)); // Aloca espaço para a cópia

  if (!lowerStr) return NULL; // Verifica erro de alocação

  for (int i = 0; i < length; i++) {
    lowerStr[i] = tolower(str[i]);
  }
  lowerStr[length] = '\0'; // Adiciona o caractere nulo

  return lowerStr;
}

bool isBefore(const char *str, const char *toCompare) {
  char *strLower = strToLower(str);
  char *toCompareLower = strToLower(toCompare);

  if (!strLower || !toCompareLower) { // Verifica erro de alocação
    free(strLower);
    free(toCompareLower);
    return false;
  }

  int minorLength = strlen(strLower) < strlen(toCompareLower) ? strlen(strLower) : strlen(toCompareLower);

  for (int i = 0; i < minorLength; i++) {
    if (strLower[i] != toCompareLower[i]) {
      bool result = strLower[i] < toCompareLower[i];
      free(strLower);
      free(toCompareLower);
      return result;
    }
  }

  bool result = strlen(strLower) < strlen(toCompareLower);
  free(strLower);
  free(toCompareLower);
  return result;
}

void insertNode(struct List* list, struct Person person) {
  struct Node* newNode = createNode(person);
  if (!newNode) return; // Verifica erro de alocação

  if (list->head == NULL) { // Lista vazia
    list->head = newNode;
    list->tail = newNode;
    return;
  }

  struct Node* currentNode = list->head;
  struct Node* previousNode = NULL;

  // Encontrar posição de inserção
  while (currentNode != NULL) {
    if (isBefore(person.name, currentNode->person.name)) {
      break;
    }
    previousNode = currentNode;
    currentNode = currentNode->next;
  }

  // Inserção no início
  if (previousNode == NULL) {
    newNode->next = list->head;
    list->head->prev = newNode;
    list->head = newNode;
    return;
  }

  // Inserção no final
  if (currentNode == NULL) {
    previousNode->next = newNode;
    newNode->prev = previousNode;
    list->tail = newNode;
    return;
  }

  // Inserção no meio
  previousNode->next = newNode;
  newNode->prev = previousNode;
  newNode->next = currentNode;
  currentNode->prev = newNode;
}

void printGrowing(struct Node* node) {
  printf("Name: %s\n", node->person.name);
  printf("Sex: %c\n", node->person.sex);
  printf("Wage: %.2f\n", node->person.wage);
  printf("------------------------------------------------------\n");
  if (node->next != NULL) {
    printGrowing(node->next);
  }
}

void printDecreasing(struct Node* node) {
  printf("Name: %s\n", node->person.name);
  printf("Sex: %c\n", node->person.sex);
  printf("Wage: %.2f\n", node->person.wage);
  printf("------------------------------------------------------\n");
  if (node->prev != NULL) {
    printDecreasing(node->prev);
  }
}

int main() {

  FILE *file = fopen("list.txt", "r");
  if (file == NULL) {
    printf("Error opening file.\n");
  }

  struct List* list = createList();

  char line[111];

  while(fgets(line, sizeof(line), file)) {
    if (line[0] != '\n') {
      struct Person person;
      person.name =  substr(line, 0, 100);
      person.sex = line[100];
      person.wage = atof(substr(line, 101, 9));

      insertNode(list, person);
    }
  }
  int option = 0;

  do {

    puts("\nMenu:");
    puts("0. Sair");
    puts("1. Mostrar dados em ordem crescente");
    puts("2. Mostrar dados em ordem decrescente");

    scanf("%d", &option);

    if (option == 1) {
      printf("------------------------------------------------------\n");
      printGrowing(list->head);
    } else if (option == 2) {
      printf("------------------------------------------------------\n");
      printDecreasing(list->tail);
    } else if (option == 0) {
      puts("Programa encerrado");
    }

  } while (option != 0);

  fclose(file);
  free(list);

  return 0;
}