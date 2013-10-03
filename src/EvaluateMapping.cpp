#include "GraphIR.cpp"
#include "Floyd-Warshall.cpp"

GraphIR *applicationGraph, *topologyGraph;
FloydWarshall *topo;

int main(int argc, char* argv[])
{
	if( argc!=4 )
	{
		cout<<"\nInvalid file usage. Parameters are as follows:";
		cout<<"\n1. Application Graph";
		cout<<"\n2. Topology Graph";
		cout<<"\n3. Mapping File(map.out)";
		exit( 0 );
	}
	
	applicationGraph = new GraphIR();
	topologyGraph = new GraphIR();
	applicationGraph->parseMETIS( applicationName );
	topologyGraph->parseMETIS( topologyName );
	int64_t numberOfTasks = applicationGraph->nvtxs, numberOfResources = topologyGraph->nvtxs, i = 0, j=0;
	for(i=0; i<numberOfResources; ++i)
		for(j=0; j<numberOfResources; ++j)
		{
			if( topologyGraph->connectivityMatrix[i][j] == 0 ) 
				topologyGraph->connectivityMatrix[i][j] = 9999999;
			else
				topologyGraph->connectivityMatrix[ i ][ j ] = ( double ) 1 / topologyGraph->connectivityMatrix[ i ][ j ];
		}

	topo = new FloydWarshall( numberOfResources );
	topo->run( topologyGraph->connectivityMatrix );
	topologyGraph->connectivityMatrix = topo->allPairShortestPath;
}
