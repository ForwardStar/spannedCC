#include "LCT_prt.h"
#include <cassert>
int LCTprt::find(int * parent, int u) {

    if (parent[u] != u) {
        parent[u] = find(parent, parent[u]);
    }

    return parent[u];

}

bool LCTprt::unioN(int * parent, int u, int v) {

    int mount_u = find(parent, u);
    int mount_v = find(parent, v);

    if (mount_u != mount_v) {
        parent[mount_u] = mount_v;
        return true;
    }

    return false;

}

bool LCTprt::cmp(node a, node b){
    if(a.ts!=b.ts)
    return a.ts<b.ts;
    return a.e.t<b.e.t;
}

bool LCTprt::cmp2(node a, node b){
    return a.e.t<b.e.t;
}

void LCTprt::build(typename std::vector<node>::iterator begin,
                   typename std::vector<node>::iterator end,
                   int fa,
                   bool is_left_child) {
    if (begin == end) return;

    // 处理当前节点
    node current_node = *begin;
    int current_number = ++number;
    tree[current_number].c = current_node;

    // 连接到父节点
    if (fa != 0) {
        if (is_left_child) {
            tree[fa].ls = current_number;
        } else {
            tree[fa].rs = current_number;
        }
    }

    auto remaining_begin = begin + 1;
    auto remaining_end = end;
    size_t remaining_size = remaining_end - remaining_begin;
    if (remaining_size == 0) return;

    // 找到中间位置并分割
    auto mid_iter = remaining_begin + remaining_size / 2;
    std::nth_element(remaining_begin, mid_iter, remaining_end, cmp2);
    tree[current_number].div = mid_iter->e.t;

    // 递归构建子树
    std::sort(remaining_begin,mid_iter+1,cmp);
    std::sort(mid_iter+1,remaining_end,cmp);
    build(remaining_begin, mid_iter+1, current_number, true);   // 左子树
    build(mid_iter+1, remaining_end, current_number, false);    // 右子树
}

// void LCTprt::build(std::vector<node> relation,int fa){
//     if(relation.empty())return ;
//     //std::cout<<relation.size()<<' '<<fa<<'\n';
//     if(fa==0){
//         tree[++number].c=relation[0];
//         assert(tree[number].c.ts>=tree[fa].c.ts);
//         if(relation.size()==1)return ;
//         std::vector<node>::iterator iter=relation.begin()+1;
//         std::vector<node> tmp(iter,relation.end());
//         size_t mid=tmp.size()/2;
//         std::nth_element(tmp.begin(),tmp.begin()+mid,tmp.end(),cmp2);
//         std::vector<node> left,right;
//         for(int i=1;i<relation.size();i++){
//             if(relation[i].e.t<=tmp[mid].e.t){
//                 left.push_back(relation[i]);
//             }
//             else{
//                 right.push_back(relation[i]);
//             }
//         }
//         int now=number;
//         tree[now].div=tmp[mid].e.t;
//         tmp.clear();
//         tmp.shrink_to_fit();
//         relation.clear();
//         relation.shrink_to_fit();
//         build(left,now);
//         build(right,now);
//         left.clear();
//         left.shrink_to_fit();
//         right.clear();
//         right.shrink_to_fit();
//     }
//     else{
//         tree[++number].c=relation[0];
//         assert(tree[number].c.ts>=tree[fa].c.ts);
//         if(tree[number].c.e.t<=tree[fa].div){
//             tree[fa].ls=number;
//         }
//         else{
//             tree[fa].rs=number;
//         }
//         if(relation.size()==1)return ;
        
//         std::vector<node>::iterator iter=relation.begin()+1;
//         std::vector<node> tmp(iter,relation.end());
//         size_t mid=tmp.size()/2;
//         std::nth_element(tmp.begin(),tmp.begin()+mid,tmp.end(),cmp2);
//         std::vector<node> left,right;
//         for(int i=1;i<relation.size();i++){
//             if(relation[i].e.t<=tmp[mid].e.t){
//                 left.push_back(relation[i]);
//             }
//             else{
//                 right.push_back(relation[i]);
//             }
//         }
//         int now=number;
//         tree[now].div=tmp[mid].e.t;
//         tmp.clear();
//         tmp.shrink_to_fit();
//         relation.clear();
//         relation.shrink_to_fit();
//         build(left,now);
//         build(right,now);
//         left.clear();
//         left.shrink_to_fit();
//         right.clear();
//         right.shrink_to_fit();
//     }
    
// }

void LCTprt::query(int* parent,int now,int ts,int te){
    if(now==0)return ;
    
    if(tree[now].c.ts<=ts ){
        if(tree[now].c.e.t>=ts && tree[now].c.e.t<=te){
            //std::cout<<tree[now].c.e.u<<' '<<tree[now].c.e.v<<' '<<tree[now].c.e.t<<'\n';
            assert(unioN(parent,tree[now].c.e.u,tree[now].c.e.v));
            if(tree[now].div>=ts){
                query(parent,tree[now].ls,ts,te);
            }
            if(tree[now].div<=te){
                query(parent,tree[now].rs,ts,te);
            }
        }
        else{
            if(tree[now].div>=ts){
                query(parent,tree[now].ls,ts,te);
            }
            if(tree[now].div<=te){
                query(parent,tree[now].rs,ts,te);
            }
        }
    }
    else return ;
}

LCTprt::LCTprt(TemporalGraph * Graph, double t_fraction){
    unsigned long long start_time = currentTime();
    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    t1 = int(tmax * t_fraction);
    cnt = 0;
    len=std::sqrt(tmax);
    E = new edge[m+1];
    tree = new PRT[m+1];
    //chunk = new std::vector<edge> [tmax+1];
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
                    relation.push_back(node(u,v,pre+1,ts));
                    //chunk[ts/len].push_back(edge(u,v,pre+1));
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
                relation.push_back(node(u,v,0,ts));

                //chunk[ts/len].push_back(edge(u,v,0));
                lct.t[now].val=ts;
                lct.pushup(now);
                lct.link(now,u+1);
                lct.link(now,v+1);
            }
        }

        //if(ts%10000000==0)
        //putProcess(double(ts) / t1, currentTime() - start_time);
    }
    sort(relation.begin(),relation.end(),cmp);
    
    build(relation.begin(),relation.end(),0,false);
    //std::cout<<number<<' '<<m<<' '<<relation.size()<<'\n';
    relation.clear();
    relation.shrink_to_fit();
}

long long LCTprt::size(){
    // int sz=0;
    // for(int i=0;i<=tmax;i++){
    //     sz+=relation[i].size();
    // }
    // for(int i=0;i<=tmax/len;i++){
    //     sz+=chunk[i].size();
    // }

    return number*28ll;
}

void LCTprt::update(TemporalGraph * Graph){
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
                    //relation[ts].push_back(edge(u,v,pre+1));
                    //chunk[ts/len].push_back(edge(u,v,pre+1));
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
                //relation[ts].push_back(edge(u,v,0));
                //chunk[ts/len].push_back(edge(u,v,0));
                lct.t[now].val=ts;
                lct.pushup(now);
                lct.link(now,u+1);
                lct.link(now,v+1);
            }
        }
        //std::sort(relation[ts].begin(),relation[ts].end(),cmp);

    }
    //for(int i=(t1+1)/len;i<=tmax/len;i++)sort(chunk[i].begin(),chunk[i].end(),cmp);
}



std::stringstream LCTprt::solve(int n,int ts,int te){
    std::stringstream Ans;
    int *parent = new int[n];
    std::vector<int> *CurrentCC = new std::vector<int>[n]();

    Ans << "The spanned connected components in [" << ts << ", " << te << "] are:\n";
    for (int u = 0; u < n; ++u) {
        parent[u] = u;
    }

    query(parent,1,ts,te);

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


void LCTp(LCTprt* Index, int vertex_num, char * query_file, char * output_file){
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