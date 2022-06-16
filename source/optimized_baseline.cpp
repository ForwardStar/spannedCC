#include "optimized_baseline.h"

int OptimizedBaseline::find(int u) {

    if (L[u] == u) {
        return u;
    }

    L[u] = find(L[u]);

    return L[u];

}

void OptimizedBaseline::optimized_unioN(int ts, int u, int v, int t) {

    int mount_u = find(u);
    int mount_v = find(v);

    if (mount_u == mount_v) {
        return;
    }

    if (mount_u > mount_v) {
        std::swap(mount_u, mount_v);
    }

    dS[ts][mount_u].push_back(std::pair<int,int>(t, mount_v));
    L[mount_v] = mount_u;

}

std::stringstream OptimizedBaseline::solve(int n, int ts, int te) {

    std::stringstream Ans;
    std::vector<int> Vis(n, 0);
    std::queue<int> Q;
    std::vector<int> *S = new std::vector<int>[n]();

    Ans << "The spanned connected components in [" << ts << ", " << te << "] are:\n";
    for (int u = 0; u < n; ++u) {
        if (dS[ts][u].empty()) {
            continue;
        }
        std::vector<std::pair<int,int>>::iterator it;
        for (it = dS[ts][u].begin(); it != dS[ts][u].end(); it++) {
            if (it->first > te) {
                break;
            }
            S[u].push_back(it->second);
            Vis[it->second]++;
        }
    }

    std::vector<int> CurrentCC;

    for (int u = 0; u < n; ++u) {
        if (!Vis[u]) {
            Vis[u] = 1;
            Q.push(u);
            CurrentCC.clear();
            CurrentCC.push_back(u);
            while (!Q.empty()) {
                int v = Q.front();
                Q.pop();
                for (auto to : S[v]) {
                    CurrentCC.push_back(to);
                    Q.push(to);
                }
            }
            std::sort(CurrentCC.begin(),CurrentCC.end());
            Ans << "{ ";
            std::vector<int>::iterator it;
            for (it = CurrentCC.begin(); it != CurrentCC.end(); it++) {
                Ans << *it << " ";
            }
            Ans << "}\n";
        }
    }

    delete [] S;
    return Ans;

}


OptimizedBaseline::OptimizedBaseline(TemporalGraph * Graph){

    int start_time = time(NULL);

    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    dS = new std::vector<std::vector<std::pair<int,int>>>[tmax + 1]();
    L.resize(n);

    for (int ts = 0; ts <= tmax; ++ts) {
        dS[ts].resize(n);
        for (int i = 0; i < n; ++i) {
            L[i] = i;
        }
        for (int te = ts; te <= tmax; ++te) {
            if (Graph->temporal_edge[te].empty()) {
                continue;
            }
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[te].begin(); it!=Graph->temporal_edge[te].end(); it++) {
                optimized_unioN(ts, it->first, it->second, te);
            }
        }
        putProcess(double(ts) / tmax, difftime(time(NULL), start_time));
    }

}

OptimizedBaseline::~OptimizedBaseline() {

    delete [] dS;

}

void optimized_baseline(OptimizedBaseline * Index, int vertex_num, char * query_file, char * output_file) {

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
        fout << Index->solve(vertex_num, ts, te).str() << std::endl;
        putProcess(double(++i) / query_num, difftime(time(NULL), start_time));
    }

    std::cout << "Average (per query): " << timeFormatting(difftime(time(NULL), start_time) / query_num).str() << std::endl;

}