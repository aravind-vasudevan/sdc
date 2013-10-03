#!/bin/bash

outputFileName="tutu"

echo " " > $outputFileName

for fileName in $1/*.log
do
	`grep -i SDC/HEFT $fileName >> $outputFileName`
done

#while read line
#do
#	runConfig=`awk -F '=' '{print $1}'`
#	ratio=`awk -F '=' '{print $2}'`
#	echo $runConfig.$ratio
#done < $fileName
