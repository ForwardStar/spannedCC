#include "commonfunctions.h"
#include "temporal_graph.h"

class FlattenedKruskal {

    private:

        int find_an_index(int t, int ts, int te);

    public:

        // n, m, tmax: graph information.
        int n, m, tmax;
        
        // each relation[t][t'][i] represents an edge.
        std::vector<std::vector<std::pair<int, int>>> *relation;
        std::vector<int> *actual_time;

        std::stringstream solve(int n, int ts, int te);

        FlattenedKruskal() {}
        FlattenedKruskal(TemporalGraph * Graph);
        ~FlattenedKruskal();

};

void flattened_kruskal(FlattenedKruskal * Index, int vertex_num, char * query_file, char * output_file);