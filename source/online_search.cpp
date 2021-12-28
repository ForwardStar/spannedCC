#include "online_search.h"

// Please refer to the pseudo code in Algorithm 1.
std::stringstream onlineSearch(TemporalGraph * Graph, int ts, int te) {

    std::stringstream Ans;
    std::unordered_set<int> Vis;
    std::set<int>::iterator it;
    std::queue<int> Q;
    
    Ans << "The spanned connected components in [" << ts << ", " << te << "] are:\n";
    for (it = Graph->vertex_set.begin(); it != Graph->vertex_set.end(); it++) {
        if (Vis.find(*it) == Vis.end()) {
            Q.push(*it);
            std::set<int> CurrentCC;
            CurrentCC.insert(*it);
            while (!Q.empty()) {
                int u = Q.front();
                Q.pop();
                Vis.insert(u);
                TemporalGraph::Edge * edge = Graph->getHeadEdge(u);
                while (edge) {
                    if (ts <= edge->interaction_time && edge->interaction_time <= te && Vis.find(edge->to) == Vis.end()) {
                        Q.push(edge->to);
                        CurrentCC.insert(edge->to);
                        Vis.insert(edge->to);
                    }
                    edge = Graph->getNextEdge(edge);
                }
            }
            std::set<int>::iterator set_iterator;
            Ans << "{ ";
            for (set_iterator = CurrentCC.begin(); set_iterator != CurrentCC.end(); set_iterator++) {
                Ans << *set_iterator << " ";
            }
            Ans << "}\n";
        }
    }

    return Ans;

}

void online(TemporalGraph * Graph, char * query_file, char * output_file) {
    
    int ts, te, tmax;
    int query_num = 0;
    std::ifstream fin(query_file);
    std::ofstream fout(output_file);

    while (fin >> ts >> te) {
        tmax = std::max(tmax, te);
        ++query_num;
    }

    fin = std::ifstream(query_file);

    int i = 0;
    int start_time = time(NULL);
    while (fin >> ts >> te) {
        putProcess(double(++i) / query_num, difftime(time(NULL), start_time));
        // Perform online BFS Search
        fout << onlineSearch(Graph, ts, te).str() << std::endl;
    }

    std::cout << "Average: " << timeFormatting(difftime(time(NULL), start_time) / query_num).str() << std::endl;

}