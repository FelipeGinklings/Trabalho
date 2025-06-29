#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "arvore_sbb.h"

using namespace std;

struct Operation {
    string operation;

    // Construtores
    Operation() : operation("") {}
    Operation(const string& op) : operation(op) {}

    // Operador de igualdade
    bool operator==(const Operation& other) const { return operation == other.operation; }

    // Operador menor que (para comparar prioridade)
    bool operator<(const Operation& other) const {
        unordered_map<string, int> priority = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
        return priority[operation] < priority[other.operation];
    }

    // Operador maior que (para comparar prioridade)
    bool operator>(const Operation& other) const {
        unordered_map<string, int> priority = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
        return priority[operation] > priority[other.operation];
    }
};

struct Data {
    string value;
    bool is_operation;

    // Construtores
    Data() : value(""), is_operation(false) {}
    Data(const string& val, bool is_op = false) : value(val), is_operation(is_op) {}
};

struct ParenthesisData {
    string expression;
    string multiplications;
    string operation;
    string sums;
    AVLTree<string> tree{};
    unordered_map<string, ParenthesisData*> next_parenthesis;

    // Construtores
    ParenthesisData() : expression(""), multiplications(""), operation(""), sums("") { initializeTree(tree); }
    ParenthesisData(const string& expr, const string& mult = "", const string& op = "", const string& sum = "")
        : expression(expr), multiplications(mult), operation(op), sums(sum) {
        initializeTree(tree);
    }
};

struct ExpressionResult {
    string multiplications;
    string operation;
    string sums;

    // Construtores
    ExpressionResult() : multiplications(""), operation(""), sums("") {}
    ExpressionResult(const string& mult, const string& op, const string& sum) : multiplications(mult), operation(op), sums(sum) {}
};