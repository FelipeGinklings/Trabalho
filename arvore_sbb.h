#ifndef SBB_TREE_HPP
#define SBB_TREE_HPP

#include <iostream>

using namespace std;

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

string calculate_tree(Node<string> *node, Node<string> *next) {
    if (next != nullptr) {
        auto left = calculate_tree(node->left, node->left->left);
        string operation = node->data;
        auto right = calculate_tree(node->right, node->right->right);
        return left + operation + right;
    }
    return node->data;
}

template <typename TYPE>
void print_tree(AVLTree<TYPE> t, bool withKey = false) {
    print_tree(t.root, withKey);
}

template <typename TYPE>
void print_tree(Node<TYPE> *node, bool withKey, const string &prefix = "", bool isLeft = true) {
    if (node == nullptr) return;

    // Print current node
    cout << prefix;

    cout << (isLeft ? "├── " : "└── ");

    if (withKey)
        cout << node->key << endl;
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