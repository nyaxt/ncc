#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <algorithm>
#include <string.h>

//! ast node interface
class ASTNode
{
public:
	virtual ~ASTNode() = 0;

	virtual void dump(std::ostream& s) const = 0;
};
typedef std::unique_ptr<ASTNode> PASTNode;

std::ostream&
operator<<(std::ostream& s, const ASTNode& n)
{
	n.dump(s);

	return s;
}

inline
ASTNode::~ASTNode()
{
	/* NOP */
}

//! liretal
class ConstNode : public ASTNode
{
public:
	ConstNode(int n)
	: m_n(n) { /* NOP */ }

	~ConstNode();

	virtual void dump(std::ostream& s) const override;	

private:
	int m_n;
};

ConstNode::~ConstNode()
{

}

void
ConstNode::dump(std::ostream& s) const
{
	s << "<ConstNode: " << m_n << ">";
}

class ApplyNode : public ASTNode
{
public:
	ApplyNode(std::string funcname, std::vector<ASTNode*> args)
	:	m_funcname(std::move(funcname)), m_args(std::move(args))
	{ /* NOP */ }

	~ApplyNode();

	virtual void dump(std::ostream& s) const override;	

private:
	std::string m_funcname;
	std::vector<ASTNode*> m_args;
};

ApplyNode::~ApplyNode()
{
	/* NOP */
}

void
ApplyNode::dump(std::ostream& s) const
{
	s << "<ApplyNode: " << m_funcname << "(";
	for(ASTNode* arg: m_args)
	{
		s << *arg << ", ";
	}
	s << ")>";
}

class Parser
{
public:
	Parser(std::string code)
	: m_code(std::move(code))
	{
		/* NOP */	
	}

	void parse();
	void dump(std::ostream& s);

	// ====== below funcs only to be called from parser impl ======

	typedef size_t noded_t;

	noded_t addNode(ASTNode* newnode)
	{
		std::cout << "addNode: " << *newnode << std::endl;

		m_astnodes.push_back(PASTNode(newnode));
		return m_astnodes.size() - 1;
	}

	ASTNode* getNode(noded_t s)
	{
		return m_astnodes[s].get();	
	}

	std::vector<ASTNode*> getNodes(const std::vector<noded_t>& nodeds)
	{
		std::vector<ASTNode*> ret; ret.resize(nodeds.size());
		std::transform(nodeds.begin(), nodeds.end(), ret.begin(), std::bind(&Parser::getNode, this, std::placeholders::_1));

		return std::move(ret);
	}

	void setRootNode(noded_t s)
	{
		setRootNode(getNode(s));
	}

	void setRootNode(ASTNode* node)
	{
		m_root = node;	
	}

private:
	std::string m_code;
	std::vector<PASTNode> m_astnodes;
	ASTNode* m_root = nullptr;
};

#define YY_CTX_LOCAL
#define YY_CTX_MEMBERS Parser* pthis;
#define YYSTYPE Parser::noded_t
#define yythis ctx->pthis
#include "parse.impl.h"

namespace {

void yyfree(yycontext* ctx)
{
	if(ctx->buf) ::free(ctx->buf);
	if(ctx->text) ::free(ctx->text);
	if(ctx->thunks) ::free(ctx->thunks);
	if(ctx->vals) ::free(ctx->vals);
}

} // end of anonymous namespace

void
Parser::parse()
{
	yycontext ctx;
	::memset(&ctx, 0, sizeof(yycontext));
	ctx.pthis = this;

	printf("yyparse(): %d\n", yyparse(&ctx));

	yyfree(&ctx);
}

void
Parser::dump(std::ostream& s)
{
	m_root->dump(s);
}

int
main(int argc, char* argv[])
{
	Parser p("1+2"); p.parse();
	p.dump(std::cout);
	std::cout << std::endl;
	
	return 0;
}

