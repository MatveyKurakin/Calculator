#pragma once

#include <string>

enum TypeLex { number, binOp, unOp, leftB, rightB, func, var };

struct Lexema {
	TypeLex type;
	double value;
	std::string name;
	int priority;
};