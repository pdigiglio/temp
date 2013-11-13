#
# Makefile - simple rules to easily build sources
#

# Main file to compile
main=part_libere
# modules to create
modules=part_libere.o

SHELL=bash

# Compilatore c++
CXX = g++

# some cpu-dependent options
MARCH = core2
MASM = intel

# standard language
STD = gnu++11

# Opzioni
CXXFLAGS = -W -Wall -Wextra -Wunreachable-code -Wunused \
		   -Wformat-security -Wmissing-noreturn \
		   -O3 -pedantic -std=$(STD) -masm=$(MASM) \
		   -march=$(MARCH) -mtune=$(MARCH) -fopenmp # -time

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
	@echo -e "[`tput setaf 4`module`tput sgr0`] $(CXX) -c  " \
		"`tput setaf 2`$*.cc`tput sgr0` -o `tput bold`$*.o`tput sgr0`" # $(CXXFLAGS)"
	@echo -e "[`tput setaf 3`depend`tput sgr0`] $(CXX) -MM" \
		" `tput setaf 2`$*.cc`tput sgr0` -o `tput bold`$*.d`tput sgr0`" # $(CXXFLAGS)"
