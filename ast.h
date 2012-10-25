#ifndef _ncc_ast_h_
#define _ncc_ast_h_

#include <iostream>
#include <memory>
#include <vector>

namespace ncc
{

class ASTNode;
typedef std::shared_ptr<ASTNode> PASTNode;

//! an ast node
class ASTNode
{
public:
	enum class type_t
	{
		CONSTANT,
		FUNCAPPLY
	};

	static PASTNode newConstant(int x);
	static PASTNode newFuncApply(std::string funcname, std::vector<PASTNode> args);

	~ASTNode();

	void addChild(PASTNode n)
	{
		m_children.push_back(std::move(n));	
	}

	const std::vector<PASTNode>& getChildren() const
	{
		return m_children;
	}

	void dump(std::ostream& s) const;

private:
	ASTNode(type_t t);

	const type_t m_type;
	std::vector<PASTNode> m_children;

	int m_constant = 0;
	std::string m_funcname;
};

inline
std::ostream&
operator<<(std::ostream& s, const ASTNode& n)
{
	n.dump(s);

	return s;
}

void foreachASTNode(const PASTNode& root, const std::function<void (const PASTNode&)>& f);
std::vector<PASTNode> findTerminalNodes(const PASTNode& root);

} // end of namespace ncc

#endif // _ncc_ast_h_

