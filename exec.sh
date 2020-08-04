#!/bin/bash
#Execute aqui pelo script.
#Parâmetros
# ./gol 128 128 1000 1 0.1
#        |   |    |  | |-> probabilidade da célula ser 1
#        |   |    |  |---> 1 grava em arquivo/0 não grava saída no arquivo
#        |   |    |------> passos de tempo
#        |   |-----------> largura
#        |---------------> altura

time ./gol-parallel 1024 1024 5000 1 0.1 $1
