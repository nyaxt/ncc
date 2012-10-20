#include <iostream>

class Parser
{
public:
	Parser(std::string code)
	: m_code(std::move(code))
	{
		/* NOP */	
	}

	void parse();

private:
	std::string m_code;
};

#include "parse.impl.h"

int
main(int argc, char* argv[])
{
	Parser p(argv[1]);
	p.parse();
	
	return 0;
}

