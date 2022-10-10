#!/bin/sh

if [ -d summaries ]; then
	echo "Found summaries"
else
	echo "summaries directory not found. Maybe create it first??"
	exit 1
fi

mkdir -p medians

for it in 1 2 3 4 5; do

echo MEDIAN FROM PARTIALS OF summaries/dense.sum TO medians/dense.med
sort -n summaries/dense.sum | awk -v OFMT='%.2f' '{a[i++]=$1;} END {print a[int(i/2)];}' > medians/dense.med

for sp in 60 70 80 85 90 95 99; do
	echo MEDIAN FROM PARTIALS OF summaries/sparse_$sp.sum TO medians/sparse_$sp.med
	sort -n summaries/sparse_$sp.sum | awk -v OFMT='%.2f' '{a[i++]=$1;} END {print a[int(i/2)];}' > medians/sparse_$sp.med

	echo MEDIAN FROM PARTIALS OF summaries/p2p_$sp.sum TO medians/p2p_$sp.med
	sort -n summaries/p2p_$sp.sum | awk -v OFMT='%.2f' '{a[i++]=$1;} END {print a[int(i/2)];}' > medians/p2p_$sp.med
done
done

