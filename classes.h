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

    bool operator==(const Operation& other) const { return operation == other.operation; }

    bool operator<(const Operation& other) const {
        unordered_map<string, int> priority = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
        return priority[operation] < priority[other.operation];
    }

    bool operator>(const Operation& other) const {
        unordered_map<string, int> priority = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
        return priority[operation] > priority[other.operation];
    }

    bool operator>=(const Operation& other) const {
        unordered_map<string, int> priority = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
        return priority[operation] >= priority[other.operation];
    }

    bool operator<=(const Operation& other) const {
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
    AVLTree<string> tree{};
    // LinkedList<string> values_list = LinkedList<string>();
    unordered_map<string, ParenthesisData*> next_parenthesis;

    // Construtores
    // ParenthesisData() : expression(""), multiplications(""), operation(""), sums("") {}
    ParenthesisData() { initialize_tree(this->tree); }
    // ParenthesisData(const string& expr, const string& mult = "", const string& op = "", const string& sum = "") : expression(expr) {
    ParenthesisData(const string& expr) : expression(expr) { initialize_tree(this->tree); }
};

struct ExpressionResult {
    string multiplications;
    string operation;
    string sums;

    // Construtores
    ExpressionResult() : multiplications(""), operation(""), sums("") {}
    ExpressionResult(const string& mult, const string& op, const string& sum) : multiplications(mult), operation(op), sums(sum) {}
};

template <typename Type>
struct LinkedList {
    Node<Type>*start, *end;
    size_t size;

    LinkedList() : start(nullptr), end(nullptr), size(0) {}

    bool push_front(Type data) {
        auto* newItem = new Node<Type>;
        newItem->data = data;
        this->size += 1;
        if (this->end == nullptr) {
            this->end = newItem;
            this->start = newItem;
        } else {
            newItem->next = this->start;
            this->start = newItem;
        }
        return true;
    }

    bool push_back(Type data) {
        auto* newItem = new Node<Type>;
        newItem->data = data;
        newItem->next = this->start;
        this->size += 1;
        if (this->end == nullptr) {
            this->end = newItem;
            this->start = newItem;
        } else {
            this->end->next = newItem;
            this->end = newItem;
        }
        return true;
    }

    vector<Type> convert_to_vector() {
        vector<Type> newVector;
        Node<Type>* nav = this->start;
        while (nav != this->end) {
            newVector.push_back(nav->data);
            nav = nav->next;
        }
        newVector.push_back(this->end->data);
        return newVector;
    }
};
