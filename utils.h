#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "circularChainedList.hpp"
#include "pilha.h"

using namespace std;

bool opLv1(const char &c) { return c == '-' || c == '+'; }
bool opLv2(const char &c) { return c == '*' || c == '/'; }

bool areEqual(vector<string> &strings, string &stringForTest) {
    string mergedString = "";
    for (const string &str : strings) mergedString += str;
    return stringForTest.length() == mergedString.length();
}

string generateExpression(int size) {
    if (size <= 0) return "";

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> numDist(1, 9);
    uniform_int_distribution<> opDist(0, 3);

    string expression = to_string(numDist(gen));

    char operators[] = {'+', '-', '*', '/'};

    for (int i = 1; i < size; i++) {
        expression += operators[opDist(gen)];
        expression += to_string(numDist(gen));
    }

    return expression;
}

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