#include <unordered_set>
#include "temporal_graph.h"

class BaselineIndex {

    private:

        // add(ts, u, t): add an index change time t of u with start time ts.
        bool add(int ts, int u, int t);

        // find(ts, u, id): find the label of u at time t(ts, u, id) with start time ts.
        int find(int ts, int u, int id);
    
        // unioN(ts, u, v, t): perform the union operation on u and v at time t with start time ts.
        void unioN(int ts, int u, int v, int t);

    public:

        // Ssize[ts][u][id]: the size of connected component at u at time t(ts, u, id) with start time ts, calculated by prefix sum;
        // S[ts][u][id]: the connected component mounted at u at time t(ts, u, id) with start time ts;
        // L[ts][u][id]: the label of u at time t(ts, u, id) with start time ts;
        // T[ts][u][id]: the index change time function which indicates the id-th index change time of u, referred to as t(ts, u, id) with start time ts.
        std::vector<std::map<int, std::vector<int>>> Ssize;
        std::vector<std::map<int, std::vector<std::unordered_set<int>>>> S;
        std::vector<std::map<int, std::vector<int>>> L;
        std::vector<std::map<int, std::vector<int>>> T;

        std::stringstream solve(TemporalGraph * Graph, int ts, int te);

        BaselineIndex(TemporalGraph * Graph);
        ~BaselineIndex() {}

};

void baseline(TemporalGraph * Graph, char * query_file, char * output_file);