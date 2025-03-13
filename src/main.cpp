#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "calculator.h"
#include "lexema.h"
#include "polynom.h"

int main() {
  std::string input;
  std::variant<long long, double, Polynom> result = static_cast<long long>(0);

  getline(std::cin, input);

  try {
    Calculator calculator(input);

    std::vector<std::string> var_name;
    calculator.getVarName(var_name);

    std::string str_var_value;

    std::vector<std::pair<std::string, std::variant<long long, double, Polynom>>> var_value(var_name.size());
    for (int i = 0; i < var_name.size(); i++) {
      std::cout << var_name[i] << " = ";
      var_value[i].first = var_name[i];

      getline(std::cin, str_var_value);
      var_value[i].second = strToVar(str_var_value);
    }
    calculator.setVarValue(var_value);

    result = calculator.countingLexems();
  } catch (const char *a) {
    std::cout << a;
    return 0;
  }
  if (std::holds_alternative<long long>(result)) {
    std::cout << std::get<long long>(result) << " long long\n";
  } else if (std::holds_alternative<double>(result)) {
    std::cout << std::get<double>(result) << " double\n";
  } else if (std::holds_alternative<Polynom>(result)) {
    std::get<Polynom>(result).print();
    std::cout << "Polynom\n";
  }
  return 0;
}