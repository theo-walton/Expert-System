#pragma once

#include "Statement.hpp"

#define UNKNOWN_1 '1'
#define UNKNOWN_2 '2'
#define UNKNOWN_3 '3'

class	Reasoning
{
	std::vector<std::string> _similar;
	std::vector<Statement>& _statements;
	char _table[26];

	std::vector<std::string>	table_match(const std::vector<std::string>&, const std::vector<std::string>&);
	void	tally_table(void);
	
public:

	Reasoning(std::vector<Statement>&);
	char	Query(char operand);
};
