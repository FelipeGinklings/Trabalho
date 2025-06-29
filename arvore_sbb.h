#ifndef SBB_TREE_HPP
#define SBB_TREE_HPP

#include <iostream>

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
void initializeTree(AVLTree<TYPE> &a) {
    a.root = nullptr;
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
void preOrder(Node<TYPE> *node) {
    if (node != nullptr) {
        std::cout << node->data << ", ";
        preOrder(node->left);
        preOrder(node->right);
    }
}

template <typename TYPE>
void inOrder(Node<TYPE> *node) {
    if (node != nullptr) {
        inOrder(node->left);
        std::cout << node->data;
        inOrder(node->right);
    }
}

template <typename TYPE>
void postOrder(Node<TYPE> *node) {
    if (node != nullptr) {
        postOrder(node->left);
        postOrder(node->right);
        std::cout << node->data << ", ";
    }
}

#endif