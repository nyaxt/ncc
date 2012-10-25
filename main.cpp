#include "parser.h"

#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <algorithm>
#include <string.h>

#include <xbyak/xbyak.h>

namespace ncc
{

class CodeGen : public Xbyak::CodeGenerator {
public:
	CodeGen(const PASTNode& ast)
	:	CodeGenerator(1024 * 64, Xbyak::AutoGrow),
		m_ast(ast)
	{
		using namespace Xbyak;
	}

	void prologue()
	{
		
	}
	
	void epilogue()
	{
		mov(rax, 42);
		ret();
	}

private:
	const PASTNode& m_ast;
};

} // end of namespace ncc

using namespace ncc;

int
main(int argc, char* argv[])
try
{
	if(argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " code" << std::endl;	
		return 1;
	}

	std::string code(argv[1]);
	PASTNode ast = parse(code); std::cout << *ast << std::endl; 

	CodeGen cg(ast);
	cg.prologue();
	cg.epilogue();
	
	cg.ready();
	int (*f)() = reinterpret_cast<int (*)()>(cg.getCode());
	printf("f() = %d\n", f());

	return 0;
}
catch(std::exception& e)
{
	std::cerr << "caught exception: " << e.what();
	return 1;
}

