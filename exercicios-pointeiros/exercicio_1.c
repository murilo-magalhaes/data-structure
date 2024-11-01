#include <stdio.h>
#include <stdlib.h>

int recebe_vetor(int *vetor, int tamanho)
{
  return malloc(tamanho * sizeof(int));
}

int main()
{

  int *vetor;
  int n, i;

  n = recebe_vetor(&vetor);
  for (i = 0; i < n; i++)
  {
    /* faz algum processamento sobre vetor[i] */
  }
  return 0;
}