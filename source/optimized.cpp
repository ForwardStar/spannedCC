#include "optimized.h"

OptimizedIndex::OptimizedIndex(TemporalGraph * Graph) {

    // L_snapshot[u][id]: the label of u at time t(u, id);
    // S_snapshot[u][id]: the spanned CC mounted at u at time t(u, id);
    // T_snapshot[u][id]: the time t(u, id);
    // Vt[t]: the mounting points at time t.
    std::vector<std::vector<int>> L_snapshot;
    std::vector<std::vector<std::vector<int>>> S_snapshot;
    std::vector<std::vector<int>> T_snapshot;
    std::vector<std::unordered_set<int>> Vt;
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
            if (L_snapshot[u] != L_snapshot[v]) {
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
        Vt.push_back(std::unordered_set<int>());
        for (int u = 0; u < Graph->numOfVertices(); ++u) {
            if (T_snapshot[u].size() > 0 && T_snapshot[u][T_snapshot[u].size() - 1] == t && S_snapshot[u][S_snapshot[u].size() - 1].size() > 0) {
                Vt[t].insert(u);
            }
        }
    }

    int n = Graph->numOfVertices();
    int m = Graph->numOfEdges();
    int tmax = Graph->tmax;
    delete Graph;

    for (int u = 0; u < n; ++u) {
        Ssize.push_back(std::vector<std::vector<int>>());
        S.push_back(std::vector<std::vector<std::vector<int>>>());
        L.push_back(std::vector<std::vector<int>>());
        T.push_back(std::vector<std::vector<int>>());
    }

    for (int t = 0; t <= tmax; ++t) {
    }

}