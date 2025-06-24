#include <iostream>
#include <vector>

using namespace std;

bool opLv1(const char &c) { return c == '-' || c == '+'; }
bool opLv2(const char &c) { return c == '*' || c == '/'; }

vector<string> createOperations(const string &testString) {
    vector<string> operations;
    bool completed = false;
    bool half = false;
    for (int i = 0; testString[i] != '\0'; i++) {
        string operation = testString.substr(i, 3);
        int rest = testString.length() - i + 1;
        if (rest <= 3) {
            operation = testString.substr(i, rest);
            i += rest - 1;
        } else if (completed && opLv1(testString[i])) {
            operation = testString.substr(i, 1);
            completed = false;
        } else if (completed) {
            operation = testString.substr(i, 2);
            i++;
        } else if (opLv1(operation[1]) && opLv2(testString[i + 3])) {
            operation = testString.substr(i, 2);
            i++;
        } else {
            completed = true;
            i += 2;
        }
        operations.push_back(operation);
    }

    return operations;
}

int main() {
    const string strForTest = "4+9+1-8*3/2+5+8-4*2+9/3-7+3+5*2-8";
    cout << strForTest << endl;
    vector<string> operations = createOperations(strForTest);
    for (const string &operation : operations) cout << operation << endl;
}
