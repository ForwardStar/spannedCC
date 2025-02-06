#include "commonfunctions.h"
#include "temporal_graph.h"

class ctsf {

    public:

        //int find_an_index(int t, int ts, int te);

        struct edge{
            int u,v,ts;
            edge(int u1=0,int v1=0, int t1=0){
                u=u1;v=v1;ts=t1;
            }
        };

        // n, m, tmax: graph information.
        int n, m, tmax, t1;
        
        // each relation[t][i] represents an edge appearing at t
        std::vector<edge> *relation;
        //std::vector<int> *actual_time;
        int find(int* parent, int u);
        bool unioN(int* parent, int u,int v);
        std::stringstream solve(int n, int ts, int te);
        void construct_disjoint_set(int * parent, int ts, int te);
        //void update(TemporalGraph * Graph);

        ctsf() {}
        ctsf(TemporalGraph * Graph, double t_fraction);
        ~ctsf() {};

        unsigned long long size();

};

void Ctsf (ctsf * Index, int vertex_num, char * query_file, char * output_file);