#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "pilha.h"

using namespace std;

bool opLv1(const char &c) { return c == '-' || c == '+'; }
bool opLv2(const char &c) { return c == '*' || c == '/'; }

struct Order {
    string outerParenthesesLevel;
    string insideParenthesesLevel;
    string insideParenthesesOrder;
    string operationOrder;
    string operationOrderNumber;
};

struct LinkOperation {
    string operation;
    int left = 0, right = 0;
};

void teste() {
    map<int, LinkOperation, greater<int>> levels;
    string teste = "(3+(8+6)/2)+(8*2+(5-(4*3+1)))*((5-(3*3))/3-(7+8)*2)";
    levels[33121] = {"3*3"};
    levels[32211] = {"7+8"};
    levels[32111] = {"5-", 0, 33121};
    levels[31122] = {"/3", 32111};
    levels[31121] = {"*2", 32211};
    levels[31111] = {"-", 31122, 31121};
    levels[23121] = {"4*3"};
    levels[23111] = {"+1", 23121};
    levels[22111] = {"5-", 0, 23111};
    levels[21121] = {"8*2"};
    levels[21111] = {"+", 21121, 22111};
    levels[12111] = {"8+6"};
    levels[11121] = {"/2", 12111};
    levels[11111] = {"3+", 0, 11121};
    levels[223] = {"*", 11111, 21111};
    levels[112] = {"+", 212, 21110};

    for (const auto &[key, value] : levels) {
        cout << "Key: " << key << " Operation: " << value.operation << " Left: " << value.left << " Right: " << value.right << endl;
    }
}

vector<string> separateOperations(const string &expression) {
    vector<string> operations;
    if (expression.length() < 3) return operations;
    bool completed = false;
    bool half = false;
    for (int i = 0; expression[i] != '\0'; i++) {
        string operation = expression.substr(i, 3);
        int rest = expression.length() - i;
        if (rest <= 3 || expression.length() == 3) {
            operation = expression.substr(i, rest);
            i += rest - 1;
        } else if (completed && opLv1(expression[i])) {
            operation = expression.substr(i, 1);
            completed = false;
        } else if (completed) {
            operation = expression.substr(i, 2);
            i++;
        } else if (opLv1(operation[1]) && opLv2(expression[i + 3])) {
            operation = expression.substr(i, 2);
            i++;
        } else {
            completed = true;
            i += 2;
        }
        operations.push_back(operation);
    }

    return operations;
}

bool areEqual(vector<string> &strings, string &stringForTest) {
    string mergedString = "";
    for (const string &str : strings) mergedString += str;
    return stringForTest == mergedString;
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