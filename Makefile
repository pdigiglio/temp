#
# Makefile - simple rules to easily build sources
#

main=part_libere
modules=part_libere.o

SHELL=/bin/bash

# Compilatore c++
CXX = g++

# some cpu-dependent options
MARCH = pentium-m
MASM = intel

# standard language
STD = gnu++11

# Opzioni
CXXFLAGS = -Wall -O2 -Wextra -pedantic -march=$(MARCH) -std=$(STD) \
		   -masm=$(MASM) -mtune=$(MARCH) -fopenmp -lm

$(main): %: %.cpp $(modules) Makefile
	@ echo
	@ echo -e "`tput bold``tput setaf 1`[INFO]`tput sgr0`"
	@ echo -e "Architettura rilevata:\t\t" \
		`tput setaf 2`` gcc -march=native -Q \
		--help=target | grep --text march | cut -f3 ``tput sgr0`
	@ echo -e "Architettura selezionata:\t" \
		`tput setaf 2`$(MARCH)`tput sgr0`
	@ echo
	@ echo -e "[`tput setaf 6`main`tput sgr0`]"\
		"$(CXX)`tput setaf 2` $<`tput sgr0` -o `tput bold`$@`tput sgr0`" \
		"$(CXXFLAGS)\n\n"


-include $(modules:.o=.d)

%.o:%.cc
	g++ -c $*.cc -o $*.o $(CXXFLAGS)
	g++ -MM $*.cc -o $*.d $(CXXFLAGS)
