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

// negative id
struct Order {
    int outerParenthesesOrder, insideParenthesesLevel, operationLevel, insideParenthesesOrder;
};

struct LinkOperation {
    string operation = "";
    LinkOperation *left = nullptr, *right = nullptr;
};

void teste() {
    map<Order, LinkOperation> levels{};
    // string teste = "(3+(8+6)/2)+(8*2+(5-(4*3+1)))*((5-(3*3))/3-(7+8)*2)-(2+2)";
    string teste = "(A+(B)/C/(D)+E)+(F+(G-(H)))";

    levels[{1, 1, 10, 1}] = {"A+"};
    levels[{1, 1, 10, 2}] = {"+E"};    
    levels[{1, 1, 15, 1}] = {"/C/"};    
    levels[{1, 2, 30, 1}] = {"B"};
    levels[{1, 2, 30, 2}] = {"D"};    

    // levels[{0, 0, 20, 0, 1}] = {"*"};
    // levels[{3, 3, 30, 1, 0}] = {"3*3"};
    // levels[{3, 2, 10, 1, 1}] = {"5", {nullptr, }};
    // levels[{3, 1, 15, 1, 1}] = {"3", "/", "-"};
    // levels[{3, 1, 20, 2, 1}] = {"*2"};
    // levels[{3, 2, 30, 2, 0}] = {"7+8"};

    // levels[{2, 2, 10, 1, 1}] = {"5", "-"};
    // levels[{2, 3, 30, 1, 0}] = {"4*3+1"};
    // levels[{2, 1, 10, 1, 1}] = {"8*2", "+"};

    // levels[{0, 0, 10, 0, 2}] = {"-"};

    // levels[{4, 1, 30, 1, 0}] = {"2+2"};

    // levels[{0, 0, 10, 0, 1}] = {"+"};
}

struct Operation {
    char operation = '\0';
    string left = "", right = "";

    string extraOperation = "";
    string extraNumber = "";
};

string endInOperation(string &expression) {
    const char firstChar = expression[0];
    const char lastChar = expression[expression.length() - 1];
    string operations = "";
    if (opLv1(firstChar) || opLv2(firstChar)) {
        expression = expression.substr(1, expression.size() - 1);
        operations += string(1, firstChar);
    }
    if (opLv1(lastChar) || opLv2(lastChar)) {
        expression.pop_back();
        operations += string(1, lastChar);
    }
    return operations;
}

string nextOperation(const string &expression, int &index, bool &completed) {
    string operation = expression.substr(index, 3);
    int rest = expression.length() - index;

    if (rest <= 3 || expression.length() == 3) {
        operation = expression.substr(index, rest);
        index += rest - 1;
    } else if (completed && opLv1(expression[index])) {
        operation = expression.substr(index, 1);
        completed = false;
    } else if (completed && opLv1(expression[index + 2])) {
        operation = expression.substr(index, 2);
        index++;
    } else if (completed) {
        operation = string(1, expression[index]);
        completed = false;
    } else if (opLv1(operation[1]) && opLv2(expression[index + 3])) {
        operation = expression.substr(index, 2);
        index++;
    } else {
        completed = true;
        index += 2;
    }

    return operation;
}

struct SeparateOperationsResult {
    vector<string> converted;
    int firstLv1;
};

SeparateOperationsResult separateOperations(const string &expression) {
    CircularLinkedList<string> operations{};
    string operationsLv1;
    string operationsLv2;
    int firstLv1 = -1;
    if (expression.length() < 3) return {};
    bool completed = false;
    bool lastLvIs2 = false;
    bool half = false;
    NodeC<string> *lastNode = nullptr;

    for (int index = 0; expression[index] != '\0'; index++) {
        string operation = nextOperation(expression, index, completed);

        const size_t opSize = operation.length();
        if (opSize == 2 && opLv2(operation[0]) && lastNode) {
            lastNode->data += operation;
            lastNode = nullptr;
        } else if (opSize == 3 && opLv2(operation[1]) || (lastLvIs2 && opSize == 1)) {
            operations.pushFront(operation);
            if (opSize == 3 && opLv2(operations.start->data[1])) lastNode = operations.start;
            lastLvIs2 = true;
            if (firstLv1 != -1) firstLv1++;
        } else {
            operations.pushBack(operation);
            if (firstLv1 == -1) firstLv1 = operations.size - 1;
            lastLvIs2 = false;
        }
    }

    auto converted = operations.convertToVector();

    return {converted, firstLv1};
}

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