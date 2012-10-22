#include "ast.h"

namespace ncc
{

ConstNode::~ConstNode()
{
	/* NOP */
}

void
ConstNode::dump(std::ostream& s) const
{
	s << "<ConstNode: " << m_n << ">";
}

ApplyNode::~ApplyNode()
{
	/* NOP */
}

void
ApplyNode::dump(std::ostream& s) const
{
	s << "<ApplyNode: " << m_funcname << "(";
	for(const auto& arg: m_args)
	{
		s << *arg << ", ";
	}
	s << ")>";
}
	
} // end of namespace ncc

