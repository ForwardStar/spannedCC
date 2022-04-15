#include "commonfunctions.h"
#include "temporal_graph.h"

class RevisedKruskal {

    private:

        // find(ts, u, id): find the label of u at time t(ts, u, id) with start time ts.
        int find(int u, int &t, int &min_keep);
    
        // unioN(ts, u, v, t): perform the union operation on u and v at time t with start time ts.
        void unioN(int ts, int u, int v, int t, int min_keep);

        int binarySearch(int u, int ts);

    public:

        // n, m, tmax: graph information.
        int n, m, tmax;
        
        std::vector<int> *L;
        std::vector<int> *T;
        std::vector<int> *Ts;
        int *keep;
        int *size;
        int num = 0;
        int total_num = 0;

        std::stringstream solve(int n, int ts, int te);

        RevisedKruskal() {}
        RevisedKruskal(TemporalGraph * Graph);
        ~RevisedKruskal();

};

void revised_kruskal(RevisedKruskal * Index, int vertex_num, char * query_file, char * output_file);