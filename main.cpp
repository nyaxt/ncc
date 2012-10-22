#include "parser.h"

#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <algorithm>
#include <string.h>

#include <xbyak/xbyak.h>

class CodeGen : public Xbyak::CodeGenerator {
public:
	CodeGen() : CodeGenerator(1024 * 64, Xbyak::AutoGrow)
	{
		using namespace Xbyak;
		
		mov(rax, 42);
		ret();
	}
};

using namespace ncc;

int
main(int argc, char* argv[])
try
{
#if 0
	CodeGen cg; cg.ready();
	int (*f)() = reinterpret_cast<int (*)()>(cg.getCode());
	printf("f() = %d\n", f());

	return 0;
#endif

	if(argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " code" << std::endl;	
		return 1;
	}

	std::string code(argv[1]);
	PASTNode ast = parse(code); std::cout << *ast << std::endl; 
	return 0;
}
catch(std::exception& e)
{
	std::cerr << "caught exception: " << e.what();
	return 1;
}

