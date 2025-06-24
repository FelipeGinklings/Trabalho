#include <iostream>

// #include "arvore.h"
#include "pilha.h"

using namespace std;

bool isValidExpression(const string &expression) {
    Stack<char> stackOfChars{};
    initializeStack(stackOfChars);
    bool isValid = true;
    auto convertChar = [](const char &closeChar) -> char {
        if (closeChar == '}') return '{';
        if (closeChar == ']') return '[';
        if (closeChar == ')') return '(';
        return '-';
    };

    for (int i = 0; expression[i] != '\0'; i++) {
        const char character = expression[i];
        if (character == '{' || character == '[' || character == '(')
            addToTopStack(stackOfChars, character);
        else if (character == '}' || character == ']' || character == ')') {
            if (stackOfChars.first == nullptr || convertChar(character) != removeFromTopStack(stackOfChars)) return false;
        }
    }
    if (stackOfChars.first != nullptr) isValid = false;
    return isValid;
}

void cleanArray(vector<int> vectorOfInts) { vectorOfInts.clear(); }

int main() {
    // Tree newThree{};
    // const string expression = "((5*3+2)/4-1)*((7+9)/(6-2))+8";
    // cout << "Expression: " << expression << endl;
    // if (!isValidExpression(expression)) return -1;
    // buildTree(expression, newThree);
    // inOrder(newThree.root);
    // cout << endl;
    // printTree(newThree.root);

    // const string teste = "*+-/0123456789";
    // for (auto &t : teste) cout << t * 1 << endl;

    vector<int> vectorOfInts = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    cleanArray(vectorOfInts);
    for (int &value : vectorOfInts) cout << value << endl;
    return 0;
}