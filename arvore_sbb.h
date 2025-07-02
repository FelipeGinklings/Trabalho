#ifndef SBB_TREE_HPP
#define SBB_TREE_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
using namespace std;

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

    // Construtores
    Data() : value(""), operation(false) {}
    Data(const string &val, bool is_op = false, bool is_parenthesis = false) : value(val), is_parenthesis(is_parenthesis), operation(is_op) {}

    Operation get_operation() { return Operation(this->value); }

    // Operator overload for cout
    friend ostream &operator<<(ostream &os, const Data &data) {
        os << data.value;
        return os;
    }
};

constexpr char VERTICAL = 'v';
constexpr char HORIZONTAL = 'h';

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

template <typename TYPE>
void initialize_tree(AVLTree<TYPE> &t) {
    t.root = nullptr;
}

template <typename TYPE>
void LL(Node<TYPE> *&root) {
    auto m = root;
    auto h = m->left;
    m->left = h->right;
    h->orientL = VERTICAL;
    m->orientL = VERTICAL;
    h->right = m;
    root = h;
}

template <typename TYPE>
void RR(Node<TYPE> *&root) {
    auto f = root;
    auto h = f->right;
    f->right = h->left;
    h->orientR = VERTICAL;
    f->orientR = VERTICAL;
    h->left = f;
    root = h;
}

template <typename TYPE>
void RL(Node<TYPE> *&root) {
    auto f = root;
    auto m = f->right;
    auto h = m->left;
    m->orientL = VERTICAL;
    f->orientR = VERTICAL;
    f->right = h->left;
    m->left = h->right;
    h->right = m;
    h->left = f;
    root = h;
}

template <typename TYPE>
void LR(Node<TYPE> *&root) {
    auto m = root;
    auto f = m->left;
    auto h = f->right;
    f->orientR = VERTICAL;
    m->orientL = VERTICAL;
    f->right = h->left;
    m->left = h->right;
    h->right = m;
    h->left = f;
    root = h;
}

template <typename TYPE>
int insert(AVLTree<TYPE> &tree, int key, TYPE data) {
    return insert(tree.root, key, data);
}

template <typename TYPE>
int insert(Node<TYPE> *&node, int key, TYPE data) {
    int n = 0;
    if (node == nullptr) {
        node = new Node<TYPE>;
        node->data = data;
        node->key = key;
        node->left = nullptr;
        node->right = nullptr;
        return 1;
    }
    if (key > node->key) {
        n = insert(node->right, key, data);
        if (n == 1) {
            node->orientR = HORIZONTAL;
            n++;
        } else {
            if (n == 2 && node->orientR == HORIZONTAL) {
                n = 1;
                if (node->right->orientR == HORIZONTAL) {
                    RR(node);
                } else {
                    RL(node);
                }
            } else {
                n = 0;
            }
        }
    }
    if (key < node->key) {
        n = insert(node->left, key, data);
        if (n == 1) {
            node->orientL = HORIZONTAL;
            n++;
        } else {
            if (n == 2 && node->orientL == HORIZONTAL) {
                n = 1;
                if (node->left->orientL == HORIZONTAL) {
                    LL(node);
                } else {
                    LR(node);
                }
            } else {
                n = 0;
            }
        }
    }
    return n;
}

template <typename TYPE>
void in_order(AVLTree<TYPE> tree) {
    in_order(tree.root);
}

template <typename TYPE>
void in_order(Node<TYPE> *node) {
    if (node != nullptr) {
        in_order(node->left);
        cout << node->data;
        in_order(node->right);
    }
}

void calculate_tree(Node<Data> *node, vector<string> &operations) {
    if (node != nullptr) {
        if (node->data.is_parenthesis) {
            node->data.is_parenthesis = false;
            vector<string> new_operations;
            calculate_tree(node, new_operations);
            node->data.value = new_operations[0];
            cout << "Parenthesis result: " << node->data.value << endl;
            node->data.is_parenthesis = true;
        } else
            calculate_tree(node->left, operations);

        operations.push_back(node->data.value);
        if (operations.size() == 3) {
            double left = stod(operations[0]);
            Operation op = Operation(operations[1]);
            double right = stod(operations[2]);
            double result = op.apply(left, right);
            cout << "Calculating: " << left << " " << op.operation << " " << right << " = " << result << endl;
            operations.clear();
            operations.push_back(to_string(result));
        }
        if (!node->data.is_parenthesis)
            calculate_tree(node->right, operations);
        else
            node->data.is_parenthesis = false;
    }
}

double calculate_tree(AVLTree<Data> tree) {
    vector<string> operations;
    calculate_tree(tree.root, operations);
    return stod(operations[0]);
}

template <typename TYPE>
void print_tree(AVLTree<TYPE> t, bool withKey = false) {
    print_tree(t.root, withKey);
}

// template <typename Data>
void print_tree(Node<Data> *node, bool withKey, const string &prefix = "", bool isLeft = true) {
    if (node == nullptr) return;

    // Print current node
    cout << prefix;

    cout << (isLeft ? "├── " : "└── ");

    if (withKey)
        cout << node->key << endl;
    else if (node->data.is_parenthesis)
        cout << "(" << node->data.value << ")" << endl;
    else
        cout << node->data << endl;

    // Recurse left and right
    bool hasLeft = node->left != nullptr;
    bool hasRight = node->right != nullptr;

    if (hasLeft || hasRight) {
        if (hasLeft) print_tree(node->left, withKey, prefix + (isLeft ? "│   " : "    "), true);
        if (hasRight) print_tree(node->right, withKey, prefix + (isLeft ? "│   " : "    "), false);
    }
}

#endif