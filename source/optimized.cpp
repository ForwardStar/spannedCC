#include "optimized.h"

bool OptimizedIndex::addts(int u, int ts) {

    Ts[u].push_back(ts);
    T[u].push_back(std::vector<int>());
    L[u].push_back(std::vector<int>());
    S[u].push_back(std::vector<std::vector<int>>());
    Ssize[u].push_back(std::vector<int>());
    return true;

}

bool OptimizedIndex::addte(int u, int te) {

    if (T[u][T[u].size() - 1].size() == 0) {
        T[u][T[u].size() - 1].push_back(te);
        L[u][L[u].size() - 1].push_back(u);
        S[u][S[u].size() - 1].push_back(std::vector<int>());
        S[u][S[u].size() - 1][0].push_back(u);
        Ssize[u][Ssize[u].size() - 1].push_back(1);
        return true;
    }
    if (T[u][T[u].size() - 1][T[u][T[u].size() - 1].size() - 1] < te) {
        T[u][T[u].size() - 1].push_back(te);
        L[u][L[u].size() - 1].push_back(L[u][L[u].size() - 1][L[u][L[u].size() - 1].size() - 1]);
        S[u][S[u].size() - 1].push_back(std::vector<int>());
        Ssize[u][Ssize[u].size() - 1].push_back(Ssize[u][Ssize[u].size() - 1][Ssize[u][Ssize[u].size() - 1].size() - 1]);
        return true;
    }
    return false;

}

int OptimizedIndex::find(int u, int id_ts, int id_te) {
    
    if (id_te == -1 || L[u][id_ts].size() == 0) {
        return u;
    }
    return L[u][id_ts][std::min(id_te, int(L[u][id_ts].size()) - 1)];

}

void OptimizedIndex::unioN(int u, int v, int t) {

    int mount_u = find(u, L[u].size() - 1, L[u][L[u].size() - 1].size() - 1);
    int mount_v = find(v, L[v].size() - 1, L[v][L[v].size() - 1].size() - 1);

    // Already in the same connected component.
    if (mount_u == mount_v) {
        return;
    }

    // Create a new critical time slot.
    addte(mount_u, t);
    addte(mount_v, t);

    // Merge the smaller connected component into the larger one.
    if (Ssize[mount_u][Ssize[mount_u].size() - 1][Ssize[mount_u][Ssize[mount_u].size() - 1].size() - 1] < \
        Ssize[mount_v][Ssize[mount_v].size() - 1][Ssize[mount_v][Ssize[mount_v].size() - 1].size() - 1]) {
        std::swap(u, v);
        std::swap(mount_u, mount_v);
    }
    Ssize[mount_u][Ssize[mount_u].size() - 1][Ssize[mount_u][Ssize[mount_u].size() - 1].size() - 1] += Ssize[mount_v][Ssize[mount_v].size() - 1][Ssize[mount_v][Ssize[mount_v].size() - 1].size() - 1];
    Ssize[mount_v][Ssize[mount_v].size() - 1][Ssize[mount_v][Ssize[mount_v].size() - 1].size() - 1] = 0;

    std::vector<std::vector<int>>::iterator it;
    std::vector<int>::iterator it1;

    for (it = S[mount_v][S[mount_v].size() - 1].begin(); it != S[mount_v][S[mount_v].size() - 1].end(); it++) {
        for (it1 = it->begin(); it1 != it->end(); it1++) {
            addte(*it1, t);
            L[*it1][L[*it1].size() - 1][L[*it1][L[*it1].size() - 1].size() - 1] = mount_u;
            S[mount_u][S[mount_u].size() - 1][S[mount_u][S[mount_u].size() - 1].size() - 1].push_back(*it1);
        }
    }

    S[mount_v][S[mount_v].size() - 1][S[mount_v][S[mount_v].size() - 1].size() - 1].clear();
    S[mount_v][S[mount_v].size() - 1][S[mount_v][S[mount_v].size() - 1].size() - 1].push_back(-1);

}

int OptimizedIndex::binarySearchts(int u, int ts) {

    int l = 0;
    int r = Ts[u].size() - 1;
    while (l < r) {
        int mid = l + r + 1 >> 1;
        if (Ts[u][mid] <= ts) {
            l = mid;
        }
        else {
            r = mid - 1;
        }
    }

    return r;

}

int OptimizedIndex::binarySearchte(int u, int id_ts, int te) {

    int l = 0;
    int r = T[u][id_ts].size() - 1;
    while (l < r) {
        int mid = l + r + 1 >> 1;
        if (T[u][id_ts][mid] <= te) {
            l = mid;
        }
        else {
            r = mid - 1;
        }
    }

    if (r == -1 || T[u][id_ts][r] <= te) {
        return r;
    }
    else {
        return -1;
    }

}

std::stringstream OptimizedIndex::solve(int n, int ts, int te) {
    
    std::stringstream Ans;
    std::vector<bool> Vis(n);
    std::vector<int> CurrentCC;

    Ans << "The spanned connected components in [" << ts << ", " << te << "] are:\n";
    for (int u = 0; u < n; ++u) {
        int id_ts = binarySearchts(u, ts);
        int id_te = binarySearchte(u, id_ts, te);
        if (id_te == -1 || T[u][id_ts][id_te] > te) {
            Ans << "{ " << u << " }\n";
            continue;
        }
        int mount_u = find(u, id_ts, id_te);
        if (!Vis[mount_u]) {
            Vis[mount_u] = 1;
            int idx_ts = binarySearchts(mount_u, ts);
            int idx_te = binarySearchte(mount_u, idx_ts, te);

            CurrentCC.clear();
            std::vector<int>::iterator it;

            for (idx_te; idx_te >= 0; --idx_te) {
                for (it = S[mount_u][idx_ts][idx_te].begin(); it != S[mount_u][idx_ts][idx_te].end(); it++) {
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

OptimizedIndex::OptimizedIndex(TemporalGraph * Graph) {

    int start_time = time(NULL);

    // L_snapshot[u][id]: the label of u at time t(u, id);
    // S_snapshot[u][id]: the spanned CC mounted at u at time t(u, id);
    // T_snapshot[u][id]: the time t(u, id);
    // Vt[t]: the mounting points at time t;
    // isolated[t]: the isolated vertices at time t.
    std::vector<std::vector<int>> L_snapshot;
    std::vector<std::vector<std::vector<int>>> S_snapshot;
    std::vector<std::vector<int>> T_snapshot;
    std::vector<std::vector<int>> Vt;
    for (int u = 0; u < Graph->numOfVertices(); ++u) {
        L_snapshot.push_back(std::vector<int>());
        S_snapshot.push_back(std::vector<std::vector<int>>());
        T_snapshot.push_back(std::vector<int>());
    }

    for (int t = 0; t <= Graph->tmax; ++t) {
        std::vector<std::pair<int, int>>::iterator it;
        for (it = Graph->temporal_edge[t].begin(); it != Graph->temporal_edge[t].end(); it++) {
            int u = it->first;
            int v = it->second;
            if (T_snapshot[u].size() == 0 || T_snapshot[u][T_snapshot[u].size() - 1] < t) {
                L_snapshot[u].push_back(u);
                S_snapshot[u].push_back(std::vector<int>());
                S_snapshot[u][S_snapshot[u].size() - 1].push_back(u);
                T_snapshot[u].push_back(t);
            }
            if (T_snapshot[v].size() == 0 || T_snapshot[v][T_snapshot[v].size() - 1] < t) {
                L_snapshot[v].push_back(v);
                S_snapshot[v].push_back(std::vector<int>());
                S_snapshot[v][S_snapshot[v].size() - 1].push_back(v);
                T_snapshot[v].push_back(t);
            }
            if (L_snapshot[u][L_snapshot[u].size() - 1] != L_snapshot[v][L_snapshot[v].size() - 1]) {
                int mount_u = L_snapshot[u][L_snapshot[u].size() - 1];
                int mount_v = L_snapshot[v][L_snapshot[v].size() - 1];
                if (S_snapshot[mount_u][S_snapshot[mount_u].size() - 1].size() < S_snapshot[mount_v][S_snapshot[mount_v].size() - 1].size()) {
                    std::swap(u, v);
                    std::swap(mount_u, mount_v);
                }
                std::vector<int>::iterator it1;
                for (it1 = S_snapshot[mount_v][S_snapshot[mount_v].size() - 1].begin(); it1 != S_snapshot[mount_v][S_snapshot[mount_v].size() - 1].end(); it1++) {
                    L_snapshot[*it1][L_snapshot[*it1].size() - 1] = mount_u;
                    S_snapshot[mount_u][S_snapshot[mount_u].size() - 1].push_back(*it1);
                }
                S_snapshot[mount_v][S_snapshot[mount_v].size() - 1].clear();
            }
        }
        Vt.push_back(std::vector<int>());
        for (int u = 0; u < Graph->numOfVertices(); ++u) {
            if (T_snapshot[u].size() > 0 && T_snapshot[u][T_snapshot[u].size() - 1] == t && S_snapshot[u][S_snapshot[u].size() - 1].size() > 0) {
                Vt[t].push_back(u);
            }
        }
    }

    std::cout << "Preprocessing finished in " << timeFormatting(difftime(time(NULL), start_time)).str() << std::endl;

    int n = Graph->numOfVertices();
    int m = Graph->numOfEdges();
    int tmax = Graph->tmax;
    delete Graph;

    for (int u = 0; u < n; ++u) {
        Ssize.push_back(std::vector<std::vector<int>>());
        S.push_back(std::vector<std::vector<std::vector<int>>>());
        L.push_back(std::vector<std::vector<int>>());
        T.push_back(std::vector<std::vector<int>>());
        Ts.push_back(std::vector<int>());
    }

    start_time = time(NULL);
    for (int ts = 0; ts <= tmax; ++ts) {
        putProcess(double(ts) / tmax, difftime(time(NULL), start_time));
        if (ts == 0) {
            for (int u = 0; u < n; ++u) {
                addts(u, 0);
            }
        }
        else {

            std::vector<int>::iterator it;

            for (it = Vt[ts - 1].begin(); it != Vt[ts - 1].end(); it++) {
                if (Ts[*it][Ts[*it].size() - 1] == ts) {
                    continue;
                }
                int id_ts = L[*it].size() - 1;
                int id_te = binarySearchte(*it, id_ts, ts);
                int mount = find(*it, id_ts, id_te);
                id_ts = L[mount].size() - 1;
                id_te = binarySearchte(mount, id_ts, ts);

                int l = 0;
                int r = T_snapshot[*it].size() - 1;
                while (l < r) {
                    int mid = l + r + 1 >> 1;
                    if (T_snapshot[*it][mid] <= ts) {
                        l = mid;
                    }
                    else {
                        r = mid - 1;
                    }
                }

                if (T_snapshot[*it][r] != ts || Ssize[mount][id_ts][id_te] != S_snapshot[*it][r].size()) {
                    id_te = L[mount][id_ts].size() - 1;
                    mount = find(mount, id_ts, id_te);
                    id_ts = L[mount].size() - 1;
                    for (id_te = S[mount][id_ts].size() - 1; id_te >= 0; --id_te) {
                        std::vector<int>::iterator it1;
                        for (it1 = S[mount][id_ts][id_te].begin(); it1 != S[mount][id_ts][id_te].end(); it1++) {
                            addts(*it1, ts);
                        }
                    }
                }
            }

            for (it = Vt[ts].begin(); it != Vt[ts].end(); it++) {
                if (Ts[*it][Ts[*it].size() - 1] == ts) {
                    continue;
                }
                int id_ts = L[*it].size() - 1;
                int id_te = binarySearchte(*it, id_ts, ts);
                int mount = find(*it, id_ts, id_te);
                id_ts = L[mount].size() - 1;
                id_te = binarySearchte(mount, id_ts, ts);

                int l = 0;
                int r = T_snapshot[*it].size() - 1;
                while (l < r) {
                    int mid = l + r + 1 >> 1;
                    if (T_snapshot[*it][mid] <= ts) {
                        l = mid;
                    }
                    else {
                        r = mid - 1;
                    }
                }

                if (id_te == -1 || Ssize[mount][id_ts][id_te] != S_snapshot[*it][r].size()) {
                    if (id_te == -1) {
                        addts(mount, ts);
                        continue;
                    }
                    id_te = L[mount][id_ts].size() - 1;
                    mount = find(mount, id_ts, id_te);
                    id_ts = L[mount].size() - 1;
                    for (id_te = S[mount][id_ts].size() - 1; id_te >= 0; --id_te) {
                        std::vector<int>::iterator it1;
                        for (it1 = S[mount][id_ts][id_te].begin(); it1 != S[mount][id_ts][id_te].end(); it1++) {
                            addts(*it1, ts);
                        }
                    }
                }
            }

        }

        for (int te = ts; te <= tmax; ++te) {
            std::vector<int>::iterator it;
            for (it = Vt[te].begin(); it != Vt[te].end(); it++) {
                if (Ts[*it][Ts[*it].size() - 1] != ts) {
                    continue;
                }

                int l = 0;
                int r = T_snapshot[*it].size() - 1;
                while (l < r) {
                    int mid = l + r + 1 >> 1;
                    if (T_snapshot[*it][mid] <= te) {
                        l = mid;
                    }
                    else {
                        r = mid - 1;
                    }
                }

                std::vector<int>::iterator it1;
                for (it1 = S_snapshot[*it][r].begin(); it1 != S_snapshot[*it][r].end(); it1++) 
                    if (Ts[*it1][Ts[*it1].size() - 1] == ts) {
                        unioN(*it, *it1, te);
                    }
            }
        }

    }

}

void optimized(OptimizedIndex * Index, int vertex_num, char * query_file, char * output_file) {

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
        fout << Index->solve(vertex_num, ts, te).str() << std::endl;
    }

    std::cout << "Average (per query): " << timeFormatting(difftime(time(NULL), start_time) / query_num).str() << std::endl;

}