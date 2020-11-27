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
const int maxn = 2005;
double T = 4000000000;
double r = 0.85;
vector<pair<int,int>>blocks;
vector<int>pos;
vector<int>neg;
int n, m;
int x[maxn], rx[maxn], y[maxn], ry[maxn]; // bound during SA
int ansx[maxn], ansrx[maxn], ansy[maxn], ansry[maxn]; // best answer
int Best_area, Best_wire, H, W, limx, limy; // H : answer's height, W : answer width, limx : height constraint, limy : width constraint
double Last_cost, Best_cost, alpha; 
vector<vector<int>>net; // nets
map<string,int>mp; // string to block or terminal
string block_name[maxn];
void cal() { // find floorplan of given sequence
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
void random_solution() { //generate random sequence
    Last_cost = 1e9;
    random_shuffle(all(pos));
    random_shuffle(all(neg));
}
int getarea(int &x,int &y) { // find height width of given floorplan
    f(n) {
        x = max(x, rx[i]);
        y = max(y, ry[i]);
    }
    return x * y;
}
int getwire() { // find wire length of given floorplan
    int ret = 0;
    for (auto &i : net) {
        int mix = INT_MAX, mxx = INT_MIN, miy = INT_MAX, mxy = INT_MIN;
        for (auto &j : i) {
            int xx = x[j] + (rx[j] - x[j]) / 2;
            int yy = y[j] + (ry[j] - y[j]) / 2;
            mix = min(mix, xx);
            mxx = max(mxx, xx);
            miy = min(miy, yy);
            mxy = max(mxy, yy);
        }
        ret += mxx - mix + mxy - miy;
    }
    return ret;
}
void upd() { // one iterate of SA
    int r = rand() % 100;
    tuple<int,int,int>mov; // store movement for roll back
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
    if (X <= limx && Y <= limy) { // fit height and width
        Ratio *= 0.8;
    }
    double Cost = ((double)Area * Ratio);
    bool force = (double)rand() / RAND_MAX < exp(-1 * (double)Cost / T);
    if (Cost <= Last_cost || force) {
        Last_cost = Cost;
    }
    else {
        // roll back
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
    int Wire = getwire();
    double Cost = (double)Area * alpha + (double)Wire * (1.0 - alpha);
    if (X <= limx && Y <= limy && Cost <= Best_cost) { // find a better solution during current iteration, update final answer
        H = X, W = Y;
        Best_wire = Wire;
        Best_area = Area;
        Best_cost = Cost;
        for (int i = 0 ; i < n ; i++) {
            ansx[i] = x[i];
            ansrx[i] = rx[i];
            ansy[i] = y[i];
            ansry[i] = ry[i];
        }
        Best_area = Area;
    }
}
int main(int argc,char **argv) {
    clock_t st,ed;
    st = clock();
    srand(time(NULL));
    Best_area = 2e9;
    fstream fin, fin2, fout; // fin: block terminal, fin2: nets
    string trash;
    fin.open(argv[2], ios::in);
    fin >> trash >> limx >> limy;
    fin >> trash >> n >> trash >> m;
    for (int i = 0 ; i < n ; i++) {
        pos.pb(i);
        neg.pb(i);
    }
    blocks.resize(n + m + 5);
    for (int i = 0 ; i < n ; i++) {
        fin >> block_name[i] >> blocks[i].F >> blocks[i].S;
        mp[block_name[i]] = i;
    }
    for (int i = n ; i < n + m ; i++) {
        string s;
        fin >> s >> trash >> blocks[i].F >> blocks[i].S;
        x[i] = rx[i] = blocks[i].F;
        y[i] = ry[i] = blocks[i].S;
        mp[s] = i;
    }
    Best_cost = 1e9;
    fin2.open(argv[3], ios::in);
    int netnum;
    fin2 >> trash >> netnum;
    alpha = stod(argv[1]);
    f(netnum) {
        fin2 >> trash;
        int k;
        fin2 >> k;
        net.pb({});
        while (k--) {
            string s;
            fin2 >> s;
            net.back().pb(mp[s]);
        }
    }
    int SA_time = 30;
    H = W = 1e9;
    while (SA_time--) {
        random_solution();
        SA();
        update_final();
    }
    fout.open(argv[4], ios::out);
    fout << fixed << setprecision(1) << Best_cost << '\n';
    fout << Best_wire << '\n';
    fout << Best_area << '\n';
    int Final_x = 0, Final_y = 0;
    f(n) {
        Final_x = max(Final_x, ansrx[i]);
        Final_y = max(Final_y, ansry[i]);
    }
    fout << Final_x << ' ' << Final_y << '\n';
    ed = clock();
    fout << (ed - st) / CLOCKS_PER_SEC << '\n';
    f(n) {
        fout << block_name[i] << ' ' << ansx[i] <<' '<< ansy[i] <<' '<< ansrx[i] <<' '<< ansry[i] << endl;
    }
}