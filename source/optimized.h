#include "commonfunctions.h"
#include "temporal_graph.h"

class OptimizedIndex {

    private:

        bool addts(int u, int ts);

        bool addte(int u, int te);

        int find(int u, int id_ts, int id_te);

        void unioN(int u, int v, int t);

        int binarySearchts(int u, int ts);

        int binarySearchte(int u, int id_ts, int te);
    
    public:

        // Ssize[u][id_ts][id_te]: the size of connected component at u at time t(u, id_ts, id_te) with start time ts, 
        //                   calculated by prefix sum;
        // S[u][id_ts][id_te]: the new vertices in the connected component mounted at u at time t(u, id_ts, id_te) with start time ts, 
        //               -1 if u becomes not a mounted point;
        // L[u][id_ts][id_te]: the label of u at time t(u, id_ts, id_te) with start time ts;
        // T[u][id_ts][id_te]: the index change time function which indicates the id-th index change time of u, 
        //               referred to as t(u, id_ts, id_te) with start time ts, note that T[u][id_ts][0] is the start time.
        std::vector<int> *Ssize;
        std::vector<std::vector<std::vector<std::vector<int>>>> S;
        std::vector<std::vector<std::vector<int>>> L;
        std::vector<std::vector<std::vector<int>>> T;
        std::vector<std::vector<int>> Ts;

        std::stringstream solve(int n, int ts, int te);

        OptimizedIndex() {}
        OptimizedIndex(TemporalGraph * Graph);
        ~OptimizedIndex() {}

};

void optimized(OptimizedIndex * Index, int vertex_num, char * query_file, char * output_file);