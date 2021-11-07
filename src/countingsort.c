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
/**
	@file countingsort.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "countingsort.h"

#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_thread_num() 0
#endif
/**
 * @brief The function is used to initiaslize with pseudorandomic numbers the array. This is a thread safe version
 * as rand_r function is used instead of rand().
 * @param my_array the pointer to the memory area of the array     
 * @param length size of my_array.
 * @param threads the number of threads.
 */
void generateArray(ELEMENT_TYPE* my_array, int length, int threads)
{
    unsigned int seed = time(NULL);
#pragma omp parallel num_threads(threads) shared(my_array,length, seed)
    {
#pragma omp for
        for (int i = 0; i < length; i++)
        {
            my_array[i] = (rand_r(&seed) % 10000);
        }
    }
}

/**
 * @brief The function is used to sort the array my_array according to the algorithm counting sort. This function provides an implementation with 
 * a time complexity of O(n^2), being so less convenient than another version of this algorithm in the sequencial performances.
 * @param my_array   the pointer to the array to be sorted.
 * @param length     size of my_array.
 * @param threads    number of threads.
 */
void countSortOn2(ELEMENT_TYPE* my_array, int length, int threads)
{
    int i, j, count;
    
    ELEMENT_TYPE *temp = (ELEMENT_TYPE *)malloc(length * sizeof(ELEMENT_TYPE));
#pragma omp parallel default(none) shared(length,my_array, temp) private(i, j, count) num_threads(threads)
#pragma omp for
    for (i = 0; i < length; i++)
    {
        count = 0;
        for (j = 0; j < length; j++)
        {
            if (my_array[j] < my_array[i])
                count++;
            else if (my_array[j] == my_array[i] && j < i)
                count++;
        }
        temp[count] = my_array[i];
    }
#pragma omp for
    for (i = 0; i < length; i++)
        memcpy(my_array + i, temp + i, sizeof(ELEMENT_TYPE));
    free(temp);
}

/**
 * @brief This function sorts an array according to the counting sort algorithm using optimized loops with optimized for loop.
 * This function has a time complexity O(n) being in the sequential version more convenien than the previous version.
 * @param my_array   a pointer to an array which must be sorted.
 * @param temp   the pointer to the array which must be sorted.
 * @param length     size of my_array.
 * @param threads    number of threads.
 */
void countSortOn(ELEMENT_TYPE* my_array, ELEMENT_TYPE* temp, int length, int threads)
{
    int max = my_array[0];
    int min = my_array[0];
    for (int i = 0; i < length; i++)
    {
        if (my_array[i] > max)
            max = my_array[i];
        else if (my_array[i] < min)
            min = my_array[i];
    }
    int range = max - min + 1;
    
    
    ELEMENT_TYPE * count = (ELEMENT_TYPE *)calloc(range + 1, sizeof(ELEMENT_TYPE));

#pragma omp parallel for shared(my_array, length, range) num_threads(threads) reduction(+ \
                                                                                 : count[:range])
    for (int i = 0; i < length; i++)
        count[my_array[i] - min] += 1;

#pragma omp single
    {
        for (int i = 1; i < range; i++)
            count[i] += count[i - 1];

        for (int i = length - 1; i >= 0; i--)
        {
            temp[count[my_array[i] - min] - 1] = my_array[i];
            count[my_array[i] - min]--;
        }
        free(count);
    }
}
