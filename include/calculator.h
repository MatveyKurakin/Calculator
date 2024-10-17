#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include "lexema.h"

using namespace std;

class Calculator {
	
	enum class Status { wait, numberSt, binOp, unOp, leftB, func };

	vector<Lexema> lexems;
	int countBoard = 0;

	Lexema getLexems(string input, int pos, int& nextPos)
	{
		Lexema l;
		if (input[pos] - '0' >= 0 && input[pos] - '0' <= 9)
		{
			long long num = 0;
			while (input[pos] - '0' >= 0 && input[pos] - '0' <= 9 && pos < input.size())
			{
				num *= 10;
				num += input[pos] - '0';
				pos++;
			}
			nextPos = pos;
			l.type = TypeLex::number;
			l.value = num;
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
		else
		{
			throw("errorName");
		}
		return l;
	}

	void checkInput() {
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
			}
			else if (currentSt == Status::numberSt) {
				if (type == TypeLex::binOp) currentSt = Status::binOp;
				else if (type == TypeLex::rightB) { currentSt = Status::numberSt; countBoard--; }
				else if (type == TypeLex::leftB) throw("errorLB");
			}
			else if (currentSt == Status::binOp) {
				if (type == TypeLex::number) currentSt = Status::numberSt;
				else if (type == TypeLex::leftB) { currentSt = Status::leftB; countBoard++; }
				else if (type == TypeLex::rightB) throw("errorRB");
				else if (type == TypeLex::binOp) throw("errorBinOp");
			}
			else if (currentSt == Status::leftB) {
				if (type == TypeLex::number) currentSt = Status::numberSt;
				else if (type == TypeLex::leftB) { currentSt = Status::leftB; countBoard++; }
				else if (type == TypeLex::rightB) throw("errorB");
				else if (type == TypeLex::binOp) throw("errorBinOp");
				else if (type == TypeLex::unOp) currentSt = Status::wait;
			}
			else if (currentSt == Status::func) {
				if (type == TypeLex::number) throw("errorNumber");
				else if (type == TypeLex::leftB) { currentSt = Status::leftB; countBoard++; }
				else if (type == TypeLex::rightB) throw("errorRB");
				else if (type == TypeLex::binOp) throw("errorBinOp");
				else if (type == TypeLex::unOp) throw("errorUnOp");
			}

			if (countBoard < 0) throw("errorCountB-");
		}
		if (countBoard != 0) throw("errorCountB+");
	}

	long long getBinOperation(long long x1, long long x2, Lexema l)
	{
		if (l.name == "*") return x1 * x2;
		else if (l.name == "/") return x1 / x2;
		else if (l.name == "+") return x1 + x2;
		else if (l.name == "-") return x1 - x2;
		else if (l.name == "%") return x1 % x2;
	}

public:

	Calculator(string input) {

		int startPos = 0;
		int endPos = 0;

		while (endPos < input.size())
		{
			lexems.push_back(getLexems(input, startPos, endPos));
			startPos = endPos;
		}

		checkInput();
	}

	long long countingLexems() {
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
		}
		while (!operation.empty())
		{
			pExpression.push(operation.top());
			operation.pop();
		}
		stack<long long> result;
		while (!pExpression.empty())
		{
			if (pExpression.front().type == TypeLex::number)
			{
				result.push((long long)pExpression.front().value);
			}
			else if (pExpression.front().type == TypeLex::binOp)
			{
				long long x1 = result.top();
				result.pop();
				long long x2 = result.top();
				result.pop();
				result.push(getBinOperation(x2, x1, pExpression.front()));
			}
			pExpression.pop();
		}
		return result.top();
	}

};




