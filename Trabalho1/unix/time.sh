#!/bin/bash

function run10Times {
	mv -i input/in1$1.txt in1.txt
	mv -i input/in2$1.txt in2.txt

	for x in 0 1 2 3 4 5 6 7 8 9; do
		./mmul $2
		mv -i out.txt output/out$1_$2_#$x.txt
	done

	mv -i in1.txt input/in1$1.txt
	mv -i in2.txt input/in2$1.txt
}

for i in 0 1 2 3 4 5 6 7 8 9; do
	for n in 1 2 4 8; do
		(time run10Times $i $n) &> output/time${i}_${n}.txt
	done
done
