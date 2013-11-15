#
# Makefile - simple rules to easily build sources
#

# Main file to compile
main	= part_libere
# Modules to create
MODULES	= part_libere round

# Directory for modules and headers (every header is supposed to be
# named like the corresponding module and vice versa)
MDIR	= ./modules/
IDIR	= ./include/

# Modules direcory (XXX is $(VPATH) a special variable?)
VPATH	= $(MDIR)

# Libraries to be linked in
LBS		= m # math library
LBSPATH	= 

# Creo l'opzione da passare al compilatore per le librerie: aggiungo
# il prefisso '-l' a tutte le librerie specificate in $(LBS) e il pre-
# fisso '-L' alle directory dove si trovano le librerie
LDFLAGS	= $(addprefix -l,$(LBS))
ifneq ($(LDFLAGS),)
	LDFLAGS += $(addprefix -L,$(LBSPATH))
endif

# Creo l'opzione da passare al compilatore per le directory dei file 
# .h "custom". L'opzione '-I-' indica che nelle directory specificate
# in precedenza bisogna cercare soltanto i file locali (cioè non in-
# clusi con '#include < ... >')
INCPATH	= $(addprefix -I,$(IDIR))
# FIXME Messaggio di errore dice che è obsoleto
# INCPATH += -I-

# Creo i file oggetto dei moduli
OBJS	= $(addsuffix .o,$(MODULES))

SHELL	= /bin/bash

# C/C++ compiler
C		= gcc
CXX		= g++
# Some cpu-dependent options
MARCH	= core2
MASM	= intel
# standard language
STD		= gnu++11

# Opzioni
CXXFLAGS = -W -Wall -Wextra -Wunreachable-code -Wunused -Wformat-security -Wmissing-noreturn \
		   -O3 -pedantic -std=$(STD) -masm=$(MASM) -march=$(MARCH) -mtune=$(MARCH) -fopenmp # -time

-include $(addsuffix .d,$(MODULES))
-include $(addsuffix .d,$(main))

$(main): %: %.cpp $(OBJS) Makefile
	@ echo
	@ echo -e "[`tput setaf 3`info`tput sgr0`] Architettura rilevata:\t\t" \
		`tput setaf 2`` gcc -march=native -Q --help=target | grep --text march | cut -f3 ``tput sgr0`
	@ echo -e "[`tput setaf 3`info`tput sgr0`] Architettura selezionata:\t" `tput setaf 2`$(MARCH)`tput sgr0`
	@ echo
	@ echo -e "[`tput setaf 6`main`tput sgr0`]"\
		"$(CXX)`tput setaf 2` $<`tput sgr0` -o `tput bold`$@`tput sgr0`" \
		"$(CXXFLAGS) $(INCPATH) $(LDFLAGS)\n"
	$(CXX) $< -o $@ $(CXXFLAGS) $(INCPATH) $(LDFLAGS)


%.o:%.cc
	@echo -e "[`tput setaf 4`module`tput sgr0`] $(CXX) -c  " \
		"`tput setaf 2`$<`tput sgr0` -o `tput bold`$*.o`tput sgr0` $(INCPATH)" # $(CXXFLAGS)"
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(INCPATH) $(LDFLAGS)

# Rule to make dependence file(s) for modules
%.d:%.cc
	@echo -e "[`tput setaf 3`depend`tput sgr0`] $(CXX) -MM" \
		" `tput setaf 2`$<`tput sgr0` -o `tput bold`$@`tput sgr0` $(INCPATH)" # $(CXXFLAGS)"
	@$(CXX) -MM -ansi $< -o $@ $(INCPATH)

# Rule to make dependence file(s) for main routine
%.d:%.cpp
	$(CXX) -MM -ansi $< -o $@ $(INCPATH)
