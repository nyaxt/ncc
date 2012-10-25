#include "ast.h"

namespace ncc
{

PASTNode
ASTNode::newConstant(int x)
{
	PASTNode ret(new ASTNode(type_t::CONSTANT));
	ret->m_constant = x;

	return std::move(ret);
}

PASTNode
ASTNode::newFuncApply(std::string funcname, std::vector<PASTNode> args)
{
	PASTNode ret(new ASTNode(type_t::FUNCAPPLY));
	ret->m_funcname = std::move(funcname);
	ret->m_children = std::move(args);

	return std::move(ret);
}

ASTNode::ASTNode(type_t t)
:	m_type(t)
{
	/* NOP */
}

ASTNode::~ASTNode()
{
	/* NOP */
}

void
ASTNode::dump(std::ostream& s) const
{
	switch(m_type)
	{
	case type_t::CONSTANT:
		s << m_constant;
		break;

	case type_t::FUNCAPPLY:
		s << m_funcname << "(";
		for(const auto& child: m_children)
		{
			s << *child;
			if(m_children.back() != child) s << ", ";
		}
		s << ")";
		break;

	default:
		s << "<unknown>";
		break;
	}
}

void
foreachASTNode(const PASTNode& root, const std::function<void (const PASTNode&)>& f)
{
	f(root);
	for(const auto& c: root->getChildren())
	{
		foreachASTNode(c, f);
	}
}

std::vector<PASTNode>
findTerminalNodes(const PASTNode& root)
{
	std::vector<PASTNode> ret;

	foreachASTNode(root, [&ret](const PASTNode& n) {
		if(n->getChildren().empty())
		{
			ret.push_back(n);	
		}
	});

	return std::move(ret);
}
	
} // end of namespace ncc

