#include <sstream>
#include "baseline.h"

bool BaselineIndex::add(int ts, int u, int t) {

    if (T[ts][u].size() == 0) {
        T[ts][u].push_back(t);
        L[ts][u].push_back(u);
        S[ts][u].push_back(std::unordered_set<int>());
        S[ts][u][0].insert(u);
        Ssize[ts][u].push_back(1);
        return true;
    }
    else if (T[ts][u][T[ts][u].size() - 1] < t) {
        T[ts][u].push_back(t);
        L[ts][u].push_back(L[ts][u][L[ts][u].size() - 1]);
        S[ts][u].push_back(std::unordered_set<int>());
        Ssize[ts][u].push_back(Ssize[ts][u][Ssize[ts][u].size() - 1]);
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
    if (Ssize[ts][mount_u][Ssize[ts][mount_u].size() - 1] < Ssize[ts][mount_v][Ssize[ts][mount_v].size() - 1]) {
        std::swap(u, v);
        std::swap(mount_u, mount_v);
    }
    Ssize[ts][mount_u][Ssize[ts][mount_u].size() - 1] += Ssize[ts][mount_v][Ssize[ts][mount_v].size() - 1];
    Ssize[ts][mount_v][Ssize[ts][mount_v].size() - 1] = 0;

    std::vector<std::unordered_set<int>>::iterator vector_iterator;
    std::unordered_set<int>::iterator unordered_set_iterator;

    for (vector_iterator = S[ts][mount_v].begin(); vector_iterator != S[ts][mount_v].end(); vector_iterator++) {
        for (unordered_set_iterator = vector_iterator->begin(); unordered_set_iterator != vector_iterator->end(); unordered_set_iterator++) {
            add(ts, *unordered_set_iterator, t);
            L[ts][*unordered_set_iterator][L[ts][*unordered_set_iterator].size() - 1] = mount_u;
            S[ts][mount_u][S[ts][mount_u].size() - 1].insert(*unordered_set_iterator);
        }
    }

    S[ts][mount_v][S[ts][mount_v].size() - 1].clear();
    S[ts][mount_v][S[ts][mount_v].size() - 1].insert(-1);

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

std::stringstream BaselineIndex::solve(TemporalGraph * Graph, int ts, int te) {
    
    std::stringstream Ans;
    std::unordered_set<int> Vis;
    std::set<int>::iterator it;

    Ans << "The spanned connected components in [" << ts << ", " << te << "] are:\n";
    for (it = Graph->vertex_set.begin(); it != Graph->vertex_set.end(); it++) {
        int idx = binarySearch(ts, *it, te);
        if (idx == -1 || T[ts][*it][idx] > te) {
            Ans << "{ " << *it << " }\n";
            continue;
        }
        int mount_u = find(ts, *it, idx);
        if (Vis.find(mount_u) == Vis.end()) {
            Vis.insert(mount_u);
            idx = binarySearch(ts, mount_u, te);

            std::set<int> CurrentCC;
            std::unordered_set<int>::iterator unordered_set_iterator;
            std::set<int>::iterator set_iterator;

            for (idx; idx >= 0; --idx) {
                for (unordered_set_iterator = S[ts][mount_u][idx].begin(); unordered_set_iterator != S[ts][mount_u][idx].end(); unordered_set_iterator++) {
                    if (*unordered_set_iterator != -1) {
                        CurrentCC.insert(*unordered_set_iterator);
                    }
                }
            }
            Ans << "{ ";
            if (CurrentCC.size() == 0) {
                Ans << *it << " ";
            }
            for (set_iterator = CurrentCC.begin(); set_iterator != CurrentCC.end(); set_iterator++) {
                Ans << *set_iterator << " ";
            }
            Ans << "}\n";
        }
    }

    return Ans;

}

BaselineIndex::BaselineIndex(TemporalGraph * Graph) {

    std::unordered_set<int>::iterator it;
    
    for (int ts = 0; ts <= Graph->tmax; ++ts) {
        T.push_back(std::map<int, std::vector<int>>());
        L.push_back(std::map<int, std::vector<int>>());
        S.push_back(std::map<int, std::vector<std::unordered_set<int>>>());
        Ssize.push_back(std::map<int, std::vector<int>>());
        for (int te = ts; te <= Graph->tmax; ++te) {
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[te].begin(); it != Graph->temporal_edge[te].end(); it++) {
                unioN(ts, it->first, it->second, te);
            }
        }
    }

}

void baseline(TemporalGraph * Graph, char * query_file, char * output_file) {

    int ts, te, tmax;
    std::ifstream fin(query_file);
    std::ofstream fout(output_file);

    while (fin >> ts >> te) {
        tmax = std::max(tmax, te);
    }

    BaselineIndex * idx = new BaselineIndex(Graph);

    fin = std::ifstream(query_file);

    while (fin >> ts >> te) {
        fout << idx->solve(Graph, ts, std::min(te, Graph->tmax)).str() << std::endl;
    }

    delete idx;

}