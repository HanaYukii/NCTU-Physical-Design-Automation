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
const int maxn = 105;
double T = 4000000000;
double r = 0.85;
vector<pair<int,int>>blocks;
vector<int>pos;
vector<int>neg;
int n;
int x[maxn], rx[maxn], y[maxn], ry[maxn];
int ansx[maxn], ansrx[maxn], ansy[maxn], ansry[maxn]; 
int lz[maxn<<2];
int sum[maxn<<2];
int Best_area, H, W, limx, limy;
double Last_cost; 
void pull(int x,int l,int r) {
    if (lz[x]) {
        sum[x] = r - l + 1;
    }
    else {
        if (l == r) sum[x] = 0;
        else sum[x] = sum[x<<1] + sum[x<<1|1];
    }
}
void update(int x,int l,int r,int ql,int qr, int d) {
    if (ql > qr)return;
    if (ql <= l && qr >= r) {
        lz[x] += d;
        pull(x,l,r);
        return;
    }
    int mid = (l + r) >> 1;
    if (ql <= mid) {
        update(x<<1,l,mid,ql,qr,d);
    }
    if (qr > mid) {
        update(x<<1|1,mid+1,r,ql,qr,d);
    }
    pull(x,l,r);
}
void check() {
    vector<tuple<int,int,int,int>>e;
    f(n) {
        int l, r, d, u;
        l = x[i], r = rx[i];
        l++;
        d = y[i], u = ry[i];
        e.pb({d,l,r,1});
        e.pb({u,l,r,-1});
    }
    int last = 0;
    ll ans = 0;
    sort(all(e));
    for (auto &i : e) {
        auto [pos, l, r, d] = i;
        if (pos != last) {
            ans += 1LL * sum[1] * (pos - last);
        }
        update(1,1,1000,l,r,d);
        last = pos;
    }
    int tot_area = 0;
    for (auto &i : blocks) {
        tot_area += i.F * i.S;
    }
    cout <<ans <<' '<<tot_area<<endl;
}
void cal() {
    int idx[n] = {};
    for (int i = 0 ; i < n ; i++) {
        idx[pos[i]] = i;
    }
    int b[n + 1] = {};
    auto update = [&](int x,int val) -> void{
        while (x <= n) {
            b[x] = max(b[x], val);
            x += x & (-x);
        }
    };
    auto query = [&](int x) -> int {
        int ret = 0;
        while (x) {
            ret = max(ret, b[x]);
            x -= x & (-x);
        }
        return ret;
    };
    for (int i = 0 ; i < n ; i++) {
        int prev = query(idx[neg[i]]);
        x[neg[i]] = prev;
        rx[neg[i]] = prev + blocks[neg[i]].F;
        update(idx[neg[i]] + 1, rx[neg[i]]);
    }
    memset(b, 0, sizeof(b));
    reverse(all(neg));
    for (int i = 0 ; i < n ; i++) {
        int prev = query(idx[neg[i]]);
        y[neg[i]] = prev;
        ry[neg[i]] = prev + blocks[neg[i]].S;
        update(idx[neg[i]] + 1, ry[neg[i]]);
    }
    reverse(all(neg));
}
void random_solution() {
    Last_cost = 1e9;
    random_shuffle(all(pos));
    random_shuffle(all(neg));
}
int getarea(int &x,int &y) {
    f1(n) {
        x = max(x, rx[i]);
        y = max(y, ry[i]);
    }
    return x * y;
}
void upd() {
    int r = rand() % 100;
    tuple<int,int,int>mov; 
    int rot = -1;
    if (r < 35) {
        int idx = rand() % n;
        rot = idx;
        swap(blocks[idx].F, blocks[idx].S);    
    }
    if (r < 50) {
        int n1 = rand() % n;
        int n2 = rand() % n;
        while (n2 == n1) {
            n2 = rand() % n;
        }
        if (rand() & 1) {
            mov = make_tuple(n1,n2,1);
            swap(pos[n1], pos[n2]);
        }
        else {
            mov = make_tuple(n1,n2,2);
            swap(neg[n1], neg[n2]);
        }
    }
    else {
        int n1 = rand() % n;
        int n2 = rand() % n;
        while (n2 == n1) {
            n2 = rand() % n;
        }
        mov = make_tuple(n1,n2,3);
        swap(pos[n1], pos[n2]);
        swap(neg[n1], neg[n2]);
    }
    cal();
    int X = 0, Y = 0;
    int Area = getarea(X, Y);
    double Ratio = (double)(X * limy) / (double)(Y * limx);
    if (Ratio < 1) {
        Ratio = 1 / Ratio;
    }
    if (X > limx) {
        Ratio *= 1.10;
    }
    if (Y > limy) {
        Ratio *= 1.10;
    }
    if (X <= limx && Y <= limy) {
        Ratio *= 0.8;
    }
    double Cost = ((double)Area * Ratio);
    bool force = (double)rand() / RAND_MAX < exp(-1 * (double)Cost / T);
    if (Cost <= Last_cost || force) {
        Last_cost = Cost;
    }
    else {
        if (get<2>(mov) == 3) {
            swap(pos[get<0>(mov)], pos[get<1>(mov)]);
            swap(neg[get<0>(mov)], neg[get<1>(mov)]);
        }
        else if (get<2>(mov) == 2) {
            swap(neg[get<0>(mov)], neg[get<1>(mov)]);
        }
        else {
            swap(pos[get<0>(mov)], pos[get<1>(mov)]);
        }
        if (~rot) {
            swap(blocks[rot].F, blocks[rot].S); 
        }
    }
}
void SA() {
    T = 4000000000;
    int iter = 500000;
    while (iter--) {
        upd();
        T *= r;
    }
}
void update_final() {
    int X = 0, Y = 0;
    int Area = getarea(X, Y);
    cout << X <<' '<<Y << endl;
    if (X <= limx && Y <= limy && Area <= Best_area) {
        H = X, W = Y;
        for (int i = 0 ; i < n ; i++) {
            ansx[i] = x[i];
            ansrx[i] = rx[i];
            ansy[i] = y[i];
            ansry[i] = ry[i];
        }
        Best_area = Area;
    }
}
int main() {
    srand(time(NULL));
    Best_area = 2e9;
    cin >> limx >> limy;
    cin >> n;
    for (int i = 0 ; i < n ; i++) {
        pos.pb(i);
        neg.pb(i);
        int x, y;
        cin >> x >> y;
        blocks.pb({x, y});
    }
    int SA_time = 30;
    H = W = 1e9;
    while (SA_time--) {
        random_solution();
        SA();
        update_final();
        cout << H << ' ' << W << endl;
    }
}