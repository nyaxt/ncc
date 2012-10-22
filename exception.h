#ifndef _ncc_exception_h_
#define _ncc_exception_h_

#include <stdexcept>

namespace ncc
{

class ParseError : public std::runtime_error
{
public:
	ParseError(const std::string& what = "parse error")
	:	std::runtime_error(what)
	{ /* NOP */ }
};

} // end of namespace ncc

#endif // _ncc_exception_h_
