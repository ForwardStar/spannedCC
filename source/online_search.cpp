#include "online_search.h"

// Please refer to the pseudo code in Algorithm 1.
std::stringstream onlineSearch(TemporalGraph * Graph, int ts, int te) {

    std::stringstream Ans;
    std::vector<bool> Vis(Graph->numOfVertices());
    std::vector<int> CurrentCC;
    std::queue<int> Q;
    TemporalGraph * G = new TemporalGraph(Graph, ts, te);
    
    Ans << "The spanned connected components in [" << ts << ", " << te << "] are:\n";
    for (int u = 0; u < G->numOfVertices(); ++u) {
        if (!Vis[u]) {
            Vis[u] = 1;
            Q.push(u);
            CurrentCC.clear();
            CurrentCC.push_back(u);
            while (!Q.empty()) {
                int v = Q.front();
                Q.pop();
                TemporalGraph::Edge * edge = G->getHeadEdge(v);
                while (edge) {
                    if (!Vis[edge->to]) {
                        Q.push(edge->to);
                        CurrentCC.push_back(edge->to);
                        Vis[edge->to] = 1;
                    }
                    edge = G->getNextEdge(edge);
                }
            }
            std::vector<int>::iterator it;
            std::sort(CurrentCC.begin(), CurrentCC.end());
            Ans << "{ ";
            for (it = CurrentCC.begin(); it != CurrentCC.end(); it++) {
                Ans << *it << " ";
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
        // Perform online BFS Search
        fout << onlineSearch(Graph, ts, te).str() << std::endl;
        putProcess(double(++i) / query_num, difftime(time(NULL), start_time));
    }

    std::cout << "Average: " << timeFormatting(difftime(time(NULL), start_time) / query_num).str() << std::endl;

}