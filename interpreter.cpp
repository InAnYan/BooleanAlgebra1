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

#include "program.hpp"
#include "ast.hpp"

vector<token_t> tokenizer(string const& prompt, scope& literals)
{
	vector<token_t> tokens;
	for(size_t i = 0; i < prompt.size(); ++i)
	{
		if(prompt[i] == ' ') continue;
		else if(prompt[i] == '+') tokens.push_back(token_t( OR, "+", i ));
		else if(prompt[i] == '*' || prompt[i] == '&') tokens.push_back(token_t( AND, "*", i ));	
		else if(prompt[i] == '^') tokens.push_back(token_t( XOR, "^", i ));
		else if(prompt[i] == '!' || prompt[i] == '-' || prompt[i] == '~') tokens.push_back(token_t( NEGATIVE, "-", i ));
		else if(prompt[i] == '(') tokens.push_back(token_t( PAR_OPEN, "(", i ));
		else if(prompt[i] == ')') tokens.push_back(token_t( PAR_CLOSE, ")", i ));
		else if(isalpha(prompt[i]))
		{
			stringstream buf;
			buf << prompt[i]; i++;
			while(isdigit(prompt[i])){ buf << prompt[i]; i++;}
			i--;
			tokens.push_back(token_t( LITERAL, buf.str(), i ));
			literals[buf.str()] = false;
		}
		else
		{
			throw tokenizer_exception(string(1, prompt[i]), i);
		}
	}
	return tokens;
}

token parser::match(token_type expected)
{
	if(parserPos < tokens.size())
	{
		token currentToken = tokens[parserPos];
		if(currentToken.type == expected)
		{
			parserPos++;
			parserCodePos = currentToken.pos;
			return currentToken;
		}
		else return nullToken;
	}
	else return nullToken;
}

token parser::match(vector<token_type_t> const& expected)
{
	if(parserPos < tokens.size())
	{
		token currentToken = tokens[parserPos];
		if(find(expected.begin(), expected.end(), currentToken.type) != expected.end())
		{
			parserPos++;
			parserCodePos = currentToken.pos;
			return currentToken;
		}
		else return nullToken;
	}
	else return nullToken;
}


token parser::require(token_type expected)
{
	token currentToken = match(expected);
	if(currentToken.type == NONE)
		throw parser_expected_exception(expected, parserCodePos);
	else return currentToken;
}

unique_ptr<expr> parser::parseUnary()
{
	if(match(NEGATIVE).type != NONE)
	{
		return move(make_unique<negative>(parseUnary()));
	}
	else if(match(PAR_OPEN).type != NONE)
	{
		unique_ptr<expr> t = move(parseAdd());
		require(PAR_CLOSE);
		return move(t);
	}
	else
	{
		return move(make_unique<literal>(require(LITERAL)));
	}
}

unique_ptr<expr> parser::parseAnd()
{
	unique_ptr<expr> left = parseUnary();
	token_t operation = match(AND);
	while(operation.type != NONE)
	{
		unique_ptr<expr> right = parseUnary();
		left = make_unique<bin_operation>(operation, move(left), move(right));
		operation = match(AND);
	}
	return move(left);
}

unique_ptr<expr> parser::parseXor()
{
	unique_ptr<expr> left = parseAnd();
	token_t operation = match(XOR);
	while (operation.type != NONE)
	{
		unique_ptr<expr> right = parseAnd();
		left = make_unique<bin_operation>(operation, move(left), move(right));
		operation = match(XOR);
	}
	return move(left);
}

unique_ptr<expr> parser::parseAdd()
{
	unique_ptr<expr> left = parseXor();
	token_t operation = match(OR);
	while(operation.type != NONE)
	{
		unique_ptr<expr> right = parseXor();
		left = make_unique<bin_operation>(operation, move(left), move(right));
		operation = match(OR);
	}
	return move(left);
}

unique_ptr<expr> parser::parse()
{
	return move(parseAdd());
}
