#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "arvore_sbb.h"

using namespace std;

int main() {
    string generated_expression = generateExpression(100);
    cout << "Generated expression: " << generated_expression << endl;
    ParenthesisData* parenthesis_data = separate_by_parenthesis(generated_expression);
    create_sub_trees(parenthesis_data);
    create_tree(parenthesis_data);
    in_order(parenthesis_data->tree);
    cout << endl;
    print_tree(parenthesis_data->tree);
    const double result = calculate_tree(parenthesis_data->tree);
    cout << "Resultado da expressão '" << generated_expression << "': " << result << endl;

    return 0;
}
