#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;
double convert(const string &str) {
    try {
        return stod(str);
    } catch (const invalid_argument &e) {
        throw invalid_argument("Invalid double string: " + str);
    } catch (const out_of_range &e) {
        throw out_of_range("Double out of range: " + str);
    }
}
int main() {
    try {
        double result = convert("1.2.3");
        cout << "Result: " << result << endl;
        cout << "No exception thrown" << endl;
    } catch (const exception &e) {
        cout << "Exception caught: " << e.what() << endl;
    }
    return 0;
}
