
#include <iostream>

#include "Statement.hpp"
#include "Reasoning.hpp"

int	main(int ac, char **av)
{
	std::vector<Statement> statements;

	for (int i = 1; i < ac; i++)
	{
		statements.push_back(Statement(av[i]));
	}

	Reasoning r(statements);
}
