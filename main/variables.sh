#!/bin/bash - 
#===============================================================================
#
#          FILE: variables.sh
# 
#         USAGE: ./variables.sh 
# 
#   DESCRIPTION: 
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YOUR NAME (), 
#  ORGANIZATION: 
#       CREATED: 08/02/2014 13:11
#      REVISION:  ---
#===============================================================================

set -o nounset     # Treat unset variables as an error

# Limite di binning
bin_lim=80
# Temperature
B=1.023
# Lattice side
side=500
tdir="./potts/L${side}/B_${B}/"
