#pragma once

#include <variant>
#include <string>

enum TypeLex { number, binOp, unOp, leftB, rightB, func, var };

struct Lexema {
	TypeLex type;
	std::variant<long long, double> value;
	std::string name;
	int priority;
};