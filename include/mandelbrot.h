#ifndef MANDELBROTH_H
#define MANDELBROTH_H
#define REAL_MIN -2.0
#define REAL_MAX -2.0
#define IMAG_MIN -1.5
#define IMAG_MAX -1.5

typedef struct Params {
  int largura;
  int altura;
  int max_iter;
  int num_threads;
  int *imagem;
  //
} Params;

#endif // !MANDELBROTH_H
