#include <bits/stdc++.h>
#include <ctime>
using namespace std;
#define ll long long
#define fr(i,j,k) for(int i=j;i<k;i++)
#define f(n) fr(i,0,n)
#define f1(n) fr(i,1,n+1)
#define pb push_back
#define F first
#define S second
#define all(x) x.begin(), x.end()
const int mod = 1e9 + 7;
const int maxn = 5e5+5;
const double tl = 4000;
clock_t b;
int state[100005];
vector<vector<int>>edges;
vector<int>p_to_e[100005];
int m, n;
int best;
int cur;
int ans[100005];
int state_cnt[100005][2];
int cnt[2];
int lb, ub;
int d[100005];
vector<int>v[100005];
map<int,vector<vector<int>>>rnd_edges;
vector<int>rnd_perm;
int fi(int x) {
    return d[x] == x ? x : d[x] = fi(d[x]);
}
void merge(int x,int y) {
    if (v[x].size() > v[y].size()) {
        swap(x, y);
    }
    d[x] = y;
    while (v[x].size()) {
        v[y].pb(v[x].back());
        v[x].pop_back();
    }
}
void cal() {
    cur = 0;
    int idx = 0;
    for (auto &j :edges) {
        for (auto &k : j) {
            state_cnt[idx][state[k]]++;
        }
        if (state_cnt[idx][0] && state_cnt[idx][1]) {
            cur++;
        }
        idx++;
    }
}
void upd() {
    best = cur;
    f1(n) {
        ans[i] = state[i];
    }
}
int D[100005];
set<pair<int,int>>z, o;
int lim;
void chg(int idx,int delta) {
    cur += delta;
    cnt[state[idx]]--;
    for (auto &j : p_to_e[idx]) {
        state_cnt[j][state[idx]]--;
        if (state_cnt[j][state[idx]] == 0) {
            for (auto &k : edges[j]) {
                if (z.count({D[k], k})) {
                    z.erase({D[k], k});
                    D[k]++;
                    z.insert({D[k], k});
                }
                else if (o.count({D[k], k})) {
                    o.erase({D[k], k});
                    D[k]++;
                    o.insert({D[k], k});
                }
            }
        }
        if (state_cnt[j][state[idx]] == 1) {
            for (auto &k : edges[j]) {
                if (state[k] != state[idx])continue;
                if (z.count({D[k], k})) {
                    z.erase({D[k], k});
                    D[k]--;
                    z.insert({D[k], k});
                }
                else if (o.count({D[k], k})) {
                    o.erase({D[k], k});
                    D[k]--;
                    o.insert({D[k], k});
                }
            }
        }
    }
    state[idx] ^= 1;
    cnt[state[idx]]++;
    for (auto &j : p_to_e[idx]) {
        state_cnt[j][state[idx]]++;
        if (state_cnt[j][state[idx]] == 1) {
            for (auto &k : edges[j]) {
                if (z.count({D[k], k})) {
                    z.erase({D[k], k});
                    D[k]--;
                    z.insert({D[k], k});
                }
                else if (o.count({D[k], k})) {
                    o.erase({D[k], k});
                    D[k]--;
                    o.insert({D[k], k});
                }
            }
        }
        if (state_cnt[j][state[idx]] == 2) {
            for (auto &k : edges[j]) {
                if (state[k] != state[idx])continue;
                if (z.count({D[k], k})) {
                    z.erase({D[k], k});
                    D[k]++;
                    z.insert({D[k], k});
                }
                else if (o.count({D[k], k})) {
                    o.erase({D[k], k});
                    D[k]++;
                    o.insert({D[k], k});
                }
            }
        }
    }
}
int mov(int idx) {
    int del = 0;
    for (auto &i : p_to_e[idx]) {
        if (state_cnt[i][state[idx]] == 1) {
            del--;
        }
        else if (state_cnt[i][state[idx]^1] == 0) {
            del++;
        }
    }
    return del;
}

void FM() {
    cnt[0] = cnt[1] = 0;
    f1(n) {
        state[i] = 0;
    }
    int sz = 0;
    random_shuffle(all(rnd_perm));
    for(auto &i : rnd_perm) {
        random_shuffle(all(v[i]));
        for (auto &j : v[i]) {
            sz++;
            state[j] = 1;
            if (sz == ub)break;
        }
        if (sz >= lb &&sz <= ub) break;
    }
    f(m) {
        state_cnt[i][0] = state_cnt[i][1] = 0;
    }
    f1(n) {
        cnt[state[i]]++;
    }
    cal();
    z.clear();
    o.clear();

    for (int i = 1 ; i <= n ; i++) {
        D[i] = mov(i);
        //cout << D[i] << endl;
        if (state[i] == 0) {
            z.insert({D[i], i});
        }
        else {
            o.insert({D[i], i});
        }
    }
    if (cur < best) {
        upd();
    }
    for (int i = 1 ; i <= n ; i++) {
        if (!z.size() || cnt[state[0]] == lb) {
            assert(o.size());
            auto a = *o.begin();
            o.erase(a);
            chg(a.S, a.F);
        }
        else if (!o.size() || cnt[state[0]] == ub) {
            assert(z.size());
            auto a = *z.begin();
            z.erase(a);
            chg(a.S, a.F);
        }
        else {
            assert(o.size() && z.size());
            auto a = *o.begin();
            auto b = *z.begin();
            if (a.F < b.F) {
                o.erase(a);
                chg(a.S, a.F);
            }
            else {
                z.erase(b);
                chg(b.S, b.F);
            }
        }
        if (cur < best) {
            upd();
        }
    }
}

void go() {
    memset(d, -1, sizeof(d));
    cin >> m >> n;
    int t = 20;
    if (n > 10000) {
        lim = n / 2;
        t = 5;
        srand(7777);
    }
    else {
        lim = n / 9;
        srand(77);
    }
    lb = n * 45 / 100 + 2;
    ub = n - lb;
    if (lb > ub) {
        lb = n / 2;
        ub = n - lb;
    }
    string s;
    getline(cin, s);
    best = 1e9;
    f(m) {
        getline(cin, s);
        stringstream ss(s);
        vector<int>add;
        int x;
        while (ss >> x) {
            add.pb(x);
        }
        if (add.size() == 1)continue;
        random_shuffle(all(add));
        rnd_edges[add.size()].pb(add);
    }

    while(t--) {
        f1(n) {
            d[i] = i;
            v[i] = {i};
        }
        edges.clear();
        for (auto &i : rnd_edges) {
            random_shuffle(all(i.S));
            for (auto &j : i.S) {
                edges.pb(j);
            }
        }
        m = edges.size();
        f1(n) {
            p_to_e[i].clear();
        }
        f(m) {
            for (auto &j : edges[i]) {
                p_to_e[j].pb(i);
            }
        }

        for (auto &i : edges) {
            for (auto &j : i) {
                int x = fi(i[0]);
                int y = fi(j);
                if (x == y)continue;
                if (y != x && (int)v[x].size() + (int)v[y].size() <= lim) {
                    merge(x, y);
                }
                else {
                    break;
                }
            }
        }
        rnd_perm.clear();
        int sum = 0;
        f1(n) {
            sum += v[i].size();
        }
        //cout << "TOT"<<sum << endl;
        f1(n) {
            if (v[i].size()) {
                rnd_perm.pb(i);
            }
        }
        int iter;
        if (n > 10000) {
            iter = 3;
        }
        else {
            iter = 20;
        }
        for (int z = 0 ; z < iter ; z++)
            FM();
        b = clock();
        if (b > tl)break;
    }
    
    int real = 0;
    for (auto &i : edges) {
        int c[2] = {};
        for (auto &j : i) {
            c[ans[j]]++;
        }
        if (c[0] && c[1]) {
            real++;
        }
    }
    int c0 = 0, c1 = 0;
    f1(n) {
        if (ans[i] == 0)
            c0++;
        else 
            c1++;
    }
    //cout << "HERE" << endl;
    //cout << c0 <<' '<<c1<<endl;
    //cout << best << ' '<<real << '\n';
    f1(n) {
        cout << ans[i] << '\n';
    }
}
int main(int argc,char **argv) {
    freopen(argv[1],"r",stdin);
    freopen("output.txt","w",stdout);
    srand(666);
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
        go();
    }
}