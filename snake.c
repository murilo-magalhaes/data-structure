#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

const int ROWS = 10;
const int COLS = 20;

struct termios oldt; // Variável global para armazenar a configuração original do terminal

void disableRawMode()
{
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restaura o modo original do terminal
}

void enableRawMode()
{
  struct termios newt;
  tcgetattr(STDIN_FILENO, &oldt); // Salva as configurações atuais do terminal
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO); // Desativa entrada canônica e eco
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  // Garante que o terminal seja restaurado ao sair do programa
  atexit(disableRawMode);
}

int getch(void)
{ // Função para capturar uma tecla no Linux/macOS
  return getchar();
}

struct Node
{
  int x;
  int y;
  struct Node *next;
  struct Node *prev;
};

struct List
{
  struct Node *head;
  struct Node *tail;
};

enum Direction { UP, DOWN, LEFT, RIGHT };

struct List *createList()
{

  struct List *list = (struct List *)malloc(sizeof(struct List));

  if (list == NULL)
  {
    printf("Not possible to create a list");
    return NULL;
  }
  list->head = NULL;
  list->tail = NULL;

  return list;
}

struct Node *createNode(const int x, const int y)
{

  struct Node *node = (struct Node *)malloc(sizeof(struct Node));
  if (node == NULL)
  {
    printf("Not possible to create a node");
    return NULL;
  }

  node->x = x;
  node->y = y;
  node->next = NULL;
  node->prev = NULL;

  return node;
}

void appendNode(struct List *list, struct Node *node) {
  if (list->head == NULL) {
    list->head = node;
    list->tail = node;
  } else {
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
  }
}

void mountSnake(struct List *list) {

  for (int i = 0; i < 3; i++) {
    struct Node *node = createNode(1, i + 1);
    appendNode(list, node);
  }

}

void clearLastLines(const int numLines)
{
  for (int i = 0; i < numLines + 3; i++)
  {
    printf("\033[F"); // Move o cursor para a linha anterior
    printf("\033[K"); // Limpa a linha atual
  }
}

bool isSnakeInCoord(const struct Node *node, const int x, const int y) {
  if (node->x == x && node->y == y) return true;
  if(node->next != NULL) isSnakeInCoord(node->next, x, y);

  return false;
}

void printBoard(const struct List *list) {
  for (int i = 0; i < ROWS; i++){
    for (int j = 0; j < COLS; j++){
      if ((i == 0 && j == 0) || (i == ROWS-1 && j == COLS-1) || (i == 0 && j == COLS-1) || (i == ROWS-1 && j == 0)) {
        printf("+");
      } else if (i == 0 || i == ROWS-1) {
        printf("-");
      } else if (j == 0 || j == COLS-1) {
        printf("|");
      } else if (isSnakeInCoord(list->head, j, i)) {
        printf("*");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }

  printf("(%d, %d)\n", list->head->x, list->head->y);
}

void moveSnake(const struct List *list, enum Direction direction) {
  struct Node *node = list->head;
  int x = node->x;
  int y = node->y;

  while (node != NULL) {

  switch (direction) {
    case UP:
      node->y--;
      break;
    case DOWN:
      node->y++;
      break;
    case LEFT:
      node->x--;
      break;
    case RIGHT:
      node->x++;
      break;
  }
    printf("%d %d\n", node->x, node->y);
    node = node->next;
  }
}

void navigation(const struct List *list)
{
  enableRawMode(); // Ativa o modo raw do terminal
  printBoard(list);

  while (1)
  {
    const int ch = getch(); // Captura o primeiro caractere

    if (ch == 'q')
    { // Tecla 'q' para sair
      printf("\nSaindo...\n");
      break;
    }

    if (ch == 27)
    { // Primeiro caractere da sequência de escape (ESC)
      if (getch() == 91)
      { // Segundo caractere '['
        switch (getch())
        { // Terceiro caractere identifica a seta
          case 65:
            // Seta cima
              if(list->head->y > 1) {
                clearLastLines(ROWS);
                moveSnake(list, UP);
                printBoard(list);
              }
          break;
          case 66:
            // Seta baixo
              if(list->head->y < ROWS - 2) {
                clearLastLines(ROWS);
                moveSnake(list, DOWN);
                printBoard(list);
              }
          break;
          case 67:
            // Seta direita
            if(list->head->x < COLS - 2) {
              clearLastLines(ROWS);
              moveSnake(list, RIGHT);
              printBoard(list);
            }
          break;
          case 68:
            // Seta esquerda
            if(list->head->x > 1) {
              clearLastLines(ROWS);
              moveSnake(list, LEFT);
              printBoard(list);
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

void printList(const struct List *list) {
  struct Node *node = list->head;
  while (node != NULL) {
    printf("%d %d\n", node->x, node->y);
    node = node->next;
  }
}

int main() {

  struct List *list = createList();
  if (list == NULL) {
    printf("Not possible to create a list");
    return 1;
  }

  mountSnake(list);

  navigation(list);

  free(list);
  return 0;
}
