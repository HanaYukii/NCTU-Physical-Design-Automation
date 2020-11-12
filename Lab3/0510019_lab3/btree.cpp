#include "btree.h"
#include <stack>
#include <cstdlib>

void Btree::Initialize() {
    const auto& size = this->fp.block_N;
    this->nodes.resize(size);
    this->root = 0;
    this->contour_root = 0;
    // initial b* tree by binary tree
    for(int n=0; n<size; n++) {
        nodes[n].left = (2*n+1 >= size) ?-1 :2*n+1;
        nodes[n].right = (2*n+2 >= size) ?-1 :2*n+2;
        nodes[n].parent = (n-1)/2;
    }
    nodes[0].parent = -1;
    this->best_cost = -1;
    this->best_x = -1;
    this->best_y = -1;
    this->r = 0.85;
    this->T = 4000000000;
}

bool Btree::IsChild(int n) {
    return this->nodes[n].left == -1 &&
           this->nodes[n].right == -1; 
}

void Btree::Compact(int test) {
    stack <int> S;
    S.push(root);
    this->contour_root = root;
    for (; !S.empty(); ) {
        //cout << "root = " << contour_root;
        int n = S.top();
        S.pop(); 
        auto& node = this->nodes[n];
        if (node.right!=-1)  S.push(node.right); 
        if (node.left!=-1)  S.push(node.left);   
        //cout << "  module: " << n << endl;
        this->PlaceModule(n, node.parent);
        //PrintContour();
        if (test)
            cin >> n;
    }
}

void Btree::PlaceModule(int n, int abut) {
    auto& blocks = this->fp.blocks;
    auto& module_n = blocks[n];
    int contour_n = n;
    int width = (nodes[n].rotate) ?module_n.height :module_n.width;
    int height = (nodes[n].rotate) ?module_n.width :module_n.height;
    if (abut == -1) {   //root
        module_n.x = 0; module_n.y = 0;
        module_n.rx = module_n.x+width;
        module_n.ry = module_n.y+height;
        return;
    }
    bool is_left = (this->nodes[abut].left == n);
    auto& module_p = blocks[abut];
    if (is_left) {
        module_n.x = module_p.rx;
        module_n.rx = module_n.x+width;
        if (nodes[abut].contour.front == -1) {
            // no previous block at x axis
            module_n.y = 0;           
            module_n.ry = module_n.y+height;   
            // update contour
            nodes[abut].contour.front = n;
            nodes[n].contour.back = abut;
            nodes[n].contour.front = -1;
            //PrintContour();
            return;
        } 
        else {                     
            nodes[n].contour.back = abut;
            nodes[n].contour.front = nodes[abut].contour.front;
            int next_i = nodes[n].contour.front;
            nodes[next_i].contour.back = n;
            nodes[abut].contour.front = n;
        }
    } else {
        module_n.x = module_p.x;
        module_n.rx = module_n.x+width;       
        if (nodes[abut].contour.back == -1) {
            contour_root = n;
            nodes[n].contour.back = -1;
            nodes[n].contour.front = abut;
            nodes[abut].contour.back = n;
        } else {
            int p_back = nodes[abut].contour.back;
            nodes[p_back].contour.front = n;
            nodes[n].contour.back = p_back;
            nodes[n].contour.front = abut;
            nodes[abut].contour.back = n;
        }
    }   
    // updata contour
    int min_y = -1;
    contour_n = nodes[n].contour.front;
    for ( ; contour_n != -1; contour_n = nodes[contour_n].contour.front) {
        min_y = max(min_y, blocks[contour_n].ry);
        if (blocks[contour_n].rx >= module_n.rx) {
            if (blocks[contour_n].rx == module_n.rx ) {
                int next_i = nodes[contour_n].contour.front;
                nodes[n].contour.front = next_i;
                if (next_i != -1)
                    nodes[next_i].contour.back = n;
            } else {
                nodes[n].contour.front = contour_n;
                nodes[contour_n].contour.back = n;
            }
            break;
        }
    }
    
    if (contour_n == -1)
        nodes[n].contour.front = -1;
    blocks[n].y = min_y;
    module_n.ry = module_n.y+height;
    //PrintContour();
}

int Btree::GetArea() {
    auto& blocks = this->fp.blocks;
    int area_x = -1, area_y = -1;
    for (int n = this->contour_root; n != -1; n = nodes[n].contour.front) {
        area_x = blocks[n].rx;
        area_y = max(area_y, blocks[n].ry);
    }
    this->tmp_x = area_x;
    this->tmp_y = area_y;
    fp.area_x = area_x;
    fp.area_y = area_y;
    int panalty_base = 0;
    int penalty_x = (this->fp.outline_x-area_x < 0) ?this->fp.outline_x-area_x+panalty_base :0;
    int penalty_y = (this->fp.outline_y-area_y < 0) ?this->fp.outline_y-area_y+panalty_base :0;
    fp.CalcHPWL(0);
    if (penalty_x<0 && penalty_y<0) {
        this->tmp_cost = area_x*area_y - fp.outline_x*fp.outline_y;
    }
    else if (penalty_x<0) {
        this->tmp_cost = area_y*(area_x-fp.outline_x);
    }
    else if (penalty_y<0) {
        this->tmp_cost = area_x*(area_y-fp.outline_y);
    }
    else {
        this->tmp_cost = 0; 
    }
    //this->tmp_cost += area_x*area_y*0.1;
    //this->tmp_cost = area_x*area_y +(penalty_x*penalty_x+penalty_y*penalty_y)*1000 + fp.total_HPWL*0.001;
    //this->tmp_cost += fp.total_HPWL*0.001;
    return area_x*area_y;
}

void Btree::PrintContour() {
    int contour_n = this->contour_root;
    cout << "Contour: ";
    for ( ; contour_n != -1; contour_n = nodes[contour_n].contour.front) {
        cout << contour_n << " ";
    }
    cout << endl;
}

void Btree::PrintContourTest() {
    int contour_n = this->contour_root;
    cout << "Contour: ";
    for ( ; contour_n != -1; contour_n = nodes[contour_n].contour.front) {
        cout << contour_n << " ";
        int a;
        cin >> a;
    }
    cout << endl;
}

void Btree::SwapNode(int n1, int n2) {
    if ( this->root == n1 )
        this->root = n2;
    else if ( this->root == n2 )
        this->root = n1;
    if (nodes[n1].left != -1) 
        nodes[nodes[n1].left].parent = n2;
    if (nodes[n1].right != -1) 
        nodes[nodes[n1].right].parent = n2;
    if (nodes[n2].left != -1) 
        nodes[nodes[n2].left].parent = n1;
    if (nodes[n2].right != -1) 
        nodes[nodes[n2].right].parent = n1;
    if (nodes[n1].parent != -1) {
        int p = nodes[n1].parent;
        if(nodes[p].left == n1) 
            nodes[p].left = n2;
        else    
            nodes[p].right = n2;
    }
    if (nodes[n2].parent != -1) {
        int p = nodes[n2].parent;
        if(nodes[p].left == n2) 
            nodes[p].left = n1;
        else    
            nodes[p].right = n1;
    }
    Swap(nodes[n1].left, nodes[n2].left);
    Swap(nodes[n1].right, nodes[n2].right);
    Swap(nodes[n1].parent, nodes[n2].parent);
}

void Btree::SwapNodeFamily(int n1, int n2) {
    bool is_left = nodes[n1].left == n2 ;
    if ( this->root == n1 )
        this->root = n2;
    else if ( this->root == n2 )
        this->root = n1;
    if (is_left) {
        // change right
        if (nodes[n1].right != -1) 
            nodes[nodes[n1].right].parent = n2;
        if (nodes[n2].right != -1) 
            nodes[nodes[n2].right].parent = n1;
        Swap(nodes[n1].right, nodes[n2].right);
        // change left
        if (nodes[n2].left != -1) 
            nodes[nodes[n2].left].parent = n1;
        nodes[n1].left = nodes[n2].left;
        nodes[n2].left = n1;

    } else {
        // change left
        if (nodes[n1].left != -1) 
             nodes[nodes[n1].left].parent = n2;
        if (nodes[n2].left != -1) 
             nodes[nodes[n2].left].parent = n1;
        Swap(nodes[n1].left, nodes[n2].left);
        // change right
        if (nodes[n2].right != -1) 
            nodes[nodes[n2].right].parent = n1;
        nodes[n1].right = nodes[n2].right;
        nodes[n2].right = n1;
    }
    // change parent
    if (nodes[n1].parent != -1) {
        int p = nodes[n1].parent;
        if(nodes[p].left == n1) 
            nodes[p].left = n2;
        else    
            nodes[p].right = n2;
    }
    nodes[n2].parent = nodes[n1].parent;
    nodes[n1].parent = n2;

}

//insert n1 to n2.child
//void Btree::InsertNode(int n1, int n2) {
//    bool left = rand()%2;
//    if (left) { //insert to n2.left
//        
//    } else {
//
//    }
//
//}

void Btree::RotateNode(int n) {
    this->nodes[n].rotate = !this->nodes[n].rotate;
}

void Btree::SA(int iteration) {
    // do first time
    this->Compact(0);
    cout << "area:" << this->GetArea() << endl;
    cout << "true area: " << this->fp.true_area << endl;
    // record best
    this->best_root = this->root;
    this->best_nodes = this->nodes;
    //PrintTree(this->root);
    int n1, n2;
    for (int n=0; n<iteration; n++) {
        //cout << "itr = " << n << endl;
        double func = (double)rand()/RAND_MAX; // >0.1:swap, <0.1:rotate
        if (func<=0.3) { // rotate
             n1 = rand()%this->fp.block_N;
            this->RotateNode(n1);
        }
        if (func>=0.50) {
            do{
                n1 = rand()%this->fp.block_N;
                n2 = rand()%this->fp.block_N;
            } while(n1==n2);
            //cout << n1 << " " << n2 << endl;
            if (nodes[n1].parent == n2 || nodes[n2].parent == n1) {
                if (nodes[n1].parent == n2)
                    this->SwapNodeFamily(n2, n1);
                else 
                    this->SwapNodeFamily(n1, n2);
                //PrintTree(this->root);
            } else {
                this->SwapNode(n1, n2);
            }
        }
        else {
            do{
                n1 = rand()%this->fp.block_N;
                n2 = rand()%this->fp.block_N;
            } while(n1==n2);
            DeleteNode(n1);
            InsertNode(n2,n1);
        }
        //cout << "after tree\n";
        //PrintTree(this->root);
        this->ResetContour();
        this->Compact(0);  
        //fp.OutputBlock();     
        this->GetArea();
        //PrintContour();
        //cout << (double)rand()/RAND_MAX << " " << exp(-1*(double)tmp_cost/T) << endl;
        //int a;
        //cin  >> a;
        bool jump = (double)rand()/RAND_MAX < exp(-1*(double)tmp_cost/T);
        if (jump)
            cout << jump << endl;
        if (this->best_cost>this->tmp_cost || jump) {
            cout << "\nBetter   " << this->best_cost-this->tmp_cost<< "\n\n";
            this->UpdateBest();
        }
        else {
            this->RecoverBest();
        }
        T = this->r*T;
        if (best_cost == 0)
            break;
    }
    this->ResetContour();
    this->Compact(0);
    this->GetArea();
}

void Btree::UpdateBest() {
    this->best_cost = this->tmp_cost;
    this->best_x = this->tmp_x;
    this->best_y = this->tmp_y;
    this->best_root = this->root;
    this->best_nodes = this->nodes;
}

void Btree::ResetContour() {
    for (auto& c:this->nodes) {
        c.contour.front = -1;
        c.contour.back = -1;
    }
}

void Btree::RecoverBest() {
    this->nodes = this->best_nodes;
    this->root = this->best_root;
}

void Btree::PrintTree(int n) {
    if (n == -1) return;
    //int a;
    //cin >> a;
    cout << n << ":(" << nodes[n].left << "," << nodes[n].right << ") p:" << nodes[n].parent << "\n";
    PrintTree(nodes[n].left);
    PrintTree(nodes[n].right);
}

void Btree::DeleteNode(int n){
    int child    = -1;	// pull which child
    int subchild = -1;   // child's subtree
    int subparent= -1; 
    Node& node = nodes[n];
    if(!this->IsChild(n)){
        bool left = rand()%2;			// choose a child to pull up
        if(node.left ==-1) left=false;
        if(node.right==-1) left=true;
  
  
        if(left){
            child = node.left;			// child will never be NIL
            if(node.right!=-1){
                subchild  = nodes[child].right;
                subparent = node.right;
                nodes[node.right].parent = child; 
                nodes[child].right = node.right;	// abut with node's another child
            }
        }
        else{
            child = node.right;
            if(node.left!=-1){
                subchild  = nodes[child].left;
                subparent = node.left;
  	            nodes[node.left].parent = child;
                nodes[child].left = node.left;
            }
        }
        //add_changed_nodes(subchild);
        nodes[child].parent = node.parent;
    }
  
    if(node.parent == -1){			// root
        //    changed_root = nodes_root;
        root = child;
    }else{					// let parent connect to child
        //add_changed_nodes(node.parent);
        if(n == nodes[node.parent].left)
            nodes[node.parent].left  = child;
        else
            nodes[node.parent].right = child;
    }
  
    // place subtree
    if(subchild != -1){
        Node &sc = nodes[subchild];
  
        while(1){
            Node &p = nodes[subparent];
  
            if(p.left==-1 || p.right==-1){
  	            sc.parent = subparent;
                if(p.left==-1) p.left = subchild;
                else p.right = subchild;
            break;
            } else{
  	            subparent = (rand()%2 ? p.left : p.right);
            }
        }
    }
}

void Btree::InsertNode(int p, int n){
  Node& parent = nodes[p];
  Node& node = nodes[n];
  node.parent = p;
  bool edge = rand()%2;

  if(edge){
    //add_changed_nodes(parent.left);
    node.left  = parent.left;
    node.right = -1;
    if(parent.left!=-1)
      nodes[parent.left].parent = n;

    parent.left = n;

  }else{
    //add_changed_nodes(parent.right);
    node.left  = -1;
    node.right = parent.right;
    if(parent.right!=-1)
      nodes[parent.right].parent = n;
    
    parent.right = n;
  }
}

