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

void testSeparateOperations() {
    // Basic tests first
    string expr1 = "1+2";
    vector<string> result1 = separateOperations(expr1);
    assert(result1.size() > 0);

    string expr2 = "1*2+3";
    vector<string> result2 = separateOperations(expr2);
    assert(result2.size() > 0);

    string expr3 = "";
    vector<string> result3 = separateOperations(expr3);
    assert(result3.empty());

    cout << "testCreateOperations passed\n";
}

void testAreEqual() {
    vector<string> vec1 = {"1", "+", "2"};
    string test1 = "1+2";
    assert(areEqual(vec1, test1) == true);

    vector<string> vec2 = {"1", "*", "3"};
    string test2 = "1+3";
    assert(areEqual(vec2, test2) == false);

    vector<string> vec3;
    string test3 = "";
    assert(areEqual(vec3, test3) == true);

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

int main() {
    testOpLv1();
    testOpLv2();
    testGenerateExpression();
    testSeparateOperations();
    testAreEqual();
    testIsValidExpression();
    cout << "All tests passed!\n" << endl;
    // string genString = "9/5+8/2+3+1/1*7+3*7/6+4+6-7+1*7*2*6*6*8*";
    // auto operation = separateOperations(genString);
    return 0;
}