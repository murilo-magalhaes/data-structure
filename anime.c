#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Necessário para sleep() e usleep()
#include <string.h>
#include <ctype.h>

// Importacoes linux
#include <termios.h>

#define ROWS 7
#define COLS 8 // 5 caracteres + '\0' para string válida
#define LETTERS 26

// Vetor de matrizes que representa as letras do alfabeto
char alphabet[LETTERS][ROWS][COLS] = {
    { " ***** ",  // A
      "**   **",
      "**   **",
      "*******",
      "**   **",
      "**   **",
      "**   **" },
    { "****** ",  // B
      "**   **",
      "**   **",
      "****** ",
      "**   **",
      "**   **",
      "****** " },

    { " ***** ",  // C
      "**   **",
      "**     ",
      "**     ",
      "**     ",
      "**   **",
      " ***** " },

    { "****** ",  // D
      "**   **",
      "**   **",
      "**   **",
      "**   **",
      "**   **",
      "****** " },

    { "*******",  // E
      "**     ",
      "**     ",
      "****** ",
      "**     ",
      "**     ",
      "*******" },

    { "*******",  // F
      "**     ",
      "**     ",
      "****** ",
      "**     ",
      "**     ",
      "**     " },

    { " ***** ",  // G
      "**     ",
      "**     ",
      "**  ***",
      "**   **",
      "**   **",
      " ***** " },

    { "**   **",  // H
      "**   **",
      "**   **",
      "*******",
      "**   **",
      "**   **",
      "**   **" },

    { "*******",  // I
      "   **  ",
      "   **  ",
      "   **  ",
      "   **  ",
      "   **  ",
      "*******" },

    { "    ***",  // J
      "     **",
      "     **",
      "     **",
      "**   **",
      "**   **",
      " ***** " },

    { "**   **",  // K
      "**  ** ",
      "** **  ",
      "***    ",
      "** **  ",
      "**  ** ",
      "**   **" },

    { "**     ",  // L
      "**     ",
      "**     ",
      "**     ",
      "**     ",
      "**     ",
      "*******" },

    { "**   **",  // M
      "*** ***",
      "*******",
      "** * **",
      "**   **",
      "**   **",
      "**   **" },

    { "**   **",  // N
      "***  **",
      "**** **",
      "** ****",
      "**  ***",
      "**   **",
      "**   **" },

    { " ***** ",  // O
      "**   **",
      "**   **",
      "**   **",
      "**   **",
      "**   **",
      " ***** " },

    { "****** ",  // P
      "**   **",
      "**   **",
      "****** ",
      "**     ",
      "**     ",
      "**     " },

    { " ***** ",  // Q
      "**   **",
      "**   **",
      "**   **",
      "** * **",
      "**  ** ",
      " **** *" },

    { "****** ",  // R
      "**   **",
      "**   **",
      "****** ",
      "**  ** ",
      "**   **",
      "**   **" },

    { " ***** ",  // S
      "**     ",
      "**     ",
      " ***** ",
      "     **",
      "     **",
      " ***** " },

    { "*******",  // T
      "   **  ",
      "   **  ",
      "   **  ",
      "   **  ",
      "   **  ",
      "   **  " },

    { "**   **",  // U
      "**   **",
      "**   **",
      "**   **",
      "**   **",
      "**   **",
      " ***** " },

    { "**   **",  // V
      "**   **",
      "**   **",
      "**   **",
      " ** ** ",
      "  ***  ",
      "   *   " },

    { "**   **",  // W
      "**   **",
      "**   **",
      "** * **",
      "*******",
      "*** ***",
      "**   **" },

    { "**   **",  // X
      " ** ** ",
      "  ***  ",
      "   *   ",
      "  ***  ",
      " ** ** ",
      "**   **" },

    { "**   **",  // Y
      " ** ** ",
      "  ***  ",
      "   *   ",
      "   *   ",
      "   *   ",
      "   *   " },

    { "*******",  // Z
      "     **",
      "    ** ",
      "   **  ",
      "  **   ",
      " **    ",
      "*******" }
};

// Função para exibir uma letra específica
void printLetter(char letter) {
    if (letter < 'A' || letter > 'Z') {
        printf("Letra inválida! Use apenas letras maiúsculas de A a Z.\n");
        return;
    }

    int index = letter - 'A';  // Converte 'A' em 0, 'B' em 1, etc.
    for (int i = 0; i < ROWS; i++) {
        printf("%s\n", alphabet[index][i]);
    }
}

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

void clearLastLines(int numLines)
{
    for (int i = 0; i < numLines; i++)
    {
        printf("\033[F"); // Move o cursor para a linha anterior
        printf("\033[K"); // Limpa a linha atual
    }
}

char *strToUpper(const char *str)
{
  const int length = (int)strlen(str);
  char *upperStr = malloc((length + 1) * sizeof(char)); // Aloca espaço para a cópia

  if (!upperStr)
    return NULL; // Verifica erro de alocação

  for (int i = 0; i < length; i++)
  {
    upperStr[i] = (char)toupper(str[i]);
  }
  upperStr[length] = '\0'; // Adiciona o caractere nulo

  return upperStr;
}

void printLayerWord(char *word, int layer) {
  int len = strlen(word);
  for (int i = 0; i < len; i++) {
    char letter = word[i];
    int index = letter - 'A';  // Converte 'A' em 0, 'B' em 1, etc.

    if (i < len - 1) {
      printf("%s\t\t", alphabet[index][layer]);
    } else {
      printf("%s\n", alphabet[index][layer]);
    }
  }
}

void animePrint(const char *word) {

  const char *upperWord = strToUpper(word);

  for (int i = 0; i < ROWS; i++) {
    printLayerWord(upperWord, i);
    sleep(1);
  }

  free(upperWord);
}



int main() {

    char *word;

    while (1) {
      printf("Enter a word: ");
      scanf("%s", word);

      animePrint(word);
    }

    return 0;
}
