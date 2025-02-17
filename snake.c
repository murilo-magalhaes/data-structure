#include <stdio.h>
#include <stdlib.h>
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

struct List *createList()
{

  struct List *list = (struct List *)malloc(sizeof(struct List));

  if (list == NULL)
  {
    printf("Not possible to create a list");
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

void mountSnake(struct List *list) {
  const int initialPos[][] = {{1,3},{1,2},{1,1}};

  for (int i = 0; i < 3; i++) {
    // Continuar
  }

}

void clearLastLines(int numLines)
{
  for (int i = 0; i < numLines + 3; i++)
  {
    printf("\033[F"); // Move o cursor para a linha anterior
    printf("\033[K"); // Limpa a linha atual
  }
}

void printBoard(int x, int y) {
  for (int i = 0; i < ROWS; i++){
    for (int j = 0; j < COLS; j++){
      if ((i == 0 && j == 0) || (i == ROWS-1 && j == COLS-1) || (i == 0 && j == COLS-1) || (i == ROWS-1 && j == 0)) {
        printf("+");
      } else if (i == 0 || i == ROWS-1) {
        printf("-");
      } else if (j == 0 || j == COLS-1) {
        printf("|");
      } else if (i == y && j == x) {
        printf("*");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }

  printf("(%d, %d)\n", x, y);
}

void navigation(int x, int y)
{
  int ch;

  enableRawMode(); // Ativa o modo raw do terminal
  printBoard(x, y);

  while (1)
  {
    ch = getch(); // Captura o primeiro caractere

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
              if(y > 1) {
                clearLastLines(ROWS);
                y--;
                printBoard(x, y);
              }
          break;
          case 66:
            // Seta baixo
              if(y < ROWS - 2) {
                clearLastLines(ROWS);
                y++;
                printBoard(x, y);
              }
          break;
          case 67:
            // Seta direita
            if(x < COLS - 2) {
              clearLastLines(ROWS);
              x++;
              printBoard(x, y);
            }
          break;
          case 68:
            // Seta esquerda
            if(x > 1) {
              clearLastLines(ROWS);
              x--;
              printBoard(x, y);
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

  struct List *list = createList();

  navigation(x, y);

  free(list);
  return 0;
}
