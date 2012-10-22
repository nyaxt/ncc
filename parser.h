#ifndef _ncc_parser_h_
#define _ncc_parser_h_

#include "ast.h"

namespace ncc
{

PASTNode parse(const std::string& code);
	
} // end of namespace ncc

#endif // _ncc_parser_h_
