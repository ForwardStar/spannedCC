#include "baseline.h"

bool BaselineIndex::add(int ts, int u, int t) {

    if (T[ts][u].size() == 0) {
        T[ts][u].push_back(t);
        L[ts][u].push_back(u);
        S[ts][u].push_back(std::vector<int>());
        S[ts][u][0].push_back(u);
        Ssize[ts][u].push_back(1);
        return true;
    }
    else if (T[ts][u][T[ts][u].size() - 1] < t) {
        T[ts][u].push_back(t);
        L[ts][u].push_back(L[ts][u][L[ts][u].size() - 1]);
        S[ts][u].push_back(std::vector<int>());
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
        int mount_u = find(ts, u, idx);
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

BaselineIndex::BaselineIndex(TemporalGraph * Graph) {

    std::unordered_set<int>::iterator it;
    
    int start_time = time(NULL);
    for (int ts = 0; ts <= Graph->tmax; ++ts) {
        putProcess(double(ts) / Graph->tmax, difftime(time(NULL), start_time));
        T.push_back(std::vector<std::vector<int>>());
        L.push_back(std::vector<std::vector<int>>());
        Ssize.push_back(std::vector<std::vector<int>>());
        S.push_back(std::vector<std::vector<std::vector<int>>>());
        for (int u = 0; u < Graph->numOfVertices(); ++u) {
            T[ts].push_back(std::vector<int>());
            L[ts].push_back(std::vector<int>());
            Ssize[ts].push_back(std::vector<int>());
            S[ts].push_back(std::vector<std::vector<int>>());
        }
        for (int te = ts; te <= Graph->tmax; ++te) {
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[te].begin(); it != Graph->temporal_edge[te].end(); it++) {
                unioN(ts, it->first, it->second, te);
            }
        }
    }

}

void BaselineIndex::serialize(std::ofstream & os) {
    
    // Serialize Ssize
    os << "Serialized Ssize:" << std::endl;
    std::vector<std::vector<std::vector<int>>>::iterator it_Ssize;
    for (it_Ssize = Ssize.begin(); it_Ssize != Ssize.end(); it_Ssize++) {
        if (it_Ssize->size() == 0) {
            continue;
        }
        os << std::endl;
        std::vector<std::vector<int>>::iterator it1;
        int u = 0;
        for (it1 = it_Ssize->begin(); it1 != it_Ssize->end(); it1++) {
            os << u++ << " ";
            std::vector<int>::iterator it2;
            for (it2 = it1->begin(); it2 != it1->end(); it2++) {
                os << *it2 << " ";
            }
            os << std::endl;
        }
    }

    // Serialize S
    os << "Serialized S:" << std::endl;
    std::vector<std::vector<std::vector<std::vector<int>>>>::iterator it_S;
    for (it_S = S.begin(); it_S != S.end(); it_S++) {
        if (it_S->size() == 0) {
            continue;
        }
        std::vector<std::vector<std::vector<int>>>::iterator it1;
        int u = 0;
        for (it1 = it_S->begin(); it1 != it_S->end(); it1++) {
            os << u++ << std::endl;
            std::vector<std::vector<int>>::iterator it2;
            for (it2 = it1->begin(); it2 != it1->end(); it2++) {
                if (it2->size() == 0) {
                    continue;
                }
                std::vector<int>::iterator it3;
                for (it3 = it2->begin(); it3 != it2->end(); it3++) {
                    os << *it3 << " ";
                }
                os << std::endl;
            }
            os << std::endl;
        }
    }

    // Serialize L
    os << "Serialized L:" << std::endl;
    std::vector<std::vector<std::vector<int>>>::iterator it_L;
    for (it_L = L.begin(); it_L != L.end(); it_L++) {
        if (it_L->size() == 0) {
            continue;
        }
        os << std::endl;
        std::vector<std::vector<int>>::iterator it1;
        int u = 0;
        for (it1 = it_L->begin(); it1 != it_L->end(); it1++) {
            os << u++ << " ";
            std::vector<int>::iterator it2;
            for (it2 = it1->begin(); it2 != it1->end(); it2++) {
                os << *it2 << " ";
            }
            os << std::endl;
        }
    }

    // Serialize T
    os << "Serialized T:" << std::endl;
    std::vector<std::vector<std::vector<int>>>::iterator it_T;
    for (it_T = T.begin(); it_T != T.end(); it_T++) {
        if (it_T->size() == 0) {
            continue;
        }
        os << std::endl;
        std::vector<std::vector<int>>::iterator it1;
        int u = 0;
        for (it1 = it_T->begin(); it1 != it_T->end(); it1++) {
            os << u++ << " ";
            std::vector<int>::iterator it2;
            for (it2 = it1->begin(); it2 != it1->end(); it2++) {
                os << *it2 << " ";
            }
            os << std::endl;
        }
    }

}

void BaselineIndex::deserialize(std::ifstream & is) {

    std::string line;
    
    // Deserialize Ssize
    std::getline(is, line);
    while (std::getline(is, line)) {
        if (line[0] == 'S' && line[line.size() - 2] == 'S') {
            break;
        }
        if (line.empty()) {
            Ssize.push_back(std::vector<std::vector<int>>());
            continue;
        }
        std::stringstream ssline(line);
        int vertex;
        ssline >> vertex;
        while (vertex >= Ssize[Ssize.size() - 1].size()) {
            Ssize[Ssize.size() - 1].push_back(std::vector<int>());
        }
        Ssize[Ssize.size() - 1][vertex] = std::vector<int>();
        int size_element;
        while (ssline >> size_element) {
            Ssize[Ssize.size() - 1][vertex].push_back(size_element);
        }
    }

    // Deserialize S
    while (std::getline(is, line)) {
        if (line[0] == 'S') {
            break;
        }
        std::stringstream ssline(line);
        int vertex;
        ssline >> vertex;
        if (S.size() == 0 || vertex < S[S.size() - 1].size()) {
            S.push_back(std::vector<std::vector<std::vector<int>>>());
        }
        while (vertex >= S[S.size() - 1].size()) {
            S[S.size() - 1].push_back(std::vector<std::vector<int>>());
        }
        S[S.size() - 1][vertex] = std::vector<std::vector<int>>();
        while (std::getline(is, line)) {
            if (line.empty()) {
                break;
            }
            ssline = std::stringstream(line);
            S[S.size() - 1][vertex].push_back(std::vector<int>());
            int cc_element;
            while (ssline >> cc_element) {
                S[S.size() - 1][vertex][S[S.size() - 1][vertex].size() - 1].push_back(cc_element);
            }
        }
    }

    // Deserialize L
    while (std::getline(is, line)) {
        if (line[0] == 'S') {
            break;
        }
        if (line.empty()) {
            L.push_back(std::vector<std::vector<int>>());
            continue;
        }
        std::stringstream ssline(line);
        int vertex;
        ssline >> vertex;
        while (vertex >= L[L.size() - 1].size()) {
            L[L.size() - 1].push_back(std::vector<int>());
        }
        L[L.size() - 1][vertex] = std::vector<int>();
        int label_element;
        while (ssline >> label_element) {
            L[L.size() - 1][vertex].push_back(label_element);
        }
    }

    // Deserialize T
    while (std::getline(is, line)) {
        if (line.empty()) {
            T.push_back(std::vector<std::vector<int>>());
            continue;
        }
        std::stringstream ssline(line);
        int vertex;
        ssline >> vertex;
        while (vertex >= T[T.size() - 1].size()) {
            T[T.size() - 1].push_back(std::vector<int>());
        }
        T[T.size() - 1][vertex] = std::vector<int>();
        int time_element;
        while (ssline >> time_element) {
            T[T.size() - 1][vertex].push_back(time_element);
        }
    }

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
        putProcess(double(++i) / query_num, difftime(time(NULL), start_time));
        fout << Index->solve(vertex_num, ts, te).str() << std::endl;
    }

    std::cout << "Average (per query): " << timeFormatting(difftime(time(NULL), start_time) / query_num).str() << std::endl;

}