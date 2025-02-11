#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#ifdef _WIN32
    #include <conio.h>  // Windows: Usa _getch()
#else
    #include <termios.h>
    #include <unistd.h>

    struct termios oldt;  // Variável global para armazenar a configuração original do terminal

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Restaura o modo original do terminal
}

void enableRawMode() {
  struct termios newt;
  tcgetattr(STDIN_FILENO, &oldt);  // Salva as configurações atuais do terminal
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);  // Desativa entrada canônica e eco
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  // Garante que o terminal seja restaurado ao sair do programa
  atexit(disableRawMode);
}

int getch(void) { // Função para capturar uma tecla no Linux/macOS
  return getchar();
}
#endif

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

void printPerson(struct Person person) {
  printf("Name: %s\n", person.name);
  printf("Sex: %c\n", person.sex);
  printf("Wage: %.2f\n", person.wage);
}

void clearTerminal() {
#ifdef _WIN32
  system("cls");  // Comando para Windows
#else
  system("clear"); // Comando para Linux/macOS
#endif
}

void clearLastLines(int numLines) {
  for (int i = 0; i < numLines; i++) {
    printf("\033[F"); // Move o cursor para a linha anterior
    printf("\033[K"); // Limpa a linha atual
  }
}

void navigation(struct List* list) {
  int ch;

  printf("\nPressione as setas do teclado (ou 'q' para sair)...\n");

  enableRawMode();  // Ativa o modo raw do terminal

  struct Node *node = list->head;
  int counter = 1;

  printf("\nExibindo pessoa %d:\n", counter);
  printPerson(node->person);

  while (1) {
    ch = getch(); // Captura o primeiro caractere

    if (ch == 'q') { // Tecla 'q' para sair
      printf("\nSaindo...\n");
      break;
    }

    if (ch == 27) { // Primeiro caractere da sequência de escape (ESC)
      if (getch() == 91) { // Segundo caractere '['
        switch (getch()) { // Terceiro caractere identifica a seta
          case 67:
            if (node->next != NULL) {
              clearLastLines(4);
              counter++;
              printf("Exibindo pessoa %d:\n", counter);
              node = node->next;
              printPerson(node->person);
            }
          break;
          case 68:
            if (node->prev != NULL) {
              clearLastLines(4);
              counter--;
              printf("Exibindo pessoa %d:\n", counter);
              node = node->prev;
              printPerson(node->person);
            }
          break;
          default:
            printf("Outra tecla pressionada.\n");
          break;
        }
      }
    }
  }

  disableRawMode(); // Restaura o terminal antes de sair
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
    puts("3. Navegação");

    printf("Escolha uma opcao: ");

    if (scanf("%d", &option) != 1) { // Se não for número, retorna erro
      printf("\nEntrada invalida! Digite um numero.\n");
      while (getchar() != '\n'); // Limpa o buffer de entrada
      option = -1; // Define um valor inválido para manter o loop
      continue;
    }

    if (option == 1) {
      printf("\n------------------------------------------------------\n");
      printGrowing(list->head);
    } else if (option == 2) {
      printf("\n------------------------------------------------------\n");
      printDecreasing(list->tail);
    } else if (option == 3) {
      printf("\nIniciando navegação...\n");
      navigation(list);


    } else if (option == 0) {
      puts("\nPrograma encerrado");
    } else {
      puts("\nOpcao invalida");
    }

  } while (option != 0);

  fclose(file);
  free(list);

  return 0;
}