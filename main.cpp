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

enum class reg_t
{
	RAX,
	RCX,
	RDX,
	RBX,
	RSP,
	RBP,
	RSI,
	RDI,
	R8,
	R9,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15
};

Xbyak::Reg
toXbyakReg(reg_t reg)
{
	using Xbyak::Reg64;

	switch(reg)
	{
	case reg_t::RAX: return Reg64(Reg64::RAX);
	case reg_t::RCX: return Reg64(Reg64::RCX);
	default:
		NCC_ASSERT(false); /* not reached */
	}
}

class CodeGen : public Xbyak::CodeGenerator
{
public:
	CodeGen()
	:	CodeGenerator(1024 * 64, Xbyak::AutoGrow)
	{ /* NOP */ }

	void emitAssignConstant(int x)
	{
		mov(rax, x);
	}

	void emitSaveAccumulatorCF(reg_t reg)
	{
		mov(toXbyakReg(reg), rax);
	}

	void emitAddCF(reg_t regAddend)
	{
		add(rax, toXbyakReg(regAddend));	
	}

	void emitRet()
	{
		ret();
	}
};

class CodeFragment;
typedef std::shared_ptr<CodeFragment> PCodeFragment;

typedef double cost_t;

class CodeFragment : public Dumpable
{
public:
	virtual ~CodeFragment() = 0;

	virtual void emit(CodeGen* cg) const = 0;
	virtual cost_t cost() const = 0;
};

inline
CodeFragment::~CodeFragment()
{
	/* NOP */
}

class Code;
typedef std::shared_ptr<Code> PCode;

class SaveAccumulatorCF;

class Code : public Dumpable
{
public:
	void concat(Code& c)
	{
		for(auto& cf: c.m_cfs) m_cfs.push_back(cf);
	}

	void addFragment(PCodeFragment cf)
	{
		m_cfs.push_back(std::move(cf));
	}

	cost_t cost() const
	{
		cost_t cost = 0;

		for(const auto& cf: m_cfs)
		{
			cost += cf->cost();
		}

		return cost;
	}
	
	void emit(CodeGen* cg) const
	{
		for(const auto& cf: m_cfs)
		{
			cf->emit(cg);
		}
	}

	reg_t saveAccumulator();

	virtual void dump(std::ostream& s) const override
	{
		s << "*** Code begin ***" << std::endl;
		for(const auto& cf: m_cfs)
		{
			s << *cf << std::endl;
		}
		s << "*** Code end ***" << std::endl;
	}

private:
	std::vector<PCodeFragment> m_cfs;
};

class AssignConstantCF : public CodeFragment
{
public:
	AssignConstantCF(int x)
	:	m_x(x)
	{ /* NOP */ }

	virtual cost_t cost() const override { return 1.0; }

	virtual void emit(CodeGen* cg) const override
	{
		cg->emitAssignConstant(m_x);
	}

	virtual void dump(std::ostream& s) const override
	{
		s << "assign " << m_x << ";";
	}

private:
	int m_x;
};

class SaveAccumulatorCF : public CodeFragment
{
public:
	SaveAccumulatorCF(reg_t reg)
	:	m_reg(reg)
	{ /* NOP */ }

	virtual cost_t cost() const override { return 1.0; }

	virtual void emit(CodeGen* cg) const override
	{
		cg->emitSaveAccumulatorCF(m_reg);
	}

	virtual void dump(std::ostream& s) const override
	{
		s << "save register " << static_cast<int>(m_reg) << ";";
	}

private:
	reg_t m_reg;
};

reg_t
Code::saveAccumulator()
{
	reg_t regUnused = reg_t::RCX; // FIXME: find unused reg for real
	m_cfs.push_back(PCodeFragment(new SaveAccumulatorCF(regUnused)));
	return regUnused;
}

class AddCF : public CodeFragment
{
public:
	AddCF(reg_t regAddend)
	:	m_regAddend(regAddend)
	{ /* NOP */ }

	virtual cost_t cost() const override { return 1.0; }

	virtual void emit(CodeGen* cg) const override
	{
		cg->emitAddCF(m_regAddend);
	}

	virtual void dump(std::ostream& s) const override
	{
		s << "accum += reg " << static_cast<int>(m_regAddend) << ";";
	}

private:
	reg_t m_regAddend;
};

class RetCF : public CodeFragment
{
public:
	virtual cost_t cost() const override
	{
		return 36.0;
	}

	virtual void emit(CodeGen* cg) const override
	{
		cg->emitRet();	
	}

	virtual void dump(std::ostream& s) const override
	{
		s << "return";
	}
};

std::vector<PCode>
patternMatch(const PASTNode& n)
{
	std::vector<PCode> candidates;

	if(n->getType() == ASTNode::type_t::CONSTANT)
	{
		PCode code(new Code);
		code->addFragment(PCodeFragment(new AssignConstantCF(n->getConstant())));

		candidates.push_back(std::move(code));
	}
	else if(n->getType() == ASTNode::type_t::FUNCAPPLY && n->getFuncName() == "+")
	{
		auto candA = patternMatch(n->getChildren()[0]);
		auto candB = patternMatch(n->getChildren()[1]);

		for(auto ca : candA)
		{
			for(auto cb : candB)
			{
				PCode code(new Code);
				code->concat(*ca);
				reg_t r = code->saveAccumulator();
				code->concat(*cb);
				code->addFragment(PCodeFragment(new AddCF(r)));
				candidates.push_back(std::move(code));
			}
		}
	}
	else if(n->getType() == ASTNode::type_t::RETURN)
	{
		const PASTNode& nodeRetVal = n->getOnlyChild();
		for(auto candRetVal : patternMatch(nodeRetVal))
		{
			candRetVal->addFragment(PCodeFragment(new RetCF()));
			candidates.push_back(std::move(candRetVal));
		}
	}

	NCC_ASSERT(! candidates.empty());
	return std::move(candidates);
}

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

	std::vector<PCode> candidates = patternMatch(ast);
	PCode bestCode = *std::min_element(candidates.begin(), candidates.end(), [](const PCode& ca, const PCode& cb) -> bool { return ca->cost() < cb->cost(); });

	std::cout << *bestCode << std::endl;

	CodeGen cg;
	bestCode->emit(&cg);
	
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

