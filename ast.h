#ifndef _ncc_ast_h_
#define _ncc_ast_h_

#include <iostream>
#include <memory>
#include <vector>

namespace ncc
{

//! ast node interface
class ASTNode
{
public:
	virtual ~ASTNode() = 0;

	virtual void dump(std::ostream& s) const = 0;
};
typedef std::shared_ptr<ASTNode> PASTNode;

inline
ASTNode::~ASTNode()
{
	/* NOP */
}

inline
std::ostream&
operator<<(std::ostream& s, const ASTNode& n)
{
	n.dump(s);

	return s;
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

//! function application
class ApplyNode : public ASTNode
{
public:
	ApplyNode(std::string funcname, std::vector<PASTNode> args)
	:	m_funcname(std::move(funcname)), m_args(std::move(args))
	{ /* NOP */ }

	~ApplyNode();

	virtual void dump(std::ostream& s) const override;	

private:
	std::string m_funcname;
	std::vector<PASTNode> m_args;
};

} // end of namespace ncc

#endif // _ncc_ast_h_
