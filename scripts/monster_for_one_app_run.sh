mkdir -p new-outputs-with-logs;
mkdir -p new-outputs-with-logs/{random-q-0.9-outputs,random-q-0.75-outputs,random-q-0.5-outputs,random-q-0.25-outputs,random-q-0.1-outputs}; 
mkdir -p new-outputs-with-logs/{temperature-q-0.9-outputs,temperature-q-0.75-outputs,temperature-q-0.5-outputs,temperature-q-0.25-outputs,temperature-q-0.1-outputs}; 
f1()
{
	for topologygraph in /home/avinash/SMALL_METS/* ; do
		topology=$(echo "$topologygraph" | awk -F . '{print $2"."$3}')
		for i in /home/avinash/RESULTS_IPDPS_2013/NEW_GRAPHS_TILED/*$1*
		do
			name=$(echo "$i" | awk -F / '{print $5"."$6}')
			/home/aravind/Work/Simulated-Annealing/SimulatedAnnealing $i $topologygraph 3 0.9 1 > new-outputs-with-logs/random-q-0.9-outputs/"$name"."$topology".log && mv bestCost1 new-outputs-with-logs/random-q-0.9-outputs/bestCost.$name.$topology && mv currentTemperature1 new-outputs-with-logs/random-q-0.9-outputs/currentTemperature.$name.$topology
			echo "$name"."$topology"
			echo $i
		done
	done
}

f2()
{
	for topologygraph in /home/avinash/SMALL_METS/* ; do
		topology=$(echo "$topologygraph" | awk -F . '{print $2"."$3}')
		for i in /home/avinash/RESULTS_IPDPS_2013/NEW_GRAPHS_TILED/*$1* 
		do
			name=$(echo "$i" | awk -F / '{print $5"."$6}')
			/home/aravind/Work/Simulated-Annealing/SimulatedAnnealing $i $topologygraph 3 0.75 2 > new-outputs-with-logs/random-q-0.75-outputs/"$name"."$topology".log && mv bestCost2 new-outputs-with-logs/random-q-0.75-outputs/bestCost.$name.$topology && mv currentTemperature2 new-outputs-with-logs/random-q-0.75-outputs/currentTemperature.$name.$topology
			echo "$name"."$topology"
		done
	done
}

f3()
{
	for topologygraph in /home/avinash/SMALL_METS/* ; do
		topology=$(echo "$topologygraph" | awk -F . '{print $2"."$3}')
		for i in /home/avinash/RESULTS_IPDPS_2013/NEW_GRAPHS_TILED/*$1* 
		do
			name=$(echo "$i" | awk -F / '{print $5"."$6}')
			/home/aravind/Work/Simulated-Annealing/SimulatedAnnealing $i $topologygraph 3 0.5 3 > new-outputs-with-logs/random-q-0.5-outputs/"$name"."$topology".log && mv bestCost3 new-outputs-with-logs/random-q-0.5-outputs/bestCost.$name.$topology && mv currentTemperature3 new-outputs-with-logs/random-q-0.5-outputs/currentTemperature.$name.$topology
			echo "$name"."$topology"
		done
	done
}

f4()
{
	for topologygraph in /home/avinash/SMALL_METS/* ; do
		topology=$(echo "$topologygraph" | awk -F . '{print $2"."$3}')
		for i in /home/avinash/RESULTS_IPDPS_2013/NEW_GRAPHS_TILED/*$1* 
		do
			name=$(echo "$i" | awk -F / '{print $5"."$6}')
			/home/aravind/Work/Simulated-Annealing/SimulatedAnnealing $i $topologygraph 3 0.25 4 > new-outputs-with-logs/random-q-0.25-outputs/"$name"."$topology".log && mv bestCost4 new-outputs-with-logs/random-q-0.25-outputs/bestCost.$name.$topology && mv currentTemperature4 new-outputs-with-logs/random-q-0.25-outputs/currentTemperature.$name.$topology
			echo "$name"."$topology"
		done
	done
}

f5()
{
	for topologygraph in /home/avinash/SMALL_METS/* ; do
		topology=$(echo "$topologygraph" | awk -F . '{print $2"."$3}')
		for i in /home/avinash/RESULTS_IPDPS_2013/NEW_GRAPHS_TILED/*$1* 
		do
			name=$(echo "$i" | awk -F / '{print $5"."$6}')
			/home/aravind/Work/Simulated-Annealing/SimulatedAnnealing $i $topologygraph 3 0.1 5 > new-outputs-with-logs/random-q-0.1-outputs/"$name"."$topology".log && mv bestCost5 new-outputs-with-logs/random-q-0.1-outputs/bestCost.$name.$topology && mv currentTemperature5 new-outputs-with-logs/random-q-0.1-outputs/currentTemperature.$name.$topology
			echo "$name"."$topology"
		done
	done
}

f6()
{
	for topologygraph in /home/avinash/SMALL_METS/* ; do
		topology=$(echo "$topologygraph" | awk -F . '{print $2"."$3}')
		for i in /home/avinash/RESULTS_IPDPS_2013/NEW_GRAPHS_TILED/*$1* 
		do
			name=$(echo "$i" | awk -F / '{print $5"."$6}')
			/home/aravind/Work/Simulated-Annealing/SimulatedAnnealing $i $topologygraph 1 0.9 6 > new-outputs-with-logs/temperature-q-0.9-outputs/"$name"."$topology".log && mv bestCost6 new-outputs-with-logs/temperature-q-0.9-outputs/bestCost.$name.$topology && mv currentTemperature6 new-outputs-with-logs/temperature-q-0.9-outputs/currentTemperature.$name.$topology
			echo "$name"."$topology"
		done
	done
}

f7()
{
	for topologygraph in /home/avinash/SMALL_METS/* ; do
		topology=$(echo "$topologygraph" | awk -F . '{print $2"."$3}')
		for i in /home/avinash/RESULTS_IPDPS_2013/NEW_GRAPHS_TILED/*$1* 
		do
			name=$(echo "$i" | awk -F / '{print $5"."$6}')
			/home/aravind/Work/Simulated-Annealing/SimulatedAnnealing $i $topologygraph 1 0.75 7 > new-outputs-with-logs/temperature-q-0.75-outputs/"$name"."$topology".log && mv bestCost7 new-outputs-with-logs/temperature-q-0.75-outputs/bestCost.$name.$topology && mv currentTemperature7 new-outputs-with-logs/temperature-q-0.75-outputs/currentTemperature.$name.$topology
			echo "$name"."$topology"
		done
	done
}

f8()
{
	for topologygraph in /home/avinash/SMALL_METS/* ; do
		topology=$(echo "$topologygraph" | awk -F . '{print $2"."$3}')
		for i in /home/avinash/RESULTS_IPDPS_2013/NEW_GRAPHS_TILED/*$1* 
		do
			name=$(echo "$i" | awk -F / '{print $5"."$6}')
			/home/aravind/Work/Simulated-Annealing/SimulatedAnnealing $i $topologygraph 1 0.5 8 > new-outputs-with-logs/temperature-q-0.5-outputs/"$name"."$topology".log && mv bestCost8 new-outputs-with-logs/temperature-q-0.5-outputs/bestCost.$name.$topology && mv currentTemperature8 new-outputs-with-logs/temperature-q-0.5-outputs/currentTemperature.$name.$topology
			echo "$name"."$topology"
		done
	done
}

f9()
{
	for topologygraph in /home/avinash/SMALL_METS/* ; do
		topology=$(echo "$topologygraph" | awk -F . '{print $2"."$3}')
		for i in /home/avinash/RESULTS_IPDPS_2013/NEW_GRAPHS_TILED/*$1* 
		do
			name=$(echo "$i" | awk -F / '{print $5"."$6}')
			/home/aravind/Work/Simulated-Annealing/SimulatedAnnealing $i $topologygraph 1 0.25 9 > new-outputs-with-logs/temperature-q-0.25-outputs/"$name"."$topology".log && mv bestCost9 new-outputs-with-logs/temperature-q-0.25-outputs/bestCost.$name.$topology && mv currentTemperature9 new-outputs-with-logs/temperature-q-0.25-outputs/currentTemperature.$name.$topology
			echo "$name"."$topology"
		done
	done
}

f10()
{
	for topologygraph in /home/avinash/SMALL_METS/* ; do
		topology=$(echo "$topologygraph" | awk -F . '{print $2"."$3}')
		for i in /home/avinash/RESULTS_IPDPS_2013/NEW_GRAPHS_TILED/*$1* 
		do
			name=$(echo "$i" | awk -F / '{print $5"."$6}')
			/home/aravind/Work/Simulated-Annealing/SimulatedAnnealing $i $topologygraph 1 0.1 0 > new-outputs-with-logs/temperature-q-0.1-outputs/"$name"."$topology".log && mv bestCost0 new-outputs-with-logs/temperature-q-0.1-outputs/bestCost.$name.$topology && mv currentTemperature0 new-outputs-with-logs/temperature-q-0.1-outputs/currentTemperature.$name.$topology
			echo "$name"."$topology"
		done
	done
}

f1 $1 &
f2 $1 &
f3 $1 &
f4 $1 &
f5 $1 &
f6 $1 &
f7 $1 &
f8 $1 &
f9 $1 &
f10 $1 &
