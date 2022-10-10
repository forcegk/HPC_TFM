#!/bin/sh

mkdir -p summaries

for it in 1 2 3 4 5; do

echo OUTPUTING IT $it PARTIALS TO summaries/dense.sum
tail -n3 dense.$it.log | head -n1 | cut -d" " -f7 >> summaries/dense.sum

for sp in 60 70 80 85 90 95 99; do
	echo OUTPUTING IT $it PARTIALS TO summaries/sparse_$sp.sum
	tail -n3 sparse_$sp.$it.log | head -n1 | cut -d" " -f7 >> summaries/sparse_$sp.sum

	echo OUTPUTING IT $it PARTIALS TO summaries/p2p_$sp.sum
	tail -n3 p2p_$sp.$it.log | head -n1 | cut -d" " -f7 >> summaries/p2p_$sp.sum
done
done

