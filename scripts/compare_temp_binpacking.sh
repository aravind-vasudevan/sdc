#mkdir -p graphs_compare_temp_binpacking;
#mkdir graphs_compare_temp_binpacking/$1;
#echo " " > graphs_compare_temp_binpacking/$1/$1.surface.dat

#let mincost=99999999
#let flag="true"
#let previoustile=0
#
qs[1]=0.9
qs[2]=0.75
qs[3]=0.5
qs[4]=0.25
qs[5]=0.1
#
for q in ${qs[@]}; do
echo $q
#	for i in /home/aravind/Work/Simulated-Annealing/outputs/*random*$q*/*$1* ; do
#	#	for counter in $(seq 2 2 20)
#	#    do
#	#	echo $i
#		wholename=$(echo "$i" | awk -F / '{print $8}')
#		tile=$(echo "$wholename" | awk -F . '{print $4}')
#		counter1=$(echo "$wholename" | awk -F . '{print $6}')
#		counter2=$(echo "$wholename" | awk -F . '{print $7}')
#	#	echo $wholename
#		let totalnodes=$counter1*$counter2
#		finalstring=$(grep 'FinalCost' $i)
#		finalcost=$(echo "$finalstring" | awk -F : '{print $2}')
#		if [ "$flag" == "true" ]; then
#			$flag="false"
#			let previoustile=$tile
#		fi
#		if [ "$tile" -ne "$previoustile" ]; then
#			echo " " >> graphs_compare_temp_binpacking/$1/$1.random.$q.surface.dat
#		fi
#		if [ -z "$finalcost" ]; then
#			echo $wholename "--" $tile "--" $totalnodes "--" $finalcost
#		else
#			echo $wholename "--" $tile "--" $totalnodes "--" $finalcost
#			echo $tile $totalnodes $finalcost >> graphs_compare_temp_binpacking/$1/$1.random.$q.surface.dat	
#		fi
#		let previoustile=$tile
#	#    done
#	done
#
#	for i in /home/aravind/Work/Simulated-Annealing/outputs/*temp*$q*/*$1* ; do
#	#	for counter in $(seq 2 2 20)
#	#    do
#	#	echo $i
#		wholename=$(echo "$i" | awk -F / '{print $8}')
#		tile=$(echo "$wholename" | awk -F . '{print $4}')
#		counter1=$(echo "$wholename" | awk -F . '{print $6}')
#		counter2=$(echo "$wholename" | awk -F . '{print $7}')
#	#	echo $wholename
#		let totalnodes=$counter1*$counter2
#		finalstring=$(grep 'FinalCost' $i)
#		finalcost=$(echo "$finalstring" | awk -F : '{print $2}')
#		if [ "$flag" == "true" ]; then
#			$flag="false"
#			let previoustile=$tile
#		fi
#		if [ "$tile" -ne "$previoustile" ]; then
#			echo " " >> graphs_compare_temp_binpacking/$1/$1.temp.$q.surface.dat
#		fi
#		if [ -z "$finalcost" ]; then
#			echo $wholename "--" $tile "--" $totalnodes "--" $finalcost
#		else
#			echo $wholename "--" $tile "--" $totalnodes "--" $finalcost
#			echo $tile $totalnodes $finalcost >> graphs_compare_temp_binpacking/$1/$1.temp.$q.surface.dat	
#		fi
#		let previoustile=$tile
#	#    done
#	done

	echo "Generating gnuplot file..."
	echo "reset" > graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "unset key" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set term postscript eps enhanced \"Sans-Serif\" 18 color" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set samples 200" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set isosamples 200" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set grid" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set dgrid3d 100,100" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	#echo "set contour surface" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set cntrparam order 10" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set cntrparam linear" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	#echo "set hidden3d" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	#echo "" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set pointsize 2.5" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
#	echo "set term png enhanced" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
#	echo "set grid" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
#	echo "set dgrid3d 500,100" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
#	echo "set hidden3d" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	
	echo "set output \"graphs_compare_temp_binpacking/"$1"/"$1"_"$q"_surface.eps\"" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set xlabel \"Vector strip size\" rotate parallel" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set ylabel \"# of nodes\" rotate by 30" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set zlabel \"Log_{10}(sec)\" rotate left" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set log z" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set xrange[50:10]" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set label 1 \"Hetrogeneous Binpacking\" at screen 0.6, screen 0.82 nopoint tc lt 5 font \"Bold,16\" " >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "set label 2 \"Our SA Approach\" at screen 0.6, screen 0.79 nopoint tc lt 3 font \"Bold,16\" " >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "splot \"/home/aravind/Work/Simulated-Annealing/graphs_compare_temp_binpacking/"$1"/"$1".temp."$q".surface.dat\" with lines lt 1 lc rgb \"#332288\", \"/home/aravind/Work/Heterogeneous-Bin-Packing/graphs-for-SA/"$1"/"$1".surface.dat\" with lines lt 1 lc rgb \"#88CCEE\"" >> graphs_compare_temp_binpacking/$1/$1.surface.gnuplot
	echo "Plotting..."
	gnuplot graphs_compare_temp_binpacking/$1/$1.surface.gnuplot


done
