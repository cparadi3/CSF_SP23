#
# Makefile for CSF Assignment 3
#

CXX = g++
CXXFLAGS = -g -Og -no-pie -Wall -Wextra -Wpedantic

LDFLAGS = -no-pie

all : csim

csim : cacheMain.o cache.o block.o set.o
	$(CXX) $(CXXFLAGS) -o csim cacheMain.o cache.o block.o set.o

cacheMain.o : cacheMain.cpp cache.h block.h set.h 
	$(CXX) $(CXXFLAGS) -c cacheMain.cpp 
cache.o : cache.cpp cache.h set.h block.h
	$(CXX) $(CXXFLAGS) -c cache.cpp cache.h set.h block.h
block.o : block.cpp block.h
	$(CXX) $(CXXFLAGS) -c block.cpp block.h
set.o: set.cpp set.h block.h
	$(CXX) $(CXXFLAGS) -c set.cpp set.h block.h
# Use this target to create a zipfile that you can submit to Gradescope
.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *.h *.cpp

clean :
	rm -f *.o csim cacheMain.o solution.zip



