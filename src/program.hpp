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

#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

enum token_type_t { NONE, LITERAL, NEGATIVE, AND, OR, XOR, PAR_OPEN, PAR_CLOSE };
typedef token_type_t const& token_type;

typedef map<string, bool> scope;

struct token_t
{
	token_type_t type;
	string str;
	size_t pos;
	token_t() : type(NONE), str(""), pos(0) {}
	token_t(token_type type, string str, size_t pos) : type(type), str(str), pos(pos) {}
};
typedef token_t const& token;

struct literal_not_found : exception
{
private:
	string name;
	size_t pos;
public:
	literal_not_found(string name, size_t position) : name(name), pos(position) {}

	const char* what() const throw() { return name.c_str(); }
	size_t where() const throw() { return pos; }
};

struct tokenizer_exception : exception
{
private:
	string name;
	size_t pos;
public:
	tokenizer_exception(string name, size_t position) : name(name), pos(position) {}

	const char* what() const throw() { return name.c_str(); }
	size_t where() const throw() { return pos; }
};

struct parser_expected_exception : exception
{
private:
	token_type_t type;
	size_t pos;
	string typeName;
public:
	parser_expected_exception(token_type type, size_t position) : type(type), pos(position)
	{
		switch(type)
		{
		case LITERAL:
			typeName = "LITERAL";
			break;
		case NEGATIVE:
			typeName = "NEGATIVE";
			break;
		case AND:
			typeName = "AND";
			break;
		case OR:
			typeName = "OR";
			break;
		case XOR:
			typeName = "XOR";
			break;
		case PAR_OPEN:
			typeName = "PARENTHESIS_OPEN";
			break;
		case PAR_CLOSE:
			typeName = "PARENTHESIS_CLOSE";
			break;
		case NONE:
			typeName = "NONE";
			break;
		}
	}

	const char* what() const throw() { return typeName.c_str(); }
	size_t where() const throw() { return pos; }
};

struct parser_end_exception : exception
{};

vector<token_t> tokenizer(string const& prompt, scope& context);

#endif
