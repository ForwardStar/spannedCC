#include "commonfunctions.h"
#include "temporal_graph.h"
class LCTindex{
    public:

    struct Link_Cut_Tree
    {
        #define lson t[x].ch[0]
        #define rson t[x].ch[1]
        int* sta;
        struct Node
        {
            int ch[2],f,rev,min,id,val;
        };
        Node* t; 
        void init(int size){
            t=new Node[size];
            sta=new int[size];
        }
        int get(int x)
        {
            return t[t[x].f].ch[1]==x;   
        }
        int isrt(int x)
        {
            return !(t[t[x].f].ch[0]==x||t[t[x].f].ch[1]==x); 
        }
        void pushup(int x)
        {
            t[x].id=x;
            t[x].min=t[x].val;   
            if(lson&&t[lson].min<t[x].min)
            {
                t[x].min=t[lson].min;
                t[x].id=t[lson].id;
            }
            if(rson&&t[rson].min<t[x].min)
            {
                t[x].min=t[rson].min;
                t[x].id=t[rson].id;  
            }
        }
        void mark(int x)
        {
            if(x) t[x].rev^=1,std::swap(lson,rson);
        }
        void pushdown(int x)
        {
            if(x&&t[x].rev)
            {
                t[x].rev=0;
                if(lson) mark(lson);
                if(rson) mark(rson);   
            }
        }
        void rotate(int x)
        {
            int old=t[x].f,fold=t[old].f,which=get(x);
            if(!isrt(old)) t[fold].ch[t[fold].ch[1]==old]=x;    
            t[old].ch[which]=t[x].ch[which^1],t[t[old].ch[which]].f=old;
            t[x].ch[which^1]=old,t[old].f=x,t[x].f=fold;
            pushup(old),pushup(x);
        }
        void splay(int x)
        {
            int u=x,v=0,fa;
            for(sta[++v]=u;!isrt(u);u=t[u].f) sta[++v]=t[u].f;     
            for(;v;--v) pushdown(sta[v]);  
            for(u=t[u].f;(fa=t[x].f)!=u;rotate(x))
            {
                if(t[fa].f!=u)
                    rotate(get(fa)==get(x)?fa:x);   
            }
        }
        void Access(int x)
        {
            for(int y=0;x;y=x,x=t[x].f)
                splay(x),rson=y,pushup(x);   
        }
        void makeroot(int x)
        {
            Access(x),splay(x),mark(x);
        }
        void split(int x,int y)
        {
            makeroot(x),Access(y),splay(y);
        }
        int findroot(int x)
        {
            Access(x),splay(x);  
            for(;lson;) x=lson;
            return x;   
        }
        void link(int x,int y)
        {
            makeroot(x),t[x].f=y;
        }
        void cut(int x,int y)
        {  
            makeroot(x),Access(y),splay(y);
            t[y].ch[0]=t[x].f=0;
            pushup(y);   
        }
        #undef lson
        #undef rson  
    }lct;


        struct edge{
            int u,v,ts;
            edge(int u1=0,int v1=0, int t1=0){
                u=u1;v=v1;ts=t1;
            }
        };
        edge* E;
        // n, m, tmax: graph information.
        int n, m, tmax, t1, cnt;
        
        // each relation[t][i] represents an edge appearing at t
        std::vector<edge> *relation;
        //std::vector<int> *actual_time;
        static bool cmp(edge a,edge b);
        int find(int* parent, int u);
        bool unioN(int* parent, int u,int v);
        std::stringstream solve(int n, int ts, int te);
        void construct_disjoint_set(int * parent, int ts, int te);
        void update(TemporalGraph * Graph);
        int size();

        LCTindex() {}
        LCTindex(TemporalGraph * Graph, double t_fraction);
        ~LCTindex() {};
};

void LCT (LCTindex* Index, int vertex_num, char * query_file, char * output_file);