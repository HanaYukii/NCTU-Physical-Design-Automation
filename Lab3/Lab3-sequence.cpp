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
const int maxn = 1e4;
vector<pair<int,int>>blocks;
vector<int>pos;
vector<int>neg;
int n;
int x[maxn], rx[maxn], y[maxn], ry[maxn];
int lz[maxn<<2];
int sum[maxn<<2];
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
        //cout << pos <<' '<<l<<' '<<r<<' '<<d<<endl;
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
    //assert(ans == tot_area);
}
void cal() {
    int idx[n] = {};
    f(n) {
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
    check();
    f(n) {
        cout << i <<' '<<blocks[i].F <<' '<<blocks[i].S <<' '<<x[i] <<' '<<rx[i] <<' '<<y[i] << ' '<<ry[i] << endl;
    }
}
void random_solution() {
    random_shuffle(all(pos));
    random_shuffle(all(neg));
    cal();
}
int main() {
    cin >> n;
    f(n) {
        pos.pb(i);
        neg.pb(i);
        blocks.pb({rand() % 50 + 1, rand() % 50 + 1});
    }
    random_solution();
}