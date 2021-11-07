/*
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
#*/

#ifndef COUNTINGSORT_H_ /* Include guard */
#define COUNTINGSORT_H_

/** macros to get execution time: both macros have to be in the same scope
*   define a double variable to use in ENDTIME before STARTTIME:
*   double x;
*   the variable will hold the execution time in seconds.
*/

#include <time.h>

/* Token concatenation used */
#define STARTTIME(id)                           \
  clock_t start_time_42_##id, end_time_42_##id; \
  start_time_42_##id = clock()

#define ENDTIME(id, x)        \
  end_time_42_##id = clock(); \
  x = ((double)(end_time_42_##id - start_time_42_##id)) / CLOCKS_PER_SEC

void generateArray(ELEMENT_TYPE *, int, int);
void countSortOn2(ELEMENT_TYPE *, int, int);
//int get_max(ELEMENT_TYPE* , int ,int );
//int get_min(ELEMENT_TYPE *, int , int );
void countSortOn(ELEMENT_TYPE *, ELEMENT_TYPE *, int, int);

#endif
