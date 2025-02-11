#include <stdio.h>
#include <stdlib.h>

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

void detectArrowKeys() {
    int ch;

    printf("Pressione as setas do teclado (ou 'q' para sair)...\n");

    enableRawMode();  // Ativa o modo raw do terminal

    while (1) {
        ch = getch(); // Captura o primeiro caractere

        if (ch == 'q') { // Tecla 'q' para sair
            printf("Saindo...\n");
            break;
        }

        if (ch == 27) { // Primeiro caractere da sequência de escape (ESC)
            if (getch() == 91) { // Segundo caractere '['
                switch (getch()) { // Terceiro caractere identifica a seta
                    case 65:
                        printf("Seta PARA CIMA detectada!\n");
                        break;
                    case 66:
                        printf("Seta PARA BAIXO detectada!\n");
                        break;
                    case 67:
                        printf("Seta PARA DIREITA detectada!\n");
                        break;
                    case 68:
                        printf("Seta PARA ESQUERDA detectada!\n");
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
    detectArrowKeys();
    return 0;
}
