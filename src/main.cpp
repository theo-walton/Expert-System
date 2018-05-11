
#include <iostream>

#include "Statement.hpp"

int	main(int ac, char **av)
{
	std::string line;
	
	std::getline (std::cin, line);
	Statement s(line);	
}
