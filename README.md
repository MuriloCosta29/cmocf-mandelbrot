# Implementação 2 — Threads

Programa em C que calcula o conjunto de Mandelbrot usando quatro implementações:

- **Serial**;
- **OpenMP**;
- **Pthreads 1:** divide a imagem em blocos de linhas;
- **Pthreads 2:** divide as linhas de forma intercalada.

## Antes de compilar

Confira a linha `LOGIN = cmocf` no Makefile. Troque `cmocf` pelas iniciais do seu
e-mail, em letras minúsculas, caso elas sejam diferentes. O Makefile repassa esse
valor ao compilador, então os nomes dos arquivos de saída também são alterados.

## Compilação

```sh
make
```

O Makefile usa o `gcc`. No Linux, esse compilador oferece suporte a OpenMP e
Pthreads.

No macOS, o comando `gcc` normalmente executa o clang da Apple, que não aceita
`-fopenmp`. Para compilar nesse sistema, instale o clang do Homebrew
(`brew install llvm`) e informe o compilador na hora de rodar o make:

```sh
make CC=/opt/homebrew/opt/llvm/bin/clang
```

## Execução

```sh
./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]
```

Exemplo:

```sh
./mandelbrot 800 800 1000 4
```

Se `num_threads` for maior que a altura da imagem, ou maior que 1024, o programa
reduz o valor para um limite seguro em vez de falhar: mais de uma thread por linha
não traz ganho, e pedir milhares de threads ao sistema pode derrubar a execução.

O programa não escreve em `stdout` nem em `stderr`. Os erros são gravados em
`errors.txt`. Durante a execução, ele cria:

- `errors.txt`;
- `times.txt`;
- `mandelbrot_cmocf_serial.pgm`;
- `mandelbrot_cmocf_openmp.pgm`;
- `mandelbrot_cmocf_pthreads1.pgm`;
- `mandelbrot_cmocf_pthreads2.pgm`.

Os nomes acima mudam automaticamente quando a variável `LOGIN` do Makefile é
alterada.

## Limpeza

```sh
make clean
```
