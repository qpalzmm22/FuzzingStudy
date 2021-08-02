#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define EPSILON 1E-8

double my_sqrt(double x){
    double approx;
    double guess = x / 2;

    while(guess != approx){
        //printf("approx = %llf\n", approx);
        approx = guess;
        guess = (approx + x / approx) / 2;   
    }
    return approx;
}

void assertEqual(double x, double y){
    //printf("x : %llf, y : %llf fabs: %llf\n", x, y, fabs(x-y));
    double result = fabs(x-y);
    assert(result < EPSILON);
}

void sqrt_program(double arg){
    if (arg < 0)
        printf("Illegal Number\n");
    else
        printf("The root of %lf is %lf", arg, my_sqrt(arg));
}

void measureTime(){
    clock_t start, end;
    double time_taken, x;

    start = clock();
    for(int i = 1; i < 100000; i++){
        assertEqual(my_sqrt(i) * my_sqrt(i), i);
    }
    end = clock();

    time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("It took %lf seconds\n", time_taken);

    start = clock();
    for(int i = 1; i < 100000; i++){
        x = 1 + rand() % 100000;
        assertEqual(my_sqrt(x) * my_sqrt(x), x);
    }
    end = clock();

    time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("It took %lf seconds\n", time_taken);
}

int * shellsort( int * elems, int elems_size ){
    int* sorted_elems = elems;
    int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1};
    int gaps_size = sizeof(gaps) / sizeof(int);
    int temp, gap;

    printf("%d\n", gaps_size);
    printf("%d\n", elems_size);

    for(int i = 0 ; i < gaps_size; i++){
        gap = gaps[i];
        printf("gap: %d\n", gap);

        for(int j = gap; j < elems_size; j++){
            temp = sorted_elems[j];
            int k = j;
            printf("gap : %d, tmp : %d, k : %d\n", gap, temp, k);

            while( k >= gap && sorted_elems[k - gap] > temp){
                printf("el[k]: %d el[k-gap] : %d\n", sorted_elems[k], sorted_elems[k-gap]);
                sorted_elems[k] = sorted_elems[k - gap];
                k -= gap;
            }
            sorted_elems[k] = temp;
        }
    }
    return sorted_elems;
}

int cmpArr(int * test, int * ans, int size){

}

int main(){
    // for(int i = 1 ; i < 1000; i++)
    //     assertEqual(my_sqrt(i) * my_sqrt(i), i);

    // measureTime();
    
    // Ex.2
    int * output_arr;
    int input_arr[] = { 3,2,1};
    int arr_size = sizeof(input_arr) / sizeof(int);
    output_arr = shellsort(input_arr, arr_size);
    
    for(int i = 0; i < arr_size; i++){
        assert({6,1,2,3,4,5} =======);
    }

    return 0;
}

