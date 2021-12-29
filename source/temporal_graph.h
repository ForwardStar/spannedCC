#ifndef temporal_graph
#define temporal_graph

#include "commonfunctions.h"

class TemporalGraph {
       
    public:

        // Edge: the structure of the edges in the temporal graph.
        struct Edge {

            // to: the destination of the edge.
            int to;

            // interaction_time: time of the interaction.
            int interaction_time;

            // next: the next edge in linked list structure.
            Edge * next;

            Edge(int v, int t, Edge * nextptr): to(v), interaction_time(t), next(nextptr) {}
            ~Edge() {}

        };

        // n: the number of vertices; m: the number of edges.
        int n, m;

        // tmax: the maximum time of all temporal edges.
        int tmax;

        // vertex_set: the set of vertices.
        std::set<int> vertex_set;

        // head_edge[vertex] --> the head edge from this vertex.
        std::map<int, Edge *> head_edge;

        // temporal_edge[t] --> the edge set at time t.
        std::vector<std::vector<std::pair<int, int>>> temporal_edge;

        // is_directed: whether the graph is a directed graph;
        // is_online: whether the solution is online search.
        bool is_directed, is_online;

        // numOfVertices(): get the number of the vertices in the graph.
        int numOfVertices();

        // numOfEdges(): get the number of the edges in the graph.  
        int numOfEdges();

        // getHeadEdge(u): get the head edge of vertex u.
        Edge * getHeadEdge(int u);

        // getNextEdge(e): get the next edge of edge e.
        Edge * getNextEdge(Edge * e);

        // getDestination(e): get the destination of the edge e.
        int getDestination(Edge * e);

        // getInteractionTime(e): get the time of the interaction.
        int getInteractionTime(Edge * e);

        // addEdge(u, v, t): add an edge (u, v, t) to the graph.
        void addEdge(int u, int v, int t);

        TemporalGraph() {}
        TemporalGraph(char *graph_file, char *graph_type, char *solution_type);
        ~TemporalGraph();

};

#endif