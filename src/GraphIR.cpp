#ifndef GRAPHIR_CPP
#define GRAPHIR_CPP

#include "GraphIR.hpp"

using namespace std;

int
GraphIR::makeGraphDirected()
{
	int64_t i=0, j=0, numberOfSuccessors, k=0;
	for(i=0; i<nvtxs; ++i)
	{
		numberOfSuccessors = connectivityMatrix[i].size();
		for(j=0; j<numberOfSuccessors; ++j)
		{
			int64_t childID = connectivityMatrix[i].list[j].first;
			int64_t childsSuccessors = connectivityMatrix[childID].size();
			bool found = false;
			for(k=0; k<childsSuccessors; ++k)
				if(connectivityMatrix[childID].list[k].first == i)
				{
					found = true;
					break;
				}
			if(found == true)
				connectivityMatrix[childID].list.erase(connectivityMatrix[childID].list.begin() + k);
		}
	}

}

int
GraphIR::parseMETIS(std::string fileName)
{
	const string& delimiters = " ";
	ifstream input_file;
	input_file.open( fileName.c_str() );
	int64_t line_no = 0;
	while( input_file.good() )
	{
		string line;
		vector < int64_t > tokens;
		getline( input_file, line );

		string::size_type lastPos = line.find_first_not_of( delimiters, 0 );

		string::size_type pos = line.find_first_of( delimiters, lastPos );
		while( string::npos != pos || string::npos != lastPos )
		{
			string temp = line.substr( lastPos, pos - lastPos );
			tokens.push_back( atoll ( temp.c_str() ) );

			lastPos = line.find_first_not_of( delimiters, pos );

			pos = line.find_first_of( delimiters, lastPos );
		}
		// cout<<"tokens.size() = "<<tokens.size();
		if( tokens.size() == 0)
			continue;
		else if( line_no == 0 )
		{
			if( tokens.size() != 4 )
			{
				cout << "Error: Input graph file format incorrect" << endl;
				return -1;
			}
			if( tokens[ 2 ] != 11 )
			{
				cout << "Error: Input graph without constraints specified" << endl;
				return -1;
			}
			nvtxs = tokens[ 0 ];
			ncon = tokens[ 3 ];
			line_no++;
			for(int i=0; i<nvtxs; ++i)
			{
				ConnectivityMatrix tempVector;
				connectivityMatrix.push_back( tempVector );
			}
			singleConstraintVwgt.resize( nvtxs, 0 );

			vwgt.resize( nvtxs * ncon, 0 );
		}
		else
		{
			if( ncon > 1 )
				if( tokens[ 1 ] == 0 && tokens[ 0 ] == 0 )
					singleConstraintVwgt[ line_no - 1 ] = 1;
				else
				{
					if( tokens[ 1 ] == 0 )
						singleConstraintVwgt[ line_no - 1 ] = tokens[ 0 ] * 1;
					else if( tokens[ 0 ] == 0 )
						singleConstraintVwgt[ line_no - 1 ] = tokens[ 1 ] * 1;
					else
						singleConstraintVwgt[ line_no - 1 ] = tokens[ 0 ] * tokens[ 1 ];
				}
			else if( ncon == 1 )
				singleConstraintVwgt[ line_no - 1 ] = tokens[ 0 ];
			for(int i = 0; i<ncon; ++i)
			{
				if(tokens[i]==0)
					tokens[i]=1;
				vwgt[ (line_no - 1) * ncon + i ] = tokens[i];
			}
			// line_no - 1 will give node ID
			for(int i = ncon; i<tokens.size(); i=i+2)
			{
				//	cout<<"a";
				pair < int64_t, double > temp;
				temp.first = tokens[ i ] - 1;
				temp.second = tokens[ i+1 ];
				connectivityMatrix[ line_no - 1 ].list.push_back(temp);
			}
			line_no++;
			//cout<<"ding"<<line_no<<"\n";
		}
	}
	input_file.close();
	/*
	   cout<<"\nDone Parsing file("<<fileName<<"). Printing its contents with vertex weights :\n";
	   for(int i=0; i<nvtxs; ++i)
	   {
	   cout<<"\n"<<vwgt[ i ]<<"\t";
	   for(int j=0; j<nvtxs; ++j)
	   cout<<"\t"<<connectivityMatrix[ i ][ j ];
	   }
	 */
//	cout<<"===> parseMETIS : Exiting\n";
}

GraphIR::GraphIR()
{
	nvtxs = ncon = 0;
}

GraphIR::~GraphIR()
{
	// free memory for the all the variables. call vector.clear();
}

GraphIR GraphIR::operator = (GraphIR param)
{
	GraphIR temp;
	temp.nvtxs = param.nvtxs;
	temp.ncon = param.ncon;
	temp.vwgt = param.vwgt;
	return (temp);
}


#endif
