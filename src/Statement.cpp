#include "Statement.hpp"

static char	get_token(std::string& expression, size_t& i)
{
	while (std::isspace(expression[i]))
		i++;

	if (std::isupper(expression[i]))
		return expression[i++];

	if (expression[i] == '=' && expression[i + 1] == '>')
	{
		i+=2;
		return IMPLIES_TOK;
	}
	if (expression[i] == '^')
	{
		i++;
		return XOR_TOK;
	}
	if (expression[i] == '|')
	{
		i++;
		return OR_TOK;
	}
	if (expression[i] == '+')
	{
		i++;
		return AND_TOK;
	}
	if (expression[i] == '!')
	{
		i++;
		return NOT_TOK;
	}
	if (expression[i] == '<' && expression[i + 1] == '=' && expression[i + 2] == '>')
	{
		i+=3;
		return IFF_TOK;
	}
	if (expression[i] == '(')
	{
		i++;
		return OPEN_TOK;
	}
	if (expression[i] == ')')
	{
		i++;
		return CLOSE_TOK;
	}
	
	return END_TOK;
}

static int	priority(char token)
{
	if (token == NOT_TOK)
		return 5;
	if (token == AND_TOK)
		return 4;
	if (token == OR_TOK)
		return 3;
	if (token == XOR_TOK)
		return 2;
	if (token == IMPLIES_TOK)
		return 1;
	if (token == IFF_TOK)
		return 0;
	return -1;
}

void	Statement::convert_to_rpn(std::string& expression)
{
	size_t i = 0;
	while (1)
	{
		char token = get_token(expression, i);
		if (token == END_TOK)
		{
			while (_stack.size() != 0)
			{
				if (_stack.top() == OPEN_TOK)
				{
					std::cout << "badly bracketed expression" << std::endl;
					exit(1);
				}
				_rpn += _stack.top();
				_stack.pop();
			}
			break;
		}

		if (std::isupper(token)) // if token is a variable, add it to the rpn string
			_rpn += token;
		else if (token == OPEN_TOK)
			_stack.push(token);
		else if (token == CLOSE_TOK) // move all tokens from the stack to the rpn string up till open bracket
		{
			while (_stack.size() != 0 && _stack.top() != OPEN_TOK)
			{
				_rpn += _stack.top();
				_stack.pop();
			}
			if (_stack.size() != 0 && _stack.top() == OPEN_TOK)
				_stack.pop();
		}
		else // for operator tokens
		{
			while (_stack.size() != 0 && priority(token) <= priority(_stack.top()))
			{
				_rpn += _stack.top();
				_stack.pop();
			}
			_stack.push(token);
		}
	}
}

void	Statement::eval_expr(std::string& values)
{
	if (values.length() != 26)
	{
		std::cout << "value size invalid" << std::endl;
		exit(1);
	}
	for (int i = 0; i < _rpn.length(); i++)
	{
		char token = _rpn[i];

		if (std::isupper(token)) // if token is operand
		{
			_operands.push(values[token - 'A'] == 'T');
		}
		else
		{
			if ((_operands.size() < 2 && token != NOT_TOK) || (_operands.size() < 1))
			{
				std::cout << "insuficient operands" << std::endl;
				throw std::exception();
			}
			_functions[token - '0']();
		}
	}
	if (_operands.size() != 1)
	{
		std::cout << "invalid stack" << std::endl;
		throw std::exception();
	}

	if (_operands.top())
	{
		_truthTable.push_back(values);
	}	
	_operands.pop();
}

void	Statement::fill_functions(void)
{
	//implies
	_functions.push_back([this]()
			     {
				     bool b = _operands.top();
				     _operands.pop();
				     bool a = _operands.top();
				     _operands.pop();
				     _operands.push(!a || b);
			     });
	//xor
	_functions.push_back([this]()
			     {
				     bool b = _operands.top();
				     _operands.pop();
				     bool a = _operands.top();
				     _operands.pop();
				     _operands.push(a ^ b);
			     });
	//or
	_functions.push_back([this]()
			     {
				     bool b = _operands.top();
				     _operands.pop();
				     bool a = _operands.top();
				     _operands.pop();
				     _operands.push(a | b);
			     });
	//and
	_functions.push_back([this]()
			     {
				     bool b = _operands.top();
				     _operands.pop();
				     bool a = _operands.top();
				     _operands.pop();
				     _operands.push(a & b);
			     });
	//not
	_functions.push_back([this]()
			     {
				     bool a = _operands.top();
				     _operands.pop();
				     _operands.push(!a);
			     });
	//iff
	_functions.push_back([this]()
			     {
				     bool b = _operands.top();
				     _operands.pop();
				     bool a = _operands.top();
				     _operands.pop();
				     _operands.push(a == b);
			     });
}

void	Statement::generate_permutations(void)
{
	//go through rpn string and get all operands that appear in it A, B, C ect..

	std::string permutation = "UUUUUUUUUUUUUUUUUUUUUUUUUU";
	size_t operandOccur[26] = {0};

	for (char c : _rpn)
		if (std::isupper(c))
			operandOccur[c - 'A']++;
	
	size_t totalOperands = 0;
	for (size_t occur : operandOccur)
		if (occur)
			totalOperands++;

	size_t operands[totalOperands];
	size_t operandsFilled = 0;
	for (size_t i = 0; i < 26; i++)
	{
		if (operandOccur[i])
		{
			operands[operandsFilled] = i;
			operandsFilled++;
		}
	}
	
	size_t bits[totalOperands];
	for (size_t i = 0; i < totalOperands; i++)
	{
		bits[i] = 1 << i;
	}

	size_t limit = std::pow(2, totalOperands);
	for (size_t n = 0; n < limit; n++)
	{
		for (size_t index = 0; index < totalOperands; index++)
		{
			permutation[operands[index]] = (n & bits[index] ? 'T' : 'F');
		}
		try
		{
//			std::cout << permutation << std::endl;
			eval_expr(permutation);
		}
		catch (std::exception)
		{
			std::cout << "bad statement" << std::endl;
			exit(1);
		}
	}
}

Statement::Statement(std::string expression) : _expression(expression)
{
	convert_to_rpn(expression);
	fill_functions();
	generate_permutations();
}

const std::vector<std::string>&	Statement::Table(void)
{
	return _truthTable;
}

void	Statement::Print(void)
{
	std::cout << _expression << std::endl;
}

