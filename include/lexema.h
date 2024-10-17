#pragma once

#include <string>

enum TypeLex { number, binOp, unOp, leftB, rightB };

struct Lexema {
	TypeLex type;
	long long value;
	std::string name;
	int priority;
};