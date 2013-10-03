mkdir -p outputs-cost-variation

echo "set term png" > output-cost-variation/temperature."$1"."$2"."$3"."$4".gnuplot
echo "set output \"output-cost-variation/temperature."$1"."$2"."$3"."$4".png\"" >> output-cost-variation/temperature."$1"."$2"."$3"."$4".gnuplot
echo "set grid" >> output-cost-variation/temperature."$1"."$2"."$3"."$4".gnuplot
echo "set xlabel \"Temperature\"" >> output-cost-variation/temperature."$1"."$2"."$3"."$4".gnuplot
echo "set ylabel \"Latency(Cost)\"" >> output-cost-variation/temperature."$1"."$2"."$3"."$4".gnuplot
echo "plot \\" >> output-cost-variation/temperature."$1"."$2"."$3"."$4".gnuplot

qs[1]=0.9
qs[2]=0.75
qs[3]=0.5
qs[4]=0.25
qs[5]=0.1
qq=0.1
for q in ${qs[@]}; do
if [ $q != 0.1 ]; then
		echo "\""$(ls outputs-with-logs/temperature-q-"$q"-outputs/bestc/bestcost.new_graphs_tiled."$1"*."$2"*"$3"."$4")"\" t 'q=$q', \\ " >> output-cost-variation/temperature."$1"."$2"."$3"."$4".gnuplot
	else
		echo "\""$(ls outputs-with-logs/temperature-q-"$q"-outputs/bestc/bestcost.new_graphs_tiled."$1"*."$2"*"$3"."$4")"\" t 'q=$q' " >> output-cost-variation/temperature."$1"."$2"."$3"."$4".gnuplot
	fi
done

#gnuplot output-cost-variation/temperature."$1"."$2"."$3"."$4".gnuplot
