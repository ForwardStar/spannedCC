#include "online_search.h"

// Please refer to the pseudo code in Algorithm 1.
std::stringstream onlineSearch(TemporalGraph * Graph, int ts, int te) {

    std::stringstream Ans;
    std::unordered_set<int> Vis;
    std::set<int> CurrentCC;
    std::queue<int> Q;
    TemporalGraph * G = new TemporalGraph(Graph, ts, te);
    
    Ans << "The spanned connected components in [" << ts << ", " << te << "] are:\n";
    for (int u = 0; u < G->numOfVertices(); ++u) {
        if (Vis.find(u) == Vis.end()) {
            Q.push(u);
            CurrentCC.clear();
            CurrentCC.insert(u);
            while (!Q.empty()) {
                int v = Q.front();
                Q.pop();
                Vis.insert(v);
                TemporalGraph::Edge * edge = G->getHeadEdge(v);
                while (edge) {
                    if (Vis.find(edge->to) == Vis.end()) {
                        Q.push(edge->to);
                        CurrentCC.insert(edge->to);
                        Vis.insert(edge->to);
                    }
                    edge = G->getNextEdge(edge);
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

    delete G;

    return Ans;

}

void online(TemporalGraph * Graph, char * query_file, char * output_file) {
    
    int ts, te;
    int query_num = 0;
    std::ifstream fin(query_file);
    std::ofstream fout(output_file);

    while (fin >> ts >> te) {
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