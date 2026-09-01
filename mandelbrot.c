#include <errno.h>
#include <limits.h>
#include <omp.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef LOGIN
#define LOGIN "cmocf"
#endif

#define SERIAL_FILE "mandelbrot_" LOGIN "_serial.pgm"
#define OPENMP_FILE "mandelbrot_" LOGIN "_openmp.pgm"
#define PTHREADS1_FILE "mandelbrot_" LOGIN "_pthreads1.pgm"

typedef struct {
  int id;
} ThreadData;

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

int pixel_value(int column, int row) {
  double real;
  double imaginary;
  double z_real = 0.0;
  double z_imaginary = 0.0;
  int iterations = 0;

  if (width == 1) {
    real = -2.0;
  } else {
    real = -2.0 + 3.0 * column / (width - 1);
  }

  if (height == 1) {
    imaginary = -1.5;
  } else {
    imaginary = -1.5 + 3.0 * row / (height - 1);
  }

  while (z_real * z_real + z_imaginary * z_imaginary <= 4.0 &&
         iterations < max_iterations) {
    double new_real = z_real * z_real - z_imaginary * z_imaginary + real;
    double new_imaginary = 2.0 * z_real * z_imaginary + imaginary;

    z_real = new_real;
    z_imaginary = new_imaginary;
    iterations++;
  }

  return (int)(iterations * 255.0 / max_iterations);
}

void calculate_row(int row) {
  int column;

  for (column = 0; column < width; column++) {
    image[(size_t)row * width + column] = pixel_value(column, row);
  }
}

void calculate_serial(void) {
  int row;

  for (row = 0; row < height; row++) {
    calculate_row(row);
  }
}

void calculate_openmp(void) {
  int row;

  omp_set_num_threads(number_of_threads);

#pragma omp parallel for
  for (row = 0; row < height; row++) {
    calculate_row(row);
  }
}

void *pthread_worker_blocks(void *argument) {
  ThreadData *data = argument;
  int first_row = data->id * height / number_of_threads;
  int last_row = (data->id + 1) * height / number_of_threads;
  int row;

  for (row = first_row; row < last_row; row++) {
    calculate_row(row);
  }

  return NULL;
}

int calculate_pthreads_blocks(void) {
  pthread_t *threads;
  ThreadData *data;
  int created = 0;
  int success = 1;
  int i;

  if ((size_t)number_of_threads > SIZE_MAX / sizeof(pthread_t) ||
      (size_t)number_of_threads > SIZE_MAX / sizeof(ThreadData)) {
    save_error("Erro: o numero de threads e muito grande.");
    return 0;
  }

  threads = malloc((size_t)number_of_threads * sizeof(pthread_t));
  data = malloc((size_t)number_of_threads * sizeof(ThreadData));

  if (threads == NULL || data == NULL) {
    save_error("Erro ao alocar memoria para as threads.");
    free(threads);
    free(data);
    return 0;
  }

  for (i = 0; i < number_of_threads; i++) {
    data[i].id = i;

    if (pthread_create(&threads[i], NULL, pthread_worker_blocks, &data[i]) !=
        0) {
      save_error("Erro ao criar uma thread.");
      success = 0;
      break;
    }
    created++;
  }

  for (i = 0; i < created; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      save_error("Erro ao aguardar uma thread.");
      success = 0;
    }
  }

  free(threads);
  free(data);
  return success;
}

int write_image(const char *filename) {
  FILE *file = fopen(filename, "w");
  int row;
  int column;

  if (file == NULL) {
    save_error("Erro ao criar um arquivo de imagem.");
    return 0;
  }

  for (row = 0; row < height; row++) {
    for (column = 0; column < width; column++) {
      char separator = column == width - 1 ? '\n' : ' ';

      if (fprintf(file, "%d%c", image[(size_t)row * width + column],
                  separator) < 0) {
        save_error("Erro ao escrever em um arquivo de imagem.");
        fclose(file);
        return 0;
      }
    }
  }

  if (fclose(file) != 0) {
    save_error("Erro ao fechar um arquivo de imagem.");
    return 0;
  }

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

  calculate_serial();
  if (!write_image(SERIAL_FILE)) {
    free(image);
    return EXIT_FAILURE;
  }

  calculate_openmp();
  if (!write_image(OPENMP_FILE)) {
    free(image);
    return EXIT_FAILURE;
  }

  if (!calculate_pthreads_blocks()) {
    free(image);
    return EXIT_FAILURE;
  }
  if (!write_image(PTHREADS1_FILE)) {
    free(image);
    return EXIT_FAILURE;
  }

  free(image);
  fclose(error_file);
  return EXIT_SUCCESS;
}
