#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
using namespace std;

struct Operation {
    int value;
    char operation;
    bool isOperation;

    Operation() {};
    Operation(char newValue) : operation(newValue) {
        const string numbers = "0123456789";
        for (const char &num : numbers) {
            if (newValue == num) {
                this->value = stoi(string(1, num));
                this->isOperation = false;
                return;
            }
        }
        this->isOperation = true;
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

void insertNode(Node *&node, Operation data, int key) {
    if (node == nullptr) {
        node = new Node;
        node->data = data;
        node->key = key;
        node->left = nullptr;
        node->right = nullptr;
    } else {
        if (key < node->key) {
            insertNode(node->left, data, key);
        } else if (key > node->key) {
            insertNode(node->right, data, key);
        } else {
            std::cout << "Data already inserted";
        }
    }
}

void buildTree(const string &expression, Tree &t) {
    t.root = nullptr;
    for (int i = 0; expression[i] != '\0'; i++) {
        auto newOperation = Operation(expression[i]);
        insertNode(t.root, newOperation, i);
    }
}

void inOrder(Node *node) {
    if (node != nullptr) {
        inOrder(node->left);
        if (node->data.isOperation)
            std::cout << node->data.operation << " ";
        else
            std::cout << node->data.value << " ";
        inOrder(node->right);
    }
}

void executeTree(Node *node) {
    if (node != nullptr) {
        executeTree(node->left);
        if (node->data.isOperation)
            std::cout << node->data.operation << " ";
        else
            std::cout << node->data.value << " ";
        executeTree(node->right);
    }
}

#endif  // TREE_HPP
