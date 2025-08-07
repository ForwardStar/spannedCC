#include "differential_kruskal.h"

int find(int * parent, int u) {

    if (parent[u] != u) {
        parent[u] = find(parent, parent[u]);
    }

    return parent[u];

}

bool unioN(int * parent, int u, int v) {

    int mount_u = find(parent, u);
    int mount_v = find(parent, v);

    if (mount_u != mount_v) {
        parent[mount_u] = mount_v;
        return true;
    }

    return false;

}

void construct_disjoint_set(DifferentialKruskal * Index, int * parent, int ts) {

    for (int t = 0; t <= ts; ++t) {
        for (int i = Index->actual_time[t].size() - 1; i >= 0; --i) {
            if (Index->actual_time[t][i] < ts) {
                break;
            }
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Index->relation[t][i].begin(); it != Index->relation[t][i].end(); it++) {
                if (!unioN(parent, it->first, it->second)) {
                    std::cout << "an error may occur." << std::endl;
                }
            }
        }
    }

}

int DifferentialKruskal::find_an_index(int t, int ts, int te) {

    int l = 0;
    int r = actual_time[t].size() - 1;

    if (r == -1 || actual_time[t][r] < ts || actual_time[t][0] > te) {
        return -1;
    }
    
    while (l < r) {
        int mid = l + r >> 1;
        if (actual_time[t][mid] >= ts && actual_time[t][mid] <= te) {
            return mid;
        }
        else {
            if (actual_time[t][mid] < ts) {
                l = mid + 1;
            }
            else {
                r = mid;
            }
        }
    }

    if (actual_time[t][l] >= ts && actual_time[t][l] <= te) {
        return l;
    }
    else {
        return -1;
    }

}

std::stringstream DifferentialKruskal::solve(int n, int ts, int te) {

    std::stringstream Ans;
    int *parent = new int[n];
    std::vector<int> *CurrentCC = new std::vector<int>[n]();

    Ans << "The spanned connected components in [" << ts << ", " << te << "] are:\n";
    for (int u = 0; u < n; ++u) {
        parent[u] = u;
    }

    for (int t = 0; t <= ts; ++t) {
        int idx = find_an_index(t, ts, te);
        if (idx == -1) {
            continue;
        }
        for (int i = idx; i >= 0; --i) {
            if (actual_time[t][i] < ts) {
                break;
            }
            std::vector<std::pair<int, int>>::iterator it;
            for (it = relation[t][i].begin(); it != relation[t][i].end(); ++it) {
                if (!unioN(parent, it->first, it->second)) {
                    std::cout << "an error may occur." << std::endl;
                }
            }
        }
        for (int i = idx + 1; i < actual_time[t].size(); ++i) {
            if (actual_time[t][i] > te) {
                break;
            }
            std::vector<std::pair<int, int>>::iterator it;
            for (it = relation[t][i].begin(); it != relation[t][i].end(); ++it) {
                if (!unioN(parent, it->first, it->second)) {
                    std::cout << "an error may occur." << std::endl;
                }
            }
        }
    }

    for (int u = 0; u < n; ++u) {
        int mount_u = find(parent, u);
        if (u < mount_u) {
            parent[u] = u;
            parent[mount_u] = u;
        }
    }

    for (int u = 0; u < n; ++u) {
        CurrentCC[find(parent, u)].push_back(u);
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

    delete [] parent;
    delete [] CurrentCC;

    return Ans;

}

void DifferentialKruskal::update(TemporalGraph * Graph) {
    int *parent = new int[n];

    for (int ts = 0; ts <= tmax; ++ts) {
        for (int u = 0; u < n; ++u) {
            parent[u] = u;
        }
        construct_disjoint_set(this, parent, ts);
        for (int te = std::max(ts, t1 + 1); te <= tmax; ++te) {
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[te].begin(); it != Graph->temporal_edge[te].end(); ++it) {
                if (unioN(parent, it->first, it->second)) {
                    if (actual_time[ts].size() == 0 || actual_time[ts][actual_time[ts].size() - 1] != te) {
                        actual_time[ts].push_back(te);
                        relation[ts].push_back(std::vector<std::pair<int, int>>());
                    }
                    relation[ts][relation[ts].size() - 1].push_back(*it);
                }
            }
        }
    }

    delete [] parent;
}

DifferentialKruskal::DifferentialKruskal(TemporalGraph * Graph, double t_fraction) {

    unsigned long long start_time = currentTime();

    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    t1 = int(tmax * t_fraction);

    relation = new std::vector<std::vector<std::pair<int, int>>>[tmax + 1];
    actual_time = new std::vector<int>[tmax + 1];
    int *parent = new int[n];
    
    for (int ts = 0; ts <= t1; ++ts) {
        for (int u = 0; u < n; ++u) {
            parent[u] = u;
        }
        construct_disjoint_set(this, parent, ts);
        for (int te = ts; te <= t1; ++te) {
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[te].begin(); it != Graph->temporal_edge[te].end(); it++) {
                if (unioN(parent, it->first, it->second)) {
                    if (actual_time[ts].size() == 0 || actual_time[ts][actual_time[ts].size() - 1] != te) {
                        actual_time[ts].push_back(te);
                        relation[ts].push_back(std::vector<std::pair<int, int>>());
                    }
                    relation[ts][relation[ts].size() - 1].push_back(*it);
                }
            }
        }
        putProcess(double(ts) / t1, currentTime() - start_time);
    }

    delete [] parent;

}

unsigned long long DifferentialKruskal::size() {
    
    unsigned long long memory = 0;
    for (int t = 0; t <= tmax; ++t) {
        std::vector<std::vector<std::pair<int, int>>>::iterator it;
        for (it = relation[t].begin(); it != relation[t].end(); it++) {
            memory += it->size();
        }
    }
    memory *= 12;
    return memory;

}

void differential_kruskal(DifferentialKruskal * Index, int vertex_num, char * query_file, char * output_file) {

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

    std::cout << "Average (per query): " << timeFormatting(double(currentTime() - start_time) / query_num).str() << std::endl;
}