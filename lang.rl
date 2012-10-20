%%{
  machine ncc;

  main := 'a' | 'bc';
}%%

%% write data;

void
Parser::parse()
{
	char* p = m_code.data();
	char* pe = m_code.data() + m_code.size();

	%% write init;
	%% write exec;
}
