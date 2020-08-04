#!/bin/bash

#Execute aqui pelo script.
#Parâmetros
# ./gol 128 128 1000 1 0.1
#        |   |    |  | |-> probabilidade da célula ser 1
#        |   |    |  |---> 1 grava em arquivo/0 não grava saída no arquivo
#        |   |    |------> passos de tempo
#        |   |-----------> largura
#        |---------------> altura
args="128 128 1000 1 0.1"

make

time ./gol $args
time ./gol-parallel $args $1

echo

cmp game_of_life.txt game_of_life_parallel.txt
exit_status=$?

if [ "$exit_status" == "0" ]; then
	echo "PROGRAM WORKING CORRECTLY"
else
	echo "FILES ARE DIFFERENT!!!!!!"
fi

exit $exit_status

