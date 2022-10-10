#!/bin/sh

mkdir -p logs

INPUT=inputs/input_long.txt

for it in 1 2 3 4 5; do

./dense.out $INPUT > logs/dense.$it.log

for sp in 60 70 80 85 90 95 99; do
	echo RUNNING LIBRSB FOR $sp% SPARSITY $it ITERATION
	./sparse_$sp.out $INPUT > logs/sparse_$sp.$it.log

	echo RUNNING P2P FOR $sp% SPARSITY $it ITERATION
	./p2p_$sp.out $INPUT > logs/p2p_$sp.$it.log
done
done
