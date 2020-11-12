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
const int maxn = 1e2;
pair<int,int>pr[maxn];
int x[maxn], rx[maxn], y[maxn], ry[maxn];
int b[maxn], p[maxn];
int n;
int lz[maxn<<2];
int sum[maxn<<2];
int root;
int ls[maxn], rs[maxn];
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
void go() { 
    vector<tuple<int,int,int,int>>e;
    f1(n) {
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
        update(1,1,100,l,r,d);
        last = pos;
    }
    cout << ans << '\n';
}
void place(int now, int pre) {
    if (pre == 0) {
        x[now] = 0;
        rx[now] = pr[now].F;
        y[now] = 0;
        ry[now] = pr[now].S;
        p[now] = b[now] = -1;
        return;
    }
    if (ls[pre] == now) {
        x[now] = rx[pre];
        rx[now] = x[now] + pr[now].F;
        if (p[pre] == 0) {
            y[now] = 0;
            ry[now] = y[now] + pr[now].S;
            p[pre] = now;
            b[now] = pre;
            p[now] = 0;
            return;
        }
        else {
            b[now] = pre;
            p[now] = p[pre];
            b[p[now]] = now;
            p[pre] = now;
        }
    }
    else {
        x[now] = x[pre];
        rx[now] = x[now] + pr[now].F;
        if (b[pre] == 0) {
            b[now] = 0;
            p[now] = pre;
            b[pre] = now;
        }
        else {
            p[b[pre]] = now;
            b[now] = b[pre];
            p[now] = pre;
            b[pre] = now;
        }
    }
    int mx = 0;
    int i;
    for (i = p[now] ; i ; i = p[i]) {
        mx = max(mx, ry[i]);
        if (rx[i] >= rx[now]) {
            if (rx[i] == rx[now]) {
                p[now] = p[i];
                if (p[i] == 0) {
                    b[p[i]] = now;
                }
            }
            else {
                p[now] = i;
                b[i] = now;
            }
            break;
        }
    }
    if (i == 0) {
        p[now] = 0;
    }
    y[now] = mx;
    ry[now] = y[now] + pr[now].S;
}
void dfs(int now, int pre) {
    if (!now) return;
    cout << now <<' '<<pre << endl;
    place(now, pre);
    //cout << now <<' '<<x[now] <<' '<<y[now]<<' '<<rx[now] <<' '<<ry[now]<< endl;
    dfs(ls[now], now);
    dfs(rs[now], now);
}
void init_tree() {
    vector<int>v;
    f1(n) {
        v.pb(i);
    }
    random_shuffle(all(v));
    root = v[0];
    for (int i = 1 ; i <= n / 2; i++) {
        if (i * 2 <= n) {
            ls[v[i - 1]] = v[i*2 - 1];
        }
        if (i * 2 + 1 <= n) {
            rs[v[i - 1]] = v[i*2];
        }
    }
    cout << root << endl;
    f1(n) {
        cout << i <<' '<<ls[i]<<' '<<rs[i] << endl;
    }
}
int main() {
    cin >> n;
    int sum = 0;
    f1(n) {
        pr[i].F = rand() % 7;
        pr[i].S = rand() % 7;
        //cin >> pr[i].F >> pr[i].S;
        sum += pr[i].F * pr[i].S;
    }
    init_tree();
    dfs(root, 0);
    go();
    cout << sum << endl;
}