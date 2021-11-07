/*
#
# Course: High Performance Computing 2021/2022
# 
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Group:
# Briglia Maria Rosaria		0622701711	m.briglia1@studenti.unisa.it               
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
#*/
/**
	@file main.c
*/

#include "countingsort.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    int length, threads;
    double time_countsort = 0.0, time_init = 0.0;

    if (argc < 3)
    {
        printf("ERROR! Usage: ./main dimarray threads");
        exit(1);
    }
     //takes the length of the array and the number of threads
    length = atoi(argv[1]); 
    threads = atoi(argv[2]);  

    //array's memory allocation
    ELEMENT_TYPE *my_array = (ELEMENT_TYPE *)calloc(length, sizeof(ELEMENT_TYPE));

    STARTTIME(1);
    // generate the array with random numbers
    generateArray(my_array, length, threads);
    ENDTIME(1, time_init);

    STARTTIME(2);
    //execute count sort algorithm
    countSortOn2(my_array, length, threads);
    ENDTIME(2, time_countsort);

   //print times in a formatted string on the output
    printf("%d;%d;%f;%f\n", length, threads, time_init, time_countsort);

   
    free(my_array);
    return 0;
}
