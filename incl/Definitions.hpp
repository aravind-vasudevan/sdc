#ifndef _DEFINITIONS_HPP
#define _DEFINITIONS_HPP

#include <limits>

#define _APP_MIPS 0
#define _APP_VEC 1
#define _TOPO_MIPS 0
#define _TOPO_VEC 1

#define TRUE 1
#define FALSE 0
extern int64_t _INF;
#define _CONSTRAINT_CHOICE 2 // 1 - Single constraint; 2 - Multiple Constraint
#define CMP_SCALE_FACTOR 1
#define BW_SCALE_FACTOR 1
#define _TOPOLOGY_BW_SCALE_FACTOR 1 // Multiplies by 10Gigs // This increases bandwidth of each link by the factor defined here
//#define _TOPOLOGY_BW_SCALE_FACTOR 1000000 // Multiplies by 10Gigs // This increases bandwidth of each link by the factor defined here
#define _TOPOLOGY_COMP_SCALE_FACTOR 1// This increases comp capacity of each link by the factor defined here
#define _SCHEDULE_PATH 0
#define _SCHEDULE_GRAPH 1
#define _STATIC_RANK 0
#define _DYNAMIC_RANK 1

extern double SDC_final_cost, HEFT_final_cost;

#endif
