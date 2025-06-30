#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "arvore_sbb.h"
#include "linked_list.hpp"

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
    int depth;
    bool operation;

    // Construtores
    Data() : value(""), operation(false) {}
    Data(const string& val, int new_depth, bool is_op = false) : value(val), depth(new_depth), operation(is_op) {}

    Operation get_operation() { return Operation(this->value); }
};

struct ParenthesisData {
    string expression = "";
    // string multiplications;
    // string operation;
    // string sums;
    // AVLTree<string> tree{};
    // LinkedList<string> values_list = LinkedList<string>();
    unordered_map<string, ParenthesisData*> next_parenthesis;

    // Construtores
    // ParenthesisData() : expression(""), multiplications(""), operation(""), sums("") {}
    ParenthesisData() {}
    // ParenthesisData(const string& expr, const string& mult = "", const string& op = "", const string& sum = "") : expression(expr) {
    ParenthesisData(const string& expr) : expression(expr) {
        // initializeTree(tree);
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