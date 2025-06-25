#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>

#include "utils.h"

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

template <typename TYPE>
struct NodeT {
    int key;
    TYPE data;
    NodeT *left, *right;
};

template <typename TYPE>
struct Tree {
    NodeT<TYPE> *root;
};

template <typename TYPE>
NodeT<TYPE> *createNode(TYPE data, int key) {
    auto node = new NodeT;
    node->data = data;
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

template <typename TYPE = char>
Tree<TYPE> createMiniTree(const string &miniExpress, int key, Tree<TYPE> &t = {}) {
    t.root = createNode(miniExpress[0], key);
    t.root->left = nullptr;
    t.root->right = nullptr;
    if (miniExpress.length() > 1) t.root->data = miniExpress[1];
    switch (miniExpress.length()) {
        case 2:
            t.root->data = miniExpress[1];
            t.root->left = createNode(miniExpress[0], key - 1);
            break;
        case 3:
            t.root->left = createNode(miniExpress[0], key - 1);
            t.root->right = createNode(miniExpress[2], key + 1);
            break;
        case 5:
            t.root->left = createNode(miniExpress[0], key - 1);
            t.root->right = createMiniTree(miniExpress.substr(2, 3), key + 3).root;
            break;
        default:
            std::cout << "A error has occurred, length is not 1, 2, 3 or 5!" << std::endl;
            break;
    }

    return t;
}

template <typename TYPE>
void initializeTree(const string &expression, Tree<TYPE> &t) {}

template <typename TYPE>
void executeTree(Tree<TYPE> &t) {}

template <typename TYPE>
void inOrder(NodeT<TYPE> *node) {
    if (node == nullptr) return;
    inOrder(node->left);
    std::cout << node->data << " ";
    inOrder(node->right);
}

template <typename TYPE>
void printTree(NodeT<TYPE> *node, const string &prefix = "", bool isLeft = true) {
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
