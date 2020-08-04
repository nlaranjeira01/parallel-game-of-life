#!/bin/bash

args="128 128 1000 1 0.1"

make

time ./gol $args
time ./gol-parallel $args

echo

cmp game_of_life.txt game_of_life_parallel.txt
exit_status=$?

if [ "$exit_status" == "0" ]; then
	echo "PROGRAM WORKING CORRECTLY"
else
	echo "FILES ARE DIFFERENT!!!!!!"
fi

exit $exit_status

