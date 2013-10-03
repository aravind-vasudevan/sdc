mkdir -p statistics-temp-random
mkdir -p statistics-temp-random/$1

qs[1]=0.9
qs[2]=0.75
qs[3]=0.5
qs[4]=0.25
qs[5]=0.1

for q in ${qs[@]}; do
	echo $q
	./calculate_stats graphs_compare_temp_random/$1/$1.temp.$q.surface.dat graphs_compare_temp_random/$1/$1.random.$q.surface.dat statistics-temp-random/$1/$1.$q.stats
done
