#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptOptimizer.h"
#include "rand.h"

/* Randomized quick sort to routine to sort a OmniOptOptimizer::population based on a particular objective chosen */
void OmniOptOptimizer::quicksort_front_obj(OmniOptOptimizer::population *pop, int objcount, int obj_array[], int obj_array_size)
{
  q_sort_front_obj (pop, objcount, obj_array, 0, obj_array_size-1);
  return;
}

/* Actual implementation of the randomized quick sort used to sort a OmniOptOptimizer::population based on a particular objective chosen */
void OmniOptOptimizer::q_sort_front_obj(OmniOptOptimizer::population *pop, int objcount, int obj_array[], int left, int right)
{
    int index;
    int temp;
    int i, j;
    double pivot;
    if (left<right)
    {
        index = rnd (left, right);
        temp = obj_array[right];
        obj_array[right] = obj_array[index];
        obj_array[index] = temp;
        pivot = pop->ind[obj_array[right]].obj[objcount];
        i = left-1;
        for (j=left; j<right; j++)
        {
            if (pop->ind[obj_array[j]].obj[objcount] <= pivot)
            {
                i+=1;
                temp = obj_array[j];
                obj_array[j] = obj_array[i];
                obj_array[i] = temp;
            }
        }
        index=i+1;
        temp = obj_array[index];
        obj_array[index] = obj_array[right];
        obj_array[right] = temp;
        q_sort_front_obj (pop, objcount, obj_array, left, index-1);
        q_sort_front_obj (pop, objcount, obj_array, index+1, right);
    }
    return;
}

/* Randomized quick sort to routine to sort a OmniOptOptimizer::population based on a particular real variable chosen */
void OmniOptOptimizer::quicksort_front_realvar(OmniOptOptimizer::population *pop, int varcount, int var_array[], int var_array_size)
{
  q_sort_front_realvar (pop, varcount, var_array, 0, var_array_size-1);
  return;
}

/* Actual implementation of the randomized quick sort used to sort a OmniOptOptimizer::population based on a particular real variable chosen */
void OmniOptOptimizer::q_sort_front_realvar(OmniOptOptimizer::population *pop, int objcount, int obj_array[], int left, int right)
{
    int index;
    int temp;
    int i, j;
    double pivot;
    if (left<right)
    {
        index = rnd (left, right);
        temp = obj_array[right];
        obj_array[right] = obj_array[index];
        obj_array[index] = temp;
        pivot = pop->ind[obj_array[right]].xreal[objcount];
        i = left-1;
        for (j=left; j<right; j++)
        {
            if (pop->ind[obj_array[j]].xreal[objcount] <= pivot)
            {
                i+=1;
                temp = obj_array[j];
                obj_array[j] = obj_array[i];
                obj_array[i] = temp;
            }
        }
        index=i+1;
        temp = obj_array[index];
        obj_array[index] = obj_array[right];
        obj_array[right] = temp;
        q_sort_front_realvar (pop, objcount, obj_array, left, index-1);
        q_sort_front_realvar (pop, objcount, obj_array, index+1, right);
    }
    return;
}

/* Randomized quick sort to routine to sort a OmniOptOptimizer::population based on a particular binary variable chosen */
void OmniOptOptimizer::quicksort_front_binvar(OmniOptOptimizer::population *pop, int varcount, int var_array[], int var_array_size)
{
  q_sort_front_binvar (pop, varcount, var_array, 0, var_array_size-1);
  return;
}

/*Actual implementation of the randomized quick sort used to sort a OmniOptOptimizer::population based on a particular binary variable chosen*/
void OmniOptOptimizer::q_sort_front_binvar(OmniOptOptimizer::population *pop, int objcount, int obj_array[], int left, int right)
{
    int index;
    int temp;
    int i, j;
    double pivot;
    if (left<right)
    {
        index = rnd (left, right);
        temp = obj_array[right];
        obj_array[right] = obj_array[index];
        obj_array[index] = temp;
        pivot = pop->ind[obj_array[right]].xbin[objcount];
        i = left-1;
        for (j=left; j<right; j++)
        {
            if (pop->ind[obj_array[j]].xbin[objcount] <= pivot)
            {
                i+=1;
                temp = obj_array[j];
                obj_array[j] = obj_array[i];
                obj_array[i] = temp;
            }
        }
        index=i+1;
        temp = obj_array[index];
        obj_array[index] = obj_array[right];
        obj_array[right] = temp;
        q_sort_front_binvar (pop, objcount, obj_array, left, index-1);
        q_sort_front_binvar (pop, objcount, obj_array, index+1, right);
    }
    return;
}

/* Randomized quick sort to routine to sort a OmniOptOptimizer::population based on crowding distance */
void OmniOptOptimizer::quicksort_dist(OmniOptOptimizer::population *pop, int *dist, int front_size)
{
  q_sort_dist (pop, dist, 0, front_size-1);
  return;
}

/* Actual implementation of the randomized quick sort used to sort a OmniOptOptimizer::population based on crowding distance */
void OmniOptOptimizer::q_sort_dist(OmniOptOptimizer::population *pop, int *dist, int left, int right)
{
    int index;
    int temp;
    int i, j;
    double pivot;
    if (left<right)
    {
        index = rnd (left, right);
        temp = dist[right];
        dist[right] = dist[index];
        dist[index] = temp;
        pivot = pop->ind[dist[right]].crowd_dist;
        i = left-1;
        for (j=left; j<right; j++)
        {
            if (pop->ind[dist[j]].crowd_dist <= pivot)
            {
                i+=1;
                temp = dist[j];
                dist[j] = dist[i];
                dist[i] = temp;
            }
        }
        index=i+1;
        temp = dist[index];
        dist[index] = dist[right];
        dist[right] = temp;
        q_sort_dist (pop, dist, left, index-1);
        q_sort_dist (pop, dist, index+1, right);
    }
    return;
}
