#include "LCT.h"

int LCTindex::find(int * parent, int u) {

    if (parent[u] != u) {
        parent[u] = find(parent, parent[u]);
    }

    return parent[u];

}

bool LCTindex::unioN(int * parent, int u, int v) {

    int mount_u = find(parent, u);
    int mount_v = find(parent, v);

    if (mount_u != mount_v) {
        parent[mount_u] = mount_v;
        return true;
    }

    return false;

}

bool LCTindex::cmp(edge a, edge b){
    return a.ts<b.ts;
}
LCTindex::LCTindex(TemporalGraph * Graph, double t_fraction){
    unsigned long long start_time = currentTime();
    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    t1 = int(tmax * t_fraction);
    cnt = 0;
    E = new edge[m+1];
    relation = new std::vector<edge> [tmax+1];
    lct.init(n+m+1);
    int* parent = new int[n];
    for(int i=1;i<=n;i++){
        lct.t[i].val=1e9;
        lct.pushup(i);
    }
    //std::cerr<<"start "<<t1<<'\n';
    for(int ts=0;ts<=t1;ts++){
        std::vector<std::pair<int, int>>::iterator it;
        //std::cerr<<ts<<' '<<cnt<<'\n';
        for (it = Graph->temporal_edge[ts].begin(); it != Graph->temporal_edge[ts].end(); it++){
            int u = it->first;
            int v = it->second;
            if(u==v)continue;
            ++cnt;
            int now = cnt+n;
            E[cnt] = edge(u,v,ts);
            //std::cerr<<u<<' '<<v<<' '<<ts<<'\n';
            
            
            int r1=lct.findroot(u+1),r2=lct.findroot(v+1);
            // if(u==569&&v==0&&ts==143){
            //     std::cerr<<"?\n";
            //     int r1 = lct.findroot(u);
            //     int r2 = lct.findroot(v);
            //     std::cerr<<r1<<' '<<r2<<"?\n";
            // }
            if(r1==r2){
                
                lct.split(u+1,v+1);
                int pre = lct.t[v+1].min;
                
                if(pre<ts){
                    relation[ts].push_back(edge(u,v,pre+1));
                    int cur = lct.t[v+1].id;
                    lct.cut(cur, E[cur-n].u+1);
                    lct.cut(cur, E[cur-n].v+1);
                    lct.t[now].val=ts;
                    lct.pushup(now);
                    lct.link(now,u+1);
                    lct.link(now,v+1);
                }
            }
            else{
                // if(u==569&&v==0&&ts==143){
                //     std::cerr<<"??\n";
                // }
                relation[ts].push_back(edge(u,v,0));
                lct.t[now].val=ts;
                lct.pushup(now);
                lct.link(now,u+1);
                lct.link(now,v+1);
            }
        }
        std::sort(relation[ts].begin(),relation[ts].end(),cmp);

        //if(ts%10000000==0)
        //putProcess(double(ts) / t1, currentTime() - start_time);
    }
}

int LCTindex::size(){
    int sz=0;
    for(int i=0;i<=tmax;i++){
        sz+=relation[i].size();
    }
    return sz*12ll;
}

void LCTindex::update(TemporalGraph * Graph){
    for(int ts=t1+1;ts<=tmax;ts++){
        std::vector<std::pair<int, int>>::iterator it;
        //std::cerr<<ts<<' '<<cnt<<'\n';
        for (it = Graph->temporal_edge[ts].begin(); it != Graph->temporal_edge[ts].end(); it++){
            int u = it->first;
            int v = it->second;
            ++cnt;
            int now = cnt+n;
            E[cnt] = edge(u,v,ts);
            //std::cerr<<u<<' '<<v<<' '<<ts<<'\n';
            
            
            int r1=lct.findroot(u+1),r2=lct.findroot(v+1);
            // if(u==569&&v==0&&ts==143){
            //     std::cerr<<"?\n";
            //     int r1 = lct.findroot(u);
            //     int r2 = lct.findroot(v);
            //     std::cerr<<r1<<' '<<r2<<"?\n";
            // }
            if(r1==r2){
                
                lct.split(u+1,v+1);
                int pre = lct.t[v+1].min;
                
                if(pre<ts){
                    relation[ts].push_back(edge(u,v,pre+1));
                    int cur = lct.t[v+1].id;
                    lct.cut(cur, E[cur-n].u+1);
                    lct.cut(cur, E[cur-n].v+1);
                    lct.t[now].val=ts;
                    lct.pushup(now);
                    lct.link(now,u+1);
                    lct.link(now,v+1);
                }
            }
            else{
                // if(u==569&&v==0&&ts==143){
                //     std::cerr<<"??\n";
                // }
                relation[ts].push_back(edge(u,v,0));
                lct.t[now].val=ts;
                lct.pushup(now);
                lct.link(now,u+1);
                lct.link(now,v+1);
            }
        }
        std::sort(relation[ts].begin(),relation[ts].end(),cmp);
    }
}

std::stringstream LCTindex::solve(int n,int ts,int te){
    std::stringstream Ans;
    int *parent = new int[n];
    std::vector<int> *CurrentCC = new std::vector<int>[n]();

    Ans << "The spanned connected components in [" << ts << ", " << te << "] are:\n";
    for (int u = 0; u < n; ++u) {
        parent[u] = u;
    }
    for(int t=ts;t<=te;t++){
        for(auto e:relation[t]){
            if(e.ts>ts)break;
            if(!unioN(parent, e.u,e.v)){
                std::cout << "an error may occur." << std::endl;
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


void LCT(LCTindex* Index, int vertex_num, char * query_file, char * output_file){
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