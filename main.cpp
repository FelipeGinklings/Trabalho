#include <iostream>

#include "arvore.h"

using namespace std;

int main() {
    int teste = 2;
    Tree newThree{};
    const string operation = "((5*3+2)/4-1)*((7+9)/(6-2))+8";
    buildTree(operation, newThree);
    inOrder(newThree.root);

    return 0;
}