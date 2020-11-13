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
const int maxn = 1e5;
pair<int,int>pr[maxn];
int x[maxn], rx[maxn], y[maxn], ry[maxn];
int b[maxn], p[maxn];
int n;
int lz[maxn<<2];
int sum[maxn<<2];
int root;
int iter = 500000;
int ls[maxn], rs[maxn], pa[maxn];
int best_area;
int tot_area;
int bpa[maxn],bls[maxn],brs[maxn],bx[maxn],brx[maxn],by[maxn],bry[maxn],broot;
pair<int,int>bpr[maxn];
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
        update(1,1,100000,l,r,d);
        last = pos;
    }
    //cout << "here"<<ans <<' '<<tot_area<<endl;
    assert(ans == tot_area);
}
void place(int now, int pre) {
    if (pre == 0) {
        x[now] = 0;
        rx[now] = pr[now].F;
        y[now] = 0;
        ry[now] = pr[now].S;
        p[now] = b[now] = 0;
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
    for (i = p[now] ; i > 0 ; i = p[i]) {
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
    //cout << now << endl;
    if (i == 0) {
        p[now] = 0;
    }
    y[now] = mx;
    ry[now] = y[now] + pr[now].S;
}
void dfs(int now, int pre) {
    if (!now) return;
    //cout << now <<' '<<pre << endl;
    place(now, pre);
    dfs(ls[now], now);
    dfs(rs[now], now);
}
void dfs2(int now, int pre) {
    if (!now) return;
    cout << now <<' '<<ls[now]<<' '<<rs[now]<<endl;
    dfs2(ls[now], now);
    dfs2(rs[now], now);
}
int getarea() {
    int x = 0;
    int y = 0;
    f1(n) {
        x = max(x, rx[i]);
        y = max(y, ry[i]);
    }
    return x * y;
}
void upd() {
    memset(x, 0, sizeof(x));
    memset(rx, 0, sizeof(rx));
    memset(y, 0, sizeof(y));
    memset(ry, 0, sizeof(ry));
    dfs(root, 0);
    //go();
    int Area = getarea();
    //cout << Area <<' '<<best_area<<endl;
    if (Area <= best_area) {
        best_area = Area;
        broot = root;
        f1(n) {
            bpa[i] = pa[i];
            bls[i] = ls[i];
            brs[i] = rs[i];
            bx[i] = x[i];
            brx[i] = rx[i];
            by[i] = y[i];
            bry[i] = ry[i];
            bpr[i].F = pr[i].F;
            bpr[i].S = pr[i].S;
        }
    }
    else {
        root = broot;
        f1(n) {
            pa[i] = bpa[i];
            ls[i] = bls[i];
            rs[i] = brs[i];
            x[i] = bx[i];
            rx[i] = brx[i];
            y[i] = by[i];
            ry[i] = bry[i];
            pr[i].F = bpr[i].F;
            pr[i].S = bpr[i].S;
        }
    }
    
}
void print() {
    dfs2(root, 0);
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
            pa[v[i*2 - 1]] = v[i - 1];
        }
        if (i * 2 + 1 <= n) {
            rs[v[i - 1]] = v[i*2];
            pa[v[i*2]] = v[i - 1];
        }
    }
}
void remove(int x) {
    int child    = 0;  // pull which child
    int subchild = 0;   // child's subtree
    int subparent= 0; 
    if(ls[x]||rs[x]){
        bool left = rand()%2;           // choose a child to pull up
        if(ls[x] == 0) left = 0;
        if(rs[x] == 0) left = 1;
  
  
        if(left){
            child = ls[x];          // child will never be NIL
            if(rs[x] != 0){
                subchild  = rs[child];
                subparent = rs[x];
                pa[rs[x]] = child; 
                rs[child] = rs[x];    // abut with node's another child
            }
        }
        else{
            child = rs[x];
            if(ls[x] != 0){
                subchild  = ls[child];
                subparent = ls[x];
                pa[ls[x]] = child;
                ls[child] = ls[x];
            }
        }
        //add_changed_nodes(subchild);
        pa[child] = pa[x];
    }
  
    if(pa[x] == 0){          // root
        //    changed_root = nodes_root;
        root = child;
    }else{                  // let parent connect to child
        //add_changed_nodes(node.parent);
        if(x == ls[pa[x]])
            ls[pa[x]] = child;
        else
            rs[pa[x]] = child;
    }
  
    // place subtree
    if(subchild != 0){
        while(1){
            if(ls[subparent] == 0 || rs[subparent] == 0){
                pa[subchild] = subparent;
                if(ls[subparent]==0) ls[subparent] = subchild;
                else rs[subparent] = subchild;
                break;
            } else{
                subparent = (rand()%2 ? ls[subparent] : rs[subparent]);
            }
        }
    }
}
void concat(int now,int pre) {
    if (rand() % 2) {
        pa[now] = pre;
        ls[now] = ls[pre];
        ls[pre] = now;
        rs[now] = 0;
        if (ls[now]) {
            pa[ls[now]] = now;
        }
    }
    else {
        pa[now] = pre;
        rs[now] = rs[pre];
        rs[pre] = now;
        ls[now] = 0;
        if (rs[now]) {
            pa[rs[now]] = now;
        }
    }
}
void swap2(int n1,int n2) {
    if (root == n1) {
        root = n2;
    }
    else if (root == n2) {
        root = n1;
    }
    if (ls[n1]) {
        pa[ls[n1]] = n2;
    }
    if (rs[n1]) {
        pa[rs[n1]] = n2;
    }
    if (ls[n2]) {
        pa[ls[n2]] = n1;
    }
    if (rs[n2]) {
        pa[rs[n2]] = n1;
    }
    if (pa[n1]) {
        if (ls[pa[n1]] == n1) {
            ls[pa[n1]] = n2;
        }
        else {
            rs[pa[n1]] = n2;
        }
    }
    if (pa[n2]) {
        if (ls[pa[n2]] == n2) {
            ls[pa[n2]] = n1;
        }
        else {
            rs[pa[n2]] = n1;
        }
    }
    swap(ls[n1], ls[n2]);
    swap(rs[n1], rs[n2]);
    swap(pa[n1], pa[n2]); 
}
void swap1(int n1,int n2) {
    if (pa[n1] == n2) {
        swap(n1, n2);
    }
    bool is_left = ls[n1] == n2 ;
    if (root == n1 )
        root = n2;
    else if ( root == n2 )
        root = n1;
    if (is_left) {
        // change right
        if (rs[n1] != 0) 
            pa[rs[n1]] = n2;
        if (rs[n2] != 0) 
            pa[rs[n2]] = n1;
        swap(rs[n1], rs[n2]);
        // change left
        if (ls[n2] != 0) 
            pa[ls[n2]] = n1;
        ls[n1] = ls[n2];
        ls[n2] = n1;

    } else {
        // change left
        if (ls[n1] != 0) 
             pa[ls[n1]] = n2;
        if (ls[n2] != 0) 
             pa[ls[n2]] = n1;
        swap(ls[n1], ls[n2]);
        // change right
        if (rs[n2] != 0) 
            pa[rs[n2]] = n1;
        rs[n1] = rs[n2];
        rs[n2] = n1;
    }
    // change parent
    if (pa[n1] != 0) {
        int p = pa[n1];
        if(ls[p] == n1) 
            ls[p] = n2;
        else    
            rs[p] = n2;
    }
    pa[n2] = pa[n1];
    pa[n1] = n2;
}
int main() {
    srand(time(NULL));
    cin >> n;
    best_area = INT_MAX;
    f1(n) {
        pr[i].F = rand() % 70 + 1;
        pr[i].S = rand() % 70 + 1;
        //cin >> pr[i].F >> pr[i].S;
        tot_area += pr[i].F * pr[i].S;
    }
    init_tree();
    upd();
    f(iter) {
        int r = rand() % 100;
        if (r < 25) {
            // rotate
            //cout << "ROTATE" << endl;
            int idx = rand() % n + 1;
            swap(pr[idx].F, pr[idx].S);
            
        }
        if (r < 50) {
            // swap
            //continue;
            //print();
            
            int n1 = rand() % n + 1;
            int n2 = rand() % n + 1;
            while (n2 == n1) {
                n2 = rand() % n + 1;
            }
            //cout << "SWAP" << ' ' << n1 <<' '<<n2<<endl;
            if (pa[n1] == n2 || pa[n2] == n1) {
                swap1(n1, n2);
            }
            else {
                swap2(n1, n2);
            }
        }
        else {
            // move
            //cout << "MOVE" << endl;
            int n1 = rand() % n + 1;
            int n2 = rand() % n + 1;
            while (n2 == n1) {
                n2 = rand() % n + 1;
            }
            remove(n1);
            concat(n1, n2);
        }
        upd();
    }
    cout << best_area << endl;
    //cout << "FINISH\n";
}