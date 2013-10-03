#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

char* substr(const char* str, int start, int end)
{
	char* a = new char[1+(end-start)]; 
	for(int i=start; i<end; i++) 
		a[i-start] = str[i]; 
	a[end-start] = '\0'; 
	return a;
}

double
atod(const char* a)
{
	double retVal = atoi(a);
	//cout<<"\ngangnam : "<<" ip= "<<a<<" retVal = "<<retVal;
	int start = 0;
	int end = 0;
	for(int i=0; a[i] != '\0'; i++){ 
		if(a[i] == '.' && start == 0){
			start = i+1; 
			end = i+1;
		}
		else if(start != 0)
			end = i+1;
			
/*		else if(start != 0 &&  (a[i] < '0' || a[i] > '9')){ 
			end = i; 
			break; 
		}*/
	}
	if(end > start){ 
		char* decimal = substr(a, start, end); 
		//cout<<" decimal  = "<<decimal;
		int dec = atoi(decimal); 
		int power = end-start; 
		retVal += ((double)dec)/(pow(10.0, (double)power)); 
	}
	//cout<<" op = "<<retVal;
	return retVal;
}

// 1st file - US - our
// 2nd file - BP - bin

int
main( int argc, char *argv[])
{
	if(argc != 4)
	{
		cout<<"\nSorry broski. Incorrect file usage. I ain't gonna run! \nRemember, its always ./binary PATH_US PATH_BIN_PACKING WHAT_ALGORITHM\n\n";
		exit(0);
	}
	const string& delimiters = " ";
	ifstream bin_input_file, our_input_file;
	ofstream log_file;
	string logfile(argv[3]);
	cout<<fixed<<logfile;
	double bin_min_cost = 9999999999, bin_max_cost = 0, our_min_cost = 9999999999, our_max_cost = 0;
	double bin_min_nodes = 9999999999, bin_min_strip = 9999999999, bin_max_nodes = 0, bin_max_strip = 0;	
	double our_min_nodes = 9999999999, our_min_strip = 9999999999, our_max_nodes = 0, our_max_strip = 0;	
	log_file.open(logfile.c_str());
	bin_input_file.open( argv[2] );
	our_input_file.open( argv[1] );
	int64_t line_no = 0, total_our_data_points = 0, better_us = 0, better_bin = 0, total_bin_data_points = 0;
	vector < double > diffSet;
	while( our_input_file.good() )
	{
		//cout<<"\n our_input_file.tellg()="<<our_input_file.tellg();
		string our_line;
		vector < double > our_tokens;
		getline( our_input_file, our_line );
		string::size_type our_lastPos = our_line.find_first_not_of( delimiters, 0 );
		string::size_type our_pos = our_line.find_first_of( delimiters, our_lastPos );
		while( string::npos != our_pos || string::npos != our_lastPos )
		{
			string temp = our_line.substr( our_lastPos, our_pos - our_lastPos );
			double d = (double) atof ( temp.c_str() );
			//cout<<" "<<d;
			our_tokens.push_back( d );
			our_lastPos = our_line.find_first_not_of( delimiters, our_pos );
			our_pos = our_line.find_first_of( delimiters, our_lastPos );
		}
		//cout<<endl<<"tokens.size() = "<<our_tokens.size();
		if( our_tokens.size() == 0)
			continue;
		else if( our_tokens.size() == 3)
		{
			if( our_min_cost > our_tokens[2] )
			{
				//cout<<"woof";
				our_min_nodes = our_tokens[1];
				our_min_strip = our_tokens[0];
				our_min_cost = our_tokens[2];
			}
			if( our_max_cost < our_tokens[2] )
			{
				//cout<<"foow";
				our_max_nodes = our_tokens[1];
				our_max_strip = our_tokens[0];
				our_max_cost = our_tokens[2];
			}
			total_our_data_points++;
		}
	}
	bin_input_file.seekg(0, ios::beg);
	our_input_file.seekg(0, ios::beg);
	//cout<<"\n our_input_file.tellg()="<<our_input_file.tellg();
	cout<<"\nTotal Data Points in our file found : "<<total_our_data_points;
	log_file<<"\nStats for comparing 1st file="<<argv[1]<<" against 2nd file="<<argv[2]<<endl;
	log_file<<"\nTotal number of data points found in 1st file : "<<total_our_data_points;
    while( bin_input_file.good() )
	{
		total_bin_data_points++;
    	string bin_line;
        vector < double > bin_tokens;
	    getline( bin_input_file, bin_line );
        string::size_type bin_lastPos = bin_line.find_first_not_of( delimiters, 0 );
    	string::size_type bin_pos = bin_line.find_first_of( delimiters, bin_lastPos );
        while( string::npos != bin_pos || string::npos != bin_lastPos )
	    {
			string temp = bin_line.substr( bin_lastPos, bin_pos - bin_lastPos );
        	bin_tokens.push_back( (double) atof ( temp.c_str() ) );
		    bin_lastPos = bin_line.find_first_not_of( delimiters, bin_pos );
       		bin_pos = bin_line.find_first_of( delimiters, bin_lastPos );
	    }
		our_input_file.clear();
		our_input_file.seekg(0, ios::beg);
		if( bin_tokens.size() == 0)
			continue;
		else if( bin_tokens.size() == 3 )
		{
			//cout<<"\naloha";
			if( bin_min_cost > bin_tokens[2] )
			{
				bin_min_nodes = bin_tokens[1];
				bin_min_strip = bin_tokens[0];
				bin_min_cost = bin_tokens[2];
			}
			if( bin_max_cost < bin_tokens[2] )
			{
				bin_max_nodes = bin_tokens[1];
				bin_max_strip = bin_tokens[0];
				bin_max_cost = bin_tokens[2];
			}
			double bin_current_cost = bin_tokens[2], our_current_cost = 0;
			int found = 0;
			//cout<<"\n our_input_file.tellg()="<<our_input_file.tellg();
			while( our_input_file.good() )
			{
				string our_line;
				vector < double > our_tokens;
				getline( our_input_file, our_line );
				string::size_type our_lastPos = our_line.find_first_not_of( delimiters, 0 );
				string::size_type our_pos = our_line.find_first_of( delimiters, our_lastPos );
				while( string::npos != our_pos || string::npos != our_lastPos )
				{
					string temp = our_line.substr( our_lastPos, our_pos - our_lastPos );
					our_tokens.push_back( (double) atof ( temp.c_str() ) );
					our_lastPos = our_line.find_first_not_of( delimiters, our_pos );
					our_pos = our_line.find_first_of( delimiters, our_lastPos );
				}
				//cout<<"meh";
				if( our_tokens.size() == 0)
					continue;
				else if( bin_tokens[0] == our_tokens[0] && bin_tokens[1] == our_tokens[1] )
				{
					//cout<<"--meooooooooooow--";
					diffSet.push_back( bin_current_cost - our_tokens[2] );
					if( our_tokens.size() == 3 )
					{
						//cout<<"woof";
						if( bin_current_cost > our_tokens[2] )
							better_us++;
						else
							better_bin++;
						break;
					}
					else
						better_bin++;
				}
			}
		}
		else
		{
			//cout<<"--godno---";
			better_us++;
		}
	}
	
	better_us += total_our_data_points - total_bin_data_points;
	cout<<"\nWriting Log File...";
	log_file<<"\nNumber of instances 1st file better than 2nd file : "<<better_us;
	log_file<<"; % of instances 1st file better than 2nd file : "<<100*better_us/total_our_data_points;
	log_file<<"\nNumber of instances 2nd file is better than 1st file : "<<better_bin;
	log_file<<"; % of instances 2nd file  is better than 1st file : "<<100*better_bin/total_our_data_points;
	
	log_file<<"\n1st File's Framework's awesome reuslts : \n\tMinimum Execution Time : "<<our_min_cost;
	log_file<<"\n\tConfig :\n\t\tNumber of Nodes : "<<(int64_t)our_min_nodes<<"\n\t\tStrip Size : "<<(int64_t)our_min_strip;
	log_file<<"\n\tMaximum Execution Time : "<<our_max_cost;
	log_file<<"\n\tConfig :\n\t\tNumber of Nodes : "<<our_max_nodes<<"\n\t\tStrip Size : "<<our_max_strip;

	log_file<<"\n2nd File's lame reuslts : \n\tMinimum Execution Time : "<<bin_min_cost;
	log_file<<"\n\tConfig :\n\t\tNumber of Nodes : "<<(int64_t)bin_min_nodes<<"\n\t\tStrip Size : "<<(int64_t)bin_min_strip;
	log_file<<"\n\tMaximum Execution Time : "<<bin_max_cost;
	log_file<<"\n\tConfig :\n\t\tNumber of Nodes : "<<bin_max_nodes<<"\n\t\tStrip Size : "<<bin_max_strip;
	
	cout<<"\nLog file done...\n";
	bin_input_file.close();
}
