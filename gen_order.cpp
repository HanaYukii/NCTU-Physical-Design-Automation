#include<bits/stdc++.h>
using namespace std;
map<string, int>group;
int net_num, layer, lim;

struct KuhnMunkres {
    int n;
    const int inf = 1e9;
    vector<int>perm;
    vector<vector<int>> g;
    vector<int> lx, ly, slack;
    vector<int> match, visx, visy;
    KuhnMunkres(int n) : n(n), g(n, vector<int>(n)),
        lx(n), ly(n), slack(n), match(n), visx(n), visy(n) {}
    vector<int> & operator[](int i) { return g[i]; }
    bool dfs(int i, bool aug) { // aug = true 表示要更新 match
        if(visx[i]) return false;
        visx[i] = true;
        for(auto &j : perm) {
            if(visy[j]) continue;
            // 一邊擴增交錯樹、尋找增廣路徑
            // 一邊更新slack：樹上的點跟樹外的點所造成的最小權重
            int d = lx[i] + ly[j] - g[i][j];
            if(d == 0) {
                visy[j] = true;
                if(match[j] == -1 || dfs(match[j], aug)) {
                    if(aug)
                        match[j] = i;
                    return true;
                }
            } else {
                slack[j] = min(slack[j], d);
            }
        }
        return false;
    }
    bool augment() { // 回傳是否有增廣路
        for(int j = 0; j < n; j++) if(!visy[j] && slack[j] == 0) {
            visy[j] = true;
            if(match[j] == -1 || dfs(match[j], false)) {
                return true;
            }
        }
        return false;
    }
    void relabel() {
        int delta = inf;
        for(int j = 0; j < n; j++) if(!visy[j]) delta = min(delta, slack[j]);
        for(int i = 0; i < n; i++) if(visx[i]) lx[i] -= delta;
        for(int j = 0; j < n; j++) {
            if(visy[j]) ly[j] += delta;
            else slack[j] -= delta;
        }
    }
    int solve() {
        for(int i = 0; i < n; i++) {
            perm.push_back(i);
            lx[i] = 0;
            for(int j = 0; j < n; j++) lx[i] = max(lx[i], g[i][j]);
        }
        fill(ly.begin(), ly.end(), 0);
        fill(match.begin(), match.end(), -1);

        for(int i = 0; i < n; i++) {
            // slack 在每一輪都要初始化
            fill(slack.begin(), slack.end(), inf);
            fill(visx.begin(), visx.end(), false);
            fill(visy.begin(), visy.end(), false);
            random_shuffle(perm.begin(), perm.end());
            if(dfs(i, true)) continue;
            // 重複調整頂標直到找到增廣路徑
            while(!augment()) relabel();
            fill(visx.begin(), visx.end(), false);
            fill(visy.begin(), visy.end(), false);
            dfs(i, true);
        }
        int ans = 0;
        for(int j = 0; j < n; j++) if(match[j] != -1) ans += g[match[j]][j];
        return ans;
    }
};

struct net
{
    string name, pin;
    int idx;
    int len_sum;
    int layer;
    pair<int,int>c1_pos, c2_pos;
    pair<int,int>c1_out, c2_out;
};
vector<net>escape_order[3];
void parse_group(string s) {
    string file = s + ".group";
    ifstream in(file);
    string name;
    int id;
    while (in >> name >> id) {
        group[name] = id;
    }
}
void solve(string s) {
    string file = s + ".net";
    ifstream in(file);
    string str;
    // Input 
    in >> str >> net_num >> layer >> lim;
    //Escape to 0
    vector<net>net_pool;
    for (int i = 0 ; i < net_num ; i++) {
        int x, y;
        string name, pin;
        net add;
        in >> name >> pin >> x >> y;
        add.name = name;
        add.pin = pin;
        add.len_sum = y;
        add.c1_pos = {x, y};
        net_pool.push_back(add);
    }
    // to 21
    in >> str >> net_num >> layer >> lim;
    for (int i = 0 ; i < net_num ; i++) {
        int x, y;
        string name, pin;
        in >> name >> pin >> x >> y;
        assert (net_pool[i].name == name);
        net_pool[i].c2_pos = {x, y};
        net_pool[i].len_sum += 21 - y;
    }
    // Set KM cost
    const int normal = 1000;
    
    auto cost = [&] (int start_x, int start_y, int end_x, int end_y, int sec_x, int len_sum) -> int {
        int base = 0;
        if (len_sum <= 8) {
            if (abs(end_x - start_x) < 2) {
                base = 1000 - 100 * (2 - abs(end_x - start_x));
            }
            else if (abs(end_x - start_x) >= 2 && abs(end_x - start_x) <= 3) {
                base = 1000;
            }
            else {
                base = 1000 - 300 * (abs(end_x - start_x) - 3);
            }
        }
        else if (len_sum <= 14) {
            if (abs(end_x - start_x) < 2) {
                base = 1000 - 100 * (2 - abs(end_x - start_x));
            }
            else if (abs(end_x - start_x) >= 2 && abs(end_x - start_x) <= 5) {
                base = 1000;
            }
            else {
                base = 1000 - 200 * (abs(end_x - start_x) - 5);
            }
        }
        else if (len_sum <= 19) {
            if (abs(end_x - start_x) <= 2) {
                base = 1000;
            }
            else {
                base = 1000 - 100 * (abs(end_x - start_x) - 2);
            }
        }
        else {
            if (abs(end_x - start_x) <= 1) {
                base = 1000;
            }
            else {
                base = 1000 - 100 * (abs(end_x - start_x) - 1);
            }
        }
        if (abs(end_x - sec_x) <= 2) {
            base += 200;
        }
        else {
            base += (200 - 100 * (abs(end_x - sec_x) - 2));
        }
        return base + 1000;
    };
    KuhnMunkres KM(layer * lim);
    for (int i = 0 ; i < net_pool.size() ; i++) {
        for (int j = 0 ; j < layer ; j++) {
            for (int k = 0 ; k < lim ; k++) {
                KM.g[i][j * lim + k] = cost(net_pool[i].c1_pos.first, net_pool[i].c1_pos.second, k, 0, net_pool[i].c2_pos.first, net_pool[i].len_sum);
            }
        }
    }
    KM.solve();
    int suc_cnt = 0;
    for (int i = 0 ; i < layer * lim ; i++) {
        if (KM.match[i] < net_num) {
            suc_cnt++;
            net_pool[KM.match[i]].layer = i / lim;
            net_pool[KM.match[i]].c1_out = {i % lim, 0};
            escape_order[i / lim].push_back(net_pool[KM.match[i]]);
        }
    }
    assert(suc_cnt == net_num);
    auto cost2 = [&] (pair<int,int>c1_pos, pair<int,int>c1_out, pair<int,int>c2_pos, pair<int,int>c2_out, int tar) -> int {
        int len1 = abs(c1_pos.first - c1_out.first) + abs(c1_pos.second - c1_out.second);
        int len2 = abs(c2_pos.first - c2_out.first) + abs(c2_pos.second - c2_out.second);
        return abs(len1 + len2 - tar);

    };
    for (int i = 0 ; i < layer ; i++) {
        if (!escape_order[i].size()) {
            continue;
        }
        int net_num = escape_order[i].size();
        vector<int>real_ans;
        int mi = 1e9, len;
        for (int desire_len = 0 ; desire_len <= 50 ; desire_len++) {
            int dp[lim + 5][net_num + 2];
            int pick[lim + 5][net_num + 2];
            pair<int,int>target;
            target.second = 21;
            target.first = 0;
            memset(dp, 0x3f, sizeof(dp));
            memset(pick, 0, sizeof(pick));
            dp[0][0] = 0;
            dp[0][1] = cost2(escape_order[i][0].c1_pos, escape_order[i][0].c1_out, escape_order[i][0].c2_pos, target, desire_len);
            pick[0][1] = 1;
            for (int p = 1 ; p < lim ; p++) {

                for (int net = 0 ; net <= net_num ; net++) {
                    if (net == 0) {
                        dp[p][net] = 0;
                    }
                    else {
                        target.first = p;
                        if (dp[p - 1][net - 1] + cost2(escape_order[i][net - 1].c1_pos, escape_order[i][net - 1].c1_out, escape_order[i][net - 1].c2_pos, target, desire_len) < dp[p - 1][net]) {
                            pick[p][net] = 1;
                            dp[p][net] = dp[p - 1][net - 1] + cost2(escape_order[i][net - 1].c1_pos, escape_order[i][net - 1].c1_out, escape_order[i][net - 1].c2_pos, target, desire_len);
                        }
                        else {
                            dp[p][net] = dp[p - 1][net];
                        }
                    }
                }
            }
            vector<int>assign_pos;
            int d1 = lim - 1, d2 = net_num;
            while (d2) {
                if (pick[d1][d2]) {
                    assign_pos.push_back(d1);
                    d1--, d2--;
                }
                else {
                    d1--;
                }
            }
            reverse(assign_pos.begin(), assign_pos.end());
            if (dp[lim - 1][net_num] <= mi) {
                len = desire_len;
                mi = dp[lim - 1][net_num];
                real_ans = assign_pos;
                //cout << len <<' '<<mi << endl;
                for (int j = 0 ; j < (int)escape_order[i].size() ; j++) {
                    escape_order[i][j].c2_out = {real_ans[j], 21};
                    //cout << 'C'<<cost2(escape_order[i][j].c1_pos, escape_order[i][j].c1_out,escape_order[i][j].c2_pos, escape_order[i][j].c2_out, desire_len) <<' ';
                }
            }
        }
        cout << "Desire Len : "<<len <<'\n'<<"Total Cost : "<<mi << endl;
        for (int j = 0 ; j < (int)escape_order[i].size() ; j++) {
            escape_order[i][j].c2_out = {real_ans[j], 21};
            cout <<cost2(escape_order[i][j].c1_pos, escape_order[i][j].c1_out,escape_order[i][j].c2_pos, escape_order[i][j].c2_out, 0) <<' ';
        }
        cout << endl;
    }
    cout << "Finish COM2" << endl;
}
void init() {
    for (auto &i : escape_order) {
        i.clear();
    }
}
/*void output(string s) {
    string filename = s + "_com0_layer0.sat";
    for (int com = 0 ; com < 2 ; com++) {
        filename[filename.size()-12] = '0' + com;
        for (int layer = 0 ; layer < 2 ; layer++) {
            filename[filename.size()-5] = '0' + layer;
            ofstream of(filename);
            of << "###Given Auto Fan-out Format###\n";
            of << "#Net Name\n";
            of << "#Component.Pin\n";
            of << "#PIN(x,y) Fanout(x, y) layer\n";
            for (auto &net : escape_order[layer]) {
                of << net.name << '\n'<<net.pin<<'\n';
                if (com == 0) {
                    of << '('<<net.c1_pos.first<<' '<<net.c1_pos.second<<"),("<<net.c1_out.first<<' '<<net.c1_out.second<<')'<<' ' <<layer<<'\n';
                }
                else {
                    of << '('<<net.c2_pos.first<<' '<<net.c2_pos.second<<"),("<<net.c2_out.first<<' '<<net.c2_out.second<<')'<<' ' <<layer<<'\n';
                }
            }
            of << "#OBS(x,y)\n";
        }
    }
}*/
void visual() {
    for (int lay = 0 ; lay < layer ; lay++) {
        int c1[22][lim], c2[22][lim];
        memset(c1, 0, sizeof(c1));
        memset(c2, 0, sizeof(c2));
        int cnt = 0;
        for (auto &net : escape_order[lay]) {
            c1[net.c1_pos.second][net.c1_pos.first] = cnt;
            c1[net.c1_out.second][net.c1_out.first] = cnt;
            cout <<net.c1_out.first<<' '<<net.c1_out.second<<'\n';
            cout <<net.c2_out.first<<' '<<net.c2_out.second<<'\n';
            c2[net.c2_out.second][net.c2_out.first] = cnt;
            c2[net.c2_pos.second][net.c2_pos.first] = cnt++;
            //cout << net.c1_pos.first <<' '<<net.c1_pos.second<<' '<<net.c2_pos.first <<' '<<net.c2_pos.second<<'\n';
        }
        cout << "Layer " <<lay<<'\n';
        cout << "COM1\n";
        for (int i = 21 ; i >= 0 ; i--) {
            for (int j = 0 ; j < lim ; j++) {
                cout << setw(3)<<c1[i][j];
            }
            cout << '\n';
        }
        cout << "COM2\n";
        for (int i = 21 ; i >= 0 ; i--) {
            for (int j = 0 ; j < lim ; j++) {
                cout << setw(3)<<c2[i][j];
            }
            cout << '\n';
        }

    }
}
void gen_order(string s) {
    cout << "Start" <<' '<< s << endl;
    init();
    solve(s);
    cout << "Algorithm Finish" << endl;
    //output(s);
    visual();
    cout << "Successfully Output" << endl;
}
int main(){
    srand(time(NULL));
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    parse_group("case2");
    gen_order("output_ddr1");
    gen_order("output_ddr2");
}