#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define fr(i,j,k) for(int i=j;i<k;i++)
#define f(n) fr(i,0,n)
#define f1(n) fr(i,1,n+1)
#define pb push_back
#define F first
#define S second
#define all(x) x.begin(), x.end()
const ll mod = 1e9 + 7;
const int maxn = 3e5+5;
const long long INF = 1LL<<60;
struct Dinic {  //O(VVE), with minimum cut 
    static const int MAXN = 10003;
    struct Edge{
        int u, v;
        long long cap, rest;
    };

    int n, m, s, t, d[MAXN], cur[MAXN];
    vector<Edge> edges;
    vector<int> G[MAXN];

    void init(){
        edges.clear();
        for ( int i = 0 ; i < n ; i++ ) G[i].clear();
        n = 0;
    }

    // min cut start
    bool side[MAXN];
    void cut(int u) {
        side[u] = 1;
        for ( int i : G[u] ) {
            if ( !side[ edges[i].v ] && edges[i].rest ) cut(edges[i].v);
        } 
    }
    // min cut end

    int add_node(){
        return n++;
    }

    void add_edge(int u, int v, long long cap){
        edges.push_back( {u, v, cap, cap} );
        edges.push_back( {v, u, 0, 0LL} );
        m = edges.size();
        G[u].push_back(m-2);
        G[v].push_back(m-1);
    }
    
    bool bfs(){
        fill(d,d+n,-1);
        queue<int> que;
        que.push(s); d[s]=0;
        while (!que.empty()){
            int u = que.front(); que.pop();
            for (int ei : G[u]){
                Edge &e = edges[ei];
                if (d[e.v] < 0 && e.rest > 0){
                    d[e.v] = d[u] + 1;
                    que.push(e.v);
                }
            }
        }
        return d[t] >= 0;
    }

    long long dfs(int u, long long a){
        if ( u == t || a == 0 ) return a;
        long long flow = 0, f;
        for ( int &i=cur[u]; i < (int)G[u].size() ; i++ ) {
            Edge &e = edges[ G[u][i] ];
            if ( d[u] + 1 != d[e.v] ) continue;
            f = dfs(e.v, min(a, e.rest) );
            if ( f > 0 ) {
                e.rest -= f;
                edges[ G[u][i]^1 ].rest += f;
                flow += f;
                a -= f;
                if ( a == 0 )break;
            }
        }
        return flow;
    }

    long long maxflow(int _s, int _t){
        s = _s, t = _t;
        long long flow = 0, mf;
        while ( bfs() ){
            fill(cur,cur+n,0);
            while ( (mf = dfs(s, INF)) ) flow += mf;
        }
        return flow;
    }
} dinic;
vector<tuple<int,int,int>>e;
vector<string>d;
int get(string s) {
    return lower_bound(all(d), s) - d.begin() + 1;
}
void go(string input) {
    freopen(input.c_str(),"r",stdin);
    string s, t;
    cin >> s >> s >> t >> t;
    vector<pair<pair<string,string>,int>>p;
    string from, to;
    int cost;
    while (cin >> to >> from >> cost) {
        p.pb({{from, to},cost});
        d.pb(from);
        d.pb(to);
    }
    sort(all(d));
    d.erase(unique(all(d)),d.end());
    for (auto &i : p) {
        int x = get(i.F.F);
        int y = get(i.F.S);
        e.pb({x, y, i.S});
    }
    vector<int>dist(d.size() + 5,mod);
    dist[get(s)] = 0;
    f((int)d.size()) {
        for (auto &j : e) {
            int from = get<0>(j);
            int to = get<1>(j);
            int c = get<2>(j);
            if (dist[from] + c < dist[to]) {
                dist[to] = dist[from] + c;
            }
        }
    }
    ofstream fout("short.txt");
    ofstream fout2("max.txt");
    f((int)d.size()) {
        fout << d[i] <<": " << dist[i + 1] << '\n';
    }
    dinic.n = d.size() + 5;
    int neg = 0;
    for (auto &i : p) {
        int x = get(i.F.F);
        int y = get(i.F.S);
        if (i.S < 0) {
            neg = 1;
        }
        if (i.S > 0)
            dinic.add_edge(x, y, i.S);
    }
    if (neg) {
        fout2 << "Max Flow: " << 0 << '\n';
    }
    else {  
        fout2 << "Max Flow: "<<dinic.maxflow(get(s),get(t)) << '\n';
    }
}
int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int c = 0;
    int t;
    if (!c) {
        t = 1;
    }
    else {
        cin >> t;
    }
    while (t--) {
        go((string)argv[1]);
    }
    
}