mkdir -p purelyrandom-verynew-outputs temperature-verynew-outputs;
mkdir -p purelyrandom-new-outputs temperature-new-outputs;

for counter in $(seq 2 2 8) ; do
    for i in /home/avinash/RESULTS_IPDPS_2013/VERYNEW_GRAPHS_TILED/*
    do
        name=$(echo "$i" | awk -F / '{print $5"."$6}')
        ./SimulatedAnnealing $i Mets/met."$counter"."$counter".grf > purelyrandom-verynew-outputs/"$name"."$counter"."$counter".log 1 &
        ./SimulatedAnnealing $i Mets/met."$counter"."$counter".grf > temperature-verynew-outputs/"$name"."$counter"."$counter".log 3 &
		echo topology"$counter"."$counter"
    done
done

for counter in $(seq 2 2 8) ; do
    for i in /home/avinash/RESULTS_IPDPS_2013/NEW_GRAPHS_TILED/*
    do
        name=$(echo "$i" | awk -F / '{print $5"."$6}')
        ./SimulatedAnnealing $i Mets/met."$counter"."$counter".grf > purelyrandom-new-outputs/"$name"."$counter"."$counter".log 1 &
        ./SimulatedAnnealing $i Mets/met."$counter"."$counter".grf > temperature-new-outputs/"$name"."$counter"."$counter".log 3 &
		echo topology"$counter"."$counter"
    done
done
