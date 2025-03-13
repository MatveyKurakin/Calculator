#pragma once

#include <Polynom.h>

#include <string>
#include <variant>

enum TypeLex { number, binOp, unOp, leftB, rightB, func, var };

struct Lexema {
  TypeLex type;
  std::variant<long long, double, Polynom> value;
  std::string name;
  int priority;
};