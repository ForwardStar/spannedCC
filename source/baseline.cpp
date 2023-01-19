#include "baseline.h"

bool BaselineIndex::add(int ts, int u, int t) {

    if (T[ts][u].size() == 0) {
        T[ts][u].push_back(t);
        L[ts][u].push_back(u);
        S[ts][u].push_back(std::vector<int>());
        S[ts][u][0].push_back(u);
        return true;
    }
    else if (T[ts][u][T[ts][u].size() - 1] < t) {
        T[ts][u].push_back(t);
        L[ts][u].push_back(L[ts][u][L[ts][u].size() - 1]);
        S[ts][u].push_back(std::vector<int>());
        return true;
    }

    return false;

}

int BaselineIndex::find(int ts, int u, int id) {
    
    if (L[ts][u].size() == 0) {
        return u;
    }
    else {
        return L[ts][u][std::min(id, int(L[ts][u].size()) - 1)];
    }

}

void BaselineIndex::unioN(int ts, int u, int v, int t) {

    int mount_u = find(ts, u, L[ts][u].size() - 1);
    int mount_v = find(ts, v, L[ts][v].size() - 1);

    // Already in the same connected component.
    if (mount_u == mount_v) {
        return;
    }

    // Create a new critical time slot.
    add(ts, mount_u, t);
    add(ts, mount_v, t);

    // Merge the smaller connected component into the larger one.
    if (Ssize[mount_u] < Ssize[mount_v]) {
        std::swap(u, v);
        std::swap(mount_u, mount_v);
    }
    Ssize[mount_u] += Ssize[mount_v];

    std::vector<std::vector<int>>::iterator it;
    std::vector<int>::iterator it1;

    for (it = S[ts][mount_v].begin(); it != S[ts][mount_v].end(); it++) {
        for (it1 = it->begin(); it1 != it->end(); it1++) {
            add(ts, *it1, t);
            L[ts][*it1][L[ts][*it1].size() - 1] = mount_u;
            S[ts][mount_u][S[ts][mount_u].size() - 1].push_back(*it1);
        }
    }

    S[ts][mount_v][S[ts][mount_v].size() - 1].clear();
    S[ts][mount_v][S[ts][mount_v].size() - 1].push_back(-1);

}

int BaselineIndex::binarySearch(int ts, int u, int t) {

    int l = 0;
    int r = T[ts][u].size() - 1;
    while (l < r) {
        int mid = l + r + 1 >> 1;
        if (T[ts][u][mid] <= t) {
            l = mid;
        }
        else {
            r = mid - 1;
        }
    }

    return r;

}

std::stringstream BaselineIndex::solve(int n, int ts, int te) {
    
    std::stringstream Ans;
    std::vector<bool> Vis(n);
    std::vector<int> CurrentCC;

    Ans << "The spanned connected components in [" << ts << ", " << te << "] are:\n";
    for (int u = 0; u < n; ++u) {
        int idx = binarySearch(ts, u, te);
        if (idx == -1 || T[ts][u][idx] > te) {
            Ans << "{ " << u << " }\n";
            continue;
        }
        int mount_u = L[ts][u][idx];
        if (!Vis[mount_u]) {
            Vis[mount_u] = 1;
            idx = binarySearch(ts, mount_u, te);

            CurrentCC.clear();

            std::vector<int>::iterator it;
            for (idx; idx >= 0; --idx) {
                for (it = S[ts][mount_u][idx].begin(); it != S[ts][mount_u][idx].end(); it++) {
                    if (*it != -1) {
                        CurrentCC.push_back(*it);
                    }
                }
            }
            if (CurrentCC.size() == 0) {
                CurrentCC.push_back(u);
            }
            std::sort(CurrentCC.begin(), CurrentCC.end());
            Ans << "{ ";
            for (it = CurrentCC.begin(); it != CurrentCC.end(); it++) {
                Ans << *it << " ";
            }
            Ans << "}\n";
        }
    }

    return Ans;

}

void BaselineIndex::update(TemporalGraph * Graph) {
    Ssize = new int[n];

    for (int ts = 0; ts <= tmax; ++ts) {
        if (ts > t1) {
            T[ts] = new std::vector<int>[n]();
            L[ts] = new std::vector<int>[n]();
            S[ts] = new std::vector<std::vector<int>>[n]();
        }
        for (int u = 0; u < Graph->numOfVertices(); ++u) {
            if (L[ts][u].size() == 0) {
                Ssize[u] = 1;
            } else {
                Ssize[L[ts][u][L[ts][u].size() - 1]] = 1;
            }
        }
        for (int te = (ts <= t1 ? t1 + 1 : 0); te <= tmax; ++te) {
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[te].begin(); it != Graph->temporal_edge[te].end(); it++) {
                unioN(ts, it->first, it->second, te);
            }
        }
    }

    delete Ssize;
}

BaselineIndex::BaselineIndex(TemporalGraph * Graph, double t_fraction) {
    
    unsigned long long start_time = currentTime();
    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    t1 = int(tmax * t_fraction);
    T = new std::vector<int> *[tmax + 1]();
    L = new std::vector<int> *[tmax + 1]();
    Ssize = new int[n];
    S = new std::vector<std::vector<int>> *[tmax + 1]();
    for (int ts = 0; ts <= t1; ++ts) {
        for (int u = 0; u < Graph->numOfVertices(); ++u) {
            Ssize[u] = 1;
        }
        T[ts] = new std::vector<int>[n]();
        L[ts] = new std::vector<int>[n]();
        S[ts] = new std::vector<std::vector<int>>[n]();
        for (int te = ts; te <= t1; ++te) {
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[te].begin(); it != Graph->temporal_edge[te].end(); it++) {
                unioN(ts, it->first, it->second, te);
            }
        }
        putProcess(double(ts) / t1, currentTime() - start_time);
    }

    delete [] Ssize;

}

BaselineIndex::~BaselineIndex() {

    for (int ts = 0; ts <= tmax; ++ts) {
        delete [] T[ts];
        delete [] L[ts];
        delete [] S[ts];
    }
    delete [] T;
    delete [] L;
    delete [] S;

}

unsigned long long BaselineIndex::size() {

    unsigned long long memory = 0;
    for (int ts = 0; ts <= tmax; ++ts) {
        for (int u = 0; u < n; ++u) {
            memory += T[ts][u].size();
        }
    }
    memory <<= 3;
    return memory;

}

void baseline(BaselineIndex * Index, int vertex_num, char * query_file, char * output_file) {

    int ts, te;
    int query_num = 0;
    std::ifstream fin(query_file);
    std::ofstream fout(output_file);

    while (fin >> ts >> te) {
        ++query_num;
    }

    fin = std::ifstream(query_file);

    int i = 0;
    unsigned long long start_time = currentTime();
    while (fin >> ts >> te) {
        fout << Index->solve(vertex_num, ts, te).str() << std::endl;
        putProcess(double(++i) / query_num, currentTime() - start_time);
    }

    std::cout << "Average (per query): " << timeFormatting((currentTime() - start_time) / query_num).str() << std::endl;

}
