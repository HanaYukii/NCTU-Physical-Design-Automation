#include "fp.h"

FloorPlan::FloorPlan() {
    start = clock();
}

void FloorPlan::ReadBlock(char* inputfile) {
    this->true_area = 0;
    fstream fs;
    fs.open(inputfile, std::fstream::in);
    string tmp, tmp2, tmp3;
    // read outline
    fs >> tmp >> tmp2 >> tmp3;
    this->outline_x = stoi(tmp2);
    this->outline_y = stoi(tmp3);
    fs >> tmp >> tmp2;
    this->block_N = stoi(tmp2);
    fs >> tmp >> tmp2;
    this->terminal_N = stoi(tmp2);
    // read blocks
    for(int n = 0; n < this->block_N; n++) {
        string name, w, h;
        fs >> name >> w >> h;
        true_area += stoi(w)*stoi(h);
        this->block_map[name] = block_map.size()-1;
        this->blocks.emplace_back(stoi(w), stoi(h), name);
    }
    // read terminals
    for(int n = 0; n < this->terminal_N; n++) {
        string name, x, y;
        fs >> name >> tmp >> x >> y;
        //cout << name << endl;
        this->block_map[name] = block_map.size()-1;
        this->terminals.emplace_back(stoi(x),stoi(y));
    }
    // for( auto it = this->block_map.begin(); it != this->block_map.end(); ++it )
    // {
    //   string key = it->first;
    //   int value = it->second;
    //   cout << "key " <<key << " " << value << endl;
    // }
    fs.close();
}

void FloorPlan::ReadNet(char* inputfile) {
    fstream fs;
    fs.open(inputfile, std::fstream::in);
    string tmp, tmp2;
    // read outline
    fs >> tmp >> tmp2;
    this->net_N = stoi(tmp2);
    this->nets.resize(this->net_N);
    for (int n = 0; n < this->net_N; n++) {
        auto& net = this->nets[n];
        fs >> tmp >> tmp2;
        net.degree = stoi(tmp2);
        net.pins.resize(net.degree);
        for (int d = 0; d < net.degree; d++) {
            fs >> tmp;
            net.pins[d] = this->block_map[tmp];
        }
    }
    fs.close();
}

void FloorPlan::CalcHPWL(bool test) {
    this->total_HPWL = 0;
    int n = 0;
    for (const auto& net:this->nets) {
        int min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;
        for (const auto& pin:net.pins) {
            int block_n;
            int n = pin;
            if(test) {
                cout << n << " (" << blocks[n].x << ", " << blocks[n].y << ") (" 
                << blocks[n].rx << ", " << blocks[n].ry << ")\n";
            }
            if (pin >= this->block_N) {
                block_n = pin - this->block_N;
                max_x = max(this->terminals[block_n].x, max_x);
                min_x = min(this->terminals[block_n].x, min_x);
                max_y = max(this->terminals[block_n].y, max_y);
                min_y = min(this->terminals[block_n].y, min_y);
            }
            else {
                block_n = pin;
                int mid_x = (this->blocks[block_n].x+this->blocks[block_n].rx)/2;
                int mid_y = (this->blocks[block_n].y+this->blocks[block_n].ry)/2;
                max_x = max(mid_x, max_x);
                min_x = min(mid_x, min_x);
                max_y = max(mid_y, max_y);
                min_y = min(mid_y, min_y);
                if (test) {
                    cout << block_n << " " << mid_x << "," << mid_y << endl;    
                }
            }
        }
        this->total_HPWL += max_x-min_x+max_y-min_y;
        if(test)
            cout << "now WL: " << this->total_HPWL << endl;
    }
    if(test) {
        cout << "test: ";
        OutputBlock();
    }
}

void FloorPlan::OutputBlock() {
    
    for (int n = 0; n < this->block_N; n++) {
        cout << n << " (" << blocks[n].x << ", " << blocks[n].y << ") (" 
        << blocks[n].rx << ", " << blocks[n].ry << ")\n";
    }
}

void FloorPlan::OutputFile(char* filename) {
    fstream fs;
    fs.open(filename, std::fstream::out);
    int cost = alpha*area_x*area_y + (1-alpha)*total_HPWL;
    fs << cost << endl;
    fs << total_HPWL << endl;
    fs << area_x*area_y << endl;
    fs << area_x << "\t" << area_y << endl;
    double cpu_time_used = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    fs << cpu_time_used << endl;
    for (const auto& b:this->blocks) {
        fs << b.name << "\t" << b.x << "\t" << b.y << "\t" << b.rx << "\t" << b.ry << endl;
    }
    fs.close();
}

void Swap(int& a, int& b){
    int tmp = a;
    a = b;
    b = tmp; 
}
