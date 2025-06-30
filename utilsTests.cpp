#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "utils.h"

using namespace std;

void testOpLv1() {
    assert(opLv1('+') == true);
    assert(opLv1('-') == true);
    assert(opLv1('*') == false);
    assert(opLv1('/') == false);
    assert(opLv1('5') == false);
    cout << "testOpLv1 passed\n";
}

void testOpLv2() {
    assert(opLv2('*') == true);
    assert(opLv2('/') == true);
    assert(opLv2('+') == false);
    assert(opLv2('-') == false);
    assert(opLv2('3') == false);
    cout << "testOpLv2 passed\n";
}

void testAreEqual() {
    vector<string> vec1 = {"1", "+", "2"};
    string test1 = "1+2";
    assert(areEqual(vec1, test1));

    vector<string> vec2 = {"1", "*", "3"};
    string test2 = "1+3";
    assert(areEqual(vec2, test2));

    vector<string> vec3;
    string test3 = "";
    assert(areEqual(vec3, test3));

    cout << "testAreEqual passed\n";
}

void testGenerateExpression() {
    string expr1 = generateExpression(0);
    assert(expr1 == "");

    string expr2 = generateExpression(-1);
    assert(expr2 == "");

    string expr3 = generateExpression(1);
    assert(expr3.length() == 1);
    assert(expr3[0] >= '1' && expr3[0] <= '9');

    string expr4 = generateExpression(3);
    assert(expr4.length() == 5);  // num + op + num + op + num

    cout << "testGenerateExpression passed\n";
}

void testIsValidExpression() {
    // Test valid expressions with parentheses
    assert(isValidExpression("()") == true);
    assert(isValidExpression("(())") == true);
    assert(isValidExpression("((()))") == true);

    // Test valid expressions with brackets
    assert(isValidExpression("[]") == true);
    assert(isValidExpression("[[]]") == true);
    assert(isValidExpression("[[][]]") == true);

    // Test valid expressions with braces
    assert(isValidExpression("{}") == true);
    assert(isValidExpression("{{}}") == true);
    assert(isValidExpression("{{}{}}") == true);

    // Test valid mixed expressions
    assert(isValidExpression("()[]{}") == true);
    assert(isValidExpression("{[()]}") == true);
    assert(isValidExpression("({[]})") == true);
    assert(isValidExpression("[{()}]") == true);

    // Test empty expression
    assert(isValidExpression("") == true);

    // Test invalid expressions - unmatched opening
    assert(isValidExpression("(") == false);
    assert(isValidExpression("[") == false);
    assert(isValidExpression("{") == false);
    assert(isValidExpression("((") == false);

    // Test invalid expressions - unmatched closing
    assert(isValidExpression(")") == false);
    assert(isValidExpression("]") == false);
    assert(isValidExpression("}") == false);
    assert(isValidExpression("))") == false);

    // Test invalid expressions - mismatched pairs
    assert(isValidExpression("(]") == false);
    assert(isValidExpression("(}") == false);
    assert(isValidExpression("[)") == false);
    assert(isValidExpression("[}") == false);
    assert(isValidExpression("{)") == false);
    assert(isValidExpression("{]") == false);

    // Test invalid expressions - wrong order
    assert(isValidExpression(")(") == false);
    assert(isValidExpression("}{") == false);
    assert(isValidExpression("][") == false);

    // Test complex invalid expressions
    assert(isValidExpression("([)]") == false);
    assert(isValidExpression("{[}]") == false);
    assert(isValidExpression("((()") == false);
    assert(isValidExpression("())") == false);

    cout << "testIsValidExpression passed\n";
}

void testGetNextLetter() {
    // Test basic functionality
    assert(get_next_letter("A") == "B");
    assert(get_next_letter("B") == "C");
    assert(get_next_letter("Z") == "A");  // wraps around

    // Test with multiplier
    assert(get_next_letter("A", 1) == "B");
    assert(get_next_letter("A", 3) == "BBB");
    assert(get_next_letter("B", 2) == "CC");

    // Test empty string
    assert(get_next_letter("") == "");

    // Test wrap around cases
    assert(get_next_letter("Y") == "Z");
    assert(get_next_letter("Z") == "A");

    cout << "testGetNextLetter passed\n";
}

void testOrganizeExpression() {
    // Test simple addition - no multiplications found
    ExpressionResult result1 = organize_expression("1+2");
    assert(result1.multiplications == "");
    assert(result1.operation == "");
    assert(result1.sums == "1+2");

    // Test simple multiplication
    ExpressionResult result2 = organize_expression("2*3");
    assert(result2.multiplications == "2*3");
    assert(result2.operation == "");
    assert(result2.sums == "");

    // Test mixed operations
    ExpressionResult result3 = organize_expression("1+2*3");
    assert(result3.multiplications == "2*3");
    assert(result3.operation == "+");
    assert(result3.sums == "1");

    // Test expression starting with multiplication
    ExpressionResult result4 = organize_expression("2*3+1");
    assert(result4.multiplications == "2*3");
    assert(result4.operation == "+");
    assert(result4.sums == "1");

    // Test complex expression
    ExpressionResult result5 = organize_expression("1+2*3+4*5");
    assert(result5.multiplications == "2*3+4*5");
    assert(result5.operation == "+");
    assert(result5.sums == "1");

    // Test negative numbers
    ExpressionResult result6 = organize_expression("-1+2*3");
    assert(result6.multiplications == "2*3");
    assert(result6.operation == "-");
    assert(result6.sums == "1");

    cout << "testOrganizeExpression passed\n";
}

void testOrganizedOperationToString() {
    // Test basic functionality
    ExpressionResult expr1("2*3", "+", "1");
    assert(organized_operation_to_string(expr1) == "2*3+1");

    ExpressionResult expr2("", "+", "1+2");
    assert(organized_operation_to_string(expr2) == "+1+2");

    ExpressionResult expr3("2*3", "", "");
    assert(organized_operation_to_string(expr3) == "2*3");

    ExpressionResult expr4("", "", "");
    assert(organized_operation_to_string(expr4) == "");

    ExpressionResult expr5("4*5", "-", "2");
    assert(organized_operation_to_string(expr5) == "4*5-2");

    cout << "testOrganizedOperationToString passed\n";
}

void testSeparateByParenthesis() {
    auto tree = AVLTree<string>{};
    initialize_tree(tree);
    // Test simple expression without parentheses
    ParenthesisData* result1 = separate_by_parenthesis("1+2");
    assert(result1->expression == "1+2");
    // assert(result1->multiplications == "");
    // assert(result1->operation == "");
    // assert(result1->sums == "1+2");
    assert(result1->next_parenthesis.empty());
    delete result1;

    // Test simple parentheses
    ParenthesisData* result2 = separate_by_parenthesis("(1+2)");
    assert(result2->expression == "A");
    // assert(result2->multiplications == "");
    // assert(result2->operation == "");
    // assert(result2->sums == "A");
    assert(result2->next_parenthesis.count("A") == 1);
    assert(result2->next_parenthesis["A"]->expression == "1+2");
    // Clean up nested data
    delete result2->next_parenthesis["A"];
    delete result2;

    // Test expression with parentheses in middle
    ParenthesisData* result3 = separate_by_parenthesis("1+(2+3)");
    assert(result3->expression == "1+A");
    // assert(result3->multiplications == "");
    // assert(result3->operation == "");
    // assert(result3->sums == "1+A");
    assert(result3->next_parenthesis.count("A") == 1);
    assert(result3->next_parenthesis["A"]->expression == "2+3");
    // Clean up
    delete result3->next_parenthesis["A"];
    delete result3;

    // Test multiple parentheses
    ParenthesisData* result4 = separate_by_parenthesis("(1+2)+(3+4)");
    assert(result4->expression == "A+B");
    // assert(result4->multiplications == "");
    // assert(result4->operation == "");
    // assert(result4->sums == "A+B");
    assert(result4->next_parenthesis.count("A") == 1);
    assert(result4->next_parenthesis.count("B") == 1);
    assert(result4->next_parenthesis["A"]->expression == "1+2");
    assert(result4->next_parenthesis["B"]->expression == "3+4");
    // Clean up
    delete result4->next_parenthesis["A"];
    delete result4->next_parenthesis["B"];
    delete result4;

    cout << "testSeparateByParenthesis passed\n";
}

int main() {
    // testOpLv1();
    // testOpLv2();
    // testGenerateExpression();
    // testAreEqual();
    // testIsValidExpression();
    // testGetNextLetter();
    // testOrganizeExpression();
    // testOrganizedOperationToString();
    // testSeparateByParenthesis();
    // cout << "All utils tests passed!\n" << endl;
    // string teste = "2+(55/(95-2*77-30+94*(32)*97)/73)";
    string teste = "3*(8/2*(3*2)+6-2)-1+2";
    // auto tree = AVLTree<string>{};
    // initializeTree(tree);
    // auto new_linked_list = LinkedList<string>();
    ParenthesisData* parenthesis_data = separate_by_parenthesis(teste);
    create_sub_trees(parenthesis_data);
    // in_order(parenthesis_data->tree);
    print_tree(parenthesis_data->tree);
    // calculate_tree(tree.root, tree.root->left);
    // navigate_inside_parenthesis(parenthesis_data, tree);
    // auto vector_teste = new_linked_list.convert_to_vector();
    return 0;
}
