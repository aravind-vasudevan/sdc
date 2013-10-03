executable=/home/aravind/Work/Task-Partitioning/Simulated-Annealing/SimulatedAnnealing
applicationgraphsdir=/home/aravind/Work/Task-Partitioning/random-app-graphs
#applicationgraphsdir=/home/servesh/hetrogpart/SA/256-app-graphs
topologygraphsdir=/home/aravind/Work/Task-Partitioning/random-topo-8-8-graphs
#destinationdir=/home/servesh/hetrogpart/SA/outputs-ISPA-randomapps
destinationdir=/home/aravind/Work/Task-Partitioning/Simulated-Annealing/outputs-12mins-8-8-graphs
DDIR=$destinationdir
TDIR=/home/servesh/hetrogpart/SA/tmp
EXEC=$executable
NTHREADS=40
TIME_BEFORE_KILL=1200

mkdir -p $destinationdir;

f()
{
    COUNTER=0;
    for (( c=0; c<=$NTHREADS; c++ ))
    do
        mkdir -p $TDIR/$c
    done

    echo > $DDIR/kill_list.log
    
	mkdir -p $destinationdir/$1;
	mkdir -p $destinationdir/$1/bestCost;
	mkdir -p $destinationdir/$1/changeInCost;
	for topologygraph in $topologygraphsdir/* ; do
		topology=$(echo "$topologygraph" | awk -F . '{print $2"."$3}')
		for i in $applicationgraphsdir/*
		do
			name=$(echo "$i" | awk -F / '{print $7}')
			
            cd $TDIR/$COUNTER
        
            echo "if [ -e \"$destinationdir/$1/$name.$topology.log\" ]" > $COUNTER.sh
			echo "then" >> $COUNTER.sh
			echo "temp=\`grep -i 'Final' $destinationdir/$1/$name.$topology.log\`" >> $COUNTER.sh
			echo "if [ -z \"$temp\" ]" >> $COUNTER.sh
			echo "then" >> $COUNTER.sh
			echo "$executable $i $topologygraph $2 $3 $4 > $destinationdir/$1/$name.$topology.log && mv bestCost$4 $destinationdir/$1/bestCost/bestCost.$name.$topology && mv changeInCost$4 $destinationdir/$1/changeInCost/changeInCost.$name.$topology" >> $COUNTER.sh
			echo "fi" >> $COUNTER.sh
			echo "else" >> $COUNTER.sh
			echo "$executable $i $topologygraph $2 $3 $4 > $destinationdir/$1/$name.$topology.log && mv bestCost$4 $destinationdir/$1/bestCost/bestCost.$name.$topology && mv changeInCost$4 $destinationdir/$1/changeInCost/changeInCost.$name.$topology" >> $COUNTER.sh
			echo "fi" >> $COUNTER.sh
            
            chmod 755 $COUNTER.sh
            ./$COUNTER.sh &

            COUNTER=$[$COUNTER +1]

            JOBS=0;
            if [[ $COUNTER == $NTHREADS ]] ; then
                for(( i=0; i<$(( $TIME_BEFORE_KILL / 10 )); i++ ))
                do
                    sleep 10
                    JOBS=0

                    for job in `ps -ef | grep -v grep | grep $EXEC | awk '{print $2 }'`
                    do
                        JOBS=$[$JOBS +1]
                    done

                    if (( $JOBS == 0 )) ; then
                        break
                    fi
                done

                if (( $JOBS > 0 )) ; then
                    for job in `ps -ef | grep -v grep | grep $EXEC | awk '{print $2 }'`
                    do
                        echo "Killing " $job
                        echo `ps -ef | grep -v grep | grep $EXEC | grep $job | awk '{print $9, $10, $11}'` >> $destinationdir/kill_list.log
                        kill -SIGTERM  $job
                    done
                fi
                COUNTER=0
            fi
			echo "$topologygraph"="$name"="$topology"
		done
	done

    for (( c=0; c<=$NTHREADS; c++ ))
    do
        rm -rf $TDIR/$c
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

#SA-random-parallel
#SA-temperature-parallel 
#metis-serial
#serial

#f temperature-q-0.5-outputs 3 0.5 8
f random-q-0.5-outputs 1 0.5 3 
