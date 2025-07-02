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
    string _convert(double value, int precision = 17) const {
        ostringstream oss;
        oss << setprecision(precision) << value;
        return oss.str();
    }

    double _convert(const string &str) const {
        try {
            return stod(str);
        } catch (const invalid_argument &e) {
            throw invalid_argument("Invalid double string: " + str);
        } catch (const out_of_range &e) {
            throw out_of_range("Double out of range: " + str);
        }
    }

    double apply(string a, string b) const {
        if (operation == "+") return this->_convert(a) + this->_convert(b);
        if (operation == "-") return this->_convert(a) - this->_convert(b);
        if (operation == "*") return this->_convert(a) * this->_convert(b);
        if (operation == "/") {
            if (b == "0") throw invalid_argument("Division by zero");
            return this->_convert(a) / this->_convert(b);
        }
        throw invalid_argument("Unknown operation: " + operation);
    }

    double apply(double a, string b) const {
        if (operation == "+") return a + this->_convert(b);
        if (operation == "-") return a - this->_convert(b);
        if (operation == "*") return a * this->_convert(b);
        if (operation == "/") {
            if (b == "0") throw invalid_argument("Division by zero");
            return a / this->_convert(b);
        }
        throw invalid_argument("Unknown operation: " + operation);
    }

    double apply(string a, double b) const {
        if (operation == "+") return this->_convert(a) + b;
        if (operation == "-") return this->_convert(a) - b;
        if (operation == "*") return this->_convert(a) * b;
        if (operation == "/") {
            if (b == 0) throw invalid_argument("Division by zero");
            return this->_convert(a) / b;
        }
        throw invalid_argument("Unknown operation: " + operation);
    }
};

struct Data {
    string value;
    bool is_parenthesis;
    bool is_mul;
    bool operation;

    Data() : value(""), is_parenthesis(false), is_mul(false), operation(false) {}
    Data(const string &val, bool is_op = false, bool is_parenthesis = false, bool is_mul = false) : value(val), is_parenthesis(is_parenthesis), is_mul(is_mul), operation(is_op) {}

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
