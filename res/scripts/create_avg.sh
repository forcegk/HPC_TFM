#!/bin/sh

if [ -d summaries ]; then
	echo "Found summaries"
else
	echo "summaries directory not found. Maybe create it first??"
	exit 1
fi

mkdir -p averages

for it in 1 2 3 4 5; do

echo AVERAGE FROM PARTIALS OF summaries/dense.sum TO averages/dense.avg
awk -v OFMT='%.2f' 'BEGIN{s=0;}{s+=$1;}END{print s/NR;}' summaries/dense.sum > averages/dense.avg

for sp in 60 70 80 85 90 95 99; do
	echo AVERAGE FROM PARTIALS OF summaries/sparse_$sp.sum TO averages/sparse_$sp.avg
	awk -v OFMT='%.2f' 'BEGIN{s=0;}{s+=$1;}END{print s/NR;}' summaries/sparse_$sp.sum > averages/sparse_$sp.avg

	echo AVERAGE FROM PARTIALS OF summaries/p2p_$sp.sum TO averages/p2p_$sp.avg
	awk -v OFMT='%.2f' 'BEGIN{s=0;}{s+=$1;}END{print s/NR;}' summaries/p2p_$sp.sum > averages/p2p_$sp.avg
done
done

