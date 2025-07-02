#ifndef SBB_TREE_HPP
#define SBB_TREE_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "utils.h"
using namespace std;

void create_tree(ParenthesisData *&parenthesis_data) {
    if (parenthesis_data->next_parenthesis.empty()) return;
    for (auto &[_, second] : parenthesis_data->next_parenthesis) {
        create_tree(second);
    }
    navigate_inside_tree(parenthesis_data->tree, parenthesis_data);
}

void calculate_tree(Node<Data> *node, vector<string> &operations) {
    if (node != nullptr) {
        if (node->data.is_parenthesis) {
            node->data.is_parenthesis = false;
            vector<string> new_operations;
            calculate_tree(node, new_operations);
            node->data.value = new_operations[0];
            // cout << "Parenthesis result: " << node->data.value << endl;
            node->data.is_parenthesis = true;
        } else
            calculate_tree(node->left, operations);

        operations.push_back(node->data.value);
        if (operations.size() == 3) {
            double left = stod(operations[0]);
            Operation op = Operation(operations[1]);
            double right = stod(operations[2]);
            double result = op.apply(left, right);
            // cout << "Calculating: " << left << " " << op.operation << " " << right << " = " << result << endl;
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

#endif