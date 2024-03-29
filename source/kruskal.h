#include "commonfunctions.h"
#include "temporal_graph.h"

class KruskalReconstructionTree {

    private:

        // find(ts, u, id): find the label of u at time t(ts, u, id) with start time ts.
        int find(int ts, int u);
    
        // unioN(ts, u, v, t): perform the union operation on u and v at time t with start time ts.
        void unioN(int ts, int u, int v, int t);

    public:

        // n, m, tmax: graph information.
        int n, m, tmax;
        
        int **L;
        int **T;
        int *size;

        std::stringstream solve(int n, int ts, int te);

        KruskalReconstructionTree() {}
        KruskalReconstructionTree(TemporalGraph * Graph);
        ~KruskalReconstructionTree();

};

void kruskal(KruskalReconstructionTree * Index, int vertex_num, char * query_file, char * output_file);