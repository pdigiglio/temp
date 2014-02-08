#!/bin/bash - 
#===============================================================================
#
#          FILE: corr_length_extract.sh
# 
#         USAGE: ./corr_length_extract.sh 
# 
#   DESCRIPTION: script to extract correlator length values after aving
#				 obtained 'em via "auto_ising.sh"
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YOUR NAME (), 
#  ORGANIZATION: 
#       CREATED: 08/02/2014 12:30
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error
source ./variables.sh

# Extract plot
line=$1
for (( i = 0; i < ${side} / 2; i ++ ))
do
	echo -e "${i}\t`sed -e "/^#/d" cl_binning.dat | head -n ${line} | tail -n 1 | cut -f $[ 4 * $i + 4 ],$[ 4 * $i + 5 ]`" >> cl_plot.dat
done

echo "[`tput setaf 3`fit`tput sgr0`]"
./ac_fit cl_plot ${B} >> correlation_length.dat

mv --update --verbose corr.dat cl_binning.dat cl_binning_{sdom,var}.png cl_plot.{dat,png} --target-directory=${tdir}
