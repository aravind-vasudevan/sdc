#ifndef FLOYD_WARSHALL_HPP
#define FLOYD_WARSHALL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <iomanip>

#include "MD5Hash.hpp"
#include "Commons.hpp"

class FloydWarshall
{
	public :
	int numberOfNodes;
	std::string filename;
	std::vector < ConnectivityMatrix > allPairShortestPath; // Initially this is the weighted connectivity matrix

	FloydWarshall(int n, std::string f); // This constructor needs to be called if memoization is to be used
	FloydWarshall(int n);
	FloydWarshall(string f);
	int run( std::vector < ConnectivityMatrix > tempMatrix ); // O(n^3) /SadFace
};

#endif
