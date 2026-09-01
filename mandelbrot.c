#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int width;
int height;
int max_iterations;
int number_of_threads;
int *image;
FILE *error_file;

void save_error(const char *message) { fprintf(error_file, "%s\n", message); }

int read_number(const char *text, int *number) {
  char *end;
  long value;

  errno = 0;
  value = strtol(text, &end, 10);

  if (errno != 0 || *text == '\0' || *end != '\0' || value <= 0 ||
      value > INT_MAX) {
    return 0;
  }

  *number = (int)value;
  return 1;
}

int main(int argc, char *argv[]) {
  error_file = fopen("errors.txt", "w");
  if (error_file == NULL) {
    return EXIT_FAILURE;
  }

  if (argc != 5) {
    save_error("Uso: mandelbrot largura altura max_iteracoes num_threads");
    return EXIT_FAILURE;
  }

  if (!read_number(argv[1], &width) || !read_number(argv[2], &height) ||
      !read_number(argv[3], &max_iterations) ||
      !read_number(argv[4], &number_of_threads)) {
    save_error("Erro: os quatro argumentos devem ser inteiros positivos.");
    return EXIT_FAILURE;
  }

  if ((size_t)width > SIZE_MAX / sizeof(int) / (size_t)height) {
    save_error("Erro: as dimensoes da imagem sao muito grandes.");
    return EXIT_FAILURE;
  }

  image = malloc((size_t)width * height * sizeof(int));
  if (image == NULL) {
    save_error("Erro ao alocar memoria para a imagem.");
    return EXIT_FAILURE;
  }

  free(image);
  fclose(error_file);
  return EXIT_SUCCESS;
}
