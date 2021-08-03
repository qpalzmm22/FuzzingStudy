// Tester for ex0 ~ ex4

#include "ex0~1.h"
#include "ex2.h"
#include "ex3~4.h"

int main()
{
    // ex0~1
    for(int i = 1 ; i < 1000; i++)
        assert_equal(my_sqrt(i) * my_sqrt(i), i);
    
    srand(0);
    measure_time_for_random_nums();

    // ex2
    int * output_arr;
    int arr_size;
    int * rand_arr;

    srand(0);

    // Manual Test Cases
    int input_arrs[3][7] = {{0,5,2,6,8,3,1},{6,2,1,5,7,8,3},{4,4,5,2,1,3,4}};
    int ans_arrs[3][7] = {{0,1,2,3,5,6,8},{1,2,3,5,6,7,8},{1,2,3,4,4,4,5}};
    
    // Ex.2. part1
    for(int i = 0 ; i < 3 ; i++){
        arr_size = sizeof(input_arrs[i]) / sizeof(int);
        output_arr = shellsort(input_arrs[i], arr_size);
        assert(cmp_arr(output_arr, ans_arrs[i], arr_size) == 0);
        assert(is_permutation(output_arr, arr_size, input_arrs[i], arr_size) != 0);
    }

    // Ex.2. part2
    for(int i = 0 ; i < 1000; i++){
        arr_size = rand() % MAX_RAND_ARR_SIZE;  // 0 ~ 19
        rand_arr = random_arr_creator(arr_size);
        output_arr = shellsort(rand_arr, arr_size);
        //print_arr(rand_arr, arr_size);

        assert(is_sorted(rand_arr, arr_size) != 0);
        assert(is_permutation(output_arr, arr_size, rand_arr, arr_size) != 0);

        printf("%d - th iteration passed \n", i+1);
        free(rand_arr);
    }


    //ex 3
    do_quad(3,4,1);
    do_quad(0,1,2);
    for (int i = 0 ; i < 1000; i++){
        int a = rand(); // 0 ~ 32767 
        int b = rand();
        int c = rand();

        do_quad(a, b, c);
    }
    calc_part3();
    calc_part4();

    return 0;
}
