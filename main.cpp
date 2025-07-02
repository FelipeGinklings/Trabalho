#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "arvore_sbb.h"

using namespace std;

int main() {
    string main_str = "2+(55/(95-2*77-30+94*(32)*97)/73)";
    ParenthesisData* parenthesis_data = separate_by_parenthesis(main_str);
    create_sub_trees(parenthesis_data);
    create_tree(parenthesis_data);
    in_order(parenthesis_data->tree);
    cout << endl;
    print_tree(parenthesis_data->tree);
    const double result = calculate_tree(parenthesis_data->tree);
    cout << "Resultado da expressão '" << main_str << "': " << result << endl;

    return 0;
}
