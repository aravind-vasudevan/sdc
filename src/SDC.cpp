#ifndef _MAIN_CPP
#define _MAIN_CPP

#include "SDC.hpp"

char* logFileAttachment;

int64_t _INF = std::numeric_limits<int64_t>::max();
double SDC_final_cost, HEFT_final_cost;

SDC :: SDC()
{
}

SDC :: ~SDC()
{
}

MetaCell :: MetaCell()
{
	myCost = pathCost = 0;
}

double
SDC :: evaluateExecutionTime( int64_t task, int64_t resource )
{
	int64_t t1 = ceil( (double) applicationGraph->vwgt[ task * applicationGraph->ncon + _APP_VEC ] / topologyGraph->vwgt[ resource * topologyGraph->ncon + _TOPO_VEC ] );
	double t2 = (double) ( applicationGraph->vwgt[ task * applicationGraph->ncon + _APP_MIPS ] * t1 ) / ( topologyGraph->vwgt[ resource * topologyGraph->ncon + _TOPO_MIPS ] );
	return t2;
}

int64_t
SDC :: initializeVariables()
{
	int64_t i=0, j=0;
	numberOfPaths = 0;
	numberOfTasks = applicationGraph->nvtxs;
	numberOfResources = topologyGraph->nvtxs;

	// Clearing all data structures, except criticalPath(it has to be set externally), to ensure safe reuse
	initialS.clear();
	currentS.clear();
	bestS.clear();
	finalMapping.clear();
	criticalPathMapping.clear();
	metaTable.clear();
	executionTimes.clear();
	allPaths.clear();
	startAndEndTimes.clear();
	pathCosts.clear();
	pathCostsUsingHEFT.clear();
	metaTable.clear();
	communicationSetupTime.clear();
	capableProcessors.clear();
	percentageCapableProcessors.clear();
	percentageIncapableProcessors.clear();
	weightOfTasks.clear();
	taskRanking.clear();
	SDC_times.clear();
	avail.clear();

	// Setting up SDC variabes
	capableProcessors.resize( numberOfTasks );
	SDC_times.resize( numberOfTasks );
	percentageCapableProcessors.resize( numberOfTasks, 0 );
	percentageIncapableProcessors.resize( numberOfTasks, 0 );
	weightOfTasks.resize( numberOfTasks, 0 );
	taskRanking.resize( numberOfTasks, -1 );
	startAndEndTimes.resize( numberOfTasks );

	communicationSetupTime.resize( numberOfResources, 0 );
	avail.resize( numberOfResources, 0 );
	// Initializing metaTable
	for(i=0; i<numberOfTasks; i++)
	{
		vector < MetaCell > tempVector;
		tempVector.resize(numberOfResources);
		metaTable.push_back(tempVector);
	}
	for( i=0; i<numberOfTasks; ++i )
	{
		vector < double > tempVector( numberOfResources );
		executionTimes.push_back( tempVector );
	}
	cout<<fixed<<"\n[init] - calculating executionTimes";
	averageComputation.resize(numberOfTasks, 0);
	double totalAverageComputation = 0;
	for( i=0; i<numberOfTasks; ++i )
	{
		double sum=0;
		//cout<<"\ntask["<<i<<"]= | ";
		for( j=0; j<numberOfResources; ++j )
		{
			executionTimes[i][j] = evaluateExecutionTime(i, j);
			sum +=	executionTimes[i][j];
			//cout<<"\t"<<executionTimes[i][j];
		}
		averageComputation[i] = sum/numberOfTasks;
		totalAverageComputation += averageComputation[i];
		//cout<<"\t| avg(task["<<i<<"]) = "<<averageComputation[i];
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
	cout<<"\n[init] - averageCommStartupTime = "<<averageCommStartupTime;
	cout<<"\n[init] - averageBandwidth(not latency) = "<<averageBandwidth;
	cout<<"\n[init] - Ratio of totalAverageComputation to averageBandwidth = "<<(double)totalAverageComputation/averageBandwidth;
}

double
SDC :: revisedEvaluateMappingWithDependencies( map <int64_t, int64_t> currentMapping, int64_t numberOfTasks, int64_t numberOfResources )
{
	double totalCost = 0, sum = 0;
	vector < int64_t > scheduledNodes;
	int i=0, j=0;
	vector < int64_t > toBeScheduled;
	for(i=0; i<numberOfTasks; ++i)
		toBeScheduled.push_back(i);
	for( i=0; toBeScheduled.size()>0; ++i )
	{
		if(i>=toBeScheduled.size())
			i=0;
		int64_t nodeToBeScheduled = toBeScheduled[i];
		int64_t numberOfPredecessors=applicationGraphTranspose->connectivityMatrix[nodeToBeScheduled].size();
		if( numberOfPredecessors == 0 )
		{
			startAndEndTimes[ nodeToBeScheduled ].first = 0;
			startAndEndTimes[ nodeToBeScheduled ].second = executionTimes[ nodeToBeScheduled ][ currentMapping[nodeToBeScheduled] ];
		}
		else
		{
			for(int k=0; k<numberOfPredecessors; ++k)
			if(find(toBeScheduled.begin(), toBeScheduled.end(), applicationGraphTranspose->connectivityMatrix[nodeToBeScheduled].list[k].first) != toBeScheduled.end())
				continue;
			// Not start task
			vector < std::pair < int64_t, double > > scheduledFinishTimes;
			for( j=0; j<nodeToBeScheduled; ++j )
			{
				// Check all parents of this task
				bool found = false;
				for(int k=0; k<applicationGraph->connectivityMatrix[j].size(); ++k)
					if(applicationGraph->connectivityMatrix[j].list[k].first == nodeToBeScheduled)
					{
						found = true;
						break;
					}
	//			if( find( applicationGraph->connectivityMatrix[j].list.begin(), applicationGraph->connectivityMatrix[j].list.end(), i ) != applicationGraph->connectivityMatrix[j].list.end() )
	//			if( applicationGraph->connectivityMatrix[j][i] != 0 )
				if(found == true)
				{
					// When parent is found check if parent is already scheduled
					if( find( scheduledNodes.begin(), scheduledNodes.end(), j ) != scheduledNodes.end() )
						scheduledFinishTimes.push_back( make_pair( j, startAndEndTimes[j].second ));
					else
					{
						cout<<"\nWoops. A parent ["<<j<<"] has not been scheduled but we are visiting its child ["<<nodeToBeScheduled<<"] to schedule it.";
						exit( 0 );
					}
				}
			}

			int64_t tempNode=0;
			double maxval = 0;
			double communicationTime = 0;// startup time + data to be sent/bw of link
			for(j=0; j<applicationGraphTranspose->connectivityMatrix[i].size(); ++j)
			{
				tempNode = applicationGraphTranspose->connectivityMatrix[i].list[j].first;
				communicationTime = communicationSetupTime[ currentMapping[tempNode] ] + ( applicationGraph->connectivityMatrix[ tempNode ][ nodeToBeScheduled ] * topologyGraph->connectivityMatrix[ currentMapping[tempNode] ][ currentMapping[nodeToBeScheduled] ] );
				if(startAndEndTimes[tempNode].second + communicationTime>maxval)
					maxval = startAndEndTimes[tempNode].second + communicationTime;
			}

	//		cout<<"\ncommunication : "<<communicationTime;
			startAndEndTimes[nodeToBeScheduled].first = maxval + communicationTime;
			startAndEndTimes[nodeToBeScheduled].second = startAndEndTimes[nodeToBeScheduled].first + executionTimes[ nodeToBeScheduled ][ currentMapping[nodeToBeScheduled] ];
		}
		if( startAndEndTimes[nodeToBeScheduled].second > totalCost )
			totalCost = startAndEndTimes[nodeToBeScheduled].second;
		scheduledNodes.push_back( nodeToBeScheduled );
		toBeScheduled.erase( toBeScheduled.begin() + i );
		--i;
	}
	return totalCost;
}

double 
SDC :: calculateTaskRanking( int64_t parent )
{
	int64_t j=0;
	if(taskRanking[parent] == -1)
	{
		int64_t numberOfSuccessors = applicationGraph->connectivityMatrix[parent].size();
		if( numberOfSuccessors == 0 )
		{
			taskRanking[parent] = 0;
			return taskRanking[parent];
		}
		double maxVal=0;
		for(j=0; j<numberOfSuccessors; ++j)
		{
			int64_t successor=applicationGraph->connectivityMatrix[parent].list[j].first;
			double commCost=applicationGraph->connectivityMatrix[parent].list[j].second;
			commCost = commCost / averageBandwidth;
			double successorBLEV=calculateTaskRanking( successor );
			if(commCost + successorBLEV > maxVal)
				maxVal=commCost + successorBLEV;
		}
		taskRanking[parent] = weightOfTasks[parent] + maxVal;
	}
	else
		return taskRanking[parent];
	return taskRanking[parent];
}

double
SDC :: calculateRanks( int choice )
{
	switch(choice)
	{
		case _STATIC_RANK:
		{
			int i;
			for(i=numberOfTasks-1; i>=0; --i)
				if(applicationGraphTranspose->connectivityMatrix[i].size() == 0)
					calculateTaskRanking(i);
			break;
		}
		case _DYNAMIC_RANK:
		{
			break;

		}
		default:
		{
			cout<<"Invalid choice to calculateRanks. "<<_STATIC_RANK<<" for static rank, "<<_DYNAMIC_RANK<<" for dynamic rank\n";
			exit(0);
		}
	}
}

double
SDC :: calculateEST( int64_t nodeToBeScheduled, int64_t processor )
{
//	cout<<"\n[SDC_calculateEST] - Entering ("<<nodeToBeScheduled<<", "<<processor<<")";
	double result = 0;
	int64_t i=0, j=0, numberOfPredecessors=applicationGraphTranspose->connectivityMatrix[nodeToBeScheduled].size();
	if(numberOfPredecessors == 0)
	{
//		cout<<"\n[SDC_calculateEST] - Parent node found. Returning.";fflush(stdout);
		result = 0;
	}
	else
	{
//		cout<<"\n[SDC_calculateEST] - Not parent node. Preds="<<numberOfPredecessors;
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
			int64_t parentsProcessor = SDCMapping[ parent.first ];
			if(parentsProcessor == processor)
				commTime = 0;
			else
				commTime = communicationSetupTime[parentsProcessor] + parent.second*topologyGraph->connectivityMatrix[parentsProcessor][processor];
			readyTime[i] = SDC_times[ parent.first ].second + commTime;
			if( max<readyTime[i] )
			{
				max = readyTime[i];
				maxi = i;
			}
		}
		result = readyTime[maxi];
	}
//	cout<<"\n[SDC_calculateEST] - Exiting";
	(avail[processor] > result ) ? result = avail[processor] : result = result;
	return result;
}


double
SDC :: calculateAEFT(int64_t nodeToBeScheduled, int64_t currentProcessor)
{
	return 1;
}

int64_t
SDC :: mapTasksToResources( char* applicationName )
{
	cout<<"\n[mapTasksToResources] - Entering...";
	int64_t i=0, j=0;
	vector <int64_t> listOfSources;

	// 1. Set the weights of task nodes with Eq(9)
	for(i=0; i<numberOfTasks; ++i)
	{
		//cout<<"\ni="<<i;
		for(j=0; j<numberOfResources; ++j)
			if( executionTimes[i][j] < _INF )
			{
				capableProcessors[i].push_back( j );
				//cout<<" | "<<j;
			}
		//cout<<"\nCP("<<i<<")="<<capableProcessors[i].size();
	}
	for(i=0; i<numberOfTasks; ++i)
	{
		percentageCapableProcessors[i] = capableProcessors[i].size()/(double)numberOfResources;
		percentageIncapableProcessors[i] = 1-percentageCapableProcessors[i];
		double sum=0;
		for(j=0; j<capableProcessors[i].size(); ++j)
		{
			sum += executionTimes[ i ][ capableProcessors[i][j] ]/(double) capableProcessors[i].size();
			weightOfTasks[i] = sum / percentageCapableProcessors[i];
		}
	}	
	//cout<<"\n[mapTasksToResources] - 1 Done";

	// 2. Set the weights of edges with Eq(2)
		// averageBandwidth and averageCommStartupTime have already been set.

	// 3. Compute the BLEVs for all tasks by traversing graph
	//    upward from the exit node.
		// Calculate a list of parents. Call calculateTaskRanking(i) with them
	int64_t numberOfExits=0;
	calculateRanks( _STATIC_RANK );
	for(i=numberOfTasks-1; i>=0; --i)
	{
		if(applicationGraphTranspose->connectivityMatrix[i].size() == 0)
			listOfSources.push_back(i);
		
		if(applicationGraph->connectivityMatrix[i].size() == 0)
			numberOfExits++;
	}
	//cout<<"\n[mapTasksToResources] - 2,3 Done";
	
	// 4. Sort the task into a list by non-increasing order of BLEV
	vector < int64_t > toBeScheduled;
	for(i=0; i<numberOfTasks; ++i)
		toBeScheduled.push_back(i);
	for(i=0;i<numberOfTasks; ++i)
	{
		double maxRank = 0;
		int64_t maxJ = 0;
		bool found = false;
		for(j=i; j<numberOfTasks; ++j)
		{
			if(maxRank<taskRanking[ toBeScheduled[j] ])
			{
				found = true;
				maxRank = taskRanking[ toBeScheduled[j] ];
				maxJ = j;
			}
		}
		if(found == true)
		{
			int64_t temp = toBeScheduled[ maxJ ];
			toBeScheduled[ maxJ ] = toBeScheduled[ i ];
			toBeScheduled[ i ] = temp;
		}
	}
	//cout<<"\n[mapTasksToResources] - 4 Done";

/*
	cout<<"\nPrinting toBeScheduled";
	for(i=0; i<numberOfTasks; ++i)
		cout<<"\ntoBeScheduled["<<i<<"]="<<toBeScheduled[i];
	fflush(stdout);
*/

	// 5. While the scheduling list is not empty; do
	vector < int64_t >::iterator it;
	for(i=0; toBeScheduled.size()>0; ++i)
	{
		//cout<<"\ntoBeScheduled.size="<<toBeScheduled.size();
		//fflush(stdout);
		// 6. Remove the first task v_i from the list for scheduling
		if(i>=toBeScheduled.size())
			i=0;
		double min = _INF;
		int64_t minCurrentProcessor = 0, nodeToBeScheduled = toBeScheduled[i];
		//cout<<"\nnodeToBeScheduled="<<nodeToBeScheduled;
		//fflush(stdout);
		int64_t numberOfPredecessors = applicationGraphTranspose->connectivityMatrix[nodeToBeScheduled].size();
		//cout<<"\nnumberOfPredecessors="<<numberOfPredecessors;
		//fflush(stdout);
		vector < double > eft, est, aeft;
		est.resize(capableProcessors[ nodeToBeScheduled ].size(), 0);
		eft.resize(capableProcessors[ nodeToBeScheduled ].size(), 0);
		aeft.resize(capableProcessors[ nodeToBeScheduled ].size(), 0);
		for(int k=0; k<numberOfPredecessors; ++k)
			if(find(toBeScheduled.begin(), toBeScheduled.end(), applicationGraphTranspose->connectivityMatrix[nodeToBeScheduled].list[k].first) != toBeScheduled.end())
				goto exitLabel;
		//cout<<"\n[mapTasksToResources] - 6 Done";
		//fflush(stdout);

		// 7. for each processor capable p_j of v_i; do
		// 8. Compute AEFT(n_i, p_j) value with Eq(11)
		//    using insertion-based policy
		for(j=0; j<capableProcessors[ nodeToBeScheduled ].size(); ++j)
		{
			int64_t currentProcessor=capableProcessors[ nodeToBeScheduled ][ j ];
			est[currentProcessor] = calculateEST( nodeToBeScheduled, currentProcessor );
			//est[currentProcessor] = 1;
			//cout<<"("<<currentProcessor<<", "<<est[currentProcessor]<<")";
			eft[currentProcessor] = executionTimes[nodeToBeScheduled][currentProcessor] + est[currentProcessor];
			double sum=0;
			int64_t numberOfSuccessors=applicationGraph->connectivityMatrix[ nodeToBeScheduled ].size();
			
			for(int64_t k=0; k<numberOfSuccessors; ++k)
			{
				int64_t successor=applicationGraph->connectivityMatrix[ nodeToBeScheduled ].list[ k ].first;
				double product=1;
				for(int64_t l=0; l<capableProcessors[ successor ].size(); ++l)
					product = product * ( applicationGraph->connectivityMatrix[ nodeToBeScheduled ].list[ k ].second * topologyGraph->connectivityMatrix[ currentProcessor ][ capableProcessors[successor][l] ]);
				sum += pow( product, (double) 1/capableProcessors[ successor ].size());
			}
			//cout<<"\nSum("<<nodeToBeScheduled<<","<<currentProcessor<<")="<<sum;
			(numberOfSuccessors==0)?aeft[ currentProcessor ] = eft[ currentProcessor ]:aeft[ currentProcessor ] = eft[ currentProcessor ] + sum/numberOfSuccessors;
			//cout<<", "<<eft[currentProcessor]<<") | ";
			
			if( min > aeft[currentProcessor] )
			{
				min = aeft[currentProcessor];
				minCurrentProcessor = currentProcessor;
			}
			
			/*
			if( min > eft[currentProcessor] )
			{
				min = eft[currentProcessor];
				minCurrentProcessor = currentProcessor;
			}
			*/
		}

		//cout<<"++++chosen="<<minCurrentProcessor;
		//cout<<"\n[mapTasksToResources] - 7,8,9 Done";
		//fflush(stdout);
		// 9. endfor
		// 10. Assign task v_i to the processor that minimize AEFT of v_i
		SDC_times[nodeToBeScheduled].first = est[minCurrentProcessor];
		SDC_times[nodeToBeScheduled].second = eft[minCurrentProcessor];
		SDCMapping[nodeToBeScheduled] = minCurrentProcessor;
		avail[minCurrentProcessor] = SDC_times[nodeToBeScheduled].second;

		// Removing from nodes to be scheduled
		//it = find(toBeScheduled.begin(), toBeScheduled.end(), nodeToBeScheduled);
		//toBeScheduled.erase(it);
		toBeScheduled.erase( toBeScheduled.begin() + i );
		--i;
		//cout<<"\n[mapTasksToResources] - 10 Done | i="<<i<<" | toBeScheduled.size()="<<toBeScheduled.size();
		exitLabel:
		;
	}
	// 11. endwhile

	cout<<"\n=============================================================";
	//cout<<"\n numberOfTasks="<<numberOfTasks;
	double maxFT = 0;
	for(i=0; i<numberOfTasks; ++i)
	{
		if(SDC_times[i].second > maxFT)
			maxFT = SDC_times[i].second;
	}
	cout<<"\n[SDC_run] - myownmakespan = "<<maxFT;
	SDC_final_cost = maxFT;
	fflush(stdout);
	//cout<<"\n[SDC_run] - SDCmakespan = "<<revisedEvaluateMappingWithDependencies(SDCMapping, numberOfTasks, numberOfResources);
	//fflush(stdout);
	cout<<"\n[SDC_run] - exiting";

	cout<<"\n[mapTasksToResources] - Exiting mapTasksToResources..";
//	cout<<"\n[mapTasksToResources] - Cost of mapping criticl path = "<<minPathCost;

}

double
SDC :: run( char* applicationName, char* topologyName, int choice=_MASTER )
{
	clock_t clockStart = clock();
	srand( time( NULL ) );
	
	applicationGraph = new GraphIR();
	applicationGraphTranspose = new GraphIR();
	topologyGraph = new GraphIR();
	topologyCopy = new GraphIR();
	cout<<"argv[1]="<<applicationName<<" with size : "<<sizeof(applicationName)<<"\nargv[2]="<<topologyName<<" with size : "<<sizeof(topologyName)<<endl;
//	cout<<"\n[run] - b4 app->parse";
	applicationGraph->parseMETIS( applicationName );
//	cout<<"\n[run] - b4 app->parse";
//	cout<<"\n[run] - b4 app->parse";
	topologyGraph->parseMETIS( topologyName );
	applicationGraph->makeGraphDirected();
//	cout<<"\n[run] - b4 app->parse";
	cout<<"\n[run] - _TOPOLOGY_BW_SCALE_FACTOR = "<<_TOPOLOGY_BW_SCALE_FACTOR;
	cout<<"\n[run] - _TOPOLOGY_COMP_SCALE_FACTOR = "<<_TOPOLOGY_COMP_SCALE_FACTOR;
	cout<<"\n[run] - read both graphs succesfully";
	//int numberOfTasks = applicationGraph.nvtxs, numberOfResources = topologyGraph.nvtxs, i = 0, j=0;
	numberOfTasks = applicationGraph->nvtxs, numberOfResources = topologyGraph->nvtxs;
	int64_t i = 0, j=0;

	// Creating inverse of the application graph just to maintain record of predecessors
	for(i=0; i<numberOfTasks; ++i)
	{
		ConnectivityMatrix tempConnectivityMatrix;
		applicationGraphTranspose->connectivityMatrix.push_back(tempConnectivityMatrix);
	}
	for(i=0; i<numberOfTasks; ++i)
	{
		for(int k=0; k<applicationGraph->connectivityMatrix[i].size(); ++k)
		{
			pair < int64_t, double > tempPair;
			tempPair.first = i;
			tempPair.second = applicationGraph->connectivityMatrix[i].list[k].second;
			applicationGraphTranspose->connectivityMatrix[applicationGraph->connectivityMatrix[i].list[k].first].list.push_back(tempPair);
		}
	}
	// sanity check - inverse of appgrf should be lower triangular
//	for(i=0; i<numberOfTasks; ++i)
//		for(int k=0; k<applicationGraphTranspose->connectivityMatrix[i].size(); ++k)
//			assert( i>applicationGraphTranspose->connectivityMatrix[i].list[k].first );


	for(i=0; i<numberOfResources; ++i)
	{
		int numberOfSuccessors = topologyGraph->connectivityMatrix[i].size();
		vector < int64_t > tempList;
		for(j=0; j<numberOfSuccessors; ++j)
			tempList.push_back(topologyGraph->connectivityMatrix[i].list[j].first);
//		cout<<"\n[run] - i="<<i<<" tempList.size="<<tempList.size();
		for(j=0; j<numberOfResources; ++j)
		{
			if( find( tempList.begin(), tempList.end(), j ) == tempList.end() )
			{
/*
				pair < int64_t, double > tempPair;
				tempPair.first = j;
				tempPair.second = _INF;
				topologyGraph->connectivityMatrix[i].list.push_back(tempPair);
*/
			}
			else
			{
//				cout<<"\t"<<j<<"->"<<topologyGraph->connectivityMatrix[i][j];
				if(topologyGraph->connectivityMatrix[i][j] == 0 && i!=j)
				{
					cout<<"\n[run] - Some link("<<i<<","<<j<<") in the topology has 0 bandwidth, hence exiting";
					exit(0);
				}
				else
					topologyGraph->connectivityMatrix[ i ][ j ] = (_TOPOLOGY_BW_SCALE_FACTOR * topologyGraph->connectivityMatrix[ i ][ j ]);
			}
		}
	}
	topologyCopy->connectivityMatrix = topologyGraph->connectivityMatrix;

	for(i=0; i<numberOfResources*topologyGraph->ncon; ++i)
		topologyGraph->vwgt[i] = topologyGraph->vwgt[i] * _TOPOLOGY_COMP_SCALE_FACTOR;
	for(i=0; i<numberOfResources*topologyGraph->ncon; ++i)
		assert(topologyGraph->vwgt[i] > 0);

	for(i=0; i<numberOfResources; ++i)
	{
		int numberOfSuccessors = topologyGraph->connectivityMatrix[i].size();
		vector < int64_t > tempList;
		for(j=0; j<numberOfSuccessors; ++j)
			tempList.push_back(topologyGraph->connectivityMatrix[i].list[j].first);
//		cout<<"\n[run] - i="<<i<<" tempList.size="<<tempList.size();
		for(j=0; j<numberOfResources; ++j)
		{
			if( find( tempList.begin(), tempList.end(), j ) == tempList.end() )
			{
				pair < int64_t, double > tempPair;
				tempPair.first = j;
				tempPair.second = _INF;
				topologyGraph->connectivityMatrix[i].list.push_back(tempPair);
			}
			else
			{
//				cout<<"\t"<<j<<"->"<<topologyGraph->connectivityMatrix[i][j];
				if(topologyGraph->connectivityMatrix[i][j] == 0 && i!=j)
				{
					cout<<"\n[run] - Some link("<<i<<","<<j<<") in the topology has 0 bandwidth, hence exiting";
					exit(0);
				}
				else
					topologyGraph->connectivityMatrix[ i ][ j ] = ( double ) 1/(topologyGraph->connectivityMatrix[ i ][ j ]);
			}
		}
	}

	cout<<"\n[run] - Done inverting bandwidths to latencies\n";
	cout<<"\n[run] - Press to start floydwarshall";
	//cin>>c;

	// Set topologyMatrix by looking at the CSR object;
	string temp(topologyName);
	topo = new FloydWarshall( topologyGraph->nvtxs, temp );
	topo->run( topologyGraph->connectivityMatrix ); 
	topologyGraph->connectivityMatrix = topo->allPairShortestPath;
	// Initing all variables needed
	initializeVariables();
	cout<<"\nchoice="<<choice;
	if( choice == _MASTER )
	{
		cout<<"\nStarting maptaskstoresources\n";
		mapTasksToResources( applicationName );

		//	cout<<"\n\nSAmakespan : "<<finalCost;
		cout<<"\nTotal time taken from start to finish = "<< ( ( double )( clock() - clockStart ) / CLOCKS_PER_SEC ) <<" seconds\n";

		/*
		   cout<<"\n\nFinal Mapping chosen :";
		   for(i=0; i<numberOfTasks; ++i)
		   cout<<endl<<"T["<<i+1<<"]--->R["<<finalMapping[i]+1<<"]";
		   finalCost = revisedEvaluateMappingWithDependencies( finalMapping, numberOfTasks, numberOfResources );
		   map < int64_t, vector < int64_t > > invertedMap;
		   map < int64_t, int64_t >::iterator it;
		   map < int64_t, vector < int64_t > >::iterator invertedIt;
		   double max = 0;
		   for( it = finalMapping.begin(); it != finalMapping.end(); ++it)
		   invertedMap[ (*it).second ].push_back( (*it).first );
		   string filename1("map");
		   string filename2(logFileAttachment);
		   filename1 = filename1 + filename2;
		   ofstream out1;
		   out1.open( filename1.c_str(), ios_base::trunc );
		   for( invertedIt = invertedMap.begin(); invertedIt != invertedMap.end(); ++invertedIt)
		   {
		   out1<<"\nN"<< (*invertedIt).first + 1 << " <- ";
		   for(i=0;i<(*invertedIt).second.size(); ++i)
		   out1<<(*invertedIt).second[i] + 1<<", ";
		   }
		 */

		cout<<endl;
		return finalCost;
	}
	else if( choice == _SLAVE ) // slave mode --> DP object is being used in some other method
	{
		cout<<"\nRunning SDC in slave mode. Inited all variables needed. Master can call any function from now.";
		cout<<"\nMaster wants to map path of length "<<criticalPath.size();
		cout<<"\nMaster hasn't asked me to do anything!";
	}
	else
	{
		cout<<"\nInvalid choice to SDC::run() - "<<choice<<"\nChoice = 0; Master mode\nChoice = 1; Slave mode\n";
		exit(0);
	}
}

int
main( int argc, char* argv[] )
{
	if(argc!=4)
	{
		cout<<"(SDC)Invalid file usage.\nParameters are as follows:";
		cout<<"\n1. Application Graph";
		cout<<"\n2. Topology Graph";
		cout<<"\n3. Log files' attachment(Valid only if running multiple instances at once to save the log files; necessary nonetheless)\n";
		exit( 0 );
	}

/*
	char* result=getHashForFile(argv[1]);
	cout<<"\nFile Hash : "<<result;
*/	
	double sdcCost, heftCost;
	SDC sdc;
	logFileAttachment = argv[ 3 ];
	sdc.run( argv[ 1 ], argv[ 2 ] );
	cout<<"\n Calling SDC::HEFT_run()";
	sdc.HEFT_run();
	cout<<"\n\nRatio(SDC/HEFT) = "<<(double)SDC_final_cost/HEFT_final_cost<<endl;
}

#endif