#ifndef _ncc_dumpable_h_
#define _ncc_dumpable_h_

#include <iostream>

namespace ncc
{

class Dumpable
{
public:
	virtual void dump(std::ostream& s) const = 0;
};

inline
std::ostream&
operator<<(std::ostream& s, const Dumpable& n)
{
	n.dump(s);

	return s;
}

} // end of namespace ncc

#endif // _ncc_dumpable_h_
