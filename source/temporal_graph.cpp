#include "temporal_graph.h"

int TemporalGraph::numOfVertices() {

    return n;

}

int TemporalGraph::numOfEdges() {

    return m;

}

TemporalGraph::Edge * TemporalGraph::getHeadEdge(int u) {

    return head_edge[u];

}

TemporalGraph::Edge * TemporalGraph::getNextEdge(Edge * e) {

    return e->next;

}

int TemporalGraph::getDestination(Edge * e) {

    return e->to;

}

int TemporalGraph::getInteractionTime(Edge * e) {

    return e->interaction_time;

}

void TemporalGraph::addEdge(int u, int v, int t) {

    if (vertex_set.find(u) == vertex_set.end()) {
        vertex_set.insert(u);
    }
    if (vertex_set.find(v) == vertex_set.end()) {
        vertex_set.insert(v);
    }
    if (head_edge.count(u)) {
        head_edge[u] = new Edge(v, t, head_edge[u]);
    }
    else {
        head_edge[u] = new Edge(v, t, nullptr);
    }

}

TemporalGraph::TemporalGraph(char *graph_file, char *graph_type) {

    int u, v, t;
    std::ifstream fin(graph_file);

    is_directed = graph_type == "Directed";
    while (fin >> u >> v >> t) {
        while (temporal_edge.size() < t + 1) {
            temporal_edge.push_back(std::vector<std::pair<int, int>>());
        }
        tmax = std::max(tmax, t);
        ++m;
        temporal_edge[t].push_back(std::make_pair(u, v));
        addEdge(u, v, t);
        if (!is_directed) {
            addEdge(v, u, t);
        }
    }

    n = vertex_set.size();

}

TemporalGraph::~TemporalGraph() {

    std::map<int, Edge *>::iterator it;

    for (it = head_edge.begin(); it != head_edge.end(); it++) {
        Edge * temp = it->second;
        while (temp) {
            Edge * next = temp->next;
            delete temp;
            temp = next;
        }
    }

}