#include <stdio.h>
#include <stdlib.h>

int recebe_vetor(int *vetor)
{

  int n;
  scanf("%d", &n);

  vetor = (int *)malloc(n * sizeof(int));

  for (int i = 0; i < n; i++)
  {
    scanf("%d", vetor[i]);
  }

  return sizeof(vetor);
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