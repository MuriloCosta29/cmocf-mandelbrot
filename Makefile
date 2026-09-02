SYSTEM = $(shell uname -s)

ifeq ($(SYSTEM),Darwin)
CC = /opt/homebrew/opt/llvm/bin/clang
else
CC = gcc
endif

CFLAGS = -std=c11 -Wall -Wextra -O2 -fopenmp -pthread

mandelbrot: mandelbrot.c
	$(CC) $(CFLAGS) mandelbrot.c -o mandelbrot

clean:
	rm -f mandelbrot
