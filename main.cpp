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

void tableOP();
void compareOP();

int main(int argc, char** argv)
{
	cout << "Boolean function truth table generator" << endl
		 << "Copyright(C) 2022 Ruslan Popov <ruslanpopov1512@gmail.com>\n" << endl;

	bool running = true;
	string prompt;
	do
	{
		cout << "Enter operation:" << endl
			 << "1. Generate truth table for one function" << endl
			 << "2. Compare two functions" << endl
			 << "3. Exit" << endl 
			 << "Enter number [1-3]: ";
		getline(cin, prompt);

		cout << endl;

		if(prompt == "3") running = false;
		else if(prompt == "1") tableOP();
		else if(prompt == "2") compareOP();
		else cout << "Wrong command" << endl;

		cout << endl;
		
	} while (running);
	
	cout << "Goodbye!" << endl;
	return 0;
}

void tableOP()
{
	vector<token_t> tokens;
	map<string, bool> context;
	unique_ptr<expr> expr1;
	string prompt;
	vector<vector<string>> map1;

	cout << "Enter function: ";
	getline(cin, prompt);

	try
	{
		// Generating function
		tokens = tokenizer(prompt, context);
		parser syntax(tokens);
		expr1 = move(syntax.parse());

		// First row contains var names
		vector<string> vars;
		int biggestVar = 0; // Used for setw argument
		for(auto i : context)
		{
			if (biggestVar < i.first.size()) biggestVar = i.first.size();
			vars.push_back(i.first);
		}
		if (biggestVar < 4) biggestVar = 4;
		vars.push_back("func");
		map1.push_back(vars);

		// Main rows
		for(int i = 0; i < pow(2.0f, (float)(vars.size()-1)); i++)
		{
			// vector<bool>
			vector<bool> res;
			for(int j = vars.size()-1 -1; j >=0 ; j--) // j - little-endian (additional -1 for "func" variable)
			{
				res.push_back(i & (0b1 << j));
				context[vars[(vars.size() - 1 - 1)-j]] = i & (0b1 << j);
			}
			res.push_back(expr1->eval(context));

			// Converting vector<bool> to vector<string>
			vector<string> res2;
			for(auto el : res)
			{
				if(el) res2.push_back("1");
				else res2.push_back("0");
			}
			map1.push_back(res2);
		}

		// Middle lines
		string sRow = string(biggestVar, '-') + "|";
		string fRow = "";
		for (int i = 0; i < vars.size(); i++) fRow += sRow;

		// Table drawing
		cout << endl << string(vars.size()*biggestVar+vars.size()+1, '-') << endl;
		for (int i = 0; i < map1.size(); i++)
		{
			cout << "|";
			for (auto j : map1[i])
			{
				cout << setw(biggestVar) << j << "|";
			}
			if(i != map1.size()-1) cout << endl << "|" << fRow << endl;
			else cout << endl << string(vars.size() * biggestVar + vars.size() + 1, '-') << endl;
		}
	}
	catch (tokenizer_exception const& e)
	{
		cout << "ERROR: There is no such operation as \'" << e.what() << "\'" << "\n\t" << prompt << "\n\t" << string(e.where(), ' ') << "^" << endl;
	}
}

void compareOP()
{
	vector<vector<string>> map1;
	vector<vector<string>> map2;
	string prompt1, prompt2;
	map<string, bool> context;

	cout << "Functions must have same variables" << endl;
	cout << "Enter first function: ";
	getline(cin, prompt1);
	cout << "Enter second function: ";
	getline(cin, prompt2);

	bool dispFirst = true; // Used for try catch

	try
	{
		// Generating function
		vector<token_t> tokens1 = tokenizer(prompt1, context);
		parser syntax1(tokens1);
		unique_ptr<expr> expr1 = move(syntax1.parse());
		dispFirst = false;
		vector<token_t> tokens2 = tokenizer(prompt2, context);
		parser syntax2(tokens2);
		unique_ptr<expr> expr2 = move(syntax2.parse());

		// First row contains var names
		vector<string> vars;
		int biggestVar = 0; // Used for setw argument
		for (auto i : context)
		{
			if (biggestVar < i.first.size()) biggestVar = i.first.size();
			vars.push_back(i.first);
		}
		if (biggestVar < 4) biggestVar = 4;
		vars.push_back("func");
		map1.push_back(vars);
		map2.push_back(vars);

		// Main rows for first function
		for (int i = 0; i < pow(2.0f, (float)(vars.size() - 1)); i++)
		{
			// vector<bool>
			vector<bool> res;
			for (int j = vars.size() - 1 - 1; j >= 0; j--) // j - little-endian (additional -1 for "func" variable)
			{
				res.push_back(i & (0b1 << j));
				context[vars[(vars.size() - 1 - 1) - j]] = i & (0b1 << j);
			}
			res.push_back(expr1->eval(context));

			// Converting vector<bool> to vector<string>
			vector<string> res2;
			for (auto el : res)
			{
				if (el) res2.push_back("1");
				else res2.push_back("0");
			}
			map1.push_back(res2);
		}

		// For second function
		for (int i = 0; i < pow(2.0f, (float)(vars.size() - 1)); i++)
		{
			// vector<bool>
			vector<bool> res;
			for (int j = vars.size() - 1 - 1; j >= 0; j--) // j - little-endian (additional -1 for "func" variable)
			{
				res.push_back(i & (0b1 << j));
				context[vars[(vars.size() - 1 - 1) - j]] = i & (0b1 << j);
			}
			res.push_back(expr2->eval(context));

			// Converting vector<bool> to vector<string>
			vector<string> res2;
			for (auto el : res)
			{
				if (el) res2.push_back("1");
				else res2.push_back("0");
			}
			map2.push_back(res2);
		}

		// Middle lines
		string sRow = string(biggestVar, '-') + "|";
		string fRow = "";
		for (int i = 0; i < vars.size(); i++) fRow += sRow;

		// Tables drawing
		cout << endl;
		int p1Padding = ((vars.size() * biggestVar + vars.size() + 1) - prompt1.size()) / 2;
		if (p1Padding < 0) p1Padding = 0;
		int p2Padding = ((vars.size() * biggestVar + vars.size() + 1) - prompt2.size()) / 2;
		if (p2Padding < 0) p2Padding = 0;
		cout << string(p1Padding, ' ') << prompt1 << string((p1Padding % 2 != 0) ? p1Padding : (p1Padding+1), ' ') << string(3, ' ') << string(p2Padding, ' ') << prompt2 << string(p2Padding, ' ') << endl;

		cout << string(vars.size() * biggestVar + vars.size() + 1, '-') << string(3, ' ') << string(vars.size() * biggestVar + vars.size() + 1, '-') << endl;
		for (int i = 0; i < map1.size(); i++)
		{
			cout << "|";
			for (auto j : map1[i])
			{
				cout << setw(biggestVar) << j << "|";
			}
			
			cout << string(3, ' ');

			cout << "|";
			for (auto j : map2[i])
			{
				cout << setw(biggestVar) << j << "|";
			}
			if (i != map1.size() - 1) cout << endl 
				<< "|" << fRow 
				<< string(3, ' ')
				<< "|" << fRow 
				<< endl;
			else cout << endl 
				<< string(vars.size() * biggestVar + vars.size() + 1, '-') 
				<< string(3, ' ')
				<< string(vars.size() * biggestVar + vars.size() + 1, '-')
				<< endl;
		}
	}
	catch (tokenizer_exception const& e)
	{
		cout << "ERROR: There is no such operation as \'" << e.what() << "\'" << "\n\t" << (dispFirst ? prompt1 : prompt2) << "\n\t" << string(e.where(), ' ') << "^" << endl;
	}
}
