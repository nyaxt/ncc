#include <iostream>
#include <memory>
#include <string.h>

class Parser
{
public:
	Parser(std::string code)
	: m_code(std::move(code))
	{
		/* NOP */	
	}

	void parse();

private:
	std::string m_code;
};

#define YY_CTX_LOCAL
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

	printf("yyparse(): %d\n", yyparse(&ctx));

	yyfree(&ctx);
}

int
main(int argc, char* argv[])
{
	Parser p("1+2"); p.parse();
	
	return 0;
}

