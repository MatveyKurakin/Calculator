#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include "calculator.h"
#include "lexema.h"

using namespace std;

int main() {
	string input;
	double result = 0;

	cin >> input;

	try {
		Calculator calculator(input);
		result = calculator.countingLexems();
	}
	catch (const char* a) {
		cout << a;
		return 0;
	}
	cout << result;
	return 0;
}