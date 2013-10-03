#ifndef GRAPHIR_HPP
#define GRAPHIR_HPP

#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "Commons.hpp"

class GraphIR
{
	public:
	int64_t nvtxs;				// Number of vertices in G(V, E)
	int ncon;					// Number of connections in G(V, E)
	std::vector < int64_t > vwgt; 	// Constraint vector.
	std::vector < int64_t > singleConstraintVwgt; 	// Constraint vector.
	std::vector < ConnectivityMatrix > connectivityMatrix;
	GraphIR operator = (GraphIR);

	int parseMETIS(std::string fileName);
	int makeGraphDirected();
	
	GraphIR();
	~GraphIR();
};

#endif
