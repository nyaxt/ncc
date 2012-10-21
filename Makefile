CXXFLAGS=--std=c++0x -O0 -g

test: ncc
	echo "1 + 2 + 3" | ncc

ncc: main.cpp parse.impl.h
	g++ $(CXXFLAGS) -o $@ $<

parse.impl.h: lang.leg
	leg -o $@ $<

