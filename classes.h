#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;

template <typename TYPE>
struct Node {
    int key;
    TYPE data;
    char orientL;
    char orientR;
    Node *left, *right;
};

template <typename TYPE>
struct AVLTree {
    Node<TYPE> *root;
};

struct Operation {
    string operation;

    // Construtores
    Operation() : operation("") {}
    Operation(const string &op) : operation(op) {}

    bool operator==(const Operation &other) const { return operation == other.operation; }

    bool operator<(const Operation &other) const {
        unordered_map<string, int> priority = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
        return priority[operation] < priority[other.operation];
    }

    bool operator>(const Operation &other) const {
        unordered_map<string, int> priority = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
        return priority[operation] > priority[other.operation];
    }

    bool operator>=(const Operation &other) const {
        unordered_map<string, int> priority = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
        return priority[operation] >= priority[other.operation];
    }

    bool operator<=(const Operation &other) const {
        unordered_map<string, int> priority = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
        return priority[operation] <= priority[other.operation];
    }

    // Function to apply the operation to two numbers
    double apply(double a, double b) const {
        if (operation == "+") return a + b;
        if (operation == "-") return a - b;
        if (operation == "*") return a * b;
        if (operation == "/") {
            if (b == 0) throw invalid_argument("Division by zero");
            return a / b;
        }
        throw invalid_argument("Unknown operation: " + operation);
    }
    double apply(string a, string b) const {
        if (operation == "+") return stod(a) + stod(b);
        if (operation == "-") return stod(a) - stod(b);
        if (operation == "*") return stod(a) * stod(b);
        if (operation == "/") {
            if (b == "0") throw invalid_argument("Division by zero");
            return stod(a) / stod(b);
        }
        throw invalid_argument("Unknown operation: " + operation);
    }

    double apply(double a, string b) const {
        if (operation == "+") return a + stod(b);
        if (operation == "-") return a - stod(b);
        if (operation == "*") return a * stod(b);
        if (operation == "/") {
            if (b == "0") throw invalid_argument("Division by zero");
            return a / stod(b);
        }
        throw invalid_argument("Unknown operation: " + operation);
    }

    double apply(string a, double b) const {
        if (operation == "+") return stod(a) + b;
        if (operation == "-") return stod(a) - b;
        if (operation == "*") return stod(a) * b;
        if (operation == "/") {
            if (b == 0) throw invalid_argument("Division by zero");
            return stod(a) / b;
        }
        throw invalid_argument("Unknown operation: " + operation);
    }
};

struct Data {
    string value;
    bool is_parenthesis;
    bool operation;

    Data() : value(""), operation(false) {}
    Data(const string &val, bool is_op = false, bool is_parenthesis = false) : value(val), is_parenthesis(is_parenthesis), operation(is_op) {}

    Operation get_operation() { return Operation(this->value); }

    friend ostream &operator<<(ostream &os, const Data &data) {
        os << data.value;
        return os;
    }
};

struct ParenthesisData {
    string expression = "";
    AVLTree<Data> tree{};
    unordered_map<string, ParenthesisData *> next_parenthesis;

    ParenthesisData() { tree.root = nullptr; }
    ParenthesisData(const string &expr) : expression(expr) { tree.root = nullptr; }
};

struct ExpressionResult {
    string multiplications;
    string operation;
    string sums;

    ExpressionResult() : multiplications(""), operation(""), sums("") {}
    ExpressionResult(const string &mult, const string &op, const string &sum) : multiplications(mult), operation(op), sums(sum) {}
};
