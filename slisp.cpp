#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"
#include "test_config.hpp"
using namespace std;


// Function to execute a short simple program with the -e flag
int short_program(Interpreter& interp, const string& program)
{
	istringstream iss(program);
	if (interp.parse(iss))
	{
		try
		{
			Expression result = interp.eval();
			cout << "(" << result << ")" << endl;
			return EXIT_SUCCESS;
		}
		catch (const exception& e)
		{
			cerr << "Error: " << e.what() << endl;
			return EXIT_FAILURE;
		}
	}
	else
	{
		cerr << "Error: Failed to parse." << endl;
		return EXIT_FAILURE;
	}
}

// Function to execute a program stored in an external file
int external_file(Interpreter& interp, const string& filename)
{
	ifstream ifs(filename);
	if (!ifs)
	{
		cerr << "Error: Cannot open file." << endl;
		return EXIT_FAILURE;
	}

	if (interp.parse(ifs))
	{
		try
		{
			Expression result = interp.eval();
			cout << "(" << result << ")" << endl;
			return EXIT_SUCCESS;
		}
		catch (const exception& e)
		{
			cerr << "Error: " << e.what() << endl;
			return EXIT_FAILURE;
		}
	}
	else
	{
		cerr << "Error: Failed to parse." << endl;
		return EXIT_FAILURE;
	}
}

// Function to run in interactive REPL mode
int interactive_repl(Interpreter& interp)
{
	string line;
	while (true)
	{
		cout << "slisp> ";
		if (!getline(cin, line))
		{
			// EOF received
			break;
		}
		if (line.empty())
		{
			continue;
		}

		istringstream iss(line);
		if (interp.parse(iss))
		{
			try
			{
				Expression result = interp.eval();
				cout << "(" << result << ")" << endl;
			}
			catch (const exception& e)
			{
				cerr << "Error: " << e.what() << endl;
				interp.resetEnvironment();
			}
		}
		else
		{
			cerr << "Error: Failed to parse." << endl;
		}
	}
	return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
	Interpreter interp;

	// Case 1: Execute short simple programs with the -e flag
	if (argc == 3 && std::string(argv[1]) == "-e")
	{
		return short_program(interp, argv[2]);
	}

	// Case 2: Execute programs stored in external files
	if (argc == 2)
	{
		return external_file(interp, argv[1]);
	}

	// Case 3: Interactive REPL mode
	if (argc == 1)
	{
		return interactive_repl(interp);
	}

	cerr << "Error: Invalid arguments." << endl;
	return EXIT_FAILURE;
}