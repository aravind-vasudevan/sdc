#ifndef HEFT_CPP
#define HEFT_CPP

#include "SDC.hpp"

using namespace std;

int
SDC :: HEFT_initialize()
{
//	cout<<"\n[HEFT_init] - entering";
	ranku.resize(numberOfTasks, 0);
	rankd.resize(numberOfTasks, 0);
	averageComputation.resize(numberOfTasks, 0);
	int i=0, j=0;
	for(; i<numberOfTasks; ++i)
	{
		double sum=0;
		for(j=0; j<numberOfResources; ++j)
			sum += executionTimes[i][j];
		averageComputation[i] = sum/numberOfTasks;
	}
//	cout<<"\n[HEFT_init] - avgCompute calculated\n";
	averageCommStartupTime = 0;
	averageBandwidth = 0;
	for(i=0; i<numberOfResources; ++i)
		averageCommStartupTime += communicationSetupTime[i];
//	cout<<"\n[HEFT_init] - SetupTime done; n="<<numberOfResources<<"\n";
	averageCommStartupTime = averageCommStartupTime/numberOfResources;
	int64_t count = 0;
	for(i=0; i<numberOfResources; ++i)
		for(int k=0; k<topologyCopy->connectivityMatrix[i].size(); ++k)
		{
			++count;
			averageBandwidth += topologyCopy->connectivityMatrix[i].list[k].second;
		}
	averageBandwidth = averageBandwidth/count;
	cout<<"\n[HEFT_init] - avgBW calculated"<<averageBandwidth<<"\n";
	for(i=0; i<numberOfTasks; ++i)
		if( applicationGraph->connectivityMatrix[i].size() == 0 ) // found leaf node
			ranku[i] = averageComputation[i];

//	cout<<"\n[HEFT_init] - Leaf node detection done\n";
//	// Creating inverse of the application graph just to maintain record of predecessors
//	for(i=0; i<numberOfTasks; ++i)
//	{
////		cout<<"\n[HEFT_init] - node "<<i<<" -> ";
////		fflush(stdout);
//		for(int k=0; k<applicationGraph->connectivityMatrix[i].size(); ++k)
//		{
////			cout<<applicationGraph->connectivityMatrix[i].list[k].first<<", ";
////			fflush(stdout);
//			pair < int64_t, double > tempPair;
//			tempPair.first = i;
//			tempPair.second = applicationGraph->connectivityMatrix[i].list[k].second;
//			applicationGraphTranspose->connectivityMatrix[applicationGraph->connectivityMatrix[i].list[k].first].list.push_back(tempPair);
////			applicationGraphTranspose->connectivityMatrix[j][i] = applicationGraph->connectivityMatrix[i][j];
//		}
//	}
//	cout<<"\n[HEFT_init] - Inverse done\n";
//	// sanity check - inverse of appgrf should be lower triangular
//	for(i=0; i<numberOfTasks; ++i)
//		for(int k=0; k<applicationGraphTranspose->connectivityMatrix[i].size(); ++k)
//			assert( i>applicationGraphTranspose->connectivityMatrix[i].list[k].first );

	// setting up variables used in scheduling
	avail.resize( numberOfResources, 0 );
	HEFT_times.resize(numberOfTasks);
	CPOP_times.resize(numberOfTasks);
//	cout<<"\n[HEFT_init] - exiting";
}

int
SDC :: HEFT_calculateRanksUp()
{
//	cout<<"\n[HEFT_calculateRanksUp] - entering";
	int i=0, j=0;
	for(i=numberOfTasks-1; i>=0; --i)
	{
		if( ranku[i] == 0 )
		{
			double max = 0;
			int numberOfSuccessors = applicationGraph->connectivityMatrix[i].size();
			for(j=0; j<numberOfSuccessors; ++j)
			{
				double temp;
				temp = averageCommStartupTime + applicationGraph->connectivityMatrix[i].list[j].second/averageBandwidth;
//				temp = applicationGraph->connectivityMatrix[i].list[j].second/averageBandwidth; // Uncomment this line and comment previous to use static upward rank.
				if(temp > max)
					max = temp;
			}
			ranku[i] = averageComputation[i] + max;
		}
	}
//	cout<<"\n[HEFT_calculateRanksUp] - exiting";
	
}

int
SDC :: HEFT_calculateRanksDown()
{
	
}

double
SDC :: HEFT_calculateEST( int64_t nodeToBeScheduled, int64_t processor )
{
//	cout<<"\n[HEFT_calculateEST] - Entering ("<<nodeToBeScheduled<<", "<<processor<<")";
	double result = 0;
	int64_t i=0, j=0, numberOfPredecessors=applicationGraphTranspose->connectivityMatrix[nodeToBeScheduled].size();
	if(numberOfPredecessors == 0)
	{
//		cout<<"\n[HEFT_calculateEST] - Parent node found. Returning.";fflush(stdout);
		result = 0;
	}
	else
	{
//		cout<<"\n[HEFT_calculateEST] - Not parent node. Preds="<<numberOfPredecessors;
		vector < double > readyTime;
		double max = 0;
		int64_t maxi;
		readyTime.resize(numberOfPredecessors, 0);
		for(i=0; i<numberOfPredecessors; ++i)
		{
			pair < int64_t, double > parent=applicationGraphTranspose->connectivityMatrix[nodeToBeScheduled].list[i];
//			cout<<" | "<<parent.first;
//			fflush(stdout);
			double commTime = 0;
			int64_t parentsProcessor = HEFTMapping[ parent.first ];
			if(parentsProcessor == processor)
				commTime = 0;
			else
				commTime = communicationSetupTime[parentsProcessor] + parent.second*topologyGraph->connectivityMatrix[parentsProcessor][processor];
			readyTime[i] = HEFT_times[ parent.first ].second + commTime;
			if( max<readyTime[i] )
			{
				max = readyTime[i];
				maxi = i;
			}
		}
		(avail[processor] > readyTime[maxi] ) ? result = avail[processor] : result = readyTime[maxi];
	}
//	cout<<"\n[HEFT_calculateEST] - Exiting";
	return result;
}

int
SDC :: HEFT_calculateSchedule(int c) //1-noninsertion; 2-insertion;
{
//	cout<<"\n[HEFT_calculateSchedule] - Entering";
	vector < int64_t > toBeScheduled;
	vector < int64_t >::iterator it;
	int64_t i=0, j=0;
	for(i=numberOfTasks-1; i>=0; --i)
		toBeScheduled.push_back(i);

	// Sort tasks in scheduling list by decreasing order of ranks
	for(i=0;i<numberOfTasks; ++i)
	{
		double maxRank = 0;
		int64_t maxJ = 0;
		bool found = false;
		for(j=i; j<numberOfTasks; ++j)
		{
			if(maxRank<ranku[toBeScheduled[j]])
			{
				found = true;
				maxRank = ranku[toBeScheduled[j]];
				maxJ = j;
			}
		}
		if(found == true)
		{
			int64_t temp = toBeScheduled[maxJ];
			toBeScheduled[maxJ] = toBeScheduled[i];
			toBeScheduled[i] = temp;
		}
	}
//	cout<<"\n[HEFT_calculateSchedule] - List of tasks sorted by rank\n";
//	for(i=0; i<numberOfTasks;++i)
//		cout<<"("<<toBeScheduled[i]<<", "<<ranku[toBeScheduled[i]]<<") | ";
//	cout<<"\n[HEFT_calculateSchedule] - toBeScheduled.size="<<toBeScheduled.size();
//	fflush(stdout);
	for(i=0; toBeScheduled.size()>0; ++i)
	{
//		cout<<"\ni="<<i;fflush(stdout);
		if(i>=toBeScheduled.size())
		{
			i=0;
//			cout<<"\n[HEFT_calculateSchedule] - wrapping index back to "<<i;
		}
		double min = _INF;
		int64_t minj = 0, nodeToBeScheduled = toBeScheduled[i];
		int64_t numberOfPredecessors = applicationGraphTranspose->connectivityMatrix[nodeToBeScheduled].size();
		vector < double > eft, est;
		est.resize(numberOfResources, 0);
		eft.resize(numberOfResources, 0);
//		cout<<"; nodeToBeSched="<<nodeToBeScheduled;
//		cout<<"; parents="<<numberOfPredecessors;fflush(stdout);
		for(int k=0; k<numberOfPredecessors; ++k)
			if(find(toBeScheduled.begin(), toBeScheduled.end(), applicationGraphTranspose->connectivityMatrix[nodeToBeScheduled].list[k].first) != toBeScheduled.end())
			{
//				cout<<"; exitLabel";fflush(stdout);
				goto exitLabel;
			}
//		cout<<"; N_r="<<numberOfResources<<"; ";fflush(stdout);
		for(j=0; j<numberOfResources; ++j)
		{
			est[j] = HEFT_calculateEST( nodeToBeScheduled, j );
//			cout<<"("<<j<<", "<<est[j];
			eft[j] = executionTimes[nodeToBeScheduled][j] + est[j];
//			cout<<", "<<eft[j]<<") | ";
			if( min > eft[j] )
			{
				min = eft[j];
				minj = j;
			}
		}
//		cout<<"++++chosen="<<minj;
		HEFT_times[nodeToBeScheduled].first = est[minj];
		HEFT_times[nodeToBeScheduled].second = eft[minj];
		HEFTMapping[nodeToBeScheduled] = minj;
		avail[minj] = HEFT_times[nodeToBeScheduled].second;

		// Removing from nodes to be scheduled
		it = find(toBeScheduled.begin(), toBeScheduled.end(), nodeToBeScheduled);
		toBeScheduled.erase(it);
		exitLabel:
		;
	}
//	cout<<"\n[HEFT_calculateSchedule] - Exiting";
}

int
SDC :: HEFT_printRanks( int c )
{
	cout<<"\n[HEFT_printRanks] - entering";
	int64_t i, j;
	switch(c)
	{
		case 1:
			for(i=0; i<numberOfTasks; ++i)
				cout<<"\n[printRanks("<<c<<")] - t["<<i<<"]"<<" = "<<ranku[i];
			break;
		case 2:
			for(i=0; i<numberOfTasks; ++i)
				cout<<"\n[printRanks("<<c<<")] - t["<<i<<"]"<<" = "<<rankd[i];
			break;
		default:
			cout<<"\nInvalid choice passed to HEFT_printRanks. 1-Up; 2-Down;\n";
			exit(0);
	}
	cout<<"\n[HEFT_printRanks] - exiting";
}

int
SDC :: HEFT_run()
{
	cout<<"\n[HEFT_run] - entering";
	fflush(stdout);
	HEFT_initialize();
	fflush(stdout);
	HEFT_calculateRanksUp();
	fflush(stdout);
	HEFT_calculateSchedule(1);
	fflush(stdout);
	//HEFT_printRanks(1);
	fflush(stdout);

	cout<<"\n=============================================================";
	double maxFT = 0;
	for(int64_t i=0; i<numberOfTasks; ++i)
		if(HEFT_times[i].second > maxFT)
			maxFT = HEFT_times[i].second;
	cout<<"\n[HEFT_run] - myownmakespan = "<<maxFT;
	HEFT_final_cost = maxFT;
	//cout<<"\n[HEFT_run] - HEFTmakespan = "<<revisedEvaluateMappingWithDependencies(HEFTMapping, numberOfTasks, numberOfResources);
	//cout<<"\n[HEFT_run] - exiting";
	return maxFT;
}

#endif
