#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define MAX_RAND_ARR_SIZE 20;

int cmp_arr(int * test, int * ans, int size)
{
    for(int i = 0 ; i < size; i++){
        if(test[i] != ans[i])
            return -1;
    }
    return 0;
}

int* random_arr_creator(int size)
{
    int * rand_array;
    
    rand_array = (int*) malloc(size * sizeof(int));

    for(int i = 0 ; i < size; i++){
        rand_array[i] = rand();
    }

    return rand_array;
}

// Ex2
int is_sorted(int *arr, int n)
{
    for(int i = 0; i < n - 1 ; i++){
        if(arr[i] > arr[i + 1])
            return 0;
    }
    return 1;
}

/* 0 for false
*  1 for true
*/
int is_permutation(int *arr1, int arr1_size, int * arr2, int arr2_size)
{
    

    if(arr1_size != arr2_size)
        return 0;
    
    for(int i = 0; i < arr1_size; i++){
        int a_count = 0;
        int b_count = 0;
        for(int j = 0; j < arr1_size; j++){
            if(arr1[i] == arr1[j])
                a_count++;
            if(arr1[i] == arr2[j])
                b_count++;
        }
        
        if(a_count != b_count)
            return 0;
    }
    
    return 1;
}

int * shellsort( int * elems, int elems_size )
{
    int* sorted_elems = elems;
    int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1};
    int gaps_size = sizeof(gaps) / sizeof(int);
    int temp, gap;

    for(int i = 0 ; i < gaps_size; i++){
        gap = gaps[i];

        for(int j = gap; j < elems_size; j++){
            temp = sorted_elems[j];
            int k = j;

            while( k >= gap && sorted_elems[k - gap] > temp){
                sorted_elems[k] = sorted_elems[k - gap];
                k -= gap;
            }
            sorted_elems[k] = temp;
        }
    }
    return sorted_elems;
}

void print_arr(int * arr, int n){
    for(int i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}
