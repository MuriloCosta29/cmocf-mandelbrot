#include <_time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> // atoi(), strtol()

// Entrada
typedef struct Params {
  int largura;       // argv[1]
  int altura;        // argv[2]
  int max_iteracoes; // argv[3]
  int num_threads;   // argv[4]
} Params;

int main(int argc, char *argv[]) {

  if (argc != 5) {
    fprintf(stderr,
            "Use: <largura>, <altura>, <max_iteracoes> <num_threads>\n");

    return 1;
  }

  Params p1;

  p1.largura = atoi(argv[1]);
  if (p1.largura <= 0) {
    fprintf(stderr, "Largura: tem que ser maior que 0\n");
    return 1;
  }

  p1.altura = atoi(argv[2]);
  if (p1.altura <= 0) {
    fprintf(stderr, "Altura: tem que ser maior que 0\n");
    return 1;
  }

  int *buffer = malloc(p1.altura * p1.largura * sizeof(int));

  if (buffer == NULL) {
    fprintf(stderr, "Erro ao alocar memória");
    return 1;
  }

  p1.max_iteracoes = atoi(argv[3]);
  if (p1.max_iteracoes <= 0) {
    fprintf(stderr, "Max_iteracoes: tem que ser maior que 0\n");
    return 1;
  }

  p1.num_threads = atoi(argv[4]);
  if (p1.num_threads <= 0) {
    fprintf(stderr, "Num_threads: tem que ser maior que 0\n");
    return 1;
  }

  return 0;
}
