#include"commonfunctions.h"
#include"temporal_graph.h"

class OptimizedBaseline{
    private:

        bool add(int ts, int u, int t);

        int find(int u);

        void optimized_unioN(int ts, int u, int v, int t);

        int binarysearch(int ts, int u, int t);

    public:

        int n, m, tmax;

        std::vector<std::vector<std::pair<int,int>>> *dS;

        std::vector<int> L;

        std::stringstream solve(int n, int ts, int te);

        OptimizedBaseline() {}
        OptimizedBaseline(TemporalGraph * Graph);
        ~OptimizedBaseline();
};

void OBaseline(OptimizedBaseline * Index, int vertex_num, char * query_file, char * output_file);