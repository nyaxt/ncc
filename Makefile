CXXFLAGS=--std=c++0x -O0 -g -fno-operator-names
#-I/usr/local/include

test: ncc
	ncc "3 - 2 + 1"

ncc: main.cpp parse.impl.h
	g++ $(CXXFLAGS) -o $@ $<

parse.impl.h: lang.leg
	leg -o $@ $<

