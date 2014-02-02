#!/bin/bash - 
#===============================================================================
#
#          FILE: take_line.sh
# 
#         USAGE: ./take_line.sh 
# 
#   DESCRIPTION: takes ${1}-th line from file ${2}
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YOUR NAME (), 
#  ORGANIZATION: 
#       CREATED: 02/02/2014 16:29
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

head --lines=${1} ${2} | tail --lines 1
