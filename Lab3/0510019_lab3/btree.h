#ifndef _BTREE_H
#define _BTREE_H

#include <cmath>
#include "fp.h"
using namespace std;

class Contour {
public:
    Contour() {front = -1, back = -1;}
    int front, back;    
};

class Node {
public:
    int left, right, parent;
    bool rotate;
    Contour contour;
};

class Btree {
public:
    vector<Node> nodes;
    vector<Node> best_nodes;
    int root;
    FloorPlan fp;
    int contour_root;
    int best_root;
    unsigned int best_cost, best_x, best_y, tmp_cost, tmp_x, tmp_y;
    double r, T;
    //----function--------------
    
    void Initialize();
    bool IsChild(int n);
    void Compact(int test);
    void PlaceModule(int n, int abut);
    int GetArea();
    void PrintContour();
    void PrintContourTest();
    void SwapNode(int n1, int n2);
    void SwapNodeFamily(int n1, int n2);
    void InsertNode(int n1, int n2);
    void SA(int iteration);
    void UpdateBest();
    void ResetContour();
    void RecoverBest();
    void PrintTree(int n);
    void RotateNode(int n);
    void DeleteNode(int n);
};



#endif
