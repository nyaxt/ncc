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

class FoundProgramBugError : public std::runtime_error
{
public:
	FoundProgramBugError(const std::string& what = "found program bug error")
	:	std::runtime_error(what)
	{ /* NOP */ }
};
#define NCC_ASSERT(x) for(;!(x); throw ncc::ParseError("expr failed: " #x))

} // end of namespace ncc

#endif // _ncc_exception_h_
