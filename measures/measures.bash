#!/bin/bash
#
# Course: High Performance Computing 2021/2022
# 
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Group:
# Briglia Maria Rosaria	0622701711	m.briglia1@studenti.unisa.it               
# Della Monica Pierpaolo	0622701701  p.dellamonica9@studenti.unisa.it 
# Giannino Pio Roberto		0622701713  p.giannino@studenti.unisa.it
#
# This file is part of FirstProjectOMP.
#
# FirstProjectOMP is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# Requirements: Parallelize and Evaluate Performances of "COUNTING SORT" Algorithm ,by using OpenMP.
# 
# To produce this analysis files produced by previous years group 02 have been used
#
# The counting sort function for O(n^2) implementation was taken here:
# https://github.com/ianliu/programacao-paralela/blob/master/omp-count-sort/main.c
#
# The counting sort function for O(n) implementation was taken here:
# https://en.wikipedia.org/wiki/Counting_sort
#
# FirstProjectOMP is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FirstProjectOMP.  If not, see <http://www.gnu.org/licenses/>.


TIME_STAMP=$(date +%s)
NMEASURES=50

ARRAY_RC=(30000 50000 80000 100000)
ARRAY_RCOn=(30000000 50000000 80000000 100000000)
ARRAY_THS=(0 1 2 4 8 16 32)
TIMEFORMAT='%3U;%3E;%3S;%P'
ARRAY_OPT=(0 1 2 3)

trap "exit" INT

SCRIPTPATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

if [[ $2 == "-k" ]]; then
	for size in "${ARRAY_RCOn[@]}"; do
		for ths in "${ARRAY_THS[@]}"; do
			for opt in "${ARRAY_OPT[@]}"; do
				ths_str=$(printf "%02d" $ths)
				
				if [[ $opt -eq 0 ]]; then
					OUT_FILE=$SCRIPTPATH/measureOn/SIZE-$size/SIZE-$size-NTH-$ths_str-O$opt.csv
				else
					OUT_FILE=$SCRIPTPATH/measureOn/SIZE-$size-O$opt/SIZE-$size-NTH-$ths_str-O$opt.csv
				fi
	
				if [[ $opt -eq 0 && $ths -ne 0 ]]; then
					continue;
				fi
			
				mkdir -p $(dirname $OUT_FILE) 2> /dev/null
				
				echo $(basename $OUT_FILE)
				echo "dimarray,threads,init,countingsort,user,elapsed,sys,pCPU" >$OUT_FILE
				
				for ((i = 0 ; i < $NMEASURES	; i++)); do
					if [[ $ths -eq 0 ]]; then
						(time $1/programOn_seq_O$opt $size $ths )2>&1 | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/;/g' -e 's/,/./g' -e 's/;/,/g' >> $OUT_FILE
						printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
						printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
					else
						(time $1/programOn_O$opt $size $ths )2>&1 | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/;/g' -e 's/,/./g' -e 's/;/,/g' >> $OUT_FILE
						printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
						printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
					fi
				done
				printf "\n"
			done
		done
	done
else
	for size in "${ARRAY_RC[@]}"; do
		for ths in "${ARRAY_THS[@]}"; do
			for opt in "${ARRAY_OPT[@]}"; do
				ths_str=$(printf "%02d" $ths)
				
				if [[ $opt -eq 0 ]]; then
					OUT_FILE=$SCRIPTPATH/measure/SIZE-$size/SIZE-$size-NTH-$ths_str-O$opt.csv
				else
					OUT_FILE=$SCRIPTPATH/measure/SIZE-$size-O$opt/SIZE-$size-NTH-$ths_str-O$opt.csv
				fi
	
				if [[ $opt -eq 0 && $ths -ne 0 ]]; then
					continue;
				fi
			
				mkdir -p $(dirname $OUT_FILE) 2> /dev/null
				
				echo $(basename $OUT_FILE)
				echo "dimarray,threads,init,countingsort,user,elapsed,sys,pCPU" >$OUT_FILE
				
				for ((i = 0 ; i < $NMEASURES	; i++)); do
					if [[ $ths -eq 0 ]]; then
						(time $1/program_seq_O$opt $size $ths )2>&1 | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/;/g' -e 's/,/./g' -e 's/;/,/g' >> $OUT_FILE
						printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
						printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
					else
						(time $1/program_O$opt $size $ths )2>&1 | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/;/g' -e 's/,/./g' -e 's/;/,/g' >> $OUT_FILE
						printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
						printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
					fi
				done
				printf "\n"
			done
		done
	done
fi

