#include "bstOld.hpp"

using namespace std;

namespace BST {

struct CharItem {
    char c;
    int key() { return c; }
};

std::ostream& operator << (std::ostream& os, const CharItem& i) {
    os << i.c;
    return os;
}

void testBST()
{
    CharItem C[] { {'E'}, {'D'}, {'H'}, {'B'}, {'F'}, {'A'}, {'C'}, {'G'} };
//    CharItem C[] { {'D'}, {'B'}, {'F'}, {'A'}, {'C'}, {'E'}, {'G'} };
//    CharItem C[] { {'A'}, {'B'}, {'C'}, {'D'}, {'E'}, {'F'}, {'G'} };
//    CharItem C[] { {'A'}, {'G'}, {'B'}, {'F'}, {'C'}, {'E'}, {'D'} };
//    CharItem C[] { {'D'}, {'E'}, {'C'}, {'F'}, {'B'}, {'G'}, {'A'} };
    const int N = sizeof(C)/sizeof(*C);
    
    BSTree<CharItem> tree;
    assert(tree.count() == 0);
    tree.insert(C, N);
    assert(tree.count() == N);
    tree.show();
    cout << " count = " << tree.count() << endl;
    cout << " height = " << tree.height() << endl;
    
    tree.remove('D');
    assert(tree.count() == N - 1);
    tree.show();
    
    tree.remove('F');
    assert(tree.count() == N - 2);
    tree.show();

    tree.remove('B');
    assert(tree.count() == N - 3);
    tree.show();

    tree.remove('E');
    assert(tree.count() == N - 4);
    tree.show();
    
    tree.remove('C');
    assert(tree.count() == N - 5);
    tree.show();

    tree.remove('A');
    assert(tree.count() == N - 6);
    tree.show();
    
    tree.remove('G');
    assert(tree.count() == N - 7);
    tree.show();
    cout << " count = " << tree.count() << endl;
    cout << " height = " << tree.height() << endl;
    
    tree.removeAll();
}

}