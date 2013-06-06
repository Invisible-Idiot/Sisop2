#!/bin/bash

function run10Times {
	for x in 0 1 2 3 4 5 6 7 8 9; do
		./mmul $1
	done
}

for n in 1 2 4 8; do
	(time run10Times $n) &> time${n}.txt
done
