#include "baseline.h"

bool BaselineIndex::add(int ts, int u, int t) {

    if (T_temp[ts][u].size() == 0) {
        T_temp[ts][u].push_back(t);
        L_temp[ts][u].push_back(u);
        S_temp[ts][u].push_back(std::vector<int>());
        S_temp[ts][u][0].push_back(u);
        Ssize[u].push_back(1);
        return true;
    }
    else if (T_temp[ts][u][T_temp[ts][u].size() - 1] < t) {
        T_temp[ts][u].push_back(t);
        L_temp[ts][u].push_back(L_temp[ts][u][L_temp[ts][u].size() - 1]);
        S_temp[ts][u].push_back(std::vector<int>());
        Ssize[u].push_back(Ssize[u][Ssize[u].size() - 1]);
        return true;
    }

    return false;

}

int BaselineIndex::find(int ts, int u, int id) {
    
    if (L_temp[ts][u].size() == 0) {
        return u;
    }
    else {
        return L_temp[ts][u][std::min(id, int(L_temp[ts][u].size()) - 1)];
    }

}

void BaselineIndex::unioN(int ts, int u, int v, int t) {

    int mount_u = find(ts, u, L_temp[ts][u].size() - 1);
    int mount_v = find(ts, v, L_temp[ts][v].size() - 1);

    // Already in the same connected component->
    if (mount_u == mount_v) {
        return;
    }

    // Create a new critical time slot->
    add(ts, mount_u, t);
    add(ts, mount_v, t);

    // Merge the smaller connected component into the larger one->
    if (Ssize[mount_u][Ssize[mount_u].size() - 1] < Ssize[mount_v][Ssize[mount_v].size() - 1]) {
        std::swap(u, v);
        std::swap(mount_u, mount_v);
    }
    Ssize[mount_u][Ssize[mount_u].size() - 1] += Ssize[mount_v][Ssize[mount_v].size() - 1];
    Ssize[mount_v][Ssize[mount_v].size() - 1] = 0;

    std::vector<std::vector<int>>::iterator it;
    std::vector<int>::iterator it1;

    for (it = S_temp[ts][mount_v].begin(); it != S_temp[ts][mount_v].end(); it++) {
        for (it1 = it->begin(); it1 != it->end(); it1++) {
            add(ts, *it1, t);
            L_temp[ts][*it1][L_temp[ts][*it1].size() - 1] = mount_u;
            S_temp[ts][mount_u][S_temp[ts][mount_u].size() - 1].push_back(*it1);
        }
    }

    S_temp[ts][mount_v][S_temp[ts][mount_v].size() - 1].clear();
    S_temp[ts][mount_v][S_temp[ts][mount_v].size() - 1].push_back(-1);

}

int BaselineIndex::binarySearch(int ts, int u, int t) {

    int l = 0;
    int r = size[ts][u] - 1;
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

            for (idx; idx >= 0; --idx) {
                for (int s = 1; s <= S[ts][mount_u][idx][0]; ++s) {
                    if (S[ts][mount_u][idx][s] != -1) {
                        CurrentCC.push_back(S[ts][mount_u][idx][s]);
                    }
                }
            }
            if (CurrentCC.size() == 0) {
                CurrentCC.push_back(u);
            }
            std::sort(CurrentCC.begin(), CurrentCC.end());
            std::vector<int>::iterator it;
            Ans << "{ ";
            for (it = CurrentCC.begin(); it != CurrentCC.end(); it++) {
                Ans << *it << " ";
            }
            Ans << "}\n";
        }
    }

    return Ans;

}

BaselineIndex::BaselineIndex(TemporalGraph * Graph) {
    
    int start_time = time(NULL);
    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    T_temp = new std::vector<int> *[tmax]();
    L_temp = new std::vector<int> *[tmax]();
    Ssize = new std::vector<int>[n]();
    S_temp = new std::vector<std::vector<int>> *[tmax]();
    for (int ts = 0; ts <= tmax; ++ts) {
        T_temp[ts] = new std::vector<int>[n]();
        L_temp[ts] = new std::vector<int>[n]();
        S_temp[ts] = new std::vector<std::vector<int>>[n]();
        for (int te = ts; te <= Graph->tmax; ++te) {
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[te].begin(); it != Graph->temporal_edge[te].end(); it++) {
                unioN(ts, it->first, it->second, te);
            }
        }
        for (int u = 0; u < Graph->numOfVertices(); ++u) {
            Ssize[u].clear();
        }
        putProcess(double(ts) / tmax, difftime(time(NULL), start_time));
    }

    delete [] Ssize;

    T = new int **[tmax];
    L = new int **[tmax];
    S = new int ***[tmax];
    size = new int *[tmax];

    for (int ts = 0; ts <= tmax; ++ts) {
        T[ts] = new int *[n];
        L[ts] = new int *[n];
        S[ts] = new int **[n];
        size[ts] = new int[n];
        for (int u = 0; u < n; ++u) {
            size[ts][u] = T_temp[ts][u].size();
            T[ts][u] = new int[size[ts][u]];
            L[ts][u] = new int[size[ts][u]];
            S[ts][u] = new int *[size[ts][u]];
            for (int s = 0; s < size[ts][u]; ++s) {
                T[ts][u][s] = T_temp[ts][u][s];
                L[ts][u][s] = L_temp[ts][u][s];
                S[ts][u][s] = new int[S_temp[ts][u][s].size() + 1];
                S[ts][u][s][0] = 0;
                std::vector<int>::iterator it;
                for (it = S_temp[ts][u][s].begin(); it != S_temp[ts][u][s].end(); it++) {
                    S[ts][u][s][++S[ts][u][s][0]] = *it;
                }
            }
        }
        delete [] T_temp[ts];
        delete [] L_temp[ts];
        delete [] S_temp[ts];
    }

    delete [] T_temp;
    delete [] L_temp;
    delete [] S_temp;

}

BaselineIndex::~BaselineIndex() {

    for (int ts = 0; ts <= tmax; ++ts) {
        for (int u = 0; u < n; ++u) {
            delete T[ts][u];
            delete L[ts][u];
            for (int s = 0; s < size[ts][u]; ++s) {
                delete S[ts][u][s];
            }
            delete S[ts][u];
        }
        delete T[ts];
        delete L[ts];
        delete S[ts];
        delete size[ts];
    }
    delete T;
    delete L;
    delete S;
    delete size;

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
    int start_time = time(NULL);
    while (fin >> ts >> te) {
        fout << Index->solve(vertex_num, ts, te).str() << std::endl;
        putProcess(double(++i) / query_num, difftime(time(NULL), start_time));
    }

    std::cout << "Average (per query): " << timeFormatting(difftime(time(NULL), start_time) / query_num).str() << std::endl;

}