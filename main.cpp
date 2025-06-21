#include <iostream>

#include "arvore.h"
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

int main() {
    int teste = 2;
    Tree newThree{};
    const string expression = "((5*3+2)/4-1)*((7+9)/(6-2))+8";
    if (!isValidExpression(expression)) return -1;
    buildTree(expression, newThree);
    inOrder(newThree.root);

    return 0;
}