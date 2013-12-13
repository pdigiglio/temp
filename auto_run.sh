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

#set -o nounset

make_header () {
	echo "/* This header contains _only_ the packing fraction */

#ifndef  eta_INC
#define  eta_INC

#define E	${1}

#endif   /* ----- #ifndef eta_INC  ----- */" > include/eta.h
}


first=90
step=1
last=110
max=200

N=250

make analisi

for (( i = ${first}; i <= ${last}; i += ${step} ))
do
	# Evaluate packing fraction $\eta$
	E="` echo "scale=3; $i/${max}" | bc -l `"

	echo
	echo " >> Eta: ${i}/${max} = "${E}
	
	# Make new header
	make_header ${E}
	echo

	DIR="./N${N}.E${E}"
	mkdir --verbose --parents ${DIR}
	# Compile and run
	make -j2 && ./free_part

	# process file
	for name in speed dp_measures
	do
		gnuplot -e "filename='${name}'" histogram.gpl
		mv --verbose --update ${name}.{dat,png} ${name}_histo.dat --target-directory=${DIR}
	done


	for (( j = 1; j <= 30; j ++ ))
	do
		for name in delta_t free_path single_delta_t
		do
			# process file
			echo "#bin_size	mean	SDOM" >> ${name}_mean.dat
			echo "$j	`./analisi ${name}.dat ${j}`" >> ${name}_mean.dat

			# write filename
			filename="${name}_${j}"

			# rename file
			mv --verbose analisi.dat ${filename}.dat

			# plot file
			gnuplot -e "filename='${filename}'" histogram.gpl
			mv --verbose --update ${filename}.{dat,png} ${filename}_histo.dat --target-directory=${DIR}
		done
	done

	mv --verbose --update {delta_t,free_path,single_delta_t}_mean.dat --target-directory=${DIR}
	# Move output files
	
done

exit $?
