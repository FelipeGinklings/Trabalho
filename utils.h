#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "classes.h"
#include "pilha.h"

using namespace std;

bool opLv1(const char &c) { return c == '-' || c == '+'; }
bool opLv2(const char &c) { return c == '*' || c == '/'; }

bool areEqual(vector<string> &strings, string &stringForTest) {
    string mergedString = "";
    for (const string &str : strings) mergedString += str;
    return stringForTest.length() == mergedString.length();
}

string generateExpression(int size) {
    if (size <= 0) return "";

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> numDist(1, 9);
    uniform_int_distribution<> opDist(0, 3);

    string expression = to_string(numDist(gen));

    char operators[] = {'+', '-', '*', '/'};

    for (int i = 1; i < size; i++) {
        expression += operators[opDist(gen)];
        expression += to_string(numDist(gen));
    }

    return expression;
}

bool isValidExpression(const string &expression) {
    Stack<char> stackOfChars{};
    initializeStack(stackOfChars);
    bool isValid = true;
    auto convertChar = [](const char &closeChar) -> char {
        if (closeChar == '}') return '{';
        if (closeChar == ']') return '[';
        if (closeChar == ')') return '(';
        return '-';
    };

    for (int i = 0; expression[i] != '\0'; i++) {
        const char character = expression[i];
        if (character == '{' || character == '[' || character == '(')
            addToTopStack(stackOfChars, character);
        else if (character == '}' || character == ']' || character == ')') {
            if (stackOfChars.first == nullptr || convertChar(character) != removeFromTopStack(stackOfChars)) return false;
        }
    }
    if (stackOfChars.first != nullptr) isValid = false;
    return isValid;
}

string get_next_letter(const string &nextLetter, int multiplier = 1) {
    if (nextLetter.empty()) return "";
    char firstChar = nextLetter[0];
    char nextChar = 'A' + (firstChar - 'A' + 1) % 26;
    return string(multiplier, nextChar);
}

ExpressionResult organize_expression(const string &expression_str) {
    regex pattern(R"([+-]?(?:\d+\.?\d*|[a-zA-Z]+)(?:[*/][+-]?(?:\d+\.?\d*|[a-zA-Z]+))+)");

    vector<string> multiplications;

    sregex_iterator iter(expression_str.begin(), expression_str.end(), pattern);
    sregex_iterator end;

    for (; iter != end; ++iter) {
        multiplications.push_back(iter->str());
    }

    if (multiplications.empty()) {
        return ExpressionResult("", "", expression_str);
    }

    string sums_str = expression_str;
    for (const string &mul : multiplications) {
        size_t pos = sums_str.find(mul);
        if (pos != string::npos) {
            sums_str.replace(pos, mul.length(), "");
        }
    }

    string multiplications_str = "";
    for (const string &mul : multiplications) {
        multiplications_str += mul;
    }

    bool start_with_plus = multiplications_str.length() > 0 && multiplications_str[0] == '+';

    if (sums_str == "") {
        return ExpressionResult(start_with_plus ? multiplications_str.substr(1) : multiplications_str, "", "");
    }

    if (!(sums_str[0] == '+' || sums_str[0] == '-')) {
        sums_str = "+" + sums_str;
    }

    return ExpressionResult(start_with_plus ? multiplications_str.substr(1) : multiplications_str, string(1, sums_str[0]), sums_str.substr(1));
}

string organized_operation_to_string(ExpressionResult organized_operation) {
    return organized_operation.multiplications + organized_operation.operation + organized_operation.sums;
}

vector<string> re_findall(const string &pattern, const string &text) {
    regex re(pattern);
    sregex_iterator it(text.begin(), text.end(), re);
    sregex_iterator end;

    vector<string> matches;
    while (it != end) {
        smatch match = *it;
        matches.push_back(match.str());
        ++it;
    }

    return matches;
}

int add_operation(AVLTree<string> &tree, int &key, string &number, sregex_iterator &iter_numbers, string &expression, string &operation, int retFlag = 1) {
    insert(tree.root, key, number);
    size_t operation_index = iter_numbers->position() + iter_numbers->length();
    if (operation_index >= expression.length()) return 2;
    operation = expression[operation_index];
    ++key;
    insert(tree.root, key, operation);
    return 1;
}

bool is_letter(const string &letter) {
    regex letters_regex(R"([a-zA-Z]+)");
    return regex_match(letter, letters_regex);
}

int add_characters_to_tree(AVLTree<string> &tree, ParenthesisData *parenthesisData, int key = 0) {
    regex pattern_numbers(R"([a-zA-Z]+|\d+\.?\d*)");
    string expression = parenthesisData->expression;
    bool first_number_is_negative = expression[0] == '-';
    sregex_iterator iter_numbers(expression.begin(), expression.end(), pattern_numbers), end;
    string number = (first_number_is_negative ? "-" : "") + iter_numbers->str();
    string operation = "";
    add_operation(tree, key, number, iter_numbers, expression, operation);
    ++iter_numbers;
    key++;
    for (key = key; iter_numbers != end; ++key, ++iter_numbers) {
        string number = iter_numbers->str();
        if (is_letter(number))
            key = add_characters_to_tree(tree, parenthesisData->next_parenthesis[number], key);
        else {
            int retFlag = add_operation(tree, key, number, iter_numbers, expression, operation);
            if (retFlag == 2) break;
        }
    }
    return key;
}

ParenthesisData *separate_by_parenthesis(const string &expression, const string &letter = "A", int multiplier = 1) {
    ParenthesisData *new_data = new ParenthesisData();
    string current_expression = "", next_letter = letter;
    int level = 0;

    for (size_t index = 0; index < expression.length(); index++) {
        char ch = expression[index];
        if (ch == '(') {
            if (!current_expression.empty() && level == 0) {
                new_data->expression += current_expression + next_letter;
                current_expression = "";
                level++;
                continue;
            }
            level++;
            if (index == 0) {
                new_data->expression += letter;
                continue;
            }
        } else if (ch == ')') {
            if (!current_expression.empty() && level == 1) {
                new_data->next_parenthesis[next_letter] = separate_by_parenthesis(current_expression);
                next_letter = get_next_letter(next_letter, multiplier);
                if (next_letter == "Z") multiplier++;
                current_expression = "";
                level--;
                continue;
            }
            level--;
        }
        current_expression += ch;
    }

    if (!current_expression.empty()) new_data->expression += current_expression;

    cout << new_data->expression << endl << endl;
    ExpressionResult new_organized_expression = organize_expression(new_data->expression);
    new_data->expression = organized_operation_to_string(new_organized_expression);
    new_data->multiplications = new_organized_expression.multiplications;
    new_data->operation = new_organized_expression.operation;
    new_data->sums = new_organized_expression.sums;
    return new_data;
}
