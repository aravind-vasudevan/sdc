executable=/home/aravind/Work/Task-Partitioning/SDC/SDC.exe
applicationgraphsdir=/home/aravind/Work/Task-Partitioning/SDC/input/random-app-graphs
topologygraphsdir=/home/aravind/Work/Task-Partitioning/SDC/input/random-topo-graphs/NEW_TOPOS
destinationdir=output

mkdir -p $destinationdir;

f()
{
	mkdir -p $destinationdir/$1;
	mkdir -p $destinationdir/$1/bestCost;
	mkdir -p $destinationdir/$1/changeInCost;
	mkdir -p $destinationdir/$1/SAmaps;
	for i in $applicationgraphsdir/*.grf ; do
		name=$(echo "$i" | awk -F / '{print $9}')
		for topologygraph in $topologygraphsdir/*.grf ; do
			topology=$(echo "$topologygraph" | awk -F . '{print $2"."$3}')
			if [ -e "$destinationdir/$1/$name.$topology.log" ]
			then
				temp=`grep -i 'Final' $destinationdir/$1/"$name"."$topology".log`
				if [ -z "$temp" ]
				then
					$executable $i $topologygraph $2 > $destinationdir/$1/"$name"."$topology".log && mv bestCost"$2" $destinationdir/$1/bestCost/bestCost.$name.$topology && mv changeInCost"$2" $destinationdir/$1/changeInCost/changeInCost.$name.$topology && mv map"$2" $destinationdir/$1/SAmaps/map.$name.$topology
				fi
			else
				$executable $i $topologygraph $2 > $destinationdir/$1/"$name"."$topology".log && mv bestCost"$2" $destinationdir/$1/bestCost/bestCost.$name.$topology && mv changeInCost"$2" $destinationdir/$1/changeInCost/changeInCost.$name.$topology && mv map"$2" $destinationdir/$1/SAmaps/map.$name.$topology
			fi
			echo "$topologygraph"="$name"="$topology"
		done
	done
}

SA-random-parallel()
{
	f random-q-0.1-outputs 1 0.1 1 &
	f random-q-0.25-outputs 1 0.25 2 &
	f random-q-0.5-outputs 1 0.5 3 &
	f random-q-0.75-outputs 1 0.75 4 &
	f random-q-0.9-outputs 1 0.9 5 
}

SA-temperature-parallel()
{
	f temperature-q-0.1-outputs 3 0.1 6 &
	f temperature-q-0.25-outputs 3 0.25 7 &
	f temperature-q-0.5-outputs 3 0.5 8 &
	f temperature-q-0.75-outputs 3 0.75 9 &
	f temperature-q-0.9-outputs 3 0.9 10 
}

#serial()
#{
#	f mutation-random-outputs 1 1 1 
#	f mutation-same-outputs 1 2 2 
#	f crossover-random-outputs 2 1 3 
#	f crossover-same-outputs 2 2 4 
#	f combination-random-outputs 3 1 5 
#	f combination-same-outputs 3 2 6 
#}

metis-serial()
{
	f metis 1 1 1
}

SA-serial()
{
	f temperature-q-0.5-outputs 3 0.5 1
#	f random-q-0.5-outputs 1 0.5 1 &
}

SDC-serial()
{
	f SDCvsHEFT-randomappvsrandnewtopos-run1 1
}

#SA-random-parallel
#SA-temperature-parallel 
#SA-serial
#metis-serial
#serial

SDC-serial

#f temperature-q-0.5-outputs 3 0.5 8 &
#f random-q-0.5-outputs 1 0.5 3 
