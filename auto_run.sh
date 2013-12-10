#!/bin/bash
#===============================================================================
#
#          FILE: auto_run.sh
# 
#         USAGE: ./auto_run.sh 
# 
#   DESCRIPTION: 
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YOUR NAME (), 
#  ORGANIZATION: 
#       CREATED: 04/12/2013 17:32
#      REVISION:  ---
#===============================================================================

set -o nounset

make_header () {
	echo "/* This header contains _only_ the packing fraction */

#ifndef  eta_INC
#define  eta_INC

#define E	${1}

#endif   /* ----- #ifndef eta_INC  ----- */" # > include/eta.h
}

move () {
	echo
	echo "##########################################################"
	echo "Sposto i file "
	echo "##########################################################"
	echo

	# controllo se ${DIR} esiste
	if [[ !( -d ${DIR} ) ]]
	then
		echo -e "\n##########################"
		mkdir --parents --verbose ${DIR}
		echo -e "##########################\n"
	fi

	# sposto i file
	# aggiungere 'data.dat'!
#	mv --verbose --update {var,sdom}.jpeg ${vs} data.dat ac_times.dat --target-directory=${DIR}

	# sposto dati e grafici degli autocorrelatori
#	for (( i = 1; i <= 30; i ++ ))
#	do
#		for ext in jpeg dat
#		do
#			mv --verbose --update ac_${i}{,.bak}.${ext} --target-directory=${DIR}
#		done
		# sposto i fit degli autocorrelatori
#		mv --verbose --update ac_${i}_{en,ss,us}.{pdf,svg} --target-directory=${DIR}
#	done
}


step=34
max=50

DIR=""
E=""
N=128

for i in ` seq 1 ${step}`
do
	# Evaluate packing fraction $\eta$
	E="` echo "scale=3; $i/${max}" | bc -l `"

	echo
	echo " >> Eta: ${i}/${max} = "${E}
	
	# Make new header
	make_header ${E}
	echo

	DIR="./N${N}.E${E}"
	move

	# Compile and run
	make -j2 # && ./free_part
	# Move output files
	
done

exit $?

#make_header
