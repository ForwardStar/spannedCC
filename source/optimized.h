#include "commonfunctions.h"
#include "temporal_graph.h"
#include "baseline.h"

class OptimizedIndex: public BaselineIndex {
    
    public:

        // Ssize[u][id_ts][id_te]: the size of connected component at u at time t(u, id_ts, id_te) with start time ts, 
        //                   calculated by prefix sum;
        // S[u][id_ts][id_te]: the new vertices in the connected component mounted at u at time t(u, id_ts, id_te) with start time ts, 
        //               -1 if u becomes not a mounted point;
        // L[u][id_ts][id_te]: the label of u at time t(u, id_ts, id_te) with start time ts;
        // T[u][id_ts][id_te]: the index change time function which indicates the id-th index change time of u, 
        //               referred to as t(u, id_ts, id_te) with start time ts.

        OptimizedIndex() {}
        OptimizedIndex(TemporalGraph * Graph);
        ~OptimizedIndex() {}

};

void optimized(OptimizedIndex * Index, int vertex_num, char * query_file, char * output_file);