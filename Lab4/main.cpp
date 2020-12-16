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
    int totalWidth;
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
int cor, del;
vector<int>pos;
vector<int>ox, oy;
vector<int>order;
vector<Row>v;
vector<int>ansx, ansy;
vector<string>header;
vector<int>ori, sites;
void TestCutRow(){
    fstream file;
    file.open("test.gdt", ios::out);
    file<<"gds2{600\nm=2018-09-14 14:26:15 a=2018-09-14 14:26:15\nlib 'asap7sc7p5t_24_SL' 1 1e-6\ncell{c=2018-09-14 14:26:15 m=2018-09-14 14:26:15 'AND2x2_ASAP7_75t_SL'\n";
    //Terminal
    for (int i = Node_cnt - Terminal_cnt; i < Node_cnt; i++){
        file << "b{" << "0" << " xy(" << ox[i] << " " << oy[i] << " " << ox[i] << " " << oy[i] + py[i] << " " << ox[i] + px[i] << " " <<  oy[i] + py[i]<< " " << ox[i] + px[i]<< " " << oy[i] << ")}" << endl;
    }
    //Row
    for (auto &i : v)
        file << "b{" << "1" << " xy(" << i.left << " " << i.Y << " " << i.left << " " << i.Y + del << " " << i.right << " " << i.Y + del << " " << i.right << " " << i.Y << ")}" << endl;
    //SubRow
    file << "}\n}\n";
}
void preprocess() {
    vector<tuple<int,int,int,int>>ter;
    ofstream fo;
    for (int i = Node_cnt - Terminal_cnt ; i < Node_cnt ; i++) {
        ter.pb({ox[i],ox[i] + px[i], oy[i], oy[i] + py[i]});
    }
    sort(all(ter));
    for (int i = 0 ; i < Row_cnt ; i++) {
        int low = cor + i * del;
        int high = low + del;
        int last = ori[i];
        map<int,int>mp;
        for (auto &j : ter) {
            if (get<3>(j) <= low || get<2>(j) >= high) {
                continue;
            }
            mp[get<0>(j)]++;
            mp[get<1>(j)]--;
            //if (i == 0)
                //cout << get<0>(j) <<' '<<get<1>(j)<<endl;
        }
        int cur = 0;
        for (auto &j : mp) {
            if (cur == 0 && j.S > 0) {
                if (j.F > last) {
                    v.pb({last, j.F, low, {}});
                    if (j.F > ori[i] + sites[i]) {
                        v.back().right = ori[i] + sites[i];
                        last = ori[i] + sites[i];
                        break;
                    }
                }
            }
            else if (cur > 0 && j.S == -cur) {
                last = max(last, j.F);
            } 
            cur += j.S;
        }
        if (last < ori[i] + sites[i]) {
            v.pb({last, ori[i] + sites[i], low, {}});
        }
    }
    //cout << v.size() << endl;
    //TestCutRow();
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
    f(Row_cnt) {
        fin >> tr >> tr;
        int x;
        for (int j = 0 ; j < 8 ; j++) {
            fin >> tr >> tr >> x;
            if (i == 0 && j == 0) {
                cor = x;
            }
            else if (i == 0 && j == 1) {
                del = x;
            }
            else if (j == 6) {
                ori.pb(x);
            }
            else if (j == 7) {
                sites.pb(x);
            }
        }
        fin >> tr;
    }
    pos.pb(cor);
    while ((int)pos.size() < Row_cnt) {
        pos.pb(pos.back() + del);
    }
    fin.close();
    fin.open(pl);
    f(3) {
        getline(fin, tr);
        header.pb(tr);
    }
    header.pb("\n");
    f(Node_cnt - Terminal_cnt) {
        int x, y;
        fin >> tr >> x >> y >> tr >> tr;
        ox.pb(x);
        oy.pb(y);
        if (ox.back() < ori.back()) {
            ox.back() = ori.back();
        }
        if (ox.back() + px[ox.size() - 1] > ori.back() + sites.back()) {
            ox.back() = ori.back() + sites.back() - px[ox.size()-1];
        }
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
        if (c.back().optimal_x + c.back().totalWidth > v[idx].right) {
            c.back().optimal_x = v[idx].right - c.back().totalWidth;
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
double placerow(int idx,int flag, int xx,int yy) {
    if (v[idx].totalWidth + px[v[idx].idx.back()] > v[idx].right - v[idx].left) {
        return 1e18;
    }
    vector<Cluster>c;
    for (auto &i : v[idx].idx) {
        if (!c.size() || c.back().optimal_x + c.back().totalWidth < ox[i]) {
            c.pb(Cluster());
            add(c.back(), i);
            c.back().optimal_x = ox[i];
            c.back().firstCell = i;
        }
        else {
            add(c.back(), i);
            collapse(c, idx);
        }
    }
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
    return (X - xx) * (X - xx) + (Y - yy) * (Y - yy);
}
void abacus(){
    ansx.resize(Node_cnt);
    ansy.resize(Node_cnt);
    order.resize(Node_cnt - Terminal_cnt);
    iota(order.begin(), order.end(), 0);
    sort(all(order),[&](int a,int b){
        return ox[a] < ox[b];
    });
    for (auto &i : order) {
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
                f = 1;
                double dis = min(abs(ox[i] - v[left].left), abs(ox[i] - v[left].right));
                if ((ox[i] <= v[left].right && ox[i] >= v[left].left) || dis * dis < best) {
                    int o = ox[i];
                    if (o < v[left].left) {
                        ox[i] = v[left].left;
                    }
                    if (ox[i] + px[i] > v[left].right) {
                        ox[i] = v[left].right - px[i];
                    }
                    v[left].idx.pb(i);
                    double cost = placerow(left, 0, o, oy[i]);
                    v[left].idx.pop_back();
                    if (cost < best) {
                        best = cost;
                        idx = left;
                    }
                    ox[i] = o;
                }
            }
            if (right < (int)v.size() && (oy[i] - v[right].Y) * (oy[i] - v[right].Y) < best) {
                f = 1;
                double dis = min(abs(ox[i] - v[right].left), abs(ox[i] - v[right].right));
                if ((ox[i] <= v[right].right && ox[i] >= v[right].left) || dis * dis < best) {
                    int o = ox[i];
                    if (o < v[right].left) {
                        ox[i] = v[right].left;
                    }
                    if (ox[i] + px[i] > v[right].right) {
                        ox[i] = v[right].right - px[i];
                    }
                    v[right].idx.pb(i);
                    double cost = placerow(right, 0, o, oy[i]);
                    v[right].idx.pop_back();
                    if (cost < best) {
                        best = cost;
                        idx = right;
                    }
                    ox[i] = o;
                }      
            }
            if (!f) {
                break;
            }
            left--, right++;
        }
        if (ox[i] < v[idx].left) {
            ox[i] = v[idx].left;
        }
        if (ox[i] + px[i] > v[idx].right) {
            ox[i] = v[idx].right - px[i];
        }
        v[idx].idx.pb(i);
        placerow(idx, 1, 0, 0);
        v[idx].totalWidth += px[i];
    }
}
void print() {
    ofstream fo;
    fo.open("output.pl", ios::out);
    for (auto &i : header) {
        fo << i << '\n';
    }
    f(Node_cnt) {
        if (i < Node_cnt - Terminal_cnt)
            fo << "o" << i <<' '<<ansx[i] <<' '<<ansy[i] << " : N\n";
        else
            fo << "o" << i <<' '<<ox[i] <<' '<<oy[i] << " : N /FIXED\n";
    }
    fo.close();
}
int main(int argc, char** argv){
    string aux = argv[1];
    parse(aux);
    abacus();
    print();
}