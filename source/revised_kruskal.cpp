#include "revised_kruskal.h"

int RevisedKruskal::find(int u, int &t, int &min_keep) {
    
    if (L[u][L[u].size() - 1] != u && T[u][T[u].size() - 1] <= t) {
        min_keep = std::min(min_keep, keep[u]);
        return find(L[u][L[u].size() - 1], t, min_keep);
    }

    return u;

}

void RevisedKruskal::unioN(int ts, int u, int v, int t, int min_keep) {

    int mount_u = find(u, t, min_keep);
    int mount_v = find(v, t, min_keep);

    // Already in the same connected component.
    if (mount_u == mount_v) {
        return;
    }

    // Merge the smaller connected component into the larger one.
    if (size[mount_u] < size[mount_v]) {
        std::swap(u, v);
        std::swap(mount_u, mount_v);
    }

    if (L[mount_v][L[mount_v].size() - 1] == mount_v) {
        if (Ts[mount_v][Ts[mount_v].size() - 1] != ts) {
            ++num;
            L[mount_v].push_back(mount_v);
            T[mount_v].push_back(ts);
            Ts[mount_v].push_back(ts);
        }
        L[mount_v][L[mount_v].size() - 1] = mount_u;
        T[mount_v][T[mount_v].size() - 1] = t;
        keep[mount_v] = min_keep;
        int now = mount_u;
        while (L[now][L[now].size() - 1] != now) {
            size[now] += size[mount_v];
            now = L[now][L[now].size() - 1];
        }
        size[now] += size[mount_v];
    }
    else {
        int tmpv = L[mount_v][L[mount_v].size() - 1];
        int tmpt = T[mount_v][T[mount_v].size() - 1];
        while (L[tmpv][L[tmpv].size() - 1] != tmpv) {
            size[tmpv] -= size[mount_v];
            tmpv = L[tmpv][L[tmpv].size() - 1];
        }
        size[tmpv] -= size[mount_v];
        tmpv = L[mount_v][L[mount_v].size() - 1];
        if (Ts[mount_v][Ts[mount_v].size() - 1] != ts) {
            ++num;
            L[mount_v].push_back(mount_v);
            T[mount_v].push_back(ts);
            Ts[mount_v].push_back(ts);
        }
        L[mount_v][L[mount_v].size() - 1] = mount_u;
        T[mount_v][T[mount_v].size() - 1] = t;
        int tmp = keep[mount_v];
        keep[mount_v] = min_keep;
        min_keep = std::min(tmp, min_keep);
        int now = mount_u;
        while (L[now][L[now].size() - 1] != now) {
            size[now] += size[mount_v];
            now = L[now][L[now].size() - 1];
        }
        size[now] += size[mount_v];
        unioN(ts, mount_v, tmpv, tmpt, min_keep);
    }

}

int RevisedKruskal::binarySearch(int u, int ts) {

    int l = 0;
    int r = std::min(ts, int(Ts[u].size()) - 1);

    while (l < r) {
        int mid = l + r + 1 >> 1;
        if (Ts[u][mid] > ts) {
            r = mid - 1;
        }
        else {
            l = mid;
        }
    }

    return l;

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
            int idx = binarySearch(current, ts);
            while (L[current][idx] != current && T[current][idx] <= te) {
                current = L[current][idx];
                idx = binarySearch(current, ts);
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

    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    L = new std::vector<int>[n];
    T = new std::vector<int>[n];
    Ts = new std::vector<int>[n];
    keep = new int[n];
    size = new int[n];
    
    for (int u = 0; u < n; ++u) {
        keep[u] = -1;
        size[u] = 1;
    }
    
    for (int ts = 0; ts <= tmax; ++ts) {
        num = 0;
        for (int u = 0; u < n; ++u) {
            if (keep[u] == ts - 1) {
                ++num;
                L[u].push_back(u);
                T[u].push_back(ts);
                Ts[u].push_back(ts);
            }
            size[u] = 1;
        }
        for (int u = 0; u < n; ++u) {
            int current = u;
            while (L[current][L[current].size() - 1] != current) {
                size[L[current][L[current].size() - 1]]++;
                current = L[current][L[current].size() - 1];
            }
        }
        for (int te = ts; te <= tmax; ++te) {
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[te].begin(); it != Graph->temporal_edge[te].end(); it++) {
                unioN(ts, it->first, it->second, te, te);
            }
        }
        total_num += num;
        putProcess(double(ts) / tmax, difftime(time(NULL), start_time));
    }

    for (int u = 0; u < n; ++u) {
        L[u].shrink_to_fit();
        T[u].shrink_to_fit();
        Ts[u].shrink_to_fit();
    }

    std::cout << "Average slots of ts: " << total_num / n << std::endl;

    delete [] keep;
    delete [] size;

}

RevisedKruskal::~RevisedKruskal() {

    delete [] L;
    delete [] T;
    delete [] Ts;

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