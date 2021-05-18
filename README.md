# MC504-projeto2

## Problema dos Monges Copistas

Trata-se de uma adaptação do problema "search-insert-delete", na qual existem três tipos de monges, que devem fazer acessos a uma lista ligada (linked_list.h) seguindo essas regras:

- Monges leitores: podem acessar a lista simultaneamente com leitores e escritores.
- Monges escritores: mutualmente exclusivos com outros escritores e apagadores.
- Monges apagadores: mutualmente exclusivos com todos os outros monges.

## Solução

O problema foi resolvido utilizando semáforos (monk.h):

- Para evitar _starvation_ de um apagador, escritores e leitores devem passar por uma catraca que é bloqueada pelo apagador.
- Tanto escritores quanto leitores possuem switches que impedem o apagador de continuar se houver algum deles.
- O escritor possui um mutex para garantir a exclusão mútua com outros apagadores

## Visualização

A visualização e execução do programa foram feitas na main.c utilizando a biblioteca ncurses.


## Compilando
Requerimentos em Ubuntu:
```bash
sudo apt-get install libncurses5-dev
```
Compilando
```bash
gcc -pthread *.c -lm -o app.out -Wall -lncurses
```
