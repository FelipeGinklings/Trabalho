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
void initializeTree(Tree<TYPE> &t) {
    t.root = nullptr;
}

template <typename TYPE>
NodeT<TYPE> *createNode(TYPE data, int key) {
    auto node = new NodeT<TYPE>;
    node->data = data;
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

Tree<string> createMiniTree(string miniExpress, int key) {
    Tree<string> miniTree;
    initializeTree(miniTree);
    const string oldMiniExpress = miniExpress;
    miniTree.root = createNode(string(1, miniExpress[0]), key);
    miniTree.root->left = nullptr;
    miniTree.root->right = nullptr;
    if (miniExpress.length() > 1)
        miniTree.root->data = miniExpress[1];
    else
        return miniTree;
    if (miniExpress.length() == 2 && opLv1(miniExpress[0])) miniExpress += miniExpress + miniExpress;
    switch (miniExpress.length()) {
        case 1:
            break;
        case 2:
            miniTree.root->left = createNode(string(1, miniExpress[0]), key - 1);
            break;
        case 3:
            miniTree.root->left = createNode(string(1, miniExpress[0]), key - 1);
            miniTree.root->right = createNode(string(1, miniExpress[2]), key + 1);
            break;
        case 5:
            miniTree.root->left = createNode(string(1, miniExpress[0]), key - 1);
            miniTree.root->right = createMiniTree(miniExpress.substr(2, 3), key + 3).root;
            break;
        case 6:
            miniTree.root->data = string(1, miniExpress[0]);
            miniTree.root->right = createNode(string(1, miniExpress[1]), key + 1);
            break;
        default:
            std::cout << "A error has occurred, length is not 1, 2, 3 or 5!" << std::endl;
            break;
    }

    return miniTree;
}

string extractFinalOperation(vector<string> &expressionsList, int firsLv1Index, bool operationIn, bool operationOut, bool operationInMiddle) {
    string finalOperation;
    int exListLen = expressionsList.size();
    const string last = expressionsList[exListLen - 1];

    if (firsLv1Index <= 0 || firsLv1Index == exListLen - 1) {
    } else if (operationIn)
        finalOperation = expressionsList[0];
    else if (operationOut && opLv1(last[0]) && last.length() == 2) {
        finalOperation = string(1, last[0]);
        expressionsList[exListLen - 1] = expressionsList[exListLen - 1].substr(1, 1);
    } else if (operationOut && last.length() == 2) {
        finalOperation = string(1, last[1]);
        expressionsList[exListLen - 1].pop_back();
    } else if (operationOut) {
        finalOperation = last;
        expressionsList.pop_back();
    } else if (operationInMiddle) {
        finalOperation = string(1, expressionsList[firsLv1Index][1]);
        expressionsList[firsLv1Index] = expressionsList[firsLv1Index].substr(1, 1);
    }

    return finalOperation;
}

void createSubTreeLv1(int firsLv1Index, vector<string> &expressionsList, int key, vector<NodeT<string> *> &trees, Tree<string> &subTreeLv1) {
    for (int index = firsLv1Index; index < expressionsList.size(); index++) {
        int newKey = (index - firsLv1Index) * 5 + key;
        trees.push_back(createMiniTree(expressionsList[index], newKey).root);
    }

    for (int index = firsLv1Index; index < trees.size(); index++) {
        NodeT<string> *recentNode = trees[index];
        const int length = expressionsList[index].length();
        if (length == 1) {
            recentNode->left = subTreeLv1.root;
            index++;
            NodeT<string> *nextNode = trees[index];
            if (expressionsList[index].length() == 2 && opLv1(expressionsList[index][1])) {
                recentNode->right = nextNode->left;
                expressionsList[index] = expressionsList[index].substr(1, 1);
                index--;
            } else if (expressionsList[index].length() == 2) {
                recentNode->right = nextNode->right;
                expressionsList[index].pop_back();
            } else
                recentNode->right = trees[index];
        } else if (length == 2 && index + 1 != trees.size()) {
            index++;
            recentNode->right = trees[index];
            NodeT<string> *nextNode = trees[index];
            if (expressionsList[index].length() == 2 && opLv1(expressionsList[index][1])) {
                recentNode->right = nextNode->left;
                expressionsList[index] = expressionsList[index].substr(1, 1);
                index--;
            } else if (expressionsList[index].length() == 2) {
                recentNode->right = nextNode->right;
                expressionsList[index].pop_back();
            } else
                recentNode->right = trees[index];
        } else if (length == 2) {
            recentNode->left = subTreeLv1.root;
        }
        subTreeLv1.root = recentNode;
    }
}

void createSubTreeLv2(int firsLv1Index, int exListLen, int key, vector<NodeT<string> *> &trees, vector<string> &expressionsList, bool operationIn, Tree<string> &subTreeLv2) {
    for (int index = 0; firsLv1Index >= 0 ? (index < firsLv1Index && firsLv1Index) : (index < exListLen); index++) {
        int newKey = (index + 1) * -5 + key;
        trees.push_back(createMiniTree(expressionsList[index], newKey).root);
    }

    for (int index = operationIn; firsLv1Index >= 0 ? (index < firsLv1Index && firsLv1Index) : (index < exListLen); index++) {
        NodeT<string> *recentNode = trees[index];
        if (expressionsList[index].length() == 1) {
            recentNode->right = subTreeLv2.root;
            index++;
            recentNode->left = trees[index];
        }
        subTreeLv2.root = recentNode;
    }
}

Tree<string> createSubTree(vector<string> &expressionsList, int key, int firsLv1Index) {
    const string first = expressionsList[0];
    int exListLen = expressionsList.size();
    const string last = expressionsList[exListLen - 1];

    bool operationIn = first.length() == 1;
    bool operationOut = last.length() == 1 || (last.length() == 2 && (opLv1(last[0]) || opLv1(last[1])));
    bool operationInMiddle = firsLv1Index != -1 && !operationIn && !operationOut;

    string finalOperation = extractFinalOperation(expressionsList, firsLv1Index, operationIn, operationOut, operationInMiddle);

    vector<NodeT<string> *> trees;
    Tree<string> mainSubTree;
    initializeTree(mainSubTree);
    Tree<string> subTreeLv1;
    initializeTree(subTreeLv1);
    Tree<string> subTreeLv2;
    initializeTree(subTreeLv2);

    createSubTreeLv2(firsLv1Index, exListLen, key, trees, expressionsList, operationIn, subTreeLv2);

    if (firsLv1Index == -1)
        return subTreeLv2;
    else if (firsLv1Index == exListLen - 1) {
        string operationStr = string(1, last[1]), number = string(1, last[0]);
        if (opLv1(last[0])) {
            operationStr = string(1, last[0]);
            number = string(1, last[1]);
        }
        mainSubTree = createMiniTree(operationStr, key);
        mainSubTree.root->left = subTreeLv2.root;
        mainSubTree.root->right = createMiniTree(number, 1).root;

        return mainSubTree;
    }

    createSubTreeLv1(firsLv1Index, expressionsList, key, trees, subTreeLv1);

    if (!firsLv1Index) return subTreeLv1;

    mainSubTree = createMiniTree(finalOperation, key);
    mainSubTree.root->left = subTreeLv2.root;
    mainSubTree.root->right = subTreeLv1.root;

    return mainSubTree;
}

template <typename TYPE>
void executeTree(Tree<TYPE> &t) {}

template <typename TYPE>
void inOrder(Tree<TYPE> t) {
    inOrder(t.root);
}

template <typename TYPE>
void inOrder(NodeT<TYPE> *node) {
    if (node == nullptr) return;
    inOrder(node->left);
    std::cout << node->data;
    inOrder(node->right);
}

template <typename TYPE>
void printTree(Tree<TYPE> t, bool withKey = false) {
    printTree(t.root, withKey);
}

template <typename TYPE>
void printTree(NodeT<TYPE> *node, bool withKey, const string &prefix = "", bool isLeft = true) {
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
        if (hasLeft) printTree(node->left, withKey, prefix + (isLeft ? "│   " : "    "), true);
        if (hasRight) printTree(node->right, withKey, prefix + (isLeft ? "│   " : "    "), false);
    }
}

#endif  // TREE_HPP
