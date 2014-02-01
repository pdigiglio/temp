#
# Makefile - simple rules to easily build sources
#

# Main file to compile
MAIN	= ising #lennard_jones #free_part
# Data analysis
ANALISI	= analisi corr_length binning
# Fit
FIT		= ac_fit
# Miscellanea
MISC	= susc
# Modules to create
COMMON	= round info_time

MODULES	= $(COMMON)

# Molecular dynamics
#MODULES += particella part_soffici # part_libere

# Ising/Potts model
MODULES	+= reticolo

.PHONY: all distclean clean

# Default rule
all: $(MAIN)

# Clean directory from dependences, objects and executable
clean:
	@-rm --recursive --force --verbose *.o *.d

distclean: clean
	@-rm --recursive --force --verbose $(MAIN) $(ANALISI) $(FIT) $(MISC)

# Directory for modules and headers
MDIR	= ../modules
IDIR	= ../include

# Modules direcory
VPATH	= $(MDIR):$(MDIR)/common

# Libraries to be linked in
LBS		= m # math library
LBSPATH	= 

# FIXME Dependence files (append dependences to prerequisite)
ifneq ( $(MAKECMDGOALS), clean )
	-include $(addsuffix .d,$(MODULES) $(MAIN) $(ANALISI))
endif

# Creo l'opzione da passare al compilatore per le librerie: aggiungo
# il prefisso '-l' a tutte le librerie specificate in $(LBS) e il pre-
# fisso '-L' alle directory dove si trovano le librerie
LDFLAGS	= $(addprefix -l,$(LBS))
ifneq ($(LBSPATH),)
	LDFLAGS += $(addprefix -L,$(LBSPATH))
endif

# Add ROOT libaries
ROOT = `root-config --libs --cflags`
#LDFLAGS += $(ROOT)

# Creo l'opzione da passare al compilatore per le directory dei file 
# .h "custom". L'opzione '-I-' indica che nelle directory specificate
# in precedenza bisogna cercare soltanto i file locali (cioè non in-
# clusi con '#include < ... >')
INCPATH	=
ifneq ($(IDIR),)
	INCPATH += $(addprefix -I,$(IDIR))
endif

# Creo i file oggetto dei moduli
OBJS	= $(addsuffix .o,$(MODULES) $(MAIN))

SHELL	= /bin/bash
# C/C++ compiler
CC		= gcc
CXX		= g++
# Some cpu-dependent options
MARCH	= core2
MASM	= intel
# standard language
STD		= gnu++11

# Opzioni
CXXFLAGS = -W -Wall -Wextra -Wunreachable-code -Wunused -Wformat-security -Wmissing-noreturn \
		   -Ofast -pedantic -std=$(STD) -masm=$(MASM) -march=$(MARCH) -mtune=$(MARCH) #-fopenmp -time

# Add includes
CXXFLAGS += $(INCPATH)

# Rule to make dependence file(s) for modules
%.d: %.cc
	@echo -e "[`tput setaf 3`depend`tput sgr0`] $(CXX) -MM -ansi -o `tput bold`$@`tput sgr0`" \
		" `tput setaf 2`$<`tput sgr0` $(INCPATH)" # $(CXXFLAGS)"
	@$(CXX) -MM -ansi $< -o $@ $(INCPATH) $(LDFLAGS)

# Rule to make dependence file(s) for main routine
%.d: %.cpp
	@echo -e "[`tput setaf 3`depend`tput sgr0`] $(CXX) -MM -ansi -o `tput bold`$@`tput sgr0`" \
		" `tput setaf 2`$<`tput sgr0` $(INCPATH)" # $(CXXFLAGS)"
	@$(CXX) -MM -ansi $< -o $@ $(INCPATH) $(LDFLAGS)

# Rule to make object files
%.o: %.cc %.d Makefile
	@echo -e "[`tput setaf 4`module`tput sgr0`] $(CXX) -o `tput bold`$@`tput sgr0`" \
		"-c `tput setaf 2`$<`tput sgr0` $(INCPATH)" # $(CXXFLAGS)"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(LDFLAGS)

# Rule to make object file for main routine
%.o: %.cpp %.d Makefile
	@echo -e "[`tput setaf 4`module`tput sgr0`] $(CXX) -o `tput bold`$@`tput sgr0`" \
		"-c `tput setaf 2`$<`tput sgr0` $(INCPATH)" # $(CXXFLAGS)"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(LDFLAGS)

$(MAIN): %: $(OBJS) Makefile
	@ echo
	@ echo -e "[`tput setaf 6`info`tput sgr0`] Architettura rilevata:\t\t" \
		`tput setaf 5`` gcc -march=native -Q --help=target | grep --text march | cut -f3 ``tput sgr0`
	@ echo -e "[`tput setaf 6`info`tput sgr0`] Architettura selezionata:\t" `tput setaf 5`$(MARCH)`tput sgr0`
	@ echo -e "[`tput bold``tput setaf 6`main`tput sgr0`]" \
		"$(CXX) -o `tput bold`$@`tput sgr0` `tput setaf 2`$(OBJS)`tput sgr0`" \
		"$(CXXFLAGS)$(INCPATH) $(LDFLAGS)\n"
	@$(CXX) $(OBJS) -o $@ $(CXXFLAGS) $(LDFLAGS)

$(ANALISI): %: %.o round.o
	@echo -e "[$(ANALISI)] $(CXX) -o `tput bold`$@`tput sgr0` -c `tput setaf 2`$^`tput sgr0` $(INCPATH)" # $(CXXFLAGS)"
	@$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

# Explicit rule to make ROOT analasys routine module
$(FIT).o: $(FIT).cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(LDFLAGS) $(ROOT)

# ROOT analysis routine
$(FIT): %: %.o round.o
	@echo -e "[`tput setaf 6`$@`tput sgr0`] $(CXX) -c `tput setaf 2`$^`tput sgr0` -o `tput bold`$@`tput sgr0` $(ROOT) $(CXXFLAGS)"
	@$(CXX) $^ -o $@ $(CXXFLAGS) $(ROOT)

$(MISC): %: %.o
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

# Produce executables
mkxeq: $(MAIN)

# make dependencies
mkdep: $(addsuffix .d,$(MODULES) $(MAIN))
	@echo ""
