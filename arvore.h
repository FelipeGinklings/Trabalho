#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>

#include "fila.h"
using namespace std;

struct Operation {
    int value;
    char operation;
    bool isOperation;
    int level;

    Operation() {};
    Operation(char newValue, int level) : operation(newValue), level(level) {
        if (newValue == '*' || newValue == '/' || newValue == '-' || newValue == '+') {
            this->isOperation = true;
            if (newValue == '*' || newValue == '/') this->value = level + 1;
        } else {
            this->isOperation = false;
            this->value = stoi(string(1, newValue));
        }
    };
};

struct Node {
    int key;
    Operation data;
    Node *left, *right;
};

struct Tree {
    Node *root;
};

Node *createNode(Operation data, int key) {
    auto node = new Node;
    node->data = data;
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

Queue<bool> insertNode(Node *&node, Operation data, int key, Queue<bool> navigation, int &counter) {}

void buildTree(const string &expression, Tree &t) {
    t.root = nullptr;
    int level = 0;
    Queue<bool> navigation{};
    initializeQueue(navigation);
    auto isOperation = [](const char &c) -> bool { return c == '*' || c == '/' || c == '-' || c == '+'; };
    int counter = 0;

    for (int i = 0; expression[i] != '\0'; i++) {
        const char &newChar = expression[i];
        if (newChar == '(') level++;
        if (newChar == ')') level--;
        if (newChar == '(' || newChar == ')') continue;
        auto newOperation = Operation(expression[i], level);
        const int newKey = (level + 1) * (isOperation ? newChar + 15 : newChar) + i;
        navigation = insertNode(t.root, newOperation, newKey, navigation, counter);
    }
}

void inOrder(Node *node) {
    if (node == nullptr) return;
    inOrder(node->left);
    if (node->data.isOperation)
        std::cout << node->data.operation << " ";
    else
        std::cout << node->data.value << " ";
    inOrder(node->right);
}

Node *executeTree(Node *node) {
    if (node == nullptr) return node;
    executeTree(node->left);
    if (node->data.isOperation)
        std::cout << node->data.operation << " ";
    else
        std::cout << node->data.value << " ";
    executeTree(node->right);
    return node;
}

void printTree(Node *node, const string &prefix = "", bool isLeft = true) {
    if (node == nullptr) return;

    // Print current node
    cout << prefix;

    cout << (isLeft ? "├── " : "└── ");

    if (node->data.isOperation)
        cout << node->data.operation << endl;
    else
        cout << node->data.value << endl;

    // Recurse left and right
    bool hasLeft = node->left != nullptr;
    bool hasRight = node->right != nullptr;

    if (hasLeft || hasRight) {
        if (hasLeft) printTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
        if (hasRight) printTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
    }
}

#endif  // TREE_HPP
