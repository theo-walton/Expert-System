#include "Reasoning.hpp"

static std::string	str_match(const std::string& a, const std::string& b) // matches strings of length 26
{
	std::string match;	
	match.resize(26);
	
	for (int i = 0; i < 26; i++)
	{
		if ((a[i] == 'T' && b[i] == 'F') || (a[i] == 'F' && b[i] == 'T'))
			return "";
		if (a[i] == b[i])
			match[i] = a[i];
		else if (a[i] == 'U')
			match[i] = b[i];
		else if (b[i] == 'U')
			match[i] = a[i];
	}
	return match;
}

std::vector<std::string>	Reasoning::table_match(const std::vector<std::string>& a,
						       const std::vector<std::string>& b)
{
	std::vector<std::string> out;
	std::string match;
	
	for (auto& str_a : a)
	{
		for (auto& str_b : b)
		{
			match = str_match(str_a, str_b);
			if (!match.empty())
				out.push_back(match);
		}
	}

	return out;
}

void	Reasoning::tally_table(void)
{
	int f_table[26] = {0};
	int t_table[26] = {0};
	int u_table[26] = {0};
	
	for (auto& str : _similar)
	{
		for (size_t i = 0; i < str.length(); i++)
		{
			if (str[i] == 'F')
				f_table[i]++;
			else if (str[i] == 'T')
				t_table[i]++;
			else if (str[i] == 'U')
				u_table[i]++;
		}
	}

	for (size_t i = 0; i < 26; i++)
	{
		if (f_table[i] > 0 && t_table[i] == 0 && u_table[i] == 0)
			_table[i] = 'F';
		else if (f_table[i] == 0 && t_table[i] > 0 && u_table[i] == 0)
			_table[i] = 'T';
		else if (f_table[i] == 0 && t_table[i] == 0 && u_table[i] > 0)
			_table[i] = UNKNOWN_1;
		else if (f_table[i] > 0 && t_table[i] > 0 && u_table[i] == 0)
			_table[i] = UNKNOWN_2;
		else
			_table[i] = UNKNOWN_3;
	}
}

Reasoning::Reasoning(std::vector<Statement>& statements) : _statements(statements)
{
	if (_statements.size() == 0)
	{
		std::cout << "no statements to reason with" << std::endl;
		exit(1);
	}
	
	_similar = statements[0].Table();
	for (int i = 1; i < statements.size(); i++)
	{
		_similar = table_match(_similar, statements[i].Table());
	}

	if (_similar.size() == 0)
	{
		std::cout << "contradicting input" << std::endl;
		exit(1);
	}
	
	tally_table();
}

char	Reasoning::Query(char operand)
{
	if (!std::isupper(operand))
	{
		std::cout << "invalid query" << std::endl;
		return 'F';
	}
	return _table['A' - operand];
}
