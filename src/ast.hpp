/*
MIT License

Copyright (c) 2022 Ruslan Popov <ruslanpopov1512@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef AST_HPP
#define AST_HPP

#include "program.hpp"

struct expr
{
	virtual bool eval(scope const& context) const = 0;
	virtual ~expr() {}
};

struct literal : expr
{
private:
	token_t original;
public:
	literal(token str) : original(str) {}
	bool eval(scope const& context) const
	{
		try
		{
			return context.at(original.str);
		}
		catch (out_of_range const& e)
		{
			throw literal_not_found(original.str, original.pos);
		}
	}
};

struct bin_operation : expr
{
private:
	unique_ptr<expr> rightExpr;
	unique_ptr<expr> leftExpr;
	token_t operation;
public:
	bin_operation(token operation, unique_ptr<expr> leftExpr, unique_ptr<expr> rightExpr) : leftExpr(move(leftExpr)), rightExpr(move(rightExpr)), operation(operation) {}

	bool eval(scope const& context) const
	{
		bool right = rightExpr->eval(context);
		bool left = leftExpr->eval(context);
	
		switch(operation.type)
		{
		case AND:
			return right and left;
		case OR:
			return right or left;
		case XOR:
			return right xor left;
		default:
			return false;
		}
	}
};

struct negative : expr
{
private:
	unique_ptr<expr> expression;
public:
	negative(unique_ptr<expr> expression) : expression(move(expression)) {}

	bool eval(scope const& context) const
	{
		return !(expression->eval(context));
	}
};

class parser
{
private:
	size_t parserPos;
	size_t parserCodePos;
	vector<token_t> const& tokens;
	token_t nullToken;

	token match(token_type expected);
	token match(vector<token_type_t> const& expected);
	token require(token_type expected);

	unique_ptr<expr> parseUnary();
	unique_ptr<expr> parseXor();
	unique_ptr<expr> parseAnd();
	unique_ptr<expr> parseAdd();
public:
	parser(vector<token_t> const& tokens) : parserPos(0), parserCodePos(0), tokens(tokens), nullToken() {}

	unique_ptr<expr> parse();
};

#endif
