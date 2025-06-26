#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "arvore.h"

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

void testEndInOperation() {
    string expr1 = "1+2+";
    string result1 = endInOperation(expr1);
    assert(result1 == "+");
    assert(expr1 == "1+2");

    string expr2 = "3*4*";
    string result2 = endInOperation(expr2);
    assert(result2 == "*");
    assert(expr2 == "3*4");

    string expr3 = "5-6/";
    string result3 = endInOperation(expr3);
    assert(result3 == "/");
    assert(expr3 == "5-6");

    string expr4 = "7+8";
    string result4 = endInOperation(expr4);
    assert(result4 == "");
    assert(expr4 == "7+8");

    string expr5 = "9";
    string result5 = endInOperation(expr5);
    assert(result5 == "");
    assert(expr5 == "9");

    cout << "testEndInOperation passed\n";
}

void testSeparateOperations() {
    // Basic tests first
    string expr1 = "1+2";
    auto [result1, firstLv1_1] = separateOperations(expr1);
    assert(result1.size() > 0 && firstLv1_1 != -1);

    string expr2 = "1*2+3";
    auto [result2, firstLv1_2] = separateOperations(expr2);
    assert(result2.size() > 0 && firstLv1_2 != -1);

    string expr3 = "";
    auto [result3, firstLv1_3] = separateOperations(expr3);
    assert(result3.empty());

    string generatedExpr = generateExpression(10);
    if (!generatedExpr.empty()) {
        auto [resultGenerated, firstLv1_gen] = separateOperations(generatedExpr);
        assert(areEqual(resultGenerated, generatedExpr));
    }
    string expr4 = "5+3*2";
    auto [result4, firstLv1_4] = separateOperations(expr4);
    assert(areEqual(result4, expr4) && firstLv1_4 != -1);

    string expr5 = "1-2/4";
    auto [result5, firstLv1_5] = separateOperations(expr5);
    assert(areEqual(result5, expr5) && firstLv1_5 != -1);

    cout << "testCreateOperations passed\n";
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

void createSimpleExpressionTree() {
    // for (int i = 2; i <= 30; i++) {
    // for (int j = 0; j < 10; j++) {
    // string genString = generateExpression(10);
    string genString = "8*9-9/1+4+2-4*8*9-8";
    string hasOperation = endInOperation(genString);
    auto [operations, firstLv1] = separateOperations(genString);
    auto newSubTree = createSubTree(operations, 0, firstLv1);
    cout << "genString: " << genString << endl;
    cout << "inOrder:   ";
    inOrder(newSubTree);
    cout << endl;
    printTree(newSubTree);
    cout << endl << endl;
    // }
    // }
}

int main() {
    testOpLv1();
    testOpLv2();
    testEndInOperation();
    testGenerateExpression();
    testSeparateOperations();
    testAreEqual();
    testIsValidExpression();
    createSimpleExpressionTree();
    cout << "All tests passed!\n" << endl;
    return 0;
}