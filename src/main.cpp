#include <iostream>
#include <variant>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include "calculator.h"
#include "lexema.h"

using namespace std;

int main() {
	string input;
	std::variant<long long, double> result = 0;

	getline(cin, input);

	try {
		Calculator calculator(input);

		std::vector<std::string> var_name;
		calculator.getVarName(var_name);

		string str_var_value;

		std::vector<std::pair<std::string, std::variant<long long, double>>> var_value(var_name.size());
		for (int i = 0; i < var_name.size(); i++) {
			std::cout << var_name[i] << " = ";
			var_value[i].first = var_name[i];

			getline(cin, str_var_value);
			var_value[i].second = strToVar(str_var_value);
		}
		calculator.setVarValue(var_value);

		result = calculator.countingLexems();
	}
	catch (const char* a) {
		cout << a;
		return 0;
	}
	if (holds_alternative<long long>(result)) {
		cout << std::get<long long>(result) << " long long";
	}
	else if (holds_alternative<double>(result)) {
		cout << std::get<double>(result) << " double";
	}
	return 0;
}