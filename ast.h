#ifndef _ncc_ast_h_
#define _ncc_ast_h_

#include "dumpable.h"
#include "exception.h"

#include <memory>
#include <vector>

namespace ncc
{

class ASTNode;
typedef std::shared_ptr<ASTNode> PASTNode;

//! an ast node
class ASTNode : public Dumpable
{
public:
	enum class type_t
	{
		CONSTANT,
		FUNCAPPLY,
		RETURN
	};

	static PASTNode newConstant(int x);
	static PASTNode newFuncApply(std::string funcname, std::vector<PASTNode> args);
	static PASTNode newReturn(PASTNode retval);

	~ASTNode();

	type_t getType() const
	{
		return m_type;	
	}

	void addChild(PASTNode n)
	{
		m_children.push_back(std::move(n));	
	}

	int getConstant() const
	{
		NCC_ASSERT(m_type == type_t::CONSTANT);
		return m_constant;
	}

	const std::string& getFuncName() const
	{
		NCC_ASSERT(m_type == type_t::FUNCAPPLY);
		return m_funcname;
	}

	const std::vector<PASTNode>& getChildren() const
	{
		return m_children;
	}

	const PASTNode& getOnlyChild() const
	{
		NCC_ASSERT(m_children.size() == 1U);	
		return m_children.front();	
	}

	virtual void dump(std::ostream& s) const override;

private:
	ASTNode(type_t t);

	const type_t m_type;
	std::vector<PASTNode> m_children;

	int m_constant = 0;
	std::string m_funcname;
};

void foreachASTNode(const PASTNode& root, const std::function<void (const PASTNode&)>& f);
std::vector<PASTNode> findTerminalNodes(const PASTNode& root);

} // end of namespace ncc

#endif // _ncc_ast_h_

