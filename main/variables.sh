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

set -o nounset                              # Treat unset variables as an error

# Temperature
B=.2
# Lattice side
side=200
tdir="./B_${B}"
