#ifndef _SDC_HPP
#define _SDC_HPP

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <cstring>
#include <utility>
#include <cassert>

#include "GraphIR.hpp"
#include "Floyd-Warshall.hpp"
#include "MD5Hash.hpp"
#include "Definitions.hpp"

class ComparisonDataStructures
{
	public:
	std::map < int64_t, int64_t > finalMapping;
	std::vector < double > ranku, rankd;
	std::vector < double > averageComputation;
	double averageCommStartupTime, averageBandwidth;
	std::map < int64_t, int64_t > HEFTMapping, CPOPMapping;
	std::vector < std::pair < double, double > > HEFT_times, CPOP_times; // .first is EST, .second is EFT
	std::vector < double > avail;
};

// This data structure holds the path information for the mapping process. This is one cell in the table.
class MetaCell
{
	public:
	double myCost, pathCost;
	std::vector < std::pair< int64_t,int64_t > > path;
	MetaCell();
};

class SDC : public ComparisonDataStructures
{
	public:

	std::vector < std::vector < int64_t > > capableProcessors;
	std::vector < double > percentageCapableProcessors;
	std::vector < double > percentageIncapableProcessors;
	std::vector < double > weightOfTasks;
	std::vector < double > taskRanking;
	std::vector < std::pair < double, double > > SDC_times;

	std::map < int64_t, int64_t > initialS, currentS, bestS, finalMapping, criticalPathMapping;
	std::map < int64_t, int64_t > SDCMapping;
	std::vector < std::pair < double, double > > startAndEndTimes; // .first is EST, .second is EFT
	std::vector < std::vector < double > > executionTimes;
	std::vector < std::vector < int64_t > > allPaths;
	std::vector < int64_t > criticalPath;
	vector < double > pathCosts, pathCostsUsingHEFT;
	int64_t numberOfPaths;
	std::vector < std::vector < MetaCell > > metaTable;
	std::vector < double > communicationSetupTime; 
	double initialC, currentC, bestC, finalCost;
	int64_t numberOfTasks, numberOfResources;
	int64_t slowestResourceID, fastestResourceID, lightestTaskID, heaviestTaskID;
	double slowestResourceValue, fastestResourceValue, lightestTaskValue, heaviestTaskValue;
	double startingTemperature, currentTemperature, endingTemperature, k, q, gamma, L;

	GraphIR *applicationGraph, *topologyGraph, *topologyCopy, *applicationGraphTranspose; 
	FloydWarshall *topo;
 	int _MOVE_CHOICE;  // 1 - for random move function; 2 - constant gamma; 3 - variable

	SDC();
	~SDC();

	// SDC Generic methods
	double evaluateMapping( std::map <int64_t, int64_t> currentMapping, int64_t numberOfTasks, int64_t numberOfResources );
	double revisedEvaluateMappingWithDependencies( std::map <int64_t, int64_t> currentMapping, int64_t numberOfTasks, int64_t numberOfResources );
	double evaluateMappingWithDependencies( std::map <int64_t, int64_t> currentMapping, int64_t numberOfTasks, int64_t numberOfResources );
	double evaluateExecutionTime( int64_t task, int64_t resource );
	int64_t initializeVariables();
	int64_t mapTasksToResources( char* applicationName );
	double run( char* applicationName, char* topologyName, int choice );

	// SDC methods
	double calculateRanks( int );
	double calculateTaskRanking( int64_t );
	double calculateAEFT( int64_t, int64_t );
	double calculateEST( int64_t, int64_t );

	// HEFT methods
	int HEFT_initialize();
	int HEFT_calculateRanksUp();
	int HEFT_calculateRanksDown();
	int HEFT_calculateSchedule( int c );
	int HEFT_run();
	int HEFT_printRanks( int n );
	double HEFT_calculateEST( int64_t, int64_t );
};

#endif