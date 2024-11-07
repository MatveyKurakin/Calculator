#pragma once

#include <iostream>
#include <variant>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <set>
#include <map>
#include <cmath>
#include "lexema.h"

using namespace std;

class Calculator {

	enum class Status { wait, numberSt, binOp, unOp, leftB, func };

	vector<Lexema> lexems;
	std::set<std::string> var_name;
	std::map<std::string, std::variant<long long, double>> variable;
	int countBoard = 0;

	Lexema getLexems(string input, int pos, int& nextPos)
	{
		Lexema l;

		while (input[pos] == ' ') {
			++pos;
		}

		if (input[pos] - '0' >= 0 && input[pos] - '0' <= 9)
		{
			double num = 0;
			while (input[pos] - '0' >= 0 && input[pos] - '0' <= 9 && pos < input.size())
			{
				num *= 10;
				num += input[pos] - '0';
				pos++;
			}
			if (input[pos] == '.')
			{
				double cnt = 0.1;
				pos++;
				while (input[pos] - '0' >= 0 && input[pos] - '0' <= 9 && pos < input.size())
				{
					num += (input[pos] - '0') * cnt;
					cnt *= 0.1;
					pos++;
				}
				l.value = static_cast<double>(num);
			}
			else {
				l.value = static_cast<long long>(num);
			}
			nextPos = pos;
			l.type = TypeLex::number;
		}
		else if (input[pos] == '+')
		{
			nextPos = pos + 1;
			l.type = TypeLex::binOp;
			l.name = "+";
			l.priority = 0;
		}
		else if (input[pos] == '-')
		{
			if (pos == 0 || input[pos - 1] == '(') {
				nextPos = pos + 1;
				l.type = TypeLex::unOp;
				l.name = "-";
			}
			else
			{
				nextPos = pos + 1;
				l.type = TypeLex::binOp;
				l.name = "-";
				l.priority = 1;
			}
		}
		else if (input[pos] == '*')
		{
			nextPos = pos + 1;
			l.type = TypeLex::binOp;
			l.name = "*";
			l.priority = 2;
		}
		else if (input[pos] == '/')
		{
			nextPos = pos + 1;
			l.type = TypeLex::binOp;
			l.name = "/";
			l.priority = 2;
		}
		else if (input[pos] == '%')
		{
			nextPos = pos + 1;
			l.type = TypeLex::binOp;
			l.name = "%";
			l.priority = 2;
		}
		else if (input[pos] == '(')
		{
			nextPos = pos + 1;
			l.type = TypeLex::leftB;
		}
		else if (input[pos] == ')')
		{
			nextPos = pos + 1;
			l.type = TypeLex::rightB;
		}
		else if (pos + 2 < input.size() && input[pos] == 'e' && input[pos + 1] == 'x' && input[pos + 2] == 'p')
		{
			nextPos = pos + 3;
			l.type = TypeLex::func;
			l.name = "exp";
			l.priority = 3;
		}
		else if (pos + 2 < input.size() && input[pos] == 's' && input[pos + 1] == 'i' && input[pos + 2] == 'n')
		{
			nextPos = pos + 3;
			l.type = TypeLex::func;
			l.name = "sin";
			l.priority = 3;
		}
		else if (pos + 2 < input.size() && input[pos] == 'c' && input[pos + 1] == 'o' && input[pos + 2] == 's')
		{
			nextPos = pos + 3;
			l.type = TypeLex::func;
			l.name = "cos";
			l.priority = 3;
		}
		else if (pos + 1 < input.size() && input[pos] == 'l' && input[pos + 1] == 'g')
		{
			nextPos = pos + 2;
			l.type = TypeLex::func;
			l.name = "lg";
			l.priority = 3;
		}
		else if (pos + 3 < input.size() && input[pos] == 's' && input[pos + 1] == 'q' && input[pos + 2] == 'r' && input[pos + 3] == 't')
		{
			nextPos = pos + 4;
			l.type = TypeLex::func;
			l.name = "sqrt";
			l.priority = 3;
		}
		else if ('a' <= input[pos] && input[pos] <= 'z' || 'A' <= input[pos] && input[pos] <= 'Z' || input[pos] == '_') {
			l.type = TypeLex::var;
			std::string name;
			while ('a' <= input[pos] && input[pos] <= 'z' || 'A' <= input[pos] && input[pos] <= 'Z' || input[pos] == '_' || '0' <= input[pos] && input[pos] <= '9') {
				name += input[pos];
				pos++;
			}
			l.name = name;
			nextPos = pos;
			var_name.insert(name);
		}
		else
		{
			throw("errorName");
		}
		return l;
	}

	void checkInput()
	{
		Status currentSt = Status::wait;

		for (int i = 0; i < lexems.size(); i++)
		{
			TypeLex type = lexems[i].type;
			if (currentSt == Status::wait) {
				if (type == TypeLex::number) currentSt = Status::numberSt;
				else if (type == TypeLex::leftB) { currentSt = Status::leftB; countBoard++; }
				else if (type == TypeLex::rightB) throw("errorRB");
				else if (type == TypeLex::binOp) throw("errorBinOp");
				else if (type == TypeLex::unOp) currentSt = Status::wait;
				else if (type == TypeLex::func) currentSt = Status::func;
				else if (type == TypeLex::var) currentSt = Status::numberSt;
			}
			else if (currentSt == Status::numberSt) {
				if (type == TypeLex::binOp) currentSt = Status::binOp;
				else if (type == TypeLex::rightB) { currentSt = Status::numberSt; countBoard--; }
				else if (type == TypeLex::leftB) throw("errorLB");
				else if (type == TypeLex::func) throw("errorFunc");
				else if (type == TypeLex::number) throw("errorNumWithoutOp");
				else if (type == TypeLex::var) throw("errorVarWithoutOp");
			}
			else if (currentSt == Status::binOp) {
				if (type == TypeLex::number) currentSt = Status::numberSt;
				else if (type == TypeLex::leftB) { currentSt = Status::leftB; countBoard++; }
				else if (type == TypeLex::rightB) throw("errorRB");
				else if (type == TypeLex::binOp) throw("errorBinOp");
				else if (type == TypeLex::func) currentSt = Status::wait;
				else if (type == TypeLex::var) currentSt = Status::numberSt;
			}
			else if (currentSt == Status::leftB) {
				if (type == TypeLex::number) currentSt = Status::numberSt;
				else if (type == TypeLex::leftB) { currentSt = Status::leftB; countBoard++; }
				else if (type == TypeLex::rightB) throw("errorB");
				else if (type == TypeLex::binOp) throw("errorBinOp");
				else if (type == TypeLex::unOp) currentSt = Status::wait;
				else if (type == TypeLex::func) currentSt = Status::wait;
				else if (type == TypeLex::var) currentSt = Status::numberSt;
			}
			else if (currentSt == Status::func) {
				if (type == TypeLex::number) throw("errorNumber");
				else if (type == TypeLex::leftB) { currentSt = Status::leftB; countBoard++; }
				else if (type == TypeLex::rightB) throw("errorRB");
				else if (type == TypeLex::binOp) throw("errorBinOp");
				else if (type == TypeLex::unOp) throw("errorUnOp");
				else if (type == TypeLex::func) throw("error");
				else if (type == TypeLex::var) throw("errorVar");
			}

			if (countBoard < 0) throw("errorCountB-");
		}
		if (countBoard != 0) throw("errorCountB+");
	}

	Lexema getBinOperation(Lexema& x1, Lexema& x2, Lexema& l)
	{
		Lexema out;
		out.type = TypeLex::number;

		if (holds_alternative<double>(x1.value) && holds_alternative<double>(x2.value)) {
			if (l.name == "*") {
				out.value = std::get<double>(x1.value) * std::get<double>(x2.value);
			}
			else if (l.name == "/") {
				out.value = std::get<double>(x1.value) / std::get<double>(x2.value);
			}
			else if (l.name == "+") {
				out.value = std::get<double>(x1.value) + std::get<double>(x2.value);
			}
			else if (l.name == "-") {
				out.value = std::get<double>(x1.value) - std::get<double>(x2.value);
			}
			else if (l.name == "%") {
				throw("op % for only int");
			}
		}
		else if (holds_alternative<double>(x1.value) && holds_alternative<long long>(x2.value)) {
			if (l.name == "*") {
				out.value = std::get<double>(x1.value) * std::get<long long>(x2.value);
			}
			else if (l.name == "/") {
				out.value = std::get<double>(x1.value) / std::get<long long>(x2.value);
			}
			else if (l.name == "+") {
				out.value = std::get<double>(x1.value) + std::get<long long>(x2.value);
			}
			else if (l.name == "-") {
				out.value = std::get<double>(x1.value) - std::get<long long>(x2.value);
			}
			else if (l.name == "%") {
				throw("op % for only int");
			}
		}
		else if (holds_alternative<long long>(x1.value) && holds_alternative<double>(x2.value)) {
			if (l.name == "*") {
				out.value = std::get<long long>(x1.value) * std::get<double>(x2.value);
			}
			else if (l.name == "/") {
				out.value = std::get<long long>(x1.value) / std::get<double>(x2.value);
			}
			else if (l.name == "+") {
				out.value = std::get<long long>(x1.value) + std::get<double>(x2.value);
			}
			else if (l.name == "-") {
				out.value = std::get<long long>(x1.value) - std::get<double>(x2.value);
			}
			else if (l.name == "%") {
				throw("op % for only int");
			}
		}
		else if (holds_alternative<long long>(x1.value) && holds_alternative<long long>(x2.value)) {
			if (l.name == "*") {
				out.value = std::get<long long>(x1.value) * std::get<long long>(x2.value);
			}
			else if (l.name == "/") {
				if (std::get<long long>(x2.value) == 0)
					throw("errorDivByZero");
				out.value = std::get<long long>(x1.value) / std::get<long long>(x2.value);
			}
			else if (l.name == "+") {
				out.value = std::get<long long>(x1.value) + std::get<long long>(x2.value);
			}
			else if (l.name == "-") {
				out.value = std::get<long long>(x1.value) - std::get<long long>(x2.value);
			}
			else if (l.name == "%") {
				out.value = std::get<long long>(x1.value) % std::get<long long>(x2.value);
			}
		}
		return out;
	}

	Lexema getFunc(Lexema& x, Lexema l)
	{
		Lexema out;
		out.type = TypeLex::number;

		if (holds_alternative<double>(x.value)) {
			if (l.name == "exp") {
				out.value = exp(std::get<double>(x.value));
			}
			else if (l.name == "sin") {
				out.value = sin(std::get<double>(x.value));
			}
			else if (l.name == "cos") {
				out.value = cos(std::get<double>(x.value));
			}
			else if (l.name == "lg") {
				out.value = log(std::get<double>(x.value)) / log(10);
			}
			else if (l.name == "sqrt") {
				out.value = sqrt(std::get<double>(x.value));
			}
		}
		else if (holds_alternative<long long>(x.value)) {
			if (l.name == "exp") {
				out.value = exp(std::get<long long>(x.value));
			}
			else if (l.name == "sin") {
				out.value = sin(std::get<long long>(x.value));
			}
			else if (l.name == "cos") {
				out.value = cos(std::get<long long>(x.value));
			}
			else if (l.name == "lg") {
				out.value = log(std::get<long long>(x.value)) / log(10);
			}
			else if (l.name == "sqrt") {
				out.value = sqrt(std::get<long long>(x.value));
			}
		}

		return out;
	}

public:

	Calculator(string input)
	{

		int startPos = 0;
		int endPos = 0;

		while (endPos < input.size())
		{
			lexems.push_back(getLexems(input, startPos, endPos));
			startPos = endPos;
		}

		checkInput();
	}

	std::variant<long long, double> countingLexems()
	{
		queue<Lexema> pExpression;
		stack<Lexema> operation;
		for (int i = 0; i < lexems.size(); i++)
		{
			if (lexems[i].type == TypeLex::number)
			{
				pExpression.push(lexems[i]);
			}
			else if (lexems[i].type == TypeLex::binOp)
			{
				while (!operation.empty() && operation.top().type != TypeLex::leftB) {
					if (operation.top().priority >= lexems[i].priority) {
						pExpression.push(operation.top());
						operation.pop();
					}
					else {
						break;
					}
				}
				operation.push(lexems[i]);
			}
			else if (lexems[i].type == TypeLex::unOp)
			{
				Lexema l;
				l.type = TypeLex::number;
				l.value = -1;
				pExpression.push(l);
				l.type = TypeLex::binOp;
				l.name = "*";
				l.priority = 2;
				operation.push(l);
			}
			else if (lexems[i].type == TypeLex::leftB)
			{
				operation.push(lexems[i]);
			}
			else if (lexems[i].type == TypeLex::rightB)
			{
				while (operation.top().type != TypeLex::leftB)
				{
					pExpression.push(operation.top());
					operation.pop();
				}
				operation.pop();
			}
			else if (lexems[i].type == TypeLex::func)
			{
				operation.push(lexems[i]);
			}
			else if (lexems[i].type == TypeLex::var)
			{
				if (variable.contains(lexems[i].name)) {
					Lexema l;
					l.type = TypeLex::number;
					l.value = variable[lexems[i].name];
					pExpression.push(l);
				}
				else {
					throw("errorVarName");
				}
			}
		}
		while (!operation.empty())
		{
			pExpression.push(operation.top());
			operation.pop();
		}
		stack<Lexema> result;
		while (!pExpression.empty())
		{
			if (pExpression.front().type == TypeLex::number)
			{
				result.push(pExpression.front());
			}
			else if (pExpression.front().type == TypeLex::binOp)
			{
				Lexema x1 = result.top();
				result.pop();
				Lexema x2 = result.top();
				result.pop();
				result.push(getBinOperation(x2, x1, pExpression.front()));
			}
			else if (pExpression.front().type == TypeLex::func)
			{
				Lexema x = result.top();
				result.pop();
				result.push(getFunc(x, pExpression.front()));
			}
			pExpression.pop();
		}
		return result.top().value;
	}

	void getVarName(std::vector<std::string>& vec)
	{
		vec = std::vector<std::string>(var_name.begin(), var_name.end());
	}

	void setVarValue(std::vector<std::pair<std::string, std::variant<long long, double>>>& vec)
	{
		if (vec.size() != var_name.size()) {
			throw("errorNotEnoughVar");
		}
		int ind = 0;
		for (int i = 0; i < vec.size(); i++) {
			variable[vec[i].first] = vec[i].second;
		}
	}
};


std::variant<long long, double> strToVar(std::string& input) 
{
	std::variant<long long, double> ans;

	int pos = 0;

	while (input[pos] == ' ') {
		++pos;
	}

	bool neg = 0;
	if (input[pos] == '-') {
		neg = 1;
		++pos;
	}

	if (input[pos] - '0' >= 0 && input[pos] - '0' <= 9)
	{
		double num = 0;
		while (input[pos] - '0' >= 0 && input[pos] - '0' <= 9 && pos < input.size())
		{
			num *= 10;
			num += input[pos] - '0';
			pos++;
		}
		if (input[pos] == '.')
		{
			double cnt = 0.1;
			pos++;
			while (input[pos] - '0' >= 0 && input[pos] - '0' <= 9 && pos < input.size())
			{
				num += (input[pos] - '0') * cnt;
				cnt *= 0.1;
				pos++;
			}
			if (neg) num *= (-1);
			ans = static_cast<double>(num);
		}
		else {
			if (neg) num *= (-1);
			ans = static_cast<long long>(num);
		}
	}
	else
	{
		throw("errorVarValue");
	}
	return ans;
}
