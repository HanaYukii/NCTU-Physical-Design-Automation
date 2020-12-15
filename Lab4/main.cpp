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
const int mod = 1e9 + 7;
const int maxn = 300005;
struct Row
{
    int left, right;
    int Y;
    vector<int>idx;
    bool operator < (const Row &x) const{
        if (Y != x.Y) {
            return Y < x.Y;
        }
        return left < x.left;;
    }
};
struct Cluster
{
    Cluster() {totalWeight = 0; totalWidth = 0; quadratic = 0;};
    int optimal_x,totalWeight,totalWidth,quadratic,firstCell,lastCell;    
};
int Node_cnt, Terminal_cnt, Row_cnt;
vector<int>px, py;
int ori, cor, del, sites;
vector<int>pos;
vector<int>ox, oy;
vector<int>order;
vector<Row>v;
vector<int>ansx, ansy;
void preprocess() {
    cout << "NOK" << endl;
    vector<tuple<int,int,int,int>>ter;
    ofstream fo;
    fo.open("boundary.txt", ios::out);
    for (int i = Node_cnt - Terminal_cnt ; i < Node_cnt ; i++) {
        ter.pb({ox[i],ox[i] + px[i], oy[i], oy[i] + py[i]});
        fo << "b{1 xy(";
        fo << ox[i] <<' '<<oy[i]+py[i] << ' '<< ox[i] <<' '<<oy[i] <<' ' <<ox[i]+px[i]<<' '<<oy[i]<<' '<<ox[i]+px[i] <<' '<<oy[i]+py[i] <<")}\n";
    }
    fo.close();
    sort(all(ter));
    for (int i = 0 ; i < Row_cnt ; i++) {
        int low = cor + i * del;
        int high = low + del;
        int last = ori;
        map<int,int>mp;
        for (auto &j : ter) {
            if (get<3>(j) <= low || get<2>(j) >= high) {
                continue;
            }
            mp[get<0>(j)]++;
            mp[get<1>(j)]--;
            if (i == 0)
                cout << get<0>(j) <<' '<<get<1>(j)<<endl;
        }
        int cur = 0;
        for (auto &j : mp) {
            if (cur == 0 && j.S > 0) {
                v.pb({last, j.F, low, {}});
            }
            else if (cur > 0 && j.S == -cur) {
                last = j.F;
            } 
            cur += j.S;
        }
        if (last < ori + sites) {
            v.pb({last, ori + sites, low, {}});
        }
    }
    cout << v.size() << endl;
}
void parse(string str) {
    ifstream fin(str);
    string tr, node, nets, wts, pl, scl, shapes;
    fin >> tr >> tr >> node >> nets >> wts >> pl >> scl >> shapes;
    fin.close();
    fin.open(node);
    string s;
    while (1) {
        getline(fin, s);
        if (s.find("NumNodes") != string::npos) {
            break;
        }
    }
    stringstream ss(s);
    ss >> s >> s >> Node_cnt;
    fin >> s >> s >> Terminal_cnt;
    f(Node_cnt - Terminal_cnt) {
        string name;
        int x, y;
        fin >> tr >> x >> y;
        px.pb(x);
        py.pb(y);
    }
    f(Terminal_cnt) {
        string name;
        int x, y;
        fin >> tr >> x >> y;
        px.pb(x);
        py.pb(y);
        fin >> tr;
    }
    fin.close();
    fin.open(scl);
    while (1) {
        getline(fin, s);
        if (s.find("NumRows") != string::npos) {
            break;
        }
    }
    stringstream ss2(s);
    ss2 >> tr >> tr >> Row_cnt;
    fin >> tr >> tr;
    int x;
    for (int j = 0 ; j < 8 ; j++) {
        fin >> tr >> tr >> x;
        if (j == 0) {
            cor = x;
        }
        if (j == 1) {
            del = x;
        }
        else if (j == 7) {
            ori = x;
        }
        else if (j == 8) {
            sites = x;
        }
    }
    pos.pb(cor);
    while ((int)pos.size() < Row_cnt) {
        pos.pb(pos.back() + del);
    }
    fin.close();
    fin.open(pl);
    f(3) {
        getline(fin, tr);
    }
    f(Node_cnt - Terminal_cnt) {
        int x, y;
        fin >> tr >> x >> y >> tr >> tr;
        ox.pb(x);
        oy.pb(y);
    }
    f(Terminal_cnt) {
        int x, y;
        fin >> tr >> x >> y >> tr >> tr >> tr;
        ox.pb(x);
        oy.pb(y);
    }
    fin.close();
    preprocess();
}
void add(Cluster &c,int i) {
    c.lastCell = i;
    c.totalWeight++;
    c.quadratic += ox[i] - c.totalWidth;
    c.totalWidth += px[i];
}
void add(Cluster &c1, Cluster &c2) {
    c1.lastCell = c2.lastCell;
    c1.totalWeight += c2.totalWeight;
    c1.quadratic += c2.quadratic - c2.totalWeight * c1.totalWidth;
    c1.totalWidth += c2.totalWidth;
}
void collapse(vector<Cluster>&c,int idx) {
    while (1) {
        c.back().optimal_x = c.back().quadratic / c.back().totalWeight;
        if (c.back().optimal_x < v[idx].left) {
            c.back().optimal_x = v[idx].left;
        }
        if (c.back().optimal_x + c.back().totalWeight > v[idx].right) {
            c.back().optimal_x = v[idx].right - c.back().totalWeight;
        }
        if (c.size() == 1) {
            break;
        }
        else {
            if (c[c.size()-2].optimal_x + c[c.size()-2].totalWidth >= c.back().optimal_x) {
                add(c[c.size()-2], c[c.size()-1]);
                c.pop_back();
            }
            else {
                break;
            }
        }
    }
}
double placerow(int idx,int flag) {
    vector<Cluster>c;
    for (auto &i : v[idx].idx) {
        if (!c.size() || c.back().optimal_x + c.back().totalWidth <= ox[i]) {
            c.pb(Cluster());
            c.back().optimal_x = ox[i];
            if (c.back().optimal_x < v[idx].left) {
                c.back().optimal_x = v[idx].left;
            }
            if (c.back().optimal_x + c.back().totalWeight > v[idx].right) {
                c.back().optimal_x = v[idx].right - c.back().totalWeight;
            }
            c.back().firstCell = i;
            add(c.back(), i);
        }
        else {
            add(c.back(), i);
            collapse(c, idx);
        }
        if (c.back().totalWidth  > v[idx].right - v[idx].left) {
            return 1e18;
        }
    }
    //cout << "NANI" << endl;
    if (flag) {
        int ptr = 0;
        for (auto &i : c) {
            int x = i.optimal_x;
            while(1) {
                ansx[v[idx].idx[ptr]] = x;
                ansy[v[idx].idx[ptr]] = v[idx].Y;
                if (v[idx].idx[ptr] == i.lastCell) {
                    ptr++;
                    break;
                }
                else {
                    x += px[v[idx].idx[ptr]];
                    ptr++;
                }
            }
        }
    }
    int X = c.back().optimal_x + c.back().totalWidth - px[v[idx].idx.back()];
    int Y = v[idx].Y;
    //cout << "GG" << endl;
    return (X - ox[v[idx].idx.back()]) * (X - ox[v[idx].idx.back()]) + (Y - oy[v[idx].idx.back()]) * (Y - oy[v[idx].idx.back()]);
}
void abacus(){
    ansx.resize(Node_cnt);
    ansy.resize(Node_cnt);
    order.resize(Node_cnt - Terminal_cnt);
    iota(order.begin(), order.end(), 0);
    sort(all(order),[&](int a,int b){
        return ox[a] < ox[b];
    });
    int cnt = 0;
    for (auto &i : order) {
        cnt++;
        if (cnt % 1000 == 0) {
            cout << cnt << endl;
        }
        double best = 1e18;
        int idx;
        int right = v.size();
        int left = 0;
        while (right - left > 1) {
            int mid = (left + right) >> 1;
            if (v[mid].Y <= oy[i]) {
                left = mid;
            }
            else {
                right = mid;
            }
        }
        while (left >= 0 || right < (int)pos.size()) {
            int f = 0;
            if (left >= 0 && (oy[i] - v[left].Y) * (oy[i] - v[left].Y) < best) {
                v[left].idx.pb(i);
                double cost = placerow(left, 0);
                v[left].idx.pop_back();
                if (cost < best) {
                    best = cost;
                    idx = left;
                }
                f = 1;
            }
            if (right < (int)v.size() && (oy[i] - v[right].Y) * (oy[i] - v[right].Y) < best) {
                v[right].idx.pb(i);
                double cost = placerow(right, 0);
                v[right].idx.pop_back();
                if (cost < best) {
                    best = cost;
                    idx = right;
                }
                f = 1;
            }
            if (!f) {
                break;
            }
            //cout << best << ' '<<left <<' '<<right << endl;
            left--, right++;
        }
        v[idx].idx.pb(i);
        //cout << "gg" << endl;
        placerow(idx, 1);
        //cout << "gg" << endl;
    }
}
void print() {
    f(100) {
        cout << ansx[i] <<' '<<ansy[i] << endl;
    }
}
int main(){
    string aux = "adaptec1.aux";
    parse(aux);
    abacus();
    print();
    cout << "OK" << endl;
}