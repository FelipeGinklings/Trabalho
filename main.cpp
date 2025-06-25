#include <iostream>

#include "arvore.h"

using namespace std;

int main() {
    bool batata = false;
    if (batata) {
        string genString = "9/6*9";
        auto [operations, firstLv1] = separateOperations(genString);
        auto newSubTree = createSubTree(operations, 0, firstLv1);
        printTree(newSubTree);
        return -1;
    }
    for (int i = 2; i <= 30; i++) {
        for (int j = 0; j < 200; j++) {
            string genString = generateExpression(i);
            string hasOperation = endInOperation(genString);
            cout << genString << endl;
            auto [operations, firstLv1] = separateOperations(genString);
            auto newSubTree = createSubTree(operations, 0, firstLv1);
        }
    }

    return 0;
}