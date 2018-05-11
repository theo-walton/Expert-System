#pragma once

#include <iostream>
#include <stack>
#include <vector>
#include <string>

#define IMPLIES_TOK '0'
#define XOR_TOK '1'
#define OR_TOK '2'
#define AND_TOK '3'
#define NOT_TOK '4'
#define IFF_TOK '5'
#define OPEN_TOK '('
#define CLOSE_TOK ')'
#define END_TOK '\0'

class	Statement
{
	std::vector<std::string> _truthTable;
	std::string _rpn;
	std::stack<char> _stack;
	std::stack<bool> _operands;
	std::vector<std::function<void()> > _functions;

	void	convert_to_rpn(std::string&);
	void	fill_functions(void);
	void	eval_expr(std::string values);
	void	generate_permutations(void);
	
public:

	Statement(std::string expression);
	
	const std::vector<std::string>&	Table(void);
};
