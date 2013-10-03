#!/bin/bash

IP_APP_GRAPHS=random-app-graphs
NSYS=3;
MKDIR="mkdir -p";
PPND="SHAMROCK"
HDIR=`pwd`;

NO_OF_FILES=`ls -lrta $IP_APP_GRAPHS | wc -l`;
SPLIT=$[$NO_OF_FILES / NSYS];

echo "Splitting $NO_OF_FILES files into $SPLIT for $NSYS systems";

SYS=0;
COUNTER=0;

$MKDIR $PPND$SYS
cd $PPND$SYS

for app_graph in $IP_APP_GRAPHS/*.grf
do
    cp $app_graph .
    
    COUNTER=$[$COUNTER +1]

    if [[ $COUNTER == $SPLIT ]] ; then
        SYS=$[$SYS +1]
        cd ../
        $MKDIR $PPND$SYS
        cd $PPND$SYS

        COUNTER=0
    fi
done


cd $HDIR

EXEC="./run-each-monster.sh"

hostnames=( "shamrock014" "shamrock031" "shamrock032" "shamrock057" "shamrock058" "shamrock059" "shamrock071" "shamrock072" "shamrock093" "shamrock094" "shamrock095" "shamrock096" "shamrock097" "shamrock098" "shamrock099" "shamrock100" "shamrock101" "shamrock102" "shamrock103" "shamrock105" "shamrock106" "shamrock107" "shamrock109" "shamrock113" "shamrock114" "shamrock115" "shamrock116" "shamrock117" "shamrock118" "shamrock131" "shamrock132" "shamrock133" "shamrock134" )

username=root

COUNTER=0;
for host in "${hostnames[@]}";
do
    ssh $username@$host nohup $HDIR/$EXEC $PPND$COUNTER > $HDIR/$COUNTER.out &
    COUNTER=$[$COUNTER +1]
    
    if [[ $COUNTER == $NSYS ]] ; then
        break;
    fi
done
