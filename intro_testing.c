#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define EPSILON 1E-8
#define MAX_ARRAY_SIZE 20

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

double my_sqrt_fixed(double x){
    assert(0 <= x);
    if (x == 0){
        return 0;
    }
    return my_sqrt(x);
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

/* -1 for false
*  0 for true
*/
int cmpArr(int * test, int * ans, int size){
    for(int i = 0 ; i < size; i++){
        if(test[i] != ans[i])
            return -1;
    }
    return 0;
}

int* createRandArray(int size){
    int * rand_array;
    time_t t;
    srand((unsigned) time(&t));
    int arr_size = rand() % MAX_ARRAY_SIZE;
    rand_array = (int*) malloc(arr_size * sizeof(int));

    for(int i = 0 ; i < arr_size; i++){
        rand_array[i] = rand() % INT_MAX;
    }

    return rand_array;
}


/* 0 for false
*  1 for true
*/
int is_sorted(int *test, int size){
    for(int i = 0; i < size - 1 ; i++){
        if(test[i] > test[i+1])
            return 0;
    }
    return 1;
}

int freeRetZero(int * num_list, int *count_list){
    free(num_list);
    free(count_list);
    return 0;
}

/* 0 for false
*  1 for true
*/
int is_permutation(int *test_arr, int * cmp_arr, int test_size, int cmp_size){
    int found, included;

    if(test_size != cmp_size)
        return 0;
    
    int num_max_size = 2; // initial value
    int num_size = 0;
    int * num_list = (int*)malloc(num_max_size * sizeof(int));
    int * count_list = (int*)malloc(num_max_size * sizeof(int));
    
    memset(num_list, 0, num_max_size * sizeof(int));
    memset(count_list, 0, num_max_size * sizeof(int));

    // make dictionary
    for(int i = 0 ; i < test_size ; i++){
        found = 0;
        for(int j = 0; j < num_size; j++){
            if (num_list[j] == test_arr[i]){
                found = 1;
                count_list[j]++;
            }
        }
        if(found == 0){
            // double the dictionary size when not enuf space

            num_list[num_size] = test_arr[i];
            count_list[num_size]++;
            
            num_size++;
            if (num_size >= num_max_size){
                num_max_size *= 2;
                num_list = (int*)realloc(num_list, num_max_size * sizeof(int));
                memset(num_list + num_max_size / 2, 0, (num_max_size / 2) * sizeof(int));

                
                count_list = (int*)realloc(count_list, num_max_size * sizeof(int));
                memset(count_list + num_max_size / 2, 0, (num_max_size / 2) * sizeof(int));
            }
        }
    }
    for(int i = 0; i < num_size; i++){
        found = 0;
        for(int j = 0; j < cmp_size; j++){
            if(cmp_arr[j] == num_list[i]){
                found = 1;
                count_list[i]--;
            }
        }
        // Contain A But Not B
        if(found == 0)
            return freeRetZero(num_list, count_list);;
    }
    
    for(int i = 0; i < cmp_size; i++){
        found = 0;
        for(int j = 0; j < num_size; j++){
            if(cmp_arr[i] == num_list[j])
                found = 1;
        }
        if(found == 0)
            return freeRetZero(num_list, count_list);;
    }
    // Contain A and Contain B but too many in A
    int count = 0;
    for(int i = 0; i < num_size; i++){
        count += count_list[i];
    }
    if(count != 0){
        return freeRetZero(num_list, count_list);;
    }
    free(num_list);
    free(count_list);

    return 1;
}

double * quadratic_solver(double a, double b, double c){
    double * solutions = (double*)malloc(sizeof(double) * 2);

    if(a == 0){
        if(b == 0){
            if(c == 0){
                solutions[0] = 0;
                solutions[1] = 0;
                return solutions;
            } else{
                // unsolvable
                solutions = 0x0;
                return solutions;

            }
        }
        else {
            solutions = 0x0;
            return solutions;
        }
    }  
    
    double q = b * b - 4 * a * c;
    if (q < 0){
        solutions = 0x0;
        return solutions;
    }

    solutions[0] = (-b + my_sqrt_fixed(q)) / (2*a);
    solutions[1] = (-b - my_sqrt_fixed(q)) / (2*a);
    
    return solutions;
}

void doQuad(double a, double b, double c){
    printf("Do Quad of %lf %lf %lf", a, b, c);
    double* solutions = quadratic_solver(a,b,c);
    if(solutions == 0x0){
        printf("(No solutions or imaginary number included)\n");
    } else {    
        printf("(%lf , %lf)\n", solutions[0], solutions[1]);
        free(solutions);
    }
}

int main(){
    // for(int i = 1 ; i < 1000; i++)
    //     assertEqual(my_sqrt(i) * my_sqrt(i), i);

    // measureTime();
    
    // Ex.2
    int * output_arr;
    int input_arr[] = {3,2,1};
    int ans[] = {1,2,3};
    int arr_size = sizeof(input_arr) / sizeof(int);
    time_t t;
    
    output_arr = shellsort(input_arr, arr_size);

    assert(cmpArr(output_arr, ans, arr_size) == 0);

    int input_arrs[3][7] = {{0,5,2,6,8,3,1},{6,2,1,5,7,8,3},{4,4,5,2,1,3,4}};
    int ans_arrs[3][7] = {{0,1,2,3,5,6,8},{1,2,3,5,6,7,8},{1,2,3,4,4,4,5}};
    
 
    // Ex.2. part1
    for(int i = 0 ; i < 3 ; i++){
        arr_size = sizeof(input_arrs[i]) / sizeof(int);
        output_arr = shellsort(input_arrs[i], arr_size);
        assert(cmpArr(output_arr, ans_arrs[i], arr_size) == 0);
        assert(is_permutation(output_arr, input_arrs[i], arr_size, arr_size) != 0);
    }

    int * rand_arr;
    // Ex.2. part2
    for(int i = 0 ; i < 1000; i++){
        srand((unsigned) time(&t));
        arr_size = rand() % MAX_ARRAY_SIZE;
        rand_arr = createRandArray(arr_size);
        output_arr = shellsort(rand_arr, arr_size);
        assert(is_sorted(rand_arr, arr_size) != 0);
        assert(is_permutation(output_arr, rand_arr, arr_size, arr_size) != 0);
        printf("%d - th iteration passed \n", i);
        free(rand_arr);
    }

    // Ex.3 
    doQuad(3,4,1);
    doQuad(0,1,2);

    float x = __FLT_MAX__;
    x = x * 1000;
    printf("float : %f\n", x);
    //my_sqrt_fixed(x);
    return 0;
}

