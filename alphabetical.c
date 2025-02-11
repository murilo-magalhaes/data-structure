#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

int main() {
    const char *name1 = "John";
    const char *name2 = "Mary";

    bool result = isBefore(name1, name2);
    printf("%s\n", result ? "true" : "false");

    return 0;
}
