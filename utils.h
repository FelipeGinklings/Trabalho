
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "classes.h"
#include "pilha.h"

using namespace std;

constexpr char VERTICAL = 'v';
constexpr char HORIZONTAL = 'h';

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

string convert(double value, int precision = 17) {
    ostringstream oss;
    oss << setprecision(precision) << value;
    return oss.str();
}

double convert(const string &str) {
    try {
        return stod(str);
    } catch (const invalid_argument &e) {
        throw invalid_argument("Invalid double string: " + str);
    } catch (const out_of_range &e) {
        throw out_of_range("Double out of range: " + str);
    }
}

bool areEqual(vector<string> &strings, string &stringForTest) {
    string mergedString = "";
    for (const string &str : strings) mergedString += str;
    return stringForTest.length() == mergedString.length();
}

string generateExpression(int size, bool withParenthesis = false, bool withFloatingNumbers = false, int minInt = 10, int maxInt = 99, double minFloat = 10.0,
                          double maxFloat = 99.9) {
    if (size <= 0) return "";

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> numDist(minInt, maxInt);
    uniform_real_distribution<> floatDist(minFloat, maxFloat);
    uniform_int_distribution<> opDist(0, 3);
    uniform_int_distribution<> typeDist(0, 1);  // 0 for int, 1 for float

    string expression;
    if (withFloatingNumbers && typeDist(gen)) {
        expression = convert(floatDist(gen), 1);
    } else {
        expression = to_string(numDist(gen));
    }

    char operators[] = {'+', '-', '*', '/'};

    for (int i = 1; i < size; i++) {
        expression += operators[opDist(gen)];
        if (withFloatingNumbers && typeDist(gen)) {
            expression += convert(floatDist(gen), 1);
        } else {
            expression += to_string(numDist(gen));
        }
    }

    if (withParenthesis && size >= 3) {
        int numParentheses = min(size / 3, 2);
        
        for (int p = 0; p < numParentheses; p++) {
            vector<int> openPositions, closePositions;

            // Find all number positions
            for (int i = 0; i < expression.length(); i++) {
                if ((isdigit(expression[i]) || expression[i] == '.') && (i == 0 || (!isdigit(expression[i - 1]) && expression[i - 1] != '.'))) {
                    openPositions.push_back(i);
                }
                if ((isdigit(expression[i]) || expression[i] == '.') && (i == expression.length() - 1 || (!isdigit(expression[i + 1]) && expression[i + 1] != '.'))) {
                    closePositions.push_back(i + 1);
                }
            }

            if (openPositions.size() >= 3 && closePositions.size() >= 3) {
                uniform_int_distribution<> openDist(0, openPositions.size() - 3);
                int openPos = openPositions[openDist(gen)];

                vector<int> validClosePos;
                for (int pos : closePositions) {
                    if (pos > openPos + 4) { // Ensure at least 3 numbers inside
                        validClosePos.push_back(pos);
                    }
                }

                if (!validClosePos.empty()) {
                    uniform_int_distribution<> closeDist(0, validClosePos.size() - 1);
                    int closePos = validClosePos[closeDist(gen)];

                    // Check if adding parentheses would create nested unnecessary parentheses
                    string inside = expression.substr(openPos, closePos - openPos);
                    if (inside.find('(') == string::npos || 
                        (inside.front() != '(' || inside.back() != ')')) {
                        expression = expression.substr(0, openPos) + "(" + inside + ")" + expression.substr(closePos);
                    }
                }
            }
        }
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

    if (multiplications.empty()) return ExpressionResult("", "", expression_str);

    string sums_str = expression_str;
    for (const string &mul : multiplications) {
        size_t pos = sums_str.find(mul);
        if (pos != string::npos) sums_str.replace(pos, mul.length(), "");
    }

    string multiplications_str = "";
    if (multiplications.size() == 1) {
        multiplications_str = multiplications[0];
    } else {
        for (size_t i = 0; i < multiplications.size(); ++i) {
            if (i > 0) multiplications_str += "+";
            multiplications_str += "(" + multiplications[i] + ")";
        }
    }
    bool start_with_plus = multiplications_str.length() > 0 && multiplications_str[0] == '+';

    if (sums_str == "") return ExpressionResult(start_with_plus ? multiplications_str.substr(1) : multiplications_str, "", "");

    if (!(sums_str[0] == '+' || sums_str[0] == '-')) sums_str = "+" + sums_str;

    return ExpressionResult(start_with_plus ? multiplications_str.substr(1) : multiplications_str, string(1, sums_str[0]), sums_str.substr(1));
}

string organized_operation_to_string(ExpressionResult organized_operation) {
    return organized_operation.multiplications + organized_operation.operation + organized_operation.sums;
}

int add_operation(ParenthesisData *&parenthesis_data, int &key, string &number, sregex_iterator &iter_numbers, string &expression, string &operation, int depth, int retFlag = 1) {
    insert(parenthesis_data->tree, key, Data(number));
    size_t operation_index = iter_numbers->position() + iter_numbers->length();
    if (operation_index >= expression.length()) return 2;
    operation = expression[operation_index];
    ++key;
    insert(parenthesis_data->tree, key, Data(operation, true));
    return 1;
}

bool is_letter(const string &letter) {
    regex letters_regex(R"([a-zA-Z]+)");
    return regex_match(letter, letters_regex);
}

int create_sub_trees(ParenthesisData *&parenthesis_data, int key = 0, int depth = 0) {
    regex pattern_numbers(R"([a-zA-Z]+|\d+\.?\d*)");
    string expression = parenthesis_data->expression;
    bool first_number_is_negative = expression[0] == '-';
    sregex_iterator iter_numbers(expression.begin(), expression.end(), pattern_numbers), end;
    string first_number = (first_number_is_negative ? "-" : "") + iter_numbers->str();
    string operation = "";
    int ret_flag = 1;
    if (is_letter(first_number)) {
        key = create_sub_trees(parenthesis_data->next_parenthesis[first_number], key + 1, depth + 1);
        ret_flag = add_operation(parenthesis_data, key, first_number, iter_numbers, expression, operation, depth);
        if (ret_flag == 2) return key;
    } else {
        ret_flag = add_operation(parenthesis_data, key, first_number, iter_numbers, expression, operation, depth);
        if (ret_flag == 2) return key;
    }
    ++iter_numbers;
    key++;
    for (key = key; iter_numbers != end; ++key, ++iter_numbers) {
        string number = iter_numbers->str();
        if (is_letter(number)) {
            key = create_sub_trees(parenthesis_data->next_parenthesis[number], key + 1, depth + 1);
            ret_flag = add_operation(parenthesis_data, key, number, iter_numbers, expression, operation, depth);
            if (ret_flag == 2) {
                key++;
                break;
            }
        } else {
            ret_flag = add_operation(parenthesis_data, key, number, iter_numbers, expression, operation, depth);
            if (ret_flag == 2) {
                key++;
                break;
            }
        }
    }
    return key;
}

ParenthesisData *separate_by_parenthesis(const string &expression, ParenthesisData *parenthesis_data = nullptr, const string &letter = "A", int multiplier = 1) {
    ParenthesisData *new_data = parenthesis_data;
    if (new_data == nullptr) new_data = new ParenthesisData();
    string current_expression = "", next_letter = letter;

    size_t start = 0;
    while (start < expression.length()) {
        size_t first_paren = expression.find('(', start);

        if (first_paren == string::npos) {
            // No more parentheses, add remaining expression
            if (start < expression.length()) {
                current_expression += expression.substr(start);
            }
            break;
        }

        // Add content before parenthesis
        if (first_paren > start) {
            current_expression += expression.substr(start, first_paren - start);
        }

        // Find matching closing parenthesis
        int level = 1;
        size_t last_paren = first_paren + 1;
        while (last_paren < expression.length() && level > 0) {
            if (expression[last_paren] == '(')
                level++;
            else if (expression[last_paren] == ')')
                level--;
            last_paren++;
        }

        if (level > 0) {
            // Unmatched parenthesis, treat as regular character
            current_expression += expression.substr(first_paren, 1);
            start = first_paren + 1;
            continue;
        }

        // Extract content inside parentheses
        string inside_parentheses = expression.substr(first_paren + 1, last_paren - first_paren - 2);

        // Add letter placeholder and create sub-parenthesis
        new_data->expression += current_expression + next_letter;
        new_data->next_parenthesis[next_letter] = separate_by_parenthesis(inside_parentheses);

        current_expression = "";
        next_letter = get_next_letter(next_letter, multiplier);
        if (next_letter == "Z") multiplier++;
        start = last_paren;
    }

    if (!current_expression.empty()) new_data->expression += current_expression;
    if (parenthesis_data != nullptr) return new_data;
    ExpressionResult new_organized_expression = organize_expression(new_data->expression);
    string organized_operation_str = organized_operation_to_string(new_organized_expression);
    if (organized_operation_str.find("(") != string::npos) {
        new_data->deleteAll();
        new_data = new ParenthesisData();
        new_data = separate_by_parenthesis(organized_operation_str, new_data);
        return new_data;
    }
    new_data->expression = organized_operation_str;
    return new_data;
}

bool is_mul(const string &value) { return value == "*" || value == "/"; }
bool is_sum(const string &value) { return value == "+" || value == "-"; }

void navigate_inside_tree(Node<Data> *&node, ParenthesisData *parenthesis_data, Node<Data> *saved = nullptr) {
    if (node != nullptr) {
        navigate_inside_tree(node->left, parenthesis_data, saved);
        if (node->left) {
            if (is_letter(node->left->data.value)) {
                string letter = node->left->data.value;
                delete node->left;
                parenthesis_data->next_parenthesis[letter]->tree.root->data.is_parenthesis = true;
                node->left = parenthesis_data->next_parenthesis[letter]->tree.root;
            }
        }
        if (node->right) {
            if (is_letter(node->right->data.value)) {
                string letter = node->right->data.value;
                delete node->right;
                parenthesis_data->next_parenthesis[letter]->tree.root->data.is_parenthesis = true;
                node->right = parenthesis_data->next_parenthesis[letter]->tree.root;
            }
        }
        navigate_inside_tree(node->right, parenthesis_data, saved);
    }
}

void navigate_inside_tree(AVLTree<Data> &tree, ParenthesisData *parenthesis_data) { navigate_inside_tree(tree.root, parenthesis_data); }

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

template <typename TYPE>
void print_tree(AVLTree<TYPE> t, bool withKey = false) {
    print_tree(t.root, withKey);
}

// template <typename Data>
void print_tree(Node<Data> *node, bool withKey, const string &prefix = "", bool isLeft = true) {
    if (node == nullptr) return;

    // Print current node
    cout << prefix;

    cout << (isLeft ? "├── " : "└── ");

    if (withKey)
        cout << node->key << endl;
    else if (node->data.is_parenthesis)
        cout << "(" << node->data.value << ")" << endl;
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