#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include "calculator.h"
#include <variant>
#include "lexema.h"

using namespace std;

int main() {
	string input;
	double result = 0;

	cin >> input;

	try {
		Calculator calculator(input);

		std::vector<std::string> var_name;
		calculator.getVarName(var_name);

		std::vector<std::pair<std::string,double>> var_value(var_name.size());
		for (int i = 0; i < var_name.size(); i++) {
			std::cout << var_name[i] << " = ";
			var_value[i].first = var_name[i];
			std::cin >> var_value[i].second;
		}
		calculator.setVarValue(var_value);

		result = calculator.countingLexems();
	}
	catch (const char* a) {
		cout << a;
		return 0;
	}
	cout << result;
	return 0;
}