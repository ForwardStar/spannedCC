#include "revised_kruskal.h"

int RevisedKruskal::find(int ts, int u) {
    
    if (L[u][ts] != u) {
        return find(ts, L[u][ts]);
    }

    return u;

}

void RevisedKruskal::unioN(int ts, int u, int v, int t) {

    int mount_u = find(ts, u);
    int mount_v = find(ts, v);

    // Already in the same connected component.
    if (mount_u == mount_v) {
        return;
    }

    // Merge the smaller connected component into the larger one.
    if (size[mount_u] < size[mount_v]) {
        std::swap(u, v);
        std::swap(mount_u, mount_v);
    }
    size[mount_u] += size[mount_v];
    
    L[mount_v][ts] = mount_u;
    T[mount_v][ts] = t;

}

std::stringstream RevisedKruskal::solve(int n, int ts, int te) {
    
    std::stringstream Ans;
    int *Vis = new int[n];
    std::vector<int> *CurrentCC = new std::vector<int>[n]();

    Ans << "The spanned connected components in [" << ts << ", " << te << "] are:\n";
    for (int u = 0; u < n; ++u) {
        Vis[u] = -1;
    }

    for (int u = 0; u < n; ++u) {
        if (Vis[u] == -1) {
            Vis[u] = u;
            CurrentCC[u].push_back(u);

            int current = u;
            while (ts <= length[current] && L[current][ts] != current && T[current][ts] <= te) {
                current = L[current][ts];
                if (Vis[current] != -1) {
                    std::vector<int>::iterator it;
                    for (it = CurrentCC[u].begin(); it != CurrentCC[u].end(); it++) {
                        Vis[*it] = Vis[current];
                        CurrentCC[Vis[current]].push_back(*it);
                    }
                    CurrentCC[u].clear();
                    break;
                }
                CurrentCC[u].push_back(current);
                Vis[current] = u;
            }
        }
    }

    for (int u = 0; u < n; ++u) {
        if (CurrentCC[u].size() == 0) {
            continue;
        }
        std::sort(CurrentCC[u].begin(), CurrentCC[u].end());
        std::vector<int>::iterator it;
        Ans << "{ ";
        for (it = CurrentCC[u].begin(); it != CurrentCC[u].end(); it++) {
            Ans << *it << " ";
        }
        Ans << "}\n";
    }

    delete [] Vis;
    delete [] CurrentCC;

    return Ans;

}

RevisedKruskal::RevisedKruskal(TemporalGraph * Graph) {
    
    int start_time = time(NULL);
    Graph->shrink_to_fit();

    start_time = time(NULL);

    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    L = new int *[n];
    T = new int *[n];
    length = new int[n];
    size = new int[n];

    for (int u = 0; u < n; ++u) {
        length[u] = 0;
    }

    for (int t = 1; t <= tmax; ++t) {
        std::vector<std::pair<int, int>>::iterator it;
        for (it = Graph->temporal_edge[t].begin(); it != Graph->temporal_edge[t].end(); it++) {
            if (length[it->first] < t) {
                length[it->first] = t;
            }
            if (length[it->second] < t) {
                length[it->second] = t;
            }
        }
    }

    for (int u = 0; u < n; ++u) {
        L[u] = new int[length[u] + 1];
        T[u] = new int[length[u] + 1];
        for (int t = 0; t <= length[u]; ++t) {
            L[u][t] = u;
            T[u][t] = t;
        }
    }
    std::cout << "Preprocessing finished in " << timeFormatting(difftime(time(NULL), start_time)).str() << std::endl;
    
    for (int ts = 0; ts <= tmax; ++ts) {
        for (int u = 0; u < n; ++u) {
            size[u] = 1;
        }
        for (int te = ts; te <= tmax; ++te) {
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[te].begin(); it != Graph->temporal_edge[te].end(); it++) {
                unioN(ts, it->first, it->second, te);
            }
        }
        putProcess(double(ts) / tmax, difftime(time(NULL), start_time));
    }

    delete [] size;

}

RevisedKruskal::~RevisedKruskal() {

    delete [] L;
    delete [] T;

}

void revised_kruskal(RevisedKruskal * Index, int vertex_num, char * query_file, char * output_file) {

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