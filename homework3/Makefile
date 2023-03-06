#
# Makefile for CSF Assignment 3
#

C_SRCS = c_hexfuncs.c c_hexmain.c hextests.c tctest.c
CFLAGS = -g -Og -no-pie -Wall -std=gnu11

LDFLAGS = -no-pie

%.o : %.c
	gcc $(CFLAGS) -c $<

all : csim

csim : cacheMain.o
	gcc -o $@ $(LDFLAGS) cacheMain.o

# Use this target to create a zipfile that you can submit to Gradescope
.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip -9r Makefile README.txt *.h *.cpp

clean :
	rm -f *.o csim cacheMain.o solution.zip



