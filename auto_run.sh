#!/bin/bash - 
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

set -o nounset                              # Treat unset variables as an error

make_header () {
	echo "/* This header contains _only_ the packing fraction */

#ifndef  eta_INC
#define  eta_INC

#define E	${E}

#endif   /* ----- #ifndef eta_INC  ----- */" > include/eta.h
}


step=35
max=50
E=""

for i in ` seq 1 ${step}`
do
	echo " >> Eta: ${i}/${max} = ` echo "scale=3; $i/${max}" | bc -l `"
	E="$i/50"
#	make_header
done

#make_header
