mkdir -p statistics-temp-metis
mkdir -p statistics-temp-metis/$1

qs[1]=0.9
qs[2]=0.75
qs[3]=0.5
qs[4]=0.25
qs[5]=0.1

for q in ${qs[@]}; do
	echo $q
	./calculate_stats graphs_compare_temp_random/$1/$1.temp.$q.surface.dat /home/avinash/graphs-SA-metis/$1.surface.dat statistics-temp-metis/$1/$1.$q.stats
done
