#include "btree.h"

int main(int argc, char* argv[]) {
    if ( argc != 5 )
        return 0;
    //cout << atoi(argv[5]) << endl;
    srand(91);
    Btree tree;
    tree.fp.alpha = atof(argv[1]);
    tree.fp.ReadBlock(argv[2]);
    tree.fp.ReadNet(argv[3]);
    tree.Initialize();
    tree.SA(2000000);
    //cout << "TEST " << tree.nodes[0].left << " " << tree.nodes[0].right << endl;
    cout << "final area = " << tree.best_x*tree.best_y << "  " << tree.best_x << "  " << tree.best_y << endl;
    cout << "true area: " << tree.fp.true_area << endl;
    tree.fp.CalcHPWL(0);
    cout << "HPWL: " << tree.fp.total_HPWL << endl;
    tree.fp.OutputFile(argv[4]);

    //tree.fp.OutputBlock();
    return 0;
}
