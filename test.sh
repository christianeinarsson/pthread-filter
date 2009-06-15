#!/bin/bash
# Usage: test

### Start main procedure

for n in 1 2 4 8 16 32 64 128
do
	for r in 10 50 100 500 1000
	do
		./a.out $n $r im1.ppm test.ppm > blur_n${n}_${r}.log
	done
done

for n in 1 2 4 8 16 32 64 128
do
	./a.out $n im1.ppm test.ppm > threshold_n${n}.log
done