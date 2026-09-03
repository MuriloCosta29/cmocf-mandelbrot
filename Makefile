# Iniciais do e-mail em letras minusculas. Muda o nome dos arquivos .pgm.
LOGIN = cmocf

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 -fopenmp -pthread -DLOGIN='"$(LOGIN)"'

all: mandelbrot

mandelbrot: mandelbrot.c
	$(CC) $(CFLAGS) mandelbrot.c -o mandelbrot

clean:
	rm -f mandelbrot mandelbrot_$(LOGIN)_*.pgm times.txt errors.txt

.PHONY: all clean
