#ifndef _FP_H
#define _FP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <climits>
#include <ctime>
using namespace std;

class Block {
public:
    Block(int w, int h, string name):width(w), height(h), name(name){}
    int x, y, rx, ry;
    int width, height;
    string name;
};

class Terminal {
public:
    Terminal(int x, int y):x(x), y(y){}
    int x, y;
};

class Net {
public:
    int degree;
    vector<int> pins;
};

class FloorPlan {
public:
    double alpha;
    int outline_x, outline_y;
    int net_N, block_N, terminal_N;
    int true_area;
    int total_HPWL;
    int area_x, area_y;
    vector<Block> blocks;
    vector<Terminal> terminals;
    vector<Net> nets;
    clock_t  start;
    map<string, int> block_map;
//---------function--------------------------
    FloorPlan();
    void ReadBlock(char* filename);
    void ReadNet(char* filename);
    void CalcHPWL(bool test);
    void OutputBlock();
    void OutputFile(char* filename);
};

void Swap(int& a, int& b);

#endif