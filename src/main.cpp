
#include <iostream>
#include <fstream>
#include <algorithm>

#include "Statement.hpp"
#include "Reasoning.hpp"

static std::string	extract_queries(std::string expr)
{
	std::string out = "";
	
	for (int i = 1; i < expr.length(); i++)
	{
		if (std::isupper(expr[i]))
			out += expr[i];
	}
	return out;
}

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "requires single filepath as argument" << std::endl;
		return 1;
	}

	std::ifstream f(av[1]);

	if (!f)
	{
		std::cout << "bad file" << std::endl;
		return 1;
	}
	
	std::vector<Statement> statements;	
	std::string line;
	std::string queries = "";

	std::cout << std::endl << "evaluating expressions:" << std::endl << std::endl;
	
	while (std::getline(f, line))
	{
		size_t index = line.find_first_of("#");
		std::string expr = line.substr(0, index);

		if (std::all_of(expr.begin(), expr.end(), isspace))
			continue;

		if (expr[0] == '=')
		{
			for (int i = 1; i < expr.length(); i++)
			{
				if (!std::isspace(expr[i]))
					statements.push_back(Statement(std::string(1, expr[i])));
			}
		}
		else if (expr[0] == '?')
		{
			queries += extract_queries(expr);
			continue;
		}
		else
			statements.push_back(Statement(expr));
	}

	for (int i = 0; i < statements.size(); i++)
	{
		std::cout << i + 1 << ": ";
		statements[i].Print();
	}
	std::cout << std::endl;
	
	Reasoning r(statements);

	// query things

	for (char c : queries)
		r.Query(c);
}
