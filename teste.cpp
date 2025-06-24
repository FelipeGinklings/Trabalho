#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

bool opLv1(const char &c) { return c == '-' || c == '+'; }
bool opLv2(const char &c) { return c == '*' || c == '/'; }

vector<string> createOperations(const string &expresion) {
    vector<string> operations;
    bool completed = false;
    bool half = false;
    for (int i = 0; expresion[i] != '\0'; i++) {
        string operation = expresion.substr(i, 3);
        int rest = expresion.length() - i + 1;
        if (rest <= 3) {
            operation = expresion.substr(i, rest);
            i += rest - 1;
        } else if (completed && opLv1(expresion[i])) {
            operation = expresion.substr(i, 1);
            completed = false;
        } else if (completed) {
            operation = expresion.substr(i, 2);
            i++;
        } else if (opLv1(operation[1]) && opLv2(expresion[i + 3])) {
            operation = expresion.substr(i, 2);
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

int main() {
    
}
