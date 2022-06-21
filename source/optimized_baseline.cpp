#include"optimized_baseline.h"



int OptimizedBaseline::find(int u){
    if(L[u]==u)return u;

    L[u]=find(L[u]);

    return L[u];
}

void OptimizedBaseline::optimized_unioN(int ts, int u, int v, int t){
    int x=find(u), y=find(v);
    if(x==y)return ;
    if(x>y)std::swap(x,y);
    dS[ts][x].push_back(std::pair<int,int>(t,y));
    L[y]=x;
}

std::stringstream OptimizedBaseline::solve(int n, int ts, int te){
    std::stringstream Ans;
    std::vector<int> Vis(n,0);
    std::vector<int> *S;
    S = new std::vector<int> [n]();
    for(int u=0;u<n;u++){
        if(dS[ts][u].empty())continue;
        std::vector<std::pair<int,int> >:: iterator iter;
        for(iter=dS[ts][u].begin();iter!=dS[ts][u].end();iter++){
            std::pair<int,int> tmp=*iter;
            if(tmp.first > te)break;
            S[u].push_back(tmp.second);
            Vis[tmp.second]++;
        }
    }
    Ans << "The spanned connected components in [" << ts << ", " << te << "] are:\n";
    std::vector<int> CurrentCC;
    for(int u=0;u<n;u++){
        if(Vis[u]==0){// means u is a root of a tree
            CurrentCC.clear();
            CurrentCC.push_back(u);
            std::queue<int> q;
            while(!q.empty())q.pop();
            q.push(u);
            while(!q.empty()){
                int U=q.front();q.pop();
                for(auto v:S[U]){
                    CurrentCC.push_back(v);
                    q.push(v);
                }
            }
            std::sort(CurrentCC.begin(),CurrentCC.end());
            Ans << "{ ";
            std::vector<int>::iterator it;
            for (it = CurrentCC.begin(); it != CurrentCC.end(); it++) {
                Ans << *it << " ";
            }
            Ans << "}\n";
        }
    }
    delete [] S;
    std::vector<int>().swap(Vis);
    std::vector<int>().swap(CurrentCC);
    return Ans;
}


OptimizedBaseline::OptimizedBaseline(TemporalGraph * Graph){

    int start_time=time(NULL);
    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    dS = new std::vector<std::vector<std::pair<int,int>>> [tmax+1]();
    L.resize(n);
    for(int ts=0;ts<=tmax;ts++){
        dS[ts].resize(n);
        // reset L
        for(int i=0;i<n;i++){
            L[i]=i;
        }
        for(int te=ts;te<=tmax;te++){
            if(Graph->temporal_edge[te].empty())continue;
            std::vector<std::pair<int, int>>::iterator it;
            for(it=Graph->temporal_edge[te].begin();it!=Graph->temporal_edge[te].end();it++){
                optimized_unioN(ts,it->first,it->second, te);
            }
        }
    }
}

OptimizedBaseline::~OptimizedBaseline(){
    std::vector<int>().swap(L);
    delete [] dS;
}

void OBaseline(OptimizedBaseline * Index, int vertex_num, char * query_file, char * output_file){
    int ts,te;
    int query_num = 0;
    std::ifstream fin(query_file);
    std::ofstream fout(output_file);

    while(fin>>ts>>te){
        ++query_num;
    }

    fin = std::ifstream(query_file);

    int i=0;
    int start_time = time(NULL);
    while(fin>>ts>>te){
        fout<< Index->solve(vertex_num, ts, te).str() << std::endl;
        putProcess(double(++i) / query_num, difftime(time(NULL), start_time));
    }

    std::cout << "Average (per query): " << timeFormatting(difftime(time(NULL), start_time) / query_num).str() << std::endl;
}